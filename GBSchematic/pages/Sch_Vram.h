#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct PixelPipeSignals;

//-----------------------------------------------------------------------------
// Internal VRAM bus

struct VramBus {
  bool MCS;
  bool MOEn;
  bool MWR;

  bool MA00;
  bool MA01;
  bool MA02;
  bool MA03;
  bool MA04;
  bool MA05;
  bool MA06;
  bool MA07;
  bool MA08;
  bool MA09;
  bool MA10;
  bool MA11;
  bool MA12;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;
};

//-----------------------------------------------------------------------------

};