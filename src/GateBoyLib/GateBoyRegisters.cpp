#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

SpriteDeltaY GateBoy::sub_sprite_y_gates() {
  auto& reg_new = gb_state;

  /*#p29.EBOS*/ wire EBOS_LY0n = not1(reg_new.reg_ly.MUWY_LY0p_odd.qp_new());
  /*_p29.DASA*/ wire DASA_LY1n = not1(reg_new.reg_ly.MYRO_LY1p_odd.qp_new());
  /*_p29.FUKY*/ wire FUKY_LY2n = not1(reg_new.reg_ly.LEXA_LY2p_odd.qp_new());
  /*_p29.FUVE*/ wire FUVE_LY3n = not1(reg_new.reg_ly.LYDO_LY3p_odd.qp_new());
  /*_p29.FEPU*/ wire FEPU_LY4n = not1(reg_new.reg_ly.LOVU_LY4p_odd.qp_new());
  /*_p29.FOFA*/ wire FOFA_LY5n = not1(reg_new.reg_ly.LEMA_LY5p_odd.qp_new());
  /*_p29.FEMO*/ wire FEMO_LY6n = not1(reg_new.reg_ly.MATO_LY6p_odd.qp_new());
  /*_p29.GUSU*/ wire GUSU_LY7n = not1(reg_new.reg_ly.LAFO_LY7p_odd.qp_new());

  /*_p29.ERUC*/ Adder ERUC_YDIFF0 = add3(EBOS_LY0n, reg_new.oam_temp_a.XUSO_OAM_DA0p.qp_new(), reg_new.SIG_GND.out_new());
  /*_p29.ENEF*/ Adder ENEF_YDIFF1 = add3(DASA_LY1n, reg_new.oam_temp_a.XEGU_OAM_DA1p.qp_new(), ERUC_YDIFF0.carry);
  /*_p29.FECO*/ Adder FECO_YDIFF2 = add3(FUKY_LY2n, reg_new.oam_temp_a.YJEX_OAM_DA2p.qp_new(), ENEF_YDIFF1.carry);
  /*_p29.GYKY*/ Adder GYKY_YDIFF3 = add3(FUVE_LY3n, reg_new.oam_temp_a.XYJU_OAM_DA3p.qp_new(), FECO_YDIFF2.carry);
  /*_p29.GOPU*/ Adder GOPU_YDIFF4 = add3(FEPU_LY4n, reg_new.oam_temp_a.YBOG_OAM_DA4p.qp_new(), GYKY_YDIFF3.carry);
  /*_p29.FUWA*/ Adder FUWA_YDIFF5 = add3(FOFA_LY5n, reg_new.oam_temp_a.WYSO_OAM_DA5p.qp_new(), GOPU_YDIFF4.carry);
  /*_p29.GOJU*/ Adder GOJU_YDIFF6 = add3(FEMO_LY6n, reg_new.oam_temp_a.XOTE_OAM_DA6p.qp_new(), FUWA_YDIFF5.carry);
  /*_p29.WUHU*/ Adder WUHU_YDIFF7 = add3(GUSU_LY7n, reg_new.oam_temp_a.YZAB_OAM_DA7p.qp_new(), GOJU_YDIFF6.carry);

  return {
    ERUC_YDIFF0,
    ENEF_YDIFF1,
    FECO_YDIFF2,
    GYKY_YDIFF3,
    GOPU_YDIFF4,
    FUWA_YDIFF5,
    GOJU_YDIFF6,
    WUHU_YDIFF7,
  };
}

//-----------------------------------------------------------------------------

