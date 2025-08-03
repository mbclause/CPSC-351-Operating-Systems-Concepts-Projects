/*
File: box.cpp
Project: Project 3
Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu,
mbclausen@csu.fullerton.edu
Date: 11/12/21

Description: This program takes one of the following commands, followed by the
name of a “box file” and optionally a list of additional filenames:
    create - takes a list of file names, stores them in the box file, and
             deletes the originals
    list - lists the file names stored in the box file
    unpack - takes a list of file names and extracts them from the box file
*/
 
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;



// struct "metadata" definition
typedef struct
{
    char   name[50];

    off_t  offset;

    mode_t  mode;

    size_t  size;
} metadata;

// global constants
const int  MAX_SIZE = 30;

const size_t HEADER_SIZE = sizeof(metadata) * MAX_SIZE;

// function prototypes
void   Create(char*  boxFile, vector<char *> fileNames);

void   List(char*  boxFile);

void   Unpack(char* boxFile, vector<char *> fileNames);



//=====================================================!
// main
//=====================================================!
int   main(int argc, char *argv[])
{
    if(argc < 3)
    {
        cerr << "Usage: ./box COMMAND BOXFILE [FILE]...\n";

        exit(1);
    }

    char  *command = argv[1];

    char  *boxFile = argv[2];

    vector<char *>  fileNames;

    // store the file names in a vector
    for(int  i = 3; i < argc; i++)
    {
        fileNames.push_back(argv[i]);
    }

    if(fileNames.size() > 30)
    {
        cerr << "Too many files to store. Limit is 30.\n";

        exit(1);
    }


    // command - "create"
    if(strcmp(command, "create") == 0)
    {
        if(argc < 4)
        {
            cerr << "Usage: ./box CREATE BOXFILE FILE...\n";

            exit(1);
        }

        // call "Create"
        Create(boxFile, fileNames);
    }

    // command - "list"
    else if(strcmp(command, "list") == 0)
    {
        // call "List"
        List(boxFile);
    }

    // command - "unpack"
    else if(strcmp(command, "unpack") == 0)
    {
        if(argc < 4)
        {
            cerr << "Usage: ./box UNPACK BOXFILE FILE...\n";

            exit(1);
        }

        // call "Unpack"
        Unpack(boxFile, fileNames);
    }

    else
    {
        cerr << "Invalid command\n";

        exit(1);
    }

    return 0;
} // end "main"



/*
    Function: Create
    Parameters: boxFile - the name of the box file to create
                fileNames - a list of names of files to store in the box file
    Returns: none
    Description: This function the box file, stores the listed files inside it,
    and deletes the originals.
*/
void   Create(char*  boxFile, vector<char *> fileNames)
{
    int   offset = HEADER_SIZE;

    metadata   header[MAX_SIZE];

    struct  stat  statbuf;

    int   boxFD = 0;

    int   fd = 0;

    int   writeSize = 0;

    void  *buffer = NULL;

    // initialize header
    for(int i = 0; i < MAX_SIZE; i++)
    {
        strcpy(header[i].name, "");

        header[i].offset = 0;

        header[i].mode = 0;

        header[i].size = 0;
    }

    // create the box file
    boxFD = open(boxFile, O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR);

    assert(boxFD != -1);

    // set the box file offset to the size of the header
    assert(lseek(boxFD, offset, SEEK_SET) == HEADER_SIZE);

    // loop through the file names
    for(unsigned int i = 0; i < fileNames.size(); i++)
    {
        // open the file
        fd = open(fileNames[i], O_RDWR);

        if(fd == -1)
        {
            cerr << "Error opening " << fileNames[i] << endl;

            exit(1);
        }

        // get file metadata
        assert(fstat(fd, &statbuf) != -1);

        // store file metadata in header
        strcpy(header[i].name, fileNames[i]);

        header[i].offset = offset;

        header[i].mode = statbuf.st_mode;

        header[i].size = statbuf.st_size;

        // read the file's contents into buffer and write it to the box file
        buffer = malloc(statbuf.st_size);

        assert(buffer != NULL);

        assert(read(fd, buffer, statbuf.st_size) != -1);

        writeSize = write(boxFD, buffer, statbuf.st_size);

        assert(writeSize != -1);

        // increment offset for next file
        offset += writeSize;

        assert(close(fd) == 0);

        // delete the file
        assert(unlink(fileNames[i]) == 0);

        free(buffer);

        buffer = NULL;
    }

    // set box file offset to 0 and write the header to it
    assert(lseek(boxFD, 0, SEEK_SET) == 0);

    assert(write(boxFD, header, HEADER_SIZE) != -1);

    assert(close(boxFD) == 0);
} // end "Create"



