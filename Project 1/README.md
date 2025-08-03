Project: Project 1
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu, mbclausen@csu.fullerton.edu
Date: 09/17/2021


thrice.cpp:

Usage: ./thrice PROG [ARGS]...

This program takes arguments from the command-line and forms a command to be executed in a child process. It repeats the command three times in sequence, waiting for the previous instance to complete before running the next instance.



stderr.cpp:

Usage: ./stderr FILE PROG [ARGS]...

This program writes a command's standard error stream to a file.



foreach.cpp:

Usage: ./foreach [-p] [-n NUM] [ARGS]... PROG

This program supplies its arguments one at a time to a
command to be executed repeatedly. There are two optional arguments: 

-p - run each instance of the command in parallel

-n NUM - run the command the specified number of times





