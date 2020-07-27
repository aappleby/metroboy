#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  // -> bunch of stuff
  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const { return _MATU_DMA_RUNNINGp.q(); }

  // -> ext bus, oam bus
  /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp() const {
    /*p04.LOGO*/ wire _LOGO_DMA_VRAMn      = not(MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire _MORY_DMA_READ_CARTn = nand(MATU_DMA_RUNNINGp(), _LOGO_DMA_VRAMn);
    return not(_MORY_DMA_READ_CARTn);
  }

  // -> top.ACYL, top.AJON
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn() const { return not(MATU_DMA_RUNNINGp()); }

  // -> oam bus, vram bus
  /*p04.LUFA*/ wire LUFA_DMA_VRM_RDp() const {
    /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    return not(MUHO_DMA_VRAM_RDn);
  }

  void dump(Dumper& d) {
    d("---------- DMA Reg  ----------\n");
    d("DMA Addr 0x%02x:%02x\n", 
      pack(MARU_DMA_A15.q(), POKU_DMA_A14.q(), PULA_DMA_A13.q(), NYGY_DMA_A12.q(), NYDO_DMA_A11.q(), PARA_DMA_A10.q(), PYNE_DMA_A09.q(), NAFA_DMA_A08.q()),
      pack(MUGU_DMA_A07.q(), NUTO_DMA_A06.q(), PYLO_DMA_A05.q(), NYKO_DMA_A04.q(), MUTY_DMA_A03.q(), NEFY_DMA_A02.q(), PYRO_DMA_A01.q(), NAKY_DMA_A00.q()));
    d("LYXE_DMA_LATCHn     %d\n", _LYXE_DMA_LATCHn.q());
    d("MATU_DMA_RUNNINGp   %d\n", _MATU_DMA_RUNNINGp.q());
    d("MYTE_DMA_DONE       %d\n", _MYTE_DMA_DONE.q());
    d("LUVY_DMA_TRIG_d0    %d\n", _LUVY_DMA_TRIG_d0.q());
    d("LENE_DMA_TRIG_d4    %d\n", _LENE_DMA_TRIG_d4.q());
    d("LOKY_DMA_LATCHp     %d\n", _LOKY_DMA_LATCHp.q());
    d("MUDA_DMA_SRC_VRAMp  %d\n", MUDA_DMA_SRC_VRAMp());
    d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_READ_CARTp());
    d("\n");
  }

  /*p04.NAKY*/ Reg2 NAKY_DMA_A00 = Reg2::D0C0;
  /*p04.PYRO*/ Reg2 PYRO_DMA_A01 = Reg2::D0C0;
  /*p04.NEFY*/ Reg2 NEFY_DMA_A02 = Reg2::D0C0;
  /*p04.MUTY*/ Reg2 MUTY_DMA_A03 = Reg2::D0C0;
  /*p04.NYKO*/ Reg2 NYKO_DMA_A04 = Reg2::D0C0;
  /*p04.PYLO*/ Reg2 PYLO_DMA_A05 = Reg2::D0C0;
  /*p04.NUTO*/ Reg2 NUTO_DMA_A06 = Reg2::D0C0;
  /*p04.MUGU*/ Reg2 MUGU_DMA_A07 = Reg2::D0C0;

  /*p04.NAFA*/ Reg2 NAFA_DMA_A08 = Reg2::D0C0;
  /*p04.PYNE*/ Reg2 PYNE_DMA_A09 = Reg2::D0C0;
  /*p04.PARA*/ Reg2 PARA_DMA_A10 = Reg2::D0C0;
  /*p04.NYDO*/ Reg2 NYDO_DMA_A11 = Reg2::D0C0;
  /*p04.NYGY*/ Reg2 NYGY_DMA_A12 = Reg2::D0C0;
  /*p04.PULA*/ Reg2 PULA_DMA_A13 = Reg2::D0C0;
  /*p04.POKU*/ Reg2 POKU_DMA_A14 = Reg2::D0C0;
  /*p04.MARU*/ Reg2 MARU_DMA_A15 = Reg2::D0C0;

private:

  wire MUDA_DMA_SRC_VRAMp() const {
    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(MARU_DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(PULA_DMA_A13.q(), POKU_DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_SRC_VRAMp;
  }

  /*p04.LYXE*/ Pin2 _LYXE_DMA_LATCHn   = Pin2::LATCH_0;
  /*p04.MATU*/ Reg2 _MATU_DMA_RUNNINGp = Reg2::D0C0;
  /*p04.MYTE*/ Reg2 _MYTE_DMA_DONE     = Reg2::D0C0;
  /*p04.LUVY*/ Reg2 _LUVY_DMA_TRIG_d0  = Reg2::D0C0;
  /*p04.LENE*/ Reg2 _LENE_DMA_TRIG_d4  = Reg2::D0C0;
  /*p04.LOKY*/ Pin2 _LOKY_DMA_LATCHp   = Pin2::LATCH_0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics