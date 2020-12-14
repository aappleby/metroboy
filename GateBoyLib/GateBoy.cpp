#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

#include <set>

//#pragma warning(disable:4189) // local variable unused
#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void GateBoy::reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                            uint8_t* _cart_buf, size_t _cart_size,
                            bool fastboot)
{
  memset(this, 0, sizeof(*this));
  sentinel1 = SENTINEL1;
  sentinel2 = SENTINEL2;
  sentinel3 = SENTINEL3;
  sentinel4 = SENTINEL4;

  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  // Run the sim twice to settle initial reset signals.

  tock_slow(0);
  commit_and_hash();
  tock_slow(0);
  commit_and_hash();

  //----------------------------------------
  // In reset

  sys_rst = 1;
  sys_fastboot = fastboot;
  run(5);

  // Out of reset
  // Start clock and sync with phase
  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run(3);

  CHECK_N(pclk.AFUR_xxxxEFGHp.qp_old());
  CHECK_P(pclk.ALEF_AxxxxFGHp.qp_old());
  CHECK_P(pclk.APUK_ABxxxxGHp.qp_old());
  CHECK_P(pclk.ADYK_ABCxxxxHp.qp_old());

  // Wait for PIN_CPU_START
  while(!rstdbg.PIN_CPU_STARTp.qp_any()) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset_boot();
  sys_clkreq = 1;
  sys_cpu_en = true;

  if (fastboot) {
    div.TERO_DIV03p_evn.reset(REG_D0C1);
    div.UNYK_DIV04p_evn.reset(REG_D0C1);
    div.UPOF_DIV15p_evn.reset(REG_D1C1);
  }

  // And clear the framebuffer
  memset(framebuffer, 4, sizeof(framebuffer));
}

//-----------------------------------------------------------------------------

void GateBoy::reset_cart(uint8_t* _boot_buf, size_t _boot_size, uint8_t* _cart_buf, size_t _cart_size) {
  reset_boot(_boot_buf, _boot_size, _cart_buf, _cart_size, true);

  PIN_CPU_BOOTp.reset(REG_D0C0);
  interrupts.PIN_CPU_INT_VBLANK.reset(REG_D1C0);
  rstdbg.PIN_CPU_STARTp.reset(REG_D0C0);

  pclk.PIN_CPU_BUKE_AxxxxxGH.reset(REG_D1C0);

  oam_latch_b.XYKY_OAM_LATCH_DB0n_odd.reset(REG_D1C0);
  oam_latch_b.YRUM_OAM_LATCH_DB1n_odd.reset(REG_D1C0);
  oam_latch_b.YSEX_OAM_LATCH_DB2n_odd.reset(REG_D1C0);
  oam_latch_b.YVEL_OAM_LATCH_DB3n_odd.reset(REG_D1C0);
  oam_latch_b.WYNO_OAM_LATCH_DB4n_odd.reset(REG_D1C0);
  oam_latch_b.CYRA_OAM_LATCH_DB5n_odd.reset(REG_D1C0);
  oam_latch_b.ZUVE_OAM_LATCH_DB6n_odd.reset(REG_D1C0);
  oam_latch_b.ECED_OAM_LATCH_DB7n_odd.reset(REG_D1C0);

  oam_latch_a.YDYV_OAM_LATCH_DA0n_odd.reset(REG_D1C0);
  oam_latch_a.YCEB_OAM_LATCH_DA1n_odd.reset(REG_D1C0);
  oam_latch_a.ZUCA_OAM_LATCH_DA2n_odd.reset(REG_D1C0);
  oam_latch_a.WONE_OAM_LATCH_DA3n_odd.reset(REG_D1C0);
  oam_latch_a.ZAXE_OAM_LATCH_DA4n_odd.reset(REG_D1C0);
  oam_latch_a.XAFU_OAM_LATCH_DA5n_odd.reset(REG_D1C0);
  oam_latch_a.YSES_OAM_LATCH_DA6n_odd.reset(REG_D1C0);
  oam_latch_a.ZECA_OAM_LATCH_DA7n_odd.reset(REG_D1C0);

  oam_temp_a.XUSO_OAM_DA0p_evn.reset(REG_D0C1);
  oam_temp_a.XEGU_OAM_DA1p_evn.reset(REG_D0C1);
  oam_temp_a.YJEX_OAM_DA2p_evn.reset(REG_D0C1);
  oam_temp_a.XYJU_OAM_DA3p_evn.reset(REG_D0C1);
  oam_temp_a.YBOG_OAM_DA4p_evn.reset(REG_D0C1);
  oam_temp_a.WYSO_OAM_DA5p_evn.reset(REG_D0C1);
  oam_temp_a.XOTE_OAM_DA6p_evn.reset(REG_D0C1);
  oam_temp_a.YZAB_OAM_DA7p_evn.reset(REG_D0C1);

  oam_temp_b.YLOR_OAM_DB0p_evn.reset(REG_D0C1);
  oam_temp_b.ZYTY_OAM_DB1p_evn.reset(REG_D0C1);
  oam_temp_b.ZYVE_OAM_DB2p_evn.reset(REG_D0C1);
  oam_temp_b.ZEZY_OAM_DB3p_evn.reset(REG_D0C1);
  oam_temp_b.GOMO_OAM_DB4p_evn.reset(REG_D0C1);
  oam_temp_b.BAXO_OAM_DB5p_evn.reset(REG_D0C1);
  oam_temp_b.YZOS_OAM_DB6p_evn.reset(REG_D0C1);
  oam_temp_b.DEPO_OAM_DB7p_evn.reset(REG_D0C1);

  oam_bus.BUS_OAM_An[0].reset(REG_D1C0);
  oam_bus.BUS_OAM_An[1].reset(REG_D1C0);
  oam_bus.BUS_OAM_An[2].reset(REG_D1C0);
  oam_bus.BUS_OAM_An[3].reset(REG_D1C0);
  oam_bus.BUS_OAM_An[4].reset(REG_D0C0);
  oam_bus.BUS_OAM_An[5].reset(REG_D1C0);
  oam_bus.BUS_OAM_An[6].reset(REG_D0C0);
  oam_bus.BUS_OAM_An[7].reset(REG_D1C0);

  ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.reset(REG_D1C0);
  ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.reset(REG_D0C0);
  ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.reset(REG_D1C0);
  ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.reset(REG_D1C0);
  ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.reset(REG_D0C0);
  ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.reset(REG_D0C0);
  ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.reset(REG_D1C0);
  ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.reset(REG_D0C0);
  ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.reset(REG_D0C0);
  ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.reset(REG_D0C0);
  ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.reset(REG_D0C0);
  ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.reset(REG_D0C0);
  ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.reset(REG_D0C0);
  ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.reset(REG_D0C0);
  ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.reset(REG_D0C0);

  ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.reset(REG_D1C0);
  ext_data_latch.RONY_EXT_DATA_LATCH_D1n.reset(REG_D0C0);
  ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.reset(REG_D1C0);
  ext_data_latch.SELO_EXT_DATA_LATCH_D3n.reset(REG_D0C0);
  ext_data_latch.SODY_EXT_DATA_LATCH_D4n.reset(REG_D1C0);
  ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.reset(REG_D1C0);
  ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.reset(REG_D1C0);
  ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.reset(REG_D1C0);

  ext_bus.PIN_EXT_A[ 0].reset(REG_D1C0);
  ext_bus.PIN_EXT_A[ 1].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[ 2].reset(REG_D1C0);
  ext_bus.PIN_EXT_A[ 3].reset(REG_D1C0);
  ext_bus.PIN_EXT_A[ 4].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[ 5].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[ 6].reset(REG_D1C0);
  ext_bus.PIN_EXT_A[ 7].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[ 8].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[ 9].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[10].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[11].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[12].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[13].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[14].reset(REG_D0C0);
  ext_bus.PIN_EXT_A[15].reset(REG_D1C0);

  ext_bus.PIN_EXT_D[0].reset(REG_D0C0);
  ext_bus.PIN_EXT_D[1].reset(REG_D1C0);
  ext_bus.PIN_EXT_D[2].reset(REG_D0C0);
  ext_bus.PIN_EXT_D[3].reset(REG_D1C0);
  ext_bus.PIN_EXT_D[4].reset(REG_D0C0);
  ext_bus.PIN_EXT_D[5].reset(REG_D0C0);
  ext_bus.PIN_EXT_D[6].reset(REG_D0C0);
  ext_bus.PIN_EXT_D[7].reset(REG_D0C0);

  vram_bus.BUS_VRAM_An[ 0].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 1].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 2].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 3].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 4].reset(REG_D0C0);
  vram_bus.BUS_VRAM_An[ 5].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 6].reset(REG_D0C0);
  vram_bus.BUS_VRAM_An[ 7].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 8].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[ 9].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[10].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[11].reset(REG_D1C0);
  vram_bus.BUS_VRAM_An[12].reset(REG_D1C0);

  vram_bus.PIN_VRAM_Ap[ 0].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 1].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 2].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 3].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 4].reset(REG_D1C0);
  vram_bus.PIN_VRAM_Ap[ 5].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 6].reset(REG_D1C0);
  vram_bus.PIN_VRAM_Ap[ 7].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 8].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[ 9].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[10].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[11].reset(REG_D0C0);
  vram_bus.PIN_VRAM_Ap[12].reset(REG_D0C0);

  rstdbg.TUBO_WAITINGp.reset(REG_D0C0);

  vclk.WUVU_ABxxEFxx.reset(REG_D1C1);
  vclk.VENA_xxCDEFxx.reset(REG_D0C0);
  vclk.WOSU_AxxDExxH.reset(REG_D1C0);

  div.UKUP_DIV00p_evn.reset(REG_D1C1);
  div.UFOR_DIV01p_evn.reset(REG_D1C0);
  div.UNER_DIV02p_evn.reset(REG_D0C0);
  div.TERO_DIV03p_evn.reset(REG_D0C1);
  div.UNYK_DIV04p_evn.reset(REG_D1C1);
  div.TAMA_DIV05p_evn.reset(REG_D1C0);
  div.UGOT_DIV06p_evn.reset(REG_D1C0);
  div.TULU_DIV07p_evn.reset(REG_D1C0);
  div.TUGO_DIV08p_evn.reset(REG_D0C0);
  div.TOFE_DIV09p_evn.reset(REG_D1C1);
  div.TERU_DIV10p_evn.reset(REG_D0C0);
  div.SOLA_DIV11p_evn.reset(REG_D1C1);
  div.SUBU_DIV12p_evn.reset(REG_D0C0);
  div.TEKA_DIV13p_evn.reset(REG_D1C1);
  div.UKET_DIV14p_evn.reset(REG_D1C0);
  div.UPOF_DIV15p_evn.reset(REG_D1C0);

  bootrom.BOOT_BITn_h.reset(REG_D1C1);

  interrupts.LOPE_FF0F_D0p.reset(REG_D1C1);

  serial.COTY_SER_CLK.reset(REG_D0C0);

  sprite_counter.DEZY_COUNT_CLKp_evn.reset(REG_D1C1);
  sprite_counter.BESE_SPRITE_COUNT0_evn.reset(REG_D0C1);
  sprite_counter.CUXY_SPRITE_COUNT1_evn.reset(REG_D0C1);
  sprite_counter.BEGO_SPRITE_COUNT2_evn.reset(REG_D0C1);
  sprite_counter.DYBE_SPRITE_COUNT3_evn.reset(REG_D0C1);

  sprite_store.XADU_SPRITE_IDX0p_evn.reset(REG_D0C1);
  sprite_store.XEDY_SPRITE_IDX1p_evn.reset(REG_D0C1);
  sprite_store.ZUZE_SPRITE_IDX2p_evn.reset(REG_D1C1);
  sprite_store.XOBE_SPRITE_IDX3p_evn.reset(REG_D0C1);
  sprite_store.YDUF_SPRITE_IDX4p_evn.reset(REG_D1C1);
  sprite_store.XECU_SPRITE_IDX5p_evn.reset(REG_D0C1);

  sprite_store.SPR_TRI_I[0].reset(REG_D0C0);
  sprite_store.SPR_TRI_I[1].reset(REG_D0C0);
  sprite_store.SPR_TRI_I[2].reset(REG_D1C0);
  sprite_store.SPR_TRI_I[3].reset(REG_D0C0);
  sprite_store.SPR_TRI_I[4].reset(REG_D1C0);
  sprite_store.SPR_TRI_I[5].reset(REG_D0C0);

  sprite_store.SPR_TRI_L[0].reset(REG_D1C0);
  sprite_store.SPR_TRI_L[1].reset(REG_D1C0);
  sprite_store.SPR_TRI_L[2].reset(REG_D1C0);
  sprite_store.SPR_TRI_L[3].reset(REG_D1C0);

  sprite_scanner.BESU_SCANNINGp_evn.reset(REG_D0C0);
  sprite_scanner.CENO_SCANNINGp_evn.reset(REG_D0C1);

  sprite_scanner.BYBA_SCAN_DONE_Ap_evn.reset(REG_D1C1);
  sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.reset(REG_D1C0);

  scan_counter.YFEL_SCAN0_evn.reset(REG_D1C1);
  scan_counter.WEWY_SCAN1_evn.reset(REG_D1C0);
  scan_counter.GOSO_SCAN2_evn.reset(REG_D1C0);
  scan_counter.ELYN_SCAN3_evn.reset(REG_D0C0);
  scan_counter.FAHA_SCAN4_evn.reset(REG_D0C1);
  scan_counter.FONY_SCAN5_evn.reset(REG_D1C1);

  sprite_fetcher.TAKA_SFETCH_RUNNINGp_xxx.reset(REG_D0C0);
  sprite_fetcher.SOBU_SFETCH_REQp_odd.reset(REG_D0C0);
  sprite_fetcher.SUDA_SFETCH_REQp_evn.reset(REG_D0C1);
  sprite_fetcher.TOXE_SFETCH_S0p_odd.reset(REG_D1C1);
  sprite_fetcher.TULY_SFETCH_S1p_odd.reset(REG_D0C0);
  sprite_fetcher.TESE_SFETCH_S2p_odd.reset(REG_D1C1);
  sprite_fetcher.TYFO_SFETCH_S0p_D1_evn.reset(REG_D1C1);
  sprite_fetcher.TOBU_SFETCH_S1p_D2_odd.reset(REG_D0C0);
  sprite_fetcher.VONU_SFETCH_S1p_D4_odd.reset(REG_D0C0);
  sprite_fetcher.SEBA_SFETCH_S1p_D5_evn.reset(REG_D0C1);

  reg_stat.RUPO_STAT_LYC_MATCHn_evn.reset(REG_D0C0);
  ppu_reg.VOGA_HBLANKp_xxx.reset(REG_D1C0);

  pix_count.XEHO_PX0p.reset(REG_D1C1);
  pix_count.SAVY_PX1p.reset(REG_D1C1);
  pix_count.XODU_PX2p.reset(REG_D1C1);
  pix_count.XYDO_PX3p.reset(REG_D0C1);
  pix_count.TUHU_PX4p.reset(REG_D0C1);
  pix_count.TUKY_PX5p.reset(REG_D1C1);
  pix_count.TAKO_PX6p.reset(REG_D0C1);
  pix_count.SYBE_PX7p.reset(REG_D1C1);

  pix_pipes.VEZO_MASK_PIPE_0_evn.reset(REG_D1C1);
  pix_pipes.WURU_MASK_PIPE_1_evn.reset(REG_D1C1);
  pix_pipes.VOSA_MASK_PIPE_2_evn.reset(REG_D1C1);
  pix_pipes.WYFU_MASK_PIPE_3_evn.reset(REG_D1C1);
  pix_pipes.XETE_MASK_PIPE_4_evn.reset(REG_D1C1);
  pix_pipes.WODA_MASK_PIPE_5_evn.reset(REG_D1C1);
  pix_pipes.VUMO_MASK_PIPE_6_evn.reset(REG_D1C1);
  pix_pipes.VAVA_MASK_PIPE_7_evn.reset(REG_D1C1);

  reg_bgp.PAVO_BGP_D0n_h.reset(REG_D1C1);
  reg_bgp.NUSY_BGP_D1n_h.reset(REG_D1C1);
  reg_bgp.PYLU_BGP_D2n_h.reset(REG_D0C1);
  reg_bgp.MAXY_BGP_D3n_h.reset(REG_D0C1);
  reg_bgp.MUKE_BGP_D4n_h.reset(REG_D0C1);
  reg_bgp.MORU_BGP_D5n_h.reset(REG_D0C1);
  reg_bgp.MOGY_BGP_D6n_h.reset(REG_D0C1);
  reg_bgp.MENA_BGP_D7n_h.reset(REG_D0C1);

  reg_lcdc.VYXE_LCDC_BGENn_h  .reset(REG_D0C1);
  reg_lcdc.XYLO_LCDC_SPENn_h  .reset(REG_D1C1);
  reg_lcdc.XYMO_LCDC_SPSIZEn_h.reset(REG_D1C1);
  rstdbg.XONA_LCDC_LCDENn_h .reset(REG_D0C1);
  reg_lcdc.XAFO_LCDC_BGMAPn_h .reset(REG_D1C1);
  reg_lcdc.WEXU_LCDC_BGTILEn_h.reset(REG_D0C1);
  reg_lcdc.WYMO_LCDC_WINENn_h .reset(REG_D1C1);
  reg_lcdc.WOKY_LCDC_WINMAPn_h.reset(REG_D1C1);

  reg_lx.RUTU_x113p_g.reset(REG_D0C1);
  reg_lx.NYPE_x113p_c.reset(REG_D0C0);
  reg_lx.SAXO_LX0p_evn.reset(REG_D0C0);
  reg_lx.TYPO_LX1p_evn.reset(REG_D1C1);
  reg_lx.VYZO_LX2p_evn.reset(REG_D0C0);
  reg_lx.TELU_LX3p_evn.reset(REG_D0C1);
  reg_lx.SUDE_LX4p_evn.reset(REG_D0C1);
  reg_lx.TAHA_LX5p_evn.reset(REG_D1C1);
  reg_lx.TYRY_LX6p_evn.reset(REG_D1C0);

  reg_ly.MYTA_y153p_evn.reset(REG_D1C0);
  reg_ly.MUWY_LY0p_evn.reset(REG_D0C0);
  reg_ly.MYRO_LY1p_evn.reset(REG_D0C1);
  reg_ly.LEXA_LY2p_evn.reset(REG_D0C1);
  reg_ly.LYDO_LY3p_evn.reset(REG_D0C1);
  reg_ly.LOVU_LY4p_evn.reset(REG_D0C1);
  reg_ly.LEMA_LY5p_evn.reset(REG_D0C1);
  reg_ly.MATO_LY6p_evn.reset(REG_D0C1);
  reg_ly.LAFO_LY7p_evn.reset(REG_D0C1);

  reg_lyc.ROPO_LY_MATCH_SYNCp_c.reset(REG_D1C0);

  lcd.CATU_LINE_P000p_a.reset(REG_D0C1);
  lcd.ANEL_LINE_P002p_c.reset(REG_D0C0);
  lcd.POPU_VBLANKp_evn.reset(REG_D1C0);
  lcd.SYGU_LINE_STROBE_evn.reset(REG_D0C1);
  lcd.MEDA_VSYNC_OUTn_evn.reset(REG_D1C1);
  lcd.LUCA_LINE_EVENp_evn.reset(REG_D1C1);
  lcd.NAPO_FRAME_EVENp_evn.reset(REG_D0C1);
  lcd.RUJU.reset(REG_D1C0);
  lcd.POFY.reset(REG_D0C0);
  lcd.POME.reset(REG_D1C0);
  lcd.PAHO_X_8_SYNC_odd.reset(REG_D0C1);
  lcd.WUSA_LCD_CLOCK_GATE.reset(REG_D0C0);

  lcd.PIN_LCD_DATA1.reset(REG_D0C0);
  lcd.PIN_LCD_DATA0.reset(REG_D0C0);
  lcd.PIN_LCD_HSYNC_evn.reset(REG_D0C0);
  lcd.PIN_LCD_FLIPS_evn.reset(REG_D0C0);
  lcd.PIN_LCD_CNTRL_evn.reset(REG_D0C0);
  lcd.PIN_LCD_LATCH_evn.reset(REG_D0C0);
  lcd.PIN_LCD_CLOCK_xxx.reset(REG_D0C0);
  lcd.PIN_LCD_VSYNC_evn.reset(REG_D1C0);

  lcd.lcd_pix_lo.reset(REG_D0C0);
  lcd.lcd_pix_hi.reset(REG_D0C0);
  memset(lcd.lcd_pipe_lo, 0x02, sizeof(lcd.lcd_pipe_lo));
  memset(lcd.lcd_pipe_hi, 0x02, sizeof(lcd.lcd_pipe_hi));

  sys_rst = false;
  sys_t1 = false;
  sys_t2 = false;
  sys_clken = true;
  sys_clkgood = true;
  sys_clkreq = true;
  sys_cpu_en = true;
  sys_fastboot = true;

  cpu.reset_cart();

  cpu_req.addr = 0xff50;
  cpu_req.data = 1;
  cpu_req.read = 0;
  cpu_req.write = 1;
  dbg_req = {0};
  bus_req = cpu_req;

  cpu_data_latch = 0xFF;
  imask_latch = 0;

  int_vblank = true;
  int_vblank_halt = true;
  int_stat = false;
  int_stat_halt = false;
  int_timer = false;
  int_timer_halt = false;
  int_serial = false;
  int_serial_halt = false;
  int_joypad = false;
  int_joypad_halt = false;

  memcpy(vid_ram, vram_boot, 8192);

  zero_ram[0x7A] = 0x39;
  zero_ram[0x7B] = 0x01;
  zero_ram[0x7C] = 0x2E;

  memcpy(framebuffer, framebuffer_boot, 160*144);

  gb_screen_x = 159;
  gb_screen_y = 152;

  sim_time = 169.62587129999756;
  phase_total = 46880728;
  phase_hash = 0xd53410c0b6bcb522;
  cumulative_hash = 0x2532ab22e64c63aa;
}

void GateBoy::set_cart(uint8_t* _boot_buf, size_t _boot_size,
                       uint8_t* _cart_buf, size_t _cart_size)
{
  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;
}


//------------------------------------------------------------------------------

void GateBoy::load_post_bootrom_state() {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();
  check_div();
  cart_buf = nullptr;
  cart_size = 0;
}

//------------------------------------------------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);

  dbg_req.addr = uint16_t(addr);
  dbg_req.data = 0;
  dbg_req.read = 1;
  dbg_req.write = 0;
  run(8);
  return cpu_data_latch;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  dbg_req.addr = uint16_t(addr);
  dbg_req.data = data;
  dbg_req.read = 0;
  dbg_req.write = 1;
  run(8);
  dbg_req = {0};
}

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_cpu_bus        = offsetof(GateBoy, BUS_CPU_D_out);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
  //const int o_ext_bus        = offsetof(GateBoy, ext_bus);
  //const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_rst_reg        = offsetof(GateBoy, rstdbg);
  const int o_pclk_reg       = offsetof(GateBoy, pclk);
  const int o_vclk_reg       = offsetof(GateBoy, vclk);
  const int o_div_reg        = offsetof(GateBoy, div);
  const int o_tim_reg        = offsetof(GateBoy, timer);
  const int o_dma_reg        = offsetof(GateBoy, dma);
  const int o_int_reg        = offsetof(GateBoy, interrupts);
  const int o_joypad         = offsetof(GateBoy, joypad);
  const int o_ser_reg        = offsetof(GateBoy, serial);
  const int o_sprite_store   = offsetof(GateBoy, sprite_store);
  const int o_sprite_scanner = offsetof(GateBoy, sprite_scanner);
  const int o_tile_fetcher   = offsetof(GateBoy, tile_fetcher);
  const int o_sprite_fetcher = offsetof(GateBoy, sprite_fetcher);
  const int o_pix_pipe       = offsetof(GateBoy, ppu_reg);
  const int o_lcd_reg        = offsetof(GateBoy, lcd);

  const int o_cpu            = offsetof(GateBoy, cpu);
  const int o_vid_ram        = offsetof(GateBoy, vid_ram);
  const int o_cart_ram       = offsetof(GateBoy, cart_ram);
  const int o_ext_ram        = offsetof(GateBoy, ext_ram);
  const int o_oam_ram        = offsetof(GateBoy, oam_ram);
  const int o_zero_ram       = offsetof(GateBoy, zero_ram);
  const int o_framebuffer    = offsetof(GateBoy, framebuffer);
  const int o_gb_screen_x    = offsetof(GateBoy, gb_screen_x);

} gb_offsets;

static std::set<int> bad_bits;

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::next_phase() {

  //----------------------------------------
  // Run one pass of our simulation.

  if (DELTA_AB) {
    cpu_req = cpu.bus_req;
    bus_req = {0};
    if (sys_cpu_en) bus_req = cpu_req;
    if (dbg_req.read || dbg_req.write) bus_req = dbg_req;
  }

  uint8_t* blob_begin = ((uint8_t*)&sentinel1) + sizeof(sentinel1);
  uint8_t* blob_end   = ((uint8_t*)&sentinel2);

  probes.begin_pass(0);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);

  tock_slow(0);
  probes.end_pass(false);

#ifdef CHECK_SINGLE_PASS
  uint64_t pass_hash_old = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  static GateBoy gb_old;
  gb_old = *this;

  probes.begin_pass(1);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);

  tock_slow(1);
  probes.end_pass(true);
#endif

  uint64_t phase_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

#ifdef CHECK_SINGLE_PASS
  if (pass_hash_old != phase_hash_new) {
    LOG_Y("Sim not stable after second pass!\n");

    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb_old;
    uint8_t* blob_new = (uint8_t*)this;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        printf("%06d %04d %02d %02d\n", phase_total, i, blob_old[i], blob_new[i]);
      }
    }

    printf("\n");

    ASSERT_P(false);
  }