void GateBoy::tock_lcdc_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  /*_p23.WARU*/ wire WARU_FF40_WRp_new = and2(reg_new.cpu_signals.CUPA_CPU_WRp_new(), reg_new.cpu_abus.VOCA_FF40p_new());
  /*_p23.XUBO*/ wire XUBO_FF40_WRn_new = not1(WARU_FF40_WRp_new);
  /*#p23.VYXE*/ reg_new.reg_lcdc.VYXE_LCDC_BGENn  .dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p23.XYLO*/ reg_new.reg_lcdc.XYLO_LCDC_SPENn  .dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p23.XYMO*/ reg_new.reg_lcdc.XYMO_LCDC_SPSIZEn.dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p23.XAFO*/ reg_new.reg_lcdc.XAFO_LCDC_BGMAPn .dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p23.WEXU*/ reg_new.reg_lcdc.WEXU_LCDC_BGTILEn.dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p23.WYMO*/ reg_new.reg_lcdc.WYMO_LCDC_WINENn .dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p23.WOKY*/ reg_new.reg_lcdc.WOKY_LCDC_WINMAPn.dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p23.XONA*/ reg_new.reg_lcdc.XONA_LCDC_LCDENn. dff9(XUBO_FF40_WRn_new, reg_new.sys_rst.XARE_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p23.VYRE*/ wire VYRE_FF40_RDp_new = and2(reg_new.cpu_signals.ASOT_CPU_RDp_new(), reg_new.cpu_abus.VOCA_FF40p_new());
  /*_p23.WYCE*/ wire WYCE_FF40_RDn_new = not1(VYRE_FF40_RDp_new);
  /*#p23.WYPO*/ triwire WYPO_LCDC0_TO_CD0_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.VYXE_LCDC_BGENn.qp_new());
  /*#p23.XERO*/ triwire XERO_LCDC1_TO_CD1_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.XYLO_LCDC_SPENn.qp_new());
  /*_p23.WYJU*/ triwire WYJU_LCDC2_TO_CD2_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.XYMO_LCDC_SPSIZEn.qp_new());
  /*_p23.WUKA*/ triwire WUKA_LCDC3_TO_CD3_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.XAFO_LCDC_BGMAPn.qp_new());
  /*_p23.VOKE*/ triwire VOKE_LCDC4_TO_CD4_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.WEXU_LCDC_BGTILEn.qp_new());
  /*_p23.VATO*/ triwire VATO_LCDC5_TO_CD5_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.WYMO_LCDC_WINENn.qp_new());
  /*#p23.VAHA*/ triwire VAHA_LCDC6_TO_CD6_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.WOKY_LCDC_WINMAPn.qp_new());
  /*#p23.XEBU*/ triwire XEBU_LCDC7_TO_CD7_new = tri6_nn(WYCE_FF40_RDn_new, reg_new.reg_lcdc.XONA_LCDC_LCDENn.qp_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(WYPO_LCDC0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(XERO_LCDC1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(WYJU_LCDC2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(WUKA_LCDC3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(VOKE_LCDC4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(VATO_LCDC5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(VAHA_LCDC6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(XEBU_LCDC7_TO_CD7_new);
}

//-----------------------------------------------------------------------------

void SpriteDeltaY::reset_to_poweron() { memset(this, 0b00011010, sizeof(*this)); }
void SpriteDeltaY::reset_to_bootrom() { memset(this, 0b00011010, sizeof(*this)); }
void SpriteDeltaY::reset_to_cart()    { memset(this, 0b00011010, sizeof(*this)); }

//-----------------------------------------------------------------------------

void PixCount::reset_to_poweron() {
  memset(this, 0b00011010, sizeof(*this));
}

void PixCount::reset_to_bootrom() {
  memset(this, 0b00011010, sizeof(*this));
}

void PixCount::reset_to_cart() {
  XEHO_PX0p_odd.state = 0b00011011;
  SAVY_PX1p_odd.state = 0b00011011;
  XODU_PX2p_odd.state = 0b00011011;
  XYDO_PX3p_odd.state = 0b00011010;
  TUHU_PX4p_odd.state = 0b00011010;
  TUKY_PX5p_odd.state = 0b00011011;
  TAKO_PX6p_odd.state = 0b00011010;
  SYBE_PX7p_odd.state = 0b00011011;
}

//-----------------------------------------------------------------------------

void BGScrollX::reset_to_poweron() { memset(this, 0b00011010, sizeof(*this)); }
void BGScrollX::reset_to_bootrom() { memset(this, 0b00011010, sizeof(*this)); }
void BGScrollX::reset_to_cart()    { memset(this, 0b00011010, sizeof(*this)); }

//-----------------------------------------------------------------------------

void BGScrollY::reset_to_poweron() { memset(this, 0b00011010, sizeof(*this)); }
void BGScrollY::reset_to_bootrom() { memset(this, 0b00011010, sizeof(*this)); }
void BGScrollY::reset_to_cart()    { memset(this, 0b00011010, sizeof(*this)); }

//-----------------------------------------------------------------------------

void SpritePix::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0); }
void SpritePix::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0); }
void SpritePix::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0); }

//-----------------------------------------------------------------------------

void RegLCDC::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

void RegLCDC::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegLCDC::reset_to_cart() {
  VYXE_LCDC_BGENn.state   = 0b00011010;
  XYLO_LCDC_SPENn.state   = 0b00011011;
  XYMO_LCDC_SPSIZEn.state = 0b00011011;
  XAFO_LCDC_BGMAPn.state  = 0b00011011;
  WEXU_LCDC_BGTILEn.state = 0b00011010;
  WYMO_LCDC_WINENn.state  = 0b00011011;
  WOKY_LCDC_WINMAPn.state = 0b00011011;
  XONA_LCDC_LCDENn.state  = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegStat::reset_to_poweron() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegStat::reset_to_bootrom() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegStat::reset_to_cart()    { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }

//-----------------------------------------------------------------------------

void RegSCY::reset_to_poweron() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegSCY::reset_to_bootrom() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegSCY::reset_to_cart()    { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }

//-----------------------------------------------------------------------------

void RegSCX::reset_to_poweron() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegSCX::reset_to_bootrom() { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }
void RegSCX::reset_to_cart()    { memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this)); }

