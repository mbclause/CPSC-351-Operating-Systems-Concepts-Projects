//======================================================================!
// File: stderr.cpp
// Project: Project 1
// Programmers: Michael Clausen, Anh Nguyen, Linh Nguyen
// email: mbclausen@csu.fullerton.edu,
//        duyanhnguyen2504@csu.fullerton.edu,
//        lnguyen1148@csu.fullerton.edu
// Date: 9/17/21
// Description: This program writes a command's standard error stream
//              to a file.
//======================================================================!

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>

#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

int  main(int argc, char *argv[])
{
    if(argc < 3)
    {
        cerr << "usage: ./stderr FILE PROG [ARGS]...\n";

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
        // child: redirect STDERR to a file
        close(STDERR_FILENO);

        open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // now exec user program
        vector<char *> myargs;

        myargs.push_back(argv[2]);     // user program

	for(int i = 3; i <= argc; i++)
	{
	    // program arguments
	    myargs.push_back(argv[i]);
	}

	myargs.push_back(NULL);             // marks end of array

	execvp(myargs[0], myargs.data());   // runs user program
    }

    else
    {
        // parent goes down this path (original process)
        auto wc = wait(NULL);

        assert(wc >= 0);
    }

    return EXIT_SUCCESS;
}
