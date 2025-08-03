/*
File: allocator.h
Project: Project 2
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 10/08/2021

Description: Declaration of Allocator class. Return values while running
methods like insertNode(), deleteNode(), deleteList() are listed in enum
object LL_ERROR. Beside two variables heap_size and heap provided for
Allocator object, two more pointers freeLHead and headerLHead are used to
navigate to free list or header list, respectively.
*/


#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

#include "node.h"

using namespace std;

enum LL_ERROR {SUCCESS, LIST_EMPTY, NOT_FOUND};

const auto KB = 1024;


class Allocator
{
public:
    static const auto DEFAULT_MEMORY_SIZE = 4 * KB;

    Allocator(size_t size = DEFAULT_MEMORY_SIZE);

    ~Allocator();

    byte *malloc(size_t);

    void free(byte *);

    void dump();

    // this function coalesces the free list after a call to "free."
    void  coalesce();

    // insertion, sort, and delete methods for the lists
    LL_ERROR   insertNode(size_t  size, byte*  bytePtr, bool isFreeList);

    LL_ERROR   deleteNode(byte*  bytePtr, bool isFreeList);

    LL_ERROR   deleteList(bool isFreeList);

    LL_ERROR   sortList();

private:
    size_t heap_size;

    byte *heap;

    // pointer to the head of the free list
    Node*   freeLHead;

    // pointer to the head of the allocated memory header list
    Node*   headerLHead;
};

#endif
