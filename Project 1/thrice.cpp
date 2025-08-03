//===========================================================================!
// File: thrice.cpp
// Project: Project 1
// Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
// Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu
//	   mbclausen@csu.fullerton.edu
// Date: 9/17/21
// Description: This program takes arguments from the command-line and
// forms a command to be executed in a child process.
// It repeats the command three times in sequence, waiting for the previous
// instance to complete before running the next instance.
//===========================================================================!

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>

#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using std::vector;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: ./thrice PROG [ARGS]...";

        exit(EXIT_FAILURE);
    }

    auto rc = fork();

    if (rc < 0)
    {
        // fork failed; exit
        perror("fork failed");

        exit(EXIT_FAILURE);
    }

    else if (rc == 0)
    {
        vector<char *> myargs;

        myargs.push_back(argv[1]); // user program

        for(int i = 2; i <= argc; i++)
        {
          // program arguments
          myargs.push_back(argv[i]);
        }

        // end of vector
        myargs.push_back(NULL);

        // call user program
        execvp(myargs[0], myargs.data());
    }

    else
    {
        // parent goes down this path (original process)
        auto wc = wait(NULL);

        assert(wc >= 0);

        rc = fork();    // calls fork() again to create second child

        if (rc < 0)
        {
            // fork failed; exit
            perror("fork failed");

            exit(EXIT_FAILURE);
        }

        else if (rc == 0)
        {
            vector<char *> myargs;

            myargs.push_back(argv[1]);     // user program

            for(int i = 2; i <= argc; i++)
            {
              // program arguments
              myargs.push_back(argv[i]);
            }

            myargs.push_back(NULL);             // end of the vector

            execvp(myargs[0], myargs.data());   // executes user program
        }

        else
        {
          // parent goes down this path
          auto wc = wait(NULL);

          assert(wc>= 0);
        }

        vector<char *> myargs;

        myargs.push_back(argv[1]);     // user program

        for(int i = 2; i <= argc; i++)
        {
          // program arguments
          myargs.push_back(argv[i]);
        }

        myargs.push_back(NULL);             // end of the vector

        execvp(myargs[0], myargs.data());   // parent executes user program
    }

    return EXIT_SUCCESS;
}
