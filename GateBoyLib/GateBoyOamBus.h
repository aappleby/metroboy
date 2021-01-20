#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOamBus {
  void reset_to_cart() {
    oam_latch_a.reset_to_cart();
    oam_latch_b.reset_to_cart();
    oam_temp_a.reset_to_cart();
    oam_temp_b.reset_to_cart();
  }

  void dump(Dumper& d) {
    d.dump_bitp   ("MAKA_LATCH_EXTp  : ", MAKA_LATCH_EXTp.state);
    d.dump_bitp   ("WUJE_CPU_OAM_WRn : ", WUJE_CPU_OAM_WRn.state);
    d.dump_bitp   ("SIG_OAM_CLKn     : ", SIG_OAM_CLKn.get_state());
    d.dump_bitp   ("SIG_OAM_WRn_A    : ", SIG_OAM_WRn_A.get_state());
    d.dump_bitp   ("SIG_OAM_WRn_B    : ", SIG_OAM_WRn_B.get_state());
    d.dump_bitp   ("SIG_OAM_OEn      : ", SIG_OAM_OEn.get_state());

    //d.dump_slice2n("BUS_OAM_An  : ", &BUS_OAM_A00n, 8);
    //d.dump_slice2n("BUS_OAM_DAn : ", &BUS_OAM_DA00n, 8);
    //d.dump_slice2n("BUS_OAM_DBn : ", &BUS_OAM_DB00n, 8);
    d.dump_slice2n("OAM LATCH A : ", &oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
    d.dump_slice2n("OAM LATCH B : ", &oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
    d.dump_slice2p("OAM TEMP A  : ", &oam_temp_a.XUSO_OAM_DA0p, 8);
    d.dump_slice2p("OAM TEMP B  : ", &oam_temp_b.YLOR_OAM_DB0p, 8);

  }

  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  OamTempA oam_temp_a;
  OamTempB oam_temp_b;
  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  /*SIG_OAM_CLKn */ SigOut SIG_OAM_CLKn;   // ABCDEFGH
  /*SIG_OAM_WRn_A*/ SigOut SIG_OAM_WRn_A;  // AxxxExxH
  /*SIG_OAM_WRn_B*/ SigOut SIG_OAM_WRn_B;  // AxxxExxH
  /*SIG_OAM_OEn  */ SigOut SIG_OAM_OEn;    // ABCDEFGH

  Gate old_oam_clk;
};

//------------------------------------------------------------------------------------------------------------------------
