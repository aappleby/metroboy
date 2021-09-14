#include "GateBoyLib/GateBoyDumper.h"
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

void GateBoyDumper::dump_sys(const GateBoy& gb, Dumper& d) {
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

  d             ("phase        : %s\n", phases[gb.sys.phase_total & 7]);
  d             ("sys_rst      : %d\n", gb.sys.rst);
  d             ("sys_t1       : %d\n", gb.sys.t1);
  d             ("sys_t2       : %d\n", gb.sys.t2);
  d             ("sys_clken    : %d\n", gb.sys.clk_en);
  d             ("sys_clkgood  : %d\n", gb.sys.clk_good);
  d             ("sys_cpuready : %d\n", gb.sys.clk_req);
  d             ("sys_cpu_en   : %d\n", gb.sys.cpu_en);
  d             ("sys_fastboot : %d\n", gb.sys.fastboot);
  d             ("sys_buttons  : %d\n", gb.sys.buttons);
  d.dump_req    ("bus_req_new  : ", gb.cpu.bus_req_new);
  d.dump_bytep  ("data_latch   : ", gb.cpu.cpu_data_latch);
  d("\n");
  //d             ("gb_screen_x  : %d\n", gb_screen_x);
  //d             ("gb_screen_y  : %d\n", gb_screen_y);
  //d.dump_bitp   ("lcd_pix_lo   : ", lcd.lcd_pix_lo.state);
  //d.dump_bitp   ("lcd_pix_hi   : ", lcd.lcd_pix_hi.state);
  //d.dump_slice2p("lcd_pipe_lo  : ", lcd.lcd_pipe_lo, 8);
  //d.dump_slice2p("lcd_pipe_hi  : ", lcd.lcd_pipe_hi, 8);
  //d("\n");
  d             ("sim_time     : %f\n",      gb.sys.sim_time);
  d             ("phase_total  : %lld\n",    gb.sys.phase_total);
  d             ("phase_origin : %lld\n",    gb.sys.phase_origin);
  d             ("phase_delta  : %lld\n",    gb.sys.phase_total - gb.sys.phase_origin);
}

void GateBoyDumper::dump_tile_fetcher(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("LAXU_BFETCH_S0p     : ", gb.gb_state.tfetch_counter.LAXU_BFETCH_S0p.state);
  d.dump_bitp   ("MESU_BFETCH_S1p     : ", gb.gb_state.tfetch_counter.MESU_BFETCH_S1p.state);
  d.dump_bitp   ("NYVA_BFETCH_S2p     : ", gb.gb_state.tfetch_counter.NYVA_BFETCH_S2p.state);
  d.dump_bitp   ("POKY_PRELOAD_LATCHp : ", gb.gb_state.tfetch_control.POKY_PRELOAD_LATCHp.state);
  d.dump_bitp   ("LONY_FETCHINGp      : ", gb.gb_state.tfetch_control.LONY_FETCHINGp.state);
  d.dump_bitp   ("LOVY_FETCH_DONEp    : ", gb.gb_state.tfetch_control.LOVY_FETCH_DONEp.state);
  d.dump_bitp   ("NYKA_FETCH_DONEp    : ", gb.gb_state.tfetch_control.NYKA_FETCH_DONEp.state);
  d.dump_bitp   ("PORY_FETCH_DONEp    : ", gb.gb_state.tfetch_control.PORY_FETCH_DONEp.state);
  d.dump_bitp   ("PYGO_FETCH_DONEp    : ", gb.gb_state.tfetch_control.PYGO_FETCH_DONEp.state);
  d.dump_bitp   ("LYZU_BFETCH_S0p_D1  : ", gb.gb_state.tfetch_control.LYZU_BFETCH_S0p_D1.state);
  d.dump_slice2n("Temp A : ", &gb.gb_state.tile_temp_a.LEGU_TILE_DA0n, 8);
  d.dump_slice2p("Temp B : ", &gb.gb_state.tile_temp_b.RAWU_TILE_DB0p, 8);
}

