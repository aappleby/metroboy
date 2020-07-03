#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramBus {

  void tick(SchematicTop& gb);
  SignalHash commit();

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

  Tribuf CPU_TRI_D0;
  Tribuf CPU_TRI_D1;
  Tribuf CPU_TRI_D2;
  Tribuf CPU_TRI_D3;
  Tribuf CPU_TRI_D4;
  Tribuf CPU_TRI_D5;
  Tribuf CPU_TRI_D6;
  Tribuf CPU_TRI_D7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics