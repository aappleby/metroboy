#include "GateBoyLib/GateBoy.h"

void dump_slice_int(Dumper& d, const char* tag, const BitBase* bits, int bit_count) {
  d(tag);
  for (int i = bit_count - 1; i >= 0; i--) {
    d.add_char(bits[i].cp_int());
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

  d             ("phase        : %s\n", phases[phase_total & 7]);
  d             ("line phase   : %d\n", line_phase_x);
  d             ("sys_rst      : %d\n", sys_rst);
  d             ("sys_t1       : %d\n", sys_t1);
  d             ("sys_t2       : %d\n", sys_t2);
  d             ("sys_clken    : %d\n", sys_clken);
  d             ("sys_clkgood  : %d\n", sys_clkgood);
  d             ("sys_cpuready : %d\n", sys_clkreq);
  d             ("sys_cpu_en   : %d\n", sys_cpu_en);
  d             ("sys_fastboot : %d\n", sys_fastboot);
  d             ("sys_buttons  : %d\n", sys_buttons);
  d.dump_req    ("bus_req_new  : ", bus_req_new);
  d.dump_bytep  ("data_latch   : ", cpu_data_latch);
  d("\n");
  //d             ("gb_screen_x  : %d\n", gb_screen_x);
  //d             ("gb_screen_y  : %d\n", gb_screen_y);
  //d.dump_bitp   ("lcd_pix_lo   : ", lcd.lcd_pix_lo.state);
  //d.dump_bitp   ("lcd_pix_hi   : ", lcd.lcd_pix_hi.state);
  //d.dump_slice2p("lcd_pipe_lo  : ", lcd.lcd_pipe_lo, 8);
  //d.dump_slice2p("lcd_pipe_hi  : ", lcd.lcd_pipe_hi, 8);
  //d("\n");
  d             ("sim_time     : %f\n",      sim_time);
  d             ("phase_total  : %lld\n",    phase_total);
  d             ("phase_origin : %lld\n",    phase_origin);
  d             ("phase_delta  : %lld\n",    phase_total - phase_origin);
  d             ("pass_hash    : %016llx\n", phase_hash);
  d             ("total_hash   : %016llx\n", cumulative_hash);
}

void GateBoy::dump_tile_fetcher(Dumper& d) {
  d.dump_bitp   ("POKY_PRELOAD_LATCHp : ", tile_fetcher.POKY_PRELOAD_LATCHp.state);
  d.dump_bitp   ("LONY_FETCHINGp      : ", tile_fetcher.LONY_FETCHINGp.state);
  d.dump_bitp   ("LOVY_FETCH_DONEp    : ", tile_fetcher.LOVY_FETCH_DONEp.state);
  d.dump_bitp   ("NYKA_FETCH_DONEp    : ", tile_fetcher.NYKA_FETCH_DONEp.state);
  d.dump_bitp   ("PORY_FETCH_DONEp    : ", tile_fetcher.PORY_FETCH_DONEp.state);
  d.dump_bitp   ("PYGO_FETCH_DONEp    : ", tile_fetcher.PYGO_FETCH_DONEp.state);
  d.dump_bitp   ("LAXU_BFETCH_S0p     : ", tile_fetcher._LAXU_BFETCH_S0p.state);
  d.dump_bitp   ("MESU_BFETCH_S1p     : ", tile_fetcher._MESU_BFETCH_S1p.state);
  d.dump_bitp   ("NYVA_BFETCH_S2p     : ", tile_fetcher._NYVA_BFETCH_S2p.state);
  d.dump_bitp   ("LYZU_BFETCH_S0p_D1  : ", tile_fetcher.LYZU_BFETCH_S0p_D1.state);
  d.dump_slice2n("Temp A : ", &tile_temp_a.LEGU_TILE_DA0n, 8);
  d.dump_slice2p("Temp B : ", &tile_temp_b.RAWU_TILE_DB0p, 8);
}

void GateBoy::dump_clocks(Dumper& d) {
  d.dump_bitp("AFUR_xxxxEFGHp : ", clk.AFUR_xxxxEFGH.state);
  d.dump_bitp("ALEF_AxxxxFGHp : ", clk.ALEF_AxxxxFGH.state);
  d.dump_bitp("APUK_ABxxxxGHp : ", clk.APUK_ABxxxxGH.state);
  d.dump_bitp("ADYK_ABCxxxxHp : ", clk.ADYK_ABCxxxxH.state);
  d("\n");
  d.dump_bitp("WUVU_ABxxEFxxp : ", clk.WUVU_ABxxEFxx.state);
  d.dump_bitp("VENA_xxCDEFxxp : ", clk.VENA_xxCDEFxx.state);
  d.dump_bitp("WOSU_AxxDExxHp : ", clk.WOSU_AxxDExxH.state);
}

void GateBoy::dump_interrupts(Dumper& d) {
  d.dump_slice2p("FF0F IF : ", &interrupts.LOPE_FF0F_D0p.state, 5);
  d.dump_slice2p("FFFF IE : ", &interrupts.IE_D0.state, 5);
  d.dump_slice2p("LATCH   : ", &interrupts.MATY_FF0F_L0p.state, 5);
  d.dump_slice2p("CPU_INT : ", &interrupts.SIG_CPU_INT_VBLANK, 5);
  d.dump_slice2p("CPU_ACK : ", &interrupts.SIG_CPU_ACK_VBLANK, 5);
}

void GateBoy::dump_joypad(Dumper& d) {
  d.dump_bitp("AWOB_WAKE_CPU   : ", joy.AWOB_WAKE_CPU.state);
  d.dump_bitp("SIG_CPU_WAKE    : ", joy.SIG_CPU_WAKE.get_state());
  d("\n");
  d.dump_bitp("PIN_67_JOY_P10   : ", joy.PIN_67_JOY_P10.qp_ext_old());
  d.dump_bitp("PIN_66_JOY_P11   : ", joy.PIN_66_JOY_P11.qp_ext_old());
  d.dump_bitp("PIN_65_JOY_P12   : ", joy.PIN_65_JOY_P12.qp_ext_old());
  d.dump_bitp("PIN_64_JOY_P13   : ", joy.PIN_64_JOY_P13.qp_ext_old());
  d.dump_bitp("PIN_63_JOY_P14   : ", joy.PIN_63_JOY_P14.qp_ext_old());
  d.dump_bitp("PIN_62_JOY_P15   : ", joy.PIN_62_JOY_P15.qp_ext_old());
  d("\n");
  d.dump_bitp("KEVU_JOYP_L0n   : ", joy.KEVU_JOYP_L0n.state);
  d.dump_bitp("KAPA_JOYP_L1n   : ", joy.KAPA_JOYP_L1n.state);
  d.dump_bitp("KEJA_JOYP_L2n   : ", joy.KEJA_JOYP_L2n.state);
  d.dump_bitp("KOLO_JOYP_L3n   : ", joy.KOLO_JOYP_L3n.state);
  d("\n");
  d.dump_bitp("BATU_JP_GLITCH0 : ", joy.BATU_JP_GLITCH0.state);
  d.dump_bitp("ACEF_JP_GLITCH1 : ", joy.ACEF_JP_GLITCH1.state);
  d.dump_bitp("AGEM_JP_GLITCH2 : ", joy.AGEM_JP_GLITCH2.state);
  d.dump_bitp("APUG_JP_GLITCH3 : ", joy.APUG_JP_GLITCH3.state);
  d("\n");
  //d.dump_bitp("JUTE_DBG_D0     : ", joy.JUTE_DBG_D0.state);
  //d.dump_bitp("KECY_DBG_D1     : ", joy.KECY_DBG_D1.state);
  //d.dump_bitp("JALE_DBG_D2     : ", joy.JALE_DBG_D2.state);
  //d.dump_bitp("KYME_DBG_D3     : ", joy.KYME_DBG_D3.state);
  d.dump_bitp("KELY_JOYP_UDLR  : ", joy.KELY_JOYP_UDLRp.state);
  d.dump_bitp("COFY_JOYP_ABCS  : ", joy.COFY_JOYP_ABCSp.state);
  //d.dump_bitp("KUKO_DBG_D6     : ", joy.KUKO_DBG_D6.state);
  //d.dump_bitp("KERU_DBG_D7     : ", joy.KERU_DBG_D7.state);
}

void GateBoy::dump_lcd(Dumper& d) {
  d.dump_bitp   ("CATU : ", lcd.CATU_x113p.state);
  d.dump_bitp   ("ANEL : ", lcd.ANEL_x113p.state);
  d.dump_bitp   ("POPU : ", lcd.POPU_y144p.state);
  d.dump_bitp   ("MYTA : ", lcd.MYTA_y153p.state);
  d.dump_bitp   ("RUTU : ", lcd.RUTU_x113p.state);
  d.dump_bitp   ("NYPE : ", lcd.NYPE_x113p.state);
  d("\n");
  d.dump_bitp   ("SYGU_LINE_STROBE    : ", lcd.SYGU_LINE_STROBE.state);
  d.dump_bitn   ("MEDA_VSYNC_OUTn     : ", lcd.MEDA_VSYNC_OUTn.state);
  d.dump_bitp   ("LUCA_LINE_EVENp     : ", lcd.LUCA_LINE_EVENp.state);
  d.dump_bitp   ("NAPO_FRAME_EVENp    : ", lcd.NAPO_FRAME_EVENp.state);
  d.dump_bitp   ("RUJU                : ", lcd.RUJU.get_state());
  d.dump_bitp   ("POFY                : ", lcd.POFY.get_state());
  d.dump_bitp   ("POME                : ", lcd.POME.get_state());
  d.dump_bitp   ("PAHO_X_8_SYNC       : ", lcd.PAHO_X_8_SYNC.state);
  d.dump_bitp   ("WUSA_LCD_CLOCK_GATE : ", lcd.WUSA_LCD_CLOCK_GATE.state);
  d("\n");
  d.dump_bitp   ("PIN_50_LCD_DATA1 : ", lcd.PIN_50_LCD_DATA1.qp_ext_old());
  d.dump_bitp   ("PIN_51_LCD_DATA0 : ", lcd.PIN_51_LCD_DATA0.qp_ext_old());
  d.dump_bitp   ("PIN_54_LCD_HSYNC : ", lcd.PIN_54_LCD_HSYNC.qp_ext_old());
  d.dump_bitp   ("PIN_56_LCD_FLIPS : ", lcd.PIN_56_LCD_FLIPS.qp_ext_old());
  d.dump_bitp   ("PIN_52_LCD_CNTRL : ", lcd.PIN_52_LCD_CNTRL.qp_ext_old());
  d.dump_bitp   ("PIN_55_LCD_LATCH : ", lcd.PIN_55_LCD_LATCH.qp_ext_old());
  d.dump_bitp   ("PIN_53_LCD_CLOCK : ", lcd.PIN_53_LCD_CLOCK.qp_ext_old());
  d.dump_bitp   ("PIN_57_LCD_VSYNC : ", lcd.PIN_57_LCD_VSYNC.qp_ext_old());
  d("\n");
  d.dump_slice2p("LX              : ", &reg_lx.SAXO_LX0p.state,  7);
  d.dump_slice2p("FF44 LY         : ", &reg_ly.MUWY_LY0p.state,  8);
  d.dump_slice2n("FF45 LYC        : ", &reg_lyc.SYRY_LYC0n.state, 8);
  d.dump_bitp   ("ROPO_LY_MATCH   : ", reg_lyc.ROPO_LY_MATCH_SYNCp.state);
  d.dump_bitp   ("RUPO_LYC_MATCHn : ", reg_stat.RUPO_LYC_MATCHn.state);
}

void GateBoy::dump_oam_bus(Dumper& d) {
  d.dump_bitp   ("MAKA_LATCH_EXTp  : ", oam.MAKA_LATCH_EXTp.state);
  d.dump_bitp   ("WUJE_CPU_OAM_WRn : ", oam.WUJE_CPU_OAM_WRn.state);
  d.dump_bitp   ("SIG_OAM_CLKn     : ", oam.SIG_OAM_CLKn.get_state());
  d.dump_bitp   ("SIG_OAM_WRn_A    : ", oam.SIG_OAM_WRn_A.get_state());
  d.dump_bitp   ("SIG_OAM_WRn_B    : ", oam.SIG_OAM_WRn_B.get_state());
  d.dump_bitp   ("SIG_OAM_OEn      : ", oam.SIG_OAM_OEn.get_state());

  //dump_slicen(d, "BUS_OAM_An  : ", &oam_bus.BUS_OAM_A00n, 8);
  //dump_slicen(d, "BUS_OAM_DAn : ", &oam_bus.BUS_OAM_DA00n, 8);
  //dump_slicen(d, "BUS_OAM_DBn : ", &oam_bus.BUS_OAM_DB00n, 8);
  d.dump_slice2n("OAM LATCH A : ", &oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
  d.dump_slice2n("OAM LATCH B : ", &oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
  d.dump_slice2p("OAM TEMP A  : ", &oam_temp_a.XUSO_OAM_DA0p, 8);
  d.dump_slice2p("OAM TEMP B  : ", &oam_temp_b.YLOR_OAM_DB0p, 8);

}

void GateBoy::dump_sprite_store(Dumper& d) {
  auto& ss = sprite_store;

  //d.dump_slice2p("SPRITE INDEX   : ", &XADU_SPRITE_IDX0p.state, 6);
  d.dump_slice2p("SPRITE COUNT   : ", &sprite_counter.BESE_SPRITE_COUNT0, 4);
  d.dump_bitp   ("DEZY_STORE_ENn : ", sprite_counter.DEZY_COUNT_CLKp.state);
  d("\n");
  d("STORE0 R%d I%02d L%02d X%03d\n", sprite_reset_flags.EBOJ_STORE0_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.YGUS_STORE0_I0n_odd), BitBase::pack_oldn(4, &ss.GYHO_STORE0_L0n_odd), BitBase::pack_oldn(8, &ss.XEPE_STORE0_X0p_odd));
  d("STORE1 R%d I%02d L%02d X%03d\n", sprite_reset_flags.CEDY_STORE1_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.CADU_STORE1_I0n_odd), BitBase::pack_oldn(4, &ss.AMES_STORE1_L0n_odd), BitBase::pack_oldn(8, &ss.DANY_STORE1_X0p_odd));
  d("STORE2 R%d I%02d L%02d X%03d\n", sprite_reset_flags.EGAV_STORE2_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.BUHE_STORE2_I0n_odd), BitBase::pack_oldn(4, &ss.YLOV_STORE2_L0n_odd), BitBase::pack_oldn(8, &ss.FOKA_STORE2_X0p_odd));
  d("STORE3 R%d I%02d L%02d X%03d\n", sprite_reset_flags.GOTA_STORE3_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.DEVY_STORE3_I0n_odd), BitBase::pack_oldn(4, &ss.ZURO_STORE3_L0n_odd), BitBase::pack_oldn(8, &ss.XOLY_STORE3_X0p_odd));
  d("STORE4 R%d I%02d L%02d X%03d\n", sprite_reset_flags.XUDY_STORE4_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.XAVE_STORE4_I0n_odd), BitBase::pack_oldn(4, &ss.CAPO_STORE4_L0n_odd), BitBase::pack_oldn(8, &ss.WEDU_STORE4_X0p_odd));
  d("STORE5 R%d I%02d L%02d X%03d\n", sprite_reset_flags.WAFY_STORE5_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.EKOP_STORE5_I0n_odd), BitBase::pack_oldn(4, &ss.ACEP_STORE5_L0n_odd), BitBase::pack_oldn(8, &ss.FUSA_STORE5_X0p_odd));
  d("STORE6 R%d I%02d L%02d X%03d\n", sprite_reset_flags.WOMY_STORE6_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.GABO_STORE6_I0n_odd), BitBase::pack_oldn(4, &ss.ZUMY_STORE6_L0n_odd), BitBase::pack_oldn(8, &ss.YCOL_STORE6_X0p_odd));
  d("STORE7 R%d I%02d L%02d X%03d\n", sprite_reset_flags.WAPO_STORE7_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.GULE_STORE7_I0n_odd), BitBase::pack_oldn(4, &ss.XYNA_STORE7_L0n_odd), BitBase::pack_oldn(8, &ss.ERAZ_STORE7_X0p_odd));
  d("STORE8 R%d I%02d L%02d X%03d\n", sprite_reset_flags.EXUQ_STORE8_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.AXUV_STORE8_I0n_odd), BitBase::pack_oldn(4, &ss.AZAP_STORE8_L0n_odd), BitBase::pack_oldn(8, &ss.EZUF_STORE8_X0p_odd));
  d("STORE9 R%d I%02d L%02d X%03d\n", sprite_reset_flags.FONO_STORE9_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &ss.YBER_STORE9_I0n_odd), BitBase::pack_oldn(4, &ss.CANA_STORE9_L0n_odd), BitBase::pack_oldn(8, &ss.XUVY_STORE9_X0p_odd));
}

