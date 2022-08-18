#include "GateBoyLib/LogicBoyState.h"
#include "GateBoyLib/GateBoyState.h"

#include "GateBoyLib/Utils.h"
#include "CoreLib/Constants.h"

#pragma warning(disable:4244) // conversion from uint32_t to uint8_t

//static_assert(sizeof(GateBoyReset) == sizeof(LogicBoyReset));

//-----------------------------------------------------------------------------

void LogicBoyState::reset() {
  memset(this, 0, sizeof(*this));
  GateBoyState gb_state;
  gb_state.reset();
  from_gb_state(gb_state);

  phase_lcd = 140329;
  phase_tfetch = 10;
  phase_sfetch = 10;
}

//-----------------------------------------------------------------------------

GBResult LogicBoyState::peek(int addr) const {
  switch(addr) {
  case ADDR_P1  : break; 
  case ADDR_SB  : break;
  case ADDR_SC  : break;
  case ADDR_DIV : return GBResult(uint8_t(reg_div >> 6));
  case ADDR_TIMA: return GBResult(reg_tima);
  case ADDR_TMA : return GBResult(reg_tma);
  case ADDR_TAC : return GBResult(reg_tac);
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
  case ADDR_LCDC: return GBResult(~reg_lcdc);
  case ADDR_STAT: break;
  case ADDR_SCY : return GBResult(~reg_scy);
  case ADDR_SCX : return GBResult(~reg_scx);
  case ADDR_LY  : return GBResult(~reg_ly);
  case ADDR_LYC : return GBResult(~reg_lyc);
  case ADDR_DMA : return GBResult(~reg_dma);
  case ADDR_BGP : return GBResult(~reg_bgp);
  case ADDR_OBP0: return GBResult(~reg_obp0);
  case ADDR_OBP1: return GBResult(~reg_obp1);
  case ADDR_WY  : return GBResult(~reg_wy);
  case ADDR_WX  : return GBResult(~reg_wx);
  }

  LOG_R("LogicBoy::peek - bad address 0x%04x\n", addr);
  return Error::NOT_FOUND;
}

//-----------------------------------------------------------------------------

GBResult LogicBoyState::poke(int addr, uint8_t data_in) {
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
  case ADDR_LCDC: { reg_lcdc = ~data_in; return GBResult::ok(); }
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

  LOG_R("LogicBoy::poke - bad address 0x%04x\n", addr);
  return Error::NOT_FOUND;
}

//-----------------------------------------------------------------------------

