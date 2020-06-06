#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"

#pragma warning(disable:4456)

using namespace Schematics;

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)

0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)

0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end

0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end

0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

TestGB gb;


static const uint8_t DMG_ROM_bin[] = {
  0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32,
  0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e,
  0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
  0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0,
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a,
  0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
  0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06,
  0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9,
  0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
  0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20,
  0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64,
  0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
  0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90,
  0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2,
  0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
  0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06,
  0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xf0, 0x42,
  0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
  0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04,
  0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17,
  0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,

  // the logo @ 0x00A8
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
  0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
  0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
  0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,

  0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c,
  0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13,
  0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
  0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
  0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50
};

//-----------------------------------------------------------------------------

void TestGB::tick_everything() {
  //----------------------------------------
  // sch_system

  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;
  
  /*p01.ANOS*/ wire ANOS_AxCxExGx = not(sys_pins.CLK_IN);
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_pins.CLKREQ);
  /*p01.UCOB*/ wire UCOB_CLKBAD   = not(sys_pins.CLK_GOOD);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(sys_pins.CLK_GOOD);

  /*p07.UBET*/   wire UBET_T1n = not(sys_pins.T1);
  /*p07.UVAR*/   wire UVAR_T2n = not(sys_pins.T2);
  /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, sys_pins.RST);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1 = and(sys_pins.T1, UVAR_T2n);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, UBET_T1n);
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not(UNOR_MODE_DBG2);
  /*p08.RYCA*/ wire RYCA_MODE_DBG2n = not(UNOR_MODE_DBG2);

  // FIXME I probably have references to these swapped somewhere
  /*p28.WEFE*/ wire WEFE_P10_Bn = not(joy_pins.P10_B);
  /*p27.VYPO*/ wire VYPO_P10_Bn = not(joy_pins.P10_B);

  //----------------------------------------
  // sch_clocks

  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH); // dma, sprite store
  /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx); // video, sprites
  /*p27.MOXE*/ wire MOXE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p27.MEHE*/ wire MEHE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p01.LAPE*/ wire LAPE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p27.TAVA*/ wire TAVA_xBxDxFxH = not(LAPE_AxCxExGx);

  /*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_AxCxExGx);
  /*p01.AMUK*/ wire AMUK_AxCxExGx = not(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet

  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not( clk_reg.PHAZ_xxCDEFxx);
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(!clk_reg.PHAZ_xxxDEFGx);
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not( clk_reg.PHAZ_xxxxEFGH);

  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);

  // joypad, reset, timer, debug clock
  /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn, AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
  /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);

  /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
  /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
  /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
  /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
  /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
  /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
  /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);
  /*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_AxxxxFGH); // and then this goes to channel 1

  // BAZE here seems incongruous
  /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);


  /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
  /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
  /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
  /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);

  /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);
  /*p01.BUVU*/ wire BUVU_xBxxxxxx = and(BUTY_CLKREQ, BALY_xBxxxxxx);
  /*p01.BYXO*/ wire BYXO_AxCDEFGH = not(BUVU_xBxxxxxx);
  /*p01.BEDO*/ wire BEDO_xBxxxxxx = not(BYXO_AxCDEFGH);
  /*p01.BOWA*/ wire BOWA_AxCDEFGH = not(BEDO_xBxxxxxx);

  // wave ram write clock
  /*p01.BUGO*/ wire BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
  /*p01.BATE*/ wire BATE_ABxxxxxH = nor(ABOL_CLKREQn, BUGO_xxCDEFxx, AROV_xxxDEFGx);
  /*p01.BASU*/ wire BASU_xxCDEFGx = not(BATE_ABxxxxxH);
  /*p01.BUKE*/ wire BUKE_ABxxxxxH = not(BASU_xxCDEFGx);
  /*p17.ABUR*/ wire ABUR_xxCDEFGx = not(BUKE_ABxxxxxH);
  /*p17.BORY*/ wire BORY_ABxxxxxH = not(ABUR_xxCDEFGx);

  /*p01.AFUR*/ clk_reg.PHAZ_xBCDExxx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD, !clk_reg.PHAZ_xxxxEFGH.a);
  /*p01.ALEF*/ clk_reg.PHAZ_xxCDEFxx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  clk_reg.PHAZ_xBCDExxx.a);
  /*p01.APUK*/ clk_reg.PHAZ_xxxDEFGx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  clk_reg.PHAZ_xxCDEFxx.a);
  /*p01.ADYK*/ clk_reg.PHAZ_xxxxEFGH.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  clk_reg.PHAZ_xxxDEFGx.a);

  wave_pins.BORY_ABxxxxxH.set(BORY_ABxxxxxH);

  //----------------------------------------
  // sch_resets

  /*p01.UNUT*/ wire TIMEOUT   = and(rst_reg.WAITING_FOR_CLKREQ, tim_reg.DIV_15);
  /*p01.TABA*/ wire TABA_RST  = or (UNOR_MODE_DBG2, UMUT_MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire ALYP_RSTn = not(TABA_RST);
  /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, sys_pins.RST);
  /*p01.ASOL*/ wire ASOL_RST  = or (AFAR_RST, sys_pins.RST);
  /*p01.AVOR*/ wire AVOR_RST  = or (rst_reg.RESET_REG, ASOL_RST);
  /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RST);   // this goes all over the place
  /*p01.DULA*/ wire DULA_RST  = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RST);
  /*p01.XORE*/ wire XORE_RST  = not(CUNU_RSTn);
  /*p01.XEBE*/ wire XEBE_RST  = not(XORE_RST);
  /*p01.WESY*/ wire WESY_RST  = not(XORE_RST);


  /*p01.UPYF*/ wire UPYF = or(sys_pins.RST, UCOB_CLKBAD);
  /*p01.XODO*/ wire XODO_VID_RST  = nand(XEBE_RST, cfg_reg.LCDC_EN);
  /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RST);
  /*p01.LYHA*/ wire LYHA_VID_RST  = not(XAPO_VID_RSTn);
  /*p01.TOFU*/ wire TOFU_VID_RST  = not(XAPO_VID_RSTn);
  /*p01.ATAR*/ wire ATAR_VID_RST  = not(XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RST);
  /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RST);
  /*p01.AMYG*/ wire AMYG_VID_RST  = not(XAPO_VID_RSTn);
  /*p01.PYRY*/ wire PYRY_VID_RST  = not(XAPO_VID_RSTn);

  /*p01.TUBO*/ rst_reg.WAITING_FOR_CLKREQ.sr_latch(!UPYF, !ABOL_CLKREQn);
  /*p01.AFER*/ rst_reg.RESET_REG.set(BOMA_xBxxxxxx, UPOJ_MODE_PROD, ASOL_RST);

  //----------------------------------------
  // sch_clocks

  /*p29.WUVU*/ wire WUVU_AxxDExxH = vclk_reg.WUVU_AxxDExxH;
  /*p21.VENA*/ wire VENA_xBCDExxx = vclk_reg.VENA_xBCDExxx;
  /*p29.WOSU*/ wire WOSU_xxCDxxGH = vclk_reg.WOSU_xxCDxxGH;
  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);
  /*p29.XUPY*/ wire XUPY_xBCxxFGx = not(WUVU_AxxDExxH);  // lcd, sprites
  /*p21.TALU*/ wire TALU_xBCDExxx = not(!VENA_xBCDExxx); // this drives the LCD xy counter
  /*p21.SONO*/ wire SONO_AxxxxFGH = not(TALU_xBCDExxx);  // lcd
  /*p29.XOCE*/ wire XOCE_ABxxEFxx = not(WOSU_xxCDxxGH);  // oam, sprites

  /*p29.WUVU*/ vclk_reg.WUVU_AxxDExxH.set( XOTA_xBxDxFxH, XAPO_VID_RSTn, !WUVU_AxxDExxH);
  /*p21.VENA*/ vclk_reg.VENA_xBCDExxx.set(!WUVU_AxxDExxH, XAPO_VID_RSTn, !VENA_xBCDExxx);
  /*p29.WOSU*/ vclk_reg.WOSU_xxCDxxGH.set( XYFY_AxCxExGx, XAPO_VID_RSTn, !WUVU_AxxDExxH);

  //----------------------------------------
  // CPU pins

  cpu_pins.CLK_GOOD.set(sys_pins.CLK_GOOD);
  cpu_pins.UMUT_MODE_DBG1.set(UMUT_MODE_DBG1);
  cpu_pins.UNOR_MODE_DBG2.set(UNOR_MODE_DBG2);

  cpu_pins.CPU_RESET.set(TABA_RST);
  cpu_pins.AFER.set(rst_reg.RESET_REG);
  cpu_pins.PIN_RESET.set(sys_pins.RST);

  cpu_pins.UPOJ_MODE_PROD  = UPOJ_MODE_PROD;
  cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;

  // phases checked and ok
  cpu_pins.BOWA_AxCDEFGH.set(BOWA_AxCDEFGH);
  cpu_pins.BEDO_xBxxxxxx.set(BEDO_xBxxxxxx);

  cpu_pins.BEKO_xBCDExxx.set(BEKO_xBCDExxx);
  cpu_pins.BUDE_AxxxxFGH.set(BUDE_AxxxxFGH);

  cpu_pins.BOLO_xBCDEFGx.set(BOLO_xBCDEFGx);
  cpu_pins.BUKE_ABxxxxxH.set(BUKE_ABxxxxxH);

  cpu_pins.BOGA_AxCDEFGH.set(BOGA_AxCDEFGH);
  cpu_pins.BOMA_xBxxxxxx.set(BOMA_xBxxxxxx);

  //----------------------------------------
  // sch_decoder

  /*p07.TUNA*/ wire TUNA_0000_FDFF = nand(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09);

  /*p07.TONA*/   wire ADDR_08n = not(cpu_pins.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(TUNA_0000_FDFF, ADDR_08n);


  /*p22.XOLA*/ wire A00n = not(cpu_pins.A00);
  /*p22.XENO*/ wire A01n = not(cpu_pins.A01);
  /*p22.XUSY*/ wire A02n = not(cpu_pins.A02);
  /*p22.XERA*/ wire A03n = not(cpu_pins.A03);


  /*p22.WADO*/ wire WADO_A00 = not(A00n);
  /*p22.WESA*/ wire WESA_A01 = not(A01n);
  /*p22.WALO*/ wire WALO_A02 = not(A02n);
  /*p22.WEPO*/ wire WEPO_A03 = not(A03n);

  /*p22.XALY*/   wire ADDR_0x00xxxx  = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A04);
  /*p22.WUTU*/ wire FF4Xn = nand(ADDR_FFXX, cpu_pins.A06, ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X  = not(FF4Xn);
  /*p22.WORU*/ wire FF40n = nand(FF4X, A00n,     A01n,     A02n,     A03n);
  /*p22.WOFA*/ wire FF41n = nand(FF4X, WADO_A00, A01n,     A02n,     A03n);
  /*p22.WAVU*/ wire FF43n = nand(FF4X, WADO_A00, WESA_A01, A02n,     A03n);
  /*p22.WYLE*/ wire FF44n = nand(FF4X, A00n,     A01n,     WALO_A02, A03n);
  /*p22.WETY*/ wire FF45n = nand(FF4X, WADO_A00, A01n,     WALO_A02, A03n);
  /*p22.WATE*/ wire FF46n = nand(FF4X, A00n,     WESA_A01, WALO_A02, A03n);
  /*p22.WYBO*/ wire FF47n = nand(FF4X, WADO_A00, WESA_A01, WALO_A02, A03n);
  /*p22.WETA*/ wire FF48n = nand(FF4X, A00n,     A01n,     A02n,     WEPO_A03);
  /*p22.VAMA*/ wire FF49n = nand(FF4X, WADO_A00, A01n,     A02n,     WEPO_A03);
  /*p22.WYVO*/ wire FF4An = nand(FF4X, A00n,     WESA_A01, A02n,     WEPO_A03);
  /*p22.WAGE*/ wire FF4Bn = nand(FF4X, WADO_A00, WESA_A01, A02n,     WEPO_A03);


  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(TUNA_0000_FDFF);
  /*p07.SOHA*/   wire ADDR_FFXXn2 = not(ADDR_FFXX);
  /*p07.ROPE*/   wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ wire ADDR_OAM = not(ADDR_FEXXn);

  /*p01.AJAX*/   wire AJAX_xxxxEFGH = not(ATYP_xBCDExxx);
  /*p01.AGUT*/   wire AGUT_xxCDEFGH = and(or(AJAX_xxxxEFGH, AROV_xxxDEFGx), cpu_pins.ADDR_VALIDn);
  /*p01.AWOD*/   wire AWOD = or(UNOR_MODE_DBG2, AGUT_xxCDEFGH);
  /*p01.ABUZ*/ wire ABUZ_ADDR_VALIDn_ABxxxxxx = not(AWOD);

  /*p03.TOVY*/ wire TOVY_A00n = not(cpu_pins.A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(cpu_pins.A01);

  /*p10.AMUS*/ wire ADDR_0xx00000 = nor(cpu_pins.A00, cpu_pins.A01, cpu_pins.A02, cpu_pins.A03, cpu_pins.A04, cpu_pins.A07);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_0xx00000, ADDR_FFXX);

  /*p08.SORE*/   wire ADDR_0000_7FFF = not(cpu_pins.A15);
  /*p08.TEVY*/   wire ADDR_NOT_VRAM = or(cpu_pins.A13, cpu_pins.A14, ADDR_0000_7FFF);
  /*p08.TEXO*/ wire TEXO_ADDR_VALID_AND_NOT_VRAM = and(cpu_pins.ADDR_VALIDn, ADDR_NOT_VRAM);

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(cpu_pins.A07, cpu_pins.A06, cpu_pins.A05, cpu_pins.A04, cpu_pins.A03);
  /*p03.RYFO*/ wire FF04_FF07 = and(cpu_pins.A02, ADDR_XX00_XX07, ADDR_FFXX);

  /*p07.TERA*/   wire BOOT_BITn  = not(bus_reg.BOOT_BIT);
  /*p07.TULO*/   wire ADDR_00XX  = nor(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09, cpu_pins.A08);
  /*p07.TUTU*/ wire TUTU_ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

  //----------------------------------------
  // sch_buscontrol

  /*p07.UJYV*/ wire UJYV_BUS_RD_MUX       = mux2n(ext_pins.RD_C, cpu_pins.CPU_RAW_RD, UNOR_MODE_DBG2);
  /*p07.TEDO*/ wire TEDO_BUS_RD           = not(UJYV_BUS_RD_MUX);
  /*p07.AJAS*/ wire AJAS_BUS_RDn          = not(TEDO_BUS_RD);
  /*p07.ASOT*/ wire ASOT_BUS_RD_ABCDEFGH  = not(AJAS_BUS_RDn);

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH  = not(AREV_CPU_WRn_ABCDExxx);

  /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH, UNOR_MODE_DBG2);
  /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
  /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_BUS_WR_xxxxxFGH);
  /*p07.CUPA*/ wire CUPA_BUS_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);

  //----------------------------------------
  // sch_lcd

  /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(lcd_reg.RUTU_NEW_LINE_d0);
  /*p21.SELA*/ wire SELA_NEW_LINE_d0  = not(PURE_NEW_LINE_d0n);
  /*p24.KASA*/ wire LINE_DONEb = not(PURE_NEW_LINE_d0n);
  
  /*p28.ABAF*/   wire VID_LINE_d4n = not(lcd_reg.VID_LINE_d4);
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or(lcd_reg.VID_LINE_d6, VID_LINE_d4n), ABEZ_VID_RSTn);
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4  = not(BYHA_VID_LINE_TRIG_d4n);

  // Vertical sync
  /*p24.NERU*/ wire LINE_000n = nor(lcd_reg.Y0, lcd_reg.Y1, lcd_reg.Y2, lcd_reg.Y3, lcd_reg.Y4, lcd_reg.Y5, lcd_reg.Y6, lcd_reg.Y7);

  // PIN_CPL
  {
    /*p24.KEDY*/ wire LCDC_ENn = not(cfg_reg.LCDC_EN);
    /*p01.UMEK*/ wire DIV_06n = not(tim_reg.DIV_06);
    /*p24.UMOB*/ wire DIV_06 = not(DIV_06n);
    /*p24.KAHE*/ wire CPLn = amux2(cfg_reg.LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
    /*p24.KYMO*/ wire KYMO = not(CPLn);
    lcd_pins.CPL.set(KYMO);
  }

  // so this is like a strobe that fires 4x per line


  /*p21.MUDE*/ wire X_RSTn = nor(lcd_reg.RUTU_NEW_LINE_d0, LYHA_VID_RST);
  /*p21.LAMA*/ wire FRAME_RSTn = nor(lcd_reg.LINE_153_d4, LYHA_VID_RST);

  /*p21.SANU*/ wire LINE_END  = and(lcd_reg.X6, lcd_reg.X5, lcd_reg.X4, lcd_reg.X0); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p21.NOKO*/ wire LINE_153  = and(lcd_reg.Y7, lcd_reg.Y4, lcd_reg.Y3, lcd_reg.Y0); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*p21.XYVO*/ wire IN_VBLANK = and(lcd_reg.Y4, lcd_reg.Y7); // 128 + 16 = 144
  
  /*p28.AWOH*/ wire AWOH_AxxDExxH = not(XUPY_xBCxxFGx);  // lcd

  /*p29.ALES*/   wire IN_VBLANKn = not(IN_VBLANK);
  /*p29.ABOV*/ wire VID_LINE_d0 = and(SELA_NEW_LINE_d0, IN_VBLANKn);

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4, ATAR_VID_RST);
  /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
  /*p29.BEBU*/   wire SCAN_DONE_d0_TRIGn = or(BALU_SCAN_RST, spr_reg.SCAN_DONE_d5, !spr_reg.SCAN_DONE_d4);
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);


  // PIN_S PIN_CPG PIN_ST, lcd regs
  {
    // lcd horizontal sync pin
    /*p24.POFY*/ wire POFY = not(lcd_reg.RUJU);
    /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY);

    // lcd line strobe
    /*p21.TOCU*/ wire C0n = not(lcd_reg.X0);
    /*p21.VEPE*/ wire C1n = not(lcd_reg.X1);
    /*p21.VUTY*/ wire C2n = not(lcd_reg.X2);
    /*p21.VATE*/ wire C3n = not(lcd_reg.X3);
    /*p21.TUDA*/ wire C4n = not(lcd_reg.X4);
    /*p21.TAFY*/ wire C5n = not(lcd_reg.X5);
    /*p21.TUJU*/ wire C6n = not(lcd_reg.X6);

    /*p21.VOKU*/ wire CNT_000n = nand(C6n, C5n, C4n, C3n, C2n, C1n, C0n); // 0000000 == 0
    /*p21.TOZU*/ wire CNT_007n = nand(C6n, C5n, C4n, C3n, lcd_reg.X2,  lcd_reg.X1,  lcd_reg.X0 ); // 0000111 == 7
    /*p21.TECE*/ wire CNT_045n = nand(C6n, lcd_reg.X5,  C4n, lcd_reg.X3,  lcd_reg.X2,  C1n, lcd_reg.X0 ); // 0101101 == 45
    /*p21.TEBO*/ wire CNT_083n = nand(lcd_reg.X6,  C5n, lcd_reg.X4,  C3n, C2n, lcd_reg.X1,  lcd_reg.X0 ); // 1010011 == 83

    /*p21.TEGY*/ wire LINE_STROBE = nand(CNT_000n, CNT_007n, CNT_045n, CNT_083n);
    /*p21.RYNO*/ wire CPGn = or(lcd_reg.LINE_STROBE, lcd_reg.RUTU_NEW_LINE_d0);
    /*p21.POGU*/ wire POGU_PIN_CPG = not(CPGn);

    /*p24.MURE*/ wire MURE_PIN_S = not(lcd_reg.VSYNC_OUTn);

    /*p24.MEDA*/ lcd_reg.VSYNC_OUTn.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, LINE_000n);

    /*p21.SYGU*/ lcd_reg.LINE_STROBE.set(SONO_AxxxxFGH, LYFE_VID_RSTn, LINE_STROBE);
    /*p21.MYTA*/ lcd_reg.LINE_153_d4.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, LINE_153);
    /*p21.POPU*/ lcd_reg.POPU_IN_VBLANK_d4.set(lcd_reg.NYPE_NEW_LINE_d4, LYFE_VID_RSTn, IN_VBLANK);
    /*p21.NYPE*/ lcd_reg.NYPE_NEW_LINE_d4.set(TALU_xBCDExxx, LYFE_VID_RSTn, lcd_reg.RUTU_NEW_LINE_d0);
    /*p28.ANEL*/ lcd_reg.VID_LINE_d6.set(AWOH_AxxDExxH, ABEZ_VID_RSTn, lcd_reg.VID_LINE_d4);
    /*p29.CATU*/ lcd_reg.VID_LINE_d4.set(XUPY_xBCxxFGx, ABEZ_VID_RSTn, VID_LINE_d0);
    /*p24.POME*/ lcd_reg.POME.sr_latch(lcd_reg.X_8_SYNC || TOFU_VID_RST, AVAP_SCAN_DONE_d0_TRIG);   // Latch loop
    /*p24.RUJU*/ lcd_reg.RUJU.sr_latch(lcd_reg.X_8_SYNC || TOFU_VID_RST, AVAP_SCAN_DONE_d0_TRIG);

    /*p21.TYRY*/ lcd_reg.X6.set(!lcd_reg.X5,   X_RSTn, !lcd_reg.X6);
    /*p21.TAHA*/ lcd_reg.X5.set(!lcd_reg.X4,   X_RSTn, !lcd_reg.X5);
    /*p21.SUDE*/ lcd_reg.X4.set(!lcd_reg.X3,   X_RSTn, !lcd_reg.X4);
    /*p21.TELU*/ lcd_reg.X3.set(!lcd_reg.X2,   X_RSTn, !lcd_reg.X3);
    /*p21.VYZO*/ lcd_reg.X2.set(!lcd_reg.X1,   X_RSTn, !lcd_reg.X2);
    /*p21.TYPO*/ lcd_reg.X1.set(!lcd_reg.X0,   X_RSTn, !lcd_reg.X1);
    /*p21.SAXO*/ lcd_reg.X0.set(TALU_xBCDExxx, X_RSTn, !lcd_reg.X0);

    /*p21.LAFO*/ lcd_reg.Y7.set(!lcd_reg.Y6, FRAME_RSTn, !lcd_reg.Y7);
    /*p21.MATO*/ lcd_reg.Y6.set(!lcd_reg.Y5, FRAME_RSTn, !lcd_reg.Y6);
    /*p21.LEMA*/ lcd_reg.Y5.set(!lcd_reg.Y4, FRAME_RSTn, !lcd_reg.Y5);
    /*p21.LOVU*/ lcd_reg.Y4.set(!lcd_reg.Y3, FRAME_RSTn, !lcd_reg.Y4);
    /*p21.LYDO*/ lcd_reg.Y3.set(!lcd_reg.Y2, FRAME_RSTn, !lcd_reg.Y3);
    /*p21.LEXA*/ lcd_reg.Y2.set(!lcd_reg.Y1, FRAME_RSTn, !lcd_reg.Y2);
    /*p21.MYRO*/ lcd_reg.Y1.set(!lcd_reg.Y0, FRAME_RSTn, !lcd_reg.Y1);
    /*p21.MUWY*/ lcd_reg.Y0.set(lcd_reg.RUTU_NEW_LINE_d0, FRAME_RSTn, !lcd_reg.Y0);

    /*p21.RUTU*/ lcd_reg.RUTU_NEW_LINE_d0.set(SONO_AxxxxFGH, LYFE_VID_RSTn, LINE_END);

    lcd_pins.S.set(MURE_PIN_S);
    lcd_pins.CPG.set(POGU_PIN_CPG);
    lcd_pins.ST.set(RUZE_PIN_ST);
  }


  //----------------------------------------
  // sch_debug - this is a hacky mess right now


  ///*p05.KURA*/ wire FF60_0n = not(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not(FF60);

  /*p07.BURO*/ dbg_reg.FF60_0.set(1, ALUR_RSTn, cpu_pins.D0);
  /*p07.AMUT*/ dbg_reg.FF60_1.set(1, ALUR_RSTn, cpu_pins.D1);

