#include "Sch_Merged.h"
#include "Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_dma(int phase, bool ABOL_CLKREQn, bool UNOR_MODE_DBG2);

  void tick_busmux(int phase);

  void tick_lcd(int phase, bool LCDC_EN);

  wire TEDO_BUS_RD() const;
  wire ASOT_BUS_RD() const;

  void tick_pipe      (bool CLKPIPE_AxCxExGx, bool NYXU_BG_SEQ_RSTn, bool WUTY_SPRITE_DONE);
  void tick_vbus      (bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_ebus      (bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_oam       (bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2, bool ACYL_OAM_ADDR_PARSE);
  void tick_bootrom   (bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_interrupts(bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2, bool WODU_RENDER_DONE);

  void tick_vram_pins();

  void tick_vram_lock(bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_oam_lock(bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);

  void tick_lyc();

  wire ALUR_RSTn() const;

  wire CUNU_RSTn() const {
    /*p01.DULA*/ wire DULA_RST = not(ALUR_RSTn());
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RST);
    return CUNU_RSTn;
  }


  wire CUPA_BUS_WR_xxxxxFGH() const {
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_reg.PHAZ_xxxxEFGH);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH);
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and (sys_pins.T2, UBET_T1n);
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH, UNOR_MODE_DBG2);
    /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_BUS_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_BUS_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);

    return CUPA_BUS_WR_xxxxxFGH;
  }

  wire FFXX() const {
    /*p07.TUNA*/ wire TUNA_0000_FDFF = nand(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09);
    /*p07.TONA*/ wire ADDR_08n = not(cpu_pins.A08);
    /*p07.SYKE*/ wire FFXX = nor(TUNA_0000_FDFF, ADDR_08n);
    return FFXX;
  }

  wire FF4X() const {
    /*p22.XALY*/ wire ADDR_0x00xxxx = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A04);
    /*p22.WUTU*/ wire FF4Xn = nand(FFXX(), cpu_pins.A06, ADDR_0x00xxxx);
    /*p22.WERO*/ wire FF4X = not(FF4Xn);
    return FF4X;
  }

  //-----------------------------------------------------------------------------


  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  ClkRegisters clk_reg; // dumped
  BusRegisters bus_reg;// dumped
  ConfigRegisters cfg_reg;// dumped
  DebugRegisters dbg_reg;// dumped
  DmaRegisters dma_reg;// dumped
  InterruptRegisters int_reg;// dumped
  JoypadRegisters joy_reg;// dumped

  LcdRegisters lcd_reg;// dumped
  PixelPipeRegisters pxp_reg;// dumped
  RstRegisters rst_reg;// dumped
  SerialRegisters ser_reg;// dumped
  SpriteRegisters spr_reg;// dumped
  SpriteStoreRegisters sst_reg;// dumped
  TimerRegisters tim_reg;// dumped
  VclkRegisters vclk_reg;// dumped
  VidRegisters vid_reg;// dumped
  OamRegisters oam_reg;// dumped

  SysPins sys_pins; // dumped
  VramPins vram_pins; // dumped
  SerialPins ser_pins; // dumped
  JoypadPins joy_pins; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  LcdPins lcd_pins; // dumped
  WavePins wave_pins;
};

//-----------------------------------------------------------------------------

};