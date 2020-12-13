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

  CHECK_N(pclk_reg.AFUR_xxxxEFGHp.qp_old());
  CHECK_P(pclk_reg.ALEF_AxxxxFGHp.qp_old());
  CHECK_P(pclk_reg.APUK_ABxxxxGHp.qp_old());
  CHECK_P(pclk_reg.ADYK_ABCxxxxHp.qp_old());

  // Wait for PIN_CPU_START
  while(!sys_cpu_start) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset_boot();
  sys_cpuready = 1;
  sys_cpu_en = true;

  if (fastboot) {
    div_reg.TERO_DIV03p_evn.reset(REG_D0C1);
    div_reg.UNYK_DIV04p_evn.reset(REG_D0C1);
    div_reg.UPOF_DIV15p_evn.reset(REG_D1C1);
  }

  // And clear the framebuffer
  memset(framebuffer, 4, sizeof(framebuffer));
}

//-----------------------------------------------------------------------------

void GateBoy::reset_cart(uint8_t* _boot_buf, size_t _boot_size, uint8_t* _cart_buf, size_t _cart_size) {
  reset_boot(_boot_buf, _boot_size, _cart_buf, _cart_size, true);

  PIN_CPU_BOOTp.reset(REG_D0C0);
  PIN_CPU_INT_VBLANK.reset(REG_D1C0);
  PIN_CPU_STARTp.reset(REG_D0C0);

  PIN_CPU_BUKE_AxxxxxGH.reset(REG_D1C0);

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

  ext_bus.ALOR_EXT_ADDR_LATCH_00p.reset(REG_D1C0);
  ext_bus.APUR_EXT_ADDR_LATCH_01p.reset(REG_D0C0);
  ext_bus.ALYR_EXT_ADDR_LATCH_02p.reset(REG_D1C0);
  ext_bus.ARET_EXT_ADDR_LATCH_03p.reset(REG_D1C0);
  ext_bus.AVYS_EXT_ADDR_LATCH_04p.reset(REG_D0C0);
  ext_bus.ATEV_EXT_ADDR_LATCH_05p.reset(REG_D0C0);
  ext_bus.AROS_EXT_ADDR_LATCH_06p.reset(REG_D1C0);
  ext_bus.ARYM_EXT_ADDR_LATCH_07p.reset(REG_D0C0);
  ext_bus.LUNO_EXT_ADDR_LATCH_08p.reset(REG_D0C0);
  ext_bus.LYSA_EXT_ADDR_LATCH_09p.reset(REG_D0C0);
  ext_bus.PATE_EXT_ADDR_LATCH_10p.reset(REG_D0C0);
  ext_bus.LUMY_EXT_ADDR_LATCH_11p.reset(REG_D0C0);
  ext_bus.LOBU_EXT_ADDR_LATCH_12p.reset(REG_D0C0);
  ext_bus.LONU_EXT_ADDR_LATCH_13p.reset(REG_D0C0);
  ext_bus.NYRE_EXT_ADDR_LATCH_14p.reset(REG_D0C0);

  ext_bus.SOMA_EXT_DATA_LATCH_D0n.reset(REG_D1C0);
  ext_bus.RONY_EXT_DATA_LATCH_D1n.reset(REG_D0C0);
  ext_bus.RAXY_EXT_DATA_LATCH_D2n.reset(REG_D1C0);
  ext_bus.SELO_EXT_DATA_LATCH_D3n.reset(REG_D0C0);
  ext_bus.SODY_EXT_DATA_LATCH_D4n.reset(REG_D1C0);
  ext_bus.SAGO_EXT_DATA_LATCH_D5n.reset(REG_D1C0);
  ext_bus.RUPA_EXT_DATA_LATCH_D6n.reset(REG_D1C0);
  ext_bus.SAZY_EXT_DATA_LATCH_D7n.reset(REG_D1C0);

  PIN_EXT_A[ 0].reset(REG_D1C0);
  PIN_EXT_A[ 1].reset(REG_D0C0);
  PIN_EXT_A[ 2].reset(REG_D1C0);
  PIN_EXT_A[ 3].reset(REG_D1C0);
  PIN_EXT_A[ 4].reset(REG_D0C0);
  PIN_EXT_A[ 5].reset(REG_D0C0);
  PIN_EXT_A[ 6].reset(REG_D1C0);
  PIN_EXT_A[ 7].reset(REG_D0C0);
  PIN_EXT_A[ 8].reset(REG_D0C0);
  PIN_EXT_A[ 9].reset(REG_D0C0);
  PIN_EXT_A[10].reset(REG_D0C0);
  PIN_EXT_A[11].reset(REG_D0C0);
  PIN_EXT_A[12].reset(REG_D0C0);
  PIN_EXT_A[13].reset(REG_D0C0);
  PIN_EXT_A[14].reset(REG_D0C0);
  PIN_EXT_A[15].reset(REG_D1C0);

  PIN_EXT_D[0].reset(REG_D0C0);
  PIN_EXT_D[1].reset(REG_D1C0);
  PIN_EXT_D[2].reset(REG_D0C0);
  PIN_EXT_D[3].reset(REG_D1C0);
  PIN_EXT_D[4].reset(REG_D0C0);
  PIN_EXT_D[5].reset(REG_D0C0);
  PIN_EXT_D[6].reset(REG_D0C0);
  PIN_EXT_D[7].reset(REG_D0C0);

  BUS_VRAM_An[ 0].reset(REG_D1C0);
  BUS_VRAM_An[ 1].reset(REG_D1C0);
  BUS_VRAM_An[ 2].reset(REG_D1C0);
  BUS_VRAM_An[ 3].reset(REG_D1C0);
  BUS_VRAM_An[ 4].reset(REG_D0C0);
  BUS_VRAM_An[ 5].reset(REG_D1C0);
  BUS_VRAM_An[ 6].reset(REG_D0C0);
  BUS_VRAM_An[ 7].reset(REG_D1C0);
  BUS_VRAM_An[ 8].reset(REG_D1C0);
  BUS_VRAM_An[ 9].reset(REG_D1C0);
  BUS_VRAM_An[10].reset(REG_D1C0);
  BUS_VRAM_An[11].reset(REG_D1C0);
  BUS_VRAM_An[12].reset(REG_D1C0);

  PIN_VRAM_Ap[ 0].reset(REG_D0C0);
  PIN_VRAM_Ap[ 1].reset(REG_D0C0);
  PIN_VRAM_Ap[ 2].reset(REG_D0C0);
  PIN_VRAM_Ap[ 3].reset(REG_D0C0);
  PIN_VRAM_Ap[ 4].reset(REG_D1C0);
  PIN_VRAM_Ap[ 5].reset(REG_D0C0);
  PIN_VRAM_Ap[ 6].reset(REG_D1C0);
  PIN_VRAM_Ap[ 7].reset(REG_D0C0);
  PIN_VRAM_Ap[ 8].reset(REG_D0C0);
  PIN_VRAM_Ap[ 9].reset(REG_D0C0);
  PIN_VRAM_Ap[10].reset(REG_D0C0);
  PIN_VRAM_Ap[11].reset(REG_D0C0);
  PIN_VRAM_Ap[12].reset(REG_D0C0);

  rst_reg.TUBO_WAITINGp.reset(REG_D0C0);

  vclk_reg.WUVU_ABxxEFxx.reset(REG_D1C1);
  vclk_reg.VENA_xxCDEFxx.reset(REG_D0C0);
  vclk_reg.WOSU_AxxDExxH.reset(REG_D1C0);

  div_reg.UKUP_DIV00p_evn.reset(REG_D1C1);
  div_reg.UFOR_DIV01p_evn.reset(REG_D1C0);
  div_reg.UNER_DIV02p_evn.reset(REG_D0C0);
  div_reg.TERO_DIV03p_evn.reset(REG_D0C1);
  div_reg.UNYK_DIV04p_evn.reset(REG_D1C1);
  div_reg.TAMA_DIV05p_evn.reset(REG_D1C0);
  div_reg.UGOT_DIV06p_evn.reset(REG_D1C0);
  div_reg.TULU_DIV07p_evn.reset(REG_D1C0);
  div_reg.TUGO_DIV08p_evn.reset(REG_D0C0);
  div_reg.TOFE_DIV09p_evn.reset(REG_D1C1);
  div_reg.TERU_DIV10p_evn.reset(REG_D0C0);
  div_reg.SOLA_DIV11p_evn.reset(REG_D1C1);
  div_reg.SUBU_DIV12p_evn.reset(REG_D0C0);
  div_reg.TEKA_DIV13p_evn.reset(REG_D1C1);
  div_reg.UKET_DIV14p_evn.reset(REG_D1C0);
  div_reg.UPOF_DIV15p_evn.reset(REG_D1C0);

  bootrom.BOOT_BITn_h.reset(REG_D1C1);

  int_reg.LOPE_FF0F_D0p.reset(REG_D1C1);

  ser_reg.COTY_SER_CLK.reset(REG_D0C0);

  sprite_store.DEZY_COUNT_CLKp_evn.reset(REG_D1C1);
  sprite_store.BESE_SPRITE_COUNT0_evn.reset(REG_D0C1);
  sprite_store.CUXY_SPRITE_COUNT1_evn.reset(REG_D0C1);
  sprite_store.BEGO_SPRITE_COUNT2_evn.reset(REG_D0C1);
  sprite_store.DYBE_SPRITE_COUNT3_evn.reset(REG_D0C1);

  sprite_store.XADU_SPRITE_IDX0p_evn.reset(REG_D0C1);
  sprite_store.XEDY_SPRITE_IDX1p_evn.reset(REG_D0C1);
  sprite_store.ZUZE_SPRITE_IDX2p_evn.reset(REG_D1C1);
  sprite_store.XOBE_SPRITE_IDX3p_evn.reset(REG_D0C1);
  sprite_store.YDUF_SPRITE_IDX4p_evn.reset(REG_D1C1);
  sprite_store.XECU_SPRITE_IDX5p_evn.reset(REG_D0C1);

  SPR_TRI_I[0].reset(REG_D0C0);
  SPR_TRI_I[1].reset(REG_D0C0);
  SPR_TRI_I[2].reset(REG_D1C0);
  SPR_TRI_I[3].reset(REG_D0C0);
  SPR_TRI_I[4].reset(REG_D1C0);
  SPR_TRI_I[5].reset(REG_D0C0);

  SPR_TRI_L[0].reset(REG_D1C0);
  SPR_TRI_L[1].reset(REG_D1C0);
  SPR_TRI_L[2].reset(REG_D1C0);
  SPR_TRI_L[3].reset(REG_D1C0);

  sprite_scanner.BESU_SCANNINGp_evn.reset(REG_D0C0);
  sprite_scanner.CENO_SCANNINGp_evn.reset(REG_D0C1);

  sprite_scanner.BYBA_SCAN_DONE_Ap_evn.reset(REG_D1C1);
  sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.reset(REG_D1C0);

  sprite_scanner.YFEL_SCAN0_evn.reset(REG_D1C1);
  sprite_scanner.WEWY_SCAN1_evn.reset(REG_D1C0);
  sprite_scanner.GOSO_SCAN2_evn.reset(REG_D1C0);
  sprite_scanner.ELYN_SCAN3_evn.reset(REG_D0C0);
  sprite_scanner.FAHA_SCAN4_evn.reset(REG_D0C1);
  sprite_scanner.FONY_SCAN5_evn.reset(REG_D1C1);

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

  ppu_reg.RUPO_STAT_LYC_MATCHn_evn.reset(REG_D0C0);
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
  reg_lcdc.XONA_LCDC_LCDENn_h .reset(REG_D0C1);
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
  lcd.RUJU_xxx.reset(REG_D1C0);
  lcd.POFY_xxx.reset(REG_D0C0);
  lcd.POME_xxx.reset(REG_D1C0);
  lcd.PAHO_X_8_SYNC_odd.reset(REG_D0C1);
  lcd.WUSA_LCD_CLOCK_GATE_xxx.reset(REG_D0C0);

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
  sys_cpuready = true;
  sys_cpu_en = true;
  sys_fastboot = true;
  sys_cpu_start = false;

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_cpu_bus        = offsetof(GateBoy, BUS_CPU_D_out);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
  const int o_ext_bus        = offsetof(GateBoy, ext_bus);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_rst_reg        = offsetof(GateBoy, rst_reg);
  const int o_pclk_reg       = offsetof(GateBoy, pclk_reg);
  const int o_vclk_reg       = offsetof(GateBoy, vclk_reg);
  const int o_div_reg        = offsetof(GateBoy, div_reg);
  const int o_tim_reg        = offsetof(GateBoy, tim_reg);
  const int o_dma_reg        = offsetof(GateBoy, dma_reg);
  const int o_int_reg        = offsetof(GateBoy, int_reg);
  const int o_joypad         = offsetof(GateBoy, joypad);
  const int o_ser_reg        = offsetof(GateBoy, ser_reg);
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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {
  (void)pass_index;

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

  wire XYMU_RENDERINGn_old = ppu_reg.XYMU_RENDERINGn.qn_any();

  wire _XODO_VID_RSTp_old = reg_lcdc.XODO_VID_RSTp(rst_reg.AVOR_SYS_RSTp());

  wire _TAVE_PRELOAD_DONE_TRIGp_old = tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGn_old);

  wire _TEVO_FETCH_TRIGp_old = or3(win_reg.SEKO_WIN_FETCH_TRIGp(), win_reg.SUZU_WIN_FIRST_TILEne(), _TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  wire _BALU_LINE_RSTp_old = lcd.BALU_LINE_RSTp(_XODO_VID_RSTp_old);

  wire _AVAP_SCAN_DONE_TRIGp_old = sprite_scanner.AVAP_SCAN_DONE_TRIGp(_BALU_LINE_RSTp_old);

  wire _NYXU_BFETCH_RSTn_old = nor3(_AVAP_SCAN_DONE_TRIGp_old, win_reg.MOSU_WIN_MODE_TRIGp(), _TEVO_FETCH_TRIGp_old);

  wire MOCE_BFETCH_DONEn_old = tile_fetcher.MOCE_BFETCH_DONEn(_NYXU_BFETCH_RSTn_old);
  wire _LYRY_BFETCH_DONEp_old = tile_fetcher.LYRY_BFETCH_DONEp(_NYXU_BFETCH_RSTn_old);
  wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp_old, win_reg.TUKU_WIN_HITn(), _LYRY_BFETCH_DONEp_old, sprite_fetcher.SOWO_SFETCH_RUNNINGn());

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

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Address decoders

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn_ext = and2(PIN_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn_ext(BUS_CPU_A));

  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp_ext = nor2(SYRO_FE00_FFFF_ext(BUS_CPU_A), TEXO_ADDR_VRAMn_ext);
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp_ext = and2(TEFA_ADDR_VRAMp_ext, BUS_CPU_A[15]);

  {
    PIN_CPU_ADDR_HIp.setp(SYRO_FE00_FFFF_ext(BUS_CPU_A));
  }

  //----------------------------------------
  // Debug control signals.

  {
    PIN_CPU_UNOR_DBG.setp(UNOR_MODE_DBG2p_ext());
    PIN_CPU_UMUT_DBG.setp(UMUT_MODE_DBG1p_ext());
  }

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.UCOB*/ wire UCOB_CLKBADp  = not1(sys_clkgood);
  /* p01.ATEZ*/ wire ATEZ_CLKBADp  = not1(sys_clkgood);
  /* p01.ABOL*/ wire ABOL_CLKREQn  = not1(sys_cpuready);

  /* p01.ATAL*/ wire ATAL_xBxDxFxH = !(phase_total & 1) && sys_clken;

  //----------------------------------------
  // Sys clock signals

  /*#p01.ATYP*/ bool ATYP_ABCDxxxx;
  /* p01.ALEF*/ bool AFEP_AxxxxFGH_clkodd;
  /* p01.APUK*/ bool AROV_xxCDEFxx;
  /*#p01.ADAR*/ bool ADAR_ABCxxxxH_clkodd;

  /* p01.UVYT*/ bool UVYT_ABCDxxxx_clk;
  /* p01.BOGA*/ bool BOGA_Axxxxxxx_clkevn;

  [
    this,
    ATEZ_CLKBADp,
    ABOL_CLKREQn,
    ATAL_xBxDxFxH,
    &ATYP_ABCDxxxx,
    &AFEP_AxxxxFGH_clkodd,
    &AROV_xxCDEFxx,
    &ADAR_ABCxxxxH_clkodd,
    &UVYT_ABCDxxxx_clk,
    &BOGA_Axxxxxxx_clkevn
  ](){
    /* p01.AFUR*/ pclk_reg.AFUR_xxxxEFGHp.dff9(!ATAL_xBxDxFxH, UPOJ_MODE_PRODn_ext(), pclk_reg.ADYK_ABCxxxxHp.qp_old());
    /* p01.ALEF*/ pclk_reg.ALEF_AxxxxFGHp.dff9( ATAL_xBxDxFxH, UPOJ_MODE_PRODn_ext(), pclk_reg.AFUR_xxxxEFGHp.qn_old());
    /* p01.APUK*/ pclk_reg.APUK_ABxxxxGHp.dff9(!ATAL_xBxDxFxH, UPOJ_MODE_PRODn_ext(), pclk_reg.ALEF_AxxxxFGHp.qn_old());
    /* p01.ADYK*/ pclk_reg.ADYK_ABCxxxxHp.dff9( ATAL_xBxDxFxH, UPOJ_MODE_PRODn_ext(), pclk_reg.APUK_ABxxxxGHp.qn_old());

    /*#p01.ATYP*/ ATYP_ABCDxxxx = not1(pclk_reg.AFUR_xxxxEFGHp.qp_new());
    /*#p01.AFEP*/ AFEP_AxxxxFGH_clkodd = not1(pclk_reg.ALEF_AxxxxFGHp.qn_new());
    /*#p01.AROV*/ AROV_xxCDEFxx = not1(pclk_reg.APUK_ABxxxxGHp.qp_new());
    /*#p01.ADAR*/ ADAR_ABCxxxxH_clkodd = not1(pclk_reg.ADYK_ABCxxxxHp.qn_new());

    /*#p01.BELU*/ wire _BELU_xxxxEFGH_clkevn = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
    /*#p01.BYRY*/ wire _BYRY_ABCDxxxx_clkevn = not1(_BELU_xxxxEFGH_clkevn);
    /*#p01.BUDE*/ wire _BUDE_xxxxEFGH_clkevn = not1(_BYRY_ABCDxxxx_clkevn);

    PIN_EXT_CLK.pin_out(_BUDE_xxxxEFGH_clkevn, _BUDE_xxxxEFGH_clkevn);
    /* p01.UVYT*/ UVYT_ABCDxxxx_clk = not1(_BUDE_xxxxEFGH_clkevn);

    /*#p01.BEKO*/ wire _BEKO_ABCDxxxx_clkevn = not1(_BUDE_xxxxEFGH_clkevn); // BEKO+BAVY parallel
    /*#p01.BAPY*/ wire _BAPY_xxxxxxGH_clkevn = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*#p01.BERU*/ wire _BERU_ABCDEFxx_clkevn = not1(_BAPY_xxxxxxGH_clkevn);
    /*#p01.BUFA*/ wire _BUFA_xxxxxxGH_clkevn = not1(_BERU_ABCDEFxx_clkevn);
    /*#p01.BOLO*/ wire _BOLO_ABCDEFxx_clkevn = not1(_BUFA_xxxxxxGH_clkevn);
    /*#p01.BEJA*/ wire _BEJA_xxxxEFGH_clkevn = nand4(_BOLO_ABCDEFxx_clkevn, _BOLO_ABCDEFxx_clkevn, _BEKO_ABCDxxxx_clkevn, _BEKO_ABCDxxxx_clkevn);
    /*#p01.BANE*/ wire _BANE_ABCDxxxx_clkevn = not1(_BEJA_xxxxEFGH_clkevn);
    /*#p01.BELO*/ wire _BELO_xxxxEFGH_clkevn = not1(_BANE_ABCDxxxx_clkevn);
    /*#p01.BAZE*/ wire _BAZE_ABCDxxxx_clkevn = not1(_BELO_xxxxEFGH_clkevn);
    /*#p01.BUTO*/ wire _BUTO_xBCDEFGH_clkodd = nand3(AFEP_AxxxxFGH_clkodd, ATYP_ABCDxxxx, _BAZE_ABCDxxxx_clkevn);
    /*#p01.BELE*/ wire _BELE_Axxxxxxx_clkevn = not1(_BUTO_xBCDEFGH_clkodd);
    /*#p01.BYJU*/ wire _BYJU_Axxxxxxx_clkevn = or2(_BELE_Axxxxxxx_clkevn, ATEZ_CLKBADp);
    /*#p01.BALY*/ wire _BALY_xBCDEFGH_clkodd = not1(_BYJU_Axxxxxxx_clkevn);
    /* p01.BOGA*/ BOGA_Axxxxxxx_clkevn = not1(_BALY_xBCDEFGH_clkodd);

    // Clocks that go back to the CPU

    /*#p01.BUTY*/ wire _BUTY_CLKREQ_ext   = not1(ABOL_CLKREQn);
    /*#p01.BUVU*/ wire _BUVU_Axxxxxxx_clkevn = and2(_BALY_xBCDEFGH_clkodd, _BUTY_CLKREQ_ext);
    /*#p01.BYXO*/ wire _BYXO_xBCDEFGH_clkodd = not1(_BUVU_Axxxxxxx_clkevn);
    /*#p01.BEDO*/ wire _BEDO_Axxxxxxx_clkevn = not1(_BYXO_xBCDEFGH_clkodd);
    /*#p01.BOWA*/ wire _BOWA_xBCDEFGH_clkodd = not1(_BEDO_Axxxxxxx_clkevn);

    /*#p01.BUGO*/ wire _BUGO_xBCDExxx_clkodd = not1(AFEP_AxxxxFGH_clkodd);
    /*#p01.BATE*/ wire _BATE_AxxxxxGH_clkevn = nor3(_BUGO_xBCDExxx_clkodd, AROV_xxCDEFxx, ABOL_CLKREQn);
    /*#p01.BASU*/ wire _BASU_xBCDEFxx_clkodd = not1(_BATE_AxxxxxGH_clkevn);
    /*#p01.BUKE*/ wire _BUKE_AxxxxxGH_clkevn = not1(_BASU_xBCDEFxx_clkodd);
    /*#p01.BOMA*/ wire _BOMA_xBCDEFGH_clkodd = not1(BOGA_Axxxxxxx_clkevn);

    PIN_CPU_BOWA_Axxxxxxx.setp(_BOWA_xBCDEFGH_clkodd);
    PIN_CPU_BEDO_xBCDEFGH.setp(_BEDO_Axxxxxxx_clkevn);
    PIN_CPU_BEKO_ABCDxxxx.setp(_BEKO_ABCDxxxx_clkevn);
    PIN_CPU_BUDE_xxxxEFGH.setp(_BUDE_xxxxEFGH_clkevn);
    PIN_CPU_BOLO_ABCDEFxx.setp(_BOLO_ABCDEFxx_clkevn);
    PIN_CPU_BUKE_AxxxxxGH.setp(_BUKE_AxxxxxGH_clkevn);
    PIN_CPU_BOMA_xBCDEFGH.setp(_BOMA_xBCDEFGH_clkodd);
    PIN_CPU_BOGA_Axxxxxxx.setp( BOGA_Axxxxxxx_clkevn);
  }();

  //----------------------------------------
  // CPU read signals

  /* p07.TEDO*/ bool TEDO_CPU_RDp;
  /* p01.APOV*/ bool APOV_CPU_WRp_clkevn;
  /* p07.TAPU*/ bool TAPU_CPU_WRp;

  [
    this,
    ADAR_ABCxxxxH_clkodd,
    ATYP_ABCDxxxx,

    &TEDO_CPU_RDp,
    &APOV_CPU_WRp_clkevn,
    &TAPU_CPU_WRp
  ](){
    /* p07.UJYV*/ wire _UJYV_CPU_RDn_ext = mux2n(UNOR_MODE_DBG2p_ext(), /*PIN_EXT_RDn.qn_new()*/ 0, PIN_CPU_RDp.qp_new()); // Ignoring debug stuff for now
    /* p07.TEDO*/ TEDO_CPU_RDp = not1(_UJYV_CPU_RDn_ext);

    /*#p01.AFAS*/ wire _AFAS_xxxxEFGx_clkevn = nor2(ADAR_ABCxxxxH_clkodd, ATYP_ABCDxxxx);
    /* p01.AREV*/ wire _AREV_ABCDxxxH_CPU_WRn_clkodd = nand2(PIN_CPU_WRp.qp_new(), _AFAS_xxxxEFGx_clkevn);
    /* p01.APOV*/ APOV_CPU_WRp_clkevn = not1(_AREV_ABCDxxxH_CPU_WRn_clkodd);

    /* p07.UBAL*/ wire _UBAL_CPU_WRn_clkevn = mux2n(UNOR_MODE_DBG2p_ext(), /*PIN_EXT_WRn.qn_new()*/ 0, APOV_CPU_WRp_clkevn); // Ignoring debug stuff for now
    /* p07.TAPU*/ TAPU_CPU_WRp = not1(_UBAL_CPU_WRn_clkevn); // xxxxEFGx
  }();


  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // DIV

  div_reg.tock(
    sys_rst,
    BUS_CPU_A,
    UCOB_CLKBADp,
    BOGA_Axxxxxxx_clkevn,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    BUS_CPU_D_out
  );

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Power-on reset handler

  [
    this,
    UCOB_CLKBADp,
    BOGA_Axxxxxxx_clkevn
  ]() {
    /* p01.AFER*/ rst_reg.AFER_SYS_RSTp_evn.dff13(BOGA_Axxxxxxx_clkevn, UPOJ_MODE_PRODn_ext(), rst_reg.ASOL_POR_DONEn.qp_old());

    /* p01.UPYF*/ wire _UPYF_ext = or2(sys_rst, UCOB_CLKBADp);
    /* p01.TUBO*/ rst_reg.TUBO_WAITINGp.nor_latch(_UPYF_ext, sys_cpuready);
    /* p01.UNUT*/ wire _UNUT_POR_TRIGn_new = and2(rst_reg.TUBO_WAITINGp.qp_new(), sys_fastboot ? div_reg.TERO_DIV03p_evn.qp_new() : div_reg.UPOF_DIV15p_evn.qp_new());
    /* p01.TABA*/ wire _TABA_POR_TRIGn_new = or3(UNOR_MODE_DBG2p_ext(), UMUT_MODE_DBG1p_ext(), _UNUT_POR_TRIGn_new);
    /*#p01.ALYP*/ wire _ALYP_RSTn_new = not1(_TABA_POR_TRIGn_new);
    /*#p01.AFAR*/ wire _AFAR_RSTp_new  = nor2(sys_rst, _ALYP_RSTn_new);
    /* p01.ASOL*/ rst_reg.ASOL_POR_DONEn.nor_latch(sys_rst, _AFAR_RSTp_new); // Schematic wrong, this is a latch.

    PIN_CPU_EXT_CLKGOOD.setp(sys_clkgood);
    PIN_CPU_EXT_RST.setp(sys_rst);
    PIN_CPU_STARTp.setp(_TABA_POR_TRIGn_new);
    sys_cpu_start = _TABA_POR_TRIGn_new;

    PIN_CPU_SYS_RSTp.setp(rst_reg.AFER_SYS_RSTp_evn.qp_new());
  }();

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp = or2(rst_reg.AFER_SYS_RSTp_evn.qp_new(), rst_reg.ASOL_POR_DONEn.qp_new());

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /* p25.TUTO*/ wire TUTO_DBG_VRAMp_new = [this, AVOR_SYS_RSTp](){
    /*#p25.SYCY*/ wire _SYCY_MODE_DBG2n_ext = not1(UNOR_MODE_DBG2p_ext());
    /*#p25.SOTO*/ dbg_reg.SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n_ext, CUNU_SYS_RSTn(AVOR_SYS_RSTp), dbg_reg.SOTO_DBG_VRAMp.qn_any());
    /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_new = and2(UNOR_MODE_DBG2p_ext(), dbg_reg.SOTO_DBG_VRAMp.qn_new());
    return _TUTO_DBG_VRAMp_new;
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /* p04.CATY*/ wire CATY_LATCH_EXTp_ext = [this, AVOR_SYS_RSTp, ATAL_xBxDxFxH](){
    /* p04.DECY*/ wire _DECY_LATCH_EXTn_ext = not1(PIN_CPU_LATCH_EXT.qp_new());
    /* p04.CATY*/ wire _CATY_LATCH_EXTp_ext = not1(_DECY_LATCH_EXTn_ext);
    /* p04.MAKA*/ oam_bus.MAKA_LATCH_EXTp_evn.dff17(ZEME_AxCxExGx(ATAL_xBxDxFxH), CUNU_SYS_RSTn(AVOR_SYS_RSTp), _CATY_LATCH_EXTp_ext);
    return _CATY_LATCH_EXTp_ext;
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // CPU Bootrom read

  bootrom.tock
  (
    BUS_CPU_A,
    BUS_CPU_D,
    UMUT_MODE_DBG1p_ext(),
    AVOR_SYS_RSTp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    boot_buf,
    BUS_CPU_D_out
  );

  PIN_CPU_BOOTp.setp(bootrom.TUTU_READ_BOOTROMp_new(BUS_CPU_A));

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Timer

  tim_reg.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    BOGA_Axxxxxxx_clkevn,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    PIN_CPU_LATCH_EXT.qp_new(),
    BUS_CPU_D_out,
    div_reg);

  wire MOBA_TIMER_OVERFLOWp_evn_new = tim_reg.MOBA_TIMER_OVERFLOWp_evn.qp_new();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // LCDC. Has to be near the top as it controls the video reset signal

  reg_lcdc.tock(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    BUS_CPU_D_out
  );

  /* p01.XODO*/ wire XODO_VID_RSTp = nand2(XEBE_SYS_RSTn(AVOR_SYS_RSTp), reg_lcdc.XONA_LCDC_LCDENn_h.qn_new());
  /* p23.WYMO*/ wire WYMO_LCDC_WINENn_new = reg_lcdc.WYMO_LCDC_WINENn_h.qn_new();
  /* p23.XONA*/ wire XONA_LCDC_LCDENp_h_new = reg_lcdc.XONA_LCDC_LCDENn_h.qn_new();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Video clocks

  [
    this,
    XODO_VID_RSTp,
    ATAL_xBxDxFxH
  ]() {
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new = not1(XODO_VID_RSTp);

    /* p29.WOSU*/ vclk_reg.WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(ATAL_xBxDxFxH),    _XAPO_VID_RSTn_new, vclk_reg.WUVU_ABxxEFxx.qn_any());
    /* p29.WUVU*/ vclk_reg.WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(ATAL_xBxDxFxH),    _XAPO_VID_RSTn_new, vclk_reg.WUVU_ABxxEFxx.qn_any());
    /* p21.VENA*/ vclk_reg.VENA_xxCDEFxx.dff17(vclk_reg.WUVU_ABxxEFxx.qn_new(), _XAPO_VID_RSTn_new, vclk_reg.VENA_xxCDEFxx.qn_any()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // DMA

  wire LAVY_FF46_WRp_clk_evn = [
    this,
    BUS_CPU_A,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  ](){
    /*#p04.LAVY*/ wire _LAVY_FF46_WRp_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XEDA_FF46p_ext(BUS_CPU_A));
    return _LAVY_FF46_WRp_clk_evn;
  }();



  [this, AVOR_SYS_RSTp, UVYT_ABCDxxxx_clk, LAVY_FF46_WRp_clk_evn](){
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG_old_evn = nor2(LAVY_FF46_WRp_clk_evn, dma_reg.LYXE_DMA_LATCHp_evn.qn_old());

    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0_evn.dff17(UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, _LUPA_DMA_TRIG_old_evn);
  }();

  [this, AVOR_SYS_RSTp, UVYT_ABCDxxxx_clk](){
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);

    /* p04.MOPA*/ wire _MOPA_xxxxEFGH_new_evn = not1(UVYT_ABCDxxxx_clk);
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4_evn.dff17(_MOPA_xxxxEFGH_new_evn, _CUNU_SYS_RSTn_new, dma_reg.LUVY_DMA_TRIG_d0_evn.qp_old());
  }();

  [this, AVOR_SYS_RSTp, LAVY_FF46_WRp_clk_evn](){
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp_new_evn = nand2(dma_reg.LENE_DMA_TRIG_d4_evn.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp_evn.nor_latch(LAVY_FF46_WRp_clk_evn, _LOKO_DMA_RSTp_new_evn);
  }();

  [this, AVOR_SYS_RSTp, UVYT_ABCDxxxx_clk](){
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp_new_evn = nand2(dma_reg.LENE_DMA_TRIG_d4_evn.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn_new = not1(_LOKO_DMA_RSTp_new_evn);

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn_old = nand6(dma_reg.NAKY_DMA_A00p_evn.qp_old(), dma_reg.PYRO_DMA_A01p_evn.qp_old(),  // 128+16+8+4+2+1 = 159
                                                   dma_reg.NEFY_DMA_A02p_evn.qp_old(), dma_reg.MUTY_DMA_A03p_evn.qp_old(),
                                                   dma_reg.NYKO_DMA_A04p_evn.qp_old(), dma_reg.MUGU_DMA_A07p_evn.qp_old());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp_old = not1(_NAVO_DMA_DONEn_old);

    /* p04.MOPA*/ wire _MOPA_xxxxEFGH_new_evn = not1(UVYT_ABCDxxxx_clk);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE_evn.dff17(_MOPA_xxxxEFGH_new_evn, _LAPA_DMA_RSTn_new, _NOLO_DMA_DONEp_old);
  }();


  /*#p04.MATU*/ wire MATU_DMA_RUNNINGp = [this, AVOR_SYS_RSTp, UVYT_ABCDxxxx_clk](){
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);

    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp_evn.dff17(UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, dma_reg.LOKY_DMA_LATCHp_evn.qp_old());
    return dma_reg.MATU_DMA_RUNNINGp_evn.qp_new();
  }();

  [this, AVOR_SYS_RSTp](){
    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn_evn = nand3(dma_reg.LOKY_DMA_LATCHp_evn.qp_old(), dma_reg.MYTE_DMA_DONE_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));
    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp_evn = nand2(dma_reg.LARA_DMA_LATCHn_evn.qp_new(), dma_reg.LENE_DMA_TRIG_d4_evn.qn_new());
    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn_evn = nand3(dma_reg.LOKY_DMA_LATCHp_evn.qp_new(), dma_reg.MYTE_DMA_DONE_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));
  }();

  [this, AVOR_SYS_RSTp, UVYT_ABCDxxxx_clk](){
    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp_new_evn = nand2(dma_reg.LENE_DMA_TRIG_d4_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));

    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn_new = not1(_LOKO_DMA_RSTp_new_evn);

    /*#p04.META*/ wire _META_DMA_CLKp_new_evn = and2(UVYT_ABCDxxxx_clk, dma_reg.LOKY_DMA_LATCHp_evn.qp_new());
    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p_evn.dff17(_META_DMA_CLKp_new_evn,             _LAPA_DMA_RSTn_new, dma_reg.NAKY_DMA_A00p_evn.qn_any());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p_evn.dff17(dma_reg.NAKY_DMA_A00p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYRO_DMA_A01p_evn.qn_any());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p_evn.dff17(dma_reg.PYRO_DMA_A01p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NEFY_DMA_A02p_evn.qn_any());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p_evn.dff17(dma_reg.NEFY_DMA_A02p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUTY_DMA_A03p_evn.qn_any());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p_evn.dff17(dma_reg.MUTY_DMA_A03p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NYKO_DMA_A04p_evn.qn_any());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p_evn.dff17(dma_reg.NYKO_DMA_A04p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYLO_DMA_A05p_evn.qn_any());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p_evn.dff17(dma_reg.PYLO_DMA_A05p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NUTO_DMA_A06p_evn.qn_any());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p_evn.dff17(dma_reg.NUTO_DMA_A06p_evn.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUGU_DMA_A07p_evn.qn_any());
  }();

  [
    this,
    BUS_CPU_A,
    BUS_CPU_D,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  ](){
    /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XEDA_FF46p_ext(BUS_CPU_A));
    /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);

    /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XEDA_FF46p_ext(BUS_CPU_A));
    /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);

    // FF46 DMA reg write/read
    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[0]);
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[1]);
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[2]);
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[3]);
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[4]);
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[5]);
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[6]);
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[7]);

    /*#p04.POLY*/ BUS_CPU_D_out[0].tri6_pn(PUSY_FF46_RDp, dma_reg.NAFA_DMA_A08n_h.qp_new());
    /* p04.ROFO*/ BUS_CPU_D_out[1].tri6_pn(PUSY_FF46_RDp, dma_reg.PYNE_DMA_A09n_h.qp_new());
    /* p04.REMA*/ BUS_CPU_D_out[2].tri6_pn(PUSY_FF46_RDp, dma_reg.PARA_DMA_A10n_h.qp_new());
    /* p04.PANE*/ BUS_CPU_D_out[3].tri6_pn(PUSY_FF46_RDp, dma_reg.NYDO_DMA_A11n_h.qp_new());
    /* p04.PARE*/ BUS_CPU_D_out[4].tri6_pn(PUSY_FF46_RDp, dma_reg.NYGY_DMA_A12n_h.qp_new());
    /* p04.RALY*/ BUS_CPU_D_out[5].tri6_pn(PUSY_FF46_RDp, dma_reg.PULA_DMA_A13n_h.qp_new());
    /* p04.RESU*/ BUS_CPU_D_out[6].tri6_pn(PUSY_FF46_RDp, dma_reg.POKU_DMA_A14n_h.qp_new());
    /* p04.NUVY*/ BUS_CPU_D_out[7].tri6_pn(PUSY_FF46_RDp, dma_reg.MARU_DMA_A15n_h.qp_new());
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  reg_lx.tock(
    XODO_VID_RSTp,
    vclk_reg.TALU_xxCDEFxx());

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
    AVOR_SYS_RSTp,
    vclk_reg.TALU_xxCDEFxx(),
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    reg_ly,
    BUS_CPU_D_out);

  lcd.tock(
    XODO_VID_RSTp,
    vclk_reg.XUPY_ABxxEFxx(),
    reg_lx,
    reg_ly);

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this,
    XODO_VID_RSTp,
    ATAL_xBxDxFxH,
    WODU_HBLANKp_old
  ]() {
    /* p21.TADY*/ wire _TADY_LINE_RSTn_new_evn  = nor2(lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp), TOFU_VID_RSTp(XODO_VID_RSTp));
    /*#p21.VOGA*/ ppu_reg.VOGA_HBLANKp_xxx.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), _TADY_LINE_RSTn_new_evn, WODU_HBLANKp_old);
  }();


  /*#p21.WEGO*/ wire WEGO_HBLANKp_new_any = or2(TOFU_VID_RSTp(XODO_VID_RSTp), ppu_reg.VOGA_HBLANKp_xxx.qp_new());
  /*#p21.PARU*/ wire PARU_VBLANKp_new_evn = not1(lcd.POPU_VBLANKp_evn.qn_new());

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Window match comparator

  bool NUKO_WX_MATCHp_old_evn;

  [
    this,
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,

    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    XODO_VID_RSTp,
    PARU_VBLANKp_new_evn,

    &NUKO_WX_MATCHp_old_evn
  ](){

    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_old_evn = xnor2(pix_count.XEHO_PX0p.qp_old(), reg_wx.MYPA_WX0n_h.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_old_evn = xnor2(pix_count.SAVY_PX1p.qp_old(), reg_wx.NOFE_WX1n_h.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_old_evn = xnor2(pix_count.XODU_PX2p.qp_old(), reg_wx.NOKE_WX2n_h.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_old_evn = xnor2(pix_count.XYDO_PX3p.qp_old(), reg_wx.MEBY_WX3n_h.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_old_evn = xnor2(pix_count.TUHU_PX4p.qp_old(), reg_wx.MYPU_WX4n_h.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p_old_evn = xnor2(pix_count.TUKY_PX5p.qp_old(), reg_wx.MYCE_WX5n_h.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p_old_evn = xnor2(pix_count.TAKO_PX6p.qp_old(), reg_wx.MUVO_WX6n_h.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p_old_evn = xnor2(pix_count.SYBE_PX7p.qp_old(), reg_wx.NUKU_WX7n_h.qn_old());

    /* p27.PUKY*/ wire _PUKY_WX_MATCHn_old_evn  = nand5(win_reg.REJO_WY_MATCH_LATCHp_evn.qp_old(), _NEZO_WX_MATCH4p_old_evn, _NORY_WX_MATCH5p_old_evn, _NONO_WX_MATCH6p_old_evn, _PASE_WX_MATCH7p_old_evn);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp_old_evn  = not1(_PUKY_WX_MATCHn_old_evn);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn_old_evn  = nand5(_NUFA_WX_MATCHp_old_evn, _MYLO_WX_MATCH0p_old_evn, _PUWU_WX_MATCH1p_old_evn, _PUHO_WX_MATCH2p_old_evn, _NYTU_WX_MATCH3p_old_evn);
    /* p27.NUKO*/ NUKO_WX_MATCHp_old_evn  = not1(_NOGY_WX_MATCHn_old_evn);

    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p_old_evn = xnor2(reg_wy.NESO_WY0n_h.qn_old(), reg_ly.MUWY_LY0p_evn.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p_old_evn = xnor2(reg_wy.NYRO_WY1n_h.qn_old(), reg_ly.MYRO_LY1p_evn.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p_old_evn = xnor2(reg_wy.NAGA_WY2n_h.qn_old(), reg_ly.LEXA_LY2p_evn.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p_old_evn = xnor2(reg_wy.MELA_WY3n_h.qn_old(), reg_ly.LYDO_LY3p_evn.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p_old_evn = xnor2(reg_wy.NULO_WY4n_h.qn_old(), reg_ly.LOVU_LY4p_evn.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p_old_evn = xnor2(reg_wy.NENE_WY5n_h.qn_old(), reg_ly.LEMA_LY5p_evn.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p_old_evn = xnor2(reg_wy.NUKA_WY6n_h.qn_old(), reg_ly.MATO_LY6p_evn.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p_old_evn = xnor2(reg_wy.NAFU_WY7n_h.qn_old(), reg_ly.LAFO_LY7p_evn.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn_old_evn = nand5(reg_lcdc.WYMO_LCDC_WINENn_h.qn_old(), _NOJO_WY_MATCH4p_old_evn, _PAGA_WY_MATCH5p_old_evn, _PEZO_WY_MATCH6p_old_evn, _NUPA_WY_MATCH7p_old_evn);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp_old_evn = not1(_PALO_WY_MATCH_HIn_old_evn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn_old_evn    = nand5(_NELE_WY_MATCH_HIp_old_evn, _NAZE_WY_MATCH0p_old_evn, _PEBO_WY_MATCH1p_old_evn, _POMO_WY_MATCH2p_old_evn, _NEVU_WY_MATCH3p_old_evn);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old_evn    = not1(_PAFU_WY_MATCHn_old_evn);

    {
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
      /* p01.PYRY*/ wire _PYRY_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
      /* p27.SARY*/ win_reg.SARY_WY_MATCHp_evn.dff17(vclk_reg.TALU_xxCDEFxx(), _XAPO_VID_RSTn_new_evn, _ROGE_WY_MATCHp_old_evn);
      /* p27.REPU*/ wire _REPU_VBLANKp_new_evn = or2(PARU_VBLANKp_new_evn, _PYRY_VID_RSTp_new_evn);
      /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp_evn.nor_latch(win_reg.SARY_WY_MATCHp_evn.qp_new(), _REPU_VBLANKp_new_evn);
    }
  }();

  reg_wy.tock(BUS_CPU_A, BUS_CPU_D, AVOR_SYS_RSTp, TEDO_CPU_RDp, TAPU_CPU_WRp, BUS_CPU_D_out);
  reg_wx.tock(BUS_CPU_A, BUS_CPU_D, AVOR_SYS_RSTp, TEDO_CPU_RDp, TAPU_CPU_WRp, BUS_CPU_D_out);

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Sprite scanner

  [
    this,
    XODO_VID_RSTp,
    ATAL_xBxDxFxH
  ]() {
    /*#p28.ANOM*/ wire _ANOM_SCAN_RSTn_new_evn  = nor2(lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp), ATAR_VID_RSTp(XODO_VID_RSTp));
    /*#p29.BALU*/ wire _BALU_SCAN_RSTp_new_evn  = not1(_ANOM_SCAN_RSTn_new_evn);
    /*#p29.BAGY*/ wire _BAGY_SCAN_RSTn_new_evn  = not1(_BALU_SCAN_RSTp_new_evn);

    {
      /*#p28.FETO*/ wire _FETO_SCAN_DONEp_old_evn = and4(sprite_scanner.YFEL_SCAN0_evn.qp_old(), sprite_scanner.WEWY_SCAN1_evn.qp_old(), sprite_scanner.GOSO_SCAN2_evn.qp_old(), sprite_scanner.FONY_SCAN5_evn.qp_old()); // 32 + 4 + 2 + 1 = 39
      /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), _BAGY_SCAN_RSTn_new_evn, sprite_scanner.BYBA_SCAN_DONE_Ap_evn.qp_old());
      /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap_evn.dff17(vclk_reg.XUPY_ABxxEFxx(),     _BAGY_SCAN_RSTn_new_evn, _FETO_SCAN_DONEp_old_evn);
    }

    {
      /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);
      /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);
      /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);
      /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);
      /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);
      /* p28.FONY*/ sprite_scanner.FONY_SCAN5_evn.RSTn(_ANOM_SCAN_RSTn_new_evn);

      /*#p28.FETO*/ wire _FETO_SCAN_DONEp_new_evn = and4(sprite_scanner.YFEL_SCAN0_evn.qp_new(), sprite_scanner.WEWY_SCAN1_evn.qp_new(), sprite_scanner.GOSO_SCAN2_evn.qp_new(), sprite_scanner.FONY_SCAN5_evn.qp_new()); // 32 + 4 + 2 + 1 = 39
      /* p28.GAVA*/ wire _GAVA_ABxxEFxx_new_evn = or2(_FETO_SCAN_DONEp_new_evn, vclk_reg.XUPY_ABxxEFxx());
      /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0_evn.dff17(_GAVA_ABxxEFxx_new_evn,                 _ANOM_SCAN_RSTn_new_evn, sprite_scanner.YFEL_SCAN0_evn.qn_any());
      /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1_evn.dff17(sprite_scanner.YFEL_SCAN0_evn.qn_any(), _ANOM_SCAN_RSTn_new_evn, sprite_scanner.WEWY_SCAN1_evn.qn_any());
      /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2_evn.dff17(sprite_scanner.WEWY_SCAN1_evn.qn_any(), _ANOM_SCAN_RSTn_new_evn, sprite_scanner.GOSO_SCAN2_evn.qn_any());
      /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3_evn.dff17(sprite_scanner.GOSO_SCAN2_evn.qn_any(), _ANOM_SCAN_RSTn_new_evn, sprite_scanner.ELYN_SCAN3_evn.qn_any());
      /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4_evn.dff17(sprite_scanner.ELYN_SCAN3_evn.qn_any(), _ANOM_SCAN_RSTn_new_evn, sprite_scanner.FAHA_SCAN4_evn.qn_any());
      /* p28.FONY*/ sprite_scanner.FONY_SCAN5_evn.dff17(sprite_scanner.FAHA_SCAN4_evn.qn_any(), _ANOM_SCAN_RSTn_new_evn, sprite_scanner.FONY_SCAN5_evn.qn_any());
    }

    {
      /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_new_xxx = or3(sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.qp_new(), _BALU_SCAN_RSTp_new_evn, sprite_scanner.BYBA_SCAN_DONE_Ap_evn.qn_new());
      /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_new_xxx = not1(_BEBU_SCAN_DONE_TRIGn_new_xxx);
      /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_TRIGp_new_xxx = or2(ATAR_VID_RSTp(XODO_VID_RSTp), _AVAP_SCAN_DONE_TRIGp_new_xxx);
      /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp_evn.dff17(vclk_reg.XUPY_ABxxEFxx(), ABEZ_VID_RSTn(XODO_VID_RSTp),  sprite_scanner.BESU_SCANNINGp_evn.qp_old());
      /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp_evn.nor_latch(lcd.CATU_LINE_P000p_a.qp_new(), _ASEN_SCAN_DONE_TRIGp_new_xxx);
    }
  }();

  wire AVAP_SCAN_DONE_TRIGp_new_xxx = [this, XODO_VID_RSTp, &AVAP_SCAN_DONE_TRIGp_new_xxx]() {
    /*#p28.ANOM*/ wire _ANOM_SCAN_RSTn_new_evn  = nor2(lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp), ATAR_VID_RSTp(XODO_VID_RSTp));
    /*#p29.BALU*/ wire _BALU_SCAN_RSTp_new_evn  = not1(_ANOM_SCAN_RSTn_new_evn);
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_new_xxx = or3(sprite_scanner.DOBA_SCAN_DONE_Bp_xxx.qp_new(), _BALU_SCAN_RSTp_new_evn, sprite_scanner.BYBA_SCAN_DONE_Ap_evn.qn_new());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_new_xxx = not1(_BEBU_SCAN_DONE_TRIGn_new_xxx);
    return _AVAP_SCAN_DONE_TRIGp_new_xxx;
  }();

  wire ACYL_SCANNINGp = [this, &ACYL_SCANNINGp](){
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(dma_reg.MATU_DMA_RUNNINGp_evn.qp_new());
    /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, sprite_scanner.BESU_SCANNINGp_evn.qp_new());
    return _ACYL_SCANNINGp_new_evn;
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /*#p21.XYMU*/ bool XYMU_RENDERINGp;
  [
    this,
    WEGO_HBLANKp_new_any,
    AVAP_SCAN_DONE_TRIGp_new_xxx,

    &XYMU_RENDERINGp
  ](){
    /*#p21.XYMU*/ ppu_reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp_new_any, AVAP_SCAN_DONE_TRIGp_new_xxx);
    /*#p21.XYMU*/ XYMU_RENDERINGp = ppu_reg.XYMU_RENDERINGn.qn_new();
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, ATAL_xBxDxFxH, TEKY_SFETCH_REQp_old](){
    wire _VYPO_VCC = 1;
    /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp_evn.dff17(LAPE_AxCxExGx(ATAL_xBxDxFxH), _VYPO_VCC, sprite_fetcher.SOBU_SFETCH_REQp_odd.qp_old());
    /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp_odd.dff17(TAVA_xBxDxFxH(ATAL_xBxDxFxH), _VYPO_VCC, TEKY_SFETCH_REQp_old);
  }();

  [this, XODO_VID_RSTp, ATAL_xBxDxFxH](){
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
    /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp_evn.dff17(MEHE_AxCxExGx(ATAL_xBxDxFxH), _XAPO_VID_RSTn_new_evn, win_reg.PYCO_WIN_MATCHp_odd.qp_old());
  }();

  [this, XODO_VID_RSTp](){
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
    /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_new_evn = not1(lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp));
    /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new_evn = nand3(reg_lcdc.WYMO_LCDC_WINENn_h.qn_new(), _XAHY_LINE_TRIGn_new_evn, _XAPO_VID_RSTn_new_evn);
    /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap_evn.nor_latch(win_reg.NUNU_WIN_MATCHp_evn.qp_new(), _XOFO_WIN_RSTp_new_evn);
  }();


  [this, XODO_VID_RSTp, ATAL_xBxDxFxH](){
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
    /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp_odd.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), _XAPO_VID_RSTn_new_evn, win_reg.PYNU_WIN_MODE_Ap_evn.qp_new());
  }();


  /*#p27.NUNY*/ bool NUNY_WIN_MODE_TRIGp_new_evn;
  [this, &NUNY_WIN_MODE_TRIGp_new_evn](){
    /*#p27.NUNY*/ NUNY_WIN_MODE_TRIGp_new_evn = and2(win_reg.PYNU_WIN_MODE_Ap_evn.qp_new(), win_reg.NOPA_WIN_MODE_Bp_odd.qn_new());
  }();


  /*#p27.NOCU*/ bool PORE_WIN_MODEp_new_evn;
  [this, &PORE_WIN_MODEp_new_evn](){
    /*#p27.NOCU*/ wire NOCU_WIN_MODEn_new_evn = not1(win_reg.PYNU_WIN_MODE_Ap_evn.qp_new());
    /* p27.PORE*/ PORE_WIN_MODEp_new_evn = not1(NOCU_WIN_MODEn_new_evn);
  }();

  [this, ATAL_xBxDxFxH, XYMU_RENDERINGp, NUNY_WIN_MODE_TRIGp_new_evn, MOCE_BFETCH_DONEn_old](){
    /* p27.NYFO*/ wire _NYFO_WX_MATCH_TRIGn_new_any = not1(NUNY_WIN_MODE_TRIGp_new_evn);
    /* p27.MOSU*/ wire _MOSU_WX_MATCH_TRIGp_new_any = not1(_NYFO_WX_MATCH_TRIGn_new_any);
    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);
    /* p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG_new_any = nor2(_MOSU_WX_MATCH_TRIGp_new_any, _LOBY_RENDERINGn_new_xxx);
    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
    /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp_xxx.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), _NAFY_RENDERING_AND_NOT_WIN_TRIG_new_any, _LYRY_BFETCH_DONEp_old);
  }();

  [this, ATAL_xBxDxFxH, XYMU_RENDERINGp, NUNY_WIN_MODE_TRIGp_new_evn](){
    /* p27.NYFO*/ wire _NYFO_WX_MATCH_TRIGn_new_any = not1(NUNY_WIN_MODE_TRIGp_new_evn);
    /* p27.MOSU*/ wire _MOSU_WX_MATCH_TRIGp_new_any = not1(_NYFO_WX_MATCH_TRIGn_new_any);
    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);
    /* p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG_new_any = nor2(_MOSU_WX_MATCH_TRIGp_new_any, _LOBY_RENDERINGn_new_xxx);
    /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp_xxx.dff17(MYVO_AxCxExGx(ATAL_xBxDxFxH), _NAFY_RENDERING_AND_NOT_WIN_TRIG_new_any, tile_fetcher.NYKA_FETCH_DONEp_xxx.qp_old());
  }();

  [this, ATAL_xBxDxFxH, XYMU_RENDERINGp](){
    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);

    /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp_odd.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), XYMU_RENDERINGp, tile_fetcher.PORY_FETCH_DONEp_xxx.qp_old());
    /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp_odd.nor_latch(tile_fetcher.PYGO_FETCH_DONEp_odd.qp_new(), _LOBY_RENDERINGn_new_xxx);
  }();

  [this, XODO_VID_RSTp, NUNY_WIN_MODE_TRIGp_new_evn](){
    /* p27.RYDY*/ win_reg.RYDY_WIN_HITp_evn = nor3(win_reg.PUKU_WIN_HITn_evn.qp_old(), tile_fetcher.PORY_FETCH_DONEp_xxx.qp_new(), PYRY_VID_RSTp(XODO_VID_RSTp));
    /* p27.PUKU*/ win_reg.PUKU_WIN_HITn_evn = nor2(win_reg.RYDY_WIN_HITp_evn.qp_new(), NUNY_WIN_MODE_TRIGp_new_evn);
    /* p27.RYDY*/ win_reg.RYDY_WIN_HITp_evn = nor3(win_reg.PUKU_WIN_HITn_evn.qp_new(), tile_fetcher.PORY_FETCH_DONEp_xxx.qp_new(), PYRY_VID_RSTp(XODO_VID_RSTp));
    /* p27.PUKU*/ win_reg.PUKU_WIN_HITn_evn = nor2(win_reg.RYDY_WIN_HITp_evn.qp_new(), NUNY_WIN_MODE_TRIGp_new_evn);
  }();

  /*#p24.TYFA*/ bool TYFA_CLKPIPE_odd;
  [
    this,
    ATAL_xBxDxFxH,

    FEPO_STORE_MATCHp_old,
    WODU_HBLANKp_old,

    &TYFA_CLKPIPE_odd
  ](){
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new_evn = not1(win_reg.RYDY_WIN_HITp_evn.qp_new());
    /*#p24.TOMU*/ wire _TOMU_WIN_HITp_new_evn = not1(_SYLO_WIN_HITn_new_evn);
    /*#p24.SOCY*/ wire _SOCY_WIN_HITn_new_evn = not1(_TOMU_WIN_HITp_new_evn);
    /*#p24.VYBO*/ wire _VYBO_CLKPIPE_xBxDxFxH_clknew = nor3(FEPO_STORE_MATCHp_old, WODU_HBLANKp_old, MYVO_AxCxExGx(ATAL_xBxDxFxH)); // FIXME old/new - but does it really matter here?
    /*#p24.TYFA*/ TYFA_CLKPIPE_odd = and3(_SOCY_WIN_HITn_new_evn, tile_fetcher.POKY_PRELOAD_LATCHp_odd.qp_new(), _VYBO_CLKPIPE_xBxDxFxH_clknew);
  }();


  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this,
    ATAL_xBxDxFxH,
    XYMU_RENDERINGp,
    TYFA_CLKPIPE_odd
  ]() {
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);

    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(tile_fetcher.DATY_SCX0n_h.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(tile_fetcher.DUZU_SCX1n_h.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(tile_fetcher.CYXU_SCX2n_h.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17(MOXE_AxCxExGx(ATAL_xBxDxFxH), XYMU_RENDERINGp, fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_old());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_xBxDxFxH_clknew_odd, XYMU_RENDERINGp, _POHU_SCX_FINE_MATCHp_old);
    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp_new_odd = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new_evn = not1(XYMU_RENDERINGp);

    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(_PAHA_RENDERINGn_new_evn, _POVA_FINE_MATCH_TRIGp_new_odd);
  }();

  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Pixel counter

  /* p21.TADY*/ wire TADY_LINE_RSTn = nor2(lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp), TOFU_VID_RSTp(XODO_VID_RSTp));
  pix_count.tock(TADY_LINE_RSTn, SACU_CLKPIPE_evn);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, ATAL_xBxDxFxH, XYMU_RENDERINGp, XODO_VID_RSTp]() {
    wire _VYPO_VCC = 1;
    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1_evn.dff17(LAPE_AxCxExGx(ATAL_xBxDxFxH), _VYPO_VCC,               sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5_evn.dff17(LAPE_AxCxExGx(ATAL_xBxDxFxH), XYMU_RENDERINGp, sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4_odd.dff17(TAVA_xBxDxFxH(ATAL_xBxDxFxH), XYMU_RENDERINGp, sprite_fetcher.TOBU_SFETCH_S1p_D2_odd.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2_odd.dff17(TAVA_xBxDxFxH(ATAL_xBxDxFxH), XYMU_RENDERINGp, sprite_fetcher.TULY_SFETCH_S1p_odd.qp_old());

    wire SECA_SFETCH_RSTn = sprite_fetcher.SECA_SFETCH_RSTn(XODO_VID_RSTp, lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp));

    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p_odd.RSTn(SECA_SFETCH_RSTn);
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p_odd.RSTn(SECA_SFETCH_RSTn);
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p_odd.RSTn(SECA_SFETCH_RSTn);

    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE_new = nand2(sprite_fetcher.TESE_SFETCH_S2p_odd.qp_new(), sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_new());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_clknew = nand2(LAPE_AxCxExGx(ATAL_xBxDxFxH), _TAME_SFETCH_CLK_GATE_new);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p_odd.dff17(_TOMA_SFETCH_xBxDxFxH_clknew,                SECA_SFETCH_RSTn, sprite_fetcher.TOXE_SFETCH_S0p_odd.qn_new());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p_odd.dff17(sprite_fetcher.TOXE_SFETCH_S0p_odd.qn_new(), SECA_SFETCH_RSTn, sprite_fetcher.TULY_SFETCH_S1p_odd.qn_new());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p_odd.dff17(sprite_fetcher.TULY_SFETCH_S1p_odd.qn_new(), SECA_SFETCH_RSTn, sprite_fetcher.TESE_SFETCH_S2p_odd.qn_new());
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  sprite_store.get_sprite(sprite_match, SPR_TRI_I, SPR_TRI_L);

  /*#p21.XENA*/ wire _XENA_STORE_MATCHn_new = not1(FEPO_STORE_MATCHp_new_evn);
  /*#p21.WODU*/ wire WODU_HBLANKp_new = and2(_XENA_STORE_MATCHn_new, pix_count.XANO_PX167p()); // WODU goes high on odd, cleared on H

  [this, XODO_VID_RSTp, XYMU_RENDERINGp](){
    wire SECA_SFETCH_RSTn = sprite_fetcher.SECA_SFETCH_RSTn(XODO_VID_RSTp, lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp));
    /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn_new = nor2(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGp)); // def nor
    /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp_xxx.nand_latch(SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn_new);
  }();

  [this, XODO_VID_RSTp, TYFA_CLKPIPE_odd, NUKO_WX_MATCHp_old_evn](){
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /* p27.ROCO*/ wire _ROCO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);
    /* p27.PYCO*/ win_reg.PYCO_WIN_MATCHp_odd.dff17(_ROCO_CLKPIPE_xBxDxFxH_clknew_odd, _XAPO_VID_RSTn_new_evn, NUKO_WX_MATCHp_old_evn);
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  win_reg.tock(XODO_VID_RSTp, ATAL_xBxDxFxH);

  [this, TYFA_CLKPIPE_odd, XYMU_RENDERINGp, NUKO_WX_MATCHp_old_evn](){
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(NUKO_WX_MATCHp_old_evn, fine_scroll.ROZE_FINE_COUNT_7n_old());
    /* p27.RYFA*/ win_reg.RYFA_WIN_FETCHn_A_evn.dff17(_SEGU_CLKPIPE_AxCxExGx_clknew_evn, XYMU_RENDERINGp, _PANY_WIN_FETCHn_old);
  }();

  [this, ATAL_xBxDxFxH, XYMU_RENDERINGp](){
    /* p27.RENE*/ win_reg.RENE_WIN_FETCHn_B_odd.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), XYMU_RENDERINGp, win_reg.RYFA_WIN_FETCHn_A_evn.qp_old());
  }();

  /* p27.TEVO*/ bool TEVO_FETCH_TRIGp_new;
  [this, XYMU_RENDERINGp, &TEVO_FETCH_TRIGp_new](){
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new_evn = not1(win_reg.RYDY_WIN_HITp_evn.qp_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_new_xxx = nand2(_SYLO_WIN_HITn_new_evn, win_reg.SOVY_WIN_HITp.qp_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_new = not1(_TUXY_WIN_FIRST_TILEne_new_xxx);
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_new = nor2(win_reg.RYFA_WIN_FETCHn_A_evn.qn_new(), win_reg.RENE_WIN_FETCHn_B_odd.qp_new());
    /* p27.TEVO*/ TEVO_FETCH_TRIGp_new = or3(_SEKO_WIN_FETCH_TRIGp_new, _SUZU_WIN_FIRST_TILEne_new, tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(XYMU_RENDERINGp)); // Schematic wrong, this is OR
  }();

  /* p27.NYXU*/ bool NYXU_BFETCH_RSTn;
  [
    this,
    NUNY_WIN_MODE_TRIGp_new_evn,
    AVAP_SCAN_DONE_TRIGp_new_xxx,
    TEVO_FETCH_TRIGp_new,

    &NYXU_BFETCH_RSTn
  ](){
    /* p27.NYFO*/ wire _NYFO_WIN_MODE_TRIGn_new_any = not1(NUNY_WIN_MODE_TRIGp_new_evn);
    /* p27.MOSU*/ wire _MOSU_WIN_MODE_TRIGp_new_any = not1(_NYFO_WIN_MODE_TRIGn_new_any);
    /* p27.NYXU*/ NYXU_BFETCH_RSTn = nor3(AVAP_SCAN_DONE_TRIGp_new_xxx, _MOSU_WIN_MODE_TRIGp_new_any, TEVO_FETCH_TRIGp_new);
  }();

  [
    this,
    XYMU_RENDERINGp,
    TYFA_CLKPIPE_odd,
    TEVO_FETCH_TRIGp_new
  ]() {
    // Fine match counter. Registers are only read as old, so this can go down as far in the list as needed.

    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new_evn = not1(XYMU_RENDERINGp);
    /*#p27.PASO*/ wire _PASO_FINE_RST_new = nor2(_PAHA_RENDERINGn_new_evn, TEVO_FETCH_TRIGp_new);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.RSTn(_PASO_FINE_RST_new);
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.RSTn(_PASO_FINE_RST_new);
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.RSTn(_PASO_FINE_RST_new);

    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_new_evn = nand3(fine_scroll.RUBU_FINE_CNT2.qp_new(), fine_scroll.ROGA_FINE_CNT1.qp_new(), fine_scroll.RYKU_FINE_CNT0.qp_new());
    /*#p27.PECU*/ wire _PECU_FINE_CLK_AxCxExGx_clknew_evn = nand2(_ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n_new_evn);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK_AxCxExGx_clknew_evn,      _PASO_FINE_RST_new, fine_scroll.RYKU_FINE_CNT0.qn_new());
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17(fine_scroll.RYKU_FINE_CNT0.qn_new(), _PASO_FINE_RST_new, fine_scroll.ROGA_FINE_CNT1.qn_new());
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17(fine_scroll.ROGA_FINE_CNT1.qn_new(), _PASO_FINE_RST_new, fine_scroll.RUBU_FINE_CNT2.qn_new());
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // OAM latch to temp storage. Might be able to move this to the bottom after the latches are set.

  oam_temp_a.latch_to_temp(
    BUS_CPU_A,
    ACYL_SCANNINGp,
    UVYT_ABCDxxxx_clk,
    vclk_reg.XYSO_xBCDxFGH(),
    MATU_DMA_RUNNINGp,
    sprite_fetcher.XUJY_OAM_CLKENp(XYMU_RENDERINGp),
    oam_latch_a);

  oam_temp_b.latch_to_temp(
    BUS_CPU_A,
    ACYL_SCANNINGp,
    UVYT_ABCDxxxx_clk,
    vclk_reg.XYSO_xBCDxFGH(),
    MATU_DMA_RUNNINGp,
    sprite_fetcher.XUJY_OAM_CLKENp(XYMU_RENDERINGp),
    oam_latch_b);

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this,
    XODO_VID_RSTp,
    ATAL_xBxDxFxH
  ]() {
    // Sprite hit line checker

    SpriteDeltaY delta = sprite_delta_y(oam_temp_a, reg_ly);

    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_new = delta.GESE_SCAN_MATCH_Yp(reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new());
    /* p29.CEHA*/ wire _CEHA_SCANNINGp_new_evn  = not1(sprite_scanner.CENO_SCANNINGp_evn.qn_new());
    /* p29.CARE*/ wire _CARE_COUNT_CLKn_new_odd = and3(vclk_reg.XOCE_xBCxxFGx(), _CEHA_SCANNINGp_new_evn, _GESE_SCAN_MATCH_Yp_new); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /* p29.DYTY*/ wire _DYTY_STORE_CLKp_new_odd = not1(_CARE_COUNT_CLKn_new_odd);

    sprite_store.update_count(XODO_VID_RSTp, ATAL_xBxDxFxH, lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp), _DYTY_STORE_CLKp_new_odd);

    SpriteStoreFlag store_flag = sprite_store.get_store_flag(_DYTY_STORE_CLKp_new_odd);

    sprite_store.store_sprite_index(store_flag, SPR_TRI_I);
    sprite_store.store_sprite_line (store_flag, SPR_TRI_L);
    sprite_store.store_sprite_x    (store_flag, oam_temp_b);
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this
  ](){
    wire VCC = 1;
    /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(oam_bus.BUS_OAM_An[2].qp_old());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(oam_bus.BUS_OAM_An[3].qp_old());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(oam_bus.BUS_OAM_An[4].qp_old());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(oam_bus.BUS_OAM_An[5].qp_old());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(oam_bus.BUS_OAM_An[6].qp_old());
    /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(oam_bus.BUS_OAM_An[7].qp_old());

    // Sprite store grabs the sprite index off the _old_ oam address bus
    /* p30.XADU*/ sprite_store.XADU_SPRITE_IDX0p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _YFOT_OAM_A2p_old);
    /* p30.XEDY*/ sprite_store.XEDY_SPRITE_IDX1p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _YFOC_OAM_A3p_old);
    /* p30.ZUZE*/ sprite_store.ZUZE_SPRITE_IDX2p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _YVOM_OAM_A4p_old);
    /* p30.XOBE*/ sprite_store.XOBE_SPRITE_IDX3p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _YMEV_OAM_A5p_old);
    /* p30.YDUF*/ sprite_store.YDUF_SPRITE_IDX4p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _XEMU_OAM_A6p_old);
    /* p30.XECU*/ sprite_store.XECU_SPRITE_IDX5p_evn.dff13(vclk_reg.WUDA_xxCDxxGH(), VCC, _YZET_OAM_A7p_old);
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this,
    XYMU_RENDERINGp
  ](){
    /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(sprite_scanner.CENO_SCANNINGp_evn.qn_new(), XYMU_RENDERINGp);
    /*#p30.WUZY*/ SPR_TRI_I[0].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.XADU_SPRITE_IDX0p_evn.qn_new());
    /* p30.WYSE*/ SPR_TRI_I[1].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.XEDY_SPRITE_IDX1p_evn.qn_new());
    /* p30.ZYSU*/ SPR_TRI_I[2].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.ZUZE_SPRITE_IDX2p_evn.qn_new());
    /* p30.WYDA*/ SPR_TRI_I[3].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.XOBE_SPRITE_IDX3p_evn.qn_new());
    /* p30.WUCO*/ SPR_TRI_I[4].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.YDUF_SPRITE_IDX4p_evn.qn_new());
    /* p30.WEZA*/ SPR_TRI_I[5].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_store.XECU_SPRITE_IDX5p_evn.qn_new());
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [
    this,
    FEPO_STORE_MATCHp_new_evn
  ](){
    wire GND = 0;

    /*#p29.EBOS*/ wire EBOS_LY0n_new_evn = not1(reg_ly.MUWY_LY0p_evn.qp_new());
    /* p29.DASA*/ wire DASA_LY1n_new_evn = not1(reg_ly.MYRO_LY1p_evn.qp_new());
    /* p29.FUKY*/ wire FUKY_LY2n_new_evn = not1(reg_ly.LEXA_LY2p_evn.qp_new());
    /* p29.FUVE*/ wire FUVE_LY3n_new_evn = not1(reg_ly.LYDO_LY3p_evn.qp_new());

    /* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), GND);
    /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), GND);
    /* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new(), _ERUC_YDIFF_C0_new);
    /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new(), _ERUC_YDIFF_C0_new);
    /* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p_evn.qp_new(), _FECO_YDIFF_C2_new);

    /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
    /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
    /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
    /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);

    /*#p30.CUCU*/ SPR_TRI_L[0].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DEGE_SPRITE_DELTA0_new);
    /*#p30.CUCA*/ SPR_TRI_L[1].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABY_SPRITE_DELTA1_new);
    /*#p30.CEGA*/ SPR_TRI_L[2].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABU_SPRITE_DELTA2_new);
    /*#p30.WENU*/ SPR_TRI_L[3].tri6_nn(FEPO_STORE_MATCHp_new_evn, _GYSA_SPRITE_DELTA3_new);
  }();

  tock_reg_stat(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    PARU_VBLANKp_new_evn
  );

  tile_fetcher.tock(ATAL_xBxDxFxH, NYXU_BFETCH_RSTn);

  [
    this,
    ATAL_xBxDxFxH,
    XYMU_RENDERINGp,
    NYXU_BFETCH_RSTn,
    MOCE_BFETCH_DONEn_old
  ](){
    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
    /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp_evn.dff17(MYVO_AxCxExGx(ATAL_xBxDxFxH), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
    /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn_new_evn = and2(tile_fetcher.LOVY_FETCH_DONEp_evn.qn_new(), XYMU_RENDERINGp);
    /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp_xxx.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn_new_evn);
  }();

  wire ABUZ_xxCDEFGH_clk_evn = [this, ATYP_ABCDxxxx, AROV_xxCDEFxx](){
    /*#p01.AJAX*/ wire _AJAX_xxxxEFGH_clk_evn = not1(ATYP_ABCDxxxx);
    /*#p01.AGUT*/ wire _AGUT_xxCDEFGH_clk_evn = or_and3(AROV_xxCDEFxx, _AJAX_xxxxEFGH_clk_evn, PIN_CPU_EXT_BUSp.qp_new());
    /*#p01.AWOD*/ wire _AWOD_ABxxxxxx_clk_evn = nor2(UNOR_MODE_DBG2p_ext(), _AGUT_xxCDEFGH_clk_evn);
    /*#p01.ABUZ*/ wire _ABUZ_xxCDEFGH_clk_evn = not1(_AWOD_ABxxxxxx_clk_evn);
    return _ABUZ_xxCDEFGH_clk_evn;
  }();

  [
    this,
    ATAL_xBxDxFxH,
    XYMU_RENDERINGp
  ](){
    /* p27.LAXU*/ wire LAXU_BFETCH_S0p_evn_new = tile_fetcher.LAXU_BFETCH_S0p_evn.qp_new();
    /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1_odd.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), XYMU_RENDERINGp, LAXU_BFETCH_S0p_evn_new);
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
    XODO_VID_RSTp,
    vclk_reg.TALU_xxCDEFxx(),
    TYFA_CLKPIPE_odd,
    SACU_CLKPIPE_evn,
    XONA_LCDC_LCDENp_h_new,
    XYMU_RENDERINGp,
    AVAP_SCAN_DONE_TRIGp_new_xxx,
    WEGO_HBLANKp_new_any,
    reg_lx.PURE_LINE_ENDn_new_evn(),
    REMY_LD0n_new,
    RAVO_LD1n_new
  );

  tock_joypad(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    BOGA_Axxxxxxx_clkevn,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  );

  tock_serial(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp
  );

  tock_interrupts(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    FEPO_STORE_MATCHp_new_evn,
    PARU_VBLANKp_new_evn,
    reg_lx.PURE_LINE_ENDn_new_evn(),
    MOBA_TIMER_OVERFLOWp_evn_new,
    WODU_HBLANKp_new);

  tock_ext(
    BUS_CPU_A,
    BUS_CPU_D,
    ABUZ_xxCDEFGH_clk_evn,
    TEDO_CPU_RDp,
    APOV_CPU_WRp_clkevn,
    TEXO_ADDR_VRAMn_ext,
    bootrom.TUTU_READ_BOOTROMp_new(BUS_CPU_A)
  );

  tock_vram(
    BUS_CPU_A,
    BUS_CPU_D,

    AVOR_SYS_RSTp,
    XODO_VID_RSTp,

    ATAL_xBxDxFxH,
    ABUZ_xxCDEFGH_clk_evn,

    CATY_LATCH_EXTp_ext,
    SOSE_ADDR_VRAMp_ext,

    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    APOV_CPU_WRp_clkevn,

    TUTO_DBG_VRAMp_new,

    lcd.ATEJ_LINE_TRIGp(XODO_VID_RSTp),
    TEVO_FETCH_TRIGp_new,
    NYXU_BFETCH_RSTn,
    PARU_VBLANKp_new_evn,
    PORE_WIN_MODEp_new_evn,
    WYMO_LCDC_WINENn_new,
    XYMU_RENDERINGp
  );

  tock_oam(
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
    ATAL_xBxDxFxH,
    UVYT_ABCDxxxx_clk,
    vclk_reg.XOCE_xBCxxFGx(),
    vclk_reg.XYSO_xBCDxFGH(),
    TAPU_CPU_WRp,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp_ext,
    MATU_DMA_RUNNINGp,
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// STAT register

void GateBoy::tock_reg_stat(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  wire ACYL_SCANNINGp,
  wire XYMU_RENDERINGp,
  wire PARU_VBLANKp)
{
  // FF41 STAT
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VARY_FF41p_ext(BUS_CPU_A));
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_clk = not1(_SEPA_FF41_WRp_clk);
  /* p21.ROXE*/ reg_stat.ROXE_STAT_HBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
  /* p21.RUFO*/ reg_stat.RUFO_STAT_VBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
  /* p21.REFE*/ reg_stat.REFE_STAT_OAI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
  /* p21.RUGU*/ reg_stat.RUGU_STAT_LYI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);

  /* p21.RYJU*/ wire _RYJU_FF41_WRn_clk_evn = not1(_SEPA_FF41_WRp_clk);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST_new_evn = or2(WESY_SYS_RSTn(AVOR_SYS_RSTp), _RYJU_FF41_WRn_clk_evn);
  /* p21.RUPO*/ ppu_reg.RUPO_STAT_LYC_MATCHn_evn.nor_latch(_PAGO_LYC_MATCH_RST_new_evn, reg_lyc.ROPO_LY_MATCH_SYNCp_c.qp_new());

  // FF41 STAT
  /* p21.SADU*/ wire _SADU_STAT_MODE0n_new  = nor2(XYMU_RENDERINGp, PARU_VBLANKp); // die NOR
  /* p21.XATY*/ wire _XATY_STAT_MODE1n_new  = nor2(ACYL_SCANNINGp, XYMU_RENDERINGp); // die NOR

  /* p21.TOBE*/ wire _TOBE_FF41_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VARY_FF41p_ext(BUS_CPU_A));
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_ext = not1(_TOBE_FF41_RDp_ext);
  /*#p21.TEBY*/ BUS_CPU_D_out[0].tri6_pn(_TOBE_FF41_RDp_ext, _SADU_STAT_MODE0n_new);
  /*#p21.WUGA*/ BUS_CPU_D_out[1].tri6_pn(_TOBE_FF41_RDp_ext, _XATY_STAT_MODE1n_new);
  /*#p21.SEGO*/ BUS_CPU_D_out[2].tri6_pn(_TOBE_FF41_RDp_ext, ppu_reg.RUPO_STAT_LYC_MATCHn_evn.qp_new());
  /* p21.PUZO*/ BUS_CPU_D_out[3].tri6_nn(_VAVE_FF41_RDn_ext, reg_stat.ROXE_STAT_HBI_ENn_h.qp_new());
  /* p21.POFO*/ BUS_CPU_D_out[4].tri6_nn(_VAVE_FF41_RDn_ext, reg_stat.RUFO_STAT_VBI_ENn_h.qp_new());
  /* p21.SASY*/ BUS_CPU_D_out[5].tri6_nn(_VAVE_FF41_RDn_ext, reg_stat.REFE_STAT_OAI_ENn_h.qp_new());
  /* p21.POTE*/ BUS_CPU_D_out[6].tri6_nn(_VAVE_FF41_RDn_ext, reg_stat.RUGU_STAT_LYI_ENn_h.qp_new());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Serial

