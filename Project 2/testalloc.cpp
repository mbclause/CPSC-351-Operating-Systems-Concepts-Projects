#include <cstdlib>
#include <cstddef>
#include <cassert>

#include <iostream>

#include "allocator.h"

using std::byte;
using std::cout;
using std::endl;
using std::flush;

void fill_block(byte *block, size_t size, char value)
{
    for (unsigned int i = 0; i < size; i++)
    {
        block[i] = (byte)value;
    }
}

void check_fill(byte *block, size_t size, char expected)
{
    for (unsigned int i = 0; i < size; i++)
    {
        char c = (char)block[i];
        assert(c == expected);
    }
}

int main()
{
    Allocator *a = new Allocator();
    assert(a != NULL);

    cout << "Creating Block A: " << flush;
    byte *bA = a->malloc(1 * KB);
    assert(bA != NULL);
    fill_block(bA, 1 * KB, 'A');
    check_fill(bA, 1 * KB, 'A');
    cout << "OK" << endl;

    cout << "Creating Block B: " << flush;
    byte *bB = a->malloc(2 * KB);
    assert(bB != NULL);
    fill_block(bB, 2 * KB, 'B');
    check_fill(bB, 2 * KB, 'B');
    cout << "OK" << endl;

    cout << "Failing to create Block C: " << flush;
    byte *bC = a->malloc(2 * KB);
    assert(bC == NULL);
    cout << "OK" << endl;

    cout << "Checking Block A: " << flush;
    check_fill(bA, 1 * KB, 'A');
    cout << "OK" << endl;

    cout << "Checking and freeing Block B: " << flush;
    check_fill(bB, 2 * KB, 'B');
    a->free(bB);
    cout << "OK" << endl;

    cout << "Creating Block C: " << flush;
    bC = a->malloc(2 * KB);
    assert(bC != NULL);
    fill_block(bC, 2 * KB, 'C');
    check_fill(bC, 2 * KB, 'C');
    cout << "OK" << endl;

    cout << "Checking Block A again: " << flush;
    check_fill(bA, 1 * KB, 'A');
    cout << "OK" << endl;

    a->free(bA);
    a->free(bC);

    a->dump();

    delete a;

    return EXIT_SUCCESS;
}
