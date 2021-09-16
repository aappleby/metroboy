#include "GateBoyLib/GateBoyState.h"

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  SIG_VCC = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND = BIT_OLD | BIT_DRIVEN;
  reg_joy.reset_to_bootrom();
  //reg_sb;
  //reg_sc;
  reg_div.reset_to_poweron();
  reg_tima.reset_to_poweron();
  reg_tma.reset_to_poweron();
  reg_tac.reset_to_poweron();
  reg_if.reset_to_poweron();
  reg_lcdc.reset_to_poweron();
  reg_stat.reset_to_poweron();
  reg_scy.reset_to_poweron();
  reg_scx.reset_to_poweron();
  reg_ly.reset_to_poweron();
  reg_lyc.reset_to_poweron();
  reg_dma.reset_to_poweron();
  reg_bgp.reset_to_poweron();
  reg_obp0.reset_to_poweron();
  reg_obp1.reset_to_poweron();
  reg_wy.reset_to_poweron();
  reg_wx.reset_to_poweron();
  reg_lx.reset_to_poweron();
  reg_ie.reset_to_poweron();
  sys_rst.reset_to_poweron();
  sys_clk.reset_to_poweron();
  //VOGA_HBLANKp;
  //XYMU_RENDERINGn;
  //MATU_DMA_RUNNINGp;
  //ACYL_SCANNINGp;
  //WODU_HBLANKp;
  //SATO_BOOT_BITn;
  //ATEJ_LINE_RSTp;
  //FEPO_STORE_MATCHp;
  cpu_signals.reset_to_poweron();
  cpu_abus.reset_to_poweron();
  cpu_dbus.reset_to_poweron();
  int_ctrl.reset_to_poweron();
  int_latch.reset_to_poweron();
  vram_abus.lo.reset_to_poweron();
  vram_abus.hi.reset_to_poweron();
  vram_dbus.reset_to_poweron();
  vram_ext_ctrl.reset_to_poweron();
  vram_ext_abus.reset_to_poweron();
  vram_ext_dbus.reset_to_poweron();
  oam_ctrl.reset_to_poweron();
  oam_abus.reset_to_poweron();
  oam_dbus_a.reset_to_poweron();
  oam_dbus_b.reset_to_poweron();
  oam_latch_a.reset_to_poweron();
  oam_latch_b.reset_to_poweron();
  oam_temp_a.reset_to_poweron();
  oam_temp_b.reset_to_poweron();
  ext_ctrl.reset_to_poweron();
  ext_abus.lo.reset_to_poweron();
  ext_abus.hi.reset_to_poweron();
  ext_dbus.reset_to_poweron();
  ext_data_latch.reset_to_poweron();
  ext_addr_latch.reset_to_poweron();
  ext_mbc.reset_to_poweron();
  zram_bus.reset_to_poweron();
  dma_ctrl.reset_to_poweron();
  dma_lo.reset_to_poweron();
  cpu_int.reset_to_poweron();
  cpu_ack.reset_to_poweron();
  joy_int.reset_to_poweron();
  joy_latch.reset_to_poweron();
  joy_ext.reset_to_poweron();
  sprite_scanner.reset_to_poweron();
  scan_counter.reset_to_poweron();
  sprite_counter.reset_to_poweron();
  sprite_index.reset_to_poweron();
  sprite_match_flags.reset_to_poweron();
  sprite_reset_flags.reset_to_poweron();
  sprite_store_flags.reset_to_poweron();
  sprite_ibus.reset_to_poweron();
  sprite_lbus.reset_to_poweron();
  store_i0.reset_to_poweron();
  store_i1.reset_to_poweron();
  store_i2.reset_to_poweron();
  store_i3.reset_to_poweron();
  store_i4.reset_to_poweron();
  store_i5.reset_to_poweron();
  store_i6.reset_to_poweron();
  store_i7.reset_to_poweron();
  store_i8.reset_to_poweron();
  store_i9.reset_to_poweron();
  store_l0.reset_to_poweron();
  store_l1.reset_to_poweron();
  store_l2.reset_to_poweron();
  store_l3.reset_to_poweron();
  store_l4.reset_to_poweron();
  store_l5.reset_to_poweron();
  store_l6.reset_to_poweron();
  store_l7.reset_to_poweron();
  store_l8.reset_to_poweron();
  store_l9.reset_to_poweron();
  store_x0.reset_to_poweron();
  store_x1.reset_to_poweron();
  store_x2.reset_to_poweron();
  store_x3.reset_to_poweron();
  store_x4.reset_to_poweron();
  store_x5.reset_to_poweron();
  store_x6.reset_to_poweron();
  store_x7.reset_to_poweron();
  store_x8.reset_to_poweron();
  store_x9.reset_to_poweron();
  sfetch_counter.reset_to_poweron();
  sfetch_control.reset_to_poweron();
  tfetch_counter.reset_to_poweron();
  tfetch_control.reset_to_poweron();
  tile_temp_a.reset_to_poweron();
  tile_temp_b.reset_to_poweron();
  win_ctrl.reset_to_poweron();
  win_x.map.reset_to_poweron();
  win_y.tile.reset_to_poweron();
  win_y.map.reset_to_poweron();
  fine_count.reset_to_poweron();
  fine_scroll.reset_to_poweron();
  flipped_sprite.reset_to_poweron();
  sprite_pix_a.reset_to_poweron();
  sprite_pix_b.reset_to_poweron();
  pix_count.reset_to_poweron();
  mask_pipe.reset_to_poweron();
  bgw_pipe_a.reset_to_poweron();
  bgw_pipe_b.reset_to_poweron();
  spr_pipe_a.reset_to_poweron();
  spr_pipe_b.reset_to_poweron();
  pal_pipe.reset_to_poweron();
  lcd.reset_to_poweron();
  //reg_NR10.reset_to_poweron();
  //reg_NR11.reset_to_poweron();
  //reg_NR12.reset_to_poweron();
  //reg_NR14.reset_to_poweron();
  //reg_NR21.reset_to_poweron();
  //reg_NR22.reset_to_poweron();
  //reg_NR24.reset_to_poweron();
  //reg_NR30.reset_to_poweron();
  //reg_NR31.reset_to_poweron();
  //reg_NR32.reset_to_poweron();
  //reg_NR34.reset_to_poweron();
  //reg_NR41.reset_to_poweron();
  //reg_NR42.reset_to_poweron();
  //reg_NR43.reset_to_poweron();
  //reg_NR44.reset_to_poweron();
  //reg_NR50.reset_to_poweron();
  //reg_NR51.reset_to_poweron();
  //reg_NR52.reset_to_poweron();

  check_state_old_and_driven_or_pulled();
}

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  SIG_VCC = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND = BIT_OLD | BIT_DRIVEN;
  reg_joy.reset_to_bootrom();
  //reg_sb.reset_to_bootrom();
  //reg_sc.reset_to_bootrom();
  reg_div.reset_to_bootrom();
  reg_tima.reset_to_bootrom();
  reg_tma.reset_to_bootrom();
  reg_tac.reset_to_bootrom();
  reg_if.reset_to_bootrom();
  reg_lcdc.reset_to_bootrom();
  reg_stat.reset_to_bootrom();
  reg_scy.reset_to_bootrom();
  reg_scx.reset_to_bootrom();
  reg_ly.reset_to_bootrom();
  reg_lyc.reset_to_bootrom();
  reg_dma.reset_to_bootrom();
  reg_bgp.reset_to_bootrom();
  reg_obp0.reset_to_bootrom();
  reg_obp1.reset_to_bootrom();
  reg_wy.reset_to_bootrom();
  reg_wx.reset_to_bootrom();
  reg_lx.reset_to_bootrom();
  reg_ie.reset_to_bootrom();
  sys_rst.reset_to_bootrom();
  sys_clk.reset_to_bootrom();
  //VOGA_HBLANKp;
  //XYMU_RENDERINGn;
  //MATU_DMA_RUNNINGp;
  //ACYL_SCANNINGp;
  //WODU_HBLANKp;
  //SATO_BOOT_BITn;
  //ATEJ_LINE_RSTp;
  //FEPO_STORE_MATCHp;
  cpu_signals.reset_to_bootrom();
  cpu_abus.reset_to_bootrom();
  cpu_dbus.reset_to_bootrom();
  int_ctrl.reset_to_bootrom();
  int_latch.reset_to_bootrom();
  vram_abus.lo.reset_to_bootrom();
  vram_abus.hi.reset_to_bootrom();
  vram_dbus.reset_to_bootrom();
  vram_ext_ctrl.reset_to_bootrom();
  vram_ext_abus.reset_to_bootrom();
  vram_ext_dbus.reset_to_bootrom();
  oam_ctrl.reset_to_bootrom();
  oam_abus.reset_to_bootrom();
  oam_dbus_a.reset_to_bootrom();
  oam_dbus_b.reset_to_bootrom();
  oam_latch_a.reset_to_bootrom();
  oam_latch_b.reset_to_bootrom();
  oam_temp_a.reset_to_bootrom();
  oam_temp_b.reset_to_bootrom();
  ext_ctrl.reset_to_bootrom();
  ext_abus.lo.reset_to_bootrom();
  ext_abus.hi.reset_to_bootrom();
  ext_dbus.reset_to_bootrom();
  ext_data_latch.reset_to_bootrom();
  ext_addr_latch.reset_to_bootrom();
  ext_mbc.reset_to_bootrom();
  zram_bus.reset_to_bootrom();
  dma_ctrl.reset_to_bootrom();
  dma_lo.reset_to_bootrom();
  cpu_int.reset_to_bootrom();
  cpu_ack.reset_to_bootrom();
  joy_int.reset_to_bootrom();
  joy_latch.reset_to_bootrom();
  joy_ext.reset_to_bootrom();

  sprite_scanner;
  scan_counter;
  sprite_counter;
  sprite_index;
  sprite_match_flags;
  sprite_reset_flags;
  sprite_store_flags;
  sprite_ibus;
  sprite_lbus;
  store_i0;
  store_i1;
  store_i2;
  store_i3;
  store_i4;
  store_i5;
  store_i6;
  store_i7;
  store_i8;
  store_i9;
  store_l0;
  store_l1;
  store_l2;
  store_l3;
  store_l4;
  store_l5;
  store_l6;
  store_l7;
  store_l8;
  store_l9;
  store_x0;
  store_x1;
  store_x2;
  store_x3;
  store_x4;
  store_x5;
  store_x6;
  store_x7;
  store_x8;
  store_x9;
  sfetch_counter;
  sfetch_control;
  tfetch_counter;
  tfetch_control;
  tile_temp_a;
  tile_temp_b;
  win_ctrl;
  win_x.map;
  win_y.tile;
  win_y.map;
  fine_count;
  fine_scroll;
  flipped_sprite;
  sprite_pix_a;
  sprite_pix_b;
  pix_count;
  mask_pipe;
  bgw_pipe_a;
  bgw_pipe_b;
  spr_pipe_a;
  spr_pipe_b;
  pal_pipe;
  lcd;
  //NR10 reg_NR10;
  //NR11 reg_NR11;
  //NR12 reg_NR12;
  //NR14 reg_NR14;
  //NR21 reg_NR21;
  //NR22 reg_NR22;
  //NR24 reg_NR24;
  //NR30 reg_NR30;
  //NR31 reg_NR31;
  //NR32 reg_NR32;
  //NR34 reg_NR34;
  //NR41 reg_NR41;
  //NR42 reg_NR42;
  //NR43 reg_NR43;
  //NR44 reg_NR44;
  //NR50 reg_NR50;
  //NR51 reg_NR51;
  //NR52 reg_NR52;

  check_state_old_and_driven_or_pulled();
}

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_cart() {

  SIG_VCC = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND = BIT_OLD | BIT_DRIVEN;
  reg_joy.reset_to_cart();
  //reg_sb.reset_to_cart();
  //reg_sc.reset_to_cart();
  reg_div.reset_to_cart();
  reg_tima.reset_to_cart();
  reg_tma.reset_to_cart();
  reg_tac.reset_to_cart();
  reg_if.reset_to_cart();
  reg_lcdc.reset_to_cart();
  reg_stat.reset_to_cart();
  reg_scy.reset_to_cart();
  reg_scx.reset_to_cart();
  reg_ly.reset_to_cart();
  reg_lyc.reset_to_cart();
  reg_dma.reset_to_cart();
  reg_bgp.reset_to_cart();
  reg_obp0.reset_to_cart();
  reg_obp1.reset_to_cart();
  reg_wy.reset_to_cart();
  reg_wx.reset_to_cart();
  reg_lx.reset_to_cart();
  reg_ie.reset_to_cart();
  sys_rst.reset_to_cart();
  sys_clk.reset_to_cart();
  
  VOGA_HBLANKp.state = 0b00011001;
  //XYMU_RENDERINGn;
  //MATU_DMA_RUNNINGp;
  //ACYL_SCANNINGp;
  //WODU_HBLANKp;
  //SATO_BOOT_BITn;
  //ATEJ_LINE_RSTp;
  //FEPO_STORE_MATCHp;

  cpu_signals.reset_to_cart();
  cpu_abus.reset_to_cart();
  cpu_dbus.reset_to_cart();
  int_ctrl.reset_to_cart();
  int_latch.reset_to_cart();
  vram_abus.lo.reset_to_cart();
  vram_abus.hi.reset_to_cart();
  vram_dbus.reset_to_cart();
  vram_ext_ctrl.reset_to_cart();
  vram_ext_abus.reset_to_cart();
  vram_ext_dbus.reset_to_cart();
  oam_ctrl.reset_to_cart();
  oam_abus.reset_to_cart();
  oam_dbus_a.reset_to_cart();
  oam_dbus_b.reset_to_cart();
  oam_latch_a.reset_to_cart();
  oam_latch_b.reset_to_cart();
  oam_temp_a.reset_to_cart();
  oam_temp_b.reset_to_cart();
  ext_ctrl.reset_to_cart();
  ext_abus.lo.reset_to_cart();
  ext_abus.hi.reset_to_cart();
  ext_dbus.reset_to_cart();
  ext_data_latch.reset_to_cart();
  ext_addr_latch.reset_to_cart();
  ext_mbc.reset_to_cart();
  zram_bus.reset_to_cart();
  dma_ctrl.reset_to_cart();
  dma_lo.reset_to_cart();
  cpu_int.reset_to_cart();
  cpu_ack.reset_to_cart();
  joy_int.reset_to_cart();
  joy_latch.reset_to_cart();
  joy_ext.reset_to_cart();

#if 0
  cpu_signals.reset_to_cart();
  cpu_abus.reset_to_cart();
  cpu_dbus.reset_to_cart();

  vram_abus.lo.reset_to_cart();
  vram_abus.hi.reset_to_cart();
  vram_dbus.reset_to_cart();
  vram_ext_ctrl.reset_to_cart();
  vram_ext_abus.reset_to_cart();
  vram_ext_dbus.reset_to_cart();

  sprite_ibus.reset_to_cart();
  sprite_lbus.reset_to_cart();

  oam_ctrl.reset_to_cart();
  oam_abus.reset_to_cart();
  oam_dbus_a.reset_to_cart();
  oam_dbus_b.reset_to_cart();
  oam_latch_a.reset_to_cart();
  oam_latch_b.reset_to_cart();
  oam_temp_a.reset_to_cart();
  oam_temp_b.reset_to_cart();

  ext_ctrl.reset_to_cart();
  ext_abus.lo.reset_to_cart();
  ext_abus.hi.reset_to_cart();
  ext_dbus.reset_to_cart();
  ext_addr_latch.reset_to_cart();
  ext_data_latch.reset_to_cart();

  sys_rst.reset_to_cart();
  sys_clk.reset_to_cart();
  reg_div.reset_to_cart();
  
  reg_if.reset_to_cart();
  reg_ie.reset_to_cart();
  int_latch.reset_to_cart();
  cpu_int.reset_to_cart();
  cpu_ack.reset_to_cart();

  //serial.reset_to_cart();

  //reset_sprite_store();
  sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

  sprite_scanner.reset_to_cart();
  scan_counter.reset_to_cart();
  sprite_index.reset_to_cart();

  sfetch_counter.reset_to_cart();
  sfetch_control.reset_to_cart();

  int_ctrl.RUPO_LYC_MATCHn.state = 0b00011000;

  reg_stat.reset_to_cart();
  pix_count.reset_to_cart();
  mask_pipe.reset_to_cart();
  lcd.REMY_LD0n.state = 0b00011000;
  lcd.RAVO_LD1n.state = 0b00011000;

  dma_lo.reset_to_cart();
  reg_dma.reset_to_cart();
  MATU_DMA_RUNNINGp.state = 0b00011010;
  dma_ctrl.reset_to_cart();

  reg_bgp.reset_to_cart();
  reg_obp0.reset_to_cart();
  reg_obp1.reset_to_cart();
  
  //joy.reset_to_cart();
  joy_int.reset_to_cart();
  reg_joy.reset_to_cart();

  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  reg_lx.reset_to_cart();
  reg_ly.reset_to_cart();

  int_ctrl.ROPO_LY_MATCH_SYNCp.state = 0b00011001;
  reg_lyc.reset_to_cart();

  WODU_HBLANKp.state = 0b00011001;

  sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  ATEJ_LINE_RSTp.state = 0b00011000;

  //reg_NR50.reset_to_cart();
  //reg_NR51.reset_to_cart();
  //reg_NR52.reset_to_cart();

#endif

  check_state_old_and_driven_or_pulled();
}

//-----------------------------------------------------------------------------

int64_t GateBoyState::hash_regression() {
  return hash_low_bit(this, sizeof(GateBoyState), HASH_INIT);
}

int64_t GateBoyState::hash_all() {
  return hash_all_bits(this, sizeof(GateBoyState), HASH_INIT);
}

//-----------------------------------------------------------------------------

Result<uint8_t, Error> GateBoyState::peek(const blob& cart_blob, int addr) const {
  switch(addr) {
  case ADDR_P1  : break; 
  case ADDR_SB  : break;
  case ADDR_SC  : break;
  case ADDR_DIV : return (uint8_t)bit_pack(reg_div) >> 6;
  case ADDR_TIMA: return (uint8_t)bit_pack(reg_tima);
  case ADDR_TMA : return (uint8_t)bit_pack(reg_tma);
  case ADDR_TAC : return (uint8_t)bit_pack(reg_tac);
  case ADDR_IF  : break;
  case ADDR_NR10: break;
  case ADDR_NR11: break;
  case ADDR_NR12: break;
  case ADDR_NR14: break;
  case ADDR_NR21: break;
  case ADDR_NR22: break;
  case ADDR_NR24: break;
  case ADDR_NR30: break;
  case ADDR_NR31: break;
  case ADDR_NR32: break;
  case ADDR_NR34: break;
  case ADDR_NR41: break;
  case ADDR_NR42: break;
  case ADDR_NR43: break;
  case ADDR_NR44: break;
  case ADDR_NR50: break;
  case ADDR_NR51: break;
  case ADDR_NR52: break;
  case ADDR_LCDC: return (uint8_t)bit_pack_inv(reg_lcdc);
  case ADDR_STAT: break;
  case ADDR_SCY : return (uint8_t)bit_pack_inv(reg_scy);
  case ADDR_SCX : return (uint8_t)bit_pack_inv(reg_scx);
  case ADDR_LY  : return (uint8_t)bit_pack_inv(reg_ly);
  case ADDR_LYC : return (uint8_t)bit_pack_inv(reg_lyc);
  case ADDR_DMA : return (uint8_t)bit_pack_inv(reg_dma);
  case ADDR_BGP : return (uint8_t)bit_pack_inv(reg_bgp);
  case ADDR_OBP0: return (uint8_t)bit_pack_inv(reg_obp0);
  case ADDR_OBP1: return (uint8_t)bit_pack_inv(reg_obp1);
  case ADDR_WY  : return (uint8_t)bit_pack_inv(reg_wy);
  case ADDR_WX  : return (uint8_t)bit_pack_inv(reg_wx);
  }

  LOG_R("GateBoy::peek - bad address 0x%04x\n", addr);
  return Error::NOT_FOUND;
}

//-----------------------------------------------------------------------------

Result<uint8_t, Error> GateBoyState::poke(blob& cart_blob, int addr, uint8_t data_in) {
  switch(addr) {
  case ADDR_P1  : break; 
  case ADDR_SB  : break;
  case ADDR_SC  : break;
  case ADDR_DIV : break;
  case ADDR_TIMA: break;
  case ADDR_TMA : break;
  case ADDR_TAC : break;
  case ADDR_IF  : break;
  case ADDR_NR10: break;
  case ADDR_NR11: break;
  case ADDR_NR12: break;
  case ADDR_NR14: break;
  case ADDR_NR21: break;
  case ADDR_NR22: break;
  case ADDR_NR24: break;
  case ADDR_NR30: break;
  case ADDR_NR31: break;
  case ADDR_NR32: break;
  case ADDR_NR34: break;
  case ADDR_NR41: break;
  case ADDR_NR42: break;
  case ADDR_NR43: break;
  case ADDR_NR44: break;
  case ADDR_NR50: break;
  case ADDR_NR51: break;
  case ADDR_NR52: break;
  case ADDR_LCDC: { bit_unpack_inv(reg_lcdc, data_in); return data_in; }
  case ADDR_STAT: break;
  case ADDR_SCY : break;
  case ADDR_SCX : break;
  case ADDR_LY  : break;
  case ADDR_LYC : break;
  case ADDR_DMA : break;
  case ADDR_BGP : break;
  case ADDR_OBP0: break;
  case ADDR_OBP1: break;
  case ADDR_WY  : break;
  case ADDR_WX  : break;
  }

  LOG_R("GateBoy::poke - bad address 0x%04x\n", addr);
  return Error::NOT_FOUND;
}

//-----------------------------------------------------------------------------

void GateBoyState::commit() {
  if (!config_drive_flags && !config_oldnew_flags) return;

  uint8_t* base = (uint8_t*)this;
  bool bad_bits = false;
  for (size_t i = 0; i < sizeof(GateBoyState); i++) {
    uint8_t s = base[i];

    if (config_drive_flags) {
      if (bool(s & BIT_DRIVEN) && bool(s & BIT_PULLED)) {
        LOG_Y("Bit %d both driven and pulled up!\n", i);
        bad_bits = true;
      }

      if (!bool(s & BIT_DRIVEN) && !bool(s & BIT_PULLED)) {
        LOG_Y("Bit %d floating!\n", i);
        bad_bits = true;
      }
    }

    if (config_oldnew_flags) {
      if ((s & (BIT_OLD | BIT_NEW)) != BIT_NEW) {
        LOG_Y("Bit %d not dirty after sim pass!\n", i);
        bad_bits = true;
      }
      base[i] = (s & 0b00001111) | BIT_OLD;
    }
  }
  CHECK_N(bad_bits);
}

//-----------------------------------------------------------------------------

void GateBoyState::check_state_old_and_driven_or_pulled() {
  if (config_drive_flags) {
    uint8_t* blob = (uint8_t*)this;
    for (auto i = 0; i < sizeof(GateBoyState); i++) {
      auto r = blob[i];
      (void)r;
      CHECK_P((r & 0xF0) == BIT_OLD);
      CHECK_P(bool(r & BIT_DRIVEN) != bool(r & BIT_PULLED));
    }
  }
}

//-----------------------------------------------------------------------------

bool GateBoyState::diff(const GateBoyState& gbb, uint8_t mask) const {
  const GateBoyState& gba = *this;

  bool result = true;

  uint8_t* bytes_a = (uint8_t*)&gba;
  uint8_t* bytes_b = (uint8_t*)&gbb;

  for (int i = 0; i < sizeof(GateBoyState); i++) {
    int byte_a = bytes_a[i] & mask;
    int byte_b = bytes_b[i] & mask;

    if (byte_a != byte_b) {
      LOG_R("MISMATCH @ %5d - ", i);
      print_field_at(i, GateBoyState::fields);
      LOG_R(": 0x%02x 0x%02x\n", byte_a, byte_b);
      result = false;
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

FieldInfo GateBoyState::fields[] = {
  DECLARE_FIELD(GateBoyState, SIG_VCC),
  DECLARE_FIELD(GateBoyState, SIG_GND),
  DECLARE_FIELD(GateBoyState, cpu_signals),
  DECLARE_FIELD(GateBoyState, cpu_abus),
  DECLARE_FIELD(GateBoyState, cpu_dbus),
  DECLARE_FIELD(GateBoyState, vram_abus),
  DECLARE_FIELD(GateBoyState, vram_dbus),
  DECLARE_FIELD(GateBoyState, vram_ext_ctrl),
  DECLARE_FIELD(GateBoyState, vram_ext_abus),
  DECLARE_FIELD(GateBoyState, vram_ext_dbus),
  DECLARE_FIELD(GateBoyState, sprite_ibus),
  DECLARE_FIELD(GateBoyState, sprite_lbus),
  DECLARE_FIELD(GateBoyState, oam_ctrl),
  DECLARE_FIELD(GateBoyState, oam_abus),
  DECLARE_FIELD(GateBoyState, oam_dbus_a),
  DECLARE_FIELD(GateBoyState, oam_dbus_b),
  DECLARE_FIELD(GateBoyState, oam_latch_a),
  DECLARE_FIELD(GateBoyState, oam_latch_b),
  DECLARE_FIELD(GateBoyState, oam_temp_a),
  DECLARE_FIELD(GateBoyState, oam_temp_b),
  DECLARE_FIELD(GateBoyState, ext_ctrl),
  DECLARE_FIELD(GateBoyState, ext_abus.lo),
  DECLARE_FIELD(GateBoyState, ext_abus.hi),
  DECLARE_FIELD(GateBoyState, ext_dbus),
  DECLARE_FIELD(GateBoyState, ext_data_latch),
  DECLARE_FIELD(GateBoyState, ext_addr_latch),
  DECLARE_FIELD(GateBoyState, ext_mbc),
  DECLARE_FIELD(GateBoyState, zram_bus),
  DECLARE_FIELD(GateBoyState, VOGA_HBLANKp),
  DECLARE_FIELD(GateBoyState, XYMU_RENDERINGn),
  DECLARE_FIELD(GateBoyState, sys_rst),
  DECLARE_FIELD(GateBoyState, sys_clk),
  DECLARE_FIELD(GateBoyState, reg_div),
  DECLARE_FIELD(GateBoyState, reg_tima),
  DECLARE_FIELD(GateBoyState, reg_tma),
  DECLARE_FIELD(GateBoyState, reg_tac),
  DECLARE_FIELD(GateBoyState, MATU_DMA_RUNNINGp),
  DECLARE_FIELD(GateBoyState, dma_ctrl),
  DECLARE_FIELD(GateBoyState, dma_lo),
  DECLARE_FIELD(GateBoyState, reg_dma),
  DECLARE_FIELD(GateBoyState, int_ctrl),
  DECLARE_FIELD(GateBoyState, reg_if),
  DECLARE_FIELD(GateBoyState, reg_ie),
  DECLARE_FIELD(GateBoyState, int_latch),
  DECLARE_FIELD(GateBoyState, cpu_int),
  DECLARE_FIELD(GateBoyState, cpu_ack),
  DECLARE_FIELD(GateBoyState, joy_int),
  DECLARE_FIELD(GateBoyState, reg_joy),
  DECLARE_FIELD(GateBoyState, joy_latch),
  DECLARE_FIELD(GateBoyState, joy_ext),
  //GEN_OFFSET(GateBoyState, serial),
  DECLARE_FIELD(GateBoyState, store_i0),
  DECLARE_FIELD(GateBoyState, store_i1),
  DECLARE_FIELD(GateBoyState, store_i2),
  DECLARE_FIELD(GateBoyState, store_i3),
  DECLARE_FIELD(GateBoyState, store_i4),
  DECLARE_FIELD(GateBoyState, store_i5),
  DECLARE_FIELD(GateBoyState, store_i6),
  DECLARE_FIELD(GateBoyState, store_i7),
  DECLARE_FIELD(GateBoyState, store_i8),
  DECLARE_FIELD(GateBoyState, store_i9),
  DECLARE_FIELD(GateBoyState, store_l0),
  DECLARE_FIELD(GateBoyState, store_l1),
  DECLARE_FIELD(GateBoyState, store_l2),
  DECLARE_FIELD(GateBoyState, store_l3),
  DECLARE_FIELD(GateBoyState, store_l4),
  DECLARE_FIELD(GateBoyState, store_l5),
  DECLARE_FIELD(GateBoyState, store_l6),
  DECLARE_FIELD(GateBoyState, store_l7),
  DECLARE_FIELD(GateBoyState, store_l8),
  DECLARE_FIELD(GateBoyState, store_l9),
  DECLARE_FIELD(GateBoyState, store_x0),
  DECLARE_FIELD(GateBoyState, store_x1),
  DECLARE_FIELD(GateBoyState, store_x2),
  DECLARE_FIELD(GateBoyState, store_x3),
  DECLARE_FIELD(GateBoyState, store_x4),
  DECLARE_FIELD(GateBoyState, store_x5),
  DECLARE_FIELD(GateBoyState, store_x6),
  DECLARE_FIELD(GateBoyState, store_x7),
  DECLARE_FIELD(GateBoyState, store_x8),
  DECLARE_FIELD(GateBoyState, store_x9),
  DECLARE_FIELD(GateBoyState, sprite_counter),
  DECLARE_FIELD(GateBoyState, FEPO_STORE_MATCHp),
  DECLARE_FIELD(GateBoyState, sprite_match_flags),
  DECLARE_FIELD(GateBoyState, sprite_reset_flags),
  DECLARE_FIELD(GateBoyState, sprite_store_flags),
  DECLARE_FIELD(GateBoyState, sprite_scanner),
  DECLARE_FIELD(GateBoyState, scan_counter),
  DECLARE_FIELD(GateBoyState, sprite_index),
  DECLARE_FIELD(GateBoyState, sfetch_counter),
  DECLARE_FIELD(GateBoyState, sfetch_control),
  DECLARE_FIELD(GateBoyState, sprite_pix_a),
  DECLARE_FIELD(GateBoyState, sprite_pix_b),
  DECLARE_FIELD(GateBoyState, tfetch_counter),
  DECLARE_FIELD(GateBoyState, tfetch_control),
  DECLARE_FIELD(GateBoyState, tile_temp_a),
  DECLARE_FIELD(GateBoyState, tile_temp_b),
  DECLARE_FIELD(GateBoyState, reg_lcdc),
  DECLARE_FIELD(GateBoyState, reg_stat),
  DECLARE_FIELD(GateBoyState, reg_scx),
  DECLARE_FIELD(GateBoyState, reg_scy),
  DECLARE_FIELD(GateBoyState, reg_wy),
  DECLARE_FIELD(GateBoyState, reg_wx),
  DECLARE_FIELD(GateBoyState, win_x.map),
  DECLARE_FIELD(GateBoyState, win_y.tile),
  DECLARE_FIELD(GateBoyState, win_y.map),
  DECLARE_FIELD(GateBoyState, win_ctrl),
  DECLARE_FIELD(GateBoyState, fine_count),
  DECLARE_FIELD(GateBoyState, fine_scroll),
  DECLARE_FIELD(GateBoyState, pix_count),
  DECLARE_FIELD(GateBoyState, mask_pipe),
  DECLARE_FIELD(GateBoyState, bgw_pipe_a),
  DECLARE_FIELD(GateBoyState, bgw_pipe_b),
  DECLARE_FIELD(GateBoyState, spr_pipe_a),
  DECLARE_FIELD(GateBoyState, spr_pipe_b),
  DECLARE_FIELD(GateBoyState, pal_pipe),
  DECLARE_FIELD(GateBoyState, lcd.REMY_LD0n),
  DECLARE_FIELD(GateBoyState, lcd.RAVO_LD1n),
  DECLARE_FIELD(GateBoyState, lcd),
  DECLARE_FIELD(GateBoyState, reg_lx),
  DECLARE_FIELD(GateBoyState, reg_ly),
  DECLARE_FIELD(GateBoyState, reg_lyc),
  DECLARE_FIELD(GateBoyState, reg_bgp),
  DECLARE_FIELD(GateBoyState, reg_obp0),
  DECLARE_FIELD(GateBoyState, reg_obp1),
  DECLARE_FIELD(GateBoyState, WODU_HBLANKp),
  DECLARE_FIELD(GateBoyState, SATO_BOOT_BITn),
  DECLARE_FIELD(GateBoyState, ATEJ_LINE_RSTp),
  DECLARE_FIELD(GateBoyState, flipped_sprite),
  END_FIELDS()
};

//-----------------------------------------------------------------------------
