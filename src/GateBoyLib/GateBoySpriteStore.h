#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------
// Tristate buses to move sprite index & line in/out of sprite store

struct SpriteIBus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_BUS_SPR_I0*/ Bus BUS_SPR_I0; // AxCxExGx
  /*_BUS_SPR_I1*/ Bus BUS_SPR_I1; // AxCxExGx
  /*_BUS_SPR_I2*/ Bus BUS_SPR_I2; // AxCxExGx
  /*_BUS_SPR_I3*/ Bus BUS_SPR_I3; // AxCxExGx
  /*_BUS_SPR_I4*/ Bus BUS_SPR_I4; // AxCxExGx
  /*_BUS_SPR_I5*/ Bus BUS_SPR_I5; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SpriteLBus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_BUS_SPR_L0*/ Bus BUS_SPR_L0; // AxCxExGx
  /*_BUS_SPR_L1*/ Bus BUS_SPR_L1; // AxCxExGx
  /*_BUS_SPR_L2*/ Bus BUS_SPR_L2; // AxCxExGx
  /*_BUS_SPR_L3*/ Bus BUS_SPR_L3; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SpriteMatchFlags {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

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
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

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

struct SpriteStoreFlags {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p29.DYHU*/ Gate DYHU_STORE0_CLKn;
  /*_p29.BUCO*/ Gate BUCO_STORE1_CLKn;
  /*_p29.GYFO*/ Gate GYFO_STORE2_CLKn;
  /*_p29.GUSA*/ Gate GUSA_STORE3_CLKn;
  /*_p29.DUKE*/ Gate DUKE_STORE4_CLKn;
  /*_p29.BEDE*/ Gate BEDE_STORE5_CLKn;
  /*_p29.WEKA*/ Gate WEKA_STORE6_CLKn;
  /*_p29.GYVO*/ Gate GYVO_STORE7_CLKn;
  /*_p29.BUKA*/ Gate BUKA_STORE8_CLKn;
  /*_p29.DECU*/ Gate DECU_STORE9_CLKn;
};

//-----------------------------------------------------------------------------
// All 10 of the sprite stores

// STORE_I - xBxxxFxx
// STORE_L - xBxxxFxx
// STORE_X - During scan, xxxDxxxH. During line, AxCxExGx

struct StoreI0 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.YGUS*/ DFF8n YGUS_STORE0_I0n;
  /*_p30.YSOK*/ DFF8n YSOK_STORE0_I1n;
  /*_p30.YZEP*/ DFF8n YZEP_STORE0_I2n;
  /*_p30.WYTE*/ DFF8n WYTE_STORE0_I3n;
  /*_p30.ZONY*/ DFF8n ZONY_STORE0_I4n;
  /*_p30.YWAK*/ DFF8n YWAK_STORE0_I5n;
};

struct StoreL0 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.GYHO*/ DFF8n GYHO_STORE0_L0n;
  /*_p30.CUFO*/ DFF8n CUFO_STORE0_L1n;
  /*_p30.BOZU*/ DFF8n BOZU_STORE0_L2n;
  /*_p30.FYHY*/ DFF8n FYHY_STORE0_L3n;
};

struct StoreX0 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p31.XEPE*/ DFF9B XEPE_STORE0_X0n;
  /*_p31.YLAH*/ DFF9B YLAH_STORE0_X1n;
  /*_p31.ZOLA*/ DFF9B ZOLA_STORE0_X2n;
  /*_p31.ZULU*/ DFF9B ZULU_STORE0_X3n;
  /*_p31.WELO*/ DFF9B WELO_STORE0_X4n;
  /*_p31.XUNY*/ DFF9B XUNY_STORE0_X5n;
  /*_p31.WOTE*/ DFF9B WOTE_STORE0_X6n;
  /*_p31.XAKO*/ DFF9B XAKO_STORE0_X7n;
};

struct StoreI1 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.CADU*/ DFF8n CADU_STORE1_I0n;
  /*_p30.CEBO*/ DFF8n CEBO_STORE1_I1n;
  /*_p30.CUFA*/ DFF8n CUFA_STORE1_I2n;
  /*_p30.COMA*/ DFF8n COMA_STORE1_I3n;
  /*_p30.CUZA*/ DFF8n CUZA_STORE1_I4n;
  /*_p30.CAJY*/ DFF8n CAJY_STORE1_I5n;
};

