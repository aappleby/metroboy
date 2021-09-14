#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/GateBoy.h"

#pragma warning(disable:4244) // conversion from uint32_t to uint8_t

void GateBoyState::to_reg(GateBoyReg& dst) const {
  const GateBoyState& src = *this;

  dst.SIG_VCC = 1;
  dst.SIG_GND = 0;

  bit_unpack(dst.reg_joy, src.reg_joy);
  //bit_unpack(dst.reg_sb, src.reg_sb);
  //bit_unpack(dst.reg_sc, src.reg_sc);
  bit_unpack(dst.reg_div, src.reg_div);
  bit_unpack(dst.reg_tima, src.reg_tima);
  bit_unpack(dst.reg_tma, src.reg_tma);
  bit_unpack(dst.reg_tac, src.reg_tac);
  bit_unpack(dst.reg_if, src.reg_if);
  bit_unpack(dst.reg_lcdc, src.reg_lcdc);
  bit_unpack(dst.reg_stat, src.reg_stat);
  bit_unpack(dst.reg_scy, src.reg_scy);
  bit_unpack(dst.reg_scx, src.reg_scx);
  bit_unpack(dst.reg_ly, src.reg_ly);
  bit_unpack(dst.reg_lyc, src.reg_lyc);
  bit_unpack(dst.reg_dma, src.reg_dma);
  bit_unpack(dst.reg_bgp, src.reg_bgp);
  bit_unpack(dst.reg_obp0, src.reg_obp0);
  bit_unpack(dst.reg_obp1, src.reg_obp1);
  bit_unpack(dst.reg_wy, src.reg_wy);
  bit_unpack(dst.reg_wx, src.reg_wx);
  bit_unpack(dst.reg_lx, src.reg_lx);
  bit_unpack(dst.reg_ie, src.reg_ie);
  bit_unpack(dst.sys_rst, bit_pack(src.sys_rst));
  bit_unpack(dst.sys_clk, bit_pack(src.sys_clk));
  bit_unpack(dst.VOGA_HBLANKp, bit_pack(src.VOGA_HBLANKp));
  bit_unpack(dst.XYMU_RENDERINGn, bit_pack(src.XYMU_RENDERINGn));
  bit_unpack(dst.MATU_DMA_RUNNINGp, bit_pack(src.MATU_DMA_RUNNINGp));
  bit_unpack(dst.ACYL_SCANNINGp, bit_pack(src.ACYL_SCANNINGp));
  bit_unpack(dst.WODU_HBLANKp, bit_pack(src.WODU_HBLANKp));
  bit_unpack(dst.SATO_BOOT_BITn, bit_pack(src.SATO_BOOT_BITn));
  bit_unpack(dst.ATEJ_LINE_RSTp, bit_pack(src.ATEJ_LINE_RSTp));
  bit_unpack(dst.FEPO_STORE_MATCHp, bit_pack(src.FEPO_STORE_MATCHp));
  bit_unpack(dst.cpu_signals, bit_pack(src.cpu_signals));
  bit_unpack(dst.cpu_abus, src.cpu_abus);
  bit_unpack(dst.cpu_dbus, src.cpu_dbus);
  bit_unpack(dst.int_ctrl, bit_pack(src.int_ctrl));
  bit_unpack(dst.int_latch, src.int_latch);
  bit_unpack(dst.vram_abus, src.vram_abus);
  bit_unpack(dst.vram_dbus, src.vram_dbus);
  bit_unpack(dst.vram_ext_ctrl, bit_pack(src.vram_ext_ctrl));
  bit_unpack(dst.vram_ext_abus, src.vram_ext_abus);
  bit_unpack(dst.vram_ext_dbus, src.vram_ext_dbus);
  bit_unpack(dst.oam_ctrl, bit_pack(src.oam_ctrl));
  bit_unpack(dst.oam_abus, src.oam_abus);
  bit_unpack(dst.oam_dbus_a, src.oam_dbus_a);
  bit_unpack(dst.oam_dbus_b, src.oam_dbus_b);
  bit_unpack(dst.oam_latch_a, src.oam_latch_a);
  bit_unpack(dst.oam_latch_b, src.oam_latch_b);
  bit_unpack(dst.oam_temp_a, src.oam_temp_a);
  bit_unpack(dst.oam_temp_b, src.oam_temp_b);
  bit_unpack(dst.ext_ctrl, bit_pack(src.ext_ctrl));
  bit_unpack(dst.ext_abus.lo, src.ext_abus.lo);
  bit_unpack(dst.ext_abus.hi, src.ext_abus.hi);
  bit_unpack(dst.ext_dbus, src.ext_dbus);
  bit_unpack(dst.ext_data_latch, src.ext_data_latch);
  bit_unpack(dst.ext_addr_latch, src.ext_addr_latch);
  bit_unpack(dst.ext_mbc, bit_pack(src.ext_mbc));
  bit_unpack(dst.zram_bus, bit_pack(src.zram_bus));
  bit_unpack(dst.dma_ctrl, bit_pack(src.dma_ctrl));
  bit_unpack(dst.dma_lo, src.dma_lo);
  bit_unpack(dst.cpu_int, src.cpu_int);
  bit_unpack(dst.cpu_ack, src.cpu_ack);
  bit_unpack(dst.joy_int, bit_pack(src.joy_int));
  bit_unpack(dst.joy_latch, src.joy_latch);
  bit_unpack(dst.joy_ext, src.joy_ext);
  bit_unpack(dst.sprite_scanner, bit_pack(src.sprite_scanner));
  bit_unpack(dst.scan_counter, src.scan_counter);
  bit_unpack(dst.sprite_counter, src.sprite_counter);
  bit_unpack(dst.sprite_index, src.sprite_index);
  bit_unpack(dst.sprite_match_flags, src.sprite_match_flags);
  bit_unpack(dst.sprite_reset_flags, src.sprite_reset_flags);
  bit_unpack(dst.sprite_store_flags, src.sprite_store_flags);
  bit_unpack(dst.sprite_ibus, src.sprite_ibus);
  bit_unpack(dst.sprite_lbus, src.sprite_lbus);
  bit_unpack(dst.store_i0, src.store_i0);
  bit_unpack(dst.store_i1, src.store_i1);
  bit_unpack(dst.store_i2, src.store_i2);
  bit_unpack(dst.store_i3, src.store_i3);
  bit_unpack(dst.store_i4, src.store_i4);
  bit_unpack(dst.store_i5, src.store_i5);
  bit_unpack(dst.store_i6, src.store_i6);
  bit_unpack(dst.store_i7, src.store_i7);
  bit_unpack(dst.store_i8, src.store_i8);
  bit_unpack(dst.store_i9, src.store_i9);
  bit_unpack(dst.store_l0, src.store_l0);
  bit_unpack(dst.store_l1, src.store_l1);
  bit_unpack(dst.store_l2, src.store_l2);
  bit_unpack(dst.store_l3, src.store_l3);
  bit_unpack(dst.store_l4, src.store_l4);
  bit_unpack(dst.store_l5, src.store_l5);
  bit_unpack(dst.store_l6, src.store_l6);
  bit_unpack(dst.store_l7, src.store_l7);
  bit_unpack(dst.store_l8, src.store_l8);
  bit_unpack(dst.store_l9, src.store_l9);
  bit_unpack(dst.store_x0, src.store_x0);
  bit_unpack(dst.store_x1, src.store_x1);
  bit_unpack(dst.store_x2, src.store_x2);
  bit_unpack(dst.store_x3, src.store_x3);
  bit_unpack(dst.store_x4, src.store_x4);
  bit_unpack(dst.store_x5, src.store_x5);
  bit_unpack(dst.store_x6, src.store_x6);
  bit_unpack(dst.store_x7, src.store_x7);
  bit_unpack(dst.store_x8, src.store_x8);
  bit_unpack(dst.store_x9, src.store_x9);
  bit_unpack(dst.sfetch_counter, src.sfetch_counter);
  bit_unpack(dst.sfetch_control, bit_pack(src.sfetch_control));
  bit_unpack(dst.tfetch_counter, src.tfetch_counter);
  bit_unpack(dst.tfetch_control, bit_pack(src.tfetch_control));
  bit_unpack(dst.tile_temp_a, src.tile_temp_a);
  bit_unpack(dst.tile_temp_b, src.tile_temp_b);
  bit_unpack(dst.win_ctrl, bit_pack(src.win_ctrl));
  bit_unpack(dst.win_x.map, src.win_x.map);
  bit_unpack(dst.win_y.tile, src.win_y.tile);
  bit_unpack(dst.win_y.map, src.win_y.map);
  bit_unpack(dst.fine_count, src.fine_count);
  bit_unpack(dst.fine_scroll, bit_pack(src.fine_scroll));
  bit_unpack(dst.flipped_sprite, src.flipped_sprite);
  bit_unpack(dst.sprite_pix_a, src.sprite_pix_a);
  bit_unpack(dst.sprite_pix_b, src.sprite_pix_b);
  bit_unpack(dst.pix_count, src.pix_count);
  bit_unpack(dst.mask_pipe, src.mask_pipe);
  bit_unpack(dst.bgw_pipe_a, src.bgw_pipe_a);
  bit_unpack(dst.bgw_pipe_b, src.bgw_pipe_b);
  bit_unpack(dst.spr_pipe_a, src.spr_pipe_a);
  bit_unpack(dst.spr_pipe_b, src.spr_pipe_b);
  bit_unpack(dst.pal_pipe, src.pal_pipe);
  bit_unpack(dst.lcd, bit_pack(src.lcd));
  //bit_unpack(dst.reg_NR10, src.reg_NR10);
  //bit_unpack(dst.reg_NR11, src.reg_NR11);
  //bit_unpack(dst.reg_NR12, src.reg_NR12);
  //bit_unpack(dst.reg_NR14, src.reg_NR14);
  //bit_unpack(dst.reg_NR21, src.reg_NR21);
  //bit_unpack(dst.reg_NR22, src.reg_NR22);
  //bit_unpack(dst.reg_NR24, src.reg_NR24);
  //bit_unpack(dst.reg_NR30, src.reg_NR30);
  //bit_unpack(dst.reg_NR31, src.reg_NR31);
  //bit_unpack(dst.reg_NR32, src.reg_NR32);
  //bit_unpack(dst.reg_NR34, src.reg_NR34);
  //bit_unpack(dst.reg_NR41, src.reg_NR41);
  //bit_unpack(dst.reg_NR42, src.reg_NR42);
  //bit_unpack(dst.reg_NR43, src.reg_NR43);
  //bit_unpack(dst.reg_NR44, src.reg_NR44);
  //bit_unpack(dst.reg_NR50, src.reg_NR50);
  //bit_unpack(dst.reg_NR51, src.reg_NR51);
  //bit_unpack(dst.reg_NR52, src.reg_NR52);
}