#if 0

  /*p07.APET*/ wire APET_MODE_DBG = or(UMUT_MODE_DBG1, UNOR_MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_pins.A05, cpu_pins.A06, TAPU_CPUWR, ADDR_111111110xx00000);

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_sig.BEDO_xBxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_sig.BEDO_xBxxxxxx, sys_sig.MODE_DBG2);

  if (AFOP_Axxxxxxx) cpu_pins.set_data(
    /*p05.ANOC*/ not(sys_sig.PIN_P10_B),
    /*p05.ATAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.AJEC*/ not(sys_sig.PIN_P10_B),
    /*p05.ASUZ*/ not(sys_sig.PIN_P10_B),
    /*p05.BENU*/ not(sys_sig.PIN_P10_B),
    /*p05.AKAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.ARAR*/ not(sys_sig.PIN_P10_B),
    /*p05.BEDA*/ not(sys_sig.PIN_P10_B)
  );

  if (LECO_xBCDEFGH) cpu_pins.set_data(
    /*p07.ROMY*/ sys_sig.PIN_P10_B,
    /*p07.RYNE*/ sys_sig.PIN_P10_B,
    /*p07.REJY*/ sys_sig.PIN_P10_B,
    /*p07.RASE*/ sys_sig.PIN_P10_B,
    /*p07.REKA*/ sys_sig.PIN_P10_B,
    /*p07.ROWE*/ sys_sig.PIN_P10_B,
    /*p07.RYKE*/ sys_sig.PIN_P10_B,
    /*p07.RARU*/ sys_sig.PIN_P10_B
  );

  // FF60 debug reg
  /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_pins.A05, cpu_pins.A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not(FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(FF60_0);


  /*p07.BURO*/ FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_pins.D0);
  /*p07.AMUT*/ FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_pins.D1);


  // FIXME
  ///*p05.KORE*/ wire P05_NC0 = nand(DBG_FF00_D7, FF60_0);
  ///*p05.KYWE*/ wire P05_NC1 = nor (DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RDn = nand(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) cpu_pins.D0 = not(/*p08.TOVO*/ not(pins.D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) cpu_pins.D1 = not(/*p08.RUZY*/ not(pins.D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) cpu_pins.D2 = not(/*p08.ROME*/ not(pins.D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) cpu_pins.D3 = not(/*p08.SAZA*/ not(pins.D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) cpu_pins.D4 = not(/*p08.TEHE*/ not(pins.D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) cpu_pins.D5 = not(/*p08.RATU*/ not(pins.D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) cpu_pins.D6 = not(/*p08.SOCA*/ not(pins.D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) cpu_pins.D7 = not(/*p08.RYBA*/ not(pins.D7_C));


  //----------
  // more debug stuff

  /*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, clk.BOGA_AxCDEFGH);
  /*p25.SOLE*/ wire SOLE = not(TUSO);

  if (VYPO_P10_Bn) bus_out.set_data(
    /*p25.TOVU*/ SOLE,
    /*p25.SOSA*/ SOLE,
    /*p25.SEDU*/ SOLE,
    /*p25.TAXO*/ SOLE,
    /*p25.TAHY*/ SOLE,
    /*p25.TESU*/ SOLE,
    /*p25.TAZU*/ SOLE,
    /*p25.TEWA*/ SOLE
  );

}
#endif

  //----------------------------------------
  // sch_dma
  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p04.MORY*/ Signal DMA_READ_CARTn;
  /*p04.MUHO*/ Signal DMA_READ_VRAMn;
  /*p04.LUFA*/ Signal DMA_READ_VRAM;
  {
    /*p04.LEBU*/   wire DMA_A15n  = not(dma_reg.DMA_A15);
    /*p04.MUDA*/   wire DMA_VRAM  = nor(dma_reg.DMA_A13, dma_reg.DMA_A14, DMA_A15n);
    /*p04.LOGO*/   wire DMA_VRAMn = not(DMA_VRAM);
    /*p04.MORY*/ DMA_READ_CARTn = nand(dma_reg.REG_DMA_RUNNING, DMA_VRAMn);
    /*p04.MUHO*/ DMA_READ_VRAMn = nand(dma_reg.REG_DMA_RUNNING, DMA_VRAM);
    /*p04.LUFA*/ DMA_READ_VRAM = not(DMA_READ_VRAMn);

    /*p22.XEDA*/ wire FF46 = not(FF46n);
    /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, CUPA_BUS_WR_xxxxxFGH);
    /*p04.LORU*/ wire FF46_WR = not(FF46_WRn);

    /*p04.MAKA*/ dma_reg.FROM_CPU5_SYNC.set(ZEME_AxCxExGx, CUNU_RSTn, cpu_pins.FROM_CPU5);

    /*p04.LOKO*/ wire DMA_RST   = nand(CUNU_RSTn, !dma_reg.REG_DMA_EN_d4);
    /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);
    /*p04.NAVO*/ wire DMA_DONEn = nand(dma_reg.DMA_A00, dma_reg.DMA_A01, dma_reg.DMA_A02, dma_reg.DMA_A03, dma_reg.DMA_A04, dma_reg.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire DMA_DONE  = not(DMA_DONEn);
    /*p04.MYTE*/ dma_reg.DMA_DONE_SYNC.set(MOPA_AxxxxFGH, DMA_RSTn, DMA_DONE);

    /*p04.LYXE*/ dma_reg.LATCH_DMA_ENn_d0.sr_latch(DMA_RST, FF46_WRn);
    /*p04.LUPA*/ wire DMA_EN = nor(FF46_WRn, dma_reg.LATCH_DMA_ENn_d0); // this seems redundant

    /*p04.LUVY*/ dma_reg.REG_DMA_EN_d0.set(UVYT_xBCDExxx, CUNU_RSTn, DMA_EN);
    /*p04.LENE*/ dma_reg.REG_DMA_EN_d4.set(MOPA_AxxxxFGH, CUNU_RSTn, dma_reg.REG_DMA_EN_d0);

    // FIXME is this sr_latch or tp_latch?
    /*p04.LOKY*/ dma_reg.LATCH_DMA_EN_d4.sr_latch(dma_reg.REG_DMA_EN_d4, dma_reg.DMA_DONE_SYNC || !CUNU_RSTn); // lara/loky nand sr latch
    /*p04.MATU*/ dma_reg.REG_DMA_RUNNING.set(UVYT_xBCDExxx, CUNU_RSTn, dma_reg.LATCH_DMA_EN_d4);

    /*p04.META*/ wire CLK_DMA_LO = and(UVYT_xBCDExxx, dma_reg.LATCH_DMA_EN_d4);
    /*p04.NAKY*/ dma_reg.DMA_A00.set(CLK_DMA_LO, DMA_RSTn, !dma_reg.DMA_A00);
    /*p04.PYRO*/ dma_reg.DMA_A01.set(!dma_reg.DMA_A00, DMA_RSTn, !dma_reg.DMA_A01);
    /*p04.NEFY*/ dma_reg.DMA_A02.set(!dma_reg.DMA_A01, DMA_RSTn, !dma_reg.DMA_A02);
    /*p04.MUTY*/ dma_reg.DMA_A03.set(!dma_reg.DMA_A02, DMA_RSTn, !dma_reg.DMA_A03);
    /*p04.NYKO*/ dma_reg.DMA_A04.set(!dma_reg.DMA_A03, DMA_RSTn, !dma_reg.DMA_A04);
    /*p04.PYLO*/ dma_reg.DMA_A05.set(!dma_reg.DMA_A04, DMA_RSTn, !dma_reg.DMA_A05);
    /*p04.NUTO*/ dma_reg.DMA_A06.set(!dma_reg.DMA_A05, DMA_RSTn, !dma_reg.DMA_A06);
    /*p04.MUGU*/ dma_reg.DMA_A07.set(!dma_reg.DMA_A06, DMA_RSTn, !dma_reg.DMA_A07);

    /*p04.NAFA*/ dma_reg.DMA_A08.set(FF46_WR, cpu_pins.D0);
    /*p04.PYNE*/ dma_reg.DMA_A09.set(FF46_WR, cpu_pins.D1);
    /*p04.PARA*/ dma_reg.DMA_A10.set(FF46_WR, cpu_pins.D2);
    /*p04.NYDO*/ dma_reg.DMA_A11.set(FF46_WR, cpu_pins.D3);
    /*p04.NYGY*/ dma_reg.DMA_A12.set(FF46_WR, cpu_pins.D4);
    /*p04.PULA*/ dma_reg.DMA_A13.set(FF46_WR, cpu_pins.D5);
    /*p04.POKU*/ dma_reg.DMA_A14.set(FF46_WR, cpu_pins.D6);
    /*p04.MARU*/ dma_reg.DMA_A15.set(FF46_WR, cpu_pins.D7);

    {
      /*p04.MOLU*/ wire FF46_RDn1 = nand(FF46, ASOT_BUS_RD_ABCDEFGH);
      /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
      /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);

      /*p04.POLY*/ cpu_pins.D0.set_tribuf(!FF46_RDn2, dma_reg.DMA_A08);
      /*p04.ROFO*/ cpu_pins.D1.set_tribuf(!FF46_RDn2, dma_reg.DMA_A09);
      /*p04.REMA*/ cpu_pins.D2.set_tribuf(!FF46_RDn2, dma_reg.DMA_A10);
      /*p04.PANE*/ cpu_pins.D3.set_tribuf(!FF46_RDn2, dma_reg.DMA_A11);
      /*p04.PARE*/ cpu_pins.D4.set_tribuf(!FF46_RDn2, dma_reg.DMA_A12);
      /*p04.RALY*/ cpu_pins.D5.set_tribuf(!FF46_RDn2, dma_reg.DMA_A13);
      /*p04.RESU*/ cpu_pins.D6.set_tribuf(!FF46_RDn2, dma_reg.DMA_A14);
      /*p04.NUVY*/ cpu_pins.D7.set_tribuf(!FF46_RDn2, dma_reg.DMA_A15);
    }
  }

  //----------------------------------------
  // Timer

  // div
  {
    /*p01.TAGY*/ wire FF04_RD = and(TEDO_BUS_RD, FF04_FF07, TOLA_A01n, TOVY_A00n);
     
    /*p01.UMEK*/ wire DIV_06n = not(tim_reg.DIV_06);
    /*p01.UREK*/ wire DIV_07n = not(tim_reg.DIV_07);
    /*p01.UTOK*/ wire DIV_08n = not(tim_reg.DIV_08);
    /*p01.SAPY*/ wire DIV_09n = not(tim_reg.DIV_09);
    /*p01.UMER*/ wire DIV_10n = not(tim_reg.DIV_10);
    /*p01.RAVE*/ wire DIV_11n = not(tim_reg.DIV_11);
    /*p01.RYSO*/ wire DIV_12n = not(tim_reg.DIV_12);
    /*p01.UDOR*/ wire DIV_13n = not(tim_reg.DIV_13);

    /*p01.TAWU*/ cpu_pins.D0.set_tribuf(FF04_RD, not(DIV_06n));
    /*p01.TAKU*/ cpu_pins.D1.set_tribuf(FF04_RD, not(DIV_07n));
    /*p01.TEMU*/ cpu_pins.D2.set_tribuf(FF04_RD, not(DIV_08n));
    /*p01.TUSE*/ cpu_pins.D3.set_tribuf(FF04_RD, not(DIV_09n));
    /*p01.UPUG*/ cpu_pins.D4.set_tribuf(FF04_RD, not(DIV_10n));
    /*p01.SEPU*/ cpu_pins.D5.set_tribuf(FF04_RD, not(DIV_11n));
    /*p01.SAWA*/ cpu_pins.D6.set_tribuf(FF04_RD, not(DIV_12n));
    /*p01.TATU*/ cpu_pins.D7.set_tribuf(FF04_RD, not(DIV_13n));

    /*p01.TAPE*/ wire FF04_WR = and(TAPU_BUS_WR_xxxxxFGH, FF04_FF07, TOLA_A01n, TOVY_A00n);
    /*p01.UFOL*/ wire DIV_RSTn = nor(UCOB_CLKBAD, sys_pins.RST, FF04_WR);

    /*p01.UKUP*/ tim_reg.DIV_00.set(BOGA_AxCDEFGH,   DIV_RSTn, !tim_reg.DIV_00);
    /*p01.UFOR*/ tim_reg.DIV_01.set(!tim_reg.DIV_00, DIV_RSTn, !tim_reg.DIV_01);
    /*p01.UNER*/ tim_reg.DIV_02.set(!tim_reg.DIV_01, DIV_RSTn, !tim_reg.DIV_02);
    /*p01.TERO*/ tim_reg.DIV_03.set(!tim_reg.DIV_02, DIV_RSTn, !tim_reg.DIV_03);
    /*p01.UNYK*/ tim_reg.DIV_04.set(!tim_reg.DIV_03, DIV_RSTn, !tim_reg.DIV_04);
    /*p01.TAMA*/ tim_reg.DIV_05.set(!tim_reg.DIV_04, DIV_RSTn, !tim_reg.DIV_05);

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2n(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire DIV_06_CLK = tim_reg.DIV_05;
    /*p01.UGOT*/ tim_reg.DIV_06.set(!DIV_06_CLK,     DIV_RSTn, !tim_reg.DIV_06);
    /*p01.TULU*/ tim_reg.DIV_07.set(!tim_reg.DIV_06, DIV_RSTn, !tim_reg.DIV_07);
    /*p01.TUGO*/ tim_reg.DIV_08.set(!tim_reg.DIV_07, DIV_RSTn, !tim_reg.DIV_08);
    /*p01.TOFE*/ tim_reg.DIV_09.set(!tim_reg.DIV_08, DIV_RSTn, !tim_reg.DIV_09);
    /*p01.TERU*/ tim_reg.DIV_10.set(!tim_reg.DIV_09, DIV_RSTn, !tim_reg.DIV_10);
    /*p01.SOLA*/ tim_reg.DIV_11.set(!tim_reg.DIV_10, DIV_RSTn, !tim_reg.DIV_11);
    /*p01.SUBU*/ tim_reg.DIV_12.set(!tim_reg.DIV_11, DIV_RSTn, !tim_reg.DIV_12);
    /*p01.TEKA*/ tim_reg.DIV_13.set(!tim_reg.DIV_12, DIV_RSTn, !tim_reg.DIV_13);
    /*p01.UKET*/ tim_reg.DIV_14.set(!tim_reg.DIV_13, DIV_RSTn, !tim_reg.DIV_14);
    /*p01.UPOF*/ tim_reg.DIV_15.set(!tim_reg.DIV_14, DIV_RSTn, !tim_reg.DIV_15);
  }

  // sch_timer.tick_tima
  {
    /*p03.TEDA*/ wire FF05_RD = and(TEDO_BUS_RD, FF04_FF07, TOLA_A01n,     cpu_pins.A00);
    /*p03.TOPE*/ wire FF05_WRn = nand(TAPU_BUS_WR_xxxxxFGH, FF04_FF07, TOLA_A01n, cpu_pins.A00);
    /*p03.UBOT*/ wire CLK_256Kn = not(tim_reg.DIV_01);
    /*p03.UVYR*/ wire CLK_64Kn  = not(tim_reg.DIV_03);
    /*p03.MEKE*/ wire INT_TIMERn = not(tim_reg.INT_TIMER);

    /*p03.MUZU*/ wire MUZU       = or(cpu_pins.FROM_CPU5, FF05_WRn);
    /*p03.MEXU*/ wire TIMA_LOAD  = nand(MUZU, ALUR_RSTn, INT_TIMERn);


    /*p01.UVYN*/ wire DIV_05n = not(tim_reg.DIV_05);
    /*p03.UKAP*/ wire CLK_MUXa = mux2n(DIV_05n,  CLK_64Kn, tim_reg.TAC_0);
    /*p01.UREK*/ wire DIV_07n = not(tim_reg.DIV_07);
    /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, DIV_07n,  tim_reg.TAC_0);
    /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa,  CLK_MUXb, tim_reg.TAC_1);

    /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !tim_reg.TAC_2);
    /*p03.MULO*/ wire TIMA_RST = not(ALUR_RSTn);
    /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(tim_reg.TMA_0, cpu_pins.D0, FF05_WRn);
    /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(tim_reg.TMA_1, cpu_pins.D1, FF05_WRn);
    /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(tim_reg.TMA_2, cpu_pins.D2, FF05_WRn);
    /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(tim_reg.TMA_3, cpu_pins.D3, FF05_WRn);
    /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(tim_reg.TMA_4, cpu_pins.D4, FF05_WRn);
    /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(tim_reg.TMA_5, cpu_pins.D5, FF05_WRn);
    /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(tim_reg.TMA_6, cpu_pins.D6, FF05_WRn);
    /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(tim_reg.TMA_7, cpu_pins.D7, FF05_WRn);
    /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
    /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
    /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
    /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
    /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
    /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
    /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
    /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);
    /*p03.REGA*/ tim_reg.TIMA_0.tima_count(TIMA_CLK,           TIMA_LOAD, TIMA_LD_0);
    /*p03.POVY*/ tim_reg.TIMA_1.tima_count(tim_reg.TIMA_0.c(), TIMA_LOAD, TIMA_LD_1);
    /*p03.PERU*/ tim_reg.TIMA_2.tima_count(tim_reg.TIMA_1.c(), TIMA_LOAD, TIMA_LD_2);
    /*p03.RATE*/ tim_reg.TIMA_3.tima_count(tim_reg.TIMA_2.c(), TIMA_LOAD, TIMA_LD_3);
    /*p03.RUBY*/ tim_reg.TIMA_4.tima_count(tim_reg.TIMA_3.c(), TIMA_LOAD, TIMA_LD_4);
    /*p03.RAGE*/ tim_reg.TIMA_5.tima_count(tim_reg.TIMA_4.c(), TIMA_LOAD, TIMA_LD_5);
    /*p03.PEDA*/ tim_reg.TIMA_6.tima_count(tim_reg.TIMA_5.c(), TIMA_LOAD, TIMA_LD_6);
    /*p03.NUGA*/ tim_reg.TIMA_7.tima_count(tim_reg.TIMA_6.c(), TIMA_LOAD, TIMA_LD_7);

    /*p03.MUGY*/ wire TIMA_LOADn = not(TIMA_LOAD);

    /*p03.NYDU*/ tim_reg.TIMA_MAX.set(BOGA_AxCDEFGH, TIMA_LOADn, tim_reg.TIMA_7.c());

    /*p03.MERY*/ wire INT_TIMER_IN = nor(!tim_reg.TIMA_MAX, tim_reg.TIMA_7.c());
    /*p03.MOBA*/ tim_reg.INT_TIMER.set(BOGA_AxCDEFGH, ALUR_RSTn, INT_TIMER_IN);

    /*p03.SOKU*/ cpu_pins.D0.set_tribuf(FF05_RD, tim_reg.TIMA_0.v());
    /*p03.RACY*/ cpu_pins.D1.set_tribuf(FF05_RD, tim_reg.TIMA_1.v());
    /*p03.RAVY*/ cpu_pins.D2.set_tribuf(FF05_RD, tim_reg.TIMA_2.v());
    /*p03.SOSY*/ cpu_pins.D3.set_tribuf(FF05_RD, tim_reg.TIMA_3.v());
    /*p03.SOMU*/ cpu_pins.D4.set_tribuf(FF05_RD, tim_reg.TIMA_4.v());
    /*p03.SURO*/ cpu_pins.D5.set_tribuf(FF05_RD, tim_reg.TIMA_5.v());
    /*p03.ROWU*/ cpu_pins.D6.set_tribuf(FF05_RD, tim_reg.TIMA_6.v());
    /*p03.PUSO*/ cpu_pins.D7.set_tribuf(FF05_RD, tim_reg.TIMA_7.v());
  }

  // sch_timer.tick_tma
  {
    /*p03.TYJU*/ wire FF06_WRn = nand(TAPU_BUS_WR_xxxxxFGH, FF04_FF07, TOVY_A00n, cpu_pins.A01);
    /*p03.TUBY*/ wire FF06_RD = and(TEDO_BUS_RD, FF04_FF07, cpu_pins.A01, TOVY_A00n);

    /*p03.SABU*/ tim_reg.TMA_0.set(FF06_WRn, ALUR_RSTn, cpu_pins.D0);
    /*p03.NYKE*/ tim_reg.TMA_1.set(FF06_WRn, ALUR_RSTn, cpu_pins.D1);
    /*p03.MURU*/ tim_reg.TMA_2.set(FF06_WRn, ALUR_RSTn, cpu_pins.D2);
    /*p03.TYVA*/ tim_reg.TMA_3.set(FF06_WRn, ALUR_RSTn, cpu_pins.D3);
    /*p03.TYRU*/ tim_reg.TMA_4.set(FF06_WRn, ALUR_RSTn, cpu_pins.D4);
    /*p03.SUFY*/ tim_reg.TMA_5.set(FF06_WRn, ALUR_RSTn, cpu_pins.D5);
    /*p03.PETO*/ tim_reg.TMA_6.set(FF06_WRn, ALUR_RSTn, cpu_pins.D6);
    /*p03.SETA*/ tim_reg.TMA_7.set(FF06_WRn, ALUR_RSTn, cpu_pins.D7);

    /*p03.SETE*/ cpu_pins.D0.set_tribuf(FF06_RD, tim_reg.TMA_0);
    /*p03.PYRE*/ cpu_pins.D1.set_tribuf(FF06_RD, tim_reg.TMA_1);
    /*p03.NOLA*/ cpu_pins.D2.set_tribuf(FF06_RD, tim_reg.TMA_2);
    /*p03.SALU*/ cpu_pins.D3.set_tribuf(FF06_RD, tim_reg.TMA_3);
    /*p03.SUPO*/ cpu_pins.D4.set_tribuf(FF06_RD, tim_reg.TMA_4);
    /*p03.SOTU*/ cpu_pins.D5.set_tribuf(FF06_RD, tim_reg.TMA_5);
    /*p03.REVA*/ cpu_pins.D6.set_tribuf(FF06_RD, tim_reg.TMA_6);
    /*p03.SAPU*/ cpu_pins.D7.set_tribuf(FF06_RD, tim_reg.TMA_7);
  }

  // sch_timer.tick_tac
  {
    /*p03.SARA*/ wire FF07_WRn = nand(TAPU_BUS_WR_xxxxxFGH, FF04_FF07, cpu_pins.A00, cpu_pins.A01);
    /*p03.SORA*/ wire FF07_RD = and(TEDO_BUS_RD, FF04_FF07, cpu_pins.A00, cpu_pins.A01);

    /*p03.SOPU*/ tim_reg.TAC_0.set(FF07_WRn, ALUR_RSTn, cpu_pins.D0);
    /*p03.SAMY*/ tim_reg.TAC_1.set(FF07_WRn, ALUR_RSTn, cpu_pins.D1);
    /*p03.SABO*/ tim_reg.TAC_2.set(FF07_WRn, ALUR_RSTn, cpu_pins.D2);

    /*p03.RYLA*/ cpu_pins.D0.set_tribuf(FF07_RD, tim_reg.TAC_0);
    /*p03.ROTE*/ cpu_pins.D1.set_tribuf(FF07_RD, tim_reg.TAC_1);
    /*p03.SUPE*/ cpu_pins.D2.set_tribuf(FF07_RD, tim_reg.TAC_2);
  }

  //----------------------------------------
  // serial
  [this, TEDO_BUS_RD, TAPU_BUS_WR_xxxxxFGH, ALUR_RSTn, TOVY_A00n, TOLA_A01n, ADDR_XX00_XX07, ADDR_FFXX] {
    /*p06.SEFY*/   wire SEFY_A02n = not(cpu_pins.A02);
    /*p06.SANO*/ wire ADDR_FF00_FF03 = and(ADDR_XX00_XX07, SEFY_A02n, ADDR_FFXX);
    /*p06.COBA*/ wire SER_CNT3n = not(ser_reg.SER_CNT3);
    /*p06.UWAM*/ wire FF02_WRn = nand(TAPU_BUS_WR_xxxxxFGH, ADDR_FF00_FF03, cpu_pins.A01, TOVY_A00n);
    /*p06.CABY*/ wire XFER_RESET = and(SER_CNT3n, ALUR_RSTn);
    /*p06.ETAF*/ ser_reg.XFER_START.set(FF02_WRn, XFER_RESET,         cpu_pins.D0);
    /*p06.CULY*/ ser_reg.XFER_DIR  .set(FF02_WRn, ALUR_RSTn, cpu_pins.D1);

    /*p01.UVYN*/ wire DIV_05n = not(tim_reg.DIV_05);
    /*p06.COTY*/ ser_reg.SER_CLK.set(DIV_05n, FF02_WRn, !ser_reg.SER_CLK);

    /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(ser_reg.SER_CLK, ser_pins.SCK_C, ser_reg.XFER_DIR);

    /*p06.CARO*/ wire SER_RST  = and(FF02_WRn, ALUR_RSTn);
    /*p06.DAWA*/ wire SER_CLK1 = or(SER_CLK_MUXn, !ser_reg.XFER_START); // this must stop the clock or something when the transmit's done
    /*p06.CAFA*/ ser_reg.SER_CNT0.set(SER_CLK1,          SER_RST, !ser_reg.SER_CNT0);
    /*p06.CYLO*/ ser_reg.SER_CNT1.set(!ser_reg.SER_CNT0, SER_RST, !ser_reg.SER_CNT1);
    /*p06.CYDE*/ ser_reg.SER_CNT2.set(!ser_reg.SER_CNT1, SER_RST, !ser_reg.SER_CNT2);
    /*p06.CALY*/ ser_reg.SER_CNT3.set(!ser_reg.SER_CNT2, SER_RST, !ser_reg.SER_CNT3);

    /*p06.URYS*/ wire FF01_WRn_xxxxDEFx = nand(TAPU_BUS_WR_xxxxxFGH, ADDR_FF00_FF03, cpu_pins.A00, TOLA_A01n);
    /*p06.DAKU*/ wire FF01_WR_ABCDxxxH  = not (FF01_WRn_xxxxDEFx);

    /*p06.CUFU*/ wire SER_DATA0_SETn = nand(cpu_pins.D0, FF01_WR_ABCDxxxH);
    /*p06.DOCU*/ wire SER_DATA1_SETn = nand(cpu_pins.D1, FF01_WR_ABCDxxxH);
    /*p06.DELA*/ wire SER_DATA2_SETn = nand(cpu_pins.D2, FF01_WR_ABCDxxxH);
    /*p06.DYGE*/ wire SER_DATA3_SETn = nand(cpu_pins.D3, FF01_WR_ABCDxxxH);
    /*p06.DOLA*/ wire SER_DATA4_SETn = nand(cpu_pins.D4, FF01_WR_ABCDxxxH);
    /*p06.ELOK*/ wire SER_DATA5_SETn = nand(cpu_pins.D5, FF01_WR_ABCDxxxH);
    /*p06.EDEL*/ wire SER_DATA6_SETn = nand(cpu_pins.D6, FF01_WR_ABCDxxxH);
    /*p06.EFEF*/ wire SER_DATA7_SETn = nand(cpu_pins.D7, FF01_WR_ABCDxxxH);

    /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D0), ALUR_RSTn);
    /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D1), ALUR_RSTn);
    /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D2), ALUR_RSTn);
    /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D3), ALUR_RSTn);
    /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D4), ALUR_RSTn);
    /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D5), ALUR_RSTn);
    /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D6), ALUR_RSTn);
    /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn_xxxxDEFx, cpu_pins.D7), ALUR_RSTn);

    /*p06.CAGE*/ wire SIN_Cn = not(ser_pins.SIN_C);
    /*p06.EDYL*/ wire SER_CLKn = not(SER_CLK1);
    /*p06.EPYT*/ wire SER_CLK2 = not(SER_CLKn);
    /*p06.DEHO*/ wire DEHO = not(SER_CLK2);
    /*p06.DAWE*/ wire SER_CLK3 = not(DEHO);
    /*p06.CUBA*/ ser_reg.SER_DATA0.set(SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, SIN_Cn);
    /*p06.DEGU*/ ser_reg.SER_DATA1.set(SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, ser_reg.SER_DATA0);
    /*p06.DYRA*/ ser_reg.SER_DATA2.set(SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, ser_reg.SER_DATA1);
    /*p06.DOJO*/ ser_reg.SER_DATA3.set(SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, ser_reg.SER_DATA2);
    /*p06.DOVU*/ ser_reg.SER_DATA4.set(SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, ser_reg.SER_DATA3);
    /*p06.EJAB*/ ser_reg.SER_DATA5.set(SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, ser_reg.SER_DATA4);
    /*p06.EROD*/ ser_reg.SER_DATA6.set(SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, ser_reg.SER_DATA5);
    /*p06.EDER*/ ser_reg.SER_DATA7.set(SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, ser_reg.SER_DATA6);
    /*p06.ELYS*/ ser_reg.SER_OUT.set(SER_CLKn, ALUR_RSTn, ser_reg.SER_DATA7);

    ///*p05.KENA*/ ser_pins.SOUT  = mux2n(DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
    /*p05.KENA*/ ser_pins.SOUT.set(ser_reg.SER_OUT);
    /*p06.KEXU*/ ser_pins.SCK_A.set(nand(ser_reg.SER_CLK, ser_reg.XFER_DIR));
                 ser_pins.SCK_B.set(ser_reg.XFER_DIR);
    /*p06.KUJO*/ ser_pins.SCK_D.set(nor(ser_reg.SER_CLK, /*p06.JAGO*/ not(ser_reg.XFER_DIR)));

    /*p06.UFEG*/ wire FF01_RD = and(TEDO_BUS_RD, ADDR_FF00_FF03, cpu_pins.A00, TOLA_A01n);

    /*p06.CUGY*/ cpu_pins.D0.set_tribuf(FF01_RD, ser_reg.SER_DATA0);
    /*p06.DUDE*/ cpu_pins.D1.set_tribuf(FF01_RD, ser_reg.SER_DATA1);
    /*p06.DETU*/ cpu_pins.D2.set_tribuf(FF01_RD, ser_reg.SER_DATA2);
    /*p06.DASO*/ cpu_pins.D3.set_tribuf(FF01_RD, ser_reg.SER_DATA3);
    /*p06.DAME*/ cpu_pins.D4.set_tribuf(FF01_RD, ser_reg.SER_DATA4);
    /*p06.EVOK*/ cpu_pins.D5.set_tribuf(FF01_RD, ser_reg.SER_DATA5);
    /*p06.EFAB*/ cpu_pins.D6.set_tribuf(FF01_RD, ser_reg.SER_DATA6);
    /*p06.ETAK*/ cpu_pins.D7.set_tribuf(FF01_RD, ser_reg.SER_DATA7);

    /*p06.UCOM*/ wire FF02_RD = and (TEDO_BUS_RD, ADDR_FF00_FF03, cpu_pins.A01, TOVY_A00n);

    /*p06.CORE*/ cpu_pins.D0.set_tribuf(FF02_RD, ser_reg.XFER_DIR);
    /*p06.ELUV*/ cpu_pins.D1.set_tribuf(FF02_RD, ser_reg.XFER_START);
  }();

  //----------------------------------------
  // sch_joypad
  [this, BOGA_AxCDEFGH, ALUR_RSTn, TEDO_BUS_RD, TAPU_BUS_WR_xxxxxFGH, ADDR_111111110xx00000] {

    // FIXME
    wire FF60_0 = 0;
    wire FF60_0n = 1;

    /*p10.AKUG*/ wire A06n = not(cpu_pins.A06);
    /*p10.BYKO*/ wire A05n = not(cpu_pins.A05);

    /*p05.KOLE*/ joy_pins.P10_A.set(nand(joy_reg.JOYP_RA,  FF60_0));
    /*p05.KYBU*/ joy_pins.P10_D.set(nor (joy_reg.JOYP_RA,  FF60_0n));
    /*p05.KYTO*/ joy_pins.P11_A.set(nand(joy_reg.JOYP_LB,  FF60_0));
    /*p05.KABU*/ joy_pins.P11_D.set(nor (joy_reg.JOYP_LB,  FF60_0n));
    /*p05.KYHU*/ joy_pins.P12_A.set(nand(joy_reg.JOYP_UC,  FF60_0));
    /*p05.KASY*/ joy_pins.P12_D.set(nor (joy_reg.JOYP_UC,  FF60_0n)); // schematic wrong
    /*p05.KORY*/ joy_pins.P13_A.set(nand(joy_reg.JOYP_DS,  FF60_0));
    /*p05.KALE*/ joy_pins.P13_D.set(nor (joy_reg.JOYP_DS,  FF60_0n));
    /*p05.KARU*/ joy_pins.P14_A.set(or(!joy_reg.JOYP_UDLR, FF60_0n));
    /*p05.KARU*/ joy_pins.P14_D.set(joy_reg.JOYP_UDLR);
    /*p05.CELA*/ joy_pins.P15_A.set(or(!joy_reg.JOYP_ABCS, FF60_0n));
    /*p05.CELA*/ joy_pins.P15_D.set(!joy_reg.JOYP_ABCS); // double check these

    /*p02.KERY*/ wire ANY_BUTTON = or(joy_pins.P13_C, joy_pins.P12_C, joy_pins.P11_C, joy_pins.P10_C);

    /*p02.AWOB*/ joy_reg.WAKE_CPU.set(ANY_BUTTON, BOGA_AxCDEFGH);

    /*p02.BATU*/ joy_reg.JP_GLITCH0.set(BOGA_AxCDEFGH, ALUR_RSTn, ANY_BUTTON);
    /*p02.ACEF*/ joy_reg.JP_GLITCH1.set(BOGA_AxCDEFGH, ALUR_RSTn, joy_reg.JP_GLITCH0);
    /*p02.AGEM*/ joy_reg.JP_GLITCH2.set(BOGA_AxCDEFGH, ALUR_RSTn, joy_reg.JP_GLITCH1);
    /*p02.APUG*/ joy_reg.JP_GLITCH3.set(BOGA_AxCDEFGH, ALUR_RSTn, joy_reg.JP_GLITCH2);

    /*p10.ATOZ*/ wire FF00_WRn   = nand(TAPU_BUS_WR_xxxxxFGH, ADDR_111111110xx00000, A06n, A05n);
    /*p05.JUTE*/ joy_reg.JOYP_RA    .set(FF00_WRn, ALUR_RSTn, cpu_pins.D0);
    /*p05.KECY*/ joy_reg.JOYP_LB    .set(FF00_WRn, ALUR_RSTn, cpu_pins.D1);
    /*p05.JALE*/ joy_reg.JOYP_UC    .set(FF00_WRn, ALUR_RSTn, cpu_pins.D2);
    /*p05.KYME*/ joy_reg.JOYP_DS    .set(FF00_WRn, ALUR_RSTn, cpu_pins.D3);
    /*p05.KELY*/ joy_reg.JOYP_UDLR  .set(FF00_WRn, ALUR_RSTn, cpu_pins.D4);
    /*p05.COFY*/ joy_reg.JOYP_ABCS  .set(FF00_WRn, ALUR_RSTn, cpu_pins.D5);
    /*p05.KUKO*/ joy_reg.DBG_FF00_D6.set(FF00_WRn, ALUR_RSTn, cpu_pins.D6);
    /*p05.KERU*/ joy_reg.DBG_FF00_D7.set(FF00_WRn, ALUR_RSTn, cpu_pins.D7);

    /*p10.ACAT*/ wire FF00_RD = and (TEDO_BUS_RD, ADDR_111111110xx00000, A06n, A05n);
    /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);
    /*p05.KEVU*/ joy_reg.JOYP_L0.set(joy_pins.P10_C, FF00_RDn);
    /*p05.KAPA*/ joy_reg.JOYP_L1.set(joy_pins.P11_C, FF00_RDn);
    /*p05.KEJA*/ joy_reg.JOYP_L2.set(joy_pins.P12_C, FF00_RDn);
    /*p05.KOLO*/ joy_reg.JOYP_L3.set(joy_pins.P13_C, FF00_RDn);

    /*p05.KEMA*/ cpu_pins.D0.set_tribuf(!FF00_RDn, joy_reg.JOYP_L0);
    /*p05.KURO*/ cpu_pins.D1.set_tribuf(!FF00_RDn, joy_reg.JOYP_L1);
    /*p05.KUVE*/ cpu_pins.D2.set_tribuf(!FF00_RDn, joy_reg.JOYP_L2);
    /*p05.JEKU*/ cpu_pins.D3.set_tribuf(!FF00_RDn, joy_reg.JOYP_L3);
    /*p05.KOCE*/ cpu_pins.D4.set_tribuf(!FF00_RDn, joy_reg.JOYP_UDLR);
    /*p05.CUDY*/ cpu_pins.D5.set_tribuf(!FF00_RDn, joy_reg.JOYP_ABCS);
                 cpu_pins.D6.set_tribuf(!FF00_RDn, joy_reg.DBG_FF00_D6);
                 cpu_pins.D7.set_tribuf(!FF00_RDn, joy_reg.DBG_FF00_D7);
  }();

  cpu_pins.TO_CPU2.set(joy_reg.WAKE_CPU);

  //----------------------------------------
  // video config

  // FF402 LCDC
  {
    /*p22.VOCA*/ wire FF40 = not(FF40n);
    /*p23.VYRE*/ wire FF40_RD = and(FF40, ASOT_BUS_RD_ABCDEFGH);
    /*p23.WYCE*/ wire FF40_RDn = not(FF40_RD);
    /*p23.WARU*/ wire FF40_WR = and(FF40, CUPA_BUS_WR_xxxxxFGH);
    /*p23.XUBO*/ wire FF40_WRn = not(FF40_WR);

    /*p23.WYPO*/ cpu_pins.D0.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGEN);
    /*p23.XERO*/ cpu_pins.D1.set_tribuf(!FF40_RDn, cfg_reg.LCDC_SPEN);
    /*p23.WYJU*/ cpu_pins.D2.set_tribuf(!FF40_RDn, cfg_reg.LCDC_SPSIZE);
    /*p23.WUKA*/ cpu_pins.D3.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGMAP);
    /*p23.VOKE*/ cpu_pins.D4.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGTILE);
    /*p23.VATO*/ cpu_pins.D5.set_tribuf(!FF40_RDn, cfg_reg.LCDC_WINEN);
    /*p23.VAHA*/ cpu_pins.D6.set_tribuf(!FF40_RDn, cfg_reg.LCDC_WINMAP);
    /*p23.XEBU*/ cpu_pins.D7.set_tribuf(!FF40_RDn, cfg_reg.LCDC_EN);

    /*p01.XARE*/ wire XARE_RESET = not(XORE_RST);
    /*p23.VYXE*/ cfg_reg.LCDC_BGEN    .set(FF40_WRn, XARE_RESET, cpu_pins.D0);
    /*p23.XYLO*/ cfg_reg.LCDC_SPEN    .set(FF40_WRn, XARE_RESET, cpu_pins.D1);
    /*p23.XYMO*/ cfg_reg.LCDC_SPSIZE  .set(FF40_WRn, XARE_RESET, cpu_pins.D2);
    /*p23.XAFO*/ cfg_reg.LCDC_BGMAP   .set(FF40_WRn, XARE_RESET, cpu_pins.D3);
    /*p23.WEXU*/ cfg_reg.LCDC_BGTILE  .set(FF40_WRn, XARE_RESET, cpu_pins.D4);
    /*p23.WYMO*/ cfg_reg.LCDC_WINEN   .set(FF40_WRn, XARE_RESET, cpu_pins.D5);
    /*p23.WOKY*/ cfg_reg.LCDC_WINMAP  .set(FF40_WRn, XARE_RESET, cpu_pins.D6);
    /*p23.XONA*/ cfg_reg.LCDC_EN      .set(FF40_WRn, XARE_RESET, cpu_pins.D7);
  }

  // FF42 SCY
  {
    /*p22.WEBU*/ wire FF42n             = nand(FF4X, A00n, WESA_A01, A02n, A03n);
    /*p22.XARO*/ wire FF42              = not(FF42n);
    /*p23.ANYP*/ wire FF42_RD           = and(FF42, ASOT_BUS_RD_ABCDEFGH);
    /*p23.BUWY*/ wire FF42_RDn          = not(FF42_RD);
    /*p23.BEDY*/ wire FF42_WR_ABCDxxxH  = and(FF42, CUPA_BUS_WR_xxxxxFGH);
    /*p23.CAVO*/ wire FF42_WRn_xxxxEFGx = not(FF42_WR_ABCDxxxH);

    /*p23.WARE*/ cpu_pins.D0.set_tribuf(!FF42_RDn, cfg_reg.SCY0);
    /*p23.GOBA*/ cpu_pins.D1.set_tribuf(!FF42_RDn, cfg_reg.SCY1);
    /*p23.GONU*/ cpu_pins.D2.set_tribuf(!FF42_RDn, cfg_reg.SCY2);
    /*p23.GODO*/ cpu_pins.D3.set_tribuf(!FF42_RDn, cfg_reg.SCY3);
    /*p23.CUSA*/ cpu_pins.D4.set_tribuf(!FF42_RDn, cfg_reg.SCY4);
    /*p23.GYZO*/ cpu_pins.D5.set_tribuf(!FF42_RDn, cfg_reg.SCY5);
    /*p23.GUNE*/ cpu_pins.D6.set_tribuf(!FF42_RDn, cfg_reg.SCY6);
    /*p23.GYZA*/ cpu_pins.D7.set_tribuf(!FF42_RDn, cfg_reg.SCY7);

    /*p23.GAVE*/ cfg_reg.SCY0.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D0);
    /*p23.FYMO*/ cfg_reg.SCY1.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D1);
    /*p23.FEZU*/ cfg_reg.SCY2.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D2);
    /*p23.FUJO*/ cfg_reg.SCY3.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D3);
    /*p23.DEDE*/ cfg_reg.SCY4.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D4);
    /*p23.FOTY*/ cfg_reg.SCY5.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D5);
    /*p23.FOHA*/ cfg_reg.SCY6.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D6);
    /*p23.FUNY*/ cfg_reg.SCY7.set(FF42_WRn_xxxxEFGx, CUNU_RSTn, cpu_pins.D7);
  }

  // FF43 SCX
  {
    /*p22.XAVY*/ wire FF43 = not(FF43n);
    /*p23.AVOG*/ wire FF43_RD = and(FF43, ASOT_BUS_RD_ABCDEFGH);
    /*p23.BEBA*/ wire FF43_RDn = not(FF43_RD);
    /*p23.ARUR*/ wire FF43_WR = and(FF43, CUPA_BUS_WR_xxxxxFGH);
    /*p23.AMUN*/ wire FF43_WRn = not(FF43_WR);

    /*p23.EDOS*/ cpu_pins.D0.set_tribuf(!FF43_RDn, cfg_reg.SCX0);
    /*p23.EKOB*/ cpu_pins.D1.set_tribuf(!FF43_RDn, cfg_reg.SCX1);
    /*p23.CUGA*/ cpu_pins.D2.set_tribuf(!FF43_RDn, cfg_reg.SCX2);
    /*p23.WONY*/ cpu_pins.D3.set_tribuf(!FF43_RDn, cfg_reg.SCX3);
    /*p23.CEDU*/ cpu_pins.D4.set_tribuf(!FF43_RDn, cfg_reg.SCX4);
    /*p23.CATA*/ cpu_pins.D5.set_tribuf(!FF43_RDn, cfg_reg.SCX5);
    /*p23.DOXE*/ cpu_pins.D6.set_tribuf(!FF43_RDn, cfg_reg.SCX6);
    /*p23.CASY*/ cpu_pins.D7.set_tribuf(!FF43_RDn, cfg_reg.SCX7);

    /*p23.DATY*/ cfg_reg.SCX0.set(FF43_WRn, CUNU_RSTn, cpu_pins.D0);
    /*p23.DUZU*/ cfg_reg.SCX1.set(FF43_WRn, CUNU_RSTn, cpu_pins.D1);
    /*p23.CYXU*/ cfg_reg.SCX2.set(FF43_WRn, CUNU_RSTn, cpu_pins.D2);
    /*p23.GUBO*/ cfg_reg.SCX3.set(FF43_WRn, CUNU_RSTn, cpu_pins.D3);
    /*p23.BEMY*/ cfg_reg.SCX4.set(FF43_WRn, CUNU_RSTn, cpu_pins.D4);
    /*p23.CUZY*/ cfg_reg.SCX5.set(FF43_WRn, CUNU_RSTn, cpu_pins.D5);
    /*p23.CABU*/ cfg_reg.SCX6.set(FF43_WRn, CUNU_RSTn, cpu_pins.D6);
    /*p23.BAKE*/ cfg_reg.SCX7.set(FF43_WRn, CUNU_RSTn, cpu_pins.D7);
  }

  // FF44 LY
  {
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p23.WAFU*/ wire FF44_RD = and(ASOT_BUS_RD_ABCDEFGH, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd_reg.Y0);
    /*p23.XEPO*/ wire LY1n = not(lcd_reg.Y1);
    /*p23.MYFA*/ wire LY2n = not(lcd_reg.Y2);
    /*p23.XUHY*/ wire LY3n = not(lcd_reg.Y3);
    /*p23.WATA*/ wire LY4n = not(lcd_reg.Y4);
    /*p23.XAGA*/ wire LY5n = not(lcd_reg.Y5);
    /*p23.XUCE*/ wire LY6n = not(lcd_reg.Y6);
    /*p23.XOWO*/ wire LY7n = not(lcd_reg.Y7);

    /*p23.VEGA*/ cpu_pins.D0.set_tribuf(!FF44_RDn, not(LY0n));
    /*p23.WUVA*/ cpu_pins.D1.set_tribuf(!FF44_RDn, not(LY1n));
    /*p23.LYCO*/ cpu_pins.D2.set_tribuf(!FF44_RDn, not(LY2n));
    /*p23.WOJY*/ cpu_pins.D3.set_tribuf(!FF44_RDn, not(LY3n));
    /*p23.VYNE*/ cpu_pins.D4.set_tribuf(!FF44_RDn, not(LY4n));
    /*p23.WAMA*/ cpu_pins.D5.set_tribuf(!FF44_RDn, not(LY5n));
    /*p23.WAVO*/ cpu_pins.D6.set_tribuf(!FF44_RDn, not(LY6n));
    /*p23.WEZE*/ cpu_pins.D7.set_tribuf(!FF44_RDn, not(LY7n));
  }

  // FF45
  {
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p23.XYLY*/ wire FF45_RD = and(ASOT_BUS_RD_ABCDEFGH, FF45);
    /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);
    /*p23.XUFA*/ wire FF45_WR = and(CUPA_BUS_WR_xxxxxFGH, FF45);
    /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

    /*p23.RETU*/ cpu_pins.D0.set_tribuf(!FF45_RDn, cfg_reg.LYC0);
    /*p23.VOJO*/ cpu_pins.D1.set_tribuf(!FF45_RDn, cfg_reg.LYC1);
    /*p23.RAZU*/ cpu_pins.D2.set_tribuf(!FF45_RDn, cfg_reg.LYC2);
    /*p23.REDY*/ cpu_pins.D3.set_tribuf(!FF45_RDn, cfg_reg.LYC3);
    /*p23.RACE*/ cpu_pins.D4.set_tribuf(!FF45_RDn, cfg_reg.LYC4);
    /*p23.VAZU*/ cpu_pins.D5.set_tribuf(!FF45_RDn, cfg_reg.LYC5);
    /*p23.VAFE*/ cpu_pins.D6.set_tribuf(!FF45_RDn, cfg_reg.LYC6);
    /*p23.PUFY*/ cpu_pins.D7.set_tribuf(!FF45_RDn, cfg_reg.LYC7);

    /*p23.SYRY*/ cfg_reg.LYC0.set(FF45_WRn, WESY_RST, cpu_pins.D0);
    /*p23.VUCE*/ cfg_reg.LYC1.set(FF45_WRn, WESY_RST, cpu_pins.D1);
    /*p23.SEDY*/ cfg_reg.LYC2.set(FF45_WRn, WESY_RST, cpu_pins.D2);
    /*p23.SALO*/ cfg_reg.LYC3.set(FF45_WRn, WESY_RST, cpu_pins.D3);
    /*p23.SOTA*/ cfg_reg.LYC4.set(FF45_WRn, WESY_RST, cpu_pins.D4);
    /*p23.VAFA*/ cfg_reg.LYC5.set(FF45_WRn, WESY_RST, cpu_pins.D5);
    /*p23.VEVO*/ cfg_reg.LYC6.set(FF45_WRn, WESY_RST, cpu_pins.D6);
    /*p23.RAHA*/ cfg_reg.LYC7.set(FF45_WRn, WESY_RST, cpu_pins.D7);
  }

  // FF47
  {
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VUSO*/ wire FF47_RD = and(ASOT_BUS_RD_ABCDEFGH, FF47);
    /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);
    /*p36.VELY*/ wire FF47_WR = and(CUPA_BUS_WR_xxxxxFGH, FF47);
    /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

    /*p36.RARO*/ cpu_pins.D0.set_tribuf(!FF47_RDn, cfg_reg.BGP0);
    /*p36.PABA*/ cpu_pins.D1.set_tribuf(!FF47_RDn, cfg_reg.BGP1);
    /*p36.REDO*/ cpu_pins.D2.set_tribuf(!FF47_RDn, cfg_reg.BGP2);
    /*p36.LOBE*/ cpu_pins.D3.set_tribuf(!FF47_RDn, cfg_reg.BGP3);
    /*p36.LACE*/ cpu_pins.D4.set_tribuf(!FF47_RDn, cfg_reg.BGP4);
    /*p36.LYKA*/ cpu_pins.D5.set_tribuf(!FF47_RDn, cfg_reg.BGP5);
    /*p36.LODY*/ cpu_pins.D6.set_tribuf(!FF47_RDn, cfg_reg.BGP6);
    /*p36.LARY*/ cpu_pins.D7.set_tribuf(!FF47_RDn, cfg_reg.BGP7);

    /*p36.PAVO*/ cfg_reg.BGP0.set(FF47_WRn, 1, cpu_pins.D0);
    /*p36.NUSY*/ cfg_reg.BGP1.set(FF47_WRn, 1, cpu_pins.D1);
    /*p36.PYLU*/ cfg_reg.BGP2.set(FF47_WRn, 1, cpu_pins.D2);
    /*p36.MAXY*/ cfg_reg.BGP3.set(FF47_WRn, 1, cpu_pins.D3);
    /*p36.MUKE*/ cfg_reg.BGP4.set(FF47_WRn, 1, cpu_pins.D4);
    /*p36.MORU*/ cfg_reg.BGP5.set(FF47_WRn, 1, cpu_pins.D5);
    /*p36.MOGY*/ cfg_reg.BGP6.set(FF47_WRn, 1, cpu_pins.D6);
    /*p36.MENA*/ cfg_reg.BGP7.set(FF47_WRn, 1, cpu_pins.D7);
  }

  // FF48
  {
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XUFY*/ wire FF48_RD  = and(ASOT_BUS_RD_ABCDEFGH, FF48);
    /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);
    /*p36.XOMA*/ wire FF48_WR  = and(CUPA_BUS_WR_xxxxxFGH, FF48);
    /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

    /*p36.XARY*/ cpu_pins.D0.set_tribuf(!FF48_RDn, cfg_reg.OBP00);
    /*p36.XOKE*/ cpu_pins.D1.set_tribuf(!FF48_RDn, cfg_reg.OBP01);
    /*p36.XUNO*/ cpu_pins.D2.set_tribuf(!FF48_RDn, cfg_reg.OBP02);
    /*p36.XUBY*/ cpu_pins.D3.set_tribuf(!FF48_RDn, cfg_reg.OBP03);
    /*p36.XAJU*/ cpu_pins.D4.set_tribuf(!FF48_RDn, cfg_reg.OBP04);
    /*p36.XOBO*/ cpu_pins.D5.set_tribuf(!FF48_RDn, cfg_reg.OBP05);
    /*p36.XAXA*/ cpu_pins.D6.set_tribuf(!FF48_RDn, cfg_reg.OBP06);
    /*p36.XAWO*/ cpu_pins.D7.set_tribuf(!FF48_RDn, cfg_reg.OBP07);

    /*p36.XUFU*/ cfg_reg.OBP00.set(FF48_WRn, 1, cpu_pins.D0);
    /*p36.XUKY*/ cfg_reg.OBP01.set(FF48_WRn, 1, cpu_pins.D1);
    /*p36.XOVA*/ cfg_reg.OBP02.set(FF48_WRn, 1, cpu_pins.D2);
    /*p36.XALO*/ cfg_reg.OBP03.set(FF48_WRn, 1, cpu_pins.D3);
    /*p36.XERU*/ cfg_reg.OBP04.set(FF48_WRn, 1, cpu_pins.D4);
    /*p36.XYZE*/ cfg_reg.OBP05.set(FF48_WRn, 1, cpu_pins.D5);
    /*p36.XUPO*/ cfg_reg.OBP06.set(FF48_WRn, 1, cpu_pins.D6);
    /*p36.XANA*/ cfg_reg.OBP07.set(FF48_WRn, 1, cpu_pins.D7);
  }

  // FF49
  {
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MUMY*/ wire FF49_RD  = and(ASOT_BUS_RD_ABCDEFGH, FF49);
    /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD); // where does this go?
    /*p36.MYXE*/ wire FF49_WR  = and(CUPA_BUS_WR_xxxxxFGH, FF49);
    /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

    /*p36.LAJU*/ cpu_pins.D0.set_tribuf(!FF49_RDn, cfg_reg.OBP10);
    /*p36.LEPA*/ cpu_pins.D1.set_tribuf(!FF49_RDn, cfg_reg.OBP11);
    /*p36.LODE*/ cpu_pins.D2.set_tribuf(!FF49_RDn, cfg_reg.OBP12);
    /*p36.LYZA*/ cpu_pins.D3.set_tribuf(!FF49_RDn, cfg_reg.OBP13);
    /*p36.LUKY*/ cpu_pins.D4.set_tribuf(!FF49_RDn, cfg_reg.OBP14);
    /*p36.LUGA*/ cpu_pins.D5.set_tribuf(!FF49_RDn, cfg_reg.OBP15);
    /*p36.LEBA*/ cpu_pins.D6.set_tribuf(!FF49_RDn, cfg_reg.OBP16);
    /*p36.LELU*/ cpu_pins.D7.set_tribuf(!FF49_RDn, cfg_reg.OBP17);

    /*p36.MOXY*/ cfg_reg.OBP10.set(FF49_WRn, 1, cpu_pins.D0);
    /*p36.LAWO*/ cfg_reg.OBP11.set(FF49_WRn, 1, cpu_pins.D1);
    /*p36.MOSA*/ cfg_reg.OBP12.set(FF49_WRn, 1, cpu_pins.D2);
    /*p36.LOSE*/ cfg_reg.OBP13.set(FF49_WRn, 1, cpu_pins.D3);
    /*p36.LUNE*/ cfg_reg.OBP14.set(FF49_WRn, 1, cpu_pins.D4);
    /*p36.LUGU*/ cfg_reg.OBP15.set(FF49_WRn, 1, cpu_pins.D5);
    /*p36.LEPU*/ cfg_reg.OBP16.set(FF49_WRn, 1, cpu_pins.D6);
    /*p36.LUXO*/ cfg_reg.OBP17.set(FF49_WRn, 1, cpu_pins.D7);
  }

  // FF4A/FF4B
  {
    /*p22.VYGA*/ wire FF4A = not(FF4An);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);
    /*p23.WAXU*/ wire FF4A_RD = and(ASOT_BUS_RD_ABCDEFGH, FF4A);
    /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);
    /*p23.WYZE*/ wire FF4B_RD = and(ASOT_BUS_RD_ABCDEFGH, FF4B);
    /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

    /*p23.PUNU*/ cpu_pins.D0.set_tribuf(!FF4A_RDn, cfg_reg.WY0);
    /*p23.PODA*/ cpu_pins.D1.set_tribuf(!FF4A_RDn, cfg_reg.WY1);
    /*p23.PYGU*/ cpu_pins.D2.set_tribuf(!FF4A_RDn, cfg_reg.WY2);
    /*p23.LOKA*/ cpu_pins.D3.set_tribuf(!FF4A_RDn, cfg_reg.WY3);
    /*p23.MEGA*/ cpu_pins.D4.set_tribuf(!FF4A_RDn, cfg_reg.WY4);
    /*p23.PELA*/ cpu_pins.D5.set_tribuf(!FF4A_RDn, cfg_reg.WY5);
    /*p23.POLO*/ cpu_pins.D6.set_tribuf(!FF4A_RDn, cfg_reg.WY6);
    /*p23.MERA*/ cpu_pins.D7.set_tribuf(!FF4A_RDn, cfg_reg.WY7);

    /*p23.LOVA*/ cpu_pins.D0.set_tribuf(!FF4B_RDn, cfg_reg.WX0);
    /*p23.MUKA*/ cpu_pins.D1.set_tribuf(!FF4B_RDn, cfg_reg.WX1);
    /*p23.MOKO*/ cpu_pins.D2.set_tribuf(!FF4B_RDn, cfg_reg.WX2);
    /*p23.LOLE*/ cpu_pins.D3.set_tribuf(!FF4B_RDn, cfg_reg.WX3);
    /*p23.MELE*/ cpu_pins.D4.set_tribuf(!FF4B_RDn, cfg_reg.WX4);
    /*p23.MUFE*/ cpu_pins.D5.set_tribuf(!FF4B_RDn, cfg_reg.WX5);
    /*p23.MULY*/ cpu_pins.D6.set_tribuf(!FF4B_RDn, cfg_reg.WX6);
    /*p23.MARA*/ cpu_pins.D7.set_tribuf(!FF4B_RDn, cfg_reg.WX7);

    /*p23.WEKO*/ wire FF4A_WR = and(CUPA_BUS_WR_xxxxxFGH, FF4A);
    /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);
    /*p23.WUZA*/ wire FF4B_WR = and(CUPA_BUS_WR_xxxxxFGH, FF4B);
    /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

    /*p01.WALU*/ wire WALU_RESET = not(XORE_RST);
    /*p23.NESO*/ cfg_reg.WY0.set(FF4A_WRn, WALU_RESET, cpu_pins.D0);
    /*p23.NYRO*/ cfg_reg.WY1.set(FF4A_WRn, WALU_RESET, cpu_pins.D1);
    /*p23.NAGA*/ cfg_reg.WY2.set(FF4A_WRn, WALU_RESET, cpu_pins.D2);
    /*p23.MELA*/ cfg_reg.WY3.set(FF4A_WRn, WALU_RESET, cpu_pins.D3);
    /*p23.NULO*/ cfg_reg.WY4.set(FF4A_WRn, WALU_RESET, cpu_pins.D4);
    /*p23.NENE*/ cfg_reg.WY5.set(FF4A_WRn, WALU_RESET, cpu_pins.D5);
    /*p23.NUKA*/ cfg_reg.WY6.set(FF4A_WRn, WALU_RESET, cpu_pins.D6);
    /*p23.NAFU*/ cfg_reg.WY7.set(FF4A_WRn, WALU_RESET, cpu_pins.D7);
    /*p23.MYPA*/ cfg_reg.WX0.set(FF4B_WRn, WALU_RESET, cpu_pins.D0);
    /*p23.NOFE*/ cfg_reg.WX1.set(FF4B_WRn, WALU_RESET, cpu_pins.D1);
    /*p23.NOKE*/ cfg_reg.WX2.set(FF4B_WRn, WALU_RESET, cpu_pins.D2);
    /*p23.MEBY*/ cfg_reg.WX3.set(FF4B_WRn, WALU_RESET, cpu_pins.D3);
    /*p23.MYPU*/ cfg_reg.WX4.set(FF4B_WRn, WALU_RESET, cpu_pins.D4);
    /*p23.MYCE*/ cfg_reg.WX5.set(FF4B_WRn, WALU_RESET, cpu_pins.D5); 
    /*p23.MUVO*/ cfg_reg.WX6.set(FF4B_WRn, WALU_RESET, cpu_pins.D6);
    /*p23.NUKU*/ cfg_reg.WX7.set(FF4B_WRn, WALU_RESET, cpu_pins.D7);
  }

  /*p21.XUGU*/   wire XUGU_X_167n = nand(vid_reg.X0, vid_reg.X1, vid_reg.X2, vid_reg.X5, vid_reg.X7); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/   wire XANO_X_167 = not(XUGU_X_167n);

  //----------------------------------------
  // sprite store

  /*p28.ASEN*/ wire ASEN                = or(ATAR_VID_RST, AVAP_SCAN_DONE_d0_TRIG);
  /*p28.BESU*/ wire BESU                = or(lcd_reg.VID_LINE_d4, ASEN);
  /*p28.BOGE*/ wire DMA_RUNNINGn        = not(dma_reg.REG_DMA_RUNNING);
  /*p28.ACYL*/ wire ACYL_OAM_ADDR_PARSE = and(DMA_RUNNINGn, BESU);

  /*p29.GESE*/ Signal SPR_MATCH_Y;
  /*p29.WUTY*/ Signal WUTY_SPRITE_DONE;
  /*p29.FEPO*/ Signal FEPO_STORE_MATCH;
  /*
  [this, &SPR_MATCH_Y, &WUTY_SPRITE_DONE, &FEPO_STORE_MATCH,
    ZEME_AxCxExGx,
    XUPY_xBCxxFGx,
    XOCE_ABxxEFxx,
    AMYG_VID_RST,
    XAPO_VID_RSTn,
    ABEZ_VID_RSTn,
    ATEJ_VID_LINE_TRIG_d4,
    BYHA_VID_LINE_TRIG_d4n,
    WEFE_P10_Bn,
    BESU]*/
  {
    /*p28.ABAK*/   wire ABAK_VID_LINE_TRIG_d4  = or (ATEJ_VID_LINE_TRIG_d4, AMYG_VID_RST);
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n = not(ABAK_VID_LINE_TRIG_d4);
    /*p29.DYBA*/ wire DYBA_VID_LINE_TRIG_d4  = not(BYVA_VID_LINE_TRIG_d4n);

    /*p29.DEGE*/ Signal SPRITE_DELTA0;
    /*p29.DABY*/ Signal SPRITE_DELTA1;
    /*p29.DABU*/ Signal SPRITE_DELTA2;
    /*p29.GYSA*/ Signal SPRITE_DELTA3;
    /*p29.EBOS*/ wire V0n = not(lcd_reg.Y0);
    /*p29.DASA*/ wire V1n = not(lcd_reg.Y1);
    /*p29.FUKY*/ wire V2n = not(lcd_reg.Y2);
    /*p29.FUVE*/ wire V3n = not(lcd_reg.Y3);
    /*p29.FEPU*/ wire V4n = not(lcd_reg.Y4);
    /*p29.FOFA*/ wire V5n = not(lcd_reg.Y5);
    /*p29.FEMO*/ wire V6n = not(lcd_reg.Y6);
    /*p29.GUSU*/ wire V7n = not(lcd_reg.Y7);

    /*p29.ERUC*/ wire YDIFF0   = add_c(V0n, !oam_reg.REG_OAM_B0, joy_pins.P10_B);
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(V0n, !oam_reg.REG_OAM_B0, joy_pins.P10_B);
    /*p29.ENEF*/ wire YDIFF1   = add_s(V1n, !oam_reg.REG_OAM_B1, YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(V1n, !oam_reg.REG_OAM_B1, YDIFF_C0);
    /*p29.FECO*/ wire YDIFF2   = add_s(V2n, !oam_reg.REG_OAM_B2, YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(V2n, !oam_reg.REG_OAM_B2, YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF3   = add_s(V3n, !oam_reg.REG_OAM_B3, YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(V3n, !oam_reg.REG_OAM_B3, YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF4   = add_s(V4n, !oam_reg.REG_OAM_B4, YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(V4n, !oam_reg.REG_OAM_B4, YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF5   = add_s(V5n, !oam_reg.REG_OAM_B5, YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(V5n, !oam_reg.REG_OAM_B5, YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF6   = add_s(V6n, !oam_reg.REG_OAM_B6, YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(V6n, !oam_reg.REG_OAM_B6, YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF7   = add_s(V7n, !oam_reg.REG_OAM_B7, YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(V7n, !oam_reg.REG_OAM_B7, YDIFF_C6);

    /*p29.DEGE*/ SPRITE_DELTA0 = not(YDIFF0);
    /*p29.DABY*/ SPRITE_DELTA1 = not(YDIFF1);
    /*p29.DABU*/ SPRITE_DELTA2 = not(YDIFF2);
    /*p29.GYSA*/ SPRITE_DELTA3 = not(YDIFF3);
    /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF4);
    /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF5);
    /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF6);
    /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF7);

    /*p29.GOVU*/ wire GOVU = or(YDIFF3, cfg_reg.LCDC_SPSIZE);
    /*p29.WOTA*/ wire SPR_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU);
    /*p29.GESE*/ SPR_MATCH_Y = not(SPR_MATCH_Yn);

    /*p30.CYKE*/ wire CYKE = not(XUPY_xBCxxFGx);
    /*p30.WUDA*/ wire WUDA = not(CYKE);
    /*p30.XADU*/ spr_reg.SPRITE_IDX0.set(WUDA, WEFE_P10_Bn, oam_pins.A2);
    /*p30.XEDY*/ spr_reg.SPRITE_IDX1.set(WUDA, WEFE_P10_Bn, oam_pins.A3);
    /*p30.ZUZE*/ spr_reg.SPRITE_IDX2.set(WUDA, WEFE_P10_Bn, oam_pins.A4);
    /*p30.XOBE*/ spr_reg.SPRITE_IDX3.set(WUDA, WEFE_P10_Bn, oam_pins.A5);
    /*p30.YDUF*/ spr_reg.SPRITE_IDX4.set(WUDA, WEFE_P10_Bn, oam_pins.A6);
    /*p30.XECU*/ spr_reg.SPRITE_IDX5.set(WUDA, WEFE_P10_Bn, oam_pins.A7);

    /*p29.CENO*/ spr_reg.STORE_SPRITE_IDXn.set(XUPY_xBCxxFGx, ABEZ_VID_RSTn, BESU);
    /*p29.BUZA*/ wire BUZA_STORE_SPRITE_IDX = and(!spr_reg.STORE_SPRITE_IDXn, vid_reg.XYMU_RENDERING_LATCH);

    /*p30.WEZA*/ spr_reg.TS_IDX_0.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX5); // order wrong
    /*p30.WUCO*/ spr_reg.TS_IDX_1.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX4);
    /*p30.WYDA*/ spr_reg.TS_IDX_2.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX3);
    /*p30.ZYSU*/ spr_reg.TS_IDX_3.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX2);
    /*p30.WYSE*/ spr_reg.TS_IDX_4.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX1);
    /*p30.WUZY*/ spr_reg.TS_IDX_5.set_tribuf(BUZA_STORE_SPRITE_IDX, spr_reg.SPRITE_IDX0);

    /*p29.TYNO*/   wire TYNO = nand(spr_reg.TOXE_SPR_SEQ0, spr_reg.SEBA_SEQ_xxxx45n, spr_reg.VONU_SEQ_xxx34xn);
    /*p29.VUSA*/   wire VUSA = or(!spr_reg.TYFO_SEQ_B0d, TYNO);
    /*p29.WUTY*/ WUTY_SPRITE_DONE = not(VUSA);


    /*p21.ACAM*/ wire X0n = not(vid_reg.X0);
    /*p21.AZUB*/ wire X1n = not(vid_reg.X1);
    /*p21.AMEL*/ wire X2n = not(vid_reg.X2);
    /*p21.AHAL*/ wire X3n = not(vid_reg.X3);
    /*p21.APUX*/ wire X4n = not(vid_reg.X4);
    /*p21.ABEF*/ wire X5n = not(vid_reg.X5);
    /*p21.ADAZ*/ wire X6n = not(vid_reg.X6);
    /*p21.ASAH*/ wire X7n = not(vid_reg.X7);

    /*p31.ZAGO*/ wire SPRITE_X0 = not(!oam_reg.REG_OAM_A0);
    /*p31.ZOCY*/ wire SPRITE_X1 = not(!oam_reg.REG_OAM_A1);
    /*p31.YPUR*/ wire SPRITE_X2 = not(!oam_reg.REG_OAM_A2);
    /*p31.YVOK*/ wire SPRITE_X3 = not(!oam_reg.REG_OAM_A3);
    /*p31.COSE*/ wire SPRITE_X4 = not(!oam_reg.REG_OAM_A4);
    /*p31.AROP*/ wire SPRITE_X5 = not(!oam_reg.REG_OAM_A5);
    /*p31.XATU*/ wire SPRITE_X6 = not(!oam_reg.REG_OAM_A6);
    /*p31.BADY*/ wire SPRITE_X7 = not(!oam_reg.REG_OAM_A7);

    /*p29.EDEN*/ wire SPRITE_COUNT0n = not(sst_reg.SPRITE_COUNT0);
    /*p29.CYPY*/ wire SPRITE_COUNT1n = not(sst_reg.SPRITE_COUNT1);
    /*p29.CAPE*/ wire SPRITE_COUNT2n = not(sst_reg.SPRITE_COUNT2);
    /*p29.CAXU*/ wire SPRITE_COUNT3n = not(sst_reg.SPRITE_COUNT3);

    /*p29.FYCU*/ wire SPRITE_COUNT0b = not(SPRITE_COUNT0n);
    /*p29.FONE*/ wire SPRITE_COUNT1b = not(SPRITE_COUNT1n);
    /*p29.EKUD*/ wire SPRITE_COUNT2b = not(SPRITE_COUNT2n);
    /*p29.ELYG*/ wire SPRITE_COUNT3b = not(SPRITE_COUNT3n);

    /*p29.DEZO*/ wire STORE0_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3n);
    /*p29.CUVA*/ wire STORE1_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3n);
    /*p29.GEBU*/ wire STORE2_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1b, SPRITE_COUNT2n, SPRITE_COUNT3n);
    /*p29.FOCO*/ wire STORE3_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1b, SPRITE_COUNT2n, SPRITE_COUNT3n);
    /*p29.CUPE*/ wire STORE4_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2b, SPRITE_COUNT3n);
    /*p29.CUGU*/ wire STORE5_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2b, SPRITE_COUNT3n);
    /*p29.WOMU*/ wire STORE6_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1b, SPRITE_COUNT2b, SPRITE_COUNT3n);
    /*p29.GUNA*/ wire STORE7_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1b, SPRITE_COUNT2b, SPRITE_COUNT3n);
    /*p29.DEWY*/ wire STORE8_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3b);
    /*p29.DOGU*/ wire STORE9_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3b);

    /*p29.CEHA*/   wire CEHA = not(!spr_reg.STORE_SPRITE_IDXn);
    /*p29.BYJO*/   wire BYJO = not(CEHA);
    /*p29.AZEM*/   wire AZEM = and(BYJO, vid_reg.XYMU_RENDERING_LATCH);
    /*p29.CARE*/   wire STORE_ENn = or(XOCE_ABxxEFxx, CEHA, SPR_MATCH_Y);
    /*p29.DYTY*/ wire STORE_EN = not(STORE_ENn);
    /*p29.AROR*/ wire MATCH_EN = and(AZEM, cfg_reg.LCDC_SPEN);

    //----------------------------------------

    /*p29.YDUG*/ Signal STORE0_MATCHn;
    /*p29.DYDU*/ Signal STORE1_MATCHn;
    /*p29.DEGO*/ Signal STORE2_MATCHn;
    /*p29.YLOZ*/ Signal STORE3_MATCHn;
    /*p29.XAGE*/ Signal STORE4_MATCHn;
    /*p29.EGOM*/ Signal STORE5_MATCHn;
    /*p29.YBEZ*/ Signal STORE6_MATCHn;
    /*p29.DYKA*/ Signal STORE7_MATCHn;
    /*p29.EFYL*/ Signal STORE8_MATCHn;
    /*p29.YGEM*/ Signal STORE9_MATCHn;

    /*p29.GEZE*/ Signal STORE0_MATCH_OUT;
    /*p29.FUMA*/ Signal STORE1_MATCH_OUT;
    /*p29.GEDE*/ Signal STORE2_MATCH_OUT;
    /*p29.WUTO*/ Signal STORE3_MATCH_OUT;
    /*p29.XYLA*/ Signal STORE4_MATCH_OUT;
    /*p29.WEJA*/ Signal STORE5_MATCH_OUT;
    /*p29.WYLA*/ Signal STORE6_MATCH_OUT;
    /*p29.FAVO*/ Signal STORE7_MATCH_OUT;
    /*p29.GYGA*/ Signal STORE8_MATCH_OUT;

    // sprite store 0
    {
      /*p31.ZOGY*/ wire STORE0_MATCH0n = xor(sst_reg.STORE0_X0, X0n);
      /*p31.ZEBA*/ wire STORE0_MATCH1n = xor(sst_reg.STORE0_X1, X1n);
      /*p31.ZAHA*/ wire STORE0_MATCH2n = xor(sst_reg.STORE0_X2, X2n);
      /*p31.ZOKY*/ wire STORE0_MATCH3n = xor(sst_reg.STORE0_X3, X3n);
      /*p31.WOJU*/ wire STORE0_MATCH4n = xor(sst_reg.STORE0_X4, X4n);
      /*p31.YFUN*/ wire STORE0_MATCH5n = xor(sst_reg.STORE0_X5, X5n);
      /*p31.WYZA*/ wire STORE0_MATCH6n = xor(sst_reg.STORE0_X6, X6n);
      /*p31.YPUK*/ wire STORE0_MATCH7n = xor(sst_reg.STORE0_X7, X7n);
      /*p31.ZAKO*/ wire STORE0_MATCHA = nor(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
      /*p31.XEBA*/ wire STORE0_MATCHB = nor(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);

      /*p29.YDUG*/ STORE0_MATCHn = nand(MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
      /*p29.WEFU*/ wire STORE0_MATCH = not(STORE0_MATCHn);
      /*p29.GEZE*/ STORE0_MATCH_OUT = or (STORE0_MATCH, joy_pins.P10_B);

      /*p29.GUVA*/ wire SPRITE0_GET = nor(STORE0_MATCHn,  joy_pins.P10_B);
      /*p29.EBOJ*/ sst_reg.SPRITE0_GET_SYNC.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE0_GET);

      /*p29.CEMY*/ wire STORE0_CLK    = or(STORE_EN, STORE0_SEL);
      /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
      /*p29.DYWE*/ wire STORE0_RST    = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE0_GET_SYNC);
      /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

      /*p29.GENY*/ wire STORE0_CLKa  = not(STORE0_CLKn);
      /*p30.YGUS*/ sst_reg.STORE0_IDX0.set(STORE0_CLKa, spr_reg.TS_IDX_0);
      /*p30.YSOK*/ sst_reg.STORE0_IDX1.set(STORE0_CLKa, spr_reg.TS_IDX_1);
      /*p30.YZEP*/ sst_reg.STORE0_IDX2.set(STORE0_CLKa, spr_reg.TS_IDX_2);
      /*p30.WYTE*/ sst_reg.STORE0_IDX3.set(STORE0_CLKa, spr_reg.TS_IDX_3);
      /*p30.ZONY*/ sst_reg.STORE0_IDX4.set(STORE0_CLKa, spr_reg.TS_IDX_4);
      /*p30.YWAK*/ sst_reg.STORE0_IDX5.set(STORE0_CLKa, spr_reg.TS_IDX_5);
    
      /*p29.ENOB*/ wire STORE0_CLKb  = not(STORE0_CLKn);
      /*p30.FYHY*/ sst_reg.STORE0_LINE0.set(STORE0_CLKb, spr_reg.TS_LINE_0);
      /*p30.GYHO*/ sst_reg.STORE0_LINE1.set(STORE0_CLKb, spr_reg.TS_LINE_1);
      /*p30.BOZU*/ sst_reg.STORE0_LINE2.set(STORE0_CLKb, spr_reg.TS_LINE_2);
      /*p30.CUFO*/ sst_reg.STORE0_LINE3.set(STORE0_CLKb, spr_reg.TS_LINE_3);

      /*p29.FUXU*/ wire STORE0_CLKc  = not(STORE0_CLKn);
      /*p31.XEPE*/ sst_reg.STORE0_X0.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X0);
      /*p31.YLAH*/ sst_reg.STORE0_X1.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X1);
      /*p31.ZOLA*/ sst_reg.STORE0_X2.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X2);
      /*p31.ZULU*/ sst_reg.STORE0_X3.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X3);
      /*p31.WELO*/ sst_reg.STORE0_X4.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X4);
      /*p31.XUNY*/ sst_reg.STORE0_X5.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X5);
      /*p31.WOTE*/ sst_reg.STORE0_X6.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X6);
      /*p31.XAKO*/ sst_reg.STORE0_X7.set(STORE0_CLKc, STORE0_RSTn, SPRITE_X7);

      /*p29.FURO*/ wire SPRITE0_GETn = not(SPRITE0_GET);
      /*p30.ZETU*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX0);
      /*p30.ZECE*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX1);
      /*p30.ZAVE*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX2);
      /*p30.WOKO*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX3);
      /*p30.ZUMU*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX4);
      /*p30.ZEDY*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE0_GETn, sst_reg.STORE0_IDX5);
      /*p30.GOFO*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE0_GETn, sst_reg.STORE0_LINE0);
      /*p30.WEHE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE0_GETn, sst_reg.STORE0_LINE1);
      /*p30.AJAL*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE0_GETn, sst_reg.STORE0_LINE2);
      /*p30.BUKY*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE0_GETn, sst_reg.STORE0_LINE3);
    }

    // sprite store 1
    {
      /*p31.EDYM*/ wire STORE1_MATCH0 = xor(sst_reg.STORE1_X0, X0n);
      /*p31.EMYB*/ wire STORE1_MATCH1 = xor(sst_reg.STORE1_X1, X1n);
      /*p31.EBEF*/ wire STORE1_MATCH2 = xor(sst_reg.STORE1_X2, X2n);
      /*p31.EWOK*/ wire STORE1_MATCH3 = xor(sst_reg.STORE1_X3, X3n);
      /*p31.COLA*/ wire STORE1_MATCH4 = xor(sst_reg.STORE1_X4, X4n);
      /*p31.BOBA*/ wire STORE1_MATCH5 = xor(sst_reg.STORE1_X5, X5n);
      /*p31.COLU*/ wire STORE1_MATCH6 = xor(sst_reg.STORE1_X6, X6n);
      /*p31.BAHU*/ wire STORE1_MATCH7 = xor(sst_reg.STORE1_X7, X7n);
      /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4, STORE1_MATCH5, STORE1_MATCH6, STORE1_MATCH7);
      /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0, STORE1_MATCH1, STORE1_MATCH2, STORE1_MATCH3);

      /*p29.DYDU*/ STORE1_MATCHn = nand(MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
      /*p29.GAJA*/ wire STORE1_MATCH = not(STORE1_MATCHn);
      /*p29.FUMA*/ STORE1_MATCH_OUT = or(STORE1_MATCH, STORE0_MATCH_OUT);

      /*p29.ENUT*/ wire SPRITE1_GET = nor(STORE1_MATCHn, STORE0_MATCH_OUT);

      /*p29.BYBY*/ wire STORE1_CLK  = or(STORE_EN, STORE1_SEL);
      /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);

      /*p29.CEDY*/ sst_reg.SPRITE1_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE1_GET);
      /*p29.EFEV*/ wire STORE1_RST = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE1_GET_SYNCn);
      /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

      /*p29.BYVY*/ wire STORE1_CLKc = not(STORE1_CLKn);
      /*p30.CAJY*/ sst_reg.STORE1_IDX0.set(STORE1_CLKc, spr_reg.TS_IDX_0);
      /*p30.CUZA*/ sst_reg.STORE1_IDX1.set(STORE1_CLKc, spr_reg.TS_IDX_1);
      /*p30.COMA*/ sst_reg.STORE1_IDX2.set(STORE1_CLKc, spr_reg.TS_IDX_2);
      /*p30.CUFA*/ sst_reg.STORE1_IDX3.set(STORE1_CLKc, spr_reg.TS_IDX_3);
      /*p30.CEBO*/ sst_reg.STORE1_IDX4.set(STORE1_CLKc, spr_reg.TS_IDX_4);
      /*p30.CADU*/ sst_reg.STORE1_IDX5.set(STORE1_CLKc, spr_reg.TS_IDX_5);
                                                                                           
      /*p29.AHOF*/ wire STORE1_CLKb = not(STORE1_CLKn);
      /*p30.ABUG*/ sst_reg.STORE1_LINE0.set(STORE1_CLKb, spr_reg.TS_LINE_0);
      /*p30.AMES*/ sst_reg.STORE1_LINE1.set(STORE1_CLKb, spr_reg.TS_LINE_1);
      /*p30.ABOP*/ sst_reg.STORE1_LINE2.set(STORE1_CLKb, spr_reg.TS_LINE_2);
      /*p30.AROF*/ sst_reg.STORE1_LINE3.set(STORE1_CLKb, spr_reg.TS_LINE_3);

      /*p29.ASYS*/ wire STORE1_CLKa = not(STORE1_CLKn);
      /*p31.DANY*/ sst_reg.STORE1_X0.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X0);
      /*p31.DUKO*/ sst_reg.STORE1_X1.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X1);
      /*p31.DESU*/ sst_reg.STORE1_X2.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X2);
      /*p31.DAZO*/ sst_reg.STORE1_X3.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X3);
      /*p31.DAKE*/ sst_reg.STORE1_X4.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X4);
      /*p31.CESO*/ sst_reg.STORE1_X5.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X5);
      /*p31.DYFU*/ sst_reg.STORE1_X6.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X6);
      /*p31.CUSY*/ sst_reg.STORE1_X7.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X7);

      /*p29.DYDO*/ wire SPRITE1_GETn = not(SPRITE1_GET);
      /*p30.BEMO*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX0);
      /*p30.CYBY*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX1);
      /*p30.BETY*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX2);
      /*p30.CEGY*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX3);
      /*p30.CELU*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX4);
      /*p30.CUBO*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE1_GETn, sst_reg.STORE1_IDX5);
      /*p30.BEFE*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE1_GETn, sst_reg.STORE1_LINE0);
      /*p30.BYRO*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE1_GETn, sst_reg.STORE1_LINE1);
      /*p30.BACO*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE1_GETn, sst_reg.STORE1_LINE2);
      /*p30.AHUM*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE1_GETn, sst_reg.STORE1_LINE3);
    }

    // sprite store 2
    {
      /*p31.FUZU*/ wire STORE2_MATCH0 = xor(sst_reg.STORE2_X0, X0n);
      /*p31.FESO*/ wire STORE2_MATCH1 = xor(sst_reg.STORE2_X1, X1n);
      /*p31.FOKY*/ wire STORE2_MATCH2 = xor(sst_reg.STORE2_X2, X2n);
      /*p31.FYVA*/ wire STORE2_MATCH3 = xor(sst_reg.STORE2_X3, X3n);
      /*p31.CEKO*/ wire STORE2_MATCH4 = xor(sst_reg.STORE2_X4, X4n);
      /*p31.DETY*/ wire STORE2_MATCH5 = xor(sst_reg.STORE2_X5, X5n);
      /*p31.DOZO*/ wire STORE2_MATCH6 = xor(sst_reg.STORE2_X6, X6n);
      /*p31.CONY*/ wire STORE2_MATCH7 = xor(sst_reg.STORE2_X7, X7n);
      /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4, STORE2_MATCH5, STORE2_MATCH6, STORE2_MATCH7);
      /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0, STORE2_MATCH1, STORE2_MATCH2, STORE2_MATCH3);

      /*p29.DEGO*/ STORE2_MATCHn = nand(MATCH_EN, STORE2_MATCHB, STORE2_MATCHA);
      /*p29.GUPO*/ wire STORE2_MATCH = not(STORE2_MATCHn);
      /*p29.GEDE*/ STORE2_MATCH_OUT = or(STORE2_MATCH, STORE1_MATCH_OUT);

      /*p29.EMOL*/ wire SPRITE2_GET = nor(STORE2_MATCHn, STORE1_MATCH_OUT);
      /*p29.EGAV*/ sst_reg.SPRITE2_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE2_GET);

      /*p29.WYXO*/ wire STORE2_CLK = or(STORE_EN, STORE2_SEL); // 0010
      /*p29.GYFO*/ wire STORE2_CLKn = not(STORE2_CLK); // 0010
      /*p29.FOKO*/ wire STORE2_RST  = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE2_GET_SYNCn);
      /*p29.GAMY*/ wire STORE2_RSTn = not(STORE2_RST);

      /*p29.BUZY*/ wire STORE2_CLKb = not(STORE2_CLKn);
      /*p30.BOXA*/ sst_reg.STORE2_IDX0.set(STORE2_CLKb, spr_reg.TS_IDX_0);
      /*p30.BUNA*/ sst_reg.STORE2_IDX1.set(STORE2_CLKb, spr_reg.TS_IDX_1);
      /*p30.BULU*/ sst_reg.STORE2_IDX2.set(STORE2_CLKb, spr_reg.TS_IDX_2);
      /*p30.BECA*/ sst_reg.STORE2_IDX3.set(STORE2_CLKb, spr_reg.TS_IDX_3);
      /*p30.BYHU*/ sst_reg.STORE2_IDX4.set(STORE2_CLKb, spr_reg.TS_IDX_4);
      /*p30.BUHE*/ sst_reg.STORE2_IDX5.set(STORE2_CLKb, spr_reg.TS_IDX_5);

      /*p29.FUKE*/ wire STORE2_CLKc = not(STORE2_CLKn);
      /*p30.YKUK*/ sst_reg.STORE2_LINE0.set(STORE2_CLKc, spr_reg.TS_LINE_0);
      /*p30.YLOV*/ sst_reg.STORE2_LINE1.set(STORE2_CLKc, spr_reg.TS_LINE_1);
      /*p30.XAZY*/ sst_reg.STORE2_LINE2.set(STORE2_CLKc, spr_reg.TS_LINE_2);
      /*p30.XOSY*/ sst_reg.STORE2_LINE3.set(STORE2_CLKc, spr_reg.TS_LINE_3);

      /*p29.CACU*/ wire STORE2_CLKa = not(STORE2_CLKn);
      /*p31.FOKA*/ sst_reg.STORE2_X0.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X0);
      /*p31.FYTY*/ sst_reg.STORE2_X1.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X1);
      /*p31.FUBY*/ sst_reg.STORE2_X2.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X2);
      /*p31.GOXU*/ sst_reg.STORE2_X3.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X3);
      /*p31.DUHY*/ sst_reg.STORE2_X4.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X4);
      /*p31.EJUF*/ sst_reg.STORE2_X5.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X5);
      /*p31.ENOR*/ sst_reg.STORE2_X6.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X6);
      /*p31.DEPY*/ sst_reg.STORE2_X7.set(STORE2_CLKa, STORE2_RSTn, SPRITE_X7);


      /*p29.FAME*/ wire SPRITE2_GETn = not(SPRITE2_GET);
      /*p30.AXEC*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX0);
      /*p30.CYRO*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX1);
      /*p30.CUVU*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX2);
      /*p30.APON*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX3);
      /*p30.AFOZ*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX4);
      /*p30.CUBE*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE2_GETn, sst_reg.STORE2_IDX5);
      /*p30.ZABY*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE2_GETn, sst_reg.STORE2_LINE0);
      /*p30.ZUKE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE2_GETn, sst_reg.STORE2_LINE1);
      /*p30.WUXE*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE2_GETn, sst_reg.STORE2_LINE2);
      /*p30.WERE*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE2_GETn, sst_reg.STORE2_LINE3);
    }

    {
      /*p31.YHOK*/ wire STORE3_MATCH0 = xor(sst_reg.XOLY, X0n);
      /*p31.YCAH*/ wire STORE3_MATCH1 = xor(sst_reg.XYBA, X1n);
      /*p31.YDAJ*/ wire STORE3_MATCH2 = xor(sst_reg.XABE, X2n);
      /*p31.YVUZ*/ wire STORE3_MATCH3 = xor(sst_reg.XEKA, X3n);
      /*p31.YVAP*/ wire STORE3_MATCH4 = xor(sst_reg.XOMY, X4n);
      /*p31.XENY*/ wire STORE3_MATCH5 = xor(sst_reg.WUHA, X5n);
      /*p31.XAVU*/ wire STORE3_MATCH6 = xor(sst_reg.WYNA, X6n);
      /*p31.XEVA*/ wire STORE3_MATCH7 = xor(sst_reg.WECO, X7n);
      /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0, STORE3_MATCH1, STORE3_MATCH2, STORE3_MATCH3);
      /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4, STORE3_MATCH5, STORE3_MATCH6, STORE3_MATCH7);

      /*p29.YLOZ*/ STORE3_MATCHn = nand(MATCH_EN, STORE3_MATCHA, STORE3_MATCHB);
      /*p29.WEBO*/ wire STORE3_MATCH = not(STORE3_MATCHn);
      /*p29.WUTO*/ STORE3_MATCH_OUT = or(STORE3_MATCH, STORE2_MATCH_OUT);

      /*p29.GYFY*/ wire SPRITE3_GET = nor(STORE3_MATCHn, STORE2_MATCH_OUT);
      /*p29.GOTA*/ sst_reg.SPRITE3_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE3_GET);

      /*p29.GAKE*/ wire STORE3_RST = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE3_GET_SYNCn);
      /*p29.WUPA*/ wire STORE3_RSTn = not(STORE3_RST);

      /*p29.GUVE*/ wire STORE3_CLK = or(STORE_EN, STORE3_SEL); // 0011
      /*p29.GUSA*/ wire STORE3_CLKn = not(STORE3_CLK);
    
      /*p29.FEKA*/ wire FEKA = not(STORE3_CLKn);
      /*p30.DAFU*/ sst_reg.DAFU.set(FEKA, spr_reg.TS_IDX_0);
      /*p30.DEBA*/ sst_reg.DEBA.set(FEKA, spr_reg.TS_IDX_1);
      /*p30.DUHA*/ sst_reg.DUHA.set(FEKA, spr_reg.TS_IDX_2);
      /*p30.DUNY*/ sst_reg.DUNY.set(FEKA, spr_reg.TS_IDX_3);
      /*p30.DESE*/ sst_reg.DESE.set(FEKA, spr_reg.TS_IDX_4);
      /*p30.DEVY*/ sst_reg.DEVY.set(FEKA, spr_reg.TS_IDX_5);

      /*p29.XYHA*/ wire XYHA = not(STORE3_CLKn);
      /*p30.ZURY*/ sst_reg.ZURY.set(XYHA, spr_reg.TS_LINE_0);
      /*p30.ZURO*/ sst_reg.ZURO.set(XYHA, spr_reg.TS_LINE_1);
      /*p30.ZENE*/ sst_reg.ZENE.set(XYHA, spr_reg.TS_LINE_2);
      /*p30.ZYLU*/ sst_reg.ZYLU.set(XYHA, spr_reg.TS_LINE_3);

      /*p29.YFAG*/ wire YFAG = not(STORE3_CLKn);
      /*p31.XOLY*/ sst_reg.XOLY.set(YFAG, STORE3_RSTn, SPRITE_X0);
      /*p31.XYBA*/ sst_reg.XYBA.set(YFAG, STORE3_RSTn, SPRITE_X1);
      /*p31.XABE*/ sst_reg.XABE.set(YFAG, STORE3_RSTn, SPRITE_X2);
      /*p31.XEKA*/ sst_reg.XEKA.set(YFAG, STORE3_RSTn, SPRITE_X3);
      /*p31.XOMY*/ sst_reg.XOMY.set(YFAG, STORE3_RSTn, SPRITE_X4);
      /*p31.WUHA*/ sst_reg.WUHA.set(YFAG, STORE3_RSTn, SPRITE_X5);
      /*p31.WYNA*/ sst_reg.WYNA.set(YFAG, STORE3_RSTn, SPRITE_X6);
      /*p31.WECO*/ sst_reg.WECO.set(YFAG, STORE3_RSTn, SPRITE_X7);

      /*p29.GYMA*/ wire SPRITE3_GETn = not(SPRITE3_GET);
      /*p30.DEZU*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE3_GETn, sst_reg.DAFU);
      /*p30.EFUD*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE3_GETn, sst_reg.DEBA);
      /*p30.DONY*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE3_GETn, sst_reg.DUHA);
      /*p30.DOWA*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE3_GETn, sst_reg.DUNY);
      /*p30.DYGO*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE3_GETn, sst_reg.DESE);
      /*p30.ENAP*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE3_GETn, sst_reg.DEVY);
      /*p30.ZYPO*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE3_GETn, sst_reg.ZURY);
      /*p30.ZEXE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE3_GETn, sst_reg.ZURO);
      /*p30.YJEM*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE3_GETn, sst_reg.ZENE);
      /*p30.YWAV*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE3_GETn, sst_reg.ZYLU);
    }

    {
      /*p31.ZYKU*/ wire ZYKU = xor(sst_reg.YBED, X4n);
      /*p31.ZYPU*/ wire ZYPU = xor(sst_reg.ZALA, X5n);
      /*p31.XAHA*/ wire XAHA = xor(sst_reg.WYDE, X6n);
      /*p31.ZEFE*/ wire ZEFE = xor(sst_reg.XEPA, X7n);
      /*p31.XEJU*/ wire XEJU = xor(sst_reg.WEDU, X0n);
      /*p31.ZATE*/ wire ZATE = xor(sst_reg.YGAJ, X1n);
      /*p31.ZAKU*/ wire ZAKU = xor(sst_reg.ZYJO, X2n);
      /*p31.YBOX*/ wire YBOX = xor(sst_reg.XURY, X3n);
      /*p31.YKOK*/ wire YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
      /*p31.YNAZ*/ wire YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

      /*p29.XAGE*/ STORE4_MATCHn = nand(MATCH_EN, YNAZ, YKOK);
      /*p29.WUNA*/ wire STORE4_MATCH = not(STORE4_MATCHn);

      /*p29.GONO*/ wire SPRITE4_GET = nor(STORE4_MATCHn, STORE3_MATCH_OUT);
      /*p29.XUDY*/ sst_reg.SPRITE4_GET_SYNC.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE4_GET);
      /*p29.WOHU*/ wire WOHU = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE4_GET_SYNC);
      /*p29.XYLA*/ STORE4_MATCH_OUT = or(STORE4_MATCH, STORE3_MATCH_OUT);

      /*p29.CECU*/ wire STORE4_CLK = or(STORE_EN, STORE4_SEL); // 0100
      /*p29.DUKE*/ wire DUKE = not(STORE4_CLK);
  
      /*p29.WUNU*/ wire WUNU = not(WOHU); // i think these are swapped
      /*p29.WOFO*/ wire WOFO = not(DUKE);
      /*p31.WEDU*/ sst_reg.WEDU.set(WUNU, WOFO, SPRITE_X0);
      /*p31.YGAJ*/ sst_reg.YGAJ.set(WUNU, WOFO, SPRITE_X1);
      /*p31.ZYJO*/ sst_reg.ZYJO.set(WUNU, WOFO, SPRITE_X2);
      /*p31.XURY*/ sst_reg.XURY.set(WUNU, WOFO, SPRITE_X3);
      /*p31.YBED*/ sst_reg.YBED.set(WUNU, WOFO, SPRITE_X4);
      /*p31.ZALA*/ sst_reg.ZALA.set(WUNU, WOFO, SPRITE_X5);
      /*p31.WYDE*/ sst_reg.WYDE.set(WUNU, WOFO, SPRITE_X6);
      /*p31.XEPA*/ sst_reg.XEPA.set(WUNU, WOFO, SPRITE_X7);

      /*p29.WYLU*/ wire WYLU = not(DUKE);
      /*p30.XYNU*/ sst_reg.XYNU.set(WYLU, spr_reg.TS_IDX_0);
      /*p30.XEGE*/ sst_reg.XEGE.set(WYLU, spr_reg.TS_IDX_1);
      /*p30.XABO*/ sst_reg.XABO.set(WYLU, spr_reg.TS_IDX_2);
      /*p30.WANU*/ sst_reg.WANU.set(WYLU, spr_reg.TS_IDX_3);
      /*p30.XEFE*/ sst_reg.XEFE.set(WYLU, spr_reg.TS_IDX_4);
      /*p30.XAVE*/ sst_reg.XAVE.set(WYLU, spr_reg.TS_IDX_5);

      /*p29.EWOT*/ wire EWOT = not(DUKE);
      /*p30.CUMU*/ sst_reg.CUMU.set(EWOT, spr_reg.TS_LINE_0);
      /*p30.CAPO*/ sst_reg.CAPO.set(EWOT, spr_reg.TS_LINE_1);
      /*p30.CONO*/ sst_reg.CONO.set(EWOT, spr_reg.TS_LINE_2);
      /*p30.CAJU*/ sst_reg.CAJU.set(EWOT, spr_reg.TS_LINE_3);

      /*p29.GOWO*/ wire SPRITE4_GETn = not(SPRITE4_GET);
      /*p30.WAJA*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE4_GETn, sst_reg.XYNU);
      /*p30.WOXY*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE4_GETn, sst_reg.XEGE);
      /*p30.XYRE*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE4_GETn, sst_reg.XABO);
      /*p30.WERU*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE4_GETn, sst_reg.WANU);
      /*p30.WEPY*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE4_GETn, sst_reg.XEFE);
      /*p30.WUXU*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE4_GETn, sst_reg.XAVE);
      /*p30.BYDO*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE4_GETn, sst_reg.CUMU);
      /*p30.BUCE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE4_GETn, sst_reg.CAPO);
      /*p30.BOVE*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE4_GETn, sst_reg.CONO);
      /*p30.BEVY*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE4_GETn, sst_reg.CAJU);
    }

    {
      /*p31.BAZY*/ wire BAZY = xor(sst_reg.CYWE, X4n);
      /*p31.CYLE*/ wire CYLE = xor(sst_reg.DYBY, X5n);
      /*p31.CEVA*/ wire CEVA = xor(sst_reg.DURY, X6n);
      /*p31.BUMY*/ wire BUMY = xor(sst_reg.CUVY, X7n);
      /*p31.GUZO*/ wire GUZO = xor(sst_reg.FUSA, X0n);
      /*p31.GOLA*/ wire GOLA = xor(sst_reg.FAXA, X1n);
      /*p31.GEVE*/ wire GEVE = xor(sst_reg.FOZY, X2n);
      /*p31.GUDE*/ wire GUDE = xor(sst_reg.FESY, X3n);
      /*p31.COGY*/ wire COGY = nor(BAZY, CYLE, CEVA, BUMY);
      /*p31.FYMA*/ wire FYMA = nor(GUZO, GOLA, GEVE, GUDE);
      /*p29.EGOM*/ STORE5_MATCHn = nand(MATCH_EN, FYMA, COGY);

      /*p29.GEGA*/ wire SPRITE5_GET = nor(STORE5_MATCHn, STORE4_MATCH_OUT);
      /*p29.WAFY*/ sst_reg.SPRITE5_GET_SYNC.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE5_GET);
      /*p29.FEVE*/ wire FEVE = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE5_GET_SYNC);
      /*p29.EJAD*/ wire EJAD = not(FEVE);
      /*p29.GABA*/ wire STORE5_MATCH = not(STORE5_MATCHn);
    
      /*p29.WEJA*/ STORE5_MATCH_OUT = or(STORE5_MATCH, STORE4_MATCH_OUT);

      /*p29.CADO*/ wire STORE5_CLK = or(STORE_EN, STORE5_SEL); // 0101
      /*p29.BEDE*/ wire BEDE = not(STORE5_CLK);

      /*p29.CYLA*/ wire CYLA = not(BEDE);
      /*p31.FUSA*/ sst_reg.FUSA.set(CYLA, EJAD, SPRITE_X0);
      /*p31.FAXA*/ sst_reg.FAXA.set(CYLA, EJAD, SPRITE_X1);
      /*p31.FOZY*/ sst_reg.FOZY.set(CYLA, EJAD, SPRITE_X2);
      /*p31.FESY*/ sst_reg.FESY.set(CYLA, EJAD, SPRITE_X3);
      /*p31.CYWE*/ sst_reg.CYWE.set(CYLA, EJAD, SPRITE_X4);
      /*p31.DYBY*/ sst_reg.DYBY.set(CYLA, EJAD, SPRITE_X5);
      /*p31.DURY*/ sst_reg.DURY.set(CYLA, EJAD, SPRITE_X6);
      /*p31.CUVY*/ sst_reg.CUVY.set(CYLA, EJAD, SPRITE_X7);

      /*p29.DYMO*/ wire DYMO = not(BEDE);
      /*p30.EKAP*/ sst_reg.EKAP.set(DYMO, spr_reg.TS_IDX_0);
      /*p30.ETAV*/ sst_reg.ETAV.set(DYMO, spr_reg.TS_IDX_1);
      /*p30.EBEX*/ sst_reg.EBEX.set(DYMO, spr_reg.TS_IDX_2);
      /*p30.GORU*/ sst_reg.GORU.set(DYMO, spr_reg.TS_IDX_3);
      /*p30.ETYM*/ sst_reg.ETYM.set(DYMO, spr_reg.TS_IDX_4);
      /*p30.EKOP*/ sst_reg.EKOP.set(DYMO, spr_reg.TS_IDX_5);

      /*p29.BUCY*/ wire BUCY = not(BEDE);
      /*p30.ANED*/ sst_reg.ANED.set(BUCY, spr_reg.TS_LINE_0);
      /*p30.ACEP*/ sst_reg.ACEP.set(BUCY, spr_reg.TS_LINE_1);
      /*p30.ABUX*/ sst_reg.ABUX.set(BUCY, spr_reg.TS_LINE_2);
      /*p30.ABEG*/ sst_reg.ABEG.set(BUCY, spr_reg.TS_LINE_3);

      /*p29.GYGY*/ wire SPRITE5_GETn = not(SPRITE5_GET);
      /*p30.DALO*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE5_GETn, sst_reg.EKAP);
      /*p30.DALY*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE5_GETn, sst_reg.ETAV);
      /*p30.DUZA*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE5_GETn, sst_reg.EBEX);
      /*p30.WAGA*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE5_GETn, sst_reg.GORU);
      /*p30.DYNY*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE5_GETn, sst_reg.ETYM);
      /*p30.DOBO*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE5_GETn, sst_reg.EKOP);
      /*p30.AWAT*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE5_GETn, sst_reg.ANED);
      /*p30.BACE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE5_GETn, sst_reg.ACEP);
      /*p30.BODU*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE5_GETn, sst_reg.ABUX);
      /*p30.BUJA*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE5_GETn, sst_reg.ABEG);
    }

    {
      /*p31.ZARE*/ wire ZARE = xor(sst_reg.ZOLY, X4n);
      /*p31.ZEMU*/ wire ZEMU = xor(sst_reg.ZOGO, X5n);
      /*p31.ZYGO*/ wire ZYGO = xor(sst_reg.ZECU, X6n);
      /*p31.ZUZY*/ wire ZUZY = xor(sst_reg.ZESA, X7n);
      /*p31.XOSU*/ wire XOSU = xor(sst_reg.YCOL, X0n);
      /*p31.ZUVU*/ wire ZUVU = xor(sst_reg.YRAC, X1n);
      /*p31.XUCO*/ wire XUCO = xor(sst_reg.YMEM, X2n);
      /*p31.ZULO*/ wire ZULO = xor(sst_reg.YVAG, X3n);
      /*p31.YWAP*/ wire YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
      /*p31.YDOT*/ wire YDOT = nor(XOSU, ZUVU, XUCO, ZULO);
      /*p29.YBEZ*/ STORE6_MATCHn = nand(MATCH_EN, YDOT, YWAP);

      /*p29.XOJA*/ wire SPRITE6_GET = nor(STORE6_MATCHn, STORE5_MATCH_OUT);
      /*p29.WOMY*/ sst_reg.SPRITE6_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE6_GET);
      /*p29.WACY*/ wire WACY = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE6_GET_SYNCn);
      /*p29.XAHO*/ wire XAHO = not(WACY);
      /*p29.WASE*/ wire STORE6_MATCH = not(STORE6_MATCHn);
      /*p29.WYLA*/ STORE6_MATCH_OUT = or(STORE6_MATCH, STORE5_MATCH_OUT);

      /*p29.XUJO*/ wire STORE6_CLK = or(STORE_EN, STORE6_SEL); // 0110
      /*p29.WEKA*/ wire WEKA = not(STORE6_CLK);

      /*p29.ZAPE*/ wire ZAPE = not(WEKA);
      /*p31.YCOL*/ sst_reg.YCOL.set(ZAPE, XAHO, SPRITE_X0);
      /*p31.YRAC*/ sst_reg.YRAC.set(ZAPE, XAHO, SPRITE_X1);
      /*p31.YMEM*/ sst_reg.YMEM.set(ZAPE, XAHO, SPRITE_X2);
      /*p31.YVAG*/ sst_reg.YVAG.set(ZAPE, XAHO, SPRITE_X3);
      /*p31.ZOLY*/ sst_reg.ZOLY.set(ZAPE, XAHO, SPRITE_X4);
      /*p31.ZOGO*/ sst_reg.ZOGO.set(ZAPE, XAHO, SPRITE_X5);
      /*p31.ZECU*/ sst_reg.ZECU.set(ZAPE, XAHO, SPRITE_X6);
      /*p31.ZESA*/ sst_reg.ZESA.set(ZAPE, XAHO, SPRITE_X7);

      /*p29.WUSE*/ wire WUSE = not(WEKA);
      /*p30.GECU*/ sst_reg.GECU.set(WUSE, spr_reg.TS_IDX_0);
      /*p30.FOXY*/ sst_reg.FOXY.set(WUSE, spr_reg.TS_IDX_1);
      /*p30.GOHU*/ sst_reg.GOHU.set(WUSE, spr_reg.TS_IDX_2);
      /*p30.FOGO*/ sst_reg.FOGO.set(WUSE, spr_reg.TS_IDX_3);
      /*p30.GACY*/ sst_reg.GACY.set(WUSE, spr_reg.TS_IDX_4);
      /*p30.GABO*/ sst_reg.GABO.set(WUSE, spr_reg.TS_IDX_5);
    
      /*p29.ZURU*/ wire ZURU = not(WEKA);
      /*p30.ZUBE*/ sst_reg.ZUBE.set(ZURU, spr_reg.TS_LINE_0);
      /*p30.ZUMY*/ sst_reg.ZUMY.set(ZURU, spr_reg.TS_LINE_1);
      /*p30.ZEXO*/ sst_reg.ZEXO.set(ZURU, spr_reg.TS_LINE_2);
      /*p30.ZAFU*/ sst_reg.ZAFU.set(ZURU, spr_reg.TS_LINE_3);

      /*p29.XYME*/ wire SPRITE6_GETn = not(SPRITE6_GET);
      /*p30.WOCY*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE6_GETn, sst_reg.GECU);
      /*p30.ELYC*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE6_GETn, sst_reg.FOXY);
      /*p30.WABO*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE6_GETn, sst_reg.GOHU);
      /*p30.EZOC*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE6_GETn, sst_reg.FOGO);
      /*p30.WYWY*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE6_GETn, sst_reg.GACY);
      /*p30.WATO*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE6_GETn, sst_reg.GABO);
      /*p30.ZUDO*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE6_GETn, sst_reg.ZUBE);
      /*p30.YBUK*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE6_GETn, sst_reg.ZUMY);
      /*p30.ZYTO*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE6_GETn, sst_reg.ZEXO);
      /*p30.YKOZ*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE6_GETn, sst_reg.ZAFU);
    }

    {
      /*p31.DUSE*/ wire STORE7_MATCH0 = xor(sst_reg.STORE7_X0, X0n);
      /*p31.DAGU*/ wire STORE7_MATCH1 = xor(sst_reg.STORE7_X1, X1n);
      /*p31.DYZE*/ wire STORE7_MATCH2 = xor(sst_reg.STORE7_X2, X2n);
      /*p31.DESO*/ wire STORE7_MATCH3 = xor(sst_reg.STORE7_X3, X3n);
      /*p31.EJOT*/ wire STORE7_MATCH4 = xor(sst_reg.STORE7_X4, X4n);
      /*p31.ESAJ*/ wire STORE7_MATCH5 = xor(sst_reg.STORE7_X5, X5n);
      /*p31.DUCU*/ wire STORE7_MATCH6 = xor(sst_reg.STORE7_X6, X6n);
      /*p31.EWUD*/ wire STORE7_MATCH7 = xor(sst_reg.STORE7_X7, X7n);
      /*p31.CYCO*/ wire STORE7_MATCHA = nor(STORE7_MATCH0, STORE7_MATCH1, STORE7_MATCH2, STORE7_MATCH3);
      /*p31.DAJE*/ wire STORE7_MATCHB = nor(STORE7_MATCH4, STORE7_MATCH5, STORE7_MATCH6, STORE7_MATCH7);
      /*p29.DYKA*/ STORE7_MATCHn = nand(MATCH_EN, STORE7_MATCHA, STORE7_MATCHB);
      /*p29.GYTE*/ wire STORE7_MATCH = not(STORE7_MATCHn);
      /*p29.FAVO*/ STORE7_MATCH_OUT = or(STORE7_MATCH, STORE6_MATCH_OUT);

      /*p29.GUTU*/ wire SPRITE7_GET = nor(STORE7_MATCHn, STORE6_MATCH_OUT);
      /*p29.WAPO*/ sst_reg.SPRITE7_GET_SYNC.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE7_GET);
      /*p29.GUKY*/ wire STORE7_RST = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE7_GET_SYNC);
      /*p29.GAFY*/ wire STORE7_RSTn = not(STORE7_RST);

      /*p29.GAPE*/ wire STORE7_CLK = or(STORE_EN, STORE7_SEL); // 0111
      /*p29.GYVO*/ wire STORE7_CLKn = not(STORE7_CLK);

      /*p29.GECY*/ wire GECY = not(STORE7_CLKn);
      /*p31.ERAZ*/ sst_reg.STORE7_X0.set(GECY, STORE7_RSTn, SPRITE_X0);
      /*p31.EPUM*/ sst_reg.STORE7_X1.set(GECY, STORE7_RSTn, SPRITE_X1);
      /*p31.EROL*/ sst_reg.STORE7_X2.set(GECY, STORE7_RSTn, SPRITE_X2);
      /*p31.EHYN*/ sst_reg.STORE7_X3.set(GECY, STORE7_RSTn, SPRITE_X3);
      /*p31.FAZU*/ sst_reg.STORE7_X4.set(GECY, STORE7_RSTn, SPRITE_X4);
      /*p31.FAXE*/ sst_reg.STORE7_X5.set(GECY, STORE7_RSTn, SPRITE_X5);
      /*p31.EXUK*/ sst_reg.STORE7_X6.set(GECY, STORE7_RSTn, SPRITE_X6);
      /*p31.FEDE*/ sst_reg.STORE7_X7.set(GECY, STORE7_RSTn, SPRITE_X7);

      /*p29.FEFO*/ wire FEFO = not(STORE7_CLKn);
      /*p30.FUZO*/ sst_reg.FUZO.set(FEFO, spr_reg.TS_IDX_0);
      /*p30.GESY*/ sst_reg.GESY.set(FEFO, spr_reg.TS_IDX_1);
      /*p30.FYSU*/ sst_reg.FYSU.set(FEFO, spr_reg.TS_IDX_2);
      /*p30.FEFA*/ sst_reg.FEFA.set(FEFO, spr_reg.TS_IDX_3);
      /*p30.GYNO*/ sst_reg.GYNO.set(FEFO, spr_reg.TS_IDX_4);
      /*p30.GULE*/ sst_reg.GULE.set(FEFO, spr_reg.TS_IDX_5);

      /*p29.WABE*/ wire WABE = not(STORE7_CLKn);
      /*p30.XYGO*/ sst_reg.XYGO.set(WABE, spr_reg.TS_LINE_0);
      /*p30.XYNA*/ sst_reg.XYNA.set(WABE, spr_reg.TS_LINE_1);
      /*p30.XAKU*/ sst_reg.XAKU.set(WABE, spr_reg.TS_LINE_2);
      /*p30.YGUM*/ sst_reg.YGUM.set(WABE, spr_reg.TS_LINE_3);

      /*p29.GUGY*/ wire SPRITE7_GETn = not(SPRITE7_GET);
      /*p30.EVYT*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE7_GETn, sst_reg.FUZO);
      /*p30.WABA*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE7_GETn, sst_reg.GESY);
      /*p30.ETAD*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE7_GETn, sst_reg.FYSU);
      /*p30.ELEP*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE7_GETn, sst_reg.FEFA);
      /*p30.WYGO*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE7_GETn, sst_reg.GYNO);
      /*p30.WAKO*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE7_GETn, sst_reg.GULE);
      /*p30.WANA*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE7_GETn, sst_reg.XYGO);
      /*p30.WAXE*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE7_GETn, sst_reg.XYNA);
      /*p30.WABU*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE7_GETn, sst_reg.XAKU);
      /*p30.YPOZ*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE7_GETn, sst_reg.YGUM);
    }

    {
      /*p31.DUZE*/ wire STORE8_MATCH0 = xor(sst_reg.EZUF, X4n);
      /*p31.DAGA*/ wire STORE8_MATCH1 = xor(sst_reg.ENAD, X5n);
      /*p31.DAWU*/ wire STORE8_MATCH2 = xor(sst_reg.EBOW, X6n);
      /*p31.EJAW*/ wire STORE8_MATCH3 = xor(sst_reg.FYCA, X7n);
      /*p31.GOHO*/ wire STORE8_MATCH4 = xor(sst_reg.GAVY, X0n);
      /*p31.GASU*/ wire STORE8_MATCH5 = xor(sst_reg.GYPU, X1n);
      /*p31.GABU*/ wire STORE8_MATCH6 = xor(sst_reg.GADY, X2n);
      /*p31.GAFE*/ wire STORE8_MATCH7 = xor(sst_reg.GAZA, X3n);
      /*p31.DAMA*/ wire STORE8_MATCHA = nor(STORE8_MATCH0, STORE8_MATCH1, STORE8_MATCH2, STORE8_MATCH3);
      /*p31.FEHA*/ wire STORE8_MATCHB = nor(STORE8_MATCH4, STORE8_MATCH5, STORE8_MATCH6, STORE8_MATCH7);
      /*p29.EFYL*/ STORE8_MATCHn = nand(MATCH_EN, STORE8_MATCHB, STORE8_MATCHA);
      /*p29.GEKE*/ wire STORE8_MATCH = not(STORE8_MATCHn);
      /*p29.GYGA*/ STORE8_MATCH_OUT = or(STORE8_MATCH, STORE7_MATCH_OUT);
      /*p29.FOXA*/ wire SPRITE8_GET = nor(STORE8_MATCHn, STORE7_MATCH_OUT);

      /*p29.EXUQ*/ sst_reg.SPRITE8_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE8_GET);
      /*p29.GORO*/ wire GORO = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE8_GET_SYNCn);
      /*p29.WUZO*/ wire WUZO = not(GORO);

      /*p29.CAHO*/ wire STORE8_CLK = or(STORE_EN, STORE8_SEL); // 1000
      /*p29.BUKA*/ wire BUKA = not(STORE8_CLK);
      /*p29.CEXU*/ wire CEXU = not(BUKA);
      /*p29.AKOL*/ wire AKOL = not(BUKA);
      /*p29.BYMY*/ wire BYMY = not(BUKA);
      /*p31.GAVY*/ sst_reg.GAVY.set(CEXU, WUZO, SPRITE_X0);
      /*p31.GYPU*/ sst_reg.GYPU.set(CEXU, WUZO, SPRITE_X1);
      /*p31.GADY*/ sst_reg.GADY.set(CEXU, WUZO, SPRITE_X2);
      /*p31.GAZA*/ sst_reg.GAZA.set(CEXU, WUZO, SPRITE_X3);
      /*p31.EZUF*/ sst_reg.EZUF.set(CEXU, WUZO, SPRITE_X4);
      /*p31.ENAD*/ sst_reg.ENAD.set(CEXU, WUZO, SPRITE_X5);
      /*p31.EBOW*/ sst_reg.EBOW.set(CEXU, WUZO, SPRITE_X6);
      /*p31.FYCA*/ sst_reg.FYCA.set(CEXU, WUZO, SPRITE_X7);

      /*p30.AXUV*/ sst_reg.AXUV.set(AKOL, spr_reg.TS_IDX_0 );
      /*p30.BADA*/ sst_reg.BADA.set(AKOL, spr_reg.TS_IDX_1 );
      /*p30.APEV*/ sst_reg.APEV.set(AKOL, spr_reg.TS_IDX_2 );
      /*p30.BADO*/ sst_reg.BADO.set(AKOL, spr_reg.TS_IDX_3 );
      /*p30.BEXY*/ sst_reg.BEXY.set(AKOL, spr_reg.TS_IDX_4 );
      /*p30.BYHE*/ sst_reg.BYHE.set(AKOL, spr_reg.TS_IDX_5 );
      /*p30.AFYM*/ sst_reg.AFYM.set(BYMY, spr_reg.TS_LINE_0);
      /*p30.AZAP*/ sst_reg.AZAP.set(BYMY, spr_reg.TS_LINE_1);
      /*p30.AFUT*/ sst_reg.AFUT.set(BYMY, spr_reg.TS_LINE_2);
      /*p30.AFYX*/ sst_reg.AFYX.set(BYMY, spr_reg.TS_LINE_3);

      /*p29.DENY*/ wire SPRITE8_GETn = not(SPRITE8_GET);
      /*p30.ADYB*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE8_GETn, sst_reg.AXUV);
      /*p30.APOB*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE8_GETn, sst_reg.BADA);
      /*p30.APYV*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE8_GETn, sst_reg.APEV);
      /*p30.AFEN*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE8_GETn, sst_reg.BADO);
      /*p30.AKYH*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE8_GETn, sst_reg.BEXY);
      /*p30.APOC*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE8_GETn, sst_reg.BYHE);
      /*p30.BUJY*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE8_GETn, sst_reg.AFYM);
      /*p30.BOSO*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE8_GETn, sst_reg.AZAP);
      /*p30.AHAC*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE8_GETn, sst_reg.AFUT);
      /*p30.BAZU*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE8_GETn, sst_reg.AFYX);
    }

    {
      /*p31.YMAM*/ wire STORE9_MATCH0 = xor(sst_reg.STORE9_X0, X0n);
      /*p31.YTYP*/ wire STORE9_MATCH1 = xor(sst_reg.STORE9_X1, X1n);
      /*p31.YFOP*/ wire STORE9_MATCH2 = xor(sst_reg.STORE9_X2, X2n);
      /*p31.YVAC*/ wire STORE9_MATCH3 = xor(sst_reg.STORE9_X3, X3n);
      /*p31.ZYWU*/ wire STORE9_MATCH4 = xor(sst_reg.STORE9_X4, X4n);
      /*p31.ZUZA*/ wire STORE9_MATCH5 = xor(sst_reg.STORE9_X5, X5n);
      /*p31.ZEJO*/ wire STORE9_MATCH6 = xor(sst_reg.STORE9_X6, X6n);
      /*p31.ZEDA*/ wire STORE9_MATCH7 = xor(sst_reg.STORE9_X7, X7n);
      /*p31.YLEV*/ wire STORE9_MATCHA = nor(STORE9_MATCH0, STORE9_MATCH1, STORE9_MATCH2, STORE9_MATCH3);
      /*p31.YTUB*/ wire STORE9_MATCHB = nor(STORE9_MATCH4, STORE9_MATCH5, STORE9_MATCH6, STORE9_MATCH7);
      /*p29.YGEM*/ STORE9_MATCHn = nand(MATCH_EN, STORE9_MATCHA, STORE9_MATCHB);

      /*p29.CATO*/ wire STORE9_CLK = or(STORE_EN, STORE9_SEL); // 1001
      /*p29.DECU*/ wire STORE9_CLKn = not(STORE9_CLK);

      /*p29.GUZE*/ wire SPRITE9_GET = nor(STORE9_MATCHn, STORE8_MATCH_OUT);
      /*p29.FONO*/ sst_reg.SPRITE9_GET_SYNC.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE9_GET);

      /*p29.DUBU*/ wire STORE9_RST  = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE9_GET_SYNC);
      /*p29.DOSY*/ wire STORE9_RSTn = not(STORE9_RST);
    
      /*p29.WEME*/ wire STORE9_CLKa = not(STORE9_CLKn);
      /*p31.XUVY*/ sst_reg.STORE9_X0.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X0);
      /*p31.XERE*/ sst_reg.STORE9_X1.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X1);
      /*p31.XUZO*/ sst_reg.STORE9_X2.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X2);
      /*p31.XEXA*/ sst_reg.STORE9_X3.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X3);
      /*p31.YPOD*/ sst_reg.STORE9_X4.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X4);
      /*p31.YROP*/ sst_reg.STORE9_X5.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X5);
      /*p31.YNEP*/ sst_reg.STORE9_X6.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X6);
      /*p31.YZOF*/ sst_reg.STORE9_X7.set(STORE9_CLKa, STORE9_RSTn, SPRITE_X7);

      /*p29.WUFA*/ wire STORE9_CLKb = not(STORE9_CLKn);
      /*p30.XUFO*/ sst_reg.STORE9_IDX0.set(STORE9_CLKb, spr_reg.TS_IDX_0);
      /*p30.XUTE*/ sst_reg.STORE9_IDX1.set(STORE9_CLKb, spr_reg.TS_IDX_1);
      /*p30.XOTU*/ sst_reg.STORE9_IDX2.set(STORE9_CLKb, spr_reg.TS_IDX_2);
      /*p30.XYFE*/ sst_reg.STORE9_IDX3.set(STORE9_CLKb, spr_reg.TS_IDX_3);
      /*p30.YZOR*/ sst_reg.STORE9_IDX4.set(STORE9_CLKb, spr_reg.TS_IDX_4);
      /*p30.YBER*/ sst_reg.STORE9_IDX5.set(STORE9_CLKb, spr_reg.TS_IDX_5);

      /*p29.FAKA*/ wire STORE9_CLKc = not(STORE9_CLKn);
      /*p30.DEWU*/ sst_reg.STORE9_LINE0.set(STORE9_CLKc, spr_reg.TS_LINE_0);
      /*p30.CANA*/ sst_reg.STORE9_LINE1.set(STORE9_CLKc, spr_reg.TS_LINE_1);
      /*p30.DYSY*/ sst_reg.STORE9_LINE2.set(STORE9_CLKc, spr_reg.TS_LINE_2);
      /*p30.FOFO*/ sst_reg.STORE9_LINE3.set(STORE9_CLKc, spr_reg.TS_LINE_3);

      /*p29.FADO*/ wire SPRITE9_GETn = not(SPRITE9_GET);
      /*p30.YHAL*/ spr_reg.TS_IDX_0.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX0);
      /*p30.YRAD*/ spr_reg.TS_IDX_1.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX1);
      /*p30.XYRA*/ spr_reg.TS_IDX_2.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX2);
      /*p30.YNEV*/ spr_reg.TS_IDX_3.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX3);
      /*p30.ZOJY*/ spr_reg.TS_IDX_4.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX4);
      /*p30.ZARO*/ spr_reg.TS_IDX_5.set_tribuf (!SPRITE9_GETn, sst_reg.STORE9_IDX5);
      /*p30.CAWO*/ spr_reg.TS_LINE_0.set_tribuf(!SPRITE9_GETn, sst_reg.STORE9_LINE0);
      /*p30.BYME*/ spr_reg.TS_LINE_1.set_tribuf(!SPRITE9_GETn, sst_reg.STORE9_LINE1);
      /*p30.COHO*/ spr_reg.TS_LINE_2.set_tribuf(!SPRITE9_GETn, sst_reg.STORE9_LINE2);
      /*p30.GATE*/ spr_reg.TS_LINE_3.set_tribuf(!SPRITE9_GETn, sst_reg.STORE9_LINE3);
    }

    //----------------------------------------

    /*p29.DEZY*/ sst_reg.STORE_EN_SYNC.set(ZEME_AxCxExGx, XAPO_VID_RSTn, STORE_EN);

    /*p29.BAKY*/ wire SPRITES_FULL = and(sst_reg.SPRITE_COUNT1, sst_reg.SPRITE_COUNT3);
    /*p29.CAKE*/ wire SPRITE_COUNT_CLK = or(SPRITES_FULL, sst_reg.STORE_EN_SYNC);
    /*p28.AZYB*/ wire SPRITE_COUNT_RSTn = not(BYHA_VID_LINE_TRIG_d4n);
    /*p29.BESE*/ sst_reg.SPRITE_COUNT0.set(SPRITE_COUNT_CLK,  SPRITE_COUNT_RSTn, !sst_reg.SPRITE_COUNT0);
    /*p29.CUXY*/ sst_reg.SPRITE_COUNT1.set(sst_reg.SPRITE_COUNT0,     SPRITE_COUNT_RSTn, !sst_reg.SPRITE_COUNT1);
    /*p29.BEGO*/ sst_reg.SPRITE_COUNT2.set(sst_reg.SPRITE_COUNT1,     SPRITE_COUNT_RSTn, !sst_reg.SPRITE_COUNT2);
    /*p29.DYBE*/ sst_reg.SPRITE_COUNT3.set(sst_reg.SPRITE_COUNT2,     SPRITE_COUNT_RSTn, !sst_reg.SPRITE_COUNT3);

    /*p29.FEFY*/ wire FEFY = nand(STORE4_MATCHn, STORE3_MATCHn, STORE2_MATCHn, STORE1_MATCHn, STORE0_MATCHn);
    /*p29.FOVE*/ wire FOVE = nand(STORE9_MATCHn, STORE8_MATCHn, STORE7_MATCHn, STORE6_MATCHn, STORE5_MATCHn);
    /*p29.FEPO*/ FEPO_STORE_MATCH = or(FEFY, FOVE);

    /*p30.WENU*/ spr_reg.TS_LINE_0.set_tribuf(FEPO_STORE_MATCH, SPRITE_DELTA1); // order wrong
    /*p30.CUCU*/ spr_reg.TS_LINE_1.set_tribuf(FEPO_STORE_MATCH, SPRITE_DELTA2);
    /*p30.CUCA*/ spr_reg.TS_LINE_2.set_tribuf(FEPO_STORE_MATCH, SPRITE_DELTA0);
    /*p30.CEGA*/ spr_reg.TS_LINE_3.set_tribuf(FEPO_STORE_MATCH, SPRITE_DELTA3);

  }
  //();

  /*p29.XEFY*/ wire XEPY_SPRITE_DONEn = not(WUTY_SPRITE_DONE);

  /*p21.XENA*/   wire XENA_STORE_MATCHn = not(FEPO_STORE_MATCH);
  /*p21.WODU*/ wire WODU_RENDER_DONE = and(XENA_STORE_MATCHn, XANO_X_167);

  //----------------------------------------
  // sprite scan counter

  [this, XUPY_xBCxxFGx, ANOM_SCAN_RSTn, BALU_SCAN_RST, ALET_xBxDxFxH] {
    /*p28.FETO*/ wire SCAN_DONE_d0 = and(spr_reg.SCAN0, spr_reg.SCAN1, spr_reg.SCAN2, spr_reg.SCAN5); // 32 + 4 + 2 + 1 = 39
    /*p28.GAVA*/ wire SCAN_CLK = or(SCAN_DONE_d0, XUPY_xBCxxFGx);
    /*p28.YFEL*/ spr_reg.SCAN0.set(SCAN_CLK, ANOM_SCAN_RSTn, !spr_reg.SCAN0);
    /*p28.WEWY*/ spr_reg.SCAN1.set(!spr_reg.SCAN0,   ANOM_SCAN_RSTn, !spr_reg.SCAN1);
    /*p28.GOSO*/ spr_reg.SCAN2.set(!spr_reg.SCAN1,   ANOM_SCAN_RSTn, !spr_reg.SCAN2);
    /*p28.ELYN*/ spr_reg.SCAN3.set(!spr_reg.SCAN2,   ANOM_SCAN_RSTn, !spr_reg.SCAN3);
    /*p28.FAHA*/ spr_reg.SCAN4.set(!spr_reg.SCAN3,   ANOM_SCAN_RSTn, !spr_reg.SCAN4);
    /*p28.FONY*/ spr_reg.SCAN5.set(!spr_reg.SCAN4,   ANOM_SCAN_RSTn, !spr_reg.SCAN5);

    // the second clock here doesn't match the usual synchronizer pattern, but it matches the die.
    /*p29.BAGY*/ wire BAGY_SCAN_RSTn = not(BALU_SCAN_RST);
    /*p29.BYBA*/ spr_reg.SCAN_DONE_d4.set(XUPY_xBCxxFGx, BAGY_SCAN_RSTn, SCAN_DONE_d0);
    /*p29.DOBA*/ spr_reg.SCAN_DONE_d5.set(ALET_xBxDxFxH, BAGY_SCAN_RSTn, spr_reg.SCAN_DONE_d4);
  }();

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
  /*p25.ROPY*/ wire ROPY_RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);



  //----------------------------------------
  // vidregisters::tick



  /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(vid_reg.RYDY_WIN_MODE_LATCH);

  /*p27.TEVO*/ wire TEVO_CLK_STOPn = [
      this, 
      ALET_xBxDxFxH,
      LAPE_AxCxExGx, 
      LOBY_RENDERINGn,
      XAPO_VID_RSTn,
      BYHA_VID_LINE_TRIG_d4n,
      WUTY_SPRITE_DONE,
      SYLO_WIN_MODEn] {
    // The first tile generated is thrown away. I'm calling that section of rendering the front porch.
    /*p24.PYGO*/   vid_reg.PYGO_TILE_DONE.set(ALET_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, vid_reg.BG_SEQ_7);
    /*p24.POKY*/ vid_reg.POKY_FRONT_PORCH_LATCHn.sr_latch(vid_reg.PYGO_TILE_DONE, LOBY_RENDERINGn);
    /*p27.ROMO*/ wire FRONT_PORCH = not(vid_reg.POKY_FRONT_PORCH_LATCHn);
    /*p27.SUVU*/ wire SUVU = nand(vid_reg.XYMU_RENDERING_LATCH, FRONT_PORCH, vid_reg.BG_SEQ_6, vid_reg.BG_SEQ_7);
    /*p27.TAVE*/ wire TAVE = not(SUVU);
    /*p27.RYCE*/   wire RYCE_SPRITE_FETCH_TRIG = and(vid_reg._SPRITE_FETCH_SYNC1, !vid_reg._SPRITE_FETCH_SYNC2);
    /*p01.ROSY*/   wire ROSY_VID_RESET = not(XAPO_VID_RSTn);
    /*p27.SECA*/ wire SPRITE_FETCH_BEGINn = nor(RYCE_SPRITE_FETCH_TRIG, ROSY_VID_RESET, BYHA_VID_LINE_TRIG_d4n);
    /*p27.VEKU*/ wire SPRITE_FETCH_ENDn = nor(WUTY_SPRITE_DONE, TAVE);
    /*p29.TAME*/ wire SEQ_5n = nand(spr_reg.TESE_SPR_SEQ2, spr_reg.TOXE_SPR_SEQ0);
    /*p27.TAKA*/ vid_reg._SPRITE_FETCH_LATCH.sr_latch(!SPRITE_FETCH_BEGINn, !SPRITE_FETCH_ENDn); // polarity?

    {
      /*p29.TOMA*/ wire TOMA_AxCxExGx = nand(LAPE_AxCxExGx, SEQ_5n);
      /*p29.TOXE*/ spr_reg.TOXE_SPR_SEQ0.set(TOMA_AxCxExGx,          SPRITE_FETCH_BEGINn, !spr_reg.TOXE_SPR_SEQ0);
      /*p29.TULY*/ spr_reg.TULY_SPR_SEQ1.set(!spr_reg.TOXE_SPR_SEQ0, SPRITE_FETCH_BEGINn, !spr_reg.TULY_SPR_SEQ1);
      /*p29.TESE*/ spr_reg.TESE_SPR_SEQ2.set(!spr_reg.TULY_SPR_SEQ1, SPRITE_FETCH_BEGINn, !spr_reg.TESE_SPR_SEQ2);
    }
    /*p27.SOVY*/   vid_reg.SOVY_WIN_MODE_SYNC.set(ALET_xBxDxFxH, XAPO_VID_RSTn, vid_reg.RYDY_WIN_MODE_LATCH);
    /*p27.TUXY*/   wire TUXY = nand(vid_reg.SOVY_WIN_MODE_SYNC, SYLO_WIN_MODEn);
    /*p27.SUZU*/   wire SUZU = not(TUXY);
    /*p27.SEKO*/   wire SEKO_WIN_TRIGGER = nor(vid_reg.WIN_MATCH_ONSCREEN_SYNC2, !vid_reg.WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.TEVO*/ wire TEVO_CLK_STOPn = nor(SEKO_WIN_TRIGGER, SUZU, TAVE);
    return TEVO_CLK_STOPn;
  }();

  /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(ALET_xBxDxFxH);

  /*p24.TOMU*/ wire TOMU_WIN_MODE  = not(SYLO_WIN_MODEn);
  /*p24.SOCY*/ wire SOCY_WIN_MODEn = not(TOMU_WIN_MODE);
  /*p27.TUKU*/ wire TUKU_WIN_MODEn = not(TOMU_WIN_MODE);


  /*p24.VYBO*/ wire VYBO_AxCxExGx = nor(FEPO_STORE_MATCH, WODU_RENDER_DONE, MYVO_xBxDxFxH);
  /*p24.TYFA*/ wire TYFA_AxCxExGx = and(SOCY_WIN_MODEn, vid_reg.POKY_FRONT_PORCH_LATCHn, VYBO_AxCxExGx);
  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
  /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p27.ROCO*/ wire ROCO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p24.SACU*/ wire CLKPIPE_AxCxExGx = nor(SEGU_xBxDxFxH, vid_reg.FINE_MATCH_DUMP);

  /*p24.PAHO*/ lcd_reg.X_8_SYNC.set(ROXO_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, vid_reg.X3);

  {

    /*p29.TYFO*/ spr_reg.TYFO_SEQ_B0d    .set(LAPE_AxCxExGx, VYPO_P10_Bn,                  spr_reg.TOXE_SPR_SEQ0);
    /*p29.TOBU*/ spr_reg.TOBU_SEQ_xx23xx .set(TAVA_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, spr_reg.TULY_SPR_SEQ1);    // note input is seq 1 not 2
    /*p29.VONU*/ spr_reg.VONU_SEQ_xxx34xn.set(TAVA_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, spr_reg.TOBU_SEQ_xx23xx);
    /*p29.SEBA*/ spr_reg.SEBA_SEQ_xxxx45n.set(LAPE_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, spr_reg.VONU_SEQ_xxx34xn); // is this clock wrong?
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)
    /*p24.LOFU*/ wire CLK_LINE_EVEN = not(lcd_reg.RUTU_NEW_LINE_d0);
    /*p24.LUCA*/ lcd_reg.LINE_EVEN.set(CLK_LINE_EVEN, LYFE_VID_RSTn, !lcd_reg.LINE_EVEN);
    /*p21.NAPO*/ lcd_reg.FRAME_EVEN.set(lcd_reg.POPU_IN_VBLANK_d4, LYFE_VID_RSTn, !lcd_reg.FRAME_EVEN);
    /*p24.MAGU*/ wire MAGU = xor(lcd_reg.FRAME_EVEN, lcd_reg.LINE_EVEN);
    /*p24.MECO*/ wire MECO = not(MAGU);
    /*p24.KEBO*/ wire KEBO = not(MECO);
    /*p01.UREK*/ wire DIV_07n = not(tim_reg.DIV_07);
    /*p24.USEC*/ wire USEC = not(DIV_07n);
    /*p24.KEDY*/ wire LCDC_ENn = not(cfg_reg.LCDC_EN);
    /*p24.KUPA*/ wire FRn  = amux2(ROXO_AxCxExGx, KEBO, LCDC_ENn, USEC);
    /*p24.KOFO*/ wire KOFO = not(FRn);
    lcd_pins.FR.set(KOFO);
  }



  /*p21.PARU*/ wire PARU_IN_VBLANK = not(!lcd_reg.POPU_IN_VBLANK_d4);

  /*p27.REPU*/ wire REPU_IN_FRAME_Y  = nor(PARU_IN_VBLANK, PYRY_VID_RST);   // schematic wrong, this is NOR


  /*p27.XAHY*/   wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4);
  /*p27.XOFO*/ wire XOFO_WIN_RST = nand(cfg_reg.LCDC_WINEN, XAHY_VID_LINE_TRIG_d4n, XAPO_VID_RSTn);

  //----------------------------------------
  // Win match and trigger

  [this,
    ROXO_AxCxExGx,
    MOXE_AxCxExGx,
    TALU_xBCDExxx,
    SEGU_xBxDxFxH,
    ALET_xBxDxFxH,
    ROCO_AxCxExGx,
    MEHE_AxCxExGx,
    CLKPIPE_AxCxExGx,
    XAPO_VID_RSTn,
    XOFO_WIN_RST,
    TEPA_RENDERINGn,
    ROPY_RENDERINGn,
    TEVO_CLK_STOPn,
    REPU_IN_FRAME_Y
    ]
  {
    /*p27.POVA*/ wire FINE_MATCH_TRIG = and(vid_reg.FINE_MATCH_SYNC1, !vid_reg.FINE_MATCH_SYNC2);
    /*p??.ROXY*/ vid_reg.FINE_MATCH_DUMP.sr_latch(TEPA_RENDERINGn, FINE_MATCH_TRIG);

    /*p27.SUHA*/ wire FINE_MATCH0n = xor(cfg_reg.SCX0, vid_reg.FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1n = xor(cfg_reg.SCX1, vid_reg.FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2n = xor(cfg_reg.SCX2, vid_reg.FINE_CNT2);
    /*p27.RONE*/ wire FINE_MATCHn  = nand(vid_reg.FINE_MATCH_DUMP, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
    /*p27.POHU*/ wire FINE_MATCH   = not(FINE_MATCHn);

    /*p27.PUXA*/ vid_reg.FINE_MATCH_SYNC1.set(ROXO_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, FINE_MATCH);

    /*p27.NYZE*/ vid_reg.FINE_MATCH_SYNC2.set(MOXE_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, vid_reg.FINE_MATCH_SYNC1);

    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(vid_reg.FINE_CNT0, vid_reg.FINE_CNT1, vid_reg.FINE_CNT2);
    /*p27.PECU*/ wire FINE_CLK = nand(ROXO_AxCxExGx, FINE_COUNT_STOPn);
    /*p27.PASO*/ wire FINE_RST = nor(TEVO_CLK_STOPn, ROPY_RENDERINGn);
    /*p27.RYKU*/ vid_reg.FINE_CNT0.set( FINE_CLK,          FINE_RST, !vid_reg.FINE_CNT0);
    /*p27.ROGA*/ vid_reg.FINE_CNT1.set(!vid_reg.FINE_CNT0, FINE_RST, !vid_reg.FINE_CNT1);
    /*p27.RUBU*/ vid_reg.FINE_CNT2.set(!vid_reg.FINE_CNT1, FINE_RST, !vid_reg.FINE_CNT2);

    /*p21.TOBA*/ wire TOBA = and(CLKPIPE_AxCxExGx, lcd_reg.CPEN_LATCH);
    /*p21.SEMU*/ wire CPn  = or(TOBA, FINE_MATCH_TRIG);
    /*p21.RYPO*/ wire RYPO = not(CPn);
    lcd_pins.CP.set(RYPO);

    // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
    /*p27.NUKO*/ Signal WIN_MATCH;
    {
      /*p27.NAZE*/ wire WY_MATCH0 = xnor(lcd_reg.Y0, cfg_reg.WY0);
      /*p27.PEBO*/ wire WY_MATCH1 = xnor(lcd_reg.Y1, cfg_reg.WY1);
      /*p27.POMO*/ wire WY_MATCH2 = xnor(lcd_reg.Y2, cfg_reg.WY2);
      /*p27.NEVU*/ wire WY_MATCH3 = xnor(lcd_reg.Y3, cfg_reg.WY3);
      /*p27.NOJO*/ wire WY_MATCH4 = xnor(lcd_reg.Y4, cfg_reg.WY4);
      /*p27.PAGA*/ wire WY_MATCH5 = xnor(lcd_reg.Y5, cfg_reg.WY5);
      /*p27.PEZO*/ wire WY_MATCH6 = xnor(lcd_reg.Y6, cfg_reg.WY6);
      /*p27.NUPA*/ wire WY_MATCH7 = xnor(lcd_reg.Y7, cfg_reg.WY7);

      /*p27.PALO*/ wire WY_MATCH_HIn   = nand(cfg_reg.LCDC_WINEN, WY_MATCH4, WY_MATCH5, WY_MATCH6, WY_MATCH7);
      /*p27.NELE*/ wire WY_MATCH_HI    = not(WY_MATCH_HIn);
      /*p27.PAFU*/ wire WY_MATCHn      = nand(WY_MATCH_HI, WY_MATCH0, WY_MATCH1, WY_MATCH2, WY_MATCH3);
      /*p27.ROGE*/ wire WY_MATCH       = not(WY_MATCHn);
      /*p27.SARY*/ vid_reg.WY_MATCH_SYNC.set(TALU_xBCDExxx, XAPO_VID_RSTn, WY_MATCH);

      // why isn't the first input of this inverted? i'ma invert it.
      /*p27.REJO*/ vid_reg.WY_MATCH_LATCH.sr_latch(!vid_reg.WY_MATCH_SYNC, REPU_IN_FRAME_Y);

      /*p27.MYLO*/ wire WX_MATCH0 = xor(vid_reg.X0, cfg_reg.WX0);
      /*p27.PUWU*/ wire WX_MATCH1 = xor(vid_reg.X1, cfg_reg.WX1);
      /*p27.PUHO*/ wire WX_MATCH2 = xor(vid_reg.X2, cfg_reg.WX2);
      /*p27.NYTU*/ wire WX_MATCH3 = xor(vid_reg.X3, cfg_reg.WX3);
      /*p27.NEZO*/ wire WX_MATCH4 = xor(vid_reg.X4, cfg_reg.WX4);
      /*p27.NORY*/ wire WX_MATCH5 = xor(vid_reg.X5, cfg_reg.WX5);
      /*p27.NONO*/ wire WX_MATCH6 = xor(vid_reg.X6, cfg_reg.WX6);
      /*p27.PASE*/ wire WX_MATCH7 = xor(vid_reg.X7, cfg_reg.WX7);

      /*p27.PUKY*/ wire WX_MATCH_HIn = nand(vid_reg.WY_MATCH_LATCH, WX_MATCH4, WX_MATCH5, WX_MATCH6, WX_MATCH7);
      /*p27.NUFA*/ wire WX_MATCH_HI  = not (WX_MATCH_HIn);
      /*p27.NOGY*/ wire WIN_MATCHn   = nand(WX_MATCH_HI, WX_MATCH0, WX_MATCH1, WX_MATCH2, WX_MATCH3);
      /*p27.NUKO*/ WIN_MATCH = not(WIN_MATCHn);
    }

    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(WIN_MATCH, FINE_COUNT_STOPn);

    /*p27.RYFA*/ vid_reg.WIN_MATCH_ONSCREEN_SYNC1.set(SEGU_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ vid_reg.WIN_MATCH_ONSCREEN_SYNC2.set(ALET_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, vid_reg.WIN_MATCH_ONSCREEN_SYNC1);

    /*p27.PYCO*/ vid_reg.WIN_MATCH_SYNC1.set(ROCO_AxCxExGx, XAPO_VID_RSTn, WIN_MATCH);
    /*p27.NUNU*/ vid_reg.WIN_MATCH_SYNC2.set(MEHE_AxCxExGx, XAPO_VID_RSTn, vid_reg.WIN_MATCH_SYNC1);

    /*p27.PYNU*/ vid_reg.PYNU_WIN_MODE_LATCH.sr_latch(vid_reg.WIN_MATCH_SYNC2, XOFO_WIN_RST);
    /*p27.NOPA*/ vid_reg.NOPA_WIN_MODE_SYNC.set(ALET_xBxDxFxH, XAPO_VID_RSTn, vid_reg.PYNU_WIN_MODE_LATCH);
  }();

  /*p27.NOCU*/   wire NOCU_WIN_MODEn = not(vid_reg.PYNU_WIN_MODE_LATCH);
  /*p27.PORE*/ wire PORE_WIN_MODE  = not(NOCU_WIN_MODEn);

  /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIG  = and(vid_reg.PYNU_WIN_MODE_LATCH, !vid_reg.NOPA_WIN_MODE_SYNC);
  /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WIN_MODE_TRIG);
  /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIG  = not(NYFO_WIN_MODE_TRIGn);

  //----------

  /*p27.NYXU*/ wire NYXU_BG_SEQ_RSTn    = nor(AVAP_SCAN_DONE_d0_TRIG, MOSU_WIN_MODE_TRIG, TEVO_CLK_STOPn);

  // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BG_SEQ_7) is RESET.
  ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
  ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BG_SEQ_7);
  /*p27.RYDY*/ vid_reg.RYDY_WIN_MODE_LATCH.sr_latch(NUNY_WIN_MODE_TRIG, PYRY_VID_RST || vid_reg.BG_SEQ_7);


  {
    /*p27.MOCE*/ wire MOCE_SEQ_01234x6x = nand(vid_reg.BG_SEQ_x1x3x5x7,
                                            vid_reg.BG_SEQ_xxxx4567,
                                            NYXU_BG_SEQ_RSTn);
    /*p27.LYRY*/ wire LYRY_SEQ_xxxxx5x7 = not(MOCE_SEQ_01234x6x);

    {
      /*p27.LEBO*/ wire BG_SEQ_CLK = nand(ALET_xBxDxFxH, MOCE_SEQ_01234x6x);
      /*p27.LAXU*/ vid_reg.BG_SEQ_x1x3x5x7.set( BG_SEQ_CLK,              NYXU_BG_SEQ_RSTn, !vid_reg.BG_SEQ_x1x3x5x7);
      /*p27.MESU*/ vid_reg.BG_SEQ_xx23xx67.set(!vid_reg.BG_SEQ_x1x3x5x7, NYXU_BG_SEQ_RSTn, !vid_reg.BG_SEQ_xx23xx67);
      /*p27.NYVA*/ vid_reg.BG_SEQ_xxxx4567.set(!vid_reg.BG_SEQ_xx23xx67, NYXU_BG_SEQ_RSTn, !vid_reg.BG_SEQ_xxxx4567);

      /*p27.LYZU*/ vid_reg.BG_SEQ_x1x3x5x7_DELAY.set(ALET_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, vid_reg.BG_SEQ_x1x3x5x7);

      /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(MOSU_WIN_MODE_TRIG, LOBY_RENDERINGn);
      /*p24.NYKA*/ vid_reg.BG_SEQ_6.set(ALET_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, LYRY_SEQ_xxxxx5x7);
      /*p24.PORY*/ vid_reg.BG_SEQ_7.set(MYVO_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, vid_reg.BG_SEQ_6);
      /*p27.LOVY*/ vid_reg.BG_SEQ5_SYNC.set(MYVO_xBxDxFxH, NYXU_BG_SEQ_RSTn, LYRY_SEQ_xxxxx5x7);
    }

    {
      /*p27.SOWO*/ wire SOWO_SPRITE_FETCH_LATCHn = not(vid_reg._SPRITE_FETCH_LATCH);
      /*p27.TEKY*/ wire TEKY_SPRITE_FETCH = and(FEPO_STORE_MATCH, TUKU_WIN_MODEn, LYRY_SEQ_xxxxx5x7, SOWO_SPRITE_FETCH_LATCHn);
      /*p27.SOBU*/ vid_reg._SPRITE_FETCH_SYNC1.set(TAVA_xBxDxFxH, VYPO_P10_Bn, TEKY_SPRITE_FETCH);
      /*p27.SUDA*/ vid_reg._SPRITE_FETCH_SYNC2.set(LAPE_AxCxExGx, VYPO_P10_Bn, vid_reg._SPRITE_FETCH_SYNC1);
    }

  }

  {
    /*p21.WEGO*/ wire WEGO = or(TOFU_VID_RST, vid_reg.RENDER_DONE_SYNC);
    /*p21.XAJO*/ wire X_009 = and(vid_reg.X0, vid_reg.X3);
    /*p21.WUSA*/ lcd_reg.CPEN_LATCH.sr_latch(X_009, WEGO);
    /*p21.XYMU*/ vid_reg.XYMU_RENDERING_LATCH.sr_latch(AVAP_SCAN_DONE_d0_TRIG, WEGO);
  }




  //----------



  // stat read
  [this, ASOT_BUS_RD_ABCDEFGH, CUPA_BUS_WR_xxxxxFGH, FF41n, WESY_RST, PARU_IN_VBLANK, ACYL_OAM_ADDR_PARSE] {
    /*p22.VARY*/ wire FF41 = not(FF41n);
    /*p21.SEPA*/ wire FF41_WR = and(CUPA_BUS_WR_xxxxxFGH, FF41);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(FF41_WR);

    /*p21.ROXE*/ vid_reg.INT_HBL_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D0);
    /*p21.RUFO*/ vid_reg.INT_VBL_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D1);
    /*p21.REFE*/ vid_reg.INT_OAM_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D2);
    /*p21.RUGU*/ vid_reg.INT_LYC_EN.set(RYVE_FF41_WRn, WESY_RST, cpu_pins.D3);

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(vid_reg.XYMU_RENDERING_LATCH, PARU_IN_VBLANK);
    /*p21.XATY*/ wire STAT_MODE1n = nor(vid_reg.XYMU_RENDERING_LATCH, ACYL_OAM_ADDR_PARSE);
    /*p21.TOBE*/ wire FF41_RDa = and(ASOT_BUS_RD_ABCDEFGH, FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?. 

    /*p21.TEBY*/ cpu_pins.D0.set_tribuf(FF41_RDa, not(STAT_MODE0n));
    /*p21.WUGA*/ cpu_pins.D1.set_tribuf(FF41_RDa, not(STAT_MODE1n));
    /*p21.SEGO*/ cpu_pins.D2.set_tribuf(FF41_RDa, not(vid_reg.RUPO_LATCH_LYC_MATCH));
    /*p21.PUZO*/ cpu_pins.D3.set_tribuf(FF41_RDb, vid_reg.INT_HBL_EN);
    /*p21.POFO*/ cpu_pins.D4.set_tribuf(FF41_RDb, vid_reg.INT_VBL_EN);
    /*p21.SASY*/ cpu_pins.D5.set_tribuf(FF41_RDb, vid_reg.INT_OAM_EN);
    /*p21.POTE*/ cpu_pins.D6.set_tribuf(FF41_RDb, vid_reg.INT_LYC_EN);
  
    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);
    /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RST, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ vid_reg.RUPO_LATCH_LYC_MATCH.sr_latch(vid_reg.ROPO_LY_MATCH_SYNC, PAGO_LYC_MATCH_RST);
  }();

  // map x counter
  [this, TEVO_CLK_STOPn, PORE_WIN_MODE, XOFO_WIN_RST] {
    // something weird here, this doesn't look like a clock
    /*p27.VETU*/ wire VETU_X_CLK = and(TEVO_CLK_STOPn, PORE_WIN_MODE);
    /*p27.XACO*/ wire XACO_X_RSTn = not(XOFO_WIN_RST);
    /*p27.WYKA*/ vid_reg.MAP_X0.set(VETU_X_CLK,      XACO_X_RSTn, !vid_reg.MAP_X0);
    /*p27.WODY*/ vid_reg.MAP_X1.set(!vid_reg.MAP_X0, XACO_X_RSTn, !vid_reg.MAP_X1);
    /*p27.WOBO*/ vid_reg.MAP_X2.set(!vid_reg.MAP_X1, XACO_X_RSTn, !vid_reg.MAP_X2);
    /*p27.WYKO*/ vid_reg.MAP_X3.set(!vid_reg.MAP_X2, XACO_X_RSTn, !vid_reg.MAP_X3);
    /*p27.XOLO*/ vid_reg.MAP_X4.set(!vid_reg.MAP_X3, XACO_X_RSTn, !vid_reg.MAP_X4);
  }();

  // tile y + map y counter
  [this, PORE_WIN_MODE, REPU_IN_FRAME_Y] {
    /*p27.WAZY*/ wire WAZY_Y_CLK = not(PORE_WIN_MODE);
    /*p27.SYNY*/ wire SYNY_IN_FRAME_Yn = not(REPU_IN_FRAME_Y);
    /*p27.VYNO*/ vid_reg.TILE_Y0.set(WAZY_Y_CLK,       SYNY_IN_FRAME_Yn, !vid_reg.TILE_Y0);
    /*p27.VUJO*/ vid_reg.TILE_Y1.set(!vid_reg.TILE_Y0, SYNY_IN_FRAME_Yn, !vid_reg.TILE_Y1);
    /*p27.VYMU*/ vid_reg.TILE_Y2.set(!vid_reg.TILE_Y1, SYNY_IN_FRAME_Yn, !vid_reg.TILE_Y2);
    /*p27.TUFU*/ vid_reg.MAP_Y0.set(!vid_reg.TILE_Y2, SYNY_IN_FRAME_Yn, !vid_reg.MAP_Y0);
    /*p27.TAXA*/ vid_reg.MAP_Y1.set(!vid_reg.MAP_Y0,  SYNY_IN_FRAME_Yn, !vid_reg.MAP_Y1);
    /*p27.TOZO*/ vid_reg.MAP_Y2.set(!vid_reg.MAP_Y1,  SYNY_IN_FRAME_Yn, !vid_reg.MAP_Y2);
    /*p27.TATE*/ vid_reg.MAP_Y3.set(!vid_reg.MAP_Y2,  SYNY_IN_FRAME_Yn, !vid_reg.MAP_Y3);
    /*p27.TEKE*/ vid_reg.MAP_Y4.set(!vid_reg.MAP_Y3,  SYNY_IN_FRAME_Yn, !vid_reg.MAP_Y4);
  }();

  /*p21.TADY*/ wire TADY_X_RST = nor(BYHA_VID_LINE_TRIG_d4n, TOFU_VID_RST);

  // having this reset connected to both RENDER_DONE_SYNC and x seems odd
  /*p21.VOGA*/ vid_reg.RENDER_DONE_SYNC.set(ALET_xBxDxFxH, TADY_X_RST, WODU_RENDER_DONE);

  // vid x position
  [this, CLKPIPE_AxCxExGx, TADY_X_RST] {
    /*p21.RYBO*/ wire RYBO = xor(vid_reg.X0, vid_reg.X1);
    /*p21.XUKE*/ wire XUKE = and(vid_reg.X0, vid_reg.X1);

    /*p21.XYLE*/ wire XYLE = and(vid_reg.X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(vid_reg.X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(vid_reg.X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(vid_reg.X4, vid_reg.X5);
    /*p21.TYBA*/ wire TYBA = and(vid_reg.X4, vid_reg.X5);
    /*p21.SURY*/ wire SURY = and(vid_reg.X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(vid_reg.X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(vid_reg.X7, SURY);
    /*p21.TOCA*/ wire TOCA = not(vid_reg.X3);

    /*p21.XEHO*/ vid_reg.X0.set(CLKPIPE_AxCxExGx, TADY_X_RST, !vid_reg.X0);
    /*p21.SAVY*/ vid_reg.X1.set(CLKPIPE_AxCxExGx, TADY_X_RST, RYBO);
    /*p21.XODU*/ vid_reg.X2.set(CLKPIPE_AxCxExGx, TADY_X_RST, XEGY);
    /*p21.XYDO*/ vid_reg.X3.set(CLKPIPE_AxCxExGx, TADY_X_RST, XORA);
    /*p21.TUHU*/ vid_reg.X4.set(TOCA,             TADY_X_RST, !vid_reg.X4);
    /*p21.TUKY*/ vid_reg.X5.set(TOCA,             TADY_X_RST, SAKE);
    /*p21.TAKO*/ vid_reg.X6.set(TOCA,             TADY_X_RST, TYGE);
    /*p21.SYBE*/ vid_reg.X7.set(TOCA,             TADY_X_RST, ROKU);
  }();

  // ly match
  [this, TALU_xBCDExxx, WESY_RST] {
    /*p21.SYFU*/ wire LY_MATCH7 = xor(lcd_reg.Y7, cfg_reg.LYC7);
    /*p21.TERY*/ wire LY_MATCH6 = xor(lcd_reg.Y6, cfg_reg.LYC6);
    /*p21.TUCY*/ wire LY_MATCH5 = xor(lcd_reg.Y5, cfg_reg.LYC5);
    /*p21.TYKU*/ wire LY_MATCH4 = xor(lcd_reg.Y4, cfg_reg.LYC4);
    /*p21.RASY*/ wire LY_MATCH3 = xor(lcd_reg.Y3, cfg_reg.LYC3);
    /*p21.REDA*/ wire LY_MATCH2 = xor(lcd_reg.Y2, cfg_reg.LYC2);
    /*p21.TYDE*/ wire LY_MATCH1 = xor(lcd_reg.Y1, cfg_reg.LYC1);
    /*p21.RYME*/ wire LY_MATCH0 = xor(lcd_reg.Y0, cfg_reg.LYC0);
    /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
    /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);
    /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
    /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);
    /*p21.ROPO*/ vid_reg.ROPO_LY_MATCH_SYNC.set(TALU_xBCDExxx, WESY_RST, LY_MATCHa);
  }();

  /*p29.SAKY*/   wire SAKY = nor(spr_reg.TULY_SPR_SEQ1, spr_reg.VONU_SEQ_xxx34xn);
  /*p29.TYSO*/   wire TYSO_SPRITE_READn = or(SAKY, TEPA_RENDERINGn); // seems wrong
  /*p29.TEXY*/ wire TEXY_SPRITE_READ = not(TYSO_SPRITE_READn);
  /*p29.ABON*/ wire SPRITE_READn = not(TEXY_SPRITE_READ);

  /*p27.LURY*/ wire LURY = and(!vid_reg.BG_SEQ5_SYNC, vid_reg.XYMU_RENDERING_LATCH);
  /*p27.LONY*/ vid_reg.BG_READ_VRAM_LATCHn.sr_latch(LURY, NYXU_BG_SEQ_RSTn); // polarity? wait, are we sure this was a latch?
  /*p27.LUSU*/ wire BG_READ_VRAMn = not(vid_reg.BG_READ_VRAM_LATCHn);
  /*p27.LENA*/ wire BGW_READ_VRAM  = not(BG_READ_VRAMn);

  //----------
  // PPU VRAM read
  {
    /*p26.FAFO*/ wire TILE_Y0S  = add_s(lcd_reg.Y0, cfg_reg.SCY0, 0);
    /*p26.FAFO*/ wire TILE_Y0C  = add_c(lcd_reg.Y0, cfg_reg.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1S  = add_s(lcd_reg.Y1, cfg_reg.SCY1, TILE_Y0C);
    /*p26.EMUX*/ wire TILE_Y1C  = add_c(lcd_reg.Y1, cfg_reg.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2S  = add_s(lcd_reg.Y2, cfg_reg.SCY2, TILE_Y1C);
    /*p26.ECAB*/ wire TILE_Y2C  = add_c(lcd_reg.Y2, cfg_reg.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0S   = add_s(lcd_reg.Y3, cfg_reg.SCY3, TILE_Y2C);
    /*p26.ETAM*/ wire MAP_Y0C   = add_c(lcd_reg.Y3, cfg_reg.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1S   = add_s(lcd_reg.Y4, cfg_reg.SCY4, MAP_Y0C);
    /*p26.DOTO*/ wire MAP_Y1C   = add_c(lcd_reg.Y4, cfg_reg.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2S   = add_s(lcd_reg.Y5, cfg_reg.SCY5, MAP_Y1C);
    /*p26.DABA*/ wire MAP_Y2C   = add_c(lcd_reg.Y5, cfg_reg.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3S   = add_s(lcd_reg.Y6, cfg_reg.SCY6, MAP_Y2C);
    /*p26.EFYK*/ wire MAP_Y3C   = add_c(lcd_reg.Y6, cfg_reg.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4S   = add_s(lcd_reg.Y7, cfg_reg.SCY7, MAP_Y3C);
    /*p26.EJOK*/ wire MAP_Y4C   = add_c(lcd_reg.Y7, cfg_reg.SCY7, MAP_Y3C);

    /*p26.ATAD*/ wire TILE_X0S  = add_s(vid_reg.X0, cfg_reg.SCX0, 0); // unused
    /*p26.ATAD*/ wire TILE_X0C  = add_c(vid_reg.X0, cfg_reg.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1S  = add_s(vid_reg.X1, cfg_reg.SCX1, TILE_X0C); // unused
    /*p26.BEHU*/ wire TILE_X1C  = add_c(vid_reg.X1, cfg_reg.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2S  = add_s(vid_reg.X2, cfg_reg.SCX2, TILE_X1C); // unused
    /*p26.APYH*/ wire TILE_X2C  = add_c(vid_reg.X2, cfg_reg.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0S   = add_s(vid_reg.X3, cfg_reg.SCX3, TILE_X2C);
    /*p26.BABE*/ wire MAP_X0C   = add_c(vid_reg.X3, cfg_reg.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1S   = add_s(vid_reg.X4, cfg_reg.SCX4, MAP_X0C);
    /*p26.ABOD*/ wire MAP_X1C   = add_c(vid_reg.X4, cfg_reg.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2S   = add_s(vid_reg.X5, cfg_reg.SCX5, MAP_X1C);
    /*p26.BEWY*/ wire MAP_X2C   = add_c(vid_reg.X5, cfg_reg.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3S   = add_s(vid_reg.X6, cfg_reg.SCX6, MAP_X2C);
    /*p26.BYCA*/ wire MAP_X3C   = add_c(vid_reg.X6, cfg_reg.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4S   = add_s(vid_reg.X7, cfg_reg.SCX7, MAP_X3C);
    /*p26.ACUL*/ wire MAP_X4C   = add_c(vid_reg.X7, cfg_reg.SCX7, MAP_X3C);

    // DMA vram read address gen
    {
      /*p04.AHOC*/ wire DMA_READ_VRAMo = not(DMA_READ_VRAM);
      /*p04.ECAL*/ vid_reg.MA00.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A00);
      /*p04.EGEZ*/ vid_reg.MA01.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A01);
      /*p04.FUHE*/ vid_reg.MA02.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A02);
      /*p04.FYZY*/ vid_reg.MA03.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A03);
      /*p04.DAMU*/ vid_reg.MA04.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A04);
      /*p04.DAVA*/ vid_reg.MA05.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A05);
      /*p04.ETEG*/ vid_reg.MA06.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A06);
      /*p04.EREW*/ vid_reg.MA07.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A07);
      /*p04.EVAX*/ vid_reg.MA08.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A08);
      /*p04.DUVE*/ vid_reg.MA09.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A09);
      /*p04.ERAF*/ vid_reg.MA10.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A10);
      /*p04.FUSY*/ vid_reg.MA11.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A11);
      /*p04.EXYF*/ vid_reg.MA12.set_tribuf(!DMA_READ_VRAMo, dma_reg.DMA_A12);
    }

    // CPU vram read address gen
    {
      /*p25.XANE*/ wire VRAM_LOCKn = nor(DMA_READ_VRAM, vid_reg.XYMU_RENDERING_LATCH);
      /*p25.XEDU*/ wire VRAM_LOCK = not(VRAM_LOCKn);

      /*p25.XAKY*/ vid_reg.MA00.set_tribuf(!VRAM_LOCK, cpu_pins.A00);
      /*p25.XUXU*/ vid_reg.MA01.set_tribuf(!VRAM_LOCK, cpu_pins.A01);
      /*p25.XYNE*/ vid_reg.MA02.set_tribuf(!VRAM_LOCK, cpu_pins.A02);
      /*p25.XODY*/ vid_reg.MA03.set_tribuf(!VRAM_LOCK, cpu_pins.A03);
      /*p25.XECA*/ vid_reg.MA04.set_tribuf(!VRAM_LOCK, cpu_pins.A04);
      /*p25.XOBA*/ vid_reg.MA05.set_tribuf(!VRAM_LOCK, cpu_pins.A05);
      /*p25.XOPO*/ vid_reg.MA06.set_tribuf(!VRAM_LOCK, cpu_pins.A06);
      /*p25.XYBO*/ vid_reg.MA07.set_tribuf(!VRAM_LOCK, cpu_pins.A07);
      /*p25.RYSU*/ vid_reg.MA08.set_tribuf(!VRAM_LOCK, cpu_pins.A08);
      /*p25.RESE*/ vid_reg.MA09.set_tribuf(!VRAM_LOCK, cpu_pins.A09);
      /*p25.RUSE*/ vid_reg.MA10.set_tribuf(!VRAM_LOCK, cpu_pins.A10);
      /*p25.RYNA*/ vid_reg.MA11.set_tribuf(!VRAM_LOCK, cpu_pins.A11);
      /*p25.RUMO*/ vid_reg.MA12.set_tribuf(!VRAM_LOCK, cpu_pins.A12);
    }

    {
      /*p27.NOFU*/ wire BG_SEQ_0123xxxx = not(vid_reg.BG_SEQ_xxxx4567);
      /*p27.NAKO*/ wire BG_SEQ_01xx45xx = not(vid_reg.BG_SEQ_xx23xx67);
      /*p27.NOGU*/ wire BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);
      /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(BG_SEQ_xx234567);
      /*p26.AXAD*/ bool AXAD_WIN_MODEn  = not(PORE_WIN_MODE);

      // Map reads happen during sequencer step 01.
      {
        /*p27.POTU*/ wire BGW_READ_01xxxxxx = and(BGW_READ_VRAM, BG_SEQ_01xxxxxx);
        {
          /*p26.ACEN*/ wire ACEN  = and(BGW_READ_01xxxxxx, AXAD_WIN_MODEn);
          /*p26.BAFY*/ wire BAFY  = not(ACEN);
          /*p26.AXEP*/ vid_reg.MA00.set_tribuf(!BAFY, MAP_X0S);
          /*p26.AFEB*/ vid_reg.MA01.set_tribuf(!BAFY, MAP_X1S);
          /*p26.ALEL*/ vid_reg.MA02.set_tribuf(!BAFY, MAP_X2S);
          /*p26.COLY*/ vid_reg.MA03.set_tribuf(!BAFY, MAP_X3S);
          /*p26.AJAN*/ vid_reg.MA04.set_tribuf(!BAFY, MAP_X4S);
          /*p26.DUHO*/ vid_reg.MA05.set_tribuf(!BAFY, MAP_Y0S);
          /*p26.CASE*/ vid_reg.MA06.set_tribuf(!BAFY, MAP_Y1S);
          /*p26.CYPO*/ vid_reg.MA07.set_tribuf(!BAFY, MAP_Y2S);
          /*p26.CETA*/ vid_reg.MA08.set_tribuf(!BAFY, MAP_Y3S);
          /*p26.DAFE*/ vid_reg.MA09.set_tribuf(!BAFY, MAP_Y4S);
          /*p26.AMUV*/ vid_reg.MA10.set_tribuf(!BAFY, cfg_reg.LCDC_BGMAP);
          /*p26.COVE*/ vid_reg.MA11.set_tribuf(!BAFY, VYPO_P10_Bn);
          /*p26.COXO*/ vid_reg.MA12.set_tribuf(!BAFY, VYPO_P10_Bn);
        }

        // Window vram map read address gen
        {
          /*p25.XEZE*/ wire XEZE_WIN_MAP_READn = nand(BGW_READ_01xxxxxx, PORE_WIN_MODE);
          /*p25.WUKO*/ wire WIN_MAP_READ = not(XEZE_WIN_MAP_READn);
          /*p27.XEJA*/ vid_reg.MA00.set_tribuf(WIN_MAP_READ, vid_reg.MAP_X0);
          /*p27.XAMO*/ vid_reg.MA01.set_tribuf(WIN_MAP_READ, vid_reg.MAP_X1);
          /*p27.XAHE*/ vid_reg.MA02.set_tribuf(WIN_MAP_READ, vid_reg.MAP_X2);
          /*p27.XULO*/ vid_reg.MA03.set_tribuf(WIN_MAP_READ, vid_reg.MAP_X3);
          /*p27.WUJU*/ vid_reg.MA04.set_tribuf(WIN_MAP_READ, vid_reg.MAP_X4);
          /*p27.VYTO*/ vid_reg.MA05.set_tribuf(WIN_MAP_READ, vid_reg.MAP_Y0);
          /*p27.VEHA*/ vid_reg.MA06.set_tribuf(WIN_MAP_READ, vid_reg.MAP_Y1);
          /*p27.VACE*/ vid_reg.MA07.set_tribuf(WIN_MAP_READ, vid_reg.MAP_Y2);
          /*p27.VOVO*/ vid_reg.MA08.set_tribuf(WIN_MAP_READ, vid_reg.MAP_Y3);
          /*p27.VULO*/ vid_reg.MA09.set_tribuf(WIN_MAP_READ, vid_reg.MAP_Y4);
          /*p27.VEVY*/ vid_reg.MA10.set_tribuf(WIN_MAP_READ, cfg_reg.LCDC_WINMAP);
          /*p27.VEZA*/ vid_reg.MA11.set_tribuf(WIN_MAP_READ, VYPO_P10_Bn);
          /*p27.VOGU*/ vid_reg.MA12.set_tribuf(WIN_MAP_READ, VYPO_P10_Bn);
        }
      }


      // Tile reads happen during sequencer steps 23 and 45.
      /*p27.NETA*/ bool NETA_TILE_READ    = and(BGW_READ_VRAM, BG_SEQ_xx234567);
      /*p27.XUHA*/ wire BG_SEQ_xxxx4567 = not(BG_SEQ_0123xxxx);


      // Background vram tile read address gen (low)
      {
        /*p26.ASUL*/ bool ASUL_TILE_READ = and (NETA_TILE_READ, AXAD_WIN_MODEn);
        /*p26.BEJE*/ bool BEJE_BG_TILE_READn = not(ASUL_TILE_READ);
        /*p26.ASUM*/ vid_reg.MA00.set_tribuf(!BEJE_BG_TILE_READn, BG_SEQ_xxxx4567);
        /*p26.EVAD*/ vid_reg.MA01.set_tribuf(!BEJE_BG_TILE_READn, TILE_Y0S);
        /*p26.DAHU*/ vid_reg.MA02.set_tribuf(!BEJE_BG_TILE_READn, TILE_Y1S);
        /*p26.DODE*/ vid_reg.MA03.set_tribuf(!BEJE_BG_TILE_READn, TILE_Y2S);
      }

      // Window vram tile read address gen (low)
      {
        /*p25.XUCY*/ wire XUCY_WIN_TILE_READn = nand(NETA_TILE_READ, PORE_WIN_MODE);
        /*p25.XONU*/ vid_reg.MA00.set_tribuf(!XUCY_WIN_TILE_READn, BG_SEQ_xxxx4567);
        /*p25.WUDO*/ vid_reg.MA01.set_tribuf(!XUCY_WIN_TILE_READn, vid_reg.TILE_Y0);
        /*p25.WAWE*/ vid_reg.MA02.set_tribuf(!XUCY_WIN_TILE_READn, vid_reg.TILE_Y1);
        /*p25.WOLU*/ vid_reg.MA03.set_tribuf(!XUCY_WIN_TILE_READn, vid_reg.TILE_Y2);
      }

      // Background/window tile read address gen (high)
      {
        /*p25.VUZA*/ bool TILE_BANK = nor(cfg_reg.LCDC_BGTILE, pxp_reg.BG_PIX_B7); // register reused
        /*p25.VAPY*/ vid_reg.MA04.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B0); // register reused
        /*p25.SEZU*/ vid_reg.MA05.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B1);
        /*p25.VEJY*/ vid_reg.MA06.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B2);
        /*p25.RUSA*/ vid_reg.MA07.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B3);
        /*p25.ROHA*/ vid_reg.MA08.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B4);
        /*p25.RESO*/ vid_reg.MA09.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B5);
        /*p25.SUVO*/ vid_reg.MA10.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B6);
        /*p25.TOBO*/ vid_reg.MA11.set_tribuf(NETA_TILE_READ, pxp_reg.BG_PIX_B7);
        /*????*/     vid_reg.MA12.set_tribuf(NETA_TILE_READ, TILE_BANK);
      }
    }

    // Sprite tile read vram address gen
    {
      /*p29.WUKY*/ wire FLIP_Y = not(oam_reg.REG_OAM_A6);
      /*p29.XUQU*/ wire SPRITE_AB = not(!spr_reg.VONU_SEQ_xxx34xn);
      /*p29.WAGO*/ wire WAGO      = xor(FLIP_Y, spr_reg.TS_LINE_0);
      /*p29.CYVU*/ wire SPRITE_Y0 = xor(FLIP_Y, spr_reg.TS_LINE_1);
      /*p29.BORE*/ wire SPRITE_Y1 = xor(FLIP_Y, spr_reg.TS_LINE_2);
      /*p29.BUVY*/ wire SPRITE_Y2 = xor(FLIP_Y, spr_reg.TS_LINE_3);
      /*p29.FUFO*/ wire LCDC_SPSIZEn = not(cfg_reg.LCDC_SPSIZE);
      /*p29.GEJY*/ wire SPRITE_Y3 = amux2(LCDC_SPSIZEn, !oam_reg.REG_OAM_B0, cfg_reg.LCDC_SPSIZE,  WAGO);

      /*p29.ABEM*/ vid_reg.MA00.set_tribuf(!SPRITE_READn, SPRITE_AB);
      /*p29.BAXE*/ vid_reg.MA01.set_tribuf(!SPRITE_READn, SPRITE_Y0);
      /*p29.ARAS*/ vid_reg.MA02.set_tribuf(!SPRITE_READn, SPRITE_Y1);
      /*p29.AGAG*/ vid_reg.MA03.set_tribuf(!SPRITE_READn, SPRITE_Y2);
      /*p29.FAMU*/ vid_reg.MA04.set_tribuf(!SPRITE_READn, SPRITE_Y3);
      /*p29.FUGY*/ vid_reg.MA05.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B1);
      /*p29.GAVO*/ vid_reg.MA06.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B2);
      /*p29.WYGA*/ vid_reg.MA07.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B3);
      /*p29.WUNE*/ vid_reg.MA08.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B4);
      /*p29.GOTU*/ vid_reg.MA09.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B5);
      /*p29.GEGU*/ vid_reg.MA10.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B6);
      /*p29.XEHE*/ vid_reg.MA11.set_tribuf(!SPRITE_READn, oam_reg.REG_OAM_B7);
      /*p29.DYSO*/ vid_reg.MA12.set_tribuf(!SPRITE_READn, joy_pins.P10_B);   // sprites always in low half of tile store
    }

    {
      /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(vid_reg.BG_SEQ_xxxx4567);
      /*p27.LAXE*/ wire BG_SEQ_0x2x4x6x  = not(vid_reg.BG_SEQ_x1x3x5x7);
      /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(vid_reg.BG_SEQ_xx23xx67);
      /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(LOBY_RENDERINGn, BG_SEQ_0x2x4x6x, vid_reg.BG_SEQ_x1x3x5x7_DELAY);
      /*p27.NYDY*/ wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, vid_reg.BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
      /*p32.METE*/ wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
      /*p27.MOFU*/ wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);

      // Background/window tile read A handler
      {
        /*p32.LOMA*/ wire BG_LATCH = not(BG_SEQ_TRIG_3);
        /*p32.LEGU*/ pxp_reg.BG_PIX_A0.tp_latch(BG_LATCH, vid_reg.MD0);
        /*p32.NUDU*/ pxp_reg.BG_PIX_A1.tp_latch(BG_LATCH, vid_reg.MD1);
        /*p32.MUKU*/ pxp_reg.BG_PIX_A2.tp_latch(BG_LATCH, vid_reg.MD2);
        /*p32.LUZO*/ pxp_reg.BG_PIX_A3.tp_latch(BG_LATCH, vid_reg.MD3);
        /*p32.MEGU*/ pxp_reg.BG_PIX_A4.tp_latch(BG_LATCH, vid_reg.MD4);
        /*p32.MYJY*/ pxp_reg.BG_PIX_A5.tp_latch(BG_LATCH, vid_reg.MD5);
        /*p32.NASA*/ pxp_reg.BG_PIX_A6.tp_latch(BG_LATCH, vid_reg.MD6);
        /*p32.NEFO*/ pxp_reg.BG_PIX_A7.tp_latch(BG_LATCH, vid_reg.MD7);
      }

      // Background/window tile read B handler
      {
        /*p32.LESO*/ wire LESO = not(MOFU);
        /*p32.AJAR*/ wire AJAR = not(LESO);
        /*p32.LABU*/ wire VRAM_TEMP_CLK = not(AJAR);
        /*p32.RAWU*/ pxp_reg.BG_PIX_B0.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD0);
        /*p32.POZO*/ pxp_reg.BG_PIX_B1.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD1);
        /*p32.PYZO*/ pxp_reg.BG_PIX_B2.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD2);
        /*p32.POXA*/ pxp_reg.BG_PIX_B3.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD3);
        /*p32.PULO*/ pxp_reg.BG_PIX_B4.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD4);
        /*p32.POJU*/ pxp_reg.BG_PIX_B5.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD5);
        /*p32.POWY*/ pxp_reg.BG_PIX_B6.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD6);
        /*p32.PYJU*/ pxp_reg.BG_PIX_B7.set(VRAM_TEMP_CLK, VYPO_P10_Bn, vid_reg.MD7);
      }
    }

    // Sprite tile read handler

    {
      /*p29.TYTU*/ wire SPR_SEQ0n = not(spr_reg.TOXE_SPR_SEQ0);
      /*p29.SYCU*/ wire SYCU = nor(SPR_SEQ0n, LOBY_RENDERINGn, spr_reg.TYFO_SEQ_B0d);
      /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(spr_reg.TULY_SPR_SEQ1, SYCU);
      /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
      /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
      /*p29.XADO*/ wire SPRITE_PIX_LATCH_A  = not(SPRITE_PIX_LATCH_An);

      /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(spr_reg.VONU_SEQ_xxx34xn, SYCU);
      /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
      /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
      /*p29.PUCO*/ wire SPRITE_PIX_LATCH_B  = not(SPRITE_PIX_LATCH_Bn);

      /*p29.XONO*/ wire XONO_FLIP_X = and(oam_reg.REG_OAM_A5, TEXY_SPRITE_READ);
      /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(vid_reg.MD7, vid_reg.MD0, XONO_FLIP_X);
      /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(vid_reg.MD6, vid_reg.MD1, XONO_FLIP_X);
      /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(vid_reg.MD5, vid_reg.MD2, XONO_FLIP_X);
      /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(vid_reg.MD4, vid_reg.MD3, XONO_FLIP_X);
      /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(vid_reg.MD3, vid_reg.MD4, XONO_FLIP_X);
      /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(vid_reg.MD2, vid_reg.MD5, XONO_FLIP_X);
      /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(vid_reg.MD1, vid_reg.MD6, XONO_FLIP_X);
      /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(vid_reg.MD0, vid_reg.MD7, XONO_FLIP_X);

      /*p33.PEFO*/ pxp_reg.SPR_PIX_A0.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP0);
      /*p33.ROKA*/ pxp_reg.SPR_PIX_A1.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP1);
      /*p33.MYTU*/ pxp_reg.SPR_PIX_A2.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP2);
      /*p33.RAMU*/ pxp_reg.SPR_PIX_A3.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP3);
      /*p33.SELE*/ pxp_reg.SPR_PIX_A4.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP4);
      /*p33.SUTO*/ pxp_reg.SPR_PIX_A5.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP5);
      /*p33.RAMA*/ pxp_reg.SPR_PIX_A6.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP6);
      /*p33.RYDU*/ pxp_reg.SPR_PIX_A7.tp_latch(SPRITE_PIX_LATCH_A, SPR_PIX_FLIP7);

      /*p33.REWO*/ pxp_reg.SPR_PIX_B0.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP0);
      /*p33.PEBA*/ pxp_reg.SPR_PIX_B1.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP1);
      /*p33.MOFO*/ pxp_reg.SPR_PIX_B2.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP2);
      /*p33.PUDU*/ pxp_reg.SPR_PIX_B3.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP3);
      /*p33.SAJA*/ pxp_reg.SPR_PIX_B4.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP4);
      /*p33.SUNY*/ pxp_reg.SPR_PIX_B5.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP5);
      /*p33.SEMO*/ pxp_reg.SPR_PIX_B6.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP6);
      /*p33.SEGA*/ pxp_reg.SPR_PIX_B7.tp_latch(SPRITE_PIX_LATCH_B, SPR_PIX_FLIP7);
    }
  }

  //----------
  // Pixel pipe

  if (1) {
    /*p34.MEFU*/ wire SPRITE_MASK0 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A0, pxp_reg.SPR_PIPE_B0);
    /*p34.MEVE*/ wire SPRITE_MASK1 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A1, pxp_reg.SPR_PIPE_B1);
    /*p34.MYZO*/ wire SPRITE_MASK2 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A2, pxp_reg.SPR_PIPE_B2);
    /*p34.RUDA*/ wire SPRITE_MASK3 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A3, pxp_reg.SPR_PIPE_B3);
    /*p34.VOTO*/ wire SPRITE_MASK4 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A4, pxp_reg.SPR_PIPE_B4);
    /*p34.VYSA*/ wire SPRITE_MASK5 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A5, pxp_reg.SPR_PIPE_B5);
    /*p34.TORY*/ wire SPRITE_MASK6 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A6, pxp_reg.SPR_PIPE_B6);
    /*p34.WOPE*/ wire SPRITE_MASK7 = or(XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A7, pxp_reg.SPR_PIPE_B7);

    /*p34.LESY*/ wire SPRITE_MASK0n = not(SPRITE_MASK0);
    /*p34.LOTA*/ wire SPRITE_MASK1n = not(SPRITE_MASK1);
    /*p34.LYKU*/ wire SPRITE_MASK2n = not(SPRITE_MASK2);
    /*p34.ROBY*/ wire SPRITE_MASK3n = not(SPRITE_MASK3);
    /*p34.TYTA*/ wire SPRITE_MASK4n = not(SPRITE_MASK4);
    /*p34.TYCO*/ wire SPRITE_MASK5n = not(SPRITE_MASK5);
    /*p34.SOKA*/ wire SPRITE_MASK6n = not(SPRITE_MASK6);
    /*p34.XOVU*/ wire SPRITE_MASK7n = not(SPRITE_MASK7);

    // Background pipe A
    {
      /*p32.LOZE*/ wire LOZE_PIPE_A_LOAD = not(NYXU_BG_SEQ_RSTn);
      /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A0);
      /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A1);
      /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A2);
      /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A3);
      /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A4);
      /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A5);
      /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A6);
      /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A7);

      /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(LOZE_PIPE_A_LOAD, /*p32.LUHE*/ not(pxp_reg.BG_PIX_A0));
      /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(LOZE_PIPE_A_LOAD, /*p32.NOLY*/ not(pxp_reg.BG_PIX_A1));
      /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(LOZE_PIPE_A_LOAD, /*p32.LEKE*/ not(pxp_reg.BG_PIX_A2));
      /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(LOZE_PIPE_A_LOAD, /*p32.LOMY*/ not(pxp_reg.BG_PIX_A3));
      /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(LOZE_PIPE_A_LOAD, /*p32.LALA*/ not(pxp_reg.BG_PIX_A4));
      /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(LOZE_PIPE_A_LOAD, /*p32.LOXA*/ not(pxp_reg.BG_PIX_A5));
      /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(LOZE_PIPE_A_LOAD, /*p32.NEZE*/ not(pxp_reg.BG_PIX_A6));
      /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(LOZE_PIPE_A_LOAD, /*p32.NOBO*/ not(pxp_reg.BG_PIX_A7));

      /*p32.MYDE*/ pxp_reg.BG_PIPE_A0.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET0, BG_PIPE_A_RST0, joy_pins.P10_B);
      /*p32.NOZO*/ pxp_reg.BG_PIPE_A1.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET1, BG_PIPE_A_RST1, pxp_reg.BG_PIPE_A0);
      /*p32.MOJU*/ pxp_reg.BG_PIPE_A2.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET2, BG_PIPE_A_RST2, pxp_reg.BG_PIPE_A1);
      /*p32.MACU*/ pxp_reg.BG_PIPE_A3.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET3, BG_PIPE_A_RST3, pxp_reg.BG_PIPE_A2);
      /*p32.NEPO*/ pxp_reg.BG_PIPE_A4.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET4, BG_PIPE_A_RST4, pxp_reg.BG_PIPE_A3);
      /*p32.MODU*/ pxp_reg.BG_PIPE_A5.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET5, BG_PIPE_A_RST5, pxp_reg.BG_PIPE_A4);
      /*p32.NEDA*/ pxp_reg.BG_PIPE_A6.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET6, BG_PIPE_A_RST6, pxp_reg.BG_PIPE_A5);
      /*p32.PYBO*/ pxp_reg.BG_PIPE_A7.set(CLKPIPE_AxCxExGx, BG_PIPE_A_SET7, BG_PIPE_A_RST7, pxp_reg.BG_PIPE_A6);
    }

    // Background pipe B
    {
      /*p32.LUXA*/ wire LUXA_PIPE_B_LOAD = not(NYXU_BG_SEQ_RSTn);
      /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B0);
      /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B1);
      /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B2);
      /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B3);
      /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B4);
      /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B5);
      /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B6);
      /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B7);

      /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(LUXA_PIPE_B_LOAD, /*p32.TOSA*/ not(!pxp_reg.BG_PIX_B0));
      /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(LUXA_PIPE_B_LOAD, /*p32.RUCO*/ not(!pxp_reg.BG_PIX_B1));
      /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(LUXA_PIPE_B_LOAD, /*p32.TYCE*/ not(!pxp_reg.BG_PIX_B2));
      /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(LUXA_PIPE_B_LOAD, /*p32.REVY*/ not(!pxp_reg.BG_PIX_B3));
      /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(LUXA_PIPE_B_LOAD, /*p32.RYGA*/ not(!pxp_reg.BG_PIX_B4));
      /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(LUXA_PIPE_B_LOAD, /*p32.RYLE*/ not(!pxp_reg.BG_PIX_B5));
      /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(LUXA_PIPE_B_LOAD, /*p32.RAPU*/ not(!pxp_reg.BG_PIX_B6));
      /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(LUXA_PIPE_B_LOAD, /*p32.SOJA*/ not(!pxp_reg.BG_PIX_B7));

      /*p32.TOMY*/ pxp_reg.BG_PIPE_B0.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET0, BG_PIPE_B_RST0, joy_pins.P10_B);
      /*p32.TACA*/ pxp_reg.BG_PIPE_B1.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET1, BG_PIPE_B_RST1, pxp_reg.BG_PIPE_B0);
      /*p32.SADY*/ pxp_reg.BG_PIPE_B2.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET2, BG_PIPE_B_RST2, pxp_reg.BG_PIPE_B1);
      /*p32.RYSA*/ pxp_reg.BG_PIPE_B3.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET3, BG_PIPE_B_RST3, pxp_reg.BG_PIPE_B2);
      /*p32.SOBO*/ pxp_reg.BG_PIPE_B4.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET4, BG_PIPE_B_RST4, pxp_reg.BG_PIPE_B3);
      /*p32.SETU*/ pxp_reg.BG_PIPE_B5.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET5, BG_PIPE_B_RST5, pxp_reg.BG_PIPE_B4);
      /*p32.RALU*/ pxp_reg.BG_PIPE_B6.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET6, BG_PIPE_B_RST6, pxp_reg.BG_PIPE_B5);
      /*p32.SOHU*/ pxp_reg.BG_PIPE_B7.set(CLKPIPE_AxCxExGx, BG_PIPE_B_SET7, BG_PIPE_B_RST7, pxp_reg.BG_PIPE_B6);
    }

    // Sprite pipe A
    {
      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(SPRITE_MASK0n, pxp_reg.SPR_PIX_A0);
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(SPRITE_MASK1n, pxp_reg.SPR_PIX_A1);
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(SPRITE_MASK2n, pxp_reg.SPR_PIX_A2);
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(SPRITE_MASK3n, pxp_reg.SPR_PIX_A3);
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(SPRITE_MASK4n, pxp_reg.SPR_PIX_A4);
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(SPRITE_MASK5n, pxp_reg.SPR_PIX_A5);
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(SPRITE_MASK6n, pxp_reg.SPR_PIX_A6);
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(SPRITE_MASK7n, pxp_reg.SPR_PIX_A7);

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(SPRITE_MASK0n, /*p33.LOZA*/ not(pxp_reg.SPR_PIX_A0));
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(SPRITE_MASK1n, /*p33.SYBO*/ not(pxp_reg.SPR_PIX_A1));
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(SPRITE_MASK2n, /*p33.LUMO*/ not(pxp_reg.SPR_PIX_A2));
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(SPRITE_MASK3n, /*p33.SOLO*/ not(pxp_reg.SPR_PIX_A3));
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(SPRITE_MASK4n, /*p33.VOBY*/ not(pxp_reg.SPR_PIX_A4));
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(SPRITE_MASK5n, /*p33.WYCO*/ not(pxp_reg.SPR_PIX_A5));
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(SPRITE_MASK6n, /*p33.SERY*/ not(pxp_reg.SPR_PIX_A6));
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(SPRITE_MASK7n, /*p33.SELU*/ not(pxp_reg.SPR_PIX_A7));

      /*p33.NYLU*/ pxp_reg.SPR_PIPE_A0.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET0, SPR_PIX_A_RST0, joy_pins.P10_B);
      /*p33.PEFU*/ pxp_reg.SPR_PIPE_A1.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET1, SPR_PIX_A_RST1, pxp_reg.SPR_PIPE_A0);
      /*p33.NATY*/ pxp_reg.SPR_PIPE_A2.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET2, SPR_PIX_A_RST2, pxp_reg.SPR_PIPE_A1);
      /*p33.PYJO*/ pxp_reg.SPR_PIPE_A3.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET3, SPR_PIX_A_RST3, pxp_reg.SPR_PIPE_A2);
      /*p33.VARE*/ pxp_reg.SPR_PIPE_A4.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET4, SPR_PIX_A_RST4, pxp_reg.SPR_PIPE_A3);
      /*p33.WEBA*/ pxp_reg.SPR_PIPE_A5.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET5, SPR_PIX_A_RST5, pxp_reg.SPR_PIPE_A4);
      /*p33.VANU*/ pxp_reg.SPR_PIPE_A6.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET6, SPR_PIX_A_RST6, pxp_reg.SPR_PIPE_A5);
      /*p33.VUPY*/ pxp_reg.SPR_PIPE_A7.set(CLKPIPE_AxCxExGx, SPR_PIX_A_SET7, SPR_PIX_A_RST7, pxp_reg.SPR_PIPE_A6);
    }

    // Sprite pipe B
    {
      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(SPRITE_MASK0n, pxp_reg.SPR_PIX_B0);
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(SPRITE_MASK1n, pxp_reg.SPR_PIX_B1);
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(SPRITE_MASK2n, pxp_reg.SPR_PIX_B2);
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(SPRITE_MASK3n, pxp_reg.SPR_PIX_B3);
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(SPRITE_MASK4n, pxp_reg.SPR_PIX_B4);
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(SPRITE_MASK5n, pxp_reg.SPR_PIX_B5);
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(SPRITE_MASK6n, pxp_reg.SPR_PIX_B6);
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(SPRITE_MASK7n, pxp_reg.SPR_PIX_B7);

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(SPRITE_MASK0n, /*p33.RATA*/ not(pxp_reg.SPR_PIX_B0));
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(SPRITE_MASK1n, /*p33.NUCA*/ not(pxp_reg.SPR_PIX_B1));
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(SPRITE_MASK2n, /*p33.LASE*/ not(pxp_reg.SPR_PIX_B2));
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(SPRITE_MASK3n, /*p33.LUBO*/ not(pxp_reg.SPR_PIX_B3));
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(SPRITE_MASK4n, /*p33.WERY*/ not(pxp_reg.SPR_PIX_B4));
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(SPRITE_MASK5n, /*p33.WURA*/ not(pxp_reg.SPR_PIX_B5));
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(SPRITE_MASK6n, /*p33.SULU*/ not(pxp_reg.SPR_PIX_B6));
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(SPRITE_MASK7n, /*p33.WAMY*/ not(pxp_reg.SPR_PIX_B7));

      /*p33.NURO*/ pxp_reg.SPR_PIPE_B0.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET0, SPR_PIX_B_RST0, joy_pins.P10_B);
      /*p33.MASO*/ pxp_reg.SPR_PIPE_B1.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET1, SPR_PIX_B_RST1, pxp_reg.SPR_PIPE_B0);
      /*p33.LEFE*/ pxp_reg.SPR_PIPE_B2.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET2, SPR_PIX_B_RST2, pxp_reg.SPR_PIPE_B1);
      /*p33.LESU*/ pxp_reg.SPR_PIPE_B3.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET3, SPR_PIX_B_RST3, pxp_reg.SPR_PIPE_B2);
      /*p33.WYHO*/ pxp_reg.SPR_PIPE_B4.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET4, SPR_PIX_B_RST4, pxp_reg.SPR_PIPE_B3);
      /*p33.WORA*/ pxp_reg.SPR_PIPE_B5.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET5, SPR_PIX_B_RST5, pxp_reg.SPR_PIPE_B4);
      /*p33.VAFO*/ pxp_reg.SPR_PIPE_B6.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET6, SPR_PIX_B_RST6, pxp_reg.SPR_PIPE_B5);
      /*p33.WUFY*/ pxp_reg.SPR_PIPE_B7.set(CLKPIPE_AxCxExGx, SPR_PIX_B_SET7, SPR_PIX_B_RST7, pxp_reg.SPR_PIPE_B6);
    }

    // Palette pipe
    {
      /*p34.PUME*/ wire SPRITE_PAL_PIPE_SET0n = nand(SPRITE_MASK0n, oam_reg.REG_OAM_A4);
      /*p34.SORO*/ wire SPRITE_PAL_PIPE_SET1n = nand(SPRITE_MASK1n, oam_reg.REG_OAM_A4);
      /*p34.PAMO*/ wire SPRITE_PAL_PIPE_SET2n = nand(SPRITE_MASK2n, oam_reg.REG_OAM_A4);
      /*p34.SUKY*/ wire SPRITE_PAL_PIPE_SET3n = nand(SPRITE_MASK3n, oam_reg.REG_OAM_A4);
      /*p34.RORA*/ wire SPRITE_PAL_PIPE_SET4n = nand(SPRITE_MASK4n, oam_reg.REG_OAM_A4);
      /*p34.MENE*/ wire SPRITE_PAL_PIPE_SET5n = nand(SPRITE_MASK5n, oam_reg.REG_OAM_A4);
      /*p34.LUKE*/ wire SPRITE_PAL_PIPE_SET6n = nand(SPRITE_MASK6n, oam_reg.REG_OAM_A4);
      /*p34.LAMY*/ wire SPRITE_PAL_PIPE_SET7n = nand(SPRITE_MASK7n, oam_reg.REG_OAM_A4);

      /*p34.SUCO*/ wire SPRITE_PAL_PIPE_RST0n = nand(SPRITE_MASK0n, /*p34.SYPY*/ not(oam_reg.REG_OAM_A4));
      /*p34.TAFA*/ wire SPRITE_PAL_PIPE_RST1n = nand(SPRITE_MASK1n, /*p34.TOTU*/ not(oam_reg.REG_OAM_A4));
      /*p34.PYZY*/ wire SPRITE_PAL_PIPE_RST2n = nand(SPRITE_MASK2n, /*p34.NARO*/ not(oam_reg.REG_OAM_A4));
      /*p34.TOWA*/ wire SPRITE_PAL_PIPE_RST3n = nand(SPRITE_MASK3n, /*p34.WEXY*/ not(oam_reg.REG_OAM_A4));
      /*p34.RUDU*/ wire SPRITE_PAL_PIPE_RST4n = nand(SPRITE_MASK4n, /*p34.RYZY*/ not(oam_reg.REG_OAM_A4));
      /*p34.PAZO*/ wire SPRITE_PAL_PIPE_RST5n = nand(SPRITE_MASK5n, /*p34.RYFE*/ not(oam_reg.REG_OAM_A4));
      /*p34.LOWA*/ wire SPRITE_PAL_PIPE_RST6n = nand(SPRITE_MASK6n, /*p34.LADY*/ not(oam_reg.REG_OAM_A4));
      /*p34.LUNU*/ wire SPRITE_PAL_PIPE_RST7n = nand(SPRITE_MASK7n, /*p34.LAFY*/ not(oam_reg.REG_OAM_A4));

      /*p34.RUGO*/ pxp_reg.PAL_PIPE_0.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, joy_pins.P10_B);
      /*p34.SATA*/ pxp_reg.PAL_PIPE_1.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, pxp_reg.PAL_PIPE_0);
      /*p34.ROSA*/ pxp_reg.PAL_PIPE_2.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, pxp_reg.PAL_PIPE_1);
      /*p34.SOMY*/ pxp_reg.PAL_PIPE_3.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, pxp_reg.PAL_PIPE_2);
      /*p34.PALU*/ pxp_reg.PAL_PIPE_4.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, pxp_reg.PAL_PIPE_3);
      /*p34.NUKE*/ pxp_reg.PAL_PIPE_5.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, pxp_reg.PAL_PIPE_4);
      /*p34.MODA*/ pxp_reg.PAL_PIPE_6.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, pxp_reg.PAL_PIPE_5);
      /*p34.LYME*/ pxp_reg.PAL_PIPE_7.set(CLKPIPE_AxCxExGx, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, pxp_reg.PAL_PIPE_6);
    }

    // Background mask pipe
    {
      /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(SPRITE_MASK0n, oam_reg.REG_OAM_A7);
      /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(SPRITE_MASK1n, oam_reg.REG_OAM_A7);
      /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(SPRITE_MASK2n, oam_reg.REG_OAM_A7);
      /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(SPRITE_MASK3n, oam_reg.REG_OAM_A7);
      /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(SPRITE_MASK4n, oam_reg.REG_OAM_A7);
      /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(SPRITE_MASK5n, oam_reg.REG_OAM_A7);
      /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(SPRITE_MASK6n, oam_reg.REG_OAM_A7);
      /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(SPRITE_MASK7n, oam_reg.REG_OAM_A7);

      /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(SPRITE_MASK0n, /*p26.XOGA*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(SPRITE_MASK1n, /*p26.XURA*/ not(oam_reg.REG_OAM_A7));
      /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(SPRITE_MASK2n, /*p26.TAJO*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(SPRITE_MASK3n, /*p26.XENU*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(SPRITE_MASK4n, /*p26.XYKE*/ not(oam_reg.REG_OAM_A7));
      /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(SPRITE_MASK5n, /*p26.XABA*/ not(oam_reg.REG_OAM_A7));
      /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(SPRITE_MASK6n, /*p26.TAFU*/ not(oam_reg.REG_OAM_A7));
      /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(SPRITE_MASK7n, /*p26.XUHO*/ not(oam_reg.REG_OAM_A7));

      /*p26.VEZO*/ pxp_reg.MASK_PIPE_0.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET0, MASK_PIPE_RST0, joy_pins.P10_B);
      /*p26.WURU*/ pxp_reg.MASK_PIPE_1.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET1, MASK_PIPE_RST1, pxp_reg.MASK_PIPE_0);
      /*p26.VOSA*/ pxp_reg.MASK_PIPE_2.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET2, MASK_PIPE_RST2, pxp_reg.MASK_PIPE_1);
      /*p26.WYFU*/ pxp_reg.MASK_PIPE_3.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET3, MASK_PIPE_RST3, pxp_reg.MASK_PIPE_2);
      /*p26.XETE*/ pxp_reg.MASK_PIPE_4.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET4, MASK_PIPE_RST4, pxp_reg.MASK_PIPE_3);
      /*p26.WODA*/ pxp_reg.MASK_PIPE_5.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET5, MASK_PIPE_RST5, pxp_reg.MASK_PIPE_4);
      /*p26.VUMO*/ pxp_reg.MASK_PIPE_6.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET6, MASK_PIPE_RST6, pxp_reg.MASK_PIPE_5);
      /*p26.VAVA*/ pxp_reg.MASK_PIPE_7.set(CLKPIPE_AxCxExGx, MASK_PIPE_SET7, MASK_PIPE_RST7, pxp_reg.MASK_PIPE_6);
    }
  }

  //----------
  // Pixel merge
  
  if (1) {
    /*p35.RAJY*/ wire PIX_BG0  = and(cfg_reg.LCDC_BGEN, pxp_reg.BG_PIPE_A7);
    /*p35.TADE*/ wire PIX_BG1  = and(cfg_reg.LCDC_BGEN, pxp_reg.BG_PIPE_B7);

    /*p35.WOXA*/ wire PIX_SP0  = and(cfg_reg.LCDC_SPEN, pxp_reg.SPR_PIPE_A7);
    /*p35.XULA*/ wire PIX_SP1  = and(cfg_reg.LCDC_SPEN, pxp_reg.SPR_PIPE_B7);

    /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

    /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, pxp_reg.MASK_PIPE_7);
    /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, pxp_reg.MASK_PIPE_7);
    /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

    /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(pxp_reg.PAL_PIPE_7);
    /*p34.LAFU*/ wire OBP0PIXELn = nand(SPRITE_PAL_PIPE_7n, BGPIXELn);
    /*p34.LEKA*/ wire OBP1PIXELn = nand(SPRITE_PAL_PIPE_7n, BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*p35.VUMU*/ wire PIX_SP0n = not(PIX_SP0);
    /*p35.WYRU*/ wire PIX_SP0a = not(PIX_SP0n);
    /*p35.WELE*/ wire PIX_SP1n = not(PIX_SP1);
    /*p35.WOLO*/ wire PIX_SP1a = not(PIX_SP1n);

    /*p35.LAVA*/ wire MASK_OPB0  = not(OBP0PIXELn);
    /*p35.VYRO*/ wire PAL_OBP0D = and(PIX_SP0a, PIX_SP1a, MASK_OPB0);
    /*p35.VATA*/ wire PAL_OBP0C = and(PIX_SP0a, PIX_SP1n, MASK_OPB0);
    /*p35.VOLO*/ wire PAL_OBP0B = and(PIX_SP0n, PIX_SP1a, MASK_OPB0);
    /*p35.VUGO*/ wire PAL_OBP0A = and(PIX_SP0n, PIX_SP1n, MASK_OPB0);
    

    /*p35.WUFU*/ wire COL_OBP00 = amux4(cfg_reg.OBP07, PAL_OBP0D,
                                        cfg_reg.OBP05, PAL_OBP0C,
                                        cfg_reg.OBP03, PAL_OBP0B,
                                        cfg_reg.OBP01, PAL_OBP0A);

    /*p35.WALY*/ wire COL_OBP01 = amux4(cfg_reg.OBP06, PAL_OBP0D,
                                        cfg_reg.OBP04, PAL_OBP0C,
                                        cfg_reg.OBP02, PAL_OBP0B,
                                        cfg_reg.OBP00, PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*p35.MEXA*/ wire PIX_SP0o = not(PIX_SP0);
    /*p35.LOZO*/ wire PIX_SP0b = not(PIX_SP0o);
    /*p35.MABY*/ wire PIX_SP1o = not(PIX_SP1);
    /*p35.LYLE*/ wire PIX_SP1b = not(PIX_SP1o);

    /*p35.LUKU*/ wire MASK_OBP1  = not(OBP1PIXELn);
    /*p35.LEDO*/ wire PAL_OBP1D = and(PIX_SP1b, PIX_SP0b, MASK_OBP1);
    /*p35.LYKY*/ wire PAL_OBP1C = and(PIX_SP1b, PIX_SP0o, MASK_OBP1);
    /*p35.LARU*/ wire PAL_OBP1B = and(PIX_SP1o, PIX_SP0b, MASK_OBP1);
    /*p35.LOPU*/ wire PAL_OBP1A = and(PIX_SP1o, PIX_SP0o, MASK_OBP1);

    /*p35.MOKA*/ wire COL_OBP10 = amux4(cfg_reg.OBP17, PAL_OBP1D,
                                        cfg_reg.OBP15, PAL_OBP1C,
                                        cfg_reg.OBP13, PAL_OBP1B,
                                        cfg_reg.OBP11, PAL_OBP1A);

    /*p35.MUFA*/ wire COL_OBP11 = amux4(cfg_reg.OBP16, PAL_OBP1D,
                                        cfg_reg.OBP14, PAL_OBP1C,
                                        cfg_reg.OBP12, PAL_OBP1B,
                                        cfg_reg.OBP10, PAL_OBP1A);

    //----------
    // Background/window palette lookup

    /*p35.SOBA*/ wire PIX_BG0n = not(PIX_BG0);
    /*p35.VYCO*/ wire PIX_BG1n = not(PIX_BG1);
    /*p35.NUPO*/ wire PIX_BG0a = not(PIX_BG0n);
    /*p35.NALE*/ wire PIX_BG1a = not(PIX_BG1n);

    /*p35.MUVE*/ wire MASK_BGP = not(BGPIXELn);
    /*p35.POBU*/ wire PAL_BGPA = and(PIX_BG1n, PIX_BG0n, MASK_BGP);
    /*p35.NUMA*/ wire PAL_BGPB = and(PIX_BG1a, PIX_BG0n, MASK_BGP);
    /*p35.NUXO*/ wire PAL_BGPC = and(PIX_BG1n, PIX_BG0a, MASK_BGP);
    /*p35.NYPO*/ wire PAL_BGPD = and(PIX_BG1a, PIX_BG0a, MASK_BGP);

    /*p35.NURA*/ wire COL_BGP1 = amux4(cfg_reg.BGP7, PAL_BGPD,
                                        cfg_reg.BGP5, PAL_BGPC,
                                        cfg_reg.BGP3, PAL_BGPB,
                                        cfg_reg.BGP1, PAL_BGPA);

    /*p35.NELO*/ wire COL_BGP0 = amux4(cfg_reg.BGP6, PAL_BGPD,
                                        cfg_reg.BGP4, PAL_BGPC,
                                        cfg_reg.BGP2, PAL_BGPB,
                                        cfg_reg.BGP0, PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ wire PATY_LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
    /*p35.PERO*/ wire PERO_LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);

    /*p35.REMY*/ wire REMY_LD0n = not(PATY_LD0);
    /*p35.RAVO*/ wire RAVO_LD1n = not(PERO_LD1);

    lcd_pins.LD0.set(not(REMY_LD0n));
    lcd_pins.LD1.set(not(RAVO_LD1n));
  }

  //----------------------------------------

  /*p04.DECY*/ wire FROM_CPU5n = not(cpu_pins.FROM_CPU5);
  /*p04.CATY*/   wire FROM_CPU5  = not(FROM_CPU5n);
  /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(ASOT_BUS_RD_ABCDEFGH, FROM_CPU5);
  /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
  /*p25.SYRO*/ wire SYRO_FE00_FFFF = not(TUNA_0000_FDFF);
  /*p25.TEFA*/   wire TEFA = nor(SYRO_FE00_FFFF, TEXO_ADDR_VALID_AND_NOT_VRAM);
  /*p25.SOSE*/ wire SOSE_ADDR_VRAM = and(cpu_pins.A15, TEFA);

  cpu_pins.SYRO.set(SYRO_FE00_FFFF);


  // CPU/DMA vram write handler
  {
    /*p25.TUTO*/ wire TUTO_DBG_VRAM  = and(UNOR_MODE_DBG2, !bus_reg.SOTO_DBG);
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not(TUTO_DBG_VRAM);

    /*p25.TAVY*/   wire MOE_Cn = not(vram_pins.MOE_C);
    /*p25.TEGU*/   wire CPU_RAM_CLK = nand(SOSE_ADDR_VRAM, AFAS_xxxxxFGH);
    /*p25.SALE*/ wire CPU_RAM_CLK2 = mux2(MOE_Cn, CPU_RAM_CLK, TUTO_DBG_VRAM);

    {

      /*p29.TYTU*/ wire SPR_SEQ0n = not(spr_reg.TOXE_SPR_SEQ0);
      /*p29.TACU*/ wire SPR_SEQ_5_TRIG = nand(spr_reg.TYFO_SEQ_B0d, SPR_SEQ0n);
      /*p25.RYLU*/ wire CPU_READ_VRAMn = nand(CPU_RAM_CLK2, ROPY_RENDERINGn);
      /*p25.SOHO*/ wire SPR_READ_VRAM = and(SPR_SEQ_5_TRIG, SPRITE_READn);
      /*p25.RAWA*/ wire SPR_READ_VRAMn = not(SPR_READ_VRAM);
      /*p25.RACU*/ wire MOEn = and(CPU_READ_VRAMn,
                                   SPR_READ_VRAMn,
                                   BG_READ_VRAMn,
                                   DMA_READ_VRAMn);
      // not at all sure about this section
      /*p25.RUTE*/ wire MOE_Dn = or (MOEn, TUTO_DBG_VRAM); // schematic wrong, second input is RACU
      /*p25.SEMA*/ wire MOE_An = and(MOEn, RACO_DBG_VRAMn);
      /*p25.REFO*/ vram_pins.MOE_A.set(not(MOE_An));
      /*p25.SAHA*/ vram_pins.MOE_D.set(not(MOE_Dn));
    }


    /*p25.SYCY*/   wire SYCY_MODE_DBG2n = not(UNOR_MODE_DBG2);
    /*p25.SOTO*/ bus_reg.SOTO_DBG.set(SYCY_MODE_DBG2n, CUNU_RSTn, !bus_reg.SOTO_DBG);


    /*p25.TUCA*/   wire CPU_VRAM_RD  = and (SOSE_ADDR_VRAM, ABUZ_ADDR_VALIDn_ABxxxxxx);
    /*p25.TEFY*/   wire MCS_Cn = not(vram_pins.MCS_C);
    /*p25.TOLE*/   wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, TUTO_DBG_VRAM);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, ROPY_RENDERINGn);






    {
      /*p25.SUTU*/ wire MCS = nor(BGW_READ_VRAM, DMA_READ_VRAM, SPRITE_READn, SERE);
      /*p25.TODE*/ wire MCS_An = and(MCS, RACO_DBG_VRAMn);
      /*p25.SEWO*/ wire MCS_Dn = or (MCS, TUTO_DBG_VRAM);
      /*p25.SOKY*/ vram_pins.MCS_A.set(not(MCS_An));
      /*p25.SETY*/ vram_pins.MCS_D.set(not(MCS_Dn));
    }

    {
      /*p25.TUJA*/ wire CPU_VRAM_WR    = and (SOSE_ADDR_VRAM, APOV_CPU_WR_xxxxxFGH);
      /*p25.SUDO*/ wire MWR_Cn = not(vram_pins.MWR_C);
      /*p25.TYJY*/ wire CPU_VRAM_WR2   = mux2(MWR_Cn, CPU_VRAM_WR , TUTO_DBG_VRAM);
      /*p25.SOHY*/ wire MWR = nand(CPU_VRAM_WR2, SERE);
      /*p25.TAXY*/ wire MWR_An = and(MWR, RACO_DBG_VRAMn);
      /*p25.SOFY*/ wire MWR_Dn = or (MWR, TUTO_DBG_VRAM);
      /*p25.SYSY*/ vram_pins.MWR_A.set(not(MWR_An));
      /*p25.RAGU*/ vram_pins.MWR_D.set(not(MWR_Dn));
    }

    {
      /*p25.TYVY*/ wire MD_TO_Dn = nand(SERE, CPU_READ_MYSTERY);
      /*p25.SEBY*/ wire MD_TO_D = not(MD_TO_Dn);
      /*p25.RUGA*/ /*p25.RERY*/ cpu_pins.D0.set_tribuf(MD_TO_D, !not(vid_reg.MD0));
      /*p25.ROTA*/ /*p25.RUNA*/ cpu_pins.D1.set_tribuf(MD_TO_D, !not(vid_reg.MD1));
      /*p25.RYBU*/ /*p25.RONA*/ cpu_pins.D2.set_tribuf(MD_TO_D, !not(vid_reg.MD2));
      /*p25.RAJU*/ /*p25.RUNO*/ cpu_pins.D3.set_tribuf(MD_TO_D, !not(vid_reg.MD3));
      /*p25.TYJA*/ /*p25.SANA*/ cpu_pins.D4.set_tribuf(MD_TO_D, !not(vid_reg.MD4));
      /*p25.REXU*/ /*p25.RORO*/ cpu_pins.D5.set_tribuf(MD_TO_D, !not(vid_reg.MD5));
      /*p25.RUPY*/ /*p25.RABO*/ cpu_pins.D6.set_tribuf(MD_TO_D, !not(vid_reg.MD6));
      /*p25.TOKU*/ /*p25.SAME*/ cpu_pins.D7.set_tribuf(MD_TO_D, !not(vid_reg.MD7));
    }

    {
      // this can't be right, md_outd and md_outc are effectively the same signal
      // yeah that's what it looks like on the schematic
      /*p25.RUVY*/   wire CPU_RAM_CLK2n = not(CPU_RAM_CLK2);
      /*p25.SAZO*/   wire MD_OUTd = and(CPU_RAM_CLK2n, SERE);
      /*p25.RYJE*/   wire MD_INb  = not(MD_OUTd);
      /*p25.REVO*/   wire MD_OUTc = not(MD_INb);
      /*p25.ROCY*/ wire CBUS_TO_VBUSb = and(MD_OUTc, MD_OUTd);
      /*p25.RAHU*/ wire CBUS_TO_VBUSn = not(CBUS_TO_VBUSb);
      /*p25.ROVE*/ wire CBUS_TO_VBUS = not(CBUS_TO_VBUSn);

      /*p25.TEME*/ vid_reg.MD0.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D0);
      /*p25.TEWU*/ vid_reg.MD1.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D1);
      /*p25.TYGO*/ vid_reg.MD2.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D2);
      /*p25.SOTE*/ vid_reg.MD3.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D3);
      /*p25.SEKE*/ vid_reg.MD4.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D4);
      /*p25.RUJO*/ vid_reg.MD5.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D5);
      /*p25.TOFA*/ vid_reg.MD6.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D6);
      /*p25.SUZA*/ vid_reg.MD7.set_tribuf(!CBUS_TO_VBUSn, cpu_pins.D7);

      /*p25.REGE*/ vram_pins.MD0_A.set(not(/*p25.SEFA*/ and(vid_reg.MD0, CBUS_TO_VBUS)));
      /*p25.RYKY*/ vram_pins.MD1_A.set(not(/*p25.SOGO*/ and(vid_reg.MD1, CBUS_TO_VBUS)));
      /*p25.RAZO*/ vram_pins.MD2_A.set(not(/*p25.SEFU*/ and(vid_reg.MD2, CBUS_TO_VBUS)));
      /*p25.RADA*/ vram_pins.MD3_A.set(not(/*p25.SUNA*/ and(vid_reg.MD3, CBUS_TO_VBUS)));
      /*p25.RYRO*/ vram_pins.MD4_A.set(not(/*p25.SUMO*/ and(vid_reg.MD4, CBUS_TO_VBUS)));
      /*p25.REVU*/ vram_pins.MD5_A.set(not(/*p25.SAZU*/ and(vid_reg.MD5, CBUS_TO_VBUS)));
      /*p25.REKU*/ vram_pins.MD6_A.set(not(/*p25.SAMO*/ and(vid_reg.MD6, CBUS_TO_VBUS)));
      /*p25.RYZE*/ vram_pins.MD7_A.set(not(/*p25.SUKE*/ and(vid_reg.MD7, CBUS_TO_VBUS)));

      /*p25.RURA*/ vram_pins.MD0_D.set(not(/*p25.SYNU*/ or(vid_reg.MD0, CBUS_TO_VBUSn)));
      /*p25.RULY*/ vram_pins.MD1_D.set(not(/*p25.SYMA*/ or(vid_reg.MD1, CBUS_TO_VBUSn)));
      /*p25.RARE*/ vram_pins.MD2_D.set(not(/*p25.ROKO*/ or(vid_reg.MD2, CBUS_TO_VBUSn)));
      /*p25.RODU*/ vram_pins.MD3_D.set(not(/*p25.SYBU*/ or(vid_reg.MD3, CBUS_TO_VBUSn)));
      /*p25.RUBE*/ vram_pins.MD4_D.set(not(/*p25.SAKO*/ or(vid_reg.MD4, CBUS_TO_VBUSn)));
      /*p25.RUMU*/ vram_pins.MD5_D.set(not(/*p25.SEJY*/ or(vid_reg.MD5, CBUS_TO_VBUSn)));
      /*p25.RYTY*/ vram_pins.MD6_D.set(not(/*p25.SEDO*/ or(vid_reg.MD6, CBUS_TO_VBUSn)));
      /*p25.RADY*/ vram_pins.MD7_D.set(not(/*p25.SAWU*/ or(vid_reg.MD7, CBUS_TO_VBUSn)));
    }

    {
      // this can't be right, md_outd and md_outc are effectively the same signal
      // yeah that's what it looks like on the schematic
      /*p25.RUVY*/   wire CPU_RAM_CLK2n = not(CPU_RAM_CLK2);
      /*p25.SAZO*/   wire MD_OUTd = and(CPU_RAM_CLK2n, SERE);
      /*p25.RYJE*/   wire MD_INb  = not(MD_OUTd);
      /*p25.REVO*/   wire MD_OUTc = not(MD_INb);
      /*p25.RELA*/ wire VEXT_TO_VBUSb  = or(MD_OUTc, MD_OUTd);
      /*p25.RENA*/ wire VEXT_TO_VBUSn = not(VEXT_TO_VBUSb);

      /*p25.RODY*/ vid_reg.MD0.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD0_C);
      /*p25.REBA*/ vid_reg.MD1.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD1_C);
      /*p25.RYDO*/ vid_reg.MD2.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD2_C);
      /*p25.REMO*/ vid_reg.MD3.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD3_C);
      /*p25.ROCE*/ vid_reg.MD4.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD4_C);
      /*p25.ROPU*/ vid_reg.MD5.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD5_C);
      /*p25.RETA*/ vid_reg.MD6.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD6_C);
      /*p25.RAKU*/ vid_reg.MD7.set_tribuf(!VEXT_TO_VBUSn, vram_pins.MD7_C);

      /*p25.ROFA*/ vram_pins.MD0_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD1_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD2_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD3_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD4_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD5_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD6_B.set(not(VEXT_TO_VBUSn));
      /*p25.ROFA*/ vram_pins.MD7_B.set(not(VEXT_TO_VBUSn));
    }
  }

  // vram pin handler
  if (1) {
    /*p25.LEXE*/ vram_pins.MA00_D.set(not(/*p25.MYFU*/ not(vid_reg.MA00)));
    /*p25.LOZU*/ vram_pins.MA01_D.set(not(/*p25.MASA*/ not(vid_reg.MA01)));
    /*p25.LACA*/ vram_pins.MA02_D.set(not(/*p25.MYRE*/ not(vid_reg.MA02)));
    /*p25.LUVO*/ vram_pins.MA03_D.set(not(/*p25.MAVU*/ not(vid_reg.MA03)));
    /*p25.LOLY*/ vram_pins.MA04_D.set(not(/*p25.MEPA*/ not(vid_reg.MA04)));
    /*p25.LALO*/ vram_pins.MA05_D.set(not(/*p25.MYSA*/ not(vid_reg.MA05)));
    /*p25.LEFA*/ vram_pins.MA06_D.set(not(/*p25.MEWY*/ not(vid_reg.MA06)));
    /*p25.LUBY*/ vram_pins.MA07_D.set(not(/*p25.MUME*/ not(vid_reg.MA07)));
    /*p25.TUJY*/ vram_pins.MA08_D.set(not(/*p25.VOVA*/ not(vid_reg.MA08)));
    /*p25.TAGO*/ vram_pins.MA09_D.set(not(/*p25.VODE*/ not(vid_reg.MA09)));
    /*p25.NUVA*/ vram_pins.MA10_D.set(not(/*p25.RUKY*/ not(vid_reg.MA10)));
    /*p25.PEDU*/ vram_pins.MA11_D.set(not(/*p25.RUMA*/ not(vid_reg.MA11)));
    /*p25.PONY*/ vram_pins.MA12_D.set(not(/*p25.REHO*/ not(vid_reg.MA12)));
  }

