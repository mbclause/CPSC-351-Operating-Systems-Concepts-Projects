Project: Project 3
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu, mbclausen@csu.fullerton.edu
Date: 11/12/2021


box.cpp:
This program takes one of the following commands, followed by the
name of a “box file” and optionally a list of additional filenames:
    create - takes a list of file names, stores them in the box file, and
             deletes the originals
    list - lists the file names stored in the box file
    unpack - takes a list of file names and extracts them from the box file


rebuild.cpp:
The program watches changes to any of the specified files.
If any of the files change, the command will be executed, then the program should go back to watching for file changes.
This continues until the user interrupts the program by pressing Control-C.
