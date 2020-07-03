#include "Sch_DMA.h"

#include "Sch_Top.h"

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

//------------------------------------------------------------------------------

DmaSignals DmaRegisters::sig(const SchematicTop& /*gb*/) const {

  DmaSignals sig;

  // Die trace:
  // LEBU = not(MARU06)
  // MUDA = nor(PULA06, POKU06, LEBU);

  // if rung 6 of MARU/PULA/POKU was QN:
  // MUDA = and(A13, A14, !A15);
  // would select last quarter of ROM, which doesn't make sense
  // so rung 6 of MARU must be Q.

  /*p04.MATU*/ sig.MATU_DMA_RUNNINGp = MATU_DMA_RUNNINGp;

  /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(DMA_A15.q());
  /*p04.MUDA*/ sig.MUDA_DMA_SRC_VRAMp = nor(DMA_A13.q(), DMA_A14.q(), LEBU_DMA_ADDR_A15n);

  sig.DMA_A00 = DMA_A00;
  sig.DMA_A01 = DMA_A01;
  sig.DMA_A02 = DMA_A02;
  sig.DMA_A03 = DMA_A03;
  sig.DMA_A04 = DMA_A04;
  sig.DMA_A05 = DMA_A05;
  sig.DMA_A06 = DMA_A06;
  sig.DMA_A07 = DMA_A07;
  sig.DMA_A08 = DMA_A08;
  sig.DMA_A09 = DMA_A09;
  sig.DMA_A10 = DMA_A10;
  sig.DMA_A11 = DMA_A11;
  sig.DMA_A12 = DMA_A12;
  sig.DMA_A13 = DMA_A13;
  sig.DMA_A14 = DMA_A14;
  sig.DMA_A15 = DMA_A15;

  return sig;
}

//------------------------------------------------------------------------------

void DmaRegisters::tick(SchematicTop& gb) {
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb.cpu_bus, gb.EXT_PIN_CLK_GOOD);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto bus_sig = gb.bus_mux.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  auto& vram_bus = gb.vram_bus;

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

  /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p04.LOKO*/ wire LOKO_DMA_RSTp = nand(CUNU_RSTn, !LENE_DMA_TRIG_d4.q());

  /*p22.WATE*/ wire WATE_FF46n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.WESA_A01p, cpu_sig.WALO_A02p, cpu_sig.XERA_A03n);
  /*p22.XEDA*/ wire XEDA_FF46p = not(WATE_FF46n);
  /*p04.MOLU*/ wire MOLU_FF46_RDn = nand(XEDA_FF46p, cpu_sig.ASOT_CPU_RD);
  /*p04.LAVY*/ wire LAVY_FF46_WRp = and (XEDA_FF46p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);

  {
    /*p04.LYXE*/ LYXE_DMA_LATCHn.nor_latch(LOKO_DMA_RSTp, LAVY_FF46_WRp);
    /*p04.LUPA*/ wire LUPA_DMA_TRIG = nor(LAVY_FF46_WRp, LYXE_DMA_LATCHn.q());

    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(clk_sig.NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);

    /*p04.LUVY*/ LUVY_DMA_TRIG_d0.set(UVYT_xBCDExxx, CUNU_RSTn, LUPA_DMA_TRIG);
    /*p04.LENE*/ LENE_DMA_TRIG_d4.set(MOPA_AxxxxFGH, CUNU_RSTn, LUVY_DMA_TRIG_d0.q());
  }

  {
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(clk_sig.NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);

    // NAND latch
    /*p04.LOKY*/ LOKY_DMA_LATCHp = nand(LARA_DMA_LATCHn, !LENE_DMA_TRIG_d4.q());
    /*p04.LARA*/ LARA_DMA_LATCHn = nand(LOKY_DMA_LATCHp, CUNU_RSTn, !MYTE_DMA_DONE.q());
    /*p04.MATU*/ MATU_DMA_RUNNINGp.set(UVYT_xBCDExxx, CUNU_RSTn, LOKY_DMA_LATCHp);
  }

  {
    /*p04.LAPA*/ wire LAPA_DMA_RSTn = not(LOKO_DMA_RSTp);
    /*p04.NAVO*/ wire NAVO_DMA_DONEn = nand(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A07.q()); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire NOLO_DMA_DONEp = not(NAVO_DMA_DONEn);

    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(clk_sig.NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);
    /*p04.MYTE*/ MYTE_DMA_DONE.set(MOPA_AxxxxFGH, LAPA_DMA_RSTn, NOLO_DMA_DONEp);

    /*p04.META*/ wire META_DMA_CLKp = and(UVYT_xBCDExxx, LOKY_DMA_LATCHp);
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
    // DMA vram read
    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn   = nand(dma_sig.MATU_DMA_RUNNINGp, dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p04.AHOC*/ wire AHOC_DMA_VRAM_RDn = not(LUFA_DMA_READ_VRAMp);

    /*p04.ECAL*/ vram_bus.TRI_A00.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A00);
    /*p04.EGEZ*/ vram_bus.TRI_A01.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A01);
    /*p04.FUHE*/ vram_bus.TRI_A02.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A02);
    /*p04.FYZY*/ vram_bus.TRI_A03.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A03);
    /*p04.DAMU*/ vram_bus.TRI_A04.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A04);
    /*p04.DAVA*/ vram_bus.TRI_A05.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A05);
    /*p04.ETEG*/ vram_bus.TRI_A06.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A06);
    /*p04.EREW*/ vram_bus.TRI_A07.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A07);
    /*p04.EVAX*/ vram_bus.TRI_A08.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A08);
    /*p04.DUVE*/ vram_bus.TRI_A09.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A09);
    /*p04.ERAF*/ vram_bus.TRI_A10.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A10);
    /*p04.FUSY*/ vram_bus.TRI_A11.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A11);
    /*p04.EXYF*/ vram_bus.TRI_A12.set_tribuf(AHOC_DMA_VRAM_RDn, dma_sig.DMA_A12);
  }

  // FF46 DMA
  {
    /*p04.LORU*/ wire LORU_FF46_WRn = not(LAVY_FF46_WRp);
    /*p04.NYGO*/ wire NYGO_FF46_RDp = not(MOLU_FF46_RDn);
    /*p04.PUSY*/ wire PUSY_FF46_RDn = not(NYGO_FF46_RDp);

    /*p04.POLY*/ cpu_bus.CPU_TRI_D0.set_tribuf(!PUSY_FF46_RDn, DMA_A08);
    /*p04.ROFO*/ cpu_bus.CPU_TRI_D1.set_tribuf(!PUSY_FF46_RDn, DMA_A09);
    /*p04.REMA*/ cpu_bus.CPU_TRI_D2.set_tribuf(!PUSY_FF46_RDn, DMA_A10);
    /*p04.PANE*/ cpu_bus.CPU_TRI_D3.set_tribuf(!PUSY_FF46_RDn, DMA_A11);
    /*p04.PARE*/ cpu_bus.CPU_TRI_D4.set_tribuf(!PUSY_FF46_RDn, DMA_A12);
    /*p04.RALY*/ cpu_bus.CPU_TRI_D5.set_tribuf(!PUSY_FF46_RDn, DMA_A13);
    /*p04.RESU*/ cpu_bus.CPU_TRI_D6.set_tribuf(!PUSY_FF46_RDn, DMA_A14);
    /*p04.NUVY*/ cpu_bus.CPU_TRI_D7.set_tribuf(!PUSY_FF46_RDn, DMA_A15);

    /*p04.NAFA*/ DMA_A08.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D0);
    /*p04.PYNE*/ DMA_A09.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D1);
    /*p04.PARA*/ DMA_A10.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D2);
    /*p04.NYDO*/ DMA_A11.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D3);
    /*p04.NYGY*/ DMA_A12.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D4);
    /*p04.PULA*/ DMA_A13.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D5);
    /*p04.POKU*/ DMA_A14.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D6);
    /*p04.MARU*/ DMA_A15.set(LORU_FF46_WRn, cpu_bus.CPU_TRI_D7);
  }
}

