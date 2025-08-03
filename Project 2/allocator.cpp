/*
File: allocator.cpp
Project: Project 2
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 10/08/2021

Description: Implementation of constructors, destructor, two main methods
malloc(), free(), and additional methods for Allocator class such as
insertNode(), deleteNode(), deleteList()
*/

#include <cassert>
#include <iomanip>
#include <iostream>

#include "allocator.h"

using namespace std;



Allocator::Allocator(size_t size)
{
    heap = new byte[size]();
    assert(heap != NULL);

    this->heap_size = size;

    //set pointers to head of both lists to NULL
    freeLHead = NULL;

    headerLHead = NULL;

    // add the heap to the free list
    insertNode(size, heap, true);
}



Allocator::~Allocator()
{
    delete[] heap;

    // delete the lists
    deleteList(true);

    deleteList(false);
}



byte *Allocator::malloc(size_t size)
{
    assert(size > 0 && size <= heap_size);

    // pointer to the start of the allocated memory
    byte*   bytePtr = NULL;

    // node to traverse the free list, starting at the head
    Node*   nodePtr = freeLHead;

    // traverse the free list
    while(nodePtr != NULL)
    {
      // break if a member is found that can fit the new allocation
      if(nodePtr->getSize() >= size)
          break;

      nodePtr = nodePtr->getNext();
    }

    // if none is found, return NULL
    if(nodePtr == NULL)
        return NULL;

    // get the address of the free list member
    bytePtr = nodePtr->getBytePtr();

    // split the free list member, the second chunk stays in the list
    nodePtr->setBytePtr(bytePtr + size);

    nodePtr->setSize(nodePtr->getSize() - size);

    // add the first chunk to the allocated memory header list
    insertNode(size, bytePtr, false);

    // return the first chunk to the caller
    return bytePtr;
}



void Allocator::free(byte *ptr)
{
    Node*   nodePtr = headerLHead;

    LL_ERROR error = SUCCESS;

    // size of the freed space
    size_t   size = 0;

    // pointer to the new free space
    byte*   bytePtr = NULL;

    // search the memory header list
    while(nodePtr != NULL)
    {
        // find the header of the memory to be freed
        if(nodePtr->getBytePtr() == ptr)
            break;

        nodePtr = nodePtr->getNext();
    }

    // check that the header was found
    assert(nodePtr != NULL);

    // save its size and address
    size = nodePtr->getSize();

    bytePtr = nodePtr->getBytePtr();

    // delete the header
    error = deleteNode(ptr, false);

    // check that delete was successfull
    assert(error == SUCCESS);

    // if the free list is empty, simply add the new allocation to it
    if(freeLHead == NULL)
        insertNode(size, bytePtr, true);

    // if it's not empty
    else
    {
        // insert the new allocation to the front of the free list
        insertNode(size, bytePtr, true);

        // coalesce the free list
        coalesce();

        // if free list still has more than one member, maintain
        // the sorted list order
        if(freeLHead->getNext() != NULL)
            sortList();
    }
}



void Allocator::dump()
{
    cerr << hex << uppercase << setfill('0');
    for (size_t i = 1; i <= heap_size; i++)
    {
        auto offset = i - 1;
        if (offset % 16 == 0)
        {
            cerr << setw(3) << offset << ": ";
        }
        cerr << setw(2) << static_cast<unsigned int>(heap[offset]);
        if (i % 16 == 0)
        {
            cerr << endl;
        }
        else
        {
            cerr << ' ';
        }
    }
}



/* Function: coalesce

   Parameters: none

   Returns: none

   Precondition: The new chunk of free memory has been added to
   the front of the free list by its caller, the frunction Free.

   Postcondition: The free list is coalesced.

   Description: This function iterates through the free list repeatedly
   until the list is coalesced. If it finds
   a block of memory contiguous with the first member, it is merged with
   the first member and thus deleted from the list.
*/

void   Allocator::coalesce()
{
    // start at second member of the free list
    Node*  nodePtr = freeLHead->getNext();

    // add the size of the first node to its byte address
    byte*  headAddrSum = freeLHead->getBytePtr() + freeLHead->getSize();

    // sum of the current node's size and byte address
    byte*  nodeAddrSum = NULL;

    LL_ERROR error = SUCCESS;

    // loop through the free list
    while(nodePtr != NULL)
    {
        // add the size of the node to its byte address
        nodeAddrSum = nodePtr->getBytePtr() + nodePtr->getSize();

        // if nodeAddrSum equals head's byte address
        if(nodeAddrSum == freeLHead->getBytePtr())
        {
            // set head's byte address to node's byte address
            freeLHead->setBytePtr(nodePtr->getBytePtr());

            // add node's size to head's size
            freeLHead->setSize(freeLHead->getSize() + nodePtr->getSize());

            // delete the node
            error = deleteNode(nodePtr->getBytePtr(), true);

            assert(error == SUCCESS);

            // iterate through the list again
            nodePtr = freeLHead->getNext();
        }

        // if headAddrSum equals node's byte address
        else if(headAddrSum == nodePtr->getBytePtr())
        {
            // add node's size to head's size
            freeLHead->setSize(freeLHead->getSize() + nodePtr->getSize());

            // delete the node
            error = deleteNode(nodePtr->getBytePtr(), true);

            assert(error == SUCCESS);

            // iterate through the list again
            nodePtr = freeLHead->getNext();
        }

        else
            nodePtr = nodePtr->getNext();
    }
}



/* Function: insertNode

   Parameters: size - size of the new list member in bytes
               bytePtr - pointer to the byte where the free space starts
               isFreeList - a boolean set to true if the list being altered
                            is the free list, false if it's the header list

   Returns: Returns LL_ERROR SUCCESS on completion.

   Description: Adds a new Node to the list, maintained in sorted
   ascending order by bytePtr address.
*/

LL_ERROR    Allocator::insertNode(size_t  size, byte* bytePtr, bool isFreeList)
{
    Node*   newNode = new Node(size, bytePtr, NULL);

    Node*   nodePtr = freeLHead;

    if(!isFreeList)
        nodePtr = headerLHead;

    if(isFreeList)
        freeLHead = newNode;

    else
        headerLHead = newNode;

    newNode->setNext(nodePtr);

    return SUCCESS;
}



/* Function: deleteNode

   Parameters: bytePtr - pointer to the byte where the deleted member's
                         free space begins
               isFreeList - a boolean set to true if the list being altered
                            is the free list, false if it's the header list

   Returns: Returns LL_ERROR SUCCESS on success, LIST_EMPTY if the list
   is empty, NOT_FOUND if the node is not in the list

   Description: Takes a pointer to the byte where the member's
   free space begins and then deletes
   that member from the list.
*/

LL_ERROR  Allocator::deleteNode(byte* bytePtr, bool isFreeList)
{
    Node*  headPtr = NULL;

    if(isFreeList)
        headPtr = freeLHead;

    else
        headPtr = headerLHead;

    if(headPtr == NULL)
        return LIST_EMPTY;

    Node*  nodePtr = headPtr;

    Node*  trailPtr = headPtr;

    Node*  leadPtr = headPtr->getNext();

    // delete head of list
    if(nodePtr->getBytePtr() == bytePtr)
    {
        delete nodePtr;

        if(isFreeList)
            freeLHead = leadPtr;

        else
            headerLHead = leadPtr;

        return SUCCESS;
    }

    while(leadPtr != NULL)
    {
        nodePtr = nodePtr->getNext();

        leadPtr = leadPtr->getNext();

        if(nodePtr->getBytePtr() == bytePtr)
        {
            trailPtr->setNext(leadPtr);

            delete nodePtr;

            return SUCCESS;
        }

        trailPtr = trailPtr->getNext();
    }

    return NOT_FOUND;
}



/* Function: sortList

   Parameters: none

   Returns: LL_ERROR SUCCESS on completion

   Precondition: The free list has at least two members. The potentially
   unsorted member is at the head of the list.

   Postcondition: The free list is sorted by bytePtr address in ascending
   order.

   Description: This function sorts the free list by inserting the first
   member into its proper place if necessary to maintain sorted ascending
   order by bytPtr address.
*/

LL_ERROR   Allocator::sortList()
{
    Node*  nodePtr = freeLHead->getNext();

    // check that free list has more than one member
    assert(nodePtr != NULL);

    Node*  trailPtr = freeLHead;

    // save the head of the list
    Node*  headPtr = freeLHead;

    // loop through free list
    while(nodePtr != NULL)
    {
        // if current node's byte address is greater than head's, break
        if(freeLHead->getBytePtr() <= nodePtr->getBytePtr())
            break;

        nodePtr = nodePtr->getNext();

        trailPtr = trailPtr->getNext();
    }

    // if list is already sorted, return success
    if(trailPtr == freeLHead)
        return SUCCESS;

    // second member is new head of the list
    headPtr = headPtr->getNext();

    // update the member's "next" pointers
    freeLHead->setNext(nodePtr);

    trailPtr->setNext(freeLHead);

    // update head pointer
    freeLHead = headPtr;

    return SUCCESS;
}



/* Function: deleteList

   Parameters: isFreeList - a boolean set to true if the list being altered
                            is the free list, false if it's the header list

   Returns: Returns LL_ERROR SUCCESS on completion.

   Description: Deletes the free list. Sets freeLHead to NULL.
*/

LL_ERROR   Allocator::deleteList(bool isFreeList)
{
    Node*  headPtr = NULL;

    if(isFreeList)
        headPtr = freeLHead;

    else
        headPtr = headerLHead;

    if(headPtr == NULL)
        return SUCCESS;

    Node*   nodePtr = headPtr;

    Node*   nodeToDelete = headPtr;

    while(nodePtr != NULL)
    {
        nodePtr = nodePtr->getNext();

        delete nodeToDelete;

        nodeToDelete = nodePtr;
    }

    if(isFreeList)
        freeLHead = NULL;

    else
        headerLHead = NULL;

    return SUCCESS;
}