struct StoreL1 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.AMES*/ DFF8n AMES_STORE1_L0n;
  /*_p30.AROF*/ DFF8n AROF_STORE1_L1n;
  /*_p30.ABOP*/ DFF8n ABOP_STORE1_L2n;
  /*_p30.ABUG*/ DFF8n ABUG_STORE1_L3n;
};

struct StoreX1 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.DANY*/ DFF9B DANY_STORE1_X0n;
  /*_p31.DUKO*/ DFF9B DUKO_STORE1_X1n;
  /*_p31.DESU*/ DFF9B DESU_STORE1_X2n;
  /*_p31.DAZO*/ DFF9B DAZO_STORE1_X3n;
  /*_p31.DAKE*/ DFF9B DAKE_STORE1_X4n;
  /*_p31.CESO*/ DFF9B CESO_STORE1_X5n;
  /*_p31.DYFU*/ DFF9B DYFU_STORE1_X6n;
  /*_p31.CUSY*/ DFF9B CUSY_STORE1_X7n;
};

struct StoreI2 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.BUHE*/ DFF8n BUHE_STORE2_I0n;
  /*_p30.BYHU*/ DFF8n BYHU_STORE2_I1n;
  /*_p30.BECA*/ DFF8n BECA_STORE2_I2n;
  /*_p30.BULU*/ DFF8n BULU_STORE2_I3n;
  /*_p30.BUNA*/ DFF8n BUNA_STORE2_I4n;
  /*_p30.BOXA*/ DFF8n BOXA_STORE2_I5n;
};

struct StoreL2 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.YLOV*/ DFF8n YLOV_STORE2_L0n;
  /*_p30.XOSY*/ DFF8n XOSY_STORE2_L1n;
  /*_p30.XAZY*/ DFF8n XAZY_STORE2_L2n;
  /*_p30.YKUK*/ DFF8n YKUK_STORE2_L3n;
};

struct StoreX2 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.FOKA*/ DFF9B FOKA_STORE2_X0n;
  /*_p31.FYTY*/ DFF9B FYTY_STORE2_X1n;
  /*_p31.FUBY*/ DFF9B FUBY_STORE2_X2n;
  /*_p31.GOXU*/ DFF9B GOXU_STORE2_X3n;
  /*_p31.DUHY*/ DFF9B DUHY_STORE2_X4n;
  /*_p31.EJUF*/ DFF9B EJUF_STORE2_X5n;
  /*_p31.ENOR*/ DFF9B ENOR_STORE2_X6n;
  /*_p31.DEPY*/ DFF9B DEPY_STORE2_X7n;
};

struct StoreI3 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.DEVY*/ DFF8n DEVY_STORE3_I0n;
  /*_p30.DESE*/ DFF8n DESE_STORE3_I1n;
  /*_p30.DUNY*/ DFF8n DUNY_STORE3_I2n;
  /*_p30.DUHA*/ DFF8n DUHA_STORE3_I3n;
  /*_p30.DEBA*/ DFF8n DEBA_STORE3_I4n;
  /*_p30.DAFU*/ DFF8n DAFU_STORE3_I5n;
};

struct StoreL3 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.ZURO*/ DFF8n ZURO_STORE3_L0n;
  /*_p30.ZYLU*/ DFF8n ZYLU_STORE3_L1n;
  /*_p30.ZENE*/ DFF8n ZENE_STORE3_L2n;
  /*_p30.ZURY*/ DFF8n ZURY_STORE3_L3n;
};

struct StoreX3 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.XOLY*/ DFF9B XOLY_STORE3_X0n;
  /*_p31.XYBA*/ DFF9B XYBA_STORE3_X1n;
  /*_p31.XABE*/ DFF9B XABE_STORE3_X2n;
  /*_p31.XEKA*/ DFF9B XEKA_STORE3_X3n;
  /*_p31.XOMY*/ DFF9B XOMY_STORE3_X4n;
  /*_p31.WUHA*/ DFF9B WUHA_STORE3_X5n;
  /*_p31.WYNA*/ DFF9B WYNA_STORE3_X6n;
  /*_p31.WECO*/ DFF9B WECO_STORE3_X7n;
};

struct StoreI4 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.XAVE*/ DFF8n XAVE_STORE4_I0n;
  /*_p30.XEFE*/ DFF8n XEFE_STORE4_I1n;
  /*_p30.WANU*/ DFF8n WANU_STORE4_I2n;
  /*_p30.XABO*/ DFF8n XABO_STORE4_I3n;
  /*_p30.XEGE*/ DFF8n XEGE_STORE4_I4n;
  /*_p30.XYNU*/ DFF8n XYNU_STORE4_I5n;
};

struct StoreL4 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.CAPO*/ DFF8n CAPO_STORE4_L0n;
  /*_p30.CAJU*/ DFF8n CAJU_STORE4_L1n;
  /*_p30.CONO*/ DFF8n CONO_STORE4_L2n;
  /*_p30.CUMU*/ DFF8n CUMU_STORE4_L3n;
};

struct StoreX4 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.WEDU*/ DFF9B WEDU_STORE4_X0n;
  /*_p31.YGAJ*/ DFF9B YGAJ_STORE4_X1n;
  /*_p31.ZYJO*/ DFF9B ZYJO_STORE4_X2n;
  /*_p31.XURY*/ DFF9B XURY_STORE4_X3n;
  /*_p31.YBED*/ DFF9B YBED_STORE4_X4n;
  /*_p31.ZALA*/ DFF9B ZALA_STORE4_X5n;
  /*_p31.WYDE*/ DFF9B WYDE_STORE4_X6n;
  /*_p31.XEPA*/ DFF9B XEPA_STORE4_X7n;
};

struct StoreI5 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.EKOP*/ DFF8n EKOP_STORE5_I0n;
  /*_p30.ETYM*/ DFF8n ETYM_STORE5_I1n;
  /*_p30.GORU*/ DFF8n GORU_STORE5_I2n;
  /*_p30.EBEX*/ DFF8n EBEX_STORE5_I3n;
  /*_p30.ETAV*/ DFF8n ETAV_STORE5_I4n;
  /*_p30.EKAP*/ DFF8n EKAP_STORE5_I5n;
};


struct StoreL5 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.ACEP*/ DFF8n ACEP_STORE5_L0n;
  /*_p30.ABEG*/ DFF8n ABEG_STORE5_L1n;
  /*_p30.ABUX*/ DFF8n ABUX_STORE5_L2n;
  /*_p30.ANED*/ DFF8n ANED_STORE5_L3n;
};

struct StoreX5 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.FUSA*/ DFF9B FUSA_STORE5_X0n;
  /*_p31.FAXA*/ DFF9B FAXA_STORE5_X1n;
  /*_p31.FOZY*/ DFF9B FOZY_STORE5_X2n;
  /*_p31.FESY*/ DFF9B FESY_STORE5_X3n;
  /*_p31.CYWE*/ DFF9B CYWE_STORE5_X4n;
  /*_p31.DYBY*/ DFF9B DYBY_STORE5_X5n;
  /*_p31.DURY*/ DFF9B DURY_STORE5_X6n;
  /*_p31.CUVY*/ DFF9B CUVY_STORE5_X7n;
};

struct StoreI6 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.GABO*/ DFF8n GABO_STORE6_I0n;
  /*_p30.GACY*/ DFF8n GACY_STORE6_I1n;
  /*_p30.FOGO*/ DFF8n FOGO_STORE6_I2n;
  /*_p30.GOHU*/ DFF8n GOHU_STORE6_I3n;
  /*_p30.FOXY*/ DFF8n FOXY_STORE6_I4n;
  /*_p30.GECU*/ DFF8n GECU_STORE6_I5n;
};

struct StoreL6 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.ZUMY*/ DFF8n ZUMY_STORE6_L0n;
  /*_p30.ZAFU*/ DFF8n ZAFU_STORE6_L1n;
  /*_p30.ZEXO*/ DFF8n ZEXO_STORE6_L2n;
  /*_p30.ZUBE*/ DFF8n ZUBE_STORE6_L3n;
};

struct StoreX6 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.YCOL*/ DFF9B YCOL_STORE6_X0n;
  /*_p31.YRAC*/ DFF9B YRAC_STORE6_X1n;
  /*_p31.YMEM*/ DFF9B YMEM_STORE6_X2n;
  /*_p31.YVAG*/ DFF9B YVAG_STORE6_X3n;
  /*_p31.ZOLY*/ DFF9B ZOLY_STORE6_X4n;
  /*_p31.ZOGO*/ DFF9B ZOGO_STORE6_X5n;
  /*_p31.ZECU*/ DFF9B ZECU_STORE6_X6n;
  /*_p31.ZESA*/ DFF9B ZESA_STORE6_X7n;
};

