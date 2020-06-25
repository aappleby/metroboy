#include "Sch_Merged.h"
#include "Sch_Pins.h"

#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_OAM.h"
#include "Sch_PPU.h"
#include "Sch_Window.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_Address.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_cpu();

  void tick_timer();
  void tick_joypad();
  void tick_lcd();
  void tick_window();

  void tick_vram_addr();

  void tick_bootrom();
  void tick_cart_data();
  void tick_cart_pins();

  void tick_oam();

  void tick_ppu();

  void tick_pixpipe();

  //----------------------------------------
  // LCD & PPU events

  wire BYHA_VID_LINE_TRIG_d4n() const {
    auto rst_sig = ResetSignals::get(*this);
    /*p28.ABAF*/ wire VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or (lcd_reg.ANEL_VID_LINE_d6.q(), VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
    return BYHA_VID_LINE_TRIG_d4n;
  }
    
  wire ATEJ_VID_LINE_TRIG_d4p() const {
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n());
    return ATEJ_VID_LINE_TRIG_d4p;
  }

  wire ACYL_PPU_USE_OAM1p() const {
    /*p28.BOGE*/ wire DMA_RUNNINGn = not(dma_reg.MATU_DMA_OAM_WRp.q());
    /*p28.ACYL*/ wire ACYL_PPU_USE_OAM1p = and(DMA_RUNNINGn, sst_reg.BESU_SCANNINGp);
    return ACYL_PPU_USE_OAM1p;
  }

  wire AVAP_SCAN_DONE_d0_TRIGp() const {
    auto rst_sig = ResetSignals::get(*this);
    /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p(), rst_sig.ATAR_VID_RSTp);
    /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
    /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, sst_reg.SCAN_DONE_d5.q(), !sst_reg.SCAN_DONE_d4.q());
    /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp = not(SCAN_DONE_d0_TRIGn);
    return AVAP_SCAN_DONE_d0_TRIGp;
  }

  // Polarity?
  wire TEXY_SPRITE_READ() const {
    // Die trace:
    // SAKY = nor(TULY17, VONU17)
    // TEPA = not(XYMU)
    // TYSO = or(SAKY, TEPA)
    // TEXY = not(TYSO)

    /*p29.SAKY*/ wire SAKY = nor(ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp);
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or (SAKY, TEPA_RENDERINGn);
    /*p29.TEXY*/ wire TEXY_SPRITE_READ = not(TYSO_SPRITE_READn);

    // So we only read a sprite if both those regs are... low? what is rung 17's polarity?
#if 0
    if (RENDERING_LATCH) {
      /*p29.TEXY*/ wire TEXY_SPRITE_READ = or(ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.VONU_SFETCH_S1_D4.q());;
    }
    else {
      /*p29.TEXY*/ wire TEXY_SPRITE_READ = 1;
    }
#endif

    return TEXY_SPRITE_READ;
  }

  wire WUTY_SPRITE_DONE() const {
    /*p29.TYNO*/ wire TYNO = nand(ppu_reg.TOXE_SFETCH_S0_D0.q(), ppu_reg.SEBA_SFETCH_S1_D5.q(), ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA = or (!ppu_reg.TYFO_SFETCH_S0_D1.q(), TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);
    return WUTY_SPRITE_DONE;
  }

  wire WODU_RENDER_DONE() const {
    /*p21.XUGU*/ wire XUGU_X_167n = nand(ppu_reg.SAXO_X0.q(), ppu_reg.TYPO_X1.q(), ppu_reg.VYZO_X2.q(), ppu_reg.TAHA_X5.q(), ppu_reg.SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ wire XANO_X_167 = not(XUGU_X_167n);
    /*p21.XENA*/ wire XENA_STORE_MATCHn = not(sst_reg.FEPO_STORE_MATCHp);
    /*p21.WODU*/ wire WODU_RENDER_DONE = and(XENA_STORE_MATCHn, XANO_X_167);
    return WODU_RENDER_DONE;
  }

  //----------------------------------------
  // CPU read/write signals

  // Polarity of this seems inconsistent...
  wire TEDO_CPU_RD() const {
#if 0
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, not(sys_pins.T1));
    if (UNOR_MODE_DBG2) {
      TEDO = not(ext_pins.RD_C);
    }
    else {
      TEDO = not(cpu_pins.CPU_RAW_RD);
    }

#endif

    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, UBET_T1n);
    /*p07.UJYV*/ wire UJYV_BUS_RD_MUX = mux2_n(ext_pins.RD_C, cpu_pins.CPU_RAW_RD, UNOR_MODE_DBG2);
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_BUS_RD_MUX);
    return TEDO_CPU_RD;
  }

  wire ASOT_CPU_RD() const {
    /*p07.AJAS*/ wire AJAS_BUS_RDn = not(TEDO_CPU_RD());
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RDn);
    return ASOT_CPU_RD;
  }

  wire AREV_CPU_WRn_ABCDExxx() const {
    auto clk_sig = clk_reg.sig(*this);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, clk_sig.AFAS_xxxxxFGH);
    return AREV_CPU_WRn_ABCDExxx;
  }

  wire APOV_CPU_WR_xxxxxFGH() const {
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx());
    return APOV_CPU_WR_xxxxxFGH;
  }

  wire TAPU_CPU_WR_xxxxxFGH() const {
    auto dbg_sig = dbg_reg.sig(*this);
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2_n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH(), dbg_sig.UNOR_MODE_DBG2n);
    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    return TAPU_CPU_WR_xxxxxFGH;
  }
  wire CUPA_CPU_WR_xxxxxFGH() const {
    /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH());
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);
    return CUPA_CPU_WR_xxxxxFGH;
  }

  //----------------------------------------
  // Window signals

  wire SYLO_WIN_HITn() const {
    /*p27.SYLO*/ wire SYLO_WIN_HITn = not(win_reg.RYDY_WIN_HIT_LATCH.q());
    return SYLO_WIN_HITn;
  }

  wire SOCY_WIN_HITn() const {
    /*p24.TOMU*/ wire TOMU_WIN_HIT = not(SYLO_WIN_HITn());
    /*p24.SOCY*/ wire SOCY_WIN_HITn = not(TOMU_WIN_HIT);
    return SOCY_WIN_HITn;
  }

  wire TUKU_WIN_HITn() const {
    /*p24.TOMU*/ wire TOMU_WIN_HIT = not(SYLO_WIN_HITn());
    /*p27.TUKU*/ wire TUKU_WIN_HITn = not(TOMU_WIN_HIT);
    return TUKU_WIN_HITn;
  }

  wire PORE_WIN_MODE() const {
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(win_reg.PYNU_WIN_MODE_LATCH.q());
    /*p27.PORE*/ wire PORE_WIN_MODE = not(NOCU_WIN_MODEn);
    return PORE_WIN_MODE;
  }

  wire XOFO_WIN_RST() const {
    auto rst_sig = ResetSignals::get(*this);
    /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or (lcd_reg.ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4p);
    /*p27.XOFO*/ wire XOFO_WIN_RST = nand(cfg_reg.LCDC_WINEN.q(), XAHY_VID_LINE_TRIG_d4n, rst_sig.XAPO_VID_RSTn);
    return XOFO_WIN_RST;
  }

  //----------------------------------------
  // DMA signals

  /*p04.MUDA*/ wire MUDA_DMA_ADDR_VRAMp() const { // def p
    // Die trace:
    // LEBU = not(MARU06)
    // MUDA = nor(PULA06, POKU06, LEBU);

#if 0
    // if rung 6 of MARU/PULA/POKU was QN:
    // MUDA = and(A13, A14, !A15);
    // would select last quarter of ROM, which doesn't make sense
    // so rung 6 of MARU must be Q.
#endif

    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n = not(dma_reg.DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_ADDR_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_ADDR_VRAMp;
  }

  //----------------------------------------
  // Address decoders

#if 0
  // This one is really weird.
  wire TEXO_ADDR_VALID_AND_NOT_VRAM() const {

    // TEVY box color wrong on die trace, but schematic correct.

    // Die trace:
    // SORE = not(A15)
    // TEVY = or(A13, A13, SORE) // A13 line not fully drawn
    // TEXO = and(ADDR_VALIDx?, TEVY)

    // The polarity of ADDR_VALIDx may be wrong

    /*p08.SORE*/ wire SORE = not(cpu_pins.A15);
    /*p08.TEVY*/ wire TEVY = or (cpu_pins.A13, cpu_pins.A14, SORE);
    /*p08.TEXO*/ wire TEXO = and(cpu_pins.ADDR_VALID, TEVY);
    return TEXO;
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

  ClockRegisters clk_reg; // dumped
  BusRegisters bus_reg;// dumped
  ConfigRegisters cfg_reg;// dumped
  DebugRegisters dbg_reg;// dumped
  DmaRegisters dma_reg;// dumped
  InterruptRegisters int_reg;// dumped
  JoypadRegisters joy_reg;// dumped

  LcdRegisters lcd_reg;// dumped
  PixelPipeRegisters pxp_reg;// dumped
  ResetRegisters rst_reg;// dumped
  SerialRegisters ser_reg;// dumped
  SpriteStoreRegisters sst_reg;// dumped
  TimerRegisters tim_reg;// dumped
  PpuRegisters ppu_reg;// dumped
  OamRegisters oam_reg;// dumped
  WindowRegisters win_reg;
  AddressRegisters adr_reg;

  SysPins sys_pins; // dumped
  VramPins vram_pins; // dumped
  JoypadPins joy_pin; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  WavePins wave_pins;

  Signal NYXU_BG_SEQ_RSTn;

  /*p27.TEVO*/ Signal TEVO_CLK_STOPn;

  /*p35.PATY*/ Signal PATY_PIX_OUT_LO;
  /*p35.PERO*/ Signal PERO_PIX_OUT_HI;
};

//-----------------------------------------------------------------------------

};