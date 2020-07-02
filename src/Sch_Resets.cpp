#include "Sch_Resets.h"
#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

ResetSignals ResetRegisters::sig(const TestGB& gb) const {
  ResetSignals sig;

  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto tim_sig = gb.tim_reg.sig(gb);

  auto& ppu_config = gb.ppu_config;

  {
    /*p01.UCOB*/ sig.UCOB_CLKBAD = not(gb.EXT_PIN_CLK_GOOD);
  }

  {
    /*p01.UNUT*/ wire UNUT_TIMEOUT  = and (TUBO_CLKREQn_LATCH, tim_sig.UPOF_DIV_15);
    /*p01.AFER*/ sig.AFER_RSTp = AFER_RSTp;
    /*p01.TABA*/ sig.TABA_RSTp = or(dbg_sig.UNOR_MODE_DBG2p, dbg_sig.UMUT_MODE_DBG1p, UNUT_TIMEOUT);
  }

  {
    /*p01.AVOR*/ sig.AVOR_RSTp = or(AFER_RSTp.q(), ASOL_RST_LATCHp.q());
    /*p01.ALUR*/ sig.ALUR_RSTn = not(sig.AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ sig.DULA_RSTp = not(sig.ALUR_RSTn);
    /*p01.CUNU*/ sig.CUNU_RSTn = not(sig.DULA_RSTp);
    /*p01.XORE*/ sig.XORE_RSTp = not(sig.CUNU_RSTn);
    /*p01.XEBE*/ sig.XEBE_RSTn = not(sig.XORE_RSTp);
    /*p01.WESY*/ sig.WESY_RSTn = not(sig.XORE_RSTp);
    /*p01.WALU*/ sig.WALU_RSTn = not(sig.XORE_RSTp);
  }

  {
    /*p01.XODO*/ sig.XODO_VID_RSTp = nand(sig.XEBE_RSTn, ppu_config.XONA_LCDC_EN);
    /*p01.XAPO*/ sig.XAPO_VID_RSTn = not(sig.XODO_VID_RSTp);
    /*p01.PYRY*/ sig.PYRY_VID_RSTp = not(sig.XAPO_VID_RSTn);
    /*p01.TOFU*/ sig.TOFU_VID_RSTp = not(sig.XAPO_VID_RSTn);
    /*p01.ATAR*/ sig.ATAR_VID_RSTp = not(sig.XAPO_VID_RSTn);
    /*p01.ABEZ*/ sig.ABEZ_VID_RSTn = not(sig.ATAR_VID_RSTp);
    /*p01.AMYG*/ sig.AMYG_VID_RSTp = not(sig.XAPO_VID_RSTn);
    /*p01.ROSY*/ sig.ROSY_VID_RSTp = not(sig.XAPO_VID_RSTn);
  }

  return sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;

  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p01.ALYP*/ wire ALYP_RSTn = not(rst_sig.TABA_RSTp);
  /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, gb.EXT_PIN_RST);

  // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
  /*p01.ASOL*/ ASOL_RST_LATCHp.nor_latch(AFAR_RST, gb.EXT_PIN_RST); // Schematic wrong, this is a latch.

  /*p01.AFER*/ AFER_RSTp.set(clk_sig.BOMA_xBxxxxxx, dbg_sig.UPOJ_MODE_PROD, ASOL_RST_LATCHp);

  // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
  // TUBO00 << cpu_pins.CLKREQ
  // TUBO01 nc
  // TUBO02 >> UNUT 
  // TUBO03 == nc
  // TUBO04 nc
  // TUBO05 << UPYF
  /*p01.UPYF*/ wire UPYF = or(gb.EXT_PIN_RST, rst_sig.UCOB_CLKBAD);
  /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(cpu_bus.PIN_CLKREQ, UPYF);
}

//-----------------------------------------------------------------------------

bool ResetRegisters::commit() {
  bool changed = false;
  /*p01.TUBO*/ changed |= TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.ASOL*/ changed |= ASOL_RST_LATCHp.commit_latch(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ changed |= AFER_RSTp.commit_reg();
  return changed;
}

//-----------------------------------------------------------------------------
