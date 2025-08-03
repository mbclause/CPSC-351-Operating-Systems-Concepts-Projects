/*
File: multiple_consumer.cpp
Project: Project 4
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 12/17/21

Description: This program implements the producer - consumer relationship.
Besides two producers odd and even, it has four consumer threads, one to
print out Fizz, one to print out Buzz, one to print out FizzBuzz and one to
print out other integers.
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
Cond def;
Cond fizzbuzz;
Cond fizz;
Cond buzz;
Mutex m;


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


/* This function calls do_fill() only with odd numbers and gives signals to
consumer threads after placing a number into the buffer successfully,
otherwise it should wait if the buffer is full. */
void*   OddProducer(void *)
{
    int  num = 0;

    for(int i = 1; i <= max; i += 2)
    {
        m.lock();

        while (num_full == bufSize)
            empty.wait(m);

        do_fill(i);

        num = buffer[use_ptr];

        if(num % 15 == 0)
            fizzbuzz.signal();

        else if(num % 3 == 0)
            fizz.signal();

        else if(num % 5 == 0)
            buzz.signal();

        else if((num % 5 != 0) && (num % 3 != 0))
            def.signal();

        else
        {

        }

        m.unlock();
    }

    return NULL;
}


/* This function calls do_fill() only with even numbers and gives signals to
consumer threads after placing a number into the buffer successfully,
otherwise it should wait if the buffer is full. */
void*   EvenProducer(void *)
{
    int  num = 0;

    for(int i = 2; i <= max; i += 2)
    {
        m.lock();

        while (num_full == bufSize)
            empty.wait(m);

        do_fill(i);

        num = buffer[use_ptr];

        if(num % 15 == 0)
            fizzbuzz.signal();

        else if(num % 3 == 0)
            fizz.signal();

        else if(num % 5 == 0)
            buzz.signal();

        else if((num % 5 != 0) && (num % 3 != 0))
            def.signal();

        else
        {

        }

        m.unlock();
    }

    return NULL;
}


/* Print out "FizzBuzz" if the value got from buffer is divisible by 3 and 5,
then give signal to producers and other consumers. It should wait if the
buffer is empty. */
void*   FizzBuzzConsumer(void  *)
{
    int  count = max / 15;

    int  num = 0;

    for(int i = 1; i <= count; i++)
    {
        m.lock();

        while((num_full == 0) || (buffer[use_ptr] == 0)
        || (buffer[use_ptr] % 15 != 0))
            fizzbuzz.wait(m);

        do_get();

        cout.flush();

        cout << "FizzBuzz ";

        empty.signal();

        num = buffer[use_ptr];

        if((num_full != 0) && (num != 0))
        {
            if(num % 3 == 0)
                fizz.signal();

            else if(num % 5 == 0)
                buzz.signal();

            else if((num % 3 != 0) && (num % 5 != 0))
                def.signal();

            else
            {

            }
        }

        m.unlock();
    }

    return NULL;
}


/* Print out "Fizz" if the value got from buffer is divisible by 3,
then give signal to producers and other consumers. It should wait if the
buffer is empty. */
void*   FizzConsumer(void  *)
{
    int  count = (max / 3) - (max / 15);

    int  num = 0;

    for(int i = 1; i <= count; i++)
    {
        m.lock();

        while((num_full == 0) || (buffer[use_ptr] == 0)
        || (buffer[use_ptr] % 15 == 0) || (buffer[use_ptr] % 3 != 0))
            fizz.wait(m);

        do_get();

        cout.flush();

        cout << "Fizz ";

        empty.signal();

        num = buffer[use_ptr];

        if((num_full != 0) && (num != 0))
        {
            if(num % 15 == 0)
                fizzbuzz.signal();

            else if(num % 5 == 0)
                buzz.signal();

            else if((num % 3 != 0) && (num % 5 != 0))
                def.signal();

            else
            {

            }
        }

        m.unlock();
    }

    return NULL;
}


/* Print out "Buzz" if the value got from buffer is divisible by 5,
then give signal to producers and other consumers. It should wait if the
buffer is empty. */
void*   BuzzConsumer(void  *)
{
    int  count = (max / 5) - (max / 15);

    int  num = 0;

    for(int i = 1; i <= count; i++)
    {
        m.lock();

        while((num_full == 0) || (buffer[use_ptr] == 0)
        || (buffer[use_ptr] % 15 == 0) || (buffer[use_ptr] % 5 != 0))
            buzz.wait(m);

        do_get();

        cout.flush();

        cout << "Buzz ";

        empty.signal();

        num = buffer[use_ptr];

        if((num_full != 0) && (num != 0))
        {
            if(num % 15 == 0)
                fizzbuzz.signal();

            else if(num % 3 == 0)
                fizz.signal();

            else if((num % 3 != 0) && (num % 5 != 0))
                def.signal();

            else
            {

            }
        }

        m.unlock();
    }

    return NULL;
}


/* Print out a number got from buffer if it's not in the above cases,
then give signal to producers and other consumers. It should wait if the
buffer is empty. */
void*   DefaultConsumer(void  *)
{
    int  tmp = 0;

    int num = 0;

    int  count = max - (max / 5) - (max / 3) + (max / 15);

    for(int i = 1; i <= count; i++)
    {
        m.lock();

        while((num_full == 0) || (buffer[use_ptr] == 0)
        || (buffer[use_ptr] % 3 == 0) || (buffer[use_ptr] % 5 == 0))
            def.wait(m);

        tmp = do_get();

        cout.flush();

        cout << tmp << " ";

        empty.signal();

        num = buffer[use_ptr];

        if((num_full != 0) && (num != 0))
        {
            if(num % 15 == 0)
                fizzbuzz.signal();

            else if(num % 3 == 0)
                fizz.signal();

            else if(num % 5 == 0)
                buzz.signal();

            else
            {

            }
        }

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

    Thread  cidDefault;

    Thread  cidFizz;

    Thread  cidBuzz;

    Thread  cidFizzBuzz;

    pidOdd.create(OddProducer);

    pidEven.create(EvenProducer);

    cidDefault.create(DefaultConsumer);

    cidFizz.create(FizzConsumer);

    cidBuzz.create(BuzzConsumer);

    cidFizzBuzz.create(FizzBuzzConsumer);

    pidOdd.join();

    pidEven.join();

    cidDefault.join();

    cidFizz.join();

    cidBuzz.join();

    cidFizzBuzz.join();

    cout.flush();

    cout << endl;

    return 0;
}
