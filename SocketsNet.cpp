// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "SocketsNet.h"

// For Windows.
#include <winsock.h>



SocketsNet::SocketsNet( void )
{
// See BuildProj.bat for how to link to the
// wsock32.lib file.
// -lwsock32.lib

// For Windows.
WSADATA wsaData;
// WSAData wsaData;


// This should be compatible with Linux.
// MAKEWORD(1,1) for Winsock 1.1,
// MAKEWORD(2,0) for Winsock 2.0:

if (WSAStartup( MAKEWORD(1,1), &wsaData) != 0 )
  throw "WSAStartup didn't work.";

}



SocketsNet::SocketsNet( const SocketsNet& in )
{
// Make the compiler think the in value is
// being used.
if( in.testForCopy == 123 )
  return;

const char* showS = "The SocketsNet copy"
         " constructor should not get called.";

throw showS;
}



SocketsNet::~SocketsNet( void )
{
// For Windows.
WSACleanup();

// delete[] cArray;
}



void SocketsNet::closeSock( Uint64 toClose )
{
// For Windows.
// returns zero on no error.

closesocket( toClose );
// Linux uses close();
}
