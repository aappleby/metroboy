#include "Sch_Serial.h"

#include "Sch_Top.h"

using namespace Schematics;

SerialRegisters::SerialRegisters() {
  SIN_C.set(0);
  SCK_C.set(0);
}

//------------------------------------------------------------------------------

void SerialRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void SerialRegisters::tock(SchematicTop& top) {

  //----------------------------------------
  /*p06.SANO*/ wire _ADDR_FF00_FF03 = and (top.SARE_XX00_XX07p(), top.SEFY_A02n(), top.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire _FF01_WRn_xxxxxFGH = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _ADDR_FF00_FF03, top.CPU_PIN_A00, top.TOLA_A01n());
  /*p06.UWAM*/ wire _FF02_WRn_xxxxxFGH = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _ADDR_FF00_FF03, top.TOVY_A00n(), top.CPU_PIN_A01);
  /*p06.UFEG*/ wire _FF01_RD = and (top.TEDO_CPU_RDp(), _ADDR_FF00_FF03, top.CPU_PIN_A00, top.TOLA_A01n());
  /*p06.UCOM*/ wire _FF02_RD = and (top.TEDO_CPU_RDp(), _ADDR_FF00_FF03, top.TOVY_A00n(), top.CPU_PIN_A01);

  /*p06.COBA*/ wire _SER_CNT3n = not(_CALY_INT_SERIALp.q());
  /*p06.CABY*/ wire _XFER_RESET = and (_SER_CNT3n, top.rst_reg.ALUR_SYS_RSTn());
  /*p06.ETAF*/ _XFER_START.set(_FF02_WRn_xxxxxFGH, _XFER_RESET, top.CPU_TRI_D0);
  /*p06.CULY*/ _XFER_DIR.set(_FF02_WRn_xxxxxFGH, top.rst_reg.ALUR_SYS_RSTn(), top.CPU_TRI_D1);

  /*p06.COTY*/ _SER_CLK.set(top.tim_reg.UVYN_DIV_05n(), _FF02_WRn_xxxxxFGH, _SER_CLK.qn());

  // XFER_DIR 0 = ext clk
  // XFER DIR 1 = int clk

  // CAVE01 << CULY17 (xfer dir)
  // CAVE02 << COTY17 (mux 1 = int clk)
  // CAVE03 << SCK_C  (mux 0 = ext clk)
  // CAVE04 nc
  // CAVE05 >> DAWA01

  /*p06.CAVE*/ wire _SER_CLK_MUXn = mux2_n(_SER_CLK.q(), SCK_C, _XFER_DIR.q());

  /*p06.DAWA*/ wire _DAWA_SER_CLK = or(_SER_CLK_MUXn, !_XFER_START.q()); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ wire _EDYL_SER_CLK = not(_DAWA_SER_CLK);
  /*p06.EPYT*/ wire _EPYT_SER_CLK = not(_EDYL_SER_CLK);
  /*p06.DEHO*/ wire _DEHO_SER_CLK = not(_EPYT_SER_CLK);
  /*p06.DAWE*/ wire _DAWE_SER_CLK = not(_DEHO_SER_CLK);

  /*p06.CARO*/ wire _SER_RST = and (_FF02_WRn_xxxxxFGH, top.rst_reg.ALUR_SYS_RSTn());
  /*p06.CAFA*/ _SER_CNT0.set(_DAWA_SER_CLK, _SER_RST, !_SER_CNT0.q());
  /*p06.CYLO*/ _SER_CNT1.set(!_SER_CNT0.q(), _SER_RST, !_SER_CNT1.q());
  /*p06.CYDE*/ _SER_CNT2.set(!_SER_CNT1.q(), _SER_RST, !_SER_CNT2.q());
  /*p06.CALY*/ _CALY_INT_SERIALp.set(!_SER_CNT2.q(), _SER_RST, !_CALY_INT_SERIALp.q());

  /*p06.DAKU*/ wire _FF01_WRp_xxxxxFGH = not (_FF01_WRn_xxxxxFGH);
  /*p06.CUFU*/ wire _SER_DATA0_SETn = nand(top.CPU_TRI_D0.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DOCU*/ wire _SER_DATA1_SETn = nand(top.CPU_TRI_D1.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DELA*/ wire _SER_DATA2_SETn = nand(top.CPU_TRI_D2.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DYGE*/ wire _SER_DATA3_SETn = nand(top.CPU_TRI_D3.q(), _FF01_WRp_xxxxxFGH);
  /*p06.DOLA*/ wire _SER_DATA4_SETn = nand(top.CPU_TRI_D4.q(), _FF01_WRp_xxxxxFGH);
  /*p06.ELOK*/ wire _SER_DATA5_SETn = nand(top.CPU_TRI_D5.q(), _FF01_WRp_xxxxxFGH);
  /*p06.EDEL*/ wire _SER_DATA6_SETn = nand(top.CPU_TRI_D6.q(), _FF01_WRp_xxxxxFGH);
  /*p06.EFEF*/ wire _SER_DATA7_SETn = nand(top.CPU_TRI_D7.q(), _FF01_WRp_xxxxxFGH);

  // COHY 5-rung
  // DUMO 5-rung
  // DYBO 5-rung
  // DAJU 5-rung
  // DYLY 5-rung
  // EHUJ 5-rung
  // EFAK 5-rung
  // EGUV 5-rung

  /*p06.COHY*/ wire _SER_DATA0_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D0.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.DUMO*/ wire _SER_DATA1_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D1.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.DYBO*/ wire _SER_DATA2_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D2.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.DAJU*/ wire _SER_DATA3_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D3.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.DYLY*/ wire _SER_DATA4_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D4.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.EHUJ*/ wire _SER_DATA5_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D5.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.EFAK*/ wire _SER_DATA6_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D6.q()), top.rst_reg.ALUR_SYS_RSTn());
  /*p06.EGUV*/ wire _SER_DATA7_RSTn = or(and(_FF01_WRn_xxxxxFGH, top.CPU_TRI_D7.q()), top.rst_reg.ALUR_SYS_RSTn());

  /*p06.CAGE*/ wire _SIN_Cn = not(SIN_C);
  /*p06.CUBA*/ _SER_DATA0.set(_DAWE_SER_CLK, _SER_DATA0_SETn, _SER_DATA0_RSTn, _SIN_Cn);
  /*p06.DEGU*/ _SER_DATA1.set(_DAWE_SER_CLK, _SER_DATA1_SETn, _SER_DATA1_RSTn, _SER_DATA0.q());
  /*p06.DYRA*/ _SER_DATA2.set(_DAWE_SER_CLK, _SER_DATA2_SETn, _SER_DATA2_RSTn, _SER_DATA1.q());
  /*p06.DOJO*/ _SER_DATA3.set(_DAWE_SER_CLK, _SER_DATA3_SETn, _SER_DATA3_RSTn, _SER_DATA2.q());
  /*p06.DOVU*/ _SER_DATA4.set(_EPYT_SER_CLK, _SER_DATA4_SETn, _SER_DATA4_RSTn, _SER_DATA3.q());
  /*p06.EJAB*/ _SER_DATA5.set(_EPYT_SER_CLK, _SER_DATA5_SETn, _SER_DATA5_RSTn, _SER_DATA4.q());
  /*p06.EROD*/ _SER_DATA6.set(_EPYT_SER_CLK, _SER_DATA6_SETn, _SER_DATA6_RSTn, _SER_DATA5.q());
  /*p06.EDER*/ _SER_DATA7.set(_EPYT_SER_CLK, _SER_DATA7_SETn, _SER_DATA7_RSTn, _SER_DATA6.q());
  /*p06.ELYS*/ _SER_OUT.set(_EDYL_SER_CLK, top.rst_reg.ALUR_SYS_RSTn(), _SER_DATA7.q());

  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
  /*p05.KENA*/ SOUT.set(_SER_OUT.q());
  /*p06.KEXU*/ SCK_A.set(nand(_SER_CLK.q(), _XFER_DIR.q()));
  SCK_B.set(_XFER_DIR.q());
  /*p06.KUJO*/ SCK_D.set(nor(_SER_CLK.q(), /*p06.JAGO*/ not(_XFER_DIR.q())));

  /*p06.CUGY*/ top.CPU_TRI_D0.set_tribuf_6p(_FF01_RD, _SER_DATA0.q());
  /*p06.DUDE*/ top.CPU_TRI_D1.set_tribuf_6p(_FF01_RD, _SER_DATA1.q());
  /*p06.DETU*/ top.CPU_TRI_D2.set_tribuf_6p(_FF01_RD, _SER_DATA2.q());
  /*p06.DASO*/ top.CPU_TRI_D3.set_tribuf_6p(_FF01_RD, _SER_DATA3.q());
  /*p06.DAME*/ top.CPU_TRI_D4.set_tribuf_6p(_FF01_RD, _SER_DATA4.q());
  /*p06.EVOK*/ top.CPU_TRI_D5.set_tribuf_6p(_FF01_RD, _SER_DATA5.q());
  /*p06.EFAB*/ top.CPU_TRI_D6.set_tribuf_6p(_FF01_RD, _SER_DATA6.q());
  /*p06.ETAK*/ top.CPU_TRI_D7.set_tribuf_6p(_FF01_RD, _SER_DATA7.q());

  /*p06.CORE*/ top.CPU_TRI_D0.set_tribuf_6p(_FF02_RD, _XFER_DIR.q());
  /*p06.ELUV*/ top.CPU_TRI_D1.set_tribuf_6p(_FF02_RD, _XFER_START.q());
}