//-----------------------------------------------------------------------------

SignalHash DmaRegisters::commit() {
  SignalHash hash;
  /*p04.MATU*/ hash << MATU_DMA_RUNNINGp.commit_reg(); // -> p25,p28
  /*p04.MYTE*/ hash << MYTE_DMA_DONE.commit_reg();
  /*p04.LUVY*/ hash << LUVY_DMA_TRIG_d0.commit_reg();
  /*p04.LENE*/ hash << LENE_DMA_TRIG_d4.commit_reg();
  /*p04.NAKY*/ hash << DMA_A00.commit_reg();
  /*p04.PYRO*/ hash << DMA_A01.commit_reg(); 
  /*p04.NEFY*/ hash << DMA_A02.commit_reg(); 
  /*p04.MUTY*/ hash << DMA_A03.commit_reg(); 
  /*p04.NYKO*/ hash << DMA_A04.commit_reg(); 
  /*p04.PYLO*/ hash << DMA_A05.commit_reg(); 
  /*p04.NUTO*/ hash << DMA_A06.commit_reg(); 
  /*p04.MUGU*/ hash << DMA_A07.commit_reg(); 
  /*p04.NAFA*/ hash << DMA_A08.commit_reg(); 
  /*p04.PYNE*/ hash << DMA_A09.commit_reg(); 
  /*p04.PARA*/ hash << DMA_A10.commit_reg(); 
  /*p04.NYDO*/ hash << DMA_A11.commit_reg(); 
  /*p04.NYGY*/ hash << DMA_A12.commit_reg(); 
  /*p04.PULA*/ hash << DMA_A13.commit_reg(); 
  /*p04.POKU*/ hash << DMA_A14.commit_reg(); 
  /*p04.MARU*/ hash << DMA_A15.commit_reg(); 
  /*p04.LYXE*/ hash << LYXE_DMA_LATCHn.commit_latch();

  // NAND latch
  /*p04.LARA*/ hash << LARA_DMA_LATCHn.commit_gate();
  /*p04.LOKY*/ hash << LOKY_DMA_LATCHp.commit_gate();

  return hash;
}

//-----------------------------------------------------------------------------


#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- DMA REG -----\n");
  //FROM_CPU5_SYNC.dump(text_painter, "FROM_CPU5_SYNC   ");
  MATU_DMA_RUNNINGp.dump(text_painter, "DMA_RUNNING  ");
  MYTE_DMA_DONE.dump(text_painter, "MYTE_DMA_DONE    ");
  LUVY_DMA_TRIG_d0.dump(text_painter, "LUVY    ");
  LENE_DMA_TRIG_d4.dump(text_painter, "LENE    ");
  LYXE_DMA_LATCHn.dump(text_painter, "LYXE ");
  LOKY_DMA_LATCHp.dump(text_painter, "LOKY  ");
  text_painter.dprintf("DMA ADDR LO      0x%02x\n", get_addr_lo());
  text_painter.dprintf("DMA ADDR HI      0x%02x\n", get_addr_hi());
  text_painter.newline();
}

int get_addr_lo() {
  return pack(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A05.q(), DMA_A06.q(), DMA_A07.q());
}
int get_addr_hi() {
  return pack(DMA_A08.q(), DMA_A09.q(), DMA_A10.q(), DMA_A11.q(), DMA_A12.q(), DMA_A13.q(), DMA_A14.q(), DMA_A15.q());
}

#endif