#pragma once
#include "GateBoyLib/Gates.h"

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

  static SpriteDeltaY sub(const OamTempA& oam_temp_a, const RegLY& reg_ly);

  wire2 GESE_SCAN_MATCH_Yp(wire2 XYMO_LCDC_SPSIZEn);

  /* p29.DEGE*/ wire2 DEGE_SPRITE_DELTA0;
  /* p29.DABY*/ wire2 DABY_SPRITE_DELTA1;
  /* p29.DABU*/ wire2 DABU_SPRITE_DELTA2;
  /* p29.GYSA*/ wire2 GYSA_SPRITE_DELTA3;
  /* p29.GACE*/ wire2 GACE_SPRITE_DELTA4;
  /* p29.GUVU*/ wire2 GUVU_SPRITE_DELTA5;
  /* p29.GYDA*/ wire2 GYDA_SPRITE_DELTA6;
  /* p29.GEWY*/ wire2 GEWY_SPRITE_DELTA7;

  /* p29.GYKY*/ wire2 GYKY_YDIFF_S3;
  /* p29.WUHU*/ wire2 WUHU_YDIFF_C7;
};

//-----------------------------------------------------------------------------
// Pixel counter

struct PixCount {
  void reset_to_cart();
  void tock(wire2 TADY_LINE_RSTn, wire2 SACU_CLKPIPE_evn);
  wire2 XANO_PX167p_old() const;
  wire2 XANO_PX167p_new() const;

  int get_old() { return BitBase::pack_old(8, &XEHO_PX0p); }
  int get_new() { return BitBase::pack_new(8, &XEHO_PX0p); }

  /*p21.XEHO*/ DFF17 XEHO_PX0p; // AxCxExGx
  /*p21.SAVY*/ DFF17 SAVY_PX1p; // AxCxExGx
  /*p21.XODU*/ DFF17 XODU_PX2p; // AxCxExGx
  /*p21.XYDO*/ DFF17 XYDO_PX3p; // AxCxExGx
  /*p21.TUHU*/ DFF17 TUHU_PX4p; // AxCxExGx
  /*p21.TUKY*/ DFF17 TUKY_PX5p; // AxCxExGx
  /*p21.TAKO*/ DFF17 TAKO_PX6p; // AxCxExGx
  /*p21.SYBE*/ DFF17 SYBE_PX7p; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct BGScrollX {
  static BGScrollX add(PixCount& pix_count, RegSCX reg_scx);

  wire2 ATAD_TILE_X0S;
  wire2 BEHU_TILE_X1S;
  wire2 APYH_TILE_X2S;
  wire2 BABE_MAP_X0S;
  wire2 ABOD_MAP_X1S;
  wire2 BEWY_MAP_X2S;
  wire2 BYCA_MAP_X3S;
  wire2 ACUL_MAP_X4S;
};

//-----------------------------------------------------------------------------

struct BGScrollY {
  static BGScrollY add(RegLY& reg_ly, RegSCY& reg_scy);

  wire2 FAFO_TILE_Y0S;
  wire2 EMUX_TILE_Y1S;
  wire2 ECAB_TILE_Y2S;
  wire2 ETAM_MAP_Y0S;
  wire2 DOTO_MAP_Y1S;
  wire2 DABA_MAP_Y2S;
  wire2 EFYK_MAP_Y3S;
  wire2 EJOK_MAP_Y4S;
};

//-----------------------------------------------------------------------------

struct SpritePix {
  static SpritePix flip(Bus BUS_VRAM_Dp[8], wire2 TEXY_SFETCHINGp, wire2 BAXO_OAM_DB5p);