#endif

  //----------------------------------------
  // Once the simulation converges, latch the data that needs to go back to the
  // CPU or test function and update the CPU if necessary.

  if (DELTA_GH) {
    cpu_data_latch = pack_u8p(8, &BUS_CPU_D_out[0]);
  }

  if (DELTA_DE && sys_cpu_en) {

    uint8_t intf = 0;
    if (int_vblank_halt) intf |= INT_VBLANK_MASK;
    if (int_stat_halt)   intf |= INT_STAT_MASK;
    if (int_timer_halt)  intf |= INT_TIMER_MASK;
    if (int_serial_halt) intf |= INT_SERIAL_MASK;
    if (int_joypad_halt) intf |= INT_JOYPAD_MASK;

    cpu.tock_de(imask_latch, intf);
  }

  //----------
  // CPU updates after HA.

  if (DELTA_HA && sys_cpu_en) {

    uint8_t intf = 0;
    if (int_vblank) intf |= INT_VBLANK_MASK;
    if (int_stat)   intf |= INT_STAT_MASK;
    if (int_timer)  intf |= INT_TIMER_MASK;
    if (int_serial) intf |= INT_SERIAL_MASK;
    if (int_joypad) intf |= INT_JOYPAD_MASK;

    cpu.tock_ha(imask_latch, intf, cpu_data_latch);
  }

  //----------
  // Done, move to the next phase.

  phase_total++;
  phase_hash = phase_hash_new;
  combine_hash(cumulative_hash, phase_hash);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {
  (void)pass_index;

  rstdbg.PIN_SYS_RST.setp(sys_rst);
  rstdbg.PIN_SYS_T1.setp(sys_t1);
  rstdbg.PIN_SYS_T2.setp(sys_t2);
  pclk.PIN_SYS_CLKGOOD.setp(sys_clkgood);
  pclk.PIN_SYS_CLK.setp(!(phase_total & 1) && sys_clken);
  pclk.PIN_SYS_CLKREQ.setp(sys_clkreq);

  if (joypad.PIN_JOY_P14.qp_old()) {
    joypad.PIN_JOY_P10.setn(sys_buttons & 0x01);
    joypad.PIN_JOY_P11.setn(sys_buttons & 0x02);
    joypad.PIN_JOY_P12.setn(sys_buttons & 0x04);
    joypad.PIN_JOY_P13.setn(sys_buttons & 0x08);
  }
  else if (joypad.PIN_JOY_P15.qp_old()) {
    joypad.PIN_JOY_P10.setn(sys_buttons & 0x10);
    joypad.PIN_JOY_P11.setn(sys_buttons & 0x20);
    joypad.PIN_JOY_P12.setn(sys_buttons & 0x40);
    joypad.PIN_JOY_P13.setn(sys_buttons & 0x80);
  }
  else {
    joypad.PIN_JOY_P10.setn(0);
    joypad.PIN_JOY_P11.setn(0);
    joypad.PIN_JOY_P12.setn(0);
    joypad.PIN_JOY_P13.setn(0);
  }

  /* hack */
  serial.PIN_SIN.pin_in(1, 1);
  serial.PIN_SCK.pin_in(0, 0);

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  uint16_t cpu_addr = DELTA_HA ? bus_req.addr & 0x00FF : bus_req.addr;

  // PIN_CPU_RDp / PIN_CPU_WRp
  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    PIN_CPU_RDp.setp(bus_req.read);
    PIN_CPU_WRp.setp(bus_req.write);
  }
  else {
    PIN_CPU_RDp.setp(0);
    PIN_CPU_WRp.setp(0);
  }

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
    PIN_CPU_LATCH_EXT.setp(1);
  }
  else {
    PIN_CPU_LATCH_EXT.setp(0);
  }

  PIN_CPU_6.setp(0);

  // Data has to be driven on EFGH or we fail the wave tests
  wire BUS_CPU_OEp = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;

  wire BUS_CPU_A[16] = {
    wire((cpu_addr >>  0) & 1),
    wire((cpu_addr >>  1) & 1),
    wire((cpu_addr >>  2) & 1),
    wire((cpu_addr >>  3) & 1),
    wire((cpu_addr >>  4) & 1),
    wire((cpu_addr >>  5) & 1),
    wire((cpu_addr >>  6) & 1),
    wire((cpu_addr >>  7) & 1),
    wire((cpu_addr >>  8) & 1),
    wire((cpu_addr >>  9) & 1),
    wire((cpu_addr >> 10) & 1),
    wire((cpu_addr >> 11) & 1),
    wire((cpu_addr >> 12) & 1),
    wire((cpu_addr >> 13) & 1),
    wire((cpu_addr >> 14) & 1),
    wire((cpu_addr >> 15) & 1)
  };

  wire BUS_CPU_D[8] = {
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 0) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 1) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 2) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 3) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 4) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 5) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 6) & 1),
    wire(!BUS_CPU_OEp || (bus_req.data_lo >> 7) & 1)
  };

  BUS_CPU_D_out[0].reset(REG_D1C0);
  BUS_CPU_D_out[1].reset(REG_D1C0);
  BUS_CPU_D_out[2].reset(REG_D1C0);
  BUS_CPU_D_out[3].reset(REG_D1C0);
  BUS_CPU_D_out[4].reset(REG_D1C0);
  BUS_CPU_D_out[5].reset(REG_D1C0);
  BUS_CPU_D_out[6].reset(REG_D1C0);
  BUS_CPU_D_out[7].reset(REG_D1C0);

  //------------------------------------------------------------------------------------------------------------------------
  // This pin is weird and I'm not sure I have it right.

  {
    bool addr_ext = (bus_req.read || bus_req.write) && (bus_req.addr < 0xFE00);
    if (bus_req.addr <= 0x00FF && !bootrom.BOOT_BITn_h.qp_old()) addr_ext = false;

    if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      PIN_CPU_EXT_BUSp.setp(addr_ext);
    }
    else {
      // This seems wrong, but it passes tests. *shrug*
      if (bus_req.addr >= 0x8000 && bus_req.addr <= 0x9FFF) {
        PIN_CPU_EXT_BUSp.setp(0);
      }
      else {
        PIN_CPU_EXT_BUSp.setp(addr_ext);
      }
    }
  }

  //------------------------------------------------------------------------------------------------------------------------
  // Address decoders

  {
    PIN_CPU_ADDR_HIp.setp(SYRO_FE00_FFFF_ext(BUS_CPU_A));
  }

  //----------------------------------------
  // Debug control signals.

  {
    PIN_CPU_UNOR_DBG.setp(rstdbg.UNOR_MODE_DBG2p());
    PIN_CPU_UMUT_DBG.setp(rstdbg.UMUT_MODE_DBG1p_ext());
  }

  //-----------------------------------------------------------------------------
  // We need the sprite match result from the previous cycle, so we recalculate it here. :/

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_old = not1(sprite_scanner.CENO_SCANNINGp_evn.qn_old());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_old = not1(_CEHA_SCANNINGp_old);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_old = and2(ppu_reg.XYMU_RENDERINGn.qn_old(), _BYJO_SCANNINGn_old);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_old = and2(_AZEM_RENDERINGp_old, reg_lcdc.XYLO_LCDC_SPENn_h.qn_old());

  SpriteMatch old_match = sprite_store.get_match(_AROR_MATCH_ENp_old, pix_count);

  /* p29.FEPO*/ wire FEPO_STORE_MATCHp_old = old_match.FEPO_STORE_MATCHp();

  SpriteGetFlag old_sprite_flag = old_match.get_flag();

  /*#p21.XENA*/ wire _XENA_STORE_MATCHn_old = not1(FEPO_STORE_MATCHp_old);
  /*#p21.WODU*/ wire WODU_HBLANKp_old = and2(_XENA_STORE_MATCHn_old, pix_count.XANO_PX167p());

  wire XYMU_RENDERINGp_old = ppu_reg.XYMU_RENDERINGp();

  wire XODO_VID_RSTp_old = rstdbg.XODO_VID_RSTp();

  wire TAVE_PRELOAD_DONE_TRIGp_old = tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGp_old);

  wire TEVO_FETCH_TRIGp_old = or3(win_reg.SEKO_WIN_FETCH_TRIGp(), win_reg.SUZU_WIN_FIRST_TILEne(), TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  wire BALU_LINE_RSTp_old = lcd.BALU_LINE_RSTp(XODO_VID_RSTp_old);

  wire AVAP_SCAN_DONE_TRIGp_old = sprite_scanner.AVAP_SCAN_DONE_TRIGp(BALU_LINE_RSTp_old);

  wire NYXU_BFETCH_RSTn_old = nor3(AVAP_SCAN_DONE_TRIGp_old, win_reg.MOSU_WIN_MODE_TRIGp(), TEVO_FETCH_TRIGp_old);

  wire MOCE_BFETCH_DONEn_old = tile_fetcher.MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn_old);
  wire LYRY_BFETCH_DONEp_old = tile_fetcher.LYRY_BFETCH_DONEp(NYXU_BFETCH_RSTn_old);
  wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp_old, win_reg.TUKU_WIN_HITn(), LYRY_BFETCH_DONEp_old, sprite_fetcher.SOWO_SFETCH_RUNNINGn());


  //----------------------------------------
  // Sys clock signals

  pclk.tock(rstdbg);

  //----------------------------------------
  // CPU read signals

  /* p07.TEDO*/ bool TEDO_CPU_RDp;
  /* p01.APOV*/ bool APOV_CPU_WRp;
  /* p07.TAPU*/ bool TAPU_CPU_WRp;

  [
    this,
    &TEDO_CPU_RDp,
    &APOV_CPU_WRp,
    &TAPU_CPU_WRp
  ](){
    /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rstdbg.UNOR_MODE_DBG2p(), /*PIN_EXT_RDn.qn_new()*/ 0, PIN_CPU_RDp.qp_new()); // Ignoring debug stuff for now
    /* p07.TEDO*/ TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

    /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(pclk.ADAR_ABCxxxxH(), pclk.ATYP_ABCDxxxx());
    /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(PIN_CPU_WRp.qp_new(), _AFAS_xxxxEFGx);
    /* p01.APOV*/ APOV_CPU_WRp = not1(_AREV_CPU_WRn);

    /* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rstdbg.UNOR_MODE_DBG2p(), /*PIN_EXT_WRn.qn_new()*/ 0, APOV_CPU_WRp); // Ignoring debug stuff for now
    /* p07.TAPU*/ TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx
  }();


  //------------------------------------------------------------------------------------------------------------------------
  // DIV

  div.tock(
    rstdbg.PIN_SYS_RST.qp_new(),
    BUS_CPU_A,
    pclk.UCOB_CLKBADp(),
    pclk.BOGA_Axxxxxxx(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    BUS_CPU_D_out
  );

  //------------------------------------------------------------------------------------------------------------------------
  // Power-on reset handler

  rstdbg.tock(
    pclk,
    sys_clkreq,
    sys_clkgood,
    sys_fastboot ? div.TERO_DIV03p_evn.qp_new() : div.UPOF_DIV15p_evn.qp_new()
  );

  //------------------------------------------------------------------------------------------------------------------------

  /* p04.DECY*/ wire DECY_LATCH_EXTn_ext = not1(PIN_CPU_LATCH_EXT.qp_new());
  /* p04.CATY*/ wire CATY_LATCH_EXTp_ext = not1(DECY_LATCH_EXTn_ext);

  /* p04.MAKA*/ oam_bus.MAKA_LATCH_EXTp_evn.dff17(pclk.ZEME_AxCxExGx(), rstdbg.AVOR_SYS_RSTp(), CATY_LATCH_EXTp_ext);

  //------------------------------------------------------------------------------------------------------------------------
  // CPU Bootrom read

  bootrom.tock
  (
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.UMUT_MODE_DBG1p_ext(),
    rstdbg.AVOR_SYS_RSTp(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    boot_buf,
    BUS_CPU_D_out
  );

  PIN_CPU_BOOTp.setp(bootrom.TUTU_READ_BOOTROMp_new(BUS_CPU_A));

  //------------------------------------------------------------------------------------------------------------------------
  // Timer

  timer.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    pclk.BOGA_Axxxxxxx(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    PIN_CPU_LATCH_EXT.qp_new(),
    BUS_CPU_D_out,
    div);

  //------------------------------------------------------------------------------------------------------------------------
  // LCDC. Has to be near the top as it controls the video reset signal

  reg_lcdc.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    BUS_CPU_D_out
  );

  rstdbg.tock2(BUS_CPU_A,
               BUS_CPU_D,
               TEDO_CPU_RDp,
               TAPU_CPU_WRp,
               BUS_CPU_D_out);

  /* p01.XODO*/ wire XODO_VID_RSTp = nand2(rstdbg.XEBE_SYS_RSTn(), rstdbg.XONA_LCDC_LCDENn_h.qn_new());
  /* p23.WYMO*/ wire WYMO_LCDC_WINENn_new = reg_lcdc.WYMO_LCDC_WINENn_h.qn_new();
  /* p23.XONA*/ wire XONA_LCDC_LCDENp_h_new = rstdbg.XONA_LCDC_LCDENn_h.qn_new();

  //------------------------------------------------------------------------------------------------------------------------
  // Video clocks

  vclk.tock(rstdbg, pclk);
  dma.tock(rstdbg.AVOR_SYS_RSTp(), pclk.UVYT_ABCDxxxx(), BUS_CPU_A, BUS_CPU_D, TEDO_CPU_RDp, TAPU_CPU_WRp, BUS_CPU_D_out);

  //------------------------------------------------------------------------------------------------------------------------

  reg_lx.tock(
    XODO_VID_RSTp,
    vclk.TALU_xxCDEFxx());

  reg_ly.tock(
    BUS_CPU_A,
    XODO_VID_RSTp,
    TEDO_CPU_RDp,
    reg_lx.NYPE_x113p_c.qp_new(),
    reg_lx.RUTU_x113p_g.qp_new(),
    BUS_CPU_D_out);

  reg_lyc.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    vclk.TALU_xxCDEFxx(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    reg_ly,
    BUS_CPU_D_out);

  lcd.tock(
    XODO_VID_RSTp,
    vclk.XUPY_ABxxEFxx(),
    reg_lx,
    reg_ly);

  //------------------------------------------------------------------------------------------------------------------------

  /* p21.TADY*/ wire _TADY_LINE_RSTn_new_evn  = nor2(lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp), rstdbg.TOFU_VID_RSTp());
  /*#p21.VOGA*/ ppu_reg.VOGA_HBLANKp_xxx.dff17(pclk.ALET_xBxDxFxH(), _TADY_LINE_RSTn_new_evn, WODU_HBLANKp_old);

  /*#p21.WEGO*/ wire WEGO_HBLANKp_new_any = or2(rstdbg.TOFU_VID_RSTp(), ppu_reg.VOGA_HBLANKp_xxx.qp_new());

  //------------------------------------------------------------------------------------------------------------------------
  // Window match comparator

  /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old = ROGE_WY_MATCHp(/*old*/ reg_wy, /*old*/ reg_ly, reg_lcdc.WYMO_LCDC_WINENn_h.qn_old());

  /* p27.SARY*/ win_reg.SARY_WY_MATCHp_evn.dff17(vclk.TALU_xxCDEFxx(), rstdbg.XAPO_VID_RSTn(), _ROGE_WY_MATCHp_old);
  /* p27.REPU*/ wire _REPU_VBLANKp_new_evn = or2(lcd.PARU_VBLANKp(), rstdbg.PYRY_VID_RSTp());
  /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp_evn.nor_latch(win_reg.SARY_WY_MATCHp_evn.qp_new(), _REPU_VBLANKp_new_evn);

  /* p27.NUKO*/ wire _NUKO_WX_MATCHp = NUKO_WX_MATCHp(pix_count, reg_wx, win_reg.REJO_WY_MATCH_LATCHp_evn.qp_old());

  reg_wy.tock(BUS_CPU_A, BUS_CPU_D, rstdbg.AVOR_SYS_RSTp(), TEDO_CPU_RDp, TAPU_CPU_WRp, BUS_CPU_D_out);
  reg_wx.tock(BUS_CPU_A, BUS_CPU_D, rstdbg.AVOR_SYS_RSTp(), TEDO_CPU_RDp, TAPU_CPU_WRp, BUS_CPU_D_out);

  //------------------------------------------------------------------------------------------------------------------------
  // Sprite scanner

  /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.dff17(pclk.ALET_xBxDxFxH(), lcd.BAGY_LINE_RSTn(XODO_VID_RSTp), sprite_scanner.BYBA_SCAN_DONE_Ap_evn.qp_old());
  /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap_evn.dff17(vclk.XUPY_ABxxEFxx(),     lcd.BAGY_LINE_RSTn(XODO_VID_RSTp), /*old*/ scan_counter.FETO_SCAN_DONEp());

  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.qp_new(), lcd.BALU_LINE_RSTp(XODO_VID_RSTp), sprite_scanner.BYBA_SCAN_DONE_Ap_evn.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(rstdbg.ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

  /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp_evn.dff17(vclk.XUPY_ABxxEFxx(), rstdbg.ABEZ_VID_RSTn(),  sprite_scanner.BESU_SCANNINGp_evn.qp_old());
  /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp_evn.nor_latch(lcd.CATU_LINE_P000p_a.qp_new(), ASEN_SCAN_DONE_TRIGp);

  //------------------------------------------------------------------------------------------------------------------------

  /*#p21.XYMU*/ ppu_reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp_new_any, AVAP_SCAN_DONE_TRIGp);
  /*#p21.XYMU*/ wire XYMU_RENDERINGp = ppu_reg.XYMU_RENDERINGn.qn_new();
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGp);

  //------------------------------------------------------------------------------------------------------------------------

  wire _VYPO_VCC = 1;
  /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp_evn.dff17(pclk.LAPE_AxCxExGx(), _VYPO_VCC, sprite_fetcher.SOBU_SFETCH_REQp_odd.qp_old());
  /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp_odd.dff17(pclk.TAVA_xBxDxFxH(), _VYPO_VCC, TEKY_SFETCH_REQp_old);

  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new_evn = nand3(reg_lcdc.WYMO_LCDC_WINENn_h.qn_new(), lcd.XAHY_LINE_RSTn(XODO_VID_RSTp), XAPO_VID_RSTn(XODO_VID_RSTp));
  /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp_evn.dff17(pclk.MEHE_AxCxExGx(), rstdbg.XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp_odd.qp_old());
  /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap_evn.nor_latch(win_reg.NUNU_WIN_MATCHp_evn.qp_new(), _XOFO_WIN_RSTp_new_evn);
  /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp_odd.dff17(pclk.ALET_xBxDxFxH(), rstdbg.XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap_evn.qp_new());

  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp_xxx.dff17(pclk.ALET_xBxDxFxH(), win_reg.NAFY_WIN_MODE_TRIGn(XYMU_RENDERINGp), LYRY_BFETCH_DONEp_old);

  /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp_xxx.dff17(pclk.MYVO_AxCxExGx(), win_reg.NAFY_WIN_MODE_TRIGn(XYMU_RENDERINGp), tile_fetcher.NYKA_FETCH_DONEp_xxx.qp_old());

  /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp_odd.dff17(pclk.ALET_xBxDxFxH(), XYMU_RENDERINGp, tile_fetcher.PORY_FETCH_DONEp_xxx.qp_old());
  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp_odd.nor_latch(tile_fetcher.PYGO_FETCH_DONEp_odd.qp_new(), LOBY_RENDERINGn);

  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp_evn = nor3(win_reg.PUKU_WIN_HITn_evn.qp_old(), tile_fetcher.PORY_FETCH_DONEp_xxx.qp_new(), rstdbg.PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn_evn = nor2(win_reg.RYDY_WIN_HITp_evn.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp());
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp_evn = nor3(win_reg.PUKU_WIN_HITn_evn.qp_new(), tile_fetcher.PORY_FETCH_DONEp_xxx.qp_new(), rstdbg.PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn_evn = nor2(win_reg.RYDY_WIN_HITp_evn.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp());

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(FEPO_STORE_MATCHp_old, WODU_HBLANKp_old, pclk.MYVO_AxCxExGx()); // FIXME old/new - but does it really matter here?
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(win_reg.SOCY_WIN_HITn(), tile_fetcher.POKY_PRELOAD_LATCHp_odd.qp_new(), VYBO_CLKPIPE_odd);

  //------------------------------------------------------------------------------------------------------------------------

  [
    this,
    XYMU_RENDERINGp,
    TYFA_CLKPIPE_odd
  ]() {
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);

    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n_h.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n_h.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n_h.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17(pclk.MOXE_AxCxExGx(), XYMU_RENDERINGp, fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_old());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_xBxDxFxH_clknew_odd, XYMU_RENDERINGp, _POHU_SCX_FINE_MATCHp_old);
    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp_new_odd = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new_evn = not1(XYMU_RENDERINGp);

    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(_PAHA_RENDERINGn_new_evn, _POVA_FINE_MATCH_TRIGp_new_odd);
  }();

  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  //------------------------------------------------------------------------------------------------------------------------
  // Pixel counter

  /* p21.TADY*/ wire TADY_LINE_RSTn = nor2(lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp), rstdbg.TOFU_VID_RSTp());
  pix_count.tock(TADY_LINE_RSTn, SACU_CLKPIPE_evn);

  //------------------------------------------------------------------------------------------------------------------------

  [this, XYMU_RENDERINGp, XODO_VID_RSTp]() {
    wire _VYPO_VCC = 1;
    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1_evn.dff17(pclk.LAPE_AxCxExGx(), _VYPO_VCC,               sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5_evn.dff17(pclk.LAPE_AxCxExGx(), XYMU_RENDERINGp, sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4_odd.dff17(pclk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, sprite_fetcher.TOBU_SFETCH_S1p_D2_odd.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2_odd.dff17(pclk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, sprite_fetcher.TULY_SFETCH_S1p_odd.qp_old());

    wire SECA_SFETCH_RSTn = sprite_fetcher.SECA_SFETCH_RSTn(XODO_VID_RSTp, lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp));

    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p_odd.RSTn(SECA_SFETCH_RSTn);
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p_odd.RSTn(SECA_SFETCH_RSTn);
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p_odd.RSTn(SECA_SFETCH_RSTn);

    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE_new = nand2(sprite_fetcher.TESE_SFETCH_S2p_odd.qp_new(), sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_new());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_clknew = nand2(pclk.LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE_new);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p_odd.dff17(_TOMA_SFETCH_xBxDxFxH_clknew,                SECA_SFETCH_RSTn, sprite_fetcher.TOXE_SFETCH_S0p_odd.qn_new());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p_odd.dff17(sprite_fetcher.TOXE_SFETCH_S0p_odd.qn_new(), SECA_SFETCH_RSTn, sprite_fetcher.TULY_SFETCH_S1p_odd.qn_new());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p_odd.dff17(sprite_fetcher.TULY_SFETCH_S1p_odd.qn_new(), SECA_SFETCH_RSTn, sprite_fetcher.TESE_SFETCH_S2p_odd.qn_new());
  }();

  //------------------------------------------------------------------------------------------------------------------------
  // Sprite store output

  wire WUTY_SFETCH_DONE_TRIGp = sprite_fetcher.WUTY_SFETCH_DONE_TRIGp();
  const auto& _pix_count = this->pix_count;
  /* p29.CEHA*/ wire _CEHA_SCANNINGp_new = not1(sprite_scanner.CENO_SCANNINGp_evn.qn_new());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_new = not1(_CEHA_SCANNINGp_new);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_new = and2(XYMU_RENDERINGp, _BYJO_SCANNINGn_new);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_new = and2(_AZEM_RENDERINGp_new, reg_lcdc.XYLO_LCDC_SPENn_h.qn_new());

  /* p28.ABAK*/ wire _ABAK_LINE_RSTp = lcd.ABAK_LINE_RSTp(XODO_VID_RSTp);

  sprite_store.reset_matched_store(_ABAK_LINE_RSTp, WUTY_SFETCH_DONE_TRIGp, old_sprite_flag);

  SpriteMatch sprite_match = sprite_store.get_match(_AROR_MATCH_ENp_new, _pix_count);

  /* p29.FEPO*/ wire FEPO_STORE_MATCHp_new_evn = sprite_match.FEPO_STORE_MATCHp();

  sprite_store.get_sprite(sprite_match);

  /*#p21.XENA*/ wire _XENA_STORE_MATCHn_new = not1(FEPO_STORE_MATCHp_new_evn);
  /*#p21.WODU*/ wire WODU_HBLANKp_new = and2(_XENA_STORE_MATCHn_new, pix_count.XANO_PX167p()); // WODU goes high on odd, cleared on H

  [this, XODO_VID_RSTp, XYMU_RENDERINGp](){
    wire SECA_SFETCH_RSTn = sprite_fetcher.SECA_SFETCH_RSTn(rstdbg.ROSY_VID_RSTp(), lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp));
    /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn_new = nor2(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGp)); // def nor
    /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp_xxx.nand_latch(SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn_new);
  }();

  [this, TYFA_CLKPIPE_odd, _NUKO_WX_MATCHp](){
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /* p27.ROCO*/ wire _ROCO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);
    /* p27.PYCO*/ win_reg.PYCO_WIN_MATCHp_odd.dff17(_ROCO_CLKPIPE_xBxDxFxH_clknew_odd, rstdbg.XAPO_VID_RSTn(), _NUKO_WX_MATCHp);
  }();

  //------------------------------------------------------------------------------------------------------------------------

  win_reg.tock(XODO_VID_RSTp, pclk.ALET_xBxDxFxH());

  [this, TYFA_CLKPIPE_odd, XYMU_RENDERINGp, _NUKO_WX_MATCHp](){
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(_NUKO_WX_MATCHp, fine_scroll.ROZE_FINE_COUNT_7n_old());
    /* p27.RYFA*/ win_reg.RYFA_WIN_FETCHn_A_evn.dff17(_SEGU_CLKPIPE_AxCxExGx_clknew_evn, XYMU_RENDERINGp, _PANY_WIN_FETCHn_old);
  }();

  [this, XYMU_RENDERINGp](){
    /* p27.RENE*/ win_reg.RENE_WIN_FETCHn_B_odd.dff17(pclk.ALET_xBxDxFxH(), XYMU_RENDERINGp, win_reg.RYFA_WIN_FETCHn_A_evn.qp_old());
  }();

  /* p27.TEVO*/ bool TEVO_FETCH_TRIGp;
  [this, XYMU_RENDERINGp, &TEVO_FETCH_TRIGp](){
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new_evn = not1(win_reg.RYDY_WIN_HITp_evn.qp_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_new_xxx = nand2(_SYLO_WIN_HITn_new_evn, win_reg.SOVY_WIN_HITp.qp_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_new = not1(_TUXY_WIN_FIRST_TILEne_new_xxx);
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_new = nor2(win_reg.RYFA_WIN_FETCHn_A_evn.qn_new(), win_reg.RENE_WIN_FETCHn_B_odd.qp_new());
    /* p27.TEVO*/ TEVO_FETCH_TRIGp = or3(_SEKO_WIN_FETCH_TRIGp_new, _SUZU_WIN_FIRST_TILEne_new, tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGp)); // Schematic wrong, this is OR
  }();

  /* p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(AVAP_SCAN_DONE_TRIGp, win_reg.MOSU_WIN_MODE_TRIGp(), TEVO_FETCH_TRIGp);

  [
    this,
    XYMU_RENDERINGp,
    TYFA_CLKPIPE_odd,
    TEVO_FETCH_TRIGp
  ]() {
    // Fine match counter. Registers are only read as old, so this can go down as far in the list as needed.

    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new_evn = not1(XYMU_RENDERINGp);
    /*#p27.PASO*/ wire _PASO_FINE_RST_new = nor2(_PAHA_RENDERINGn_new_evn, TEVO_FETCH_TRIGp);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.RSTn(_PASO_FINE_RST_new);
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.RSTn(_PASO_FINE_RST_new);
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.RSTn(_PASO_FINE_RST_new);

    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_new_evn = nand3(fine_scroll.RUBU_FINE_CNT2.qp_new(), fine_scroll.ROGA_FINE_CNT1.qp_new(), fine_scroll.RYKU_FINE_CNT0.qp_new());
    /*#p27.PECU*/ wire _PECU_FINE_CLK_AxCxExGx_clknew_evn = nand2(_ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n_new_evn);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK_AxCxExGx_clknew_evn,      _PASO_FINE_RST_new, fine_scroll.RYKU_FINE_CNT0.qn_new());
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17(fine_scroll.RYKU_FINE_CNT0.qn_new(), _PASO_FINE_RST_new, fine_scroll.ROGA_FINE_CNT1.qn_new());
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17(fine_scroll.ROGA_FINE_CNT1.qn_new(), _PASO_FINE_RST_new, fine_scroll.RUBU_FINE_CNT2.qn_new());
  }();

  //------------------------------------------------------------------------------------------------------------------------

  /*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp_evn.qp_new());

  oam_temp_a.latch_to_temp(
    BUS_CPU_A,
    ACYL_SCANNINGp,
    pclk.UVYT_ABCDxxxx(),
    vclk.XYSO_xBCDxFGH(),
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    sprite_fetcher.XUJY_OAM_CLKENp(XYMU_RENDERINGp),
    oam_latch_a);

  oam_temp_b.latch_to_temp(
    BUS_CPU_A,
    ACYL_SCANNINGp,
    pclk.UVYT_ABCDxxxx(),
    vclk.XYSO_xBCDxFGH(),
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    sprite_fetcher.XUJY_OAM_CLKENp(XYMU_RENDERINGp),
    oam_latch_b);

  //------------------------------------------------------------------------------------------------------------------------

  // Sprite hit line checker

  SpriteDeltaY delta = sprite_delta_y(oam_temp_a, reg_ly);

  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp = delta.GESE_SCAN_MATCH_Yp(reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new());
  /* p29.CARE*/ wire _CARE_COUNT_CLKn = and3(vclk.XOCE_xBCxxFGx(), sprite_scanner.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
  /* p29.DYTY*/ wire _DYTY_COUNT_CLKp = not1(_CARE_COUNT_CLKn);

  sprite_counter.update_count(rstdbg.XAPO_VID_RSTn(), pclk.ZEME_AxCxExGx(), lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp), _DYTY_COUNT_CLKp);

  SpriteStoreFlag store_flag = sprite_counter.get_store_flag(_DYTY_COUNT_CLKp);

  sprite_store.store_sprite_index(store_flag);
  sprite_store.store_sprite_line (store_flag);
  sprite_store.store_sprite_x    (store_flag, oam_temp_b);

  //------------------------------------------------------------------------------------------------------------------------

  sprite_store.get_sprite_index(vclk.WUDA_xxCDxxGH(), XYMU_RENDERINGp, sprite_scanner.CENO_SCANNINGp_evn.qn_new(), oam_bus.BUS_OAM_An);
  sprite_store.get_sprite_line(FEPO_STORE_MATCHp_new_evn, reg_ly, oam_temp_a);

  reg_stat.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    lcd.PARU_VBLANKp(),
    reg_lyc,
    BUS_CPU_D_out);

  tile_fetcher.tock2(pclk.ALET_xBxDxFxH(), NYXU_BFETCH_RSTn);

  [
    this,
    XYMU_RENDERINGp,
    NYXU_BFETCH_RSTn,
    MOCE_BFETCH_DONEn_old
  ](){
    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
    /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp_evn.dff17(pclk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
    /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn_new_evn = and2(tile_fetcher.LOVY_FETCH_DONEp_evn.qn_new(), XYMU_RENDERINGp);
    /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp_xxx.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn_new_evn);
    /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1_odd.dff17(pclk.ALET_xBxDxFxH(), XYMU_RENDERINGp, tile_fetcher.LAXU_BFETCH_S0p_evn.qp_new());
  }();

  wire ABUZ_xxCDEFGH = [this](){
    /*#p01.AJAX*/ wire _AJAX_xxxxEFGH = not1(pclk.ATYP_ABCDxxxx());
    /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(pclk.AROV_xxCDEFxx(), _AJAX_xxxxEFGH, PIN_CPU_EXT_BUSp.qp_new());
    /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(rstdbg.UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire _ABUZ_xxCDEFGH = not1(_AWOD_ABxxxxxx);
    return _ABUZ_xxCDEFGH;
  }();

  /*#p35.REMY*/ bool REMY_LD0n_new;
  /*#p35.RAVO*/ bool RAVO_LD1n_new;

  tock_pix_pipe(
    BUS_CPU_A,
    BUS_CPU_D,
    XODO_VID_RSTp,

    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    XYMU_RENDERINGp,
    SACU_CLKPIPE_evn,
    NYXU_BFETCH_RSTn,

    REMY_LD0n_new,
    RAVO_LD1n_new
  );

  tock_lcd(
    vclk.TALU_xxCDEFxx(),
    TYFA_CLKPIPE_odd,
    SACU_CLKPIPE_evn,
    XONA_LCDC_LCDENp_h_new,
    XYMU_RENDERINGp,
    AVAP_SCAN_DONE_TRIGp,
    WEGO_HBLANKp_new_any,
    reg_lx.PURE_LINE_ENDn_new_evn(),
    REMY_LD0n_new,
    RAVO_LD1n_new
  );

  joypad.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    pclk.BOGA_Axxxxxxx(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    BUS_CPU_D_out
  );

  serial.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.ALUR_SYS_RSTn(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    div.TAMA_DIV05p_evn.qp_new(),
    BUS_CPU_D_out
  );

  interrupts.PIN_CPU_ACK_VBLANK.setp(wire(cpu.int_ack & INT_VBLANK_MASK));
  interrupts.PIN_CPU_ACK_STAT  .setp(wire(cpu.int_ack & INT_STAT_MASK));
  interrupts.PIN_CPU_ACK_TIMER .setp(wire(cpu.int_ack & INT_TIMER_MASK));
  interrupts.PIN_CPU_ACK_SERIAL.setp(wire(cpu.int_ack & INT_SERIAL_MASK));
  interrupts.PIN_CPU_ACK_JOYPAD.setp(wire(cpu.int_ack & INT_JOYPAD_MASK));

  interrupts.tock(
    rstdbg,
    joypad,
    reg_stat,
    reg_lyc,
    serial,

    BUS_CPU_A,
    BUS_CPU_D,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    lcd.PARU_VBLANKp(),
    reg_lx.PURE_LINE_ENDn_new_evn(),
    timer.MOBA_TIMER_OVERFLOWp.qp_new(),
    WODU_HBLANKp_new,
    BUS_CPU_D_out);


  if (DELTA_DE) {
    int_vblank_halt = interrupts.LOPE_FF0F_D0p.qp_new();
    int_stat_halt   = interrupts.LALU_FF0F_D1p.qp_new();
    int_serial_halt = interrupts.UBUL_FF0F_D3p.qp_new();
    int_joypad_halt = interrupts.ULAK_FF0F_D4p.qp_new();
  }

  if (DELTA_GH) {
    // this one latches funny, some hardware bug
    int_timer_halt = interrupts.NYBO_FF0F_D2p.qp_new();

    int_vblank = interrupts.LOPE_FF0F_D0p.qp_new();
    int_stat   = interrupts.LALU_FF0F_D1p.qp_new();
    int_timer  = interrupts.NYBO_FF0F_D2p.qp_new();
    int_serial = interrupts.UBUL_FF0F_D3p.qp_new();
    int_joypad = interrupts.ULAK_FF0F_D4p.qp_new();
  }

  if (DELTA_HA) {
    imask_latch = pack_u8p(5, &interrupts.IE_D0);
  }

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn_ext = and2(PIN_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn_ext(BUS_CPU_A));
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp_ext = nor2(SYRO_FE00_FFFF_ext(BUS_CPU_A), TEXO_ADDR_VRAMn_ext);
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp_ext = and2(TEFA_ADDR_VRAMp_ext, BUS_CPU_A[15]);

  tock_ext(
    BUS_CPU_A,
    BUS_CPU_D,
    ABUZ_xxCDEFGH,
    TEDO_CPU_RDp,
    APOV_CPU_WRp,
    TEXO_ADDR_VRAMn_ext,
    bootrom.TUTU_READ_BOOTROMp_new(BUS_CPU_A)
  );

  tock_vram(
    BUS_CPU_A,
    BUS_CPU_D,

    rstdbg.AVOR_SYS_RSTp(),
    XODO_VID_RSTp,

    pclk.ATAL_xBxDxFxH(),
    ABUZ_xxCDEFGH,

    CATY_LATCH_EXTp_ext,
    SOSE_ADDR_VRAMp_ext,

    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    APOV_CPU_WRp,

    rstdbg.TUTO_DBG_VRAMp(rstdbg.UNOR_MODE_DBG2p()),

    lcd.ATEJ_LINE_RSTp(XODO_VID_RSTp),
    TEVO_FETCH_TRIGp,
    NYXU_BFETCH_RSTn,
    lcd.PARU_VBLANKp(),
    WYMO_LCDC_WINENn_new,
    XYMU_RENDERINGp
  );

  scan_counter.tock(vclk.XUPY_ABxxEFxx(), lcd.ANOM_LINE_RSTn(XODO_VID_RSTp));

  tock_oam(
    BUS_CPU_A,
    BUS_CPU_D,
    rstdbg.AVOR_SYS_RSTp(),
    pclk.ATAL_xBxDxFxH(),
    pclk.UVYT_ABCDxxxx(),
    vclk.XOCE_xBCxxFGx(),
    vclk.XYSO_xBCDxFGH(),
    TAPU_CPU_WRp,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp_ext,
    XYMU_RENDERINGp,
    ACYL_SCANNINGp
  );

  tock_zram(
    BUS_CPU_A,
    BUS_CPU_D,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  );

  //----------------------------------------
  // Save signals for next phase.

  oam_clk_old = !oam_bus.PIN_OAM_CLKn.qp_new();
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_pix_pipe(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire XODO_VID_RSTp_new_h,

  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  wire XYMU_RENDERINGp,
  wire SACU_CLKPIPE_AxCxExGx_clknew_evn,
  wire NYXU_BFETCH_RSTn_new_xxx,

  wire &REMY_LD0n_new,
  wire &RAVO_LD1n_new)
{

  //----------------------------------------
  // Tile temp storage

  [
    this,
    XYMU_RENDERINGp
  ](){
    wire _VYPO_VCC = 1;

    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);

    /* p27.LAXU*/ wire LAXU_BFETCH_S0p_evn_new = tile_fetcher.LAXU_BFETCH_S0p_evn.qp_new();
    /* p27.MESU*/ wire MESU_BFETCH_S1p_evn_new = tile_fetcher.MESU_BFETCH_S1p_evn.qp_new();
    /* p27.NYVA*/ wire NYVA_BFETCH_S2p_evn_new = tile_fetcher.NYVA_BFETCH_S2p_evn.qp_new();

    /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n_new_evn = not1(LAXU_BFETCH_S0p_evn_new);
    /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_new_evn = not1(MESU_BFETCH_S1p_evn_new);
    /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_new_evn = not1(NYVA_BFETCH_S2p_evn_new);

    /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp_new = nor3(_LOBY_RENDERINGn_new_xxx, _LAXE_BFETCH_S0n_new_evn, tile_fetcher.LYZU_BFETCH_S0p_D1_odd.qp_new()); // MYSO fires on fetch phase 2, 6, 10

    /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn_new = nand3(_MYSO_STORE_VRAM_DATA_TRIGp_new, MESU_BFETCH_S1p_evn_new, _NOFU_BFETCH_S2n_new_evn); // NYDY on fetch phase 6
    /*#p32.METE*/ wire _METE_LATCH_TILE_DAp_new = not1(_NYDY_LATCH_TILE_DAn_new);
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn_new = not1(_METE_LATCH_TILE_DAp_new);

    /* p32.LEGU*/ tile_temp_a.LEGU_TILE_DA0n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[0].qp_old());
    /* p32.NUDU*/ tile_temp_a.NUDU_TILE_DA1n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[1].qp_old());
    /* p32.MUKU*/ tile_temp_a.MUKU_TILE_DA2n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[2].qp_old());
    /* p32.LUZO*/ tile_temp_a.LUZO_TILE_DA3n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[3].qp_old());
    /* p32.MEGU*/ tile_temp_a.MEGU_TILE_DA4n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[4].qp_old());
    /* p32.MYJY*/ tile_temp_a.MYJY_TILE_DA5n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[5].qp_old());
    /* p32.NASA*/ tile_temp_a.NASA_TILE_DA6n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[6].qp_old());
    /* p32.NEFO*/ tile_temp_a.NEFO_TILE_DA7n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, vram_bus.BUS_VRAM_Dp[7].qp_old());

    /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp_new = and2(_MYSO_STORE_VRAM_DATA_TRIGp_new, _NAKO_BFETCH_S1n_new_evn); // MOFU fires on fetch phase 2 and 10
    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn_new = not1(_MOFU_LATCH_TILE_DBp_new);
    /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp_new = not1(_LESO_LATCH_TILE_DBn_new); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn_new = not1(_LUVE_LATCH_TILE_DBp_new);

    /* p32.RAWU*/ tile_temp_b.RAWU_TILE_DB0p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[0].qp_old()); // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /* p32.POZO*/ tile_temp_b.POZO_TILE_DB1p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[1].qp_old());
    /* p32.PYZO*/ tile_temp_b.PYZO_TILE_DB2p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[2].qp_old());
    /* p32.POXA*/ tile_temp_b.POXA_TILE_DB3p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[3].qp_old());
    /* p32.PULO*/ tile_temp_b.PULO_TILE_DB4p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[4].qp_old());
    /* p32.POJU*/ tile_temp_b.POJU_TILE_DB5p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[5].qp_old());
    /* p32.POWY*/ tile_temp_b.POWY_TILE_DB6p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[6].qp_old());
    /* p32.PYJU*/ tile_temp_b.PYJU_TILE_DB7p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[7].qp_old());
  }();

  //----------------------------------------
  // Sprite temp storage

  [
    this,
    XYMU_RENDERINGp

  ](){
    // Sprite X flipper
    /* p29.TEPA*/ wire _TEPA_RENDERINGp_old = not1(ppu_reg.XYMU_RENDERINGn.qn_old());
    /* p29.SAKY*/ wire _SAKY_SFETCH_old = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_old(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_old());

    /* p29.TYSO*/ wire _TYSO_SPRITE_READn_old = or2(_SAKY_SFETCH_old, _TEPA_RENDERINGp_old); // def or
    /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_old = not1(_TYSO_SPRITE_READn_old);
    /*#p29.XONO*/ wire _XONO_FLIP_X_old = and2(oam_temp_b.BAXO_OAM_DB5p_evn.qp_old(), _TEXY_SPR_READ_VRAMp_old);
    /* p33.PUTE*/ wire _PUTE_FLIP0p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[7].qp_old(), vram_bus.BUS_VRAM_Dp[0].qp_old());
    /* p33.PELO*/ wire _PELO_FLIP1p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[6].qp_old(), vram_bus.BUS_VRAM_Dp[1].qp_old());
    /* p33.PONO*/ wire _PONO_FLIP2p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[5].qp_old(), vram_bus.BUS_VRAM_Dp[2].qp_old());
    /* p33.POBE*/ wire _POBE_FLIP3p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[4].qp_old(), vram_bus.BUS_VRAM_Dp[3].qp_old());
    /* p33.PACY*/ wire _PACY_FLIP4p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[3].qp_old(), vram_bus.BUS_VRAM_Dp[4].qp_old());
    /* p33.PUGU*/ wire _PUGU_FLIP5p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[2].qp_old(), vram_bus.BUS_VRAM_Dp[5].qp_old());
    /* p33.PAWE*/ wire _PAWE_FLIP6p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[1].qp_old(), vram_bus.BUS_VRAM_Dp[6].qp_old());
    /* p33.PULY*/ wire _PULY_FLIP7p_old = mux2p(_XONO_FLIP_X_old, vram_bus.BUS_VRAM_Dp[0].qp_old(), vram_bus.BUS_VRAM_Dp[7].qp_old());


    /*#p29.TOPU*/ wire _TOPU_CLKp_new = and2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.SYCU_SFETCH_S0pe(XYMU_RENDERINGp));
    /*#p29.VYWA*/ wire _VYWA_CLKn_new = not1(_TOPU_CLKp_new);
    /*#p29.WENY*/ wire _WENY_CLKp_new = not1(_VYWA_CLKn_new);
    /*#p29.XADO*/ wire _XADO_CLKn_new = not1(_WENY_CLKp_new);
    /* p33.REWO*/ sprite_temp_a.REWO_SPRITE_DA0n.dff8n(_XADO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.PEBA*/ sprite_temp_a.PEBA_SPRITE_DA1n.dff8n(_XADO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MOFO*/ sprite_temp_a.MOFO_SPRITE_DA2n.dff8n(_XADO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.PUDU*/ sprite_temp_a.PUDU_SPRITE_DA3n.dff8n(_XADO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SAJA*/ sprite_temp_a.SAJA_SPRITE_DA4n.dff8n(_XADO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUNY*/ sprite_temp_a.SUNY_SPRITE_DA5n.dff8n(_XADO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.SEMO*/ sprite_temp_a.SEMO_SPRITE_DA6n.dff8n(_XADO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.SEGA*/ sprite_temp_a.SEGA_SPRITE_DA7n.dff8n(_XADO_CLKn_new, _PULY_FLIP7p_old);

    /*#p29.RACA*/ wire _RACA_CLKp_new = and2(sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new(), sprite_fetcher.SYCU_SFETCH_S0pe(XYMU_RENDERINGp));
    /*#p29.PEBY*/ wire _PEBY_CLKn_new = not1(_RACA_CLKp_new);
    /*#p29.NYBE*/ wire _NYBE_CLKp_new = not1(_PEBY_CLKn_new);
    /*#p29.PUCO*/ wire _PUCO_CLKn_new = not1(_NYBE_CLKp_new);
    /* p33.PEFO*/ sprite_temp_b.PEFO_SPRITE_DB0n.dff8n(_PUCO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.ROKA*/ sprite_temp_b.ROKA_SPRITE_DB1n.dff8n(_PUCO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MYTU*/ sprite_temp_b.MYTU_SPRITE_DB2n.dff8n(_PUCO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.RAMU*/ sprite_temp_b.RAMU_SPRITE_DB3n.dff8n(_PUCO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SELE*/ sprite_temp_b.SELE_SPRITE_DB4n.dff8n(_PUCO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUTO*/ sprite_temp_b.SUTO_SPRITE_DB5n.dff8n(_PUCO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.RAMA*/ sprite_temp_b.RAMA_SPRITE_DB6n.dff8n(_PUCO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.RYDU*/ sprite_temp_b.RYDU_SPRITE_DB7n.dff8n(_PUCO_CLKn_new, _PULY_FLIP7p_old);
  }();

  //----------------------------------------
  // Pixel pipes

  [
    this,
    SACU_CLKPIPE_AxCxExGx_clknew_evn,
    NYXU_BFETCH_RSTn_new_xxx
  ]() {
    wire _VYPO_VCC = 1;
    wire GND = 0;

    // Sprite temp loads  on AB/CD/EF/GH
    // Tile   temp loads  on AB/CD/EF/GH
    // Pipes       shifts on HA/BC/DE/FG
    // WUTY        fires  on HA/BC/DE/FG
    // NYXU        fires  on HA/BC/DE/FG

    // The pipes aren't shifting when we load a sprite, so it's OK to use the old value from the sprite pipes to construct the mask.
    /* p29.XEFY*/ wire _XEPY_SPRITE_DONEn_new  = not1(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp());
    /* p34.MEFU*/ wire _MEFU_SPRITE_MASK0n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.NYLU_SPR_PIPE_B0_evn.qp_old(), pix_pipes.NURO_SPR_PIPE_A0_evn.qp_old());
    /* p34.MEVE*/ wire _MEVE_SPRITE_MASK1n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.PEFU_SPR_PIPE_B1_evn.qp_old(), pix_pipes.MASO_SPR_PIPE_A1_evn.qp_old());
    /* p34.MYZO*/ wire _MYZO_SPRITE_MASK2n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.NATY_SPR_PIPE_B2_evn.qp_old(), pix_pipes.LEFE_SPR_PIPE_A2_evn.qp_old());
    /* p34.RUDA*/ wire _RUDA_SPRITE_MASK3n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.PYJO_SPR_PIPE_B3_evn.qp_old(), pix_pipes.LESU_SPR_PIPE_A3_evn.qp_old());
    /* p34.VOTO*/ wire _VOTO_SPRITE_MASK4n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.VARE_SPR_PIPE_B4_evn.qp_old(), pix_pipes.WYHO_SPR_PIPE_A4_evn.qp_old());
    /* p34.VYSA*/ wire _VYSA_SPRITE_MASK5n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.WEBA_SPR_PIPE_B5_evn.qp_old(), pix_pipes.WORA_SPR_PIPE_A5_evn.qp_old());
    /* p34.TORY*/ wire _TORY_SPRITE_MASK6n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.VANU_SPR_PIPE_B6_evn.qp_old(), pix_pipes.VAFO_SPR_PIPE_A6_evn.qp_old());
    /* p34.WOPE*/ wire _WOPE_SPRITE_MASK7n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipes.VUPY_SPR_PIPE_B7_evn.qp_old(), pix_pipes.WUFY_SPR_PIPE_A7_evn.qp_old());

    /* p34.LESY*/ wire _LESY_SPRITE_MASK0p_new = not1(_MEFU_SPRITE_MASK0n_new);
    /* p34.LOTA*/ wire _LOTA_SPRITE_MASK1p_new = not1(_MEVE_SPRITE_MASK1n_new);
    /* p34.LYKU*/ wire _LYKU_SPRITE_MASK2p_new = not1(_MYZO_SPRITE_MASK2n_new);
    /* p34.ROBY*/ wire _ROBY_SPRITE_MASK3p_new = not1(_RUDA_SPRITE_MASK3n_new);
    /* p34.TYTA*/ wire _TYTA_SPRITE_MASK4p_new = not1(_VOTO_SPRITE_MASK4n_new);
    /* p34.TYCO*/ wire _TYCO_SPRITE_MASK5p_new = not1(_VYSA_SPRITE_MASK5n_new);
    /* p34.SOKA*/ wire _SOKA_SPRITE_MASK6p_new = not1(_TORY_SPRITE_MASK6n_new);
    /* p34.XOVU*/ wire _XOVU_SPRITE_MASK7p_new = not1(_WOPE_SPRITE_MASK7n_new);


    // Reload all the pipes using set/rst
    {
      /* p32.LOZE*/ wire _LOZE_PIPE_A_LOADp_new = not1(NYXU_BFETCH_RSTn_new_xxx);
      /* p32.LAKY*/ wire _BG_PIPE_A_SET0_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.LEGU_TILE_DA0n_odd.qn_new());
      /* p32.NYXO*/ wire _BG_PIPE_A_SET1_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.NUDU_TILE_DA1n_odd.qn_new());
      /* p32.LOTO*/ wire _BG_PIPE_A_SET2_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.MUKU_TILE_DA2n_odd.qn_new());
      /* p32.LYDU*/ wire _BG_PIPE_A_SET3_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.LUZO_TILE_DA3n_odd.qn_new());
      /* p32.MYVY*/ wire _BG_PIPE_A_SET4_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.MEGU_TILE_DA4n_odd.qn_new());
      /* p32.LODO*/ wire _BG_PIPE_A_SET5_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.MYJY_TILE_DA5n_odd.qn_new());
      /* p32.NUTE*/ wire _BG_PIPE_A_SET6_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.NASA_TILE_DA6n_odd.qn_new());
      /* p32.NAJA*/ wire _BG_PIPE_A_SET7_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_temp_a.NEFO_TILE_DA7n_odd.qn_new());

      /* p32.LUHE*/ wire _BG_PIX_A0n_new = not1(tile_temp_a.LEGU_TILE_DA0n_odd.qn_new());
      /* p32.NOLY*/ wire _BG_PIX_A1n_new = not1(tile_temp_a.NUDU_TILE_DA1n_odd.qn_new());
      /* p32.LEKE*/ wire _BG_PIX_A2n_new = not1(tile_temp_a.MUKU_TILE_DA2n_odd.qn_new());
      /* p32.LOMY*/ wire _BG_PIX_A3n_new = not1(tile_temp_a.LUZO_TILE_DA3n_odd.qn_new());
      /* p32.LALA*/ wire _BG_PIX_A4n_new = not1(tile_temp_a.MEGU_TILE_DA4n_odd.qn_new());
      /* p32.LOXA*/ wire _BG_PIX_A5n_new = not1(tile_temp_a.MYJY_TILE_DA5n_odd.qn_new());
      /* p32.NEZE*/ wire _BG_PIX_A6n_new = not1(tile_temp_a.NASA_TILE_DA6n_odd.qn_new());
      /* p32.NOBO*/ wire _BG_PIX_A7n_new = not1(tile_temp_a.NEFO_TILE_DA7n_odd.qn_new());

      /* p32.LOTY*/ wire _BG_PIPE_A_RST0_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A0n_new);
      /* p32.NEXA*/ wire _BG_PIPE_A_RST1_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A1n_new);
      /* p32.LUTU*/ wire _BG_PIPE_A_RST2_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A2n_new);
      /* p32.LUJA*/ wire _BG_PIPE_A_RST3_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A3n_new);
      /* p32.MOSY*/ wire _BG_PIPE_A_RST4_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A4n_new);
      /* p32.LERU*/ wire _BG_PIPE_A_RST5_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A5n_new);
      /* p32.NYHA*/ wire _BG_PIPE_A_RST6_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A6n_new);
      /* p32.NADY*/ wire _BG_PIPE_A_RST7_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A7n_new);

      /* p32.PYBO*/ pix_pipes.PYBO_BGW_PIPE_A7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET7_new, _BG_PIPE_A_RST7_new, pix_pipes.NEDA_BGW_PIPE_A6_evn.qp_old());
      /* p32.NEDA*/ pix_pipes.NEDA_BGW_PIPE_A6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET6_new, _BG_PIPE_A_RST6_new, pix_pipes.MODU_BGW_PIPE_A5_evn.qp_old());
      /* p32.MODU*/ pix_pipes.MODU_BGW_PIPE_A5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET5_new, _BG_PIPE_A_RST5_new, pix_pipes.NEPO_BGW_PIPE_A4_evn.qp_old());
      /* p32.NEPO*/ pix_pipes.NEPO_BGW_PIPE_A4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET4_new, _BG_PIPE_A_RST4_new, pix_pipes.MACU_BGW_PIPE_A3_evn.qp_old());
      /* p32.MACU*/ pix_pipes.MACU_BGW_PIPE_A3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET3_new, _BG_PIPE_A_RST3_new, pix_pipes.MOJU_BGW_PIPE_A2_evn.qp_old());
      /* p32.MOJU*/ pix_pipes.MOJU_BGW_PIPE_A2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET2_new, _BG_PIPE_A_RST2_new, pix_pipes.NOZO_BGW_PIPE_A1_evn.qp_old());
      /* p32.NOZO*/ pix_pipes.NOZO_BGW_PIPE_A1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET1_new, _BG_PIPE_A_RST1_new, pix_pipes.MYDE_BGW_PIPE_A0_evn.qp_old());
      /* p32.MYDE*/ pix_pipes.MYDE_BGW_PIPE_A0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_A_SET0_new, _BG_PIPE_A_RST0_new, GND);
    }

    {
      /* p32.LUXA*/ wire _LUXA_PIPE_B_LOADp_new = not1(NYXU_BFETCH_RSTn_new_xxx);
      /* p32.TUXE*/ wire _BG_PIPE_B_SET0_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.RAWU_TILE_DB0p_odd.qp_new());
      /* p32.SOLY*/ wire _BG_PIPE_B_SET1_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.POZO_TILE_DB1p_odd.qp_new());
      /* p32.RUCE*/ wire _BG_PIPE_B_SET2_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.PYZO_TILE_DB2p_odd.qp_new());
      /* p32.RYJA*/ wire _BG_PIPE_B_SET3_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.POXA_TILE_DB3p_odd.qp_new());
      /* p32.RUTO*/ wire _BG_PIPE_B_SET4_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.PULO_TILE_DB4p_odd.qp_new());
      /* p32.RAJA*/ wire _BG_PIPE_B_SET5_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.POJU_TILE_DB5p_odd.qp_new());
      /* p32.RAJO*/ wire _BG_PIPE_B_SET6_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.POWY_TILE_DB6p_odd.qp_new());
      /* p32.RAGA*/ wire _BG_PIPE_B_SET7_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_temp_b.PYJU_TILE_DB7p_odd.qp_new());

      /* p32.TOSA*/ wire _BG_PIX_B0n_new = not1(tile_temp_b.RAWU_TILE_DB0p_odd.qp_new());
      /* p32.RUCO*/ wire _BG_PIX_B1n_new = not1(tile_temp_b.POZO_TILE_DB1p_odd.qp_new());
      /* p32.TYCE*/ wire _BG_PIX_B2n_new = not1(tile_temp_b.PYZO_TILE_DB2p_odd.qp_new());
      /* p32.REVY*/ wire _BG_PIX_B3n_new = not1(tile_temp_b.POXA_TILE_DB3p_odd.qp_new());
      /* p32.RYGA*/ wire _BG_PIX_B4n_new = not1(tile_temp_b.PULO_TILE_DB4p_odd.qp_new());
      /* p32.RYLE*/ wire _BG_PIX_B5n_new = not1(tile_temp_b.POJU_TILE_DB5p_odd.qp_new());
      /* p32.RAPU*/ wire _BG_PIX_B6n_new = not1(tile_temp_b.POWY_TILE_DB6p_odd.qp_new());
      /* p32.SOJA*/ wire _BG_PIX_B7n_new = not1(tile_temp_b.PYJU_TILE_DB7p_odd.qp_new());

      /* p32.SEJA*/ wire _BG_PIPE_B_RST0_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B0n_new);
      /* p32.SENO*/ wire _BG_PIPE_B_RST1_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B1n_new);
      /* p32.SURE*/ wire _BG_PIPE_B_RST2_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B2n_new);
      /* p32.SEBO*/ wire _BG_PIPE_B_RST3_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B3n_new);
      /* p32.SUCA*/ wire _BG_PIPE_B_RST4_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B4n_new);
      /* p32.SYWE*/ wire _BG_PIPE_B_RST5_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B5n_new);
      /* p32.SUPU*/ wire _BG_PIPE_B_RST6_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B6n_new);
      /* p32.RYJY*/ wire _BG_PIPE_B_RST7_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B7n_new);

      /* p32.SOHU*/ pix_pipes.SOHU_BGW_PIPE_B7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET7_new, _BG_PIPE_B_RST7_new, pix_pipes.RALU_BGW_PIPE_B6_evn.qp_old());
      /* p32.RALU*/ pix_pipes.RALU_BGW_PIPE_B6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET6_new, _BG_PIPE_B_RST6_new, pix_pipes.SETU_BGW_PIPE_B5_evn.qp_old());
      /* p32.SETU*/ pix_pipes.SETU_BGW_PIPE_B5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET5_new, _BG_PIPE_B_RST5_new, pix_pipes.SOBO_BGW_PIPE_B4_evn.qp_old());
      /* p32.SOBO*/ pix_pipes.SOBO_BGW_PIPE_B4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET4_new, _BG_PIPE_B_RST4_new, pix_pipes.RYSA_BGW_PIPE_B3_evn.qp_old());
      /* p32.RYSA*/ pix_pipes.RYSA_BGW_PIPE_B3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET3_new, _BG_PIPE_B_RST3_new, pix_pipes.SADY_BGW_PIPE_B2_evn.qp_old());
      /* p32.SADY*/ pix_pipes.SADY_BGW_PIPE_B2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET2_new, _BG_PIPE_B_RST2_new, pix_pipes.TACA_BGW_PIPE_B1_evn.qp_old());
      /* p32.TACA*/ pix_pipes.TACA_BGW_PIPE_B1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET1_new, _BG_PIPE_B_RST1_new, pix_pipes.TOMY_BGW_PIPE_B0_evn.qp_old());
      /* p32.TOMY*/ pix_pipes.TOMY_BGW_PIPE_B0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _BG_PIPE_B_SET0_new, _BG_PIPE_B_RST0_new, GND);
    }

    // Sprite pipe A
    {
      /* p33.PABE*/ wire _PABE_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, sprite_temp_a.REWO_SPRITE_DA0n.qn_new());
      /* p33.MYTO*/ wire _MYTO_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, sprite_temp_a.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LELA*/ wire _LELA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, sprite_temp_a.MOFO_SPRITE_DA2n.qn_new());
      /* p33.MAME*/ wire _MAME_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, sprite_temp_a.PUDU_SPRITE_DA3n.qn_new());
      /* p33.VEXU*/ wire _VEXU_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, sprite_temp_a.SAJA_SPRITE_DA4n.qn_new());
      /* p33.VABY*/ wire _VABY_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, sprite_temp_a.SUNY_SPRITE_DA5n.qn_new());
      /* p33.TUXA*/ wire _TUXA_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, sprite_temp_a.SEMO_SPRITE_DA6n.qn_new());
      /* p33.VUNE*/ wire _VUNE_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, sprite_temp_a.SEGA_SPRITE_DA7n.qn_new());

      /* p33.RATA*/ wire _RATA_SPR_PIX_DA0n_new = not1(sprite_temp_a.REWO_SPRITE_DA0n.qn_new());
      /* p33.NUCA*/ wire _NUCA_SPR_PIX_DA1n_new = not1(sprite_temp_a.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LASE*/ wire _LASE_SPR_PIX_DA2n_new = not1(sprite_temp_a.MOFO_SPRITE_DA2n.qn_new());
      /* p33.LUBO*/ wire _LUBO_SPR_PIX_DA3n_new = not1(sprite_temp_a.PUDU_SPRITE_DA3n.qn_new());
      /* p33.WERY*/ wire _WERY_SPR_PIX_DA4n_new = not1(sprite_temp_a.SAJA_SPRITE_DA4n.qn_new());
      /* p33.WURA*/ wire _WURA_SPR_PIX_DA5n_new = not1(sprite_temp_a.SUNY_SPRITE_DA5n.qn_new());
      /* p33.SULU*/ wire _SULU_SPR_PIX_DA6n_new = not1(sprite_temp_a.SEMO_SPRITE_DA6n.qn_new());
      /* p33.WAMY*/ wire _WAMY_SPR_PIX_DA7n_new = not1(sprite_temp_a.SEGA_SPRITE_DA7n.qn_new());

      /* p33.PYZU*/ wire _PYZU_SPR_PIX_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _RATA_SPR_PIX_DA0n_new);
      /* p33.MADA*/ wire _MADA_SPR_PIX_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _NUCA_SPR_PIX_DA1n_new);
      /* p33.LYDE*/ wire _LYDE_SPR_PIX_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _LASE_SPR_PIX_DA2n_new);
      /* p33.LUFY*/ wire _LUFY_SPR_PIX_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _LUBO_SPR_PIX_DA3n_new);
      /* p33.XATO*/ wire _XATO_SPR_PIX_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _WERY_SPR_PIX_DA4n_new);
      /* p33.XEXU*/ wire _XEXU_SPR_PIX_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _WURA_SPR_PIX_DA5n_new);
      /* p33.TUPE*/ wire _TUPE_SPR_PIX_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _SULU_SPR_PIX_DA6n_new);
      /* p33.XYVE*/ wire _XYVE_SPR_PIX_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _WAMY_SPR_PIX_DA7n_new);

      /* p33.WUFY*/ pix_pipes.WUFY_SPR_PIPE_A7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _VUNE_SPR_PIX_SET7_new, _XYVE_SPR_PIX_RST7_new, pix_pipes.VAFO_SPR_PIPE_A6_evn.qp_old());
      /* p33.VAFO*/ pix_pipes.VAFO_SPR_PIPE_A6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TUXA_SPR_PIX_SET6_new, _TUPE_SPR_PIX_RST6_new, pix_pipes.WORA_SPR_PIPE_A5_evn.qp_old());
      /* p33.WORA*/ pix_pipes.WORA_SPR_PIPE_A5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _VABY_SPR_PIX_SET5_new, _XEXU_SPR_PIX_RST5_new, pix_pipes.WYHO_SPR_PIPE_A4_evn.qp_old());
      /* p33.WYHO*/ pix_pipes.WYHO_SPR_PIPE_A4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _VEXU_SPR_PIX_SET4_new, _XATO_SPR_PIX_RST4_new, pix_pipes.LESU_SPR_PIPE_A3_evn.qp_old());
      /* p33.LESU*/ pix_pipes.LESU_SPR_PIPE_A3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _MAME_SPR_PIX_SET3_new, _LUFY_SPR_PIX_RST3_new, pix_pipes.LEFE_SPR_PIPE_A2_evn.qp_old());
      /* p33.LEFE*/ pix_pipes.LEFE_SPR_PIPE_A2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _LELA_SPR_PIX_SET2_new, _LYDE_SPR_PIX_RST2_new, pix_pipes.MASO_SPR_PIPE_A1_evn.qp_old());
      /* p33.MASO*/ pix_pipes.MASO_SPR_PIPE_A1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _MYTO_SPR_PIX_SET1_new, _MADA_SPR_PIX_RST1_new, pix_pipes.NURO_SPR_PIPE_A0_evn.qp_old());
      /* p33.NURO*/ pix_pipes.NURO_SPR_PIPE_A0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _PABE_SPR_PIX_SET0_new, _PYZU_SPR_PIX_RST0_new, GND);
    }

    // Sprite pipe B
    {
      /* p33.MEZU*/ wire _MEZU_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, sprite_temp_b.PEFO_SPRITE_DB0n.qn_new());
      /* p33.RUSY*/ wire _RUSY_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, sprite_temp_b.ROKA_SPRITE_DB1n.qn_new());
      /* p33.MYXA*/ wire _MYXA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, sprite_temp_b.MYTU_SPRITE_DB2n.qn_new());
      /* p33.RANO*/ wire _RANO_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, sprite_temp_b.RAMU_SPRITE_DB3n.qn_new());
      /* p33.TYGA*/ wire _TYGA_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, sprite_temp_b.SELE_SPRITE_DB4n.qn_new());
      /* p33.VUME*/ wire _VUME_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, sprite_temp_b.SUTO_SPRITE_DB5n.qn_new());
      /* p33.TAPO*/ wire _TAPO_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, sprite_temp_b.RAMA_SPRITE_DB6n.qn_new());
      /* p33.TESO*/ wire _TESO_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, sprite_temp_b.RYDU_SPRITE_DB7n.qn_new());

      /* p33.LOZA*/ wire _LOZA_SPR_PIX_DB0n_new = not1(sprite_temp_b.PEFO_SPRITE_DB0n.qn_new());
      /* p33.SYBO*/ wire _SYBO_SPR_PIX_DB1n_new = not1(sprite_temp_b.ROKA_SPRITE_DB1n.qn_new());
      /* p33.LUMO*/ wire _LUMO_SPR_PIX_DB2n_new = not1(sprite_temp_b.MYTU_SPRITE_DB2n.qn_new());
      /* p33.SOLO*/ wire _SOLO_SPR_PIX_DB3n_new = not1(sprite_temp_b.RAMU_SPRITE_DB3n.qn_new());
      /* p33.VOBY*/ wire _VOBY_SPR_PIX_DB4n_new = not1(sprite_temp_b.SELE_SPRITE_DB4n.qn_new());
      /* p33.WYCO*/ wire _WYCO_SPR_PIX_DB5n_new = not1(sprite_temp_b.SUTO_SPRITE_DB5n.qn_new());
      /* p33.SERY*/ wire _SERY_SPR_PIX_DB6n_new = not1(sprite_temp_b.RAMA_SPRITE_DB6n.qn_new());
      /* p33.SELU*/ wire _SELU_SPR_PIX_DB7n_new = not1(sprite_temp_b.RYDU_SPRITE_DB7n.qn_new());

      /* p33.MOFY*/ wire _MOFY_SPR_PIX_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _LOZA_SPR_PIX_DB0n_new);
      /* p33.RUCA*/ wire _RUCA_SPR_PIX_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _SYBO_SPR_PIX_DB1n_new);
      /* p33.MAJO*/ wire _MAJO_SPR_PIX_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _LUMO_SPR_PIX_DB2n_new);
      /* p33.REHU*/ wire _REHU_SPR_PIX_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _SOLO_SPR_PIX_DB3n_new);
      /* p33.WAXO*/ wire _WAXO_SPR_PIX_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _VOBY_SPR_PIX_DB4n_new);
      /* p33.XOLE*/ wire _XOLE_SPR_PIX_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _WYCO_SPR_PIX_DB5n_new);
      /* p33.TABY*/ wire _TABY_SPR_PIX_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _SERY_SPR_PIX_DB6n_new);
      /* p33.TULA*/ wire _TULA_SPR_PIX_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _SELU_SPR_PIX_DB7n_new);

      /* p33.VUPY*/ pix_pipes.VUPY_SPR_PIPE_B7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TESO_SPR_PIX_SET7_new, _TULA_SPR_PIX_RST7_new, pix_pipes.VANU_SPR_PIPE_B6_evn.qp_old());
      /* p33.VANU*/ pix_pipes.VANU_SPR_PIPE_B6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TAPO_SPR_PIX_SET6_new, _TABY_SPR_PIX_RST6_new, pix_pipes.WEBA_SPR_PIPE_B5_evn.qp_old());
      /* p33.WEBA*/ pix_pipes.WEBA_SPR_PIPE_B5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _VUME_SPR_PIX_SET5_new, _XOLE_SPR_PIX_RST5_new, pix_pipes.VARE_SPR_PIPE_B4_evn.qp_old());
      /* p33.VARE*/ pix_pipes.VARE_SPR_PIPE_B4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TYGA_SPR_PIX_SET4_new, _WAXO_SPR_PIX_RST4_new, pix_pipes.PYJO_SPR_PIPE_B3_evn.qp_old());
      /* p33.PYJO*/ pix_pipes.PYJO_SPR_PIPE_B3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _RANO_SPR_PIX_SET3_new, _REHU_SPR_PIX_RST3_new, pix_pipes.NATY_SPR_PIPE_B2_evn.qp_old());
      /* p33.NATY*/ pix_pipes.NATY_SPR_PIPE_B2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _MYXA_SPR_PIX_SET2_new, _MAJO_SPR_PIX_RST2_new, pix_pipes.PEFU_SPR_PIPE_B1_evn.qp_old());
      /* p33.PEFU*/ pix_pipes.PEFU_SPR_PIPE_B1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _RUSY_SPR_PIX_SET1_new, _RUCA_SPR_PIX_RST1_new, pix_pipes.NYLU_SPR_PIPE_B0_evn.qp_old());
      /* p33.NYLU*/ pix_pipes.NYLU_SPR_PIPE_B0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _MEZU_SPR_PIX_SET0_new, _MOFY_SPR_PIX_RST0_new, GND);
    }

    {
      /* p34.PUME*/ wire _PUME_PAL_PIPE_SET0n_new = nand2(_LESY_SPRITE_MASK0p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.SORO*/ wire _SORO_PAL_PIPE_SET1n_new = nand2(_LOTA_SPRITE_MASK1p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n_new = nand2(_LYKU_SPRITE_MASK2p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n_new = nand2(_ROBY_SPRITE_MASK3p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.RORA*/ wire _RORA_PAL_PIPE_SET4n_new = nand2(_TYTA_SPRITE_MASK4p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.MENE*/ wire _MENE_PAL_PIPE_SET5n_new = nand2(_TYCO_SPRITE_MASK5p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n_new = nand2(_SOKA_SPRITE_MASK6p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n_new = nand2(_XOVU_SPRITE_MASK7p_new, oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());

      /* p34.SYPY*/ wire _SYPY_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.TOTU*/ wire _TOTU_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.NARO*/ wire _NARO_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.WEXY*/ wire _WEXY_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.RYZY*/ wire _RYZY_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.RYFE*/ wire _RYFE_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.LADY*/ wire _LADY_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());
      /* p34.LAFY*/ wire _LAFY_new = not1(oam_temp_b.GOMO_OAM_DB4p_evn.qp_new());

      /* p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n_new = nand2(_LESY_SPRITE_MASK0p_new, _SYPY_new);
      /* p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n_new = nand2(_LOTA_SPRITE_MASK1p_new, _TOTU_new);
      /* p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n_new = nand2(_LYKU_SPRITE_MASK2p_new, _NARO_new);
      /* p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n_new = nand2(_ROBY_SPRITE_MASK3p_new, _WEXY_new);
      /* p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n_new = nand2(_TYTA_SPRITE_MASK4p_new, _RYZY_new);
      /* p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n_new = nand2(_TYCO_SPRITE_MASK5p_new, _RYFE_new);
      /* p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n_new = nand2(_SOKA_SPRITE_MASK6p_new, _LADY_new);
      /* p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n_new = nand2(_XOVU_SPRITE_MASK7p_new, _LAFY_new);

      /* p34.LYME*/ pix_pipes.LYME_PAL_PIPE_D7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _LAMY_PAL_PIPE_SET7n_new, _LUNU_PAL_PIPE_RST7n_new, pix_pipes.MODA_PAL_PIPE_D6_evn.qp_old());
      /* p34.MODA*/ pix_pipes.MODA_PAL_PIPE_D6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _LUKE_PAL_PIPE_SET6n_new, _LOWA_PAL_PIPE_RST6n_new, pix_pipes.NUKE_PAL_PIPE_D5_evn.qp_old());
      /* p34.NUKE*/ pix_pipes.NUKE_PAL_PIPE_D5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _MENE_PAL_PIPE_SET5n_new, _PAZO_PAL_PIPE_RST5n_new, pix_pipes.PALU_PAL_PIPE_D4_evn.qp_old());
      /* p34.PALU*/ pix_pipes.PALU_PAL_PIPE_D4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _RORA_PAL_PIPE_SET4n_new, _RUDU_PAL_PIPE_RST4n_new, pix_pipes.SOMY_PAL_PIPE_D3_evn.qp_old());
      /* p34.SOMY*/ pix_pipes.SOMY_PAL_PIPE_D3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _SUKY_PAL_PIPE_SET3n_new, _TOWA_PAL_PIPE_RST3n_new, pix_pipes.ROSA_PAL_PIPE_D2_evn.qp_old());
      /* p34.ROSA*/ pix_pipes.ROSA_PAL_PIPE_D2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _PAMO_PAL_PIPE_SET2n_new, _PYZY_PAL_PIPE_RST2n_new, pix_pipes.SATA_PAL_PIPE_D1_evn.qp_old());
      /* p34.SATA*/ pix_pipes.SATA_PAL_PIPE_D1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _SORO_PAL_PIPE_SET1n_new, _TAFA_PAL_PIPE_RST1n_new, pix_pipes.RUGO_PAL_PIPE_D0_evn.qp_old());
      /* p34.RUGO*/ pix_pipes.RUGO_PAL_PIPE_D0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _PUME_PAL_PIPE_SET0n_new, _SUCO_PAL_PIPE_RST0n_new, GND);
    }

    {
      /* p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XALA*/ wire _XALA_MASK_PIPE_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XELY*/ wire _XELY_MASK_PIPE_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());

      /* p26.XOGA*/ wire _XOGA_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XURA*/ wire _XURA_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.TAJO*/ wire _TAJO_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XENU*/ wire _XENU_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XYKE*/ wire _XYKE_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XABA*/ wire _XABA_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.TAFU*/ wire _TAFU_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());
      /* p26.XUHO*/ wire _XUHO_new = not1(oam_temp_b.DEPO_OAM_DB7p_evn.qp_new());

      /* p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _XOGA_new);
      /* p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _XURA_new);
      /* p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _TAJO_new);
      /* p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _XENU_new);
      /* p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _XYKE_new);
      /* p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _XABA_new);
      /* p26.TENA*/ wire _TENA_MASK_PIPE_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _TAFU_new);
      /* p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _XUHO_new);

      /* p26.VAVA*/ pix_pipes.VAVA_MASK_PIPE_7_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TUWU_MASK_PIPE_SET7_new, _WUBU_MASK_PIPE_RST7_new, pix_pipes.VUMO_MASK_PIPE_6_evn.qp_old());
      /* p26.VUMO*/ pix_pipes.VUMO_MASK_PIPE_6_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TYKO_MASK_PIPE_SET6_new, _TENA_MASK_PIPE_RST6_new, pix_pipes.WODA_MASK_PIPE_5_evn.qp_old());
      /* p26.WODA*/ pix_pipes.WODA_MASK_PIPE_5_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _XELY_MASK_PIPE_SET5_new, _WUJA_MASK_PIPE_RST5_new, pix_pipes.XETE_MASK_PIPE_4_evn.qp_old());
      /* p26.XETE*/ pix_pipes.XETE_MASK_PIPE_4_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _XUKU_MASK_PIPE_SET4_new, _WEDY_MASK_PIPE_RST4_new, pix_pipes.WYFU_MASK_PIPE_3_evn.qp_old());
      /* p26.WYFU*/ pix_pipes.WYFU_MASK_PIPE_3_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _XYRU_MASK_PIPE_SET3_new, _WEVO_MASK_PIPE_RST3_new, pix_pipes.VOSA_MASK_PIPE_2_evn.qp_old());
      /* p26.VOSA*/ pix_pipes.VOSA_MASK_PIPE_2_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TYRA_MASK_PIPE_SET2_new, _TUFO_MASK_PIPE_RST2_new, pix_pipes.WURU_MASK_PIPE_1_evn.qp_old());
      /* p26.WURU*/ pix_pipes.WURU_MASK_PIPE_1_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _XALA_MASK_PIPE_SET1_new, _WEDE_MASK_PIPE_RST1_new, pix_pipes.VEZO_MASK_PIPE_0_evn.qp_old());
      /* p26.VEZO*/ pix_pipes.VEZO_MASK_PIPE_0_evn.dff22(SACU_CLKPIPE_AxCxExGx_clknew_evn, _TEDE_MASK_PIPE_SET0_new, _WOKA_MASK_PIPE_RST0_new, _VYPO_VCC);
    }
  }();

  //----------------------------------------
  // Pixel output

  [
    this,
    BUS_CPU_A,
    BUS_CPU_D,
    XODO_VID_RSTp_new_h,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    &REMY_LD0n_new,
    &RAVO_LD1n_new

  ]() {
    {
      // FF47 BGP
      /* p36.VUSO*/ wire _VUSO_FF47_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), WERA_FF47p_ext(BUS_CPU_A));
      /* p36.VELY*/ wire _VELY_FF47_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), WERA_FF47p_ext(BUS_CPU_A));
      /* p36.TEPO*/ wire _TEPO_FF47_WRp_clk = not1(_VELY_FF47_WRp_clk);
      /* p36.TEPY*/ wire _TEPY_FF47_RDn_ext = not1(_VUSO_FF47_RDp_ext);

      /*#p36.PAVO*/ reg_bgp.PAVO_BGP_D0n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[0]);
      /* p36.NUSY*/ reg_bgp.NUSY_BGP_D1n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[1]);
      /* p36.PYLU*/ reg_bgp.PYLU_BGP_D2n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[2]);
      /* p36.MAXY*/ reg_bgp.MAXY_BGP_D3n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[3]);
      /* p36.MUKE*/ reg_bgp.MUKE_BGP_D4n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[4]);
      /* p36.MORU*/ reg_bgp.MORU_BGP_D5n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[5]);
      /* p36.MOGY*/ reg_bgp.MOGY_BGP_D6n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[6]);
      /* p36.MENA*/ reg_bgp.MENA_BGP_D7n_h.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_D[7]);

      /*#p36.RARO*/ BUS_CPU_D_out[0].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.PAVO_BGP_D0n_h.qp_new());
      /* p36.PABA*/ BUS_CPU_D_out[1].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.NUSY_BGP_D1n_h.qp_new());
      /* p36.REDO*/ BUS_CPU_D_out[2].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.PYLU_BGP_D2n_h.qp_new());
      /* p36.LOBE*/ BUS_CPU_D_out[3].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.MAXY_BGP_D3n_h.qp_new());
      /* p36.LACE*/ BUS_CPU_D_out[4].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.MUKE_BGP_D4n_h.qp_new());
      /* p36.LYKA*/ BUS_CPU_D_out[5].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.MORU_BGP_D5n_h.qp_new());
      /* p36.LODY*/ BUS_CPU_D_out[6].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.MOGY_BGP_D6n_h.qp_new());
      /* p36.LARY*/ BUS_CPU_D_out[7].tri6_nn(_TEPY_FF47_RDn_ext, reg_bgp.MENA_BGP_D7n_h.qp_new());
    }

    {
      // FF48 OBP0
      /* p36.XUFY*/ wire _XUFY_FF48_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XAYO_FF48p_ext(BUS_CPU_A));
      /* p36.XOMA*/ wire _XOMA_FF48_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XAYO_FF48p_ext(BUS_CPU_A));
      /* p36.XELO*/ wire _XELO_FF48_WRn_clk = not1(_XOMA_FF48_WRp_clk);
      /* p36.XOZY*/ wire _XOZY_FF48_RDn_ext = not1(_XUFY_FF48_RDp_ext);

      /* p36.XUFU*/ reg_obp0.XUFU_OBP0_D0n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[0]);
      /* p36.XUKY*/ reg_obp0.XUKY_OBP0_D1n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[1]);
      /* p36.XOVA*/ reg_obp0.XOVA_OBP0_D2n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[2]);
      /* p36.XALO*/ reg_obp0.XALO_OBP0_D3n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[3]);
      /* p36.XERU*/ reg_obp0.XERU_OBP0_D4n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[4]);
      /* p36.XYZE*/ reg_obp0.XYZE_OBP0_D5n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[5]);
      /* p36.XUPO*/ reg_obp0.XUPO_OBP0_D6n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[6]);
      /* p36.XANA*/ reg_obp0.XANA_OBP0_D7n_h.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_D[7]);

      /*#p36.XARY*/ BUS_CPU_D_out[0].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XUFU_OBP0_D0n_h.qp_new());
      /* p36.XOKE*/ BUS_CPU_D_out[1].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XUKY_OBP0_D1n_h.qp_new());
      /* p36.XUNO*/ BUS_CPU_D_out[2].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XOVA_OBP0_D2n_h.qp_new());
      /* p36.XUBY*/ BUS_CPU_D_out[3].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XALO_OBP0_D3n_h.qp_new());
      /* p36.XAJU*/ BUS_CPU_D_out[4].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XERU_OBP0_D4n_h.qp_new());
      /* p36.XOBO*/ BUS_CPU_D_out[5].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XYZE_OBP0_D5n_h.qp_new());
      /* p36.XAXA*/ BUS_CPU_D_out[6].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XUPO_OBP0_D6n_h.qp_new());
      /* p36.XAWO*/ BUS_CPU_D_out[7].tri6_nn(_XOZY_FF48_RDn_ext, reg_obp0.XANA_OBP0_D7n_h.qp_new());
    }

    {
      // FF49 OBP1
      /* p36.MUMY*/ wire MUMY_FF49_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), TEGO_FF49p_ext(BUS_CPU_A));
      /* p36.MYXE*/ wire MYXE_FF49_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), TEGO_FF49p_ext(BUS_CPU_A));
      /* p36.LEHO*/ wire _LEHO_FF49_WRn_clk = not1(MYXE_FF49_WRp_clk);
      /* p36.LOTE*/ wire _LOTE_FF49_RDn_ext = not1(MUMY_FF49_RDp_ext);

      /* p36.MOXY*/ reg_obp1.MOXY_OBP1_D0n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[0]);
      /* p36.LAWO*/ reg_obp1.LAWO_OBP1_D1n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[1]);
      /* p36.MOSA*/ reg_obp1.MOSA_OBP1_D2n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[2]);
      /* p36.LOSE*/ reg_obp1.LOSE_OBP1_D3n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[3]);
      /* p36.LUNE*/ reg_obp1.LUNE_OBP1_D4n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[4]);
      /* p36.LUGU*/ reg_obp1.LUGU_OBP1_D5n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[5]);
      /* p36.LEPU*/ reg_obp1.LEPU_OBP1_D6n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[6]);
      /* p36.LUXO*/ reg_obp1.LUXO_OBP1_D7n_h.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_D[7]);

      /*#p36.LAJU*/ BUS_CPU_D_out[0].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.MOXY_OBP1_D0n_h.qp_new());
      /* p36.LEPA*/ BUS_CPU_D_out[1].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LAWO_OBP1_D1n_h.qp_new());
      /* p36.LODE*/ BUS_CPU_D_out[2].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.MOSA_OBP1_D2n_h.qp_new());
      /* p36.LYZA*/ BUS_CPU_D_out[3].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LOSE_OBP1_D3n_h.qp_new());
      /* p36.LUKY*/ BUS_CPU_D_out[4].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LUNE_OBP1_D4n_h.qp_new());
      /* p36.LUGA*/ BUS_CPU_D_out[5].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LUGU_OBP1_D5n_h.qp_new());
      /* p36.LEBA*/ BUS_CPU_D_out[6].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LEPU_OBP1_D6n_h.qp_new());
      /* p36.LELU*/ BUS_CPU_D_out[7].tri6_nn(_LOTE_FF49_RDn_ext, reg_obp1.LUXO_OBP1_D7n_h.qp_new());
    }

    // Pixel merge + emit
    /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp_new  = and2(pix_pipes.PYBO_BGW_PIPE_A7_evn.qp_new(), reg_lcdc.VYXE_LCDC_BGENn_h.qn_new());
    /*#p35.TADE*/ wire TADE_PIX_BG_HIp_new  = and2(pix_pipes.SOHU_BGW_PIPE_B7_evn.qp_new(), reg_lcdc.VYXE_LCDC_BGENn_h.qn_new());
    /*#p35.XULA*/ wire XULA_PIX_SP_LOp_new  = and2(reg_lcdc.XYLO_LCDC_SPENn_h.qn_new(), pix_pipes.WUFY_SPR_PIPE_A7_evn.qp_new());
    /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp_new  = and2(reg_lcdc.XYLO_LCDC_SPENn_h.qn_new(), pix_pipes.VUPY_SPR_PIPE_B7_evn.qp_new());

    /*#p35.NULY*/ wire NULY_PIX_SP_MASKn_new = nor2(WOXA_PIX_SP_HIp_new, XULA_PIX_SP_LOp_new);

    /*#p35.RYFU*/ wire RYFU_MASK_BG0_new = and2(RAJY_PIX_BG_LOp_new, pix_pipes.VAVA_MASK_PIPE_7_evn.qp_new());
    /*#p35.RUTA*/ wire RUTA_MASK_BG1_new = and2(TADE_PIX_BG_HIp_new, pix_pipes.VAVA_MASK_PIPE_7_evn.qp_new());
    /*#p35.POKA*/ wire POKA_BGPIXELn_new = nor3(NULY_PIX_SP_MASKn_new, RUTA_MASK_BG1_new, RYFU_MASK_BG0_new);

    /*#p34.LOME*/ wire LOME_PAL_PIPE_7n_new = not1(pix_pipes.LYME_PAL_PIPE_D7_evn.qp_new());
    /*#p34.LAFU*/ wire LAFU_OBP0PIXELn_new = nand2(LOME_PAL_PIPE_7n_new, POKA_BGPIXELn_new);
    /*#p34.LEKA*/ wire LEKA_OBP1PIXELn_new = nand2(pix_pipes.LYME_PAL_PIPE_D7_evn.qp_new(), POKA_BGPIXELn_new);

    //----------
    // Sprite palette 0 lookup

    /*#p35.WELE*/ wire WELE_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
    /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp_new = not1(WELE_PIX_SP_LOn_new);
    /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
    /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp_new = not1(VUMU_PIX_SP_HIn_new);

    /*#p35.LAVA*/ wire LAVA_MASK_OPB0_new  = not1(LAFU_OBP0PIXELn_new);

    /*#p35.VUGO*/ wire VUGO_PAL_OBP0A_new = and3(VUMU_PIX_SP_HIn_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
    /*#p35.VOLO*/ wire VOLO_PAL_OBP0B_new = and3(VUMU_PIX_SP_HIn_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm
    /*#p35.VATA*/ wire VATA_PAL_OBP0C_new = and3(WYRU_PIX_SP_HIp_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
    /*#p35.VYRO*/ wire VYRO_PAL_OBP0D_new = and3(WYRU_PIX_SP_HIp_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm

    /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI_new = amux4(reg_obp0.XANA_OBP0_D7n_h.qn_new(), VYRO_PAL_OBP0D_new,
                                                    reg_obp0.XYZE_OBP0_D5n_h.qn_new(), VATA_PAL_OBP0C_new,
                                                    reg_obp0.XALO_OBP0_D3n_h.qn_new(), VOLO_PAL_OBP0B_new,
                                                    reg_obp0.XUKY_OBP0_D1n_h.qn_new(), VUGO_PAL_OBP0A_new);

    /*#p35.WALY*/ wire WALY_COL_OBP0_LO_new = amux4(reg_obp0.XUPO_OBP0_D6n_h.qn_new(), VYRO_PAL_OBP0D_new,
                                                    reg_obp0.XERU_OBP0_D4n_h.qn_new(), VATA_PAL_OBP0C_new,
                                                    reg_obp0.XOVA_OBP0_D2n_h.qn_new(), VOLO_PAL_OBP0B_new,
                                                    reg_obp0.XUFU_OBP0_D0n_h.qn_new(), VUGO_PAL_OBP0A_new);

    //----------
    // Sprite palette 1 lookup

    /*#p35.MABY*/ wire MABY_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
    /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp_new = not1(MABY_PIX_SP_LOn_new);
    /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
    /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp_new = not1(MEXA_PIX_SP_HIn_new);

    /*#p35.LUKU*/ wire LUKU_MASK_OBP1_new  = not1(LEKA_OBP1PIXELn_new);

    /* p#35.LOPU*/ wire LOPU_PAL_OBP1A_new = and3(MEXA_PIX_SP_HIn_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
    /* p#35.LYKY*/ wire LYKY_PAL_OBP1B_new = and3(MEXA_PIX_SP_HIn_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm
    /* p#35.LARU*/ wire LARU_PAL_OBP1C_new = and3(LOZO_PIX_SP_HIp_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
    /* p#35.LEDO*/ wire LEDO_PAL_OBP1D_new = and3(LOZO_PIX_SP_HIp_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm

    /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI_new = amux4(reg_obp1.LUXO_OBP1_D7n_h.qn_new(), LEDO_PAL_OBP1D_new,
                                                    reg_obp1.LUGU_OBP1_D5n_h.qn_new(), LARU_PAL_OBP1C_new,
                                                    reg_obp1.LOSE_OBP1_D3n_h.qn_new(), LYKY_PAL_OBP1B_new,
                                                    reg_obp1.LAWO_OBP1_D1n_h.qn_new(), LOPU_PAL_OBP1A_new);

    /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO_new = amux4(LEDO_PAL_OBP1D_new, reg_obp1.LEPU_OBP1_D6n_h.qn_new(),
                                                    LARU_PAL_OBP1C_new, reg_obp1.LUNE_OBP1_D4n_h.qn_new(),
                                                    LYKY_PAL_OBP1B_new, reg_obp1.MOSA_OBP1_D2n_h.qn_new(),
                                                    LOPU_PAL_OBP1A_new, reg_obp1.MOXY_OBP1_D0n_h.qn_new());

    //----------
    // Background/window palette lookup

    /* p35.SOBA*/ wire SOBA_PIX_BG_LOn_new = not1(RAJY_PIX_BG_LOp_new);
    /* p35.NUPO*/ wire NUPO_PIX_BG_LOp_new = not1(SOBA_PIX_BG_LOn_new);
    /* p35.VYCO*/ wire VYCO_PIX_BG_HIn_new = not1(TADE_PIX_BG_HIp_new);
    /* p35.NALE*/ wire NALE_PIX_BG_HIp_new = not1(VYCO_PIX_BG_HIn_new);

    /* p35.MUVE*/ wire MUVE_MASK_BGP_new = not1(POKA_BGPIXELn_new);

    /* p35.POBU*/ wire POBU_PAL_BGPA_new = and3(VYCO_PIX_BG_HIn_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
    /* p35.NUXO*/ wire NUXO_PAL_BGPB_new = and3(VYCO_PIX_BG_HIn_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm
    /* p35.NUMA*/ wire NUMA_PAL_BGPC_new = and3(NALE_PIX_BG_HIp_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
    /* p35.NYPO*/ wire NYPO_PAL_BGPD_new = and3(NALE_PIX_BG_HIp_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm

    /*#p35.NELO*/ wire NELO_COL_BG_LO_new = amux4(NYPO_PAL_BGPD_new, reg_bgp.MOGY_BGP_D6n_h.qn_new(),
                                                  NUMA_PAL_BGPC_new, reg_bgp.MUKE_BGP_D4n_h.qn_new(),
                                                  NUXO_PAL_BGPB_new, reg_bgp.PYLU_BGP_D2n_h.qn_new(),
                                                  POBU_PAL_BGPA_new, reg_bgp.PAVO_BGP_D0n_h.qn_new());

    /*#p35.NURA*/ wire NURA_COL_BG_HI_new = amux4(reg_bgp.MENA_BGP_D7n_h.qn_new(), NYPO_PAL_BGPD_new,
                                                  reg_bgp.MORU_BGP_D5n_h.qn_new(), NUMA_PAL_BGPC_new,
                                                  reg_bgp.MAXY_BGP_D3n_h.qn_new(), NUXO_PAL_BGPB_new,
                                                  reg_bgp.NUSY_BGP_D1n_h.qn_new(), POBU_PAL_BGPA_new);

    //----------
    // Pixel merge and send

    /*#p35.PERO*/ wire _PERO_COL_LO_new = or3(NELO_COL_BG_LO_new, WALY_COL_OBP0_LO_new, MUFA_COL_OBP1_LO_new);
    /*#p35.PATY*/ wire _PATY_COL_HI_new = or3(NURA_COL_BG_HI_new, WUFU_COL_OBP0_HI_new, MOKA_COL_OBP1_HI_new);

    /*#p35.REMY*/ wire _REMY_LD0n_new = not1(_PERO_COL_LO_new);
    /*#p35.RAVO*/ wire _RAVO_LD1n_new = not1(_PATY_COL_HI_new);

    /*#p35.REMY*/ REMY_LD0n_new = _REMY_LD0n_new;
    /*#p35.RAVO*/ RAVO_LD1n_new = _RAVO_LD1n_new;
  }();
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcd(
    wire TALU_xxCDEFxx_clkevn,
    wire TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
    wire SACU_CLKPIPE_AxCxExGx_clknew_evn,
    wire XONA_LCDC_LCDENp_h_new,
    wire XYMU_RENDERINGp_new_xxx,
    wire AVAP_SCAN_DONE_TRIGp_new_xxx,
    wire WEGO_HBLANKp_new_any,
    wire PURE_LINE_ENDn_new_evn,
    wire REMY_LD0n_new,
    wire RAVO_LD1n_new)
{
  lcd.tock_pins(
    rstdbg.XODO_VID_RSTp(),
    TALU_xxCDEFxx_clkevn,
    TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
    XONA_LCDC_LCDENp_h_new,
    XYMU_RENDERINGp_new_xxx,
    AVAP_SCAN_DONE_TRIGp_new_xxx,
    pix_count.XYDO_PX3p.qp_old(),
    div.TULU_DIV07p_evn.qp_new(),
    REMY_LD0n_new,
    RAVO_LD1n_new,
    reg_lx,
    reg_ly
  );

  //----------------------------------------

  [
    this,
    XONA_LCDC_LCDENp_h_new,
    PURE_LINE_ENDn_new_evn
  ](){
    /* p01.UMEK*/ wire _UMEK_DIV06n_new = not1(div.UGOT_DIV06p_evn.qp_new());
    /*#p24.KASA*/ wire _KASA_LINE_ENDp_new = not1(PURE_LINE_ENDn_new_evn);
    /*#p24.UMOB*/ wire _UMOB_DIV_06p_new = not1(_UMEK_DIV06n_new);
    /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new_evn = not1(XONA_LCDC_LCDENp_h_new);
    /*#p24.KAHE*/ wire _KAHE_LINE_ENDp_new = amux2(rstdbg.XONA_LCDC_LCDENn_h.qn_new(), _KASA_LINE_ENDp_new, _KEDY_LCDC_ENn_new_evn, _UMOB_DIV_06p_new);
    /*#p24.KYMO*/ wire _KYMO_LINE_ENDn_new = not1(_KAHE_LINE_ENDp_new);
    lcd.PIN_LCD_LATCH_evn.pin_out(_KYMO_LINE_ENDn_new, _KYMO_LINE_ENDn_new);
  }();

  //----------------------------------------

  [
    this,
    WEGO_HBLANKp_new_any,
    SACU_CLKPIPE_AxCxExGx_clknew_evn
  ](){
    /*#p21.XAJO*/ wire _XAJO_X_009p_new = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
    /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p_new, WEGO_HBLANKp_new_any);
    /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK_new = and2(lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_AxCxExGx_clknew_evn);
    /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK_new = or2(_TOBA_LCD_CLOCK_new, fine_scroll.POVA_FINE_MATCH_TRIGp());
    /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK_new = not1(_SEMU_LCD_CLOCK_new);
    lcd.PIN_LCD_CLOCK_xxx.pin_out(_RYPO_LCD_CLOCK_new, _RYPO_LCD_CLOCK_new);
  }();

  //probe(9, "WUSA_LCD_CLOCK_GATE", lcd.WUSA_LCD_CLOCK_GATE_xxx.qp_new());

  //----------------------------------------
  // LCD pixel line buffer

  [this]() {
    if (!old_lcd_clock && lcd.PIN_LCD_CLOCK_xxx.qp_new()) {
      gb_screen_x++;
    }
    if (lcd.PIN_LCD_HSYNC_evn.qp_new() || lcd.PIN_LCD_LATCH_evn.qp_new()) {
      gb_screen_x = 0;
    }

    lcd.lcd_pix_lo.nor_latch(lcd.PIN_LCD_DATA0.qp_new(), lcd.PIN_LCD_CLOCK_xxx.qp_new() | lcd.PIN_LCD_HSYNC_evn.qp_new());
    lcd.lcd_pix_hi.nor_latch(lcd.PIN_LCD_DATA1.qp_new(), lcd.PIN_LCD_CLOCK_xxx.qp_new() | lcd.PIN_LCD_HSYNC_evn.qp_new());

    if (!old_lcd_latch && lcd.PIN_LCD_LATCH_evn.qp_new()) {
      if (gb_screen_y < 144) {
        for (int x = 0; x < 159; x++) {
          uint8_t p0 = lcd.lcd_pipe_lo[x + 1].qp_old();
          uint8_t p1 = lcd.lcd_pipe_hi[x + 1].qp_old();
          framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
        }
        {
          uint8_t p0 = lcd.lcd_pix_lo.qp_new();
          uint8_t p1 = lcd.lcd_pix_hi.qp_new();
          framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
        }
      }

      gb_screen_y++;
      if (lcd.PIN_LCD_VSYNC_evn.qp_new()) {
        gb_screen_y = 0;
      }
    }

    for (int i = 0; i < 159; i++) {
      lcd.lcd_pipe_lo[i].dff(!lcd.PIN_LCD_CLOCK_xxx.qp_new(), 1, 1, lcd.lcd_pipe_lo[i + 1].qp_old());
      lcd.lcd_pipe_hi[i].dff(!lcd.PIN_LCD_CLOCK_xxx.qp_new(), 1, 1, lcd.lcd_pipe_hi[i + 1].qp_old());
    }

    lcd.lcd_pipe_lo[159].dff(!lcd.PIN_LCD_CLOCK_xxx.qp_new(), 1, 1, lcd.lcd_pix_lo.qp_new());
    lcd.lcd_pipe_hi[159].dff(!lcd.PIN_LCD_CLOCK_xxx.qp_new(), 1, 1, lcd.lcd_pix_hi.qp_new());

    old_lcd_clock = lcd.PIN_LCD_CLOCK_xxx.qp_new();
    old_lcd_latch = lcd.PIN_LCD_LATCH_evn.qp_new();
  }();
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_ext(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire ABUZ_xxCDEFGH_clk_evn,
    wire TEDO_CPU_RDp,
    wire APOV_CPU_WRp_clkevn,
    wire TEXO_ADDR_VRAMn_ext,
    wire TUTU_READ_BOOTROMp_new)
{
  // Don't think I need this.
  /*
  PIN_EXT_D[0].reset();
  PIN_EXT_D[1].reset();
  PIN_EXT_D[2].reset();
  PIN_EXT_D[3].reset();
  PIN_EXT_D[4].reset();
  PIN_EXT_D[5].reset();
  PIN_EXT_D[6].reset();
  PIN_EXT_D[7].reset();
  */

  //----------------------------------------
  // External address mux

  [this, BUS_CPU_A, TEXO_ADDR_VRAMn_ext](){
    /* p08.LOXO*/ wire _LOXO_HOLDn_new = and_or3(rstdbg.MULE_MODE_DBG1n_ext(), TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
    /* p08.LASY*/ wire _LASY_HOLDp_new = not1(_LOXO_HOLDn_new);
    /* p08.MATE*/ wire _MATE_HOLDn_new = not1(_LASY_HOLDp_new);
    /* p08.ALOR*/ ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 0]);
    /* p08.APUR*/ ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 1]);
    /* p08.ALYR*/ ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 2]);
    /* p08.ARET*/ ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 3]);
    /* p08.AVYS*/ ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 4]);
    /* p08.ATEV*/ ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 5]);
    /* p08.AROS*/ ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 6]);
    /* p08.ARYM*/ ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 7]);
    /* p08.LUNO*/ ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 8]);
    /* p08.LYSA*/ ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 9]);
    /* p08.PATE*/ ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[10]);
    /* p08.LUMY*/ ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[11]);
    /* p08.LOBU*/ ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[12]);
    /* p08.LONU*/ ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[13]);
    /* p08.NYRE*/ ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[14]);

    wire _LUMA_DMA_CARTp_new = dma.LUMA_DMA_CARTp_new();
    /* p08.AMET*/ wire _AMET_A00p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NAKY_DMA_A00p_evn.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
    /* p08.ATOL*/ wire _ATOL_A01p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.PYRO_DMA_A01p_evn.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
    /* p08.APOK*/ wire _APOK_A02p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NEFY_DMA_A02p_evn.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
    /* p08.AMER*/ wire _AMER_A03p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.MUTY_DMA_A03p_evn.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
    /* p08.ATEM*/ wire _ATEM_A04p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NYKO_DMA_A04p_evn.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
    /* p08.ATOV*/ wire _ATOV_A05p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.PYLO_DMA_A05p_evn.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
    /* p08.ATYR*/ wire _ATYR_A06p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NUTO_DMA_A06p_evn.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
    /*#p08.ASUR*/ wire _ASUR_A07p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.MUGU_DMA_A07p_evn.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
    /*#p08.MANO*/ wire _MANO_A08p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NAFA_DMA_A08n_h.qn_new(),   ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
    /* p08.MASU*/ wire _MASU_A09p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.PYNE_DMA_A09n_h.qn_new(),   ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
    /* p08.PAMY*/ wire _PAMY_A10p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.PARA_DMA_A10n_h.qn_new(),   ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
    /* p08.MALE*/ wire _MALE_A11p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NYDO_DMA_A11n_h.qn_new(),   ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
    /* p08.MOJY*/ wire _MOJY_A12p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.NYGY_DMA_A12n_h.qn_new(),   ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
    /* p08.MUCE*/ wire _MUCE_A13p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.PULA_DMA_A13n_h.qn_new(),   ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
    /* p08.PEGE*/ wire _PEGE_A14p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma.POKU_DMA_A14n_h.qn_new(),   ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

    /* p08.KUPO*/ wire _KUPO_new_evn = nand2(_AMET_A00p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.CABA*/ wire _CABA_new_evn = nand2(_ATOL_A01p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.BOKU*/ wire _BOKU_new_evn = nand2(_APOK_A02p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.BOTY*/ wire _BOTY_new_evn = nand2(_AMER_A03p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.BYLA*/ wire _BYLA_new_evn = nand2(_ATEM_A04p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.BADU*/ wire _BADU_new_evn = nand2(_ATOV_A05p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.CEPU*/ wire _CEPU_new_evn = nand2(_ATYR_A06p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.DEFY*/ wire _DEFY_new_evn = nand2(_ASUR_A07p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.MYNY*/ wire _MYNY_new_evn = nand2(_MANO_A08p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.MUNE*/ wire _MUNE_new_evn = nand2(_MASU_A09p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.ROXU*/ wire _ROXU_new_evn = nand2(_PAMY_A10p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.LEPY*/ wire _LEPY_new_evn = nand2(_MALE_A11p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.LUCE*/ wire _LUCE_new_evn = nand2(_MOJY_A12p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.LABE*/ wire _LABE_new_evn = nand2(_MUCE_A13p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.PUHE*/ wire _PUHE_new_evn = nand2(_PEGE_A14p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());

    /* p08.KOTY*/ wire _KOTY_new_evn = nor2 (_AMET_A00p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.COTU*/ wire _COTU_new_evn = nor2 (_ATOL_A01p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.BAJO*/ wire _BAJO_new_evn = nor2 (_APOK_A02p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.BOLA*/ wire _BOLA_new_evn = nor2 (_AMER_A03p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.BEVO*/ wire _BEVO_new_evn = nor2 (_ATEM_A04p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.AJAV*/ wire _AJAV_new_evn = nor2 (_ATOV_A05p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.CYKA*/ wire _CYKA_new_evn = nor2 (_ATYR_A06p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.COLO*/ wire _COLO_new_evn = nor2 (_ASUR_A07p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.MEGO*/ wire _MEGO_new_evn = nor2 (_MANO_A08p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.MENY*/ wire _MENY_new_evn = nor2 (_MASU_A09p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.RORE*/ wire _RORE_new_evn = nor2 (_PAMY_A10p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.LYNY*/ wire _LYNY_new_evn = nor2 (_MALE_A11p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.LOSO*/ wire _LOSO_new_evn = nor2 (_MOJY_A12p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.LEVA*/ wire _LEVA_new_evn = nor2 (_MUCE_A13p_new_evn, rstdbg.UNOR_MODE_DBG2p());
    /* p08.PAHY*/ wire _PAHY_new_evn = nor2 (_PEGE_A14p_new_evn, rstdbg.UNOR_MODE_DBG2p());

    ext_bus.PIN_EXT_A[ 0].pin_out(_KUPO_new_evn, _KOTY_new_evn);
    ext_bus.PIN_EXT_A[ 1].pin_out(_CABA_new_evn, _COTU_new_evn);
    ext_bus.PIN_EXT_A[ 2].pin_out(_BOKU_new_evn, _BAJO_new_evn);
    ext_bus.PIN_EXT_A[ 3].pin_out(_BOTY_new_evn, _BOLA_new_evn);
    ext_bus.PIN_EXT_A[ 4].pin_out(_BYLA_new_evn, _BEVO_new_evn);
    ext_bus.PIN_EXT_A[ 5].pin_out(_BADU_new_evn, _AJAV_new_evn);
    ext_bus.PIN_EXT_A[ 6].pin_out(_CEPU_new_evn, _CYKA_new_evn);
    ext_bus.PIN_EXT_A[ 7].pin_out(_DEFY_new_evn, _COLO_new_evn);
    ext_bus.PIN_EXT_A[ 8].pin_out(_MYNY_new_evn, _MEGO_new_evn);
    ext_bus.PIN_EXT_A[ 9].pin_out(_MUNE_new_evn, _MENY_new_evn);
    ext_bus.PIN_EXT_A[10].pin_out(_ROXU_new_evn, _RORE_new_evn);
    ext_bus.PIN_EXT_A[11].pin_out(_LEPY_new_evn, _LYNY_new_evn);
    ext_bus.PIN_EXT_A[12].pin_out(_LUCE_new_evn, _LOSO_new_evn);
    ext_bus.PIN_EXT_A[13].pin_out(_LABE_new_evn, _LEVA_new_evn);
    ext_bus.PIN_EXT_A[14].pin_out(_PUHE_new_evn, _PAHY_new_evn);
  }();

  // A15 is "special"
  [this, BUS_CPU_A, ABUZ_xxCDEFGH_clk_evn, TUTU_READ_BOOTROMp_new](){
    /* p08.SOBY*/ wire _SOBY_A15n_new = nor2(BUS_CPU_A[15], TUTU_READ_BOOTROMp_new);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx_clknew = nand2(ABUZ_xxCDEFGH_clk_evn, _SOBY_A15n_new);
    /* p08.TAZY*/ wire _TAZY_A15p_new = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _SEPY_A15p_ABxxxxxx_clknew);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n_new = nand2(_TAZY_A15p_new, rstdbg.RYCA_MODE_DBG2n_ext());
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n_new = nor2 (_TAZY_A15p_new, rstdbg.UNOR_MODE_DBG2p());
    ext_bus.PIN_EXT_A[15].pin_out(_SUZE_PIN_EXT_A15n_new, _RULO_PIN_EXT_A15n_new);
  }();

  //----------------------------------------
  // CPU data bus to data pins

  [this, BUS_CPU_D, TEDO_CPU_RDp, TEXO_ADDR_VRAMn_ext](){
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /* p08.REDU*/ wire _REDU_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
    /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
    /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp.qp_new(), _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
    /* p08.RORU*/ wire _RORU_CBD_TO_EPDn_new = mux2p(rstdbg.UNOR_MODE_DBG2p(), _REDU_CPU_RDn_ext, _MOTY_CPU_EXT_RD_ext);
    /* p08.LULA*/ wire _LULA_CBD_TO_EPDp_new = not1(_RORU_CBD_TO_EPDn_new);

    /* p25.RUXA*/ wire _RUXA_new = nand2(BUS_CPU_D[0], _LULA_CBD_TO_EPDp_new);
    /* p25.RUJA*/ wire _RUJA_new = nand2(BUS_CPU_D[1], _LULA_CBD_TO_EPDp_new);
    /* p25.RABY*/ wire _RABY_new = nand2(BUS_CPU_D[2], _LULA_CBD_TO_EPDp_new);
    /* p25.RERA*/ wire _RERA_new = nand2(BUS_CPU_D[3], _LULA_CBD_TO_EPDp_new);
    /* p25.RORY*/ wire _RORY_new = nand2(BUS_CPU_D[4], _LULA_CBD_TO_EPDp_new);
    /* p25.RYVO*/ wire _RYVO_new = nand2(BUS_CPU_D[5], _LULA_CBD_TO_EPDp_new);
    /* p25.RAFY*/ wire _RAFY_new = nand2(BUS_CPU_D[6], _LULA_CBD_TO_EPDp_new);
    /* p25.RAVU*/ wire _RAVU_new = nand2(BUS_CPU_D[7], _LULA_CBD_TO_EPDp_new);

    /* p08.RUNE*/ wire _RUNE_new = nor2 (BUS_CPU_D[0], _RORU_CBD_TO_EPDn_new);
    /* p08.RYPU*/ wire _RYPU_new = nor2 (BUS_CPU_D[1], _RORU_CBD_TO_EPDn_new);
    /* p08.SULY*/ wire _SULY_new = nor2 (BUS_CPU_D[2], _RORU_CBD_TO_EPDn_new);
    /* p08.SEZE*/ wire _SEZE_new = nor2 (BUS_CPU_D[3], _RORU_CBD_TO_EPDn_new);
    /* p08.RESY*/ wire _RESY_new = nor2 (BUS_CPU_D[4], _RORU_CBD_TO_EPDn_new);
    /* p08.TAMU*/ wire _TAMU_new = nor2 (BUS_CPU_D[5], _RORU_CBD_TO_EPDn_new);
    /* p08.ROGY*/ wire _ROGY_new = nor2 (BUS_CPU_D[6], _RORU_CBD_TO_EPDn_new);
    /* p08.RYDA*/ wire _RYDA_new = nor2 (BUS_CPU_D[7], _RORU_CBD_TO_EPDn_new);

    ext_bus.PIN_EXT_D[0].pin_out(_LULA_CBD_TO_EPDp_new, _RUXA_new, _RUNE_new);
    ext_bus.PIN_EXT_D[1].pin_out(_LULA_CBD_TO_EPDp_new, _RUJA_new, _RYPU_new);
    ext_bus.PIN_EXT_D[2].pin_out(_LULA_CBD_TO_EPDp_new, _RABY_new, _SULY_new);
    ext_bus.PIN_EXT_D[3].pin_out(_LULA_CBD_TO_EPDp_new, _RERA_new, _SEZE_new);
    ext_bus.PIN_EXT_D[4].pin_out(_LULA_CBD_TO_EPDp_new, _RORY_new, _RESY_new);
    ext_bus.PIN_EXT_D[5].pin_out(_LULA_CBD_TO_EPDp_new, _RYVO_new, _TAMU_new);
    ext_bus.PIN_EXT_D[6].pin_out(_LULA_CBD_TO_EPDp_new, _RAFY_new, _ROGY_new);
    ext_bus.PIN_EXT_D[7].pin_out(_LULA_CBD_TO_EPDp_new, _RAVU_new, _RYDA_new);
  }();

  //----------------------------------------
  // Pins

  [this, TEXO_ADDR_VRAMn_ext](){
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
    /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
    /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp.qp_new(), _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
    /* p08.TYMU*/ wire _TYMU_EXT_RDn_new = nor2(dma.LUMA_DMA_CARTp_new(), _MOTY_CPU_EXT_RD_ext);
    /* p08.UGAC*/ wire _UGAC_RD_A_new = nand2(_TYMU_EXT_RDn_new, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.URUN*/ wire _URUN_RD_D_new = nor2 (_TYMU_EXT_RDn_new, rstdbg.UNOR_MODE_DBG2p());
    ext_bus.PIN_EXT_RDn.pin_out(_UGAC_RD_A_new, _URUN_RD_D_new);
  }();

  [this, TEXO_ADDR_VRAMn_ext, APOV_CPU_WRp_clkevn](){
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
    /* p08.MEXO*/ wire _MEXO_CPU_WRn_new = not1(APOV_CPU_WRp_clkevn);
    /* p08.NEVY*/ wire _NEVY_new = or2(_MEXO_CPU_WRn_new, _MOCA_DBG_EXT_RD_ext);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn_new = or2(_NEVY_new, dma.LUMA_DMA_CARTp_new());
    /* p08.UVER*/ wire _UVER_WR_A_new = nand2(_PUVA_EXT_WRn_new, rstdbg.TOVA_MODE_DBG2n_ext());
    /* p08.USUF*/ wire _USUF_WR_D_new = nor2 (_PUVA_EXT_WRn_new, rstdbg.UNOR_MODE_DBG2p());
    ext_bus.PIN_EXT_WRn.pin_out(_UVER_WR_A_new, _USUF_WR_D_new);
  }();

  [this, BUS_CPU_A, ABUZ_xxCDEFGH_clk_evn](){
    /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk = and3(ABUZ_xxCDEFGH_clk_evn, TYNU_A000_FFFFp_ext(BUS_CPU_A), TUNA_0000_FDFF_ext(BUS_CPU_A));
    /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk);
    ext_bus.PIN_EXT_CSn.pin_out(_TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew);
  }();

  //----------------------------------------
  // FIXME - implement MBC1

  // 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
  // 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
  // A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

  // 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
  // 2000-3FFF - ROM Bank Number (Write Only) Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also. That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
  // But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.
  // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
  // 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

  // MBC1_RAM_EN

  // MBC1_BANK_D0
  // MBC1_BANK_D1
  // MBC1_BANK_D2
  // MBC1_BANK_D3
  // MBC1_BANK_D4
  // MBC1_BANK_D5
  // MBC1_BANK_D6

  // MBC1_MODE

  /*
  {

    bool bank_0 = nor(MBC1_BANK_D0, MBC1_BANK_D1, MBC1_BANK_D2, MBC1_BANK_D3, MBC1_BANK_D4);

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp_new();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp_new();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp_new();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp_new();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp_new();
    wire cart_rom_a19 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp_new();
    wire cart_rom_a20 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp_new();

    wire cart_ram_a13 = MBC1_MODE.qp_new() ? MBC1_BANK_D5.qp_new() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp_new() ? MBC1_BANK_D6.qp_new() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = PIN_EXT_RDn.qp_new();
      wire CEn = PIN_EXT_A[15].qp_new();

      if (!CEn && !OEn) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */

  //----------------------------------------
  // The actual "cart" stuff.

  [this](){
    uint16_t ext_addr_latch = pack_u16p(16, &ext_bus.PIN_EXT_A[ 0]);
    uint8_t ext_data_latch = pack_u8p(8, &ext_bus.PIN_EXT_D[0]);

    // ROM read
    uint16_t rom_addr = ext_addr_latch & 0x7FFF;
    wire rom_CEn_new = ext_bus.PIN_EXT_A[15].qp_new();
    wire rom_OEp_new = !rom_CEn_new && !ext_bus.PIN_EXT_RDn.qp_new() && cart_buf;
    if (rom_OEp_new)  ext_data_latch = cart_buf[rom_addr];

    // Ext RAM read
    uint16_t eram_addr = (ext_addr_latch & 0x1FFF);
    wire eram_CE1n_new = ext_bus.PIN_EXT_CSn.qp_new();
    wire eram_CE2_new  = ext_bus.PIN_EXT_A[14].qp_new();
    wire eram_WRp_new  = !eram_CE1n_new && eram_CE2_new && !ext_bus.PIN_EXT_WRn.qp_new();
    wire eram_OEp_new  = !eram_CE1n_new && eram_CE2_new && !eram_WRp_new && !ext_bus.PIN_EXT_RDn.qp_new();
    if (eram_WRp_new) ext_ram[eram_addr]  = ext_data_latch;
    if (eram_OEp_new) ext_data_latch = ext_ram[eram_addr];

    // Cart RAM read
    uint16_t cram_addr = (ext_addr_latch & 0x1FFF);
    wire cram_CS1n_new = ext_bus.PIN_EXT_CSn.qp_new();
    wire cram_CS2_new  = ext_bus.PIN_EXT_A[13].qp_new() && !ext_bus.PIN_EXT_A[14].qp_new() && ext_bus.PIN_EXT_A[15].qp_new();
    wire cram_WRp_new  = !cram_CS1n_new && cram_CS2_new && !ext_bus.PIN_EXT_WRn.qp_new();
    wire cram_OEp_new  = !cram_CS1n_new && cram_CS2_new && !ext_bus.PIN_EXT_RDn.qp_new();

    if (cram_WRp_new) cart_ram[cram_addr] = ext_data_latch;
    if (cram_OEp_new) ext_data_latch = cart_ram[cram_addr];

    wire ext_OEp = rom_OEp_new || eram_OEp_new || cram_OEp_new;
    ext_bus.PIN_EXT_D[0].pin_in(ext_OEp, wire(ext_data_latch & 0x01));
    ext_bus.PIN_EXT_D[1].pin_in(ext_OEp, wire(ext_data_latch & 0x02));
    ext_bus.PIN_EXT_D[2].pin_in(ext_OEp, wire(ext_data_latch & 0x04));
    ext_bus.PIN_EXT_D[3].pin_in(ext_OEp, wire(ext_data_latch & 0x08));
    ext_bus.PIN_EXT_D[4].pin_in(ext_OEp, wire(ext_data_latch & 0x10));
    ext_bus.PIN_EXT_D[5].pin_in(ext_OEp, wire(ext_data_latch & 0x20));
    ext_bus.PIN_EXT_D[6].pin_in(ext_OEp, wire(ext_data_latch & 0x40));
    ext_bus.PIN_EXT_D[7].pin_in(ext_OEp, wire(ext_data_latch & 0x80));
  }();

  //----------------------------------------
  // Data pins to data latch, and back to cpu bus.

  [this, TEXO_ADDR_VRAMn_ext](){
    // External data bus -> latch -> CPU data bus
    /* p08.LAVO*/ wire _LAVO_HOLDn_new = nand3(PIN_CPU_RDp.qp_new(), TEXO_ADDR_VRAMn_ext, PIN_CPU_LATCH_EXT.qp_new());
    /*#p08.SOMA*/ ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[0].qn_new());
    /* p08.RONY*/ ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[1].qn_new());
    /* p08.RAXY*/ ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[2].qn_new());
    /* p08.SELO*/ ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[3].qn_new());
    /* p08.SODY*/ ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[4].qn_new());
    /* p08.SAGO*/ ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[5].qn_new());
    /* p08.RUPA*/ ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[6].qn_new());
    /* p08.SAZY*/ ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn_new, ext_bus.PIN_EXT_D[7].qn_new());

    /*#p08.RYMA*/ BUS_CPU_D_out[0].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
    /* p08.RUVO*/ BUS_CPU_D_out[1].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
    /* p08.RYKO*/ BUS_CPU_D_out[2].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
    /* p08.TAVO*/ BUS_CPU_D_out[3].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
    /* p08.TEPE*/ BUS_CPU_D_out[4].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
    /* p08.SAFO*/ BUS_CPU_D_out[5].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
    /* p08.SEVU*/ BUS_CPU_D_out[6].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
    /* p08.TAJU*/ BUS_CPU_D_out[7].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());
  }();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// VRAM interface

void GateBoy::tock_vram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],

    wire AVOR_SYS_RSTp,
    wire XODO_VID_RSTp,

    wire ATAL_xBxDxFxH,
    wire ABUZ_xxCDEFGH,

    wire CATY_LATCH_EXTp,
    wire SOSE_ADDR_VRAMp,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire APOV_CPU_WRp,

    wire TUTO_DBG_VRAMp,

    wire ATEJ_LINE_RSTp,
    wire TEVO_FETCH_TRIGp,
    wire NYXU_BFETCH_RSTn,
    wire PARU_VBLANKp,
    wire WYMO_LCDC_WINENn,
    wire XYMU_RENDERINGp)
{
  // I don't think I need this.
  /*
  BUS_VRAM_Dp[0].reset();
  BUS_VRAM_Dp[1].reset();
  BUS_VRAM_Dp[2].reset();
  BUS_VRAM_Dp[3].reset();
  BUS_VRAM_Dp[4].reset();
  BUS_VRAM_Dp[5].reset();
  BUS_VRAM_Dp[6].reset();
  BUS_VRAM_Dp[7].reset();
  */
  // i don't think i need this.
  /*
  PIN_VRAM_Dp[0].reset();
  PIN_VRAM_Dp[1].reset();
  PIN_VRAM_Dp[2].reset();
  PIN_VRAM_Dp[3].reset();
  PIN_VRAM_Dp[4].reset();
  PIN_VRAM_Dp[5].reset();
  PIN_VRAM_Dp[6].reset();
  PIN_VRAM_Dp[7].reset();
  */

  //----------------------------------------
  // Debug stuff, disabled.

  /*#p25.SERE*/ bool SERE_CPU_VRAM_RDp_new;
  /*#p25.SALE*/ bool SALE_CPU_VRAM_WRn_new;

  [
    this,
    SOSE_ADDR_VRAMp,
    ABUZ_xxCDEFGH,
    XYMU_RENDERINGp,

    &SERE_CPU_VRAM_RDp_new,
    &SALE_CPU_VRAM_WRn_new
  ](){
    // Ignoring debug for now
    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_new =  and2(SOSE_ADDR_VRAMp, ABUZ_xxCDEFGH);
    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_new = nand2(SOSE_ADDR_VRAMp, PIN_CPU_WRp.qp_new());  // Schematic wrong, second input is PIN_CPU_WRp

    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn_new());

    /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp_ext = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_new /*)*/;
    /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_new = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/       _TEGU_CPU_VRAM_WRn_new /*)*/;

    /*#p25.ROPY*/ wire _ROPY_RENDERINGn_new = not1(XYMU_RENDERINGp);
    /*#p25.SERE*/ wire _SERE_CPU_VRAM_RDp_new = and2(_TOLE_CPU_VRAM_RDp_ext, _ROPY_RENDERINGn_new);

    SERE_CPU_VRAM_RDp_new = _SERE_CPU_VRAM_RDp_new;
    SALE_CPU_VRAM_WRn_new = _SALE_CPU_VRAM_WRn_new;
  }();

  //----------------------------------------
  // CPU VRAM address generator

  vram_bus.cpu_addr_to_vram_addr(BUS_CPU_A, XYMU_RENDERINGp, dma.LUFA_DMA_VRAMp_new());

  //----------------------------------------
  // DMA VRAM address generator

  [this](){
    /* p04.NAKY*/ wire _NAKY_DMA_A00p_new = dma.NAKY_DMA_A00p_evn.qp_new();
    /* p04.PYRO*/ wire _PYRO_DMA_A01p_new = dma.PYRO_DMA_A01p_evn.qp_new();
    /* p04.NEFY*/ wire _NEFY_DMA_A02p_new = dma.NEFY_DMA_A02p_evn.qp_new();
    /* p04.MUTY*/ wire _MUTY_DMA_A03p_new = dma.MUTY_DMA_A03p_evn.qp_new();
    /* p04.NYKO*/ wire _NYKO_DMA_A04p_new = dma.NYKO_DMA_A04p_evn.qp_new();
    /* p04.PYLO*/ wire _PYLO_DMA_A05p_new = dma.PYLO_DMA_A05p_evn.qp_new();
    /* p04.NUTO*/ wire _NUTO_DMA_A06p_new = dma.NUTO_DMA_A06p_evn.qp_new();
    /* p04.MUGU*/ wire _MUGU_DMA_A07p_new = dma.MUGU_DMA_A07p_evn.qp_new();
    /* p04.NAFA*/ wire _NAFA_DMA_A08p_new = dma.NAFA_DMA_A08n_h.qn_new();
    /* p04.PYNE*/ wire _PYNE_DMA_A09p_new = dma.PYNE_DMA_A09n_h.qn_new();
    /* p04.PARA*/ wire _PARA_DMA_A10p_new = dma.PARA_DMA_A10n_h.qn_new();
    /* p04.NYDO*/ wire _NYDO_DMA_A11p_new = dma.NYDO_DMA_A11n_h.qn_new();
    /* p04.NYGY*/ wire _NYGY_DMA_A12p_new = dma.NYGY_DMA_A12n_h.qn_new();

    /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_new = not1(dma.LUFA_DMA_VRAMp_new());
    /* p04.ECAL*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_AHOC_DMA_VRAMn_new, _NAKY_DMA_A00p_new);
    /* p04.EGEZ*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_AHOC_DMA_VRAMn_new, _PYRO_DMA_A01p_new);
    /* p04.FUHE*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_AHOC_DMA_VRAMn_new, _NEFY_DMA_A02p_new);
    /* p04.FYZY*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_AHOC_DMA_VRAMn_new, _MUTY_DMA_A03p_new);
    /* p04.DAMU*/ vram_bus.BUS_VRAM_An[ 4].tri6_nn(_AHOC_DMA_VRAMn_new, _NYKO_DMA_A04p_new);
    /* p04.DAVA*/ vram_bus.BUS_VRAM_An[ 5].tri6_nn(_AHOC_DMA_VRAMn_new, _PYLO_DMA_A05p_new);
    /* p04.ETEG*/ vram_bus.BUS_VRAM_An[ 6].tri6_nn(_AHOC_DMA_VRAMn_new, _NUTO_DMA_A06p_new);
    /*#p04.EREW*/ vram_bus.BUS_VRAM_An[ 7].tri6_nn(_AHOC_DMA_VRAMn_new, _MUGU_DMA_A07p_new);
    /*#p04.EVAX*/ vram_bus.BUS_VRAM_An[ 8].tri6_nn(_AHOC_DMA_VRAMn_new, _NAFA_DMA_A08p_new);
    /* p04.DUVE*/ vram_bus.BUS_VRAM_An[ 9].tri6_nn(_AHOC_DMA_VRAMn_new, _PYNE_DMA_A09p_new);
    /* p04.ERAF*/ vram_bus.BUS_VRAM_An[10].tri6_nn(_AHOC_DMA_VRAMn_new, _PARA_DMA_A10p_new);
    /* p04.FUSY*/ vram_bus.BUS_VRAM_An[11].tri6_nn(_AHOC_DMA_VRAMn_new, _NYDO_DMA_A11p_new);
    /* p04.EXYF*/ vram_bus.BUS_VRAM_An[12].tri6_nn(_AHOC_DMA_VRAMn_new, _NYGY_DMA_A12p_new);
  }();

  //----------------------------------------
  // SCX/SCY read/write

  [
    this,
    BUS_CPU_A,
    BUS_CPU_D,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  ](){
    // FF42 SCY, FF43 SCX

    /* p23.ANYP*/ wire _ANYP_FF42_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XARO_FF42p_ext(BUS_CPU_A));
    /* p23.BEDY*/ wire _BEDY_FF42_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XARO_FF42p_ext(BUS_CPU_A));

    /* p23.AVOG*/ wire _AVOG_FF43_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XAVY_FF43p_ext(BUS_CPU_A));
    /* p23.ARUR*/ wire _ARUR_FF43_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XAVY_FF43p_ext(BUS_CPU_A));

    /* FF42 SCY */
    /* p23.CAVO*/ wire _CAVO_FF42_WRn_clk = not1(_BEDY_FF42_WRp_clk);
    /* p23.GAVE*/ reg_scy.GAVE_SCY0n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[0]);
    /* p23.FYMO*/ reg_scy.FYMO_SCY1n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[1]);
    /* p23.FEZU*/ reg_scy.FEZU_SCY2n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[2]);
    /* p23.FUJO*/ reg_scy.FUJO_SCY3n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[3]);
    /* p23.DEDE*/ reg_scy.DEDE_SCY4n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[4]);
    /* p23.FOTY*/ reg_scy.FOTY_SCY5n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[5]);
    /* p23.FOHA*/ reg_scy.FOHA_SCY6n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[6]);
    /* p23.FUNY*/ reg_scy.FUNY_SCY7n_h.dff9(_CAVO_FF42_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[7]);

    // FF43 SCX
    /* p23.AMUN*/ wire _AMUN_FF43_WRn_clk = not1(_ARUR_FF43_WRp_clk);
    /* p23.DATY*/ reg_scx.DATY_SCX0n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[0]);
    /* p23.DUZU*/ reg_scx.DUZU_SCX1n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[1]);
    /* p23.CYXU*/ reg_scx.CYXU_SCX2n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[2]);
    /* p23.GUBO*/ reg_scx.GUBO_SCX3n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[3]);
    /* p23.BEMY*/ reg_scx.BEMY_SCX4n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[4]);
    /* p23.CUZY*/ reg_scx.CUZY_SCX5n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[5]);
    /* p23.CABU*/ reg_scx.CABU_SCX6n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[6]);
    /* p23.BAKE*/ reg_scx.BAKE_SCX7n_h.dff9(_AMUN_FF43_WRn_clk, rstdbg.CUNU_SYS_RSTn(), BUS_CPU_D[7]);

    /* FF42 SCY */
    /* p23.BUWY*/ wire _BUWY_FF42_RDn_ext = not1(_ANYP_FF42_RDp_ext);
    /*#p23.WARE*/ BUS_CPU_D_out[0].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.GAVE_SCY0n_h.qp_new());
    /* p23.GOBA*/ BUS_CPU_D_out[1].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FYMO_SCY1n_h.qp_new());
    /* p23.GONU*/ BUS_CPU_D_out[2].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FEZU_SCY2n_h.qp_new());
    /* p23.GODO*/ BUS_CPU_D_out[3].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FUJO_SCY3n_h.qp_new());
    /* p23.CUSA*/ BUS_CPU_D_out[4].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.DEDE_SCY4n_h.qp_new());
    /* p23.GYZO*/ BUS_CPU_D_out[5].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FOTY_SCY5n_h.qp_new());
    /* p23.GUNE*/ BUS_CPU_D_out[6].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FOHA_SCY6n_h.qp_new());
    /* p23.GYZA*/ BUS_CPU_D_out[7].tri6_nn(_BUWY_FF42_RDn_ext, reg_scy.FUNY_SCY7n_h.qp_new());

    // FF43 SCX
    /* p23.BEBA*/ wire _BEBA_FF43_RDn_ext = not1(_AVOG_FF43_RDp_ext);
    /*#p23.EDOS*/ BUS_CPU_D_out[0].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.DATY_SCX0n_h.qp_new());
    /* p23.EKOB*/ BUS_CPU_D_out[1].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.DUZU_SCX1n_h.qp_new());
    /* p23.CUGA*/ BUS_CPU_D_out[2].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.CYXU_SCX2n_h.qp_new());
    /* p23.WONY*/ BUS_CPU_D_out[3].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.GUBO_SCX3n_h.qp_new());
    /* p23.CEDU*/ BUS_CPU_D_out[4].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.BEMY_SCX4n_h.qp_new());
    /* p23.CATA*/ BUS_CPU_D_out[5].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.CUZY_SCX5n_h.qp_new());
    /* p23.DOXE*/ BUS_CPU_D_out[6].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.CABU_SCX6n_h.qp_new());
    /* p23.CASY*/ BUS_CPU_D_out[7].tri6_nn(_BEBA_FF43_RDn_ext, reg_scx.BAKE_SCX7n_h.qp_new());
  }();

  //----------------------------------------
  // Background/window map and tile adddress generator

  [
    this,
    XODO_VID_RSTp,

    ATEJ_LINE_RSTp,
    TEVO_FETCH_TRIGp,
    WYMO_LCDC_WINENn,
    PARU_VBLANKp
  ]() {

    // X/Y scroll adder
#pragma warning(push)
#pragma warning(disable : 4189)

    /*#p26.ATAD*/ wire _ATAD_TILE_X0S_new = add_s(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n_h.qn_new(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C_new = add_c(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n_h.qn_new(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S_new = add_s(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n_h.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C_new = add_c(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n_h.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2S_new = add_s(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n_h.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2C_new = add_c(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n_h.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0S_new  = add_s(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n_h.qn_new(), _APYH_TILE_X2C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0C_new  = add_c(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n_h.qn_new(), _APYH_TILE_X2C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S_new  = add_s(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n_h.qn_new(), _BABE_MAP_X0C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C_new  = add_c(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n_h.qn_new(), _BABE_MAP_X0C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S_new  = add_s(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n_h.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C_new  = add_c(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n_h.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S_new  = add_s(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n_h.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C_new  = add_c(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n_h.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S_new  = add_s(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n_h.qn_new(), _BYCA_MAP_X3C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C_new  = add_c(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n_h.qn_new(), _BYCA_MAP_X3C_new);

    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S_new = add_s(reg_ly.MUWY_LY0p_evn.qp_new(), reg_scy.GAVE_SCY0n_h.qn_new(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C_new = add_c(reg_ly.MUWY_LY0p_evn.qp_new(), reg_scy.GAVE_SCY0n_h.qn_new(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S_new = add_s(reg_ly.MYRO_LY1p_evn.qp_new(), reg_scy.FYMO_SCY1n_h.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C_new = add_c(reg_ly.MYRO_LY1p_evn.qp_new(), reg_scy.FYMO_SCY1n_h.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S_new = add_s(reg_ly.LEXA_LY2p_evn.qp_new(), reg_scy.FEZU_SCY2n_h.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C_new = add_c(reg_ly.LEXA_LY2p_evn.qp_new(), reg_scy.FEZU_SCY2n_h.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S_new  = add_s(reg_ly.LYDO_LY3p_evn.qp_new(), reg_scy.FUJO_SCY3n_h.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C_new  = add_c(reg_ly.LYDO_LY3p_evn.qp_new(), reg_scy.FUJO_SCY3n_h.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S_new  = add_s(reg_ly.LOVU_LY4p_evn.qp_new(), reg_scy.DEDE_SCY4n_h.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C_new  = add_c(reg_ly.LOVU_LY4p_evn.qp_new(), reg_scy.DEDE_SCY4n_h.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2S_new  = add_s(reg_ly.LEMA_LY5p_evn.qp_new(), reg_scy.FOTY_SCY5n_h.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2C_new  = add_c(reg_ly.LEMA_LY5p_evn.qp_new(), reg_scy.FOTY_SCY5n_h.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S_new  = add_s(reg_ly.MATO_LY6p_evn.qp_new(), reg_scy.FOHA_SCY6n_h.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C_new  = add_c(reg_ly.MATO_LY6p_evn.qp_new(), reg_scy.FOHA_SCY6n_h.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S_new  = add_s(reg_ly.LAFO_LY7p_evn.qp_new(), reg_scy.FUNY_SCY7n_h.qn_new(), _EFYK_MAP_Y3C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C_new  = add_c(reg_ly.LAFO_LY7p_evn.qp_new(), reg_scy.FUNY_SCY7n_h.qn_new(), _EFYK_MAP_Y3C_new);
#pragma warning(pop)

    // Window map coordinate counter
    [
      this,
      XODO_VID_RSTp,
      ATEJ_LINE_RSTp,
      TEVO_FETCH_TRIGp,
      PARU_VBLANKp,
      WYMO_LCDC_WINENn
    ](){
      /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_new = not1(ATEJ_LINE_RSTp);

      /* p27.VETU*/ wire _VETU_WIN_MAP_clknew = and2(TEVO_FETCH_TRIGp, win_reg.PORE_WIN_MODEp());
      /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new   = nand3(WYMO_LCDC_WINENn, _XAHY_LINE_TRIGn_new, rstdbg.XAPO_VID_RSTn());
      /* p27.XACO*/ wire _XACO_WIN_RSTn_new = not1(_XOFO_WIN_RSTp_new);
      /* p27.WYKA*/ win_map_x.WYKA_WIN_X3_evn.dff17(_VETU_WIN_MAP_clknew,               _XACO_WIN_RSTn_new, win_map_x.WYKA_WIN_X3_evn.qn_any());
      /* p27.WODY*/ win_map_x.WODY_WIN_X4_evn.dff17(win_map_x.WYKA_WIN_X3_evn.qn_new(), _XACO_WIN_RSTn_new, win_map_x.WODY_WIN_X4_evn.qn_any());
      /* p27.WOBO*/ win_map_x.WOBO_WIN_X5_evn.dff17(win_map_x.WODY_WIN_X4_evn.qn_new(), _XACO_WIN_RSTn_new, win_map_x.WOBO_WIN_X5_evn.qn_any());
      /* p27.WYKO*/ win_map_x.WYKO_WIN_X6_evn.dff17(win_map_x.WOBO_WIN_X5_evn.qn_new(), _XACO_WIN_RSTn_new, win_map_x.WYKO_WIN_X6_evn.qn_any());
      /* p27.XOLO*/ win_map_x.XOLO_WIN_X7_evn.dff17(win_map_x.WYKO_WIN_X6_evn.qn_new(), _XACO_WIN_RSTn_new, win_map_x.XOLO_WIN_X7_evn.qn_any());

      // Every time we leave win mode we increment win_y
      /* p27.WAZY*/ wire _WAZY_WIN_Y_CLKp = not1(win_reg.PORE_WIN_MODEp());
      /* p27.REPU*/ wire _REPU_VBLANKp_new = or2(PARU_VBLANKp, rstdbg.PYRY_VID_RSTp());
      /* p27.SYNY*/ wire _SYNY_VBLANKn_new = not1(_REPU_VBLANKp_new);
      /* p27.VYNO*/ win_line_y.VYNO_WIN_Y0_evn.dff17(_WAZY_WIN_Y_CLKp,                    _SYNY_VBLANKn_new, win_line_y.VYNO_WIN_Y0_evn.qn_any());
      /* p27.VUJO*/ win_line_y.VUJO_WIN_Y1_evn.dff17(win_line_y.VYNO_WIN_Y0_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.VUJO_WIN_Y1_evn.qn_any());
      /* p27.VYMU*/ win_line_y.VYMU_WIN_Y2_evn.dff17(win_line_y.VUJO_WIN_Y1_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.VYMU_WIN_Y2_evn.qn_any());
      /* p27.TUFU*/ win_line_y.TUFU_WIN_Y3_evn.dff17(win_line_y.VYMU_WIN_Y2_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.TUFU_WIN_Y3_evn.qn_any());
      /* p27.TAXA*/ win_line_y.TAXA_WIN_Y4_evn.dff17(win_line_y.TUFU_WIN_Y3_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.TAXA_WIN_Y4_evn.qn_any());
      /* p27.TOZO*/ win_line_y.TOZO_WIN_Y5_evn.dff17(win_line_y.TAXA_WIN_Y4_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.TOZO_WIN_Y5_evn.qn_any());
      /* p27.TATE*/ win_line_y.TATE_WIN_Y6_evn.dff17(win_line_y.TOZO_WIN_Y5_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.TATE_WIN_Y6_evn.qn_any());
      /* p27.TEKE*/ win_line_y.TEKE_WIN_Y7_evn.dff17(win_line_y.TATE_WIN_Y6_evn.qn_new(), _SYNY_VBLANKn_new, win_line_y.TEKE_WIN_Y7_evn.qn_any());
    }();

    {
      // BG map read address
      /* p26.ACEN*/ wire _ACEN_BGD_MAP_READp_new = and2(tile_fetcher.POTU_BGW_MAP_READp(), win_reg.AXAD_WIN_MODEn());
      /* p26.BAFY*/ wire _BAFY_BG_MAP_READn_new = not1(_ACEN_BGD_MAP_READp_new);
      /* p26.AXEP*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_BAFY_BG_MAP_READn_new, _BABE_MAP_X0S_new);
      /* p26.AFEB*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_BAFY_BG_MAP_READn_new, _ABOD_MAP_X1S_new);
      /* p26.ALEL*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_BAFY_BG_MAP_READn_new, _BEWY_MAP_X2S_new);
      /* p26.COLY*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_BAFY_BG_MAP_READn_new, _BYCA_MAP_X3S_new);
      /* p26.AJAN*/ vram_bus.BUS_VRAM_An[ 4].tri6_nn(_BAFY_BG_MAP_READn_new, _ACUL_MAP_X4S_new);
      /* p26.DUHO*/ vram_bus.BUS_VRAM_An[ 5].tri6_nn(_BAFY_BG_MAP_READn_new, _ETAM_MAP_Y0S_new);
      /* p26.CASE*/ vram_bus.BUS_VRAM_An[ 6].tri6_nn(_BAFY_BG_MAP_READn_new, _DOTO_MAP_Y1S_new);
      /* p26.CYPO*/ vram_bus.BUS_VRAM_An[ 7].tri6_nn(_BAFY_BG_MAP_READn_new, _DABA_MAP_Y2S_new);
      /* p26.CETA*/ vram_bus.BUS_VRAM_An[ 8].tri6_nn(_BAFY_BG_MAP_READn_new, _EFYK_MAP_Y3S_new);
      /* p26.DAFE*/ vram_bus.BUS_VRAM_An[ 9].tri6_nn(_BAFY_BG_MAP_READn_new, _EJOK_MAP_Y4S_new);
      /*#p26.AMUV*/ vram_bus.BUS_VRAM_An[10].tri6_nn(_BAFY_BG_MAP_READn_new, reg_lcdc.XAFO_LCDC_BGMAPn_h.qn_new());
      /* p26.COVE*/ vram_bus.BUS_VRAM_An[11].tri6_nn(_BAFY_BG_MAP_READn_new, 1);
      /* p26.COXO*/ vram_bus.BUS_VRAM_An[12].tri6_nn(_BAFY_BG_MAP_READn_new, 1);
    }


    {
      // Win map read address
      /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp_new = and2(tile_fetcher.POTU_BGW_MAP_READp(), win_reg.PORE_WIN_MODEp());
      /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn_new = not1(_XEZE_WIN_MAP_READp_new);
      /*#p27.XEJA*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_WUKO_WIN_MAP_READn_new, win_map_x.WYKA_WIN_X3_evn.qp_new());
      /* p27.XAMO*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_WUKO_WIN_MAP_READn_new, win_map_x.WODY_WIN_X4_evn.qp_new());
      /* p27.XAHE*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_WUKO_WIN_MAP_READn_new, win_map_x.WOBO_WIN_X5_evn.qp_new());
      /* p27.XULO*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_WUKO_WIN_MAP_READn_new, win_map_x.WYKO_WIN_X6_evn.qp_new());
      /* p27.WUJU*/ vram_bus.BUS_VRAM_An[ 4].tri6_nn(_WUKO_WIN_MAP_READn_new, win_map_x.XOLO_WIN_X7_evn.qp_new());
      /*#p27.VYTO*/ vram_bus.BUS_VRAM_An[ 5].tri6_nn(_WUKO_WIN_MAP_READn_new, win_line_y.TUFU_WIN_Y3_evn.qp_new());
      /* p27.VEHA*/ vram_bus.BUS_VRAM_An[ 6].tri6_nn(_WUKO_WIN_MAP_READn_new, win_line_y.TAXA_WIN_Y4_evn.qp_new());
      /* p27.VACE*/ vram_bus.BUS_VRAM_An[ 7].tri6_nn(_WUKO_WIN_MAP_READn_new, win_line_y.TOZO_WIN_Y5_evn.qp_new());
      /* p27.VOVO*/ vram_bus.BUS_VRAM_An[ 8].tri6_nn(_WUKO_WIN_MAP_READn_new, win_line_y.TATE_WIN_Y6_evn.qp_new());
      /* p27.VULO*/ vram_bus.BUS_VRAM_An[ 9].tri6_nn(_WUKO_WIN_MAP_READn_new, win_line_y.TEKE_WIN_Y7_evn.qp_new());
      /*#p27.VEVY*/ vram_bus.BUS_VRAM_An[10].tri6_nn(_WUKO_WIN_MAP_READn_new, reg_lcdc.WOKY_LCDC_WINMAPn_h.qn_new());
      /* p27.VEZA*/ vram_bus.BUS_VRAM_An[11].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);
      /* p27.VOGU*/ vram_bus.BUS_VRAM_An[12].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);
    }

    // BG/Win tile read address
    {
      /* p26.ASUL*/ wire _ASUL_BGD_TILE_READp_new =  and2(tile_fetcher.NETA_BGW_TILE_READp(), win_reg.AXAD_WIN_MODEn());
      /* p26.BEJE*/ wire _BEJE_BG_TILE_READn_new = not1(_ASUL_BGD_TILE_READp_new);
      /*#p27.XUHA*/ wire _XUHA_FETCH_HILOp_new = not1(tile_fetcher.NOFU_BFETCH_S2n());

      /*#p26.ASUM*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_BEJE_BG_TILE_READn_new, _XUHA_FETCH_HILOp_new);
      /* p26.EVAD*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_BEJE_BG_TILE_READn_new, _FAFO_TILE_Y0S_new);
      /* p26.DAHU*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_BEJE_BG_TILE_READn_new, _EMUX_TILE_Y1S_new);
      /* p26.DODE*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_BEJE_BG_TILE_READn_new, _ECAB_TILE_Y2S_new);
    }
    {
      /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn_new = nand2(tile_fetcher.NETA_BGW_TILE_READp(), win_reg.PORE_WIN_MODEp());
      /*#p27.XUHA*/ wire _XUHA_FETCH_HILOp_new_evn = not1(tile_fetcher.NOFU_BFETCH_S2n());
      /*#p25.XONU*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_XUCY_WIN_TILE_READn_new, _XUHA_FETCH_HILOp_new_evn);
      /*#p25.WUDO*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_XUCY_WIN_TILE_READn_new, win_line_y.VYNO_WIN_Y0_evn.qp_new());
      /*#p25.WAWE*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_XUCY_WIN_TILE_READn_new, win_line_y.VUJO_WIN_Y1_evn.qp_new());
      /*#p25.WOLU*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_XUCY_WIN_TILE_READn_new, win_line_y.VYMU_WIN_Y2_evn.qp_new());
    }
    {
      /*#p25.VAPY*/ vram_bus.BUS_VRAM_An[ 4].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.RAWU_TILE_DB0p_odd.qp_new());
      /*#p25.SEZU*/ vram_bus.BUS_VRAM_An[ 5].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.POZO_TILE_DB1p_odd.qp_new());
      /*#p25.VEJY*/ vram_bus.BUS_VRAM_An[ 6].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.PYZO_TILE_DB2p_odd.qp_new());
      /*#p25.RUSA*/ vram_bus.BUS_VRAM_An[ 7].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.POXA_TILE_DB3p_odd.qp_new());
      /*#p25.ROHA*/ vram_bus.BUS_VRAM_An[ 8].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.PULO_TILE_DB4p_odd.qp_new());
      /*#p25.RESO*/ vram_bus.BUS_VRAM_An[ 9].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.POJU_TILE_DB5p_odd.qp_new());
      /*#p25.SUVO*/ vram_bus.BUS_VRAM_An[10].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.POWY_TILE_DB6p_odd.qp_new());
      /*#p25.TOBO*/ vram_bus.BUS_VRAM_An[11].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), tile_temp_b.PYJU_TILE_DB7p_odd.qp_new());
    }
    {
      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp_new = nor2(tile_temp_b.PYJU_TILE_DB7p_odd.qp_new(), reg_lcdc.WEXU_LCDC_BGTILEn_h.qn_new());
      /*#p25.VURY*/ vram_bus.BUS_VRAM_An[12].tri6_pn(tile_fetcher.NETA_BGW_TILE_READp(), _VUZA_TILE_BANKp_new);
    }
  }();

  //----------------------------------------
  // Sprite read VRAM address generator

  [
    this,
    XYMU_RENDERINGp
  ](){
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp_new = not1(oam_temp_b.YZOS_OAM_DB6p_evn.qp_new());
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn_new = not1(reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new());

    /*#p29.XUQU*/ wire _XUQU_SPRITE_AB_new = not1(sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qn_new());
    /*#p29.CYVU*/ wire _CYVU_L0_new = xor2(_WUKY_FLIP_Yp_new, sprite_store.SPR_TRI_L[0].qp_new());
    /*#p29.BORE*/ wire _BORE_L1_new = xor2(_WUKY_FLIP_Yp_new, sprite_store.SPR_TRI_L[1].qp_new());
    /*#p29.BUVY*/ wire _BUVY_L2_new = xor2(_WUKY_FLIP_Yp_new, sprite_store.SPR_TRI_L[2].qp_new());
    /*#p29.WAGO*/ wire _WAGO_L3_new = xor2(_WUKY_FLIP_Yp_new, sprite_store.SPR_TRI_L[3].qp_new());
    /*#p29.GEJY*/ wire _GEJY_L3_new = amux2(oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), _FUFO_LCDC_SPSIZEn_new, reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new(), _WAGO_L3_new);

    /* p29.TEPA*/ wire _TEPA_RENDERINGp_new = not1(XYMU_RENDERINGp);
    /* p29.SAKY*/ wire _SAKY_SFETCHn_new = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn_new = or2(_SAKY_SFETCHn_new, _TEPA_RENDERINGp_new); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp_new = not1(_TYSO_SFETCHINGn_new);
    /* p29.ABON*/ wire _ABON_SFETCHINGn_new = not1(_TEXY_SFETCHINGp_new);
    /* p29.ABEM*/ vram_bus.BUS_VRAM_An[ 0].tri6_nn(_ABON_SFETCHINGn_new, _XUQU_SPRITE_AB_new);
    /* p29.BAXE*/ vram_bus.BUS_VRAM_An[ 1].tri6_nn(_ABON_SFETCHINGn_new, _CYVU_L0_new);
    /* p29.ARAS*/ vram_bus.BUS_VRAM_An[ 2].tri6_nn(_ABON_SFETCHINGn_new, _BORE_L1_new);
    /* p29.AGAG*/ vram_bus.BUS_VRAM_An[ 3].tri6_nn(_ABON_SFETCHINGn_new, _BUVY_L2_new);
    /* p29.FAMU*/ vram_bus.BUS_VRAM_An[ 4].tri6_nn(_ABON_SFETCHINGn_new, _GEJY_L3_new);
    /*#p29.FUGY*/ vram_bus.BUS_VRAM_An[ 5].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new());
    /* p29.GAVO*/ vram_bus.BUS_VRAM_An[ 6].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new());
    /* p29.WYGA*/ vram_bus.BUS_VRAM_An[ 7].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XYJU_OAM_DA3p_evn.qp_new());
    /* p29.WUNE*/ vram_bus.BUS_VRAM_An[ 8].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YBOG_OAM_DA4p_evn.qp_new());
    /* p29.GOTU*/ vram_bus.BUS_VRAM_An[ 9].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.WYSO_OAM_DA5p_evn.qp_new());
    /* p29.GEGU*/ vram_bus.BUS_VRAM_An[10].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XOTE_OAM_DA6p_evn.qp_new());
    /* p29.XEHE*/ vram_bus.BUS_VRAM_An[11].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YZAB_OAM_DA7p_evn.qp_new());
    /* p29.DYSO*/ vram_bus.BUS_VRAM_An[12].tri6_nn(_ABON_SFETCHINGn_new, 0);   // sprites always in low half of tile store
  }();

  //----------------------------------------
  // VRAM address bus to VRAM pins

  [this](){
    // PIN_VRAM_A driver
    /* p25.MYFU*/ wire _MYFUp_new = not1(vram_bus.BUS_VRAM_An[ 0].qp_new());
    /* p25.MASA*/ wire _MASAp_new = not1(vram_bus.BUS_VRAM_An[ 1].qp_new());
    /* p25.MYRE*/ wire _MYREp_new = not1(vram_bus.BUS_VRAM_An[ 2].qp_new());
    /* p25.MAVU*/ wire _MAVUp_new = not1(vram_bus.BUS_VRAM_An[ 3].qp_new());
    /* p25.MEPA*/ wire _MEPAp_new = not1(vram_bus.BUS_VRAM_An[ 4].qp_new());
    /* p25.MYSA*/ wire _MYSAp_new = not1(vram_bus.BUS_VRAM_An[ 5].qp_new());
    /* p25.MEWY*/ wire _MEWYp_new = not1(vram_bus.BUS_VRAM_An[ 6].qp_new());
    /* p25.MUME*/ wire _MUMEp_new = not1(vram_bus.BUS_VRAM_An[ 7].qp_new());
    /* p25.VOVA*/ wire _VOVAp_new = not1(vram_bus.BUS_VRAM_An[ 8].qp_new());
    /* p25.VODE*/ wire _VODEp_new = not1(vram_bus.BUS_VRAM_An[ 9].qp_new());
    /* p25.RUKY*/ wire _RUKYp_new = not1(vram_bus.BUS_VRAM_An[10].qp_new());
    /* p25.RUMA*/ wire _RUMAp_new = not1(vram_bus.BUS_VRAM_An[11].qp_new());
    /* p25.REHO*/ wire _REHOp_new = not1(vram_bus.BUS_VRAM_An[12].qp_new());

    /* p25.LEXE*/ wire _LEXEn_new = not1(_MYFUp_new);
    /* p25.LOZU*/ wire _LOZUn_new = not1(_MASAp_new);
    /* p25.LACA*/ wire _LACAn_new = not1(_MYREp_new);
    /* p25.LUVO*/ wire _LUVOn_new = not1(_MAVUp_new);
    /* p25.LOLY*/ wire _LOLYn_new = not1(_MEPAp_new);
    /* p25.LALO*/ wire _LALOn_new = not1(_MYSAp_new);
    /* p25.LEFA*/ wire _LEFAn_new = not1(_MEWYp_new);
    /* p25.LUBY*/ wire _LUBYn_new = not1(_MUMEp_new);
    /* p25.TUJY*/ wire _TUJYn_new = not1(_VOVAp_new);
    /* p25.TAGO*/ wire _TAGOn_new = not1(_VODEp_new);
    /* p25.NUVA*/ wire _NUVAn_new = not1(_RUKYp_new);
    /* p25.PEDU*/ wire _PEDUn_new = not1(_RUMAp_new);
    /* p25.PONY*/ wire _PONYn_new = not1(_REHOp_new);

    vram_bus.PIN_VRAM_Ap[ 0].pin_out(_LEXEn_new, _LEXEn_new);
    vram_bus.PIN_VRAM_Ap[ 1].pin_out(_LOZUn_new, _LOZUn_new);
    vram_bus.PIN_VRAM_Ap[ 2].pin_out(_LACAn_new, _LACAn_new);
    vram_bus.PIN_VRAM_Ap[ 3].pin_out(_LUVOn_new, _LUVOn_new);
    vram_bus.PIN_VRAM_Ap[ 4].pin_out(_LOLYn_new, _LOLYn_new);
    vram_bus.PIN_VRAM_Ap[ 5].pin_out(_LALOn_new, _LALOn_new);
    vram_bus.PIN_VRAM_Ap[ 6].pin_out(_LEFAn_new, _LEFAn_new);
    vram_bus.PIN_VRAM_Ap[ 7].pin_out(_LUBYn_new, _LUBYn_new);
    vram_bus.PIN_VRAM_Ap[ 8].pin_out(_TUJYn_new, _TUJYn_new);
    vram_bus.PIN_VRAM_Ap[ 9].pin_out(_TAGOn_new, _TAGOn_new);
    vram_bus.PIN_VRAM_Ap[10].pin_out(_NUVAn_new, _NUVAn_new);
    vram_bus.PIN_VRAM_Ap[11].pin_out(_PEDUn_new, _PEDUn_new);
    vram_bus.PIN_VRAM_Ap[12].pin_out(_PONYn_new, _PONYn_new);
  }();

  //----------------------------------------
  // CPU data bus to VRAM data pins

  [
    this,
    BUS_CPU_D,
    SALE_CPU_VRAM_WRn_new,
    SERE_CPU_VRAM_RDp_new
  ](){
    /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_new = not1(SALE_CPU_VRAM_WRn_new);
    /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_new = and2(SERE_CPU_VRAM_RDp_new, _RUVY_CPU_VRAM_WRp_new);

    /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_new = not1(_SAZO_CBD_TO_VPDp_new);
    /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_new = not1(_RYJE_CBD_TO_VPDn_new);

    /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp_new = and2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);
    /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_new =  or2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);

    /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn_new = not1(_ROCY_CBD_TO_VPDp_new);
    /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_new = not1(_RELA_CBD_TO_VPDp_new);

    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp_new = not1(_RAHU_CBD_TO_VPDn_new);
    /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp_new = not1(_RENA_CBD_TO_VPDn_new);

    /* p25.TEME*/ vram_bus.BUS_VRAM_Dp[0].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[0]);
    /* p25.TEWU*/ vram_bus.BUS_VRAM_Dp[1].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[1]);
    /*#p25.TYGO*/ vram_bus.BUS_VRAM_Dp[2].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[2]);
    /* p25.SOTE*/ vram_bus.BUS_VRAM_Dp[3].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[3]);
    /* p25.SEKE*/ vram_bus.BUS_VRAM_Dp[4].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[4]);
    /* p25.RUJO*/ vram_bus.BUS_VRAM_Dp[5].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[5]);
    /* p25.TOFA*/ vram_bus.BUS_VRAM_Dp[6].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[6]);
    /* p25.SUZA*/ vram_bus.BUS_VRAM_Dp[7].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[7]);

    /*#p25.SEFA*/ wire _SEFA_D0p_new = and2(vram_bus.BUS_VRAM_Dp[0].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SOGO*/ wire _SOGO_D1p_new = and2(vram_bus.BUS_VRAM_Dp[1].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SEFU*/ wire _SEFU_D2p_new = and2(vram_bus.BUS_VRAM_Dp[2].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUNA*/ wire _SUNA_D3p_new = and2(vram_bus.BUS_VRAM_Dp[3].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUMO*/ wire _SUMO_D4p_new = and2(vram_bus.BUS_VRAM_Dp[4].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SAZU*/ wire _SAZU_D5p_new = and2(vram_bus.BUS_VRAM_Dp[5].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SAMO*/ wire _SAMO_D6p_new = and2(vram_bus.BUS_VRAM_Dp[6].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUKE*/ wire _SUKE_D7p_new = and2(vram_bus.BUS_VRAM_Dp[7].qp_new(), _ROVE_CBD_TO_VPDp_new);

    /*#p25.SYNU*/ wire _SYNU_D0p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[0].qp_new());
    /* p25.SYMA*/ wire _SYMA_D1p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[1].qp_new());
    /* p25.ROKO*/ wire _ROKO_D2p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[2].qp_new());
    /* p25.SYBU*/ wire _SYBU_D3p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[3].qp_new());
    /* p25.SAKO*/ wire _SAKO_D4p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[4].qp_new());
    /* p25.SEJY*/ wire _SEJY_D5p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[5].qp_new());
    /* p25.SEDO*/ wire _SEDO_D6p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[6].qp_new());
    /* p25.SAWU*/ wire _SAWU_D7p_new = or2(_RAHU_CBD_TO_VPDn_new, vram_bus.BUS_VRAM_Dp[7].qp_new());

    /*#p25.REGE*/ wire _REGE_D0n_new = not1(_SEFA_D0p_new);
    /* p25.RYKY*/ wire _RYKY_D1n_new = not1(_SOGO_D1p_new);
    /* p25.RAZO*/ wire _RAZO_D2n_new = not1(_SEFU_D2p_new);
    /* p25.RADA*/ wire _RADA_D3n_new = not1(_SUNA_D3p_new);
    /* p25.RYRO*/ wire _RYRO_D4n_new = not1(_SUMO_D4p_new);
    /* p25.REVU*/ wire _REVU_D5n_new = not1(_SAZU_D5p_new);
    /* p25.REKU*/ wire _REKU_D6n_new = not1(_SAMO_D6p_new);
    /* p25.RYZE*/ wire _RYZE_D7n_new = not1(_SUKE_D7p_new);

    /*#p25.RURA*/ wire _RURA_D0n_new = not1(_SYNU_D0p_new);
    /* p25.RULY*/ wire _RULY_D1n_new = not1(_SYMA_D1p_new);
    /* p25.RARE*/ wire _RARE_D2n_new = not1(_ROKO_D2p_new);
    /* p25.RODU*/ wire _RODU_D3n_new = not1(_SYBU_D3p_new);
    /* p25.RUBE*/ wire _RUBE_D4n_new = not1(_SAKO_D4p_new);
    /* p25.RUMU*/ wire _RUMU_D5n_new = not1(_SEJY_D5p_new);
    /* p25.RYTY*/ wire _RYTY_D6n_new = not1(_SEDO_D6p_new);
    /* p25.RADY*/ wire _RADY_D7n_new = not1(_SAWU_D7p_new);

    vram_bus.PIN_VRAM_Dp[0].pin_out(_ROFA_CBD_TO_VPDp_new, _REGE_D0n_new, _RURA_D0n_new);
    vram_bus.PIN_VRAM_Dp[1].pin_out(_ROFA_CBD_TO_VPDp_new, _RYKY_D1n_new, _RULY_D1n_new);
    vram_bus.PIN_VRAM_Dp[2].pin_out(_ROFA_CBD_TO_VPDp_new, _RAZO_D2n_new, _RARE_D2n_new);
    vram_bus.PIN_VRAM_Dp[3].pin_out(_ROFA_CBD_TO_VPDp_new, _RADA_D3n_new, _RODU_D3n_new);
    vram_bus.PIN_VRAM_Dp[4].pin_out(_ROFA_CBD_TO_VPDp_new, _RYRO_D4n_new, _RUBE_D4n_new);
    vram_bus.PIN_VRAM_Dp[5].pin_out(_ROFA_CBD_TO_VPDp_new, _REVU_D5n_new, _RUMU_D5n_new);
    vram_bus.PIN_VRAM_Dp[6].pin_out(_ROFA_CBD_TO_VPDp_new, _REKU_D6n_new, _RYTY_D6n_new);
    vram_bus.PIN_VRAM_Dp[7].pin_out(_ROFA_CBD_TO_VPDp_new, _RYZE_D7n_new, _RADY_D7n_new);
  }();

  //----------------------------------------
  // PIN_VRAM_CSn

  [
    this,
    APOV_CPU_WRp,
    TUTO_DBG_VRAMp,
    SOSE_ADDR_VRAMp,
    SERE_CPU_VRAM_RDp_new,
    SALE_CPU_VRAM_WRn_new,
    XYMU_RENDERINGp
  ]() {
    /* p29.SAKY*/ wire _SAKY_SFETCHn_new = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGp_new = not1(XYMU_RENDERINGp);
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn_new = or2(_SAKY_SFETCHn_new, _TEPA_RENDERINGp_new); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp_new = not1(_TYSO_SFETCHINGn_new);
    /* p27.LUSU*/ wire _LUSU_FETCHINGn_new = not1(tile_fetcher.LONY_FETCHINGp_xxx.qp_new());
    /* p27.LENA*/ wire _LENA_BFETCHINGp_new = not1(_LUSU_FETCHINGn_new);
    /*#p25.SUTU*/ wire _SUTU_MCSn_new   = nor4(_LENA_BFETCHINGp_new, dma.LUFA_DMA_VRAMp_new(), _TEXY_SFETCHINGp_new, SERE_CPU_VRAM_RDp_new);

    /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(TUTO_DBG_VRAMp);
    /* p25.TODE*/ wire _TODE_MCSn_A_new = and2(_SUTU_MCSn_new, _RACO_DBG_VRAMn_new);
    /* p25.SEWO*/ wire _SEWO_MCSn_D_new =  or2(_SUTU_MCSn_new, TUTO_DBG_VRAMp);
    /* p25.SOKY*/ wire _SOKY_MCSp_A_new = not1(_TODE_MCSn_A_new);
    /* p25.SETY*/ wire _SETY_MCSp_D_new = not1(_SEWO_MCSn_D_new);

    vram_bus.PIN_VRAM_CSn.pin_out(_SOKY_MCSp_A_new, _SETY_MCSp_D_new); // FIXME not actually using this pin, but we should

    /* p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp_ext = and2(SOSE_ADDR_VRAMp, APOV_CPU_WRp);
    /* p25.SUDO*/ wire _SUDO_MWRp_new = not1(/*vram_bus.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /* p25.TYJY*/ wire _TYJY_VRAM_WRp_new = mux2p(TUTO_DBG_VRAMp, _SUDO_MWRp_new, _TUJA_CPU_VRAM_WRp_ext);
    /* p25.SOHY*/ wire _SOHY_MWRn_new   = nand2(_TYJY_VRAM_WRp_new, SERE_CPU_VRAM_RDp_new);

    /* p25.TAXY*/ wire _TAXY_MWRn_A_new = and2(_SOHY_MWRn_new, _RACO_DBG_VRAMn_new);
    /* p25.SOFY*/ wire _SOFY_MWRn_D_new =  or2(_SOHY_MWRn_new, TUTO_DBG_VRAMp);
    /* p25.SYSY*/ wire _SYSY_MWRp_A_new = not1(_TAXY_MWRn_A_new);
    /* p25.RAGU*/ wire _RAGU_MWRp_D_new = not1(_SOFY_MWRn_D_new);

    vram_bus.PIN_VRAM_WRn.pin_out(_SYSY_MWRp_A_new, _RAGU_MWRp_D_new);

    /*#p29.TYFO*/ wire _TYFO_SFETCH_S0p_D1_new = sprite_fetcher.TYFO_SFETCH_S0p_D1_evn.qp_new();
    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_new = not1(sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_new());
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_new = nand2(_TYFO_SFETCH_S0p_D1_new, _TYTU_SFETCH_S0n_new);

    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp_new = and2(_TACU_SPR_SEQ_5_TRIG_new, _TEXY_SFETCHINGp_new);

    /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new_evn = nor2(dma.LUFA_DMA_VRAMp_new(), XYMU_RENDERINGp);
    /* p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn_new = nand2(SALE_CPU_VRAM_WRn_new, _XANE_VRAM_LOCKn_new_evn);
    /* p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn_new = not1(_SOHO_SPR_VRAM_RDp_new);
    /* p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn_new = not1(tile_fetcher.LONY_FETCHINGp_xxx.qp_new());
    /* p25.APAM*/ wire _APAM_DMA_VRAMn_new    = not1(dma.LUFA_DMA_VRAMp_new());
    /* p25.RACU*/ wire _RACU_MOEn_new   = and4(_RYLU_CPU_VRAM_RDn_new, _RAWA_SPR_VRAM_RDn_new, _MYMA_BGW_VRAM_RDn_new, _APAM_DMA_VRAMn_new); // def and

    /* p25.SEMA*/ wire _SEMA_MOEn_A_new = and2(_RACU_MOEn_new, _RACO_DBG_VRAMn_new);
    /* p25.RUTE*/ wire _RUTE_MOEn_D_new =  or2(_RACU_MOEn_new, TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /* p25.REFO*/ wire _REFO_MOEn_A_new = not1(_SEMA_MOEn_A_new);
    /* p25.SAHA*/ wire _SAHA_MOEn_D_new = not1(_RUTE_MOEn_D_new);
    vram_bus.PIN_VRAM_OEn.pin_out(_REFO_MOEn_A_new, _SAHA_MOEn_D_new);
  }();

  //----------------------------------------
  // The actual VRAM

  uint16_t vram_addr_latch;
  uint8_t vram_data_latch;

  [this, &vram_addr_latch, &vram_data_latch](){
    vram_addr_latch = pack_u16p(13, vram_bus.PIN_VRAM_Ap);
    vram_data_latch = pack_u8p (8,  vram_bus.PIN_VRAM_Dp);

    if (!vram_bus.PIN_VRAM_WRn.qp_new()) {
      vid_ram[vram_addr_latch] = vram_data_latch;
    }
    if (!vram_bus.PIN_VRAM_OEn.qp_new()) {
      vram_data_latch = vid_ram[vram_addr_latch];
    }
  }();

  //----------------------------------------
  // VRAM pins to VRAM bus

  [
    this,
    SALE_CPU_VRAM_WRn_new,
    SERE_CPU_VRAM_RDp_new,
    vram_data_latch
  ](){
    /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_new = not1(SALE_CPU_VRAM_WRn_new);
    /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_new = and2(SERE_CPU_VRAM_RDp_new, _RUVY_CPU_VRAM_WRp_new);

    /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_new = not1(_SAZO_CBD_TO_VPDp_new);
    /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_new = not1(_RYJE_CBD_TO_VPDn_new);
    /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_new =  or2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);
    /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_new = not1(_RELA_CBD_TO_VPDp_new);

    vram_bus.PIN_VRAM_Dp[0].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x01);
    vram_bus.PIN_VRAM_Dp[1].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x02);
    vram_bus.PIN_VRAM_Dp[2].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x04);
    vram_bus.PIN_VRAM_Dp[3].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x08);
    vram_bus.PIN_VRAM_Dp[4].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x10);
    vram_bus.PIN_VRAM_Dp[5].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x20);
    vram_bus.PIN_VRAM_Dp[6].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x40);
    vram_bus.PIN_VRAM_Dp[7].pin_in(!vram_bus.PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x80);

    /* p25.RODY*/ vram_bus.BUS_VRAM_Dp[0].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[0].qn_new());
    /* p25.REBA*/ vram_bus.BUS_VRAM_Dp[1].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[1].qn_new());
    /* p25.RYDO*/ vram_bus.BUS_VRAM_Dp[2].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[2].qn_new());
    /* p25.REMO*/ vram_bus.BUS_VRAM_Dp[3].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[3].qn_new());
    /* p25.ROCE*/ vram_bus.BUS_VRAM_Dp[4].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[4].qn_new());
    /* p25.ROPU*/ vram_bus.BUS_VRAM_Dp[5].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[5].qn_new());
    /* p25.RETA*/ vram_bus.BUS_VRAM_Dp[6].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[6].qn_new());
    /* p25.RAKU*/ vram_bus.BUS_VRAM_Dp[7].tri6_pn(_RENA_CBD_TO_VPDn_new, vram_bus.PIN_VRAM_Dp[7].qn_new());
  }();

  //----------------------------------------
  // VRAM bus to CPU bus

  [this, TEDO_CPU_RDp, CATY_LATCH_EXTp, SERE_CPU_VRAM_RDp_new]() {
    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);
    /* p28.MYNU*/ wire _MYNU_CPU_RDn_ext = nand2(_ASOT_CPU_RDp_ext, CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp_ext = not1(_MYNU_CPU_RDn_ext);

    /* p25.RERY*/ wire _RERY_VBUS_D0n_new = not1(vram_bus.BUS_VRAM_Dp[0].qp_new());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n_new = not1(vram_bus.BUS_VRAM_Dp[1].qp_new());
    /* p25.RONA*/ wire _RONA_VBUS_D2n_new = not1(vram_bus.BUS_VRAM_Dp[2].qp_new());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n_new = not1(vram_bus.BUS_VRAM_Dp[3].qp_new());
    /* p25.SANA*/ wire _SANA_VBUS_D4n_new = not1(vram_bus.BUS_VRAM_Dp[4].qp_new());
    /* p25.RORO*/ wire _RORO_VBUS_D5n_new = not1(vram_bus.BUS_VRAM_Dp[5].qp_new());
    /* p25.RABO*/ wire _RABO_VBUS_D6n_new = not1(vram_bus.BUS_VRAM_Dp[6].qp_new());
    /* p25.SAME*/ wire _SAME_VBUS_D7n_new = not1(vram_bus.BUS_VRAM_Dp[7].qp_new());

    /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn_new = nand2(SERE_CPU_VRAM_RDp_new, _LEKO_CPU_RDp_ext);
    /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp_new = not1(_TYVY_VBD_TO_CBDn_new);

    /*#p25.RUGA*/ BUS_CPU_D_out[0].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RERY_VBUS_D0n_new);
    /* p25.ROTA*/ BUS_CPU_D_out[1].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RUNA_VBUS_D1n_new);
    /* p25.RYBU*/ BUS_CPU_D_out[2].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RONA_VBUS_D2n_new);
    /* p25.RAJU*/ BUS_CPU_D_out[3].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RUNO_VBUS_D3n_new);
    /* p25.TYJA*/ BUS_CPU_D_out[4].tri6_pn(_SEBY_VBD_TO_CBDp_new, _SANA_VBUS_D4n_new);
    /* p25.REXU*/ BUS_CPU_D_out[5].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RORO_VBUS_D5n_new);
    /* p25.RUPY*/ BUS_CPU_D_out[6].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RABO_VBUS_D6n_new);
    /* p25.TOKU*/ BUS_CPU_D_out[7].tri6_pn(_SEBY_VBD_TO_CBDp_new, _SAME_VBUS_D7n_new);
  }();
}

//------------------------------------------------------------------------------------------------------------------------
// OAM interface

void GateBoy::tock_oam(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire ATAL_xBxDxFxH,
    wire UVYT_ABCDxxxx,
    wire XOCE_xBCxxFGx,
    wire XYSO_xBCDxFGH,
    wire TAPU_CPU_WRp,
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp,
    wire XYMU_RENDERINGp,
    wire ACYL_SCANNINGp)
{
  //----------------------------------------
  // OAM address bus mux

  [this](){
    // DMA OAM write address driver
    /* p04.DUGA*/ wire _DUGA_DMA_RUNNINGn_new_evn = not1(dma.MATU_DMA_RUNNINGp_evn.qp_new());
    /* p28.FODO*/ oam_bus.BUS_OAM_An[0].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.NAKY_DMA_A00p_evn.qp_new());
    /* p28.FESA*/ oam_bus.BUS_OAM_An[1].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.PYRO_DMA_A01p_evn.qp_new());
    /* p28.FAGO*/ oam_bus.BUS_OAM_An[2].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.NEFY_DMA_A02p_evn.qp_new());
    /* p28.FYKY*/ oam_bus.BUS_OAM_An[3].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.MUTY_DMA_A03p_evn.qp_new());
    /* p28.ELUG*/ oam_bus.BUS_OAM_An[4].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.NYKO_DMA_A04p_evn.qp_new());
    /* p28.EDOL*/ oam_bus.BUS_OAM_An[5].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.PYLO_DMA_A05p_evn.qp_new());
    /* p28.FYDU*/ oam_bus.BUS_OAM_An[6].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.NUTO_DMA_A06p_evn.qp_new());
    /* p28.FETU*/ oam_bus.BUS_OAM_An[7].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma.MUGU_DMA_A07p_evn.qp_new());
  }();

  [this, XYMU_RENDERINGp](){
    wire VCC = 1;
    // OAM address from sprite fetcher
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(dma.MATU_DMA_RUNNINGp_evn.qp_new());
    /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.BETE*/ wire _BETE_SFETCHINGn_new = not1(_AJON_RENDERINGp_new_evn);
    /* p28.GECA*/ oam_bus.BUS_OAM_An[0].tri6_nn(_BETE_SFETCHINGn_new, VCC);
    /* p28.WYDU*/ oam_bus.BUS_OAM_An[1].tri6_nn(_BETE_SFETCHINGn_new, VCC);
    /* p28.GYBU*/ oam_bus.BUS_OAM_An[2].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[0].qp_new());
    /* p28.GYKA*/ oam_bus.BUS_OAM_An[3].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[1].qp_new());
    /* p28.FABY*/ oam_bus.BUS_OAM_An[4].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[2].qp_new());
    /* p28.FACO*/ oam_bus.BUS_OAM_An[5].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[3].qp_new());
    /* p28.FUGU*/ oam_bus.BUS_OAM_An[6].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[4].qp_new());
    /* p28.FYKE*/ oam_bus.BUS_OAM_An[7].tri6_nn(_BETE_SFETCHINGn_new, sprite_store.SPR_TRI_I[5].qp_new());
  }();

  [this, ACYL_SCANNINGp](){
    // OAM address from sprite scanner
    wire GND = 0;
    /* p28.APAR*/ wire _APAR_SCANNINGn_new_evn = not1(ACYL_SCANNINGp);
    /* p28.GEFY*/ oam_bus.BUS_OAM_An[0].tri6_nn(_APAR_SCANNINGn_new_evn, GND);
    /* p28.WUWE*/ oam_bus.BUS_OAM_An[1].tri6_nn(_APAR_SCANNINGn_new_evn, GND);
    /* p28.GUSE*/ oam_bus.BUS_OAM_An[2].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.YFEL_SCAN0_evn.qp_new());
    /* p28.GEMA*/ oam_bus.BUS_OAM_An[3].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.WEWY_SCAN1_evn.qp_new());
    /* p28.FUTO*/ oam_bus.BUS_OAM_An[4].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.GOSO_SCAN2_evn.qp_new());
    /* p28.FAKU*/ oam_bus.BUS_OAM_An[5].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.ELYN_SCAN3_evn.qp_new());
    /* p28.GAMA*/ oam_bus.BUS_OAM_An[6].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.FAHA_SCAN4_evn.qp_new());
    /* p28.GOBY*/ oam_bus.BUS_OAM_An[7].tri6_nn(_APAR_SCANNINGn_new_evn, scan_counter.FONY_SCAN5_evn.qp_new());
  }();

  //----------------------------------------
  // DMA write cart data to oam

  oam_bus.ext_to_bus(
    ext_bus.PIN_EXT_D,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    dma.PULA_DMA_A13n_h.qn_new(),
    dma.POKU_DMA_A14n_h.qn_new(),
    dma.MARU_DMA_A15n_h.qn_new());

  //----------------------------------------
  // DMA write VRAM to oam

  oam_bus.vram_to_bus(
    vram_bus.BUS_VRAM_Dp,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    dma.PULA_DMA_A13n_h.qn_new(),
    dma.POKU_DMA_A14n_h.qn_new(),
    dma.MARU_DMA_A15n_h.qn_new());

  //----------------------------------------
  // CPU write to OAM

  oam_bus.cpu_to_bus(
    BUS_CPU_A,
    BUS_CPU_D,
    UVYT_ABCDxxxx,
    TAPU_CPU_WRp,
    XYMU_RENDERINGp,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    ACYL_SCANNINGp);

  //----------------------------------------

  oam_bus.update_pins(
    BUS_CPU_A,
    UVYT_ABCDxxxx,
    XYSO_xBCDxFGH,
    XOCE_xBCxxFGx,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    CATY_LATCH_EXTp,

    XYMU_RENDERINGp,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    ACYL_SCANNINGp,

    sprite_fetcher.XUJY_OAM_CLKENp(XYMU_RENDERINGp),
    sprite_fetcher.XUJA_SPR_OAM_LATCHn(XYMU_RENDERINGp));

  //----------------------------------------
  // The actual OAM ram

  uint8_t oam_addr_latch   = pack_u8n(7, &oam_bus.BUS_OAM_An[1]);
  uint8_t oam_data_latch_a = pack_u8n(8, &oam_bus.BUS_OAM_DAn[0]);
  uint8_t oam_data_latch_b = pack_u8n(8, &oam_bus.BUS_OAM_DBn[0]);

  if (!oam_clk_old && !oam_bus.PIN_OAM_CLKn.qp_new()) {
    if (!oam_bus.PIN_OAM_WRn_A.qp_new()) oam_ram[(oam_addr_latch << 1) + 0] = oam_data_latch_a;
    if (!oam_bus.PIN_OAM_WRn_B.qp_new()) oam_ram[(oam_addr_latch << 1) + 1] = oam_data_latch_b;
  }

  if (!oam_bus.PIN_OAM_OEn.qp_new()) {
    oam_data_latch_a = oam_ram[(oam_addr_latch << 1) + 0];
    oam_data_latch_b = oam_ram[(oam_addr_latch << 1) + 1];
  }

  //----------------------------------------
  // OAM bus to OAM data latch

  oam_latch_a.latch_bus(
    BUS_CPU_A,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp,
    oam_bus.BUS_OAM_DAn,
    ACYL_SCANNINGp,
    XOCE_xBCxxFGx,
    sprite_fetcher.XUJA_SPR_OAM_LATCHn(XYMU_RENDERINGp),
    oam_data_latch_a);

  oam_latch_b.latch_bus(
    BUS_CPU_A,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp,
    oam_bus.BUS_OAM_DBn,
    ACYL_SCANNINGp,
    XOCE_xBCxxFGx,
    sprite_fetcher.XUJA_SPR_OAM_LATCHn(XYMU_RENDERINGp),
    oam_data_latch_b);

  oam_latch_a.latch_to_cpu(
    BUS_CPU_A,
    oam_bus.BUS_OAM_An,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    BUS_CPU_D_out
  );

  oam_latch_b.latch_to_cpu(
    BUS_CPU_A,
    oam_bus.BUS_OAM_An,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp,
    dma.MATU_DMA_RUNNINGp_evn.qp_new(),
    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    BUS_CPU_D_out
  );
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_zram(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp)
{
  // ZRAM control signals are
  // clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // TEDO_CPU_RDp();
  // TAPU_CPU_WRp_xxxxEFGx()
  // _SYKE_FF00_FFFFp
  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  uint16_t cpu_addr = pack_u16p(16, BUS_CPU_A);

  uint8_t zram_data_latch = pack_u8p(8, BUS_CPU_D);

  wire zram_hit = (cpu_addr >= 0xFF80) && (cpu_addr <= 0xFFFE);

  wire zram_clk_new = nand2(TAPU_CPU_WRp, zram_hit);

  if (!zram_clk_old && zram_clk_new) {
    zero_ram[cpu_addr & 0x007F] = zram_data_latch;
  }
  zram_clk_old = zram_clk_new;

  if (zram_hit && TEDO_CPU_RDp) {
    zram_data_latch = zero_ram[cpu_addr & 0x007F];
  }

  if ((cpu_addr >= 0xFF80) && (cpu_addr <= 0xFFFE)) {
    BUS_CPU_D_out[0].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x01));
    BUS_CPU_D_out[1].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x02));
    BUS_CPU_D_out[2].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x04));
    BUS_CPU_D_out[3].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x08));
    BUS_CPU_D_out[4].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x10));
    BUS_CPU_D_out[5].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x20));
    BUS_CPU_D_out[6].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x40));
    BUS_CPU_D_out[7].tri(TEDO_CPU_RDp, wire(zram_data_latch & 0x80));
  }
}

