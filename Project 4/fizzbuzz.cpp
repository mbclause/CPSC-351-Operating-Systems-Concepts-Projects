/*
File: fizzbuzz.cpp
Project: Project 4
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 12/17/21

Description: This program takes a number as a command-line parameter to count
up from 1: print out "fizz" if the number is divisible by 3, "buzz" if the
number is divisible by 5,"fizzbuzz" if the number is divisible by 3 and 5,
otherwise print out the number itself.
*/

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

int  main(int  argc, char*  argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: fizzbuzz [NUM]\n";

        exit(1);
    }

    int  max = stoi(argv[1]);

    for(int i = 1; i <= max; i++)
    {
        if(i % 15 == 0)
            cout << "Fizz Buzz";

        else if(i % 3 == 0)
            cout << "Fizz";

        else if(i % 5 == 0)
            cout << "Buzz";

        else
            cout << i;

        if(i != max)
            cout << ", ";
    }

    cout << endl;

    return 0;
}