void LogicBoyState::to_gb_state(GateBoyState& dst) const {
  const LogicBoyState& src = *this;

  dst.SIG_VCC.state = 1;
  dst.SIG_GND.state = 0;

  bit_unpack(dst.reg_joy, src.reg_joy);
  //bit_unpack(dst.reg_sb, src.reg_sb);
  //bit_unpack(dst.reg_sc, src.reg_sc);
  bit_unpack(dst.reg_div, src.reg_div);
  bit_unpack(dst.reg_tima, src.reg_tima);
  bit_unpack(dst.reg_tma, src.reg_tma);
  bit_unpack(dst.reg_tac, src.reg_tac);
  bit_unpack(dst.reg_if, src.reg_if);
  bit_unpack(dst.reg_lcdc, ~src.reg_lcdc);
  bit_unpack(dst.reg_stat, ~src.reg_stat);
  bit_unpack(dst.reg_scy, ~src.reg_scy);
  bit_unpack(dst.reg_scx, ~src.reg_scx);
  bit_unpack(dst.reg_ly, src.reg_ly);
  bit_unpack(dst.reg_lyc, ~src.reg_lyc);
  bit_unpack(dst.reg_dma, ~src.reg_dma);
  bit_unpack(dst.reg_bgp, ~src.reg_bgp);
  bit_unpack(dst.reg_obp0, ~src.reg_obp0);
  bit_unpack(dst.reg_obp1, ~src.reg_obp1);
  bit_unpack(dst.reg_wy, ~src.reg_wy);
  bit_unpack(dst.reg_wx, ~src.reg_wx);
  bit_unpack(dst.reg_lx, src.reg_lx);
  bit_unpack(dst.reg_ie, src.reg_ie);
  bit_unpack(dst.sys_rst, bit_pack(src.sys_rst));
  bit_unpack(dst.sys_clk, bit_pack(src.sys_clk));
  bit_unpack(dst.VOGA_HBLANKp_evn, bit_pack(src.VOGA_HBLANKp));
  bit_unpack(dst.XYMU_RENDERING_LATCHn, bit_pack(src.XYMU_RENDERINGn));
  bit_unpack(dst.MATU_DMA_RUNNINGp_odd, bit_pack(src.MATU_DMA_RUNNINGp));
  bit_unpack(dst.ACYL_SCANNINGp_odd, bit_pack(src.ACYL_SCANNINGp_odd));
  bit_unpack(dst.WODU_HBLANK_GATEp_odd, bit_pack(src.WODU_HBLANKp_odd));
  bit_unpack(dst.ATEJ_LINE_RST_TRIGp_odd, bit_pack(src.ATEJ_LINE_RSTp_odd));
  bit_unpack(dst.FEPO_STORE_MATCHp_odd, bit_pack(src.FEPO_STORE_MATCHp));
  bit_unpack(dst.cpu_signals, bit_pack(src.cpu_signals));
  bit_unpack(dst.cpu_abus, src.cpu_abus);
  bit_unpack(dst.cpu_dbus, src.cpu_dbus);
  bit_unpack(dst.int_ctrl, bit_pack(src.int_ctrl));
  bit_unpack(dst.int_latch, src.int_latch);
  bit_unpack(dst.vram_abus, src.vram_abus);
  bit_unpack(dst.vram_dbus, src.vram_dbus);
  bit_unpack(dst.oam_ctrl, bit_pack(src.oam_ctrl));
  bit_unpack(dst.oam_abus, src.oam_abus);
  bit_unpack(dst.oam_dbus_a, src.oam_dbus_a);
  bit_unpack(dst.oam_dbus_b, src.oam_dbus_b);
  bit_unpack(dst.oam_latch_a, src.oam_latch_a);
  bit_unpack(dst.oam_latch_b, src.oam_latch_b);
  bit_unpack(dst.oam_temp_a, ~src.oam_temp_a);
  bit_unpack(dst.oam_temp_b, ~src.oam_temp_b);
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
  bit_unpack(dst.sprite_scanner, bit_pack(src.sprite_scanner));
  bit_unpack(dst.scan_counter, src.scan_counter);
  bit_unpack(dst.sprite_counter, src.sprite_counter);
  bit_unpack(dst.sprite_index, src.sprite_index);
  bit_unpack(dst.sprite_match_flags_odd, src.sprite_match_flags);
  bit_unpack(dst.sprite_reset_flags_odd, src.sprite_reset_flags);
  bit_unpack(dst.sprite_ibus, src.sprite_ibus);
  bit_unpack(dst.sprite_lbus, src.sprite_lbus);
  bit_unpack(dst.store_i0, ~src.store_i[0]);
  bit_unpack(dst.store_i1, ~src.store_i[1]);
  bit_unpack(dst.store_i2, ~src.store_i[2]);
  bit_unpack(dst.store_i3, ~src.store_i[3]);
  bit_unpack(dst.store_i4, ~src.store_i[4]);
  bit_unpack(dst.store_i5, ~src.store_i[5]);
  bit_unpack(dst.store_i6, ~src.store_i[6]);
  bit_unpack(dst.store_i7, ~src.store_i[7]);
  bit_unpack(dst.store_i8, ~src.store_i[8]);
  bit_unpack(dst.store_i9, ~src.store_i[9]);
  bit_unpack(dst.store_l0, ~src.store_l[0]);
  bit_unpack(dst.store_l1, ~src.store_l[1]);
  bit_unpack(dst.store_l2, ~src.store_l[2]);
  bit_unpack(dst.store_l3, ~src.store_l[3]);
  bit_unpack(dst.store_l4, ~src.store_l[4]);
  bit_unpack(dst.store_l5, ~src.store_l[5]);
  bit_unpack(dst.store_l6, ~src.store_l[6]);
  bit_unpack(dst.store_l7, ~src.store_l[7]);
  bit_unpack(dst.store_l8, ~src.store_l[8]);
  bit_unpack(dst.store_l9, ~src.store_l[9]);
  bit_unpack(dst.store_x0, ~src.store_x[0]);
  bit_unpack(dst.store_x1, ~src.store_x[1]);
  bit_unpack(dst.store_x2, ~src.store_x[2]);
  bit_unpack(dst.store_x3, ~src.store_x[3]);
  bit_unpack(dst.store_x4, ~src.store_x[4]);
  bit_unpack(dst.store_x5, ~src.store_x[5]);
  bit_unpack(dst.store_x6, ~src.store_x[6]);
  bit_unpack(dst.store_x7, ~src.store_x[7]);
  bit_unpack(dst.store_x8, ~src.store_x[8]);
  bit_unpack(dst.store_x9, ~src.store_x[9]);
  bit_unpack(dst.sfetch_counter_evn, src.sfetch_counter_evn);
  bit_unpack(dst.sfetch_control, bit_pack(src.sfetch_control));
  bit_unpack(dst.tfetch_counter, src.tfetch_counter_odd);
  bit_unpack(dst.tfetch_control, bit_pack(src.tfetch_control));
  bit_unpack(dst.tile_temp_a, src.tile_temp_a);
  bit_unpack(dst.tile_temp_b, src.tile_temp_b);
  bit_unpack(dst.win_ctrl, bit_pack(src.win_ctrl));
  bit_unpack(dst.win_x.map, src.win_x.map);
  bit_unpack(dst.win_y.tile, src.win_y.tile);
  bit_unpack(dst.win_y.map, src.win_y.map);
  bit_unpack(dst.fine_count_odd, src.fine_count_odd);
  bit_unpack(dst.fine_scroll, bit_pack(src.fine_scroll));
  bit_unpack(dst.flipped_sprite, src.flipped_sprite);
  bit_unpack(dst.sprite_pix_a, ~src.sprite_pix_a);
  bit_unpack(dst.sprite_pix_b, ~src.sprite_pix_b);
  bit_unpack(dst.pix_count, src.pix_count);
  bit_unpack(dst.mask_pipe, src.mask_pipe);
  bit_unpack(dst.bgw_pipe_a, src.bgw_pipe_a);
  bit_unpack(dst.bgw_pipe_b, src.bgw_pipe_b);
  bit_unpack(dst.spr_pipe_a, src.spr_pipe_a);
  bit_unpack(dst.spr_pipe_b, src.spr_pipe_b);
  bit_unpack(dst.pal_pipe, src.pal_pipe);
  bit_unpack(dst.lcd, bit_pack(src.lcd));

  dst.ch1 = src.ch1;
  dst.ch2 = src.ch2;
  dst.ch3 = src.ch3;
  dst.ch4 = src.ch4;
  dst.spu = src.spu;
  dst.wave_dbus = src.wave_dbus;
}