void GateBoyDumper::dump_clocks(const GateBoy& gb, Dumper& d) {
  d.dump_bitp("AFUR_xxxxEFGHp : ", gb.gb_state.sys_clk.AFUR_xxxxEFGH.state);
  d.dump_bitp("ALEF_AxxxxFGHp : ", gb.gb_state.sys_clk.ALEF_AxxxxFGH.state);
  d.dump_bitp("APUK_ABxxxxGHp : ", gb.gb_state.sys_clk.APUK_ABxxxxGH.state);
  d.dump_bitp("ADYK_ABCxxxxHp : ", gb.gb_state.sys_clk.ADYK_ABCxxxxH.state);
  d("\n");
  d.dump_bitp("WUVU_ABxxEFxxp : ", gb.gb_state.sys_clk.WUVU_ABxxEFxx.state);
  d.dump_bitp("VENA_xxCDEFxxp : ", gb.gb_state.sys_clk.VENA_xxCDEFxx.state);
  d.dump_bitp("WOSU_AxxDExxHp : ", gb.gb_state.sys_clk.WOSU_AxxDExxH.state);
}

void GateBoyDumper::dump_interrupts(const GateBoy& gb, Dumper& d) {
  d.dump_slice2p("FF0F IF : ", &gb.gb_state.reg_if, 5);
  d.dump_slice2p("FFFF IE : ", &gb.gb_state.reg_ie, 5);
  d.dump_slice2p("LATCH   : ", &gb.gb_state.int_latch, 5);
  d.dump_slice2p("CPU_INT : ", &gb.gb_state.cpu_int, 5);
  d.dump_slice2p("CPU_ACK : ", &gb.gb_state.cpu_ack, 5);
  d.dump_bitp   ("ROPO_LY_MATCH   : ", gb.gb_state.int_ctrl.ROPO_LY_MATCH_SYNCp.state);
  d.dump_bitp   ("RUPO_LYC_MATCHn : ", gb.gb_state.int_ctrl.RUPO_LYC_MATCHn.state);
  d.dump_bitp   ("NYDU_TIMA7p_DELAY    : ", gb.gb_state.int_ctrl.NYDU_TIMA7p_DELAY.state);
  d.dump_bitp   ("MOBA_TIMER_OVERFLOWp : ", gb.gb_state.int_ctrl.MOBA_TIMER_OVERFLOWp.state);
}

void GateBoyDumper::dump_joypad(const GateBoy& gb, Dumper& d) {
  d.dump_bitp("AWOB_WAKE_CPU   : ", gb.gb_state.int_ctrl.AWOB_WAKE_CPU.state);
  d.dump_bitp("SIG_CPU_WAKE    : ", gb.gb_state.int_ctrl.SIG_CPU_WAKE.state);
  d("\n");
  d.dump_bitp("PIN_67_JOY_P10   : ", gb.gb_state.joy_ext.PIN_67_JOY_P10.state);
  d.dump_bitp("PIN_66_JOY_P11   : ", gb.gb_state.joy_ext.PIN_66_JOY_P11.state);
  d.dump_bitp("PIN_65_JOY_P12   : ", gb.gb_state.joy_ext.PIN_65_JOY_P12.state);
  d.dump_bitp("PIN_64_JOY_P13   : ", gb.gb_state.joy_ext.PIN_64_JOY_P13.state);
  d.dump_bitp("PIN_63_JOY_P14   : ", gb.gb_state.joy_ext.PIN_63_JOY_P14.state);
  d.dump_bitp("PIN_62_JOY_P15   : ", gb.gb_state.joy_ext.PIN_62_JOY_P15.state);
  d("\n");
  d.dump_bitp("KEVU_JOYP_L0n   : ", gb.gb_state.joy_latch.KEVU_JOYP_L0n.state);
  d.dump_bitp("KAPA_JOYP_L1n   : ", gb.gb_state.joy_latch.KAPA_JOYP_L1n.state);
  d.dump_bitp("KEJA_JOYP_L2n   : ", gb.gb_state.joy_latch.KEJA_JOYP_L2n.state);
  d.dump_bitp("KOLO_JOYP_L3n   : ", gb.gb_state.joy_latch.KOLO_JOYP_L3n.state);
  d("\n");
  d.dump_bitp("BATU_JP_GLITCH0 : ", gb.gb_state.joy_int.BATU_JP_GLITCH0.state);
  d.dump_bitp("ACEF_JP_GLITCH1 : ", gb.gb_state.joy_int.ACEF_JP_GLITCH1.state);
  d.dump_bitp("AGEM_JP_GLITCH2 : ", gb.gb_state.joy_int.AGEM_JP_GLITCH2.state);
  d.dump_bitp("APUG_JP_GLITCH3 : ", gb.gb_state.joy_int.APUG_JP_GLITCH3.state);
  d("\n");
  //d.dump_bitp("JUTE_DBG_D0     : ", joy.JUTE_DBG_D0.state);
  //d.dump_bitp("KECY_DBG_D1     : ", joy.KECY_DBG_D1.state);
  //d.dump_bitp("JALE_DBG_D2     : ", joy.JALE_DBG_D2.state);
  //d.dump_bitp("KYME_DBG_D3     : ", joy.KYME_DBG_D3.state);
  d.dump_bitp("KELY_JOYP_UDLR  : ", gb.gb_state.reg_joy.KELY_JOYP_UDLRp.state);
  d.dump_bitp("COFY_JOYP_ABCS  : ", gb.gb_state.reg_joy.COFY_JOYP_ABCSp.state);
  //d.dump_bitp("KUKO_DBG_D6     : ", joy.KUKO_DBG_D6.state);
  //d.dump_bitp("KERU_DBG_D7     : ", joy.KERU_DBG_D7.state);
}

