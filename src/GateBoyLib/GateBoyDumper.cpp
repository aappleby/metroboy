#include "GateBoyLib/GateBoy.h"

char cp_int(uint8_t state) {
  if (state & BIT_DRIVEN) return bit(state) ? '1' : '0';
  if (state & BIT_PULLED) return bit(state) ? '^' : 'v';
  return 'X';
}

void dump_slice_int(Dumper& d, const char* tag, const BitBase* bits, int bit_count) {
  d(tag);
  for (int i = bit_count - 1; i >= 0; i--) {
    d.add_char(cp_int(bits[i].state));
  }
  d.add_char('\n');
}

/*
void dump_slicen(Dumper& d, const char* tag, const BitBase* bits, int bit_count) {
  d(tag);
  for (int i = bit_count - 1; i >= 0; i--) {
    d.add_char(bits[i].cn());
  }
  d.add_char('\n');
}
*/

void GateBoy::dump_sys(Dumper& d) const {
  const char* phases[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  d             ("phase        : %s\n", phases[gbs.phase_total & 7]);
  d             ("sys_rst      : %d\n", gbs.sys_rst);
  d             ("sys_t1       : %d\n", gbs.sys_t1);
  d             ("sys_t2       : %d\n", gbs.sys_t2);
  d             ("sys_clken    : %d\n", gbs.sys_clken);
  d             ("sys_clkgood  : %d\n", gbs.sys_clkgood);
  d             ("sys_cpuready : %d\n", gbs.sys_clkreq);
  d             ("sys_cpu_en   : %d\n", gbs.sys_cpu_en);
  d             ("sys_fastboot : %d\n", gbs.sys_fastboot);
  d             ("sys_buttons  : %d\n", gbs.sys_buttons);
  d.dump_req    ("bus_req_new  : ", gbc.bus_req_new);
  d.dump_bytep  ("data_latch   : ", gbc.cpu_data_latch);
  d("\n");
  //d             ("gb_screen_x  : %d\n", gb_screen_x);
  //d             ("gb_screen_y  : %d\n", gb_screen_y);
  //d.dump_bitp   ("lcd_pix_lo   : ", lcd.lcd_pix_lo.state);
  //d.dump_bitp   ("lcd_pix_hi   : ", lcd.lcd_pix_hi.state);
  //d.dump_slice2p("lcd_pipe_lo  : ", lcd.lcd_pipe_lo, 8);
  //d.dump_slice2p("lcd_pipe_hi  : ", lcd.lcd_pipe_hi, 8);
  //d("\n");
  d             ("sim_time     : %f\n",      gbs.sim_time);
  d             ("phase_total  : %lld\n",    gbs.phase_total);
  d             ("phase_origin : %lld\n",    gbs.phase_origin);
  d             ("phase_delta  : %lld\n",    gbs.phase_total - gbs.phase_origin);
}

void GateBoy::dump_tile_fetcher(Dumper& d) {
  d.dump_bitp   ("POKY_PRELOAD_LATCHp : ", gbr.tile_fetcher.POKY_PRELOAD_LATCHp.state);
  d.dump_bitp   ("LONY_FETCHINGp      : ", gbr.tile_fetcher.LONY_FETCHINGp.state);
  d.dump_bitp   ("LOVY_FETCH_DONEp    : ", gbr.tile_fetcher.LOVY_FETCH_DONEp.state);
  d.dump_bitp   ("NYKA_FETCH_DONEp    : ", gbr.tile_fetcher.NYKA_FETCH_DONEp.state);
  d.dump_bitp   ("PORY_FETCH_DONEp    : ", gbr.tile_fetcher.PORY_FETCH_DONEp.state);
  d.dump_bitp   ("PYGO_FETCH_DONEp    : ", gbr.tile_fetcher.PYGO_FETCH_DONEp.state);
  d.dump_bitp   ("LAXU_BFETCH_S0p     : ", gbr.tile_fetcher.LAXU_BFETCH_S0p.state);
  d.dump_bitp   ("MESU_BFETCH_S1p     : ", gbr.tile_fetcher.MESU_BFETCH_S1p.state);
  d.dump_bitp   ("NYVA_BFETCH_S2p     : ", gbr.tile_fetcher.NYVA_BFETCH_S2p.state);
  d.dump_bitp   ("LYZU_BFETCH_S0p_D1  : ", gbr.tile_fetcher.LYZU_BFETCH_S0p_D1.state);
  d.dump_slice2n("Temp A : ", &gbr.tile_temp_a.LEGU_TILE_DA0n, 8);
  d.dump_slice2p("Temp B : ", &gbr.tile_temp_b.RAWU_TILE_DB0p, 8);
}

void GateBoy::dump_clocks(Dumper& d) {
  d.dump_bitp("AFUR_xxxxEFGHp : ", gbr.clk.AFUR_xxxxEFGH.state);
  d.dump_bitp("ALEF_AxxxxFGHp : ", gbr.clk.ALEF_AxxxxFGH.state);
  d.dump_bitp("APUK_ABxxxxGHp : ", gbr.clk.APUK_ABxxxxGH.state);
  d.dump_bitp("ADYK_ABCxxxxHp : ", gbr.clk.ADYK_ABCxxxxH.state);
  d("\n");
  d.dump_bitp("WUVU_ABxxEFxxp : ", gbr.clk.WUVU_ABxxEFxx.state);
  d.dump_bitp("VENA_xxCDEFxxp : ", gbr.clk.VENA_xxCDEFxx.state);
  d.dump_bitp("WOSU_AxxDExxHp : ", gbr.clk.WOSU_AxxDExxH.state);
}

void GateBoy::dump_interrupts(Dumper& d) {
  d.dump_slice2p("FF0F IF : ", &gbr.reg_if, 5);
  d.dump_slice2p("FFFF IE : ", &gbr.reg_ie, 5);
  d.dump_slice2p("LATCH   : ", &gbr.latch_if, 5);
  d.dump_slice2p("CPU_INT : ", &gbr.cpu_int, 5);
  d.dump_slice2p("CPU_ACK : ", &gbr.cpu_ack, 5);
}

void GateBoy::dump_joypad(Dumper& d) {
  d.dump_bitp("AWOB_WAKE_CPU   : ", gbr.int_ctrl.AWOB_WAKE_CPU.state);
  d.dump_bitp("SIG_CPU_WAKE    : ", gbr.int_ctrl.SIG_CPU_WAKE.state);
  d("\n");
  d.dump_bitp("PIN_67_JOY_P10   : ", gbr.joy_ext.PIN_67_JOY_P10.state);
  d.dump_bitp("PIN_66_JOY_P11   : ", gbr.joy_ext.PIN_66_JOY_P11.state);
  d.dump_bitp("PIN_65_JOY_P12   : ", gbr.joy_ext.PIN_65_JOY_P12.state);
  d.dump_bitp("PIN_64_JOY_P13   : ", gbr.joy_ext.PIN_64_JOY_P13.state);
  d.dump_bitp("PIN_63_JOY_P14   : ", gbr.joy_ext.PIN_63_JOY_P14.state);
  d.dump_bitp("PIN_62_JOY_P15   : ", gbr.joy_ext.PIN_62_JOY_P15.state);
  d("\n");
  d.dump_bitp("KEVU_JOYP_L0n   : ", gbr.joy_latch.KEVU_JOYP_L0n.state);
  d.dump_bitp("KAPA_JOYP_L1n   : ", gbr.joy_latch.KAPA_JOYP_L1n.state);
  d.dump_bitp("KEJA_JOYP_L2n   : ", gbr.joy_latch.KEJA_JOYP_L2n.state);
  d.dump_bitp("KOLO_JOYP_L3n   : ", gbr.joy_latch.KOLO_JOYP_L3n.state);
  d("\n");
  d.dump_bitp("BATU_JP_GLITCH0 : ", gbr.joy_int.BATU_JP_GLITCH0.state);
  d.dump_bitp("ACEF_JP_GLITCH1 : ", gbr.joy_int.ACEF_JP_GLITCH1.state);
  d.dump_bitp("AGEM_JP_GLITCH2 : ", gbr.joy_int.AGEM_JP_GLITCH2.state);
  d.dump_bitp("APUG_JP_GLITCH3 : ", gbr.joy_int.APUG_JP_GLITCH3.state);
  d("\n");
  //d.dump_bitp("JUTE_DBG_D0     : ", joy.JUTE_DBG_D0.state);
  //d.dump_bitp("KECY_DBG_D1     : ", joy.KECY_DBG_D1.state);
  //d.dump_bitp("JALE_DBG_D2     : ", joy.JALE_DBG_D2.state);
  //d.dump_bitp("KYME_DBG_D3     : ", joy.KYME_DBG_D3.state);
  d.dump_bitp("KELY_JOYP_UDLR  : ", gbr.joy_reg.KELY_JOYP_UDLRp.state);
  d.dump_bitp("COFY_JOYP_ABCS  : ", gbr.joy_reg.COFY_JOYP_ABCSp.state);
  //d.dump_bitp("KUKO_DBG_D6     : ", joy.KUKO_DBG_D6.state);
  //d.dump_bitp("KERU_DBG_D7     : ", joy.KERU_DBG_D7.state);
}

void GateBoy::dump_lcd(Dumper& d) {
  d.dump_bitp   ("CATU : ", gbr.lcd.CATU_x113p.state);
  d.dump_bitp   ("ANEL : ", gbr.lcd.ANEL_x113p.state);
  d.dump_bitp   ("POPU : ", gbr.lcd.POPU_y144p.state);
  d.dump_bitp   ("MYTA : ", gbr.lcd.MYTA_y153p.state);
  d.dump_bitp   ("RUTU : ", gbr.lcd.RUTU_x113p.state);
  d.dump_bitp   ("NYPE : ", gbr.lcd.NYPE_x113p.state);
  d("\n");
  d.dump_bitp   ("SYGU_LINE_STROBE    : ", gbr.lcd.SYGU_LINE_STROBE.state);
  d.dump_bitn   ("MEDA_VSYNC_OUTn     : ", gbr.lcd.MEDA_VSYNC_OUTn.state);
  d.dump_bitp   ("LUCA_LINE_EVENp     : ", gbr.lcd.LUCA_LINE_EVENp.state);
  d.dump_bitp   ("NAPO_FRAME_EVENp    : ", gbr.lcd.NAPO_FRAME_EVENp.state);
  d.dump_bitp   ("RUJU                : ", gbr.lcd.RUJU.state);
  d.dump_bitp   ("POFY                : ", gbr.lcd.POFY.state);
  d.dump_bitp   ("POME                : ", gbr.lcd.POME.state);
  d.dump_bitp   ("PAHO_X_8_SYNC       : ", gbr.lcd.PAHO_X_8_SYNC.state);
  d.dump_bitp   ("WUSA_LCD_CLOCK_GATE : ", gbr.lcd.WUSA_LCD_CLOCK_GATE.state);
  d("\n");
  d.dump_bitp   ("PIN_50_LCD_DATA1 : ", gbr.lcd.PIN_50_LCD_DATA1.state);
  d.dump_bitp   ("PIN_51_LCD_DATA0 : ", gbr.lcd.PIN_51_LCD_DATA0.state);
  d.dump_bitp   ("PIN_54_LCD_HSYNC : ", gbr.lcd.PIN_54_LCD_HSYNC.state);
  d.dump_bitp   ("PIN_56_LCD_FLIPS : ", gbr.lcd.PIN_56_LCD_FLIPS.state);
  d.dump_bitp   ("PIN_52_LCD_CNTRL : ", gbr.lcd.PIN_52_LCD_CNTRL.state);
  d.dump_bitp   ("PIN_55_LCD_LATCH : ", gbr.lcd.PIN_55_LCD_LATCH.state);
  d.dump_bitp   ("PIN_53_LCD_CLOCK : ", gbr.lcd.PIN_53_LCD_CLOCK.state);
  d.dump_bitp   ("PIN_57_LCD_VSYNC : ", gbr.lcd.PIN_57_LCD_VSYNC.state);
  d("\n");
  d.dump_slice2p("LX              : ", &gbr.reg_lx.SAXO_LX0p.state,  7);
  d.dump_slice2p("FF44 LY         : ", &gbr.reg_ly.MUWY_LY0p.state,  8);
  d.dump_slice2n("FF45 LYC        : ", &gbr.reg_lyc.SYRY_LYC0n.state, 8);
  d.dump_bitp   ("ROPO_LY_MATCH   : ", gbr.ROPO_LY_MATCH_SYNCp.state);
  d.dump_bitp   ("RUPO_LYC_MATCHn : ", gbr.RUPO_LYC_MATCHn.state);
}

void GateBoy::dump_oam_bus(Dumper& d) {
  d.dump_bitp   ("MAKA_LATCH_EXTp  : ", gbr.oam_ctrl.MAKA_LATCH_EXTp.state);
  d.dump_bitp   ("WUJE_CPU_OAM_WRn : ", gbr.oam_ctrl.WUJE_CPU_OAM_WRn.state);
  d.dump_bitp   ("SIG_OAM_CLKn     : ", gbr.oam_ctrl.SIG_OAM_CLKn.state);
  d.dump_bitp   ("SIG_OAM_WRn_A    : ", gbr.oam_ctrl.SIG_OAM_WRn_A.state);
  d.dump_bitp   ("SIG_OAM_WRn_B    : ", gbr.oam_ctrl.SIG_OAM_WRn_B.state);
  d.dump_bitp   ("SIG_OAM_OEn      : ", gbr.oam_ctrl.SIG_OAM_OEn.state);

  //dump_slicen(d, "BUS_OAM_An  : ", &oam_bus.BUS_OAM_A00n, 8);
  //dump_slicen(d, "BUS_OAM_DAn : ", &oam_bus.BUS_OAM_DA00n, 8);
  //dump_slicen(d, "BUS_OAM_DBn : ", &oam_bus.BUS_OAM_DB00n, 8);
  d.dump_slice2n("OAM LATCH A : ", &gbr.oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
  d.dump_slice2n("OAM LATCH B : ", &gbr.oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
  d.dump_slice2p("OAM TEMP A  : ", &gbr.oam_temp_a.XUSO_OAM_DA0p, 8);
  d.dump_slice2p("OAM TEMP B  : ", &gbr.oam_temp_b.YLOR_OAM_DB0p, 8);

}

void GateBoy::dump_sprite_store(Dumper& d) {
  //d.dump_slice2p("SPRITE INDEX   : ", &XADU_SPRITE_IDX0p.state, 6);
  d.dump_slice2p("SPRITE COUNT   : ", &gbr.sprite_counter.BESE_SPRITE_COUNT0, 4);
  d.dump_bitp   ("DEZY_STORE_ENn : ", gbr.DEZY_COUNT_CLKp.state);
  d("\n");
  d("STORE0 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.EBOJ_STORE0_RSTp.state, bit_pack_inv(gbr.store_i0), bit_pack_inv(gbr.store_l0), bit_pack_inv(gbr.store_x0));
  d("STORE1 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.CEDY_STORE1_RSTp.state, bit_pack_inv(gbr.store_i1), bit_pack_inv(gbr.store_l1), bit_pack_inv(gbr.store_x1));
  d("STORE2 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.EGAV_STORE2_RSTp.state, bit_pack_inv(gbr.store_i2), bit_pack_inv(gbr.store_l2), bit_pack_inv(gbr.store_x2));
  d("STORE3 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.GOTA_STORE3_RSTp.state, bit_pack_inv(gbr.store_i3), bit_pack_inv(gbr.store_l3), bit_pack_inv(gbr.store_x3));
  d("STORE4 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.XUDY_STORE4_RSTp.state, bit_pack_inv(gbr.store_i4), bit_pack_inv(gbr.store_l4), bit_pack_inv(gbr.store_x4));
  d("STORE5 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.WAFY_STORE5_RSTp.state, bit_pack_inv(gbr.store_i5), bit_pack_inv(gbr.store_l5), bit_pack_inv(gbr.store_x5));
  d("STORE6 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.WOMY_STORE6_RSTp.state, bit_pack_inv(gbr.store_i6), bit_pack_inv(gbr.store_l6), bit_pack_inv(gbr.store_x6));
  d("STORE7 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.WAPO_STORE7_RSTp.state, bit_pack_inv(gbr.store_i7), bit_pack_inv(gbr.store_l7), bit_pack_inv(gbr.store_x7));
  d("STORE8 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.EXUQ_STORE8_RSTp.state, bit_pack_inv(gbr.store_i8), bit_pack_inv(gbr.store_l8), bit_pack_inv(gbr.store_x8));
  d("STORE9 R%d I%02d L%02d X%03d\n", gbr.sprite_reset_flags.FONO_STORE9_RSTp.state, bit_pack_inv(gbr.store_i9), bit_pack_inv(gbr.store_l9), bit_pack_inv(gbr.store_x9));
}

void GateBoy::dump_mbc1(Dumper& d) {
  d.dump_bitp   ("MBC1_MODE    : ", gbr.ext_mbc.MBC1_MODE.state);
  d.dump_bitp   ("MBC1_RAM_EN  : ", gbr.ext_mbc.MBC1_RAM_EN.state);
  d.dump_slice2p("MBC1_BANK    : ", &gbr.ext_mbc.MBC1_BANK0, 7);
  d.dump_slice2p("MBC1_BANK_LO : ", &gbr.ext_mbc.MBC1_BANK0, 5);
  d.dump_slice2p("MBC1_BANK_HI : ", &gbr.ext_mbc.MBC1_BANK5, 2);
}

void GateBoy::dump_cpu_bus(Dumper& d) {
  d.dump_bitp   ("SIG_CPU_RDp       : ", gbr.cpu_signals.SIG_IN_CPU_RDp.state);
  d.dump_bitp   ("SIG_CPU_WRp       : ", gbr.cpu_signals.SIG_IN_CPU_WRp.state);
  d.dump_bitp   ("SIG_CPU_UNOR_DBG  : ", gbr.cpu_signals.SIG_CPU_UNOR_DBG.state);
  d.dump_bitp   ("SIG_CPU_ADDR_HIp  : ", gbr.cpu_signals.SIG_CPU_ADDR_HIp.state);
  d.dump_bitp   ("SIG_CPU_UMUT_DBG  : ", gbr.cpu_signals.SIG_CPU_UMUT_DBG.state);
  d.dump_bitp   ("SIG_CPU_EXT_BUSp  : ", gbr.cpu_signals.SIG_IN_CPU_EXT_BUSp.state);
  //d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
  d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", gbr.cpu_signals.SIG_IN_CPU_LATCH_EXT.state);
  d.dump_bitp   ("BOOT_BITn         : ", gbr.cpu_signals.TEPU_BOOT_BITn.state);
  d.dump_bitp   ("SIG_CPU_BOOTp     : ", gbr.cpu_signals.SIG_CPU_BOOTp.state);
  d.dump_bitp   ("TEDO_CPU_RDp      : ", gbr.cpu_signals.TEDO_CPU_RDp.state);
  d.dump_bitp   ("APOV_CPU_WRp      : ", gbr.cpu_signals.APOV_CPU_WRp.state);
  d.dump_bitp   ("TAPU_CPU_WRp      : ", gbr.cpu_signals.TAPU_CPU_WRp.state);
  d.dump_slice2p("BUS_CPU_A : ",     (BitBase*)&gbr.cpu_abus_new.BUS_CPU_A00p, 16);
  dump_slice_int(d,  "BUS_CPU_D : ", (BitBase*)&gbr.cpu_dbus_new.BUS_CPU_D00p, 8);
}

void GateBoy::dump_dma(Dumper& d) {
  d.dump_slice2p("DMA_A_LOW  : ", &gbr.dma_lo.NAKY_DMA_A00p, 8);
  d.dump_slice2n("DMA_A_HIGH : ", &gbr.dma_hi.NAFA_DMA_A08n, 8);
  d             ("DMA Addr   : 0x%02x:%02x\n", bit_pack_inv(gbr.dma_hi), bit_pack(gbr.dma_lo));
  d.dump_bitp   ("MATU_DMA_RUNNINGp : ", gbr.dma_ctrl.MATU_DMA_RUNNINGp.state);
  d.dump_bitp   ("LYXE_DMA_LATCHp   : ", gbr.dma_ctrl.LYXE_DMA_LATCHp  .state);
  d.dump_bitp   ("MYTE_DMA_DONE     : ", gbr.dma_ctrl.MYTE_DMA_DONE    .state);
  d.dump_bitp   ("LUVY_DMA_TRIG_d0  : ", gbr.dma_ctrl.LUVY_DMA_TRIG_d0 .state);
  d.dump_bitp   ("LENE_DMA_TRIG_d4  : ", gbr.dma_ctrl.LENE_DMA_TRIG_d4 .state);
  d.dump_bitp   ("LOKY_DMA_LATCHp   : ", gbr.dma_ctrl.LOKY_DMA_LATCHp  .state);
}


void GateBoy::dump_ext_bus(Dumper& d) {
  d.dump_slice2n("PIN_01_ADDR : ", &gbr.ext_abus.PIN_01_A00, 16);

  //d.dump_slice2n(   "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);
  //dump_slice2b  (d, "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);

  d.dump_bitn   ("PIN_80_CSn  : ", gbr.ext_ctrl.PIN_80_CSn.state);
  d.dump_bitn   ("PIN_79_RDn  : ", gbr.ext_ctrl.PIN_79_RDn.state);
  d.dump_bitn   ("PIN_78_WRn  : ", gbr.ext_ctrl.PIN_78_WRn.state);
  d.dump_slice2p("ADDR LATCH  : ", &gbr.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p, 15);
  d.dump_slice2n("DATA LATCH  : ", &gbr.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n, 8);
}

void GateBoy::dump_vram_bus(Dumper& d) {
  d.dump_bitp   ("PIN_43_VRAM_CSn  : ", gbr.vram_ext_ctrl.PIN_43_VRAM_CSn.state);
  d.dump_bitp   ("PIN_45_VRAM_OEn  : ", gbr.vram_ext_ctrl.PIN_45_VRAM_OEn.state);
  d.dump_bitp   ("PIN_49_VRAM_WRn  : ", gbr.vram_ext_ctrl.PIN_49_VRAM_WRn.state);
  d.dump_slice2p("PIN_34_VRAM_ADDR : ", &gbr.vram_ext_abus.PIN_34_VRAM_A00, 13);
  d.dump_slice2p("PIN_25_VRAM_DATA : ", &gbr.vram_ext_dbus.PIN_33_VRAM_D00, 8);
  d.dump_slice2n("BUS_VRAM_An      : ", &gbr.vram_abus.BUS_VRAM_A00n, 13);
  d.dump_slice2p("BUS_VRAM_Dp      : ", &gbr.vram_dbus.BUS_VRAM_D00p, 8);
}

void GateBoy::dump_sprite_fetcher(Dumper& d) {
  d.dump_bitp   ("TAKA_SFETCH_RUNNINGp : ", gbr.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state);
  d.dump_bitp   ("SOBU_SFETCH_REQp     : ", gbr.sprite_fetcher.SOBU_SFETCH_REQp    .state);
  d.dump_bitp   ("SUDA_SFETCH_REQp     : ", gbr.sprite_fetcher.SUDA_SFETCH_REQp    .state);
  d.dump_bitp   ("TOXE_SFETCH_S0       : ", gbr.sprite_fetcher.TOXE_SFETCH_S0p     .state);
  d.dump_bitp   ("TULY_SFETCH_S1       : ", gbr.sprite_fetcher.TULY_SFETCH_S1p     .state);
  d.dump_bitp   ("TESE_SFETCH_S2       : ", gbr.sprite_fetcher.TESE_SFETCH_S2p     .state);
  d.dump_bitp   ("TYFO_SFETCH_S0_D1    : ", gbr.sprite_fetcher.TYFO_SFETCH_S0p_D1  .state);
  d.dump_bitp   ("TOBU_SFETCH_S1_D2    : ", gbr.sprite_fetcher.TOBU_SFETCH_S1p_D2  .state);
  d.dump_bitp   ("VONU_SFETCH_S1_D4    : ", gbr.sprite_fetcher.VONU_SFETCH_S1p_D4  .state);
  d.dump_bitp   ("SEBA_SFETCH_S1_D5    : ", gbr.sprite_fetcher.SEBA_SFETCH_S1p_D5  .state);
  d.dump_slice2n("Temp A : ", &gbr.sprite_pix_a.REWO_SPRITE_DA0n, 8);
  d.dump_slice2n("Temp B : ", &gbr.sprite_pix_b.PEFO_SPRITE_DB0n, 8);
}

void GateBoy::dump_timer(Dumper& d) {
  d.dump_slice2p("DIV16 : ", &gbr.div.UKUP_DIV00p, 16);
  d.dump_slice2p("FF04 DIV  : ", &gbr.div.UGOT_DIV06p, 8);
  d.dump_slice2p("FF05 TIMA : ", &gbr.tima, 8);
  d.dump_slice2p("FF06 TMA  : ", &gbr.tma, 8);
  d.dump_slice2p("FF07 TAC  : ", &gbr.tac, 3);
  d.dump_bitp   ("NYDU_TIMA7p_DELAY    : ", gbr.int_ctrl.NYDU_TIMA7p_DELAY.state);
  d.dump_bitp   ("MOBA_TIMER_OVERFLOWp : ", gbr.int_ctrl.MOBA_TIMER_OVERFLOWp.state);
}

void GateBoy::dump_resets(Dumper& d) {
  d.dump_bitp("PIN_71_RST     : ", gbr.rst.PIN_71_RST.state);
  d.dump_bitp("PIN_77_T1      : ", gbr.rst.PIN_77_T1.state);
  d.dump_bitp("PIN_76_T2      : ", gbr.rst.PIN_76_T2.state);

  d.dump_bitp("TUBO_WAITINGp  : ", gbr.rst.TUBO_WAITINGp.state);
  d.dump_bitn("ASOL_POR_DONEn : ", gbr.rst.ASOL_POR_DONEn.state);
  d.dump_bitp("AFER_SYS_RSTp  : ", gbr.rst.AFER_SYS_RSTp.state);
  d.dump_bitp("SOTO_DBG_VRAMp : ", gbr.rst.SOTO_DBG_VRAMp.state);

  d.dump_bitp("SIG_CPU_EXT_CLKGOOD : ", gbr.rst.SIG_CPU_EXT_CLKGOOD.state);
  d.dump_bitp("SIG_CPU_EXT_RESETp  : ", gbr.rst.SIG_CPU_EXT_RESETp .state);
  d.dump_bitp("SIG_CPU_STARTp      : ", gbr.rst.SIG_CPU_STARTp     .state);
  d.dump_bitp("SIG_CPU_INT_RESETp  : ", gbr.rst.SIG_CPU_INT_RESETp .state);
}

void GateBoy::dump_sprite_scanner(Dumper& d) {
  d.dump_slice2p("SCAN INDEX        : ", &gbr.scan_counter.YFEL_SCAN0, 6);
  d.dump_bitp   ("BESU_SCANNINGp    : ", gbr.sprite_scanner.BESU_SCANNINGn.state);
  d.dump_bitp   ("CENO_SCANNINGp    : ", gbr.sprite_scanner.CENO_SCANNINGn.state);
  d.dump_bitp   ("BYBA_SCAN_DONE_Ap : ", gbr.sprite_scanner.BYBA_SCAN_DONE_Ap.state);
  d.dump_bitp   ("DOBA_SCAN_DONE_Bp : ", gbr.sprite_scanner.DOBA_SCAN_DONE_Bp.state);
}

void GateBoy::dump_serial(Dumper& d) {
#if 0
  d.dump_bitp   ("ETAF_SER_RUNNING : ", serial.ETAF_SER_RUN.state);
  d.dump_bitp   ("CULY_XFER_DIR    : ", serial.CULY_SER_DIR.state);
  d.dump_bitp   ("COTY_SER_CLK     : ", serial.COTY_SER_CLK.state);
  d.dump_bitp   ("ELYS_SER_OUT     : ", serial.ELYS_SER_OUT.state);
  d.dump_slice2p("CAFA_SER_CNT     : ", &serial.CAFA_SER_CNT0, 4);
  d.dump_slice2p("CUBA_SER_DATA    : ", &serial.CUBA_SER_DATA0, 8);
#endif
}

void GateBoy::dump_ppu(Dumper& d) {
  d.dump_slice2n("FF40 LCDC  : ", &gbr.reg_lcdc.VYXE_LCDC_BGENn, 8);
  d.dump_slice2n("FF41 STAT  : ", &gbr.reg_stat.ROXE_STAT_HBI_ENn, 4);
  d.dump_slice2n("FF42 SCY   : ", &gbr.reg_scy.GAVE_SCY0n, 8);
  d.dump_slice2n("FF43 SCX   : ", &gbr.reg_scx.DATY_SCX0n, 8);
  d.dump_slice2n("FF47 BGP   : ", &gbr.reg_bgp.PAVO_BGP_D0n, 8);
  d.dump_slice2n("FF48 OBP0  : ", &gbr.reg_obp0.XUFU_OBP0_D0n, 8);
  d.dump_slice2n("FF49 OBP1  : ", &gbr.reg_obp1.MOXY_OBP1_D0n, 8);
  d.dump_slice2n("FF4A WY    : ", &gbr.reg_wy.NESO_WY0n, 8);
  d.dump_slice2n("FF4B WX    : ", &gbr.reg_wx.MYPA_WX0n, 8);
  d.dump_slice2p("WIN MAP X  : ", &gbr.win_x.WYKA_WIN_MAP_X0, 5);
  d.dump_slice2p("WIN MAP Y  : ", &gbr.win_y.TUFU_WIN_MAP_Y0, 8);
  d.dump_slice2p("WIN TILE Y : ", &gbr.win_y.VYNO_WIN_TILE_Y0, 3);
  d("\n");
  d.dump_slice2p("PIX COUNT  : ", &gbr.pix_count.XEHO_PX0p, 8);
  d.dump_slice2p("BG PIPE A  : ", &gbr.bgw_pipe_a, 8);
  d.dump_slice2p("BG PIPE B  : ", &gbr.bgw_pipe_b, 8);
  d.dump_slice2p("SPR PIPE A : ", &gbr.spr_pipe_a, 8);
  d.dump_slice2p("SPR PIPE B : ", &gbr.spr_pipe_b, 8);
  d.dump_slice2p("PAL PIPE   : ", &gbr.pal_pipe, 8);
  d.dump_slice2p("MASK PIPE  : ", &gbr.mask_pipe, 8);
  d.dump_bitn   ("REMY_LD0n  : ", gbr.REMY_LD0n.state);
  d.dump_bitn   ("RAVO_LD1n  : ", gbr.RAVO_LD1n.state);
  d("\n");
  d.dump_bitp("XYMU_RENDERINGn        : ", gbr.XYMU_RENDERINGn.state);
  d.dump_bitp("PYNU_WIN_MODE_Ap       : ", gbr.win_reg.PYNU_WIN_MODE_Ap.state);
  d.dump_bitp("PUKU_WIN_HITn          : ", gbr.win_reg.PUKU_WIN_HITn.state);
  d.dump_bitp("RYDY_WIN_HITp          : ", gbr.win_reg.RYDY_WIN_HITp.state);
  d.dump_bitp("SOVY_WIN_FIRST_TILE_B  : ", gbr.win_reg.SOVY_WIN_HITp.state);
  d.dump_bitp("NOPA_WIN_MODE_B        : ", gbr.win_reg.NOPA_WIN_MODE_Bp.state);
  d.dump_bitp("PYCO_WX_MATCH_A        : ", gbr.win_reg.PYCO_WIN_MATCHp.state);
  d.dump_bitp("NUNU_WX_MATCH_B        : ", gbr.win_reg.NUNU_WIN_MATCHp.state);
  d.dump_bitp("REJO_WY_MATCH_LATCH    : ", gbr.win_reg.REJO_WY_MATCH_LATCHp.state);
  d.dump_bitp("SARY_WY_MATCH          : ", gbr.win_reg.SARY_WY_MATCHp.state);
  d.dump_bitp("RYFA_FETCHn_A          : ", gbr.win_reg.RYFA_WIN_FETCHn_A.state);
  d.dump_bitp("RENE_FETCHn_B          : ", gbr.win_reg.RENE_WIN_FETCHn_B.state);
  d.dump_bitp("RYKU_FINE_CNT0         : ", gbr.fine_scroll.RYKU_FINE_CNT0.state);
  d.dump_bitp("ROGA_FINE_CNT1         : ", gbr.fine_scroll.ROGA_FINE_CNT1.state);
  d.dump_bitp("RUBU_FINE_CNT2         : ", gbr.fine_scroll.RUBU_FINE_CNT2.state);
  d.dump_bitp("PUXA_FINE_MATCH_A      : ", gbr.fine_scroll.PUXA_SCX_FINE_MATCH_A.state);
  d.dump_bitp("NYZE_FINE_MATCH_B      : ", gbr.fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
  d.dump_bitp("ROXY_FINE_SCROLL_DONEn : ", gbr.fine_scroll.ROXY_FINE_SCROLL_DONEn.state);
  d.dump_bitp("VOGA_HBLANKp           : ", gbr.VOGA_HBLANKp.state);
  d("\n");
}

void GateBoy::dump_spu(Dumper& d) {
  (void)d;

  //d.dump_bitp   ("HADA_ALL_SOUND_ONp     : ", reg_NR52.HADA_ALL_SOUND_ONp.state);
  //d.dump_slice2p("NR50 : ", &reg_NR50.APEG_VOL_L0, 8);


}

#if 0
  d("\002===== OAM =====\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", oam_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");
  d("\002===== ZRAM =====\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", zero_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");
#endif