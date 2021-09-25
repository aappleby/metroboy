#include "GateBoyLib/GateBoyState.h"

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  SIG_VCC.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND.state = BIT_OLD | BIT_DRIVEN;
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
  oam_ctrl.reset_to_poweron();
  oam_abus.reset_to_poweron();
  oam_dbus_a.reset_to_poweron();
  oam_dbus_b.reset_to_poweron();
  oam_latch_a.reset_to_poweron();
  oam_latch_b.reset_to_poweron();
  oam_temp_a.reset_to_poweron();
  oam_temp_b.reset_to_poweron();
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
  sprite_scanner.reset_to_poweron();
  scan_counter.reset_to_poweron();
  sprite_counter.reset_to_poweron();
  sprite_index.reset_to_poweron();
  sprite_match_flags_odd.reset_to_poweron();
  sprite_reset_flags_odd.reset_to_poweron();
  sprite_store_flags_evn.reset_to_poweron();
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
  sfetch_counter_evn.reset_to_poweron();
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

  SIG_VCC.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND.state = BIT_OLD | BIT_DRIVEN;
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
  VOGA_HBLANKp_evn.state = BIT_OLD | BIT_DRIVEN;
  XYMU_RENDERINGn.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  MATU_DMA_RUNNINGp_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  ACYL_SCANNINGp_odd.state = BIT_OLD | BIT_DRIVEN;
  WODU_HBLANKp_odd.state = BIT_OLD | BIT_DRIVEN;
  SATO_BOOT_BITn.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  ATEJ_LINE_RSTp_odd.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  FEPO_STORE_MATCHp_odd.state = BIT_OLD | BIT_DRIVEN;
  cpu_signals.reset_to_bootrom();
  cpu_abus.reset_to_bootrom();
  cpu_dbus.reset_to_bootrom();
  int_ctrl.reset_to_bootrom();
  int_latch.reset_to_bootrom();
  vram_abus.lo.reset_to_bootrom();
  vram_abus.hi.reset_to_bootrom();
  vram_dbus.reset_to_bootrom();
  oam_ctrl.reset_to_bootrom();
  oam_abus.reset_to_bootrom();
  oam_dbus_a.reset_to_bootrom();
  oam_dbus_b.reset_to_bootrom();
  oam_latch_a.reset_to_bootrom();
  oam_latch_b.reset_to_bootrom();
  oam_temp_a.reset_to_bootrom();
  oam_temp_b.reset_to_bootrom();
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
  sprite_scanner.reset_to_bootrom();
  scan_counter.reset_to_bootrom();
  sprite_counter.reset_to_bootrom();
  sprite_index.reset_to_bootrom();
  sprite_match_flags_odd.reset_to_bootrom();
  sprite_reset_flags_odd.reset_to_bootrom();
  sprite_store_flags_evn.reset_to_bootrom();
  sprite_ibus.reset_to_bootrom();
  sprite_lbus.reset_to_bootrom();
  store_i0.reset_to_bootrom();
  store_i1.reset_to_bootrom();
  store_i2.reset_to_bootrom();
  store_i3.reset_to_bootrom();
  store_i4.reset_to_bootrom();
  store_i5.reset_to_bootrom();
  store_i6.reset_to_bootrom();
  store_i7.reset_to_bootrom();
  store_i8.reset_to_bootrom();
  store_i9.reset_to_bootrom();
  store_l0.reset_to_bootrom();
  store_l1.reset_to_bootrom();
  store_l2.reset_to_bootrom();
  store_l3.reset_to_bootrom();
  store_l4.reset_to_bootrom();
  store_l5.reset_to_bootrom();
  store_l6.reset_to_bootrom();
  store_l7.reset_to_bootrom();
  store_l8.reset_to_bootrom();
  store_l9.reset_to_bootrom();
  store_x0.reset_to_bootrom();
  store_x1.reset_to_bootrom();
  store_x2.reset_to_bootrom();
  store_x3.reset_to_bootrom();
  store_x4.reset_to_bootrom();
  store_x5.reset_to_bootrom();
  store_x6.reset_to_bootrom();
  store_x7.reset_to_bootrom();
  store_x8.reset_to_bootrom();
  store_x9.reset_to_bootrom();
  sfetch_counter_evn.reset_to_bootrom();
  sfetch_control.reset_to_bootrom();
  tfetch_counter.reset_to_bootrom();
  tfetch_control.reset_to_bootrom();
  tile_temp_a.reset_to_bootrom();
  tile_temp_b.reset_to_bootrom();
  win_ctrl.reset_to_bootrom();
  win_x.map.reset_to_bootrom();
  win_y.tile.reset_to_bootrom();
  win_y.map.reset_to_bootrom();
  fine_count.reset_to_bootrom();
  fine_scroll.reset_to_bootrom();
  flipped_sprite.reset_to_bootrom();
  sprite_pix_a.reset_to_bootrom();
  sprite_pix_b.reset_to_bootrom();
  pix_count.reset_to_bootrom();
  mask_pipe.reset_to_bootrom();
  bgw_pipe_a.reset_to_bootrom();
  bgw_pipe_b.reset_to_bootrom();
  spr_pipe_a.reset_to_bootrom();
  spr_pipe_b.reset_to_bootrom();
  pal_pipe.reset_to_bootrom();
  lcd.reset_to_bootrom();
  //reg_NR10.reset_to_bootrom();
  //reg_NR11.reset_to_bootrom();
  //reg_NR12.reset_to_bootrom();
  //reg_NR14.reset_to_bootrom();
  //reg_NR21.reset_to_bootrom();
  //reg_NR22.reset_to_bootrom();
  //reg_NR24.reset_to_bootrom();
  //reg_NR30.reset_to_bootrom();
  //reg_NR31.reset_to_bootrom();
  //reg_NR32.reset_to_bootrom();
  //reg_NR34.reset_to_bootrom();
  //reg_NR41.reset_to_bootrom();
  //reg_NR42.reset_to_bootrom();
  //reg_NR43.reset_to_bootrom();
  //reg_NR44.reset_to_bootrom();
  //reg_NR50.reset_to_bootrom();
  //reg_NR51.reset_to_bootrom();
  //reg_NR52.reset_to_bootrom();

  check_state_old_and_driven_or_pulled();
}

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  SIG_VCC.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND.state = BIT_OLD | BIT_DRIVEN;
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
  
  VOGA_HBLANKp_evn.state = 0b00011001;
  XYMU_RENDERINGn.state = 0x19;
  MATU_DMA_RUNNINGp_odd.state = 0x1A;
  ACYL_SCANNINGp_odd.state = 0x18;
  WODU_HBLANKp_odd.state = 0b00011001;
  SATO_BOOT_BITn.state = 0x19;
  ATEJ_LINE_RSTp_odd.state = 0b00011000;
  FEPO_STORE_MATCHp_odd.state = 0x18;

  cpu_signals.reset_to_cart();
  cpu_abus.reset_to_cart();
  cpu_dbus.reset_to_cart();
  int_ctrl.reset_to_cart();
  int_latch.reset_to_cart();
  vram_abus.lo.reset_to_cart();
  vram_abus.hi.reset_to_cart();
  vram_dbus.reset_to_cart();
  oam_ctrl.reset_to_cart();
  oam_abus.reset_to_cart();
  oam_dbus_a.reset_to_cart();
  oam_dbus_b.reset_to_cart();
  oam_latch_a.reset_to_cart();
  oam_latch_b.reset_to_cart();
  oam_temp_a.reset_to_cart();
  oam_temp_b.reset_to_cart();
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
  sprite_scanner.reset_to_cart();
  scan_counter.reset_to_cart();
  sprite_counter.reset_to_cart();
  sprite_index.reset_to_cart();
  sprite_match_flags_odd.reset_to_cart();
  sprite_reset_flags_odd.reset_to_cart();
  sprite_store_flags_evn.reset_to_cart();
  sprite_ibus.reset_to_cart();
  sprite_lbus.reset_to_cart();
  store_i0.reset_to_cart();
  store_i1.reset_to_cart();
  store_i2.reset_to_cart();
  store_i3.reset_to_cart();
  store_i4.reset_to_cart();
  store_i5.reset_to_cart();
  store_i6.reset_to_cart();
  store_i7.reset_to_cart();
  store_i8.reset_to_cart();
  store_i9.reset_to_cart();
  store_l0.reset_to_cart();
  store_l1.reset_to_cart();
  store_l2.reset_to_cart();
  store_l3.reset_to_cart();
  store_l4.reset_to_cart();
  store_l5.reset_to_cart();
  store_l6.reset_to_cart();
  store_l7.reset_to_cart();
  store_l8.reset_to_cart();
  store_l9.reset_to_cart();
  store_x0.reset_to_cart();
  store_x1.reset_to_cart();
  store_x2.reset_to_cart();
  store_x3.reset_to_cart();
  store_x4.reset_to_cart();
  store_x5.reset_to_cart();
  store_x6.reset_to_cart();
  store_x7.reset_to_cart();
  store_x8.reset_to_cart();
  store_x9.reset_to_cart();
  sfetch_counter_evn.reset_to_cart();
  sfetch_control.reset_to_cart();
  tfetch_counter.reset_to_cart();
  tfetch_control.reset_to_cart();
  tile_temp_a.reset_to_cart();
  tile_temp_b.reset_to_cart();
  win_ctrl.reset_to_cart();
  win_x.map.reset_to_cart();
  win_y.tile.reset_to_cart();
  win_y.map.reset_to_cart();
  fine_count.reset_to_cart();
  fine_scroll.reset_to_cart();
  flipped_sprite.reset_to_cart();
  sprite_pix_a.reset_to_cart();
  sprite_pix_b.reset_to_cart();
  pix_count.reset_to_cart();
  mask_pipe.reset_to_cart();
  bgw_pipe_a.reset_to_cart();
  bgw_pipe_b.reset_to_cart();
  spr_pipe_a.reset_to_cart();
  spr_pipe_b.reset_to_cart();
  pal_pipe.reset_to_cart();
  lcd.reset_to_cart();
  //reg_NR10.reset_to_cart();
  //reg_NR11.reset_to_cart();
  //reg_NR12.reset_to_cart();
  //reg_NR14.reset_to_cart();
  //reg_NR21.reset_to_cart();
  //reg_NR22.reset_to_cart();
  //reg_NR24.reset_to_cart();
  //reg_NR30.reset_to_cart();
  //reg_NR31.reset_to_cart();
  //reg_NR32.reset_to_cart();
  //reg_NR34.reset_to_cart();
  //reg_NR41.reset_to_cart();
  //reg_NR42.reset_to_cart();
  //reg_NR43.reset_to_cart();
  //reg_NR44.reset_to_cart();
  //reg_NR50.reset_to_cart();
  //reg_NR51.reset_to_cart();
  //reg_NR52.reset_to_cart();

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

GBResult GateBoyState::peek(int addr) const {
  switch(addr) {
  case ADDR_P1  : break; 
  case ADDR_SB  : break;
  case ADDR_SC  : break;
  case ADDR_DIV : return GBResult((uint8_t)bit_pack(reg_div) >> 6);
  case ADDR_TIMA: return GBResult((uint8_t)bit_pack(reg_tima));
  case ADDR_TMA : return GBResult((uint8_t)bit_pack(reg_tma));
  case ADDR_TAC : return GBResult((uint8_t)bit_pack(reg_tac));
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
  case ADDR_LCDC: return GBResult((uint8_t)bit_pack_inv(reg_lcdc));
  case ADDR_STAT: break;
  case ADDR_SCY : return GBResult((uint8_t)bit_pack_inv(reg_scy));
  case ADDR_SCX : return GBResult((uint8_t)bit_pack_inv(reg_scx));
  case ADDR_LY  : return GBResult((uint8_t)bit_pack_inv(reg_ly));
  case ADDR_LYC : return GBResult((uint8_t)bit_pack_inv(reg_lyc));
  case ADDR_DMA : return GBResult((uint8_t)bit_pack_inv(reg_dma));
  case ADDR_BGP : return GBResult((uint8_t)bit_pack_inv(reg_bgp));
  case ADDR_OBP0: return GBResult((uint8_t)bit_pack_inv(reg_obp0));
  case ADDR_OBP1: return GBResult((uint8_t)bit_pack_inv(reg_obp1));
  case ADDR_WY  : return GBResult((uint8_t)bit_pack_inv(reg_wy));
  case ADDR_WX  : return GBResult((uint8_t)bit_pack_inv(reg_wx));
  }

  LOG_R("GateBoy::peek - bad address 0x%04x\n", addr);
  return Error::NOT_FOUND;
}

//-----------------------------------------------------------------------------

GBResult GateBoyState::poke(int addr, uint8_t data_in) {
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
  case ADDR_LCDC: { bit_unpack_inv(reg_lcdc, data_in); return GBResult::ok(); }
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
  if (!config_check_flags && !config_use_flags) return;

  uint8_t* cursor = (uint8_t*)this;
  bool bad_bits = false;
  for (size_t i = 0; i < sizeof(GateBoyState); i++) {
    uint8_t s = *cursor;
    if (config_check_flags) {
      auto drive_flags = s & (BIT_DRIVEN | BIT_PULLED);

      if (drive_flags == (BIT_DRIVEN | BIT_PULLED)) {
        LOG_Y("Bit %d both driven and pulled up!\n", i);
        bad_bits = true;
      }

      if (drive_flags == 0) {
        LOG_Y("Bit %d floating!\n", i);
        bad_bits = true;
      }

      auto oldnew_flags = s & (BIT_OLD | BIT_NEW);

      if (oldnew_flags != BIT_NEW) {
        LOG_Y("Bit %d not dirty after sim pass!\n", i);
        bad_bits = true;
      }
    }

    *cursor++ = (s & 0b00001111) | BIT_OLD;
  }
  CHECK_N(bad_bits);
}

//-----------------------------------------------------------------------------

void GateBoyState::check_state_old_and_driven_or_pulled() {
  if (config_check_flags) {
    uint8_t* blob = (uint8_t*)this;
    for (auto i = 0; i < sizeof(GateBoyState); i++) {
      auto r = blob[i];
      (void)r;

      if ((r & 0xF0) != BIT_OLD) {
        LOG_R("BAD OLD STATE ");
        print_field_at(i, GateBoyState::fields);
        LOG_R(" = 0x%02x\n", r);
      }

      //CHECK_P((r & 0xF0) == BIT_OLD);

      if (!(r & BIT_DRIVEN) && !(r & BIT_PULLED)) {
        LOG_R("BAD OLD STATE ");
        print_field_at(i, GateBoyState::fields);
        LOG_R(" = 0x%02x\n", r);
      }

      //CHECK_P(bool(r & BIT_DRIVEN) != bool(r & BIT_PULLED));
    }
  }
}

//-----------------------------------------------------------------------------

bool GateBoyState::diff(const GateBoyState& gbb, uint8_t mask) const {
  const GateBoyState& gba = *this;

  bool mismatch = false;

  uint8_t* bytes_a = (uint8_t*)&gba;
  uint8_t* bytes_b = (uint8_t*)&gbb;

  for (int i = 0; i < sizeof(GateBoyState); i++) {
    int byte_a = bytes_a[i] & mask;
    int byte_b = bytes_b[i] & mask;

    if (byte_a != byte_b) {
      LOG_R("MISMATCH @ %5d - ", i);
      print_field_at(i, GateBoyState::fields);
      LOG_R(": 0x%02x 0x%02x\n", byte_a, byte_b);
      mismatch = true;
    }
  }

  return mismatch;
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
  DECLARE_FIELD(GateBoyState, ext_data_latch),
  DECLARE_FIELD(GateBoyState, ext_addr_latch),
  DECLARE_FIELD(GateBoyState, ext_mbc),
  DECLARE_FIELD(GateBoyState, zram_bus),
  DECLARE_FIELD(GateBoyState, VOGA_HBLANKp_evn),
  DECLARE_FIELD(GateBoyState, XYMU_RENDERINGn),
  DECLARE_FIELD(GateBoyState, sys_rst),
  DECLARE_FIELD(GateBoyState, sys_clk),
  DECLARE_FIELD(GateBoyState, reg_div),
  DECLARE_FIELD(GateBoyState, reg_tima),
  DECLARE_FIELD(GateBoyState, reg_tma),
  DECLARE_FIELD(GateBoyState, reg_tac),
  DECLARE_FIELD(GateBoyState, MATU_DMA_RUNNINGp_odd),
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
  DECLARE_FIELD(GateBoyState, FEPO_STORE_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, sprite_match_flags_odd),
  DECLARE_FIELD(GateBoyState, sprite_reset_flags_odd),
  DECLARE_FIELD(GateBoyState, sprite_store_flags_evn),
  DECLARE_FIELD(GateBoyState, sprite_scanner),
  DECLARE_FIELD(GateBoyState, scan_counter),
  DECLARE_FIELD(GateBoyState, sprite_index),
  DECLARE_FIELD(GateBoyState, sfetch_counter_evn),
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
  DECLARE_FIELD(GateBoyState, WODU_HBLANKp_odd),
  DECLARE_FIELD(GateBoyState, SATO_BOOT_BITn),
  DECLARE_FIELD(GateBoyState, ATEJ_LINE_RSTp_odd),
  DECLARE_FIELD(GateBoyState, flipped_sprite),
  END_FIELDS()
};

//-----------------------------------------------------------------------------
