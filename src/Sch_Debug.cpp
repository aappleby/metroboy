#include "Sch_Debug.h"

#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

DebugSignals DebugRegisters::sig(const TestGB& gb) const {
  /*p07.UBET*/ wire UBET_T1n = not(gb.sys_pins.T1);
  /*p07.UVAR*/ wire UVAR_T2n = not(gb.sys_pins.T2);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1 = and (gb.sys_pins.T1, UVAR_T2n);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2n = and (gb.sys_pins.T2, UBET_T1n);
  /*p08.TOVA*/ wire TOVA_MODE_DBG2p = not(UNOR_MODE_DBG2n);
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, gb.sys_pins.RST);
  /*p08.RYCA*/ wire RYCA_MODE_DBG2p = not(UNOR_MODE_DBG2n);
  /*p25.TUTO*/ wire TUTO_DBG_VRAM = and (UNOR_MODE_DBG2n, !SOTO_DBG);

  /*p??.APAP*/ wire APAP = not(gb.cpu_pins.ADDR_VALID); // Missing from schematic
  /*p01.ABUZ*/ wire AWOD = nor(UNOR_MODE_DBG2n, APAP);
  /*p01.ABUZ*/ wire ABUZ = not(AWOD);


  return {
    .UBET_T1n = UBET_T1n,
    .UVAR_T2n = UVAR_T2n,
    .UMUT_MODE_DBG1 = UMUT_MODE_DBG1,
    .UNOR_MODE_DBG2n = UNOR_MODE_DBG2n,
    .TOVA_MODE_DBG2p = TOVA_MODE_DBG2p,
    .UPOJ_MODE_PRODn = UPOJ_MODE_PRODn,
    .RYCA_MODE_DBG2p = RYCA_MODE_DBG2p,
    .TUTO_DBG_VRAM = TUTO_DBG_VRAM,
    .ABUZ = ABUZ,
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
  return changed;
}

//-----------------------------------------------------------------------------
