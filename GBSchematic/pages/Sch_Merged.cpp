#include "Sch_ApuControl.h"
#include "Sch_Bootrom.h"
#include "Sch_BusControl.h"
#include "Sch_Cart.h"
#include "Sch_Channel1.h"
#include "Sch_Channel2.h"
#include "Sch_Channel3.h"
#include "Sch_Channel4.h"
#include "Sch_Clocks.h"
#include "Sch_Common.h"
#include "Sch_CpuPins.h"
#include "Sch_Debug.h"
#include "Sch_Decoder.h"
#include "Sch_DMA.h"
#include "Sch_Interrupts.h"
#include "Sch_Joypad.h"
#include "Sch_LCD.h"
#include "Sch_OAM.h"
#include "Sch_Pins.h"
#include "Sch_PixelPipe.h"
#include "Sch_Ram.h"
#include "Sch_Registers.h"
#include "Sch_Resets.h"
#include "Sch_Serial.h"
#include "Sch_Sprites.h"
#include "Sch_SpriteStore.h"
#include "Sch_System.h"
#include "Sch_Timer.h"
#include "Sch_Video.h"
#include "Sch_Vram.h"

using namespace Schematics;

struct ExtPins {
  bool CLK;
  bool CLK_GOOD;
  bool RST;
  bool T1;
  bool T2;
};


static ApuControl apu;
static Channel1 ch1;
static Channel2 ch2;
static Channel3 ch3;
static Channel4 ch4;
static ClkRegisters clk_reg;
static ConfigRegisters cfg_reg;
static DebugRegisters dbg_reg;
static DmaRegisters dma_reg;
static InterruptRegisters int_reg;
static JoypadRegisters joy_reg;
static LcdRegisters lcd_reg;
static OamRegisters oam_reg;
static PixelPipeRegisters pxp_reg;
static RstRegisters rst_reg;
static SerialRegisters ser_reg;
static SpriteRegisters spr_reg;
static SpriteStoreRegisters sst_reg;
static SysRegisters sys_reg;
static TimerRegisters tim_reg;
static VclkRegisters vclk_reg;
static VidRegisters vid_reg;

static Bootrom bootrom;
static Ram8K ram;
static Ram8K vram;

static ExtPins ext_pins;
static VramPins vram_pins;
static SerialPins ser_pins;
static LcdPins lcd_pins;
static JoypadPins joy_pins;
static CpuPins cpu_pins;
static CartPins cart_pins;
static OamPins oam_pins;

static VramBus vram_bus;

static BusTristates bus_tri;
static SpriteTristate sil_tri;

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

void tick_everything() {
  // sch_system

  /*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  /*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;
  /*p01.ANOS*/ wire ANOS_xBxDxFxH = not(ext_pins.CLK);
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(ANOS_xBxDxFxH);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_pins.CLKREQ);
  /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);
  /*p01.UCOB*/ wire UCOB_CLKBAD   = not(ext_pins.CLK_GOOD);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(ext_pins.CLK_GOOD);

  /*p07.UBET*/ wire UBET_T1n = not(ext_pins.T1);
  /*p07.UVAR*/ wire UVAR_T2n = not(ext_pins.T2);
  /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, ext_pins.RST);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1 = and(ext_pins.T1, UVAR_T2n);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(ext_pins.T2, UBET_T1n);
  /*p28.WEFE*/ wire WEFE_P10_Bn = not(joy_pins.P10_B);

  // sch_clocks

  /*p01.AFUR*/ clk_reg.PHAZ_ABCDxxxx.set(ATAL_AxCxExGx, UPOJ_MODE_PROD, !clk_reg.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ clk_reg.PHAZ_xBCDExxx.set(ATAL_AxCxExGx, UPOJ_MODE_PROD,  clk_reg.PHAZ_ABCDxxxx);
  /*p01.APUK*/ clk_reg.PHAZ_xxCDEFxx.set(ATAL_AxCxExGx, UPOJ_MODE_PROD,  clk_reg.PHAZ_xBCDExxx);
  /*p01.ADYK*/ clk_reg.PHAZ_xxxDEFGx.set(ATAL_AxCxExGx, UPOJ_MODE_PROD,  clk_reg.PHAZ_xxCDEFxx);

  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_xBxDxFxH); // dma, sprite store
  /*p01.ALET*/ wire ALET_AxCxExGx = not(ZEME_xBxDxFxH); // video, sprites

  /*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_xBxDxFxH);
  /*p01.AMUK*/ wire AMUK_xBxDxFxH = not(ATAG_xBxDxFxH); // apu master 4m clock

  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( clk_reg.PHAZ_xBCDExxx);
  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!clk_reg.PHAZ_ABCDxxxx);
  /*p01.AROV*/ wire AROV_xxCDEFxx = not(!clk_reg.PHAZ_xxCDEFxx);
  /*p01.AJAX*/ wire AJAX_xxxxEFGH = not(ATYP_ABCDxxxx);

  /*p01.ADAR*/ wire ADAR_ABCxxxxH = not(clk_reg.PHAZ_xxxDEFGx);
  /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  // joypad, reset, timer, debug clock
  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);

  // gated on BUTY_CLKREQ
  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(ABOL_CLKREQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.DOVA*/ wire DOVA_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ wire UVYT_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ wire MOPA_xxxxEFGH = not(UVYT_ABCDxxxx);

  // BAZE here seems incongruous
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, ATEZ_CLKBAD);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ wire BOGA_xBCDEFGH = not(BALY_Axxxxxxx);

  /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(BUTY_CLKREQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ wire BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);

  // wave ram write clock
  /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
  /*p01.BATE*/ wire BATE_AxxxxxGH = nor(ABOL_CLKREQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
  /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
  /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
  /*p17.ABUR*/ wire ABUR_xBCDEFxx = not(BUKE_AxxxxxGH);
  /*p17.BORY*/ wire BORY_AxxxxxGH = not(ABUR_xBCDEFxx);

  // sch_resets

  /*p01.UPYF*/ wire UPYF        = or(ext_pins.RST, UCOB_CLKBAD);
  /*p01.BOMA*/ wire RESET_CLK   = not(BOGA_xBCDEFGH);
  /*p01.UNUT*/ wire TIMEOUT     = and(rst_reg.WAITING_FOR_CLKREQ, tim_reg.DIV_15);
  /*p01.TABA*/ wire CPU_RESET   = or(UNOR_MODE_DBG2, UMUT_MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);

  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, ext_pins.RST);
  /*p01.ASOL*/ wire RESET_IN    = or (AFAR, ext_pins.RST);
  /*p01.AVOR*/ wire AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);

  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);
  /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ wire VID_RESET   = nand(XEBE_RESET, cfg_reg.LCDC_EN);
  /*p01.XAPO*/ wire VID_RESETn  = not(VID_RESET);

  /*p01.TUBO*/ rst_reg.WAITING_FOR_CLKREQ.sr_latch(!UPYF, !ABOL_CLKREQn);
  /*p01.AFER*/ rst_reg.RESET_REG.set(RESET_CLK, UPOJ_MODE_PROD, RESET_IN);

  // sch_clocks

  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_xBxDxFxH);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p29.WUVU*/ vclk_reg.WUVU_AxxDExxH.set(          XOTA_xBxDxFxH, VID_RESETn, !vclk_reg.WUVU_AxxDExxH);
  /*p21.VENA*/ vclk_reg.VENA_xBCDExxx.set(!vclk_reg.WUVU_AxxDExxH, VID_RESETn, !vclk_reg.VENA_xBCDExxx);
  /*p29.WOSU*/ vclk_reg.WOSU_xxCDxxGH.set(          XYFY_AxCxExGx, VID_RESETn, !vclk_reg.WUVU_AxxDExxH);

  /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!vclk_reg.WUVU_AxxDExxH, !vclk_reg.WOSU_xxCDxxGH);
  /*p29.XUPY*/ wire XUPY_xBCxxFGx = not(vclk_reg.WUVU_AxxDExxH);  // lcd, sprites
  /*p28.AWOH*/ wire AWOH_AxxDExxH = not(XUPY_xBCxxFGx);  // lcd
  /*p21.TALU*/ wire TALU_xBCDExxx = not(!vclk_reg.VENA_xBCDExxx); // this drives the LCD xy counter
  /*p21.SONO*/ wire SONO_AxxxxFGH = not(TALU_xBCDExxx);  // lcd
  /*p29.XOCE*/ wire XOCE_ABxxEFxx = not(vclk_reg.WOSU_xxCDxxGH);  // oam, sprites
  /*p29.XYSO*/ wire XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);  // oam

  // sch_decoder

  /*p10.AMUS*/ wire ADDR_0xx00000 = nor(bus_tri.A00(), bus_tri.A01(), bus_tri.A02(), bus_tri.A03(), bus_tri.A04(), bus_tri.A07());
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus_tri.A15(), bus_tri.A14(), bus_tri.A13(), bus_tri.A12(), bus_tri.A11(), bus_tri.A10(), bus_tri.A09());
  /*p07.TONA*/ wire ADDR_08n = not(bus_tri.A08());
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_0xx00000, ADDR_FFXX);

  /*p22.XOLA*/ wire A00n = not(bus_tri.A00());
  /*p22.XENO*/ wire A01n = not(bus_tri.A01());
  /*p22.XUSY*/ wire A02n = not(bus_tri.A02());
  /*p22.XERA*/ wire A03n = not(bus_tri.A03());
  /*p22.XALY*/ wire ADDR_0x00xxxx  = nor(bus_tri.A07(), bus_tri.A05(), bus_tri.A04());
  /*p22.WUTU*/ wire FF4Xn          = nand(ADDR_FFXX, bus_tri.A06(), ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X           = not(FF4Xn);

  /*p07.SEMY*/ wire ADDR_XX0X = nor(bus_tri.A07(), bus_tri.A06(), bus_tri.A05(), bus_tri.A04());
  /*p07.SAPA*/ wire ADDR_XXXF = and(bus_tri.A00(), bus_tri.A01(), bus_tri.A02(), bus_tri.A03());

  /*p22.WADO*/ wire WADO_A00 = not(A00n);
  /*p22.WESA*/ wire WESA_A01 = not(A01n);
  /*p22.WALO*/ wire WALO_A02 = not(A02n);
  /*p22.WEPO*/ wire WEPO_A03 = not(A03n);

  /*p22.WORU*/ wire FF40n = nand(FF4X, A00n, A01n, A02n, A03n);
  /*p22.WOFA*/ wire FF41n = nand(FF4X, WADO_A00, A01n, A02n, A03n);
  /*p22.WEBU*/ wire FF42n = nand(FF4X, A00n, bus_tri.A01(), A02n, A03n);
  /*p22.WAVU*/ wire FF43n = nand(FF4X, bus_tri.A00(), bus_tri.A01(), A02n, A03n);
  /*p22.WYLE*/ wire FF44n = nand(FF4X, A00n, A01n, WALO_A02,  A03n);
  /*p22.WETY*/ wire FF45n = nand(FF4X, bus_tri.A00(), A01n, bus_tri.A02(), A03n);
  /*p22.WATE*/ wire FF46n = nand(FF4X, A00n, WESA_A01, WALO_A02, A03n);
  /*p22.WYBO*/ wire FF47n = nand(FF4X, bus_tri.A00(), bus_tri.A01(), bus_tri.A02(), A03n);
  /*p22.WETA*/ wire FF48n = nand(FF4X, A00n, A01n, A02n, bus_tri.A03());
  /*p22.VAMA*/ wire FF49n = nand(FF4X, bus_tri.A00(), A01n, A02n, bus_tri.A03());
  /*p22.WYVO*/ wire FF4An = nand(FF4X, A00n, bus_tri.A01(), A02n, bus_tri.A03());
  /*p22.WAGE*/ wire FF4Bn = nand(FF4X, bus_tri.A00(), bus_tri.A01(), A02n, bus_tri.A03());

  /*p22.VOCA*/ wire FF40 = not(FF40n);
  /*p22.VARY*/ wire FF41 = not(FF41n);
  /*p22.XARO*/ wire FF42 = not(FF42n);
  /*p22.XAVY*/ wire FF43 = not(FF43n);
  /*p22.XOGY*/ wire FF44 = not(FF44n);
  /*p22.XAYU*/ wire FF45 = not(FF45n);
  /*p22.XEDA*/ wire FF46 = not(FF46n);
  /*p22.WERA*/ wire FF47 = not(FF47n);
  /*p22.XAYO*/ wire FF48 = not(FF48n);
  /*p22.TEGO*/ wire FF49 = not(FF49n);
  /*p22.VYGA*/ wire FF4A = not(FF4An);
  /*p22.VUMY*/ wire FF4B = not(FF4Bn);

  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p07.SOHA*/ wire ADDR_FFXXn2 = not(ADDR_FFXX);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ wire ADDR_OAM = not(ADDR_FEXXn);

  /*p01.AGUT*/ wire AGUT_xxCDEFGH = and(or(AJAX_xxxxEFGH, AROV_xxCDEFxx), cpu_pins.ADDR_VALID);
  /*p01.AWOD*/ wire AWOD = or(UNOR_MODE_DBG2, AGUT_xxCDEFGH);
  /*p01.ABUZ*/ wire ADDR_VALID_xBCxxxxx = not(AWOD);

  /*p03.TOVY*/ wire TOVY_A00n = not(bus_tri.A00());
  /*p08.TOLA*/ wire TOLA_A01n = not(bus_tri.A01());
  /*p06.SEFY*/ wire SEFY_A02n = not(bus_tri.A02());

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(bus_tri.A07(), bus_tri.A06(), bus_tri.A05(), bus_tri.A04(), bus_tri.A03());
  /*p06.SANO*/ wire ADDR_FF00_FF03 = and(ADDR_XX00_XX07, SEFY_A02n, ADDR_FFXX);

  /*p03.RYFO*/ wire FF04_FF07 = and(bus_tri.A02(), ADDR_XX00_XX07, ADDR_FFXX);

  // sch_buscontrol

  /*p01.AREV*/ wire AREV = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx = not(AREV);

  /*p07.UJYV*/ wire CPU_RD_MUX = mux2n(cart_pins.RDn_C, cpu_pins.CPU_RAW_RD, UNOR_MODE_DBG2);
  /*p07.UBAL*/ wire CPU_WR_MUX = mux2n(cart_pins.WRn_C, CPU_WR_xxxxEFGx, UNOR_MODE_DBG2);

  /*p07.TEDO*/ wire TEDO_CPURD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ wire TAPU_CPUWR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(TEDO_CPURD);
  /*p07.DYKY*/ wire CPU_WRn = not(TAPU_CPUWR);

  /*p07.ASOT*/ wire ASOT_CPURD = not(CPU_RDn);
  /*p07.CUPA*/ wire CUPA_CPUWR = not(CPU_WRn);

  /*p08.SORE*/ wire ADDR_0000_7FFF = not(bus_tri.A15());
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(bus_tri.A13(), bus_tri.A14(), ADDR_0000_7FFF);
  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_pins.ADDR_VALID, ADDR_NOT_VRAM);
  /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);

  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, UMUT_MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(cpu_pins.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), cpu_pins.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);

  /*p08.REDU*/ wire CPU_RDo = not(TEDO_CPURD);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
  /*p08.RORU*/ wire CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, UNOR_MODE_DBG2);
  /*p08.LULA*/ wire CBUS_TO_CEXT  = not(CBUS_TO_CEXTn);

  // sch_lcd

  /*p01.LYHA*/ wire VID_RESET2 = not(VID_RESETn);
  /*p21.MUDE*/ wire X_RSTn = nor(lcd_reg.NEW_LINE_d0a, VID_RESET2);
  /*p21.SAXO*/ lcd_reg.X0.set(TALU_xBCDExxx, X_RSTn, !lcd_reg.X0);
  /*p21.TYPO*/ lcd_reg.X1.set(!lcd_reg.X0,   X_RSTn, !lcd_reg.X1);
  /*p21.VYZO*/ lcd_reg.X2.set(!lcd_reg.X1,   X_RSTn, !lcd_reg.X2);
  /*p21.TELU*/ lcd_reg.X3.set(!lcd_reg.X2,   X_RSTn, !lcd_reg.X3);
  /*p21.SUDE*/ lcd_reg.X4.set(!lcd_reg.X3,   X_RSTn, !lcd_reg.X4);
  /*p21.TAHA*/ lcd_reg.X5.set(!lcd_reg.X4,   X_RSTn, !lcd_reg.X5);
  /*p21.TYRY*/ lcd_reg.X6.set(!lcd_reg.X5,   X_RSTn, !lcd_reg.X6);

  /*p21.SANU*/ wire LINE_END     = and(lcd_reg.X6, lcd_reg.X5, lcd_reg.X4, lcd_reg.X0); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
  /*p21.RUTU*/ lcd_reg.NEW_LINE_d0a.set(SONO_AxxxxFGH, VID_RESETn, LINE_END);
  /*p21.NYPE*/ lcd_reg.NEW_LINE_d4a.set(TALU_xBCDExxx, VID_RESETn2, lcd_reg.NEW_LINE_d0a);
  
  /*p21.NOKO*/ wire LINE_153_d0  = and(lcd_reg.Y7, lcd_reg.Y4, lcd_reg.Y3, lcd_reg.Y0); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*p21.MYTA*/ lcd_reg.LINE_153_d4.set(lcd_reg.NEW_LINE_d4a, VID_RESETn2, LINE_153_d0);

  /*p21.LAMA*/ wire FRAME_RSTn   = nor(lcd_reg.LINE_153_d4, VID_RESET2);
  /*p21.MUWY*/ lcd_reg.Y0.set(lcd_reg.NEW_LINE_d0a, FRAME_RSTn, !lcd_reg.Y0);
  /*p21.MYRO*/ lcd_reg.Y1.set(!lcd_reg.Y0,          FRAME_RSTn, !lcd_reg.Y1);
  /*p21.LEXA*/ lcd_reg.Y2.set(!lcd_reg.Y1,          FRAME_RSTn, !lcd_reg.Y2);
  /*p21.LYDO*/ lcd_reg.Y3.set(!lcd_reg.Y2,          FRAME_RSTn, !lcd_reg.Y3);
  /*p21.LOVU*/ lcd_reg.Y4.set(!lcd_reg.Y3,          FRAME_RSTn, !lcd_reg.Y4);
  /*p21.LEMA*/ lcd_reg.Y5.set(!lcd_reg.Y4,          FRAME_RSTn, !lcd_reg.Y5);
  /*p21.MATO*/ lcd_reg.Y6.set(!lcd_reg.Y5,          FRAME_RSTn, !lcd_reg.Y6);
  /*p21.LAFO*/ lcd_reg.Y7.set(!lcd_reg.Y6,          FRAME_RSTn, !lcd_reg.Y7);

  /*p21.XYVO*/ wire LINE_144_d0  = and(lcd_reg.Y4, lcd_reg.Y7); // 128 + 16 = 144
  /*p21.POPU*/ lcd_reg.VBLANK_d4.set(lcd_reg.NEW_LINE_d4a, VID_RESETn2, LINE_144_d0);

  /*p21.PURE*/ wire NEW_LINE_d0n = not(lcd_reg.NEW_LINE_d0a);
  /*p21.SELA*/ wire NEW_LINE_d0b = not(NEW_LINE_d0n);
  /*p29.ALES*/ wire LINE_144_d0n = not(LINE_144_d0);
  /*p29.ABOV*/ wire VID_LINE_d0  = and(NEW_LINE_d0b, LINE_144_d0n);
  /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
  /*p29.CATU*/ lcd_reg.VID_LINE_d4.set(XUPY_xBCxxFGx, VID_RESETn3, VID_LINE_d0);
  /*p28.ANEL*/ lcd_reg.VID_LINE_d6.set(AWOH_AxxDExxH, VID_RESETn3, lcd_reg.VID_LINE_d4);

  /*p01.AMYG*/ wire VID_RESET7  = not(VID_RESETn);

  /*p28.ABAF*/ wire VID_LINE_d4n = not(lcd_reg.VID_LINE_d4);

  /*p21.PARU*/ wire VBLANK_d4b = not(!lcd_reg.VBLANK_d4);
  /*p21.TOLU*/ wire INT_VBLn = not(VBLANK_d4b);

  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or(lcd_reg.VID_LINE_d6, VID_LINE_d4n), VID_RESETn3);
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4  = not(BYHA_VID_LINE_TRIG_d4n);
  /*p28.ABAK*/ wire ABAK_VID_LINE_TRIG_d4  = or (ATEJ_VID_LINE_TRIG_d4, VID_RESET7);
  /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4);
  /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n = not(ABAK_VID_LINE_TRIG_d4);
  /*p29.DYBA*/ wire DYBA_VID_LINE_TRIG_d4  = not(BYVA_VID_LINE_TRIG_d4n);

  // sch_debug

  /*p27.VYPO*/ wire VYPO_P10Bn = not(joy_pins.P10_B);

  // sch_dma

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, CUPA_CPUWR);
  /*p04.LORU*/ wire FF46_WR = not(FF46_WRn);
 
  /*p04.LEBU*/ wire DMA_A15n  = not(dma_reg.DMA_A15);
  /*p04.MUDA*/ wire DMA_VRAM  = nor(dma_reg.DMA_A13, dma_reg.DMA_A14, DMA_A15n);
  /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);
  /*p04.MORY*/ wire DMA_READ_CARTn = nand(dma_reg.REG_DMA_RW_EN, DMA_VRAMn);
  /*p04.MUHO*/ wire DMA_READ_VRAMn = nand(dma_reg.REG_DMA_RW_EN, DMA_VRAM);
  /*p04.LUMA*/ wire DMA_READ_CART = not(DMA_READ_CARTn);
  /*p04.LUFA*/ wire DMA_READ_VRAM = not(DMA_READ_VRAMn);

  /*p04.MAKA*/ dma_reg.FROM_CPU5_SYNC.set(ZEME_xBxDxFxH, CUNU_RESETn, cpu_pins.FROM_CPU5);
  /*p04.NAXY*/ wire NAXY = nor(dma_reg.FROM_CPU5_SYNC, dma_reg.REG_DMA_EN_d0);
  /*p04.POWU*/ wire DMA_WRITE_OAM = and(dma_reg.REG_DMA_RW_EN, NAXY);

  /*p04.LOKO*/ wire DMA_RST   = nand(CUNU_RESETn, !dma_reg.REG_DMA_EN_d4);
  /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);
  /*p04.NAVO*/ wire DMA_DONEn = nand(dma_reg.DMA_A00, dma_reg.DMA_A01, dma_reg.DMA_A02, dma_reg.DMA_A03, dma_reg.DMA_A04, dma_reg.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ wire DMA_DONE  = not(DMA_DONEn);
  /*p04.MYTE*/ dma_reg.DMA_DONE_SYNC.set(MOPA_xxxxEFGH, DMA_RSTn, DMA_DONE);

  /*p04.LYXE*/ dma_reg.LATCH_DMA_ENn_d0.sr_latch(DMA_RST, FF46_WRn);
  /*p04.LUPA*/ wire DMA_EN = nor(FF46_WRn, dma_reg.LATCH_DMA_ENn_d0); // this seems redundant

  /*p04.LUVY*/ dma_reg.REG_DMA_EN_d0.set(UVYT_ABCDxxxx, CUNU_RESETn, DMA_EN);
  /*p04.LENE*/ dma_reg.REG_DMA_EN_d4.set(MOPA_xxxxEFGH, CUNU_RESETn, dma_reg.REG_DMA_EN_d0);

  /*p04.LOKY*/ dma_reg.LATCH_DMA_EN_d4.tp_latch(dma_reg.REG_DMA_EN_d4, dma_reg.DMA_DONE_SYNC || !CUNU_RESETn); // lara/loky nand sr latch
  /*p04.MATU*/ dma_reg.REG_DMA_RW_EN.set(UVYT_ABCDxxxx, CUNU_RESETn, dma_reg.LATCH_DMA_EN_d4);

  /*p04.META*/ wire CLK_DMA_LO = and(UVYT_ABCDxxxx, dma_reg.LATCH_DMA_EN_d4);
  /*p04.NAKY*/ dma_reg.DMA_A00.set(CLK_DMA_LO, DMA_RSTn, !dma_reg.DMA_A00);
  /*p04.PYRO*/ dma_reg.DMA_A01.set(!dma_reg.DMA_A00, DMA_RSTn, !dma_reg.DMA_A01);
  /*p04.NEFY*/ dma_reg.DMA_A02.set(!dma_reg.DMA_A01, DMA_RSTn, !dma_reg.DMA_A02);
  /*p04.MUTY*/ dma_reg.DMA_A03.set(!dma_reg.DMA_A02, DMA_RSTn, !dma_reg.DMA_A03);
  /*p04.NYKO*/ dma_reg.DMA_A04.set(!dma_reg.DMA_A03, DMA_RSTn, !dma_reg.DMA_A04);
  /*p04.PYLO*/ dma_reg.DMA_A05.set(!dma_reg.DMA_A04, DMA_RSTn, !dma_reg.DMA_A05);
  /*p04.NUTO*/ dma_reg.DMA_A06.set(!dma_reg.DMA_A05, DMA_RSTn, !dma_reg.DMA_A06);
  /*p04.MUGU*/ dma_reg.DMA_A07.set(!dma_reg.DMA_A06, DMA_RSTn, !dma_reg.DMA_A07);

  /*p04.NAFA*/ dma_reg.DMA_A08.set(FF46_WR, bus_tri.D0());
  /*p04.PYNE*/ dma_reg.DMA_A09.set(FF46_WR, bus_tri.D1());
  /*p04.PARA*/ dma_reg.DMA_A10.set(FF46_WR, bus_tri.D2());
  /*p04.NYDO*/ dma_reg.DMA_A11.set(FF46_WR, bus_tri.D3());
  /*p04.NYGY*/ dma_reg.DMA_A12.set(FF46_WR, bus_tri.D4());
  /*p04.PULA*/ dma_reg.DMA_A13.set(FF46_WR, bus_tri.D5());
  /*p04.POKU*/ dma_reg.DMA_A14.set(FF46_WR, bus_tri.D6());
  /*p04.MARU*/ dma_reg.DMA_A15.set(FF46_WR, bus_tri.D7());

  /*p04.MOLU*/ wire FF46_RDn1 = nand(FF46, ASOT_CPURD);
  /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
  /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);

  if (FF46_RDn2) bus_tri.set_data(
    /*p04.POLY*/ dma_reg.DMA_A08,
    /*p04.ROFO*/ dma_reg.DMA_A09,
    /*p04.REMA*/ dma_reg.DMA_A10,
    /*p04.PANE*/ dma_reg.DMA_A11,
    /*p04.PARE*/ dma_reg.DMA_A12,
    /*p04.RALY*/ dma_reg.DMA_A13,
    /*p04.RESU*/ dma_reg.DMA_A14,
    /*p04.NUVY*/ dma_reg.DMA_A15
  );

  // sch_timer.tick_div

  /*p01.TAPE*/ wire FF04_WR = and(TAPU_CPUWR, FF04_FF07, TOLA_A01n, TOVY_A00n);
  /*p01.TAGY*/ wire FF04_RD = and(TEDO_CPURD, FF04_FF07, TOLA_A01n, TOVY_A00n);

  /*p01.UFOL*/ wire DIV_RSTn = nor(UCOB_CLKBAD, ext_pins.RST, FF04_WR);
  
  ///*p01.ULUR*/ wire DIV_06_CLK = mux2n(clk_sig.BOGA_xBCDEFGH, !DIV_05, sys_sig.FF60_1);
  wire DIV_06_CLK = !tim_reg.DIV_05;

  /*p01.UKUP*/ tim_reg.DIV_00.set(BOGA_xBCDEFGH,   DIV_RSTn, !tim_reg.DIV_00);
  /*p01.UFOR*/ tim_reg.DIV_01.set(!tim_reg.DIV_00, DIV_RSTn, !tim_reg.DIV_01);
  /*p01.UNER*/ tim_reg.DIV_02.set(!tim_reg.DIV_01, DIV_RSTn, !tim_reg.DIV_02);
  /*p01.TERO*/ tim_reg.DIV_03.set(!tim_reg.DIV_02, DIV_RSTn, !tim_reg.DIV_03);
  /*p01.UNYK*/ tim_reg.DIV_04.set(!tim_reg.DIV_03, DIV_RSTn, !tim_reg.DIV_04);
  /*p01.TAMA*/ tim_reg.DIV_05.set(!tim_reg.DIV_04, DIV_RSTn, !tim_reg.DIV_05);

  // schematic says DIV_06_CLK is not inverted, but the counter acts funny if it isn't.
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

  /*p01.UMEK*/ wire DIV_06n   = not(tim_reg.DIV_06);
  /*p01.UREK*/ wire DIV_07n   = not(tim_reg.DIV_07);
  /*p01.UTOK*/ wire DIV_08n   = not(tim_reg.DIV_08);
  /*p01.SAPY*/ wire DIV_09n   = not(tim_reg.DIV_09);
  /*p01.UMER*/ wire DIV_10n   = not(tim_reg.DIV_10);
  /*p01.RAVE*/ wire DIV_11n   = not(tim_reg.DIV_11);
  /*p01.RYSO*/ wire DIV_12n   = not(tim_reg.DIV_12);
  /*p01.UDOR*/ wire DIV_13n   = not(tim_reg.DIV_13);
  
  if (FF04_RD) bus_tri.set_data(
    /*p01.TAWU*/ not(DIV_06n),
    /*p01.TAKU*/ not(DIV_07n),
    /*p01.TEMU*/ not(DIV_08n),
    /*p01.TUSE*/ not(DIV_09n),
    /*p01.UPUG*/ not(DIV_10n),
    /*p01.SEPU*/ not(DIV_11n),
    /*p01.SAWA*/ not(DIV_12n),
    /*p01.TATU*/ not(DIV_13n)
  );

  // sch_timer.tick_tima

  /*p03.TOPE*/ wire FF05_WRn = nand(TAPU_CPUWR, FF04_FF07, TOLA_A01n, bus_tri.A00());
  /*p03.TEDA*/ wire FF05_RD = and(TEDO_CPURD, FF04_FF07, TOLA_A01n,     bus_tri.A00());

  /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(tim_reg.TMA_0, bus_tri.D0(), FF05_WRn);
  /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(tim_reg.TMA_1, bus_tri.D1(), FF05_WRn);
  /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(tim_reg.TMA_2, bus_tri.D2(), FF05_WRn);
  /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(tim_reg.TMA_3, bus_tri.D3(), FF05_WRn);
  /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(tim_reg.TMA_4, bus_tri.D4(), FF05_WRn);
  /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(tim_reg.TMA_5, bus_tri.D5(), FF05_WRn);
  /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(tim_reg.TMA_6, bus_tri.D6(), FF05_WRn);
  /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(tim_reg.TMA_7, bus_tri.D7(), FF05_WRn);

  /*p03.MULO*/ wire TIMA_RST = not(SYS_RESETn);

  /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
  /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
  /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
  /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
  /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
  /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
  /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
  /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);

  // Clock mux
  /*p03.UBOT*/ wire CLK_256Kn = not(tim_reg.DIV_01);
  /*p03.UVYR*/ wire CLK_64Kn  = not(tim_reg.DIV_03);
  /*p01.UVYN*/ wire CLK_16Kn  = not(tim_reg.DIV_05);
  /*p01.UREK*/ wire CLK_4Kn   = not(tim_reg.DIV_07);

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(CLK_16Kn,  CLK_64Kn, tim_reg.TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, CLK_4Kn,  tim_reg.TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa,  CLK_MUXb, tim_reg.TAC_1);

  /*p03.MUZU*/ wire MUZU       = or(cpu_pins.FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(tim_reg.INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD  = nand(MUZU, SYS_RESETn, INT_TIMERn);
  /*p03.MUGY*/ wire TIMA_LOADn = not(TIMA_LOAD);

  /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !tim_reg.TAC_2);

  /*p03.REGA*/ tim_reg.TIMA_0.set(TIMA_CLK,   TIMA_LOAD, TIMA_LD_0);
  /*p03.POVY*/ tim_reg.TIMA_1.set(tim_reg.TIMA_0.c(), TIMA_LOAD, TIMA_LD_1);
  /*p03.PERU*/ tim_reg.TIMA_2.set(tim_reg.TIMA_1.c(), TIMA_LOAD, TIMA_LD_2);
  /*p03.RATE*/ tim_reg.TIMA_3.set(tim_reg.TIMA_2.c(), TIMA_LOAD, TIMA_LD_3);
  /*p03.RUBY*/ tim_reg.TIMA_4.set(tim_reg.TIMA_3.c(), TIMA_LOAD, TIMA_LD_4);
  /*p03.RAGE*/ tim_reg.TIMA_5.set(tim_reg.TIMA_4.c(), TIMA_LOAD, TIMA_LD_5);
  /*p03.PEDA*/ tim_reg.TIMA_6.set(tim_reg.TIMA_5.c(), TIMA_LOAD, TIMA_LD_6);
  /*p03.NUGA*/ tim_reg.TIMA_7.set(tim_reg.TIMA_6.c(), TIMA_LOAD, TIMA_LD_7);

  /*p03.MERY*/ wire INT_TIMER_IN = nor(!tim_reg.TIMA_MAX, tim_reg.TIMA_7.c());

  /*p03.NYDU*/ tim_reg.TIMA_MAX.set(BOGA_xBCDEFGH, TIMA_LOADn, tim_reg.TIMA_7.c());
  /*p03.MOBA*/ tim_reg.INT_TIMER.set(BOGA_xBCDEFGH, SYS_RESETn, INT_TIMER_IN);

  if (FF05_RD) bus_tri.set_data(
    /*p03.SOKU*/ tim_reg.TIMA_0.v(),
    /*p03.RACY*/ tim_reg.TIMA_1.v(),
    /*p03.RAVY*/ tim_reg.TIMA_2.v(),
    /*p03.SOSY*/ tim_reg.TIMA_3.v(),
    /*p03.SOMU*/ tim_reg.TIMA_4.v(),
    /*p03.SURO*/ tim_reg.TIMA_5.v(),
    /*p03.ROWU*/ tim_reg.TIMA_6.v(),
    /*p03.PUSO*/ tim_reg.TIMA_7.v()
  );

  // sch_timer.tick_tma

  /*p03.TYJU*/ wire FF06_WRn = nand(TAPU_CPUWR, FF04_FF07, TOVY_A00n, bus_tri.A01());
  /*p03.TUBY*/ wire FF06_RD = and(TEDO_CPURD, FF04_FF07, bus_tri.A01(), TOVY_A00n);

  /*p03.SABU*/ tim_reg.TMA_0.set(FF06_WRn, SYS_RESETn, bus_tri.D0());
  /*p03.NYKE*/ tim_reg.TMA_1.set(FF06_WRn, SYS_RESETn, bus_tri.D1());
  /*p03.MURU*/ tim_reg.TMA_2.set(FF06_WRn, SYS_RESETn, bus_tri.D2());
  /*p03.TYVA*/ tim_reg.TMA_3.set(FF06_WRn, SYS_RESETn, bus_tri.D3());
  /*p03.TYRU*/ tim_reg.TMA_4.set(FF06_WRn, SYS_RESETn, bus_tri.D4());
  /*p03.SUFY*/ tim_reg.TMA_5.set(FF06_WRn, SYS_RESETn, bus_tri.D5());
  /*p03.PETO*/ tim_reg.TMA_6.set(FF06_WRn, SYS_RESETn, bus_tri.D6());
  /*p03.SETA*/ tim_reg.TMA_7.set(FF06_WRn, SYS_RESETn, bus_tri.D7());

  if (FF06_RD) bus_tri.set_data(
    /*p03.SETE*/ tim_reg.TMA_0,
    /*p03.PYRE*/ tim_reg.TMA_1,
    /*p03.NOLA*/ tim_reg.TMA_2,
    /*p03.SALU*/ tim_reg.TMA_3,
    /*p03.SUPO*/ tim_reg.TMA_4,
    /*p03.SOTU*/ tim_reg.TMA_5,
    /*p03.REVA*/ tim_reg.TMA_6,
    /*p03.SAPU*/ tim_reg.TMA_7
  );

  // sch_timer.tick_tac

  /*p03.SARA*/ wire FF07_WRn = nand(TAPU_CPUWR, FF04_FF07, bus_tri.A00(), bus_tri.A01());
  /*p03.SORA*/ wire FF07_RD = and(TEDO_CPURD, FF04_FF07, bus_tri.A00(), bus_tri.A01());

  /*p03.SOPU*/ tim_reg.TAC_0.set(FF07_WRn, SYS_RESETn, bus_tri.D0());
  /*p03.SAMY*/ tim_reg.TAC_1.set(FF07_WRn, SYS_RESETn, bus_tri.D1());
  /*p03.SABO*/ tim_reg.TAC_2.set(FF07_WRn, SYS_RESETn, bus_tri.D2());

  if (FF07_RD) bus_tri.set_data(
    /*p03.RYLA*/ tim_reg.TAC_0,
    /*p03.ROTE*/ tim_reg.TAC_1,
    /*p03.SUPE*/ tim_reg.TAC_2
  );

  // sch_serial

  /*p06.COBA*/ wire SER_CNT3n = not(ser_reg.SER_CNT3);

  /*p06.UWAM*/ wire FF02_WRn = nand(TAPU_CPUWR, ADDR_FF00_FF03, bus_tri.A01(), TOVY_A00n);
  /*p06.CABY*/ wire XFER_RESET = and(SER_CNT3n, SYS_RESETn);
  /*p06.ETAF*/ ser_reg.XFER_START.set(FF02_WRn, XFER_RESET,         bus_tri.D0());
  /*p06.CULY*/ ser_reg.XFER_DIR  .set(FF02_WRn, SYS_RESETn, bus_tri.D1());

  /*p01.UVYN*/ wire CLK_16K = not(tim_reg.DIV_05);
  /*p06.COTY*/ ser_reg.SER_CLK.set(CLK_16K, FF02_WRn, !ser_reg.SER_CLK);

  /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(ser_reg.SER_CLK, ser_pins.SCK_C, ser_reg.XFER_DIR);

  /*p06.CARO*/ wire SER_RST  = and(FF02_WRn, SYS_RESETn);
  /*p06.DAWA*/ wire SER_CLK1 = or(SER_CLK_MUXn, !ser_reg.XFER_START); // this must stop the clock or something when the transmit's done
  /*p06.CAFA*/ ser_reg.SER_CNT0.set(SER_CLK1,          SER_RST, !ser_reg.SER_CNT0);
  /*p06.CYLO*/ ser_reg.SER_CNT1.set(!ser_reg.SER_CNT0, SER_RST, !ser_reg.SER_CNT1);
  /*p06.CYDE*/ ser_reg.SER_CNT2.set(!ser_reg.SER_CNT1, SER_RST, !ser_reg.SER_CNT2);
  /*p06.CALY*/ ser_reg.SER_CNT3.set(!ser_reg.SER_CNT2, SER_RST, !ser_reg.SER_CNT3);
  
  /*p06.URYS*/ wire FF01_WRn = nand(TAPU_CPUWR, ADDR_FF00_FF03, bus_tri.A00(), TOLA_A01n);
  /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

  /*p06.CUFU*/ wire SER_DATA0_SETn = nand(bus_tri.D0(), FF01_WR);
  /*p06.DOCU*/ wire SER_DATA1_SETn = nand(bus_tri.D1(), FF01_WR);
  /*p06.DELA*/ wire SER_DATA2_SETn = nand(bus_tri.D2(), FF01_WR);
  /*p06.DYGE*/ wire SER_DATA3_SETn = nand(bus_tri.D3(), FF01_WR);
  /*p06.DOLA*/ wire SER_DATA4_SETn = nand(bus_tri.D4(), FF01_WR);
  /*p06.ELOK*/ wire SER_DATA5_SETn = nand(bus_tri.D5(), FF01_WR);
  /*p06.EDEL*/ wire SER_DATA6_SETn = nand(bus_tri.D6(), FF01_WR);
  /*p06.EFEF*/ wire SER_DATA7_SETn = nand(bus_tri.D7(), FF01_WR);

  /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, bus_tri.D0()), SYS_RESETn);
  /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, bus_tri.D1()), SYS_RESETn);
  /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, bus_tri.D2()), SYS_RESETn);
  /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, bus_tri.D3()), SYS_RESETn);
  /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, bus_tri.D4()), SYS_RESETn);
  /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, bus_tri.D5()), SYS_RESETn);
  /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, bus_tri.D6()), SYS_RESETn);
  /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, bus_tri.D7()), SYS_RESETn);

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
  /*p06.ELYS*/ ser_reg.SER_OUT.set(SER_CLKn, SYS_RESETn, ser_reg.SER_DATA7);

  ///*p05.KENA*/ ser_pins.SOUT  = mux2n(DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
  ser_pins.SOUT = ser_reg.SER_OUT;
  /*p06.KEXU*/ ser_pins.SCK_A = nand(ser_reg.SER_CLK, ser_reg.XFER_DIR);
  /*p06.CULY*/ ser_pins.SCK_B = ser_reg.XFER_DIR;
  /*p06.KUJO*/ ser_pins.SCK_D = nor (ser_reg.SER_CLK, /*p06.JAGO*/ not(ser_reg.XFER_DIR));

  /*p06.UFEG*/ wire FF01_RD = and(TEDO_CPURD, ADDR_FF00_FF03, bus_tri.A00(), TOLA_A01n);

  if (FF01_RD) bus_tri.set_data(
    /*p06.CUGY*/ ser_reg.SER_DATA0,
    /*p06.DUDE*/ ser_reg.SER_DATA1,
    /*p06.DETU*/ ser_reg.SER_DATA2,
    /*p06.DASO*/ ser_reg.SER_DATA3,
    /*p06.DAME*/ ser_reg.SER_DATA4,
    /*p06.EVOK*/ ser_reg.SER_DATA5,
    /*p06.EFAB*/ ser_reg.SER_DATA6,
    /*p06.ETAK*/ ser_reg.SER_DATA7
  );

  /*p06.UCOM*/ wire FF02_RD = and (TEDO_CPURD, ADDR_FF00_FF03, bus_tri.A01(), TOVY_A00n);

  if (FF02_RD) bus_tri.set_data(
    /*p06.CORE*/ ser_reg.XFER_DIR,
    /*p06.ELUV*/ ser_reg.XFER_START
  );

  // sch_joypad

  // FIXME
  wire FF60_0 = 0;
  wire FF60_0n = 1;

  /*p10.AKUG*/ wire A06n = not(bus_tri.A06());
  /*p10.BYKO*/ wire A05n = not(bus_tri.A05());

  /*p05.KOLE*/ joy_pins.P10_A = nand(joy_reg.JOYP_RA,  FF60_0);
  /*p05.KYBU*/ joy_pins.P10_D = nor (joy_reg.JOYP_RA,  FF60_0n);
  /*p05.KYTO*/ joy_pins.P11_A = nand(joy_reg.JOYP_LB,  FF60_0);
  /*p05.KABU*/ joy_pins.P11_D = nor (joy_reg.JOYP_LB,  FF60_0n);
  /*p05.KYHU*/ joy_pins.P12_A = nand(joy_reg.JOYP_UC,  FF60_0);
  /*p05.KASY*/ joy_pins.P12_D = nor (joy_reg.JOYP_UC,  FF60_0n); // schematic wrong
  /*p05.KORY*/ joy_pins.P13_A = nand(joy_reg.JOYP_DS,  FF60_0);
  /*p05.KALE*/ joy_pins.P13_D = nor (joy_reg.JOYP_DS,  FF60_0n);
  /*p05.KARU*/ joy_pins.P14_A = or(!joy_reg.JOYP_UDLR, FF60_0n);
  /*p05.KARU*/ joy_pins.P14_D = joy_reg.JOYP_UDLR;
  /*p05.CELA*/ joy_pins.P15_A = or(!joy_reg.JOYP_ABCS, FF60_0n);
  /*p05.KARU*/ joy_pins.P15_D = !joy_reg.JOYP_ABCS;

  /*p02.KERY*/ wire ANY_BUTTON = or(joy_pins.P13_C, joy_pins.P12_C, joy_pins.P11_C, joy_pins.P10_C);

  /*p02.AWOB*/ joy_reg.WAKE_CPU.set(ANY_BUTTON, BOGA_xBCDEFGH);

  /*p02.BATU*/ joy_reg.JP_GLITCH0.set(BOGA_xBCDEFGH, SYS_RESETn, ANY_BUTTON);
  /*p02.ACEF*/ joy_reg.JP_GLITCH1.set(BOGA_xBCDEFGH, SYS_RESETn, joy_reg.JP_GLITCH0);
  /*p02.AGEM*/ joy_reg.JP_GLITCH2.set(BOGA_xBCDEFGH, SYS_RESETn, joy_reg.JP_GLITCH1);
  /*p02.APUG*/ joy_reg.JP_GLITCH3.set(BOGA_xBCDEFGH, SYS_RESETn, joy_reg.JP_GLITCH2);
  /*p02.ASOK*/ wire INT_JP = and(joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);

  /*p10.ATOZ*/ wire FF00_WRn   = nand(TAPU_CPUWR, ADDR_111111110xx00000, A06n, A05n);
  /*p05.JUTE*/ joy_reg.JOYP_RA    .set(FF00_WRn, SYS_RESETn, bus_tri.D0());
  /*p05.KECY*/ joy_reg.JOYP_LB    .set(FF00_WRn, SYS_RESETn, bus_tri.D1());
  /*p05.JALE*/ joy_reg.JOYP_UC    .set(FF00_WRn, SYS_RESETn, bus_tri.D2());
  /*p05.KYME*/ joy_reg.JOYP_DS    .set(FF00_WRn, SYS_RESETn, bus_tri.D3());
  /*p05.KELY*/ joy_reg.JOYP_UDLR  .set(FF00_WRn, SYS_RESETn, bus_tri.D4());
  /*p05.COFY*/ joy_reg.JOYP_ABCS  .set(FF00_WRn, SYS_RESETn, bus_tri.D5());
  /*p05.KUKO*/ joy_reg.DBG_FF00_D6.set(FF00_WRn, SYS_RESETn, bus_tri.D6());
  /*p05.KERU*/ joy_reg.DBG_FF00_D7.set(FF00_WRn, SYS_RESETn, bus_tri.D7());

  /*p10.ACAT*/ wire FF00_RD = and (TEDO_CPURD, ADDR_111111110xx00000, A06n, A05n);
  /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);
  /*p05.KEVU*/ joy_reg.JOYP_L0.set(joy_pins.P10_C, FF00_RDn);
  /*p05.KAPA*/ joy_reg.JOYP_L1.set(joy_pins.P11_C, FF00_RDn);
  /*p05.KEJA*/ joy_reg.JOYP_L2.set(joy_pins.P12_C, FF00_RDn);
  /*p05.KOLO*/ joy_reg.JOYP_L3.set(joy_pins.P13_C, FF00_RDn);

  if (!FF00_RDn) bus_tri.set_data(
    /*p05.KEMA*/ joy_reg.JOYP_L0,
    /*p05.KURO*/ joy_reg.JOYP_L1,
    /*p05.KUVE*/ joy_reg.JOYP_L2,
    /*p05.JEKU*/ joy_reg.JOYP_L3,
    /*p05.KOCE*/ joy_reg.JOYP_UDLR,
    /*p05.CUDY*/ joy_reg.JOYP_ABCS,
    /*p05.KUKO*/ joy_reg.DBG_FF00_D6,
    /*p05.KERU*/ joy_reg.DBG_FF00_D7
  );

  // video config

  /*p01.WESY*/ wire WESY_RESET  = not(XORE_RESET);
  /*p01.WALU*/ wire WALU_RESET  = not(XORE_RESET);
  /*p01.XARE*/ wire XARE_RESET  = not(XORE_RESET);

  //----------

  /*p23.VYRE*/ wire FF40_RD = and(FF40, ASOT_CPURD);
  /*p23.WYCE*/ wire FF40_RDn = not(FF40_RD);
  /*p23.WARU*/ wire FF40_WR = and(FF40, CUPA_CPUWR);
  /*p23.XUBO*/ wire FF40_WRn = not(FF40_WR);

  if (!FF40_RDn) bus_tri.set_data(
    /*p23.WYPO*/ cfg_reg.LCDC_BGEN,
    /*p23.XERO*/ cfg_reg.LCDC_SPEN,
    /*p23.WYJU*/ cfg_reg.LCDC_SPSIZE,
    /*p23.WUKA*/ cfg_reg.LCDC_BGMAP,
    /*p23.VOKE*/ cfg_reg.LCDC_BGTILE,
    /*p23.VATO*/ cfg_reg.LCDC_WINEN,
    /*p23.VAHA*/ cfg_reg.LCDC_WINMAP,
    /*p23.XEBU*/ cfg_reg.LCDC_EN
  );

  /*p23.VYXE*/ cfg_reg.LCDC_BGEN    .set(FF40_WRn, XARE_RESET, bus_tri.D0());
  /*p23.XYLO*/ cfg_reg.LCDC_SPEN    .set(FF40_WRn, XARE_RESET, bus_tri.D1());
  /*p23.XYMO*/ cfg_reg.LCDC_SPSIZE  .set(FF40_WRn, XARE_RESET, bus_tri.D2());
  /*p23.XAFO*/ cfg_reg.LCDC_BGMAP   .set(FF40_WRn, XARE_RESET, bus_tri.D3());
  /*p23.WEXU*/ cfg_reg.LCDC_BGTILE  .set(FF40_WRn, XARE_RESET, bus_tri.D4());
  /*p23.WYMO*/ cfg_reg.LCDC_WINEN   .set(FF40_WRn, XARE_RESET, bus_tri.D5());
  /*p23.WOKY*/ cfg_reg.LCDC_WINMAP  .set(FF40_WRn, XARE_RESET, bus_tri.D6());
  /*p23.XONA*/ cfg_reg.LCDC_EN      .set(FF40_WRn, XARE_RESET, bus_tri.D7());

  //----------

  /*p23.ANYP*/ wire FF42_RD = and(FF42, ASOT_CPURD);
  /*p23.BUWY*/ wire FF42_RDn = not(FF42_RD);
  /*p23.BEDY*/ wire FF42_WR = and(FF42, CUPA_CPUWR);
  /*p23.CAVO*/ wire FF42_WRn = not(FF42_WR);

  if (!FF42_RDn) bus_tri.set_data(
    /*p23.WARE*/ cfg_reg.SCY0,
    /*p23.GOBA*/ cfg_reg.SCY1,
    /*p23.GONU*/ cfg_reg.SCY2,
    /*p23.GODO*/ cfg_reg.SCY3,
    /*p23.CUSA*/ cfg_reg.SCY4,
    /*p23.GYZO*/ cfg_reg.SCY5,
    /*p23.GUNE*/ cfg_reg.SCY6,
    /*p23.GYZA*/ cfg_reg.SCY7
  );

  /*p23.GAVE*/ cfg_reg.SCY0.set(FF42_WRn, CUNU_RESETn, bus_tri.D0());
  /*p23.FYMO*/ cfg_reg.SCY1.set(FF42_WRn, CUNU_RESETn, bus_tri.D1());
  /*p23.FEZU*/ cfg_reg.SCY2.set(FF42_WRn, CUNU_RESETn, bus_tri.D2());
  /*p23.FUJO*/ cfg_reg.SCY3.set(FF42_WRn, CUNU_RESETn, bus_tri.D3());
  /*p23.DEDE*/ cfg_reg.SCY4.set(FF42_WRn, CUNU_RESETn, bus_tri.D4());
  /*p23.FOTY*/ cfg_reg.SCY5.set(FF42_WRn, CUNU_RESETn, bus_tri.D5());
  /*p23.FOHA*/ cfg_reg.SCY6.set(FF42_WRn, CUNU_RESETn, bus_tri.D6());
  /*p23.FUNY*/ cfg_reg.SCY7.set(FF42_WRn, CUNU_RESETn, bus_tri.D7());

  //----------

  /*p23.AVOG*/ wire FF43_RD = and(FF43, ASOT_CPURD);
  /*p23.BEBA*/ wire FF43_RDn = not(FF43_RD);
  /*p23.ARUR*/ wire FF43_WR = and(FF43, CUPA_CPUWR);
  /*p23.AMUN*/ wire FF43_WRn = not(FF43_WR);

  if (!FF43_RDn) bus_tri.set_data(
    /*p23.EDOS*/ cfg_reg.SCX0,
    /*p23.EKOB*/ cfg_reg.SCX1,
    /*p23.CUGA*/ cfg_reg.SCX2,
    /*p23.WONY*/ cfg_reg.SCX3,
    /*p23.CEDU*/ cfg_reg.SCX4,
    /*p23.CATA*/ cfg_reg.SCX5,
    /*p23.DOXE*/ cfg_reg.SCX6,
    /*p23.CASY*/ cfg_reg.SCX7
  );

  /*p23.DATY*/ cfg_reg.SCX0.set(FF43_WRn, CUNU_RESETn, bus_tri.D0());
  /*p23.DUZU*/ cfg_reg.SCX1.set(FF43_WRn, CUNU_RESETn, bus_tri.D1());
  /*p23.CYXU*/ cfg_reg.SCX2.set(FF43_WRn, CUNU_RESETn, bus_tri.D2());
  /*p23.GUBO*/ cfg_reg.SCX3.set(FF43_WRn, CUNU_RESETn, bus_tri.D3());
  /*p23.BEMY*/ cfg_reg.SCX4.set(FF43_WRn, CUNU_RESETn, bus_tri.D4());
  /*p23.CUZY*/ cfg_reg.SCX5.set(FF43_WRn, CUNU_RESETn, bus_tri.D5());
  /*p23.CABU*/ cfg_reg.SCX6.set(FF43_WRn, CUNU_RESETn, bus_tri.D6());
  /*p23.BAKE*/ cfg_reg.SCX7.set(FF43_WRn, CUNU_RESETn, bus_tri.D7());

  //----------

  /*p23.XYLY*/ wire FF45_RD = and(ASOT_CPURD, FF45);
  /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);
  /*p23.XUFA*/ wire FF45_WR = and(CUPA_CPUWR, FF45);
  /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

  if (!FF45_RDn) bus_tri.set_data(
    /*p23.RETU*/ cfg_reg.LYC0,
    /*p23.VOJO*/ cfg_reg.LYC1,
    /*p23.RAZU*/ cfg_reg.LYC2,
    /*p23.REDY*/ cfg_reg.LYC3,
    /*p23.RACE*/ cfg_reg.LYC4,
    /*p23.VAZU*/ cfg_reg.LYC5,
    /*p23.VAFE*/ cfg_reg.LYC6,
    /*p23.PUFY*/ cfg_reg.LYC7
  );

  /*p23.SYRY*/ cfg_reg.LYC0.set(FF45_WRn, WESY_RESET, bus_tri.D0());
  /*p23.VUCE*/ cfg_reg.LYC1.set(FF45_WRn, WESY_RESET, bus_tri.D1());
  /*p23.SEDY*/ cfg_reg.LYC2.set(FF45_WRn, WESY_RESET, bus_tri.D2());
  /*p23.SALO*/ cfg_reg.LYC3.set(FF45_WRn, WESY_RESET, bus_tri.D3());
  /*p23.SOTA*/ cfg_reg.LYC4.set(FF45_WRn, WESY_RESET, bus_tri.D4());
  /*p23.VAFA*/ cfg_reg.LYC5.set(FF45_WRn, WESY_RESET, bus_tri.D5());
  /*p23.VEVO*/ cfg_reg.LYC6.set(FF45_WRn, WESY_RESET, bus_tri.D6());
  /*p23.RAHA*/ cfg_reg.LYC7.set(FF45_WRn, WESY_RESET, bus_tri.D7());

  //----------

  /*p36.VUSO*/ wire FF47_RD = and(ASOT_CPURD, FF47);
  /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);
  /*p36.VELY*/ wire FF47_WR = and(CUPA_CPUWR, FF47);
  /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

  if (!FF47_RDn) bus_tri.set_data(
    /*p36.RARO*/ cfg_reg.BGP0,
    /*p36.PABA*/ cfg_reg.BGP1,
    /*p36.REDO*/ cfg_reg.BGP2,
    /*p36.LOBE*/ cfg_reg.BGP3,
    /*p36.LACE*/ cfg_reg.BGP4,
    /*p36.LYKA*/ cfg_reg.BGP5,
    /*p36.LODY*/ cfg_reg.BGP6,
    /*p36.LARY*/ cfg_reg.BGP7
  );

  /*p36.PAVO*/ cfg_reg.BGP0.set(FF47_WRn, 1, bus_tri.D0());
  /*p36.NUSY*/ cfg_reg.BGP1.set(FF47_WRn, 1, bus_tri.D1());
  /*p36.PYLU*/ cfg_reg.BGP2.set(FF47_WRn, 1, bus_tri.D2());
  /*p36.MAXY*/ cfg_reg.BGP3.set(FF47_WRn, 1, bus_tri.D3());
  /*p36.MUKE*/ cfg_reg.BGP4.set(FF47_WRn, 1, bus_tri.D4());
  /*p36.MORU*/ cfg_reg.BGP5.set(FF47_WRn, 1, bus_tri.D5());
  /*p36.MOGY*/ cfg_reg.BGP6.set(FF47_WRn, 1, bus_tri.D6());
  /*p36.MENA*/ cfg_reg.BGP7.set(FF47_WRn, 1, bus_tri.D7());

  //----------

  /*p36.XUFY*/ wire FF48_RD  = and(ASOT_CPURD, FF48);
  /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);
  /*p36.XOMA*/ wire FF48_WR  = and(CUPA_CPUWR, FF48);
  /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

  if (!FF48_RDn) bus_tri.set_data(
    /*p36.XARY*/ cfg_reg.OBP00,
    /*p36.XOKE*/ cfg_reg.OBP01,
    /*p36.XUNO*/ cfg_reg.OBP02,
    /*p36.XUBY*/ cfg_reg.OBP03,
    /*p36.XAJU*/ cfg_reg.OBP04,
    /*p36.XOBO*/ cfg_reg.OBP05,
    /*p36.XAXA*/ cfg_reg.OBP06,
    /*p36.XAWO*/ cfg_reg.OBP07
  );

  /*p36.XUFU*/ cfg_reg.OBP00.set(FF48_WRn, 1, bus_tri.D0());
  /*p36.XUKY*/ cfg_reg.OBP01.set(FF48_WRn, 1, bus_tri.D1());
  /*p36.XOVA*/ cfg_reg.OBP02.set(FF48_WRn, 1, bus_tri.D2());
  /*p36.XALO*/ cfg_reg.OBP03.set(FF48_WRn, 1, bus_tri.D3());
  /*p36.XERU*/ cfg_reg.OBP04.set(FF48_WRn, 1, bus_tri.D4());
  /*p36.XYZE*/ cfg_reg.OBP05.set(FF48_WRn, 1, bus_tri.D5());
  /*p36.XUPO*/ cfg_reg.OBP06.set(FF48_WRn, 1, bus_tri.D6());
  /*p36.XANA*/ cfg_reg.OBP07.set(FF48_WRn, 1, bus_tri.D7());

  //----------

  /*p36.MUMY*/ wire FF49_RD  = and(ASOT_CPURD, FF49);
  /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD);
  /*p36.MYXE*/ wire FF49_WR  = and(CUPA_CPUWR, FF49);
  /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

  if (!FF48_RDn) bus_tri.set_data(
    /*p36.LAJU*/ cfg_reg.OBP10,
    /*p36.LEPA*/ cfg_reg.OBP11,
    /*p36.LODE*/ cfg_reg.OBP12,
    /*p36.LYZA*/ cfg_reg.OBP13,
    /*p36.LUKY*/ cfg_reg.OBP14,
    /*p36.LUGA*/ cfg_reg.OBP15,
    /*p36.LEBA*/ cfg_reg.OBP16,
    /*p36.LELU*/ cfg_reg.OBP17
  );

  /*p36.MOXY*/ cfg_reg.OBP10.set(FF49_WRn, 1, bus_tri.D0());
  /*p36.LAWO*/ cfg_reg.OBP11.set(FF49_WRn, 1, bus_tri.D1());
  /*p36.MOSA*/ cfg_reg.OBP12.set(FF49_WRn, 1, bus_tri.D2());
  /*p36.LOSE*/ cfg_reg.OBP13.set(FF49_WRn, 1, bus_tri.D3());
  /*p36.LUNE*/ cfg_reg.OBP14.set(FF49_WRn, 1, bus_tri.D4());
  /*p36.LUGU*/ cfg_reg.OBP15.set(FF49_WRn, 1, bus_tri.D5());
  /*p36.LEPU*/ cfg_reg.OBP16.set(FF49_WRn, 1, bus_tri.D6());
  /*p36.LUXO*/ cfg_reg.OBP17.set(FF49_WRn, 1, bus_tri.D7());

  //----------

  /*p23.WAXU*/ wire FF4A_RD = and(ASOT_CPURD, FF4A);
  /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);
  /*p23.WEKO*/ wire FF4A_WR = and(CUPA_CPUWR, FF4A);
  /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

  if (!FF4A_RDn) bus_tri.set_data(
    /*p23.PUNU*/ cfg_reg.WY0,
    /*p23.PODA*/ cfg_reg.WY1,
    /*p23.PYGU*/ cfg_reg.WY2,
    /*p23.LOKA*/ cfg_reg.WY3,
    /*p23.MEGA*/ cfg_reg.WY4,
    /*p23.PELA*/ cfg_reg.WY5,
    /*p23.POLO*/ cfg_reg.WY6,
    /*p23.MERA*/ cfg_reg.WY7
  );

  /*p23.NESO*/ cfg_reg.WY0.set(FF4A_WRn, WALU_RESET, bus_tri.D0());
  /*p23.NYRO*/ cfg_reg.WY1.set(FF4A_WRn, WALU_RESET, bus_tri.D1());
  /*p23.NAGA*/ cfg_reg.WY2.set(FF4A_WRn, WALU_RESET, bus_tri.D2());
  /*p23.MELA*/ cfg_reg.WY3.set(FF4A_WRn, WALU_RESET, bus_tri.D3());
  /*p23.NULO*/ cfg_reg.WY4.set(FF4A_WRn, WALU_RESET, bus_tri.D4());
  /*p23.NENE*/ cfg_reg.WY5.set(FF4A_WRn, WALU_RESET, bus_tri.D5());
  /*p23.NUKA*/ cfg_reg.WY6.set(FF4A_WRn, WALU_RESET, bus_tri.D6());
  /*p23.NAFU*/ cfg_reg.WY7.set(FF4A_WRn, WALU_RESET, bus_tri.D7());

  //----------

  /*p23.WYZE*/ wire FF4B_RD = and(ASOT_CPURD, FF4B);
  /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);
  /*p23.WUZA*/ wire FF4B_WR = and(CUPA_CPUWR, FF4B);
  /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

  if (!FF4B_RDn) bus_tri.set_data(
    /*p23.LOVA*/ cfg_reg.WX0,
    /*p23.MUKA*/ cfg_reg.WX1,
    /*p23.MOKO*/ cfg_reg.WX2,
    /*p23.LOLE*/ cfg_reg.WX3,
    /*p23.MELE*/ cfg_reg.WX4,
    /*p23.MUFE*/ cfg_reg.WX5,
    /*p23.MULY*/ cfg_reg.WX6,
    /*p23.MARA*/ cfg_reg.WX7
  );

  /*p23.MYPA*/ cfg_reg.WX0.set(FF4B_WRn, WALU_RESET, bus_tri.D0());
  /*p23.NOFE*/ cfg_reg.WX1.set(FF4B_WRn, WALU_RESET, bus_tri.D1());
  /*p23.NOKE*/ cfg_reg.WX2.set(FF4B_WRn, WALU_RESET, bus_tri.D2());
  /*p23.MEBY*/ cfg_reg.WX3.set(FF4B_WRn, WALU_RESET, bus_tri.D3());
  /*p23.MYPU*/ cfg_reg.WX4.set(FF4B_WRn, WALU_RESET, bus_tri.D4());
  /*p23.MYCE*/ cfg_reg.WX5.set(FF4B_WRn, WALU_RESET, bus_tri.D5()); 
  /*p23.MUVO*/ cfg_reg.WX6.set(FF4B_WRn, WALU_RESET, bus_tri.D6());
  /*p23.NUKU*/ cfg_reg.WX7.set(FF4B_WRn, WALU_RESET, bus_tri.D7());

  // sch_bootrom

  /*p07.TULO*/ wire ADDR_00XX  = nor(bus_tri.A15(), bus_tri.A14(), bus_tri.A13(), bus_tri.A12(), bus_tri.A11(), bus_tri.A10(), bus_tri.A09(), bus_tri.A08());
  /*p07.TERA*/ wire BOOT_BITn  = not(bootrom.BOOT_BIT);
  /*p07.TUTU*/ wire ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

  /*p07.ZORO*/ wire ADDR_0XXX = nor(bus_tri.A15(), bus_tri.A14(), bus_tri.A13(), bus_tri.A12());
  /*p07.ZADU*/ wire ADDR_X0XX = nor(bus_tri.A11(), bus_tri.A10(), bus_tri.A09(), bus_tri.A08());
  /*p07.ZUFA*/ wire ADDR_00XX2 = and(ADDR_0XXX, ADDR_X0XX);
  /*p07.YAZA*/ wire MODE_DBG1n = not(UMUT_MODE_DBG1);
  /*p07.YULA*/ wire BOOT_RD    = and(TEDO_CPURD, MODE_DBG1n, ADDR_BOOT);
  /*p07.ZADO*/ wire BOOT_CSn   = nand(BOOT_RD, ADDR_00XX2);
  /*p07.ZERY*/ wire BOOT_CS    = not(BOOT_CSn);

  /*p07.ZYBA*/ wire ADDR_00n = not(bus_tri.A00());
  /*p07.ZUVY*/ wire ADDR_01n = not(bus_tri.A01());
  /*p07.ZUFY*/ wire ADDR_04n = not(bus_tri.A04());
  /*p07.ZERA*/ wire ADDR_05n = not(bus_tri.A05());
  /*p07.ZOLE*/ wire ADDR_00  = and(ADDR_01n, ADDR_00n);
  /*p07.ZAJE*/ wire ADDR_01  = and(ADDR_01n, bus_tri.A00());
  /*p07.ZUBU*/ wire ADDR_10  = and(bus_tri.A01(), ADDR_00n);
  /*p07.ZAPY*/ wire ADDR_11  = and(bus_tri.A01(), bus_tri.A00());

  /*p07.ZETE*/ wire BOOTROM_A1nA0n = not(ADDR_00);
  /*p07.ZEFU*/ wire BOOTROM_A1nA0  = not(ADDR_01);
  /*p07.ZYRO*/ wire BOOTROM_A1A0n  = not(ADDR_10);
  /*p07.ZAPA*/ wire BOOTROM_A1A0   = not(ADDR_11);
  /*p07.ZYGA*/ wire BOOTROM_A2n    = not(bus_tri.A02());
  /*p07.ZYKY*/ wire BOOTROM_A3n    = not(bus_tri.A03());
  /*p07.ZYKY*/ wire BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
  /*p07.ZYGA*/ wire BOOTROM_A5nA4  = and(ADDR_05n, bus_tri.A04());
  /*p07.ZOVY*/ wire BOOTROM_A5A4n  = and(bus_tri.A05(), ADDR_04n);
  /*p07.ZUKO*/ wire BOOTROM_A5A4   = and(bus_tri.A05(), bus_tri.A04());
  /*p07.ZAGE*/ wire BOOTROM_A6n    = not(bus_tri.A06());
  /*p07.ZYRA*/ wire BOOTROM_A7n    = not(bus_tri.A07());

  /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(bus_tri.A07(), bus_tri.A05(), bus_tri.A03(), bus_tri.A02(), bus_tri.A01(), bus_tri.A00());
  /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(bus_tri.A04(), bus_tri.A06());
  /*p07.TEXE*/ wire FF50_RD  = and (TEDO_CPURD, ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
  /*p07.TUGE*/ wire FF50_WRn = nand(TAPU_CPUWR, ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
  /*p07.SATO*/ wire BOOT_BIT_IN  = or(bus_tri.D0(), bootrom.BOOT_BIT);
  
  /*p07.TEPU*/ bootrom.BOOT_BIT.set(FF50_WRn, SYS_RESETn, BOOT_BIT_IN);

  /*p07.SYPU*/ if (FF50_RD) bus_tri.set_data(bootrom.BOOT_BIT);

  // how does this not collide with cart reads?
  if (BOOT_CS && BOOT_RD) {
    uint16_t addr = (uint16_t)bus_tri.get_addr();
    bus_tri.set_data(DMG_ROM_bin[addr & 0xFF]);
  }

  // sch_sprites

  /*p28.ANOM*/ wire SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4, VID_RESET6);
  /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
  /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);

  /*p28.FETO*/ wire SCAN_DONE_d0 = and(spr_reg.SCAN0, spr_reg.SCAN1, spr_reg.SCAN2, spr_reg.SCAN5); // 32 + 4 + 2 + 1 = 39
  /*p28.GAVA*/ wire SCAN_CLK = or(SCAN_DONE_d0, XUPY_xBCxxFGx);
  /*p28.YFEL*/ spr_reg.SCAN0.set(SCAN_CLK, SCAN_RSTn, !spr_reg.SCAN0);
  /*p28.WEWY*/ spr_reg.SCAN1.set(!spr_reg.SCAN0,   SCAN_RSTn, !spr_reg.SCAN1);
  /*p28.GOSO*/ spr_reg.SCAN2.set(!spr_reg.SCAN1,   SCAN_RSTn, !spr_reg.SCAN2);
  /*p28.ELYN*/ spr_reg.SCAN3.set(!spr_reg.SCAN2,   SCAN_RSTn, !spr_reg.SCAN3);
  /*p28.FAHA*/ spr_reg.SCAN4.set(!spr_reg.SCAN3,   SCAN_RSTn, !spr_reg.SCAN4);
  /*p28.FONY*/ spr_reg.SCAN5.set(!spr_reg.SCAN4,   SCAN_RSTn, !spr_reg.SCAN5);

  // the second clock here doesn't match the usual synchronizer pattern, but it matches the die.
  /*p29.BYBA*/ spr_reg.SCAN_DONE_d4.set(XUPY_xBCxxFGx, SCAN_RSTo, SCAN_DONE_d0);
  /*p29.DOBA*/ spr_reg.SCAN_DONE_d5.set(ALET_AxCxExGx, SCAN_RSTo, spr_reg.SCAN_DONE_d4);

  // which means this trigger is exactly 1 phase long
  /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or(SCAN_RSTa, spr_reg.SCAN_DONE_d5, !spr_reg.SCAN_DONE_d4);
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);

  /*p01.ROSY*/ wire VID_RESET5 = not(VID_RESETn);

  /*p28.ASEN*/ wire ASEN = or(VID_RESET6, AVAP_SCAN_DONE_d0_TRIG);
  /*p28.BESU*/ wire BESU = or(lcd_reg.VID_LINE_d4, ASEN);


  /*p28.BOGE*/ wire DMA_RW_ENn = not(dma_reg.REG_DMA_RW_EN);
  /*p28.ACYL*/ wire ACYL_OAM_ADDR_PARSE = and(DMA_RW_ENn, BESU);

  ///*p29.AROR*/ wire MATCH_EN = this->MATCH_EN(vid_reg, cfg_reg);

  /*p29.CEHA*/ wire CEHA = not(!spr_reg.STORE_SPRITE_IDXn);
  /*p29.BYJO*/ wire BYJO = not(CEHA);
  /*p29.AZEM*/ wire AZEM = and(BYJO, vid_reg.XYMU_RENDERING_LATCH);
  /*p29.AROR*/ wire MATCH_EN = and(AZEM, cfg_reg.LCDC_SPEN);


  /*p29.TYTU*/ wire SPR_SEQ0n = not(spr_reg.TOXE_SPR_SEQ0);
  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
  /*p29.SYCU*/ wire SYCU = nor(SPR_SEQ0n, LOBY_RENDERINGn, spr_reg.TYFO_SEQ_B0d);

  /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(spr_reg.TULY_SPR_SEQ1, SYCU);
  /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
  /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
  /*p29.XADO*/ wire SPRITE_PIX_LATCH_A  = not(SPRITE_PIX_LATCH_An);

  /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(spr_reg.VONU_SEQ_xxx34xn, SYCU);
  /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
  /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
  /*p29.PUCO*/ wire SPRITE_PIX_LATCH_B  = not(SPRITE_PIX_LATCH_Bn);

  /*p29.TYNO*/ wire TYNO = nand(spr_reg.TOXE_SPR_SEQ0, spr_reg.SEBA_SEQ_xxxx45n, spr_reg.VONU_SEQ_xxx34xn);
  /*p29.VUSA*/ wire VUSA = or(!spr_reg.TYFO_SEQ_B0d, TYNO);
  /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);

  /*p29.SAKY*/ wire SAKY = nor(spr_reg.TULY_SPR_SEQ1, spr_reg.VONU_SEQ_xxx34xn);
  /*p29.TEPA*/ wire RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
  /*p29.TYSO*/ wire SPRITE_READq = or(SAKY, RENDERINGn); // seems wrong
  /*p29.TEXY*/ wire SPRITE_READ = not(SPRITE_READq);
    
  ///*p29.XONO*/ wire FLIP_X = and(oam.OAM_A_D5, SPRITE_READn);

  //----------
  // Sprite y comparator

  //SpriteDelta spr_delta = storeMatchY(sys_sig, lcd_reg, oam_reg, vid_cfg);

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

  /*p29.DEGE*/ wire SPRITE_DELTA0 = not(YDIFF0);
  /*p29.DABY*/ wire SPRITE_DELTA1 = not(YDIFF1);
  /*p29.DABU*/ wire SPRITE_DELTA2 = not(YDIFF2);
  /*p29.GYSA*/ wire SPRITE_DELTA3 = not(YDIFF3);
  /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF4);
  /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF5);
  /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF6);
  /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF7);

  /*p29.GOVU*/ wire GOVU = or(YDIFF3, cfg_reg.LCDC_SPSIZE);
  /*p29.WOTA*/ wire SPR_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU);
  /*p29.GESE*/ wire SPR_MATCH_Y = not(SPR_MATCH_Yn);


  /*p29.CARE*/ wire STORE_ENn = or(XOCE_ABxxEFxx, CEHA, SPR_MATCH_Y);
  /*p29.DYTY*/ wire STORE_EN = not(STORE_ENn);

  /*p28.WEFE*/ wire WEFE = not(joy_pins.P10_B);
  /*p30.CYKE*/ wire CYKE = not(XUPY_xBCxxFGx);
  /*p30.WUDA*/ wire WUDA = not(CYKE);
  /*p30.XADU*/ spr_reg.SPRITE_IDX0.set(WUDA, WEFE, oam_pins.A2);
  /*p30.XEDY*/ spr_reg.SPRITE_IDX1.set(WUDA, WEFE, oam_pins.A3);
  /*p30.ZUZE*/ spr_reg.SPRITE_IDX2.set(WUDA, WEFE, oam_pins.A4);
  /*p30.XOBE*/ spr_reg.SPRITE_IDX3.set(WUDA, WEFE, oam_pins.A5);
  /*p30.YDUF*/ spr_reg.SPRITE_IDX4.set(WUDA, WEFE, oam_pins.A6);
  /*p30.XECU*/ spr_reg.SPRITE_IDX5.set(WUDA, WEFE, oam_pins.A7);

  /*p29.CENO*/ spr_reg.STORE_SPRITE_IDXn.set(XUPY_xBCxxFGx, VID_RESETn3, BESU);
  /*p29.BUZA*/ wire STORE_SPRITE_IDX = and(!spr_reg.STORE_SPRITE_IDXn, vid_reg.XYMU_RENDERING_LATCH);

