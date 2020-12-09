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
    int div = div_reg.get_div();
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

  /* p07.UBET*/ wire UBETp_ext()           const { return not1(wire(sys_t1)); }
  /* p07.UVAR*/ wire UVARp_ext()           const { return not1(wire(sys_t2)); }
  /* p07.UMUT*/ wire UMUT_MODE_DBG1p_ext() const { return and2(wire(sys_t1), UVARp_ext()); }
  /* p07.UNOR*/ wire UNOR_MODE_DBG2p_ext() const { return and2(wire(sys_t2), UBETp_ext()); }
  /* p07.UPOJ*/ wire UPOJ_MODE_PRODn_ext() const { return nand3(UBETp_ext(), UVARp_ext(), wire(sys_rst)); }
  /* p08.RYCA*/ wire RYCA_MODE_DBG2n_ext() const { return not1(UNOR_MODE_DBG2p_ext()); }
  /* p08.TOVA*/ wire TOVA_MODE_DBG2n_ext() const { return not1(UNOR_MODE_DBG2p_ext()); }
  /* p08.MULE*/ wire MULE_MODE_DBG1n_ext() const { return not1(UMUT_MODE_DBG1p_ext()); }

  //-----------------------------------------------------------------------------

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow(int pass_index);

  void tock_ext();


  void tock_vram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],

    wire AVOR_SYS_RSTp_new,
    wire XODO_VID_RSTp_new_h,

    wire ATAL_xBxDxFxH_clk_odd,
    wire ABUZ_xxCDEFGH_clk_evn,

    wire CATY_LATCH_EXTp_ext,
    wire SOSE_ADDR_VRAMp_ext,
    wire WERO_ADDR_PPUp_ext,
    wire XOLA_A00n_ext, wire WADO_A00p_ext,
    wire XENO_A01n_ext, wire WESA_A01p_ext,
    wire XERA_A03n_ext, wire WEPO_A03p_ext,
    wire XUSY_A02n_ext, wire WALO_A02p_ext,

    wire TEDO_CPU_RDp_ext,
    wire TAPU_CPU_WRp_clkevn,
    wire APOV_CPU_WRp_clkevn,

    wire TUTO_DBG_VRAMp_new,

    wire LUFA_DMA_VRAMp_new_xxx,
    wire ATEJ_LINE_TRIGp_new_evn,
    wire TEVO_FETCH_TRIGp_new,
    wire NYXU_BFETCH_RSTn_new_xxx,
    wire PARU_VBLANKp_new_evn,
    wire PORE_WIN_MODEp_new_evn,
    wire WYMO_LCDC_WINENn_new,
    wire XYMU_RENDERINGp_new_xxx,

    wire XEHO_PX0p_new_evn,
    wire SAVY_PX1p_new_evn,
    wire XODU_PX2p_new_evn,
    wire XYDO_PX3p_new_evn,
    wire TUHU_PX4p_new_evn,
    wire TUKY_PX5p_new_evn,
    wire TAKO_PX6p_new_evn,
    wire SYBE_PX7p_new_evn
  );

  void tock_oam(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp_new,
    wire ATAL_xBxDxFxH_clk_odd,
    wire UVYT_ABCDxxxx_clkevn,
    wire XOCE_xBCxxFGx_clkodd,
    wire XYSO_xBCDxFGH_clkodd,
    wire TAPU_CPU_WRp_clkevn,
    wire TEDO_CPU_RDp_ext,
    wire SARO_ADDR_OAMp_ext,
    wire CATY_LATCH_EXTp_ext,
    wire MATU_DMA_RUNNINGp_new_evn,
    wire XYMU_RENDERINGp_new_xxx,
    wire ACYL_SCANNINGp_new_evn
  );

  void tock_zram(uint16_t cpu_addr, wire BUS_CPU_D[8], wire TEDO_CPU_RDp_ext, wire TAPU_CPU_WRp_clkevn);


  //-----------------------------------------------------------------------------

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }


  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------
  // CPU interface

  BusOut BUS_CPU_D_out[8];

  PinOut PIN_CPU_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  PinOut PIN_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  PinIn  PIN_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  PinOut PIN_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  PinIn  PIN_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  PinOut PIN_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  PinIn  PIN_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  PinOut PIN_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  PinIn  PIN_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joypad int ack
  PinOut PIN_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joypad int

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinOut PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  PinOut PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  PinOut PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  PinOut PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  PinOut PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  PinOut PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  PinOut PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  PinOut PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3

  PinOut PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  PinOut PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state

  //----------

  OamBus oam_bus;
  BusOut BUS_OAM_An[8];  // ABCDEFGH
  BusIO  BUS_OAM_DAn[8];
  BusIO  BUS_OAM_DBn[8];
  PinOut PIN_OAM_CLKn;   // ABCDEFGH
  PinOut PIN_OAM_WRn_A;  // AxxxExxH
  PinOut PIN_OAM_WRn_B;  // AxxxExxH
  PinOut PIN_OAM_OEn;    // ABCDEFGH

  //----------

  ExtBus ext_bus;
  PinOut PIN_EXT_CLK;    // PIN_75
  PinOut PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  PinOut PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  PinOut PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  PinOut PIN_EXT_A[16];
  PinIO  PIN_EXT_D[8];

  //----------

  VramBus vram_bus;
  BusOut BUS_VRAM_An[13];
  BusIO  BUS_VRAM_Dp[8];
  PinOut PIN_VRAM_CSn; // PIN_43
  PinOut PIN_VRAM_OEn; // PIN_45
  PinOut PIN_VRAM_WRn; // PIN_49
  PinOut PIN_VRAM_Ap[13];
  PinIO  PIN_VRAM_Dp[8];

  //----------

  ClockRegisters clk_reg;
  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  //----------

  DivRegister    div_reg;
  TimerRegisters tim_reg;

  //----------

  DmaRegisters dma_reg;

  //----------

  InterruptRegisters int_reg;
  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  //----------

  JoypadRegisters joypad;
  PinOut PIN_JOY_P14; // PIN_63
  PinOut PIN_JOY_P15; // PIN_62
  PinIn  PIN_JOY_P10; // PIN_67   Pressing a button pulls the corresponding pin _down_.
  PinIn  PIN_JOY_P11; // PIN_66
  PinIn  PIN_JOY_P12; // PIN_65
  PinIn  PIN_JOY_P13; // PIN_64

  //----------

  SerialRegisters ser_reg;
  PinIO  PIN_SCK;  // PIN_68
  PinIn  PIN_SIN;  // PIN_69
  PinOut PIN_SOUT; // PIN_70

  //----------

  SpriteStore        sprite_store;
  BusIO SPR_TRI_I_evn[6]; // AxCxExGx
  BusIO SPR_TRI_L_evn[4]; // AxCxExGx

  //----------

  SpriteScanner      sprite_scanner;

  //----------

  TileFetcher        tile_fetcher;

  //----------

  SpriteFetcher      sprite_fetcher;

  //----------

  PixelPipe          pix_pipe;

  //----------

  LcdRegisters lcd_reg;

  /*PIN_50*/ PinOut PIN_LCD_DATA1;
  /*PIN_51*/ PinOut PIN_LCD_DATA0_evn;
  /*PIN_54*/ PinOut PIN_LCD_HSYNC_evn;
  /*PIN_56*/ PinOut PIN_LCD_FLIPS_evn;
  /*PIN_52*/ PinOut PIN_LCD_CNTRL_evn;
  /*PIN_55*/ PinOut PIN_LCD_LATCH_evn;
  /*PIN_53*/ PinOut PIN_LCD_CLOCK_xxx;
  /*PIN_57*/ PinOut PIN_LCD_VSYNC_evn;

  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];

  //----------

  uint64_t sentinel2 = SENTINEL2;

  bool oam_clk_old;
  bool zram_clk_old;

  //bool XYMU_RENDERINGp_old_xxx;
  //bool MATU_DMA_RUNNINGp_old_evn;
  //bool BAXO_OAM_DB5p_old_evn;
  //bool BESU_SCANNINGp_old_evn;
  //bool WYMO_LCDC_WINENp_old_h;

  bool WODU_HBLANKp_old;
  bool NYXU_BFETCH_RSTn_old_xxx;
  bool TAVE_PRELOAD_DONE_TRIGp_old;
  bool FEPO_STORE_MATCHp_old_evn;
  bool MOCE_BFETCH_DONEn_old_xxx;

  bool GUVA_SPRITE0_GETp_old_evn;
  bool ENUT_SPRITE1_GETp_old_evn;
  bool EMOL_SPRITE2_GETp_old_evn;
  bool GYFY_SPRITE3_GETp_old_evn;
  bool GONO_SPRITE4_GETp_old_evn;
  bool GEGA_SPRITE5_GETp_old_evn;
  bool XOJA_SPRITE6_GETp_old_evn;
  bool GUTU_SPRITE7_GETp_old_evn;
  bool FOXA_SPRITE8_GETp_old_evn;
  bool GUZE_SPRITE9_GETp_old_evn;

  bool WODU_old;
  bool WODU_older;

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
  bool sys_in_reset_sequence = 0;

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
