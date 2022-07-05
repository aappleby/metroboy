#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------
// Bottom-level registers that don't have dependencies on anything else.

// #define ADDR_LCDC        0xFF40
// #define ADDR_STAT        0xFF41
// #define ADDR_SCY         0xFF42
// #define ADDR_SCX         0xFF43
// #define ADDR_LY          0xFF44
// #define ADDR_LYC         0xFF45
// #define ADDR_DMA         0xFF46
// #define ADDR_BGP         0xFF47
// #define ADDR_OBP0        0xFF48
// #define ADDR_OBP1        0xFF49
// #define ADDR_WY          0xFF4A
// #define ADDR_WX          0xFF4B

//-----------------------------------------------------------------------------

struct SpriteDeltaY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p29.ERUC*/ Adder ERUC_YDIFF0;
  /*_p29.ENEF*/ Adder ENEF_YDIFF1;
  /*_p29.FECO*/ Adder FECO_YDIFF2;
  /*_p29.GYKY*/ Adder GYKY_YDIFF3;
  /*_p29.GOPU*/ Adder GOPU_YDIFF4;
  /*_p29.FUWA*/ Adder FUWA_YDIFF5;
  /*_p29.GOJU*/ Adder GOJU_YDIFF6;
  /*_p29.WUHU*/ Adder WUHU_YDIFF7;
};

//-----------------------------------------------------------------------------
// Pixel counter

struct PixCount {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p21.XEHO*/ DFF17 XEHO_PX0p_odd; // AxCxExGx
  /*_p21.SAVY*/ DFF17 SAVY_PX1p_odd; // AxCxExGx
  /*_p21.XODU*/ DFF17 XODU_PX2p_odd; // AxCxExGx
  /*_p21.XYDO*/ DFF17 XYDO_PX3p_odd; // AxCxExGx
  /*_p21.TUHU*/ DFF17 TUHU_PX4p_odd; // AxCxExGx
  /*_p21.TUKY*/ DFF17 TUKY_PX5p_odd; // AxCxExGx
  /*_p21.TAKO*/ DFF17 TAKO_PX6p_odd; // AxCxExGx
  /*_p21.SYBE*/ DFF17 SYBE_PX7p_odd; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct BGScrollX {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  Adder ATAD_TILE_X0;
  Adder BEHU_TILE_X1;
  Adder APYH_TILE_X2;
  Adder BABE_MAP_X0;
  Adder ABOD_MAP_X1;
  Adder BEWY_MAP_X2;
  Adder BYCA_MAP_X3;
  Adder ACUL_MAP_X4;
};

//-----------------------------------------------------------------------------

struct BGScrollY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  Adder FAFO_TILE_Y0;
  Adder EMUX_TILE_Y1;
  Adder ECAB_TILE_Y2;
  Adder ETAM_MAP_Y0;
  Adder DOTO_MAP_Y1;
  Adder DABA_MAP_Y2;
  Adder EFYK_MAP_Y3;
  Adder EJOK_MAP_Y4;
};

//-----------------------------------------------------------------------------

struct SpritePix {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  Gate PUTE_FLIP0p;
  Gate PELO_FLIP1p;
  Gate PONO_FLIP2p;
  Gate POBE_FLIP3p;
  Gate PACY_FLIP4p;
  Gate PUGU_FLIP5p;
  Gate PAWE_FLIP6p;
  Gate PULY_FLIP7p;
};

//-----------------------------------------------------------------------------
// FF40 LCDC

