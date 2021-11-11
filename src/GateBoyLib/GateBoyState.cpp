#include "GateBoyLib/GateBoyState.h"

#include "GateBoyLib/Utils.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

wire GateBoyState::TEXO_ADDR_VRAMn_new   () const { /*#p08.TEXO*/ return and2(cpu_signals.SIG_IN_CPU_EXT_BUSp.out_any(), cpu_abus.TEVY_ADDR_VRAMn_new()); }
wire GateBoyState::TEFA_ADDR_VRAMp_new   () const { /*#p25.TEFA*/ return nor2(cpu_abus.SYRO_FE00_FFFF_new(), TEXO_ADDR_VRAMn_new()); }
wire GateBoyState::SOSE_ADDR_VRAMp_new   () const { /*#p25.SOSE*/ return and2(TEFA_ADDR_VRAMp_new(), cpu_abus.BUS_CPU_A15p.out_any()); }
wire GateBoyState::LEVO_ADDR_VRAMn_new   () const { /*_p08.LEVO*/ return not1(TEXO_ADDR_VRAMn_new()); }
wire GateBoyState::TUJA_CPU_VRAM_WRp_new () const { /*_p25.TUJA*/ return and2(SOSE_ADDR_VRAMp_new(), cpu_signals.APOV_CPU_WRp.out_any()); }

wire GateBoyState::XODO_VID_RSTp_new() const { /*_p01.XODO*/ return nand2(sys_rst.XEBE_SYS_RSTn_new(), reg_lcdc.XONA_LCDC_LCDENn.qn_new()); }
wire GateBoyState::XAPO_VID_RSTn_new() const { /*_p01.XAPO*/ return not1(XODO_VID_RSTp_new()); }
wire GateBoyState::LYHA_VID_RSTp_new() const { /*_p01.LYHA*/ return not1(XAPO_VID_RSTn_new()); }
wire GateBoyState::LYFE_VID_RSTn_new() const { /*_p01.LYFE*/ return not1(LYHA_VID_RSTp_new()); }
wire GateBoyState::TOFU_VID_RSTp_new() const { /*_p01.TOFU*/ return not1(XAPO_VID_RSTn_new()); }
wire GateBoyState::ROSY_VID_RSTp_new() const { /*_p01.ROSY*/ return not1(XAPO_VID_RSTn_new()); }
wire GateBoyState::ATAR_VID_RSTp_new() const { /*#p01.ATAR*/ return not1(XAPO_VID_RSTn_new()); }
wire GateBoyState::ABEZ_VID_RSTn_new() const { /*#p01.ABEZ*/ return not1(ATAR_VID_RSTp_new()); }
wire GateBoyState::PYRY_VID_RSTp_new() const { /*_p01.PYRY*/ return not1(XAPO_VID_RSTn_new()); }
wire GateBoyState::AMYG_VID_RSTp_new() const { /*_p01.AMYG*/ return not1(XAPO_VID_RSTn_new()); }

wire GateBoyState::TOLE_CPU_VRAM_RDp_new() const
{
  // Ignoring debug for now
  ///*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
  ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
  ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
  ///*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = mux2p(TEFY_VRAM_MCSp, TUTO_DBG_VRAMp, TUCA_CPU_VRAM_RDp);

  /*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp_new = nand2(SOSE_ADDR_VRAMp_new(), cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new());
  /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp_new = not1(TUCA_CPU_VRAM_RDp_new);

  return TOLE_CPU_VRAM_RDp_new;
}

wire GateBoyState::SALE_CPU_VRAM_WRn_new() const
{
  // Ignoring debug for now
  ///*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_IN_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
  ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
  ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
  ///*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2p(TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);

  /*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn_new = and2(SOSE_ADDR_VRAMp_new(), cpu_signals.SIG_IN_CPU_WRp.out_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
  /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn_new = not1(TEGU_CPU_VRAM_WRn_new);

  return SALE_CPU_VRAM_WRn_new;
}


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
  fine_count_odd.reset_to_poweron();
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
  VOGA_HBLANKp_evn.state = 0b00011010;
  XYMU_RENDERING_LATCHn.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  MATU_DMA_RUNNINGp_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  ACYL_SCANNINGp_odd.state = BIT_OLD | BIT_DRIVEN;
  WODU_HBLANK_GATEp_odd.state = BIT_OLD | BIT_DRIVEN;
  ATEJ_LINE_RST_TRIGp_odd.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
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
  fine_count_odd.reset_to_bootrom();
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
  
  VOGA_HBLANKp_evn.state = 0b00011011;
  XYMU_RENDERING_LATCHn.state = 0b00011001;
  MATU_DMA_RUNNINGp_odd.state = 0b00011000;
  ACYL_SCANNINGp_odd.state = 0b00011000;
  WODU_HBLANK_GATEp_odd.state = 0b00011001;
  ATEJ_LINE_RST_TRIGp_odd.state = 0b00011000;
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
  fine_count_odd.reset_to_cart();
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
  DECLARE_FIELD(GateBoyState, sprite_store_flags_evn),

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
  DECLARE_FIELD(GateBoyState, win_ctrl.PYNU_WIN_MODE_Ap_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.PUKU_WIN_HITn_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.RYDY_WIN_HITp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.SOVY_WIN_HITp_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.NOPA_WIN_MODE_Bp_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.PYCO_WIN_MATCHp_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.NUNU_WIN_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.REJO_WY_MATCH_LATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.SARY_WY_MATCHp_odd),
  DECLARE_FIELD(GateBoyState, win_ctrl.RYFA_WIN_FETCHn_A_evn),
  DECLARE_FIELD(GateBoyState, win_ctrl.RENE_WIN_FETCHn_B_evn),

  DECLARE_FIELD(GateBoyState, fine_count_odd),
  
  DECLARE_FIELD(GateBoyState, fine_scroll.PUXA_SCX_FINE_MATCH_evn),
  DECLARE_FIELD(GateBoyState, fine_scroll.NYZE_SCX_FINE_MATCH_odd),
  DECLARE_FIELD(GateBoyState, fine_scroll.ROXY_FINE_SCROLL_DONEn_evn),

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
  END_FIELDS()
};

//-----------------------------------------------------------------------------