#if 0
    // so the address bus is technically a tribuf, but we're going to ignore
    // this debug circuit for now.
    {
      // If we're in debug mode 2, drive external address bus onto internal address
      // these may be backwards, probably don't want to drive external address onto bus normally...

      /*p08.KOVA*/ wire A00_Cn = not(ext_pins.A00_C);
      /*p08.CAMU*/ wire A01_Cn = not(ext_pins.A01_C);
      /*p08.BUXU*/ wire A02_Cn = not(ext_pins.A02_C);
      /*p08.BASE*/ wire A03_Cn = not(ext_pins.A03_C);
      /*p08.AFEC*/ wire A04_Cn = not(ext_pins.A04_C);
      /*p08.ABUP*/ wire A05_Cn = not(ext_pins.A05_C);
      /*p08.CYGU*/ wire A06_Cn = not(ext_pins.A06_C);
      /*p08.COGO*/ wire A07_Cn = not(ext_pins.A07_C);
      /*p08.MUJY*/ wire A08_Cn = not(ext_pins.A08_C);
      /*p08.NENA*/ wire A09_Cn = not(ext_pins.A09_C);
      /*p08.SURA*/ wire A10_Cn = not(ext_pins.A10_C);
      /*p08.MADY*/ wire A11_Cn = not(ext_pins.A11_C);
      /*p08.LAHE*/ wire A12_Cn = not(ext_pins.A12_C);
      /*p08.LURA*/ wire A13_Cn = not(ext_pins.A13_C);
      /*p08.PEVO*/ wire A14_Cn = not(ext_pins.A14_C);
      /*p08.RAZA*/ wire A15_Cn = not(ext_pins.A15_C);

      /*p08.KEJO*/ cpu_pins.A00.set(!TOVA_MODE_DBG2n, not(A00_Cn));
      /*p08.BYXE*/ cpu_pins.A01.set(!TOVA_MODE_DBG2n, not(A01_Cn));
      /*p08.AKAN*/ cpu_pins.A02.set(!TOVA_MODE_DBG2n, not(A02_Cn));
      /*p08.ANAR*/ cpu_pins.A03.set(!TOVA_MODE_DBG2n, not(A03_Cn));
      /*p08.AZUV*/ cpu_pins.A04.set(!TOVA_MODE_DBG2n, not(A04_Cn));
      /*p08.AJOV*/ cpu_pins.A05.set(!TOVA_MODE_DBG2n, not(A05_Cn));
      /*p08.BYNE*/ cpu_pins.A06.set(!TOVA_MODE_DBG2n, not(A06_Cn));
      /*p08.BYNA*/ cpu_pins.A07.set(!TOVA_MODE_DBG2n, not(A07_Cn));
      /*p08.LOFA*/ cpu_pins.A08.set(!TOVA_MODE_DBG2n, not(A08_Cn));
      /*p08.MAPU*/ cpu_pins.A09.set(!TOVA_MODE_DBG2n, not(A09_Cn));
      /*p08.RALA*/ cpu_pins.A10.set(!TOVA_MODE_DBG2n, not(A10_Cn));
      /*p08.LORA*/ cpu_pins.A11.set(!TOVA_MODE_DBG2n, not(A11_Cn));
      /*p08.LYNA*/ cpu_pins.A12.set(!TOVA_MODE_DBG2n, not(A12_Cn));
      /*p08.LEFY*/ cpu_pins.A13.set(!TOVA_MODE_DBG2n, not(A13_Cn));
      /*p08.NEFE*/ cpu_pins.A14.set(!TOVA_MODE_DBG2n, not(A14_Cn));
      /*p08.SYZU*/ cpu_pins.A15.set(!TOVA_MODE_DBG2n, not(A15_Cn));
    }
