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
    d("LYXE_DMA_LATCHn     %d\n", _LYXE_DMA_LATCHn);
    d("MATU_DMA_RUNNINGp   %d\n", _MATU_DMA_RUNNINGp.q());
    d("MYTE_DMA_DONE       %d\n", _MYTE_DMA_DONE.q());
    d("LUVY_DMA_TRIG_d0    %d\n", _LUVY_DMA_TRIG_d0.q());
    d("LENE_DMA_TRIG_d4    %d\n", _LENE_DMA_TRIG_d4.q());
    d("LOKY_DMA_LATCHp     %d\n", _LOKY_DMA_LATCHp);
    d("MUDA_DMA_SRC_VRAMp  %d\n", MUDA_DMA_SRC_VRAMp());
    d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_READ_CARTp());
    d("\n");
  }

  /*p04.NAKY*/ Reg NAKY_DMA_A00 = REG_D0C0;
  /*p04.PYRO*/ Reg PYRO_DMA_A01 = REG_D0C0;
  /*p04.NEFY*/ Reg NEFY_DMA_A02 = REG_D0C0;
  /*p04.MUTY*/ Reg MUTY_DMA_A03 = REG_D0C0;
  /*p04.NYKO*/ Reg NYKO_DMA_A04 = REG_D0C0;
  /*p04.PYLO*/ Reg PYLO_DMA_A05 = REG_D0C0;
  /*p04.NUTO*/ Reg NUTO_DMA_A06 = REG_D0C0;
  /*p04.MUGU*/ Reg MUGU_DMA_A07 = REG_D0C0;

  /*p04.NAFA*/ Reg NAFA_DMA_A08 = REG_D0C0;
  /*p04.PYNE*/ Reg PYNE_DMA_A09 = REG_D0C0;
  /*p04.PARA*/ Reg PARA_DMA_A10 = REG_D0C0;
  /*p04.NYDO*/ Reg NYDO_DMA_A11 = REG_D0C0;
  /*p04.NYGY*/ Reg NYGY_DMA_A12 = REG_D0C0;
  /*p04.PULA*/ Reg PULA_DMA_A13 = REG_D0C0;
  /*p04.POKU*/ Reg POKU_DMA_A14 = REG_D0C0;
  /*p04.MARU*/ Reg MARU_DMA_A15 = REG_D0C0;

private:

  wire MUDA_DMA_SRC_VRAMp() const {
    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(MARU_DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(PULA_DMA_A13.q(), POKU_DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_SRC_VRAMp;
  }

  /*p04.LYXE*/ Tri _LYXE_DMA_LATCHn   = TRI_D0NP;
  /*p04.MATU*/ Reg _MATU_DMA_RUNNINGp = REG_D0C0;
  /*p04.MYTE*/ Reg _MYTE_DMA_DONE     = REG_D0C0;
  /*p04.LUVY*/ Reg _LUVY_DMA_TRIG_d0  = REG_D0C0;
  /*p04.LENE*/ Reg _LENE_DMA_TRIG_d4  = REG_D0C0;
  /*p04.LOKY*/ Tri _LOKY_DMA_LATCHp   = TRI_D0NP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics