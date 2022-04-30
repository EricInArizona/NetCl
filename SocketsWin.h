// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


// This is not in the WinAPI directory because
// I don't want to have to link other programs
// that use WinApi to the Windows sockets
// library Ws2_32.lib.

// A good tutorial:
// https://beej.us/guide/bgnet/html/

// DNS over HTTPS
// https://en.wikipedia.org/wiki/DNS_over_HTTPS



#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"
// #include "../CppBase/Str.h"
// #include "../CppBase/RangeC.h"


class SocketsWin
  {
  private:
  Int32 testForCopy = 123;
  // Statistical data is kept here.
  // And error data.
  // And hacking attempt data.

  public:
  SocketsWin( void );
  SocketsWin( const SocketsWin &in );
  ~SocketsWin( void );
  void closeSocket( Uint64 toClose );
  // void getAddressInfo( void );

  Uint64 openClient( const char* domain,
                     const char* port,
                     CharBuf& errorBuf );

  Int32 sendBuf( const Uint64 sendToSock,
                 const CharBuf& sendBuf,
                 CharBuf& errorBuf );

  bool receiveBuf( const Uint64 recSock,
                   CharBuf& recCharBuf,
                   CharBuf& errorBuf );

  };
