#include "Sch_Resets.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

ResetSignals ResetRegisters::sig(const SchematicTop& gb) const {
  return sig(
    gb.tim_reg.sig().UPOF_DIV_15,
    gb.dbg_reg.sig(gb).UMUT_MODE_DBG1p,
    gb.dbg_reg.sig(gb).UNOR_MODE_DBG2p,
    gb.ppu_config.XONA_LCDC_EN);
}

ResetSignals ResetRegisters::sig(wire UPOF_DIV_15, wire UMUT_MODE_DBG1p, wire UNOR_MODE_DBG2p, wire XONA_LCDC_EN) const {
  ResetSignals sig;

  {
    /*p01.UNUT*/ wire UNUT_TIMEOUT  = and (TUBO_CLKREQn_LATCH, UPOF_DIV_15);
    /*p01.AFER*/ sig.AFER_RSTp = AFER_RSTp;
    /*p01.TABA*/ sig.TABA_RSTp = or(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, UNUT_TIMEOUT);
  }

  {
    /*p01.AVOR*/ sig.AVOR_RSTp = or(AFER_RSTp.q(), ASOL_RST_LATCHp.q());   
  }

  {
    /*p01.AVOR*/ wire AVOR_RSTp = or(AFER_RSTp.q(), ASOL_RST_LATCHp.q());   
    /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
    /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, XONA_LCDC_EN);
    /*p01.XAPO*/ sig.XAPO_VID_RSTn = not(XODO_VID_RSTp);
  }

  return sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tick(SchematicTop& gb) {
  tick(gb.clk_reg.sig(),
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

  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_sig.AFUR_xBCDExxx);
  /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(!clk_sig.APUK_xxxDEFGx);
  /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_sig.ALEF_xxCDEFxx);
  /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
  /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
  /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
  /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
  /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
  /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
  /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
  /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
  /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
  /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(EXT_PIN_CLK_GOOD);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);

  /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
  /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);
  /*p01.AFER*/ AFER_RSTp.set(BOMA_xBxxxxxx, dbg_sig.UPOJ_MODE_PROD, ASOL_RST_LATCHp);

  // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
  // TUBO00 << cpu_pins.CLKREQ
  // TUBO01 nc
  // TUBO02 >> UNUT 
  // TUBO03 == nc
  // TUBO04 nc
  // TUBO05 << UPYF
  /*p01.UCOB*/ wire UCOB_CLKBAD = not(EXT_PIN_CLK_GOOD);
  /*p01.UPYF*/ wire UPYF = or(EXT_PIN_RST, UCOB_CLKBAD);
  /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(cpu_bus.CPU_PIN_CLKREQ, UPYF);
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