//-----------------------------------------------------------------------------

void RegLY::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

void RegLY::reset_to_bootrom() {
  MUWY_LY0p_odd.state = 0b00011000;
  MYRO_LY1p_odd.state = 0b00011010;
  LEXA_LY2p_odd.state = 0b00011010;
  LYDO_LY3p_odd.state = 0b00011010;
  LOVU_LY4p_odd.state = 0b00011010;
  LEMA_LY5p_odd.state = 0b00011010;
  MATO_LY6p_odd.state = 0b00011010;
  LAFO_LY7p_odd.state = 0b00011010;
}

void RegLY::reset_to_cart() {
  MUWY_LY0p_odd.state = 0b00011000;
  MYRO_LY1p_odd.state = 0b00011010;
  LEXA_LY2p_odd.state = 0b00011010;
  LYDO_LY3p_odd.state = 0b00011010;
  LOVU_LY4p_odd.state = 0b00011010;
  LEMA_LY5p_odd.state = 0b00011010;
  MATO_LY6p_odd.state = 0b00011010;
  LAFO_LY7p_odd.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegLX::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

void RegLX::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
  SAXO_LX0p_odd.state = BIT_OLD | BIT_DRIVEN;
}

void RegLX::reset_to_cart() {
  SAXO_LX0p_odd.state = 0b00011000;
  TYPO_LX1p_odd.state = 0b00011011;
  VYZO_LX2p_odd.state = 0b00011000;
  TELU_LX3p_odd.state = 0b00011010;
  SUDE_LX4p_odd.state = 0b00011010;
  TAHA_LX5p_odd.state = 0b00011011;
  TYRY_LX6p_odd.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void RegLYC::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF);
}

void RegLYC::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF);
}

void RegLYC::reset_to_cart() {
  SYRY_LYC0n.state = 0b00011011;
  VUCE_LYC1n.state = 0b00011011;
  SEDY_LYC2n.state = 0b00011011;
  SALO_LYC3n.state = 0b00011011;
  SOTA_LYC4n.state = 0b00011011;
  VAFA_LYC5n.state = 0b00011011;
  VEVO_LYC6n.state = 0b00011011;
  RAHA_LYC7n.state = 0b00011011;
}

uint8_t RegLYC::get() const {
  return (uint8_t)bit_pack_inv(*this);
}

//-----------------------------------------------------------------------------

void RegBGP::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegBGP::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegBGP::reset_to_cart() {
  PAVO_BGP_D0n.state = 0b00011011;
  NUSY_BGP_D1n.state = 0b00011011;
  PYLU_BGP_D2n.state = 0b00011010;
  MAXY_BGP_D3n.state = 0b00011010;
  MUKE_BGP_D4n.state = 0b00011010;
  MORU_BGP_D5n.state = 0b00011010;
  MOGY_BGP_D6n.state = 0b00011010;
  MENA_BGP_D7n.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegOBP0::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegOBP0::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegOBP0::reset_to_cart() {
  XUFU_OBP0_D0n.state = 0b00011010;
  XUKY_OBP0_D1n.state = 0b00011010;
  XOVA_OBP0_D2n.state = 0b00011010;
  XALO_OBP0_D3n.state = 0b00011010;
  XERU_OBP0_D4n.state = 0b00011010;
  XYZE_OBP0_D5n.state = 0b00011010;
  XUPO_OBP0_D6n.state = 0b00011010;
  XANA_OBP0_D7n.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegOBP1::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegOBP1::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00);
}

