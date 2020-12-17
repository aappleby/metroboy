#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOamBus {
  void reset_app() {
    BUS_OAM_An[0].reset(REG_D1C0);
    BUS_OAM_An[1].reset(REG_D1C0);
    BUS_OAM_An[2].reset(REG_D1C0);
    BUS_OAM_An[3].reset(REG_D1C0);
    BUS_OAM_An[4].reset(REG_D0C0);
    BUS_OAM_An[5].reset(REG_D1C0);
    BUS_OAM_An[6].reset(REG_D0C0);
    BUS_OAM_An[7].reset(REG_D1C0);

    oam_latch_a.reset_app();
    oam_latch_b.reset_app();
    oam_temp_a.reset_app();
    oam_temp_b.reset_app();
  }

  void latch_bus(GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, wire XOCE_xBCxxFGx, wire XUJA_SPR_OAM_LATCHn);
  void latch_to_cpu(GateBoyCpuBus& cpu_bus, wire MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp, wire XYMU_RENDERINGp);
  void latch_to_temp(GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, wire MATU_DMA_RUNNINGp, wire XUJY_OAM_CLKENp);
  void ext_to_data_bus (const GateBoyDMA& dma, PinIO PIN_EXT_D[8]);
  void vram_to_data_bus(const GateBoyDMA& dma, BusIO BUS_VRAM_Dp[8]);
  void dma_to_addr_bus(GateBoyDMA& dma);
  void sprite_index_to_addr_bus(GateBoyDMA& dma, BusIO SPR_TRI_I[6], wire XYMU_RENDERINGp);
  void scan_index_to_addr_bus(SpriteScanner& sprite_scanner, wire ACYL_SCANNINGp);
  void cpu_to_data_bus(GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire XYMU_RENDERINGp, wire MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp);
  void set_pin_clk(GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp, wire XUJY_OAM_CLKENp);
  void set_pin_wr (GateBoyResetDebug& rst, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire XYMU_RENDERINGp, wire MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp);
  void set_pin_oe (GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, wire XUJA_SPR_OAM_LATCHn);

  void tock(GateBoyCpuBus& cpu_bus, wire XOCE_xBCxxFGx, wire ACYL_SCANNINGp, wire XUJA_SPR_OAM_LATCHn, uint8_t* oam_ram);

  void dump(Dumper& d) {
    d.dump_bitp   ("MAKA_LATCH_EXTp ", MAKA_LATCH_EXTp.state);
    d.dump_bitp   ("WUJE_CPU_OAM_WRn", WUJE_CPU_OAM_WRn.state);
    d.dump_slice2n("BUS_OAM_An      ", BUS_OAM_An, 8);
    d.dump_slice2n("BUS_OAM_DAn     ", BUS_OAM_DAn, 8);
    d.dump_slice2n("BUS_OAM_DBn     ", BUS_OAM_DBn, 8);
    d.dump_bitp   ("PIN_OAM_CLKn    ", PIN_OAM_CLKn.state);
    d.dump_bitp   ("PIN_OAM_WRn_A   ", PIN_OAM_WRn_A.state);
    d.dump_bitp   ("PIN_OAM_WRn_B   ", PIN_OAM_WRn_B.state);
    d.dump_bitp   ("PIN_OAM_OEn     ", PIN_OAM_OEn.state);
    d.dump_slice2n("OAM LATCH A     ", &oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
    d.dump_slice2n("OAM LATCH B     ", &oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
    d.dump_slice2p("OAM TEMP A      ", &oam_temp_a.XUSO_OAM_DA0p, 8);
    d.dump_slice2p("OAM TEMP B      ", &oam_temp_b.YLOR_OAM_DB0p, 8);
  }

  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  OamTempA oam_temp_a;
  OamTempB oam_temp_b;
  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  BusOut BUS_OAM_An[8];  // ABCDEFGH
  BusIO  BUS_OAM_DAn[8];
  BusIO  BUS_OAM_DBn[8];
  PinOut PIN_OAM_CLKn;   // ABCDEFGH
  PinOut PIN_OAM_WRn_A;  // AxxxExxH
  PinOut PIN_OAM_WRn_B;  // AxxxExxH
  PinOut PIN_OAM_OEn;    // ABCDEFGH
  Signal old_oam_clk;
};

//------------------------------------------------------------------------------------------------------------------------