#if 0
  /*p30.WENU*/ if (FEPO_STORE_MATCH)      sil_tri.TS_LINE_0 = SPRITE_DELTA1; // order wrong
  /*p30.CUCU*/ if (FEPO_STORE_MATCH)      sil_tri.TS_LINE_1 = SPRITE_DELTA2;
  /*p30.CUCA*/ if (FEPO_STORE_MATCH)      sil_tri.TS_LINE_2 = SPRITE_DELTA0;
  /*p30.CEGA*/ if (FEPO_STORE_MATCH)      sil_tri.TS_LINE_3 = SPRITE_DELTA3;
  /*p30.WEZA*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_0  = spr_reg.SPRITE_IDX5; // order wrong
  /*p30.WUCO*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_1  = spr_reg.SPRITE_IDX4;
  /*p30.WYDA*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_2  = spr_reg.SPRITE_IDX3;
  /*p30.ZYSU*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_3  = spr_reg.SPRITE_IDX2;
  /*p30.WYSE*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_4  = spr_reg.SPRITE_IDX1;
  /*p30.WUZY*/ if (BUZA_STORE_SPRITE_IDX) sil_tri.TS_IDX_5  = spr_reg.SPRITE_IDX0;
#endif

  /*p01.LAPE*/ wire LAPE_xBxDxFxH = not(ALET_AxCxExGx);
  /*p27.TAVA*/ wire TAVA_AxCxExGx = not(LAPE_xBxDxFxH);

  /*p29.TAME*/ wire SEQ_5n = nand(spr_reg.TESE_SPR_SEQ2, spr_reg.TOXE_SPR_SEQ0);

  ///*p29.TOMA*/ SEQ_CLK = or(AxCxExGx, SEQ_5);

