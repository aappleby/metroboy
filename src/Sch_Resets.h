#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ResetRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  SignalHash commit();

  wire get_rst() const {
    return _SYS_PIN_RSTp.get();
  }

  void set_rst(bool rst) {
    _SYS_PIN_RSTp.set(rst);
  }

  void set_cpu_ready(bool ready) {
    _CPU_PIN_READYp.set(ready);
  }

  void set_t1t2(bool t1, bool t2) {
    _SYS_PIN_T1n.set(t1);
    _SYS_PIN_T2n.set(t2);
  }

  //-----------------------------------------------------------------------------
  // Resets

  wire SYS_PIN_RSTp() const { return _SYS_PIN_RSTp.get(); }

  wire ALUR_SYS_RSTn() const { // used everywhere
    /*p01.AVOR*/ wire AVOR_SYS_RSTp = or(_AFER_SYS_RSTp.q(), _ASOL_POR_DONEn.q());
    /*p01.ALUR*/ wire ALUR_SYS_RSTn = not(AVOR_SYS_RSTp);
    return ALUR_SYS_RSTn;
  }

  wire CUNU_SYS_RSTn() const { // tile fetcher, dma, maka, soto
    /*p01.DULA*/ wire DULA_SYS_RSTp = not(ALUR_SYS_RSTn());
    /*p01.CUNU*/ wire CUNU_SYS_RSTn = not(DULA_SYS_RSTp);
    return CUNU_SYS_RSTn;
  }

  /*p01.XORE*/ wire XORE_SYS_RSTp() const { return not(CUNU_SYS_RSTn()); }
  /*p01.WALU*/ wire WALU_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }
  /*p01.WESY*/ wire WESY_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }
  /*p03.MULO*/ wire MULO_SYS_RSTn() const { return not(ALUR_SYS_RSTn()); }

  wire XAPO_VID_RSTn() const {
    /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not(XORE_SYS_RSTp());
    /*p01.XODO*/ wire _XODO_VID_RSTp = nand(_XEBE_SYS_RSTn, _XONA_LCDC_EN);
    /*p01.XAPO*/ wire _XAPO_VID_RSTn = not(_XODO_VID_RSTp);
    return _XAPO_VID_RSTn;
  }
  /*p01.ATAR*/ wire ATAR_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not(ATAR_VID_RSTp()); }
  /*p01.TOFU*/ wire TOFU_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.ROSY*/ wire ROSY_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.AMYG*/ wire AMYG_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYHA*/ wire LYHA_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYFE*/ wire LYFE_VID_RSTn() const { return not(LYHA_VID_RSTp()); }
  /*p01.PYRY*/ wire PYRY_VID_RSTp() const { return not(XAPO_VID_RSTn()); }

  /*p01.TABA*/ wire TABA_POR_TRIGn() const {
#ifdef FAST_BOOT
    // Just wait until DIV = 4 instead of DIV = 32768
    /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), _UPOF_DIV_1);
#else
    /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), _UPOF_DIV_15);
#endif
    return or(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  }

  /*p01.AFER*/ wire AFER_SYS_RSTp() const { return _AFER_SYS_RSTp.q(); }

  //-----------------------------------------------------------------------------
  // Debug stuff

  wire CPU_PIN_STARTp() const { return _CPU_PIN_STARTp.get(); }
  wire CPU_PIN_READYp() const { return _CPU_PIN_READYp.get(); }

  /*p01.ABOL*/ wire ABOL_CLKREQn() const { return not(_CPU_PIN_READYp.get()); }

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp() const { return and (UNOR_MODE_DBG2p(), SOTO_DBG.qn()); }
  /*p25.RACO*/ wire RACO_DBG_VRAMn() const { return not(TUTO_DBG_VRAMp()); }

  /*p07.UBET*/ wire UBET_T1p()        const { return not(_SYS_PIN_T1n); }
  /*p07.UVAR*/ wire UVAR_T2p()        const { return not(_SYS_PIN_T2n); }
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and(_SYS_PIN_T1n, UVAR_T2p()); }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and(_SYS_PIN_T2n, UBET_T1p()); }
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand(UBET_T1p(), UVAR_T2p(), _SYS_PIN_RSTp); }
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not(UNOR_MODE_DBG2p()); }

  //-----------------------------------------------------------------------------

private:

  Signal _XONA_LCDC_EN;
  Signal _UPOF_DIV_15;

  ExtPinIn  _SYS_PIN_RSTp;   // PIN_71 -> UPOJ, UPYF, AFAR, ASOL, UFOL
  ExtPinIn  _SYS_PIN_T2n;    // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  ExtPinIn  _SYS_PIN_T1n;    // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.TUBO*/ NorLatch _TUBO_CPU_READYn;

  // If AVOR_RSTp was 1 in run mode
  // then ALUR_RSTn = 0
  // then MULO_TIMA_RST = 1
  // then PUXY/NERO = 0 -> stuck
  // Therefore AVOR_RSTp = 0 in run mode
  // Therefore ASOL|AFER = 0 in run mode

  /*p01.ASOL*/ NorLatch _ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg13    _AFER_SYS_RSTp; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

  CpuPinOut _CPU_PIN_STARTp;    // top center port PORTC_04: <- P01.CPU_RESET
  CpuPinIn  _CPU_PIN_READYp;    // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  CpuPinOut _CPU_PIN_SYS_RSTp;  // top center port PORTC_01: <- P01.AFER , reset related reg
  CpuPinOut _CPU_PIN_EXT_RST;   // top center port PORTC_02: <- PIN_RESET directly connected to the pad 

  /*p25.SOTO*/ Reg17 SOTO_DBG;

  CpuPinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  CpuPinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

};


//-----------------------------------------------------------------------------

}; // namespace Schematics