#endif

#if 0

    // Cart responds to 0x0000 - 0x7FFF (rom)
    // Cart responds to 0xA000 - 0xBFFF (ram)
    // Ram  responds to 0xC000 - 0xDFFF (ram)

    ADDR_NOT_VRAM = or(cpu_pins.A13, cpu_pins.A14, ADDR_0000_7FFF);
    ADDR_RAM = and(A000_FFFF, TUNA_0000_FDFF);

    // this seems reasonable
    RD_A = 1;
    WR_A = 0;

    EXT_DATA_PINS_OUT = 0;
    if (cpu_pins.ADDR_VALIDn && ADDR_NOT_VRAM && cpu_pins.CPU_RAW_WR) {
      RD_A = 0;
      WR_A = xxxxEFGx;
      EXT_DATA_PINS_OUT = 1;
    }

    // this doesn't completely make sense...
    EXT_ADDR_15 = cpu_pins.A15;
    if (ADDR_00XX &&& !BOOT_BIT) EXT_ADDR_15 = 1;
    if (cpu_pins.ADDR_VALIDn) EXT_ADDR_15 |= xxCDEFGH;  // whyyyy?

    DATA_LATCH = cpu_pins.ADDR_VALIDn && ADDR_NOT_VRAM && cpu_pins.CPU_RAW_RD ? not(cpu_pins.FROM_CPU5) : 1;
    ADDR_LATCH = cpu_pins.ADDR_VALIDn && ADDR_NOT_VRAM;

    // this doesn't make much sense either
    if (cpu_pins.ADDR_VALIDn) {
      CS_A = and(ABxxxxxx, ADDR_RAM);
    }
    else {
      CS_A = ADDR_RAM;
    }

    ext_pins.PHI.set(BUDE_AxxxxFGH);
    ext_pins.WR_A.set(WR_A);
    ext_pins.RD_A.set(RD_A);
    ext_pins.CS_A.set(CS_A);