#if 0
  /*p29.TOMA*/ wire TOMA_AxCxExGx = nand(LAPE_xBxDxFxH, SEQ_5n);
  /*p27.SECA*/ wire SPR_SEQ_RST = nor(RYCE_SPRITE_FETCH_TRIG, VID_RESET5, BYHA_VID_LINE_TRIG_d4n);
  /*p29.TOXE*/ spr_reg.TOXE_SPR_SEQ0.set(TOMA_AxCxExGx,          SPR_SEQ_RST, !spr_reg.TOXE_SPR_SEQ0);
  /*p29.TULY*/ spr_reg.TULY_SPR_SEQ1.set(!spr_reg.TOXE_SPR_SEQ0, SPR_SEQ_RST, !spr_reg.TULY_SPR_SEQ1);
  /*p29.TESE*/ spr_reg.TESE_SPR_SEQ2.set(!spr_reg.TULY_SPR_SEQ1, SPR_SEQ_RST, !spr_reg.TESE_SPR_SEQ2);
#endif

  /*p29.TYFO*/ spr_reg.TYFO_SEQ_B0d    .set(LAPE_xBxDxFxH, VYPO_P10Bn,                   spr_reg.TOXE_SPR_SEQ0);
  /*p29.TOBU*/ spr_reg.TOBU_SEQ_xx23xx .set(TAVA_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, spr_reg.TULY_SPR_SEQ1);    // note input is seq 1 not 2
  /*p29.VONU*/ spr_reg.VONU_SEQ_xxx34xn.set(TAVA_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, spr_reg.TOBU_SEQ_xx23xx);
  /*p29.SEBA*/ spr_reg.SEBA_SEQ_xxxx45n.set(LAPE_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, spr_reg.VONU_SEQ_xxx34xn); // is this clock wrong?

  // sprite store

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

  // sprite store 0

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

  /*p29.YDUG*/ wire STORE0_MATCHn = nand(MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
  /*p29.WEFU*/ wire STORE0_MATCH = not(STORE0_MATCHn);
  /*p29.GEZE*/ wire STORE0_MATCH_OUT = or (STORE0_MATCH, joy_pins.P10_B);

  /*p29.GUVA*/ wire SPRITE0_GET = nor(STORE0_MATCHn,  joy_pins.P10_B);
  /*p29.EBOJ*/ sst_reg.SPRITE0_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE0_GET);

  /*p29.CEMY*/ wire STORE0_CLK    = or(STORE_EN, STORE0_SEL);
  /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
  /*p29.DYWE*/ wire STORE0_RST    = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE0_GET_SYNCn);
  /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

  /*p29.GENY*/ wire STORE0_CLKa  = not(STORE0_CLKn);
  /*p30.YGUS*/ sst_reg.STORE0_IDX0.set(STORE0_CLK, sil_tri.TS_IDX_0);
  /*p30.YSOK*/ sst_reg.STORE0_IDX1.set(STORE0_CLK, sil_tri.TS_IDX_1);
  /*p30.YZEP*/ sst_reg.STORE0_IDX2.set(STORE0_CLK, sil_tri.TS_IDX_2);
  /*p30.WYTE*/ sst_reg.STORE0_IDX3.set(STORE0_CLK, sil_tri.TS_IDX_3);
  /*p30.ZONY*/ sst_reg.STORE0_IDX4.set(STORE0_CLK, sil_tri.TS_IDX_4);
  /*p30.YWAK*/ sst_reg.STORE0_IDX5.set(STORE0_CLK, sil_tri.TS_IDX_5);
    
  /*p29.ENOB*/ wire STORE0_CLKb  = not(STORE0_CLKn);
  /*p30.FYHY*/ sst_reg.STORE0_LINE0.set(STORE0_CLKb, sil_tri.TS_LINE_0);
  /*p30.GYHO*/ sst_reg.STORE0_LINE1.set(STORE0_CLKb, sil_tri.TS_LINE_1);
  /*p30.BOZU*/ sst_reg.STORE0_LINE2.set(STORE0_CLKb, sil_tri.TS_LINE_2);
  /*p30.CUFO*/ sst_reg.STORE0_LINE3.set(STORE0_CLKb, sil_tri.TS_LINE_3);

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
  /*p30.ZETU*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_0  = sst_reg.STORE0_IDX0;
  /*p30.ZECE*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_1  = sst_reg.STORE0_IDX1;
  /*p30.ZAVE*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_2  = sst_reg.STORE0_IDX2;
  /*p30.WOKO*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_3  = sst_reg.STORE0_IDX3;
  /*p30.ZUMU*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_4  = sst_reg.STORE0_IDX4;
  /*p30.ZEDY*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_5  = sst_reg.STORE0_IDX5;
  /*p30.GOFO*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_0 = sst_reg.STORE0_LINE0;
  /*p30.WEHE*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_1 = sst_reg.STORE0_LINE1;
  /*p30.AJAL*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_2 = sst_reg.STORE0_LINE2;
  /*p30.BUKY*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_3 = sst_reg.STORE0_LINE3;

  // sprite store 1

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

  /*p29.DYDU*/ wire STORE1_MATCHn = nand(MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
  /*p29.GAJA*/ wire STORE1_MATCH = not(STORE1_MATCHn);
  /*p29.FUMA*/ wire STORE1_MATCH_OUT = or(STORE1_MATCH, STORE0_MATCH_OUT);

  /*p29.ENUT*/ wire SPRITE1_GETn = nor(STORE1_MATCHn, STORE0_MATCH_OUT);

  /*p29.BYBY*/ wire STORE1_CLK  = or(STORE_EN, STORE1_SEL);
  /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);

  /*p29.CEDY*/ sst_reg.SPRITE1_GET_SYNCn.set(WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE1_GETn);
  /*p29.EFEV*/ wire STORE1_RST = or(DYBA_VID_LINE_TRIG_d4, sst_reg.SPRITE1_GET_SYNCn);
  /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

  /*p29.BYVY*/ wire STORE1_CLKc = not(STORE1_CLKn);
  /*p30.CAJY*/ sst_reg.STORE1_IDX0.set(STORE1_CLKc, sil_tri.TS_IDX_0);
  /*p30.CUZA*/ sst_reg.STORE1_IDX1.set(STORE1_CLKc, sil_tri.TS_IDX_1);
  /*p30.COMA*/ sst_reg.STORE1_IDX2.set(STORE1_CLKc, sil_tri.TS_IDX_2);
  /*p30.CUFA*/ sst_reg.STORE1_IDX3.set(STORE1_CLKc, sil_tri.TS_IDX_3);
  /*p30.CEBO*/ sst_reg.STORE1_IDX4.set(STORE1_CLKc, sil_tri.TS_IDX_4);
  /*p30.CADU*/ sst_reg.STORE1_IDX5.set(STORE1_CLKc, sil_tri.TS_IDX_5);
                                                                                           
  /*p29.AHOF*/ wire STORE1_CLKb = not(STORE1_CLKn);
  /*p30.ABUG*/ sst_reg.STORE1_LINE0.set(STORE1_CLKb, sil_tri.TS_LINE_0);
  /*p30.AMES*/ sst_reg.STORE1_LINE1.set(STORE1_CLKb, sil_tri.TS_LINE_1);
  /*p30.ABOP*/ sst_reg.STORE1_LINE2.set(STORE1_CLKb, sil_tri.TS_LINE_2);
  /*p30.AROF*/ sst_reg.STORE1_LINE3.set(STORE1_CLKb, sil_tri.TS_LINE_3);

  /*p29.ASYS*/ wire STORE1_CLKa = not(STORE1_CLKn);
  /*p31.DANY*/ sst_reg.STORE1_X0.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X0);
  /*p31.DUKO*/ sst_reg.STORE1_X1.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X1);
  /*p31.DESU*/ sst_reg.STORE1_X2.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X2);
  /*p31.DAZO*/ sst_reg.STORE1_X3.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X3);
  /*p31.DAKE*/ sst_reg.STORE1_X4.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X4);
  /*p31.CESO*/ sst_reg.STORE1_X5.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X5);
  /*p31.DYFU*/ sst_reg.STORE1_X6.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X6);
  /*p31.CUSY*/ sst_reg.STORE1_X7.set(STORE1_CLKa, STORE1_RSTn, SPRITE_X7);

  /*p29.DYDO*/ wire SPRITE1_GET = not(SPRITE1_GETn);
  /*p30.BEMO*/ if (SPRITE1_GET) sil_tri.TS_IDX_0  = not(!sst_reg.STORE1_IDX0);
  /*p30.CYBY*/ if (SPRITE1_GET) sil_tri.TS_IDX_1  = not(!sst_reg.STORE1_IDX1);
  /*p30.BETY*/ if (SPRITE1_GET) sil_tri.TS_IDX_2  = not(!sst_reg.STORE1_IDX2);
  /*p30.CEGY*/ if (SPRITE1_GET) sil_tri.TS_IDX_3  = not(!sst_reg.STORE1_IDX3);
  /*p30.CELU*/ if (SPRITE1_GET) sil_tri.TS_IDX_4  = not(!sst_reg.STORE1_IDX4);
  /*p30.CUBO*/ if (SPRITE1_GET) sil_tri.TS_IDX_5  = not(!sst_reg.STORE1_IDX5);
  /*p30.BEFE*/ if (SPRITE1_GET) sil_tri.TS_LINE_0 = not(!sst_reg.STORE1_LINE0);
  /*p30.BYRO*/ if (SPRITE1_GET) sil_tri.TS_LINE_1 = not(!sst_reg.STORE1_LINE1);
  /*p30.BACO*/ if (SPRITE1_GET) sil_tri.TS_LINE_2 = not(!sst_reg.STORE1_LINE2);
  /*p30.AHUM*/ if (SPRITE1_GET) sil_tri.TS_LINE_3 = not(!sst_reg.STORE1_LINE3);


}


//-----------------------------------------------------------------------------