/*
    Function: List
    Parameters: boxFile - the name of the box file to open
    Returns: none
    Description: This function lists the names of the files stored in the
    box file.
*/
void   List(char*  boxFile)
{
    int   fd = 0;

    metadata   *header = new metadata[MAX_SIZE];

    assert(header != NULL);

    // open box file
    fd = open(boxFile, O_RDWR | O_SYNC);

    if(fd == -1)
    {
        cerr << "Error opening box file. Check box file name input.\n";

        exit(1);
    }

    assert(lseek(fd, 0, SEEK_SET) == 0);

    // read header from boxfile
    assert(read(fd, header, HEADER_SIZE) != -1);

    // list file names in header
    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(strcmp(header[i].name, "") == 0)
            break;

        cout << header[i].name << endl;
    }

    assert(close(fd) == 0);

    delete []header;
} // end "List"



/*
    Function: Unpack
    Parameters: boxFile - the name of the box file to open
                fileNames - a list of names of files to extract from the
                            box file
    Returns: none
    Description: This function extracts the listed files from the box file.
    The box file remains unchanged.
*/
void   Unpack(char* boxFile, vector<char *> fileNames)
{
    int  boxFD = 0;

    int  fd = 0;

    metadata   *header = new metadata[MAX_SIZE];

    assert(header != NULL);

    bool   found = false;

    off_t  offset = 0;

    mode_t  mode = 0;

    size_t  size = 0;

    int  index = 0;

    void   *buf = NULL;

    // open box file
    boxFD = open(boxFile, O_RDWR | O_SYNC);

    if(fd == -1)
    {
        cerr << "Error opening box file. Check box file name input.\n";

        exit(1);
    }

    assert(lseek(boxFD, 0, SEEK_SET) == 0);

    // read header from box file
    assert(read(boxFD, header, HEADER_SIZE) != -1);

    assert(lseek(boxFD, HEADER_SIZE, SEEK_SET) == HEADER_SIZE);

    // loop through file names
    for(unsigned int i = 0; i < fileNames.size(); i++)
    {
        // loop through the header to find the file
        for(int j = 0; ; j++)
        {
            // end of header, break
            if(strcmp(header[j].name, "") == 0)
            {
                found = false;

                break;
            }

            // file is found, store index and break
            if(strcmp(header[j].name, fileNames[i]) == 0)
            {
                found = true;

                index = j;

                break;
            }
        }

        if(found == false)
        {
            cerr << "File not found.\n";

            exit(1);
        }

        // file was found
        else
        {
            // get file metadata from header
            size = header[index].size;

            mode = header[index].mode;

            offset = header[index].offset;

            buf = malloc(size);

            // set offset to the file offset and read file into buffer
            assert(lseek(boxFD, offset, SEEK_SET) == offset);

            assert(read(boxFD, buf, size) != -1);

            // create the file and write buffer to it
            fd = open(fileNames[i], O_CREAT | O_RDWR | O_SYNC,
                 S_IRUSR | S_IWUSR);

            assert(fd != -1);

            assert(write(fd, buf, size) != -1);

            // update file mode
            assert(fchmod(fd, mode) == 0);

            assert(close(fd) == 0);

            free(buf);

            buf = NULL;
        }
    }

    assert(close(boxFD) == 0);

    delete []header;
} // end "Unpack"
