#include "Sch_Resets.h"
#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

ResetSignals ResetRegisters::sig(const TestGB& gb) const {
  /*p01.UCOB*/ wire UCOB_CLKBAD = not(gb.sys_pins.PIN_CLK_GOOD);

  /*p23.XONA*/ wire XONA_LCDC_EN = gb.cfg_reg.XONA_LCDC_EN.q();

  /*p01.AVOR*/ wire AVOR_RSTp = or(RESET_REGp.q(), ASOL_RST_LATCHp.q());

  /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
  /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
  /*p01.WESY*/ wire WESY_RSTn = not(XORE_RSTp);
  /*p01.WALU*/ wire WALU_RSTn = not(XORE_RSTp);

  /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, XONA_LCDC_EN);
  /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
  /*p01.PYRY*/ wire PYRY_VID_RSTp = not(XAPO_VID_RSTn);
  /*p01.TOFU*/ wire TOFU_VID_RSTp = not(XAPO_VID_RSTn);
  /*p01.LYHA*/ wire LYHA_VID_RSTp = not(XAPO_VID_RSTn);
  /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
  /*p01.AMYG*/ wire AMYG_VID_RSTp = not(XAPO_VID_RSTn);
  /*p01.ROSY*/ wire ROSY_VID_RSTp = not(XAPO_VID_RSTn);

  return {
    .UCOB_CLKBAD = UCOB_CLKBAD,

    .ALUR_RSTn = ALUR_RSTn,
    .DULA_RSTp = DULA_RSTp,
    .CUNU_RSTn = CUNU_RSTn,
    .XORE_RSTp = XORE_RSTp,
    .XEBE_RSTn = XEBE_RSTn,
    .WESY_RSTn = WESY_RSTn,
    .WALU_RSTn = WALU_RSTn,

    .XODO_VID_RSTp = XODO_VID_RSTp,
    .XAPO_VID_RSTn = XAPO_VID_RSTn,
    .PYRY_VID_RSTp = PYRY_VID_RSTp,
    .TOFU_VID_RSTp = TOFU_VID_RSTp,
    .LYHA_VID_RSTp = LYHA_VID_RSTp,
    .LYFE_VID_RSTn = LYFE_VID_RSTn,
    .ATAR_VID_RSTp = ATAR_VID_RSTp,
    .ABEZ_VID_RSTn = ABEZ_VID_RSTn,
    .AMYG_VID_RSTp = AMYG_VID_RSTp,
    .ROSY_VID_RSTp = ROSY_VID_RSTp,
  };
}

//-----------------------------------------------------------------------------

void ResetRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p01.UNUT*/ wire TIMEOUT = and (TUBO_CLKREQn_LATCH, gb.tim_reg.UPOF_DIV_15);
  /*p01.TABA*/ wire TABA_RST = or(dbg_sig.UNOR_MODE_DBG2p, dbg_sig.UMUT_MODE_DBG1p, TIMEOUT);
  /*p01.ALYP*/ wire ALYP_RSTn = not(TABA_RST);
  /*p01.AFAR*/ wire AFAR_RST = nor(ALYP_RSTn, gb.sys_pins.RST);

  // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
  /*p01.ASOL*/ ASOL_RST_LATCHp.nor_latch(AFAR_RST, gb.sys_pins.RST); // Schematic wrong, this is a latch.

  /*p01.AFER*/ RESET_REGp.set(clk_sig.PIN_BOMA_xBxxxxxx, dbg_sig.UPOJ_MODE_PROD, ASOL_RST_LATCHp);

  gb.cpu_bus.PIN_CPU_RESET.set(TABA_RST);
  gb.cpu_bus.PIN_AFER.set(RESET_REGp.q());

  //gb.cpu_pins.PIN_RESET.set(sys_pins.RST);

  // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
  // TUBO00 << cpu_pins.CLKREQ
  // TUBO01 nc
  // TUBO02 >> UNUT 
  // TUBO03 == nc
  // TUBO04 nc
  // TUBO05 << UPYF
  /*p01.UPYF*/ wire UPYF = or(gb.sys_pins.RST, rst_sig.UCOB_CLKBAD);
  /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(gb.cpu_bus.PIN_CLKREQ, UPYF);
}

//-----------------------------------------------------------------------------

bool ResetRegisters::commit() {
  bool changed = false;
  /*p01.TUBO*/ changed |= TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.AFER*/ changed |= RESET_REGp.commit_reg();
  return changed;
}

//-----------------------------------------------------------------------------
