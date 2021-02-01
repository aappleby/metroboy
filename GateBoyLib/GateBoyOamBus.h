#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOamBus {
  void reset_to_cart() {
    MAKA_LATCH_EXTp.state  = 0b00011010;
    WUJE_CPU_OAM_WRn.state = 0b00011001;
    SIG_OAM_CLKn.state  = 0b00011001;
    SIG_OAM_WRn_A.state = 0b00011001;
    SIG_OAM_WRn_B.state = 0b00011001;
    SIG_OAM_OEn.state   = 0b00011001;
    old_oam_clk.state   = 0b00011000;

    BUS_OAM_A00n.state  = 0b00011101;
    BUS_OAM_A01n.state  = 0b00011101;
    BUS_OAM_A02n.state  = 0b00011101;
    BUS_OAM_A03n.state  = 0b00011101;
    BUS_OAM_A04n.state  = 0b00011100;
    BUS_OAM_A05n.state  = 0b00011101;
    BUS_OAM_A06n.state  = 0b00011100;
    BUS_OAM_A07n.state  = 0b00011101;
    BUS_OAM_DA00n.state = 0b00011100;
    BUS_OAM_DA01n.state = 0b00011100;
    BUS_OAM_DA02n.state = 0b00011100;
    BUS_OAM_DA03n.state = 0b00011100;
    BUS_OAM_DA04n.state = 0b00011100;
    BUS_OAM_DA05n.state = 0b00011100;
    BUS_OAM_DA06n.state = 0b00011100;
    BUS_OAM_DA07n.state = 0b00011100;
    BUS_OAM_DB00n.state = 0b00011100;
    BUS_OAM_DB01n.state = 0b00011100;
    BUS_OAM_DB02n.state = 0b00011100;
    BUS_OAM_DB03n.state = 0b00011100;
    BUS_OAM_DB04n.state = 0b00011100;
    BUS_OAM_DB05n.state = 0b00011100;
    BUS_OAM_DB06n.state = 0b00011100;
    BUS_OAM_DB07n.state = 0b00011100;
  }

  void reset_for_pass() {
    BUS_OAM_A00n.reset_for_pass();
    BUS_OAM_A01n.reset_for_pass();
    BUS_OAM_A02n.reset_for_pass();
    BUS_OAM_A03n.reset_for_pass();
    BUS_OAM_A04n.reset_for_pass();
    BUS_OAM_A05n.reset_for_pass();
    BUS_OAM_A06n.reset_for_pass();
    BUS_OAM_A07n.reset_for_pass();

    BUS_OAM_DA00n.reset_for_pass();
    BUS_OAM_DA01n.reset_for_pass();
    BUS_OAM_DA02n.reset_for_pass();
    BUS_OAM_DA03n.reset_for_pass();
    BUS_OAM_DA04n.reset_for_pass();
    BUS_OAM_DA05n.reset_for_pass();
    BUS_OAM_DA06n.reset_for_pass();
    BUS_OAM_DA07n.reset_for_pass();

    BUS_OAM_DB00n.reset_for_pass();
    BUS_OAM_DB01n.reset_for_pass();
    BUS_OAM_DB02n.reset_for_pass();
    BUS_OAM_DB03n.reset_for_pass();
    BUS_OAM_DB04n.reset_for_pass();
    BUS_OAM_DB05n.reset_for_pass();
    BUS_OAM_DB06n.reset_for_pass();
    BUS_OAM_DB07n.reset_for_pass();
  }

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  /*SIG_OAM_CLKn */ SigOut SIG_OAM_CLKn;   // ABCDEFGH
  /*SIG_OAM_WRn_A*/ SigOut SIG_OAM_WRn_A;  // AxxxExxH
  /*SIG_OAM_WRn_B*/ SigOut SIG_OAM_WRn_B;  // AxxxExxH
  /*SIG_OAM_OEn  */ SigOut SIG_OAM_OEn;    // ABCDEFGH

  Gate old_oam_clk;

  //----------------------------------------

  /*BUS_OAM_A00n*/ Bus BUS_OAM_A00n;  // ABCDEFGH
  /*BUS_OAM_A01n*/ Bus BUS_OAM_A01n;  // ABCDEFGH
  /*BUS_OAM_A02n*/ Bus BUS_OAM_A02n;  // ABCDEFGH
  /*BUS_OAM_A03n*/ Bus BUS_OAM_A03n;  // ABCDEFGH
  /*BUS_OAM_A04n*/ Bus BUS_OAM_A04n;  // ABCDEFGH
  /*BUS_OAM_A05n*/ Bus BUS_OAM_A05n;  // ABCDEFGH
  /*BUS_OAM_A06n*/ Bus BUS_OAM_A06n;  // ABCDEFGH
  /*BUS_OAM_A07n*/ Bus BUS_OAM_A07n;  // ABCDEFGH

  /*BUS_OAM_DA00n*/ Bus BUS_OAM_DA00n;
  /*BUS_OAM_DA01n*/ Bus BUS_OAM_DA01n;
  /*BUS_OAM_DA02n*/ Bus BUS_OAM_DA02n;
  /*BUS_OAM_DA03n*/ Bus BUS_OAM_DA03n;
  /*BUS_OAM_DA04n*/ Bus BUS_OAM_DA04n;
  /*BUS_OAM_DA05n*/ Bus BUS_OAM_DA05n;
  /*BUS_OAM_DA06n*/ Bus BUS_OAM_DA06n;
  /*BUS_OAM_DA07n*/ Bus BUS_OAM_DA07n;

  /*BUS_OAM_DB00n*/ Bus BUS_OAM_DB00n;
  /*BUS_OAM_DB01n*/ Bus BUS_OAM_DB01n;
  /*BUS_OAM_DB02n*/ Bus BUS_OAM_DB02n;
  /*BUS_OAM_DB03n*/ Bus BUS_OAM_DB03n;
  /*BUS_OAM_DB04n*/ Bus BUS_OAM_DB04n;
  /*BUS_OAM_DB05n*/ Bus BUS_OAM_DB05n;
  /*BUS_OAM_DB06n*/ Bus BUS_OAM_DB06n;
  /*BUS_OAM_DB07n*/ Bus BUS_OAM_DB07n;
};

//------------------------------------------------------------------------------------------------------------------------