#endif

#if 0
    // TYNU, LAGU, and LOXO are weird

    /*p08.SORE*/ SORE = not(A15)
    /*p08.TEVY*/ TEVY = or(A13, A14, SORE) // overlay border color wrong, this is OR
    /*p08.TEXO*/ TEXO = and(FROM_CPU4, TEVY)
    /*p08.SOGY*/ SOGY = not(A14)
    /*p08.TUMA*/ TUMA = and(A13, SOGY, A15)
    /*p08.TYNU*/ TYNU = weird(A15, A14, TUMA)
    /*p08.TOZA*/ TOZA = and(TYNU, ABUZ, TUNA)
    /*p08.MEXO*/ MEXO = not(APOV)
    /*p08.MOCA*/ MOCA = nor(TEXO, UMUT)
    /*p08.NEVY*/ NEVY = or(MEXO, MOCA)
    /*p08.PUVA*/ PUVA = or(NEVY, LUMA)
    /*p08.LEVO*/ LEVO = not(TEXO)
    /*p08.LAGU*/ LAGU = weird(CPU_RAW_RD, LEVO, FROM_CPU3)
    /*p08.LYWE*/ LYWE = not(LAGU)
    /*p08.MOTY*/ MOTY = or(MOCA, LYWE)
    /*p08.TYMU*/ TYMU = nor(LUMA, MOTY)
    /*p08.REDU*/ REDU = not(TEDO)
    /*p08.RORU*/ RORU = green box(UNOR, REDU, MOTY) // polarity?
    /*p08.LULA*/ LULA = not(RORU)
    /*p08.MULE*/ MULE = not(UMUT)
    /*p08.LOXO*/ LOXO = weird(MULE, TEXO, UMUT)
    /*p08.LASY*/ LASY = not(LOXO)
    /*p08.MATE*/ MATE = not(LASY)
    /*p08.LAVO*/ LAVO = nand(CPU_RAW_RD, TEXO, FROM_CPU5)
    /*p08.SOBY*/ SOBY = nor(A15, TUTU)   // schematic says CPU_WR? but it's TUTU....
    /*p08.SEPY*/ SEPY = nand(ABUZ, SOBY) // the ABUZ line goes below TEME
    /*p08.TAZY*/ TAZY = green box (LUMA, MARU?, SEPY)
    /*p08.UVER*/ UVER = nand(PUVA, TOVA)
    /*p08.USUF*/ USUF = nor(UNOR, PUVA)
    /*p08.UGAC*/ UGAC = nand(TYMU, TOVA)
    /*p08.URUN*/ URUN = nor(UNOR, TYMU)
    /*p08.TYHO*/ TYHO = green box(LUMA, MARU?, TOZA)

    CS_a = TYHO
    RD_a = UGAC
    RD_c = UJYV
    RD_d = URUN
    WR_a = UVER
    WR_c = UBAL
    WR_d = USUF


