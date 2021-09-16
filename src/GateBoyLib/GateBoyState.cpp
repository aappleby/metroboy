#include "GateBoyLib/GateBoyState.h"

//-----------------------------------------------------------------------------

void GateBoyState::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));
}

void GateBoyState::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  reg_joy.reset_to_bootrom();

  reg_dma.NAFA_DMA_A08n.state = 0b00011010;
  reg_dma.PYNE_DMA_A09n.state = 0b00011010;
  reg_dma.PARA_DMA_A10n.state = 0b00011010;
  reg_dma.NYDO_DMA_A11n.state = 0b00011010;
  reg_dma.NYGY_DMA_A12n.state = 0b00011010;                           
  reg_dma.PULA_DMA_A13n.state = 0b00011010;
  reg_dma.POKU_DMA_A14n.state = 0b00011010;
  reg_dma.MARU_DMA_A15n.state = 0b00011010;

  reg_bgp.PAVO_BGP_D0n.state = 0b00011010;
  reg_bgp.NUSY_BGP_D1n.state = 0b00011010;
  reg_bgp.PYLU_BGP_D2n.state = 0b00011010;
  reg_bgp.MAXY_BGP_D3n.state = 0b00011010;
  reg_bgp.MUKE_BGP_D4n.state = 0b00011010;
  reg_bgp.MORU_BGP_D5n.state = 0b00011010;
  reg_bgp.MOGY_BGP_D6n.state = 0b00011010;
  reg_bgp.MENA_BGP_D7n.state = 0b00011010;

  reg_obp0.XUFU_OBP0_D0n.state = 0b00011010;
  reg_obp0.XUKY_OBP0_D1n.state = 0b00011010;
  reg_obp0.XOVA_OBP0_D2n.state = 0b00011010;
  reg_obp0.XALO_OBP0_D3n.state = 0b00011010;
  reg_obp0.XERU_OBP0_D4n.state = 0b00011010;
  reg_obp0.XYZE_OBP0_D5n.state = 0b00011010;
  reg_obp0.XUPO_OBP0_D6n.state = 0b00011010;
  reg_obp0.XANA_OBP0_D7n.state = 0b00011010;

  reg_obp1.MOXY_OBP1_D0n.state = 0b00011010;
  reg_obp1.LAWO_OBP1_D1n.state = 0b00011010;
  reg_obp1.MOSA_OBP1_D2n.state = 0b00011010;
  reg_obp1.LOSE_OBP1_D3n.state = 0b00011010;
  reg_obp1.LUNE_OBP1_D4n.state = 0b00011010;
  reg_obp1.LUGU_OBP1_D5n.state = 0b00011010;
  reg_obp1.LEPU_OBP1_D6n.state = 0b00011010;
  reg_obp1.LUXO_OBP1_D7n.state = 0b00011010;

  SIG_VCC = BIT_OLD | BIT_DRIVEN | BIT_DATA;

  //gb_state.cpu_abus.reset_to_poweron();
  //gb_state.cpu_dbus.reset_to_poweron();

  //gb_state.sprite_ibus.reset_to_poweron();
  //gb_state.sprite_lbus.reset_to_poweron();

  //gb_state.joy_int.reset_to_poweron();

  reg_joy.reset_to_bootrom();
  reg_tima.reset_to_bootrom();

  check_state_old_and_driven_or_pulled();
}

void GateBoyState::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  VOGA_HBLANKp.state = 0b00011001;

  cpu_signals.reset_to_cart();
  cpu_abus.reset_to_cart_new();
  cpu_dbus.reset_to_cart_new();

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