//------------------------------------------------------------------------------------------------------------------------
// Debug stuff I disabled

#if 0

/* p07.APET*/ wire APET_MODE_DBG = or2(clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/* p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A[ 5], BUS_CPU_A[ 6], TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/* p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/* p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/* p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/* p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/* p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /* p05.ANOC*/ not1(GND),
  /* p05.ATAJ*/ not1(GND),
  /* p05.AJEC*/ not1(GND),
  /* p05.ASUZ*/ not1(GND),
  /* p05.BENU*/ not1(GND),
  /* p05.AKAJ*/ not1(GND),
  /* p05.ARAR*/ not1(GND),
  /* p05.BEDA*/ not1(GND)
);

if (LECO_xBCDEFGH) set_data(
  /* p07.ROMY*/ GND,
  /* p07.RYNE*/ GND,
  /* p07.REJY*/ GND,
  /* p07.RASE*/ GND,
  /* p07.REKA*/ GND,
  /* p07.ROWE*/ GND,
  /* p07.RYKE*/ GND,
  /* p07.RARU*/ GND
);



//----------
// more debug stuff

/* p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.PIN_CPU_BOGA_xBCDEFGH);
/* p25.SOLE*/ wire SOLE = not1(TUSO);

if (VYPO_GND) bus_out.set_data(
  /* p25.TOVU*/ SOLE,
  /* p25.SOSA*/ SOLE,
  /* p25.SEDU*/ SOLE,
  /* p25.TAXO*/ SOLE,
  /* p25.TAHY*/ SOLE,
  /* p25.TESU*/ SOLE,
  /* p25.TAZU*/ SOLE,
  /* p25.TEWA*/ SOLE
);

