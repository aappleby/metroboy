#include "Sch_Serial.h"

#include "Sch_Top.h"

using namespace Schematics;

void SerialRegisters::dump(Dumper& d) const {
  d("----------  Serial  ----------\n");
  d("XFER_START  %c\n", _XFER_START.c());
  d("XFER_DIR    %c\n", _XFER_DIR  .c());
  d("SER_CLK     %c\n", _SER_CLK   .c());
  d("SER_CNT     %d\n", pack(0, _SER_CNT2.q(), _SER_CNT1.q(), _SER_CNT0.q()));
  d("SER_DATA    0x%02x\n", get_data());
  d("SER_OUT     %c\n", _SER_OUT.c());
  d("SCK_A       %c\n", SCK_A.c());
  d("SCK_B       %c\n", SCK_B.c());
  d("SCK_C       %c\n", SCK_C.c());
  d("SCK_D       %c\n", SCK_D.c());
  d("SIN_C       %c\n", SIN_C.c());
  d("SOUT        %c\n", SOUT .c());
  d("_CALY_SER_INTp %c\n", _CALY_SER_INTp.c());
  d("\n");
}


//------------------------------------------------------------------------------

void SerialRegisters::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void SerialRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  //----------------------------------------
  /*p06.SANO*/ wire _ADDR_FF00_FF03 = and3(cpu_bus.SARE_XX00_XX07p(), cpu_bus.SEFY_A02n(), cpu_bus.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire _FF01_WRn_xxxxxFGH = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _ADDR_FF00_FF03, cpu_bus.CPU_BUS_A00.q(), cpu_bus.TOLA_A01n());
  /*p06.UWAM*/ wire _FF02_WRn_xxxxxFGH = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.CPU_BUS_A01.q());
  /*p06.UFEG*/ wire _FF01_RD = and4(top.TEDO_CPU_RDp(), _ADDR_FF00_FF03, cpu_bus.CPU_BUS_A00.q(), cpu_bus.TOLA_A01n());
  /*p06.UCOM*/ wire _FF02_RD = and4(top.TEDO_CPU_RDp(), _ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.CPU_BUS_A01.q());

  /*p06.COBA*/ wire _SER_CNT3n = not1(_CALY_SER_INTp.q());
  /*p06.CABY*/ wire _XFER_RESET = and2(_SER_CNT3n, top.clk_reg.ALUR_SYS_RSTn());
  /*p06.ETAF*/ _XFER_START = dff17(_FF02_WRn_xxxxxFGH, _XFER_RESET, cpu_bus.CPU_BUS_D0.q());
  /*p06.CULY*/ _XFER_DIR = dff17(_FF02_WRn_xxxxxFGH, top.clk_reg.ALUR_SYS_RSTn(), cpu_bus.CPU_BUS_D1.q());

  /*p06.COTY*/ _SER_CLK = dff17(top.tim_reg.UVYN_DIV_05n(), _FF02_WRn_xxxxxFGH, _SER_CLK.qn());

  // XFER_DIR 0 = ext clk
  // XFER DIR 1 = int clk

  // CAVE01 << CULY17 (xfer dir)
  // CAVE02 << COTY17 (mux 1 = int clk)
  // CAVE03 << SCK_C  (mux 0 = ext clk)
  // CAVE04 nc
  // CAVE05 >> DAWA01

  /*p06.CAVE*/ wire _SER_CLK_MUXn = mux2_n(_SER_CLK.q(), SCK_C.q(), _XFER_DIR.q());

  /*p06.DAWA*/ wire _DAWA_SER_CLK = or2(_SER_CLK_MUXn, !_XFER_START.q()); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /*p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /*p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /*p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /*p06.CARO*/ wire _SER_RST = and2(_FF02_WRn_xxxxxFGH, top.clk_reg.ALUR_SYS_RSTn());
  /*p06.CAFA*/ _SER_CNT0 = dff17(_DAWA_SER_CLK, _SER_RST, !_SER_CNT0.q());
  /*p06.CYLO*/ _SER_CNT1 = dff17(!_SER_CNT0.q(), _SER_RST, !_SER_CNT1.q());
  /*p06.CYDE*/ _SER_CNT2 = dff17(!_SER_CNT1.q(), _SER_RST, !_SER_CNT2.q());
  /*p06.CALY*/ _CALY_SER_INTp = dff17(!_SER_CNT2.q(), _SER_RST, !_CALY_SER_INTp.q());

  /*p06.DAKU*/ wire _FF01_WRp_xxxxxFGH = not1(_FF01_WRn_xxxxxFGH);
  /*p06.CUFU*/ wire _SER_DATA0_SETn = nand2(cpu_bus.CPU_BUS_D0.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DOCU*/ wire _SER_DATA1_SETn = nand2(cpu_bus.CPU_BUS_D1.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DELA*/ wire _SER_DATA2_SETn = nand2(cpu_bus.CPU_BUS_D2.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DYGE*/ wire _SER_DATA3_SETn = nand2(cpu_bus.CPU_BUS_D3.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DOLA*/ wire _SER_DATA4_SETn = nand2(cpu_bus.CPU_BUS_D4.q(), _FF01_WRp_xxxxxFGH);
  /*p06.ELOK*/ wire _SER_DATA5_SETn = nand2(cpu_bus.CPU_BUS_D5.q(), _FF01_WRp_xxxxxFGH);
  /*p06.EDEL*/ wire _SER_DATA6_SETn = nand2(cpu_bus.CPU_BUS_D6.q(), _FF01_WRp_xxxxxFGH);
  /*p06.EFEF*/ wire _SER_DATA7_SETn = nand2(cpu_bus.CPU_BUS_D7.q(), _FF01_WRp_xxxxxFGH);

  // COHY 5-rung
  // DUMO 5-rung
  // DYBO 5-rung
  // DAJU 5-rung
  // DYLY 5-rung
  // EHUJ 5-rung
  // EFAK 5-rung
  // EGUV 5-rung

  /*p06.COHY*/ wire _SER_DATA0_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D0.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.DUMO*/ wire _SER_DATA1_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D1.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.DYBO*/ wire _SER_DATA2_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D2.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.DAJU*/ wire _SER_DATA3_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D3.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.DYLY*/ wire _SER_DATA4_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D4.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.EHUJ*/ wire _SER_DATA5_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D5.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.EFAK*/ wire _SER_DATA6_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D6.q()), top.clk_reg.ALUR_SYS_RSTn());
  /*p06.EGUV*/ wire _SER_DATA7_RSTn = or2(and2(_FF01_WRn_xxxxxFGH, cpu_bus.CPU_BUS_D7.q()), top.clk_reg.ALUR_SYS_RSTn());

  /*p06.CAGE*/ wire _SIN_Cn = not1(SIN_C.q());
  /*p06.CUBA*/ _SER_DATA0 = dff22(_DAWE_SER_CLK, _SER_DATA0_SETn, _SER_DATA0_RSTn, _SIN_Cn);
  /*p06.DEGU*/ _SER_DATA1 = dff22(_DAWE_SER_CLK, _SER_DATA1_SETn, _SER_DATA1_RSTn, _SER_DATA0.q());
  /*p06.DYRA*/ _SER_DATA2 = dff22(_DAWE_SER_CLK, _SER_DATA2_SETn, _SER_DATA2_RSTn, _SER_DATA1.q());
  /*p06.DOJO*/ _SER_DATA3 = dff22(_DAWE_SER_CLK, _SER_DATA3_SETn, _SER_DATA3_RSTn, _SER_DATA2.q());
  /*p06.DOVU*/ _SER_DATA4 = dff22(_EPYT_SER_CLK, _SER_DATA4_SETn, _SER_DATA4_RSTn, _SER_DATA3.q());
  /*p06.EJAB*/ _SER_DATA5 = dff22(_EPYT_SER_CLK, _SER_DATA5_SETn, _SER_DATA5_RSTn, _SER_DATA4.q());
  /*p06.EROD*/ _SER_DATA6 = dff22(_EPYT_SER_CLK, _SER_DATA6_SETn, _SER_DATA6_RSTn, _SER_DATA5.q());
  /*p06.EDER*/ _SER_DATA7 = dff22(_EPYT_SER_CLK, _SER_DATA7_SETn, _SER_DATA7_RSTn, _SER_DATA6.q());
  /*p06.ELYS*/ _SER_OUT   = dff17(_EDYL_SER_CLK, top.clk_reg.ALUR_SYS_RSTn(), _SER_DATA7.q());

  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
  /*p05.KENA*/ SOUT = _SER_OUT.q();
  /*p06.KEXU*/ SCK_A = nand2(_SER_CLK.q(), _XFER_DIR.q());
  SCK_B = _XFER_DIR.q();
  /*p06.KUJO*/ SCK_D = nor2(_SER_CLK.q(), /*p06.JAGO*/ not1(_XFER_DIR.q()));

  /*p06.CUGY*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_FF01_RD, _SER_DATA0.q());
  /*p06.DUDE*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_FF01_RD, _SER_DATA1.q());
  /*p06.DETU*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_FF01_RD, _SER_DATA2.q());
  /*p06.DASO*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_FF01_RD, _SER_DATA3.q());
  /*p06.DAME*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_FF01_RD, _SER_DATA4.q());
  /*p06.EVOK*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_FF01_RD, _SER_DATA5.q());
  /*p06.EFAB*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_FF01_RD, _SER_DATA6.q());
  /*p06.ETAK*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_FF01_RD, _SER_DATA7.q());

  /*p06.CORE*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_FF02_RD, _XFER_DIR.q());
  /*p06.ELUV*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_FF02_RD, _XFER_START.q());
}

//------------------------------------------------------------------------------