void RegOBP1::reset_to_cart() {
  MOXY_OBP1_D0n.state = 0b00011010;
  LAWO_OBP1_D1n.state = 0b00011010;
  MOSA_OBP1_D2n.state = 0b00011010;
  LOSE_OBP1_D3n.state = 0b00011010;
  LUNE_OBP1_D4n.state = 0b00011010;
  LUGU_OBP1_D5n.state = 0b00011010;
  LEPU_OBP1_D6n.state = 0b00011010;
  LUXO_OBP1_D7n.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegWY::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWY::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWY::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWY::set(uint8_t wy) {
  NESO_WY0n.state = BIT_CLOCK | bit(~wy, 0);
  NYRO_WY1n.state = BIT_CLOCK | bit(~wy, 1);
  NAGA_WY2n.state = BIT_CLOCK | bit(~wy, 2);
  MELA_WY3n.state = BIT_CLOCK | bit(~wy, 3);
  NULO_WY4n.state = BIT_CLOCK | bit(~wy, 4);
  NENE_WY5n.state = BIT_CLOCK | bit(~wy, 5);
  NUKA_WY6n.state = BIT_CLOCK | bit(~wy, 6);
  NAFU_WY7n.state = BIT_CLOCK | bit(~wy, 7);
}

int RegWY::get() const { return bit_pack_inv(*this); }

//-----------------------------------------------------------------------------

void RegWX::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWX::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWX::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void RegWX::set(uint8_t wx) {
  MYPA_WX0n.state = BIT_CLOCK | bit(~wx, 0);
  NOFE_WX1n.state = BIT_CLOCK | bit(~wx, 1);
  NOKE_WX2n.state = BIT_CLOCK | bit(~wx, 2);
  MEBY_WX3n.state = BIT_CLOCK | bit(~wx, 3);
  MYPU_WX4n.state = BIT_CLOCK | bit(~wx, 4);
  MYCE_WX5n.state = BIT_CLOCK | bit(~wx, 5);
  MUVO_WX6n.state = BIT_CLOCK | bit(~wx, 6);
  NUKU_WX7n.state = BIT_CLOCK | bit(~wx, 7);
}

int RegWX::get() const {
  return bit_pack_inv(*this);
}

//-----------------------------------------------------------------------------

void OamLatchA::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamLatchA::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamLatchA::reset_to_cart() {
  YDYV_OAM_LATCH_DA0n.state = 0b00011001;
  YCEB_OAM_LATCH_DA1n.state = 0b00011001;
  ZUCA_OAM_LATCH_DA2n.state = 0b00011001;
  WONE_OAM_LATCH_DA3n.state = 0b00011001;
  ZAXE_OAM_LATCH_DA4n.state = 0b00011001;
  XAFU_OAM_LATCH_DA5n.state = 0b00011001;
  YSES_OAM_LATCH_DA6n.state = 0b00011001;
  ZECA_OAM_LATCH_DA7n.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void OamLatchB::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamLatchB::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamLatchB::reset_to_cart() {
  XYKY_OAM_LATCH_DB0n.state = 0b00011001;
  YRUM_OAM_LATCH_DB1n.state = 0b00011001;
  YSEX_OAM_LATCH_DB2n.state = 0b00011001;
  YVEL_OAM_LATCH_DB3n.state = 0b00011001;
  WYNO_OAM_LATCH_DB4n.state = 0b00011001;
  CYRA_OAM_LATCH_DB5n.state = 0b00011001;
  ZUVE_OAM_LATCH_DB6n.state = 0b00011001;
  ECED_OAM_LATCH_DB7n.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void OamTempA::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamTempA::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void OamTempA::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

//-----------------------------------------------------------------------------

void OamTempB::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void OamTempB::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA, sizeof(*this));
}

void OamTempB::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

//-----------------------------------------------------------------------------

void TileTempA::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF); }
void TileTempA::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF); }
void TileTempA::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF); }

//-----------------------------------------------------------------------------

void TileTempB::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }
void TileTempB::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }
void TileTempB::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void SpritePixA::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
void SpritePixA::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
void SpritePixA::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }

//-----------------------------------------------------------------------------

void SpritePixB::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
void SpritePixB::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
void SpritePixB::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }

//-----------------------------------------------------------------------------

void WinMapX::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void WinMapX::reset_to_bootrom() {
  WYKA_WIN_MAP_X0.state = BIT_OLD | BIT_DRIVEN;
  WODY_WIN_MAP_X1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  WOBO_WIN_MAP_X2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  WYKO_WIN_MAP_X3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  XOLO_WIN_MAP_X4.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

void WinMapX::reset_to_cart() {
  WYKA_WIN_MAP_X0.state = BIT_OLD | BIT_DRIVEN;
  WODY_WIN_MAP_X1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  WOBO_WIN_MAP_X2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  WYKO_WIN_MAP_X3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  XOLO_WIN_MAP_X4.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

//-----------------------------------------------------------------------------

void WinTileY::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void WinTileY::reset_to_bootrom() {
  VYNO_WIN_TILE_Y0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  VUJO_WIN_TILE_Y1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  VYMU_WIN_TILE_Y2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

void WinTileY::reset_to_cart() {
  VYNO_WIN_TILE_Y0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  VUJO_WIN_TILE_Y1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  VYMU_WIN_TILE_Y2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

//-----------------------------------------------------------------------------

void WinMapY::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void WinMapY::reset_to_bootrom() {
  TUFU_WIN_MAP_Y0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TAXA_WIN_MAP_Y1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TOZO_WIN_MAP_Y2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TATE_WIN_MAP_Y3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TEKE_WIN_MAP_Y4.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

void WinMapY::reset_to_cart() {
  TUFU_WIN_MAP_Y0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TAXA_WIN_MAP_Y1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TOZO_WIN_MAP_Y2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TATE_WIN_MAP_Y3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  TEKE_WIN_MAP_Y4.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

//-----------------------------------------------------------------------------