  wire2 PUTE_FLIP0p;
  wire2 PELO_FLIP1p;
  wire2 PONO_FLIP2p;
  wire2 POBE_FLIP3p;
  wire2 PACY_FLIP4p;
  wire2 PUGU_FLIP5p;
  wire2 PAWE_FLIP6p;
  wire2 PULY_FLIP7p;
};

//-----------------------------------------------------------------------------
// FF40 LCDC

struct RegLCDC {
  void reset_to_cart() {
    VYXE_LCDC_BGENn  .reset(1, 0);
    XYLO_LCDC_SPENn  .reset(1, 1);
    XYMO_LCDC_SPSIZEn.reset(1, 1);
    XAFO_LCDC_BGMAPn .reset(1, 1);
    WEXU_LCDC_BGTILEn.reset(1, 0);
    WYMO_LCDC_WINENn .reset(1, 1);
    WOKY_LCDC_WINMAPn.reset(1, 1);
    XONA_LCDC_LCDENn .reset(1, 0);
  }

  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn;   // xxxxxxxH
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn;   // xxxxxxxH
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn; // xxxxxxxH
  /*p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPn;  // xxxxxxxH
  /*p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEn; // xxxxxxxH
  /*p23.WYMO*/ DFF9 WYMO_LCDC_WINENn;  // xxxxxxxH
  /*p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPn; // xxxxxxxH
  /*p23.WOKY*/ DFF9 XONA_LCDC_LCDENn;  // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF41 - STAT

struct RegStat {

  void dump(Dumper& d) {
    d.dump_slice2n("FF41 STAT  : ", &ROXE_STAT_HBI_ENn, 4);
  }

  void reset_to_cart() {
    RUPO_LYC_MATCHn.reset(0);
  }

  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);
  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, const RegLYC& reg_lyc);
  void read(GateBoyCpuBus& cpu_bus, wire2 ACYL_SCANNINGp, wire2 XYMU_RENDERINGp, wire2 PARU_VBLANKp);

  /*p21.RUPO*/ NorLatch RUPO_LYC_MATCHn;       // xxCxxxxx

  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn; // xxxxxxxH
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn; // xxxxxxxH
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn; // xxxxxxxH
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF42 SCY

struct RegSCY {