//-----------------------------------------------------------------------------

void LogicBoyState::from_gb_state(const GateBoyState& src) {
  LogicBoyState& dst = *this;
  
  dst.reg_joy = bit_pack(src.reg_joy);
  //dst.reg_sb = bit_pack(src.reg_sb);
  //dst.reg_sc = bit_pack(src.reg_sc);
  dst.reg_div = bit_pack(src.reg_div);
  dst.reg_tima = bit_pack(src.reg_tima);
  dst.reg_tma = bit_pack(src.reg_tma);
  dst.reg_tac = bit_pack(src.reg_tac);
  dst.reg_if = bit_pack(src.reg_if);
  dst.reg_lcdc = ~bit_pack(src.reg_lcdc);
  dst.reg_stat = ~bit_pack(src.reg_stat);
  dst.reg_scy = ~bit_pack(src.reg_scy);
  dst.reg_scx = ~bit_pack(src.reg_scx);
  dst.reg_ly = bit_pack(src.reg_ly);
  dst.reg_lyc = ~bit_pack(src.reg_lyc);
  dst.reg_dma = ~bit_pack(src.reg_dma);
  dst.reg_bgp = ~bit_pack(src.reg_bgp);
  dst.reg_obp0 = ~bit_pack(src.reg_obp0);
  dst.reg_obp1 = ~bit_pack(src.reg_obp1);
  dst.reg_wy = ~bit_pack(src.reg_wy);
  dst.reg_wx = ~bit_pack(src.reg_wx);
  dst.reg_lx = bit_pack(src.reg_lx);
  dst.reg_ie = bit_pack(src.reg_ie);

  dst.sys_rst = bit_purge(src.sys_rst);
  dst.sys_clk = bit_purge(src.sys_clk);

  dst.VOGA_HBLANKp = bit_pack(src.VOGA_HBLANKp_evn);
  dst.XYMU_RENDERINGn = bit_pack(src.XYMU_RENDERING_LATCHn);
  dst.MATU_DMA_RUNNINGp = bit_pack(src.MATU_DMA_RUNNINGp_odd);
  dst.ACYL_SCANNINGp_odd = bit_pack(src.ACYL_SCANNINGp_odd);
  dst.WODU_HBLANKp_odd = bit_pack(src.WODU_HBLANK_GATEp_odd);
  dst.ATEJ_LINE_RSTp_odd = bit_pack(src.ATEJ_LINE_RST_TRIGp_odd);
  dst.FEPO_STORE_MATCHp = bit_pack(src.FEPO_STORE_MATCHp_odd);
  dst.cpu_signals = bit_purge(src.cpu_signals);
  dst.cpu_abus = bit_pack(src.cpu_abus);
  dst.cpu_dbus = bit_pack(src.cpu_dbus);
  dst.int_ctrl = bit_purge(src.int_ctrl);
  dst.int_latch = bit_pack(src.int_latch);
  dst.vram_abus = bit_pack(src.vram_abus);
  dst.vram_dbus = bit_pack(src.vram_dbus);
  dst.oam_ctrl = bit_purge(src.oam_ctrl);
  dst.oam_abus = bit_pack(src.oam_abus);
  dst.oam_dbus_a = bit_pack(src.oam_dbus_a);
  dst.oam_dbus_b = bit_pack(src.oam_dbus_b);
  dst.oam_latch_a = bit_pack(src.oam_latch_a);
  dst.oam_latch_b = bit_pack(src.oam_latch_b);
  dst.oam_temp_a = ~bit_pack(src.oam_temp_a);
  dst.oam_temp_b = ~bit_pack(src.oam_temp_b);
  dst.ext_data_latch = bit_pack(src.ext_data_latch);
  dst.ext_addr_latch = bit_pack(src.ext_addr_latch);
  
  dst.ext_mbc = bit_purge(src.ext_mbc);
  dst.zram_bus = bit_purge(src.zram_bus);
  dst.dma_ctrl = bit_purge(src.dma_ctrl);

  dst.dma_lo = bit_pack(src.dma_lo);
  dst.cpu_int = bit_pack(src.cpu_int);
  dst.cpu_ack = bit_pack(src.cpu_ack);
  dst.joy_int = bit_purge(src.joy_int);
  dst.joy_latch = bit_pack(src.joy_latch);
  dst.sprite_scanner = bit_purge(src.sprite_scanner);
  dst.scan_counter = bit_pack(src.scan_counter);
  dst.sprite_counter = bit_pack(src.sprite_counter);
  dst.sprite_index = bit_pack(src.sprite_index);
  dst.sprite_match_flags = bit_pack(src.sprite_match_flags_odd);
  dst.sprite_reset_flags = bit_pack(src.sprite_reset_flags_odd);
  dst.sprite_ibus = bit_pack(src.sprite_ibus);
  dst.sprite_lbus = bit_pack(src.sprite_lbus);
  dst.store_i[0] = ~bit_pack(src.store_i0);
  dst.store_i[1] = ~bit_pack(src.store_i1);
  dst.store_i[2] = ~bit_pack(src.store_i2);
  dst.store_i[3] = ~bit_pack(src.store_i3);
  dst.store_i[4] = ~bit_pack(src.store_i4);
  dst.store_i[5] = ~bit_pack(src.store_i5);
  dst.store_i[6] = ~bit_pack(src.store_i6);
  dst.store_i[7] = ~bit_pack(src.store_i7);
  dst.store_i[8] = ~bit_pack(src.store_i8);
  dst.store_i[9] = ~bit_pack(src.store_i9);
  dst.store_l[0] = ~bit_pack(src.store_l0);
  dst.store_l[1] = ~bit_pack(src.store_l1);
  dst.store_l[2] = ~bit_pack(src.store_l2);
  dst.store_l[3] = ~bit_pack(src.store_l3);
  dst.store_l[4] = ~bit_pack(src.store_l4);
  dst.store_l[5] = ~bit_pack(src.store_l5);
  dst.store_l[6] = ~bit_pack(src.store_l6);
  dst.store_l[7] = ~bit_pack(src.store_l7);
  dst.store_l[8] = ~bit_pack(src.store_l8);
  dst.store_l[9] = ~bit_pack(src.store_l9);
  dst.store_x[0] = ~bit_pack(src.store_x0);
  dst.store_x[1] = ~bit_pack(src.store_x1);
  dst.store_x[2] = ~bit_pack(src.store_x2);
  dst.store_x[3] = ~bit_pack(src.store_x3);
  dst.store_x[4] = ~bit_pack(src.store_x4);
  dst.store_x[5] = ~bit_pack(src.store_x5);
  dst.store_x[6] = ~bit_pack(src.store_x6);
  dst.store_x[7] = ~bit_pack(src.store_x7);
  dst.store_x[8] = ~bit_pack(src.store_x8);
  dst.store_x[9] = ~bit_pack(src.store_x9);
  dst.sfetch_counter_evn = bit_pack(src.sfetch_counter_evn);
  dst.sfetch_control = bit_purge(src.sfetch_control);
  dst.tfetch_counter_odd = bit_pack(src.tfetch_counter);
  dst.tfetch_control = bit_purge(src.tfetch_control);
  dst.tile_temp_a = bit_pack(src.tile_temp_a);
  dst.tile_temp_b = bit_pack(src.tile_temp_b);
  dst.win_ctrl = bit_purge(src.win_ctrl);
  dst.win_x.map = bit_pack(src.win_x.map);
  dst.win_y.tile = bit_pack(src.win_y.tile);
  dst.win_y.map = bit_pack(src.win_y.map);
  dst.fine_count_odd = bit_pack(src.fine_count_odd);
  dst.fine_scroll = bit_purge(src.fine_scroll);
  dst.flipped_sprite = bit_pack(src.flipped_sprite);
  dst.sprite_pix_a = ~bit_pack(src.sprite_pix_a);
  dst.sprite_pix_b = ~bit_pack(src.sprite_pix_b);
  dst.pix_count = bit_pack(src.pix_count);
  dst.mask_pipe = bit_pack(src.mask_pipe);
  dst.bgw_pipe_a = bit_pack(src.bgw_pipe_a);
  dst.bgw_pipe_b = bit_pack(src.bgw_pipe_b);
  dst.spr_pipe_a = bit_pack(src.spr_pipe_a);
  dst.spr_pipe_b = bit_pack(src.spr_pipe_b);
  dst.pal_pipe = bit_pack(src.pal_pipe);
  dst.lcd = bit_purge(src.lcd);

  dst.ch1 = src.ch1;
  dst.ch2 = src.ch2;
  dst.ch3 = src.ch3;
  dst.ch4 = src.ch4;
  dst.spu = src.spu;
  dst.wave_dbus = src.wave_dbus;
}

//-----------------------------------------------------------------------------
