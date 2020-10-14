#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset_cart() {
    DEGE_SPRITE_DELTA0.reset(TRI_D0NP);
    DABY_SPRITE_DELTA1.reset(TRI_D0NP);
    DABU_SPRITE_DELTA2.reset(TRI_D0NP);
    GYSA_SPRITE_DELTA3.reset(TRI_D0NP);
    GACE_SPRITE_DELTA4.reset(TRI_D0NP);
    GUVU_SPRITE_DELTA5.reset(TRI_D0NP);
    GYDA_SPRITE_DELTA6.reset(TRI_D0NP);
    GEWY_SPRITE_DELTA7.reset(TRI_D0NP);
    CARE_STORE_ENp_ABxxEFxx.reset(TRI_D0NP);

    BESU_SCANNINGp.reset(TRI_D0NP);
    CENO_SCANNINGp.reset(REG_D0C1);

    YFEL_SCAN0.reset(REG_D1C1);
    WEWY_SCAN1.reset(REG_D1C0);
    GOSO_SCAN2.reset(REG_D1C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C1);
    FONY_SCAN5.reset(REG_D1C1);

    BYBA_SCAN_DONE_A.reset(REG_D1C1);
    DOBA_SCAN_DONE_B.reset(REG_D1C0);

    XADU_SPRITE_IDX0p.reset(REG_D0C1);
    XEDY_SPRITE_IDX1p.reset(REG_D0C1);
    ZUZE_SPRITE_IDX2p.reset(REG_D1C1);
    XOBE_SPRITE_IDX3p.reset(REG_D0C1);
    YDUF_SPRITE_IDX4p.reset(REG_D1C1);
    XECU_SPRITE_IDX5p.reset(REG_D0C1);

    XUSO_OAM_DA0p.reset(REG_D0C1);
    XEGU_OAM_DA1p.reset(REG_D0C1);
    YJEX_OAM_DA2p.reset(REG_D0C1);
    XYJU_OAM_DA3p.reset(REG_D0C1);
    YBOG_OAM_DA4p.reset(REG_D0C1);
    WYSO_OAM_DA5p.reset(REG_D0C1);
    XOTE_OAM_DA6p.reset(REG_D0C1);
    YZAB_OAM_DA7p.reset(REG_D0C1);

    YLOR_OAM_DB0p.reset(REG_D0C1);
    ZYTY_OAM_DB1p.reset(REG_D0C1);
    ZYVE_OAM_DB2p.reset(REG_D0C1);
    ZEZY_OAM_DB3p.reset(REG_D0C1);
    GOMO_OAM_DB4p.reset(REG_D0C1);
    BAXO_OAM_DB5p.reset(REG_D0C1);
    YZOS_OAM_DB6p.reset(REG_D0C1);
    DEPO_OAM_DB7p.reset(REG_D0C1);
  }

  void reset_boot() {
    DEGE_SPRITE_DELTA0.reset(TRI_HZNP);
    DABY_SPRITE_DELTA1.reset(TRI_HZNP);
    DABU_SPRITE_DELTA2.reset(TRI_HZNP);
    GYSA_SPRITE_DELTA3.reset(TRI_HZNP);
    GACE_SPRITE_DELTA4.reset(TRI_HZNP);
    GUVU_SPRITE_DELTA5.reset(TRI_HZNP);
    GYDA_SPRITE_DELTA6.reset(TRI_HZNP);
    GEWY_SPRITE_DELTA7.reset(TRI_HZNP);
    CARE_STORE_ENp_ABxxEFxx.reset(TRI_HZNP);

    BESU_SCANNINGp.reset(TRI_D0NP);
    CENO_SCANNINGp.reset(REG_D0C0);

    YFEL_SCAN0.reset(REG_D0C0);
    WEWY_SCAN1.reset(REG_D0C0);
    GOSO_SCAN2.reset(REG_D0C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C0);
    FONY_SCAN5.reset(REG_D0C0);

    BYBA_SCAN_DONE_A.reset(REG_D0C0);
    DOBA_SCAN_DONE_B.reset(REG_D0C0);

    XADU_SPRITE_IDX0p.reset(REG_D0C0);
    XEDY_SPRITE_IDX1p.reset(REG_D0C0);
    ZUZE_SPRITE_IDX2p.reset(REG_D0C0);
    XOBE_SPRITE_IDX3p.reset(REG_D0C0);
    YDUF_SPRITE_IDX4p.reset(REG_D0C0);
    XECU_SPRITE_IDX5p.reset(REG_D0C0);

    XUSO_OAM_DA0p.reset(REG_D0C0);
    XEGU_OAM_DA1p.reset(REG_D0C0);
    YJEX_OAM_DA2p.reset(REG_D0C0);
    XYJU_OAM_DA3p.reset(REG_D0C0);
    YBOG_OAM_DA4p.reset(REG_D0C0);
    WYSO_OAM_DA5p.reset(REG_D0C0);
    XOTE_OAM_DA6p.reset(REG_D0C0);
    YZAB_OAM_DA7p.reset(REG_D0C0);

    YLOR_OAM_DB0p.reset(REG_D0C0);
    ZYTY_OAM_DB1p.reset(REG_D0C0);
    ZYVE_OAM_DB2p.reset(REG_D0C0);
    ZEZY_OAM_DB3p.reset(REG_D0C0);
    GOMO_OAM_DB4p.reset(REG_D0C0);
    BAXO_OAM_DB5p.reset(REG_D0C0);
    YZOS_OAM_DB6p.reset(REG_D0C0);
    DEPO_OAM_DB7p.reset(REG_D0C0);
  }

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  //----------------------------------------
  // Signals

  /*p29.DEGE*/ Sig DEGE_SPRITE_DELTA0; // -> sprite store
  /*p29.DABY*/ Sig DABY_SPRITE_DELTA1; // -> sprite store
  /*p29.DABU*/ Sig DABU_SPRITE_DELTA2; // -> sprite store
  /*p29.GYSA*/ Sig GYSA_SPRITE_DELTA3; // -> sprite store
  /*p29.GACE*/ Sig GACE_SPRITE_DELTA4;
  /*p29.GUVU*/ Sig GUVU_SPRITE_DELTA5;
  /*p29.GYDA*/ Sig GYDA_SPRITE_DELTA6;
  /*p29.GEWY*/ Sig GEWY_SPRITE_DELTA7;

  /*p29.CARE*/ Sig CARE_STORE_ENp_ABxxEFxx; // to sprite store

  //----------------------------------------
  // States

  /*p28.BESU*/ NorLatch BESU_SCANNINGp; // -> oam bus
  /*p29.CENO*/ DFF17 CENO_SCANNINGp;    // -> oam bus

  /*p28.YFEL*/ DFF17 YFEL_SCAN0; // -> oam bus address mux
  /*p28.WEWY*/ DFF17 WEWY_SCAN1;
  /*p28.GOSO*/ DFF17 GOSO_SCAN2;
  /*p28.ELYN*/ DFF17 ELYN_SCAN3;
  /*p28.FAHA*/ DFF17 FAHA_SCAN4;
  /*p28.FONY*/ DFF17 FONY_SCAN5;

  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_A; // -> top.AVAP_RENDER_START_TRIGp
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_B; // -> top.AVAP_RENDER_START_TRIGp

  /*p30.XADU*/ DFF13 XADU_SPRITE_IDX0p;
  /*p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p;
  /*p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p;
  /*p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p;
  /*p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p;
  /*p30.XECU*/ DFF13 XECU_SPRITE_IDX5p;

  // switching these to DFF8n breajs sprite4_*_b

  // oam byte 0, byte 2
  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

}; // namespace Schematics