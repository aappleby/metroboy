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

//------------------------------------------------------------------------------

DmaSignals DmaRegisters::sig(const TestGB& /*gb*/) const {

  DmaSignals sig;

  // Die trace:
  // LEBU = not(MARU06)
  // MUDA = nor(PULA06, POKU06, LEBU);

  // if rung 6 of MARU/PULA/POKU was QN:
  // MUDA = and(A13, A14, !A15);
  // would select last quarter of ROM, which doesn't make sense
  // so rung 6 of MARU must be Q.

  /*p04.LEBU*/ wire _LEBU_DMA_ADDR_A15n  = not(DMA_A15.q());
  /*p04.MUDA*/ wire _MUDA_DMA_ADDR_VRAMp = nor(DMA_A13.q(), DMA_A14.q(), _LEBU_DMA_ADDR_A15n);
  /*p04.MUHO*/ wire _MUHO_DMA_VRAM_RDn   = nand(MATU_DMA_OAM_WRp.q(), _MUDA_DMA_ADDR_VRAMp);
  /*p04.LOGO*/ wire _LOGO_DMA_VRAMn      = not(_MUDA_DMA_ADDR_VRAMp);
  /*p04.MORY*/ sig.MORY_DMA_READ_CARTn = nand(MATU_DMA_OAM_WRp.q(), _LOGO_DMA_VRAMn); // This seems wrong, like it should be DMA_READ_CART = and(DMA_RUNNING, !DMA_VRAM);
  /*p04.LUMA*/ sig.LUMA_DMA_READ_CARTp = not(sig.MORY_DMA_READ_CARTn);
  /*p25.CEDE*/ sig.CEDE_DMA_READ_CARTn = not(sig.LUMA_DMA_READ_CARTp);
  /*p04.LUFA*/ sig.LUFA_DMA_READ_VRAMp = not(_MUHO_DMA_VRAM_RDn);
  /*p28.BOGE*/ sig.BOGE_DMA_RUNNINGn    = not(MATU_DMA_OAM_WRp.q());

  sig.MATU_DMA_OAM_WRp = MATU_DMA_OAM_WRp;

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

void DmaRegisters::tick(TestGB& gb) {
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto bus_sig = gb.bus_mux.sig(gb);

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

  /*p04.LOKO*/ wire LOKO_DMA_RSTp = nand(rst_sig.CUNU_RSTn, !LENE_DMA_TRIG_d4.q());

  /*p22.WATE*/ wire WATE_FF46n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.WESA_A01p, cpu_sig.WALO_A02p, cpu_sig.XERA_A03n);
  /*p22.XEDA*/ wire XEDA_FF46p = not(WATE_FF46n);

  /*p04.MOLU*/ wire MOLU_FF46_RDn = nand(XEDA_FF46p, cpu_sig.ASOT_CPU_RD);
  /*p04.LAVY*/ wire LAVY_FF46_WRp = and (XEDA_FF46p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);

  {
    /*p04.LYXE*/ LYXE_DMA_LATCHn.nor_latch(LOKO_DMA_RSTp, LAVY_FF46_WRp);
    /*p04.LUPA*/ wire LUPA_DMA_TRIG = nor(LAVY_FF46_WRp, LYXE_DMA_LATCHn.q());
    /*p04.LUVY*/ LUVY_DMA_TRIG_d0.set(clk_sig.UVYT_xBCDExxx, rst_sig.CUNU_RSTn, LUPA_DMA_TRIG);
    /*p04.LENE*/ LENE_DMA_TRIG_d4.set(clk_sig.MOPA_AxxxxFGH, rst_sig.CUNU_RSTn, LUVY_DMA_TRIG_d0.q());
  }

  {
    // NAND latch
    /*p04.LOKY*/ LOKY_DMA_LATCHp = nand(LARA_DMA_LATCHn, !LENE_DMA_TRIG_d4.q());
    /*p04.LARA*/ LARA_DMA_LATCHn = nand(LOKY_DMA_LATCHp, rst_sig.CUNU_RSTn, !MYTE_DMA_DONE.q());
    /*p04.MATU*/ MATU_DMA_OAM_WRp.set(clk_sig.UVYT_xBCDExxx, rst_sig.CUNU_RSTn, LOKY_DMA_LATCHp);
  }

  {
    /*p04.LAPA*/ wire LAPA_DMA_RSTn = not(LOKO_DMA_RSTp);
    /*p04.NAVO*/ wire NAVO_DMA_DONEn = nand(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A07.q()); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire NOLO_DMA_DONEp = not(NAVO_DMA_DONEn);
    /*p04.MYTE*/ MYTE_DMA_DONE.set(clk_sig.MOPA_AxxxxFGH, LAPA_DMA_RSTn, NOLO_DMA_DONEp);

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
    /*p04.LORU*/ wire LORU_FF46_WRn = not(LAVY_FF46_WRp);
    /*p04.NAFA*/ DMA_A08.set(LORU_FF46_WRn, cpu_bus.TRI_D0);
    /*p04.PYNE*/ DMA_A09.set(LORU_FF46_WRn, cpu_bus.TRI_D1);
    /*p04.PARA*/ DMA_A10.set(LORU_FF46_WRn, cpu_bus.TRI_D2);
    /*p04.NYDO*/ DMA_A11.set(LORU_FF46_WRn, cpu_bus.TRI_D3);
    /*p04.NYGY*/ DMA_A12.set(LORU_FF46_WRn, cpu_bus.TRI_D4);
    /*p04.PULA*/ DMA_A13.set(LORU_FF46_WRn, cpu_bus.TRI_D5);
    /*p04.POKU*/ DMA_A14.set(LORU_FF46_WRn, cpu_bus.TRI_D6);
    /*p04.MARU*/ DMA_A15.set(LORU_FF46_WRn, cpu_bus.TRI_D7);
  }


  {
    /*p04.NYGO*/ wire NYGO_FF46_RDp = not(MOLU_FF46_RDn);
    /*p04.PUSY*/ wire PUSY_FF46_RDn = not(NYGO_FF46_RDp);
    /*p04.POLY*/ cpu_bus.TRI_D0.set_tribuf(!PUSY_FF46_RDn, DMA_A08.q());
    /*p04.ROFO*/ cpu_bus.TRI_D1.set_tribuf(!PUSY_FF46_RDn, DMA_A09.q());
    /*p04.REMA*/ cpu_bus.TRI_D2.set_tribuf(!PUSY_FF46_RDn, DMA_A10.q());
    /*p04.PANE*/ cpu_bus.TRI_D3.set_tribuf(!PUSY_FF46_RDn, DMA_A11.q());
    /*p04.PARE*/ cpu_bus.TRI_D4.set_tribuf(!PUSY_FF46_RDn, DMA_A12.q());
    /*p04.RALY*/ cpu_bus.TRI_D5.set_tribuf(!PUSY_FF46_RDn, DMA_A13.q());
    /*p04.RESU*/ cpu_bus.TRI_D6.set_tribuf(!PUSY_FF46_RDn, DMA_A14.q());
    /*p04.NUVY*/ cpu_bus.TRI_D7.set_tribuf(!PUSY_FF46_RDn, DMA_A15.q());
  }

  {
    // DMA vram read
    /*p04.ECAL*/ vram_bus.TRI_A00.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A00);
    /*p04.EGEZ*/ vram_bus.TRI_A01.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A01);
    /*p04.FUHE*/ vram_bus.TRI_A02.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A02);
    /*p04.FYZY*/ vram_bus.TRI_A03.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A03);
    /*p04.DAMU*/ vram_bus.TRI_A04.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A04);
    /*p04.DAVA*/ vram_bus.TRI_A05.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A05);
    /*p04.ETEG*/ vram_bus.TRI_A06.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A06);
    /*p04.EREW*/ vram_bus.TRI_A07.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A07);
    /*p04.EVAX*/ vram_bus.TRI_A08.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A08);
    /*p04.DUVE*/ vram_bus.TRI_A09.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A09);
    /*p04.ERAF*/ vram_bus.TRI_A10.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A10);
    /*p04.FUSY*/ vram_bus.TRI_A11.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A11);
    /*p04.EXYF*/ vram_bus.TRI_A12.set_tribuf(bus_sig.AHOC_DMA_VRAM_RDn, DMA_A12);
  }
}

