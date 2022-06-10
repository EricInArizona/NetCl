// Copyright Eric Chauvin, 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "MainApp.h"

#include "../CppBase/StIO.h"
#include "../CppBase/Str.h"
// #include "../LinuxApi/SetStack.h"
#include "../CppBase/Casting.h"
#include "../CppBase/Threads.h"

#include "../WinApi/Signals.h"
// #include "../LinuxApi/Signals.h"

#include "../Network/NetClient.h"



// int MainApp::mainLoop( int argc, char* argv[] )
int MainApp::mainLoop( void )
{
Int32 delay = 200; // milliseconds.

try
{
// Throws an exception if things are not right.
BasicTypes::thingsAreRight();


StIO::putS( "Programming by Eric Chauvin." );
StIO::printF( "Version date: " );
StIO::putS( getVersionStr() );
StIO::putS( " " );

// For Linux:
// Int32 stackSize = SetStack::getSize();
// Str showS( stackSize );
// mainIO.appendChars( "Stack size: " );
// mainIO.appendStr( showS );
// mainIO.appendChars( "\n\n" );

Signals::setupControlCSignal();
Signals::setupFpeSignal();
Signals::setupIllegalOpSignal();
Signals::setupBadMemSignal();


testSockets();


StIO::putS( "End of main app." );

Threads::sleep( delay );

return 0;
}
catch( const char* in )
  {
  StIO::putS( "Exception in main loop.\n" );
  StIO::putS( in );

  Threads::sleep( delay );
  return 1;
  }

catch( ... )
  {
  const char* in = "An unknown exception"
                   " happened in the main loop.\n";

  StIO::putS( in );

  Threads::sleep( delay );
  return 1;
  }
}


void MainApp::testSockets( void )
{
Str toSend( "Hello. How are you today?\n" );

Str domain( "127.0.0.1" );
Str port( "443" );

NetClient client;

if( !client.connect( domain,
                     port ))
  {
  StIO::putS(
        "client.connect() returned false." );
  return;
  }

StIO::putS( "About to send string." );

if( !client.sendStr( toSend ))
  {
  StIO::putS( "Could not send the whole string." );
  }

StIO::printF( "Sent string." );
// StIO::printFD( howMany );
// StIO::printF( " bytes.\n" );

Threads::sleep( 1000 * 5 );

client.closeSocket();
StIO::putS( "Closed test socket." );
}
