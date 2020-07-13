#include "Sch_BusMux.h"

#include "Sch_Top.h"

using namespace Schematics;

BusMux::BusMux() {
}

//------------------------------------------------------------------------------

void BusMux::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void BusMux::tock(SchematicTop& top) {

  // CPU controls OAM bus if there's no scanning, rendering, or dmaing.
  {
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or (top.ACYL_SCANNINGp(), top.ppu_reg.XYMU_RENDERINGp(), top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.GARO*/ top.oam_bus.OAM_TRI_A0.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A00);
    /*p28.WACU*/ top.oam_bus.OAM_TRI_A1.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A01);
    /*p28.GOSE*/ top.oam_bus.OAM_TRI_A2.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A02);
    /*p28.WAPE*/ top.oam_bus.OAM_TRI_A3.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A03);
    /*p28.FEVU*/ top.oam_bus.OAM_TRI_A4.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A04);
    /*p28.GERA*/ top.oam_bus.OAM_TRI_A5.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A05);
    /*p28.WAXA*/ top.oam_bus.OAM_TRI_A6.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A06);
    /*p28.FOBY*/ top.oam_bus.OAM_TRI_A7.set_tribuf_6n(_ASAM_CPU_OAM_RDn, top.int_bus.CPU_PIN_A07);
  }

  {
    // DMA vram -> oam
    /*p28.WUZU*/ top.oam_bus.OAM_TRI_DA0.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D0);
    /*p28.AXER*/ top.oam_bus.OAM_TRI_DA1.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D1);
    /*p28.ASOX*/ top.oam_bus.OAM_TRI_DA2.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D2);
    /*p28.CETU*/ top.oam_bus.OAM_TRI_DA3.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D3);
    /*p28.ARYN*/ top.oam_bus.OAM_TRI_DA4.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D4);
    /*p28.ACOT*/ top.oam_bus.OAM_TRI_DA5.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D5);
    /*p28.CUJE*/ top.oam_bus.OAM_TRI_DA6.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D6);
    /*p28.ATER*/ top.oam_bus.OAM_TRI_DA7.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D7);

    /*p28.WOWA*/ top.oam_bus.OAM_TRI_DB0.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D0);
    /*p28.AVEB*/ top.oam_bus.OAM_TRI_DB1.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D1);
    /*p28.AMUH*/ top.oam_bus.OAM_TRI_DB2.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D2);
    /*p28.COFO*/ top.oam_bus.OAM_TRI_DB3.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D3);
    /*p28.AZOZ*/ top.oam_bus.OAM_TRI_DB4.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D4);
    /*p28.AGYK*/ top.oam_bus.OAM_TRI_DB5.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D5);
    /*p28.BUSE*/ top.oam_bus.OAM_TRI_DB6.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D6);
    /*p28.ANUM*/ top.oam_bus.OAM_TRI_DB7.set_tribuf_6n(top.dma_reg.AZAR_DMA_VRM_TO_OAMn(), top.vram_bus.VRM_TRI_D7);
  }

  // CPU bus -> oam
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and (top.int_bus.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx());
    /*p28.WUJE*/ wire _WUJE_CPU_OAM_WRp = or (top.clk_reg.XYNY_ABCDxxxx(), _XUTO_CPU_OAM_WRp);
    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRn = not(_WUJE_CPU_OAM_WRp);
    /*p28.APAG*/ wire _APAG_CPU_OAM_WRp = amux2(_XUPA_CPU_OAM_WRn, top.AMAB_OAM_LOCKn(), top.AJUJ_OAM_BUSYn(), top.int_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CPU_OAM_WRn = not(_APAG_CPU_OAM_WRp);

    /*p28.ZAXA*/ top.oam_bus.OAM_TRI_DA0.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D0);
    /*p28.ZAKY*/ top.oam_bus.OAM_TRI_DA1.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D1);
    /*p28.WULE*/ top.oam_bus.OAM_TRI_DA2.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D2);
    /*p28.ZOZO*/ top.oam_bus.OAM_TRI_DA3.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D3);
    /*p28.ZUFO*/ top.oam_bus.OAM_TRI_DA4.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D4);
    /*p28.ZATO*/ top.oam_bus.OAM_TRI_DA5.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D5);
    /*p28.YVUC*/ top.oam_bus.OAM_TRI_DA6.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D6);
    /*p28.ZUFE*/ top.oam_bus.OAM_TRI_DA7.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D7);

    /*p28.ZAMY*/ top.oam_bus.OAM_TRI_DB0.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D0);
    /*p28.ZOPU*/ top.oam_bus.OAM_TRI_DB1.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D1);
    /*p28.WYKY*/ top.oam_bus.OAM_TRI_DB2.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D2);
    /*p28.ZAJA*/ top.oam_bus.OAM_TRI_DB3.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D3);
    /*p28.ZUGA*/ top.oam_bus.OAM_TRI_DB4.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D4);
    /*p28.ZUMO*/ top.oam_bus.OAM_TRI_DB5.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D5);
    /*p28.XYTO*/ top.oam_bus.OAM_TRI_DB6.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D6);
    /*p28.ZYFA*/ top.oam_bus.OAM_TRI_DB7.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.int_bus.INT_TRI_D7);
  }

  // CPU bus -> VRAM bus
  {
    /*p25.XEDU*/ wire _XEDU_VRAM_UNLOCKEDn = not(top.XANE_VRAM_LOCKn());

    /*p25.XAKY*/ top.vram_bus.VRM_TRI_A00.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A00);
    /*p25.XUXU*/ top.vram_bus.VRM_TRI_A01.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A01);
    /*p25.XYNE*/ top.vram_bus.VRM_TRI_A02.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A02);
    /*p25.XODY*/ top.vram_bus.VRM_TRI_A03.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A03);
    /*p25.XECA*/ top.vram_bus.VRM_TRI_A04.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A04);
    /*p25.XOBA*/ top.vram_bus.VRM_TRI_A05.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A05);
    /*p25.XOPO*/ top.vram_bus.VRM_TRI_A06.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A06);
    /*p25.XYBO*/ top.vram_bus.VRM_TRI_A07.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A07);
    /*p25.RYSU*/ top.vram_bus.VRM_TRI_A08.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A08);
    /*p25.RESE*/ top.vram_bus.VRM_TRI_A09.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A09);
    /*p25.RUSE*/ top.vram_bus.VRM_TRI_A10.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A10);
    /*p25.RYNA*/ top.vram_bus.VRM_TRI_A11.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A11);
    /*p25.RUMO*/ top.vram_bus.VRM_TRI_A12.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.int_bus.CPU_PIN_A12);
  }
}