#endif

  if (1) {
    // Cart responds to 0x0000 - 0x7FFF (rom)
    // Cart responds to 0xA000 - 0xBFFF (ram)
    // Ram  responds to 0xC000 - 0xDFFF (ram)

    /*p04.LEBU*/ wire DMA_A15n  = not(dma_reg.DMA_A15);
    /*p04.MUDA*/ wire DMA_VRAM  = nor(dma_reg.DMA_A13, dma_reg.DMA_A14, DMA_A15n);
    /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);
    /*p04.MORY*/ wire DMA_READ_CARTn = nand(dma_reg.REG_DMA_RUNNING, DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CART = not(DMA_READ_CARTn);

    /*p08.SORE*/ wire SORE_ADDR_0000_7FFF = not(cpu_pins.A15);
    /*p08.TEVY*/ wire TEVY_ADDR_NOT_VRAM = or(cpu_pins.A13, cpu_pins.A14, SORE_ADDR_0000_7FFF);
    /*p08.TEXO*/ wire TEXO_ADDR_VALID_AND_NOT_VRAM = and(cpu_pins.ADDR_VALIDn, TEVY_ADDR_NOT_VRAM);

    /*p08.SOGY*/ wire SOGY = not(cpu_pins.A14);
    /*p08.TUMA*/ wire TUMA_A000_BFFF = and(cpu_pins.A13, SOGY, cpu_pins.A15);
    /*p08.TYNU*/ wire TYNU_A000_FFFF = or(and(cpu_pins.A15, cpu_pins.A14), TUMA_A000_BFFF);
    /*p08.TOZA*/ wire TOZA_ADDR_RAM_ABxxxxxx = and(ABUZ_ADDR_VALIDn_ABxxxxxx, TYNU_A000_FFFF, TUNA_0000_FDFF);

    // FIXME SUFFIX
    /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(APOV_CPU_WR_xxxxxFGH);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RDn = nor(TEXO_ADDR_VALID_AND_NOT_VRAM, UMUT_MODE_DBG1);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, MOCA_DBG_EXT_RDn);
    /*p08.PUVA*/ wire PUVA_WR_OUTn = or(NEVY, LUMA_DMA_READ_CART);
      
      

    /*p08.LEVO*/ wire LEVO_ADDR_VALID_AND_NOT_VRAMn = not(TEXO_ADDR_VALID_AND_NOT_VRAM);
    /*p08.LAGU*/ wire LAGU = or(and(cpu_pins.CPU_RAW_RD, LEVO_ADDR_VALID_AND_NOT_VRAMn), cpu_pins.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or(MOCA_DBG_EXT_RDn, LYWE);
    /*p08.TYMU*/ wire TYMU_RD_OUTn = nor(LUMA_DMA_READ_CART, MOTY_CPU_EXT_RD);

    /*p08.REDU*/ wire REDU_CPU_RDo = not(TEDO_BUS_RD);
    /*p08.RORU*/ wire RORU_EXT_DATA_PINS_OUTn = mux2(REDU_CPU_RDo, MOTY_CPU_EXT_RD, UNOR_MODE_DBG2);
    /*p08.LULA*/ wire LULA_EXT_DATA_PINS_OUT  = not(RORU_EXT_DATA_PINS_OUTn);

    // Address bus to address pins mux
    /*p08.MULE*/ wire MULE_MODE_DBG1o = not(UMUT_MODE_DBG1);
    /*p08.LOXO*/ wire LOXO = or(and(MULE_MODE_DBG1o, TEXO_ADDR_VALID_AND_NOT_VRAM), UMUT_MODE_DBG1);
    /*p08.LASY*/ wire LASY = not(LOXO);
    /*p08.MATE*/ wire MATE_LATCH_INT_ADDR  = not(LASY);

    /*p08.LAVO*/ wire LAVO_LATCH_EXT_DATA = nand(cpu_pins.CPU_RAW_RD, TEXO_ADDR_VALID_AND_NOT_VRAM, cpu_pins.FROM_CPU5);

    /*p08.SOBY*/ wire SOBY = nor(cpu_pins.A15, TUTU_ADDR_BOOT);
    /*p08.SEPY*/ wire SEPY_ADDR_LATCH_15 = nand(ABUZ_ADDR_VALIDn_ABxxxxxx, SOBY); // wat?
    /*p08.TAZY*/ wire TAZY_EXT_ADDR_15 = mux2(dma_reg.DMA_A15, SEPY_ADDR_LATCH_15, LUMA_DMA_READ_CART);


    /*p08.UVER*/ wire UVER_WRn_A = nand(PUVA_WR_OUTn, TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire USUF_WRn_D = nor (PUVA_WR_OUTn, UNOR_MODE_DBG2);
    /*p08.UGAC*/ wire UGAC_RDn_A = nand(TYMU_RD_OUTn, TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire URUN_RDn_D = nor (TYMU_RD_OUTn, UNOR_MODE_DBG2);
    /*p08.TYHO*/ wire TYHO_CSn_A = mux2(dma_reg.DMA_A15, TOZA_ADDR_RAM_ABxxxxxx, LUMA_DMA_READ_CART); // ABxxxxxx

    /* PIN_75 */ ext_pins.PHI .set(BUDE_AxxxxFGH);
    /* PIN_78 */ ext_pins.WR_A.set(UVER_WRn_A);
    /* PIN_78 */ ext_pins.WR_D.set(USUF_WRn_D);
    /* PIN_79 */ ext_pins.RD_A.set(UGAC_RDn_A);
    /* PIN_79 */ ext_pins.RD_D.set(URUN_RDn_D);
    /* PIN_80 */ ext_pins.CS_A.set(TYHO_CSn_A);

    // internal address bus to external address bus
    {
      /*p08.ALOR*/ bus_reg.INT_ADDR_LATCH_00.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A00);
      /*p08.APUR*/ bus_reg.INT_ADDR_LATCH_01.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A01);
      /*p08.ALYR*/ bus_reg.INT_ADDR_LATCH_02.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A02);
      /*p08.ARET*/ bus_reg.INT_ADDR_LATCH_03.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A03);
      /*p08.AVYS*/ bus_reg.INT_ADDR_LATCH_04.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A04);
      /*p08.ATEV*/ bus_reg.INT_ADDR_LATCH_05.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A05);
      /*p08.AROS*/ bus_reg.INT_ADDR_LATCH_06.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A06);
      /*p08.ARYM*/ bus_reg.INT_ADDR_LATCH_07.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A07);
      /*p08.LUNO*/ bus_reg.INT_ADDR_LATCH_08.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A08);
      /*p08.LYSA*/ bus_reg.INT_ADDR_LATCH_09.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A09);
      /*p08.PATE*/ bus_reg.INT_ADDR_LATCH_10.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A10);
      /*p08.LUMY*/ bus_reg.INT_ADDR_LATCH_11.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A11);
      /*p08.LOBU*/ bus_reg.INT_ADDR_LATCH_12.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A12);
      /*p08.LONU*/ bus_reg.INT_ADDR_LATCH_13.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A13);
      /*p08.NYRE*/ bus_reg.INT_ADDR_LATCH_14.tp_latch(MATE_LATCH_INT_ADDR, cpu_pins.A14);
 
      /*p08.AMET*/ wire EXT_ADDR_00 = mux2(dma_reg.DMA_A00, bus_reg.INT_ADDR_LATCH_00, LUMA_DMA_READ_CART);
      /*p08.ATOL*/ wire EXT_ADDR_01 = mux2(dma_reg.DMA_A01, bus_reg.INT_ADDR_LATCH_01, LUMA_DMA_READ_CART);
      /*p08.APOK*/ wire EXT_ADDR_02 = mux2(dma_reg.DMA_A02, bus_reg.INT_ADDR_LATCH_02, LUMA_DMA_READ_CART);
      /*p08.AMER*/ wire EXT_ADDR_03 = mux2(dma_reg.DMA_A03, bus_reg.INT_ADDR_LATCH_03, LUMA_DMA_READ_CART);
      /*p08.ATEM*/ wire EXT_ADDR_04 = mux2(dma_reg.DMA_A04, bus_reg.INT_ADDR_LATCH_04, LUMA_DMA_READ_CART);
      /*p08.ATOV*/ wire EXT_ADDR_05 = mux2(dma_reg.DMA_A05, bus_reg.INT_ADDR_LATCH_05, LUMA_DMA_READ_CART);
      /*p08.ATYR*/ wire EXT_ADDR_06 = mux2(dma_reg.DMA_A06, bus_reg.INT_ADDR_LATCH_06, LUMA_DMA_READ_CART);
      /*p08.ASUR*/ wire EXT_ADDR_07 = mux2(dma_reg.DMA_A07, bus_reg.INT_ADDR_LATCH_07, LUMA_DMA_READ_CART);
      /*p08.MANO*/ wire EXT_ADDR_08 = mux2(dma_reg.DMA_A08, bus_reg.INT_ADDR_LATCH_08, LUMA_DMA_READ_CART);
      /*p08.MASU*/ wire EXT_ADDR_09 = mux2(dma_reg.DMA_A09, bus_reg.INT_ADDR_LATCH_09, LUMA_DMA_READ_CART);
      /*p08.PAMY*/ wire EXT_ADDR_10 = mux2(dma_reg.DMA_A10, bus_reg.INT_ADDR_LATCH_10, LUMA_DMA_READ_CART);
      /*p08.MALE*/ wire EXT_ADDR_11 = mux2(dma_reg.DMA_A11, bus_reg.INT_ADDR_LATCH_11, LUMA_DMA_READ_CART);
      /*p08.MOJY*/ wire EXT_ADDR_12 = mux2(dma_reg.DMA_A12, bus_reg.INT_ADDR_LATCH_12, LUMA_DMA_READ_CART);
      /*p08.MUCE*/ wire EXT_ADDR_13 = mux2(dma_reg.DMA_A13, bus_reg.INT_ADDR_LATCH_13, LUMA_DMA_READ_CART);
      /*p08.PEGE*/ wire EXT_ADDR_14 = mux2(dma_reg.DMA_A14, bus_reg.INT_ADDR_LATCH_14, LUMA_DMA_READ_CART);

      /*p08.KUPO*/ ext_pins.A00_A.set(nand(EXT_ADDR_00, TOVA_MODE_DBG2n));
      /*p08.CABA*/ ext_pins.A01_A.set(nand(EXT_ADDR_01, TOVA_MODE_DBG2n));
      /*p08.BOKU*/ ext_pins.A02_A.set(nand(EXT_ADDR_02, TOVA_MODE_DBG2n));
      /*p08.BOTY*/ ext_pins.A03_A.set(nand(EXT_ADDR_03, TOVA_MODE_DBG2n));
      /*p08.BYLA*/ ext_pins.A04_A.set(nand(EXT_ADDR_04, TOVA_MODE_DBG2n));
      /*p08.BADU*/ ext_pins.A05_A.set(nand(EXT_ADDR_05, TOVA_MODE_DBG2n));
      /*p08.CEPU*/ ext_pins.A06_A.set(nand(EXT_ADDR_06, TOVA_MODE_DBG2n));
      /*p08.DEFY*/ ext_pins.A07_A.set(nand(EXT_ADDR_07, TOVA_MODE_DBG2n));
      /*p08.MYNY*/ ext_pins.A08_A.set(nand(EXT_ADDR_08, TOVA_MODE_DBG2n));
      /*p08.MUNE*/ ext_pins.A09_A.set(nand(EXT_ADDR_09, TOVA_MODE_DBG2n));
      /*p08.ROXU*/ ext_pins.A10_A.set(nand(EXT_ADDR_10, TOVA_MODE_DBG2n));
      /*p08.LEPY*/ ext_pins.A11_A.set(nand(EXT_ADDR_11, TOVA_MODE_DBG2n));
      /*p08.LUCE*/ ext_pins.A12_A.set(nand(EXT_ADDR_12, TOVA_MODE_DBG2n));
      /*p08.LABE*/ ext_pins.A13_A.set(nand(EXT_ADDR_13, TOVA_MODE_DBG2n));
      /*p08.PUHE*/ ext_pins.A14_A.set(nand(EXT_ADDR_14, TOVA_MODE_DBG2n));
      /*p08.SUZE*/ ext_pins.A15_A.set(nand(TAZY_EXT_ADDR_15, RYCA_MODE_DBG2n)); // wat?

      /*p08.KOTY*/ ext_pins.A00_D.set(nor (EXT_ADDR_00, UNOR_MODE_DBG2));
      /*p08.COTU*/ ext_pins.A01_D.set(nor (EXT_ADDR_01, UNOR_MODE_DBG2));
      /*p08.BAJO*/ ext_pins.A02_D.set(nor (EXT_ADDR_02, UNOR_MODE_DBG2));
      /*p08.BOLA*/ ext_pins.A03_D.set(nor (EXT_ADDR_03, UNOR_MODE_DBG2));
      /*p08.BEVO*/ ext_pins.A04_D.set(nor (EXT_ADDR_04, UNOR_MODE_DBG2));
      /*p08.AJAV*/ ext_pins.A05_D.set(nor (EXT_ADDR_05, UNOR_MODE_DBG2));
      /*p08.CYKA*/ ext_pins.A06_D.set(nor (EXT_ADDR_06, UNOR_MODE_DBG2));
      /*p08.COLO*/ ext_pins.A07_D.set(nor (EXT_ADDR_07, UNOR_MODE_DBG2));
      /*p08.MEGO*/ ext_pins.A08_D.set(nor (EXT_ADDR_08, UNOR_MODE_DBG2));
      /*p08.MENY*/ ext_pins.A09_D.set(nor (EXT_ADDR_09, UNOR_MODE_DBG2));
      /*p08.RORE*/ ext_pins.A10_D.set(nor (EXT_ADDR_10, UNOR_MODE_DBG2));
      /*p08.LYNY*/ ext_pins.A11_D.set(nor (EXT_ADDR_11, UNOR_MODE_DBG2));
      /*p08.LOSO*/ ext_pins.A12_D.set(nor (EXT_ADDR_12, UNOR_MODE_DBG2));
      /*p08.LEVA*/ ext_pins.A13_D.set(nor (EXT_ADDR_13, UNOR_MODE_DBG2));
      /*p08.PAHY*/ ext_pins.A14_D.set(nor (EXT_ADDR_14, UNOR_MODE_DBG2));
      /*p08.RULO*/ ext_pins.A15_D.set(nor (TAZY_EXT_ADDR_15, UNOR_MODE_DBG2));
    }

    // internal data bus to external data bus
    {
      /*p08.LULA*/ ext_pins.D0_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D1_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D2_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D3_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D4_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D5_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D6_B.set(LULA_EXT_DATA_PINS_OUT);
      /*p08.LULA*/ ext_pins.D7_B.set(LULA_EXT_DATA_PINS_OUT);

      /*p25.RUXA*/ ext_pins.D0_A.set(nand(cpu_pins.D0, LULA_EXT_DATA_PINS_OUT));
      /*p25.RUJA*/ ext_pins.D1_A.set(nand(cpu_pins.D1, LULA_EXT_DATA_PINS_OUT));
      /*p25.RABY*/ ext_pins.D2_A.set(nand(cpu_pins.D2, LULA_EXT_DATA_PINS_OUT));
      /*p25.RERA*/ ext_pins.D3_A.set(nand(cpu_pins.D3, LULA_EXT_DATA_PINS_OUT));
      /*p25.RORY*/ ext_pins.D4_A.set(nand(cpu_pins.D4, LULA_EXT_DATA_PINS_OUT));
      /*p25.RYVO*/ ext_pins.D5_A.set(nand(cpu_pins.D5, LULA_EXT_DATA_PINS_OUT));
      /*p25.RAFY*/ ext_pins.D6_A.set(nand(cpu_pins.D6, LULA_EXT_DATA_PINS_OUT));
      /*p25.RAVU*/ ext_pins.D7_A.set(nand(cpu_pins.D7, LULA_EXT_DATA_PINS_OUT));

      /*p08.RUNE*/ ext_pins.D0_D.set(nor (cpu_pins.D0, RORU_EXT_DATA_PINS_OUTn));
      /*p08.RYPU*/ ext_pins.D1_D.set(nor (cpu_pins.D1, RORU_EXT_DATA_PINS_OUTn));
      /*p08.SULY*/ ext_pins.D2_D.set(nor (cpu_pins.D2, RORU_EXT_DATA_PINS_OUTn));
      /*p08.SEZE*/ ext_pins.D3_D.set(nor (cpu_pins.D3, RORU_EXT_DATA_PINS_OUTn));
      /*p08.RESY*/ ext_pins.D4_D.set(nor (cpu_pins.D4, RORU_EXT_DATA_PINS_OUTn));
      /*p08.TAMU*/ ext_pins.D5_D.set(nor (cpu_pins.D5, RORU_EXT_DATA_PINS_OUTn));
      /*p08.ROGY*/ ext_pins.D6_D.set(nor (cpu_pins.D6, RORU_EXT_DATA_PINS_OUTn));
      /*p08.RYDA*/ ext_pins.D7_D.set(nor (cpu_pins.D7, RORU_EXT_DATA_PINS_OUTn));
    }

    // external data bus to internal data bus
    {
      /*p08.SOMA*/ bus_reg.EXT_DATA_LATCH_00.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D0_C);
      /*p08.RONY*/ bus_reg.EXT_DATA_LATCH_01.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D1_C);
      /*p08.RAXY*/ bus_reg.EXT_DATA_LATCH_02.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D2_C);
      /*p08.SELO*/ bus_reg.EXT_DATA_LATCH_03.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D3_C);
      /*p08.SODY*/ bus_reg.EXT_DATA_LATCH_04.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D4_C);
      /*p08.SAGO*/ bus_reg.EXT_DATA_LATCH_05.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D5_C);
      /*p08.RUPA*/ bus_reg.EXT_DATA_LATCH_06.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D6_C);
      /*p08.SAZY*/ bus_reg.EXT_DATA_LATCH_07.tp_latch(LAVO_LATCH_EXT_DATA, ext_pins.D7_C);

      /*p08.RYMA*/ cpu_pins.D0.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_00);
      /*p08.RUVO*/ cpu_pins.D1.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_01);
      /*p08.RYKO*/ cpu_pins.D2.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_02);
      /*p08.TAVO*/ cpu_pins.D3.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_03);
      /*p08.TEPE*/ cpu_pins.D4.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_04);
      /*p08.SAFO*/ cpu_pins.D5.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_05);
      /*p08.SEVU*/ cpu_pins.D6.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_06);
      /*p08.TAJU*/ cpu_pins.D7.set_tribuf(!LAVO_LATCH_EXT_DATA, bus_reg.EXT_DATA_LATCH_07);
    }
  }



  //----------
  // OAM address generator

  /*p28.AJON*/ wire OAM_ADDR_RENDER     = and(DMA_RUNNINGn, vid_reg.XYMU_RENDERING_LATCH); // ppu can read oam when there's rendering but no dma

  if (1) {
    // cpu can read oam when there's no parsing, rendering, or dma
    // so byte 0 is the palette number? something wrong here...
    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GARO*/ wire A0n = not(cpu_pins.A00);
    /*p28.WACU*/ wire A1n = not(cpu_pins.A01);
    /*p28.GOSE*/ wire A2n = not(cpu_pins.A02);
    /*p28.WAPE*/ wire A3n = not(cpu_pins.A03);
    /*p28.FEVU*/ wire A4n = not(cpu_pins.A04);
    /*p28.GERA*/ wire A5n = not(cpu_pins.A05);
    /*p28.WAXA*/ wire A6n = not(cpu_pins.A06);
    /*p28.FOBY*/ wire A7n = not(cpu_pins.A07);

    /*p28.FYKE*/ wire IDX_0n = not(spr_reg.TS_IDX_0);
    /*p28.FUGU*/ wire IDX_1n = not(spr_reg.TS_IDX_1);
    /*p28.FACO*/ wire IDX_2n = not(spr_reg.TS_IDX_2);
    /*p28.FABY*/ wire IDX_3n = not(spr_reg.TS_IDX_3);
    /*p28.GYKA*/ wire IDX_4n = not(spr_reg.TS_IDX_4);
    /*p28.GYBU*/ wire IDX_5n = not(spr_reg.TS_IDX_5);

    /*p28.WUWE*/ wire SCANZn = not(joy_pins.P10_B);
    /*p28.GUSE*/ wire SCAN0n = not(spr_reg.SCAN0);
    /*p28.GEMA*/ wire SCAN1n = not(spr_reg.SCAN1);
    /*p28.FUTO*/ wire SCAN2n = not(spr_reg.SCAN2);
    /*p28.FAKU*/ wire SCAN3n = not(spr_reg.SCAN3);
    /*p28.GAMA*/ wire SCAN4n = not(spr_reg.SCAN4);
    /*p28.GOBY*/ wire SCAN5n = not(spr_reg.SCAN5);

    /*p28.FETU*/ wire DMA_A07n = not(dma_reg.DMA_A07);
    /*p28.FYDU*/ wire DMA_A06n = not(dma_reg.DMA_A06);
    /*p28.EDOL*/ wire DMA_A05n = not(dma_reg.DMA_A05);
    /*p28.ELUG*/ wire DMA_A04n = not(dma_reg.DMA_A04);
    /*p28.FYKY*/ wire DMA_A03n = not(dma_reg.DMA_A03);
    /*p28.FAGO*/ wire DMA_A02n = not(dma_reg.DMA_A02);
    /*p28.FESA*/ wire DMA_A01n = not(dma_reg.DMA_A01);
    /*p28.FODO*/ wire DMA_A00n = not(dma_reg.DMA_A00);

    /*p28.GEFY*/ wire GEFY = not(joy_pins.P10_B);
    /*p28.GECA*/ wire GECA = not(WEFE_P10_Bn);
    /*p28.WYDU*/ wire WYDU = not(WEFE_P10_Bn);

    /*p28.ASAM*/ wire OAM_ADDR_CPUn    = or(ACYL_OAM_ADDR_PARSE, vid_reg.XYMU_RENDERING_LATCH, dma_reg.REG_DMA_RUNNING);
    /*p28.BETE*/ wire OAM_ADDR_RENDERn = not(OAM_ADDR_RENDER);
    /*p28.APAR*/ wire OAM_ADDR_PARSEn  = not(ACYL_OAM_ADDR_PARSE);
    /*p04.DUGA*/ wire OAM_ADDR_DMAn    = not(dma_reg.REG_DMA_RUNNING); // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p28.GEKA*/ oam_pins.A0.set(not((A0n & !OAM_ADDR_CPUn) | (GECA   & !OAM_ADDR_RENDERn) | (GEFY   & !OAM_ADDR_PARSEn) | (DMA_A00n & !OAM_ADDR_DMAn)));
    /*p28.ZYFO*/ oam_pins.A1.set(not((A1n & !OAM_ADDR_CPUn) | (WYDU   & !OAM_ADDR_RENDERn) | (SCANZn & !OAM_ADDR_PARSEn) | (DMA_A01n & !OAM_ADDR_DMAn)));
    /*p28.YFOT*/ oam_pins.A2.set(not((A2n & !OAM_ADDR_CPUn) | (IDX_5n & !OAM_ADDR_RENDERn) | (SCAN0n & !OAM_ADDR_PARSEn) | (DMA_A02n & !OAM_ADDR_DMAn)));
    /*p28.YFOC*/ oam_pins.A3.set(not((A3n & !OAM_ADDR_CPUn) | (IDX_4n & !OAM_ADDR_RENDERn) | (SCAN1n & !OAM_ADDR_PARSEn) | (DMA_A03n & !OAM_ADDR_DMAn)));
    /*p28.YVOM*/ oam_pins.A4.set(not((A4n & !OAM_ADDR_CPUn) | (IDX_3n & !OAM_ADDR_RENDERn) | (SCAN2n & !OAM_ADDR_PARSEn) | (DMA_A04n & !OAM_ADDR_DMAn)));
    /*p28.YMEV*/ oam_pins.A5.set(not((A5n & !OAM_ADDR_CPUn) | (IDX_2n & !OAM_ADDR_RENDERn) | (SCAN3n & !OAM_ADDR_PARSEn) | (DMA_A05n & !OAM_ADDR_DMAn)));
    /*p28.XEMU*/ oam_pins.A6.set(not((A6n & !OAM_ADDR_CPUn) | (IDX_1n & !OAM_ADDR_RENDERn) | (SCAN4n & !OAM_ADDR_PARSEn) | (DMA_A06n & !OAM_ADDR_DMAn)));
    /*p28.YZET*/ oam_pins.A7.set(not((A7n & !OAM_ADDR_CPUn) | (IDX_0n & !OAM_ADDR_RENDERn) | (SCAN5n & !OAM_ADDR_PARSEn) | (DMA_A07n & !OAM_ADDR_DMAn)));
  }


  {
    /*p29.TUVO*/ wire SPR_OAM_RDn = or(LOBY_RENDERINGn, spr_reg.TULY_SPR_SEQ1, spr_reg.VONU_SEQ_xxx34xn); // TUVO is low for the first 4 phases of a sprite fetch
    /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!WUVU_AxxDExxH, !WOSU_xxCDxxGH);
    /*p29.XYSO*/ wire XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);  // oam
    /*p25.AVER*/ wire AVER = and(ACYL_OAM_ADDR_PARSE, XYSO_ABCxDEFx);
    /*p29.TYTU*/ wire SPR_SEQ0n = not(spr_reg.TOXE_SPR_SEQ0);
    /*p29.TACU*/ wire SPR_SEQ_5_TRIG = nand(spr_reg.TYFO_SEQ_B0d, SPR_SEQ0n);
    /*p25.VAPE*/ wire VAPE = and(SPR_OAM_RDn, SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire XUJY = not(VAPE); // XUJY seems to have a one-phase glitch?
    /*p25.CUFE*/ wire CUFE_OAM_WR = and(or(ADDR_OAM, dma_reg.REG_DMA_RUNNING), MOPA_AxxxxFGH); // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())
    /*p25.BYCU*/ wire OAM_IN_CLK = nand(AVER, XUJY, CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p25.COTA*/ oam_pins.CLK_A.set(not(OAM_IN_CLK));

    /*p28.AJEP*/ wire AJEP = and(ACYL_OAM_ADDR_PARSE, XOCE_ABxxEFxx);
    /*p28.WEFY*/ wire WEFY = and(SPR_OAM_RDn, spr_reg.TYFO_SEQ_B0d);
    /*p28.XUJA*/ wire XUJA = not(WEFY);
    /*p28.BOTA*/ wire CPU_RD_OAMn = nand(FROM_CPU5n, ADDR_OAM, ASOT_BUS_RD_ABCDEFGH); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire OAM_LATCHn = and (AJEP, XUJA, CPU_RD_OAMn);
    /*p28.BODE*/ wire OAM_LATCH  = not(OAM_LATCHn); // to the tribus receiver below
    /*p28.YVAL*/ wire OAM_LATCHo = not(OAM_LATCH);
    /*p28.YRYV*/ wire OAM_LATCHb = not(OAM_LATCHo);
    /*p28.ZODO*/ oam_pins.OE.set(not(OAM_LATCHb));

    /*p31.XYKY*/ oam_reg.LATCH_OAM_A0.tp_latch(OAM_LATCH, oam_pins.A_D0);
    /*p31.YRUM*/ oam_reg.LATCH_OAM_A1.tp_latch(OAM_LATCH, oam_pins.A_D1);
    /*p31.YSEX*/ oam_reg.LATCH_OAM_A2.tp_latch(OAM_LATCH, oam_pins.A_D2);
    /*p31.YVEL*/ oam_reg.LATCH_OAM_A3.tp_latch(OAM_LATCH, oam_pins.A_D3);
    /*p31.WYNO*/ oam_reg.LATCH_OAM_A4.tp_latch(OAM_LATCH, oam_pins.A_D4);
    /*p31.CYRA*/ oam_reg.LATCH_OAM_A5.tp_latch(OAM_LATCH, oam_pins.A_D5);
    /*p31.ZUVE*/ oam_reg.LATCH_OAM_A6.tp_latch(OAM_LATCH, oam_pins.A_D6);
    /*p31.ECED*/ oam_reg.LATCH_OAM_A7.tp_latch(OAM_LATCH, oam_pins.A_D7);

    /*p29.YDYV*/ oam_reg.LATCH_OAM_B0.tp_latch(OAM_LATCH, oam_pins.B_D0);
    /*p29.YCEB*/ oam_reg.LATCH_OAM_B1.tp_latch(OAM_LATCH, oam_pins.B_D1);
    /*p29.ZUCA*/ oam_reg.LATCH_OAM_B2.tp_latch(OAM_LATCH, oam_pins.B_D2);
    /*p29.WONE*/ oam_reg.LATCH_OAM_B3.tp_latch(OAM_LATCH, oam_pins.B_D3);
    /*p29.ZAXE*/ oam_reg.LATCH_OAM_B4.tp_latch(OAM_LATCH, oam_pins.B_D4);
    /*p29.XAFU*/ oam_reg.LATCH_OAM_B5.tp_latch(OAM_LATCH, oam_pins.B_D5);
    /*p29.YSES*/ oam_reg.LATCH_OAM_B6.tp_latch(OAM_LATCH, oam_pins.B_D6);
    /*p29.ZECA*/ oam_reg.LATCH_OAM_B7.tp_latch(OAM_LATCH, oam_pins.B_D7);

    /*p31.XEGA*/ wire OAM_A_IN_CLK = not(oam_pins.CLK_A);
    /*p31.YLOR*/ oam_reg.REG_OAM_A0.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A0);
    /*p31.ZYTY*/ oam_reg.REG_OAM_A1.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A1);
    /*p31.ZYVE*/ oam_reg.REG_OAM_A2.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A2);
    /*p31.ZEZY*/ oam_reg.REG_OAM_A3.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A3);
    /*p31.GOMO*/ oam_reg.REG_OAM_A4.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A4);
    /*p31.BAXO*/ oam_reg.REG_OAM_A5.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A5);
    /*p31.YZOS*/ oam_reg.REG_OAM_A6.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A6);
    /*p31.DEPO*/ oam_reg.REG_OAM_A7.set(OAM_A_IN_CLK, oam_reg.LATCH_OAM_A7);

    /*p29.YWOK*/ wire OAM_B_IN_CLK = not(oam_pins.CLK_A);
    /*p29.XUSO*/ oam_reg.REG_OAM_B0.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B0);
    /*p29.XEGU*/ oam_reg.REG_OAM_B1.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B1);
    /*p29.YJEX*/ oam_reg.REG_OAM_B2.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B2);
    /*p29.XYJU*/ oam_reg.REG_OAM_B3.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B3);
    /*p29.YBOG*/ oam_reg.REG_OAM_B4.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B4);
    /*p29.WYSO*/ oam_reg.REG_OAM_B5.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B5);
    /*p29.XOTE*/ oam_reg.REG_OAM_B6.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B6);
    /*p29.YZAB*/ oam_reg.REG_OAM_B7.set(OAM_B_IN_CLK, oam_reg.LATCH_OAM_B7);
  }

  {
    /*p28.BETE*/ wire OAM_ADDR_RENDERn    = not(OAM_ADDR_RENDER);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(dma_reg.REG_DMA_RUNNING, ACYL_OAM_ADDR_PARSE, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(ADDR_OAM, OAM_BUSYn);
    /*p04.NAXY*/ wire NAXY = nor(dma_reg.FROM_CPU5_SYNC, dma_reg.REG_DMA_EN_d0);
    /*p04.POWU*/ wire DMA_WRITE_OAM = and(dma_reg.REG_DMA_RUNNING, NAXY);
    /*p04.WYJA*/ wire WYJA_OAM_WR    = or(and(OAM_LOCKn, CUPA_BUS_WR_xxxxxFGH), DMA_WRITE_OAM);
    /*p28.WAFO*/ wire WAFO_OAM_A0n   = not(oam_pins.A0);
    /*p28.YNYC*/ wire YNYC_OAM_B_WRn = and(WYJA_OAM_WR, WAFO_OAM_A0n);
    /*p28.YLYC*/ wire YLYC_OAM_A_WRn = and(WYJA_OAM_WR, oam_pins.A0);
    /*p28.ZONE*/ oam_pins.WR_A.set(not(YLYC_OAM_A_WRn));
    /*p28.ZOFE*/ oam_pins.WR_B.set(not(YNYC_OAM_B_WRn));

    /*p28.XUTO*/ wire CPU_OAM_WR = and(ADDR_OAM, CUPA_BUS_WR_xxxxxFGH);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(MOPA_AxxxxFGH);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.ADAH*/ wire ADDR_OAMn = not(ADDR_OAM);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);
    /*p28.ZAXA*/ oam_pins.A_D0.set_tribuf(D_TO_OAMD, cpu_pins.D0);
    /*p28.ZAKY*/ oam_pins.A_D1.set_tribuf(D_TO_OAMD, cpu_pins.D1);
    /*p28.WULE*/ oam_pins.A_D2.set_tribuf(D_TO_OAMD, cpu_pins.D2);
    /*p28.ZOZO*/ oam_pins.A_D3.set_tribuf(D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUFO*/ oam_pins.A_D4.set_tribuf(D_TO_OAMD, cpu_pins.D4);
    /*p28.ZATO*/ oam_pins.A_D5.set_tribuf(D_TO_OAMD, cpu_pins.D5);
    /*p28.YVUC*/ oam_pins.A_D6.set_tribuf(D_TO_OAMD, cpu_pins.D6);
    /*p28.ZUFE*/ oam_pins.A_D7.set_tribuf(D_TO_OAMD, cpu_pins.D7);

    /*p28.ZAMY*/ oam_pins.B_D0.set_tribuf(D_TO_OAMD, cpu_pins.D0); // small tri
    /*p28.ZOPU*/ oam_pins.B_D1.set_tribuf(D_TO_OAMD, cpu_pins.D1);
    /*p28.WYKY*/ oam_pins.B_D2.set_tribuf(D_TO_OAMD, cpu_pins.D2);
    /*p28.ZAJA*/ oam_pins.B_D3.set_tribuf(D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUGA*/ oam_pins.B_D4.set_tribuf(D_TO_OAMD, cpu_pins.D4);
    /*p28.ZUMO*/ oam_pins.B_D5.set_tribuf(D_TO_OAMD, cpu_pins.D5);
    /*p28.XYTO*/ oam_pins.B_D6.set_tribuf(D_TO_OAMD, cpu_pins.D6);
    /*p28.ZYFA*/ oam_pins.B_D7.set_tribuf(D_TO_OAMD, cpu_pins.D7);

    /*p28.WUKU*/ wire OAM_A_CPU_RD = and(CPU_READ_MYSTERY, OAM_LOCKn, oam_pins.A0);
    /*p28.GUKO*/ wire OAM_B_CPU_RD = and(CPU_READ_MYSTERY, OAM_LOCKn, WAFO_OAM_A0n);
    /*p28.WEWU*/ wire OAM_A_CPU_RDn  = not(OAM_A_CPU_RD);
    /*p28.WUME*/ wire OAM_B_CPU_RDn  = not(OAM_B_CPU_RD);

    /*p31.XACA*/ cpu_pins.D0.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A0);
    /*p31.XAGU*/ cpu_pins.D1.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A1);
    /*p31.XEPU*/ cpu_pins.D2.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A2);
    /*p31.XYGU*/ cpu_pins.D3.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A3);
    /*p31.XUNA*/ cpu_pins.D4.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A4);
    /*p31.DEVE*/ cpu_pins.D5.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A5);
    /*p31.ZEHA*/ cpu_pins.D6.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A6);
    /*p31.FYRA*/ cpu_pins.D7.set_tribuf(!OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A7);

    /*p29.YFAP*/ cpu_pins.D0.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B0); // big tri
    /*p29.XELE*/ cpu_pins.D1.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B1);
    /*p29.YPON*/ cpu_pins.D2.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B2);
    /*p29.XUVO*/ cpu_pins.D3.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B3);
    /*p29.ZYSA*/ cpu_pins.D4.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B4);
    /*p29.YWEG*/ cpu_pins.D5.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B5);
    /*p29.XABU*/ cpu_pins.D6.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B6);
    /*p29.YTUX*/ cpu_pins.D7.set_tribuf(!OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B7);
  }
  
  {
    /*p28.AZAR*/ wire AZAR_DMA_READ_VRAMn = not(DMA_READ_VRAM);
    /*p28.WUZU*/ oam_pins.A_D0.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD0);
    /*p28.AXER*/ oam_pins.A_D1.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD1);
    /*p28.ASOX*/ oam_pins.A_D2.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD2);
    /*p28.CETU*/ oam_pins.A_D3.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD3);
    /*p28.ARYN*/ oam_pins.A_D4.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD4);
    /*p28.ACOT*/ oam_pins.A_D5.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD5);
    /*p28.CUJE*/ oam_pins.A_D6.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD6);
    /*p28.ATER*/ oam_pins.A_D7.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD7);

    /*p28.WOWA*/ oam_pins.B_D0.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD0);
    /*p28.AVEB*/ oam_pins.B_D1.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD1);
    /*p28.AMUH*/ oam_pins.B_D2.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD2);
    /*p28.COFO*/ oam_pins.B_D3.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD3);
    /*p28.AZOZ*/ oam_pins.B_D4.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD4);
    /*p28.AGYK*/ oam_pins.B_D5.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD5);
    /*p28.BUSE*/ oam_pins.B_D6.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD6);
    /*p28.ANUM*/ oam_pins.B_D7.set_tribuf(!AZAR_DMA_READ_VRAMn, vid_reg.MD7);
  }

  {
    /*p25.WEJO*/ oam_pins.A_D0.set_tribuf(!DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BUBO*/ oam_pins.A_D1.set_tribuf(!DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BETU*/ oam_pins.A_D2.set_tribuf(!DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CYME*/ oam_pins.A_D3.set_tribuf(!DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BAXU*/ oam_pins.A_D4.set_tribuf(!DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUHU*/ oam_pins.A_D5.set_tribuf(!DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BYNY*/ oam_pins.A_D6.set_tribuf(!DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BYPY*/ oam_pins.A_D7.set_tribuf(!DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));

    /*p25.WASA*/ oam_pins.B_D0.set_tribuf(!DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BOMO*/ oam_pins.B_D1.set_tribuf(!DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BASA*/ oam_pins.B_D2.set_tribuf(!DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CAKO*/ oam_pins.B_D3.set_tribuf(!DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BUMA*/ oam_pins.B_D4.set_tribuf(!DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUPY*/ oam_pins.B_D5.set_tribuf(!DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BASY*/ oam_pins.B_D6.set_tribuf(!DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BAPE*/ oam_pins.B_D7.set_tribuf(!DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));
  }

  //----------------------------------------
  // sch_bootrom

  cpu_pins.READ_BOOTROM.set(TUTU_ADDR_BOOT);

  /*[this, TEDO_CPURD, TAPU_CPUWR, ALUR_RSTn, ADDR_FFXX, TUTU_ADDR_BOOT, UMUT_MODE_DBG1]*/
  {

    /*p07.ZORO*/ wire ADDR_0XXX = nor(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12);
    /*p07.ZADU*/ wire ADDR_X0XX = nor(cpu_pins.A11, cpu_pins.A10, cpu_pins.A09, cpu_pins.A08);
    /*p07.ZUFA*/ wire ADDR_00XX2 = and(ADDR_0XXX, ADDR_X0XX);
    /*p07.YAZA*/ wire MODE_DBG1n = not(UMUT_MODE_DBG1);
    /*p07.YULA*/ wire BOOT_RD    = and(TEDO_BUS_RD, MODE_DBG1n, TUTU_ADDR_BOOT);
    /*p07.ZADO*/ wire BOOT_CSn   = nand(BOOT_RD, ADDR_00XX2);
    /*p07.ZERY*/ wire BOOT_CS    = not(BOOT_CSn);

    ///*p07.ZYBA*/ wire ADDR_00n = not(cpu_pins.A00);
    ///*p07.ZUVY*/ wire ADDR_01n = not(cpu_pins.A01);
    ///*p07.ZUFY*/ wire ADDR_04n = not(cpu_pins.A04);
    ///*p07.ZERA*/ wire ADDR_05n = not(cpu_pins.A05);
    ///*p07.ZOLE*/ wire ADDR_00  = and(ADDR_01n, ADDR_00n);
    ///*p07.ZAJE*/ wire ADDR_01  = and(ADDR_01n, cpu_pins.A00);
    ///*p07.ZUBU*/ wire ADDR_10  = and(cpu_pins.A01, ADDR_00n);
    ///*p07.ZAPY*/ wire ADDR_11  = and(cpu_pins.A01, cpu_pins.A00);

    ///*p07.ZETE*/ wire BOOTROM_A1nA0n = not(ADDR_00);
    ///*p07.ZEFU*/ wire BOOTROM_A1nA0  = not(ADDR_01);
    ///*p07.ZYRO*/ wire BOOTROM_A1A0n  = not(ADDR_10);
    ///*p07.ZAPA*/ wire BOOTROM_A1A0   = not(ADDR_11);
    ///*p07.ZYGA*/ wire BOOTROM_A2n    = not(cpu_pins.A02);
    ///*p07.ZYKY*/ wire BOOTROM_A3n    = not(cpu_pins.A03);
    ///*p07.ZYKY*/ wire BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
    ///*p07.ZYGA*/ wire BOOTROM_A5nA4  = and(ADDR_05n, cpu_pins.A04);
    ///*p07.ZOVY*/ wire BOOTROM_A5A4n  = and(cpu_pins.A05, ADDR_04n);
    ///*p07.ZUKO*/ wire BOOTROM_A5A4   = and(cpu_pins.A05, cpu_pins.A04);
    ///*p07.ZAGE*/ wire BOOTROM_A6n    = not(cpu_pins.A06);
    ///*p07.ZYRA*/ wire BOOTROM_A7n    = not(cpu_pins.A07);

    /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A03, cpu_pins.A02, cpu_pins.A01, cpu_pins.A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(cpu_pins.A04, cpu_pins.A06);
    /*p07.TEXE*/ wire FF50_RD  = and (TEDO_BUS_RD, ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
    /*p07.TUGE*/ wire FF50_WRn = nand(TAPU_BUS_WR_xxxxxFGH, ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
    /*p07.SATO*/ wire BOOT_BIT_IN  = or(cpu_pins.D0, bus_reg.BOOT_BIT);
  
    /*p07.TEPU*/ bus_reg.BOOT_BIT.set(FF50_WRn, ALUR_RSTn, BOOT_BIT_IN);

    /*p07.SYPU*/ cpu_pins.D0.set_tribuf(FF50_RD, bus_reg.BOOT_BIT);

    // how does this not collide with cart reads?

    uint16_t addr = (uint16_t)cpu_pins.get_addr();
    uint8_t data = DMG_ROM_bin[addr & 0xFF];

    cpu_pins.D0.set_tribuf(BOOT_CS && BOOT_RD, data & 0x01);
    cpu_pins.D1.set_tribuf(BOOT_CS && BOOT_RD, data & 0x02);
    cpu_pins.D2.set_tribuf(BOOT_CS && BOOT_RD, data & 0x04);
    cpu_pins.D3.set_tribuf(BOOT_CS && BOOT_RD, data & 0x08);
    cpu_pins.D4.set_tribuf(BOOT_CS && BOOT_RD, data & 0x10);
    cpu_pins.D5.set_tribuf(BOOT_CS && BOOT_RD, data & 0x20);
    cpu_pins.D6.set_tribuf(BOOT_CS && BOOT_RD, data & 0x40);
    cpu_pins.D7.set_tribuf(BOOT_CS && BOOT_RD, data & 0x80);
  }
  //();


  //----------------------------------------
  // interrupts

  if (1) {
    /*p07.SEMY*/ wire ADDR_XX0X = nor(cpu_pins.A07, cpu_pins.A06, cpu_pins.A05, cpu_pins.A04);
    /*p07.SAPA*/ wire ADDR_XXXF = and(cpu_pins.A00, cpu_pins.A01, cpu_pins.A02, cpu_pins.A03);
    /*p07.ROLO*/ wire FF0F_RDn = nand(ADDR_XX0X, ADDR_XXXF, ADDR_FFXX, TEDO_BUS_RD); // schematic wrong, is NAND
    /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);
    /*p07.REFA*/ wire FF0F_WRn = nand(ADDR_XX0X, ADDR_XXXF, ADDR_FFXX, TAPU_BUS_WR_xxxxxFGH); // schematic wrong, is NAND
    /*p02.ROTU*/ wire FF0F_WRa = not(FF0F_WRn);
    /*p02.PESU*/ wire FF0F_IN  = not(joy_pins.P10_B);

    {
#if 1
      // set and reset must be async
      // reset must take priority over set

      /*p02.LOPE*/ int_reg.FF0F_0.set(PARU_IN_VBLANK, 1);

      if (FF0F_WRa) {
        int_reg.FF0F_0.set_async(cpu_pins.D0);
      }

      if (!ALUR_RSTn || cpu_pins.ACK_VBLANK) {
        int_reg.FF0F_0.rst_async();
      }

#else

      /*p21.TOLU*/ wire INT_VBLn = not(PARU_IN_VBLANK);
      /*p02.LETY*/ wire INT_VBL_ACK  = not(cpu_pins.ACK_VBLANK);
      /*p21.VYPU*/ wire VYPU_INT_VBL  = not(INT_VBLn);
      /*p02.MUXE*/ wire MUXE = or(cpu_pins.D0, FF0F_WRn);
      /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa, INT_VBL_ACK,  cpu_pins.D0);
      /*p02.LYTA*/ wire FF0F_RST0 = and(MUXE, INT_VBL_ACK,  ALUR_RSTn);
      /*p02.LOPE*/ int_reg.FF0F_0.set(VYPU_INT_VBL,      FF0F_SET0, FF0F_RST0, FF0F_IN);
#endif
    }

    {
      /*p02.LEJA*/ wire INT_SER_ACK  = not(cpu_pins.ACK_STAT);
      /*p02.NABE*/ wire NABE = or(cpu_pins.D1, FF0F_WRn);
      /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa, INT_SER_ACK,  cpu_pins.D1);
      /*p02.MOVU*/ wire FF0F_RST1 = and(NABE, INT_SER_ACK,  ALUR_RSTn);
      /*p02.UBUL*/ int_reg.FF0F_1.set(ser_reg.SER_CNT3,  FF0F_SET1, FF0F_RST1, FF0F_IN);
    }

    {
      // this and timer int are swapped?
      /*p02.LESA*/ wire INT_JOY_ACK  = not(cpu_pins.ACK_TIMER);
      /*p02.RAKE*/ wire RAKE = or(cpu_pins.D2, FF0F_WRn);
      /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa, INT_JOY_ACK,  cpu_pins.D2);
      /*p02.PYGA*/ wire FF0F_RST2 = and(RAKE, INT_JOY_ACK,  ALUR_RSTn);
      /*p02.ASOK*/ wire INT_JP = and(joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);
      /*p02.ULAK*/ int_reg.FF0F_2.set(INT_JP,            FF0F_SET2, FF0F_RST2, FF0F_IN);
    }

    {
      // uhhhh probably not ack_serial here either? wtf did i do?
      /*p21.TOLU*/ wire INT_VBLn = not(PARU_IN_VBLANK);
      /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu_pins.ACK_SERIAL);
      /*p02.SULO*/ wire SULO = or(cpu_pins.D3, FF0F_WRn);
      /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, cpu_pins.D3);
      /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, ALUR_RSTn);
      /*p21.TAPA*/ wire TAPA_INT_OAM = and(INT_VBLn, SELA_NEW_LINE_d0);
      /*p21.TARU*/ wire TARU_INT_HBL = and(INT_VBLn, WODU_RENDER_DONE);
      /*p21.SUKO*/ wire INT_STATb = amux4(vid_reg.INT_LYC_EN, vid_reg.ROPO_LY_MATCH_SYNC,
                                          vid_reg.INT_OAM_EN, TAPA_INT_OAM,
                                          vid_reg.INT_VBL_EN, PARU_IN_VBLANK, // polarity?
                                          vid_reg.INT_HBL_EN, TARU_INT_HBL);
      /*p21.TUVA*/ wire TUVA_INT_STATn = not(INT_STATb);
      /*p21.VOTY*/ wire VOTY_INT_STAT  = not(TUVA_INT_STATn);
      /*p02.LALU*/ int_reg.FF0F_3.set(VOTY_INT_STAT,     FF0F_SET3, FF0F_RST3, FF0F_IN);
    }

    {
      /*p02.LAMO*/ wire INT_TIM_ACK  = not(cpu_pins.ACK_JOYPAD);
      /*p02.SEME*/ wire SEME = or(cpu_pins.D4, FF0F_WRn);
      /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa, INT_TIM_ACK,  cpu_pins.D4);
      /*p02.TYME*/ wire FF0F_RST4 = and(SEME, INT_TIM_ACK,  ALUR_RSTn);
      /*p02.NYBO*/ int_reg.FF0F_4.set(tim_reg.INT_TIMER, FF0F_SET4, FF0F_RST4, FF0F_IN);
    }

    {
      /*p02.MATY*/ int_reg.FF0F_L0.tp_latch(FF0F_RDn, int_reg.FF0F_0);
      /*p02.NEJY*/ int_reg.FF0F_L1.tp_latch(FF0F_RDn, int_reg.FF0F_1);
      /*p02.NUTY*/ int_reg.FF0F_L2.tp_latch(FF0F_RDn, int_reg.FF0F_2);
      /*p02.MOPO*/ int_reg.FF0F_L3.tp_latch(FF0F_RDn, int_reg.FF0F_3);
      /*p02.PAVY*/ int_reg.FF0F_L4.tp_latch(FF0F_RDn, int_reg.FF0F_4);

      /*p02.NELA*/ cpu_pins.D0.set_tribuf(FF0F_RDa, int_reg.FF0F_L0); // small tri
      /*p02.NABO*/ cpu_pins.D1.set_tribuf(FF0F_RDa, int_reg.FF0F_L1); // small tri
      /*p02.ROVA*/ cpu_pins.D2.set_tribuf(FF0F_RDa, int_reg.FF0F_L2);
      /*p02.PADO*/ cpu_pins.D3.set_tribuf(FF0F_RDa, int_reg.FF0F_L3);
      /*p02.PEGY*/ cpu_pins.D4.set_tribuf(FF0F_RDa, int_reg.FF0F_L4);
    }
  }

  cpu_pins.INT_VBLANK.set(int_reg.FF0F_0);
  cpu_pins.INT_SERIAL.set(int_reg.FF0F_1);
  cpu_pins.INT_JOYPAD.set(int_reg.FF0F_2);
  cpu_pins.INT_STAT.set(int_reg.FF0F_3);
  cpu_pins.INT_TIMER.set(int_reg.FF0F_4);
}



void TestGB::wtf_cpu_wr() {
  /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
  /*p07.UVAR*/ wire UVAR_T2n = not(sys_pins.T2);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, UBET_T1n);

  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not( clk_reg.PHAZ_xxxxEFGH);
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p07.TUNA*/ wire TUNA_0000_FDFF = nand(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09);
  /*p07.TONA*/ wire ADDR_08n = not(cpu_pins.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(TUNA_0000_FDFF, ADDR_08n);
  /*p03.TOVY*/ wire TOVY_A00n = not(cpu_pins.A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(cpu_pins.A01);
  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(cpu_pins.A07, cpu_pins.A06, cpu_pins.A05, cpu_pins.A04, cpu_pins.A03);
  /*p03.RYFO*/ wire FF04_FF07 = and(cpu_pins.A02, ADDR_XX00_XX07, ADDR_FFXX);
  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH  = not(AREV_CPU_WRn_ABCDExxx);
  /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH, UNOR_MODE_DBG2);
  /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
  /*p01.TAPE*/ wire FF04_WR = and(TAPU_BUS_WR_xxxxxFGH, FF04_FF07, TOLA_A01n, TOVY_A00n);
}




void TestGB::tick_timer(wire ALUR_RSTn, wire BOGA_AxCDEFGH) {



  //----------------------------------------
  // Timer

  /*p01.ATYP*/ wire ATYP_xBCDExxx =  clk_reg.PHAZ_xBCDExxx;
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = !clk_reg.PHAZ_xxxxEFGH;
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);

  /*p01.AREV*/ wire AREV_ABCDExxx_WRn = cpu_pins.CPU_RAW_WR ? !AFAS_xxxxxFGH : 1;

  // something wrong here, the sense of these is inverted...

  /*p01.TAPE*/ wire FF04_WR = and(AREV_ABCDExxx_WRn, cpu_pins.get_addr() == 0xFF04);
  /*p03.TOPE*/ wire FF05_WR = and(AREV_ABCDExxx_WRn, cpu_pins.get_addr() == 0xFF05);
  /*p03.TYJU*/ wire FF06_WR = and(AREV_ABCDExxx_WRn, cpu_pins.get_addr() == 0xFF06);
  /*p03.SARA*/ wire FF07_WR = and(AREV_ABCDExxx_WRn, cpu_pins.get_addr() == 0xFF07);

  {
    /*p01.UKUP*/ tim_reg.DIV_00.set(BOGA_AxCDEFGH,   !FF04_WR, !tim_reg.DIV_00);
    /*p01.UFOR*/ tim_reg.DIV_01.set(!tim_reg.DIV_00, !FF04_WR, !tim_reg.DIV_01);
    /*p01.UNER*/ tim_reg.DIV_02.set(!tim_reg.DIV_01, !FF04_WR, !tim_reg.DIV_02);
    /*p01.TERO*/ tim_reg.DIV_03.set(!tim_reg.DIV_02, !FF04_WR, !tim_reg.DIV_03);
    /*p01.UNYK*/ tim_reg.DIV_04.set(!tim_reg.DIV_03, !FF04_WR, !tim_reg.DIV_04);
    /*p01.TAMA*/ tim_reg.DIV_05.set(!tim_reg.DIV_04, !FF04_WR, !tim_reg.DIV_05);
    /*p01.UGOT*/ tim_reg.DIV_06.set(!tim_reg.DIV_05, !FF04_WR, !tim_reg.DIV_06);
    /*p01.TULU*/ tim_reg.DIV_07.set(!tim_reg.DIV_06, !FF04_WR, !tim_reg.DIV_07);
    /*p01.TUGO*/ tim_reg.DIV_08.set(!tim_reg.DIV_07, !FF04_WR, !tim_reg.DIV_08);
    /*p01.TOFE*/ tim_reg.DIV_09.set(!tim_reg.DIV_08, !FF04_WR, !tim_reg.DIV_09);
    /*p01.TERU*/ tim_reg.DIV_10.set(!tim_reg.DIV_09, !FF04_WR, !tim_reg.DIV_10);
    /*p01.SOLA*/ tim_reg.DIV_11.set(!tim_reg.DIV_10, !FF04_WR, !tim_reg.DIV_11);
    /*p01.SUBU*/ tim_reg.DIV_12.set(!tim_reg.DIV_11, !FF04_WR, !tim_reg.DIV_12);
    /*p01.TEKA*/ tim_reg.DIV_13.set(!tim_reg.DIV_12, !FF04_WR, !tim_reg.DIV_13);
    /*p01.UKET*/ tim_reg.DIV_14.set(!tim_reg.DIV_13, !FF04_WR, !tim_reg.DIV_14);
    /*p01.UPOF*/ tim_reg.DIV_15.set(!tim_reg.DIV_14, !FF04_WR, !tim_reg.DIV_15);
  }

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(!tim_reg.DIV_05, !tim_reg.DIV_03,  tim_reg.TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(!tim_reg.DIV_01, !tim_reg.DIV_07,  tim_reg.TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa,        CLK_MUXb,         tim_reg.TAC_1);
  /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !tim_reg.TAC_2);

  if (FF05_WR) {
    /*p03.MEXU*/ wire TIMA_LOAD  = or(!cpu_pins.FROM_CPU5, tim_reg.INT_TIMER);

    /*p03.REGA*/ tim_reg.TIMA_0.tima_count(TIMA_CLK,           TIMA_LOAD, cpu_pins.D0);
    /*p03.POVY*/ tim_reg.TIMA_1.tima_count(tim_reg.TIMA_0.c(), TIMA_LOAD, cpu_pins.D1);
    /*p03.PERU*/ tim_reg.TIMA_2.tima_count(tim_reg.TIMA_1.c(), TIMA_LOAD, cpu_pins.D2);
    /*p03.RATE*/ tim_reg.TIMA_3.tima_count(tim_reg.TIMA_2.c(), TIMA_LOAD, cpu_pins.D3);
    /*p03.RUBY*/ tim_reg.TIMA_4.tima_count(tim_reg.TIMA_3.c(), TIMA_LOAD, cpu_pins.D4);
    /*p03.RAGE*/ tim_reg.TIMA_5.tima_count(tim_reg.TIMA_4.c(), TIMA_LOAD, cpu_pins.D5);
    /*p03.PEDA*/ tim_reg.TIMA_6.tima_count(tim_reg.TIMA_5.c(), TIMA_LOAD, cpu_pins.D6);
    /*p03.NUGA*/ tim_reg.TIMA_7.tima_count(tim_reg.TIMA_6.c(), TIMA_LOAD, cpu_pins.D7);

    /*p03.NYDU*/ tim_reg.TIMA_MAX.set(BOGA_AxCDEFGH, !TIMA_LOAD, tim_reg.TIMA_7.c());
  }
  else {
    /*p03.MEXU*/ wire TIMA_LOAD  = tim_reg.INT_TIMER;

    /*p03.REGA*/ tim_reg.TIMA_0.tima_count(TIMA_CLK,           TIMA_LOAD, tim_reg.TMA_0);
    /*p03.POVY*/ tim_reg.TIMA_1.tima_count(tim_reg.TIMA_0.c(), TIMA_LOAD, tim_reg.TMA_1);
    /*p03.PERU*/ tim_reg.TIMA_2.tima_count(tim_reg.TIMA_1.c(), TIMA_LOAD, tim_reg.TMA_2);
    /*p03.RATE*/ tim_reg.TIMA_3.tima_count(tim_reg.TIMA_2.c(), TIMA_LOAD, tim_reg.TMA_3);
    /*p03.RUBY*/ tim_reg.TIMA_4.tima_count(tim_reg.TIMA_3.c(), TIMA_LOAD, tim_reg.TMA_4);
    /*p03.RAGE*/ tim_reg.TIMA_5.tima_count(tim_reg.TIMA_4.c(), TIMA_LOAD, tim_reg.TMA_5);
    /*p03.PEDA*/ tim_reg.TIMA_6.tima_count(tim_reg.TIMA_5.c(), TIMA_LOAD, tim_reg.TMA_6);
    /*p03.NUGA*/ tim_reg.TIMA_7.tima_count(tim_reg.TIMA_6.c(), TIMA_LOAD, tim_reg.TMA_7);

    /*p03.NYDU*/ tim_reg.TIMA_MAX.set(BOGA_AxCDEFGH, !TIMA_LOAD, tim_reg.TIMA_7.c());
  }

  /*p03.MOBA*/ tim_reg.INT_TIMER.set(BOGA_AxCDEFGH, ALUR_RSTn, and(tim_reg.TIMA_MAX, !tim_reg.TIMA_7.c()));

  /*p03.SABU*/ tim_reg.TMA_0.set(!FF06_WR, 1, cpu_pins.D0);
  /*p03.NYKE*/ tim_reg.TMA_1.set(!FF06_WR, 1, cpu_pins.D1);
  /*p03.MURU*/ tim_reg.TMA_2.set(!FF06_WR, 1, cpu_pins.D2);
  /*p03.TYVA*/ tim_reg.TMA_3.set(!FF06_WR, 1, cpu_pins.D3);
  /*p03.TYRU*/ tim_reg.TMA_4.set(!FF06_WR, 1, cpu_pins.D4);
  /*p03.SUFY*/ tim_reg.TMA_5.set(!FF06_WR, 1, cpu_pins.D5);
  /*p03.PETO*/ tim_reg.TMA_6.set(!FF06_WR, 1, cpu_pins.D6);
  /*p03.SETA*/ tim_reg.TMA_7.set(!FF06_WR, 1, cpu_pins.D7);

  /*p03.SOPU*/ tim_reg.TAC_0.set(!FF07_WR, 1, cpu_pins.D0);
  /*p03.SAMY*/ tim_reg.TAC_1.set(!FF07_WR, 1, cpu_pins.D1);
  /*p03.SABO*/ tim_reg.TAC_2.set(!FF07_WR, 1, cpu_pins.D2);

  //----------

  /*p01.TAGY*/ wire FF04_RD = and(not(cpu_pins.CPU_RAW_RD), cpu_pins.get_addr() == 0xFF04);
  /*p03.TEDA*/ wire FF05_RD = and(not(cpu_pins.CPU_RAW_RD), cpu_pins.get_addr() == 0xFF05);
  /*p03.TUBY*/ wire FF06_RD = and(not(cpu_pins.CPU_RAW_RD), cpu_pins.get_addr() == 0xFF06);
  /*p03.SORA*/ wire FF07_RD = and(not(cpu_pins.CPU_RAW_RD), cpu_pins.get_addr() == 0xFF07);
  
  /*p01.TAWU*/ cpu_pins.D0.set_tribuf(FF04_RD, tim_reg.DIV_06);
  /*p01.TAKU*/ cpu_pins.D1.set_tribuf(FF04_RD, tim_reg.DIV_07);
  /*p01.TEMU*/ cpu_pins.D2.set_tribuf(FF04_RD, tim_reg.DIV_08);
  /*p01.TUSE*/ cpu_pins.D3.set_tribuf(FF04_RD, tim_reg.DIV_09);
  /*p01.UPUG*/ cpu_pins.D4.set_tribuf(FF04_RD, tim_reg.DIV_10);
  /*p01.SEPU*/ cpu_pins.D5.set_tribuf(FF04_RD, tim_reg.DIV_11);
  /*p01.SAWA*/ cpu_pins.D6.set_tribuf(FF04_RD, tim_reg.DIV_12);
  /*p01.TATU*/ cpu_pins.D7.set_tribuf(FF04_RD, tim_reg.DIV_13);

  /*p03.SOKU*/ cpu_pins.D0.set_tribuf(FF05_RD, tim_reg.TIMA_0.v());
  /*p03.RACY*/ cpu_pins.D1.set_tribuf(FF05_RD, tim_reg.TIMA_1.v());
  /*p03.RAVY*/ cpu_pins.D2.set_tribuf(FF05_RD, tim_reg.TIMA_2.v());
  /*p03.SOSY*/ cpu_pins.D3.set_tribuf(FF05_RD, tim_reg.TIMA_3.v());
  /*p03.SOMU*/ cpu_pins.D4.set_tribuf(FF05_RD, tim_reg.TIMA_4.v());
  /*p03.SURO*/ cpu_pins.D5.set_tribuf(FF05_RD, tim_reg.TIMA_5.v());
  /*p03.ROWU*/ cpu_pins.D6.set_tribuf(FF05_RD, tim_reg.TIMA_6.v());
  /*p03.PUSO*/ cpu_pins.D7.set_tribuf(FF05_RD, tim_reg.TIMA_7.v());

  /*p03.SETE*/ cpu_pins.D0.set_tribuf(FF06_RD, tim_reg.TMA_0);
  /*p03.PYRE*/ cpu_pins.D1.set_tribuf(FF06_RD, tim_reg.TMA_1);
  /*p03.NOLA*/ cpu_pins.D2.set_tribuf(FF06_RD, tim_reg.TMA_2);
  /*p03.SALU*/ cpu_pins.D3.set_tribuf(FF06_RD, tim_reg.TMA_3);
  /*p03.SUPO*/ cpu_pins.D4.set_tribuf(FF06_RD, tim_reg.TMA_4);
  /*p03.SOTU*/ cpu_pins.D5.set_tribuf(FF06_RD, tim_reg.TMA_5);
  /*p03.REVA*/ cpu_pins.D6.set_tribuf(FF06_RD, tim_reg.TMA_6);
  /*p03.SAPU*/ cpu_pins.D7.set_tribuf(FF06_RD, tim_reg.TMA_7);

  /*p03.RYLA*/ cpu_pins.D0.set_tribuf(FF07_RD, tim_reg.TAC_0);
  /*p03.ROTE*/ cpu_pins.D1.set_tribuf(FF07_RD, tim_reg.TAC_1);
  /*p03.SUPE*/ cpu_pins.D2.set_tribuf(FF07_RD, tim_reg.TAC_2);
}