void GateBoy::dump_mbc1(Dumper& d) {
  d.dump_bitp   ("MBC1_MODE    : ", ext_pins.MBC1_MODE.state);
  d.dump_bitp   ("MBC1_RAM_EN  : ", ext_pins.MBC1_RAM_EN.state);
  d.dump_slice2p("MBC1_BANK    : ", &ext_pins.MBC1_BANK0, 7);
  d.dump_slice2p("MBC1_BANK_LO : ", &ext_pins.MBC1_BANK0, 5);
  d.dump_slice2p("MBC1_BANK_HI : ", &ext_pins.MBC1_BANK5, 2);
}

void GateBoy::dump_cpu_bus(Dumper& d) {
  d.dump_bitp   ("SIG_CPU_RDp       : ", cpu_signals.SIG_IN_CPU_RDp.get_state());
  d.dump_bitp   ("SIG_CPU_WRp       : ", cpu_signals.SIG_IN_CPU_WRp.get_state());
  d.dump_bitp   ("SIG_CPU_UNOR_DBG  : ", cpu_signals.SIG_CPU_UNOR_DBG.get_state());
  d.dump_bitp   ("SIG_CPU_ADDR_HIp  : ", cpu_signals.SIG_CPU_ADDR_HIp.get_state());
  d.dump_bitp   ("SIG_CPU_UMUT_DBG  : ", cpu_signals.SIG_CPU_UMUT_DBG.get_state());
  d.dump_bitp   ("SIG_CPU_EXT_BUSp  : ", cpu_signals.SIG_IN_CPU_EXT_BUSp.get_state());
  //d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
  d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", cpu_signals.SIG_IN_CPU_LATCH_EXT.get_state());
  d.dump_bitp   ("BOOT_BITn         : ", cpu_signals.TEPU_BOOT_BITn_h.state);
  d.dump_bitp   ("SIG_CPU_BOOTp     : ", cpu_signals.SIG_CPU_BOOTp.get_state());
  d.dump_bitp   ("TEDO_CPU_RDp      : ", cpu_signals.TEDO_CPU_RDp.get_state());
  d.dump_bitp   ("APOV_CPU_WRp      : ", cpu_signals.APOV_CPU_WRp.get_state());
  d.dump_bitp   ("TAPU_CPU_WRp      : ", cpu_signals.TAPU_CPU_WRp.get_state());
  d.dump_slice2p("BUS_CPU_A : ",     (BitBase*)&new_bus.BUS_CPU_A00p, 16);
  dump_slice_int(d,  "BUS_CPU_D : ", (BitBase*)&new_bus.BUS_CPU_D00p, 8);
}

