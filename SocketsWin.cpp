// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "SocketsWin.h"
#include "../CppBase/Casting.h"


// I hate to have to put a #define statement
// into any of my code.  But if you need to
// include the Windows.h file then
// you have to define
//     #define WIN32_LEAN_AND_MEAN
// So that it doesn't include winsock.h.
// And of course this would only be done in
// a .cpp file and not a header file
// #include <windows.h>

// #include <stdio.h>


// For Windows.
// #include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h> // getaddrinfo()

// Need to link with Ws2_32.lib,
// Mswsock.lib,
// and Advapi32.lib ?

// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")


// For Linux.
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>



SocketsWin::SocketsWin( void )
{
// See BuildProj.bat for how to link to the
// Windows .lib file.

// For Windows.
WSADATA wsaData;

// MAKEWORD(1,1) for Winsock 1.1,
// MAKEWORD(2,0) for Winsock 2.0:
// MAKEWORD(2,2) for Winsock 2.2:

if( WSAStartup( MAKEWORD(2,2), &wsaData ) != 0 )
  throw "WSAStartup didn't work.";

}



SocketsWin::SocketsWin( const SocketsWin& in )
{
// Make the compiler think the in value is
// being used.
if( in.testForCopy == 123 )
  return;

const char* showS = "The SocketsWin copy"
         " constructor should not get called.";

throw showS;
}



SocketsWin::~SocketsWin( void )
{
// For Windows.
WSACleanup();
}



void SocketsWin::closeSocket( Uint64 toClose )
{
// For Windows.
// returns zero on no error.

//    iResult = shutdown(ConnectSocket, SD_SEND);

closesocket( toClose );
// Linux uses close();
}



Uint64 SocketsWin::openClient( const char* domain,
                               const char* port,
                               CharBuf& errorBuf )
{
// result is a linked list.
// In Linux is it a pointer to a pointer?
// const struct addrinfo** result;

struct addrinfo* result = nullptr;
struct addrinfo* ptr = nullptr;
struct addrinfo hints;

// memset( &hints, 0, sizeof( hints ));
ZeroMemory( &hints, sizeof(hints) );

// It's unspecified so it's either IPV4 or IPV6.

hints.ai_family = AF_UNSPEC;

hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;

// Port 443 for https.

// htons() host to network short
// htonl() host to network long
// ntohs() network to host short
// ntohl() network to host long

// If it's a server use nullptr for the domain?

Int32 status = getaddrinfo(
              domain, // "www.thedomain.com"
              port, // "443" "https", "ftp", etc.
              &hints,
              &result );

if( status != 0 )
  {
  errorBuf.appendChars(
               "SocketWin getaddrinfo error.\n" );
  return 0;
  }

// SOCKET clientSocket = INVALID_SOCKET;
Uint64 clientSocket = INVALID_SOCKET;

// Try the possible connections.
Int32 count = 0;
for( ptr = result; ptr != nullptr;
                              ptr = ptr->ai_next )
  {
  // Make sure nothing is really-bad wrong here.
  count++;
  if( count > 5 )
    {
    errorBuf.appendChars(
     "SocketWin too many sockets for connect.\n" );

    return 0;
    }

  clientSocket = socket( ptr->ai_family,
                          ptr->ai_socktype,
                          ptr->ai_protocol );

  if( clientSocket == INVALID_SOCKET )
    {
    errorBuf.appendChars(
      "SocketWin no sockets left for connect.\n" );

    // WSAGetLastError());
    return 0;
    }

  Int32 connectResult = connect( clientSocket,
                         ptr->ai_addr,
                         Casting::U64ToI32(
                         ptr->ai_addrlen ));

  if( connectResult == SOCKET_ERROR )
    {
    errorBuf.appendChars(
      "SocketWin trying the next socket.\n" );

    closesocket( clientSocket );
    clientSocket = INVALID_SOCKET;
    continue; // Try to connect to the next
              // valid socket.
    }

  // It should have a good connected socket.
  break;
  }

errorBuf.appendChars(
        "SocketWin connected to " );
errorBuf.appendChars( domain );
errorBuf.appendChars( "\n" );

freeaddrinfo( result );

// Make it non blocking.  0 is blocking.
// Non zero is non blocking.
// Uint32L iMode = 1;

// setsockopt()

// for Linux:
// #include <unistd.h>
// #include <fcntl.h>
// sockfd = socket(PF_INET, SOCK_STREAM, 0);
// fcntl(sockfd, F_SETFL, O_NONBLOCK);

// Windows doesn't support this.
// Because it's always non blocking?
// Int32 ioResult = ioctlsocket( clientSocket,
//                    Casting::u32ToI32( FIONBIO ),
//                    &iMode );

//  Int32 error = WSAGetLastError();
// errorBuf.appendChars(
//               "socket ioctl failed.\n" );
//   errorBuf.appendChars( "Error is: " );
//  Str errorS( error );
//  errorBuf.appendStr( errorS );
//  errorBuf.appendChars( "\n" );

return clientSocket;
}



Int32 SocketsWin::sendBuf(
                   const Uint64 sendToSock,
                   const CharBuf& sendBuf,
                   CharBuf& errorBuf )
{
if( sendToSock == 0 )
  {
  errorBuf.appendChars(
   "SocketsWin sendBuf() sendToSock is zero.\n" );

  return -1;
  }

const Int32 howMany = sendBuf.getLast();
Int32 result = send( sendToSock,
                     sendBuf.getBufPoint(),
                     howMany,
                     0 );

if( result == SOCKET_ERROR )
  {
  errorBuf.appendChars(
             "SocketsWin sendBuf() error.\n" );
  // WSAGetLastError());
  closesocket( sendToSock );
  return -1;
  }

// How many did it actually send?
return result;
}



bool SocketsWin::receiveBuf(
                   const Uint64 recSock,
                   CharBuf& recCharBuf,
                   CharBuf& errorBuf )
{
// poll() or
// select()

if( recSock == 0 )
  {
  errorBuf.appendChars(
   "SocketsWin receiveBuf() recSock is zero.\n" );

  return false;
  }

const Int32 bufLen = 1024 * 32;
// On the stack.

char recBuf[bufLen];

// Keep reading for a reasonable length of time.
for( Int32 loops = 0; loops < 2; loops++ )
  {
  Int32 result = recv( recSock, recBuf,
                       bufLen, 0 );

  if( result == 0 )
    {
    // The connection was _gracefully_ closed.
    errorBuf.appendChars(
            "receiveBuf() connection closed.\n" );

    return false;
    }

  if( result < 0 )
    {
    // EAGAIN or EWOULDBLOCK

    Int32 error = WSAGetLastError();
    errorBuf.appendChars( "socket recv error is: " );
    Str errorS( error );
    errorBuf.appendStr( errorS );
    errorBuf.appendChars( "\n" );

    // This might just be saying it would block.
    return false;
    }

  for( Int32 count = 0; count < result; count++ )
    recCharBuf.appendChar( recBuf[count] );

  }

return true;
}