//-----------------------------------------------------------------------------

bool DmaRegisters::commit() {
  bool changed = false;
  /*p04.MATU*/ changed |= MATU_DMA_OAM_WRp.commit_reg(); // -> p25,p28
  /*p04.MYTE*/ changed |= MYTE_DMA_DONE.commit_reg();
  /*p04.LUVY*/ changed |= LUVY_DMA_TRIG_d0.commit_reg();
  /*p04.LENE*/ changed |= LENE_DMA_TRIG_d4.commit_reg();
  /*p04.NAKY*/ changed |= DMA_A00.commit_reg();
  /*p04.PYRO*/ changed |= DMA_A01.commit_reg(); 
  /*p04.NEFY*/ changed |= DMA_A02.commit_reg(); 
  /*p04.MUTY*/ changed |= DMA_A03.commit_reg(); 
  /*p04.NYKO*/ changed |= DMA_A04.commit_reg(); 
  /*p04.PYLO*/ changed |= DMA_A05.commit_reg(); 
  /*p04.NUTO*/ changed |= DMA_A06.commit_reg(); 
  /*p04.MUGU*/ changed |= DMA_A07.commit_reg(); 
  /*p04.NAFA*/ changed |= DMA_A08.commit_reg(); 
  /*p04.PYNE*/ changed |= DMA_A09.commit_reg(); 
  /*p04.PARA*/ changed |= DMA_A10.commit_reg(); 
  /*p04.NYDO*/ changed |= DMA_A11.commit_reg(); 
  /*p04.NYGY*/ changed |= DMA_A12.commit_reg(); 
  /*p04.PULA*/ changed |= DMA_A13.commit_reg(); 
  /*p04.POKU*/ changed |= DMA_A14.commit_reg(); 
  /*p04.MARU*/ changed |= DMA_A15.commit_reg(); 
  /*p04.LYXE*/ changed |= LYXE_DMA_LATCHn.commit_latch();

  // NAND latch
  /*p04.LARA*/ changed |= LARA_DMA_LATCHn.commit_gate();
  /*p04.LOKY*/ changed |= LOKY_DMA_LATCHp.commit_gate();

  return changed;
}

//-----------------------------------------------------------------------------


#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- DMA REG -----\n");
  //FROM_CPU5_SYNC.dump(text_painter, "FROM_CPU5_SYNC   ");
  MATU_DMA_OAM_WRp.dump(text_painter, "DMA_RUNNING  ");
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