void GateBoyDumper::dump_lcd(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("CATU : ", gb.gb_state.lcd.CATU_x113p.state);
  d.dump_bitp   ("ANEL : ", gb.gb_state.lcd.ANEL_x113p.state);
  d.dump_bitp   ("POPU : ", gb.gb_state.lcd.POPU_y144p.state);
  d.dump_bitp   ("MYTA : ", gb.gb_state.lcd.MYTA_y153p.state);
  d.dump_bitp   ("RUTU : ", gb.gb_state.lcd.RUTU_x113p.state);
  d.dump_bitp   ("NYPE : ", gb.gb_state.lcd.NYPE_x113p.state);
  d("\n");
  d.dump_bitp   ("SYGU_LINE_STROBE    : ", gb.gb_state.lcd.SYGU_LINE_STROBE.state);
  d.dump_bitn   ("MEDA_VSYNC_OUTn     : ", gb.gb_state.lcd.MEDA_VSYNC_OUTn.state);
  d.dump_bitp   ("LUCA_LINE_EVENp     : ", gb.gb_state.lcd.LUCA_LINE_EVENp.state);
  d.dump_bitp   ("NAPO_FRAME_EVENp    : ", gb.gb_state.lcd.NAPO_FRAME_EVENp.state);
  d.dump_bitp   ("RUJU                : ", gb.gb_state.lcd.RUJU.state);
  d.dump_bitp   ("POFY                : ", gb.gb_state.lcd.POFY.state);
  d.dump_bitp   ("POME                : ", gb.gb_state.lcd.POME.state);
  d.dump_bitp   ("PAHO_X_8_SYNC       : ", gb.gb_state.lcd.PAHO_X_8_SYNC.state);
  d.dump_bitp   ("WUSA_LCD_CLOCK_GATE : ", gb.gb_state.lcd.WUSA_LCD_CLOCK_GATE.state);
  d("\n");
  d.dump_bitp   ("PIN_50_LCD_DATA1 : ", gb.gb_state.lcd.PIN_50_LCD_DATA1.state);
  d.dump_bitp   ("PIN_51_LCD_DATA0 : ", gb.gb_state.lcd.PIN_51_LCD_DATA0.state);
  d.dump_bitp   ("PIN_54_LCD_HSYNC : ", gb.gb_state.lcd.PIN_54_LCD_HSYNC.state);
  d.dump_bitp   ("PIN_56_LCD_FLIPS : ", gb.gb_state.lcd.PIN_56_LCD_FLIPS.state);
  d.dump_bitp   ("PIN_52_LCD_CNTRL : ", gb.gb_state.lcd.PIN_52_LCD_CNTRL.state);
  d.dump_bitp   ("PIN_55_LCD_LATCH : ", gb.gb_state.lcd.PIN_55_LCD_LATCH.state);
  d.dump_bitp   ("PIN_53_LCD_CLOCK : ", gb.gb_state.lcd.PIN_53_LCD_CLOCK.state);
  d.dump_bitp   ("PIN_57_LCD_VSYNC : ", gb.gb_state.lcd.PIN_57_LCD_VSYNC.state);
  d("\n");
  d.dump_slice2p("LX              : ", &gb.gb_state.reg_lx.SAXO_LX0p.state,  7);
  d.dump_slice2p("FF44 LY         : ", &gb.gb_state.reg_ly.MUWY_LY0p.state,  8);
  d.dump_slice2n("FF45 LYC        : ", &gb.gb_state.reg_lyc.SYRY_LYC0n.state, 8);
}

