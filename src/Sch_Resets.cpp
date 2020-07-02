#include "Sch_Resets.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

ResetSignals ResetRegisters::sig(const SchematicTop& gb) const {
  return sig(gb.tim_reg.sig(),
             gb.dbg_reg.sig(gb),
             gb.ppu_config);
}

ResetSignals ResetRegisters::sig(const TimerSignals& tim_sig, const DebugSignals& dbg_sig, const PpuConfig& ppu_config) const {
  ResetSignals sig;

  {
    /*p01.UNUT*/ wire UNUT_TIMEOUT  = and (TUBO_CLKREQn_LATCH, tim_sig.UPOF_DIV_15);
    /*p01.AFER*/ sig.AFER_RSTp = AFER_RSTp;
    /*p01.TABA*/ sig.TABA_RSTp = or(dbg_sig.UNOR_MODE_DBG2p, dbg_sig.UMUT_MODE_DBG1p, UNUT_TIMEOUT);
  }

  {
    /*p01.AVOR*/ sig.AVOR_RSTp = or(AFER_RSTp.q(), ASOL_RST_LATCHp.q());   
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(sig.AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
    /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, ppu_config.XONA_LCDC_EN);
    /*p01.XAPO*/ sig.XAPO_VID_RSTn = not(XODO_VID_RSTp);
  }

  return sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tick(SchematicTop& gb) {
  tick(gb.clk_reg.sig(gb),
       gb.dbg_reg.sig(gb),
       gb.rst_reg.sig(gb),
       gb.cpu_bus,
       gb.EXT_PIN_RST,
       gb.EXT_PIN_CLK_GOOD);
}

void ResetRegisters::tick(
  const ClockSignals& clk_sig,
  const DebugSignals& dbg_sig,
  const ResetSignals& rst_sig,
  const CpuBus& cpu_bus,
  wire EXT_PIN_RST,
  wire EXT_PIN_CLK_GOOD) {

  /*p01.ALYP*/ wire ALYP_RSTn = not(rst_sig.TABA_RSTp);
  /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, EXT_PIN_RST);

  // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
  /*p01.ASOL*/ ASOL_RST_LATCHp.nor_latch(AFAR_RST, EXT_PIN_RST); // Schematic wrong, this is a latch.

  /*p01.AFER*/ AFER_RSTp.set(clk_sig.BOMA_xBxxxxxx, dbg_sig.UPOJ_MODE_PROD, ASOL_RST_LATCHp);

  // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
  // TUBO00 << cpu_pins.CLKREQ
  // TUBO01 nc
  // TUBO02 >> UNUT 
  // TUBO03 == nc
  // TUBO04 nc
  // TUBO05 << UPYF
  /*p01.UCOB*/ wire UCOB_CLKBAD = not(EXT_PIN_CLK_GOOD);
  /*p01.UPYF*/ wire UPYF = or(EXT_PIN_RST, UCOB_CLKBAD);
  /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(cpu_bus.PIN_CLKREQ, UPYF);
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;
  /*p01.TUBO*/ hash << TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.ASOL*/ hash << ASOL_RST_LATCHp.commit_latch(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << AFER_RSTp.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------
