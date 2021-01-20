#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyBuses.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyDMA {

  void write_dma_sync(GateBoyCpuBus& cpu_bus, const GateBoyBuses& new_bus);
  void tock(GateBoyResetDebug& rst, GateBoyClock& clk, GateBoyCpuBus& cpu_bus, const GateBoyBuses& new_bus);
  void read_dma(GateBoyCpuBus& cpu_bus, GateBoyBuses& new_bus);

  wire LUMA_DMA_CARTp() const;
  wire LUFA_DMA_VRAMp() const;

  wire BOGE_DMA_RUNNINGn() const {
    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp.qp_new());
    return BOGE_DMA_RUNNINGn;
  }
  wire DUGA_DMA_RUNNINGn() const {
    /* p04.DUGA*/ wire DUGA_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp.qp_new());
    return DUGA_DMA_RUNNINGn;
  }

  void dump(Dumper& d) {
    d.dump_slice2p("DMA_A_LOW  : ", &NAKY_DMA_A00p, 8);
    d.dump_slice2n("DMA_A_HIGH : ", &NAFA_DMA_A08n, 8);
    d             ("DMA Addr   : 0x%02x:%02x\n", BitBase::pack_oldn(8, &NAFA_DMA_A08n), BitBase::pack_old(8, &NAKY_DMA_A00p));
    d.dump_bitp   ("MATU_DMA_RUNNINGp : ", MATU_DMA_RUNNINGp.state);
    d.dump_bitp   ("LYXE_DMA_LATCHp   : ", LYXE_DMA_LATCHp  .state);
    d.dump_bitp   ("MYTE_DMA_DONE     : ", MYTE_DMA_DONE    .state);
    d.dump_bitp   ("LUVY_DMA_TRIG_d0  : ", LUVY_DMA_TRIG_d0 .state);
    d.dump_bitp   ("LENE_DMA_TRIG_d4  : ", LENE_DMA_TRIG_d4 .state);
    d.dump_bitp   ("LOKY_DMA_LATCHp   : ", LOKY_DMA_LATCHp  .get_state());
    d("\n");
  }

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p;      // Axxxxxxx
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p;      // Axxxxxxx
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p;      // Axxxxxxx
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p;      // Axxxxxxx
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p;      // Axxxxxxx
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p;      // Axxxxxxx
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p;      // Axxxxxxx
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p;      // Axxxxxxx

  /*p04.NAFA*/ DFF8p NAFA_DMA_A08n;      // xxxxxxxH
  /*p04.PYNE*/ DFF8p PYNE_DMA_A09n;      // xxxxxxxH
  /*p04.PARA*/ DFF8p PARA_DMA_A10n;      // xxxxxxxH
  /*p04.NYDO*/ DFF8p NYDO_DMA_A11n;      // xxxxxxxH
  /*p04.NYGY*/ DFF8p NYGY_DMA_A12n;      // xxxxxxxH
  /*p04.PULA*/ DFF8p PULA_DMA_A13n;      // xxxxxxxH
  /*p04.POKU*/ DFF8p POKU_DMA_A14n;      // xxxxxxxH
  /*p04.MARU*/ DFF8p MARU_DMA_A15n;      // xxxxxxxH

  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp;  // Axxxxxxx
  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp; // xxxxExxx
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE;      // xxxxExxx
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0;   // Axxxxxxx
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4;   // xxxxExxx

  /*p04.LARA*/ Gate LARA_DMA_LATCHn;     // xxxxExxx - NAND latch w/ LOKY
  /*p04.LOKY*/ Gate LOKY_DMA_LATCHp;     // xxxxExxx - NAND latch w/ LARA
};

//------------------------------------------------------------------------------------------------------------------------
