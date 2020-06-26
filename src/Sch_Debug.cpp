#include "Sch_Debug.h"

#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

DebugSignals DebugRegisters::sig(const TestGB& gb) const {
  const auto& sys_pins = gb.sys_pins;
  const auto& cpu_pins = gb.cpu_pins;

  /*p07.UBET*/ wire _UBET_T1n = not(sys_pins.T1);
  /*p07.UVAR*/ wire _UVAR_T2n = not(sys_pins.T2);
  /*p07.UMUT*/ wire _UMUT_MODE_DBG1 = and (sys_pins.T1, _UVAR_T2n);
  /*p07.UNOR*/ wire _UNOR_MODE_DBG2n = and (sys_pins.T2, _UBET_T1n);
  /*p08.TOVA*/ wire _TOVA_MODE_DBG2p = not(_UNOR_MODE_DBG2n);
  /*p07.UPOJ*/ wire _UPOJ_MODE_PRODn = nand(_UBET_T1n, _UVAR_T2n, sys_pins.RST);
  /*p08.RYCA*/ wire _RYCA_MODE_DBG2p = not(_UNOR_MODE_DBG2n);
  /*p25.TUTO*/ wire _TUTO_DBG_VRAM = and (_UNOR_MODE_DBG2n, !SOTO_DBG);

  /*p??.APAP*/ wire _APAP = not(cpu_pins.ADDR_VALID); // Missing from schematic
  /*p01.AWOD*/ wire _AWOD = nor(_UNOR_MODE_DBG2n, _APAP);
  /*p01.ABUZ*/ wire _ABUZ = not(_AWOD);

  // what is this for exactly?
  // wire ABUZ = or(UNOR_MODE_DBG2n, !gb.cpu_pins.ADDR_VALID);

  return {
    .UBET_T1n = _UBET_T1n,
    .UVAR_T2n = _UVAR_T2n,
    .UMUT_MODE_DBG1 = _UMUT_MODE_DBG1,
    .UNOR_MODE_DBG2n = _UNOR_MODE_DBG2n,
    .TOVA_MODE_DBG2p = _TOVA_MODE_DBG2p,
    .UPOJ_MODE_PRODn = _UPOJ_MODE_PRODn,
    .RYCA_MODE_DBG2p = _RYCA_MODE_DBG2p,
    .TUTO_DBG_VRAM = _TUTO_DBG_VRAM,
    .ABUZ = _ABUZ,
  };
}

//-----------------------------------------------------------------------------

void DebugRegisters::tick(const TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = sig(gb);
  auto rst_sig = ResetSignals::get(gb);

  /*p25.SYCY*/ wire SYCY_DBG_CLOCKp = not(dbg_sig.UNOR_MODE_DBG2n);
  /*p25.SOTO*/ SOTO_DBG.set(SYCY_DBG_CLOCKp, rst_sig.CUNU_RSTn, !SOTO_DBG);

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);
  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;

  wire FROM_CPU5 = gb.cpu_pins.FROM_CPU5;
  /*p04.MAKA*/ FROM_CPU5_SYNC.set(clk_sig.ZEME_AxCxExGx, rst_sig.CUNU_RSTn, FROM_CPU5);
}

//-----------------------------------------------------------------------------

bool DebugRegisters::commit() {
  bool changed = false;
  /*p25.SOTO*/ changed |= SOTO_DBG.commit_reg();
  /*p04.MAKA*/ changed |= FROM_CPU5_SYNC.commit_reg();
  //changed |= cpu_pins.UNOR_MODE_DBG2.commit_pinout();         // PORTA_02: <- P07.UNOR_MODE_DBG2
  //changed |= cpu_pins.UMUT_MODE_DBG1.commit_pinout();         // PORTA_05: <- P07.UMUT_MODE_DBG1
  return changed;
}

//-----------------------------------------------------------------------------

    //text_painter.dprintf("UNOR_MODE_DBG2           %d\n", UNOR_MODE_DBG2        .a.val);
    //text_painter.dprintf("UMUT_MODE_DBG1           %d\n", UMUT_MODE_DBG1        .a.val);
    //text_painter.dprintf("UPOJ_MODE_PRODn  %d\n", UPOJ_MODE_PRODn );
    //text_painter.dprintf("UMUT_MODE_DBG1  %d\n", UMUT_MODE_DBG1 );
    //text_painter.dprintf("UNOR_MODE_DBG2  %d\n", UNOR_MODE_DBG2 );
    //text_painter.dprintf("TOVA_MODE_DBG2n %d\n", TOVA_MODE_DBG2n);
    //text_painter.dprintf("RYCA_MODE_DBG2n %d\n", RYCA_MODE_DBG2n);
