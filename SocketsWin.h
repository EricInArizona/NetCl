// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


// This is not in the WinAPI directory because
// I don't want to have to link other programs
// to the Windows sockets library wsock32.lib.


// A good tutorial:
// https://beej.us/guide/bgnet/html/

// DNS over HTTPS
// https://en.wikipedia.org/wiki/DNS_over_HTTPS



// "try to use getaddrinfo() to get all the
// struct sockaddr info, instead of packing the
// structures by hand. This will keep you IP
// version-agnostic, and will eliminate many of
// the subsequent steps."

// === Got to:
// 5 System Calls or Bust



// Port 443 for https.

// htons() host to network short
// htonl() host to network long
// ntohs() network to host short
// ntohl() network to host long

#include "../CppBase/BasicTypes.h"
// #include "../CppBase/Str.h"
// #include "../CppBase/RangeC.h"


class SocketsWin
  {
  private:
  Int32 testForCopy = 123;

  public:
  SocketsWin( void );
  SocketsWin( const SocketsWin &in );
  ~SocketsWin( void );
  void closeSocket( Uint64 toClose );
  // void getAddressInfo( void );
  Uint64 openClient( void );


  };
