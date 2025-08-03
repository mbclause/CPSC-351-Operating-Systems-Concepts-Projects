Project: Project 2
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu, mbclausen@csu.fullerton.edu
Date: 10/08/2021

allocator.h:
Header file containing class definition.


node.h:
This header file declares Node class  which constructs node objects as members of free list or header list.


allocator.cpp:
This program dynamically allocates a small array of std:byte objects. It treats the block of bytes as if it were a heap.
The malloc() function allocates size bytes and returns a pointer to the allocated memory in the array, using the first fit strategy.
The free() function frees the memory space pointed to by ptr, which was returned by a previous call to malloc(). The function manages free-space by using split() and coalesce().


testalloc.cpp:
Client program that uses the allocator.

Makefile:
To run the make file, enter make
To run the example program, enter make test or ./testalloc