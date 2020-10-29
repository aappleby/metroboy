#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void dump(Dumper& d) const {
    d("\002===== DMA Reg =====\001\n");

    int dma_addr_hi = pack_p(!NAFA_DMA_A08n.qp08(), !PYNE_DMA_A09n.qp08(), !PARA_DMA_A10n.qp08(), !NYDO_DMA_A11n.qp08(),
                             !NYGY_DMA_A12n.qp08(), !PULA_DMA_A13n.qp08(), !POKU_DMA_A14n.qp08(), !MARU_DMA_A15n.qp08());

    int dma_addr_lo = pack_p(NAKY_DMA_A00p.qp17(), PYRO_DMA_A01p.qp17(), NEFY_DMA_A02p.qp17(), MUTY_DMA_A03p.qp17(),
                             NYKO_DMA_A04p.qp17(), PYLO_DMA_A05p.qp17(), NUTO_DMA_A06p.qp17(), MUGU_DMA_A07p.qp17());

    d("DMA Addr 0x%02x:%02x\n",dma_addr_hi, dma_addr_lo);
    d("MATU_DMA_RUNNINGp   %d\n", MATU_DMA_RUNNINGp.qp17());
    d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_CARTp.qp());
    d("LUFA_DMA_VRM_RDp    %d\n", LUFA_DMA_VRAMp.qp());
    d("LYXE_DMA_LATCHn     %d\n", LYXE_DMA_LATCHp);
    d("MYTE_DMA_DONE       %d\n", !MYTE_DMA_DONE.qn16());
    d("LUVY_DMA_TRIG_d0    %d\n",  LUVY_DMA_TRIG_d0.qp17());
    d("LENE_DMA_TRIG_d4    %d\n", !LENE_DMA_TRIG_d4.qn16());
    d("LOKY_DMA_LATCHp     %d\n",  LOKY_DMA_LATCHp.qp03());
    d("\n");
  }

  void reset_cart() {
    MATU_DMA_RUNNINGp.reset(REG_D0C1);

    LUMA_DMA_CARTp.reset(TRI_D0NP);
    LUFA_DMA_VRAMp.reset(TRI_D0NP);
    BOGE_DMA_RUNNINGn.reset(TRI_D1NP);

    NAKY_DMA_A00p.reset(REG_D0C0);
    PYRO_DMA_A01p.reset(REG_D0C1);
    NEFY_DMA_A02p.reset(REG_D0C1);
    MUTY_DMA_A03p.reset(REG_D0C1);
    NYKO_DMA_A04p.reset(REG_D0C1);
    PYLO_DMA_A05p.reset(REG_D0C1);
    NUTO_DMA_A06p.reset(REG_D0C1);
    MUGU_DMA_A07p.reset(REG_D0C1);

    NAFA_DMA_A08n.reset(REG_D0C1);
    PYNE_DMA_A09n.reset(REG_D0C1);
    PARA_DMA_A10n.reset(REG_D0C1);
    NYDO_DMA_A11n.reset(REG_D0C1);
    NYGY_DMA_A12n.reset(REG_D0C1);
    PULA_DMA_A13n.reset(REG_D0C1);
    POKU_DMA_A14n.reset(REG_D0C1);
    MARU_DMA_A15n.reset(REG_D0C1);

    LYXE_DMA_LATCHp.reset(TRI_D0NP);

    MYTE_DMA_DONE.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0.reset(REG_D0C1);
    LENE_DMA_TRIG_d4.reset(REG_D0C0);

    LOKY_DMA_LATCHp.reset(TRI_D0NP);
  }

  void reset_boot() {
    MATU_DMA_RUNNINGp.reset(REG_D0C0);

    LUMA_DMA_CARTp.reset(TRI_HZNP);
    LUFA_DMA_VRAMp.reset(TRI_HZNP);

    NAKY_DMA_A00p.reset(REG_D0C0);
    PYRO_DMA_A01p.reset(REG_D0C0);
    NEFY_DMA_A02p.reset(REG_D0C0);
    MUTY_DMA_A03p.reset(REG_D0C0);
    NYKO_DMA_A04p.reset(REG_D0C0);
    PYLO_DMA_A05p.reset(REG_D0C0);
    NUTO_DMA_A06p.reset(REG_D0C0);
    MUGU_DMA_A07p.reset(REG_D0C0);

    NAFA_DMA_A08n.reset(REG_D0C0);
    PYNE_DMA_A09n.reset(REG_D0C0);
    PARA_DMA_A10n.reset(REG_D0C0);
    NYDO_DMA_A11n.reset(REG_D0C0);
    NYGY_DMA_A12n.reset(REG_D0C0);
    PULA_DMA_A13n.reset(REG_D0C0);
    POKU_DMA_A14n.reset(REG_D0C0);
    MARU_DMA_A15n.reset(REG_D0C0);

    LYXE_DMA_LATCHp.reset(TRI_D0NP);

    MYTE_DMA_DONE.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0.reset(REG_D0C0);
    LENE_DMA_TRIG_d4.reset(REG_D0C0);

    LOKY_DMA_LATCHp.reset(TRI_D0NP);
  }

  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp;

  /*p04.LUMA*/ Sig LUMA_DMA_CARTp;
  /*p04.LUFA*/ Sig LUFA_DMA_VRAMp;
  /*p28.BOGE*/ Sig BOGE_DMA_RUNNINGn;

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p;
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p;
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p;
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p;
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p;
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p;
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p;
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p;

  /*p04.NAFA*/ DFF8p NAFA_DMA_A08n;
  /*p04.PYNE*/ DFF8p PYNE_DMA_A09n;
  /*p04.PARA*/ DFF8p PARA_DMA_A10n;
  /*p04.NYDO*/ DFF8p NYDO_DMA_A11n;
  /*p04.NYGY*/ DFF8p NYGY_DMA_A12n;
  /*p04.PULA*/ DFF8p PULA_DMA_A13n;
  /*p04.POKU*/ DFF8p POKU_DMA_A14n;
  /*p04.MARU*/ DFF8p MARU_DMA_A15n;

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp;
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE;
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4;

  /*p04.LOKY*/ NandLatch LOKY_DMA_LATCHp; // NAND latch w/ LARA
  ///*p04.LARA*/ Tri   LARA_DMA_LATCHn = TRI_D1NP; // NAND latch w/ LOKY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics