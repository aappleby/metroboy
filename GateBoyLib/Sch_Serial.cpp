#include "GateBoyLib/Sch_Serial.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SerialRegisters::dump(Dumper& d) const {
  d("\002===== Serial =====\001\n");
  d("XFER_START  %c\n", ETAF_XFER_START.c());
  d("XFER_DIR    %c\n", CULY_XFER_DIR  .c());
  d("SER_CLK     %c\n", COTY_SER_CLK   .c());
  d("SER_CNT     %d\n", pack_p(!CAFA_SER_CNT0.qn16(), !CYLO_SER_CNT1.qn16(), !CYDE_SER_CNT2.qn16(), 0));
  d("SER_DATA    0x%02x\n", get_data());
  d("SER_OUT     %c\n", ELYS_SER_OUT.c());
  d("SCK         %c\n", SCK.c());
  d("SIN         %c\n", SIN.c());
  d("SOUT        %c\n", SOUT.c());
  d("_CALY_SER_INTp %c\n", CALY_INT_SERp.c());
  d("\n");
}

//------------------------------------------------------------------------------

void SerialRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  //----------------------------------------
  /*p06.SANO*/ wire _SANO_ADDR_FF00_FF03 = and3(cpu_bus.SARE_XX00_XX07p(), cpu_bus.SEFY_A02n(), cpu_bus.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire _URYS_FF01_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());
  /*p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /*p06.UFEG*/ wire _UFEG_FF01_RDp = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());

  /*p06.COBA*/ wire _COBA_SER_CNT3n = not1(CALY_INT_SERp.qp17());
  /*p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, top.clk_reg.ALUR_SYS_RSTn);

  /*p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ETAF*/ ETAF_XFER_START.dff17(_UWAM_FF02_WRn, _CABY_XFER_RESET, cpu_bus.BUS_CPU_D7p.qp());
  /*p06.CULY*/ CULY_XFER_DIR  .dff17(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());

  /*p06.UCOM*/ wire _UCOM_FF02_RD = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ELUV*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UCOM_FF02_RD, ETAF_XFER_START.qn16());
  /*p06.CORE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UCOM_FF02_RD, CULY_XFER_DIR.qn16());

  /*p06.COTY*/ COTY_SER_CLK.dff17(top.tim_reg.UVYN_DIV_05n, _UWAM_FF02_WRn, COTY_SER_CLK.qn16());

  /* p06.CAVE*/ wire _CAVE_SER_CLK_MUXn = mux2n(CULY_XFER_DIR.qp17(), COTY_SER_CLK.qp17(), SCK.qn());

  /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK_MUXn, ETAF_XFER_START.qn16()); // this must stop the clock or something when the transmit's done
  /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn);
  /* p06.CAFA*/ CAFA_SER_CNT0.dff17(_DAWA_SER_CLK, _CARO_SER_RST,   CAFA_SER_CNT0.qn16());
  /* p06.CYLO*/ CYLO_SER_CNT1.dff17(CAFA_SER_CNT0.qn16(), _CARO_SER_RST, CYLO_SER_CNT1.qn16());
  /* p06.CYDE*/ CYDE_SER_CNT2.dff17(CYLO_SER_CNT1.qn16(), _CARO_SER_RST, CYDE_SER_CNT2.qn16());
  /* p06.CALY*/ CALY_INT_SERp.dff17(CYDE_SER_CNT2.qn16(), _CARO_SER_RST, CALY_INT_SERp.qn16());

  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(cpu_bus.BUS_CPU_D0p.qp(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(cpu_bus.BUS_CPU_D1p.qp(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(cpu_bus.BUS_CPU_D2p.qp(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(cpu_bus.BUS_CPU_D3p.qp(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(cpu_bus.BUS_CPU_D4p.qp(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(cpu_bus.BUS_CPU_D5p.qp(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(cpu_bus.BUS_CPU_D6p.qp(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(cpu_bus.BUS_CPU_D7p.qp(), _DAKU_FF01_WRp);

  // COHY matches BYHA, and BYHA's C input _must_ be and, so this is (A | B) & C
  // and dff22's SET and RST _must_ be SETn/RSTn

  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D0p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D1p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D2p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D3p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D4p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D5p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D6p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D7p.qp(), top.clk_reg.ALUR_SYS_RSTn);

  /* p06.CAGE*/ wire _CAGE_SIN_Cp = not1(SIN.qn());
  /* p06.CUBA*/ CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SIN_Cp);
  /* p06.DEGU*/ DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, CUBA_SER_DATA0.qp16());
  /* p06.DYRA*/ DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, DEGU_SER_DATA1.qp16());
  /* p06.DOJO*/ DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, DYRA_SER_DATA2.qp16());
  /* p06.DOVU*/ DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, DOJO_SER_DATA3.qp16());
  /* p06.EJAB*/ EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, DOVU_SER_DATA4.qp16());
  /* p06.EROD*/ EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, EJAB_SER_DATA5.qp16());
  /* p06.EDER*/ EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, EROD_SER_DATA6.qp16());

  /* p06.ELYS*/ ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, top.clk_reg.ALUR_SYS_RSTn, EDER_SER_DATA7.qp16());

  // FIXME hacking out debug stuff
  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);

  /* p06.KEXU*/ wire _KEXU = nand2(COTY_SER_CLK.qp17(), CULY_XFER_DIR.qp17());
  /* p06.KUJO*/ wire _KUJO = nor2(COTY_SER_CLK.qp17(), /*p06.JAGO*/ not1(CULY_XFER_DIR.qp17()));

  /* p06.KEXU*/ SCK.io_pin(_KEXU, _KUJO, CULY_XFER_DIR.qp17());
  /* p05.KENA*/ SOUT.io_pin(ELYS_SER_OUT.qp17(), ELYS_SER_OUT.qp17());

  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UFEG_FF01_RDp, CUBA_SER_DATA0.qn15());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_UFEG_FF01_RDp, DEGU_SER_DATA1.qn15());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_UFEG_FF01_RDp, DYRA_SER_DATA2.qn15());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_UFEG_FF01_RDp, DOJO_SER_DATA3.qn15());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_UFEG_FF01_RDp, DOVU_SER_DATA4.qn15());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_UFEG_FF01_RDp, EJAB_SER_DATA5.qn15());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_UFEG_FF01_RDp, EROD_SER_DATA6.qn15());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UFEG_FF01_RDp, EDER_SER_DATA7.qn15());
}

//------------------------------------------------------------------------------