void GateBoyState::from_reg(const GateBoyReg& src) {
  GateBoyState& dst = *this;
  
  dst.reg_joy = bit_pack(src.reg_joy);
  //dst.reg_sb = bit_pack(src.reg_sb);
  //dst.reg_sc = bit_pack(src.reg_sc);
  dst.reg_div = bit_pack(src.reg_div);
  dst.reg_tima = bit_pack(src.reg_tima);
  dst.reg_tma = bit_pack(src.reg_tma);
  dst.reg_tac = bit_pack(src.reg_tac);
  dst.reg_if = bit_pack(src.reg_if);
  dst.reg_lcdc = bit_pack(src.reg_lcdc);
  dst.reg_stat = bit_pack(src.reg_stat);
  dst.reg_scy = bit_pack(src.reg_scy);
  dst.reg_scx = bit_pack(src.reg_scx);
  dst.reg_ly = bit_pack(src.reg_ly);
  dst.reg_lyc = bit_pack(src.reg_lyc);
  dst.reg_dma = bit_pack(src.reg_dma);
  dst.reg_bgp = bit_pack(src.reg_bgp);
  dst.reg_obp0 = bit_pack(src.reg_obp0);
  dst.reg_obp1 = bit_pack(src.reg_obp1);
  dst.reg_wy = bit_pack(src.reg_wy);
  dst.reg_wx = bit_pack(src.reg_wx);
  dst.reg_lx = bit_pack(src.reg_lx);
  dst.reg_ie = bit_pack(src.reg_ie);

  memcpy(&dst.sys_rst, &src.sys_rst, sizeof(GateBoyResetDebug));

  dst.sys_clk = (src.sys_clk);
  dst.VOGA_HBLANKp = bit_pack(src.VOGA_HBLANKp);
  dst.XYMU_RENDERINGn = bit_pack(src.XYMU_RENDERINGn);
  dst.MATU_DMA_RUNNINGp = bit_pack(src.MATU_DMA_RUNNINGp);
  dst.ACYL_SCANNINGp = bit_pack(src.ACYL_SCANNINGp);
  dst.WODU_HBLANKp = bit_pack(src.WODU_HBLANKp);
  dst.SATO_BOOT_BITn = bit_pack(src.SATO_BOOT_BITn);
  dst.ATEJ_LINE_RSTp = bit_pack(src.ATEJ_LINE_RSTp);
  dst.FEPO_STORE_MATCHp = bit_pack(src.FEPO_STORE_MATCHp);
  dst.cpu_signals = (src.cpu_signals);
  dst.cpu_abus = bit_pack(src.cpu_abus);
  dst.cpu_dbus = bit_pack(src.cpu_dbus);
  dst.int_ctrl = (src.int_ctrl);
  dst.int_latch = bit_pack(src.int_latch);
  dst.vram_abus = bit_pack(src.vram_abus);
  dst.vram_dbus = bit_pack(src.vram_dbus);
  dst.vram_ext_ctrl = (src.vram_ext_ctrl);
  dst.vram_ext_abus = bit_pack(src.vram_ext_abus);
  dst.vram_ext_dbus = bit_pack(src.vram_ext_dbus);
  dst.oam_ctrl = (src.oam_ctrl);
  dst.oam_abus = bit_pack(src.oam_abus);
  dst.oam_dbus_a = bit_pack(src.oam_dbus_a);
  dst.oam_dbus_b = bit_pack(src.oam_dbus_b);
  dst.oam_latch_a = bit_pack(src.oam_latch_a);
  dst.oam_latch_b = bit_pack(src.oam_latch_b);
  dst.oam_temp_a = bit_pack(src.oam_temp_a);
  dst.oam_temp_b = bit_pack(src.oam_temp_b);
  dst.ext_ctrl = (src.ext_ctrl);
  dst.ext_abus.lo = bit_pack(src.ext_abus.lo);
  dst.ext_abus.hi = bit_pack(src.ext_abus.hi);
  dst.ext_dbus = bit_pack(src.ext_dbus);
  dst.ext_data_latch = bit_pack(src.ext_data_latch);
  dst.ext_addr_latch = bit_pack(src.ext_addr_latch);
  dst.ext_mbc = (src.ext_mbc);
  dst.zram_bus = (src.zram_bus);
  dst.dma_ctrl = (src.dma_ctrl);
  dst.dma_lo = bit_pack(src.dma_lo);
  dst.cpu_int = bit_pack(src.cpu_int);
  dst.cpu_ack = bit_pack(src.cpu_ack);
  dst.joy_int = (src.joy_int);
  dst.joy_latch = bit_pack(src.joy_latch);
  dst.joy_ext = bit_pack(src.joy_ext);
  dst.sprite_scanner = (src.sprite_scanner);
  dst.scan_counter = bit_pack(src.scan_counter);
  dst.sprite_counter = bit_pack(src.sprite_counter);
  dst.sprite_index = bit_pack(src.sprite_index);
  dst.sprite_match_flags = bit_pack(src.sprite_match_flags);
  dst.sprite_reset_flags = bit_pack(src.sprite_reset_flags);
  dst.sprite_store_flags = bit_pack(src.sprite_store_flags);
  dst.sprite_ibus = bit_pack(src.sprite_ibus);
  dst.sprite_lbus = bit_pack(src.sprite_lbus);
  dst.store_i0 = bit_pack(src.store_i0);
  dst.store_i1 = bit_pack(src.store_i1);
  dst.store_i2 = bit_pack(src.store_i2);
  dst.store_i3 = bit_pack(src.store_i3);
  dst.store_i4 = bit_pack(src.store_i4);
  dst.store_i5 = bit_pack(src.store_i5);
  dst.store_i6 = bit_pack(src.store_i6);
  dst.store_i7 = bit_pack(src.store_i7);
  dst.store_i8 = bit_pack(src.store_i8);
  dst.store_i9 = bit_pack(src.store_i9);
  dst.store_l0 = bit_pack(src.store_l0);
  dst.store_l1 = bit_pack(src.store_l1);
  dst.store_l2 = bit_pack(src.store_l2);
  dst.store_l3 = bit_pack(src.store_l3);
  dst.store_l4 = bit_pack(src.store_l4);
  dst.store_l5 = bit_pack(src.store_l5);
  dst.store_l6 = bit_pack(src.store_l6);
  dst.store_l7 = bit_pack(src.store_l7);
  dst.store_l8 = bit_pack(src.store_l8);
  dst.store_l9 = bit_pack(src.store_l9);
  dst.store_x0 = bit_pack(src.store_x0);
  dst.store_x1 = bit_pack(src.store_x1);
  dst.store_x2 = bit_pack(src.store_x2);
  dst.store_x3 = bit_pack(src.store_x3);
  dst.store_x4 = bit_pack(src.store_x4);
  dst.store_x5 = bit_pack(src.store_x5);
  dst.store_x6 = bit_pack(src.store_x6);
  dst.store_x7 = bit_pack(src.store_x7);
  dst.store_x8 = bit_pack(src.store_x8);
  dst.store_x9 = bit_pack(src.store_x9);
  dst.sfetch_counter = bit_pack(src.sfetch_counter);
  dst.sfetch_control = (src.sfetch_control);
  dst.tfetch_counter = bit_pack(src.tfetch_counter);
  dst.tfetch_control = (src.tfetch_control);
  dst.tile_temp_a = bit_pack(src.tile_temp_a);
  dst.tile_temp_b = bit_pack(src.tile_temp_b);
  dst.win_ctrl = (src.win_ctrl);
  dst.win_x.map = bit_pack(src.win_x.map);
  dst.win_y.tile = bit_pack(src.win_y.tile);
  dst.win_y.map = bit_pack(src.win_y.map);
  dst.fine_count = bit_pack(src.fine_count);
  dst.fine_scroll = (src.fine_scroll);
  dst.flipped_sprite = bit_pack(src.flipped_sprite);
  dst.sprite_pix_a = bit_pack(src.sprite_pix_a);
  dst.sprite_pix_b = bit_pack(src.sprite_pix_b);
  dst.pix_count = bit_pack(src.pix_count);
  dst.mask_pipe = bit_pack(src.mask_pipe);
  dst.bgw_pipe_a = bit_pack(src.bgw_pipe_a);
  dst.bgw_pipe_b = bit_pack(src.bgw_pipe_b);
  dst.spr_pipe_a = bit_pack(src.spr_pipe_a);
  dst.spr_pipe_b = bit_pack(src.spr_pipe_b);
  dst.pal_pipe = bit_pack(src.pal_pipe);
  dst.lcd = (src.lcd);
  //dst.reg_NR10 = (src.reg_NR10);
  //dst.reg_NR11 = (src.reg_NR11);
  //dst.reg_NR12 = (src.reg_NR12);
  //dst.reg_NR14 = (src.reg_NR14);
  //dst.reg_NR21 = (src.reg_NR21);
  //dst.reg_NR22 = (src.reg_NR22);
  //dst.reg_NR24 = (src.reg_NR24);
  //dst.reg_NR30 = (src.reg_NR30);
  //dst.reg_NR31 = (src.reg_NR31);
  //dst.reg_NR32 = (src.reg_NR32);
  //dst.reg_NR34 = (src.reg_NR34);
  //dst.reg_NR41 = (src.reg_NR41);
  //dst.reg_NR42 = (src.reg_NR42);
  //dst.reg_NR43 = (src.reg_NR43);
  //dst.reg_NR44 = (src.reg_NR44);
  //dst.reg_NR50 = (src.reg_NR50);
  //dst.reg_NR51 = (src.reg_NR51);
  //dst.reg_NR52 = (src.reg_NR52);
}