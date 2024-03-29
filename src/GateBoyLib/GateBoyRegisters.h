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
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

  /*#p23.VYXE*/ DFF9 VYXE_LCDC_BGENp;   // xxxxxxxH
  /*_p23.XYLO*/ DFF9 XYLO_LCDC_SPENp;   // xxxxxxxH
  /*_p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEp; // xxxxxxxH
  /*_p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPp;  // xxxxxxxH
  /*_p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEp; // xxxxxxxH
  /*_p23.WYMO*/ DFF9 WYMO_LCDC_WINENp;  // xxxxxxxH
  /*_p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPp; // xxxxxxxH
  /*_p23.XONA*/ DFF9 XONA_LCDC_LCDENp;  // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF41 - STAT

struct RegStat {
  void reset();

  /*_p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENp; // xxxxxxxH
  /*_p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENp; // xxxxxxxH
  /*_p21.REFE*/ DFF9 REFE_STAT_OAI_ENp; // xxxxxxxH
  /*_p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENp; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF42 SCY

struct RegSCY {
  void reset();

  /*_p23.GAVE*/ DFF9 GAVE_SCY0p;          // xxxxxxxH
  /*_p23.FYMO*/ DFF9 FYMO_SCY1p;          // xxxxxxxH
  /*_p23.FEZU*/ DFF9 FEZU_SCY2p;          // xxxxxxxH
  /*_p23.FUJO*/ DFF9 FUJO_SCY3p;          // xxxxxxxH
  /*_p23.DEDE*/ DFF9 DEDE_SCY4p;          // xxxxxxxH
  /*_p23.FOTY*/ DFF9 FOTY_SCY5p;          // xxxxxxxH
  /*_p23.FOHA*/ DFF9 FOHA_SCY6p;          // xxxxxxxH
  /*_p23.FUNY*/ DFF9 FUNY_SCY7p;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF43 SCX

struct RegSCX {
  void reset();

  /*_p23.DATY*/ DFF9 DATY_SCX0p;          // xxxxxxxH
  /*_p23.DUZU*/ DFF9 DUZU_SCX1p;          // xxxxxxxH
  /*_p23.CYXU*/ DFF9 CYXU_SCX2p;          // xxxxxxxH
  /*_p23.GUBO*/ DFF9 GUBO_SCX3p;          // xxxxxxxH
  /*_p23.BEMY*/ DFF9 BEMY_SCX4p;          // xxxxxxxH
  /*_p23.CUZY*/ DFF9 CUZY_SCX5p;          // xxxxxxxH
  /*_p23.CABU*/ DFF9 CABU_SCX6p;          // xxxxxxxH
  /*_p23.BAKE*/ DFF9 BAKE_SCX7p;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF44 LY

struct RegLY {
  void reset();

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
  void reset();

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
  void reset();
  uint8_t get() const;

  /*_p23.SYRY*/ DFF9 SYRY_LYC0p; // xxxxxxxH
  /*_p23.VUCE*/ DFF9 VUCE_LYC1p; // xxxxxxxH
  /*_p23.SEDY*/ DFF9 SEDY_LYC2p; // xxxxxxxH
  /*_p23.SALO*/ DFF9 SALO_LYC3p; // xxxxxxxH
  /*_p23.SOTA*/ DFF9 SOTA_LYC4p; // xxxxxxxH
  /*_p23.VAFA*/ DFF9 VAFA_LYC5p; // xxxxxxxH
  /*_p23.VEVO*/ DFF9 VEVO_LYC6p; // xxxxxxxH
  /*_p23.RAHA*/ DFF9 RAHA_LYC7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF47 - BGP

struct RegBGP {
  void reset();

  /*#p36.PAVO*/ DFF8 PAVO_BGP_D0p; // xxxxxxxH
  /*_p36.NUSY*/ DFF8 NUSY_BGP_D1p; // xxxxxxxH
  /*_p36.PYLU*/ DFF8 PYLU_BGP_D2p; // xxxxxxxH
  /*_p36.MAXY*/ DFF8 MAXY_BGP_D3p; // xxxxxxxH
  /*_p36.MUKE*/ DFF8 MUKE_BGP_D4p; // xxxxxxxH
  /*_p36.MORU*/ DFF8 MORU_BGP_D5p; // xxxxxxxH
  /*_p36.MOGY*/ DFF8 MOGY_BGP_D6p; // xxxxxxxH
  /*_p36.MENA*/ DFF8 MENA_BGP_D7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF48 - OBP0

struct RegOBP0 {
  void reset();

  /*_p36.XUFU*/ DFF8 XUFU_OBP0_D0p; // xxxxxxxH
  /*_p36.XUKY*/ DFF8 XUKY_OBP0_D1p; // xxxxxxxH
  /*_p36.XOVA*/ DFF8 XOVA_OBP0_D2p; // xxxxxxxH
  /*_p36.XALO*/ DFF8 XALO_OBP0_D3p; // xxxxxxxH
  /*_p36.XERU*/ DFF8 XERU_OBP0_D4p; // xxxxxxxH
  /*_p36.XYZE*/ DFF8 XYZE_OBP0_D5p; // xxxxxxxH
  /*_p36.XUPO*/ DFF8 XUPO_OBP0_D6p; // xxxxxxxH
  /*_p36.XANA*/ DFF8 XANA_OBP0_D7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF49 - OBP1

struct RegOBP1 {
  void reset();

  /*_p36.MOXY*/ DFF8 MOXY_OBP1_D0p; // xxxxxxxH
  /*_p36.LAWO*/ DFF8 LAWO_OBP1_D1p; // xxxxxxxH
  /*_p36.MOSA*/ DFF8 MOSA_OBP1_D2p; // xxxxxxxH
  /*_p36.LOSE*/ DFF8 LOSE_OBP1_D3p; // xxxxxxxH
  /*_p36.LUNE*/ DFF8 LUNE_OBP1_D4p; // xxxxxxxH
  /*_p36.LUGU*/ DFF8 LUGU_OBP1_D5p; // xxxxxxxH
  /*_p36.LEPU*/ DFF8 LEPU_OBP1_D6p; // xxxxxxxH
  /*_p36.LUXO*/ DFF8 LUXO_OBP1_D7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4A - WY

struct RegWY {
  void reset();

  void set(uint8_t wy);
  int get() const;

  /*_p23.NESO*/ DFF9 NESO_WY0p; // xxxxxxxH
  /*_p23.NYRO*/ DFF9 NYRO_WY1p; // xxxxxxxH
  /*_p23.NAGA*/ DFF9 NAGA_WY2p; // xxxxxxxH
  /*_p23.MELA*/ DFF9 MELA_WY3p; // xxxxxxxH
  /*_p23.NULO*/ DFF9 NULO_WY4p; // xxxxxxxH
  /*_p23.NENE*/ DFF9 NENE_WY5p; // xxxxxxxH
  /*_p23.NUKA*/ DFF9 NUKA_WY6p; // xxxxxxxH
  /*_p23.NAFU*/ DFF9 NAFU_WY7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4B - WX

struct RegWX {
  void reset();

  void set(uint8_t wx);
  int get() const;

  /*_p23.MYPA*/ DFF9 MYPA_WX0p; // xxxxxxxH
  /*_p23.NOFE*/ DFF9 NOFE_WX1p; // xxxxxxxH
  /*_p23.NOKE*/ DFF9 NOKE_WX2p; // xxxxxxxH
  /*_p23.MEBY*/ DFF9 MEBY_WX3p; // xxxxxxxH
  /*_p23.MYPU*/ DFF9 MYPU_WX4p; // xxxxxxxH
  /*_p23.MYCE*/ DFF9 MYCE_WX5p; // xxxxxxxH
  /*_p23.MUVO*/ DFF9 MUVO_WX6p; // xxxxxxxH
  /*_p23.NUKU*/ DFF9 NUKU_WX7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct OamLatchA {
  void reset();

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
  void reset();

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
  void reset();

  /*#p29.XUSO*/ DFF8 XUSO_OAM_DA0n; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*_p29.XEGU*/ DFF8 XEGU_OAM_DA1n; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*_p29.YJEX*/ DFF8 YJEX_OAM_DA2n; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*_p29.XYJU*/ DFF8 XYJU_OAM_DA3n; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*_p29.YBOG*/ DFF8 YBOG_OAM_DA4n; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*_p29.WYSO*/ DFF8 WYSO_OAM_DA5n; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*_p29.XOTE*/ DFF8 XOTE_OAM_DA6n; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*_p29.YZAB*/ DFF8 YZAB_OAM_DA7n; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
};

//-----------------------------------------------------------------------------
// oam byte 1, byte 3

struct OamTempB {
  void reset();

  /*_p31.YLOR*/ DFF8 YLOR_OAM_DB0n; // AxxxExxx - sprite x bit 0,
  /*_p31.ZYTY*/ DFF8 ZYTY_OAM_DB1n; // AxxxExxx - sprite x bit 1,
  /*_p31.ZYVE*/ DFF8 ZYVE_OAM_DB2n; // AxxxExxx - sprite x bit 2,
  /*_p31.ZEZY*/ DFF8 ZEZY_OAM_DB3n; // AxxxExxx - sprite x bit 3,
  /*_p31.GOMO*/ DFF8 GOMO_OAM_DB4n; // AxxxExxx - sprite x bit 4, sprite palette
  /*_p31.BAXO*/ DFF8 BAXO_OAM_DB5n; // AxxxExxx - sprite x bit 5, sprite x flip
  /*_p31.YZOS*/ DFF8 YZOS_OAM_DB6n; // AxxxExxx - sprite x bit 6, sprite y flip
  /*_p31.DEPO*/ DFF8 DEPO_OAM_DB7n; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct TileTempA {
  void reset();

  /*_p32.LEGU*/ DFF8 LEGU_TILE_DA0p;   // xBxDxFxH
  /*_p32.NUDU*/ DFF8 NUDU_TILE_DA1p;   // xBxDxFxH
  /*_p32.MUKU*/ DFF8 MUKU_TILE_DA2p;   // xBxDxFxH
  /*_p32.LUZO*/ DFF8 LUZO_TILE_DA3p;   // xBxDxFxH
  /*_p32.MEGU*/ DFF8 MEGU_TILE_DA4p;   // xBxDxFxH
  /*_p32.MYJY*/ DFF8 MYJY_TILE_DA5p;   // xBxDxFxH
  /*_p32.NASA*/ DFF8 NASA_TILE_DA6p;   // xBxDxFxH
  /*_p32.NEFO*/ DFF8 NEFO_TILE_DA7p;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct TileTempB {
  void reset();

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
  void reset();

  /*_p33.REWO*/ DFF8 REWO_SPRITE_DA0p; // xBxDxFxH
  /*_p33.PEBA*/ DFF8 PEBA_SPRITE_DA1p; // xBxDxFxH
  /*_p33.MOFO*/ DFF8 MOFO_SPRITE_DA2p; // xBxDxFxH
  /*_p33.PUDU*/ DFF8 PUDU_SPRITE_DA3p; // xBxDxFxH
  /*_p33.SAJA*/ DFF8 SAJA_SPRITE_DA4p; // xBxDxFxH
  /*_p33.SUNY*/ DFF8 SUNY_SPRITE_DA5p; // xBxDxFxH
  /*_p33.SEMO*/ DFF8 SEMO_SPRITE_DA6p; // xBxDxFxH
  /*_p33.SEGA*/ DFF8 SEGA_SPRITE_DA7p; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct SpritePixB {
  void reset();

  /*_p33.PEFO*/ DFF8 PEFO_SPRITE_DB0p; // xBxDxFxH
  /*_p33.ROKA*/ DFF8 ROKA_SPRITE_DB1p; // xBxDxFxH
  /*_p33.MYTU*/ DFF8 MYTU_SPRITE_DB2p; // xBxDxFxH
  /*_p33.RAMU*/ DFF8 RAMU_SPRITE_DB3p; // xBxDxFxH
  /*_p33.SELE*/ DFF8 SELE_SPRITE_DB4p; // xBxDxFxH
  /*_p33.SUTO*/ DFF8 SUTO_SPRITE_DB5p; // xBxDxFxH
  /*_p33.RAMA*/ DFF8 RAMA_SPRITE_DB6p; // xBxDxFxH
  /*_p33.RYDU*/ DFF8 RYDU_SPRITE_DB7p; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct WinMapX {
  void reset();

  /*_p27.WYKA*/ DFF17 WYKA_WIN_MAP_X0;   // AxCxExGx
  /*_p27.WODY*/ DFF17 WODY_WIN_MAP_X1;   // AxCxExGx
  /*_p27.WOBO*/ DFF17 WOBO_WIN_MAP_X2;   // AxCxExGx
  /*_p27.WYKO*/ DFF17 WYKO_WIN_MAP_X3;   // AxCxExGx
  /*_p27.XOLO*/ DFF17 XOLO_WIN_MAP_X4;   // AxCxExGx
};

//-----------------------------------------------------------------------------

struct WinTileY {
  void reset();

  /*_p27.VYNO*/ DFF17 VYNO_WIN_TILE_Y0;  // AxCxExGh probably, but not enough data.
  /*_p27.VUJO*/ DFF17 VUJO_WIN_TILE_Y1;  // AxCxExGh probably, but not enough data.
  /*_p27.VYMU*/ DFF17 VYMU_WIN_TILE_Y2;  // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------

struct WinMapY {
  void reset();

  /*_p27.TUFU*/ DFF17 TUFU_WIN_MAP_Y0;   // AxCxExGh probably, but not enough data.
  /*_p27.TAXA*/ DFF17 TAXA_WIN_MAP_Y1;   // AxCxExGh probably, but not enough data.
  /*_p27.TOZO*/ DFF17 TOZO_WIN_MAP_Y2;   // AxCxExGh probably, but not enough data.
  /*_p27.TATE*/ DFF17 TATE_WIN_MAP_Y3;   // AxCxExGh probably, but not enough data.
  /*_p27.TEKE*/ DFF17 TEKE_WIN_MAP_Y4;   // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------