void GateBoy::dump_dma(Dumper& d) {
  d.dump_slice2p("DMA_A_LOW  : ", &dma.NAKY_DMA_A00p, 8);
  d.dump_slice2n("DMA_A_HIGH : ", &dma.NAFA_DMA_A08n, 8);
  d             ("DMA Addr   : 0x%02x:%02x\n", BitBase::pack_oldn(8, &dma.NAFA_DMA_A08n), BitBase::pack_old(8, &dma.NAKY_DMA_A00p));
  d.dump_bitp   ("MATU_DMA_RUNNINGp : ", dma.MATU_DMA_RUNNINGp.state);
  d.dump_bitp   ("LYXE_DMA_LATCHp   : ", dma.LYXE_DMA_LATCHp  .state);
  d.dump_bitp   ("MYTE_DMA_DONE     : ", dma.MYTE_DMA_DONE    .state);
  d.dump_bitp   ("LUVY_DMA_TRIG_d0  : ", dma.LUVY_DMA_TRIG_d0 .state);
  d.dump_bitp   ("LENE_DMA_TRIG_d4  : ", dma.LENE_DMA_TRIG_d4 .state);
  d.dump_bitp   ("LOKY_DMA_LATCHp   : ", dma.LOKY_DMA_LATCHp  .get_state());
}


