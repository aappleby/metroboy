#include "Sch_Merged.h"
#include "Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_pipe      ();
  void tick_vbus      ();
  void tick_ebus      ();
  void tick_oam       ();
  void tick_bootrom   ();
  void tick_interrupts();
  void tick_serial(CpuPins& cpu_pins);

  void tick_sprite_store();

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

  wire AFAS_xxxxxFGH() const {
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_reg.PHAZ_xxxxEFGH);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_reg.PHAZ_xBCDExxx);
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    return AFAS_xxxxxFGH;
  }

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

  wire MYVO_xBxDxFxH() const {
    /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(ALET_xBxDxFxH());
    return MYVO_xBxDxFxH;
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

  wire AVAP_SCAN_DONE_d0_TRIG() const {
    /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4(), ATAR_VID_RST());
    /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
    /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, spr_reg.SCAN_DONE_d5, !spr_reg.SCAN_DONE_d4);
    /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);
    return AVAP_SCAN_DONE_d0_TRIG;
  }

  wire XOFO_WIN_RST() const {
    /*p28.ABAF*/ wire VID_LINE_d4n = not(lcd_reg.VID_LINE_d4);
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.VID_LINE_d6, VID_LINE_d4n), ABEZ_VID_RSTn());
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4 = not(BYHA_VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4);
    /*p27.XOFO*/ wire XOFO_WIN_RST = nand(cfg_reg.LCDC_WINEN, XAHY_VID_LINE_TRIG_d4n, XAPO_VID_RSTn());
    return XOFO_WIN_RST;
  }

  wire TEXY_SPRITE_READ() const {
    /*p29.SAKY*/ wire SAKY = nor(spr_reg.TULY_SPR_SEQ1, spr_reg.VONU_SEQ_xxx34xn);
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(vid_reg.RENDERING_LATCH);
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or (SAKY, TEPA_RENDERINGn); // seems wrong
    /*p29.TEXY*/ wire TEXY_SPRITE_READ = not(TYSO_SPRITE_READn);
    return TEXY_SPRITE_READ;
  }

  //----------------------------------------

  wire TEDO_BUS_RD() const {
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and (sys_pins.T2, UBET_T1n);
    /*p07.UJYV*/ wire UJYV_BUS_RD_MUX = mux2n(ext_pins.RD_C, cpu_pins.CPU_RAW_RD, UNOR_MODE_DBG2);
    /*p07.TEDO*/ wire TEDO_BUS_RD = not(UJYV_BUS_RD_MUX);
    return TEDO_BUS_RD;
  }

  wire ASOT_BUS_RD() const {
    /*p07.AJAS*/ wire AJAS_BUS_RDn = not(TEDO_BUS_RD());
    /*p07.ASOT*/ wire ASOT_BUS_RD = not(AJAS_BUS_RDn);
    return ASOT_BUS_RD;
  }

  //----------------------------------------

  wire AREV_CPU_WRn_ABCDExxx() const {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH());
    return AREV_CPU_WRn_ABCDExxx;
  }

  wire APOV_CPU_WR_xxxxxFGH() const {
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx());
    return APOV_CPU_WR_xxxxxFGH;
  }

  wire TAPU_BUS_WR_xxxxxFGH() const {
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH(), UNOR_MODE_DBG2());
    /*p07.TAPU*/ wire TAPU_BUS_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    return TAPU_BUS_WR_xxxxxFGH;
  }
  wire CUPA_BUS_WR_xxxxxFGH() const {
    /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_BUS_WR_xxxxxFGH());
    /*p07.CUPA*/ wire CUPA_BUS_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);
    return CUPA_BUS_WR_xxxxxFGH;
  }

  //----------------------------------------

  wire SYLO_WIN_MODEn() const {
    /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(vid_reg.RYDY_WIN_MODE_LATCH);
    return SYLO_WIN_MODEn;
  }

  wire TOMU_WIN_MODE() const {
    /*p24.TOMU*/ wire TOMU_WIN_MODE = not(SYLO_WIN_MODEn());
    return TOMU_WIN_MODE;
  }

  wire DMA_ADDR_VRAM() const {
    /*p04.LEBU*/ wire DMA_A15n = not(dma_reg.DMA_A15);
    /*p04.MUDA*/ wire DMA_ADDR_VRAM = nor(dma_reg.DMA_A13, dma_reg.DMA_A14, DMA_A15n);
    return DMA_ADDR_VRAM;
  }

  wire TEXO_ADDR_VALID_AND_NOT_VRAM() const {
    /*p08.SORE*/ wire SORE_ADDR_0000_7FFF = not(cpu_pins.A15);
    /*p08.TEVY*/ wire TEVY_ADDR_NOT_VRAM = or (cpu_pins.A13, cpu_pins.A14, SORE_ADDR_0000_7FFF);
    /*p08.TEXO*/ wire TEXO_ADDR_VALID_AND_NOT_VRAM = and (cpu_pins.ADDR_VALIDn, TEVY_ADDR_NOT_VRAM);
    return TEXO_ADDR_VALID_AND_NOT_VRAM;
  }

  wire TUNA_0000_FDFF() const {
    /*p07.TUNA*/ wire TUNA_0000_FDFF = nand(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09);
    return TUNA_0000_FDFF;
  }

  wire SYRO_FE00_FFFF() const {
    /*p25.SYRO*/ wire SYRO_FE00_FFFF = not(TUNA_0000_FDFF());
    return SYRO_FE00_FFFF;
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

  wire CPU_ADDR_FF46() const {
    /*p22.XUSY*/ wire A02n = not(cpu_pins.A02);
    /*p22.WALO*/ wire WALO_A02 = not(A02n);
    /*p22.XENO*/ wire A01n = not(cpu_pins.A01);
    /*p22.WESA*/ wire WESA_A01 = not(A01n);
    /*p22.XERA*/ wire A03n = not(cpu_pins.A03);
    /*p22.XOLA*/ wire A00n = not(cpu_pins.A00);
    /*p22.WATE*/ wire FF46n = nand(FF4X(), A00n, WESA_A01, WALO_A02, A03n);
    /*p22.XEDA*/ wire CPU_ADDR_FF46 = not(FF46n);
    return CPU_ADDR_FF46;
  }

  wire PPU_USE_OAM1() const {
    /*p28.BOGE*/ wire DMA_RUNNINGn = not(dma_reg.DMA_RUNNING);
    /*p28.ASEN*/ wire ASEN = or (ATAR_VID_RST(), AVAP_SCAN_DONE_d0_TRIG());
    /*p28.BESU*/ wire BESU = or (lcd_reg.VID_LINE_d4, ASEN);
    /*p28.ACYL*/ wire PPU_USE_OAM1 = and (DMA_RUNNINGn, BESU);
    return PPU_USE_OAM1;
  }

  wire WUTY_SPRITE_DONE() const {
    /*p29.TYNO*/ wire TYNO = nand(spr_reg.TOXE_SPR_SEQ0, spr_reg.SEBA_SEQ_xxxx45n, spr_reg.VONU_SEQ_xxx34xn);
    /*p29.VUSA*/ wire VUSA = or (!spr_reg.TYFO_SEQ_B0d, TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);
    return WUTY_SPRITE_DONE;
  }

  wire WODU_RENDER_DONE() const {
    /*p21.XUGU*/ wire XUGU_X_167n = nand(vid_reg.X0, vid_reg.X1, vid_reg.X2, vid_reg.X5, vid_reg.X7); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ wire XANO_X_167 = not(XUGU_X_167n);
    /*p21.XENA*/ wire XENA_STORE_MATCHn = not(FEPO_STORE_MATCH);
    /*p21.WODU*/ wire WODU_RENDER_DONE = and (XENA_STORE_MATCHn, XANO_X_167);
    return WODU_RENDER_DONE;
  }


#if 0
  wire SEGU_xBxDxFxH(wire FEPO_STORE_MATCH, wire WODU_RENDER_DONE) const {
    /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(ALET_xBxDxFxH());
    /*p24.VYBO*/ wire VYBO_AxCxExGx = nor(FEPO_STORE_MATCH, WODU_RENDER_DONE, MYVO_xBxDxFxH);
    /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(vid_reg.RYDY_WIN_MODE_LATCH);
    /*p24.TOMU*/ wire TOMU_WIN_MODE = not(SYLO_WIN_MODEn);
    /*p24.SOCY*/ wire SOCY_WIN_MODEn = not(TOMU_WIN_MODE);
    /*p24.TYFA*/ wire TYFA_AxCxExGx = and (SOCY_WIN_MODEn, vid_reg.POKY_FRONT_PORCH_LATCHn, VYBO_AxCxExGx);
    /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
    return SEGU_xBxDxFxH;

  }

  wire ROXO_AxCxExGx() const {
    /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);
    return ROXO_AxCxExGx;
  }
#endif

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
  JoypadPins joy_pins; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  LcdPins lcd_pins; // dumped
  WavePins wave_pins;

  /*p29.FEPO*/ Signal FEPO_STORE_MATCH;
  /*p29.GUVA*/ Signal SPRITE0_GET;
  /*p29.ENUT*/ Signal SPRITE1_GET;
  /*p29.EMOL*/ Signal SPRITE2_GET;
  /*p29.GYFY*/ Signal SPRITE3_GET;
  /*p29.GONO*/ Signal SPRITE4_GET;
  /*p29.GEGA*/ Signal SPRITE5_GET;
  /*p29.XOJA*/ Signal SPRITE6_GET;
  /*p29.GUTU*/ Signal SPRITE7_GET;
  /*p29.FOXA*/ Signal SPRITE8_GET;
  /*p29.GUZE*/ Signal SPRITE9_GET;

  Signal NYXU_BG_SEQ_RSTn;

  Signal YDIFF0, YDIFF1, YDIFF2, YDIFF3, YDIFF4, YDIFF5, YDIFF6, YDIFF7, YDIFF_C7;
  Signal STORE_EN;
  Signal MATCH_EN;
};

//-----------------------------------------------------------------------------

};