struct RegLCDC {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p23.VYXE*/ DFF9B VYXE_LCDC_BGENp;   // xxxxxxxH
  /*_p23.XYLO*/ DFF9B XYLO_LCDC_SPENp;   // xxxxxxxH
  /*_p23.XYMO*/ DFF9B XYMO_LCDC_SPSIZEp; // xxxxxxxH
  /*_p23.XAFO*/ DFF9B XAFO_LCDC_BGMAPp;  // xxxxxxxH
  /*_p23.WEXU*/ DFF9B WEXU_LCDC_BGTILEp; // xxxxxxxH
  /*_p23.WYMO*/ DFF9B WYMO_LCDC_WINENp;  // xxxxxxxH
  /*_p23.WOKY*/ DFF9B WOKY_LCDC_WINMAPp; // xxxxxxxH
  /*_p23.XONA*/ DFF9B XONA_LCDC_LCDENp;  // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF41 - STAT

struct RegStat {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p21.ROXE*/ DFF9B ROXE_STAT_HBI_ENp; // xxxxxxxH
  /*_p21.RUFO*/ DFF9B RUFO_STAT_VBI_ENp; // xxxxxxxH
  /*_p21.REFE*/ DFF9B REFE_STAT_OAI_ENp; // xxxxxxxH
  /*_p21.RUGU*/ DFF9B RUGU_STAT_LYI_ENp; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF42 SCY

struct RegSCY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p23.GAVE*/ DFF9B GAVE_SCY0p;          // xxxxxxxH
  /*_p23.FYMO*/ DFF9B FYMO_SCY1p;          // xxxxxxxH
  /*_p23.FEZU*/ DFF9B FEZU_SCY2p;          // xxxxxxxH
  /*_p23.FUJO*/ DFF9B FUJO_SCY3p;          // xxxxxxxH
  /*_p23.DEDE*/ DFF9B DEDE_SCY4p;          // xxxxxxxH
  /*_p23.FOTY*/ DFF9B FOTY_SCY5p;          // xxxxxxxH
  /*_p23.FOHA*/ DFF9B FOHA_SCY6p;          // xxxxxxxH
  /*_p23.FUNY*/ DFF9B FUNY_SCY7p;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF43 SCX

struct RegSCX {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p23.DATY*/ DFF9B DATY_SCX0p;          // xxxxxxxH
  /*_p23.DUZU*/ DFF9B DUZU_SCX1p;          // xxxxxxxH
  /*_p23.CYXU*/ DFF9B CYXU_SCX2p;          // xxxxxxxH
  /*_p23.GUBO*/ DFF9B GUBO_SCX3p;          // xxxxxxxH
  /*_p23.BEMY*/ DFF9B BEMY_SCX4p;          // xxxxxxxH
  /*_p23.CUZY*/ DFF9B CUZY_SCX5p;          // xxxxxxxH
  /*_p23.CABU*/ DFF9B CABU_SCX6p;          // xxxxxxxH
  /*_p23.BAKE*/ DFF9B BAKE_SCX7p;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF44 LY

struct RegLY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p21.MUWY*/ DFF17 MUWY_LY0p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.MYRO*/ DFF17 MYRO_LY1p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.LEXA*/ DFF17 LEXA_LY2p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.LYDO*/ DFF17 LYDO_LY3p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.LOVU*/ DFF17 LOVU_LY4p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.LEMA*/ DFF17 LEMA_LY5p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.MATO*/ DFF17 MATO_LY6p_odd;  // xxCxxxGx Ticks on G, reset on C
  /*#p21.LAFO*/ DFF17 LAFO_LY7p_odd;  // xxCxxxGx Ticks on G, reset on C
};

//-----------------------------------------------------------------------------
// LCD x counter isn't exposed to CPU

struct RegLX {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p21.SAXO*/ DFF17 SAXO_LX0p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.TYPO*/ DFF17 TYPO_LX1p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.VYZO*/ DFF17 VYZO_LX2p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.TELU*/ DFF17 TELU_LX3p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.SUDE*/ DFF17 SUDE_LX4p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.TAHA*/ DFF17 TAHA_LX5p_odd; // xxCxxxGx Ticks on C, reset on G
  /*#p21.TYRY*/ DFF17 TYRY_LX6p_odd; // xxCxxxGx Ticks on C, reset on G
};

//-----------------------------------------------------------------------------
// FF45 LYC

struct RegLYC {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();
  uint8_t get() const;

