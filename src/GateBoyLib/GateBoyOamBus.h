#pragma once

#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct OamControl {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*_p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  /*_SIG_OAM_CLKn */ SigOut SIG_OAM_CLKn;   // ABCDEFGH
  /*_SIG_OAM_WRn_A*/ SigOut SIG_OAM_WRn_A;  // AxxxExxH
  /*_SIG_OAM_WRn_B*/ SigOut SIG_OAM_WRn_B;  // AxxxExxH
  /*_SIG_OAM_OEn  */ SigOut SIG_OAM_OEn;    // ABCDEFGH

  Gate old_oam_clk;
};

//-----------------------------------------------------------------------------

struct OamABus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_BUS_OAM_A00n*/ Bus BUS_OAM_A00n;  // ABCDEFGH
  /*_BUS_OAM_A01n*/ Bus BUS_OAM_A01n;  // ABCDEFGH
  /*_BUS_OAM_A02n*/ Bus BUS_OAM_A02n;  // ABCDEFGH
  /*_BUS_OAM_A03n*/ Bus BUS_OAM_A03n;  // ABCDEFGH
  /*_BUS_OAM_A04n*/ Bus BUS_OAM_A04n;  // ABCDEFGH
  /*_BUS_OAM_A05n*/ Bus BUS_OAM_A05n;  // ABCDEFGH
  /*_BUS_OAM_A06n*/ Bus BUS_OAM_A06n;  // ABCDEFGH
  /*_BUS_OAM_A07n*/ Bus BUS_OAM_A07n;  // ABCDEFGH
};

//-----------------------------------------------------------------------------

struct OamDBusA {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_BUS_OAM_DA00n*/ Bus BUS_OAM_DA00n;
  /*_BUS_OAM_DA01n*/ Bus BUS_OAM_DA01n;
  /*_BUS_OAM_DA02n*/ Bus BUS_OAM_DA02n;
  /*_BUS_OAM_DA03n*/ Bus BUS_OAM_DA03n;
  /*_BUS_OAM_DA04n*/ Bus BUS_OAM_DA04n;
  /*_BUS_OAM_DA05n*/ Bus BUS_OAM_DA05n;
  /*_BUS_OAM_DA06n*/ Bus BUS_OAM_DA06n;
  /*_BUS_OAM_DA07n*/ Bus BUS_OAM_DA07n;
};

//-----------------------------------------------------------------------------

struct OamDBusB {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_BUS_OAM_DB00n*/ Bus BUS_OAM_DB00n;
  /*_BUS_OAM_DB01n*/ Bus BUS_OAM_DB01n;
  /*_BUS_OAM_DB02n*/ Bus BUS_OAM_DB02n;
  /*_BUS_OAM_DB03n*/ Bus BUS_OAM_DB03n;
  /*_BUS_OAM_DB04n*/ Bus BUS_OAM_DB04n;
  /*_BUS_OAM_DB05n*/ Bus BUS_OAM_DB05n;
  /*_BUS_OAM_DB06n*/ Bus BUS_OAM_DB06n;
  /*_BUS_OAM_DB07n*/ Bus BUS_OAM_DB07n;
};

//-----------------------------------------------------------------------------