struct StoreI7 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.GULE*/ DFF8n GULE_STORE7_I0n;
  /*_p30.GYNO*/ DFF8n GYNO_STORE7_I1n;
  /*_p30.FEFA*/ DFF8n FEFA_STORE7_I2n;
  /*_p30.FYSU*/ DFF8n FYSU_STORE7_I3n;
  /*_p30.GESY*/ DFF8n GESY_STORE7_I4n;
  /*_p30.FUZO*/ DFF8n FUZO_STORE7_I5n;
};

struct StoreL7 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.XYNA*/ DFF8n XYNA_STORE7_L0n;
  /*_p30.YGUM*/ DFF8n YGUM_STORE7_L1n;
  /*_p30.XAKU*/ DFF8n XAKU_STORE7_L2n;
  /*_p30.XYGO*/ DFF8n XYGO_STORE7_L3n;
};

struct StoreX7 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.ERAZ*/ DFF9B ERAZ_STORE7_X0n;
  /*_p31.EPUM*/ DFF9B EPUM_STORE7_X1n;
  /*_p31.EROL*/ DFF9B EROL_STORE7_X2n;
  /*_p31.EHYN*/ DFF9B EHYN_STORE7_X3n;
  /*_p31.FAZU*/ DFF9B FAZU_STORE7_X4n;
  /*_p31.FAXE*/ DFF9B FAXE_STORE7_X5n;
  /*_p31.EXUK*/ DFF9B EXUK_STORE7_X6n;
  /*_p31.FEDE*/ DFF9B FEDE_STORE7_X7n;
};

struct StoreI8 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.AXUV*/ DFF8n AXUV_STORE8_I0n;
  /*_p30.BADA*/ DFF8n BADA_STORE8_I1n;
  /*_p30.APEV*/ DFF8n APEV_STORE8_I2n;
  /*_p30.BADO*/ DFF8n BADO_STORE8_I3n;
  /*_p30.BEXY*/ DFF8n BEXY_STORE8_I4n;
  /*_p30.BYHE*/ DFF8n BYHE_STORE8_I5n;
};

struct StoreL8 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.AZAP*/ DFF8n AZAP_STORE8_L0n;
  /*_p30.AFYX*/ DFF8n AFYX_STORE8_L1n;
  /*_p30.AFUT*/ DFF8n AFUT_STORE8_L2n;
  /*_p30.AFYM*/ DFF8n AFYM_STORE8_L3n;
};

struct StoreX8 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.EZUF*/ DFF9B EZUF_STORE8_X0n;
  /*_p31.ENAD*/ DFF9B ENAD_STORE8_X1n;
  /*_p31.EBOW*/ DFF9B EBOW_STORE8_X2n;
  /*_p31.FYCA*/ DFF9B FYCA_STORE8_X3n;
  /*_p31.GAVY*/ DFF9B GAVY_STORE8_X4n;
  /*_p31.GYPU*/ DFF9B GYPU_STORE8_X5n;
  /*_p31.GADY*/ DFF9B GADY_STORE8_X6n;
  /*_p31.GAZA*/ DFF9B GAZA_STORE8_X7n;
};

struct StoreI9 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.YBER*/ DFF8n YBER_STORE9_I0n;
  /*_p30.YZOR*/ DFF8n YZOR_STORE9_I1n;
  /*_p30.XYFE*/ DFF8n XYFE_STORE9_I2n;
  /*_p30.XOTU*/ DFF8n XOTU_STORE9_I3n;
  /*_p30.XUTE*/ DFF8n XUTE_STORE9_I4n;
  /*_p30.XUFO*/ DFF8n XUFO_STORE9_I5n;
};

struct StoreL9 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p30.CANA*/ DFF8n CANA_STORE9_L0n;
  /*_p30.FOFO*/ DFF8n FOFO_STORE9_L1n;
  /*_p30.DYSY*/ DFF8n DYSY_STORE9_L2n;
  /*_p30.DEWU*/ DFF8n DEWU_STORE9_L3n;
};

struct StoreX9 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.XUVY*/ DFF9B XUVY_STORE9_X0n;
  /*_p31.XERE*/ DFF9B XERE_STORE9_X1n;
  /*_p31.XUZO*/ DFF9B XUZO_STORE9_X2n;
  /*_p31.XEXA*/ DFF9B XEXA_STORE9_X3n;
  /*_p31.YPOD*/ DFF9B YPOD_STORE9_X4n;
  /*_p31.YROP*/ DFF9B YROP_STORE9_X5n;
  /*_p31.YNEP*/ DFF9B YNEP_STORE9_X6n;
  /*_p31.YZOF*/ DFF9B YZOF_STORE9_X7n;
};

//-----------------------------------------------------------------------------