  void read (GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n;          // xxxxxxxH
  /*p23.FYMO*/ DFF9 FYMO_SCY1n;          // xxxxxxxH
  /*p23.FEZU*/ DFF9 FEZU_SCY2n;          // xxxxxxxH
  /*p23.FUJO*/ DFF9 FUJO_SCY3n;          // xxxxxxxH
  /*p23.DEDE*/ DFF9 DEDE_SCY4n;          // xxxxxxxH
  /*p23.FOTY*/ DFF9 FOTY_SCY5n;          // xxxxxxxH
  /*p23.FOHA*/ DFF9 FOHA_SCY6n;          // xxxxxxxH
  /*p23.FUNY*/ DFF9 FUNY_SCY7n;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF43 SCX

struct RegSCX {

  void read (GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.DATY*/ DFF9 DATY_SCX0n;          // xxxxxxxH
  /*p23.DUZU*/ DFF9 DUZU_SCX1n;          // xxxxxxxH
  /*p23.CYXU*/ DFF9 CYXU_SCX2n;          // xxxxxxxH
  /*p23.GUBO*/ DFF9 GUBO_SCX3n;          // xxxxxxxH
  /*p23.BEMY*/ DFF9 BEMY_SCX4n;          // xxxxxxxH
  /*p23.CUZY*/ DFF9 CUZY_SCX5n;          // xxxxxxxH
  /*p23.CABU*/ DFF9 CABU_SCX6n;          // xxxxxxxH
  /*p23.BAKE*/ DFF9 BAKE_SCX7n;          // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF44 LY

struct RegLY {
  void reset_to_cart() {
    MYTA_y153p.reset(0, 1);
    MUWY_LY0p .reset(0, 0);
    MYRO_LY1p .reset(1, 0);
    LEXA_LY2p .reset(1, 0);
    LYDO_LY3p .reset(1, 0);
    LOVU_LY4p .reset(1, 0);
    LEMA_LY5p .reset(1, 0);
    MATO_LY6p .reset(1, 0);
    LAFO_LY7p .reset(1, 0);
  }

  uint8_t get_old() const  { return (uint8_t)BitBase::pack_old(8, &MUWY_LY0p); }
  uint8_t get_new() const  { return (uint8_t)BitBase::pack_new(8, &MUWY_LY0p); }

  void read(GateBoyCpuBus& cpu_bus);
  void tock2(GateBoyResetDebug& rst, RegLX& reg_lx);

  /*#p24.NERU*/ wire2 NERU_y000p() const {
    return nor8b(LAFO_LY7p.qp_new2(), LOVU_LY4p.qp_new2(), LYDO_LY3p.qp_new2(), MUWY_LY0p.qp_new2(),
                 MYRO_LY1p.qp_new2(), LEXA_LY2p.qp_new2(), LEMA_LY5p.qp_new2(), MATO_LY6p.qp_new2());
  }

  /*#p21.XYVO*/ wire2 XYVO_y144p_old() const { return and2(LOVU_LY4p.qp_old2(), LAFO_LY7p.qp_old2()); } // 128 + 16 = 144
  /*#p21.XYVO*/ wire2 XYVO_y144p_new() const { return and2(LOVU_LY4p.qp_new2(), LAFO_LY7p.qp_new2()); } // 128 + 16 = 144

  /*p21.MYTA*/ DFF17 MYTA_y153p; // xxCxxxxH

  /*p21.MUWY*/ DFF17 MUWY_LY0p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MYRO*/ DFF17 MYRO_LY1p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEXA*/ DFF17 LEXA_LY2p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LYDO*/ DFF17 LYDO_LY3p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LOVU*/ DFF17 LOVU_LY4p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEMA*/ DFF17 LEMA_LY5p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MATO*/ DFF17 MATO_LY6p;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LAFO*/ DFF17 LAFO_LY7p;  // xxCxxxGx Ticks on G, reset on C
};

//-----------------------------------------------------------------------------
// LCD x counter isn't exposed to CPU

struct RegLX {
  void reset_to_cart() {
    RUTU_x113p.reset(1, 0);
    NYPE_x113p.reset(0, 0);
    SAXO_LX0p .reset(0, 0);
    TYPO_LX1p .reset(1, 1);
    VYZO_LX2p .reset(0, 0);
    TELU_LX3p .reset(1, 0);
    SUDE_LX4p .reset(1, 0);
    TAHA_LX5p .reset(1, 1);
    TYRY_LX6p .reset(0, 1);
  }

  uint8_t get_old() const  { return (uint8_t)BitBase::pack_old(8, &SAXO_LX0p); }
  uint8_t get_new() const  { return (uint8_t)BitBase::pack_new(8, &SAXO_LX0p); }

  wire2 PURE_LINE_ENDn() const {
    /*#p21.PURE*/ wire2 _PURE_LINE_ENDn_new_evn = not1b(RUTU_x113p.qp_new2());
    return _PURE_LINE_ENDn_new_evn;
  }

  wire2 TEGY_STROBE() const;

  wire2 NYPE_x113p_new() const { return NYPE_x113p.qp_new2(); }
  wire2 NYPE_x113n_new() const { return NYPE_x113p.qn_new2(); }

  wire2 RUTU_x113p_old() const { return RUTU_x113p.qp_old2(); }
  wire2 RUTU_x113n_old() const { return RUTU_x113p.qn_old(); }

  wire2 RUTU_x113p_new() const { return RUTU_x113p.qp_new2(); }
  wire2 RUTU_x113n_new() const { return RUTU_x113p.qn_new2(); }

  /*#p24.LOFU*/ wire2 LOFU_x113n_old() const { return not1b(RUTU_x113p_old()); }
  /*#p24.LOFU*/ wire2 LOFU_x113n_new() const { return not1b(RUTU_x113p_new()); }

  void tock(GateBoyResetDebug& rst, GateBoyClock& clk);

  /*p21.RUTU*/ DFF17 RUTU_x113p;  // xxxxxxGx
  /*p21.NYPE*/ DFF17 NYPE_x113p;  // xxCxxxxx

  /*p21.SAXO*/ DFF17 SAXO_LX0p; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYPO*/ DFF17 TYPO_LX1p; // xxCxxxGx Ticks on C, reset on G
  /*p21.VYZO*/ DFF17 VYZO_LX2p; // xxCxxxGx Ticks on C, reset on G
  /*p21.TELU*/ DFF17 TELU_LX3p; // xxCxxxGx Ticks on C, reset on G
  /*p21.SUDE*/ DFF17 SUDE_LX4p; // xxCxxxGx Ticks on C, reset on G
  /*p21.TAHA*/ DFF17 TAHA_LX5p; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYRY*/ DFF17 TYRY_LX6p; // xxCxxxGx Ticks on C, reset on G
};

//-----------------------------------------------------------------------------
// FF45 LYC

struct RegLYC {
  void reset_to_cart() {
    ROPO_LY_MATCH_SYNCp.reset(0, 1);
    SYRY_LYC0n.reset(1, 1);
    VUCE_LYC1n.reset(1, 1);
    SEDY_LYC2n.reset(1, 1);
    SALO_LYC3n.reset(1, 1);
    SOTA_LYC4n.reset(1, 1);
    VAFA_LYC5n.reset(1, 1);
    VEVO_LYC6n.reset(1, 1);
    RAHA_LYC7n.reset(1, 1);
  }

  uint8_t get() const { return (uint8_t)BitBase::pack_oldn(8, &SYRY_LYC0n); }

  void read (GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);
  void tock2(GateBoyResetDebug& rst, GateBoyClock& clk, const RegLY& reg_ly);

  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp;   // xxCxxxxx

  /*p23.SYRY*/ DFF9 SYRY_LYC0n; // xxxxxxxH
  /*p23.VUCE*/ DFF9 VUCE_LYC1n; // xxxxxxxH
  /*p23.SEDY*/ DFF9 SEDY_LYC2n; // xxxxxxxH
  /*p23.SALO*/ DFF9 SALO_LYC3n; // xxxxxxxH
  /*p23.SOTA*/ DFF9 SOTA_LYC4n; // xxxxxxxH
  /*p23.VAFA*/ DFF9 VAFA_LYC5n; // xxxxxxxH
  /*p23.VEVO*/ DFF9 VEVO_LYC6n; // xxxxxxxH
  /*p23.RAHA*/ DFF9 RAHA_LYC7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF47 - BGP

struct RegBGP {
  void reset_to_cart() {
    PAVO_BGP_D0n.reset(1, 1);
    NUSY_BGP_D1n.reset(1, 1);
    PYLU_BGP_D2n.reset(1, 0);
    MAXY_BGP_D3n.reset(1, 0);
    MUKE_BGP_D4n.reset(1, 0);
    MORU_BGP_D5n.reset(1, 0);
    MOGY_BGP_D6n.reset(1, 0);
    MENA_BGP_D7n.reset(1, 0);
  }

  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyCpuBus& cpu_bus);

  /*p36.PAVO*/ DFF8p PAVO_BGP_D0n; // xxxxxxxH
  /*p36.NUSY*/ DFF8p NUSY_BGP_D1n; // xxxxxxxH
  /*p36.PYLU*/ DFF8p PYLU_BGP_D2n; // xxxxxxxH
  /*p36.MAXY*/ DFF8p MAXY_BGP_D3n; // xxxxxxxH
  /*p36.MUKE*/ DFF8p MUKE_BGP_D4n; // xxxxxxxH
  /*p36.MORU*/ DFF8p MORU_BGP_D5n; // xxxxxxxH
  /*p36.MOGY*/ DFF8p MOGY_BGP_D6n; // xxxxxxxH
  /*p36.MENA*/ DFF8p MENA_BGP_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF48 - OBP0

struct RegOBP0 {
  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyCpuBus& cpu_bus);

  /*p36.XUFU*/ DFF8p XUFU_OBP0_D0n; // xxxxxxxH
  /*p36.XUKY*/ DFF8p XUKY_OBP0_D1n; // xxxxxxxH
  /*p36.XOVA*/ DFF8p XOVA_OBP0_D2n; // xxxxxxxH
  /*p36.XALO*/ DFF8p XALO_OBP0_D3n; // xxxxxxxH
  /*p36.XERU*/ DFF8p XERU_OBP0_D4n; // xxxxxxxH
  /*p36.XYZE*/ DFF8p XYZE_OBP0_D5n; // xxxxxxxH
  /*p36.XUPO*/ DFF8p XUPO_OBP0_D6n; // xxxxxxxH
  /*p36.XANA*/ DFF8p XANA_OBP0_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF49 - OBP1

struct RegOBP1 {
  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyCpuBus& cpu_bus);

  /*p36.MOXY*/ DFF8p MOXY_OBP1_D0n; // xxxxxxxH
  /*p36.LAWO*/ DFF8p LAWO_OBP1_D1n; // xxxxxxxH
  /*p36.MOSA*/ DFF8p MOSA_OBP1_D2n; // xxxxxxxH
  /*p36.LOSE*/ DFF8p LOSE_OBP1_D3n; // xxxxxxxH
  /*p36.LUNE*/ DFF8p LUNE_OBP1_D4n; // xxxxxxxH
  /*p36.LUGU*/ DFF8p LUGU_OBP1_D5n; // xxxxxxxH
  /*p36.LEPU*/ DFF8p LEPU_OBP1_D6n; // xxxxxxxH
  /*p36.LUXO*/ DFF8p LUXO_OBP1_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4A - WY

struct RegWY {
  void set(uint8_t wy) {
    NESO_WY0n.reset(1, bit(~wy, 0));
    NYRO_WY1n.reset(1, bit(~wy, 1));
    NAGA_WY2n.reset(1, bit(~wy, 2));
    MELA_WY3n.reset(1, bit(~wy, 3));
    NULO_WY4n.reset(1, bit(~wy, 4));
    NENE_WY5n.reset(1, bit(~wy, 5));
    NUKA_WY6n.reset(1, bit(~wy, 6));
    NAFU_WY7n.reset(1, bit(~wy, 7));
  }

  int get() const { return BitBase::pack_oldn(8, &NESO_WY0n); }

  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.NESO*/ DFF9 NESO_WY0n; // xxxxxxxH
  /*p23.NYRO*/ DFF9 NYRO_WY1n; // xxxxxxxH
  /*p23.NAGA*/ DFF9 NAGA_WY2n; // xxxxxxxH
  /*p23.MELA*/ DFF9 MELA_WY3n; // xxxxxxxH
  /*p23.NULO*/ DFF9 NULO_WY4n; // xxxxxxxH
  /*p23.NENE*/ DFF9 NENE_WY5n; // xxxxxxxH
  /*p23.NUKA*/ DFF9 NUKA_WY6n; // xxxxxxxH
  /*p23.NAFU*/ DFF9 NAFU_WY7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4B - WX

struct RegWX {
  void set(uint8_t wx) {
    MYPA_WX0n.reset(1, bit(~wx, 0));
    NOFE_WX1n.reset(1, bit(~wx, 1));
    NOKE_WX2n.reset(1, bit(~wx, 2));
    MEBY_WX3n.reset(1, bit(~wx, 3));
    MYPU_WX4n.reset(1, bit(~wx, 4));
    MYCE_WX5n.reset(1, bit(~wx, 5));
    MUVO_WX6n.reset(1, bit(~wx, 6));
    NUKU_WX7n.reset(1, bit(~wx, 7));
  }

  int get() const { return BitBase::pack_oldn(8, &MYPA_WX0n); }

  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.MYPA*/ DFF9 MYPA_WX0n; // xxxxxxxH
  /*p23.NOFE*/ DFF9 NOFE_WX1n; // xxxxxxxH
  /*p23.NOKE*/ DFF9 NOKE_WX2n; // xxxxxxxH
  /*p23.MEBY*/ DFF9 MEBY_WX3n; // xxxxxxxH
  /*p23.MYPU*/ DFF9 MYPU_WX4n; // xxxxxxxH
  /*p23.MYCE*/ DFF9 MYCE_WX5n; // xxxxxxxH
  /*p23.MUVO*/ DFF9 MUVO_WX6n; // xxxxxxxH
  /*p23.NUKU*/ DFF9 NUKU_WX7n; // xxxxxxxH
};

//------------------------------------------------------------------------------------------------------------------------

struct OamLatchA {

  void reset_to_cart() {
    YDYV_OAM_LATCH_DA0n.reset(1);
    YCEB_OAM_LATCH_DA1n.reset(1);
    ZUCA_OAM_LATCH_DA2n.reset(1);
    WONE_OAM_LATCH_DA3n.reset(1);
    ZAXE_OAM_LATCH_DA4n.reset(1);
    XAFU_OAM_LATCH_DA5n.reset(1);
    YSES_OAM_LATCH_DA6n.reset(1);
    ZECA_OAM_LATCH_DA7n.reset(1);
  }

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n; // xBxDxFxx // Proooobably all odd clocks?
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n; // xBxDxFxx
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n; // xBxDxFxx
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n; // xBxDxFxx
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n; // xBxDxFxx
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n; // xBxDxFxx
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n; // xBxDxFxx
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n; // xBxDxFxx
};

//------------------------------------------------------------------------------------------------------------------------

struct OamLatchB {

  void reset_to_cart() {
    XYKY_OAM_LATCH_DB0n.reset(1);
    YRUM_OAM_LATCH_DB1n.reset(1);
    YSEX_OAM_LATCH_DB2n.reset(1);
    YVEL_OAM_LATCH_DB3n.reset(1);
    WYNO_OAM_LATCH_DB4n.reset(1);
    CYRA_OAM_LATCH_DB5n.reset(1);
    ZUVE_OAM_LATCH_DB6n.reset(1);
    ECED_OAM_LATCH_DB7n.reset(1);
  }

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n; // xBxxxFxx // Proooobably all odd clocks?
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n; // xBxxxFxx
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n; // xBxxxFxx
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n; // xBxxxFxx
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n; // xBxxxFxx
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n; // xBxxxFxx
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n; // xBxxxFxx
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n; // xBxxxFxx
};

//------------------------------------------------------------------------------------------------------------------------
// oam byte 0, byte 2

struct OamTempA {

  void reset_to_cart() {
    XUSO_OAM_DA0p.reset(1, 0);
    XEGU_OAM_DA1p.reset(1, 0);
    YJEX_OAM_DA2p.reset(1, 0);
    XYJU_OAM_DA3p.reset(1, 0);
    YBOG_OAM_DA4p.reset(1, 0);
    WYSO_OAM_DA5p.reset(1, 0);
    XOTE_OAM_DA6p.reset(1, 0);
    YZAB_OAM_DA7p.reset(1, 0);
  }

  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
};

//------------------------------------------------------------------------------------------------------------------------
// oam byte 1, byte 3

struct OamTempB {

  void reset_to_cart() {
    YLOR_OAM_DB0p.reset(1, 0);
    ZYTY_OAM_DB1p.reset(1, 0);
    ZYVE_OAM_DB2p.reset(1, 0);
    ZEZY_OAM_DB3p.reset(1, 0);
    GOMO_OAM_DB4p.reset(1, 0);
    BAXO_OAM_DB5p.reset(1, 0);
    YZOS_OAM_DB6p.reset(1, 0);
    DEPO_OAM_DB7p.reset(1, 0);
  }

  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // AxxxExxx - sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // AxxxExxx - sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // AxxxExxx - sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // AxxxExxx - sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // AxxxExxx - sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // AxxxExxx - sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // AxxxExxx - sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct TileTempA {

  void tock(Bus BUS_VRAM_Dp[8], wire2 LOMA_LATCH_TILE_DAn);

  void dump(Dumper& d) {
    d.dump_slice2n("Tile Temp A : ", &LEGU_TILE_DA0n, 8);
  }

  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n;   // xBxDxFxH
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n;   // xBxDxFxH
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n;   // xBxDxFxH
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n;   // xBxDxFxH
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n;   // xBxDxFxH
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n;   // xBxDxFxH
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n;   // xBxDxFxH
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct TileTempB {
  void tock(Bus BUS_VRAM_Dp[8], wire2 _LABU_LATCH_TILE_DBn);

  void dump(Dumper& d) {
    d.dump_slice2p("Tile Temp B : ", &RAWU_TILE_DB0p, 8);
  }

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p;   // xBxDxFxH
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p;   // xBxDxFxH
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p;   // xBxDxFxH
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p;   // xBxDxFxH
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p;   // xBxDxFxH
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p;   // xBxDxFxH
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p;   // xBxDxFxH
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct SpritePixA {
  void store_sprite_pix(SpritePix sprite, wire2 XADO_STORE_SPRITE_An);

  void dump(Dumper& d) {
    d.dump_slice2n("Sprite Temp A : ", &REWO_SPRITE_DA0n, 8);
  }

  /*p33.REWO*/ DFF8n REWO_SPRITE_DA0n; // xBxDxFxH
  /*p33.PEBA*/ DFF8n PEBA_SPRITE_DA1n; // xBxDxFxH
  /*p33.MOFO*/ DFF8n MOFO_SPRITE_DA2n; // xBxDxFxH
  /*p33.PUDU*/ DFF8n PUDU_SPRITE_DA3n; // xBxDxFxH
  /*p33.SAJA*/ DFF8n SAJA_SPRITE_DA4n; // xBxDxFxH
  /*p33.SUNY*/ DFF8n SUNY_SPRITE_DA5n; // xBxDxFxH
  /*p33.SEMO*/ DFF8n SEMO_SPRITE_DA6n; // xBxDxFxH
  /*p33.SEGA*/ DFF8n SEGA_SPRITE_DA7n; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct SpritePixB {
  void store_sprite_pix(SpritePix sprite, wire2 PUCO_STORE_SPRITE_Bn);

  void dump(Dumper& d) {
    d.dump_slice2n("Sprite Temp B : ", &PEFO_SPRITE_DB0n, 8);
  }


  /*p33.PEFO*/ DFF8n PEFO_SPRITE_DB0n; // xBxDxFxH
  /*p33.ROKA*/ DFF8n ROKA_SPRITE_DB1n; // xBxDxFxH
  /*p33.MYTU*/ DFF8n MYTU_SPRITE_DB2n; // xBxDxFxH
  /*p33.RAMU*/ DFF8n RAMU_SPRITE_DB3n; // xBxDxFxH
  /*p33.SELE*/ DFF8n SELE_SPRITE_DB4n; // xBxDxFxH
  /*p33.SUTO*/ DFF8n SUTO_SPRITE_DB5n; // xBxDxFxH
  /*p33.RAMA*/ DFF8n RAMA_SPRITE_DB6n; // xBxDxFxH
  /*p33.RYDU*/ DFF8n RYDU_SPRITE_DB7n; // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct WinMapX {
  void tock(const GateBoyResetDebug& rst, wire2 TEVO_FETCH_TRIGp, wire2 PORE_WIN_MODEp, wire2 WYMO_LCDC_WINENp, wire2 XAHY_LINE_RSTn);

  void dump(Dumper& d) {
    d.dump_slice2p("Win Map X : ", &WYKA_WIN_X3, 5);
  }

  // Current window map coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3;      // AxCxExGx
  /*p27.WODY*/ DFF17 WODY_WIN_X4;      // AxCxExGx
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5;      // AxCxExGx
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6;      // AxCxExGx
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7;      // AxCxExGx
};

//-----------------------------------------------------------------------------

struct WinLineY {
  void tock(const GateBoyResetDebug& rst, wire2 PORE_WIN_MODEp, wire2 PARU_VBLANKp);

  void dump(Dumper& d) {
    d.dump_slice2p("Win Line Y : ", &VYNO_WIN_Y0, 8);
  }

  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0;      // AxCxExGh probably, but not enough data.
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1;      // AxCxExGh probably, but not enough data.
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2;      // AxCxExGh probably, but not enough data.
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3;      // AxCxExGh probably, but not enough data.
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4;      // AxCxExGh probably, but not enough data.
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5;      // AxCxExGh probably, but not enough data.
  /*p27.TATE*/ DFF17 TATE_WIN_Y6;      // AxCxExGh probably, but not enough data.
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7;      // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------
