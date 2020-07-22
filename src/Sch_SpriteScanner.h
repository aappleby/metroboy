#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;

//-----------------------------------------------------------------------------

struct SpriteScanner {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);

  // -> sprite store
  wire DEGE_SPRITE_DELTA0() const { return not(_ERUC_YDIFF_S0); }
  wire DABY_SPRITE_DELTA1() const { return not(_ENEF_YDIFF_S1); }
  wire DABU_SPRITE_DELTA2() const { return not(_FECO_YDIFF_S2); }
  wire GYSA_SPRITE_DELTA3() const { return not(_GYKY_YDIFF_S3); }

  // -> sprite store
  /*p29.DYTY*/ wire DYTY_STORE_ENn_xxCDxxGH() const { return not(_CARE_STORE_ENp_ABxxEFxx); }

  // -> top.ACYL
  /*p28.BESU*/ wire BESU_SCANNINGp() const { return _BESU_SCANNINGp.q(); }

  // -> sprite store
  /*p29.AROR*/ wire AROR_MATCH_ENp() const { return and(AZEM_RENDERINGp(), _XYLO_LCDC_SPEN); }

  // -> ppu.XYMU/POFY, scanner.ASEN, top.NYXU
  /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp() const {
    /*p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or(_BALU_LINE_RSTp, DOBA_SCAN_DONE_B(), !BYBA_SCAN_DONE_A());
    return not(_BEBU_SCAN_DONE_TRIGn);
  }

  // -> sprite store
  /*p29.BUZA*/ wire BUZA_STORE_SPRITE_INDX() const { return and(_CENO_SCANNINGp.qn(), _XYMU_RENDERINGp); }

  // -> oam bus
  /*p28.YFEL*/ wire YFEL_SCAN0() const { return _YFEL_SCAN0.q(); }
  /*p28.WEWY*/ wire WEWY_SCAN1() const { return _WEWY_SCAN1.q(); }
  /*p28.GOSO*/ wire GOSO_SCAN2() const { return _GOSO_SCAN2.q(); }
  /*p28.ELYN*/ wire ELYN_SCAN3() const { return _ELYN_SCAN3.q(); }
  /*p28.FAHA*/ wire FAHA_SCAN4() const { return _FAHA_SCAN4.q(); }
  /*p28.FONY*/ wire FONY_SCAN5() const { return _FONY_SCAN5.q(); }

  void dump(Dumper& d) {
    d("----------SpriteScan ---------\n");
    d("_BESU_SCANNINGp   %c\n", _BESU_SCANNINGp  .c());
    d("_CENO_SCANNINGp   %c\n", _CENO_SCANNINGp  .c());
    d("_BYBA_SCAN_DONE_A %c\n", _BYBA_SCAN_DONE_A.c());
    d("_DOBA_SCAN_DONE_B %c\n", _DOBA_SCAN_DONE_B.c());
    d("SCAN INDEX        %02d\n", 
      pack(0, 0, _FONY_SCAN5.q(), _FAHA_SCAN4.q(),
          _ELYN_SCAN3.q(), _GOSO_SCAN2.q(), _WEWY_SCAN1.q(), _YFEL_SCAN0.q()));
    d("\n");
  }

private:

  /*p29.AZEM*/ wire AZEM_RENDERINGp() const { return and(BYJO_SCANNINGn(), _XYMU_RENDERINGp); }
  /*p29.BYJO*/ wire BYJO_SCANNINGn() const { return not(CEHA_SCANNINGp()); }
  /*p29.CEHA*/ wire CEHA_SCANNINGp() const { return not(_CENO_SCANNINGp.qn()); }
  /*p29.DOBA*/ wire DOBA_SCAN_DONE_B() const { return _DOBA_SCAN_DONE_B.q(); }
  /*p29.BYBA*/ wire BYBA_SCAN_DONE_A() const { return _BYBA_SCAN_DONE_A.q(); }

  //----------------------------------------
  // Signals

  Sig2 _XYLO_LCDC_SPEN;
  Sig2 _XYMU_RENDERINGp;
  Sig2 _BALU_LINE_RSTp;
  Sig2 _BAGY_LINE_RSTn;

  /*p29.ERUC*/ Sig2 _ERUC_YDIFF_S0;
  /*p29.ERUC*/ Sig2 _ERUC_YDIFF_C0;
  /*p29.ENEF*/ Sig2 _ENEF_YDIFF_S1;
  /*p29.ENEF*/ Sig2 _ENEF_YDIFF_C1;
  /*p29.FECO*/ Sig2 _FECO_YDIFF_S2;
  /*p29.FECO*/ Sig2 _FECO_YDIFF_C2;
  /*p29.GYKY*/ Sig2 _GYKY_YDIFF_S3;
  /*p29.GYKY*/ Sig2 _GYKY_YDIFF_C3;
  /*p29.GOPU*/ Sig2 _GOPU_YDIFF_S4;
  /*p29.GOPU*/ Sig2 _GOPU_YDIFF_C4;
  /*p29.FUWA*/ Sig2 _FUWA_YDIFF_S5;
  /*p29.FUWA*/ Sig2 _FUWA_YDIFF_C5;
  /*p29.GOJU*/ Sig2 _GOJU_YDIFF_S6;
  /*p29.GOJU*/ Sig2 _GOJU_YDIFF_C6;
  /*p29.WUHU*/ Sig2 _WUHU_YDIFF_S7;
  /*p29.WUHU*/ Sig2 _WUHU_YDIFF_C7;

  /*p29.CARE*/ Sig2 _CARE_STORE_ENp_ABxxEFxx; // to sprite store

  //----------------------------------------
  // States

  /*p28.BESU*/ Pin2 _BESU_SCANNINGp = Pin2::LATCH_0;
  /*p29.CENO*/ Reg2 _CENO_SCANNINGp = Reg2::D0C0;

  /*p28.YFEL*/ Reg2 _YFEL_SCAN0 = Reg2::D0C0;
  /*p28.WEWY*/ Reg2 _WEWY_SCAN1 = Reg2::D0C0;
  /*p28.GOSO*/ Reg2 _GOSO_SCAN2 = Reg2::D0C0;
  /*p28.ELYN*/ Reg2 _ELYN_SCAN3 = Reg2::D0C0;
  /*p28.FAHA*/ Reg2 _FAHA_SCAN4 = Reg2::D0C0;
  /*p28.FONY*/ Reg2 _FONY_SCAN5 = Reg2::D0C0;
  /*p29.BYBA*/ Reg2 _BYBA_SCAN_DONE_A = Reg2::D0C0;
  /*p29.DOBA*/ Reg2 _DOBA_SCAN_DONE_B = Reg2::D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics