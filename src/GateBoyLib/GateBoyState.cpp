#include "GateBoyLib/GateBoyState.h"

//------------------------------------------------------------------------------------------------------------------------

struct MemberOffset {
  const char* name;
  int offset;
};

#define GEN_OFFSET(A) { #A, offsetof(GateBoyState, A) }

MemberOffset gb_offsets[] = {
  GEN_OFFSET(SIG_VCC),
  GEN_OFFSET(SIG_GND),
  GEN_OFFSET(cpu_signals),
  GEN_OFFSET(cpu_abus),
  GEN_OFFSET(cpu_dbus),
  GEN_OFFSET(vram_abus),
  GEN_OFFSET(vram_dbus),
  GEN_OFFSET(vram_ext_ctrl),
  GEN_OFFSET(vram_ext_abus),
  GEN_OFFSET(vram_ext_dbus),
  GEN_OFFSET(sprite_ibus),
  GEN_OFFSET(sprite_lbus),
  GEN_OFFSET(oam_ctrl),
  GEN_OFFSET(oam_abus),
  GEN_OFFSET(oam_dbus_a),
  GEN_OFFSET(oam_dbus_b),
  GEN_OFFSET(oam_latch_a),
  GEN_OFFSET(oam_latch_b),
  GEN_OFFSET(oam_temp_a),
  GEN_OFFSET(oam_temp_b),
  GEN_OFFSET(ext_ctrl),
  GEN_OFFSET(ext_abus.lo),
  GEN_OFFSET(ext_abus.hi),
  GEN_OFFSET(ext_dbus),
  GEN_OFFSET(ext_data_latch),
  GEN_OFFSET(ext_addr_latch),
  GEN_OFFSET(ext_mbc),
  GEN_OFFSET(zram_bus),
  GEN_OFFSET(VOGA_HBLANKp),
  GEN_OFFSET(XYMU_RENDERINGn),
  GEN_OFFSET(sys_rst),
  GEN_OFFSET(sys_clk),
  GEN_OFFSET(reg_div),
  GEN_OFFSET(reg_tima),
  GEN_OFFSET(reg_tma),
  GEN_OFFSET(reg_tac),
  GEN_OFFSET(MATU_DMA_RUNNINGp),
  GEN_OFFSET(dma_ctrl),
  GEN_OFFSET(dma_lo),
  GEN_OFFSET(reg_dma),
  GEN_OFFSET(int_ctrl),
  GEN_OFFSET(reg_if),
  GEN_OFFSET(reg_ie),
  GEN_OFFSET(int_latch),
  GEN_OFFSET(cpu_int),
  GEN_OFFSET(cpu_ack),
  GEN_OFFSET(joy_int),
  GEN_OFFSET(reg_joy),
  GEN_OFFSET(joy_latch),
  GEN_OFFSET(joy_ext),
  //GEN_OFFSET(serial),
  GEN_OFFSET(store_i0),
  GEN_OFFSET(store_i1),
  GEN_OFFSET(store_i2),
  GEN_OFFSET(store_i3),
  GEN_OFFSET(store_i4),
  GEN_OFFSET(store_i5),
  GEN_OFFSET(store_i6),
  GEN_OFFSET(store_i7),
  GEN_OFFSET(store_i8),
  GEN_OFFSET(store_i9),
  GEN_OFFSET(store_l0),
  GEN_OFFSET(store_l1),
  GEN_OFFSET(store_l2),
  GEN_OFFSET(store_l3),
  GEN_OFFSET(store_l4),
  GEN_OFFSET(store_l5),
  GEN_OFFSET(store_l6),
  GEN_OFFSET(store_l7),
  GEN_OFFSET(store_l8),
  GEN_OFFSET(store_l9),
  GEN_OFFSET(store_x0),
  GEN_OFFSET(store_x1),
  GEN_OFFSET(store_x2),
  GEN_OFFSET(store_x3),
  GEN_OFFSET(store_x4),
  GEN_OFFSET(store_x5),
  GEN_OFFSET(store_x6),
  GEN_OFFSET(store_x7),
  GEN_OFFSET(store_x8),
  GEN_OFFSET(store_x9),
  GEN_OFFSET(sprite_counter),
  GEN_OFFSET(FEPO_STORE_MATCHp),
  GEN_OFFSET(sprite_match_flags),
  GEN_OFFSET(sprite_reset_flags),
  GEN_OFFSET(sprite_store_flags),
  GEN_OFFSET(sprite_scanner),
  GEN_OFFSET(scan_counter),
  GEN_OFFSET(sprite_index),
  GEN_OFFSET(sfetch_control),
  GEN_OFFSET(sprite_pix_a),
  GEN_OFFSET(sprite_pix_b),
  GEN_OFFSET(tfetch_counter),
  GEN_OFFSET(tfetch_control),
  GEN_OFFSET(tile_temp_a),
  GEN_OFFSET(tile_temp_b),
  GEN_OFFSET(reg_lcdc),
  GEN_OFFSET(reg_stat),
  GEN_OFFSET(reg_scx),
  GEN_OFFSET(reg_scy),
  GEN_OFFSET(reg_wy),
  GEN_OFFSET(reg_wx),
  GEN_OFFSET(win_x.map),
  GEN_OFFSET(win_y.tile),
  GEN_OFFSET(win_y.map),
  GEN_OFFSET(win_ctrl),
  GEN_OFFSET(fine_count),
  GEN_OFFSET(fine_scroll),
  GEN_OFFSET(pix_count),
  GEN_OFFSET(mask_pipe),
  GEN_OFFSET(bgw_pipe_a),
  GEN_OFFSET(bgw_pipe_b),
  GEN_OFFSET(spr_pipe_a),
  GEN_OFFSET(spr_pipe_b),
  GEN_OFFSET(pal_pipe),
  GEN_OFFSET(lcd.REMY_LD0n),
  GEN_OFFSET(lcd.RAVO_LD1n),
  GEN_OFFSET(lcd),
  GEN_OFFSET(reg_lx),
  GEN_OFFSET(reg_ly),
  GEN_OFFSET(reg_lyc),
  GEN_OFFSET(reg_bgp),
  GEN_OFFSET(reg_obp0),
  GEN_OFFSET(reg_obp1),
  GEN_OFFSET(WODU_HBLANKp),
  GEN_OFFSET(SATO_BOOT_BITn),
  GEN_OFFSET(ATEJ_LINE_RSTp),
  GEN_OFFSET(flipped_sprite),
};

