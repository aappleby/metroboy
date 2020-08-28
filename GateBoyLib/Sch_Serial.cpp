#include "GateBoyLib/Sch_Serial.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

void SerialRegisters::dump(Dumper& d) const {
  d("----------  Serial  ----------\n");
  d("XFER_START  %c\n", ETAF_XFER_START.c());
  d("XFER_DIR    %c\n", CULY_XFER_DIR  .c());
  d("SER_CLK     %c\n", COTY_SER_CLK   .c());
  d("SER_CNT     %d\n", pack_p(!CAFA_SER_CNT0.qn(), !CYLO_SER_CNT1.qn(), !CYDE_SER_CNT2.qn(), 0));
  d("SER_DATA    0x%02x\n", get_data());
  d("SER_OUT     %c\n", ELYS_SER_OUT.c());
  d("SCK         %c\n", SCK.c());
  d("SIN         %c\n", SIN.c());
  d("SOUT        %c\n", SOUT.c());
  d("_CALY_SER_INTp %c\n", CALY_INT_SERp.c());
  d("\n");
}


//------------------------------------------------------------------------------

void SerialRegisters::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void SerialRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  //----------------------------------------
  /*p06.SANO*/ wire SANO_ADDR_FF00_FF03 = and3(cpu_bus.SARE_XX00_XX07p(), cpu_bus.SEFY_A02n(), cpu_bus.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire URYS_FF01_WRn_xxxxxFGH = nand4(top.TAPU_CPU_WRp_xxxxEFGx, SANO_ADDR_FF00_FF03, cpu_bus.CPU_BUS_A00.tp(), cpu_bus.TOLA_A01n());
  /*p06.UFEG*/ wire UFEG_FF01_RD = and4(top.TEDO_CPU_RDp, SANO_ADDR_FF00_FF03, cpu_bus.CPU_BUS_A00.tp(), cpu_bus.TOLA_A01n());

  /*p06.COBA*/ wire COBA_SER_CNT3n = not1(CALY_INT_SERp.qp());
  /*p06.CABY*/ wire CABY_XFER_RESET = and2(COBA_SER_CNT3n, top.clk_reg.ALUR_SYS_RSTn);
  
  /*p06.UWAM*/ wire UWAM_FF02_WRn_xxxxxFGH = nand4(top.TAPU_CPU_WRp_xxxxEFGx, SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.CPU_BUS_A01.tp());
  {

    /*p06.ETAF*/ ETAF_XFER_START = dff17_A(UWAM_FF02_WRn_xxxxxFGH, CABY_XFER_RESET, cpu_bus.CPU_BUS_D0p.tp());
    /*p06.CULY*/ CULY_XFER_DIR   = dff17_AB(UWAM_FF02_WRn_xxxxxFGH, top.clk_reg.ALUR_SYS_RSTn, cpu_bus.CPU_BUS_D1p.tp());


    /*p06.UCOM*/ wire UCOM_FF02_RD = and4(top.TEDO_CPU_RDp, SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.CPU_BUS_A01.tp());
    /*p06.CORE*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(UCOM_FF02_RD, CULY_XFER_DIR.qn());
    /*p06.ELUV*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(UCOM_FF02_RD, ETAF_XFER_START.qn());
  }

  /*p01.UVYN*/ wire UVYN_DIV_05n = not1(top.tim_reg.TAMA_DIV_05.qp());
  /*p06.COTY*/ COTY_SER_CLK = dff17_AB(UVYN_DIV_05n, UWAM_FF02_WRn_xxxxxFGH, COTY_SER_CLK.qn());

  /*p06.CAVE*/ wire CAVE_SER_CLK_MUXn = mux2_n(COTY_SER_CLK.qp(), SCK.qn(), CULY_XFER_DIR.qp());

  /*p06.DAWA*/ wire DAWA_SER_CLK = or2(CAVE_SER_CLK_MUXn, ETAF_XFER_START.qn()); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ wire EDYL_SER_CLK = not1(DAWA_SER_CLK);
  /*p06.EPYT*/ wire EPYT_SER_CLK = not1(EDYL_SER_CLK);
  /*p06.DEHO*/ wire DEHO_SER_CLK = not1(EPYT_SER_CLK);
  /*p06.DAWE*/ wire DAWE_SER_CLK = not1(DEHO_SER_CLK);

  /*p06.CARO*/ wire CARO_SER_RST = and2(UWAM_FF02_WRn_xxxxxFGH, top.clk_reg.ALUR_SYS_RSTn);
  /*p06.CAFA*/ CAFA_SER_CNT0 = dff17_A(DAWA_SER_CLK, CARO_SER_RST,   CAFA_SER_CNT0.qn());
  /*p06.CYLO*/ CYLO_SER_CNT1 = dff17_A(CAFA_SER_CNT0.qn(), CARO_SER_RST, CYLO_SER_CNT1.qn());
  /*p06.CYDE*/ CYDE_SER_CNT2 = dff17_A(CYLO_SER_CNT1.qn(), CARO_SER_RST, CYDE_SER_CNT2.qn());
  /*p06.CALY*/ CALY_INT_SERp = dff17_AB(CYDE_SER_CNT2.qn(), CARO_SER_RST, CALY_INT_SERp.qn());

  /*p06.DAKU*/ wire DAKU_FF01_WRp_xxxxxFGH = not1(URYS_FF01_WRn_xxxxxFGH);
  /*p06.CUFU*/ wire CUFU_SER_DATA0_SETn = nand2(cpu_bus.CPU_BUS_D0p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.DOCU*/ wire DOCU_SER_DATA1_SETn = nand2(cpu_bus.CPU_BUS_D1p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.DELA*/ wire DELA_SER_DATA2_SETn = nand2(cpu_bus.CPU_BUS_D2p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.DYGE*/ wire DYGE_SER_DATA3_SETn = nand2(cpu_bus.CPU_BUS_D3p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.DOLA*/ wire DOLA_SER_DATA4_SETn = nand2(cpu_bus.CPU_BUS_D4p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.ELOK*/ wire ELOK_SER_DATA5_SETn = nand2(cpu_bus.CPU_BUS_D5p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.EDEL*/ wire EDEL_SER_DATA6_SETn = nand2(cpu_bus.CPU_BUS_D6p.tp(), DAKU_FF01_WRp_xxxxxFGH);
  /*p06.EFEF*/ wire EFEL_SER_DATA7_SETn = nand2(cpu_bus.CPU_BUS_D7p.tp(), DAKU_FF01_WRp_xxxxxFGH);

  // COHY matches BYHA, and BYHA's C input _must_be and, so this is (A | B) & C
  // and dff22's SET and RST _must_ be SETn/RSTn

  /*p06.COHY*/ wire COHY_SER_DATA0_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D0p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.DUMO*/ wire DUMO_SER_DATA1_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D1p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.DYBO*/ wire DYBO_SER_DATA2_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D2p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.DAJU*/ wire DAJU_SER_DATA3_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D3p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.DYLY*/ wire DYLY_SER_DATA4_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D4p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.EHUJ*/ wire EHUJ_SER_DATA5_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D5p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.EFAK*/ wire EFAK_SER_DATA6_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D6p.tp(), top.clk_reg.ALUR_SYS_RSTn);
  /*p06.EGUV*/ wire EGUV_SER_DATA7_RSTn = or_and3(URYS_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D7p.tp(), top.clk_reg.ALUR_SYS_RSTn);

  /*p06.CAGE*/ wire CAGE_SIN_Cp = not1(SIN.qn());
  /*p06.CUBA*/ CUBA_SER_DATA0 = dff22(DAWE_SER_CLK, CUFU_SER_DATA0_SETn, COHY_SER_DATA0_RSTn, CAGE_SIN_Cp);
  /*p06.DEGU*/ DEGU_SER_DATA1 = dff22(DAWE_SER_CLK, DOCU_SER_DATA1_SETn, DUMO_SER_DATA1_RSTn, CUBA_SER_DATA0.qp());
  /*p06.DYRA*/ DYRA_SER_DATA2 = dff22(DAWE_SER_CLK, DELA_SER_DATA2_SETn, DYBO_SER_DATA2_RSTn, DEGU_SER_DATA1.qp());
  /*p06.DOJO*/ DOJO_SER_DATA3 = dff22(DAWE_SER_CLK, DYGE_SER_DATA3_SETn, DAJU_SER_DATA3_RSTn, DYRA_SER_DATA2.qp());
  /*p06.DOVU*/ DOVU_SER_DATA4 = dff22(EPYT_SER_CLK, DOLA_SER_DATA4_SETn, DYLY_SER_DATA4_RSTn, DOJO_SER_DATA3.qp());
  /*p06.EJAB*/ EJAB_SER_DATA5 = dff22(EPYT_SER_CLK, ELOK_SER_DATA5_SETn, EHUJ_SER_DATA5_RSTn, DOVU_SER_DATA4.qp());
  /*p06.EROD*/ EROD_SER_DATA6 = dff22(EPYT_SER_CLK, EDEL_SER_DATA6_SETn, EFAK_SER_DATA6_RSTn, EJAB_SER_DATA5.qp());
  /*p06.EDER*/ EDER_SER_DATA7 = dff22(EPYT_SER_CLK, EFEL_SER_DATA7_SETn, EGUV_SER_DATA7_RSTn, EROD_SER_DATA6.qp());
  /*p06.ELYS*/ ELYS_SER_OUT   = dff17_B(EDYL_SER_CLK, top.clk_reg.ALUR_SYS_RSTn, EDER_SER_DATA7.qp());

  // FIXME hacking out debug stuff
  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);

  /*p06.KEXU*/ wire KEXU = nand2(COTY_SER_CLK.qp(), CULY_XFER_DIR.qp());
  /*p06.KUJO*/ wire KUJO = nor2(COTY_SER_CLK.qp(), /*p06.JAGO*/ not1(CULY_XFER_DIR.qp()));

  /*p06.KEXU*/ SCK = io_pin(KEXU, KUJO, CULY_XFER_DIR.qp());
  /*p05.KENA*/ SOUT = io_pin(ELYS_SER_OUT.qp(), ELYS_SER_OUT.qp());

  /*#p06.CUGY*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(UFEG_FF01_RD, CUBA_SER_DATA0.qn());
  /* p06.DUDE*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(UFEG_FF01_RD, DEGU_SER_DATA1.qn());
  /* p06.DETU*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(UFEG_FF01_RD, DYRA_SER_DATA2.qn());
  /* p06.DASO*/ cpu_bus.CPU_BUS_D3p = tribuf_6pn(UFEG_FF01_RD, DOJO_SER_DATA3.qn());
  /* p06.DAME*/ cpu_bus.CPU_BUS_D4p = tribuf_6pn(UFEG_FF01_RD, DOVU_SER_DATA4.qn());
  /* p06.EVOK*/ cpu_bus.CPU_BUS_D5p = tribuf_6pn(UFEG_FF01_RD, EJAB_SER_DATA5.qn());
  /* p06.EFAB*/ cpu_bus.CPU_BUS_D6p = tribuf_6pn(UFEG_FF01_RD, EROD_SER_DATA6.qn());
  /*#p06.ETAK*/ cpu_bus.CPU_BUS_D7p = tribuf_6pn(UFEG_FF01_RD, EDER_SER_DATA7.qn());
}

//------------------------------------------------------------------------------
