#pragma once

#include "Sch_Timer.h"
#include "Sch_LCD.h"
#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_PPU.h"
#include "Sch_Window.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_PixPipe.h"
#include "Sch_Joypad.h"
#include "Sch_Serial.h"

#include "Sch_CpuBus.h"
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  void preset_sys();
  void preset_cpu(uint16_t addr, uint8_t data, bool read, bool write);
  void preset_ext();
  void preset_joy();
  void preset_vram();
  void preset_oam();

  SignalHash tick();

  //-----------------------------------------------------------------------------

  void tick_vram_pins();
  void tick_top_regs();
  void tick_cpu_bus();

  //-----------------------------------------------------------------------------

  SignalHash commit_io();
  SignalHash commit_ibus();
  SignalHash commit_vbus();
  SignalHash commit_ebus();
  SignalHash commit_obus();

  SignalHash commit_lcd_pins();
  SignalHash commit_joy_pins();

  SignalHash commit_top_regs();

  //-----------------------------------------------------------------------------
  // Debug signals

  wire UMUT_MODE_DBG1p() const;
  wire UNOR_MODE_DBG2p() const;
  wire UPOJ_MODE_PROD() const;
  wire TOVA_MODE_DBG2n() const;
  wire TUTO_DBG_VRAMp() const;

  //-----------------------------------------------------------------------------
  // Clock signals

  wire AFUR_xBCDExxx() const;
  wire ALEF_xxCDEFxx() const;
  wire APUK_xxxDEFGx() const;
  wire ADYK_xxxxEFGH() const;

  wire WUVU_AxxDExxH() const;
  wire VENA_xBCDExxx() const;
  wire WOSU_xxCDxxGH() const;

  wire BYJU_AxCDEFGH() const;

  //-----------------------------------------------------------------------------
  // Timer signals

  wire UVYN_DIV_05n() const;
  wire UMEK_DIV_06n() const;
  wire UREK_DIV_07n() const;
  wire UPOF_DIV_15() const;
  wire MOBA_INT_TIMERp() const;

  //-----------------------------------------------------------------------------
  // Reset signals

  wire AVOR_RSTp() const;
  wire XAPO_VID_RSTn() const;

  //-----------------------------------------------------------------------------
  // CPU signals

  wire UJYV_CPU_RD() const;
  wire UBAL_CPU_WRp_ABCDExxx() const;

  int get_addr() const;
  int get_data() const;
  void set_data(bool oe, uint8_t data);

  //-----------------------------------------------------------------------------
  // Bus mux signals

  /*p28.GEKA*/ wire GEKA_OAM_A0p() const;
  /*p28.ZYFO*/ wire ZYFO_OAM_A1p() const;
  /*p28.YFOT*/ wire YFOT_OAM_A2p() const;
  /*p28.YFOC*/ wire YFOC_OAM_A3p() const;
  /*p28.YVOM*/ wire YVOM_OAM_A4p() const;
  /*p28.YMEV*/ wire YMEV_OAM_A5p() const;
  /*p28.XEMU*/ wire XEMU_OAM_A6p() const;
  /*p28.YZET*/ wire YZET_OAM_A7p() const;

  /*p31.YLOR*/ wire YLOR_SPRITE_X0() const;
  /*p31.ZYTY*/ wire ZYTY_SPRITE_X1() const;
  /*p31.ZYVE*/ wire ZYVE_SPRITE_X2() const;
  /*p31.ZEZY*/ wire ZEZY_SPRITE_X3() const;
  /*p31.GOMO*/ wire GOMO_SPRITE_X4() const;
  /*p31.BAXO*/ wire BAXO_SPRITE_X5() const;
  /*p31.YZOS*/ wire YZOS_SPRITE_X6() const;
  /*p31.DEPO*/ wire DEPO_SPRITE_X7() const;

  /*p29.XUSO*/ wire XUSO_SPRITE_Y0() const;
  /*p29.XEGU*/ wire XEGU_SPRITE_Y1() const;
  /*p29.YJEX*/ wire YJEX_SPRITE_Y2() const;
  /*p29.XYJU*/ wire XYJU_SPRITE_Y3() const;
  /*p29.YBOG*/ wire YBOG_SPRITE_Y4() const;
  /*p29.WYSO*/ wire WYSO_SPRITE_Y5() const;
  /*p29.XOTE*/ wire XOTE_SPRITE_Y6() const;
  /*p29.YZAB*/ wire YZAB_SPRITE_Y7() const;

  //-----------------------------------------------------------------------------
  // DMA signals

  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const;
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp() const;

  /*p04.NAKY*/ wire DMA_A00() const;
  /*p04.PYRO*/ wire DMA_A01() const;
  /*p04.NEFY*/ wire DMA_A02() const;
  /*p04.MUTY*/ wire DMA_A03() const;
  /*p04.NYKO*/ wire DMA_A04() const;
  /*p04.PYLO*/ wire DMA_A05() const;
  /*p04.NUTO*/ wire DMA_A06() const;
  /*p04.MUGU*/ wire DMA_A07() const;
  /*p04.NAFA*/ wire DMA_A08() const;
  /*p04.PYNE*/ wire DMA_A09() const;
  /*p04.PARA*/ wire DMA_A10() const;
  /*p04.NYDO*/ wire DMA_A11() const;
  /*p04.NYGY*/ wire DMA_A12() const;
  /*p04.PULA*/ wire DMA_A13() const;
  /*p04.POKU*/ wire DMA_A14() const;
  /*p04.MARU*/ wire DMA_A15() const;

  //-----------------------------------------------------------------------------
  // LCD signals

  wire BYHA_VID_LINE_TRIG_d4n() const;

  wire RUTU_LINE_END_F() const;
  wire CATU_LINE_END_B() const;
  wire POPU_VBLANK_d4() const;
  wire ROPO_LY_MATCH_SYNCp() const;
  wire MUWY_Y0() const;
  wire MYRO_Y1() const;
  wire LEXA_Y2() const;
  wire LYDO_Y3() const;
  wire LOVU_Y4() const;
  wire LEMA_Y5() const;
  wire MATO_Y6() const;
  wire LAFO_Y7() const;

  //-----------------------------------------------------------------------------
  // Address decoder signals

  wire WERO_FF40_FF4Fp() const;

  //-----------------------------------------------------------------------------
  // PPU signals

  wire XYMU_RENDERINGp() const;
  wire TEVO_FINE_RSTp() const;
  wire SERE_VRAM_RD() const;
  wire NYXU_TILE_FETCHER_RSTn() const;
  wire ACYL_SCANNINGp() const;
  wire LYRY_BFETCH_DONEp() const;
  wire SEGU_CLKPIPEn() const;
  wire SACU_CLKPIPEp() const;
  wire ROZE_FINE_COUNT_7n() const;
  wire VOTY_INT_STATp() const;
  wire TEKY_SPRITE_FETCH() const;
  wire VEKU_SFETCH_RUNNING_RSTn() const;

  wire XEHO_X0() const;
  wire SAVY_X1() const;
  wire XODU_X2() const;
  wire XYDO_X3() const;
  wire TUHU_X4() const;
  wire TUKY_X5() const;
  wire TAKO_X6() const;
  wire SYBE_X7() const;

  //-----------------------------------------------------------------------------
  // Tile fetcher signals

  wire PORY_FETCH_DONE_Bp() const;
  wire LONY_BG_READ_VRAM_LATCHp() const;

  wire DATY_SCX0() const;
  wire DUZU_SCX1() const;
  wire CYXU_SCX2() const;

  //-----------------------------------------------------------------------------
  // Sprite scanner signals

  wire CEHA_SCANNINGp() const;
  wire BESU_SCANNINGp() const;
  wire CENO_SCANNINGp() const;

  wire AVAP_RENDER_START_RST() const;
  wire CARE_STORE_ENp_ABxxEFxx() const;

  wire DEGE_SPRITE_DELTA0() const;
  wire DABY_SPRITE_DELTA1() const;
  wire DABU_SPRITE_DELTA2() const;
  wire GYSA_SPRITE_DELTA3() const;

  wire YFEL_SCAN0() const;
  wire WEWY_SCAN1() const;
  wire GOSO_SCAN2() const;
  wire ELYN_SCAN3() const;
  wire FAHA_SCAN4() const;
  wire FONY_SCAN5() const;

  //-----------------------------------------------------------------------------
  // Sprite store signals

  wire FEPO_STORE_MATCHp() const;

  //-----------------------------------------------------------------------------
  // Sprite fetcher signals

  wire WEFY_SPR_READp() const;
  wire VAPE_FETCH_OAM_CLK() const;
  wire VUSA_PIPE_LOAD_SPRITEn() const;
  wire SOWO_SFETCH_RUNNINGn() const;
  wire TEXY_SPRITE_READp() const;
  wire SOHO_SPR_VRAM_RDp() const;

  wire SPR_PIX_A0() const;
  wire SPR_PIX_A1() const;
  wire SPR_PIX_A2() const;
  wire SPR_PIX_A3() const;
  wire SPR_PIX_A4() const;
  wire SPR_PIX_A5() const;
  wire SPR_PIX_A6() const;
  wire SPR_PIX_A7() const;

  wire SPR_PIX_B0() const;
  wire SPR_PIX_B1() const;
  wire SPR_PIX_B2() const;
  wire SPR_PIX_B3() const;
  wire SPR_PIX_B4() const;
  wire SPR_PIX_B5() const;
  wire SPR_PIX_B6() const;
  wire SPR_PIX_B7() const;

  //-----------------------------------------------------------------------------
  // Window signals

  wire NOCU_WIN_MODEn() const;
  wire NUNY_WX_MATCHpe() const;

  wire WYKA_WIN_X3() const;
  wire WODY_WIN_X4() const;
  wire WOBO_WIN_X5() const;
  wire WYKO_WIN_X6() const;
  wire XOLO_WIN_X7() const;

  wire VYNO_WIN_Y0() const;
  wire VUJO_WIN_Y1() const;
  wire VYMU_WIN_Y2() const;
  wire TUFU_WIN_Y3() const;
  wire TAXA_WIN_Y4() const;
  wire TOZO_WIN_Y5() const;
  wire TATE_WIN_Y6() const;
  wire TEKE_WIN_Y7() const;

  //-----------------------------------------------------------------------------
  // Misc signals

  wire BOOT_BITn() const;
  wire ASOK_INT_JOYPADp() const;
  wire CALY_INT_SERIALp() const;

  //-----------------------------------------------------------------------------
  // Internal state for debugging

  int phase_counter = -32;

  //-----------------------------------------------------------------------------
  // Top level registers

  // FF40 - LCDC (in top because it's used everywhere)
  /*p23.VYXE*/ Reg9 VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg9 XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg9 XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg9 XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg9 WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg9 WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg9 WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg9 XONA_LCDC_EN;

  /*p32.LEGU*/ Reg8 LEGU_BG_PIX_A0;
  /*p32.NUDU*/ Reg8 NUDU_BG_PIX_A1;
  /*p32.MUKU*/ Reg8 MUKU_BG_PIX_A2;
  /*p32.LUZO*/ Reg8 LUZO_BG_PIX_A3;
  /*p32.MEGU*/ Reg8 MEGU_BG_PIX_A4;
  /*p32.MYJY*/ Reg8 MYJY_BG_PIX_A5;
  /*p32.NASA*/ Reg8 NASA_BG_PIX_A6;
  /*p32.NEFO*/ Reg8 NEFO_BG_PIX_A7; // color wrong on die

  /*p32.RAWU*/ Reg13 RAWU_BG_PIX_B0; // also holds tile index during fetch
  /*p32.POZO*/ Reg13 POZO_BG_PIX_B1;
  /*p32.PYZO*/ Reg13 PYZO_BG_PIX_B2; 
  /*p32.POXA*/ Reg13 POXA_BG_PIX_B3; 
  /*p32.PULO*/ Reg13 PULO_BG_PIX_B4; 
  /*p32.POJU*/ Reg13 POJU_BG_PIX_B5; 
  /*p32.POWY*/ Reg13 POWY_BG_PIX_B6; 
  /*p32.PYJU*/ Reg13 PYJU_BG_PIX_B7;

  //-----------------------------------------------------------------------------
  // Internal bus to CPU

  PinOut CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  CPU_PIN_RD;            // top right port PORTA_00: ->
  PinIn  CPU_PIN_WR;            // top right port PORTA_01: ->
  PinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut CPU_PIN_SYRO;          // top right port PORTA_03: <- P25.SYRO
  PinOut CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  PinIn  CPU_PIN_ACK_VBLANK;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  PinOut CPU_PIN_INT_VBLANK;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  PinIn  CPU_PIN_ACK_STAT;      // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  PinOut CPU_PIN_INT_STAT;      // bottom right port PORTB_07: <-        P02.LALU, stat int
  PinIn  CPU_PIN_ACK_TIMER;     // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  PinOut CPU_PIN_INT_TIMER;     // bottom right port PORTB_11: <-        P02.NYBO, timer int
  PinIn  CPU_PIN_ACK_SERIAL;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  PinOut CPU_PIN_INT_SERIAL;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  PinIn  CPU_PIN_ACK_JOYPAD;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
  PinOut CPU_PIN_INT_JOYPAD;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  PinIn  CPU_PIN_CLKREQ;        // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  PinOut CPU_PIN_AFER_RSTp;     // top center port PORTC_01: <- P01.AFER , reset related reg
  PinOut CPU_PIN_EXT_RESET;     // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut CPU_PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut CPU_PIN_TABA_RSTp;     // top center port PORTC_04: <- P01.CPU_RESET

  PinIn  CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinOut CPU_PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  PinOut CPU_PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx
  PinOut CPU_PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  PinOut CPU_PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA
  PinOut CPU_PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  PinIn  CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinOut CPU_PIN_BUKE_ABxxxxxH; // top left port PORTD_07: <- BUKE_ABxxxxxH
  PinOut CPU_PIN_BOMA_xBxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  PinOut CPU_PIN_BOGA_AxCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3

  PinIn  CPU_PIN_A00;           // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01;           // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02;           // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03;           // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04;           // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05;           // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06;           // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07;           // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08;           // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09;           // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10;           // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11;           // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12;           // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13;           // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14;           // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15;           // bottom right port PORTB_30: -> A15
                                
  Tribuf CPU_TRI_D0;            // bottom left port: <>
  Tribuf CPU_TRI_D1;            // bottom left port: <>
  Tribuf CPU_TRI_D2;            // bottom left port: <>
  Tribuf CPU_TRI_D3;            // bottom left port: <>
  Tribuf CPU_TRI_D4;            // bottom left port: <>
  Tribuf CPU_TRI_D5;            // bottom left port: <>
  Tribuf CPU_TRI_D6;            // bottom left port: <>
  Tribuf CPU_TRI_D7;            // bottom left port: <>

  //-----------------------------------------------------------------------------
  // OAM bus

  PinOut OAM_PIN_CLK;
  PinOut OAM_PIN_OE;
  PinOut OAM_PIN_WR_A;
  PinOut OAM_PIN_WR_B;

  PinOut OAM_PIN_A0;
  PinOut OAM_PIN_A1;
  PinOut OAM_PIN_A2;
  PinOut OAM_PIN_A3;
  PinOut OAM_PIN_A4;
  PinOut OAM_PIN_A5;
  PinOut OAM_PIN_A6;
  PinOut OAM_PIN_A7;

  Tribuf OAM_PIN_DA0;
  Tribuf OAM_PIN_DA1;
  Tribuf OAM_PIN_DA2;
  Tribuf OAM_PIN_DA3;
  Tribuf OAM_PIN_DA4;
  Tribuf OAM_PIN_DA5;
  Tribuf OAM_PIN_DA6;
  Tribuf OAM_PIN_DA7;

  Tribuf OAM_PIN_DB0;
  Tribuf OAM_PIN_DB1;
  Tribuf OAM_PIN_DB2;
  Tribuf OAM_PIN_DB3;
  Tribuf OAM_PIN_DB4;
  Tribuf OAM_PIN_DB5;
  Tribuf OAM_PIN_DB6;
  Tribuf OAM_PIN_DB7;

  //-----------------------------------------------------------------------------
  // LCD pins

  PinOut LCD_PIN_LD1;  // PIN_50 
  PinOut LCD_PIN_LD0;  // PIN_51 
  PinOut LCD_PIN_CPG;  // PIN_52 
  PinOut LCD_PIN_CP;   // PIN_53 
  PinOut LCD_PIN_ST;   // PIN_54 
  PinOut LCD_PIN_CPL;  // PIN_55 
  PinOut LCD_PIN_FR;   // PIN_56 
  PinOut LCD_PIN_S;    // PIN_57 

  //-----------------------------------------------------------------------------
  // Sys pins

  PinIn  SYS_PIN_RST;          // PIN_71 
  PinIn  SYS_PIN_CLK_xBxDxFxH; // PIN_74 
  PinIn  SYS_PIN_CLK_GOOD;     // PIN_74 
  PinIn  SYS_PIN_T2;           // PIN_76 
  PinIn  SYS_PIN_T1;           // PIN_77 

  //-----------------------------------------------------------------------------
  // Joypad pins

  // The B connections on the joypad pins are werid.
  // They seem to be used as an input, or at least I can't find the driver
  // PESU
  // RARU ROWE RYKE RYNE RASE REJY REKA ROMY
  // RUNY VYPO TOMY? SEZU? RAWU? PUTE? MYDE RUGO? NYLU WYMO?
  // WEFE WUWE GEFY WYGA? FABY ECAB? DYSO ERUC GEZE GUVA 
  // ARAR ATAJ ASUZ AJEC AKAJ ANOC BENU BEDA
  // BEKU

  PinOut JOY_PIN_P10_A;   // PIN_67 <- P05.KOLE
  PinOut JOY_PIN_P10_B;   // PIN_67 -> BENU BEDA ATAJ ASUZ AJEC AKAJ ANOC ARAR
  PinIn  JOY_PIN_P10_C;   // PIN_67 -> P02.KERY, P05.KEVU
  PinOut JOY_PIN_P10_D;   // PIN_67 <- P05.KYBU

  PinOut JOY_PIN_P11_A;   // PIN_66 <- P05.KYTO
  PinOut JOY_PIN_P11_B;   // PIN_66 
  PinIn  JOY_PIN_P11_C;   // PIN_66 -> P02.KERY, P05.KAPA
  PinOut JOY_PIN_P11_D;   // PIN_66 <- P05.KABU

  PinOut JOY_PIN_P12_A;   // PIN_65 <- P05.KYHU
  PinOut JOY_PIN_P12_B;   // PIN_65 
  PinIn  JOY_PIN_P12_C;   // PIN_65 -> P02.KERY, P05.KEJA
  PinOut JOY_PIN_P12_D;   // PIN_65 <- P05.KASY

  PinOut JOY_PIN_P13_A;   // PIN_64 <- P05.KORY
  PinOut JOY_PIN_P13_B;   // PIN_64 
  PinIn  JOY_PIN_P13_C;   // PIN_64 -> P02.KERY, P05.KOLO
  PinOut JOY_PIN_P13_D;   // PIN_64 <- P05.KALE

  PinOut JOY_PIN_P14_A;   // PIN_63 <- p05.KARU
  PinOut JOY_PIN_P14_D;   // PIN_63 <- p05.KELY

  PinOut JOY_PIN_P15_A;   // PIN_62 <- p05.CELA
  PinOut JOY_PIN_P15_D;   // PIN_62 <- p05.COFY

  //-----------------------------------------------------------------------------
  // Ext bus

  PinOut EXT_PIN_CLK;     // PIN_75 <- P01.BUDE/BEVA

  PinOut EXT_PIN_WRn_A;   // PIN_78 <- P08.UVER
  PinIn  EXT_PIN_WRn_C;   // PIN_78 -> P07.UBAL
  PinOut EXT_PIN_WRn_D;   // PIN_78 <- P08.USUF

  PinOut EXT_PIN_RDn_A;   // PIN_79 <- P08.UGAC
  PinIn  EXT_PIN_RDn_C;   // PIN_79 -> P07.UJYV
  PinOut EXT_PIN_RDn_D;   // PIN_79 <- P08.URUN

  PinOut EXT_PIN_CSn_A;   // PIN_80 <- P08.TYHO

  PinOut EXT_PIN_A00_A;   // PIN_01 <- P08.KUPO
  PinIn  EXT_PIN_A00_C;   // PIN_01 -> P08.KOVA
  PinOut EXT_PIN_A00_D;   // PIN_01 <- P08.KOTY

  PinOut EXT_PIN_A01_A;   // PIN_02 <- P08.CABA
  PinIn  EXT_PIN_A01_C;   // PIN_02 -> P08.CAMU
  PinOut EXT_PIN_A01_D;   // PIN_02 <- P08.COTU

  PinOut EXT_PIN_A02_A;   // PIN_03 <- P08.BOKU
  PinIn  EXT_PIN_A02_C;   // PIN_03 -> P08.BUXU
  PinOut EXT_PIN_A02_D;   // PIN_03 <- P08.BAJO

  PinOut EXT_PIN_A03_A;   // PIN_04 <- P08.BOTY
  PinIn  EXT_PIN_A03_C;   // PIN_04 -> P08.BASE
  PinOut EXT_PIN_A03_D;   // PIN_04 <- P08.BOLA

  PinOut EXT_PIN_A04_A;   // PIN_05 <- P08.BYLA
  PinIn  EXT_PIN_A04_C;   // PIN_05 -> P08.AFEC
  PinOut EXT_PIN_A04_D;   // PIN_05 <- P08.BEVO

  PinOut EXT_PIN_A05_A;   // PIN_06 <- P08.BADU
  PinIn  EXT_PIN_A05_C;   // PIN_06 -> P08.ABUP
  PinOut EXT_PIN_A05_D;   // PIN_06 <- P08.AJAV

  PinOut EXT_PIN_A06_A;   // PIN_07 <- P08.CEPU
  PinIn  EXT_PIN_A06_C;   // PIN_07 -> P08.CYGU
  PinOut EXT_PIN_A06_D;   // PIN_07 <- P08.CYKA

  PinOut EXT_PIN_A07_A;   // PIN_08 <- P08.DEFY
  PinIn  EXT_PIN_A07_C;   // PIN_08 -> P08.COGO
  PinOut EXT_PIN_A07_D;   // PIN_08 <- P08.COLO

  PinOut EXT_PIN_A08_A;   // PIN_09 <- P08.MYNY
  PinIn  EXT_PIN_A08_C;   // PIN_09 -> P08.MUJY
  PinOut EXT_PIN_A08_D;   // PIN_09 <- P08.MEGO

  PinOut EXT_PIN_A09_A;   // PIN_10 <- P08.MUNE
  PinIn  EXT_PIN_A09_C;   // PIN_10 -> P08.NENA
  PinOut EXT_PIN_A09_D;   // PIN_10 <- P08.MENY

  PinOut EXT_PIN_A10_A;   // PIN_11 <- P08.ROXU
  PinIn  EXT_PIN_A10_C;   // PIN_11 -> P08.SURA
  PinOut EXT_PIN_A10_D;   // PIN_11 <- P08.RORE

  PinOut EXT_PIN_A11_A;   // PIN_12 <- P08.LEPY
  PinIn  EXT_PIN_A11_C;   // PIN_12 -> P08.MADY
  PinOut EXT_PIN_A11_D;   // PIN_12 <- P08.LYNY

  PinOut EXT_PIN_A12_A;   // PIN_13 <- P08.LUCE
  PinIn  EXT_PIN_A12_C;   // PIN_13 -> P08.LAHE
  PinOut EXT_PIN_A12_D;   // PIN_13 <- P08.LOSO

  PinOut EXT_PIN_A13_A;   // PIN_14 <- P08.LABE
  PinIn  EXT_PIN_A13_C;   // PIN_14 -> P08.LURA
  PinOut EXT_PIN_A13_D;   // PIN_14 <- P08.LEVA

  PinOut EXT_PIN_A14_A;   // PIN_15 <- P08.PUHE
  PinIn  EXT_PIN_A14_C;   // PIN_15 -> P08.PEVO
  PinOut EXT_PIN_A14_D;   // PIN_15 <- P08.PAHY

  PinOut EXT_PIN_A15_A;   // PIN_16 <- P08.SUZE
  PinIn  EXT_PIN_A15_C;   // PIN_16 -> P08.RAZA
  PinOut EXT_PIN_A15_D;   // PIN_16 <- P08.RULO

  PinOut EXT_PIN_D0_A;    // PIN_17 <- P08.RUXA
  PinOut EXT_PIN_D0_B;    // PIN_17 <- P08.LULA
  PinIn  EXT_PIN_D0_C;    // PIN_17 -> P08.TOVO,SOMA
  PinOut EXT_PIN_D0_D;    // PIN_17 <- P08.RUNE

  PinOut EXT_PIN_D1_A;    // PIN_18 <- P08.RUJA
  PinOut EXT_PIN_D1_B;    // PIN_18 <- P08.LULA
  PinIn  EXT_PIN_D1_C;    // PIN_18 -> P08.RUZY,RONY
  PinOut EXT_PIN_D1_D;    // PIN_18 <- P08.RYPU

  PinOut EXT_PIN_D2_A;    // PIN_19 <- P08.RABY
  PinOut EXT_PIN_D2_B;    // PIN_19 <- P08.LULA
  PinIn  EXT_PIN_D2_C;    // PIN_19 -> P08.ROME,RAXY
  PinOut EXT_PIN_D2_D;    // PIN_19 <- P08.SULY

  PinOut EXT_PIN_D3_A;    // PIN_20 <- P08.RERA
  PinOut EXT_PIN_D3_B;    // PIN_20 <- P08.LULA
  PinIn  EXT_PIN_D3_C;    // PIN_20 -> P08.SAZA,SELO
  PinOut EXT_PIN_D3_D;    // PIN_20 <- P08.SEZE

  PinOut EXT_PIN_D4_A;    // PIN_21 <- P08.RORY
  PinOut EXT_PIN_D4_B;    // PIN_21 <- P08.LULA
  PinIn  EXT_PIN_D4_C;    // PIN_21 -> P08.TEHE,SODY
  PinOut EXT_PIN_D4_D;    // PIN_21 <- P08.RESY

  PinOut EXT_PIN_D5_A;    // PIN_22 <- P08.RYVO
  PinOut EXT_PIN_D5_B;    // PIN_22 <- P08.LULA
  PinIn  EXT_PIN_D5_C;    // PIN_22 -> P08.RATU,SAGO
  PinOut EXT_PIN_D5_D;    // PIN_22 <- P08.TAMU

  PinOut EXT_PIN_D6_A;    // PIN_23 <- P08.RAFY
  PinOut EXT_PIN_D6_B;    // PIN_23 <- P08.LULA
  PinIn  EXT_PIN_D6_C;    // PIN_23 -> P08.SOCA,RUPA
  PinOut EXT_PIN_D6_D;    // PIN_23 <- P08.ROGY

  PinOut EXT_PIN_D7_A;    // PIN_24 <- P08.RAVU
  PinOut EXT_PIN_D7_B;    // PIN_24 <- P08.LULA
  PinIn  EXT_PIN_D7_C;    // PIN_24 -> P08.RYBA,SAZY
  PinOut EXT_PIN_D7_D;    // PIN_24 <- P08.RYDA

  //-----------------------------------------------------------------------------
  // VRAM bus

  PinOut VRAM_PIN_MCSn_A;   // PIN_43 <- P25.SOKY
  PinIn  VRAM_PIN_MCSn_C;   // PIN_43 -> P25.TEFY
  PinOut VRAM_PIN_MCSn_D;   // PIN_43 <- P25.SETY

  PinOut VRAM_PIN_MOEn_A;   // PIN_45 <- P25.REFO
  PinIn  VRAM_PIN_MOEn_C;   // PIN_45 -> P25.TAVY
  PinOut VRAM_PIN_MOEn_D;   // PIN_45 <- P25.SAHA

  PinOut VRAM_PIN_MWRn_A;   // PIN_49 <- P25.SYSY
  PinIn  VRAM_PIN_MWRn_C;   // PIN_49 -> P25.SUDO
  PinOut VRAM_PIN_MWRn_D;   // PIN_49 <- P25.RAGU

  PinOut VRAM_PIN_MA00_AD;  // PIN_34 <- P04.ECAL
  PinOut VRAM_PIN_MA01_AD;  // PIN_35 <- P04.EGEZ
  PinOut VRAM_PIN_MA02_AD;  // PIN_36 <- P04.FUHE
  PinOut VRAM_PIN_MA03_AD;  // PIN_37 <- P04.FYZY
  PinOut VRAM_PIN_MA04_AD;  // PIN_38 <- P04.DAMU
  PinOut VRAM_PIN_MA05_AD;  // PIN_39 <- P04.DAVA
  PinOut VRAM_PIN_MA06_AD;  // PIN_40 <- P04.ETEG
  PinOut VRAM_PIN_MA07_AD;  // PIN_41 <- P04.EREW
  PinOut VRAM_PIN_MA08_AD;  // PIN_48 <- P04.EVAX
  PinOut VRAM_PIN_MA09_AD;  // PIN_47 <- P04.DUVE
  PinOut VRAM_PIN_MA10_AD;  // PIN_44 <- P04.ERAF
  PinOut VRAM_PIN_MA11_AD;  // PIN_46 <- P04.FUSY
  PinOut VRAM_PIN_MA12_AD;  // PIN_42 <- P04.EXYF

  PinOut VRAM_PIN_MD0_A;    // PIN_33 <- P25.REGE
  PinOut VRAM_PIN_MD1_A;    // PIN_31 <- P25.RYKY
  PinOut VRAM_PIN_MD2_A;    // PIN_30 <- P25.RAZO
  PinOut VRAM_PIN_MD3_A;    // PIN_29 <- P25.RADA
  PinOut VRAM_PIN_MD4_A;    // PIN_28 <- P25.RYRO
  PinOut VRAM_PIN_MD5_A;    // PIN_27 <- P25.REVU
  PinOut VRAM_PIN_MD6_A;    // PIN_26 <- P25.REKU
  PinOut VRAM_PIN_MD7_A;    // PIN_25 <- P25.RYZE

  PinOut VRAM_PIN_MD0_B;    // PIN_33 <- P25.ROFA
  PinOut VRAM_PIN_MD1_B;    // PIN_31 <- P25.ROFA
  PinOut VRAM_PIN_MD2_B;    // PIN_30 <- P25.ROFA
  PinOut VRAM_PIN_MD3_B;    // PIN_29 <- P25.ROFA
  PinOut VRAM_PIN_MD4_B;    // PIN_28 <- P25.ROFA
  PinOut VRAM_PIN_MD5_B;    // PIN_27 <- P25.ROFA
  PinOut VRAM_PIN_MD6_B;    // PIN_26 <- P25.ROFA
  PinOut VRAM_PIN_MD7_B;    // PIN_25 <- P25.ROFA

  PinIn  VRAM_PIN_MD0_C;    // PIN_33 -> P25.RODY
  PinIn  VRAM_PIN_MD1_C;    // PIN_31 -> P25.REBA
  PinIn  VRAM_PIN_MD2_C;    // PIN_30 -> P25.RYDO
  PinIn  VRAM_PIN_MD3_C;    // PIN_29 -> P25.REMO
  PinIn  VRAM_PIN_MD4_C;    // PIN_28 -> P25.ROCE
  PinIn  VRAM_PIN_MD5_C;    // PIN_27 -> P25.ROPU
  PinIn  VRAM_PIN_MD6_C;    // PIN_26 -> P25.RETA
  PinIn  VRAM_PIN_MD7_C;    // PIN_25 -> P25.RAKU

  PinOut VRAM_PIN_MD0_D;    // PIN_33 <- P25.RURA
  PinOut VRAM_PIN_MD1_D;    // PIN_31 <- P25.RULY
  PinOut VRAM_PIN_MD2_D;    // PIN_30 <- P25.RARE
  PinOut VRAM_PIN_MD3_D;    // PIN_29 <- P25.RODU
  PinOut VRAM_PIN_MD4_D;    // PIN_28 <- P25.RUBE
  PinOut VRAM_PIN_MD5_D;    // PIN_27 <- P25.RUMU
  PinOut VRAM_PIN_MD6_D;    // PIN_26 <- P25.RYTY
  PinOut VRAM_PIN_MD7_D;    // PIN_25 <- P25.RADY

  Tribuf VRM_TRI_A00;
  Tribuf VRM_TRI_A01;
  Tribuf VRM_TRI_A02;
  Tribuf VRM_TRI_A03;
  Tribuf VRM_TRI_A04;
  Tribuf VRM_TRI_A05;
  Tribuf VRM_TRI_A06;
  Tribuf VRM_TRI_A07;
  Tribuf VRM_TRI_A08;
  Tribuf VRM_TRI_A09;
  Tribuf VRM_TRI_A10;
  Tribuf VRM_TRI_A11;
  Tribuf VRM_TRI_A12;

  Tribuf VRM_TRI_D0;
  Tribuf VRM_TRI_D1;
  Tribuf VRM_TRI_D2;
  Tribuf VRM_TRI_D3;
  Tribuf VRM_TRI_D4;
  Tribuf VRM_TRI_D5;
  Tribuf VRM_TRI_D6;
  Tribuf VRM_TRI_D7;

  //-----------------------------------------------------------------------------
  // Sprite store tribufs

  Tribuf SPR_TRI_IDX_0;
  Tribuf SPR_TRI_IDX_1;
  Tribuf SPR_TRI_IDX_2;
  Tribuf SPR_TRI_IDX_3;
  Tribuf SPR_TRI_IDX_4;
  Tribuf SPR_TRI_IDX_5;

  Tribuf SPR_TRI_LINE_0;
  Tribuf SPR_TRI_LINE_1;
  Tribuf SPR_TRI_LINE_2;
  Tribuf SPR_TRI_LINE_3;

  //-----------------------------------------------------------------------------
  // Sub-modules

private:

  ClockRegisters clk_reg;
  DebugRegisters dbg_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters joy_reg;
  LcdRegisters lcd_reg;
  PixelPipeRegisters pxp_reg;
  ResetRegisters rst_reg;
  SerialRegisters ser_reg;
  SpriteStoreRegisters sst_reg;
  TimerRegisters tim_reg;
  PpuRegisters ppu_reg;
  WindowRegisters win_reg;
  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  BusMux bus_mux;
  Bootrom bootrom;
};

//-----------------------------------------------------------------------------

};