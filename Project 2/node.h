
/*
Project: Project 2
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 10/08/2021

file: Node.h
class: Node
Description: Each object is a member of either the free or header lists,
implemented using a linked list structure. Each stores the size of
the space, a pointer to the byte where it begins,
and a pointer to the next member of the list.
*/

#ifndef NODE_H
#define NODE_H

#include <cstddef>

using namespace std;

class  Node
{
    public:
        // constructors
        Node() {size = 0; bytePtr = NULL; next = NULL;};

        Node(size_t newSize, byte*  newBytePtr, Node*  newNext)
        {size = newSize; bytePtr = newBytePtr; next = newNext;};

        ~Node() {bytePtr = NULL; next = NULL;};

        // getter and setter functions
        size_t   getSize() {return size;};

        void  setSize(size_t  newSize) {size = newSize;};

        byte*   getBytePtr() {return bytePtr;};

        void  setBytePtr(byte*  newBytePtr) {bytePtr = newBytePtr;};

        Node* getNext() {return next;};

        void  setNext(Node*  newNext) {next = newNext;};

    private:
        // size in bytes of the free list member
        size_t   size;

        // pointer to the byte where the free memory begins
        byte  *bytePtr;

        // pointer to next member of list
        Node  *next;
};

#endif