///* p05.KORE*/ wire P05_NC0 = nand2(KERU_DBG_FF00_D7, FF60_0);
///* p05.KYWE*/ wire P05_NC1 = nor4 (KERU_DBG_FF00_D7, FF60_0o);

/* p08.LYRA*/ wire DBG_D_RDn = nand2(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/* p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D[0] = not1(/* p08.TOVO*/ not1(pins.PIN_D0_C));
/* p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D[1] = not1(/* p08.RUZY*/ not1(pins.PIN_D1_C));
/* p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D[2] = not1(/* p08.ROME*/ not1(pins.PIN_D2_C));
/* p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D[3] = not1(/* p08.SAZA*/ not1(pins.PIN_D3_C));
/* p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D[4] = not1(/* p08.TEHE*/ not1(pins.PIN_D4_C));
/* p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D[5] = not1(/* p08.RATU*/ not1(pins.PIN_D5_C));
/* p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D[6] = not1(/* p08.SOCA*/ not1(pins.PIN_D6_C));
/* p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D[7] = not1(/* p08.RYBA*/ not1(pins.PIN_D7_C));

// hack, not correct
{
  // FF60 debug state
  /* p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /* p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A[ 5], BUS_CPU_A[ 6], bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /* p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /* p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /* p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D[0]);
  /* p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D[1]);

  ///* p05.KURA*/ wire FF60_0n = not1(FF60);
  ///* p05.JEVA*/ wire FF60_0o = not1(FF60);
}


// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A[ 0].qn_new());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A[ 1].qn_new());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A[ 2].qn_new());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A[ 3].qn_new());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A[ 4].qn_new());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A[ 5].qn_new());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A[ 6].qn_new());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A[ 7].qn_new());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A[ 8].qn_new());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A[ 9].qn_new());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A[10].qn_new());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A[11].qn_new());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A[12].qn_new());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A[13].qn_new());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A[14].qn_new());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A[15].qn_new());

  // KEJO_01 << KOVA_02
  // KEJO_02
  // KEJO_03
  // KEJO_04 << TOVA_02
  // KEJO_05 << KOVA_02
  // KEJO_06
  // KEJO_07
  // KEJO_08
  // KEJO_09 >> BUS_CPU_A[ 0]p
  // KEJO_10

  /* p08.KEJO*/ BUS_CPU_A[ 0] = tribuf_10np(TOVA_MODE_DBG2n, KOVA_A00p);
  /* p08.BYXE*/ BUS_CPU_A[ 1] = tribuf_10np(TOVA_MODE_DBG2n, CAMU_A01p);
  /* p08.AKAN*/ BUS_CPU_A[ 2] = tribuf_10np(TOVA_MODE_DBG2n, BUXU_A02p);
  /* p08.ANAR*/ BUS_CPU_A[ 3] = tribuf_10np(TOVA_MODE_DBG2n, BASE_A03p);
  /* p08.AZUV*/ BUS_CPU_A[ 4] = tribuf_10np(TOVA_MODE_DBG2n, AFEC_A04p);
  /* p08.AJOV*/ BUS_CPU_A[ 5] = tribuf_10np(TOVA_MODE_DBG2n, ABUP_A05p);
  /* p08.BYNE*/ BUS_CPU_A[ 6] = tribuf_10np(TOVA_MODE_DBG2n, CYGU_A06p);
  /* p08.BYNA*/ BUS_CPU_A[ 7] = tribuf_10np(TOVA_MODE_DBG2n, COGO_A07p);
  /* p08.LOFA*/ BUS_CPU_A[ 8] = tribuf_10np(TOVA_MODE_DBG2n, MUJY_A08p);
  /* p08.MAPU*/ BUS_CPU_A[ 9] = tribuf_10np(TOVA_MODE_DBG2n, NENA_A09p);
  /* p08.RALA*/ BUS_CPU_A[10] = tribuf_10np(TOVA_MODE_DBG2n, SURA_A10p);
  /* p08.LORA*/ BUS_CPU_A[11] = tribuf_10np(TOVA_MODE_DBG2n, MADY_A11p);
  /* p08.LYNA*/ BUS_CPU_A[12] = tribuf_10np(TOVA_MODE_DBG2n, LAHE_A12p);
  /* p08.LEFY*/ BUS_CPU_A[13] = tribuf_10np(TOVA_MODE_DBG2n, LURA_A13p);
  /* p08.NEFE*/ BUS_CPU_A[14] = tribuf_10np(TOVA_MODE_DBG2n, PEVO_A14p);
  /* p08.SYZU*/ BUS_CPU_A[15] = tribuf_10np(TOVA_MODE_DBG2n, RAZA_A15p);
}

//------------------------------------------------------------------------------------------------------------------------

#endif