void GateBoy::dump_ext_bus(Dumper& d) {
  d.dump_slice2n("PIN_01_ADDR : ", &ext_pins.PIN_01_A00, 16);

  //d.dump_slice2n(   "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);
  //dump_slice2b  (d, "PIN_17_DATA : ", &ext_pins.PIN_17_D00, 8);

  d.dump_bitn   ("PIN_80_CSn  : ", ext_pins.PIN_80_CSn.qp_ext_old());
  d.dump_bitn   ("PIN_79_RDn  : ", ext_pins.PIN_79_RDn.qp_ext_old());
  d.dump_bitn   ("PIN_78_WRn  : ", ext_pins.PIN_78_WRn.qp_ext_old());
  d.dump_slice2p("ADDR LATCH  : ", &ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p, 15);
  d.dump_slice2n("DATA LATCH  : ", &ext_data_latch.SOMA_EXT_DATA_LATCH_D0n, 8);
}

void GateBoy::dump_vram_bus(Dumper& d) {
  d.dump_bitp   ("PIN_43_VRAM_CSn  : ", vram_pins.PIN_43_VRAM_CSn.qp_ext_old());
  d.dump_bitp   ("PIN_45_VRAM_OEn  : ", vram_pins.PIN_45_VRAM_OEn.qp_ext_old());
  d.dump_bitp   ("PIN_49_VRAM_WRn  : ", vram_pins.PIN_49_VRAM_WRn.qp_ext_old());
  d.dump_slice2p("PIN_34_VRAM_ADDR : ", &vram_pins.PIN_34_VRAM_A00, 13);
  d.dump_slice2p("PIN_25_VRAM_DATA : ", &vram_pins.PIN_33_VRAM_D00, 8);
  d.dump_slice2n("BUS_VRAM_An      : ", &vram_bus.BUS_VRAM_A00n, 13);
  d.dump_slice2p("BUS_VRAM_Dp      : ", &vram_bus.BUS_VRAM_D00p, 8);
}

