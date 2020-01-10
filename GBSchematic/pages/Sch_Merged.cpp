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

static Ram8K ram;
static Ram8K vram;

static ExtPins ext_pins;
static VramPins vram_pins;
static SerialPins ser_pins;
static LcdPins lcd_pins;
static JoypadPins joy_pins;
static CpuPins cpu_pins;
static CartPins cart_pins;

static BusTristates bus_tri;

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

  // sch_debug

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

}


//-----------------------------------------------------------------------------