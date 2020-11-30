#pragma once

#include "CoreLib/File.h"
#include "CoreLib/CPU.h"
#include "CoreLib/Constants.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/Sch_Timer.h"
#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_DMA.h"
#include "GateBoyLib/Sch_SpriteStore.h"
#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_PixPipe.h"
#include "GateBoyLib/Sch_Joypad.h"
#include "GateBoyLib/Sch_Serial.h"
#include "GateBoyLib/Sch_Interrupts.h"
#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_ExtBus.h"
#include "GateBoyLib/Sch_OamBus.h"
#include "GateBoyLib/Sch_VramBus.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                  uint8_t* _cart_buf, size_t _cart_size,
                  bool fastboot = true);
  void reset_cart(uint8_t* _boot_buf, size_t _boot_size,
                  uint8_t* _cart_buf, size_t _cart_size);



  void load_post_bootrom_state();
  void run_reset_sequence(bool fastboot = true);

  //----------------------------------------

  void load_dump(const char* filename) {
    load_obj(filename, *this);
    check_sentinel();
  }

  void save_dump(const char* filename) {
    save_obj(filename, *this);
  }

  //----------------------------------------

  void check_div() const {
    int div = tim_reg.get_div();
    if (div != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  void check_sentinel() const {
    if (sentinel1 != SENTINEL1) {
      printf("sentinel1 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel2 != SENTINEL2) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel3 != SENTINEL3) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow();

  //-----------------------------------------------------------------------------

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }


  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  OamBus oam_bus;
  ExtBus ext_bus;
  VramBus vram_bus;


  ClockRegisters     clk_reg;
  DmaRegisters       dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters    joypad;
  LcdRegisters       lcd_reg;
  PixelPipe          pix_pipe;
  SerialRegisters    ser_reg;
  SpriteStore        sprite_store;
  TimerRegisters     tim_reg;
  TileFetcher        tile_fetcher;
  SpriteFetcher      sprite_fetcher;
  SpriteScanner      sprite_scanner;

  /*p07.TEPU*/ DFF17 BOOT_BITn;
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  Pin2 PIN_CPU_WAKE; // top right wire by itself <- P02.AWOB

  Pin2 PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Pin2 PIN_CPU_WRp;           // top right port PORTA_01: ->
  Pin2 PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Pin2 PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  Pin2 PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Pin2 PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  Pin2 PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  Pin2 PIN_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  Pin2 PIN_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  Pin2 PIN_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  Pin2 PIN_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  Pin2 PIN_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  Pin2 PIN_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  Pin2 PIN_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  Pin2 PIN_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  Pin2 PIN_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joypad int ack
  Pin2 PIN_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joypad int

  Pin2 PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  Pin2 PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: // this is the "put address on bus" clock
  Pin2 PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02:
  Pin2 PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: // this is the "reset for next cycle" clock
  Pin2 PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: // this is the "put write data on bus" clock
  Pin2 PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05:
  Pin2 PIN_CPU_LATCH_EXT_s;   // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  Pin2 PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: // this is probably the "latch bus data" clock
  Pin2 PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: (RESET_CLK) // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  Pin2 PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: - test pad 3

  Pin2 PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  Pin2 PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  Pin2 PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  Pin2 PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state

  Pin2 PIN_EXT_CLK;    // PIN_75
  Pin2 PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  Pin2 PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  Pin2 PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  Pin2 PIN_EXT_A[16];
  Pin2 PIN_EXT_D_in[8];
  Pin2 PIN_EXT_D_out[8];

  Pin2 PIN_JOY_P14; // PIN_63
  Pin2 PIN_JOY_P15; // PIN_62
  Pin2 PIN_JOY_P10; // PIN_67   Pressing a button pulls the corresponding pin _down_.
  Pin2 PIN_JOY_P11; // PIN_66
  Pin2 PIN_JOY_P12; // PIN_65
  Pin2 PIN_JOY_P13; // PIN_64

  Pin2 PIN_SCK; // PIN_68
  Pin2 PIN_SIN; // PIN_69
  Pin2 PIN_SOUT; // PIN_70

  Pin2 PIN_OAM_CLK;
  Pin2 PIN_OAM_WR_A;
  Pin2 PIN_OAM_WR_B;
  Pin2 PIN_OAM_OEn;

  Pin2 PIN_VRAM_CSn; // PIN_43
  Pin2 PIN_VRAM_OEn; // PIN_45
  Pin2 PIN_VRAM_WRn; // PIN_49

  // not yet stable
  //Bus2 BUS_VRAM_An[13];
  //Pin2 PIN_VRAM_Ap[13];
  //Bus2 BUS_VRAM_Dp_in[8];
  //Pin2 PIN_VRAM_Dp_in[8];
  //Bus2 BUS_VRAM_Dp_out[8];
  //Pin2 PIN_VRAM_Dp_out[8];

  /*PIN_50*/ Pin2 PIN_LCD_DATA1;
  /*PIN_51*/ Pin2 PIN_LCD_DATA0;
  /*PIN_54*/ Pin2 PIN_LCD_HSYNC;
  /*PIN_56*/ Pin2 PIN_LCD_FLIPS;
  /*PIN_52*/ Pin2 PIN_LCD_CNTRL;
  /*PIN_55*/ Pin2 PIN_LCD_LATCH;
  /*PIN_53*/ Pin2 PIN_LCD_CLOCK;
  /*PIN_57*/ Pin2 PIN_LCD_VSYNC;



  // ok this is technically part of the lcd, but registers need to go in this section...
  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];

  uint64_t sentinel2 = SENTINEL2;

  //-----------------------------------------------------------------------------
  // Control stuff

  bool sys_rst = 1;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_cpuready = 0;
  bool sys_cpu_en = 0;
  bool sys_fastboot = 0;
  bool sys_statediff = 1;
  bool sys_cpu_start = 0;

  uint8_t sys_buttons = 0;

  double   sim_time = 0;
  int32_t  phase_total = 0;
  int32_t  pass_total = 0;
  uint64_t pass_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;
  Probes   probes;

  //-----------------------------------------------------------------------------
  // CPU

  CPU      cpu;
  Req      cpu_req = {0};
  Req      dbg_req = {0};
  Req      bus_req = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;

  bool int_vblank = 0;
  bool int_vblank_halt = 0;

  bool int_stat = 0;
  bool int_stat_halt = 0;

  bool int_timer = 0;
  bool int_timer_halt = 0;

  bool int_serial = 0;
  bool int_serial_halt = 0;

  bool int_joypad = 0;
  bool int_joypad_halt = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t* boot_buf = nullptr;
  size_t   boot_size = 0;

  uint8_t* cart_buf = nullptr;
  size_t   cart_size = 0;

  uint16_t ext_addr_latch = 0;
  uint16_t vram_addr_latch = 0;
  uint8_t oam_addr_latch = 0;

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t ext_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  uint8_t framebuffer[160*144];
  uint8_t gb_screen_x = 0;
  uint8_t gb_screen_y = 0;
  bool old_lcd_clock = 0;
  bool old_lcd_latch = 0;
  uint8_t lcd_data_latch = 0;

  //-----------------------------------------------------------------------------

  uint64_t sentinel3 = SENTINEL3;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
