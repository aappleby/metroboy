#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct VramBus {

  void tick(TestGB& gb);
  bool commit();

  //----------
  // VRAM bus

  Tribuf TRI_A00;
  Tribuf TRI_A01;
  Tribuf TRI_A02;
  Tribuf TRI_A03;
  Tribuf TRI_A04;
  Tribuf TRI_A05;
  Tribuf TRI_A06;
  Tribuf TRI_A07;
  Tribuf TRI_A08;
  Tribuf TRI_A09;
  Tribuf TRI_A10;
  Tribuf TRI_A11;
  Tribuf TRI_A12;

  Tribuf TRI_D0;
  Tribuf TRI_D1;
  Tribuf TRI_D2;
  Tribuf TRI_D3;
  Tribuf TRI_D4;
  Tribuf TRI_D5;
  Tribuf TRI_D6;
  Tribuf TRI_D7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics