#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"
#include "Sch_Timer.h"

using namespace Schematics;


void SerialRegisters::tick(TestGB& gb) {

  wire TAPU_CPU_WR_xxxxxFGH = gb.TAPU_CPU_WR_xxxxxFGH();
  wire TEDO_CPU_RD = gb.TEDO_CPU_RD();

  auto rst_sig = ResetSignals::get(gb);
  auto tim_sig = TimerSignals::get(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);
  auto& cpu_pins2 = gb.cpu_pins;

  //----------------------------------------

  /*p06.SANO*/ wire _ADDR_FF00_FF03 = and (adr_sig.SARE_XX00_XX07p, adr_sig.SEFY_A02n, adr_sig.SYKE_FF00_FFFFp);
  /*p06.URYS*/ wire _FF01_WR_xxxxxFGHn = nand(TAPU_CPU_WR_xxxxxFGH, _ADDR_FF00_FF03, cpu_pins2.A00, adr_sig.TOLA_A01n);
  /*p06.DAKU*/ wire _FF01_WR_xxxxxFGHp = not (_FF01_WR_xxxxxFGHn);
  /*p06.UWAM*/ wire _FF02_WRn_xxxxxFGH = nand(TAPU_CPU_WR_xxxxxFGH, _ADDR_FF00_FF03, cpu_pins2.A01, adr_sig.TOVY_A00n);
  /*p06.UFEG*/ wire _FF01_RD = and (TEDO_CPU_RD, _ADDR_FF00_FF03, cpu_pins2.A00, adr_sig.TOLA_A01n);
  /*p06.UCOM*/ wire _FF02_RD = and (TEDO_CPU_RD, _ADDR_FF00_FF03, cpu_pins2.A01, adr_sig.TOVY_A00n);

  /*p06.COBA*/ wire _SER_CNT3n = not(SER_CNT3.q());
  /*p06.CABY*/ wire _XFER_RESET = and (_SER_CNT3n, rst_sig.ALUR_RSTn);
  /*p06.ETAF*/ XFER_START.set(_FF02_WRn_xxxxxFGH, _XFER_RESET, cpu_pins2.D0);
  /*p06.CULY*/ XFER_DIR.set(_FF02_WRn_xxxxxFGH, rst_sig.ALUR_RSTn, cpu_pins2.D1);

  /*p06.COTY*/ SER_CLK.set(tim_sig.UVYN_DIV_05n, _FF02_WRn_xxxxxFGH, !SER_CLK.q());

  /*p06.CAVE*/ wire _SER_CLK_MUXn = mux2_n(SER_CLK.q(), SCK_C.q(), XFER_DIR.q());
  /*p06.DAWA*/ wire _DAWA_SER_CLK = or (_SER_CLK_MUXn, !XFER_START.q()); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ wire _EDYL_SER_CLK = not(_DAWA_SER_CLK);
  /*p06.EPYT*/ wire _EPYT_SER_CLK = not(_EDYL_SER_CLK);
  /*p06.DEHO*/ wire _DEHO_SER_CLK = not(_EPYT_SER_CLK);
  /*p06.DAWE*/ wire _DAWE_SER_CLK = not(_DEHO_SER_CLK);

  /*p06.CARO*/ wire _SER_RST = and (_FF02_WRn_xxxxxFGH, rst_sig.ALUR_RSTn);
  /*p06.CAFA*/ SER_CNT0.set(_DAWA_SER_CLK, _SER_RST, !SER_CNT0.q());
  /*p06.CYLO*/ SER_CNT1.set(!SER_CNT0.q(), _SER_RST, !SER_CNT1.q());
  /*p06.CYDE*/ SER_CNT2.set(!SER_CNT1.q(), _SER_RST, !SER_CNT2.q());
  /*p06.CALY*/ SER_CNT3.set(!SER_CNT2.q(), _SER_RST, !SER_CNT3.q());

  /*p06.CUFU*/ wire _SER_DATA0_SETn = nand(cpu_pins2.D0, _FF01_WR_xxxxxFGHp);
  /*p06.DOCU*/ wire _SER_DATA1_SETn = nand(cpu_pins2.D1, _FF01_WR_xxxxxFGHp);
  /*p06.DELA*/ wire _SER_DATA2_SETn = nand(cpu_pins2.D2, _FF01_WR_xxxxxFGHp);
  /*p06.DYGE*/ wire _SER_DATA3_SETn = nand(cpu_pins2.D3, _FF01_WR_xxxxxFGHp);
  /*p06.DOLA*/ wire _SER_DATA4_SETn = nand(cpu_pins2.D4, _FF01_WR_xxxxxFGHp);
  /*p06.ELOK*/ wire _SER_DATA5_SETn = nand(cpu_pins2.D5, _FF01_WR_xxxxxFGHp);
  /*p06.EDEL*/ wire _SER_DATA6_SETn = nand(cpu_pins2.D6, _FF01_WR_xxxxxFGHp);
  /*p06.EFEF*/ wire _SER_DATA7_SETn = nand(cpu_pins2.D7, _FF01_WR_xxxxxFGHp);

  /*p06.COHY*/ wire _SER_DATA0_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D0), rst_sig.ALUR_RSTn);
  /*p06.DUMO*/ wire _SER_DATA1_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D1), rst_sig.ALUR_RSTn);
  /*p06.DYBO*/ wire _SER_DATA2_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D2), rst_sig.ALUR_RSTn);
  /*p06.DAJU*/ wire _SER_DATA3_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D3), rst_sig.ALUR_RSTn);
  /*p06.DYLY*/ wire _SER_DATA4_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D4), rst_sig.ALUR_RSTn);
  /*p06.EHUJ*/ wire _SER_DATA5_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D5), rst_sig.ALUR_RSTn);
  /*p06.EFAK*/ wire _SER_DATA6_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D6), rst_sig.ALUR_RSTn);
  /*p06.EGUV*/ wire _SER_DATA7_RSTn = or (and (_FF01_WR_xxxxxFGHn, cpu_pins2.D7), rst_sig.ALUR_RSTn);

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

  /*p06.CUGY*/ cpu_pins2.D0.set_tribuf(_FF01_RD, SER_DATA0.q());
  /*p06.DUDE*/ cpu_pins2.D1.set_tribuf(_FF01_RD, SER_DATA1.q());
  /*p06.DETU*/ cpu_pins2.D2.set_tribuf(_FF01_RD, SER_DATA2.q());
  /*p06.DASO*/ cpu_pins2.D3.set_tribuf(_FF01_RD, SER_DATA3.q());
  /*p06.DAME*/ cpu_pins2.D4.set_tribuf(_FF01_RD, SER_DATA4.q());
  /*p06.EVOK*/ cpu_pins2.D5.set_tribuf(_FF01_RD, SER_DATA5.q());
  /*p06.EFAB*/ cpu_pins2.D6.set_tribuf(_FF01_RD, SER_DATA6.q());
  /*p06.ETAK*/ cpu_pins2.D7.set_tribuf(_FF01_RD, SER_DATA7.q());

  /*p06.CORE*/ cpu_pins2.D0.set_tribuf(_FF02_RD, XFER_DIR.q());
  /*p06.ELUV*/ cpu_pins2.D1.set_tribuf(_FF02_RD, XFER_START.q());
}
