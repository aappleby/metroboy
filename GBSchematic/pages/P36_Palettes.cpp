#include "P36_Palettes.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P36_Palettes_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // BGP
  /*p36.VUSO*/ c.p36.FF47_RD  = and(b.sys.CPU_RD2, b.p22.FF47); // polarity?
  /*p36.VELY*/ c.p36.FF47_WR  = and(b.sys.CPU_WR2, b.p22.FF47);
  /*p36.TEPY*/ c.p36.FF47_RDn = not(b.p36.FF47_RD);
  /*p36.TEPO*/ c.p36.FF47_WRn = not(b.p36.FF47_WR);

  /*p36.MENA*/ c.p36.BGP_D7 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D7, b.D7);
  /*p36.MORU*/ c.p36.BGP_D5 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D5, b.D5);
  /*p36.MAXY*/ c.p36.BGP_D3 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D3, b.D3);
  /*p36.NUSY*/ c.p36.BGP_D1 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D1, b.D1);
  /*p36.MOGY*/ c.p36.BGP_D6 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D6, b.D6);
  /*p36.MUKE*/ c.p36.BGP_D4 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D4, b.D4);
  /*p36.PYLU*/ c.p36.BGP_D2 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D2, b.D2);
  /*p36.PAVO*/ c.p36.BGP_D0 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D0, b.D0);

  /*p36.LARY*/ if (b.p36.FF47_RDn) c.D7 = b.p36.BGP_D7;
  /*p36.LYKA*/ if (b.p36.FF47_RDn) c.D5 = b.p36.BGP_D5;
  /*p36.LOBE*/ if (b.p36.FF47_RDn) c.D3 = b.p36.BGP_D3;
  /*p36.PABA*/ if (b.p36.FF47_RDn) c.D1 = b.p36.BGP_D1;
  /*p36.LODY*/ if (b.p36.FF47_RDn) c.D6 = b.p36.BGP_D6;
  /*p36.LACE*/ if (b.p36.FF47_RDn) c.D4 = b.p36.BGP_D4;
  /*p36.REDO*/ if (b.p36.FF47_RDn) c.D2 = b.p36.BGP_D2;
  /*p36.RARO*/ if (b.p36.FF47_RDn) c.D0 = b.p36.BGP_D0;

  // OBP0
  /*p36.XUFY*/ c.p36.FF48_RD  = and(b.sys.CPU_RD2, b.p22.FF48);
  /*p36.XOMA*/ c.p36.FF48_WR  = and(b.sys.CPU_WR2, b.p22.FF48);
  /*p36.XOZY*/ c.p36.FF48_RDn = not(b.p36.FF48_RD);
  /*p36.XELO*/ c.p36.FF48_WRn = not(b.p36.FF48_WR);

  /*p36.XANA*/ c.p36.OBP0_D7 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D7, b.D7);
  /*p36.XYZE*/ c.p36.OBP0_D5 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D5, b.D5);
  /*p36.XALO*/ c.p36.OBP0_D3 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D3, b.D3);
  /*p36.XUKY*/ c.p36.OBP0_D1 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D1, b.D1);
  /*p36.XUPO*/ c.p36.OBP0_D6 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D6, b.D6);
  /*p36.XERU*/ c.p36.OBP0_D4 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D4, b.D4);
  /*p36.XOVA*/ c.p36.OBP0_D2 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D2, b.D2);
  /*p36.XUFU*/ c.p36.OBP0_D0 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D0, b.D0);

  /*p36.XAWO*/ if (b.p36.FF48_RDn) c.D7 = b.p36.OBP0_D7;
  /*p36.XOBO*/ if (b.p36.FF48_RDn) c.D5 = b.p36.OBP0_D5;
  /*p36.XUBY*/ if (b.p36.FF48_RDn) c.D3 = b.p36.OBP0_D3;
  /*p36.XOKE*/ if (b.p36.FF48_RDn) c.D1 = b.p36.OBP0_D1;
  /*p36.XAXA*/ if (b.p36.FF48_RDn) c.D6 = b.p36.OBP0_D6;
  /*p36.XAJU*/ if (b.p36.FF48_RDn) c.D4 = b.p36.OBP0_D4;
  /*p36.XUNO*/ if (b.p36.FF48_RDn) c.D2 = b.p36.OBP0_D2;
  /*p36.XARY*/ if (b.p36.FF48_RDn) c.D0 = b.p36.OBP0_D0;

  // OBP1
  /*p36.MUMY*/ c.p36.FF49_RD  = and(b.sys.CPU_RD2, b.p22.FF49);
  /*p36.MYXE*/ c.p36.FF49_WR  = and(b.sys.CPU_WR2, b.p22.FF49);
  /*p36.LOTE*/ c.p36.FF49_RDn = not(b.p36.FF49_RD);
  /*p36.LEHO*/ c.p36.FF49_WRn = not(b.p36.FF49_WR);

  /*p36.LUXO*/ c.p36.OBP1_D7 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D7, b.D7);
  /*p36.LUGU*/ c.p36.OBP1_D5 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D5, b.D5);
  /*p36.LOSE*/ c.p36.OBP1_D3 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D3, b.D3);
  /*p36.LAWO*/ c.p36.OBP1_D1 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D1, b.D1);
  /*p36.LEPU*/ c.p36.OBP1_D6 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D6, b.D6);
  /*p36.LUNE*/ c.p36.OBP1_D4 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D4, b.D4);
  /*p36.MOSA*/ c.p36.OBP1_D2 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D2, b.D2);
  /*p36.MOXY*/ c.p36.OBP1_D0 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D0, b.D0);

  /*p36.LELU*/ if (b.p36.FF49_RDn) c.D7 = b.p36.OBP1_D7;
  /*p36.LUGA*/ if (b.p36.FF49_RDn) c.D5 = b.p36.OBP1_D5;
  /*p36.LYZA*/ if (b.p36.FF49_RDn) c.D3 = b.p36.OBP1_D3;
  /*p36.LEPA*/ if (b.p36.FF49_RDn) c.D1 = b.p36.OBP1_D1;
  /*p36.LEBA*/ if (b.p36.FF49_RDn) c.D6 = b.p36.OBP1_D6;
  /*p36.LUKY*/ if (b.p36.FF49_RDn) c.D4 = b.p36.OBP1_D4;
  /*p36.LODE*/ if (b.p36.FF49_RDn) c.D2 = b.p36.OBP1_D2;
  /*p36.LAJU*/ if (b.p36.FF49_RDn) c.D0 = b.p36.OBP1_D0;
}