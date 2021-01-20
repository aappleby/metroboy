#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOamBus {
  void reset_to_cart() {
    oam_latch_a.reset_to_cart();
    oam_latch_b.reset_to_cart();
    oam_temp_a.reset_to_cart();
    oam_temp_b.reset_to_cart();
  }

  void latch_bus(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, wire XOCE_xBCxxFGx, wire XUJA_SPR_OAM_LATCHn);
  void latch_to_cpu(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, DFF17 MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp, NorLatch XYMU_RENDERINGn);
  void latch_to_temp_a(GateBoyBuses& new_bus, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, DFF17 MATU_DMA_RUNNINGp, wire XUJY_OAM_CLKENp);
  void latch_to_temp_b(GateBoyBuses& new_bus, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, DFF17 MATU_DMA_RUNNINGp, wire XUJY_OAM_CLKENp);
  void ext_to_data_bus (GateBoyBuses& new_bus, const GateBoyDMA& dma, const GateBoyExtBus& ext_bus);
  void vram_to_data_bus(GateBoyBuses& new_bus, const GateBoyDMA& dma, const GateBoyVramBus& vram_bus);
  void dma_to_addr_bus(GateBoyBuses& new_bus, GateBoyDMA& dma);
  void sprite_index_to_addr_bus(GateBoyBuses& new_bus, const GateBoyDMA& dma, const GateBoySpriteStore& sprite_store, NorLatch XYMU_RENDERINGn);
  void scan_index_to_addr_bus(GateBoyBuses& new_bus, SpriteScanner& sprite_scanner, wire ACYL_SCANNINGp);
  void cpu_to_addr_bus(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, NorLatch XYMU_RENDERINGn, DFF17 MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp);
  void cpu_to_data_bus(GateBoyBuses& new_bus, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, NorLatch XYMU_RENDERINGp, DFF17 MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp);
  void set_pin_clk(GateBoyBuses& new_bus, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, DFF17 MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp, wire XUJY_OAM_CLKENp);
  void set_pin_wr (GateBoyBuses& new_bus, GateBoyResetDebug& rst, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, NorLatch XYMU_RENDERINGn, DFF17 MATU_DMA_RUNNINGp, wire ACYL_SCANNINGp);
  void set_pin_oe (GateBoyBuses& new_bus, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, wire ACYL_SCANNINGp, wire XUJA_SPR_OAM_LATCHn);

  void tock(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, wire XOCE_xBCxxFGx, wire ACYL_SCANNINGp, wire XUJA_SPR_OAM_LATCHn, uint8_t* oam_ram);

  void dump(Dumper& d) {
    d.dump_bitp   ("MAKA_LATCH_EXTp  : ", MAKA_LATCH_EXTp.state);
    d.dump_bitp   ("WUJE_CPU_OAM_WRn : ", WUJE_CPU_OAM_WRn.state);
    d.dump_bitp   ("SIG_OAM_CLKn     : ", SIG_OAM_CLKn.get_state());
    d.dump_bitp   ("SIG_OAM_WRn_A    : ", SIG_OAM_WRn_A.get_state());
    d.dump_bitp   ("SIG_OAM_WRn_B    : ", SIG_OAM_WRn_B.get_state());
    d.dump_bitp   ("SIG_OAM_OEn      : ", SIG_OAM_OEn.get_state());

    //d.dump_slice2n("BUS_OAM_An  : ", &BUS_OAM_A00n, 8);
    //d.dump_slice2n("BUS_OAM_DAn : ", &BUS_OAM_DA00n, 8);
    //d.dump_slice2n("BUS_OAM_DBn : ", &BUS_OAM_DB00n, 8);
    d.dump_slice2n("OAM LATCH A : ", &oam_latch_a.YDYV_OAM_LATCH_DA0n, 8);
    d.dump_slice2n("OAM LATCH B : ", &oam_latch_b.XYKY_OAM_LATCH_DB0n, 8);
    d.dump_slice2p("OAM TEMP A  : ", &oam_temp_a.XUSO_OAM_DA0p, 8);
    d.dump_slice2p("OAM TEMP B  : ", &oam_temp_b.YLOR_OAM_DB0p, 8);

  }

  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  OamTempA oam_temp_a;
  OamTempB oam_temp_b;
  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  /*SIG_OAM_CLKn */ SigOut SIG_OAM_CLKn;   // ABCDEFGH
  /*SIG_OAM_WRn_A*/ SigOut SIG_OAM_WRn_A;  // AxxxExxH
  /*SIG_OAM_WRn_B*/ SigOut SIG_OAM_WRn_B;  // AxxxExxH
  /*SIG_OAM_OEn  */ SigOut SIG_OAM_OEn;    // ABCDEFGH

  Gate old_oam_clk;
};

//------------------------------------------------------------------------------------------------------------------------
