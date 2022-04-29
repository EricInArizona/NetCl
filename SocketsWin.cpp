// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "SocketsWin.h"

// If you need to include the Windows.h file then
// you should define
//     #define WIN32_LEAN_AND_MEAN
// So that it doesn't include winsock.h.
// #include <windows.h>


// For Windows.
// #include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


// For Linux.
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>



SocketsWin::SocketsWin( void )
{
// See BuildProj.bat for how to link to the
// wsock32.lib file.
// -lwsock32.lib

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

// delete[] cArray;
}



void SocketsWin::closeSocket( Uint64 toClose )
{
// For Windows.
// returns zero on no error.

closesocket( toClose );
// Linux uses close();
}



Uint64 SocketsWin::openClient( void )
{
struct addrinfo* result = nullptr;
struct addrinfo* ptr = nullptr;
struct addrinfo hints;

// memset( &hints, 0, sizeof( hints ));
ZeroMemory( &hints, sizeof(hints) );

// It's unspecified so it's either IPV4 or IPV6
// or something else.

hints.ai_family = AF_UNSPEC;

hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;


Int32 status = getaddrinfo(
             "www.durangoherald.com", // get news
             "https",
             &hints,
             &result );

if( status != 0 )
  throw "getaddrinfo didn't work.";

SOCKET ConnectSocket = INVALID_SOCKET;

ptr = result;
ConnectSocket = socket( ptr->ai_family,
                        ptr->ai_socktype,
                        ptr->ai_protocol );

return 0;
}



/*
void SocketsNet::getAddressInfo( void )
{
// node is the domain or IP address string.
// "www.thisdomain.com" or "123.456.789.123".
// service is like "https" or a port number
// string like "443".
// results is a linked list.

// getaddrinfo( const char* node,
//              const char* service,
//              const struct addrinfo* hints,
//              const struct addrinfo** results );

// If it's a server use nullptr for the domain.

struct addrinfo hints;
struct addrinfo* results = nullptr;

memset( &hints, 0, sizeof( hints ));

// It's unspecified so it's either IPV4 or IPV6
// or something else.
hints.ai_family = AF_UNSPEC;
// In Linux is results a pointer to a pointer?
// Not a pointer?
Int32 status = getaddrinfo(
             "www.durangoherald.com", // get news
             "https",
             &hints,
             &results );

if( status != 0 )
  throw "getaddrinfo error.";

}
*/






/*
==========================
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

*/