  /*_p23.SYRY*/ DFF9B SYRY_LYC0p; // xxxxxxxH
  /*_p23.VUCE*/ DFF9B VUCE_LYC1p; // xxxxxxxH
  /*_p23.SEDY*/ DFF9B SEDY_LYC2p; // xxxxxxxH
  /*_p23.SALO*/ DFF9B SALO_LYC3p; // xxxxxxxH
  /*_p23.SOTA*/ DFF9B SOTA_LYC4p; // xxxxxxxH
  /*_p23.VAFA*/ DFF9B VAFA_LYC5p; // xxxxxxxH
  /*_p23.VEVO*/ DFF9B VEVO_LYC6p; // xxxxxxxH
  /*_p23.RAHA*/ DFF9B RAHA_LYC7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF47 - BGP

struct RegBGP {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p36.PAVO*/ DFF8pB PAVO_BGP_D0n; // xxxxxxxH
  /*_p36.NUSY*/ DFF8pB NUSY_BGP_D1n; // xxxxxxxH
  /*_p36.PYLU*/ DFF8pB PYLU_BGP_D2n; // xxxxxxxH
  /*_p36.MAXY*/ DFF8pB MAXY_BGP_D3n; // xxxxxxxH
  /*_p36.MUKE*/ DFF8pB MUKE_BGP_D4n; // xxxxxxxH
  /*_p36.MORU*/ DFF8pB MORU_BGP_D5n; // xxxxxxxH
  /*_p36.MOGY*/ DFF8pB MOGY_BGP_D6n; // xxxxxxxH
  /*_p36.MENA*/ DFF8pB MENA_BGP_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF48 - OBP0

struct RegOBP0 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p36.XUFU*/ DFF8pB XUFU_OBP0_D0n; // xxxxxxxH
  /*_p36.XUKY*/ DFF8pB XUKY_OBP0_D1n; // xxxxxxxH
  /*_p36.XOVA*/ DFF8pB XOVA_OBP0_D2n; // xxxxxxxH
  /*_p36.XALO*/ DFF8pB XALO_OBP0_D3n; // xxxxxxxH
  /*_p36.XERU*/ DFF8pB XERU_OBP0_D4n; // xxxxxxxH
  /*_p36.XYZE*/ DFF8pB XYZE_OBP0_D5n; // xxxxxxxH
  /*_p36.XUPO*/ DFF8pB XUPO_OBP0_D6n; // xxxxxxxH
  /*_p36.XANA*/ DFF8pB XANA_OBP0_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF49 - OBP1

struct RegOBP1 {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p36.MOXY*/ DFF8pB MOXY_OBP1_D0n; // xxxxxxxH
  /*_p36.LAWO*/ DFF8pB LAWO_OBP1_D1n; // xxxxxxxH
  /*_p36.MOSA*/ DFF8pB MOSA_OBP1_D2n; // xxxxxxxH
  /*_p36.LOSE*/ DFF8pB LOSE_OBP1_D3n; // xxxxxxxH
  /*_p36.LUNE*/ DFF8pB LUNE_OBP1_D4n; // xxxxxxxH
  /*_p36.LUGU*/ DFF8pB LUGU_OBP1_D5n; // xxxxxxxH
  /*_p36.LEPU*/ DFF8pB LEPU_OBP1_D6n; // xxxxxxxH
  /*_p36.LUXO*/ DFF8pB LUXO_OBP1_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4A - WY

struct RegWY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  void set(uint8_t wy);
  int get() const;

  /*_p23.NESO*/ DFF9B NESO_WY0p; // xxxxxxxH
  /*_p23.NYRO*/ DFF9B NYRO_WY1p; // xxxxxxxH
  /*_p23.NAGA*/ DFF9B NAGA_WY2p; // xxxxxxxH
  /*_p23.MELA*/ DFF9B MELA_WY3p; // xxxxxxxH
  /*_p23.NULO*/ DFF9B NULO_WY4p; // xxxxxxxH
  /*_p23.NENE*/ DFF9B NENE_WY5p; // xxxxxxxH
  /*_p23.NUKA*/ DFF9B NUKA_WY6p; // xxxxxxxH
  /*_p23.NAFU*/ DFF9B NAFU_WY7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4B - WX

struct RegWX {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  void set(uint8_t wx);
  int get() const;

