#include "Sch_Merged.h"
#include "Sch_Pins.h"

#include "Sch_CPU.h"
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

  //-----------------------------------------------------------------------------


  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  CpuRegisters cpu_reg;
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