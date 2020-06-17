#include "Sch_Merged.h"
#include "Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_dma(int phase, bool ABOL_CLKREQn);

  void tick_busmux(int phase);

  void tick_lcd(int phase, bool LCDC_EN);

  wire TEDO_BUS_RD() const;
  wire ASOT_BUS_RD() const;

  void tick_pipe      (bool CLKPIPE_AxCxExGx, bool NYXU_BG_SEQ_RSTn, bool WUTY_SPRITE_DONE);
  void tick_vbus      ();
  void tick_ebus      ();
  void tick_oam       (bool ACYL_OAM_ADDR_PARSE);
  void tick_bootrom   ();
  void tick_interrupts(bool WODU_RENDER_DONE);

  void tick_vram_pins();

  void tick_vram_lock();
  void tick_oam_lock ();

  void tick_lyc();
  //----------------------------------------

  wire UMUT_MODE_DBG1() const {
    /*p07.UVAR*/ wire UVAR_T2n = not(sys_pins.T2);
    /*p07.UMUT*/ wire UMUT_MODE_DBG1 = and (sys_pins.T1, UVAR_T2n);
    return UMUT_MODE_DBG1;
  }

  wire UNOR_MODE_DBG2() const {
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and (sys_pins.T2, UBET_T1n);
    return UNOR_MODE_DBG2;
  }

  wire UPOJ_MODE_PROD() const {
    /*p07.UVAR*/ wire UVAR_T2n = not(sys_pins.T2);
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, sys_pins.RST);
    return UPOJ_MODE_PROD;
  }

  //----------------------------------------

  wire BUKE_ABxxxxxH() const {
    /*p01.ABOL*/ wire ABOL_CLKREQn = not(cpu_pins.CLKREQ);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.PHAZ_xxCDEFxx);
    /*p01.BUGO*/ wire BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(!clk_reg.PHAZ_xxxDEFGx);
    /*p01.BATE*/ wire BATE_ABxxxxxH = nor(ABOL_CLKREQn, BUGO_xxCDEFxx, AROV_xxxDEFGx);
    /*p01.BASU*/ wire BASU_xxCDEFGx = not(BATE_ABxxxxxH);
    /*p01.BUKE*/ wire BUKE_ABxxxxxH = not(BASU_xxCDEFGx);
    return BUKE_ABxxxxxH;
  }

  wire ANOS_AxCxExGx() const {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(sys_pins.CLK_IN_xBxDxFxH);
    return ANOS_AxCxExGx;
  }

  wire ATAL_xBxDxFxH() const {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx());
    return ATAL_xBxDxFxH;
  }

  wire ALET_xBxDxFxH() const {
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH());
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
    return ALET_xBxDxFxH;
  }

  wire BUDE_AxxxxFGH() const {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.ABOL*/ wire ABOL_CLKREQn = not(cpu_pins.CLKREQ);
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    return BUDE_AxxxxFGH;
  }

  wire BOLO_xBCDEFGx() const {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.ABOL*/ wire ABOL_CLKREQn = not(cpu_pins.CLKREQ);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(!clk_reg.PHAZ_xxxDEFGx);
    /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn, AROV_xxxDEFGx, ATYP_xBCDExxx);
    /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
    /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
    /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    return BOLO_xBCDEFGx;
  }

  wire BALY_xBxxxxxx() const {
    /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH());
    /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx(), BEKO_xBCDExxx);
    /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
    /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
    /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.PHAZ_xxCDEFxx);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
    /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
    /*p01.ATEZ*/ wire ATEZ_CLKBAD = not(sys_pins.CLK_GOOD);
    /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
    return BALY_xBxxxxxx;
  }

  wire BOGA_AxCDEFGH() const {
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx());
    return BOGA_AxCDEFGH;
  }

  //----------------------------------------

  wire ALUR_RSTn() const;

  wire CUNU_RSTn() const {
    /*p01.DULA*/ wire DULA_RST = not(ALUR_RSTn());
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RST);
    return CUNU_RSTn;
  }


  //----------------------------------------

  wire XAPO_VID_RSTn() const {
    /*p01.XORE*/ wire XORE_RST = not(CUNU_RSTn());
    /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RST);
    /*p01.XODO*/ wire XODO_VID_RST = nand(XEBE_RSTn, cfg_reg.LCDC_EN);
    /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RST);
    return XAPO_VID_RSTn;
  }

  wire ATAR_VID_RST() const {
    /*p01.ATAR*/ wire ATAR_VID_RST = not(XAPO_VID_RSTn());
    return ATAR_VID_RST;
  }

  wire ABEZ_VID_RSTn() const {
    /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RST());
    return ABEZ_VID_RSTn;
  }

  //----------------------------------------

  wire BYHA_VID_LINE_TRIG_d4n() const {
    /*p28.ABAF*/ wire VID_LINE_d4n = not(lcd_reg.VID_LINE_d4);
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.VID_LINE_d6, VID_LINE_d4n), ABEZ_VID_RSTn());
    return BYHA_VID_LINE_TRIG_d4n;
  }
    
  wire ATEJ_VID_LINE_TRIG_d4() const {
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4 = not(BYHA_VID_LINE_TRIG_d4n());
    return ATEJ_VID_LINE_TRIG_d4;
  }

  //----------------------------------------

  wire AREV_CPU_WRn_ABCDExxx() const {
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_reg.PHAZ_xxxxEFGH);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH);
    return AREV_CPU_WRn_ABCDExxx;
  }

  wire APOV_CPU_WR_xxxxxFGH() const {
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx());
    return APOV_CPU_WR_xxxxxFGH;
  }

  wire CUPA_BUS_WR_xxxxxFGH() const {
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH(), UNOR_MODE_DBG2());
    /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_BUS_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_BUS_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);

    return CUPA_BUS_WR_xxxxxFGH;
  }

  wire TAPU_BUS_WR_xxxxxFGH() const {
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH(), UNOR_MODE_DBG2());
    /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    return TAPU_BUS_WR_xxxxxFGH;
  }
  //----------------------------------------

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