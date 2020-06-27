#include "Sch_DMA.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

#if 0

// LAVY00 << CUPA04
// LAVY01 << XEDA02
// LAVY02 nc
// LAVY03 >> LORU00

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LAVY = and2(FF46, CUPA)
// NAVO = nand6(dma addr)
// NOLO = not(NAVO)
// LOKO = nand2(CUNU, !LENE) // check neg output?
// LUPA = nor2(LAVY, LYXE)
// LUVY = reg
// MATU = reg
// LENE = reg
// MYTE_DMA_DONE = reg
// LYXE = latch(LOKO, LAVY)
// LARA = nand3(LOKY, CUNU, !MYTE_DMA_DONE)
// LOKY = nand2(LARA, !LENE)
// META = and2(UVYT, LOKY)
// LORU = not(LAVY)

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LUPA00 << LAVY
// LUPA01 << LYXE
// LUPA02 >> LUVY

// LOGO = not(MUDA);
// MORY = nand(MATU17, LOGO) MATU17 must be Q
// LUMA = not(MORY);

#endif


DmaSignals DmaRegisters::sig(const TestGB& /*gb*/) const {

  // Die trace:
  // LEBU = not(MARU06)
  // MUDA = nor(PULA06, POKU06, LEBU);

#if 0
    // if rung 6 of MARU/PULA/POKU was QN:
    // MUDA = and(A13, A14, !A15);
    // would select last quarter of ROM, which doesn't make sense
    // so rung 6 of MARU must be Q.
#endif

  /*p04.LEBU*/ wire _LEBU_DMA_ADDR_A15n  = not(DMA_A15.q());
  /*p04.MUDA*/ wire _MUDA_DMA_ADDR_VRAMp = nor(DMA_A13.q(), DMA_A14.q(), _LEBU_DMA_ADDR_A15n);
  /*p04.MUHO*/ wire _MUHO_DMA_VRAM_RDn   = nand(MATU_DMA_OAM_WRp.q(), _MUDA_DMA_ADDR_VRAMp);
  /*p04.LOGO*/ wire _LOGO_DMA_VRAMn      = not(_MUDA_DMA_ADDR_VRAMp);
  /*p04.MORY*/ wire _MORY_DMA_READ_CARTn = nand(MATU_DMA_OAM_WRp.q(), _LOGO_DMA_VRAMn); // This seems wrong, like it should be DMA_READ_CART = and(DMA_RUNNING, !DMA_VRAM);
  /*p04.LUMA*/ wire _LUMA_DMA_READ_CARTp = not(_MORY_DMA_READ_CARTn);
  /*p04.LUFA*/ wire _LUFA_DMA_READ_VRAMp = not(_MUHO_DMA_VRAM_RDn);
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn    = not(MATU_DMA_OAM_WRp.q());

  return {
    .BOGE_DMA_RUNNINGn = BOGE_DMA_RUNNINGn,
    .MORY_DMA_READ_CARTn = _MORY_DMA_READ_CARTn,
    .LUMA_DMA_READ_CARTp = _LUMA_DMA_READ_CARTp,
    .LUFA_DMA_READ_VRAMp = _LUFA_DMA_READ_VRAMp,

    .MATU_DMA_OAM_WRp = MATU_DMA_OAM_WRp,

    .DMA_A00 = DMA_A00,
    .DMA_A01 = DMA_A01,
    .DMA_A02 = DMA_A02,
    .DMA_A03 = DMA_A03,
    .DMA_A04 = DMA_A04,
    .DMA_A05 = DMA_A05,
    .DMA_A06 = DMA_A06,
    .DMA_A07 = DMA_A07,
    .DMA_A08 = DMA_A08,
    .DMA_A09 = DMA_A09,
    .DMA_A10 = DMA_A10,
    .DMA_A11 = DMA_A11,
    .DMA_A12 = DMA_A12,
    .DMA_A13 = DMA_A13,
    .DMA_A14 = DMA_A14,
    .DMA_A15 = DMA_A15,
  };
}

void DmaRegisters::tick(TestGB& gb) {
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto rst_sig = gb.rst_reg.sig(gb);

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  //----------------------------------------

  // Arms on ground, NOR latch
  // LYXE00 << LAVY03
  // LYXE01 nc
  // LYXE02 >> LUPA01
  // LYXE03 >> nc
  // LYXE04 nc
  // LYXE05 << LOKO02

  // if LOKO02 goes high, LYXE02 goes high.
  // if LAVY03 goes high, LYXE02 goes low.

  /*p04.LOKO*/ wire LOKO_DMA_RSTp = nand(rst_sig.CUNU_RSTn, !LENE_DMA_TRIG_d4.q());
  /*p04.LAPA*/ wire LAPA_DMA_RSTn = not(LOKO_DMA_RSTp);

  /*p04.LAVY*/ wire LAVY_FF46_WRp = and (adr_sig.XEDA_FF46p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
  {
    /*p04.LYXE*/ LYXE_DMA_LATCHn.nor_latch(LOKO_DMA_RSTp, LAVY_FF46_WRp);
  }

  {
    /*p04.LUPA*/ wire LUPA_DMA_TRIG = nor(LAVY_FF46_WRp, LYXE_DMA_LATCHn.q());
    /*p04.LUVY*/ LUVY_DMA_TRIG_d0.set(clk_sig.UVYT_xBCDExxx, rst_sig.CUNU_RSTn, LUPA_DMA_TRIG);
    /*p04.LENE*/ LENE_DMA_TRIG_d4.set(clk_sig.MOPA_AxxxxFGH, rst_sig.CUNU_RSTn, LUVY_DMA_TRIG_d0.q());
  }

  {
    /*p04.NAVO*/ wire NAVO_DMA_DONEn = nand(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A07.q()); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire NOLO_DMA_DONEp = not(NAVO_DMA_DONEn);
    /*p04.MYTE*/ MYTE_DMA_DONE.set(clk_sig.MOPA_AxxxxFGH, LAPA_DMA_RSTn, NOLO_DMA_DONEp);
  }

  // NAND latch
  /*p04.LOKY*/ LOKY_DMA_LATCHp = nand(LARA_DMA_LATCHn, !LENE_DMA_TRIG_d4.q());
  /*p04.LARA*/ LARA_DMA_LATCHn = nand(LOKY_DMA_LATCHp, rst_sig.CUNU_RSTn, !MYTE_DMA_DONE.q());
  /*p04.MATU*/ MATU_DMA_OAM_WRp.set(clk_sig.UVYT_xBCDExxx, rst_sig.CUNU_RSTn, LOKY_DMA_LATCHp);

  {
    /*p04.META*/ wire META_DMA_CLKp = and(clk_sig.UVYT_xBCDExxx, LOKY_DMA_LATCHp);

    /*p04.NAKY*/ DMA_A00.set(META_DMA_CLKp, LAPA_DMA_RSTn, !DMA_A00.q());
    /*p04.PYRO*/ DMA_A01.set(DMA_A00.qn(),  LAPA_DMA_RSTn, DMA_A01.qn());
    /*p04.NEFY*/ DMA_A02.set(DMA_A01.qn(),  LAPA_DMA_RSTn, DMA_A02.qn());
    /*p04.MUTY*/ DMA_A03.set(DMA_A02.qn(),  LAPA_DMA_RSTn, DMA_A03.qn());
    /*p04.NYKO*/ DMA_A04.set(DMA_A03.qn(),  LAPA_DMA_RSTn, DMA_A04.qn());
    /*p04.PYLO*/ DMA_A05.set(DMA_A04.qn(),  LAPA_DMA_RSTn, DMA_A05.qn());
    /*p04.NUTO*/ DMA_A06.set(DMA_A05.qn(),  LAPA_DMA_RSTn, DMA_A06.qn());
    /*p04.MUGU*/ DMA_A07.set(DMA_A06.qn(),  LAPA_DMA_RSTn, DMA_A07.qn());
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    /*p04.LAVY*/ wire FF46_WR = and (adr_sig.XEDA_FF46p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p04.LORU*/ wire FF46_WRn = not(FF46_WR);
    /*p04.NAFA*/ DMA_A08.set(FF46_WRn, cpu_bus.TS_D0);
    /*p04.PYNE*/ DMA_A09.set(FF46_WRn, cpu_bus.TS_D1);
    /*p04.PARA*/ DMA_A10.set(FF46_WRn, cpu_bus.TS_D2);
    /*p04.NYDO*/ DMA_A11.set(FF46_WRn, cpu_bus.TS_D3);
    /*p04.NYGY*/ DMA_A12.set(FF46_WRn, cpu_bus.TS_D4);
    /*p04.PULA*/ DMA_A13.set(FF46_WRn, cpu_bus.TS_D5);
    /*p04.POKU*/ DMA_A14.set(FF46_WRn, cpu_bus.TS_D6);
    /*p04.MARU*/ DMA_A15.set(FF46_WRn, cpu_bus.TS_D7);
  }


  {
    auto& cpu_bus = gb.cpu_bus;
    /*p04.MOLU*/ wire FF46_RDn1 = nand(adr_sig.XEDA_FF46p, cpu_sig.ASOT_CPU_RD);
    /*p04.NYGO*/ wire FF46_RD = not(FF46_RDn1);
    /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);
    /*p04.POLY*/ cpu_bus.TS_D0.set_tribuf(!FF46_RDn2, DMA_A08.q());
    /*p04.ROFO*/ cpu_bus.TS_D1.set_tribuf(!FF46_RDn2, DMA_A09.q());
    /*p04.REMA*/ cpu_bus.TS_D2.set_tribuf(!FF46_RDn2, DMA_A10.q());
    /*p04.PANE*/ cpu_bus.TS_D3.set_tribuf(!FF46_RDn2, DMA_A11.q());
    /*p04.PARE*/ cpu_bus.TS_D4.set_tribuf(!FF46_RDn2, DMA_A12.q());
    /*p04.RALY*/ cpu_bus.TS_D5.set_tribuf(!FF46_RDn2, DMA_A13.q());
    /*p04.RESU*/ cpu_bus.TS_D6.set_tribuf(!FF46_RDn2, DMA_A14.q());
    /*p04.NUVY*/ cpu_bus.TS_D7.set_tribuf(!FF46_RDn2, DMA_A15.q());
  }

  {
    auto dma_sig = sig(gb);
    auto& ext_bus = gb.ext_bus;
    auto& oam_bus = gb.oam_bus;

    /*p25.WEJO*/ oam_bus.PIN_DA0.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_bus.PIN_D0_C)));
    /*p25.BUBO*/ oam_bus.PIN_DA1.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_bus.PIN_D1_C)));
    /*p25.BETU*/ oam_bus.PIN_DA2.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_bus.PIN_D2_C)));
    /*p25.CYME*/ oam_bus.PIN_DA3.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_bus.PIN_D3_C)));
    /*p25.BAXU*/ oam_bus.PIN_DA4.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_bus.PIN_D4_C)));
    /*p25.BUHU*/ oam_bus.PIN_DA5.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_bus.PIN_D5_C)));
    /*p25.BYNY*/ oam_bus.PIN_DA6.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_bus.PIN_D6_C)));
    /*p25.BYPY*/ oam_bus.PIN_DA7.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_bus.PIN_D7_C)));
    /*p25.WASA*/ oam_bus.PIN_DB0.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_bus.PIN_D0_C)));
    /*p25.BOMO*/ oam_bus.PIN_DB1.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_bus.PIN_D1_C)));
    /*p25.BASA*/ oam_bus.PIN_DB2.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_bus.PIN_D2_C)));
    /*p25.CAKO*/ oam_bus.PIN_DB3.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_bus.PIN_D3_C)));
    /*p25.BUMA*/ oam_bus.PIN_DB4.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_bus.PIN_D4_C)));
    /*p25.BUPY*/ oam_bus.PIN_DB5.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_bus.PIN_D5_C)));
    /*p25.BASY*/ oam_bus.PIN_DB6.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_bus.PIN_D6_C)));
    /*p25.BAPE*/ oam_bus.PIN_DB7.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_bus.PIN_D7_C)));
  }

  {
    auto dma_sig = sig(gb);
    auto& vram_bus = gb.vram_bus;
    auto& oam_bus = gb.oam_bus;

    /*p28.AZAR*/ wire _AZAR_DMA_READ_VRAMn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ oam_bus.PIN_DA0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD0);
    /*p28.AXER*/ oam_bus.PIN_DA1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD1);
    /*p28.ASOX*/ oam_bus.PIN_DA2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD2);
    /*p28.CETU*/ oam_bus.PIN_DA3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD3);
    /*p28.ARYN*/ oam_bus.PIN_DA4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD4);
    /*p28.ACOT*/ oam_bus.PIN_DA5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD5);
    /*p28.CUJE*/ oam_bus.PIN_DA6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD6);
    /*p28.ATER*/ oam_bus.PIN_DA7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD7);
    /*p28.WOWA*/ oam_bus.PIN_DB0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD0);
    /*p28.AVEB*/ oam_bus.PIN_DB1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD1);
    /*p28.AMUH*/ oam_bus.PIN_DB2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD2);
    /*p28.COFO*/ oam_bus.PIN_DB3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD3);
    /*p28.AZOZ*/ oam_bus.PIN_DB4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD4);
    /*p28.AGYK*/ oam_bus.PIN_DB5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD5);
    /*p28.BUSE*/ oam_bus.PIN_DB6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD6);
    /*p28.ANUM*/ oam_bus.PIN_DB7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.TS_MD7);
  }
}

bool DmaRegisters::commit() {
  bool changed = false;
  return changed;
}