//======================================================================!
// File: foreach.cpp
// Project: Project 1
// Programmers: Michael Clausen, Anh Nguyen, Linh Nguyen
// email: mbclausen@csu.fullerton.edu,
//        duyanhnguyen2504@csu.fullerton.edu,
//        lnguyen1148@csu.fullerton.edu
// Date: 9/17/21
// Description: This program supplies its arguments one at a time to a
// command to be executed repeatedly. Using -p runs the commands in
// parallel. Using -n NUM runs each command NUM times.
//======================================================================!

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
    // check for command line usage
    if (argc < 3)
    {
        cerr << "Usage: foreach [-p] [-n NUM] [ARGS]... PROG\n";

        exit(EXIT_FAILURE);
    }

    // checks if optional arguments have been set
    bool optArgs = false;

    int  rc_wait;

    int  opt;

    int  numTimes;

    // use getopt for optional args
    while ((opt = getopt(argc, argv, "pn:")) != -1)
    {
        switch(opt)
        {
            // run each instance of the command in parallel
            case 'p':
              optArgs = true;

              // loop as many times as there are arguments
              for (int i = 2; i <= (argc - 2); i++)
              {
                  auto rc = fork();

                  if (rc < 0)
                  {
                      //fork failed; exit
                      perror("fork failed");

                      exit(EXIT_FAILURE);
                  }

                  else if (rc == 0)
                  {
                      // child goes down this path
                      // set program and single argument
                      vector<char *> myargs;

                      myargs.push_back(argv[argc - 1]);

                      myargs.push_back(argv[i]);

                      myargs.push_back(NULL);

                      // run new program
                      execvp(myargs[0], myargs.data());
                   }

                  else
                  {
                      //parent goes down this path (main)
                  }
              } // end for loop

              // wait for all programs to end before ending ./foreach
              for(int i = 2; i <= (argc - 2); i++)
              {
                  rc_wait = wait(NULL);

                  assert(rc_wait >= 0);
              }

              break;

          // run the command numTimes
          case 'n':
              optArgs = true;

              // get user number
              numTimes = atoi(optarg);

              // loop as many times as there are args
              for (int i = 3; i <= (argc - 2); i++)
              {
                  auto rc = fork();

                  if (rc < 0)
                  {
                      //fork failed; exit
                      perror("fork failed");

                      exit(EXIT_FAILURE);
                  }

                  else if (rc == 0)
                  {
                      // child goes down this path
                      // get program and single argument
                      vector<char *> myargs;

                      myargs.push_back(argv[argc - 1]);

                      myargs.push_back(argv[i]);

                      myargs.push_back(NULL);

                     // run command numTimes
                     for (int j = 1; j <= numTimes; j++)
                     {
                         auto rc_child = fork();

                         if (rc_child < 0)
                         {
                             //fork failed; exit
                             perror("fork failed");

                             exit(EXIT_FAILURE);
                         }

                         else if (rc_child == 0)
                         {
                             //child goes down this path, run new program
                             execvp(myargs[0], myargs.data());
                         }

                         else
                         {
                             //parent goes down this path (main), wait for child to finish
                             int rc_wait1 = wait(NULL);

                             assert(rc_wait1 >= 0);
                         }
                     } //end for j loop

                     // kill parent process
                     exit(EXIT_SUCCESS);
                 } // end if (rc == 0)

                else
                {
                    //parent goes down this path (main), wait for child process to finish
                    int rc_wait = wait(NULL);

                    assert(rc_wait >= 0);
                }
              } // end for i loop

              break;

              // default case, check for usage
              default:
                  cerr << "Usage: ./foreach [-p] [-n NUM] [ARGS]... PROG\n";

                  exit(EXIT_FAILURE);

          } //end switch
      } //end while

      // if no optional args provided, run program normally
      if(!optArgs)
      {
          // loop as many times as there are args
          for (int i = 1; i <= (argc - 2); i++)
          {
              auto rc = fork();

              if (rc < 0)
              {
                  //fork failed; exit
                  perror("fork failed");

                  exit(EXIT_FAILURE);
              }

              else if (rc == 0)
              {
                  //child goes down this path, get program and one arg
                  vector<char *> myargs;

                  myargs.push_back(argv[argc - 1]);

                  //program arguments
                  myargs.push_back(argv[i]);

                  myargs.push_back(NULL);

                  // run new program
                  execvp(myargs[0], myargs.data());
              }

              else
              {
                  //parent goes down this path (main), wait for child to finish
                  int rc_wait = wait(NULL);

                  assert(rc_wait >= 0);
              }
          } // end for loop
      } // end if

    return EXIT_SUCCESS;
} // end main