//------------------------------------------------------------------------------

SignalHash SerialRegisters::commit() {
  SignalHash hash;

  /*p06.ETAF*/ hash << _XFER_START.commit();
  /*p06.CULY*/ hash << _XFER_DIR.commit();
  /*p06.COTY*/ hash << _SER_CLK.commit();
  /*p06.CAFA*/ hash << _SER_CNT0.commit();
  /*p06.CYLO*/ hash << _SER_CNT1.commit();
  /*p06.CYDE*/ hash << _SER_CNT2.commit();
  /*p06.CALY*/ hash << _CALY_INT_SERIALp.commit();
  /*p06.CUBA*/ hash << _SER_DATA0.commit();
  /*p06.DEGU*/ hash << _SER_DATA1.commit();
  /*p06.DYRA*/ hash << _SER_DATA2.commit();
  /*p06.DOJO*/ hash << _SER_DATA3.commit();
  /*p06.DOVU*/ hash << _SER_DATA4.commit();
  /*p06.EJAB*/ hash << _SER_DATA5.commit();
  /*p06.EROD*/ hash << _SER_DATA6.commit();
  /*p06.EDER*/ hash << _SER_DATA7.commit();
  /*p06.ELYS*/ hash << _SER_OUT.commit();

  /* PIN_68 */ hash << SCK_A.commit();   // <- P06.KEXU
  /* PIN_68 */ hash << SCK_B.commit();   // <- P06.CULY
  /* PIN_68 */ hash << SCK_C.commit();   // -> P06.CAVE
  /* PIN_68 */ hash << SCK_D.commit();   // <- P06.KUJO
  ///* PIN_69 */ hash << SIN_A.commit();   // nc?
  ///* PIN_69 */ hash << SIN_B.commit();   // nc?
  /* PIN_69 */ hash << SIN_C.commit();   // -> P06.CAGE
  ///* PIN_69 */ hash << SIN_D.commit();   // nc?
  /* PIN_70 */ hash << SOUT.commit();    // <- P05.KENA
  return hash;
}

//------------------------------------------------------------------------------