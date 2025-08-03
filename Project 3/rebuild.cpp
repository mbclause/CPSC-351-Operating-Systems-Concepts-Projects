//======================================================================!
// File: rebuild.cpp
// Project: Project 3
// Programmers: Anh Nguyen, Linh Nguyen, Michael Clausen
// Email: duyanhnguyen2504@csu.fullerton.edu, lnguyen1148@csu.fullerton.edu
//        mbclausen@csu.fullerton.edu
// Date: 11/12/21
// Description: The program watches changes to any of the specified files If any of the files change,
// the command should be executed, then the program should go back to watching for file changes.
// This continues until the user interrupts the program by pressing Control-C.
//======================================================================!
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <cassert>

using namespace std;



int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: ./rebuild COMMAND [FILE]\n";

        exit(1);
    }

    vector<char *>  fileNames;

    vector<int>   WDs;

    int    iFD = 0;

    ssize_t   len = 0;

    char   buf[4096]    // assume the maximum size of buffer is 4096 bytes
    __attribute__ ((aligned(__alignof__(struct inotify_event))));

    const struct inotify_event *event;

    for(int i = 2; i < argc; i++)
    {
        fileNames.push_back(argv[i]);
    }

    unsigned int  numFiles = fileNames.size();

    iFD = inotify_init();   // create an instance of inotify 

    if (iFD == -1)
    {
        cerr << "inotify_init error\n";

        exit(1);
    }

    for(unsigned int i = 0; i < numFiles; i++)  // add new watches to watch list
    {
        WDs.push_back(inotify_add_watch(iFD, fileNames[i],
            IN_CLOSE_WRITE));

        if(WDs[i] == -1)
        {
            cerr << "Error adding watch for " << fileNames[i] << ". Error: "
            << strerror(errno) << endl;

            exit(1);
        }
    }

    // repeat the step of watching for changes, executing the command 
    // until user hits ctrl + C
    while(true) 
    {
        cout << "Watching for changes to ";

        for(unsigned int i = 0; i < numFiles; i++)
        {
            cout << fileNames[i] << ", ";
        }

        cout << "...\n";

        // read inotify file descriptor into buffer
        len = read(iFD, buf, sizeof(buf));  

        if(len == -1 && errno != EAGAIN)
        {
            cerr << "Error reading from inotify fd: " <<
            strerror(errno) << endl;

            exit(1);
        }

        for (char *ptr = buf; ptr < buf + len;  // extract events from buffer
             ptr += sizeof(struct inotify_event) + event->len)
        {
            event = (const struct inotify_event *) ptr;

            for(unsigned int i = 0; i < numFiles; i++)
            {
                if(WDs[i] == event->wd) // identify the file changed
                    cout << fileNames[i] << " has changed...\n";

                if(event->mask & IN_IGNORED)
                {
                    if(inotify_add_watch(iFD, fileNames[i],
                        IN_CLOSE_WRITE) == -1)
                        {
                            cerr << "Error adding watch for " <<
                            fileNames[i] << ". Error: "
                            << strerror(errno) << endl;

                            exit(1);
                        }
                }
            }
        }

        auto rc = fork();   // create a new process to execute the command

        if (rc < 0)
        {
            cerr << "fork failed\n";

            exit(EXIT_FAILURE);
        }

        else if (rc == 0)
        {
            vector<char *> myargs;

            myargs.push_back(argv[1]);

            myargs.push_back(NULL);

            execvp(myargs[0], myargs.data());   // execute the command
        }

        else
        {
            // parent goes down this path (original process)
            auto wc = wait(NULL);

            assert(wc >= 0);
        }
    }

    assert(close(iFD) == 0);

    return 0;
}