//------------------------------------------------------------------------------

SignalHash BusMux::commit(SchematicTop& /*top*/) {
  SignalHash hash;

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_PINS -----\n");
  text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
  text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
  text_painter.dprintf("EXT_PIN_WRn_A  %d\n", EXT_PIN_WRn_A.a.val);
  text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

  text_painter.add_text("Axx   ");
  dump2(text_painter, PIN_A7.a);
  dump2(text_painter, PIN_A6.a);
  dump2(text_painter, PIN_A5.a);
  dump2(text_painter, PIN_A4.a);
  dump2(text_painter, PIN_A3.a);
  dump2(text_painter, PIN_A2.a);
  dump2(text_painter, PIN_A1.a);
  dump2(text_painter, PIN_A0.a);
  text_painter.newline();

  text_painter.add_text("A_Dx  ");
  dump2(text_painter, PIN_DA7.a);
  dump2(text_painter, PIN_DA6.a);
  dump2(text_painter, PIN_DA5.a);
  dump2(text_painter, PIN_DA4.a);
  dump2(text_painter, PIN_DA3.a);
  dump2(text_painter, PIN_DA2.a);
  dump2(text_painter, PIN_DA1.a);
  dump2(text_painter, PIN_DA0.a);
  text_painter.newline();

  text_painter.add_text("B_Dx  ");
  dump2(text_painter, PIN_DB7.a);
  dump2(text_painter, PIN_DB6.a);
  dump2(text_painter, PIN_DB5.a);
  dump2(text_painter, PIN_DB4.a);
  dump2(text_painter, PIN_DB3.a);
  dump2(text_painter, PIN_DB2.a);
  dump2(text_painter, PIN_DB1.a);
  dump2(text_painter, PIN_DB0.a);
  text_painter.newline();

  text_painter.newline();
}

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_REG -----\n");
  text_painter.dprintf("LATCH_A 0x%02x\n", pack(_XYKY_LATCH_OAM_A0.q(), _YRUM_LATCH_OAM_A1.q(), _YSEX_LATCH_OAM_A2.q(), YVEL_LATCH_OAM_A3.q(), WYNO_LATCH_OAM_A4.q(), CYRA_LATCH_OAM_A5.q(), ZUVE_LATCH_OAM_A6.q(), ECED_LATCH_OAM_A7.q()));
  text_painter.dprintf("LATCH_B 0x%02x\n", pack(YDYV_LATCH_OAM_B0.q(), YCEB_LATCH_OAM_B1.q(), ZUCA_LATCH_OAM_B2.q(), WONE_LATCH_OAM_B3.q(), ZAXE_LATCH_OAM_B4.q(), XAFU_LATCH_OAM_B5.q(), YSES_LATCH_OAM_B6.q(), ZECA_LATCH_OAM_B7.q()));
  text_painter.dprintf("REG_A   0x%02x\n", pack(YLOR_SPRITE_X0.q(), ZYTY_SPRITE_X1.q(), ZYVE_SPRITE_X2.q(), ZEZY_SPRITE_X3.q(), GOMO_SPRITE_X4.q(), BAXO_SPRITE_X5.q(), YZOS_SPRITE_X6.q(), DEPO_SPRITE_X7.q()));
  text_painter.dprintf("REG_B   0x%02x\n", pack(XUSO_SPRITE_Y0.q(), XEGU_SPRITE_Y1.q(), YJEX_SPRITE_Y2.q(), XYJU_SPRITE_Y3.q(), YBOG_SPRITE_Y4.q(), WYSO_SPRITE_Y5.q(), XOTE_SPRITE_Y6.q(), YZAB_SPRITE_Y7.q()));
  text_painter.newline();
}

#endif