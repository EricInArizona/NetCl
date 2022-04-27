// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
// #include "../CppBase/Str.h"
// #include "../CppBase/RangeC.h"


class SocketsNet
  {
  private:
  Int32 testForCopy = 123;

  public:
  SocketsNet( void );
  SocketsNet( const SocketsNet &in );
  ~SocketsNet( void );
  void closeSock( Uint64 toClose );


  };
