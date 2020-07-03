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
#include "Sch_VramBus.h"
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

  void preset_t1t2(bool t1, bool t2) {
    PIN_T1.preset(true, t1);
    PIN_T2.preset(true, t2);
  }

  void preset_rd(bool rd)            { CPU_PIN_RD.preset(true, rd); }
  bool preset_wr(bool wr)            { CPU_PIN_WR.preset(true, wr); }
  bool preset_addr_valid(bool valid) { CPU_PIN_ADDR_VALID.preset(true, valid); }

  //-----------------------------------------------------------------------------

  wire TUTO_DBG_VRAMp() const {
    /*p07.UBET*/ wire UBET_T1n = not(PIN_T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
    /*p25.TUTO*/ wire TUTO_DBG_VRAMp = and (UNOR_MODE_DBG2p, !dbg_reg.SOTO_DBG);
    return TUTO_DBG_VRAMp;
  }

  wire UPOJ_MODE_PROD() const {
    /*p07.UBET*/ wire UBET_T1n = not(PIN_T1);
    /*p07.UVAR*/ wire UVAR_T2n = not(PIN_T2);
    /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, EXT_PIN_RST);
    return UPOJ_MODE_PROD;
  }

  wire UMUT_MODE_DBG1p() const {
    /*p07.UVAR*/ wire UVAR_T2n = not(PIN_T2);
    /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (PIN_T1, UVAR_T2n);
    return UMUT_MODE_DBG1p;
  }

  wire UNOR_MODE_DBG2p() const {
    /*p07.UBET*/ wire UBET_T1n = not(PIN_T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
    return UNOR_MODE_DBG2p;
  }

  wire TOVA_MODE_DBG2n() const {
    /*p07.UBET*/ wire UBET_T1n = not(PIN_T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
    /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not(UNOR_MODE_DBG2p);
    return TOVA_MODE_DBG2n;
  }

  // still not sure what this is...
  wire ABUZ() const {
    /*p07.UBET*/ wire UBET_T1n = not(PIN_T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
    /*p??.APAP*/ wire APAP = not(CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD = nor(UNOR_MODE_DBG2p, APAP);
    /*p01.ABUZ*/ wire ABUZ = not(AWOD);
    return ABUZ;
  }

  //-----------------------------------------------------------------------------

  wire BOGA_AxCDEFGH() const {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.AFUR_xBCDExxx);
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(!clk_reg.APUK_xxxDEFGx);
    /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.ALEF_xxCDEFxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
    /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
    /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
    /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
    /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
    /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
    /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
    /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(EXT_PIN_CLK_GOOD);
    /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    return BOGA_AxCDEFGH;
  }

  //-----------------------------------------------------------------------------

  wire AVOR_RSTp() const {
    /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
    return AVOR_RSTp;
  }

  wire XAPO_VID_RSTn() const {
    /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
    /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
    /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, ppu_config.XONA_LCDC_EN);
    /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
    return XAPO_VID_RSTn;
  }

  //-----------------------------------------------------------------------------

  int get_addr() const {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03, CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
      CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11, CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  void preset_addr(bool oe, uint16_t addr) {
    CPU_PIN_A00.preset(oe, addr & 0x0001);
    CPU_PIN_A01.preset(oe, addr & 0x0002);
    CPU_PIN_A02.preset(oe, addr & 0x0004);
    CPU_PIN_A03.preset(oe, addr & 0x0008);
    CPU_PIN_A04.preset(oe, addr & 0x0010);
    CPU_PIN_A05.preset(oe, addr & 0x0020);
    CPU_PIN_A06.preset(oe, addr & 0x0040);
    CPU_PIN_A07.preset(oe, addr & 0x0080);
    CPU_PIN_A08.preset(oe, addr & 0x0100);
    CPU_PIN_A09.preset(oe, addr & 0x0200);
    CPU_PIN_A10.preset(oe, addr & 0x0400);
    CPU_PIN_A11.preset(oe, addr & 0x0800);
    CPU_PIN_A12.preset(oe, addr & 0x1000);
    CPU_PIN_A13.preset(oe, addr & 0x2000);
    CPU_PIN_A14.preset(oe, addr & 0x4000);
    CPU_PIN_A15.preset(oe, addr & 0x8000);
  }

  //-----------------------------------------------------------------------------

  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  /* PIN_74 */ PinIn PIN_CLK_IN_xBxDxFxH;
  /* PIN_76 */ PinIn PIN_T2;
  /* PIN_77 */ PinIn PIN_T1;

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

  VramBus vram_bus;

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