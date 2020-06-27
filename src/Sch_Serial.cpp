#include "Sch_Serial.h"

#include "TestGB.h"
#include "Sch_Timer.h"

using namespace Schematics;

//------------------------------------------------------------------------------

SerialSignals SerialRegisters::sig(const TestGB& /*gb*/) const {
  return {
    .CALY_INT_SERIALp = CALY_INT_SERIALp,
  };
}

void SerialRegisters::tick(TestGB& gb) {

  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto tim_sig = gb.tim_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto& cpu_pins2 = gb.cpu_bus;

  wire TAPU_CPU_WR_xxxxxFGH = cpu_sig.TAPU_CPU_WR_xxxxxFGH;
  wire TEDO_CPU_RD = cpu_sig.TEDO_CPU_RD;

  //----------------------------------------

  /*p06.SANO*/ wire _ADDR_FF00_FF03 = and (adr_sig.SARE_XX00_XX07p, adr_sig.SEFY_A02n, adr_sig.SYKE_FF00_FFFFp);
  /*p06.URYS*/ wire _FF01_WR_xxxxxFGHn = nand(TAPU_CPU_WR_xxxxxFGH, _ADDR_FF00_FF03, cpu_pins2.PIN_A00, adr_sig.TOLA_A01n);
  /*p06.DAKU*/ wire _FF01_WR_xxxxxFGHp = not (_FF01_WR_xxxxxFGHn);
  /*p06.UWAM*/ wire _FF02_WRn_xxxxxFGH = nand(TAPU_CPU_WR_xxxxxFGH, _ADDR_FF00_FF03, cpu_pins2.PIN_A01, adr_sig.TOVY_A00n);
  /*p06.UFEG*/ wire _FF01_RD = and (TEDO_CPU_RD, _ADDR_FF00_FF03, cpu_pins2.PIN_A00, adr_sig.TOLA_A01n);
  /*p06.UCOM*/ wire _FF02_RD = and (TEDO_CPU_RD, _ADDR_FF00_FF03, cpu_pins2.PIN_A01, adr_sig.TOVY_A00n);

  /*p06.COBA*/ wire _SER_CNT3n = not(CALY_INT_SERIALp.q());
  /*p06.CABY*/ wire _XFER_RESET = and (_SER_CNT3n, rst_sig.ALUR_RSTn);
  /*p06.ETAF*/ XFER_START.set(_FF02_WRn_xxxxxFGH, _XFER_RESET, cpu_pins2.TS_D0);
  /*p06.CULY*/ XFER_DIR.set(_FF02_WRn_xxxxxFGH, rst_sig.ALUR_RSTn, cpu_pins2.TS_D1);

  /*p06.COTY*/ SER_CLK.set(tim_sig.UVYN_DIV_05n, _FF02_WRn_xxxxxFGH, !SER_CLK.q());

  // XFER_DIR 0 = ext clk
  // XFER DIR 1 = int clk

  // CAVE01 << CULY17 (xfer dir)
  // CAVE02 << COTY17 (mux 1 = int clk)
  // CAVE03 << SCK_C  (mux 0 = ext clk)
  // CAVE04 nc
  // CAVE05 >> DAWA01

  /*p06.CAVE*/ wire _SER_CLK_MUXn = mux2_n(SER_CLK.q(), SCK_C.q(), XFER_DIR.q());

  /*p06.DAWA*/ wire _DAWA_SER_CLK = or(_SER_CLK_MUXn, !XFER_START.q()); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ wire _EDYL_SER_CLK = not(_DAWA_SER_CLK);
  /*p06.EPYT*/ wire _EPYT_SER_CLK = not(_EDYL_SER_CLK);
  /*p06.DEHO*/ wire _DEHO_SER_CLK = not(_EPYT_SER_CLK);
  /*p06.DAWE*/ wire _DAWE_SER_CLK = not(_DEHO_SER_CLK);

  /*p06.CARO*/ wire _SER_RST = and (_FF02_WRn_xxxxxFGH, rst_sig.ALUR_RSTn);
  /*p06.CAFA*/ SER_CNT0.set(_DAWA_SER_CLK, _SER_RST, !SER_CNT0.q());
  /*p06.CYLO*/ SER_CNT1.set(!SER_CNT0.q(), _SER_RST, !SER_CNT1.q());
  /*p06.CYDE*/ SER_CNT2.set(!SER_CNT1.q(), _SER_RST, !SER_CNT2.q());
  /*p06.CALY*/ CALY_INT_SERIALp.set(!SER_CNT2.q(), _SER_RST, !CALY_INT_SERIALp.q());

  /*p06.CUFU*/ wire _SER_DATA0_SETn = nand(cpu_pins2.TS_D0, _FF01_WR_xxxxxFGHp);
  /*p06.DOCU*/ wire _SER_DATA1_SETn = nand(cpu_pins2.TS_D1, _FF01_WR_xxxxxFGHp);
  /*p06.DELA*/ wire _SER_DATA2_SETn = nand(cpu_pins2.TS_D2, _FF01_WR_xxxxxFGHp);
  /*p06.DYGE*/ wire _SER_DATA3_SETn = nand(cpu_pins2.TS_D3, _FF01_WR_xxxxxFGHp);
  /*p06.DOLA*/ wire _SER_DATA4_SETn = nand(cpu_pins2.TS_D4, _FF01_WR_xxxxxFGHp);
  /*p06.ELOK*/ wire _SER_DATA5_SETn = nand(cpu_pins2.TS_D5, _FF01_WR_xxxxxFGHp);
  /*p06.EDEL*/ wire _SER_DATA6_SETn = nand(cpu_pins2.TS_D6, _FF01_WR_xxxxxFGHp);
  /*p06.EFEF*/ wire _SER_DATA7_SETn = nand(cpu_pins2.TS_D7, _FF01_WR_xxxxxFGHp);

  // COHY 5-rung
  // DUMO 5-rung
  // DYBO 5-rung
  // DAJU 5-rung
  // DYLY 5-rung
  // EHUJ 5-rung
  // EFAK 5-rung
  // EGUV 5-rung

  /*p06.COHY*/ wire _SER_DATA0_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D0), rst_sig.ALUR_RSTn);
  /*p06.DUMO*/ wire _SER_DATA1_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D1), rst_sig.ALUR_RSTn);
  /*p06.DYBO*/ wire _SER_DATA2_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D2), rst_sig.ALUR_RSTn);
  /*p06.DAJU*/ wire _SER_DATA3_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D3), rst_sig.ALUR_RSTn);
  /*p06.DYLY*/ wire _SER_DATA4_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D4), rst_sig.ALUR_RSTn);
  /*p06.EHUJ*/ wire _SER_DATA5_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D5), rst_sig.ALUR_RSTn);
  /*p06.EFAK*/ wire _SER_DATA6_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D6), rst_sig.ALUR_RSTn);
  /*p06.EGUV*/ wire _SER_DATA7_RSTn = or(and(_FF01_WR_xxxxxFGHn, cpu_pins2.TS_D7), rst_sig.ALUR_RSTn);

  /*p06.CAGE*/ wire _SIN_Cn = not(SIN_C);
  /*p06.CUBA*/ SER_DATA0.set(_DAWE_SER_CLK, _SER_DATA0_SETn, _SER_DATA0_RSTn, _SIN_Cn);
  /*p06.DEGU*/ SER_DATA1.set(_DAWE_SER_CLK, _SER_DATA1_SETn, _SER_DATA1_RSTn, SER_DATA0.q());
  /*p06.DYRA*/ SER_DATA2.set(_DAWE_SER_CLK, _SER_DATA2_SETn, _SER_DATA2_RSTn, SER_DATA1.q());
  /*p06.DOJO*/ SER_DATA3.set(_DAWE_SER_CLK, _SER_DATA3_SETn, _SER_DATA3_RSTn, SER_DATA2.q());
  /*p06.DOVU*/ SER_DATA4.set(_EPYT_SER_CLK, _SER_DATA4_SETn, _SER_DATA4_RSTn, SER_DATA3.q());
  /*p06.EJAB*/ SER_DATA5.set(_EPYT_SER_CLK, _SER_DATA5_SETn, _SER_DATA5_RSTn, SER_DATA4.q());
  /*p06.EROD*/ SER_DATA6.set(_EPYT_SER_CLK, _SER_DATA6_SETn, _SER_DATA6_RSTn, SER_DATA5.q());
  /*p06.EDER*/ SER_DATA7.set(_EPYT_SER_CLK, _SER_DATA7_SETn, _SER_DATA7_RSTn, SER_DATA6.q());
  /*p06.ELYS*/ SER_OUT.set(_EDYL_SER_CLK, rst_sig.ALUR_RSTn, SER_DATA7.q());

  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
  /*p05.KENA*/ SOUT.set(SER_OUT.q());
  /*p06.KEXU*/ SCK_A.set(nand(SER_CLK.q(), XFER_DIR.q()));
  SCK_B.set(XFER_DIR.q());
  /*p06.KUJO*/ SCK_D.set(nor(SER_CLK.q(), /*p06.JAGO*/ not(XFER_DIR.q())));

  /*p06.CUGY*/ cpu_pins2.TS_D0.set_tribuf(_FF01_RD, SER_DATA0.q());
  /*p06.DUDE*/ cpu_pins2.TS_D1.set_tribuf(_FF01_RD, SER_DATA1.q());
  /*p06.DETU*/ cpu_pins2.TS_D2.set_tribuf(_FF01_RD, SER_DATA2.q());
  /*p06.DASO*/ cpu_pins2.TS_D3.set_tribuf(_FF01_RD, SER_DATA3.q());
  /*p06.DAME*/ cpu_pins2.TS_D4.set_tribuf(_FF01_RD, SER_DATA4.q());
  /*p06.EVOK*/ cpu_pins2.TS_D5.set_tribuf(_FF01_RD, SER_DATA5.q());
  /*p06.EFAB*/ cpu_pins2.TS_D6.set_tribuf(_FF01_RD, SER_DATA6.q());
  /*p06.ETAK*/ cpu_pins2.TS_D7.set_tribuf(_FF01_RD, SER_DATA7.q());

  /*p06.CORE*/ cpu_pins2.TS_D0.set_tribuf(_FF02_RD, XFER_DIR.q());
  /*p06.ELUV*/ cpu_pins2.TS_D1.set_tribuf(_FF02_RD, XFER_START.q());
}

//------------------------------------------------------------------------------

bool SerialRegisters::commit() {
  bool changed = false;

  /*p06.ETAF*/ changed |= XFER_START.commit_reg();
  /*p06.CULY*/ changed |= XFER_DIR.commit_reg();
  /*p06.COTY*/ changed |= SER_CLK.commit_reg();
  /*p06.CAFA*/ changed |= SER_CNT0.commit_reg();
  /*p06.CYLO*/ changed |= SER_CNT1.commit_reg();
  /*p06.CYDE*/ changed |= SER_CNT2.commit_reg();
  /*p06.CALY*/ changed |= CALY_INT_SERIALp.commit_reg();
  /*p06.CUBA*/ changed |= SER_DATA0.commit_reg();
  /*p06.DEGU*/ changed |= SER_DATA1.commit_reg();
  /*p06.DYRA*/ changed |= SER_DATA2.commit_reg();
  /*p06.DOJO*/ changed |= SER_DATA3.commit_reg();
  /*p06.DOVU*/ changed |= SER_DATA4.commit_reg();
  /*p06.EJAB*/ changed |= SER_DATA5.commit_reg();
  /*p06.EROD*/ changed |= SER_DATA6.commit_reg();
  /*p06.EDER*/ changed |= SER_DATA7.commit_reg();
  /*p06.ELYS*/ changed |= SER_OUT.commit_reg();

  /* PIN_68 */ changed |= SCK_A.commit_pinout();   // <- P06.KEXU
  /* PIN_68 */ changed |= SCK_B.commit_pinout();   // <- P06.CULY
  /* PIN_68 */ changed |= SCK_C.clear_preset();   // -> P06.CAVE
  /* PIN_68 */ changed |= SCK_D.commit_pinout();   // <- P06.KUJO
  ///* PIN_69 */ changed |= SIN_A.commit();   // nc?
  ///* PIN_69 */ changed |= SIN_B.commit();   // nc?
  /* PIN_69 */ changed |= SIN_C.clear_preset();   // -> P06.CAGE
  ///* PIN_69 */ changed |= SIN_D.commit();   // nc?
  /* PIN_70 */ changed |= SOUT.commit_pinout();    // <- P05.KENA
  return changed;
}

//------------------------------------------------------------------------------

void SerialRegisters::dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- SER_REG -----\n");
  text_painter.dprintf("SER_CLK    %d\n", SER_CLK.a.val);
  text_painter.dprintf("XFER_START %d\n", XFER_START.a.val);
  text_painter.dprintf("XFER_DIR   %d\n", XFER_DIR.a.val);
  text_painter.dprintf("SER_OUT    %d\n", SER_OUT.a.val);
  text_painter.dprintf("SER_CNT    %d\n", ser_cnt());
  text_painter.dprintf("SER_DATA   %d\n", ser_data());
  text_painter.newline();
}

void SerialRegisters::dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- SER_PINS -----\n");
  text_painter.dprintf("SCK  %d:%d:%d:%d\n", SCK_A.a.val, SCK_B.a.val, SCK_C.a.val, SCK_D.a.val);
  text_painter.dprintf("SIN  %d:%d:%d:%d\n", SIN_A.a.val, SIN_B.a.val, SIN_C.a.val, SIN_D.a.val);
  text_painter.dprintf("SOUT %d\n", SOUT.a.val);
  text_painter.newline();
}

//------------------------------------------------------------------------------