void GateBoy::tock_serial(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp)
{

  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);

  //----------

  /* hack */
  PIN_SIN.pin_in(1, 1);
  PIN_SCK.pin_in(0, 0);

  bool DAWA_SER_clknew;
  {
    /* p01.UVYN*/ wire _UVYN_DIV_05n_next = not1(div_reg.TAMA_DIV05p_evn.qp_new());
    /* p06.UWAM*/ wire _UWAM_FF02_WRn_clk = nand4(TAPU_CPU_WRp, SANO_FF00_FF03p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n_ext(BUS_CPU_A));
    /* p06.COTY*/ ser_reg.COTY_SER_CLK .dff17(_UVYN_DIV_05n_next, _UWAM_FF02_WRn_clk, ser_reg.COTY_SER_CLK.qn_old());
    /* p06.CULY*/ ser_reg.CULY_XFER_DIR.dff17(_UWAM_FF02_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[0]);

    /* p06.CARO*/ wire _CARO_SER_RST_new = and2(_UWAM_FF02_WRn_clk, _ALUR_SYS_RSTn_new);
    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.RSTn(_CARO_SER_RST_new);
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.RSTn(_CARO_SER_RST_new);
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.RSTn(_CARO_SER_RST_new);
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.RSTn(_CARO_SER_RST_new);

    /* p06.COBA*/ wire _COBA_SER_CNT3n_new  = not1(ser_reg.CALY_SER_CNT3.qp_new());
    /* p06.CABY*/ wire _CABY_XFER_RESET_new = and2(_COBA_SER_CNT3n_new, _ALUR_SYS_RSTn_new);
    /* p06.ETAF*/ ser_reg.ETAF_SER_RUNNING.dff17(_UWAM_FF02_WRn_clk, _CABY_XFER_RESET_new, BUS_CPU_D[7]);

    /* p06.CAVE*/ wire _CAVE_SER_clknew = mux2n(ser_reg.CULY_XFER_DIR.qp_new(), ser_reg.COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
    /* p06.DAWA*/ wire _DAWA_SER_clknew = or2(_CAVE_SER_clknew, ser_reg.ETAF_SER_RUNNING.qn_new());

    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_clknew,               _CARO_SER_RST_new, ser_reg.CAFA_SER_CNT0.qn_new());
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17(ser_reg.CAFA_SER_CNT0.qn_new(), _CARO_SER_RST_new, ser_reg.CYLO_SER_CNT1.qn_new());
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17(ser_reg.CYLO_SER_CNT1.qn_new(), _CARO_SER_RST_new, ser_reg.CYDE_SER_CNT2.qn_new());
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17(ser_reg.CYDE_SER_CNT2.qn_new(), _CARO_SER_RST_new, ser_reg.CALY_SER_CNT3.qn_new());

    DAWA_SER_clknew =_DAWA_SER_clknew;
  }

  {
    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(DAWA_SER_clknew);
    /* p06.ELYS*/ ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK_new, _ALUR_SYS_RSTn_new, ser_reg.EDER_SER_DATA7.qp_old());
    /* p05.KENA*/ wire _KENA = /*mux2n(KUKO_DBG_FF00_D6,*/ ser_reg.ELYS_SER_OUT.qp_new() /*, FF60_0)*/; // FIXME hacking out debug stuff

    PIN_SOUT.pin_out(_KENA, _KENA);
  }

  {
    /* p06.KEXU*/ wire _KEXU_new = nand2(ser_reg.COTY_SER_CLK.qp_new(), ser_reg.CULY_XFER_DIR.qp_new());
    /* p06.JAGO*/ wire _JAGO_new = not1(ser_reg.CULY_XFER_DIR.qp_new());
    /* p06.KUJO*/ wire _KUJO_new = nor2(ser_reg.COTY_SER_CLK.qp_new(), _JAGO_new);

    PIN_SCK.pin_out(ser_reg.CULY_XFER_DIR.qp_new(), _KEXU_new, _KUJO_new);
  }

  {
    /* p06.URYS*/ wire _URYS_FF01_WRn_clk   = nand4(TAPU_CPU_WRp, SANO_FF00_FF03p_ext(BUS_CPU_A), TOLA_A01n_ext(BUS_CPU_A), BUS_CPU_A[ 0]);

    /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[0], _ALUR_SYS_RSTn_new);
    /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[1], _ALUR_SYS_RSTn_new);
    /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[2], _ALUR_SYS_RSTn_new);
    /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[3], _ALUR_SYS_RSTn_new);
    /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[4], _ALUR_SYS_RSTn_new);
    /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[5], _ALUR_SYS_RSTn_new);
    /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[6], _ALUR_SYS_RSTn_new);
    /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[7], _ALUR_SYS_RSTn_new);

    /* p06.DAKU*/ wire _DAKU_FF01_WRp_clk = not1(_URYS_FF01_WRn_clk);
    /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn_new = nand2(BUS_CPU_D[0], _DAKU_FF01_WRp_clk);
    /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn_new = nand2(BUS_CPU_D[1], _DAKU_FF01_WRp_clk);
    /* p06.DELA*/ wire _DELA_SER_DATA2_SETn_new = nand2(BUS_CPU_D[2], _DAKU_FF01_WRp_clk);
    /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn_new = nand2(BUS_CPU_D[3], _DAKU_FF01_WRp_clk);
    /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn_new = nand2(BUS_CPU_D[4], _DAKU_FF01_WRp_clk);
    /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn_new = nand2(BUS_CPU_D[5], _DAKU_FF01_WRp_clk);
    /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn_new = nand2(BUS_CPU_D[6], _DAKU_FF01_WRp_clk);
    /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn_new = nand2(BUS_CPU_D[7], _DAKU_FF01_WRp_clk);

    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(DAWA_SER_clknew);
    /* p06.EPYT*/ wire _EPYT_SER_CLK_new = not1(_EDYL_SER_CLK_new);
    /* p06.DEHO*/ wire _DEHO_SER_CLK_new = not1(_EPYT_SER_CLK_new);
    /* p06.DAWE*/ wire _DAWE_SER_CLK_new = not1(_DEHO_SER_CLK_new);

    /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(PIN_SIN.qp_new());

    /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK_new, _EFEL_SER_DATA7_SETn_new, _EGUV_SER_DATA7_RSTn_new, ser_reg.EROD_SER_DATA6.qp_old());
    /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK_new, _EDEL_SER_DATA6_SETn_new, _EFAK_SER_DATA6_RSTn_new, ser_reg.EJAB_SER_DATA5.qp_old());
    /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK_new, _ELOK_SER_DATA5_SETn_new, _EHUJ_SER_DATA5_RSTn_new, ser_reg.DOVU_SER_DATA4.qp_old());
    /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK_new, _DOLA_SER_DATA4_SETn_new, _DYLY_SER_DATA4_RSTn_new, ser_reg.DOJO_SER_DATA3.qp_old());
    /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK_new, _DYGE_SER_DATA3_SETn_new, _DAJU_SER_DATA3_RSTn_new, ser_reg.DYRA_SER_DATA2.qp_old());
    /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK_new, _DELA_SER_DATA2_SETn_new, _DYBO_SER_DATA2_RSTn_new, ser_reg.DEGU_SER_DATA1.qp_old());
    /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK_new, _DOCU_SER_DATA1_SETn_new, _DUMO_SER_DATA1_RSTn_new, ser_reg.CUBA_SER_DATA0.qp_old());
    /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK_new, _CUFU_SER_DATA0_SETn_new, _COHY_SER_DATA0_RSTn_new, _CAGE_SER_IN_new);
  }

  {
    /* p06.UFEG*/ wire _UFEG_FF01_RDp_ext   =  and4(TEDO_CPU_RDp, SANO_FF00_FF03p_ext(BUS_CPU_A),  TOLA_A01n_ext(BUS_CPU_A), BUS_CPU_A[ 0]);
    /* p06.UCOM*/ wire _UCOM_FF02_RDp_ext   =  and4(TEDO_CPU_RDp, SANO_FF00_FF03p_ext(BUS_CPU_A),  BUS_CPU_A[ 1],            TOVY_A00n_ext(BUS_CPU_A));

    /* FF01 SER */
    /*#p06.CUGY*/ BUS_CPU_D_out[0].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.CUBA_SER_DATA0.qn_new());
    /* p06.DUDE*/ BUS_CPU_D_out[1].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DEGU_SER_DATA1.qn_new());
    /* p06.DETU*/ BUS_CPU_D_out[2].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DYRA_SER_DATA2.qn_new());
    /* p06.DASO*/ BUS_CPU_D_out[3].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DOJO_SER_DATA3.qn_new());
    /* p06.DAME*/ BUS_CPU_D_out[4].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DOVU_SER_DATA4.qn_new());
    /* p06.EVOK*/ BUS_CPU_D_out[5].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EJAB_SER_DATA5.qn_new());
    /* p06.EFAB*/ BUS_CPU_D_out[6].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EROD_SER_DATA6.qn_new());
    /*#p06.ETAK*/ BUS_CPU_D_out[7].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EDER_SER_DATA7.qn_new());

    /* FF02 SER */
    /* p06.CORE*/ BUS_CPU_D_out[0].tri6_pn(_UCOM_FF02_RDp_ext, ser_reg.CULY_XFER_DIR.qn_new());
    /* p06.ELUV*/ BUS_CPU_D_out[7].tri6_pn(_UCOM_FF02_RDp_ext, ser_reg.ETAF_SER_RUNNING.qn_new());
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_interrupts(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  wire FEPO_STORE_MATCHp_new_evn,
  wire PARU_VBLANKp_new_evn,
  wire PURE_LINE_ENDn_new_evn,
  wire MOBA_TIMER_OVERFLOWp_evn_new,
  wire WODU_HBLANKp_new_odd)
{
  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);

  /* p07.ROLO*/ wire _ROLO_FF0F_RDn_ext = nand4(TEDO_CPU_RDp,    SYKE_ADDR_HIp_ext(BUS_CPU_A),  SEMY_XX_0000xxxxp_ext(BUS_CPU_A), SAPA_XX_xxxx1111p_ext(BUS_CPU_A)); // schematic wrong, is NAND
  /* p07.REFA*/ wire _REFA_FF0F_WRn_clk = nand4(TAPU_CPU_WRp, SYKE_ADDR_HIp_ext(BUS_CPU_A),  SEMY_XX_0000xxxxp_ext(BUS_CPU_A), SAPA_XX_xxxx1111p_ext(BUS_CPU_A)); // schematic wrong, is NAND

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  PIN_CPU_ACK_VBLANK.setp(wire(cpu.int_ack & INT_VBLANK_MASK));
  PIN_CPU_ACK_STAT  .setp(wire(cpu.int_ack & INT_STAT_MASK));
  PIN_CPU_ACK_TIMER .setp(wire(cpu.int_ack & INT_TIMER_MASK));
  PIN_CPU_ACK_SERIAL.setp(wire(cpu.int_ack & INT_SERIAL_MASK));
  PIN_CPU_ACK_JOYPAD.setp(wire(cpu.int_ack & INT_JOYPAD_MASK));

  /* p02.LETY*/ wire _LETY_INT_VBL_ACKn_ext  = not1(PIN_CPU_ACK_VBLANK.qp_new());
  /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn_ext = not1(PIN_CPU_ACK_STAT.qp_new());
  /* p02.LESA*/ wire _LESA_INT_TIM_ACKn_ext  = not1(PIN_CPU_ACK_TIMER.qp_new());
  /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn_ext  = not1(PIN_CPU_ACK_SERIAL.qp_new());
  /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn_ext  = not1(PIN_CPU_ACK_JOYPAD.qp_new());

  /* p02.ROTU*/ wire _ROTU_FF0F_WRp_clk = not1(_REFA_FF0F_WRn_clk);
  /* p02.MYZU*/ wire _MYZU_FF0F_SET0n_new = nand3(_ROTU_FF0F_WRp_clk, _LETY_INT_VBL_ACKn_ext,  BUS_CPU_D[0]);
  /* p02.MODY*/ wire _MODY_FF0F_SET1n_new = nand3(_ROTU_FF0F_WRp_clk, _LEJA_INT_STAT_ACKn_ext, BUS_CPU_D[1]);
  /* p02.PYHU*/ wire _PYHU_FF0F_SET2n_new = nand3(_ROTU_FF0F_WRp_clk, _LESA_INT_TIM_ACKn_ext,  BUS_CPU_D[2]);
  /* p02.TOME*/ wire _TOME_FF0F_SET3n_new = nand3(_ROTU_FF0F_WRp_clk, _LUFE_INT_SER_ACKn_ext,  BUS_CPU_D[3]);
  /* p02.TOGA*/ wire _TOGA_FF0F_SET4n_new = nand3(_ROTU_FF0F_WRp_clk, _LAMO_INT_JOY_ACKn_ext,  BUS_CPU_D[4]);

  /*#p02.MUXE*/ wire _MUXE_INT0_WRn_new = or2(BUS_CPU_D[0], _REFA_FF0F_WRn_clk);
  /* p02.NABE*/ wire _NABE_INT1_WRn_new = or2(BUS_CPU_D[1], _REFA_FF0F_WRn_clk);
  /* p02.RAKE*/ wire _RAKE_INT2_WRn_new = or2(BUS_CPU_D[2], _REFA_FF0F_WRn_clk);
  /* p02.SULO*/ wire _SULO_INT3_WRn_new = or2(BUS_CPU_D[3], _REFA_FF0F_WRn_clk);
  /* p02.SEME*/ wire _SEME_INT4_WRn_new = or2(BUS_CPU_D[4], _REFA_FF0F_WRn_clk);

  /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n_new = and3(_MUXE_INT0_WRn_new, _LETY_INT_VBL_ACKn_ext,  _ALUR_SYS_RSTn_new);
  /* p02.MOVU*/ wire _MOVU_FF0F_RST1n_new = and3(_NABE_INT1_WRn_new, _LEJA_INT_STAT_ACKn_ext, _ALUR_SYS_RSTn_new);
  /* p02.PYGA*/ wire _PYGA_FF0F_RST2n_new = and3(_RAKE_INT2_WRn_new, _LESA_INT_TIM_ACKn_ext,  _ALUR_SYS_RSTn_new);
  /* p02.TUNY*/ wire _TUNY_FF0F_RST3n_new = and3(_SULO_INT3_WRn_new, _LUFE_INT_SER_ACKn_ext,  _ALUR_SYS_RSTn_new);
  /* p02.TYME*/ wire _TYME_FF0F_RST4n_new = and3(_SEME_INT4_WRn_new, _LAMO_INT_JOY_ACKn_ext,  _ALUR_SYS_RSTn_new);

  /*#p21.TOLU*/ wire _TOLU_VBLANKn_new    = not1(PARU_VBLANKp_new_evn);
  /*#p21.SELA*/ wire _SELA_LINE_ENDp_new  = not1(PURE_LINE_ENDn_new_evn);
  /*#p21.TAPA*/ wire _TAPA_INT_OAM_t3_new   = and2(_TOLU_VBLANKn_new, _SELA_LINE_ENDp_new);
  /*#p21.TARU*/ wire _TARU_INT_HBL_t3_new   = and2(WODU_HBLANKp_new_odd, _TOLU_VBLANKn_new);
  /*#p21.SUKO*/ wire _SUKO_INT_STATp_t3_new = amux4(reg_stat.RUGU_STAT_LYI_ENn_h.qn_new(), reg_lyc.ROPO_LY_MATCH_SYNCp_c.qp_new(),
                                                    reg_stat.REFE_STAT_OAI_ENn_h.qn_new(), _TAPA_INT_OAM_t3_new,
                                                    reg_stat.RUFO_STAT_VBI_ENn_h.qn_new(), PARU_VBLANKp_new_evn, // polarity?
                                                    reg_stat.ROXE_STAT_HBI_ENn_h.qn_new(), _TARU_INT_HBL_t3_new);

  /*#p21.VYPU*/ wire _VYPU_INT_VBLANKp_t3_new = not1(_TOLU_VBLANKn_new);
  /* p02.ASOK*/ wire _ASOK_INT_JOYp_new       = and2(joypad.APUG_JP_GLITCH3.qp_new(), joypad.BATU_JP_GLITCH0.qp_new());
  /*#p21.TUVA*/ wire _TUVA_INT_STATn_t3_new   = not1(_SUKO_INT_STATp_t3_new);
  /*#p21.VOTY*/ wire _VOTY_INT_STATp_t3_new   = not1(_TUVA_INT_STATn_t3_new);

  wire _PESU_VCC = 1;
  /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22(_VYPU_INT_VBLANKp_t3_new,       _MYZU_FF0F_SET0n_new, _LYTA_FF0F_RST0n_new, _PESU_VCC);
  /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22(_VOTY_INT_STATp_t3_new,         _MODY_FF0F_SET1n_new, _MOVU_FF0F_RST1n_new, _PESU_VCC);
  /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22(MOBA_TIMER_OVERFLOWp_evn_new,   _PYHU_FF0F_SET2n_new, _PYGA_FF0F_RST2n_new, _PESU_VCC);
  /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22(ser_reg.CALY_SER_CNT3.qp_new(), _TOME_FF0F_SET3n_new, _TUNY_FF0F_RST3n_new, _PESU_VCC);
  /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22(_ASOK_INT_JOYp_new,             _TOGA_FF0F_SET4n_new, _TYME_FF0F_RST4n_new, _PESU_VCC);

  // FIXME this inversion fixes a bunch of tests...
  // MATY is one of those big yellow latchy things
  /* p02.MATY*/ int_reg.MATY_FF0F_L0p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ int_reg.MOPO_FF0F_L1p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ int_reg.PAVY_FF0F_L2p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ int_reg.NEJY_FF0F_L3p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ int_reg.NUTY_FF0F_L4p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  PIN_CPU_INT_VBLANK.setp(int_reg.LOPE_FF0F_D0p.qp_new());
  PIN_CPU_INT_STAT  .setp(int_reg.LALU_FF0F_D1p.qp_new());
  PIN_CPU_INT_TIMER .setp(int_reg.NYBO_FF0F_D2p.qp_new());
  PIN_CPU_INT_SERIAL.setp(int_reg.UBUL_FF0F_D3p.qp_new());
  PIN_CPU_INT_JOYPAD.setp(int_reg.ULAK_FF0F_D4p.qp_new());

  if (DELTA_DE) {
    int_vblank_halt = int_reg.LOPE_FF0F_D0p.qp_new();
    int_stat_halt   = int_reg.LALU_FF0F_D1p.qp_new();
    int_serial_halt = int_reg.UBUL_FF0F_D3p.qp_new();
    int_joypad_halt = int_reg.ULAK_FF0F_D4p.qp_new();
  }

  if (DELTA_GH) {
    // this one latches funny, some hardware bug
    int_timer_halt = int_reg.NYBO_FF0F_D2p.qp_new();

    int_vblank = int_reg.LOPE_FF0F_D0p.qp_new();
    int_stat   = int_reg.LALU_FF0F_D1p.qp_new();
    int_timer  = int_reg.NYBO_FF0F_D2p.qp_new();
    int_serial = int_reg.UBUL_FF0F_D3p.qp_new();
    int_joypad = int_reg.ULAK_FF0F_D4p.qp_new();
  }

  // FF0F INTF
  /* p02.POLA*/ wire _POLA_FF0F_RDp_ext = not1(_ROLO_FF0F_RDn_ext);
  /*#p02.NELA*/ BUS_CPU_D_out[0].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.MATY_FF0F_L0p.qn_new());
  /*#p02.NABO*/ BUS_CPU_D_out[1].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA*/ BUS_CPU_D_out[2].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO*/ BUS_CPU_D_out[3].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY*/ BUS_CPU_D_out[4].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.NUTY_FF0F_L4p.qn_new());

  {
    // FFFF - IE
    // This is technically in the CPU, but we're going to implement it here for now.

    uint16_t cpu_addr = pack_u16p(16, BUS_CPU_A);
    wire FFFF_HIT_ext = cpu_addr == 0xFFFF;
    wire FFFF_RDn_ext = nand2(TEDO_CPU_RDp, FFFF_HIT_ext);
    wire FFFF_WRn_ext = nand2(TAPU_CPU_WRp, FFFF_HIT_ext);

    IE_D0.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_D[0]);
    IE_D1.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_D[1]);
    IE_D2.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_D[2]);
    IE_D3.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_D[3]);
    IE_D4.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_D[4]);

    BUS_CPU_D_out[0].tri6_nn(FFFF_RDn_ext, IE_D0.qn_new());
    BUS_CPU_D_out[1].tri6_nn(FFFF_RDn_ext, IE_D1.qn_new());
    BUS_CPU_D_out[2].tri6_nn(FFFF_RDn_ext, IE_D2.qn_new());
    BUS_CPU_D_out[3].tri6_nn(FFFF_RDn_ext, IE_D3.qn_new());
    BUS_CPU_D_out[4].tri6_nn(FFFF_RDn_ext, IE_D4.qn_new());
  }

  if (DELTA_HA) {
    imask_latch = pack_u8p(5, &IE_D0);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Joypad

void GateBoy::tock_joypad(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire BOGA_Axxxxxxx_clkevn,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp)
{

  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);

  /* p10.ACAT*/ wire _ACAT_FF00_RDp_ext     =  and4(TEDO_CPU_RDp,    ANAP_FF_0xx00000_ext(BUS_CPU_A), AKUG_A06n_ext(BUS_CPU_A), BYKO_A05n_ext(BUS_CPU_A));
  /* p10.ATOZ*/ wire _ATOZ_FF00_WRn_clk     = nand4(TAPU_CPU_WRp, ANAP_FF_0xx00000_ext(BUS_CPU_A), AKUG_A06n_ext(BUS_CPU_A), BYKO_A05n_ext(BUS_CPU_A));

  // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
  // That also means that _both_ P14 and P15 are selected at reset :/
  /* p05.JUTE*/ joypad.JUTE_JOYP_RA     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[0]);
  /* p05.KECY*/ joypad.KECY_JOYP_LB     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[1]);
  /* p05.JALE*/ joypad.JALE_JOYP_UC     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[2]);
  /* p05.KYME*/ joypad.KYME_JOYP_DS     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[3]);
  /* p05.KELY*/ joypad.KELY_JOYP_UDLRp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[4]);
  /* p05.COFY*/ joypad.COFY_JOYP_ABCSp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[5]);
  /* p05.KUKO*/ joypad.KUKO_DBG_FF00_D6n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[6]);
  /* p05.KERU*/ joypad.KERU_DBG_FF00_D7n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[7]);

  wire BURO_FF60_0p_new = 0; // FIXME hacking out debug stuff
  /* p05.KURA*/ wire _KURA_JOYP_DBGn_new = not1(BURO_FF60_0p_new);

  /*
  // lcd ribbon voltages after bootrom
  04 5 left & b
  05 0 diodes 1&2
  06 5 down & start
  07 5 up & select
  08 5 right & a
  09 0 diodes 3 & 4
  */

  /* p05.KARU*/ wire _KARU_OEp_new = or2(joypad.KELY_JOYP_UDLRp.qn_new(), _KURA_JOYP_DBGn_new);
  /* p05.CELA*/ wire _CELA_OEp_new = or2(joypad.COFY_JOYP_ABCSp.qn_new(), _KURA_JOYP_DBGn_new);

  PIN_JOY_P14.pin_out(_KARU_OEp_new, joypad.KELY_JOYP_UDLRp.qn_new(), joypad.KELY_JOYP_UDLRp.qn_new());
  PIN_JOY_P15.pin_out(_CELA_OEp_new, joypad.COFY_JOYP_ABCSp.qn_new(), joypad.COFY_JOYP_ABCSp.qn_new());


#if 0
  /* p05.KOLE*/ wire _KOLE = nand2(joypad.JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
  /* p05.KYBU*/ wire _KYBU = nor2 (joypad.JUTE_JOYP_RA.qp17(), _KURA);
  /* p05.KYTO*/ wire _KYTO = nand2(joypad.KECY_JOYP_LB.qp17(), BURO_FF60_0p);
  /* p05.KABU*/ wire _KABU = nor2 (joypad.KECY_JOYP_LB.qp17(), _KURA);
  /* p05.KYHU*/ wire _KYHU = nand2(joypad.JALE_JOYP_UC.qp17(), BURO_FF60_0p);
  /* p05.KASY*/ wire _KASY = nor2 (joypad.JALE_JOYP_UC.qp17(), _KURA);
  /* p05.KORY*/ wire _KORY = nand2(joypad.KYME_JOYP_DS.qp17(), BURO_FF60_0p);
  /* p05.KALE*/ wire _KALE = nor2 (joypad.KYME_JOYP_DS.qp17(), _KURA);

  PIN_JOY_P10.pin_out(_KOLE, _KYBU);
  PIN_JOY_P11.pin_out(_KYTO, _KABU);
  PIN_JOY_P12.pin_out(_KYHU, _KASY);
  PIN_JOY_P13.pin_out(_KORY, _KALE);
#endif


  if (PIN_JOY_P14.qp_new()) {
    PIN_JOY_P10.setn(sys_buttons & 0x01);
    PIN_JOY_P11.setn(sys_buttons & 0x02);
    PIN_JOY_P12.setn(sys_buttons & 0x04);
    PIN_JOY_P13.setn(sys_buttons & 0x08);
  }
  else if (PIN_JOY_P15.qp_new()) {
    PIN_JOY_P10.setn(sys_buttons & 0x10);
    PIN_JOY_P11.setn(sys_buttons & 0x20);
    PIN_JOY_P12.setn(sys_buttons & 0x40);
    PIN_JOY_P13.setn(sys_buttons & 0x80);
  }
  else {
    PIN_JOY_P10.setn(0);
    PIN_JOY_P11.setn(0);
    PIN_JOY_P12.setn(0);
    PIN_JOY_P13.setn(0);
  }
  /* p02.KERY*/ wire _KERY_ANY_BUTTONp_ext = or4(PIN_JOY_P13.qp_new(), PIN_JOY_P12.qp_new(), PIN_JOY_P11.qp_new(), PIN_JOY_P10.qp_new());

  /* p02.AWOB*/ joypad.AWOB_WAKE_CPU.tp_latch(BOGA_Axxxxxxx_clkevn, _KERY_ANY_BUTTONp_ext);
  wire _AWOB_WAKE_CPUp_new = joypad.AWOB_WAKE_CPU.qp_new();

  PIN_CPU_WAKE.setp(_AWOB_WAKE_CPUp_new);

  /* p02.APUG*/ joypad.APUG_JP_GLITCH3.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, joypad.AGEM_JP_GLITCH2.qp_old());
  /* p02.AGEM*/ joypad.AGEM_JP_GLITCH2.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, joypad.ACEF_JP_GLITCH1.qp_old());
  /* p02.ACEF*/ joypad.ACEF_JP_GLITCH1.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, joypad.BATU_JP_GLITCH0.qp_old());
  /* p02.BATU*/ joypad.BATU_JP_GLITCH0.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, _KERY_ANY_BUTTONp_ext);

  /* p05.BYZO*/ wire _BYZO_FF00_RDn_ext = not1(_ACAT_FF00_RDp_ext);
  /* p05.KEVU*/ joypad.KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P10.qp_new());
  /* p05.KAPA*/ joypad.KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P11.qp_new());
  /* p05.KEJA*/ joypad.KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P12.qp_new());
  /* p05.KOLO*/ joypad.KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P13.qp_new());

    // FF00 P1 / JOYP
  /* p05.KEMA*/ BUS_CPU_D_out[0].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KEVU_JOYP_L0n.qp_new());
  /* p05.KURO*/ BUS_CPU_D_out[1].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KAPA_JOYP_L1n.qp_new());
  /* p05.KUVE*/ BUS_CPU_D_out[2].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KEJA_JOYP_L2n.qp_new());
  /* p05.JEKU*/ BUS_CPU_D_out[3].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KOLO_JOYP_L3n.qp_new());
  /* p05.KOCE*/ BUS_CPU_D_out[4].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KELY_JOYP_UDLRp.qn_new());
  /* p05.CUDY*/ BUS_CPU_D_out[5].tri6_nn(_BYZO_FF00_RDn_ext, joypad.COFY_JOYP_ABCSp.qn_new());
  /* p??.????*/ BUS_CPU_D_out[6].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KUKO_DBG_FF00_D6n.qp_new());
  /* p??.????*/ BUS_CPU_D_out[7].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KERU_DBG_FF00_D7n.qp_new());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

    /* p32.LEGU*/ tile_fetcher.LEGU_TILE_DA0n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[0].qp_old());
    /* p32.NUDU*/ tile_fetcher.NUDU_TILE_DA1n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[1].qp_old());
    /* p32.MUKU*/ tile_fetcher.MUKU_TILE_DA2n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[2].qp_old());
    /* p32.LUZO*/ tile_fetcher.LUZO_TILE_DA3n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[3].qp_old());
    /* p32.MEGU*/ tile_fetcher.MEGU_TILE_DA4n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[4].qp_old());
    /* p32.MYJY*/ tile_fetcher.MYJY_TILE_DA5n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[5].qp_old());
    /* p32.NASA*/ tile_fetcher.NASA_TILE_DA6n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[6].qp_old());
    /* p32.NEFO*/ tile_fetcher.NEFO_TILE_DA7n_odd.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp[7].qp_old());

    /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp_new = and2(_MYSO_STORE_VRAM_DATA_TRIGp_new, _NAKO_BFETCH_S1n_new_evn); // MOFU fires on fetch phase 2 and 10
    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn_new = not1(_MOFU_LATCH_TILE_DBp_new);
    /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp_new = not1(_LESO_LATCH_TILE_DBn_new); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn_new = not1(_LUVE_LATCH_TILE_DBp_new);

    /* p32.RAWU*/ tile_fetcher.RAWU_TILE_DB0p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[0].qp_old()); // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /* p32.POZO*/ tile_fetcher.POZO_TILE_DB1p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[1].qp_old());
    /* p32.PYZO*/ tile_fetcher.PYZO_TILE_DB2p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[2].qp_old());
    /* p32.POXA*/ tile_fetcher.POXA_TILE_DB3p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[3].qp_old());
    /* p32.PULO*/ tile_fetcher.PULO_TILE_DB4p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[4].qp_old());
    /* p32.POJU*/ tile_fetcher.POJU_TILE_DB5p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[5].qp_old());
    /* p32.POWY*/ tile_fetcher.POWY_TILE_DB6p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[6].qp_old());
    /* p32.PYJU*/ tile_fetcher.PYJU_TILE_DB7p_odd.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp[7].qp_old());
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
    /* p33.PUTE*/ wire _PUTE_FLIP0p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[7].qp_old(), BUS_VRAM_Dp[0].qp_old());
    /* p33.PELO*/ wire _PELO_FLIP1p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[6].qp_old(), BUS_VRAM_Dp[1].qp_old());
    /* p33.PONO*/ wire _PONO_FLIP2p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[5].qp_old(), BUS_VRAM_Dp[2].qp_old());
    /* p33.POBE*/ wire _POBE_FLIP3p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[4].qp_old(), BUS_VRAM_Dp[3].qp_old());
    /* p33.PACY*/ wire _PACY_FLIP4p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[3].qp_old(), BUS_VRAM_Dp[4].qp_old());
    /* p33.PUGU*/ wire _PUGU_FLIP5p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[2].qp_old(), BUS_VRAM_Dp[5].qp_old());
    /* p33.PAWE*/ wire _PAWE_FLIP6p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[1].qp_old(), BUS_VRAM_Dp[6].qp_old());
    /* p33.PULY*/ wire _PULY_FLIP7p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp[0].qp_old(), BUS_VRAM_Dp[7].qp_old());


    /*#p29.TOPU*/ wire _TOPU_CLKp_new = and2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.SYCU_SFETCH_S0pe(XYMU_RENDERINGp));
    /*#p29.VYWA*/ wire _VYWA_CLKn_new = not1(_TOPU_CLKp_new);
    /*#p29.WENY*/ wire _WENY_CLKp_new = not1(_VYWA_CLKn_new);
    /*#p29.XADO*/ wire _XADO_CLKn_new = not1(_WENY_CLKp_new);
    /* p33.REWO*/ vram_bus.REWO_SPRITE_DA0n.dff8n(_XADO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.PEBA*/ vram_bus.PEBA_SPRITE_DA1n.dff8n(_XADO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MOFO*/ vram_bus.MOFO_SPRITE_DA2n.dff8n(_XADO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.PUDU*/ vram_bus.PUDU_SPRITE_DA3n.dff8n(_XADO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SAJA*/ vram_bus.SAJA_SPRITE_DA4n.dff8n(_XADO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUNY*/ vram_bus.SUNY_SPRITE_DA5n.dff8n(_XADO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.SEMO*/ vram_bus.SEMO_SPRITE_DA6n.dff8n(_XADO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.SEGA*/ vram_bus.SEGA_SPRITE_DA7n.dff8n(_XADO_CLKn_new, _PULY_FLIP7p_old);

    /*#p29.RACA*/ wire _RACA_CLKp_new = and2(sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new(), sprite_fetcher.SYCU_SFETCH_S0pe(XYMU_RENDERINGp));
    /*#p29.PEBY*/ wire _PEBY_CLKn_new = not1(_RACA_CLKp_new);
    /*#p29.NYBE*/ wire _NYBE_CLKp_new = not1(_PEBY_CLKn_new);
    /*#p29.PUCO*/ wire _PUCO_CLKn_new = not1(_NYBE_CLKp_new);
    /* p33.PEFO*/ vram_bus.PEFO_SPRITE_DB0n.dff8n(_PUCO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.ROKA*/ vram_bus.ROKA_SPRITE_DB1n.dff8n(_PUCO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MYTU*/ vram_bus.MYTU_SPRITE_DB2n.dff8n(_PUCO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.RAMU*/ vram_bus.RAMU_SPRITE_DB3n.dff8n(_PUCO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SELE*/ vram_bus.SELE_SPRITE_DB4n.dff8n(_PUCO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUTO*/ vram_bus.SUTO_SPRITE_DB5n.dff8n(_PUCO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.RAMA*/ vram_bus.RAMA_SPRITE_DB6n.dff8n(_PUCO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.RYDU*/ vram_bus.RYDU_SPRITE_DB7n.dff8n(_PUCO_CLKn_new, _PULY_FLIP7p_old);
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
      /* p32.LAKY*/ wire _BG_PIPE_A_SET0_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.LEGU_TILE_DA0n_odd.qn_new());
      /* p32.NYXO*/ wire _BG_PIPE_A_SET1_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.NUDU_TILE_DA1n_odd.qn_new());
      /* p32.LOTO*/ wire _BG_PIPE_A_SET2_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.MUKU_TILE_DA2n_odd.qn_new());
      /* p32.LYDU*/ wire _BG_PIPE_A_SET3_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.LUZO_TILE_DA3n_odd.qn_new());
      /* p32.MYVY*/ wire _BG_PIPE_A_SET4_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.MEGU_TILE_DA4n_odd.qn_new());
      /* p32.LODO*/ wire _BG_PIPE_A_SET5_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.MYJY_TILE_DA5n_odd.qn_new());
      /* p32.NUTE*/ wire _BG_PIPE_A_SET6_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.NASA_TILE_DA6n_odd.qn_new());
      /* p32.NAJA*/ wire _BG_PIPE_A_SET7_new = nand2(_LOZE_PIPE_A_LOADp_new, tile_fetcher.NEFO_TILE_DA7n_odd.qn_new());

      /* p32.LUHE*/ wire _BG_PIX_A0n_new = not1(tile_fetcher.LEGU_TILE_DA0n_odd.qn_new());
      /* p32.NOLY*/ wire _BG_PIX_A1n_new = not1(tile_fetcher.NUDU_TILE_DA1n_odd.qn_new());
      /* p32.LEKE*/ wire _BG_PIX_A2n_new = not1(tile_fetcher.MUKU_TILE_DA2n_odd.qn_new());
      /* p32.LOMY*/ wire _BG_PIX_A3n_new = not1(tile_fetcher.LUZO_TILE_DA3n_odd.qn_new());
      /* p32.LALA*/ wire _BG_PIX_A4n_new = not1(tile_fetcher.MEGU_TILE_DA4n_odd.qn_new());
      /* p32.LOXA*/ wire _BG_PIX_A5n_new = not1(tile_fetcher.MYJY_TILE_DA5n_odd.qn_new());
      /* p32.NEZE*/ wire _BG_PIX_A6n_new = not1(tile_fetcher.NASA_TILE_DA6n_odd.qn_new());
      /* p32.NOBO*/ wire _BG_PIX_A7n_new = not1(tile_fetcher.NEFO_TILE_DA7n_odd.qn_new());

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
      /* p32.TUXE*/ wire _BG_PIPE_B_SET0_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.RAWU_TILE_DB0p_odd.qp_new());
      /* p32.SOLY*/ wire _BG_PIPE_B_SET1_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.POZO_TILE_DB1p_odd.qp_new());
      /* p32.RUCE*/ wire _BG_PIPE_B_SET2_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.PYZO_TILE_DB2p_odd.qp_new());
      /* p32.RYJA*/ wire _BG_PIPE_B_SET3_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.POXA_TILE_DB3p_odd.qp_new());
      /* p32.RUTO*/ wire _BG_PIPE_B_SET4_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.PULO_TILE_DB4p_odd.qp_new());
      /* p32.RAJA*/ wire _BG_PIPE_B_SET5_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.POJU_TILE_DB5p_odd.qp_new());
      /* p32.RAJO*/ wire _BG_PIPE_B_SET6_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.POWY_TILE_DB6p_odd.qp_new());
      /* p32.RAGA*/ wire _BG_PIPE_B_SET7_new = nand2(_LUXA_PIPE_B_LOADp_new, tile_fetcher.PYJU_TILE_DB7p_odd.qp_new());

      /* p32.TOSA*/ wire _BG_PIX_B0n_new = not1(tile_fetcher.RAWU_TILE_DB0p_odd.qp_new());
      /* p32.RUCO*/ wire _BG_PIX_B1n_new = not1(tile_fetcher.POZO_TILE_DB1p_odd.qp_new());
      /* p32.TYCE*/ wire _BG_PIX_B2n_new = not1(tile_fetcher.PYZO_TILE_DB2p_odd.qp_new());
      /* p32.REVY*/ wire _BG_PIX_B3n_new = not1(tile_fetcher.POXA_TILE_DB3p_odd.qp_new());
      /* p32.RYGA*/ wire _BG_PIX_B4n_new = not1(tile_fetcher.PULO_TILE_DB4p_odd.qp_new());
      /* p32.RYLE*/ wire _BG_PIX_B5n_new = not1(tile_fetcher.POJU_TILE_DB5p_odd.qp_new());
      /* p32.RAPU*/ wire _BG_PIX_B6n_new = not1(tile_fetcher.POWY_TILE_DB6p_odd.qp_new());
      /* p32.SOJA*/ wire _BG_PIX_B7n_new = not1(tile_fetcher.PYJU_TILE_DB7p_odd.qp_new());

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
      /* p33.PABE*/ wire _PABE_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, vram_bus.REWO_SPRITE_DA0n.qn_new());
      /* p33.MYTO*/ wire _MYTO_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, vram_bus.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LELA*/ wire _LELA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, vram_bus.MOFO_SPRITE_DA2n.qn_new());
      /* p33.MAME*/ wire _MAME_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, vram_bus.PUDU_SPRITE_DA3n.qn_new());
      /* p33.VEXU*/ wire _VEXU_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, vram_bus.SAJA_SPRITE_DA4n.qn_new());
      /* p33.VABY*/ wire _VABY_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, vram_bus.SUNY_SPRITE_DA5n.qn_new());
      /* p33.TUXA*/ wire _TUXA_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, vram_bus.SEMO_SPRITE_DA6n.qn_new());
      /* p33.VUNE*/ wire _VUNE_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, vram_bus.SEGA_SPRITE_DA7n.qn_new());

      /* p33.RATA*/ wire _RATA_SPR_PIX_DA0n_new = not1(vram_bus.REWO_SPRITE_DA0n.qn_new());
      /* p33.NUCA*/ wire _NUCA_SPR_PIX_DA1n_new = not1(vram_bus.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LASE*/ wire _LASE_SPR_PIX_DA2n_new = not1(vram_bus.MOFO_SPRITE_DA2n.qn_new());
      /* p33.LUBO*/ wire _LUBO_SPR_PIX_DA3n_new = not1(vram_bus.PUDU_SPRITE_DA3n.qn_new());
      /* p33.WERY*/ wire _WERY_SPR_PIX_DA4n_new = not1(vram_bus.SAJA_SPRITE_DA4n.qn_new());
      /* p33.WURA*/ wire _WURA_SPR_PIX_DA5n_new = not1(vram_bus.SUNY_SPRITE_DA5n.qn_new());
      /* p33.SULU*/ wire _SULU_SPR_PIX_DA6n_new = not1(vram_bus.SEMO_SPRITE_DA6n.qn_new());
      /* p33.WAMY*/ wire _WAMY_SPR_PIX_DA7n_new = not1(vram_bus.SEGA_SPRITE_DA7n.qn_new());

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
      /* p33.MEZU*/ wire _MEZU_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, vram_bus.PEFO_SPRITE_DB0n.qn_new());
      /* p33.RUSY*/ wire _RUSY_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, vram_bus.ROKA_SPRITE_DB1n.qn_new());
      /* p33.MYXA*/ wire _MYXA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, vram_bus.MYTU_SPRITE_DB2n.qn_new());
      /* p33.RANO*/ wire _RANO_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, vram_bus.RAMU_SPRITE_DB3n.qn_new());
      /* p33.TYGA*/ wire _TYGA_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, vram_bus.SELE_SPRITE_DB4n.qn_new());
      /* p33.VUME*/ wire _VUME_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, vram_bus.SUTO_SPRITE_DB5n.qn_new());
      /* p33.TAPO*/ wire _TAPO_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, vram_bus.RAMA_SPRITE_DB6n.qn_new());
      /* p33.TESO*/ wire _TESO_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, vram_bus.RYDU_SPRITE_DB7n.qn_new());

      /* p33.LOZA*/ wire _LOZA_SPR_PIX_DB0n_new = not1(vram_bus.PEFO_SPRITE_DB0n.qn_new());
      /* p33.SYBO*/ wire _SYBO_SPR_PIX_DB1n_new = not1(vram_bus.ROKA_SPRITE_DB1n.qn_new());
      /* p33.LUMO*/ wire _LUMO_SPR_PIX_DB2n_new = not1(vram_bus.MYTU_SPRITE_DB2n.qn_new());
      /* p33.SOLO*/ wire _SOLO_SPR_PIX_DB3n_new = not1(vram_bus.RAMU_SPRITE_DB3n.qn_new());
      /* p33.VOBY*/ wire _VOBY_SPR_PIX_DB4n_new = not1(vram_bus.SELE_SPRITE_DB4n.qn_new());
      /* p33.WYCO*/ wire _WYCO_SPR_PIX_DB5n_new = not1(vram_bus.SUTO_SPRITE_DB5n.qn_new());
      /* p33.SERY*/ wire _SERY_SPR_PIX_DB6n_new = not1(vram_bus.RAMA_SPRITE_DB6n.qn_new());
      /* p33.SELU*/ wire _SELU_SPR_PIX_DB7n_new = not1(vram_bus.RYDU_SPRITE_DB7n.qn_new());

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcd(
    wire XODO_VID_RSTp_new_h,
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
    XODO_VID_RSTp_new_h,
    TALU_xxCDEFxx_clkevn,
    TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
    XONA_LCDC_LCDENp_h_new,
    XYMU_RENDERINGp_new_xxx,
    AVAP_SCAN_DONE_TRIGp_new_xxx,
    pix_count.XYDO_PX3p.qp_old(),
    div_reg.TULU_DIV07p_evn.qp_new(),
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
    /* p01.UMEK*/ wire _UMEK_DIV06n_new = not1(div_reg.UGOT_DIV06p_evn.qp_new());
    /*#p24.KASA*/ wire _KASA_LINE_ENDp_new = not1(PURE_LINE_ENDn_new_evn);
    /*#p24.UMOB*/ wire _UMOB_DIV_06p_new = not1(_UMEK_DIV06n_new);
    /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new_evn = not1(XONA_LCDC_LCDENp_h_new);
    /*#p24.KAHE*/ wire _KAHE_LINE_ENDp_new = amux2(reg_lcdc.XONA_LCDC_LCDENn_h.qn_new(), _KASA_LINE_ENDp_new, _KEDY_LCDC_ENn_new_evn, _UMOB_DIV_06p_new);
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
    /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE_xxx.nor_latch(_XAJO_X_009p_new, WEGO_HBLANKp_new_any);
    /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK_new = and2(lcd.WUSA_LCD_CLOCK_GATE_xxx.qp_new(), SACU_CLKPIPE_AxCxExGx_clknew_evn);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    /* p08.LOXO*/ wire _LOXO_HOLDn_new = and_or3(MULE_MODE_DBG1n_ext(), TEXO_ADDR_VRAMn_ext, UMUT_MODE_DBG1p_ext());
    /* p08.LASY*/ wire _LASY_HOLDp_new = not1(_LOXO_HOLDn_new);
    /* p08.MATE*/ wire _MATE_HOLDn_new = not1(_LASY_HOLDp_new);
    /* p08.ALOR*/ ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 0]);
    /* p08.APUR*/ ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 1]);
    /* p08.ALYR*/ ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 2]);
    /* p08.ARET*/ ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 3]);
    /* p08.AVYS*/ ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 4]);
    /* p08.ATEV*/ ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 5]);
    /* p08.AROS*/ ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 6]);
    /* p08.ARYM*/ ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 7]);
    /* p08.LUNO*/ ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 8]);
    /* p08.LYSA*/ ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 9]);
    /* p08.PATE*/ ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[10]);
    /* p08.LUMY*/ ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[11]);
    /* p08.LOBU*/ ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[12]);
    /* p08.LONU*/ ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[13]);
    /* p08.NYRE*/ ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[14]);

    wire _LUMA_DMA_CARTp_new = dma_reg.LUMA_DMA_CARTp_new();
    /* p08.AMET*/ wire _AMET_A00p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAKY_DMA_A00p_evn.qp_new(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp_new());
    /* p08.ATOL*/ wire _ATOL_A01p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYRO_DMA_A01p_evn.qp_new(), ext_bus.APUR_EXT_ADDR_LATCH_01p.qp_new());
    /* p08.APOK*/ wire _APOK_A02p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NEFY_DMA_A02p_evn.qp_new(), ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp_new());
    /* p08.AMER*/ wire _AMER_A03p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUTY_DMA_A03p_evn.qp_new(), ext_bus.ARET_EXT_ADDR_LATCH_03p.qp_new());
    /* p08.ATEM*/ wire _ATEM_A04p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYKO_DMA_A04p_evn.qp_new(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp_new());
    /* p08.ATOV*/ wire _ATOV_A05p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYLO_DMA_A05p_evn.qp_new(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp_new());
    /* p08.ATYR*/ wire _ATYR_A06p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NUTO_DMA_A06p_evn.qp_new(), ext_bus.AROS_EXT_ADDR_LATCH_06p.qp_new());
    /*#p08.ASUR*/ wire _ASUR_A07p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUGU_DMA_A07p_evn.qp_new(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp_new());
    /*#p08.MANO*/ wire _MANO_A08p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAFA_DMA_A08n_h.qn_new(),   ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp_new());
    /* p08.MASU*/ wire _MASU_A09p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYNE_DMA_A09n_h.qn_new(),   ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp_new());
    /* p08.PAMY*/ wire _PAMY_A10p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PARA_DMA_A10n_h.qn_new(),   ext_bus.PATE_EXT_ADDR_LATCH_10p.qp_new());
    /* p08.MALE*/ wire _MALE_A11p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYDO_DMA_A11n_h.qn_new(),   ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp_new());
    /* p08.MOJY*/ wire _MOJY_A12p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYGY_DMA_A12n_h.qn_new(),   ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp_new());
    /* p08.MUCE*/ wire _MUCE_A13p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PULA_DMA_A13n_h.qn_new(),   ext_bus.LONU_EXT_ADDR_LATCH_13p.qp_new());
    /* p08.PEGE*/ wire _PEGE_A14p_new_evn = mux2p(_LUMA_DMA_CARTp_new, dma_reg.POKU_DMA_A14n_h.qn_new(),   ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp_new());

    /* p08.KUPO*/ wire _KUPO_new_evn = nand2(_AMET_A00p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.CABA*/ wire _CABA_new_evn = nand2(_ATOL_A01p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.BOKU*/ wire _BOKU_new_evn = nand2(_APOK_A02p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.BOTY*/ wire _BOTY_new_evn = nand2(_AMER_A03p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.BYLA*/ wire _BYLA_new_evn = nand2(_ATEM_A04p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.BADU*/ wire _BADU_new_evn = nand2(_ATOV_A05p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.CEPU*/ wire _CEPU_new_evn = nand2(_ATYR_A06p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.DEFY*/ wire _DEFY_new_evn = nand2(_ASUR_A07p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.MYNY*/ wire _MYNY_new_evn = nand2(_MANO_A08p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.MUNE*/ wire _MUNE_new_evn = nand2(_MASU_A09p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.ROXU*/ wire _ROXU_new_evn = nand2(_PAMY_A10p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.LEPY*/ wire _LEPY_new_evn = nand2(_MALE_A11p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.LUCE*/ wire _LUCE_new_evn = nand2(_MOJY_A12p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.LABE*/ wire _LABE_new_evn = nand2(_MUCE_A13p_new_evn, TOVA_MODE_DBG2n_ext());
    /* p08.PUHE*/ wire _PUHE_new_evn = nand2(_PEGE_A14p_new_evn, TOVA_MODE_DBG2n_ext());

    /* p08.KOTY*/ wire _KOTY_new_evn = nor2 (_AMET_A00p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.COTU*/ wire _COTU_new_evn = nor2 (_ATOL_A01p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.BAJO*/ wire _BAJO_new_evn = nor2 (_APOK_A02p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.BOLA*/ wire _BOLA_new_evn = nor2 (_AMER_A03p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.BEVO*/ wire _BEVO_new_evn = nor2 (_ATEM_A04p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.AJAV*/ wire _AJAV_new_evn = nor2 (_ATOV_A05p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.CYKA*/ wire _CYKA_new_evn = nor2 (_ATYR_A06p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.COLO*/ wire _COLO_new_evn = nor2 (_ASUR_A07p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.MEGO*/ wire _MEGO_new_evn = nor2 (_MANO_A08p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.MENY*/ wire _MENY_new_evn = nor2 (_MASU_A09p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.RORE*/ wire _RORE_new_evn = nor2 (_PAMY_A10p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.LYNY*/ wire _LYNY_new_evn = nor2 (_MALE_A11p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.LOSO*/ wire _LOSO_new_evn = nor2 (_MOJY_A12p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.LEVA*/ wire _LEVA_new_evn = nor2 (_MUCE_A13p_new_evn, UNOR_MODE_DBG2p_ext());
    /* p08.PAHY*/ wire _PAHY_new_evn = nor2 (_PEGE_A14p_new_evn, UNOR_MODE_DBG2p_ext());

    PIN_EXT_A[ 0].pin_out(_KUPO_new_evn, _KOTY_new_evn);
    PIN_EXT_A[ 1].pin_out(_CABA_new_evn, _COTU_new_evn);
    PIN_EXT_A[ 2].pin_out(_BOKU_new_evn, _BAJO_new_evn);
    PIN_EXT_A[ 3].pin_out(_BOTY_new_evn, _BOLA_new_evn);
    PIN_EXT_A[ 4].pin_out(_BYLA_new_evn, _BEVO_new_evn);
    PIN_EXT_A[ 5].pin_out(_BADU_new_evn, _AJAV_new_evn);
    PIN_EXT_A[ 6].pin_out(_CEPU_new_evn, _CYKA_new_evn);
    PIN_EXT_A[ 7].pin_out(_DEFY_new_evn, _COLO_new_evn);
    PIN_EXT_A[ 8].pin_out(_MYNY_new_evn, _MEGO_new_evn);
    PIN_EXT_A[ 9].pin_out(_MUNE_new_evn, _MENY_new_evn);
    PIN_EXT_A[10].pin_out(_ROXU_new_evn, _RORE_new_evn);
    PIN_EXT_A[11].pin_out(_LEPY_new_evn, _LYNY_new_evn);
    PIN_EXT_A[12].pin_out(_LUCE_new_evn, _LOSO_new_evn);
    PIN_EXT_A[13].pin_out(_LABE_new_evn, _LEVA_new_evn);
    PIN_EXT_A[14].pin_out(_PUHE_new_evn, _PAHY_new_evn);
  }();

  // A15 is "special"
  [this, BUS_CPU_A, ABUZ_xxCDEFGH_clk_evn, TUTU_READ_BOOTROMp_new](){
    /* p08.SOBY*/ wire _SOBY_A15n_new = nor2(BUS_CPU_A[15], TUTU_READ_BOOTROMp_new);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx_clknew = nand2(ABUZ_xxCDEFGH_clk_evn, _SOBY_A15n_new);
    /* p08.TAZY*/ wire _TAZY_A15p_new = mux2p(dma_reg.LUMA_DMA_CARTp_new(), dma_reg.MARU_DMA_A15n_h.qn_new(), _SEPY_A15p_ABxxxxxx_clknew);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n_new = nand2(_TAZY_A15p_new, RYCA_MODE_DBG2n_ext());
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n_new = nor2 (_TAZY_A15p_new, UNOR_MODE_DBG2p_ext());
    PIN_EXT_A[15].pin_out(_SUZE_PIN_EXT_A15n_new, _RULO_PIN_EXT_A15n_new);
  }();

  //----------------------------------------
  // CPU data bus to data pins

  [this, BUS_CPU_D, TEDO_CPU_RDp, TEXO_ADDR_VRAMn_ext](){
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /* p08.REDU*/ wire _REDU_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, UMUT_MODE_DBG1p_ext());
    /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
    /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp.qp_new(), _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
    /* p08.RORU*/ wire _RORU_CBD_TO_EPDn_new = mux2p(UNOR_MODE_DBG2p_ext(), _REDU_CPU_RDn_ext, _MOTY_CPU_EXT_RD_ext);
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

    PIN_EXT_D[0].pin_out(_LULA_CBD_TO_EPDp_new, _RUXA_new, _RUNE_new);
    PIN_EXT_D[1].pin_out(_LULA_CBD_TO_EPDp_new, _RUJA_new, _RYPU_new);
    PIN_EXT_D[2].pin_out(_LULA_CBD_TO_EPDp_new, _RABY_new, _SULY_new);
    PIN_EXT_D[3].pin_out(_LULA_CBD_TO_EPDp_new, _RERA_new, _SEZE_new);
    PIN_EXT_D[4].pin_out(_LULA_CBD_TO_EPDp_new, _RORY_new, _RESY_new);
    PIN_EXT_D[5].pin_out(_LULA_CBD_TO_EPDp_new, _RYVO_new, _TAMU_new);
    PIN_EXT_D[6].pin_out(_LULA_CBD_TO_EPDp_new, _RAFY_new, _ROGY_new);
    PIN_EXT_D[7].pin_out(_LULA_CBD_TO_EPDp_new, _RAVU_new, _RYDA_new);
  }();

  //----------------------------------------
  // Pins

  [this, TEXO_ADDR_VRAMn_ext](){
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, UMUT_MODE_DBG1p_ext());
    /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
    /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp.qp_new(), _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
    /* p08.TYMU*/ wire _TYMU_EXT_RDn_new = nor2(dma_reg.LUMA_DMA_CARTp_new(), _MOTY_CPU_EXT_RD_ext);
    /* p08.UGAC*/ wire _UGAC_RD_A_new = nand2(_TYMU_EXT_RDn_new, TOVA_MODE_DBG2n_ext());
    /* p08.URUN*/ wire _URUN_RD_D_new = nor2 (_TYMU_EXT_RDn_new, UNOR_MODE_DBG2p_ext());
    PIN_EXT_RDn.pin_out(_UGAC_RD_A_new, _URUN_RD_D_new);
  }();

  [this, TEXO_ADDR_VRAMn_ext, APOV_CPU_WRp_clkevn](){
    /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, UMUT_MODE_DBG1p_ext());
    /* p08.MEXO*/ wire _MEXO_CPU_WRn_new = not1(APOV_CPU_WRp_clkevn);
    /* p08.NEVY*/ wire _NEVY_new = or2(_MEXO_CPU_WRn_new, _MOCA_DBG_EXT_RD_ext);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn_new = or2(_NEVY_new, dma_reg.LUMA_DMA_CARTp_new());
    /* p08.UVER*/ wire _UVER_WR_A_new = nand2(_PUVA_EXT_WRn_new, TOVA_MODE_DBG2n_ext());
    /* p08.USUF*/ wire _USUF_WR_D_new = nor2 (_PUVA_EXT_WRn_new, UNOR_MODE_DBG2p_ext());
    PIN_EXT_WRn.pin_out(_UVER_WR_A_new, _USUF_WR_D_new);
  }();

  [this, BUS_CPU_A, ABUZ_xxCDEFGH_clk_evn](){
    /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk = and3(ABUZ_xxCDEFGH_clk_evn, TYNU_A000_FFFFp_ext(BUS_CPU_A), TUNA_0000_FDFF_ext(BUS_CPU_A));
    /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew = mux2p(dma_reg.LUMA_DMA_CARTp_new(), dma_reg.MARU_DMA_A15n_h.qn_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk);
    PIN_EXT_CSn.pin_out(_TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew);
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
    uint16_t ext_addr_latch = pack_u16p(16, &PIN_EXT_A[ 0]);
    uint8_t ext_data_latch = pack_u8p(8, &PIN_EXT_D[0]);

    // ROM read
    uint16_t rom_addr = ext_addr_latch & 0x7FFF;
    wire rom_CEn_new = PIN_EXT_A[15].qp_new();
    wire rom_OEp_new = !rom_CEn_new && !PIN_EXT_RDn.qp_new() && cart_buf;
    if (rom_OEp_new)  ext_data_latch = cart_buf[rom_addr];

    // Ext RAM read
    uint16_t eram_addr = (ext_addr_latch & 0x1FFF);
    wire eram_CE1n_new = PIN_EXT_CSn.qp_new();
    wire eram_CE2_new  = PIN_EXT_A[14].qp_new();
    wire eram_WRp_new  = !eram_CE1n_new && eram_CE2_new && !PIN_EXT_WRn.qp_new();
    wire eram_OEp_new  = !eram_CE1n_new && eram_CE2_new && !eram_WRp_new && !PIN_EXT_RDn.qp_new();
    if (eram_WRp_new) ext_ram[eram_addr]  = ext_data_latch;
    if (eram_OEp_new) ext_data_latch = ext_ram[eram_addr];

    // Cart RAM read
    uint16_t cram_addr = (ext_addr_latch & 0x1FFF);
    wire cram_CS1n_new = PIN_EXT_CSn.qp_new();
    wire cram_CS2_new  = PIN_EXT_A[13].qp_new() && !PIN_EXT_A[14].qp_new() && PIN_EXT_A[15].qp_new();
    wire cram_WRp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_WRn.qp_new();
    wire cram_OEp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_RDn.qp_new();

    if (cram_WRp_new) cart_ram[cram_addr] = ext_data_latch;
    if (cram_OEp_new) ext_data_latch = cart_ram[cram_addr];

    wire ext_OEp = rom_OEp_new || eram_OEp_new || cram_OEp_new;
    PIN_EXT_D[0].pin_in(ext_OEp, wire(ext_data_latch & 0x01));
    PIN_EXT_D[1].pin_in(ext_OEp, wire(ext_data_latch & 0x02));
    PIN_EXT_D[2].pin_in(ext_OEp, wire(ext_data_latch & 0x04));
    PIN_EXT_D[3].pin_in(ext_OEp, wire(ext_data_latch & 0x08));
    PIN_EXT_D[4].pin_in(ext_OEp, wire(ext_data_latch & 0x10));
    PIN_EXT_D[5].pin_in(ext_OEp, wire(ext_data_latch & 0x20));
    PIN_EXT_D[6].pin_in(ext_OEp, wire(ext_data_latch & 0x40));
    PIN_EXT_D[7].pin_in(ext_OEp, wire(ext_data_latch & 0x80));
  }();

  //----------------------------------------
  // Data pins to data latch, and back to cpu bus.

  [this, TEXO_ADDR_VRAMn_ext](){
    // External data bus -> latch -> CPU data bus
    /* p08.LAVO*/ wire _LAVO_HOLDn_new = nand3(PIN_CPU_RDp.qp_new(), TEXO_ADDR_VRAMn_ext, PIN_CPU_LATCH_EXT.qp_new());
    /*#p08.SOMA*/ ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[0].qn_new());
    /* p08.RONY*/ ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[1].qn_new());
    /* p08.RAXY*/ ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[2].qn_new());
    /* p08.SELO*/ ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[3].qn_new());
    /* p08.SODY*/ ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[4].qn_new());
    /* p08.SAGO*/ ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[5].qn_new());
    /* p08.RUPA*/ ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[6].qn_new());
    /* p08.SAZY*/ ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[7].qn_new());

    /*#p08.RYMA*/ BUS_CPU_D_out[0].tri6_nn(_LAVO_HOLDn_new, ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp_new());
    /* p08.RUVO*/ BUS_CPU_D_out[1].tri6_nn(_LAVO_HOLDn_new, ext_bus.RONY_EXT_DATA_LATCH_D1n.qp_new());
    /* p08.RYKO*/ BUS_CPU_D_out[2].tri6_nn(_LAVO_HOLDn_new, ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp_new());
    /* p08.TAVO*/ BUS_CPU_D_out[3].tri6_nn(_LAVO_HOLDn_new, ext_bus.SELO_EXT_DATA_LATCH_D3n.qp_new());
    /* p08.TEPE*/ BUS_CPU_D_out[4].tri6_nn(_LAVO_HOLDn_new, ext_bus.SODY_EXT_DATA_LATCH_D4n.qp_new());
    /* p08.SAFO*/ BUS_CPU_D_out[5].tri6_nn(_LAVO_HOLDn_new, ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp_new());
    /* p08.SEVU*/ BUS_CPU_D_out[6].tri6_nn(_LAVO_HOLDn_new, ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp_new());
    /* p08.TAJU*/ BUS_CPU_D_out[7].tri6_nn(_LAVO_HOLDn_new, ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp_new());
  }();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// VRAM interface

void GateBoy::tock_vram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],

    wire AVOR_SYS_RSTp,
    wire XODO_VID_RSTp_new_h,

    wire ATAL_xBxDxFxH_clk_odd,
    wire ABUZ_xxCDEFGH_clk_evn,

    wire CATY_LATCH_EXTp_ext,
    wire SOSE_ADDR_VRAMp_ext,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire APOV_CPU_WRp_clkevn,

    wire TUTO_DBG_VRAMp_new,

    wire ATEJ_LINE_TRIGp,
    wire TEVO_FETCH_TRIGp_new,
    wire NYXU_BFETCH_RSTn_new_xxx,
    wire PARU_VBLANKp_new_evn,
    wire PORE_WIN_MODEp_new_evn,
    wire WYMO_LCDC_WINENn_new,
    wire XYMU_RENDERINGp_new_xxx)
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
    SOSE_ADDR_VRAMp_ext,
    ABUZ_xxCDEFGH_clk_evn,
    XYMU_RENDERINGp_new_xxx,

    &SERE_CPU_VRAM_RDp_new,
    &SALE_CPU_VRAM_WRn_new
  ](){
    // Ignoring debug for now
    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_new =  and2(SOSE_ADDR_VRAMp_ext, ABUZ_xxCDEFGH_clk_evn);
    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_new = nand2(SOSE_ADDR_VRAMp_ext, PIN_CPU_WRp.qp_new());  // Schematic wrong, second input is PIN_CPU_WRp

    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn_new());

    /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp_ext = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_new /*)*/;
    /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_new = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/       _TEGU_CPU_VRAM_WRn_new /*)*/;

    /*#p25.ROPY*/ wire _ROPY_RENDERINGn_new = not1(XYMU_RENDERINGp_new_xxx);
    /*#p25.SERE*/ wire _SERE_CPU_VRAM_RDp_new = and2(_TOLE_CPU_VRAM_RDp_ext, _ROPY_RENDERINGn_new);

    SERE_CPU_VRAM_RDp_new = _SERE_CPU_VRAM_RDp_new;
    SALE_CPU_VRAM_WRn_new = _SALE_CPU_VRAM_WRn_new;
  }();

  //----------------------------------------
  // CPU VRAM address generator

  [this, BUS_CPU_A, XYMU_RENDERINGp_new_xxx](){
    // CPU to VRAM address driver
    /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new_evn = nor2(dma_reg.LUFA_DMA_VRAMp_new(), XYMU_RENDERINGp_new_xxx);
    /* p25.XEDU*/ wire _XEDU_VRAM_LOCKp_new_evn = not1(_XANE_VRAM_LOCKn_new_evn);
    /* p25.XAKY*/ BUS_VRAM_An[ 0].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 0]);
    /* p25.XUXU*/ BUS_VRAM_An[ 1].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 1]);
    /* p25.XYNE*/ BUS_VRAM_An[ 2].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 2]);
    /* p25.XODY*/ BUS_VRAM_An[ 3].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 3]);
    /* p25.XECA*/ BUS_VRAM_An[ 4].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 4]);
    /* p25.XOBA*/ BUS_VRAM_An[ 5].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 5]);
    /* p25.XOPO*/ BUS_VRAM_An[ 6].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 6]);
    /* p25.XYBO*/ BUS_VRAM_An[ 7].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 7]);
    /* p25.RYSU*/ BUS_VRAM_An[ 8].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 8]);
    /* p25.RESE*/ BUS_VRAM_An[ 9].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 9]);
    /* p25.RUSE*/ BUS_VRAM_An[10].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[10]);
    /* p25.RYNA*/ BUS_VRAM_An[11].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[11]);
    /* p25.RUMO*/ BUS_VRAM_An[12].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[12]);
  }();

  //----------------------------------------
  // DMA VRAM address generator

  [this](){
    /* p04.NAKY*/ wire _NAKY_DMA_A00p_new = dma_reg.NAKY_DMA_A00p_evn.qp_new();
    /* p04.PYRO*/ wire _PYRO_DMA_A01p_new = dma_reg.PYRO_DMA_A01p_evn.qp_new();
    /* p04.NEFY*/ wire _NEFY_DMA_A02p_new = dma_reg.NEFY_DMA_A02p_evn.qp_new();
    /* p04.MUTY*/ wire _MUTY_DMA_A03p_new = dma_reg.MUTY_DMA_A03p_evn.qp_new();
    /* p04.NYKO*/ wire _NYKO_DMA_A04p_new = dma_reg.NYKO_DMA_A04p_evn.qp_new();
    /* p04.PYLO*/ wire _PYLO_DMA_A05p_new = dma_reg.PYLO_DMA_A05p_evn.qp_new();
    /* p04.NUTO*/ wire _NUTO_DMA_A06p_new = dma_reg.NUTO_DMA_A06p_evn.qp_new();
    /* p04.MUGU*/ wire _MUGU_DMA_A07p_new = dma_reg.MUGU_DMA_A07p_evn.qp_new();
    /* p04.NAFA*/ wire _NAFA_DMA_A08p_new = dma_reg.NAFA_DMA_A08n_h.qn_new();
    /* p04.PYNE*/ wire _PYNE_DMA_A09p_new = dma_reg.PYNE_DMA_A09n_h.qn_new();
    /* p04.PARA*/ wire _PARA_DMA_A10p_new = dma_reg.PARA_DMA_A10n_h.qn_new();
    /* p04.NYDO*/ wire _NYDO_DMA_A11p_new = dma_reg.NYDO_DMA_A11n_h.qn_new();
    /* p04.NYGY*/ wire _NYGY_DMA_A12p_new = dma_reg.NYGY_DMA_A12n_h.qn_new();

    /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_new = not1(dma_reg.LUFA_DMA_VRAMp_new());
    /* p04.ECAL*/ BUS_VRAM_An[ 0].tri6_nn(_AHOC_DMA_VRAMn_new, _NAKY_DMA_A00p_new);
    /* p04.EGEZ*/ BUS_VRAM_An[ 1].tri6_nn(_AHOC_DMA_VRAMn_new, _PYRO_DMA_A01p_new);
    /* p04.FUHE*/ BUS_VRAM_An[ 2].tri6_nn(_AHOC_DMA_VRAMn_new, _NEFY_DMA_A02p_new);
    /* p04.FYZY*/ BUS_VRAM_An[ 3].tri6_nn(_AHOC_DMA_VRAMn_new, _MUTY_DMA_A03p_new);
    /* p04.DAMU*/ BUS_VRAM_An[ 4].tri6_nn(_AHOC_DMA_VRAMn_new, _NYKO_DMA_A04p_new);
    /* p04.DAVA*/ BUS_VRAM_An[ 5].tri6_nn(_AHOC_DMA_VRAMn_new, _PYLO_DMA_A05p_new);
    /* p04.ETEG*/ BUS_VRAM_An[ 6].tri6_nn(_AHOC_DMA_VRAMn_new, _NUTO_DMA_A06p_new);
    /*#p04.EREW*/ BUS_VRAM_An[ 7].tri6_nn(_AHOC_DMA_VRAMn_new, _MUGU_DMA_A07p_new);
    /*#p04.EVAX*/ BUS_VRAM_An[ 8].tri6_nn(_AHOC_DMA_VRAMn_new, _NAFA_DMA_A08p_new);
    /* p04.DUVE*/ BUS_VRAM_An[ 9].tri6_nn(_AHOC_DMA_VRAMn_new, _PYNE_DMA_A09p_new);
    /* p04.ERAF*/ BUS_VRAM_An[10].tri6_nn(_AHOC_DMA_VRAMn_new, _PARA_DMA_A10p_new);
    /* p04.FUSY*/ BUS_VRAM_An[11].tri6_nn(_AHOC_DMA_VRAMn_new, _NYDO_DMA_A11p_new);
    /* p04.EXYF*/ BUS_VRAM_An[12].tri6_nn(_AHOC_DMA_VRAMn_new, _NYGY_DMA_A12p_new);
  }();

  //----------------------------------------
  // SCX/SCY read/write

  [
    this,
    BUS_CPU_A,
    BUS_CPU_D,
    AVOR_SYS_RSTp,
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
    /* p23.GAVE*/ tile_fetcher.GAVE_SCY0n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p23.FYMO*/ tile_fetcher.FYMO_SCY1n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p23.FEZU*/ tile_fetcher.FEZU_SCY2n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
    /* p23.FUJO*/ tile_fetcher.FUJO_SCY3n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
    /* p23.DEDE*/ tile_fetcher.DEDE_SCY4n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
    /* p23.FOTY*/ tile_fetcher.FOTY_SCY5n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
    /* p23.FOHA*/ tile_fetcher.FOHA_SCY6n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
    /* p23.FUNY*/ tile_fetcher.FUNY_SCY7n_h.dff9(_CAVO_FF42_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

    // FF43 SCX
    /* p23.AMUN*/ wire _AMUN_FF43_WRn_clk = not1(_ARUR_FF43_WRp_clk);
    /* p23.DATY*/ tile_fetcher.DATY_SCX0n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p23.DUZU*/ tile_fetcher.DUZU_SCX1n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p23.CYXU*/ tile_fetcher.CYXU_SCX2n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
    /* p23.GUBO*/ tile_fetcher.GUBO_SCX3n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
    /* p23.BEMY*/ tile_fetcher.BEMY_SCX4n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
    /* p23.CUZY*/ tile_fetcher.CUZY_SCX5n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
    /* p23.CABU*/ tile_fetcher.CABU_SCX6n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
    /* p23.BAKE*/ tile_fetcher.BAKE_SCX7n_h.dff9(_AMUN_FF43_WRn_clk, CUNU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

    /* FF42 SCY */
    /* p23.BUWY*/ wire _BUWY_FF42_RDn_ext = not1(_ANYP_FF42_RDp_ext);
    /*#p23.WARE*/ BUS_CPU_D_out[0].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.GAVE_SCY0n_h.qp_new());
    /* p23.GOBA*/ BUS_CPU_D_out[1].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FYMO_SCY1n_h.qp_new());
    /* p23.GONU*/ BUS_CPU_D_out[2].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FEZU_SCY2n_h.qp_new());
    /* p23.GODO*/ BUS_CPU_D_out[3].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FUJO_SCY3n_h.qp_new());
    /* p23.CUSA*/ BUS_CPU_D_out[4].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.DEDE_SCY4n_h.qp_new());
    /* p23.GYZO*/ BUS_CPU_D_out[5].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FOTY_SCY5n_h.qp_new());
    /* p23.GUNE*/ BUS_CPU_D_out[6].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FOHA_SCY6n_h.qp_new());
    /* p23.GYZA*/ BUS_CPU_D_out[7].tri6_nn(_BUWY_FF42_RDn_ext, tile_fetcher.FUNY_SCY7n_h.qp_new());

    // FF43 SCX
    /* p23.BEBA*/ wire _BEBA_FF43_RDn_ext = not1(_AVOG_FF43_RDp_ext);
    /*#p23.EDOS*/ BUS_CPU_D_out[0].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.DATY_SCX0n_h.qp_new());
    /* p23.EKOB*/ BUS_CPU_D_out[1].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.DUZU_SCX1n_h.qp_new());
    /* p23.CUGA*/ BUS_CPU_D_out[2].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.CYXU_SCX2n_h.qp_new());
    /* p23.WONY*/ BUS_CPU_D_out[3].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.GUBO_SCX3n_h.qp_new());
    /* p23.CEDU*/ BUS_CPU_D_out[4].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.BEMY_SCX4n_h.qp_new());
    /* p23.CATA*/ BUS_CPU_D_out[5].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.CUZY_SCX5n_h.qp_new());
    /* p23.DOXE*/ BUS_CPU_D_out[6].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.CABU_SCX6n_h.qp_new());
    /* p23.CASY*/ BUS_CPU_D_out[7].tri6_nn(_BEBA_FF43_RDn_ext, tile_fetcher.BAKE_SCX7n_h.qp_new());
  }();

  //----------------------------------------
  // Background/window map and tile adddress generator

  [
    this,
    XODO_VID_RSTp_new_h,

    ATEJ_LINE_TRIGp,
    TEVO_FETCH_TRIGp_new,
    PORE_WIN_MODEp_new_evn,
    WYMO_LCDC_WINENn_new,
    PARU_VBLANKp_new_evn
  ]() {

    // X/Y scroll adder
#pragma warning(push)
#pragma warning(disable : 4189)

    /*#p26.ATAD*/ wire _ATAD_TILE_X0S_new = add_s(pix_count.XEHO_PX0p.qp_new(), tile_fetcher.DATY_SCX0n_h.qn_new(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C_new = add_c(pix_count.XEHO_PX0p.qp_new(), tile_fetcher.DATY_SCX0n_h.qn_new(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S_new = add_s(pix_count.SAVY_PX1p.qp_new(), tile_fetcher.DUZU_SCX1n_h.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C_new = add_c(pix_count.SAVY_PX1p.qp_new(), tile_fetcher.DUZU_SCX1n_h.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2S_new = add_s(pix_count.XODU_PX2p.qp_new(), tile_fetcher.CYXU_SCX2n_h.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2C_new = add_c(pix_count.XODU_PX2p.qp_new(), tile_fetcher.CYXU_SCX2n_h.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0S_new  = add_s(pix_count.XYDO_PX3p.qp_new(), tile_fetcher.GUBO_SCX3n_h.qn_new(), _APYH_TILE_X2C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0C_new  = add_c(pix_count.XYDO_PX3p.qp_new(), tile_fetcher.GUBO_SCX3n_h.qn_new(), _APYH_TILE_X2C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S_new  = add_s(pix_count.TUHU_PX4p.qp_new(), tile_fetcher.BEMY_SCX4n_h.qn_new(), _BABE_MAP_X0C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C_new  = add_c(pix_count.TUHU_PX4p.qp_new(), tile_fetcher.BEMY_SCX4n_h.qn_new(), _BABE_MAP_X0C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S_new  = add_s(pix_count.TUKY_PX5p.qp_new(), tile_fetcher.CUZY_SCX5n_h.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C_new  = add_c(pix_count.TUKY_PX5p.qp_new(), tile_fetcher.CUZY_SCX5n_h.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S_new  = add_s(pix_count.TAKO_PX6p.qp_new(), tile_fetcher.CABU_SCX6n_h.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C_new  = add_c(pix_count.TAKO_PX6p.qp_new(), tile_fetcher.CABU_SCX6n_h.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S_new  = add_s(pix_count.SYBE_PX7p.qp_new(), tile_fetcher.BAKE_SCX7n_h.qn_new(), _BYCA_MAP_X3C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C_new  = add_c(pix_count.SYBE_PX7p.qp_new(), tile_fetcher.BAKE_SCX7n_h.qn_new(), _BYCA_MAP_X3C_new);

    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S_new = add_s(reg_ly.MUWY_LY0p_evn.qp_new(), tile_fetcher.GAVE_SCY0n_h.qn_new(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C_new = add_c(reg_ly.MUWY_LY0p_evn.qp_new(), tile_fetcher.GAVE_SCY0n_h.qn_new(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S_new = add_s(reg_ly.MYRO_LY1p_evn.qp_new(), tile_fetcher.FYMO_SCY1n_h.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C_new = add_c(reg_ly.MYRO_LY1p_evn.qp_new(), tile_fetcher.FYMO_SCY1n_h.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S_new = add_s(reg_ly.LEXA_LY2p_evn.qp_new(), tile_fetcher.FEZU_SCY2n_h.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C_new = add_c(reg_ly.LEXA_LY2p_evn.qp_new(), tile_fetcher.FEZU_SCY2n_h.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S_new  = add_s(reg_ly.LYDO_LY3p_evn.qp_new(), tile_fetcher.FUJO_SCY3n_h.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C_new  = add_c(reg_ly.LYDO_LY3p_evn.qp_new(), tile_fetcher.FUJO_SCY3n_h.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S_new  = add_s(reg_ly.LOVU_LY4p_evn.qp_new(), tile_fetcher.DEDE_SCY4n_h.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C_new  = add_c(reg_ly.LOVU_LY4p_evn.qp_new(), tile_fetcher.DEDE_SCY4n_h.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2S_new  = add_s(reg_ly.LEMA_LY5p_evn.qp_new(), tile_fetcher.FOTY_SCY5n_h.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2C_new  = add_c(reg_ly.LEMA_LY5p_evn.qp_new(), tile_fetcher.FOTY_SCY5n_h.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S_new  = add_s(reg_ly.MATO_LY6p_evn.qp_new(), tile_fetcher.FOHA_SCY6n_h.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C_new  = add_c(reg_ly.MATO_LY6p_evn.qp_new(), tile_fetcher.FOHA_SCY6n_h.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S_new  = add_s(reg_ly.LAFO_LY7p_evn.qp_new(), tile_fetcher.FUNY_SCY7n_h.qn_new(), _EFYK_MAP_Y3C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C_new  = add_c(reg_ly.LAFO_LY7p_evn.qp_new(), tile_fetcher.FUNY_SCY7n_h.qn_new(), _EFYK_MAP_Y3C_new);
#pragma warning(pop)

    // Window map coordinate counter
    [
      this,
      XODO_VID_RSTp_new_h,
      ATEJ_LINE_TRIGp,
      TEVO_FETCH_TRIGp_new,
      PORE_WIN_MODEp_new_evn,
      PARU_VBLANKp_new_evn,
      WYMO_LCDC_WINENn_new
    ](){
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new = not1(XODO_VID_RSTp_new_h);
      /* p01.PYRY*/ wire _PYRY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);

      /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_new = not1(ATEJ_LINE_TRIGp);

      /* p27.VETU*/ wire _VETU_WIN_MAP_clknew = and2(TEVO_FETCH_TRIGp_new, PORE_WIN_MODEp_new_evn);
      /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new   = nand3(WYMO_LCDC_WINENn_new, _XAHY_LINE_TRIGn_new, _XAPO_VID_RSTn_new);
      /* p27.XACO*/ wire _XACO_WIN_RSTn_new = not1(_XOFO_WIN_RSTp_new);
      /* p27.WYKA*/ tile_fetcher.WYKA_WIN_X3_evn.dff17(_VETU_WIN_MAP_clknew,                  _XACO_WIN_RSTn_new, tile_fetcher.WYKA_WIN_X3_evn.qn_any());
      /* p27.WODY*/ tile_fetcher.WODY_WIN_X4_evn.dff17(tile_fetcher.WYKA_WIN_X3_evn.qn_new(), _XACO_WIN_RSTn_new, tile_fetcher.WODY_WIN_X4_evn.qn_any());
      /* p27.WOBO*/ tile_fetcher.WOBO_WIN_X5_evn.dff17(tile_fetcher.WODY_WIN_X4_evn.qn_new(), _XACO_WIN_RSTn_new, tile_fetcher.WOBO_WIN_X5_evn.qn_any());
      /* p27.WYKO*/ tile_fetcher.WYKO_WIN_X6_evn.dff17(tile_fetcher.WOBO_WIN_X5_evn.qn_new(), _XACO_WIN_RSTn_new, tile_fetcher.WYKO_WIN_X6_evn.qn_any());
      /* p27.XOLO*/ tile_fetcher.XOLO_WIN_X7_evn.dff17(tile_fetcher.WYKO_WIN_X6_evn.qn_new(), _XACO_WIN_RSTn_new, tile_fetcher.XOLO_WIN_X7_evn.qn_any());

      // Every time we leave win mode we increment win_y
      /* p27.WAZY*/ wire _WAZY_WIN_Y_CLKp = not1(PORE_WIN_MODEp_new_evn);
      /* p27.REPU*/ wire _REPU_VBLANKp_new = or2(PARU_VBLANKp_new_evn, _PYRY_VID_RSTp_new);
      /* p27.SYNY*/ wire _SYNY_VBLANKn_new = not1(_REPU_VBLANKp_new);
      /* p27.VYNO*/ tile_fetcher.VYNO_WIN_Y0_evn.dff17(_WAZY_WIN_Y_CLKp,              _SYNY_VBLANKn_new,     tile_fetcher.VYNO_WIN_Y0_evn.qn_any());
      /* p27.VUJO*/ tile_fetcher.VUJO_WIN_Y1_evn.dff17(tile_fetcher.VYNO_WIN_Y0_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.VUJO_WIN_Y1_evn.qn_any());
      /* p27.VYMU*/ tile_fetcher.VYMU_WIN_Y2_evn.dff17(tile_fetcher.VUJO_WIN_Y1_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.VYMU_WIN_Y2_evn.qn_any());
      /* p27.TUFU*/ tile_fetcher.TUFU_WIN_Y3_evn.dff17(tile_fetcher.VYMU_WIN_Y2_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.TUFU_WIN_Y3_evn.qn_any());
      /* p27.TAXA*/ tile_fetcher.TAXA_WIN_Y4_evn.dff17(tile_fetcher.TUFU_WIN_Y3_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.TAXA_WIN_Y4_evn.qn_any());
      /* p27.TOZO*/ tile_fetcher.TOZO_WIN_Y5_evn.dff17(tile_fetcher.TAXA_WIN_Y4_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.TOZO_WIN_Y5_evn.qn_any());
      /* p27.TATE*/ tile_fetcher.TATE_WIN_Y6_evn.dff17(tile_fetcher.TOZO_WIN_Y5_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.TATE_WIN_Y6_evn.qn_any());
      /* p27.TEKE*/ tile_fetcher.TEKE_WIN_Y7_evn.dff17(tile_fetcher.TATE_WIN_Y6_evn.qn_new(), _SYNY_VBLANKn_new, tile_fetcher.TEKE_WIN_Y7_evn.qn_any());
    }();

    /* p27.LUSU*/ wire _LUSU_FETCHINGn_new = not1(tile_fetcher.LONY_FETCHINGp_xxx.qp_new());
    /* p27.MESU*/ wire _MESU_BFETCH_S1p_evn_new = tile_fetcher.MESU_BFETCH_S1p_evn.qp_new();
    /* p27.NYVA*/ wire _NYVA_BFETCH_S2p_evn_new = tile_fetcher.NYVA_BFETCH_S2p_evn.qp_new();

    /* p27.LENA*/ wire _LENA_BFETCHINGp_new = not1(_LUSU_FETCHINGn_new);
    /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_new_evn = not1(_MESU_BFETCH_S1p_evn_new);
    /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_new_evn = not1(_NYVA_BFETCH_S2p_evn_new);
    /* p27.NOGU*/ wire _NOGU_BFETCH_01p_new_evn = nand2(_NAKO_BFETCH_S1n_new_evn, _NOFU_BFETCH_S2n_new_evn);
    /* p27.NENY*/ wire _NENY_BFETCH_01n_new_evn    = not1(_NOGU_BFETCH_01p_new_evn);
    /* p27.POTU*/ wire _POTU_BGW_MAP_READp_new = and2(_LENA_BFETCHINGp_new,      _NENY_BFETCH_01n_new_evn);
    /* p27.NETA*/ wire _NETA_BGW_TILE_READp_new = and2(_LENA_BFETCHINGp_new, _NOGU_BFETCH_01p_new_evn);

    {
      // BG map read address
      /* p26.AXAD*/ wire _AXAD_WIN_MODEn_new  = not1(PORE_WIN_MODEp_new_evn);
      /* p26.ACEN*/ wire _ACEN_BGD_MAP_READp_new = and2(_POTU_BGW_MAP_READp_new,  _AXAD_WIN_MODEn_new);
      /* p26.BAFY*/ wire _BAFY_BG_MAP_READn_new = not1(_ACEN_BGD_MAP_READp_new);
      /* p26.AXEP*/ BUS_VRAM_An[ 0].tri6_nn(_BAFY_BG_MAP_READn_new, _BABE_MAP_X0S_new);
      /* p26.AFEB*/ BUS_VRAM_An[ 1].tri6_nn(_BAFY_BG_MAP_READn_new, _ABOD_MAP_X1S_new);
      /* p26.ALEL*/ BUS_VRAM_An[ 2].tri6_nn(_BAFY_BG_MAP_READn_new, _BEWY_MAP_X2S_new);
      /* p26.COLY*/ BUS_VRAM_An[ 3].tri6_nn(_BAFY_BG_MAP_READn_new, _BYCA_MAP_X3S_new);
      /* p26.AJAN*/ BUS_VRAM_An[ 4].tri6_nn(_BAFY_BG_MAP_READn_new, _ACUL_MAP_X4S_new);
      /* p26.DUHO*/ BUS_VRAM_An[ 5].tri6_nn(_BAFY_BG_MAP_READn_new, _ETAM_MAP_Y0S_new);
      /* p26.CASE*/ BUS_VRAM_An[ 6].tri6_nn(_BAFY_BG_MAP_READn_new, _DOTO_MAP_Y1S_new);
      /* p26.CYPO*/ BUS_VRAM_An[ 7].tri6_nn(_BAFY_BG_MAP_READn_new, _DABA_MAP_Y2S_new);
      /* p26.CETA*/ BUS_VRAM_An[ 8].tri6_nn(_BAFY_BG_MAP_READn_new, _EFYK_MAP_Y3S_new);
      /* p26.DAFE*/ BUS_VRAM_An[ 9].tri6_nn(_BAFY_BG_MAP_READn_new, _EJOK_MAP_Y4S_new);
      /*#p26.AMUV*/ BUS_VRAM_An[10].tri6_nn(_BAFY_BG_MAP_READn_new, reg_lcdc.XAFO_LCDC_BGMAPn_h.qn_new());
      /* p26.COVE*/ BUS_VRAM_An[11].tri6_nn(_BAFY_BG_MAP_READn_new, 1);
      /* p26.COXO*/ BUS_VRAM_An[12].tri6_nn(_BAFY_BG_MAP_READn_new, 1);
    }


    {
      // Win map read address
      /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp_new = and2(_POTU_BGW_MAP_READp_new,  PORE_WIN_MODEp_new_evn);
      /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn_new = not1(_XEZE_WIN_MAP_READp_new);
      /*#p27.XEJA*/ BUS_VRAM_An[ 0].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.WYKA_WIN_X3_evn.qp_new());
      /* p27.XAMO*/ BUS_VRAM_An[ 1].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.WODY_WIN_X4_evn.qp_new());
      /* p27.XAHE*/ BUS_VRAM_An[ 2].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.WOBO_WIN_X5_evn.qp_new());
      /* p27.XULO*/ BUS_VRAM_An[ 3].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.WYKO_WIN_X6_evn.qp_new());
      /* p27.WUJU*/ BUS_VRAM_An[ 4].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.XOLO_WIN_X7_evn.qp_new());
      /*#p27.VYTO*/ BUS_VRAM_An[ 5].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.TUFU_WIN_Y3_evn.qp_new());
      /* p27.VEHA*/ BUS_VRAM_An[ 6].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.TAXA_WIN_Y4_evn.qp_new());
      /* p27.VACE*/ BUS_VRAM_An[ 7].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.TOZO_WIN_Y5_evn.qp_new());
      /* p27.VOVO*/ BUS_VRAM_An[ 8].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.TATE_WIN_Y6_evn.qp_new());
      /* p27.VULO*/ BUS_VRAM_An[ 9].tri6_nn(_WUKO_WIN_MAP_READn_new, tile_fetcher.TEKE_WIN_Y7_evn.qp_new());
      /*#p27.VEVY*/ BUS_VRAM_An[10].tri6_nn(_WUKO_WIN_MAP_READn_new, reg_lcdc.WOKY_LCDC_WINMAPn_h.qn_new());
      /* p27.VEZA*/ BUS_VRAM_An[11].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);
      /* p27.VOGU*/ BUS_VRAM_An[12].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);
    }

    // BG/Win tile read address
    {
      /* p26.AXAD*/ wire _AXAD_WIN_MODEn_new_evn  = not1(PORE_WIN_MODEp_new_evn);
      /* p26.ASUL*/ wire _ASUL_BGD_TILE_READp_new =  and2(_NETA_BGW_TILE_READp_new, _AXAD_WIN_MODEn_new_evn);
      /* p26.BEJE*/ wire _BEJE_BG_TILE_READn_new = not1(_ASUL_BGD_TILE_READp_new);
      /*#p27.XUHA*/ wire _XUHA_FETCH_HILOp_new = not1(_NOFU_BFETCH_S2n_new_evn);

      /*#p26.ASUM*/ BUS_VRAM_An[ 0].tri6_nn(_BEJE_BG_TILE_READn_new, _XUHA_FETCH_HILOp_new);
      /* p26.EVAD*/ BUS_VRAM_An[ 1].tri6_nn(_BEJE_BG_TILE_READn_new, _FAFO_TILE_Y0S_new);
      /* p26.DAHU*/ BUS_VRAM_An[ 2].tri6_nn(_BEJE_BG_TILE_READn_new, _EMUX_TILE_Y1S_new);
      /* p26.DODE*/ BUS_VRAM_An[ 3].tri6_nn(_BEJE_BG_TILE_READn_new, _ECAB_TILE_Y2S_new);
    }
    {
      /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn_new = nand2(_NETA_BGW_TILE_READp_new, PORE_WIN_MODEp_new_evn);
      /*#p27.XUHA*/ wire _XUHA_FETCH_HILOp_new_evn = not1(_NOFU_BFETCH_S2n_new_evn);
      /*#p25.XONU*/ BUS_VRAM_An[ 0].tri6_nn(_XUCY_WIN_TILE_READn_new, _XUHA_FETCH_HILOp_new_evn);
      /*#p25.WUDO*/ BUS_VRAM_An[ 1].tri6_nn(_XUCY_WIN_TILE_READn_new, tile_fetcher.VYNO_WIN_Y0_evn.qp_new());
      /*#p25.WAWE*/ BUS_VRAM_An[ 2].tri6_nn(_XUCY_WIN_TILE_READn_new, tile_fetcher.VUJO_WIN_Y1_evn.qp_new());
      /*#p25.WOLU*/ BUS_VRAM_An[ 3].tri6_nn(_XUCY_WIN_TILE_READn_new, tile_fetcher.VYMU_WIN_Y2_evn.qp_new());
    }
    {
      /*#p25.VAPY*/ BUS_VRAM_An[ 4].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.RAWU_TILE_DB0p_odd.qp_new());
      /*#p25.SEZU*/ BUS_VRAM_An[ 5].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.POZO_TILE_DB1p_odd.qp_new());
      /*#p25.VEJY*/ BUS_VRAM_An[ 6].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.PYZO_TILE_DB2p_odd.qp_new());
      /*#p25.RUSA*/ BUS_VRAM_An[ 7].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.POXA_TILE_DB3p_odd.qp_new());
      /*#p25.ROHA*/ BUS_VRAM_An[ 8].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.PULO_TILE_DB4p_odd.qp_new());
      /*#p25.RESO*/ BUS_VRAM_An[ 9].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.POJU_TILE_DB5p_odd.qp_new());
      /*#p25.SUVO*/ BUS_VRAM_An[10].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.POWY_TILE_DB6p_odd.qp_new());
      /*#p25.TOBO*/ BUS_VRAM_An[11].tri6_pn(_NETA_BGW_TILE_READp_new, tile_fetcher.PYJU_TILE_DB7p_odd.qp_new());
    }
    {
      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp_new = nor2(tile_fetcher.PYJU_TILE_DB7p_odd.qp_new(), reg_lcdc.WEXU_LCDC_BGTILEn_h.qn_new());
      /*#p25.VURY*/ BUS_VRAM_An[12].tri6_pn(_NETA_BGW_TILE_READp_new, _VUZA_TILE_BANKp_new);
    }
  }();

  //----------------------------------------
  // Sprite read VRAM address generator

  [
    this,
    XYMU_RENDERINGp_new_xxx
  ](){
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp_new = not1(oam_temp_b.YZOS_OAM_DB6p_evn.qp_new());
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn_new = not1(reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new());

    /*#p29.XUQU*/ wire _XUQU_SPRITE_AB_new = not1(sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qn_new());
    /*#p29.CYVU*/ wire _CYVU_L0_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L[0].qp_new());
    /*#p29.BORE*/ wire _BORE_L1_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L[1].qp_new());
    /*#p29.BUVY*/ wire _BUVY_L2_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L[2].qp_new());
    /*#p29.WAGO*/ wire _WAGO_L3_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L[3].qp_new());
    /*#p29.GEJY*/ wire _GEJY_L3_new = amux2(oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), _FUFO_LCDC_SPSIZEn_new, reg_lcdc.XYMO_LCDC_SPSIZEn_h.qn_new(), _WAGO_L3_new);

    /* p29.TEPA*/ wire _TEPA_RENDERINGp_new = not1(XYMU_RENDERINGp_new_xxx);
    /* p29.SAKY*/ wire _SAKY_SFETCHn_new = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn_new = or2(_SAKY_SFETCHn_new, _TEPA_RENDERINGp_new); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp_new = not1(_TYSO_SFETCHINGn_new);
    /* p29.ABON*/ wire _ABON_SFETCHINGn_new = not1(_TEXY_SFETCHINGp_new);
    /* p29.ABEM*/ BUS_VRAM_An[ 0].tri6_nn(_ABON_SFETCHINGn_new, _XUQU_SPRITE_AB_new);
    /* p29.BAXE*/ BUS_VRAM_An[ 1].tri6_nn(_ABON_SFETCHINGn_new, _CYVU_L0_new);
    /* p29.ARAS*/ BUS_VRAM_An[ 2].tri6_nn(_ABON_SFETCHINGn_new, _BORE_L1_new);
    /* p29.AGAG*/ BUS_VRAM_An[ 3].tri6_nn(_ABON_SFETCHINGn_new, _BUVY_L2_new);
    /* p29.FAMU*/ BUS_VRAM_An[ 4].tri6_nn(_ABON_SFETCHINGn_new, _GEJY_L3_new);
    /*#p29.FUGY*/ BUS_VRAM_An[ 5].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new());
    /* p29.GAVO*/ BUS_VRAM_An[ 6].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new());
    /* p29.WYGA*/ BUS_VRAM_An[ 7].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XYJU_OAM_DA3p_evn.qp_new());
    /* p29.WUNE*/ BUS_VRAM_An[ 8].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YBOG_OAM_DA4p_evn.qp_new());
    /* p29.GOTU*/ BUS_VRAM_An[ 9].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.WYSO_OAM_DA5p_evn.qp_new());
    /* p29.GEGU*/ BUS_VRAM_An[10].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.XOTE_OAM_DA6p_evn.qp_new());
    /* p29.XEHE*/ BUS_VRAM_An[11].tri6_nn(_ABON_SFETCHINGn_new, oam_temp_a.YZAB_OAM_DA7p_evn.qp_new());
    /* p29.DYSO*/ BUS_VRAM_An[12].tri6_nn(_ABON_SFETCHINGn_new, 0);   // sprites always in low half of tile store
  }();

  //----------------------------------------
  // VRAM address bus to VRAM pins

  [this](){
    // PIN_VRAM_A driver
    /* p25.MYFU*/ wire _MYFUp_new = not1(BUS_VRAM_An[ 0].qp_new());
    /* p25.MASA*/ wire _MASAp_new = not1(BUS_VRAM_An[ 1].qp_new());
    /* p25.MYRE*/ wire _MYREp_new = not1(BUS_VRAM_An[ 2].qp_new());
    /* p25.MAVU*/ wire _MAVUp_new = not1(BUS_VRAM_An[ 3].qp_new());
    /* p25.MEPA*/ wire _MEPAp_new = not1(BUS_VRAM_An[ 4].qp_new());
    /* p25.MYSA*/ wire _MYSAp_new = not1(BUS_VRAM_An[ 5].qp_new());
    /* p25.MEWY*/ wire _MEWYp_new = not1(BUS_VRAM_An[ 6].qp_new());
    /* p25.MUME*/ wire _MUMEp_new = not1(BUS_VRAM_An[ 7].qp_new());
    /* p25.VOVA*/ wire _VOVAp_new = not1(BUS_VRAM_An[ 8].qp_new());
    /* p25.VODE*/ wire _VODEp_new = not1(BUS_VRAM_An[ 9].qp_new());
    /* p25.RUKY*/ wire _RUKYp_new = not1(BUS_VRAM_An[10].qp_new());
    /* p25.RUMA*/ wire _RUMAp_new = not1(BUS_VRAM_An[11].qp_new());
    /* p25.REHO*/ wire _REHOp_new = not1(BUS_VRAM_An[12].qp_new());

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

    PIN_VRAM_Ap[ 0].pin_out(_LEXEn_new, _LEXEn_new);
    PIN_VRAM_Ap[ 1].pin_out(_LOZUn_new, _LOZUn_new);
    PIN_VRAM_Ap[ 2].pin_out(_LACAn_new, _LACAn_new);
    PIN_VRAM_Ap[ 3].pin_out(_LUVOn_new, _LUVOn_new);
    PIN_VRAM_Ap[ 4].pin_out(_LOLYn_new, _LOLYn_new);
    PIN_VRAM_Ap[ 5].pin_out(_LALOn_new, _LALOn_new);
    PIN_VRAM_Ap[ 6].pin_out(_LEFAn_new, _LEFAn_new);
    PIN_VRAM_Ap[ 7].pin_out(_LUBYn_new, _LUBYn_new);
    PIN_VRAM_Ap[ 8].pin_out(_TUJYn_new, _TUJYn_new);
    PIN_VRAM_Ap[ 9].pin_out(_TAGOn_new, _TAGOn_new);
    PIN_VRAM_Ap[10].pin_out(_NUVAn_new, _NUVAn_new);
    PIN_VRAM_Ap[11].pin_out(_PEDUn_new, _PEDUn_new);
    PIN_VRAM_Ap[12].pin_out(_PONYn_new, _PONYn_new);
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

    // CPU to VRAM write
    /* p25.TEME*/ BUS_VRAM_Dp[0].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[0]);
    /* p25.TEWU*/ BUS_VRAM_Dp[1].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[1]);
    /*#p25.TYGO*/ BUS_VRAM_Dp[2].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[2]);
    /* p25.SOTE*/ BUS_VRAM_Dp[3].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[3]);
    /* p25.SEKE*/ BUS_VRAM_Dp[4].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[4]);
    /* p25.RUJO*/ BUS_VRAM_Dp[5].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[5]);
    /* p25.TOFA*/ BUS_VRAM_Dp[6].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[6]);
    /* p25.SUZA*/ BUS_VRAM_Dp[7].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_D[7]);

    /*#p25.SEFA*/ wire _SEFA_D0p_new = and2(BUS_VRAM_Dp[0].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SOGO*/ wire _SOGO_D1p_new = and2(BUS_VRAM_Dp[1].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SEFU*/ wire _SEFU_D2p_new = and2(BUS_VRAM_Dp[2].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUNA*/ wire _SUNA_D3p_new = and2(BUS_VRAM_Dp[3].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUMO*/ wire _SUMO_D4p_new = and2(BUS_VRAM_Dp[4].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SAZU*/ wire _SAZU_D5p_new = and2(BUS_VRAM_Dp[5].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SAMO*/ wire _SAMO_D6p_new = and2(BUS_VRAM_Dp[6].qp_new(), _ROVE_CBD_TO_VPDp_new);
    /* p25.SUKE*/ wire _SUKE_D7p_new = and2(BUS_VRAM_Dp[7].qp_new(), _ROVE_CBD_TO_VPDp_new);

    /*#p25.SYNU*/ wire _SYNU_D0p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[0].qp_new());
    /* p25.SYMA*/ wire _SYMA_D1p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[1].qp_new());
    /* p25.ROKO*/ wire _ROKO_D2p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[2].qp_new());
    /* p25.SYBU*/ wire _SYBU_D3p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[3].qp_new());
    /* p25.SAKO*/ wire _SAKO_D4p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[4].qp_new());
    /* p25.SEJY*/ wire _SEJY_D5p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[5].qp_new());
    /* p25.SEDO*/ wire _SEDO_D6p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[6].qp_new());
    /* p25.SAWU*/ wire _SAWU_D7p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp[7].qp_new());

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

    PIN_VRAM_Dp[0].pin_out(_ROFA_CBD_TO_VPDp_new, _REGE_D0n_new, _RURA_D0n_new);
    PIN_VRAM_Dp[1].pin_out(_ROFA_CBD_TO_VPDp_new, _RYKY_D1n_new, _RULY_D1n_new);
    PIN_VRAM_Dp[2].pin_out(_ROFA_CBD_TO_VPDp_new, _RAZO_D2n_new, _RARE_D2n_new);
    PIN_VRAM_Dp[3].pin_out(_ROFA_CBD_TO_VPDp_new, _RADA_D3n_new, _RODU_D3n_new);
    PIN_VRAM_Dp[4].pin_out(_ROFA_CBD_TO_VPDp_new, _RYRO_D4n_new, _RUBE_D4n_new);
    PIN_VRAM_Dp[5].pin_out(_ROFA_CBD_TO_VPDp_new, _REVU_D5n_new, _RUMU_D5n_new);
    PIN_VRAM_Dp[6].pin_out(_ROFA_CBD_TO_VPDp_new, _REKU_D6n_new, _RYTY_D6n_new);
    PIN_VRAM_Dp[7].pin_out(_ROFA_CBD_TO_VPDp_new, _RYZE_D7n_new, _RADY_D7n_new);
  }();

  //----------------------------------------
  // PIN_VRAM_CSn

  [
    this,
    SERE_CPU_VRAM_RDp_new,
    TUTO_DBG_VRAMp_new,
    XYMU_RENDERINGp_new_xxx
  ]() {
    /* p29.SAKY*/ wire _SAKY_SFETCHn_new = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGp_new = not1(XYMU_RENDERINGp_new_xxx);
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn_new = or2(_SAKY_SFETCHn_new, _TEPA_RENDERINGp_new); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp_new = not1(_TYSO_SFETCHINGn_new);
    /* p27.LUSU*/ wire _LUSU_FETCHINGn_new = not1(tile_fetcher.LONY_FETCHINGp_xxx.qp_new());
    /* p27.LENA*/ wire _LENA_BFETCHINGp_new = not1(_LUSU_FETCHINGn_new);
    /*#p25.SUTU*/ wire _SUTU_MCSn_new   = nor4(_LENA_BFETCHINGp_new, dma_reg.LUFA_DMA_VRAMp_new(), _TEXY_SFETCHINGp_new, SERE_CPU_VRAM_RDp_new);

    /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(TUTO_DBG_VRAMp_new);
    /* p25.TODE*/ wire _TODE_MCSn_A_new = and2(_SUTU_MCSn_new, _RACO_DBG_VRAMn_new);
    /* p25.SEWO*/ wire _SEWO_MCSn_D_new =  or2(_SUTU_MCSn_new, TUTO_DBG_VRAMp_new);
    /* p25.SOKY*/ wire _SOKY_MCSp_A_new = not1(_TODE_MCSn_A_new);
    /* p25.SETY*/ wire _SETY_MCSp_D_new = not1(_SEWO_MCSn_D_new);

    PIN_VRAM_CSn.pin_out(_SOKY_MCSp_A_new, _SETY_MCSp_D_new); // FIXME not actually using this pin, but we should
  }();

  //----------------------------------------
  // PIN_VRAM_WRn

  [
    this,
    TUTO_DBG_VRAMp_new,
    SOSE_ADDR_VRAMp_ext,
    APOV_CPU_WRp_clkevn,
    SERE_CPU_VRAM_RDp_new
  ](){

    /* p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp_ext = and2(SOSE_ADDR_VRAMp_ext, APOV_CPU_WRp_clkevn);
    /* p25.SUDO*/ wire _SUDO_MWRp_new = not1(/*vram_bus.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /* p25.TYJY*/ wire _TYJY_VRAM_WRp_new = mux2p(TUTO_DBG_VRAMp_new, _SUDO_MWRp_new, _TUJA_CPU_VRAM_WRp_ext);
    /* p25.SOHY*/ wire _SOHY_MWRn_new   = nand2(_TYJY_VRAM_WRp_new, SERE_CPU_VRAM_RDp_new);

    /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(TUTO_DBG_VRAMp_new);
    /* p25.TAXY*/ wire _TAXY_MWRn_A_new = and2(_SOHY_MWRn_new, _RACO_DBG_VRAMn_new);
    /* p25.SOFY*/ wire _SOFY_MWRn_D_new =  or2(_SOHY_MWRn_new, TUTO_DBG_VRAMp_new);
    /* p25.SYSY*/ wire _SYSY_MWRp_A_new = not1(_TAXY_MWRn_A_new);
    /* p25.RAGU*/ wire _RAGU_MWRp_D_new = not1(_SOFY_MWRn_D_new);

    PIN_VRAM_WRn.pin_out(_SYSY_MWRp_A_new, _RAGU_MWRp_D_new);
  }();

  //----------------------------------------
  // PIN_VRAM_OEn

  [
    this,
    XYMU_RENDERINGp_new_xxx,
    TUTO_DBG_VRAMp_new,
    SALE_CPU_VRAM_WRn_new
  ](){
    /*#p29.TYFO*/ wire _TYFO_SFETCH_S0p_D1_new = sprite_fetcher.TYFO_SFETCH_S0p_D1_evn.qp_new();
    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_new = not1(sprite_fetcher.TOXE_SFETCH_S0p_odd.qp_new());
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_new = nand2(_TYFO_SFETCH_S0p_D1_new, _TYTU_SFETCH_S0n_new);

    /* p29.SAKY*/ wire _SAKY_SFETCHn_new = nor2(sprite_fetcher.TULY_SFETCH_S1p_odd.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGp_new = not1(XYMU_RENDERINGp_new_xxx);
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn_new = or2(_SAKY_SFETCHn_new, _TEPA_RENDERINGp_new); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp_new = not1(_TYSO_SFETCHINGn_new);
    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp_new = and2(_TACU_SPR_SEQ_5_TRIG_new, _TEXY_SFETCHINGp_new);

    /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new_evn = nor2(dma_reg.LUFA_DMA_VRAMp_new(), XYMU_RENDERINGp_new_xxx);
    /* p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn_new = nand2(SALE_CPU_VRAM_WRn_new, _XANE_VRAM_LOCKn_new_evn);
    /* p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn_new = not1(_SOHO_SPR_VRAM_RDp_new);
    /* p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn_new = not1(tile_fetcher.LONY_FETCHINGp_xxx.qp_new());
    /* p25.APAM*/ wire _APAM_DMA_VRAMn_new    = not1(dma_reg.LUFA_DMA_VRAMp_new());
    /* p25.RACU*/ wire _RACU_MOEn_new   = and4(_RYLU_CPU_VRAM_RDn_new, _RAWA_SPR_VRAM_RDn_new, _MYMA_BGW_VRAM_RDn_new, _APAM_DMA_VRAMn_new); // def and

    /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(TUTO_DBG_VRAMp_new);
    /* p25.SEMA*/ wire _SEMA_MOEn_A_new = and2(_RACU_MOEn_new, _RACO_DBG_VRAMn_new);
    /* p25.RUTE*/ wire _RUTE_MOEn_D_new =  or2(_RACU_MOEn_new, TUTO_DBG_VRAMp_new); // schematic wrong, second input is RACU
    /* p25.REFO*/ wire _REFO_MOEn_A_new = not1(_SEMA_MOEn_A_new);
    /* p25.SAHA*/ wire _SAHA_MOEn_D_new = not1(_RUTE_MOEn_D_new);
    PIN_VRAM_OEn.pin_out(_REFO_MOEn_A_new, _SAHA_MOEn_D_new);
  }();

  //----------------------------------------
  // The actual VRAM

  uint16_t vram_addr_latch;
  uint8_t vram_data_latch;

  [this, &vram_addr_latch, &vram_data_latch](){
    vram_addr_latch = pack_u16p(13, PIN_VRAM_Ap);
    vram_data_latch = pack_u8p (8,  PIN_VRAM_Dp);

    if (!PIN_VRAM_WRn.qp_new()) {
      vid_ram[vram_addr_latch] = vram_data_latch;
    }
    if (!PIN_VRAM_OEn.qp_new()) {
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

    PIN_VRAM_Dp[0].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x01);
    PIN_VRAM_Dp[1].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x02);
    PIN_VRAM_Dp[2].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x04);
    PIN_VRAM_Dp[3].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x08);
    PIN_VRAM_Dp[4].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x10);
    PIN_VRAM_Dp[5].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x20);
    PIN_VRAM_Dp[6].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x40);
    PIN_VRAM_Dp[7].pin_in(!PIN_VRAM_OEn.qp_new(), vram_data_latch & 0x80);

    /* p25.RODY*/ BUS_VRAM_Dp[0].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[0].qn_new());
    /* p25.REBA*/ BUS_VRAM_Dp[1].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[1].qn_new());
    /* p25.RYDO*/ BUS_VRAM_Dp[2].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[2].qn_new());
    /* p25.REMO*/ BUS_VRAM_Dp[3].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[3].qn_new());
    /* p25.ROCE*/ BUS_VRAM_Dp[4].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[4].qn_new());
    /* p25.ROPU*/ BUS_VRAM_Dp[5].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[5].qn_new());
    /* p25.RETA*/ BUS_VRAM_Dp[6].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[6].qn_new());
    /* p25.RAKU*/ BUS_VRAM_Dp[7].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp[7].qn_new());
  }();

  //----------------------------------------
  // VRAM bus to CPU bus

  [this, TEDO_CPU_RDp, CATY_LATCH_EXTp_ext, SERE_CPU_VRAM_RDp_new]() {
    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);
    /* p28.MYNU*/ wire _MYNU_CPU_RDn_ext = nand2(_ASOT_CPU_RDp_ext, CATY_LATCH_EXTp_ext);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp_ext = not1(_MYNU_CPU_RDn_ext);

    /* p25.RERY*/ wire _RERY_VBUS_D0n_new = not1(BUS_VRAM_Dp[0].qp_new());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n_new = not1(BUS_VRAM_Dp[1].qp_new());
    /* p25.RONA*/ wire _RONA_VBUS_D2n_new = not1(BUS_VRAM_Dp[2].qp_new());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n_new = not1(BUS_VRAM_Dp[3].qp_new());
    /* p25.SANA*/ wire _SANA_VBUS_D4n_new = not1(BUS_VRAM_Dp[4].qp_new());
    /* p25.RORO*/ wire _RORO_VBUS_D5n_new = not1(BUS_VRAM_Dp[5].qp_new());
    /* p25.RABO*/ wire _RABO_VBUS_D6n_new = not1(BUS_VRAM_Dp[6].qp_new());
    /* p25.SAME*/ wire _SAME_VBUS_D7n_new = not1(BUS_VRAM_Dp[7].qp_new());

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    wire MATU_DMA_RUNNINGp,
    wire XYMU_RENDERINGp,
    wire ACYL_SCANNINGp)
{
  //----------------------------------------
  // OAM address bus mux

  [this, MATU_DMA_RUNNINGp](){
    // DMA OAM write address driver
    /* p04.DUGA*/ wire _DUGA_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
    /* p28.FODO*/ oam_bus.BUS_OAM_An[0].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.NAKY_DMA_A00p_evn.qp_new());
    /* p28.FESA*/ oam_bus.BUS_OAM_An[1].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.PYRO_DMA_A01p_evn.qp_new());
    /* p28.FAGO*/ oam_bus.BUS_OAM_An[2].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.NEFY_DMA_A02p_evn.qp_new());
    /* p28.FYKY*/ oam_bus.BUS_OAM_An[3].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.MUTY_DMA_A03p_evn.qp_new());
    /* p28.ELUG*/ oam_bus.BUS_OAM_An[4].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.NYKO_DMA_A04p_evn.qp_new());
    /* p28.EDOL*/ oam_bus.BUS_OAM_An[5].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.PYLO_DMA_A05p_evn.qp_new());
    /* p28.FYDU*/ oam_bus.BUS_OAM_An[6].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.NUTO_DMA_A06p_evn.qp_new());
    /* p28.FETU*/ oam_bus.BUS_OAM_An[7].tri6_nn(_DUGA_DMA_RUNNINGn_new_evn, dma_reg.MUGU_DMA_A07p_evn.qp_new());
  }();

  [this, XYMU_RENDERINGp, MATU_DMA_RUNNINGp](){
    wire VCC = 1;
    // OAM address from sprite fetcher
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.BETE*/ wire _BETE_SFETCHINGn_new = not1(_AJON_RENDERINGp_new_evn);
    /* p28.GECA*/ oam_bus.BUS_OAM_An[0].tri6_nn(_BETE_SFETCHINGn_new, VCC);
    /* p28.WYDU*/ oam_bus.BUS_OAM_An[1].tri6_nn(_BETE_SFETCHINGn_new, VCC);
    /* p28.GYBU*/ oam_bus.BUS_OAM_An[2].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[0].qp_new());
    /* p28.GYKA*/ oam_bus.BUS_OAM_An[3].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[1].qp_new());
    /* p28.FABY*/ oam_bus.BUS_OAM_An[4].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[2].qp_new());
    /* p28.FACO*/ oam_bus.BUS_OAM_An[5].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[3].qp_new());
    /* p28.FUGU*/ oam_bus.BUS_OAM_An[6].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[4].qp_new());
    /* p28.FYKE*/ oam_bus.BUS_OAM_An[7].tri6_nn(_BETE_SFETCHINGn_new, SPR_TRI_I[5].qp_new());
  }();

  [this, ACYL_SCANNINGp](){
    // OAM address from sprite scanner
    wire GND = 0;
    /* p28.APAR*/ wire _APAR_SCANNINGn_new_evn = not1(ACYL_SCANNINGp);
    /* p28.GEFY*/ oam_bus.BUS_OAM_An[0].tri6_nn(_APAR_SCANNINGn_new_evn, GND);
    /* p28.WUWE*/ oam_bus.BUS_OAM_An[1].tri6_nn(_APAR_SCANNINGn_new_evn, GND);
    /* p28.GUSE*/ oam_bus.BUS_OAM_An[2].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.YFEL_SCAN0_evn.qp_new());
    /* p28.GEMA*/ oam_bus.BUS_OAM_An[3].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.WEWY_SCAN1_evn.qp_new());
    /* p28.FUTO*/ oam_bus.BUS_OAM_An[4].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.GOSO_SCAN2_evn.qp_new());
    /* p28.FAKU*/ oam_bus.BUS_OAM_An[5].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.ELYN_SCAN3_evn.qp_new());
    /* p28.GAMA*/ oam_bus.BUS_OAM_An[6].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.FAHA_SCAN4_evn.qp_new());
    /* p28.GOBY*/ oam_bus.BUS_OAM_An[7].tri6_nn(_APAR_SCANNINGn_new_evn, sprite_scanner.FONY_SCAN5_evn.qp_new());
  }();

  //----------------------------------------
  // DMA write cart data to oam

  oam_bus.ext_to_bus(
    PIN_EXT_D,
    MATU_DMA_RUNNINGp,
    dma_reg.PULA_DMA_A13n_h.qn_new(),
    dma_reg.POKU_DMA_A14n_h.qn_new(),
    dma_reg.MARU_DMA_A15n_h.qn_new());

  //----------------------------------------
  // DMA write VRAM to oam

  oam_bus.vram_to_bus(
    BUS_VRAM_Dp,
    MATU_DMA_RUNNINGp,
    dma_reg.PULA_DMA_A13n_h.qn_new(),
    dma_reg.POKU_DMA_A14n_h.qn_new(),
    dma_reg.MARU_DMA_A15n_h.qn_new());

  //----------------------------------------
  // CPU write to OAM

  oam_bus.cpu_to_bus(
    BUS_CPU_A,
    BUS_CPU_D,
    UVYT_ABCDxxxx,
    TAPU_CPU_WRp,
    XYMU_RENDERINGp,
    MATU_DMA_RUNNINGp,
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
    MATU_DMA_RUNNINGp,
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
    MATU_DMA_RUNNINGp,
    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    BUS_CPU_D_out
  );

  oam_latch_b.latch_to_cpu(
    BUS_CPU_A,
    oam_bus.BUS_OAM_An,
    TEDO_CPU_RDp,
    CATY_LATCH_EXTp,
    MATU_DMA_RUNNINGp,
    ACYL_SCANNINGp,
    XYMU_RENDERINGp,
    BUS_CPU_D_out
  );
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------

#endif
