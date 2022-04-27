// Copyright Eric Chauvin 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


/*
To Do:

It can be a straight pass through server.
Like to see the packets in TLS.

If I can specifiy the remote server in XLib then I
can pass it through and read the messages.

Could I put a TLS translater and pass through to
the old map image server?

For sockets.
man poll
 the poll() system call),

The simplest, basic message server.
Sends a message to another client or a list of clients
or all clients.

I could make a local Unix Domain Socket server without
 encryption and then
once I work it out I can make an internet server with
AES encryption.

stdlib.h

 Execute the given line as a shell command.

extern int system (const char *__command) __wur;

What is it execlv() or what?

unistd.h

   Return -1 for errors, 0 to the new process,
   and the process ID of the new process to the old process.
extern __pid_t fork (void) __THROWNL;


*/






#pragma once


// I never use the C preprocessor except to use
// #include statements.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/FileIO.h"



class MainApp
  {
  private:
  FileIO mainIO;

  public:
  inline static const char* getVersionStr( void )
    {
    return "4/27/2022";
    }

  // Int32 mainLoop( Int32 argc, char* argv[] );
  Int32 mainLoop( void );

  };
