#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------
// Tristate buses to move sprite index & line in/out of sprite store

struct SpriteIBus {
  void reset();

  /*_BUS_SPR_I0*/ Bus BUS_SPR_I0; // AxCxExGx
  /*_BUS_SPR_I1*/ Bus BUS_SPR_I1; // AxCxExGx
  /*_BUS_SPR_I2*/ Bus BUS_SPR_I2; // AxCxExGx
  /*_BUS_SPR_I3*/ Bus BUS_SPR_I3; // AxCxExGx
  /*_BUS_SPR_I4*/ Bus BUS_SPR_I4; // AxCxExGx
  /*_BUS_SPR_I5*/ Bus BUS_SPR_I5; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SpriteLBus {
  void reset();

  /*_BUS_SPR_L0*/ Bus BUS_SPR_L0; // AxCxExGx
  /*_BUS_SPR_L1*/ Bus BUS_SPR_L1; // AxCxExGx
  /*_BUS_SPR_L2*/ Bus BUS_SPR_L2; // AxCxExGx
  /*_BUS_SPR_L3*/ Bus BUS_SPR_L3; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SpriteMatchFlags {
  void reset();

  /*_p29.GUVA*/ Gate GUVA_SPRITE0_GETp;
  /*_p29.ENUT*/ Gate ENUT_SPRITE1_GETp;
  /*_p29.EMOL*/ Gate EMOL_SPRITE2_GETp;
  /*_p29.GYFY*/ Gate GYFY_SPRITE3_GETp;
  /*_p29.GONO*/ Gate GONO_SPRITE4_GETp;
  /*_p29.GEGA*/ Gate GEGA_SPRITE5_GETp;
  /*_p29.XOJA*/ Gate XOJA_SPRITE6_GETp;
  /*_p29.GUTU*/ Gate GUTU_SPRITE7_GETp;
  /*_p29.FOXA*/ Gate FOXA_SPRITE8_GETp;
  /*_p29.GUZE*/ Gate GUZE_SPRITE9_GETp;
};

//-----------------------------------------------------------------------------

struct SpriteResetFlags {
  void reset();

  /*_p29.EBOJ*/ DFF17 EBOJ_STORE0_RSTp;   // AxCxExGx
  /*_p29.CEDY*/ DFF17 CEDY_STORE1_RSTp;   // AxCxExGx
  /*_p29.EGAV*/ DFF17 EGAV_STORE2_RSTp;   // AxCxExGx
  /*_p29.GOTA*/ DFF17 GOTA_STORE3_RSTp;   // AxCxExGx
  /*_p29.XUDY*/ DFF17 XUDY_STORE4_RSTp;   // AxCxExGx
  /*_p29.WAFY*/ DFF17 WAFY_STORE5_RSTp;   // AxCxExGx
  /*_p29.WOMY*/ DFF17 WOMY_STORE6_RSTp;   // AxCxExGx
  /*_p29.WAPO*/ DFF17 WAPO_STORE7_RSTp;   // AxCxExGx
  /*_p29.EXUQ*/ DFF17 EXUQ_STORE8_RSTp;   // AxCxExGx
  /*_p29.FONO*/ DFF17 FONO_STORE9_RSTp;   // AxCxExGx
};

//-----------------------------------------------------------------------------
// All 10 of the sprite stores

// STORE_I - xBxxxFxx
// STORE_L - xBxxxFxx
// STORE_X - During scan, xxxDxxxH. During line, AxCxExGx

struct StoreI0 {
  void reset();

  /*_p30.YGUS*/ DFF8 YGUS_STORE0_I0p;
  /*_p30.YSOK*/ DFF8 YSOK_STORE0_I1p;
  /*_p30.YZEP*/ DFF8 YZEP_STORE0_I2p;
  /*_p30.WYTE*/ DFF8 WYTE_STORE0_I3p;
  /*_p30.ZONY*/ DFF8 ZONY_STORE0_I4p;
  /*_p30.YWAK*/ DFF8 YWAK_STORE0_I5p;
};

struct StoreL0 {
  void reset();

  /*_p30.GYHO*/ DFF8 GYHO_STORE0_L0p;
  /*_p30.CUFO*/ DFF8 CUFO_STORE0_L1p;
  /*_p30.BOZU*/ DFF8 BOZU_STORE0_L2p;
  /*_p30.FYHY*/ DFF8 FYHY_STORE0_L3p;
};

struct StoreX0 {
  void reset();

  /*#p31.XEPE*/ DFF9 XEPE_STORE0_X0p;
  /*_p31.YLAH*/ DFF9 YLAH_STORE0_X1p;
  /*_p31.ZOLA*/ DFF9 ZOLA_STORE0_X2p;
  /*_p31.ZULU*/ DFF9 ZULU_STORE0_X3p;
  /*_p31.WELO*/ DFF9 WELO_STORE0_X4p;
  /*_p31.XUNY*/ DFF9 XUNY_STORE0_X5p;
  /*_p31.WOTE*/ DFF9 WOTE_STORE0_X6p;
  /*_p31.XAKO*/ DFF9 XAKO_STORE0_X7p;
};

struct StoreI1 {
  void reset();

  /*_p30.CADU*/ DFF8 CADU_STORE1_I0p;
  /*_p30.CEBO*/ DFF8 CEBO_STORE1_I1p;
  /*_p30.CUFA*/ DFF8 CUFA_STORE1_I2p;
  /*_p30.COMA*/ DFF8 COMA_STORE1_I3p;
  /*_p30.CUZA*/ DFF8 CUZA_STORE1_I4p;
  /*_p30.CAJY*/ DFF8 CAJY_STORE1_I5p;
};

struct StoreL1 {
  void reset();

  /*_p30.AMES*/ DFF8 AMES_STORE1_L0p;
  /*_p30.AROF*/ DFF8 AROF_STORE1_L1p;
  /*_p30.ABOP*/ DFF8 ABOP_STORE1_L2p;
  /*_p30.ABUG*/ DFF8 ABUG_STORE1_L3p;
};

struct StoreX1 {
  void reset();

  /*_p31.DANY*/ DFF9 DANY_STORE1_X0p;
  /*_p31.DUKO*/ DFF9 DUKO_STORE1_X1p;
  /*_p31.DESU*/ DFF9 DESU_STORE1_X2p;
  /*_p31.DAZO*/ DFF9 DAZO_STORE1_X3p;
  /*_p31.DAKE*/ DFF9 DAKE_STORE1_X4p;
  /*_p31.CESO*/ DFF9 CESO_STORE1_X5p;
  /*_p31.DYFU*/ DFF9 DYFU_STORE1_X6p;
  /*_p31.CUSY*/ DFF9 CUSY_STORE1_X7p;
};

struct StoreI2 {
  void reset();

  /*_p30.BUHE*/ DFF8 BUHE_STORE2_I0p;
  /*_p30.BYHU*/ DFF8 BYHU_STORE2_I1p;
  /*_p30.BECA*/ DFF8 BECA_STORE2_I2p;
  /*_p30.BULU*/ DFF8 BULU_STORE2_I3p;
  /*_p30.BUNA*/ DFF8 BUNA_STORE2_I4p;
  /*_p30.BOXA*/ DFF8 BOXA_STORE2_I5p;
};

struct StoreL2 {
  void reset();

  /*_p30.YLOV*/ DFF8 YLOV_STORE2_L0p;
  /*_p30.XOSY*/ DFF8 XOSY_STORE2_L1p;
  /*_p30.XAZY*/ DFF8 XAZY_STORE2_L2p;
  /*_p30.YKUK*/ DFF8 YKUK_STORE2_L3p;
};

struct StoreX2 {
  void reset();

  /*_p31.FOKA*/ DFF9 FOKA_STORE2_X0p;
  /*_p31.FYTY*/ DFF9 FYTY_STORE2_X1p;
  /*_p31.FUBY*/ DFF9 FUBY_STORE2_X2p;
  /*_p31.GOXU*/ DFF9 GOXU_STORE2_X3p;
  /*_p31.DUHY*/ DFF9 DUHY_STORE2_X4p;
  /*_p31.EJUF*/ DFF9 EJUF_STORE2_X5p;
  /*_p31.ENOR*/ DFF9 ENOR_STORE2_X6p;
  /*_p31.DEPY*/ DFF9 DEPY_STORE2_X7p;
};

struct StoreI3 {
  void reset();

  /*_p30.DEVY*/ DFF8 DEVY_STORE3_I0p;
  /*_p30.DESE*/ DFF8 DESE_STORE3_I1p;
  /*_p30.DUNY*/ DFF8 DUNY_STORE3_I2p;
  /*_p30.DUHA*/ DFF8 DUHA_STORE3_I3p;
  /*_p30.DEBA*/ DFF8 DEBA_STORE3_I4p;
  /*_p30.DAFU*/ DFF8 DAFU_STORE3_I5p;
};

struct StoreL3 {
  void reset();

  /*_p30.ZURO*/ DFF8 ZURO_STORE3_L0p;
  /*_p30.ZYLU*/ DFF8 ZYLU_STORE3_L1p;
  /*_p30.ZENE*/ DFF8 ZENE_STORE3_L2p;
  /*_p30.ZURY*/ DFF8 ZURY_STORE3_L3p;
};

struct StoreX3 {
  void reset();

  /*_p31.XOLY*/ DFF9 XOLY_STORE3_X0p;
  /*_p31.XYBA*/ DFF9 XYBA_STORE3_X1p;
  /*_p31.XABE*/ DFF9 XABE_STORE3_X2p;
  /*_p31.XEKA*/ DFF9 XEKA_STORE3_X3p;
  /*_p31.XOMY*/ DFF9 XOMY_STORE3_X4p;
  /*_p31.WUHA*/ DFF9 WUHA_STORE3_X5p;
  /*_p31.WYNA*/ DFF9 WYNA_STORE3_X6p;
  /*_p31.WECO*/ DFF9 WECO_STORE3_X7p;
};

struct StoreI4 {
  void reset();

  /*_p30.XAVE*/ DFF8 XAVE_STORE4_I0p;
  /*_p30.XEFE*/ DFF8 XEFE_STORE4_I1p;
  /*_p30.WANU*/ DFF8 WANU_STORE4_I2p;
  /*_p30.XABO*/ DFF8 XABO_STORE4_I3p;
  /*_p30.XEGE*/ DFF8 XEGE_STORE4_I4p;
  /*_p30.XYNU*/ DFF8 XYNU_STORE4_I5p;
};

struct StoreL4 {
  void reset();

  /*_p30.CAPO*/ DFF8 CAPO_STORE4_L0p;
  /*_p30.CAJU*/ DFF8 CAJU_STORE4_L1p;
  /*_p30.CONO*/ DFF8 CONO_STORE4_L2p;
  /*_p30.CUMU*/ DFF8 CUMU_STORE4_L3p;
};

struct StoreX4 {
  void reset();

  /*_p31.WEDU*/ DFF9 WEDU_STORE4_X0p;
  /*_p31.YGAJ*/ DFF9 YGAJ_STORE4_X1p;
  /*_p31.ZYJO*/ DFF9 ZYJO_STORE4_X2p;
  /*_p31.XURY*/ DFF9 XURY_STORE4_X3p;
  /*_p31.YBED*/ DFF9 YBED_STORE4_X4p;
  /*_p31.ZALA*/ DFF9 ZALA_STORE4_X5p;
  /*_p31.WYDE*/ DFF9 WYDE_STORE4_X6p;
  /*_p31.XEPA*/ DFF9 XEPA_STORE4_X7p;
};

struct StoreI5 {
  void reset();

  /*_p30.EKOP*/ DFF8 EKOP_STORE5_I0p;
  /*_p30.ETYM*/ DFF8 ETYM_STORE5_I1p;
  /*_p30.GORU*/ DFF8 GORU_STORE5_I2p;
  /*_p30.EBEX*/ DFF8 EBEX_STORE5_I3p;
  /*_p30.ETAV*/ DFF8 ETAV_STORE5_I4p;
  /*_p30.EKAP*/ DFF8 EKAP_STORE5_I5p;
};


struct StoreL5 {
  void reset();

  /*_p30.ACEP*/ DFF8 ACEP_STORE5_L0p;
  /*_p30.ABEG*/ DFF8 ABEG_STORE5_L1p;
  /*_p30.ABUX*/ DFF8 ABUX_STORE5_L2p;
  /*_p30.ANED*/ DFF8 ANED_STORE5_L3p;
};

struct StoreX5 {
  void reset();

  /*_p31.FUSA*/ DFF9 FUSA_STORE5_X0p;
  /*_p31.FAXA*/ DFF9 FAXA_STORE5_X1p;
  /*_p31.FOZY*/ DFF9 FOZY_STORE5_X2p;
  /*_p31.FESY*/ DFF9 FESY_STORE5_X3p;
  /*_p31.CYWE*/ DFF9 CYWE_STORE5_X4p;
  /*_p31.DYBY*/ DFF9 DYBY_STORE5_X5p;
  /*_p31.DURY*/ DFF9 DURY_STORE5_X6p;
  /*_p31.CUVY*/ DFF9 CUVY_STORE5_X7p;
};

struct StoreI6 {
  void reset();

  /*_p30.GABO*/ DFF8 GABO_STORE6_I0p;
  /*_p30.GACY*/ DFF8 GACY_STORE6_I1p;
  /*_p30.FOGO*/ DFF8 FOGO_STORE6_I2p;
  /*_p30.GOHU*/ DFF8 GOHU_STORE6_I3p;
  /*_p30.FOXY*/ DFF8 FOXY_STORE6_I4p;
  /*_p30.GECU*/ DFF8 GECU_STORE6_I5p;
};

struct StoreL6 {
  void reset();

  /*_p30.ZUMY*/ DFF8 ZUMY_STORE6_L0p;
  /*_p30.ZAFU*/ DFF8 ZAFU_STORE6_L1p;
  /*_p30.ZEXO*/ DFF8 ZEXO_STORE6_L2p;
  /*_p30.ZUBE*/ DFF8 ZUBE_STORE6_L3p;
};

struct StoreX6 {
  void reset();

  /*_p31.YCOL*/ DFF9 YCOL_STORE6_X0p;
  /*_p31.YRAC*/ DFF9 YRAC_STORE6_X1p;
  /*_p31.YMEM*/ DFF9 YMEM_STORE6_X2p;
  /*_p31.YVAG*/ DFF9 YVAG_STORE6_X3p;
  /*_p31.ZOLY*/ DFF9 ZOLY_STORE6_X4p;
  /*_p31.ZOGO*/ DFF9 ZOGO_STORE6_X5p;
  /*_p31.ZECU*/ DFF9 ZECU_STORE6_X6p;
  /*_p31.ZESA*/ DFF9 ZESA_STORE6_X7p;
};

struct StoreI7 {
  void reset();

  /*_p30.GULE*/ DFF8 GULE_STORE7_I0p;
  /*_p30.GYNO*/ DFF8 GYNO_STORE7_I1p;
  /*_p30.FEFA*/ DFF8 FEFA_STORE7_I2p;
  /*_p30.FYSU*/ DFF8 FYSU_STORE7_I3p;
  /*_p30.GESY*/ DFF8 GESY_STORE7_I4p;
  /*_p30.FUZO*/ DFF8 FUZO_STORE7_I5p;
};

struct StoreL7 {
  void reset();

  /*_p30.XYNA*/ DFF8 XYNA_STORE7_L0p;
  /*_p30.YGUM*/ DFF8 YGUM_STORE7_L1p;
  /*_p30.XAKU*/ DFF8 XAKU_STORE7_L2p;
  /*_p30.XYGO*/ DFF8 XYGO_STORE7_L3p;
};

struct StoreX7 {
  void reset();

  /*_p31.ERAZ*/ DFF9 ERAZ_STORE7_X0p;
  /*_p31.EPUM*/ DFF9 EPUM_STORE7_X1p;
  /*_p31.EROL*/ DFF9 EROL_STORE7_X2p;
  /*_p31.EHYN*/ DFF9 EHYN_STORE7_X3p;
  /*_p31.FAZU*/ DFF9 FAZU_STORE7_X4p;
  /*_p31.FAXE*/ DFF9 FAXE_STORE7_X5p;
  /*_p31.EXUK*/ DFF9 EXUK_STORE7_X6p;
  /*_p31.FEDE*/ DFF9 FEDE_STORE7_X7p;
};

struct StoreI8 {
  void reset();

  /*_p30.AXUV*/ DFF8 AXUV_STORE8_I0p;
  /*_p30.BADA*/ DFF8 BADA_STORE8_I1p;
  /*_p30.APEV*/ DFF8 APEV_STORE8_I2p;
  /*_p30.BADO*/ DFF8 BADO_STORE8_I3p;
  /*_p30.BEXY*/ DFF8 BEXY_STORE8_I4p;
  /*_p30.BYHE*/ DFF8 BYHE_STORE8_I5p;
};

struct StoreL8 {
  void reset();

  /*_p30.AZAP*/ DFF8 AZAP_STORE8_L0p;
  /*_p30.AFYX*/ DFF8 AFYX_STORE8_L1p;
  /*_p30.AFUT*/ DFF8 AFUT_STORE8_L2p;
  /*_p30.AFYM*/ DFF8 AFYM_STORE8_L3p;
};

struct StoreX8 {
  void reset();

  /*_p31.EZUF*/ DFF9 EZUF_STORE8_X0p;
  /*_p31.ENAD*/ DFF9 ENAD_STORE8_X1p;
  /*_p31.EBOW*/ DFF9 EBOW_STORE8_X2p;
  /*_p31.FYCA*/ DFF9 FYCA_STORE8_X3p;
  /*_p31.GAVY*/ DFF9 GAVY_STORE8_X4p;
  /*_p31.GYPU*/ DFF9 GYPU_STORE8_X5p;
  /*_p31.GADY*/ DFF9 GADY_STORE8_X6p;
  /*_p31.GAZA*/ DFF9 GAZA_STORE8_X7p;
};

struct StoreI9 {
  void reset();

  /*_p30.YBER*/ DFF8 YBER_STORE9_I0p;
  /*_p30.YZOR*/ DFF8 YZOR_STORE9_I1p;
  /*_p30.XYFE*/ DFF8 XYFE_STORE9_I2p;
  /*_p30.XOTU*/ DFF8 XOTU_STORE9_I3p;
  /*_p30.XUTE*/ DFF8 XUTE_STORE9_I4p;
  /*_p30.XUFO*/ DFF8 XUFO_STORE9_I5p;
};

struct StoreL9 {
  void reset();

  /*_p30.CANA*/ DFF8 CANA_STORE9_L0p;
  /*_p30.FOFO*/ DFF8 FOFO_STORE9_L1p;
  /*_p30.DYSY*/ DFF8 DYSY_STORE9_L2p;
  /*_p30.DEWU*/ DFF8 DEWU_STORE9_L3p;
};

struct StoreX9 {
  void reset();

  /*_p31.XUVY*/ DFF9 XUVY_STORE9_X0p;
  /*_p31.XERE*/ DFF9 XERE_STORE9_X1p;
  /*_p31.XUZO*/ DFF9 XUZO_STORE9_X2p;
  /*_p31.XEXA*/ DFF9 XEXA_STORE9_X3p;
  /*_p31.YPOD*/ DFF9 YPOD_STORE9_X4p;
  /*_p31.YROP*/ DFF9 YROP_STORE9_X5p;
  /*_p31.YNEP*/ DFF9 YNEP_STORE9_X6p;
  /*_p31.YZOF*/ DFF9 YZOF_STORE9_X7p;
};

//-----------------------------------------------------------------------------