void GateBoy::dump_sprite_fetcher(Dumper& d) {
  d.dump_bitp   ("TAKA_SFETCH_RUNNINGp : ", sprite_fetcher.TAKA_SFETCH_RUNNINGp.state);
  d.dump_bitp   ("SOBU_SFETCH_REQp     : ", sprite_fetcher.SOBU_SFETCH_REQp    .state);
  d.dump_bitp   ("SUDA_SFETCH_REQp     : ", sprite_fetcher.SUDA_SFETCH_REQp    .state);
  d.dump_bitp   ("TOXE_SFETCH_S0       : ", sprite_fetcher.TOXE_SFETCH_S0p     .state);
  d.dump_bitp   ("TULY_SFETCH_S1       : ", sprite_fetcher.TULY_SFETCH_S1p     .state);
  d.dump_bitp   ("TESE_SFETCH_S2       : ", sprite_fetcher.TESE_SFETCH_S2p     .state);
  d.dump_bitp   ("TYFO_SFETCH_S0_D1    : ", sprite_fetcher.TYFO_SFETCH_S0p_D1  .state);
  d.dump_bitp   ("TOBU_SFETCH_S1_D2    : ", sprite_fetcher.TOBU_SFETCH_S1p_D2  .state);
  d.dump_bitp   ("VONU_SFETCH_S1_D4    : ", sprite_fetcher.VONU_SFETCH_S1p_D4  .state);
  d.dump_bitp   ("SEBA_SFETCH_S1_D5    : ", sprite_fetcher.SEBA_SFETCH_S1p_D5  .state);
  d.dump_slice2n("Temp A : ", &sprite_pix_a.REWO_SPRITE_DA0n, 8);
  d.dump_slice2n("Temp B : ", &sprite_pix_b.PEFO_SPRITE_DB0n, 8);
}

void GateBoy::dump_timer(Dumper& d) {
  d.dump_slice2p("DIV16 : ", &div.UKUP_DIV00p, 16);
  d.dump_slice2p("FF04 DIV  : ", &div.UGOT_DIV06p, 8);
  d.dump_slice2p("FF05 TIMA : ", &timer.REGA_TIMA0p, 8);
  d.dump_slice2p("FF06 TMA  : ", &timer.SABU_TMA0p, 8);
  d.dump_slice2p("FF07 TAC  : ", &timer.SOPU_TAC0p, 3);
  d.dump_bitp   ("NYDU_TIMA7p_DELAY    : ", timer.NYDU_TIMA7p_DELAY.state);
  d.dump_bitp   ("MOBA_TIMER_OVERFLOWp : ", timer.MOBA_TIMER_OVERFLOWp.state);
}

void GateBoy::dump_resets(Dumper& d) {
  d.dump_bitp("PIN_71_RST     : ", rst.PIN_71_RST.qp_ext_old());
  d.dump_bitp("PIN_77_T1      : ", rst.PIN_77_T1.qp_ext_old());
  d.dump_bitp("PIN_76_T2      : ", rst.PIN_76_T2.qp_ext_old());

  d.dump_bitp("TUBO_WAITINGp  : ", rst.TUBO_WAITINGp.state);
  d.dump_bitn("ASOL_POR_DONEn : ", rst.ASOL_POR_DONEn.state);
  d.dump_bitp("AFER_SYS_RSTp  : ", rst.AFER_SYS_RSTp.state);
  d.dump_bitp("SOTO_DBG_VRAMp : ", rst.SOTO_DBG_VRAMp.state);

  d.dump_bitp("SIG_CPU_EXT_CLKGOOD : ", rst.SIG_CPU_EXT_CLKGOOD.state);
  d.dump_bitp("SIG_CPU_EXT_RESETp  : ", rst.SIG_CPU_EXT_RESETp .state);
  d.dump_bitp("SIG_CPU_STARTp      : ", rst.SIG_CPU_STARTp     .state);
  d.dump_bitp("SIG_CPU_INT_RESETp  : ", rst.SIG_CPU_INT_RESETp .state);
}

void GateBoy::dump_sprite_scanner(Dumper& d) {
  d.dump_slice2p("SCAN INDEX        : ", &sprite_scanner.YFEL_SCAN0, 6);
  d.dump_bitp   ("BESU_SCANNINGp    : ", sprite_scanner.BESU_SCANNINGp.state);
  d.dump_bitp   ("CENO_SCANNINGp    : ", sprite_scanner.CENO_SCANNINGn.state);
  d.dump_bitp   ("BYBA_SCAN_DONE_Ap : ", sprite_scanner.BYBA_SCAN_DONE_Ap.state);
  d.dump_bitp   ("DOBA_SCAN_DONE_Bp : ", sprite_scanner.DOBA_SCAN_DONE_Bp.state);
}

void GateBoy::dump_serial(Dumper& d) {
  d.dump_bitp   ("ETAF_SER_RUNNING : ", serial.ETAF_SER_RUN.state);
  d.dump_bitp   ("CULY_XFER_DIR    : ", serial.CULY_SER_DIR.state);
  d.dump_bitp   ("COTY_SER_CLK     : ", serial.COTY_SER_CLK.state);
  d.dump_bitp   ("ELYS_SER_OUT     : ", serial.ELYS_SER_OUT.state);
  d.dump_slice2p("CAFA_SER_CNT     : ", &serial.CAFA_SER_CNT0, 4);
  d.dump_slice2p("CUBA_SER_DATA    : ", &serial.CUBA_SER_DATA0, 8);
}

void GateBoy::dump_ppu(Dumper& d) {
  d.dump_slice2n("FF40 LCDC  : ", &reg_lcdc.VYXE_LCDC_BGENn, 8);
  d.dump_slice2n("FF41 STAT  : ", &reg_stat.ROXE_STAT_HBI_ENn, 4);
  d.dump_slice2n("FF42 SCY   : ", &reg_scy.GAVE_SCY0n, 8);
  d.dump_slice2n("FF43 SCX   : ", &reg_scx.DATY_SCX0n, 8);
  d.dump_slice2n("FF47 BGP   : ", &reg_bgp.PAVO_BGP_D0n, 8);
  d.dump_slice2n("FF48 OBP0  : ", &reg_obp0.XUFU_OBP0_D0n, 8);
  d.dump_slice2n("FF49 OBP1  : ", &reg_obp1.MOXY_OBP1_D0n, 8);
  d.dump_slice2n("FF4A WY    : ", &reg_wy.NESO_WY0n, 8);
  d.dump_slice2n("FF4B WX    : ", &reg_wx.MYPA_WX0n, 8);
  d.dump_slice2p("WIN MAP X  : ", &win_coords.WYKA_WIN_MAP_X0, 5);
  d.dump_slice2p("WIN MAP Y  : ", &win_coords.TUFU_WIN_MAP_Y0, 8);
  d.dump_slice2p("WIN TILE Y : ", &win_coords.VYNO_WIN_TILE_Y0, 3);
  d("\n");
  d.dump_slice2p("PIX COUNT  : ", &pix_count.XEHO_PX0p, 8);
  d.dump_slice2p("BG PIPE A  : ", &pix_pipes.MYDE_BGW_PIPE_A0, 8);
  d.dump_slice2p("BG PIPE B  : ", &pix_pipes.TOMY_BGW_PIPE_B0, 8);
  d.dump_slice2p("SPR PIPE A : ", &pix_pipes.NURO_SPR_PIPE_A0, 8);
  d.dump_slice2p("SPR PIPE B : ", &pix_pipes.NYLU_SPR_PIPE_B0, 8);
  d.dump_slice2p("PAL PIPE   : ", &pix_pipes.RUGO_PAL_PIPE_D0, 8);
  d.dump_slice2p("MASK PIPE  : ", &pix_pipes.VEZO_MASK_PIPE_0, 8);
  d.dump_bitn   ("REMY_LD0n  : ", pix_pipes.REMY_LD0n.get_state());
  d.dump_bitn   ("RAVO_LD1n  : ", pix_pipes.RAVO_LD1n.get_state());
  d("\n");
  d.dump_bitp("XYMU_RENDERINGn        : ", XYMU_RENDERINGn.state);
  d.dump_bitp("PYNU_WIN_MODE_Ap       : ", win_reg.PYNU_WIN_MODE_Ap.state);
  d.dump_bitp("PUKU_WIN_HITn          : ", win_reg.PUKU_WIN_HITn.get_state());
  d.dump_bitp("RYDY_WIN_HITp          : ", win_reg.RYDY_WIN_HITp.get_state());
  d.dump_bitp("SOVY_WIN_FIRST_TILE_B  : ", win_reg.SOVY_WIN_HITp.state);
  d.dump_bitp("NOPA_WIN_MODE_B        : ", win_reg.NOPA_WIN_MODE_Bp.state);
  d.dump_bitp("PYCO_WX_MATCH_A        : ", win_reg.PYCO_WIN_MATCHp.state);
  d.dump_bitp("NUNU_WX_MATCH_B        : ", win_reg.NUNU_WIN_MATCHp.state);
  d.dump_bitp("REJO_WY_MATCH_LATCH    : ", win_reg.REJO_WY_MATCH_LATCHp.state);
  d.dump_bitp("SARY_WY_MATCH          : ", win_reg.SARY_WY_MATCHp.state);
  d.dump_bitp("RYFA_FETCHn_A          : ", win_reg.RYFA_WIN_FETCHn_A.state);
  d.dump_bitp("RENE_FETCHn_B          : ", win_reg.RENE_WIN_FETCHn_B.state);
  d.dump_bitp("RYKU_FINE_CNT0         : ", fine_scroll.RYKU_FINE_CNT0.state);
  d.dump_bitp("ROGA_FINE_CNT1         : ", fine_scroll.ROGA_FINE_CNT1.state);
  d.dump_bitp("RUBU_FINE_CNT2         : ", fine_scroll.RUBU_FINE_CNT2.state);
  d.dump_bitp("PUXA_FINE_MATCH_A      : ", fine_scroll.PUXA_SCX_FINE_MATCH_A.state);
  d.dump_bitp("NYZE_FINE_MATCH_B      : ", fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
  d.dump_bitp("ROXY_FINE_SCROLL_DONEn : ", fine_scroll.ROXY_FINE_SCROLL_DONEn.state);
  d.dump_bitp("VOGA_HBLANKp           : ", VOGA_HBLANKp.state);
  d("\n");
}

void GateBoy::dump_spu(Dumper& d) {
  d.dump_bitp   ("HADA_ALL_SOUND_ONp     : ", reg_NR52.HADA_ALL_SOUND_ONp.state);
  d.dump_slice2p("NR50 : ", &reg_NR50.APEG_VOL_L0, 8);


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