void GateBoyDumper::dump_oam_bus(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("MAKA_LATCH_EXTp  : ", gb.gb_state.oam_ctrl.MAKA_LATCH_EXTp.state);
  d.dump_bitp   ("WUJE_CPU_OAM_WRn : ", gb.gb_state.oam_ctrl.WUJE_CPU_OAM_WRn.state);
  d.dump_bitp   ("SIG_OAM_CLKn     : ", gb.gb_state.oam_ctrl.SIG_OAM_CLKn.state);
  d.dump_bitp   ("SIG_OAM_WRn_A    : ", gb.gb_state.oam_ctrl.SIG_OAM_WRn_A.state);
  d.dump_bitp   ("SIG_OAM_WRn_B    : ", gb.gb_state.oam_ctrl.SIG_OAM_WRn_B.state);
  d.dump_bitp   ("SIG_OAM_OEn      : ", gb.gb_state.oam_ctrl.SIG_OAM_OEn.state);

  //dump_slicen(d, "BUS_OAM_An  : ", &oam_bus.BUS_OAM_A00n, 8);
  //dump_slicen(d, "BUS_OAM_DAn : ", &oam_bus.BUS_OAM_DA00n, 8);
  //dump_slicen(d, "BUS_OAM_DBn : ", &oam_bus.BUS_OAM_DB00n, 8);
  d.dump_slice2n("OAM LATCH A : ", &gb.gb_state.oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
  d.dump_slice2n("OAM LATCH B : ", &gb.gb_state.oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
  d.dump_slice2p("OAM TEMP A  : ", &gb.gb_state.oam_temp_a.XUSO_OAM_DA0p, 8);
  d.dump_slice2p("OAM TEMP B  : ", &gb.gb_state.oam_temp_b.YLOR_OAM_DB0p, 8);

}

void GateBoyDumper::dump_sprite_store(const GateBoy& gb, Dumper& d) {
  //d.dump_slice2p("SPRITE INDEX   : ", &XADU_SPRITE_IDX0p.state, 6);
  d.dump_slice2p("SPRITE COUNT   : ", &gb.gb_state.sprite_counter.BESE_SPRITE_COUNT0, 4);
  d("\n");
  d("STORE0 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.EBOJ_STORE0_RSTp.state, bit_pack_inv(gb.gb_state.store_i0), bit_pack_inv(gb.gb_state.store_l0), bit_pack_inv(gb.gb_state.store_x0));
  d("STORE1 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.CEDY_STORE1_RSTp.state, bit_pack_inv(gb.gb_state.store_i1), bit_pack_inv(gb.gb_state.store_l1), bit_pack_inv(gb.gb_state.store_x1));
  d("STORE2 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.EGAV_STORE2_RSTp.state, bit_pack_inv(gb.gb_state.store_i2), bit_pack_inv(gb.gb_state.store_l2), bit_pack_inv(gb.gb_state.store_x2));
  d("STORE3 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.GOTA_STORE3_RSTp.state, bit_pack_inv(gb.gb_state.store_i3), bit_pack_inv(gb.gb_state.store_l3), bit_pack_inv(gb.gb_state.store_x3));
  d("STORE4 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.XUDY_STORE4_RSTp.state, bit_pack_inv(gb.gb_state.store_i4), bit_pack_inv(gb.gb_state.store_l4), bit_pack_inv(gb.gb_state.store_x4));
  d("STORE5 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.WAFY_STORE5_RSTp.state, bit_pack_inv(gb.gb_state.store_i5), bit_pack_inv(gb.gb_state.store_l5), bit_pack_inv(gb.gb_state.store_x5));
  d("STORE6 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.WOMY_STORE6_RSTp.state, bit_pack_inv(gb.gb_state.store_i6), bit_pack_inv(gb.gb_state.store_l6), bit_pack_inv(gb.gb_state.store_x6));
  d("STORE7 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.WAPO_STORE7_RSTp.state, bit_pack_inv(gb.gb_state.store_i7), bit_pack_inv(gb.gb_state.store_l7), bit_pack_inv(gb.gb_state.store_x7));
  d("STORE8 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.EXUQ_STORE8_RSTp.state, bit_pack_inv(gb.gb_state.store_i8), bit_pack_inv(gb.gb_state.store_l8), bit_pack_inv(gb.gb_state.store_x8));
  d("STORE9 R%d I%02d L%02d X%03d\n", gb.gb_state.sprite_reset_flags.FONO_STORE9_RSTp.state, bit_pack_inv(gb.gb_state.store_i9), bit_pack_inv(gb.gb_state.store_l9), bit_pack_inv(gb.gb_state.store_x9));
}

void GateBoyDumper::dump_mbc1(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("MBC1_MODE    : ",  gb.gb_state.ext_mbc.MBC1_MODE.state);
  d.dump_bitp   ("MBC1_RAM_EN  : ",  gb.gb_state.ext_mbc.MBC1_RAM_EN.state);
  d.dump_slice2p("MBC1_BANK    : ", &gb.gb_state.ext_mbc.MBC1_BANK0, 7);
  d.dump_slice2p("MBC1_BANK_LO : ", &gb.gb_state.ext_mbc.MBC1_BANK0, 5);
  d.dump_slice2p("MBC1_BANK_HI : ", &gb.gb_state.ext_mbc.MBC1_BANK5, 2);
}

void GateBoyDumper::dump_cpu_bus(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("SIG_CPU_RDp       : ", gb.gb_state.cpu_signals.SIG_IN_CPU_RDp.state);
  d.dump_bitp   ("SIG_CPU_WRp       : ", gb.gb_state.cpu_signals.SIG_IN_CPU_WRp.state);
  d.dump_bitp   ("SIG_CPU_UNOR_DBG  : ", gb.gb_state.cpu_signals.SIG_CPU_UNOR_DBG.state);
  d.dump_bitp   ("SIG_CPU_ADDR_HIp  : ", gb.gb_state.cpu_signals.SIG_CPU_ADDR_HIp.state);
  d.dump_bitp   ("SIG_CPU_UMUT_DBG  : ", gb.gb_state.cpu_signals.SIG_CPU_UMUT_DBG.state);
  d.dump_bitp   ("SIG_CPU_EXT_BUSp  : ", gb.gb_state.cpu_signals.SIG_IN_CPU_EXT_BUSp.state);
  //d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
  d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", gb.gb_state.cpu_signals.SIG_IN_CPU_DBUS_FREE.state);
  d.dump_bitp   ("BOOT_BITn         : ", gb.gb_state.cpu_signals.TEPU_BOOT_BITn.state);
  d.dump_bitp   ("SIG_CPU_BOOTp     : ", gb.gb_state.cpu_signals.SIG_CPU_BOOTp.state);
  d.dump_bitp   ("TEDO_CPU_RDp      : ", gb.gb_state.cpu_signals.TEDO_CPU_RDp.state);
  d.dump_bitp   ("APOV_CPU_WRp      : ", gb.gb_state.cpu_signals.APOV_CPU_WRp.state);
  d.dump_bitp   ("TAPU_CPU_WRp      : ", gb.gb_state.cpu_signals.TAPU_CPU_WRp.state);
  d.dump_slice2p("BUS_CPU_A : ",     (BitBase*)&gb.gb_state.cpu_abus.BUS_CPU_A00p, 16);
  dump_slice_int(d,  "BUS_CPU_D : ", (BitBase*)&gb.gb_state.cpu_dbus.BUS_CPU_D00p, 8);
}

void GateBoyDumper::dump_dma(const GateBoy& gb, Dumper& d) {
  d.dump_slice2p("DMA_A_LOW  : ", &gb.gb_state.dma_lo.NAKY_DMA_A00p, 8);
  d.dump_slice2n("DMA_A_HIGH : ", &gb.gb_state.reg_dma.NAFA_DMA_A08n, 8);
  d             ("DMA Addr   : 0x%02x:%02x\n", bit_pack_inv(gb.gb_state.reg_dma), bit_pack(gb.gb_state.dma_lo));
  d.dump_bitp   ("MATU_DMA_RUNNINGp : ", gb.gb_state.MATU_DMA_RUNNINGp.state);
  d.dump_bitp   ("LYXE_DMA_LATCHp   : ", gb.gb_state.dma_ctrl.LYXE_DMA_LATCHp  .state);
  d.dump_bitp   ("MYTE_DMA_DONE     : ", gb.gb_state.dma_ctrl.MYTE_DMA_DONE    .state);
  d.dump_bitp   ("LUVY_DMA_TRIG_d0  : ", gb.gb_state.dma_ctrl.LUVY_DMA_TRIG_d0 .state);
  d.dump_bitp   ("LENE_DMA_TRIG_d4  : ", gb.gb_state.dma_ctrl.LENE_DMA_TRIG_d4 .state);
  d.dump_bitp   ("LOKY_DMA_LATCHp   : ", gb.gb_state.dma_ctrl.LOKY_DMA_LATCHp  .state);
}


void GateBoyDumper::dump_ext_bus(const GateBoy& gb, Dumper& d) {
  d.dump_slice2n("PIN_01_ADDR : ", &gb.gb_state.ext_abus.lo.PIN_01_A00, 16);

  //d.dump_slice2n(   "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);
  //dump_slice2b  (d, "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);

  d.dump_bitn   ("PIN_80_CSn  : ", gb.gb_state.ext_ctrl.PIN_80_CSn.state);
  d.dump_bitn   ("PIN_79_RDn  : ", gb.gb_state.ext_ctrl.PIN_79_RDn.state);
  d.dump_bitn   ("PIN_78_WRn  : ", gb.gb_state.ext_ctrl.PIN_78_WRn.state);
  d.dump_slice2p("ADDR LATCH  : ", &gb.gb_state.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p, 15);
  d.dump_slice2n("DATA LATCH  : ", &gb.gb_state.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n, 8);
}

void GateBoyDumper::dump_vram_bus(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("PIN_43_VRAM_CSn  : ", gb.gb_state.vram_ext_ctrl.PIN_43_VRAM_CSn.state);
  d.dump_bitp   ("PIN_45_VRAM_OEn  : ", gb.gb_state.vram_ext_ctrl.PIN_45_VRAM_OEn.state);
  d.dump_bitp   ("PIN_49_VRAM_WRn  : ", gb.gb_state.vram_ext_ctrl.PIN_49_VRAM_WRn.state);
  d.dump_slice2p("PIN_34_VRAM_ADDR : ", &gb.gb_state.vram_ext_abus.PIN_34_VRAM_A00, 13);
  d.dump_slice2p("PIN_25_VRAM_DATA : ", &gb.gb_state.vram_ext_dbus.PIN_33_VRAM_D00, 8);
  d.dump_slice2n("BUS_VRAM_An      : ", &gb.gb_state.vram_abus.lo.BUS_VRAM_A00n, 13);
  d.dump_slice2p("BUS_VRAM_Dp      : ", &gb.gb_state.vram_dbus.BUS_VRAM_D00p, 8);
}

void GateBoyDumper::dump_sprite_fetcher(const GateBoy& gb, Dumper& d) {
  d.dump_bitp   ("TOXE_SFETCH_S0       : ", gb.gb_state.sfetch_counter.TOXE_SFETCH_S0p     .state);
  d.dump_bitp   ("TULY_SFETCH_S1       : ", gb.gb_state.sfetch_counter.TULY_SFETCH_S1p     .state);
  d.dump_bitp   ("TESE_SFETCH_S2       : ", gb.gb_state.sfetch_counter.TESE_SFETCH_S2p     .state);
  d.dump_bitp   ("TAKA_SFETCH_RUNNINGp : ", gb.gb_state.sfetch_control.TAKA_SFETCH_RUNNINGp.state);
  d.dump_bitp   ("SOBU_SFETCH_REQp     : ", gb.gb_state.sfetch_control.SOBU_SFETCH_REQp    .state);
  d.dump_bitp   ("SUDA_SFETCH_REQp     : ", gb.gb_state.sfetch_control.SUDA_SFETCH_REQp    .state);
  d.dump_bitp   ("TYFO_SFETCH_S0_D1    : ", gb.gb_state.sfetch_control.TYFO_SFETCH_S0p_D1  .state);
  d.dump_bitp   ("TOBU_SFETCH_S1_D2    : ", gb.gb_state.sfetch_control.TOBU_SFETCH_S1p_D2  .state);
  d.dump_bitp   ("VONU_SFETCH_S1_D4    : ", gb.gb_state.sfetch_control.VONU_SFETCH_S1p_D4  .state);
  d.dump_bitp   ("SEBA_SFETCH_S1_D5    : ", gb.gb_state.sfetch_control.SEBA_SFETCH_S1p_D5  .state);
  d.dump_slice2n("Temp A : ", &gb.gb_state.sprite_pix_a.REWO_SPRITE_DA0n, 8);
  d.dump_slice2n("Temp B : ", &gb.gb_state.sprite_pix_b.PEFO_SPRITE_DB0n, 8);
}

void GateBoyDumper::dump_timer(const GateBoy& gb, Dumper& d) {
  d.dump_slice2p("DIV16 : ", &gb.gb_state.reg_div.UKUP_DIV00p, 16);
  d.dump_slice2p("FF04 DIV  : ", &gb.gb_state.reg_div.UGOT_DIV06p, 8);
  d.dump_slice2p("FF05 TIMA : ", &gb.gb_state.reg_tima, 8);
  d.dump_slice2p("FF06 TMA  : ", &gb.gb_state.reg_tma, 8);
  d.dump_slice2p("FF07 TAC  : ", &gb.gb_state.reg_tac, 3);
}

void GateBoyDumper::dump_resets(const GateBoy& gb, Dumper& d) {
  d.dump_bitp("PIN_71_RST     : ", gb.gb_state.sys_rst.PIN_71_RST.state);
  d.dump_bitp("PIN_77_T1      : ", gb.gb_state.sys_rst.PIN_77_T1.state);
  d.dump_bitp("PIN_76_T2      : ", gb.gb_state.sys_rst.PIN_76_T2.state);

  d.dump_bitp("TUBO_WAITINGp  : ", gb.gb_state.sys_rst.TUBO_WAITINGp.state);
  d.dump_bitn("ASOL_POR_DONEn : ", gb.gb_state.sys_rst.ASOL_POR_DONEn.state);
  d.dump_bitp("AFER_SYS_RSTp  : ", gb.gb_state.sys_rst.AFER_SYS_RSTp.state);
  d.dump_bitp("SOTO_DBG_VRAMp : ", gb.gb_state.sys_rst.SOTO_DBG_VRAMp.state);

  d.dump_bitp("SIG_CPU_EXT_CLKGOOD : ", gb.gb_state.sys_rst.SIG_CPU_EXT_CLKGOOD.state);
  d.dump_bitp("SIG_CPU_EXT_RESETp  : ", gb.gb_state.sys_rst.SIG_CPU_EXT_RESETp .state);
  d.dump_bitp("SIG_CPU_STARTp      : ", gb.gb_state.sys_rst.SIG_CPU_STARTp     .state);
  d.dump_bitp("SIG_CPU_INT_RESETp  : ", gb.gb_state.sys_rst.SIG_CPU_INT_RESETp .state);
}

void GateBoyDumper::dump_sprite_scanner(const GateBoy& gb, Dumper& d) {
  d.dump_slice2p("SCAN INDEX        : ", &gb.gb_state.scan_counter.YFEL_SCAN0, 6);
  d.dump_bitp   ("BESU_SCANNINGp    : ", gb.gb_state.sprite_scanner.BESU_SCANNINGn.state);
  d.dump_bitp   ("CENO_SCANNINGp    : ", gb.gb_state.sprite_scanner.CENO_SCANNINGn.state);
  d.dump_bitp   ("DEZY_COUNT_CLKp   : ", gb.gb_state.sprite_scanner.DEZY_COUNT_CLKp.state);
  d.dump_bitp   ("BYBA_SCAN_DONE_Ap : ", gb.gb_state.sprite_scanner.BYBA_SCAN_DONE_Ap.state);
  d.dump_bitp   ("DOBA_SCAN_DONE_Bp : ", gb.gb_state.sprite_scanner.DOBA_SCAN_DONE_Bp.state);
}

void GateBoyDumper::dump_serial(const GateBoy& gb, Dumper& d) {
#if 0
  d.dump_bitp   ("ETAF_SER_RUNNING : ", serial.ETAF_SER_RUN.state);
  d.dump_bitp   ("CULY_XFER_DIR    : ", serial.CULY_SER_DIR.state);
  d.dump_bitp   ("COTY_SER_CLK     : ", serial.COTY_SER_CLK.state);
  d.dump_bitp   ("ELYS_SER_OUT     : ", serial.ELYS_SER_OUT.state);
  d.dump_slice2p("CAFA_SER_CNT     : ", &serial.CAFA_SER_CNT0, 4);
  d.dump_slice2p("CUBA_SER_DATA    : ", &serial.CUBA_SER_DATA0, 8);
#endif
}

void GateBoyDumper::dump_ppu(const GateBoy& gb, Dumper& d) {
  d.dump_slice2n("FF40 LCDC  : ", &gb.gb_state.reg_lcdc.VYXE_LCDC_BGENn, 8);
  d.dump_slice2n("FF41 STAT  : ", &gb.gb_state.reg_stat.ROXE_STAT_HBI_ENn, 4);
  d.dump_slice2n("FF42 SCY   : ", &gb.gb_state.reg_scy.GAVE_SCY0n, 8);
  d.dump_slice2n("FF43 SCX   : ", &gb.gb_state.reg_scx.DATY_SCX0n, 8);
  d.dump_slice2n("FF47 BGP   : ", &gb.gb_state.reg_bgp.PAVO_BGP_D0n, 8);
  d.dump_slice2n("FF48 OBP0  : ", &gb.gb_state.reg_obp0.XUFU_OBP0_D0n, 8);
  d.dump_slice2n("FF49 OBP1  : ", &gb.gb_state.reg_obp1.MOXY_OBP1_D0n, 8);
  d.dump_slice2n("FF4A WY    : ", &gb.gb_state.reg_wy.NESO_WY0n, 8);
  d.dump_slice2n("FF4B WX    : ", &gb.gb_state.reg_wx.MYPA_WX0n, 8);
  d.dump_slice2p("WIN MAP X  : ", &gb.gb_state.win_x.map.WYKA_WIN_MAP_X0, 5);
  d.dump_slice2p("WIN MAP Y  : ", &gb.gb_state.win_y.map.TUFU_WIN_MAP_Y0, 5);
  d.dump_slice2p("WIN TILE Y : ", &gb.gb_state.win_y.tile.VYNO_WIN_TILE_Y0, 3);
  d("\n");
  d.dump_slice2p("PIX COUNT  : ", &gb.gb_state.pix_count.XEHO_PX0p, 8);
  d.dump_slice2p("BG PIPE A  : ", &gb.gb_state.bgw_pipe_a, 8);
  d.dump_slice2p("BG PIPE B  : ", &gb.gb_state.bgw_pipe_b, 8);
  d.dump_slice2p("SPR PIPE A : ", &gb.gb_state.spr_pipe_a, 8);
  d.dump_slice2p("SPR PIPE B : ", &gb.gb_state.spr_pipe_b, 8);
  d.dump_slice2p("PAL PIPE   : ", &gb.gb_state.pal_pipe, 8);
  d.dump_slice2p("MASK PIPE  : ", &gb.gb_state.mask_pipe, 8);
  d.dump_bitn   ("REMY_LD0n  : ", gb.gb_state.lcd.REMY_LD0n.state);
  d.dump_bitn   ("RAVO_LD1n  : ", gb.gb_state.lcd.RAVO_LD1n.state);
  d("\n");
  d.dump_bitp("XYMU_RENDERINGn        : ", gb.gb_state.XYMU_RENDERINGn.state);
  d.dump_bitp("PYNU_WIN_MODE_Ap       : ", gb.gb_state.win_ctrl.PYNU_WIN_MODE_Ap.state);
  d.dump_bitp("PUKU_WIN_HITn          : ", gb.gb_state.win_ctrl.PUKU_WIN_HITn.state);
  d.dump_bitp("RYDY_WIN_HITp          : ", gb.gb_state.win_ctrl.RYDY_WIN_HITp.state);
  d.dump_bitp("SOVY_WIN_FIRST_TILE_B  : ", gb.gb_state.win_ctrl.SOVY_WIN_HITp.state);
  d.dump_bitp("NOPA_WIN_MODE_B        : ", gb.gb_state.win_ctrl.NOPA_WIN_MODE_Bp.state);
  d.dump_bitp("PYCO_WX_MATCH_A        : ", gb.gb_state.win_ctrl.PYCO_WIN_MATCHp.state);
  d.dump_bitp("NUNU_WX_MATCH_B        : ", gb.gb_state.win_ctrl.NUNU_WIN_MATCHp.state);
  d.dump_bitp("REJO_WY_MATCH_LATCH    : ", gb.gb_state.win_ctrl.REJO_WY_MATCH_LATCHp.state);
  d.dump_bitp("SARY_WY_MATCH          : ", gb.gb_state.win_ctrl.SARY_WY_MATCHp.state);
  d.dump_bitp("RYFA_FETCHn_A          : ", gb.gb_state.win_ctrl.RYFA_WIN_FETCHn_A.state);
  d.dump_bitp("RENE_FETCHn_B          : ", gb.gb_state.win_ctrl.RENE_WIN_FETCHn_B.state);
  d.dump_bitp("RYKU_FINE_CNT0         : ", gb.gb_state.fine_count.RYKU_FINE_CNT0.state);
  d.dump_bitp("ROGA_FINE_CNT1         : ", gb.gb_state.fine_count.ROGA_FINE_CNT1.state);
  d.dump_bitp("RUBU_FINE_CNT2         : ", gb.gb_state.fine_count.RUBU_FINE_CNT2.state);
  d.dump_bitp("PUXA_FINE_MATCH_A      : ", gb.gb_state.fine_scroll.PUXA_SCX_FINE_MATCH_A.state);
  d.dump_bitp("NYZE_FINE_MATCH_B      : ", gb.gb_state.fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
  d.dump_bitp("ROXY_FINE_SCROLL_DONEn : ", gb.gb_state.fine_scroll.ROXY_FINE_SCROLL_DONEn.state);
  d.dump_bitp("VOGA_HBLANKp           : ", gb.gb_state.VOGA_HBLANKp.state);
  d("\n");
}

void GateBoyDumper::dump_spu(const GateBoy& gb, Dumper& d) {
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