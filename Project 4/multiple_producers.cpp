/*
File: multiple_producers.cpp
Project: Project 4
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 12/17/21

Description: This program implements the producer - consumer relationship.
There are two producers, one producing odd values, the other producing even
values, and place them in the bounded buffer. The consumer thread removes the
integer from the buffer and print the corresponding output value.
*/

#include <iostream>
#include <cstdlib>
#include <cassert>

#include "pthread-wrappers.h"

using std::cout;
using std::cerr;
using std::endl;

int max;
int bufSize;
int *buffer;

int use_ptr  = 0;
int fill_ptr = 0;
int num_full = 0;

Cond empty;
Cond fill;
Mutex m;


void   Fizzbuzz(int  num) // This function prints out the values as required
{
    assert(num > 0);

    if(num % 15 == 0)
        cout << "Fizzbuzz ";

    else if (num % 3 == 0)
        cout << "Fizz ";

    else if (num % 5 == 0)
        cout << "Buzz ";

    else
        cout << num << " ";
}


// This function places a number into the buffer and increment the num_full
// variable by 1
void    do_fill(int value)
{
    buffer[fill_ptr] = value;

    fill_ptr = (fill_ptr + 1) % bufSize;

    num_full++;
}


// This function reads a number from the buffer, decrement the num_full
// variable by 1
int    do_get()
{
    int tmp = buffer[use_ptr];

    use_ptr = (use_ptr + 1) % bufSize;

    num_full--;

    return tmp;
}


/* This function calls do_fill() only with odd numbers and gives a signal
after placing a number into the buffer successfully, otherwise it should wait
if the buffer is full. */
void*   OddProducer(void *)
{
    for(int i = 1; i <= max; i += 2)
    {
        m.lock();

        while (num_full == bufSize)
            empty.wait(m);

        do_fill(i);

        fill.signal();

        m.unlock();
    }

    return NULL;
}


/* This function calls do_fill() only with even numbers and gives a signal
after placing a number into the buffer successfully, otherwise it should wait
if the buffer is full. */
void*   EvenProducer(void *)
{
    for(int i = 2; i <= max; i += 2)
    {
        m.lock();

        while (num_full == bufSize)
            empty.wait(m);

        do_fill(i);

        fill.signal();

        m.unlock();
    }

    return NULL;
}


/* Consumer thread calls the do_get() function if the buffer is not empty,
gives a signal after getting a number from the buffer successfully, calls
Fizzbuzz() to print out appropriate values, otherwise it should wait if the
buffer is full. */
void*   consumer(void  *)
{
    int  tmp = 0;

    for(int i = 1; i <= max; i++)
    {
        m.lock();

        while (num_full == 0)
            fill.wait(m);

        tmp = do_get();

        Fizzbuzz(tmp);

        empty.signal();

        m.unlock();
    }

    return NULL;
}



int  main(int  argc, char*  argv[])
{
    if(argc != 3)
    {
        cerr << "Usage: " << argv[0] << " [NUM] [BUFSIZE]\n";

        exit(1);
    }

    max = atoi(argv[1]);

    bufSize = atoi(argv[2]);

    buffer = (int *) malloc(bufSize * sizeof(int));

    assert(buffer != NULL);

    for(int i = 0; i < bufSize; i++)
        buffer[i] = 0;

    Thread  pidOdd;

    Thread  pidEven;

    Thread  cid;

    pidOdd.create(OddProducer);

    pidEven.create(EvenProducer);

    cid.create(consumer);

    pidOdd.join();

    pidEven.join();

    cid.join();

    cout << endl;

    return 0;
}
