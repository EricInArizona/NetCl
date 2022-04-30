// Copyright Eric Chauvin 2021 - 2022.


// See https://ericinarizona.github.io/
// For more information.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// I never use the C preprocessor except to use
// #include statements.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/FileIO.h"
#include "SocketsWin.h"



class MainApp
  {
  private:
  FileIO mainIO;

  // The constructor does WSAStartup() and
  // the destructor does WSACleanup(). So the
  // Windows dlls stay going for the life of
  // this app.  Also statistical and other data
  // is kept for the life of the app.

  SocketsWin socketsWin;

  public:
  inline static const char* getVersionStr( void )
    {
    return "4/30/2022";
    }

  // Int32 mainLoop( Int32 argc, char* argv[] );
  Int32 mainLoop( void );
  void testSockets( void );

  };
