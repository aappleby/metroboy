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

void DmaRegisters::tick(SchematicTop& top) {
  
  /*p01.ALUR*/ nwire ALUR_RSTn = not(top.AVOR_RSTp());
  /*p01.DULA*/ pwire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ nwire CUNU_RSTn = not(DULA_RSTp);

  PSignal MOLU_FF46_RDp;
  PSignal LAVY_FF46_WRp;
  {
    /*p22.XOLA*/ nwire XOLA_A00n = not(top.CPU_PIN_A00);
    /*p22.XENO*/ nwire XENO_A01n = not(top.CPU_PIN_A01);
    /*p22.XUSY*/ nwire XUSY_A02n = not(top.CPU_PIN_A02);
    /*p22.XERA*/ nwire XERA_A03n = not(top.CPU_PIN_A03);

    /*p22.WESA*/ pwire WESA_A01p = not(XENO_A01n);
    /*p22.WALO*/ pwire WALO_A02p = not(XUSY_A02n);

    /*p22.WATE*/ nwire WATE_FF46n = nand(top.WERO_FF4Xp(), XOLA_A00n, WESA_A01p, WALO_A02p, XERA_A03n);
    /*p22.XEDA*/ pwire XEDA_FF46p = not(WATE_FF46n);
    /*p07.TEDO*/ pwire TEDO_CPU_RD = not(top.UJYV_CPU_RDn());
    /*p07.AJAS*/ nwire AJAS_CPU_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ pwire ASOT_CPU_RD = not(AJAS_CPU_RD);
    /*p04.MOLU*/ MOLU_FF46_RDp = and(XEDA_FF46p, ASOT_CPU_RD);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p04.LAVY*/ LAVY_FF46_WRp = and (XEDA_FF46p, CUPA_CPU_WR_xxxxxFGH);
  }

  {
    /*p04.LOKO*/ wire LOKO_DMA_RSTp = nand(CUNU_RSTn, !LENE_DMA_TRIG_d4.q());

    /*p04.LYXE*/ LYXE_DMA_LATCHn.nor_latch(LOKO_DMA_RSTp, LAVY_FF46_WRp);
    /*p04.LUPA*/ wire LUPA_DMA_TRIG = nor((pwire)LAVY_FF46_WRp, LYXE_DMA_LATCHn.q());

    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(top.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!top.AFUR_xBCDExxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);

    /*p04.LUVY*/ LUVY_DMA_TRIG_d0.set(UVYT_xBCDExxx, CUNU_RSTn, LUPA_DMA_TRIG);
    /*p04.LENE*/ LENE_DMA_TRIG_d4.set(MOPA_AxxxxFGH, CUNU_RSTn, LUVY_DMA_TRIG_d0.q());
  }

  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(top.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!top.AFUR_xBCDExxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);

    // NAND latch
    /*p04.LOKY*/ LOKY_DMA_LATCHp = nand(LARA_DMA_LATCHn, !LENE_DMA_TRIG_d4.q());
    /*p04.LARA*/ LARA_DMA_LATCHn = nand(LOKY_DMA_LATCHp, CUNU_RSTn, !MYTE_DMA_DONE.q());
    /*p04.MATU*/ MATU_DMA_RUNNINGp.set(UVYT_xBCDExxx, CUNU_RSTn, LOKY_DMA_LATCHp);
  }

  {
    /*p04.LOKO*/ wire LOKO_DMA_RSTp = nand(CUNU_RSTn, !LENE_DMA_TRIG_d4.q());

    /*p04.LAPA*/ wire LAPA_DMA_RSTn = not(LOKO_DMA_RSTp);
    /*p04.NAVO*/ wire NAVO_DMA_DONEn = nand(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A07.q()); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire NOLO_DMA_DONEp = not(NAVO_DMA_DONEn);

    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(top.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!top.AFUR_xBCDExxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
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
    /*p04.MUHO*/ nwire MUHO_DMA_READ_VRAMn   = nand(top.MATU_DMA_RUNNINGp(), top.MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ pwire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p04.AHOC*/ nwire AHOC_DMA_VRAM_RDn = not(LUFA_DMA_READ_VRAMp);

    /*p04.ECAL*/ top.VRM_TRI_A00.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A00());
    /*p04.EGEZ*/ top.VRM_TRI_A01.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A01());
    /*p04.FUHE*/ top.VRM_TRI_A02.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A02());
    /*p04.FYZY*/ top.VRM_TRI_A03.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A03());
    /*p04.DAMU*/ top.VRM_TRI_A04.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A04());
    /*p04.DAVA*/ top.VRM_TRI_A05.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A05());
    /*p04.ETEG*/ top.VRM_TRI_A06.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A06());
    /*p04.EREW*/ top.VRM_TRI_A07.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A07());
    /*p04.EVAX*/ top.VRM_TRI_A08.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A08());
    /*p04.DUVE*/ top.VRM_TRI_A09.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A09());
    /*p04.ERAF*/ top.VRM_TRI_A10.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A10());
    /*p04.FUSY*/ top.VRM_TRI_A11.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A11());
    /*p04.EXYF*/ top.VRM_TRI_A12.set_tribuf_6n(AHOC_DMA_VRAM_RDn, top.DMA_A12());
  }

  // FF46 DMA
  {
    /*p04.NYGO*/ nwire NYGO_FF46_RDn = not(MOLU_FF46_RDp);
    /*p04.PUSY*/ pwire PUSY_FF46_RDp = not(NYGO_FF46_RDn);

    // FIXME polarity
    /*p04.POLY*/ top.CPU_TRI_D0.set_tribuf_6p(PUSY_FF46_RDp, DMA_A08);
    /*p04.ROFO*/ top.CPU_TRI_D1.set_tribuf_6p(PUSY_FF46_RDp, DMA_A09);
    /*p04.REMA*/ top.CPU_TRI_D2.set_tribuf_6p(PUSY_FF46_RDp, DMA_A10);
    /*p04.PANE*/ top.CPU_TRI_D3.set_tribuf_6p(PUSY_FF46_RDp, DMA_A11);
    /*p04.PARE*/ top.CPU_TRI_D4.set_tribuf_6p(PUSY_FF46_RDp, DMA_A12);
    /*p04.RALY*/ top.CPU_TRI_D5.set_tribuf_6p(PUSY_FF46_RDp, DMA_A13);
    /*p04.RESU*/ top.CPU_TRI_D6.set_tribuf_6p(PUSY_FF46_RDp, DMA_A14);
    /*p04.NUVY*/ top.CPU_TRI_D7.set_tribuf_6p(PUSY_FF46_RDp, DMA_A15);

    /*p04.LORU*/ nwire LORU_FF46_WRn = not(LAVY_FF46_WRp);
    /*p??.PYSU*/ pwire PYSU_FF46_WRp = not(LORU_FF46_WRn); // not on schematic

    /*p04.NAFA*/ DMA_A08.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D0);
    /*p04.PYNE*/ DMA_A09.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D1);
    /*p04.PARA*/ DMA_A10.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D2);
    /*p04.NYDO*/ DMA_A11.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D3);
    /*p04.NYGY*/ DMA_A12.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D4);
    /*p04.PULA*/ DMA_A13.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D5);
    /*p04.POKU*/ DMA_A14.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D6);
    /*p04.MARU*/ DMA_A15.set(LORU_FF46_WRn.as_pwire(), PYSU_FF46_WRp.as_nwire(), top.CPU_TRI_D7);
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