#include "Sch_Debug.h"

#include "TestGB.h"

using namespace Schematics;

//text_painter.dprintf("UNOR_MODE_DBG2           %d\n", UNOR_MODE_DBG2        .a.val);
//text_painter.dprintf("UMUT_MODE_DBG1           %d\n", UMUT_MODE_DBG1        .a.val);
//text_painter.dprintf("UPOJ_MODE_PRODn  %d\n", UPOJ_MODE_PRODn );
//text_painter.dprintf("UMUT_MODE_DBG1  %d\n", UMUT_MODE_DBG1 );
//text_painter.dprintf("UNOR_MODE_DBG2  %d\n", UNOR_MODE_DBG2 );
//text_painter.dprintf("TOVA_MODE_DBG2n %d\n", TOVA_MODE_DBG2n);
//text_painter.dprintf("RYCA_MODE_DBG2n %d\n", RYCA_MODE_DBG2n);

//-----------------------------------------------------------------------------

DebugSignals DebugRegisters::sig(const TestGB& gb) const {
  DebugSignals sig;

  auto& ext_pins_in = gb.ext_pins_in;
  auto& cpu_bus = gb.cpu_bus;

  wire P10_B = 0;

  /*p27.VYPO*/ sig.VYPO_P10_Bn = not(P10_B);
  /*p28.WEFE*/ sig.WEFE_P10_Bn = not(P10_B);
  /*p28.WUWE*/ sig.WUWE_P10_Bn = not(P10_B);
  /*p28.GEFY*/ sig.GEFY_P10_Bn = not(P10_B);
  /*p28.GECA*/ sig.GECA_P10_Bp = not(sig.WEFE_P10_Bn);
  /*p28.WYDU*/ sig.WYDU_P10_Bp = not(sig.WEFE_P10_Bn);



  /*p07.UBET*/ sig.UBET_T1n = not(ext_pins_in.PIN_T1);
  /*p07.UVAR*/ sig.UVAR_T2n = not(ext_pins_in.PIN_T2);
  /*p07.UMUT*/ sig.UMUT_MODE_DBG1p = and (ext_pins_in.PIN_T1, sig.UVAR_T2n);
  /*p07.UNOR*/ sig.UNOR_MODE_DBG2p = and (ext_pins_in.PIN_T2, sig.UBET_T1n); // Must be UNORp, see UJYV/UBAL
  /*p08.TOVA*/ sig.TOVA_MODE_DBG2n = not(sig.UNOR_MODE_DBG2p);
  /*p07.UPOJ*/ sig.UPOJ_MODE_PROD = nand(sig.UBET_T1n, sig.UVAR_T2n, ext_pins_in.PIN_RST);
  /*p08.RYCA*/ sig.RYCA_MODE_DBG2n = not(sig.UNOR_MODE_DBG2p);
  /*p25.TUTO*/ sig.TUTO_DBG_VRAMp = and (sig.UNOR_MODE_DBG2p, !SOTO_DBG);

  /*p??.APAP*/ wire _APAP = not(cpu_bus.PIN_ADDR_VALID); // Missing from schematic
  /*p01.AWOD*/ wire _AWOD = nor(sig.UNOR_MODE_DBG2p, _APAP);
  /*p01.ABUZ*/ sig.ABUZ = not(_AWOD);

  /*p25.RACO*/ sig.RACO_DBG_VRAMn = not(sig.TUTO_DBG_VRAMp);


  // what is this for exactly?
  // wire ABUZ = or(UNOR_MODE_DBG2n, !gb.cpu_pins.ADDR_VALID);

  return sig;
}

//-----------------------------------------------------------------------------

void DebugRegisters::tick(const TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p25.SYCY*/ wire SYCY_DBG_CLOCKp = not(dbg_sig.UNOR_MODE_DBG2p);
  /*p25.SOTO*/ SOTO_DBG.set(SYCY_DBG_CLOCKp, rst_sig.CUNU_RSTn, !SOTO_DBG);

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);
  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------

bool DebugRegisters::commit() {
  bool changed = false;
  /*p25.SOTO*/ changed |= SOTO_DBG.commit_reg();
  //changed |= cpu_pins.UNOR_MODE_DBG2.commit_pinout();         // PORTA_02: <- P07.UNOR_MODE_DBG2
  //changed |= cpu_pins.UMUT_MODE_DBG1.commit_pinout();         // PORTA_05: <- P07.UMUT_MODE_DBG1
  /*p07.BURO*/ changed |= BURO_FF60_0.commit_reg();
  /*p07.AMUT*/ changed |= AMUT_FF60_1.commit_reg();

  return changed;
}

//-----------------------------------------------------------------------------
