#include "GateBoyLib/GateBoyState.h"

#include "GateBoyLib/Utils.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

wire GateBoyState::XAPO_VID_RSTn_new() const {
  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(sys_rst.AFER_SYS_RSTp.qp_new(), sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*#p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*_p01.XEBE*/ wire XEBE_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*_p01.XODO*/ wire XODO_VID_RSTp = nand2(XEBE_SYS_RSTn, reg_lcdc.XONA_LCDC_LCDENp.qp_new());
  /*_p01.XAPO*/ wire XAPO_VID_RSTn_new = not1(XODO_VID_RSTp);
  return XAPO_VID_RSTn_new;

}
wire GateBoyState::LYHA_VID_RSTp_new() const { /*_p01.LYHA*/ wire LYHA_VID_RSTp_new = not1(XAPO_VID_RSTn_new()); return LYHA_VID_RSTp_new; }
wire GateBoyState::TOFU_VID_RSTp_new() const { /*_p01.TOFU*/ wire TOFU_VID_RSTp_new = not1(XAPO_VID_RSTn_new()); return TOFU_VID_RSTp_new; }
wire GateBoyState::ROSY_VID_RSTp_new() const { /*_p01.ROSY*/ wire ROSY_VID_RSTp_new = not1(XAPO_VID_RSTn_new()); return ROSY_VID_RSTp_new; }
wire GateBoyState::ATAR_VID_RSTp_new() const { /*#p01.ATAR*/ wire ATAR_VID_RSTp_new = not1(XAPO_VID_RSTn_new()); return ATAR_VID_RSTp_new; }
wire GateBoyState::PYRY_VID_RSTp_new() const { /*_p01.PYRY*/ wire PYRY_VID_RSTp_new = not1(XAPO_VID_RSTn_new()); return PYRY_VID_RSTp_new; }
wire GateBoyState::LYFE_VID_RSTn_new() const { /*_p01.LYFE*/ wire LYFE_VID_RSTn_new = not1(LYHA_VID_RSTp_new()); return LYFE_VID_RSTn_new; }
wire GateBoyState::ABEZ_VID_RSTn_new() const { /*#p01.ABEZ*/ wire ABEZ_VID_RSTn_new = not1(ATAR_VID_RSTp_new()); return ABEZ_VID_RSTn_new; }

//-----------------------------------------------------------------------------

void GateBoyState::poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));
  SIG_VCC.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND.state = BIT_OLD | BIT_DRIVEN;

  cpu_dbus.BUS_CPU_D00p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D01p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D02p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D03p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D04p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D05p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D06p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  cpu_dbus.BUS_CPU_D07p.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
}

//-----------------------------------------------------------------------------

void GateBoyState::reset() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(GateBoyState));

  SIG_VCC.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_GND.state = BIT_OLD | BIT_DRIVEN;
  reg_joy.reset();
  //reg_sb.reset();
  //reg_sc.reset();
  reg_div.reset();
  reg_tima.reset();
  reg_tma.reset();
  reg_tac.reset();
  reg_if.reset();
  reg_lcdc.reset();
  reg_stat.reset();
  reg_scy.reset();
  reg_scx.reset();
  reg_ly.reset();
  reg_lyc.reset();
  reg_dma.reset();
  reg_bgp.reset();
  reg_obp0.reset();
  reg_obp1.reset();
  reg_wy.reset();
  reg_wx.reset();
  reg_lx.reset();
  reg_ie.reset();
  sys_rst.reset();
  sys_clk.reset();
  
  VOGA_HBLANKp_evn.state = 0b00011011;
  XYMU_RENDERING_LATCHn.state = 0b00011001;
  MATU_DMA_RUNNINGp_odd.state = 0b00011000;
  ACYL_SCANNINGp_odd.state = 0b00011000;
  WODU_HBLANK_GATEp_odd.state = 0b00011001;
  ATEJ_LINE_RST_TRIGp_odd.state = 0b00011000;
  FEPO_STORE_MATCHp_odd.state = 0x18;

  cpu_signals.reset();
  cpu_abus.reset();
  cpu_dbus.reset();
  int_ctrl.reset();
  int_latch.reset();
  vram_abus.lo.reset();
  vram_abus.hi.reset();
  vram_dbus.reset();
  oam_ctrl.reset();
  oam_abus.reset();
  oam_dbus_a.reset();
  oam_dbus_b.reset();
  oam_latch_a.reset();
  oam_latch_b.reset();
  oam_temp_a.reset();
  oam_temp_b.reset();
  ext_data_latch.reset();
  ext_addr_latch.reset();
  ext_mbc.reset();
  zram_bus.reset();
  dma_ctrl.reset();
  dma_lo.reset();
  cpu_int.reset();
  cpu_ack.reset();
  joy_int.reset();
  joy_latch.reset();
  sprite_scanner.reset();
  scan_counter.reset();
  sprite_counter.reset();
  sprite_index.reset();
  sprite_match_flags_odd.reset();
  sprite_reset_flags_odd.reset();
  sprite_ibus.reset();
  sprite_lbus.reset();
  store_i0.reset();
  store_i1.reset();
  store_i2.reset();
  store_i3.reset();
  store_i4.reset();
  store_i5.reset();
  store_i6.reset();
  store_i7.reset();
  store_i8.reset();
  store_i9.reset();
  store_l0.reset();
  store_l1.reset();
  store_l2.reset();
  store_l3.reset();
  store_l4.reset();
  store_l5.reset();
  store_l6.reset();
  store_l7.reset();
  store_l8.reset();
  store_l9.reset();
  store_x0.reset();
  store_x1.reset();
  store_x2.reset();
  store_x3.reset();
  store_x4.reset();
  store_x5.reset();
  store_x6.reset();
  store_x7.reset();
  store_x8.reset();
  store_x9.reset();
  sfetch_counter_evn.reset();
  sfetch_control.reset();
  tfetch_counter.reset();
  tfetch_control.reset();
  tile_temp_a.reset();
  tile_temp_b.reset();
  win_ctrl.reset();
  win_x.map.reset();
  win_y.tile.reset();
  win_y.map.reset();
  fine_count_odd.reset();
  fine_scroll.reset();
  flipped_sprite.reset();
  sprite_pix_a.reset();
  sprite_pix_b.reset();
  pix_count.reset();
  mask_pipe.reset();
  bgw_pipe_a.reset();
  bgw_pipe_b.reset();
  spr_pipe_a.reset();
  spr_pipe_b.reset();
  pal_pipe.reset();
  lcd.reset();

#ifdef SIM_AUDIO
  spu.reset();
  ch1.reset();
  ch2.reset();
  ch3.reset();
  ch4.reset();
  wave_dbus.reset();
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
#if 0
  static bool dumped = false;
  if (!dumped) {
    for (int i = 0; i < sizeof(GateBoyState); i++) {
      printf("@%04x ", i);
      print_field_at(i, GateBoyState::fields);
      printf("\n");
    }
    /*
    for (int i = 0; i < 1024; i++) {
      auto f = GateBoyState::fields[i];
      if (f.name == nullptr) break;
      printf("%d %s 0x%04x %d\n", i, f.name, f.offset, f.size);
    }
    */
    dumped = true;
  }
#endif

  if (!config_check_flags && !config_use_flags) return;

  uint8_t* cursor = (uint8_t*)this;
  bool bad_bits = false;
  for (size_t i = 0; i < sizeof(GateBoyState); i++) {
    uint8_t s = *cursor;
    if (config_check_flags) {
      auto drive_flags = s & (BIT_DRIVEN | BIT_PULLED);

      bool bad_bit = false;

      if (drive_flags == (BIT_DRIVEN | BIT_PULLED)) {
        LOG_Y("Bit %d both driven and pulled up! ", i);
        bad_bit = true;
      }

      if (drive_flags == 0) {
        LOG_Y("Bit %d floating! ", i);
        bad_bit = true;
      }

      auto oldnew_flags = s & (BIT_OLD | BIT_NEW);

      if (oldnew_flags != BIT_NEW) {
        LOG_Y("Bit %d not dirty after sim pass! ", i);
        bad_bit = true;
      }

      if (bad_bit) {
        print_field_at(int(i), GateBoyState::fields);
        printf("\n");
      }


      bad_bits |= bad_bit;
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

  DECLARE_FIELD(GateBoyState, oam_ctrl.MAKA_LATCH_EXTp),
  DECLARE_FIELD(GateBoyState, oam_ctrl.WUJE_CPU_OAM_WRn),
  DECLARE_FIELD(GateBoyState, oam_ctrl.SIG_OAM_CLKn),
  DECLARE_FIELD(GateBoyState, oam_ctrl.SIG_OAM_WRn_A),
  DECLARE_FIELD(GateBoyState, oam_ctrl.SIG_OAM_WRn_B),
  DECLARE_FIELD(GateBoyState, oam_ctrl.SIG_OAM_OEn),
  DECLARE_FIELD(GateBoyState, oam_ctrl.old_oam_clk),
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
  DECLARE_FIELD(GateBoyState, XYMU_RENDERING_LATCHn),
  DECLARE_FIELD(GateBoyState, MATU_DMA_RUNNINGp_odd),
  DECLARE_FIELD(GateBoyState, ACYL_SCANNINGp_odd),
  DECLARE_FIELD(GateBoyState, WODU_HBLANK_GATEp_odd),
  DECLARE_FIELD(GateBoyState, ATEJ_LINE_RST_TRIGp_odd),
  DECLARE_FIELD(GateBoyState, FEPO_STORE_MATCHp_odd),


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

  DECLARE_FIELD(GateBoyState, sprite_scanner.FETO_SCAN_DONEp),
  DECLARE_FIELD(GateBoyState, sprite_scanner.AVAP_SCAN_DONE_tp_odd),
  DECLARE_FIELD(GateBoyState, sprite_scanner.BESU_SCAN_DONEn_odd),
  DECLARE_FIELD(GateBoyState, sprite_scanner.CENO_SCAN_DONEn_odd),
  DECLARE_FIELD(GateBoyState, sprite_scanner.DEZY_INC_COUNTn_odd),
  DECLARE_FIELD(GateBoyState, sprite_scanner.BYBA_SCAN_DONEp_odd),
  DECLARE_FIELD(GateBoyState, sprite_scanner.DOBA_SCAN_DONEp_evn),

  DECLARE_FIELD(GateBoyState, scan_counter),
  DECLARE_FIELD(GateBoyState, sprite_index),
  DECLARE_FIELD(GateBoyState, sfetch_counter_evn),

  DECLARE_FIELD(GateBoyState, sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd),
  DECLARE_FIELD(GateBoyState, sfetch_control.TEXY_SFETCHINGp_evn),
  DECLARE_FIELD(GateBoyState, sfetch_control.TAKA_SFETCH_RUNNINGp_evn),
  DECLARE_FIELD(GateBoyState, sfetch_control.SOBU_SFETCH_REQp_evn),
  DECLARE_FIELD(GateBoyState, sfetch_control.SUDA_SFETCH_REQp_odd),
  DECLARE_FIELD(GateBoyState, sfetch_control.TYFO_SFETCH_S0p_D1_odd),
  DECLARE_FIELD(GateBoyState, sfetch_control.TOBU_SFETCH_S1p_D2_evn),
  DECLARE_FIELD(GateBoyState, sfetch_control.VONU_SFETCH_S1p_D4_evn),
  DECLARE_FIELD(GateBoyState, sfetch_control.SEBA_SFETCH_S1p_D5_odd),

  DECLARE_FIELD(GateBoyState, sprite_pix_a),
  DECLARE_FIELD(GateBoyState, sprite_pix_b),

  DECLARE_FIELD(GateBoyState, tfetch_counter),

  DECLARE_FIELD(GateBoyState, tfetch_control.LYRY_BFETCH_DONEp_odd),
  DECLARE_FIELD(GateBoyState, tfetch_control.POKY_PRELOAD_LATCHp_evn),
  DECLARE_FIELD(GateBoyState, tfetch_control.LONY_TFETCHINGp),
  DECLARE_FIELD(GateBoyState, tfetch_control.LOVY_TFETCH_DONEp),
  DECLARE_FIELD(GateBoyState, tfetch_control.NYKA_FETCH_DONEp_evn),
  DECLARE_FIELD(GateBoyState, tfetch_control.PORY_FETCH_DONEp_odd),
  DECLARE_FIELD(GateBoyState, tfetch_control.PYGO_FETCH_DONEp_evn),
  DECLARE_FIELD(GateBoyState, tfetch_control.LYZU_BFETCH_S0p_D1),

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
  
  DECLARE_FIELD(GateBoyState, win_ctrl.NUKO_WX_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.ROGE_WY_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.PYNU_WIN_MODE_LATCHp),
  DECLARE_FIELD(GateBoyState, win_ctrl.PUKU_WIN_HITn_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.RYDY_WIN_HITp),
  DECLARE_FIELD(GateBoyState, win_ctrl.SOVY_WIN_HITp),
  DECLARE_FIELD(GateBoyState, win_ctrl.NOPA_WIN_MODE_Bp_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.PYCO_WIN_MATCHp_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.NUNU_WIN_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.REJO_WY_MATCH_LATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.SARY_WY_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.RYFA_WIN_FETCHn_A),
  DECLARE_FIELD(GateBoyState, win_ctrl.RENE_WIN_FETCHn_B),

  DECLARE_FIELD(GateBoyState, fine_count_odd),
  
  DECLARE_FIELD(GateBoyState, fine_scroll.PUXA_SCX_FINE_MATCH_evn),
  DECLARE_FIELD(GateBoyState, fine_scroll.NYZE_SCX_FINE_MATCH_odd),
  DECLARE_FIELD(GateBoyState, fine_scroll.ROXY_FINE_SCROLL_DONEn),

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
  DECLARE_FIELD(GateBoyState, flipped_sprite),

#ifdef SIM_AUDIO
  DECLARE_FIELD(GateBoyState, spu),
  DECLARE_FIELD(GateBoyState, ch1),
  DECLARE_FIELD(GateBoyState, ch2),
  DECLARE_FIELD(GateBoyState, ch3),
  DECLARE_FIELD(GateBoyState, ch4),
  DECLARE_FIELD(GateBoyState, wave_dbus),
#endif
  

  END_FIELDS()
};

//-----------------------------------------------------------------------------
