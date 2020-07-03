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

#include "Sch_VramPins.h"
#include "Sch_CpuBus.h"
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"
#include "Sch_PpuConfig.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  void tick_everything();
  SignalHash commit_everything();

  //-----------------------------------------------------------------------------
  // Debug signals

  wire UMUT_MODE_DBG1p() const;
  wire UNOR_MODE_DBG2p() const;
  wire UPOJ_MODE_PROD() const;
  wire TOVA_MODE_DBG2n() const;
  wire TUTO_DBG_VRAMp() const;
  wire ABUZ() const;
  void preset_t1t2(bool t1, bool t2);

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
  void preset_addr(bool oe, uint16_t addr);
  int get_data() const;
  void set_data(bool oe, uint8_t data);
  void preset_rd(bool rd);
  void preset_wr(bool wr);
  void preset_addr_valid(bool valid);

  //-----------------------------------------------------------------------------
  // Bootrom signals

  wire BOOT_BITn() const;
  wire TUTU_BOOTp() const;

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

  //------------------------------------------------------------------------------
  // Joypad signals

  wire ASOK_INT_JPp() const;

  //-----------------------------------------------------------------------------
  // LCD signals


  wire BYHA_VID_LINE_TRIG_d4n() const {
    /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
    /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), ABEZ_VID_RSTn);
    return BYHA_VID_LINE_TRIG_d4n;
  }

  wire RUTU_NEW_LINE_d0() const { return lcd_reg.RUTU_NEW_LINE_d0; }
  wire CATU_VID_LINE_d4() const { return lcd_reg.CATU_VID_LINE_d4; }
  wire POPU_VBLANK_d4() const { return lcd_reg.POPU_VBLANK_d4; }
  wire ROPO_LY_MATCH_SYNCp() const { return lcd_reg.ROPO_LY_MATCH_SYNCp; }
  wire MUWY_Y0() const { return lcd_reg.MUWY_Y0; }
  wire MYRO_Y1() const { return lcd_reg.MYRO_Y1; }
  wire LEXA_Y2() const { return lcd_reg.LEXA_Y2; }
  wire LYDO_Y3() const { return lcd_reg.LYDO_Y3; }
  wire LOVU_Y4() const { return lcd_reg.LOVU_Y4; }
  wire LEMA_Y5() const { return lcd_reg.LEMA_Y5; }
  wire MATO_Y6() const { return lcd_reg.MATO_Y6; }
  wire LAFO_Y7() const { return lcd_reg.LAFO_Y7; }

  //-----------------------------------------------------------------------------
  // Address decoder signals

  wire WERO_FF40_FF4Fp() const {
    /*p07.TONA*/ wire TONA_A08n = not(CPU_PIN_A08);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
    /*p22.XALY*/ wire XALY_0x00xxxxp = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p22.WUTU*/ wire WUTU_FF40_FF4Fn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
    /*p22.WERO*/ wire WERO_FF40_FF4Fp = not(WUTU_FF40_FF4Fn);
    return WERO_FF40_FF4Fp;
  }

  //-----------------------------------------------------------------------------

  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */


  //----------

  PinIn  CPU_PIN_RD;         // top right port PORTA_00
  PinIn  CPU_PIN_WR;         // top right port PORTA_01
  PinIn  CPU_PIN_ADDR_VALID; // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.
  PinIn  CPU_PIN5;           // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinIn  CPU_PIN6;           // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinIn  CPU_PIN_CLKREQ;     // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  PinIn  CPU_PIN_A00; // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01; // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02; // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03; // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04; // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05; // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06; // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07; // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08; // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09; // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10; // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11; // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12; // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13; // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14; // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15; // bottom right port PORTB_30: -> A15

  Tribuf CPU_TRI_D0;
  Tribuf CPU_TRI_D1;
  Tribuf CPU_TRI_D2;
  Tribuf CPU_TRI_D3;
  Tribuf CPU_TRI_D4;
  Tribuf CPU_TRI_D5;
  Tribuf CPU_TRI_D6;
  Tribuf CPU_TRI_D7;

  PinOut CPU_PIN_BOOTp;    // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

  //----------
  // ext bus

  /* PIN_01 */ PinIn EXT_PIN_A00_C;   // -> P08.KOVA
  /* PIN_02 */ PinIn EXT_PIN_A01_C;   // -> P08.CAMU
  /* PIN_03 */ PinIn EXT_PIN_A02_C;   // -> P08.BUXU
  /* PIN_04 */ PinIn EXT_PIN_A03_C;   // -> P08.BASE
  /* PIN_05 */ PinIn EXT_PIN_A04_C;   // -> P08.AFEC
  /* PIN_06 */ PinIn EXT_PIN_A05_C;   // -> P08.ABUP
  /* PIN_07 */ PinIn EXT_PIN_A06_C;   // -> P08.CYGU
  /* PIN_08 */ PinIn EXT_PIN_A07_C;   // -> P08.COGO
  /* PIN_09 */ PinIn EXT_PIN_A08_C;   // -> P08.MUJY
  /* PIN_10 */ PinIn EXT_PIN_A09_C;   // -> P08.NENA
  /* PIN_11 */ PinIn EXT_PIN_A10_C;   // -> P08.SURA
  /* PIN_12 */ PinIn EXT_PIN_A11_C;   // -> P08.MADY
  /* PIN_13 */ PinIn EXT_PIN_A12_C;   // -> P08.LAHE
  /* PIN_14 */ PinIn EXT_PIN_A13_C;   // -> P08.LURA
  /* PIN_15 */ PinIn EXT_PIN_A14_C;   // -> P08.PEVO
  /* PIN_16 */ PinIn EXT_PIN_A15_C;   // -> P08.RAZA
  /* PIN_74 */ PinIn PIN_CLK_IN_xBxDxFxH;
  /* PIN_76 */ PinIn EXT_PIN_T2;
  /* PIN_77 */ PinIn EXT_PIN_T1;
  /* PIN_78 */ PinIn EXT_PIN_WRn_C;   // -> P07.UBAL
  /* PIN_79 */ PinIn EXT_PIN_RD_C;   // -> P07.UJYV

  //----------
  // VRAM bus

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

  //----------

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

  VramPins vram_pins;

  CpuBus  cpu_bus;
  CpuPinsOut cpu_pins_out;

  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  PpuConfig ppu_config;

  BusMux bus_mux;

  Bootrom bootrom;

  /* PIN_71 */ PinIn EXT_PIN_RST;
  /* PIN_74 */ PinIn EXT_PIN_CLK_GOOD;

  /* PIN_17 */ PinIn EXT_PIN_D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ PinIn EXT_PIN_D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ PinIn EXT_PIN_D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ PinIn EXT_PIN_D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ PinIn EXT_PIN_D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ PinIn EXT_PIN_D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ PinIn EXT_PIN_D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ PinIn EXT_PIN_D7_C;    // -> P08.RYBA,SAZY

private:

};

//-----------------------------------------------------------------------------

};