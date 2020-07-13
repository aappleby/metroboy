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

// Die trace:
// LEBU = not(MARU06)
// MUDA = nor(PULA06, POKU06, LEBU);

// if rung 6 of MARU/PULA/POKU was QN:
// MUDA = and(A13, A14, !A15);
// would select last quarter of ROM, which doesn't make sense
// so rung 6 of MARU must be Q.

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

#endif

//------------------------------------------------------------------------------

void DmaRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void DmaRegisters::tock(SchematicTop& top) {
  
  /*p22.WATE*/ wire _WATE_FF46n = nand(top.WERO_FF4Xp(), top.XOLA_A00n(), top.WESA_A01p(), top.WALO_A02p(), top.XERA_A03n());
  /*p22.XEDA*/ wire _XEDA_FF46p = not(_WATE_FF46n);
  /*p04.MOLU*/ wire _MOLU_FF46_RDp = and(_XEDA_FF46p, top.ASOT_CPU_RDp());
  /*p04.LAVY*/ wire _LAVY_FF46_WRp = and(_XEDA_FF46p, top.CUPA_CPU_WRp_xxxxEFGx());
  /*p04.LOKO*/ wire _LOKO_DMA_RSTp = nand(top.rst_reg.CUNU_SYS_RSTn(), !LENE_DMA_TRIG_d4.q());

  {
    /*p04.LYXE*/ LYXE_DMA_LATCHn.nor_latch(_LOKO_DMA_RSTp, _LAVY_FF46_WRp);
    /*p04.LUPA*/ wire _LUPA_DMA_TRIG = nor(_LAVY_FF46_WRp, LYXE_DMA_LATCHn.q());
    /*p04.LUVY*/ LUVY_DMA_TRIG_d0.set(top.clk_reg.UVYT_ABCDxxxx(), top.rst_reg.CUNU_SYS_RSTn(), _LUPA_DMA_TRIG);
    /*p04.LENE*/ LENE_DMA_TRIG_d4.set(top.clk_reg.MOPA_xxxxEFGH(), top.rst_reg.CUNU_SYS_RSTn(), LUVY_DMA_TRIG_d0.q());
  }

  {
    // NAND latch
    /*p04.LOKY*/ LOKY_DMA_LATCHp = nand(LARA_DMA_LATCHn, !LENE_DMA_TRIG_d4.q());
    /*p04.LARA*/ LARA_DMA_LATCHn = nand(LOKY_DMA_LATCHp,    top.rst_reg.CUNU_SYS_RSTn(), !MYTE_DMA_DONE.q());
    /*p04.MATU*/ _MATU_DMA_RUNNINGp.set(top.clk_reg.UVYT_ABCDxxxx(), top.rst_reg.CUNU_SYS_RSTn(), LOKY_DMA_LATCHp);
  }

  {
    /*p04.LAPA*/ wire _LAPA_DMA_RSTn = not(_LOKO_DMA_RSTp);
    /*p04.NAVO*/ wire _NAVO_DMA_DONEn = nand(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A07.q()); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire _NOLO_DMA_DONEp = not(_NAVO_DMA_DONEn);

    /*p04.MYTE*/ MYTE_DMA_DONE.set(top.clk_reg.MOPA_xxxxEFGH(), _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);

    /*p04.META*/ wire _META_DMA_CLKp = and(top.clk_reg.UVYT_ABCDxxxx(), LOKY_DMA_LATCHp);
    /*p04.NAKY*/ DMA_A00.set(_META_DMA_CLKp, _LAPA_DMA_RSTn, !DMA_A00.q());
    /*p04.PYRO*/ DMA_A01.set(DMA_A00.qn(),  _LAPA_DMA_RSTn, DMA_A01.qn());
    /*p04.NEFY*/ DMA_A02.set(DMA_A01.qn(),  _LAPA_DMA_RSTn, DMA_A02.qn());
    /*p04.MUTY*/ DMA_A03.set(DMA_A02.qn(),  _LAPA_DMA_RSTn, DMA_A03.qn());
    /*p04.NYKO*/ DMA_A04.set(DMA_A03.qn(),  _LAPA_DMA_RSTn, DMA_A04.qn());
    /*p04.PYLO*/ DMA_A05.set(DMA_A04.qn(),  _LAPA_DMA_RSTn, DMA_A05.qn());
    /*p04.NUTO*/ DMA_A06.set(DMA_A05.qn(),  _LAPA_DMA_RSTn, DMA_A06.qn());
    /*p04.MUGU*/ DMA_A07.set(DMA_A06.qn(),  _LAPA_DMA_RSTn, DMA_A07.qn());
  }

  {
    // DMA vram read
    /*p04.ECAL*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A00.q());
    /*p04.EGEZ*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A01.q());
    /*p04.FUHE*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A02.q());
    /*p04.FYZY*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A03.q());
    /*p04.DAMU*/ top.vram_bus.VRM_TRI_A04.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A04.q());
    /*p04.DAVA*/ top.vram_bus.VRM_TRI_A05.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A05.q());
    /*p04.ETEG*/ top.vram_bus.VRM_TRI_A06.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A06.q());
    /*p04.EREW*/ top.vram_bus.VRM_TRI_A07.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A07.q());
    /*p04.EVAX*/ top.vram_bus.VRM_TRI_A08.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A08.q());
    /*p04.DUVE*/ top.vram_bus.VRM_TRI_A09.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A09.q());
    /*p04.ERAF*/ top.vram_bus.VRM_TRI_A10.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A10.q());
    /*p04.FUSY*/ top.vram_bus.VRM_TRI_A11.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A11.q());
    /*p04.EXYF*/ top.vram_bus.VRM_TRI_A12.set_tribuf_6n(top.dma_reg.AHOC_DMA_VRAM_RDn(), DMA_A12.q());
  }

  // FF46 DMA
  {
    /*p04.NYGO*/ wire _NYGO_FF46_RDn = not(_MOLU_FF46_RDp);
    /*p04.PUSY*/ wire _PUSY_FF46_RDp = not(_NYGO_FF46_RDn);
    /*p04.POLY*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A08.q());
    /*p04.ROFO*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A09.q());
    /*p04.REMA*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A10.q());
    /*p04.PANE*/ top.int_bus.INT_TRI_D3.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A11.q());
    /*p04.PARE*/ top.int_bus.INT_TRI_D4.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A12.q());
    /*p04.RALY*/ top.int_bus.INT_TRI_D5.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A13.q());
    /*p04.RESU*/ top.int_bus.INT_TRI_D6.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A14.q());
    /*p04.NUVY*/ top.int_bus.INT_TRI_D7.set_tribuf_6p(_PUSY_FF46_RDp, DMA_A15.q());

    /*p04.LORU*/ wire _LORU_FF46_WRn = not(_LAVY_FF46_WRp);
    /*p??.PYSU*/ wire _PYSU_FF46_WRp = not(_LORU_FF46_WRn); // not on schematic
    /*p04.NAFA*/ DMA_A08.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D0.q());
    /*p04.PYNE*/ DMA_A09.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D1.q());
    /*p04.PARA*/ DMA_A10.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D2.q());
    /*p04.NYDO*/ DMA_A11.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D3.q());
    /*p04.NYGY*/ DMA_A12.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D4.q());
    /*p04.PULA*/ DMA_A13.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D5.q());
    /*p04.POKU*/ DMA_A14.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D6.q());
    /*p04.MARU*/ DMA_A15.set(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.int_bus.INT_TRI_D7.q());
  }

  // DMA controls OAM address if it's running.
  {
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not(MATU_DMA_RUNNINGp());
    /*p28.FODO*/ top.oam_bus.OAM_TRI_A0.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A00.q());
    /*p28.FESA*/ top.oam_bus.OAM_TRI_A1.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A01.q());
    /*p28.FAGO*/ top.oam_bus.OAM_TRI_A2.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A02.q());
    /*p28.FYKY*/ top.oam_bus.OAM_TRI_A3.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A03.q());
    /*p28.ELUG*/ top.oam_bus.OAM_TRI_A4.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A04.q());
    /*p28.EDOL*/ top.oam_bus.OAM_TRI_A5.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A05.q());
    /*p28.FYDU*/ top.oam_bus.OAM_TRI_A6.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A06.q());
    /*p28.FETU*/ top.oam_bus.OAM_TRI_A7.set_tribuf_6n(_DUGA_DMA_OAM_RDn, DMA_A07.q());
  }
}

//-----------------------------------------------------------------------------

SignalHash DmaRegisters::commit() {
  SignalHash hash;
  /*p04.MATU*/ hash << _MATU_DMA_RUNNINGp.commit(); // -> p25,p28
  /*p04.MYTE*/ hash << MYTE_DMA_DONE.commit();
  /*p04.LUVY*/ hash << LUVY_DMA_TRIG_d0.commit();
  /*p04.LENE*/ hash << LENE_DMA_TRIG_d4.commit();
  /*p04.NAKY*/ hash << DMA_A00.commit();
  /*p04.PYRO*/ hash << DMA_A01.commit(); 
  /*p04.NEFY*/ hash << DMA_A02.commit(); 
  /*p04.MUTY*/ hash << DMA_A03.commit(); 
  /*p04.NYKO*/ hash << DMA_A04.commit(); 
  /*p04.PYLO*/ hash << DMA_A05.commit(); 
  /*p04.NUTO*/ hash << DMA_A06.commit(); 
  /*p04.MUGU*/ hash << DMA_A07.commit(); 
  /*p04.NAFA*/ hash << DMA_A08.commit(); 
  /*p04.PYNE*/ hash << DMA_A09.commit(); 
  /*p04.PARA*/ hash << DMA_A10.commit(); 
  /*p04.NYDO*/ hash << DMA_A11.commit(); 
  /*p04.NYGY*/ hash << DMA_A12.commit(); 
  /*p04.PULA*/ hash << DMA_A13.commit(); 
  /*p04.POKU*/ hash << DMA_A14.commit(); 
  /*p04.MARU*/ hash << DMA_A15.commit(); 
  /*p04.LYXE*/ hash << LYXE_DMA_LATCHn.commit();

  // NAND latch
  /*p04.LARA*/ hash << LARA_DMA_LATCHn.commit();
  /*p04.LOKY*/ hash << LOKY_DMA_LATCHp.commit();

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