//-----------------------------------------------------------------------------

void print_field_at(int offset) {
  int len = sizeof(gb_offsets) / sizeof(gb_offsets[0]);

  int min_idx = -1;
  int min_delta = 1000000;

  for (int i = 0; i < len; i++) {
    int delta = offset - gb_offsets[i].offset;
    if (delta >= 0 && delta < min_delta) {
      min_idx = i;
      min_delta = delta;
    }
  }

  if (min_idx >= 0) {
    LOG_R("%12.12s + %d", gb_offsets[min_idx].name, min_delta);
  }
  else {
    LOG_R("<could not find field>");
  }
}

//-----------------------------------------------------------------------------

void GateBoyState::diff(const GateBoyState& gbb, uint8_t mask) const {
  const GateBoyState& gba = *this;

  TestResults results;

  uint8_t* bytes_a = (uint8_t*)&gba;
  uint8_t* bytes_b = (uint8_t*)&gbb;

  for (int i = 0; i < sizeof(GateBoyState); i++) {
    int byte_a = bytes_a[i] & mask;
    int byte_b = bytes_b[i] & mask;

    if (byte_a != byte_b) {
      LOG_R("MISMATCH @ %5d - ", i);
      print_field_at(i);
      LOG_R(": 0x%02x 0x%02x\n", byte_a, byte_b);
    }

    //EXPECT_EQ(byte_a, byte_b, ": [%5d] = 0x%02x, [%5d] = 0x%02x\n", ia, byte_a, ib, byte_b);
  }
}

//-----------------------------------------------------------------------------