  /*_p23.MYPA*/ DFF9B MYPA_WX0p; // xxxxxxxH
  /*_p23.NOFE*/ DFF9B NOFE_WX1p; // xxxxxxxH
  /*_p23.NOKE*/ DFF9B NOKE_WX2p; // xxxxxxxH
  /*_p23.MEBY*/ DFF9B MEBY_WX3p; // xxxxxxxH
  /*_p23.MYPU*/ DFF9B MYPU_WX4p; // xxxxxxxH
  /*_p23.MYCE*/ DFF9B MYCE_WX5p; // xxxxxxxH
  /*_p23.MUVO*/ DFF9B MUVO_WX6p; // xxxxxxxH
  /*_p23.NUKU*/ DFF9B NUKU_WX7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct OamLatchA {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n; // xBxDxFxx // Proooobably all odd clocks?
  /*_p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n; // xBxDxFxx
  /*_p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n; // xBxDxFxx
  /*_p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n; // xBxDxFxx
  /*_p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n; // xBxDxFxx
  /*_p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n; // xBxDxFxx
  /*_p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n; // xBxDxFxx
  /*_p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n; // xBxDxFxx
};

//-----------------------------------------------------------------------------

struct OamLatchB {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n; // xBxxxFxx // Proooobably all odd clocks?
  /*_p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n; // xBxxxFxx
  /*_p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n; // xBxxxFxx
  /*_p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n; // xBxxxFxx
  /*_p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n; // xBxxxFxx
  /*_p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n; // xBxxxFxx
  /*_p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n; // xBxxxFxx
  /*_p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n; // xBxxxFxx
};

//-----------------------------------------------------------------------------
// oam byte 0, byte 2

struct OamTempA {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*_p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*_p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*_p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*_p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*_p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*_p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*_p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
};

//-----------------------------------------------------------------------------
// oam byte 1, byte 3

struct OamTempB {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // AxxxExxx - sprite x bit 0,
  /*_p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // AxxxExxx - sprite x bit 1,
  /*_p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // AxxxExxx - sprite x bit 2,
  /*_p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // AxxxExxx - sprite x bit 3,
  /*_p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // AxxxExxx - sprite x bit 4, sprite palette
  /*_p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // AxxxExxx - sprite x bit 5, sprite x flip
  /*_p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // AxxxExxx - sprite x bit 6, sprite y flip
  /*_p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct TileTempA {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p32.LEGU*/ DFF8pB LEGU_TILE_DA0n;   // xBxDxFxH
  /*_p32.NUDU*/ DFF8pB NUDU_TILE_DA1n;   // xBxDxFxH
  /*_p32.MUKU*/ DFF8pB MUKU_TILE_DA2n;   // xBxDxFxH
  /*_p32.LUZO*/ DFF8pB LUZO_TILE_DA3n;   // xBxDxFxH
  /*_p32.MEGU*/ DFF8pB MEGU_TILE_DA4n;   // xBxDxFxH
  /*_p32.MYJY*/ DFF8pB MYJY_TILE_DA5n;   // xBxDxFxH
  /*_p32.NASA*/ DFF8pB NASA_TILE_DA6n;   // xBxDxFxH
  /*_p32.NEFO*/ DFF8pB NEFO_TILE_DA7n;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct TileTempB {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p32.RAWU*/ DFF11 RAWU_TILE_DB0p;   // xBxDxFxH
  /*_p32.POZO*/ DFF11 POZO_TILE_DB1p;   // xBxDxFxH
  /*_p32.PYZO*/ DFF11 PYZO_TILE_DB2p;   // xBxDxFxH
  /*_p32.POXA*/ DFF11 POXA_TILE_DB3p;   // xBxDxFxH
  /*_p32.PULO*/ DFF11 PULO_TILE_DB4p;   // xBxDxFxH
  /*_p32.POJU*/ DFF11 POJU_TILE_DB5p;   // xBxDxFxH
  /*_p32.POWY*/ DFF11 POWY_TILE_DB6p;   // xBxDxFxH
  /*_p32.PYJU*/ DFF11 PYJU_TILE_DB7p;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct SpritePixA {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p33.REWO*/ DFF8n REWO_SPRITE_DA0n; // xBxDxFxH
  /*_p33.PEBA*/ DFF8n PEBA_SPRITE_DA1n; // xBxDxFxH
  /*_p33.MOFO*/ DFF8n MOFO_SPRITE_DA2n; // xBxDxFxH
  /*_p33.PUDU*/ DFF8n PUDU_SPRITE_DA3n; // xBxDxFxH
  /*_p33.SAJA*/ DFF8n SAJA_SPRITE_DA4n; // xBxDxFxH
  /*_p33.SUNY*/ DFF8n SUNY_SPRITE_DA5n; // xBxDxFxH
  /*_p33.SEMO*/ DFF8n SEMO_SPRITE_DA6n; // xBxDxFxH
  /*_p33.SEGA*/ DFF8n SEGA_SPRITE_DA7n; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct SpritePixB {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p33.PEFO*/ DFF8n PEFO_SPRITE_DB0n; // xBxDxFxH
  /*_p33.ROKA*/ DFF8n ROKA_SPRITE_DB1n; // xBxDxFxH
  /*_p33.MYTU*/ DFF8n MYTU_SPRITE_DB2n; // xBxDxFxH
  /*_p33.RAMU*/ DFF8n RAMU_SPRITE_DB3n; // xBxDxFxH
  /*_p33.SELE*/ DFF8n SELE_SPRITE_DB4n; // xBxDxFxH
  /*_p33.SUTO*/ DFF8n SUTO_SPRITE_DB5n; // xBxDxFxH
  /*_p33.RAMA*/ DFF8n RAMA_SPRITE_DB6n; // xBxDxFxH
  /*_p33.RYDU*/ DFF8n RYDU_SPRITE_DB7n; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct WinMapX {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p27.WYKA*/ DFF17 WYKA_WIN_MAP_X0;   // AxCxExGx
  /*_p27.WODY*/ DFF17 WODY_WIN_MAP_X1;   // AxCxExGx
  /*_p27.WOBO*/ DFF17 WOBO_WIN_MAP_X2;   // AxCxExGx
  /*_p27.WYKO*/ DFF17 WYKO_WIN_MAP_X3;   // AxCxExGx
  /*_p27.XOLO*/ DFF17 XOLO_WIN_MAP_X4;   // AxCxExGx
};

//-----------------------------------------------------------------------------

struct WinTileY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p27.VYNO*/ DFF17 VYNO_WIN_TILE_Y0;  // AxCxExGh probably, but not enough data.
  /*_p27.VUJO*/ DFF17 VUJO_WIN_TILE_Y1;  // AxCxExGh probably, but not enough data.
  /*_p27.VYMU*/ DFF17 VYMU_WIN_TILE_Y2;  // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------

struct WinMapY {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p27.TUFU*/ DFF17 TUFU_WIN_MAP_Y0;   // AxCxExGh probably, but not enough data.
  /*_p27.TAXA*/ DFF17 TAXA_WIN_MAP_Y1;   // AxCxExGh probably, but not enough data.
  /*_p27.TOZO*/ DFF17 TOZO_WIN_MAP_Y2;   // AxCxExGh probably, but not enough data.
  /*_p27.TATE*/ DFF17 TATE_WIN_MAP_Y3;   // AxCxExGh probably, but not enough data.
  /*_p27.TEKE*/ DFF17 TEKE_WIN_MAP_Y4;   // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------
