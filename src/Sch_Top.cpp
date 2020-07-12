#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

SchematicTop::SchematicTop() {
  EXT_PIN_WRp_C.set(0);
  EXT_PIN_RDp_C.set(0);

  EXT_PIN_A00_C.set(0);
  EXT_PIN_A01_C.set(0);
  EXT_PIN_A02_C.set(0);
  EXT_PIN_A03_C.set(0);
  EXT_PIN_A04_C.set(0);
  EXT_PIN_A05_C.set(0);
  EXT_PIN_A06_C.set(0);
  EXT_PIN_A07_C.set(0);
  EXT_PIN_A08_C.set(0);
  EXT_PIN_A09_C.set(0);
  EXT_PIN_A10_C.set(0);
  EXT_PIN_A11_C.set(0);
  EXT_PIN_A12_C.set(0);
  EXT_PIN_A13_C.set(0);
  EXT_PIN_A14_C.set(0);
  EXT_PIN_A15_C.set(0);

  EXT_PIN_D0_C.set(0);
  EXT_PIN_D1_C.set(0);
  EXT_PIN_D2_C.set(0);
  EXT_PIN_D3_C.set(0);
  EXT_PIN_D4_C.set(0);
  EXT_PIN_D5_C.set(0);
  EXT_PIN_D6_C.set(0);
  EXT_PIN_D7_C.set(0);

  // HACK preset these so we don't read hiz
  ser_reg.SIN_C.set(0);
  ser_reg.SCK_C.set(0);

  JOY_PIN_P10_C.set(0);
  JOY_PIN_P11_C.set(0);
  JOY_PIN_P12_C.set(0);
  JOY_PIN_P13_C.set(0);

  VRAM_PIN_MCSn_C.set(0);
  VRAM_PIN_MOEn_C.set(0);
  VRAM_PIN_MWRn_C.set(0);

  CPU_PIN6.set(0);

  CPU_TRI_D0.preset_a(0);
  CPU_TRI_D1.preset_a(0);
  CPU_TRI_D2.preset_a(0);
  CPU_TRI_D3.preset_a(0);
  CPU_TRI_D4.preset_a(0);
  CPU_TRI_D5.preset_a(0);
  CPU_TRI_D6.preset_a(0);
  CPU_TRI_D7.preset_a(0);

  SPR_TRI_INDX_0.preset_a(0);
  SPR_TRI_INDX_1.preset_a(0);
  SPR_TRI_INDX_2.preset_a(0);
  SPR_TRI_INDX_3.preset_a(0);
  SPR_TRI_INDX_4.preset_a(0);
  SPR_TRI_INDX_5.preset_a(0);
  SPR_TRI_LINE_0.preset_a(0);
  SPR_TRI_LINE_1.preset_a(0);
  SPR_TRI_LINE_2.preset_a(0);
  SPR_TRI_LINE_3.preset_a(0);

  VRM_TRI_A00.preset_a(0);
  VRM_TRI_A01.preset_a(0);
  VRM_TRI_A02.preset_a(0);
  VRM_TRI_A03.preset_a(0);
  VRM_TRI_A04.preset_a(0);
  VRM_TRI_A05.preset_a(0);
  VRM_TRI_A06.preset_a(0);
  VRM_TRI_A07.preset_a(0);
  VRM_TRI_A08.preset_a(0);
  VRM_TRI_A09.preset_a(0);
  VRM_TRI_A10.preset_a(0);
  VRM_TRI_A11.preset_a(0);
  VRM_TRI_A12.preset_a(0);

  VRM_TRI_D0.preset_a(0);
  VRM_TRI_D1.preset_a(0);
  VRM_TRI_D2.preset_a(0);
  VRM_TRI_D3.preset_a(0);
  VRM_TRI_D4.preset_a(0);
  VRM_TRI_D5.preset_a(0);
  VRM_TRI_D6.preset_a(0);
  VRM_TRI_D7.preset_a(0);
}

//-----------------------------------------------------------------------------

void SchematicTop::set_cpu_bus(Req req) {

  CPU_PIN_RDp.set(req.read);
  CPU_PIN_WRp.set(req.write);

  // this probably isn't right
  CPU_PIN_ADDR_VALID.set(req.read || req.write);

  CPU_PIN_A00.set(req.addr & 0x0001);
  CPU_PIN_A01.set(req.addr & 0x0002);
  CPU_PIN_A02.set(req.addr & 0x0004);
  CPU_PIN_A03.set(req.addr & 0x0008);
  CPU_PIN_A04.set(req.addr & 0x0010);
  CPU_PIN_A05.set(req.addr & 0x0020);
  CPU_PIN_A06.set(req.addr & 0x0040);
  CPU_PIN_A07.set(req.addr & 0x0080);
  CPU_PIN_A08.set(req.addr & 0x0100);
  CPU_PIN_A09.set(req.addr & 0x0200);
  CPU_PIN_A10.set(req.addr & 0x0400);
  CPU_PIN_A11.set(req.addr & 0x0800);
  CPU_PIN_A12.set(req.addr & 0x1000);
  CPU_PIN_A13.set(req.addr & 0x2000);
  CPU_PIN_A14.set(req.addr & 0x4000);
  CPU_PIN_A15.set(req.addr & 0x8000);

  if (req.write) {
    CPU_TRI_D0.preset_a(req.data_lo & 0x01);
    CPU_TRI_D1.preset_a(req.data_lo & 0x02);
    CPU_TRI_D2.preset_a(req.data_lo & 0x04);
    CPU_TRI_D3.preset_a(req.data_lo & 0x08);
    CPU_TRI_D4.preset_a(req.data_lo & 0x10);
    CPU_TRI_D5.preset_a(req.data_lo & 0x20);
    CPU_TRI_D6.preset_a(req.data_lo & 0x40);
    CPU_TRI_D7.preset_a(req.data_lo & 0x80);

    CPU_TRI_D0.preset_b(req.data_lo & 0x01);
    CPU_TRI_D1.preset_b(req.data_lo & 0x02);
    CPU_TRI_D2.preset_b(req.data_lo & 0x04);
    CPU_TRI_D3.preset_b(req.data_lo & 0x08);
    CPU_TRI_D4.preset_b(req.data_lo & 0x10);
    CPU_TRI_D5.preset_b(req.data_lo & 0x20);
    CPU_TRI_D6.preset_b(req.data_lo & 0x40);
    CPU_TRI_D7.preset_b(req.data_lo & 0x80);
  }

  CPU_PIN_ACK_JOYPAD.set(0);
  CPU_PIN_ACK_SERIAL.set(0);
  CPU_PIN_ACK_STAT.set(0);
  CPU_PIN_ACK_TIMER.set(0);
  CPU_PIN_ACK_VBLANK.set(0);
}

//-----------------------------------------------------------------------------

void SchematicTop::set_buttons(uint8_t buttons) {
  if (joy_reg.KELY_JOYP_UDLR.q()) {
    JOY_PIN_P10_C.set(buttons & 0x01);
    JOY_PIN_P11_C.set(buttons & 0x02);
    JOY_PIN_P12_C.set(buttons & 0x04);
    JOY_PIN_P13_C.set(buttons & 0x08);
  }
  else if (joy_reg.COFY_JOYP_ABCS.q()) {
    JOY_PIN_P10_C.set(buttons & 0x10);
    JOY_PIN_P11_C.set(buttons & 0x20);
    JOY_PIN_P12_C.set(buttons & 0x40);
    JOY_PIN_P13_C.set(buttons & 0x80);
  }
  else {
    JOY_PIN_P10_C.set(1);
    JOY_PIN_P11_C.set(1);
    JOY_PIN_P12_C.set(1);
    JOY_PIN_P13_C.set(1);
  }
}

//-----------------------------------------------------------------------------

void SchematicTop::set_vram_bus(uint8_t data) {
  VRAM_PIN_MD0_C.set(data & 0x01);
  VRAM_PIN_MD1_C.set(data & 0x02);
  VRAM_PIN_MD2_C.set(data & 0x04);
  VRAM_PIN_MD3_C.set(data & 0x08);
  VRAM_PIN_MD4_C.set(data & 0x10);
  VRAM_PIN_MD5_C.set(data & 0x20);
  VRAM_PIN_MD6_C.set(data & 0x40);
  VRAM_PIN_MD7_C.set(data & 0x80);
}

//-----------------------------------------------------------------------------

void SchematicTop::set_oam_bus(wire OE, uint16_t data) {
  if (OE) {
    bus_mux.OAM_PIN_DA0.preset_a(data & 0x0001);
    bus_mux.OAM_PIN_DA1.preset_a(data & 0x0002);
    bus_mux.OAM_PIN_DA2.preset_a(data & 0x0004);
    bus_mux.OAM_PIN_DA3.preset_a(data & 0x0008);
    bus_mux.OAM_PIN_DA4.preset_a(data & 0x0010);
    bus_mux.OAM_PIN_DA5.preset_a(data & 0x0020);
    bus_mux.OAM_PIN_DA6.preset_a(data & 0x0040);
    bus_mux.OAM_PIN_DA7.preset_a(data & 0x0080);

    bus_mux.OAM_PIN_DB0.preset_a(data & 0x0100);
    bus_mux.OAM_PIN_DB1.preset_a(data & 0x0200);
    bus_mux.OAM_PIN_DB2.preset_a(data & 0x0400);
    bus_mux.OAM_PIN_DB3.preset_a(data & 0x0800);
    bus_mux.OAM_PIN_DB4.preset_a(data & 0x1000);
    bus_mux.OAM_PIN_DB5.preset_a(data & 0x2000);
    bus_mux.OAM_PIN_DB6.preset_a(data & 0x4000);
    bus_mux.OAM_PIN_DB7.preset_a(data & 0x8000);
  }
  else {
    bus_mux.OAM_PIN_DA0.preset_a(HIZ);
    bus_mux.OAM_PIN_DA1.preset_a(HIZ);
    bus_mux.OAM_PIN_DA2.preset_a(HIZ);
    bus_mux.OAM_PIN_DA3.preset_a(HIZ);
    bus_mux.OAM_PIN_DA4.preset_a(HIZ);
    bus_mux.OAM_PIN_DA5.preset_a(HIZ);
    bus_mux.OAM_PIN_DA6.preset_a(HIZ);
    bus_mux.OAM_PIN_DA7.preset_a(HIZ);

    bus_mux.OAM_PIN_DB0.preset_a(HIZ);
    bus_mux.OAM_PIN_DB1.preset_a(HIZ);
    bus_mux.OAM_PIN_DB2.preset_a(HIZ);
    bus_mux.OAM_PIN_DB3.preset_a(HIZ);
    bus_mux.OAM_PIN_DB4.preset_a(HIZ);
    bus_mux.OAM_PIN_DB5.preset_a(HIZ);
    bus_mux.OAM_PIN_DB6.preset_a(HIZ);
    bus_mux.OAM_PIN_DB7.preset_a(HIZ);
  }
}

//-----------------------------------------------------------------------------

void SchematicTop::set_ext_bus(uint16_t data) {
  EXT_PIN_D0_C.set(data & 0x01);
  EXT_PIN_D1_C.set(data & 0x02);
  EXT_PIN_D2_C.set(data & 0x04);
  EXT_PIN_D3_C.set(data & 0x08);
  EXT_PIN_D4_C.set(data & 0x10);
  EXT_PIN_D5_C.set(data & 0x20);
  EXT_PIN_D6_C.set(data & 0x40);
  EXT_PIN_D7_C.set(data & 0x80);
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::tick() {
  SignalHash hash;

  bool verbose = false;

  if (verbose) printf("SchematicTop::tick\n");

  clk_reg.tick(*this);
  clk_reg.tock(*this);

  dbg_reg.tick(*this);
  dbg_reg.tock(*this);

  rst_reg.tick(*this);
  rst_reg.tock(*this);

  tim_reg.tick(*this);
  tim_reg.tock(*this);

  bootrom.tick(*this);
  bootrom.tock(*this);

  dma_reg.tick(*this);
  dma_reg.tock(*this);

  ser_reg.tick(*this);
  ser_reg.tock(*this);

  joy_reg.tick(*this);
  joy_reg.tock(*this);

  sprite_scanner.tick(*this);
  sprite_scanner.tock(*this);

  lcd_reg.tick(*this);
  lcd_reg.tock(*this);

  bus_mux.tick(*this);
  bus_mux.tock(*this);

  sprite_store.tick(*this);  // after bus mux
  sprite_store.tock(*this);  // after bus mux

  win_reg.tick(*this); // after sprite store
  win_reg.tock(*this); // after sprite store

  ppu_reg.tick(*this); // after window
  ppu_reg.tock(*this); // after window

  tick_vram_pins(); // before sprite fetcher

  sprite_fetcher.tick(*this); // after window
  sprite_fetcher.tock(*this); // after window

  pxp_reg.tick(*this); // after window
  pxp_reg.tock(*this); // after window

  tile_fetcher.tick(*this); // after window
  tile_fetcher.tock(*this); // after window

  int_reg.tick(*this);
  int_reg.tock(*this);

  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
    /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);
    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(BUTY_CLKREQ, BALY_Axxxxxxx());
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BEDO*/ wire BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
    /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);
    CPU_PIN_BOWA_AxCDEFGH.set(BOWA_xBCDEFGH);
    CPU_PIN_BEDO_xBxxxxxx.set(BEDO_Axxxxxxx);
  }
    
  CPU_PIN_BEKO_xBCDExxx.set(BEKO_ABCDxxxx());
  CPU_PIN_BUDE_AxxxxFGH.set(BUDE_xxxxEFGH());
  CPU_PIN_BOLO_xBCDEFGx.set(BOLO_ABCDEFxx());
    
  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(clk_reg.APUK_xxCDEFxx.qn());
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not(clk_reg.ALEF_xBCDExxx.q());
    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(ABOL_CLKREQn,
                                          BUGO_xBCDExxx,
                                          AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
    CPU_PIN_BUKE_ABxxxxxH.set(BUKE_AxxxxxGH);
  }

  CPU_PIN_BOMA_xBxxxxxx.set(BOMA_Axxxxxxx());
  CPU_PIN_BOGA_AxCDEFGH.set(BOGA_xBCDEFGH());

  {
    wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_ABCDxxxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /* PIN_75 */ EXT_PIN_CLK.set(BUDE_AxxxxFGH);
  }

  CPU_PIN_EXT_RST.set(SYS_PIN_RSTp);
  CPU_PIN_EXT_CLKGOOD.set(SYS_PIN_CLK_A);

  {
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    CPU_PIN_ADDR_HI.set(SYRO_FE00_FFFFp);
  }

  //----------
  // FF40 LCDC
  {
    /*p22.WORU*/ wire WORU_FF40n = nand(WERO_FF4Xp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n());
    /*p22.VOCA*/ wire VOCA_FF40p = not(WORU_FF40n);

    /*p23.VYRE*/ wire VYRE_FF40_RDp = and (VOCA_FF40p, ASOT_CPU_RDp());
    /*p23.WYCE*/ wire WYCE_FF40_RDn = not(VYRE_FF40_RDp);

    /*p23.WYPO*/ CPU_TRI_D0.set_tribuf_6n(WYCE_FF40_RDn, VYXE_LCDC_BGEN.q());
    /*p23.XERO*/ CPU_TRI_D1.set_tribuf_6n(WYCE_FF40_RDn, XYLO_LCDC_SPEN.q());
    /*p23.WYJU*/ CPU_TRI_D2.set_tribuf_6n(WYCE_FF40_RDn, XYMO_LCDC_SPSIZE.q());
    /*p23.WUKA*/ CPU_TRI_D3.set_tribuf_6n(WYCE_FF40_RDn, XAFO_LCDC_BGMAP.q());
    /*p23.VOKE*/ CPU_TRI_D4.set_tribuf_6n(WYCE_FF40_RDn, WEXU_LCDC_BGTILE.q());
    /*p23.VATO*/ CPU_TRI_D5.set_tribuf_6n(WYCE_FF40_RDn, WYMO_LCDC_WINEN.q());
    /*p23.VAHA*/ CPU_TRI_D6.set_tribuf_6n(WYCE_FF40_RDn, WOKY_LCDC_WINMAP.q());
    /*p23.XEBU*/ CPU_TRI_D7.set_tribuf_6n(WYCE_FF40_RDn, XONA_LCDC_EN.q());

    /*p23.WARU*/ wire WARU_FF40_WRp = and (VOCA_FF40p, CUPA_CPU_WRp_xxxxEFGx());
    /*p23.XUBO*/ wire XUBO_FF40_WRn = not(WARU_FF40_WRp);

    /*p01.XARE*/ wire XARE_RSTn = not(XORE_SYS_RSTp());
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D0.q());
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D1.q());
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D2.q());
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D3.q());
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D4.q());
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D5.q());
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D6.q());
    /*p23.XONA*/ XONA_LCDC_EN     .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D7.q());
  }

  //----------

  if (verbose) printf("SchematicTop::commit\n");

  hash << clk_reg.commit();
  hash << dbg_reg.commit();
  hash << rst_reg.commit();
  hash << tim_reg.commit();
  hash << bootrom.commit();
  hash << dma_reg.commit();
  hash << ser_reg.commit();
  hash << joy_reg.commit(*this);
  hash << sprite_scanner.commit();
  hash << lcd_reg.commit(*this);
  hash << bus_mux.commit(*this);
 
  hash << sprite_store.commit(*this); // after bus mux
  hash << win_reg.commit(); // after sprite store
  hash << ppu_reg.commit(*this); // after sprite store
  hash << sprite_fetcher.commit(); // after window
  hash << pxp_reg.commit(*this); // after window
  hash << tile_fetcher.commit(*this); // after window
  hash << int_reg.commit();

  hash << SYS_PIN_RSTp.commit();
  hash << SYS_PIN_CLK_B.commit();
  hash << SYS_PIN_CLK_A.commit();
  hash << SYS_PIN_T2n.commit();
  hash << SYS_PIN_T1n.commit();

  //----------------------------------------
  // SOC-to-CPU

  hash << CPU_PIN_SYS_RSTp.commit();
  hash << CPU_PIN_EXT_RST.commit();       // PORTC_02: <- PIN_RESET directly connected to the pad
  hash << CPU_PIN_EXT_CLKGOOD.commit();   // PORTC_03: <- CLKIN_A
  hash << CPU_PIN_STARTp.commit();
  hash << CPU_PIN_BOOTp.commit();         // PORTA_04: <- TUTU
  hash << CPU_PIN_ADDR_HI.commit();       // PORTA_03: <- SYRO

  hash << CPU_PIN_BOWA_AxCDEFGH.commit(); // PORTD_01: <- BOWA
  hash << CPU_PIN_BEDO_xBxxxxxx.commit(); // PORTD_02: <- BEDO _____fgh
  hash << CPU_PIN_BEKO_xBCDExxx.commit(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  hash << CPU_PIN_BUDE_AxxxxFGH.commit(); // PORTD_04: <- BUDE abcd____
  hash << CPU_PIN_BOLO_xBCDEFGx.commit(); // PORTD_05: <- BOLO
  hash << CPU_PIN_BUKE_ABxxxxxH.commit(); // PORTD_07: <- BUKE _____f__
  hash << CPU_PIN_BOMA_xBxxxxxx.commit(); // PORTD_08: <- BOMA _____fgh
  hash << CPU_PIN_BOGA_AxCDEFGH.commit(); // PORTD_09: <- BOGA abcde___

  //----------------------------------------
  // CPU-to-SOC

  hash << CPU_PIN_READYp.commit();         // PORTC_00: -> ABOL
  hash << CPU_PIN6.commit();               // PORTD_00: -> LEXY, doesn't do anything
  hash << CPU_PIN5.commit();               // PORTD_06: -> FROM_CPU5

  hash << CPU_PIN_ACK_VBLANK.commit();     // PORTB_01: -> LETY, vblank int ack
  hash << CPU_PIN_ACK_STAT.commit();       // PORTB_05: -> LEJA, stat int ack
  hash << CPU_PIN_ACK_TIMER.commit();      // PORTB_09: -> LESA, timer int ack
  hash << CPU_PIN_ACK_SERIAL.commit();     // PORTB_13: -> LUFE, serial int ack
  hash << CPU_PIN_ACK_JOYPAD.commit();     // PORTB_17: -> LAMO, joypad int ack

  hash << CPU_PIN_RDp.commit();            // PORTA_00: -> UJYV, LAGU, LAVO
  hash << CPU_PIN_WRp.commit();            // PORTA_01: -> AREV, LAGU.
  hash << CPU_PIN_ADDR_VALID.commit();     // PORTA_06: -> APAP, TEXO

  hash << CPU_PIN_A00.commit();
  hash << CPU_PIN_A01.commit();
  hash << CPU_PIN_A02.commit();
  hash << CPU_PIN_A03.commit();
  hash << CPU_PIN_A04.commit();
  hash << CPU_PIN_A05.commit();
  hash << CPU_PIN_A06.commit();
  hash << CPU_PIN_A07.commit();
  hash << CPU_PIN_A08.commit();
  hash << CPU_PIN_A09.commit();
  hash << CPU_PIN_A10.commit();
  hash << CPU_PIN_A11.commit();
  hash << CPU_PIN_A12.commit();
  hash << CPU_PIN_A13.commit();
  hash << CPU_PIN_A14.commit();
  hash << CPU_PIN_A15.commit();

  hash << CPU_TRI_D0.commit();
  hash << CPU_TRI_D1.commit();
  hash << CPU_TRI_D2.commit();
  hash << CPU_TRI_D3.commit();
  hash << CPU_TRI_D4.commit();
  hash << CPU_TRI_D5.commit();
  hash << CPU_TRI_D6.commit();
  hash << CPU_TRI_D7.commit();

  //----------------------------------------

  hash << VRAM_PIN_MCSn_C.commit();   // PIN_43 -> TEFY
  hash << VRAM_PIN_MOEn_C.commit();   // PIN_45 -> TAVY
  hash << VRAM_PIN_MWRn_C.commit();   // PIN_49 -> SUDOs

  hash << VRAM_PIN_MD0_C.commit();    // PIN_33 -> RODY
  hash << VRAM_PIN_MD1_C.commit();    // PIN_31 -> REBA
  hash << VRAM_PIN_MD2_C.commit();    // PIN_30 -> RYDO
  hash << VRAM_PIN_MD3_C.commit();    // PIN_29 -> REMO
  hash << VRAM_PIN_MD4_C.commit();    // PIN_28 -> ROCE
  hash << VRAM_PIN_MD5_C.commit();    // PIN_27 -> ROPU
  hash << VRAM_PIN_MD6_C.commit();    // PIN_26 -> RETA
  hash << VRAM_PIN_MD7_C.commit();    // PIN_25 -> RAKU

  //----------------------------------------

  hash << EXT_PIN_RDp_C.commit();     // PIN_79 -> UJYV
  hash << EXT_PIN_WRp_C.commit();     // PIN_78 -> UBAL

  hash << EXT_PIN_A00_C.commit();     // PIN_01 -> KOVA
  hash << EXT_PIN_A01_C.commit();     // PIN_02 -> CAMU
  hash << EXT_PIN_A02_C.commit();     // PIN_03 -> BUXU
  hash << EXT_PIN_A03_C.commit();     // PIN_04 -> BASE
  hash << EXT_PIN_A04_C.commit();     // PIN_05 -> AFEC
  hash << EXT_PIN_A05_C.commit();     // PIN_06 -> ABUP
  hash << EXT_PIN_A06_C.commit();     // PIN_07 -> CYGU
  hash << EXT_PIN_A07_C.commit();     // PIN_08 -> COGO
  hash << EXT_PIN_A08_C.commit();     // PIN_09 -> MUJY
  hash << EXT_PIN_A09_C.commit();     // PIN_10 -> NENA
  hash << EXT_PIN_A10_C.commit();     // PIN_11 -> SURA
  hash << EXT_PIN_A11_C.commit();     // PIN_12 -> MADY
  hash << EXT_PIN_A12_C.commit();     // PIN_13 -> LAHE
  hash << EXT_PIN_A13_C.commit();     // PIN_14 -> LURA
  hash << EXT_PIN_A14_C.commit();     // PIN_15 -> PEVO
  hash << EXT_PIN_A15_C.commit();     // PIN_16 -> RAZA

  hash << EXT_PIN_D0_C.commit();      // PIN_17 -> TOVO,SOMA
  hash << EXT_PIN_D1_C.commit();      // PIN_18 -> RUZY,RONY
  hash << EXT_PIN_D2_C.commit();      // PIN_19 -> ROME,RAXY
  hash << EXT_PIN_D3_C.commit();      // PIN_20 -> SAZA,SELO
  hash << EXT_PIN_D4_C.commit();      // PIN_21 -> TEHE,SODY
  hash << EXT_PIN_D5_C.commit();      // PIN_22 -> RATU,SAGO
  hash << EXT_PIN_D6_C.commit();      // PIN_23 -> SOCA,RUPA
  hash << EXT_PIN_D7_C.commit();      // PIN_24 -> RYBA,SAZY

  //----------------------------------------

  hash << JOY_PIN_P10_C.commit();     // PIN_67-> KERY, KEVU
  hash << JOY_PIN_P11_C.commit();     // PIN_66-> KERY, P05.KAPA
  hash << JOY_PIN_P12_C.commit();     // PIN_65-> KERY, P05.KEJA
  hash << JOY_PIN_P13_C.commit();     // PIN_64-> KERY, P05.KOLO


  hash << VRM_TRI_A00.commit();
  hash << VRM_TRI_A01.commit();
  hash << VRM_TRI_A02.commit();
  hash << VRM_TRI_A03.commit();
  hash << VRM_TRI_A04.commit();
  hash << VRM_TRI_A05.commit();
  hash << VRM_TRI_A06.commit();
  hash << VRM_TRI_A07.commit();
  hash << VRM_TRI_A08.commit();
  hash << VRM_TRI_A09.commit();
  hash << VRM_TRI_A10.commit();
  hash << VRM_TRI_A11.commit();
  hash << VRM_TRI_A12.commit();

  hash << EXT_PIN_CLK.commit();      // PIN_75 <- BUDE/BEVA

  hash << VRAM_PIN_MCSn_A.commit();   // PIN_43 <- SOKY
  hash << VRAM_PIN_MCSn_D.commit();   // PIN_43 <- SETY

  hash << VRAM_PIN_MOEn_A.commit();   // PIN_45 <- REFO
  hash << VRAM_PIN_MOEn_D.commit();   // PIN_45 <- SAHA

  hash << VRAM_PIN_MWRn_A.commit();   // PIN_49 <- SYSY
  hash << VRAM_PIN_MWRn_D.commit();   // PIN_49 <- RAGU

  hash << VRAM_PIN_MA00_AD.commit();  // PIN_34 <- ECAL
  hash << VRAM_PIN_MA01_AD.commit();  // PIN_35 <- EGEZ
  hash << VRAM_PIN_MA02_AD.commit();  // PIN_36 <- FUHE
  hash << VRAM_PIN_MA03_AD.commit();  // PIN_37 <- FYZY
  hash << VRAM_PIN_MA04_AD.commit();  // PIN_38 <- DAMU
  hash << VRAM_PIN_MA05_AD.commit();  // PIN_39 <- DAVA
  hash << VRAM_PIN_MA06_AD.commit();  // PIN_40 <- ETEG
  hash << VRAM_PIN_MA07_AD.commit();  // PIN_41 <- EREW
  hash << VRAM_PIN_MA08_AD.commit();  // PIN_48 <- EVAX
  hash << VRAM_PIN_MA09_AD.commit();  // PIN_47 <- DUVE
  hash << VRAM_PIN_MA10_AD.commit();  // PIN_44 <- ERAF
  hash << VRAM_PIN_MA11_AD.commit();  // PIN_46 <- FUSY
  hash << VRAM_PIN_MA12_AD.commit();  // PIN_42 <- EXYF

  hash << VRAM_PIN_MD0_A.commit();    // PIN_33 <- REGE
  hash << VRAM_PIN_MD0_B.commit();    // PIN_33 <- ROFA
  hash << VRAM_PIN_MD0_D.commit();    // PIN_33 <- RURA
  hash << VRAM_PIN_MD1_A.commit();    // PIN_31 <- RYKY
  hash << VRAM_PIN_MD1_B.commit();    // PIN_31 <- ROFA
  hash << VRAM_PIN_MD1_D.commit();    // PIN_31 <- RULY
  hash << VRAM_PIN_MD2_A.commit();    // PIN_30 <- RAZO
  hash << VRAM_PIN_MD2_B.commit();    // PIN_30 <- ROFA
  hash << VRAM_PIN_MD2_D.commit();    // PIN_30 <- RARE
  hash << VRAM_PIN_MD3_A.commit();    // PIN_29 <- RADA
  hash << VRAM_PIN_MD3_B.commit();    // PIN_29 <- ROFA
  hash << VRAM_PIN_MD3_D.commit();    // PIN_29 <- RODU
  hash << VRAM_PIN_MD4_A.commit();    // PIN_28 <- RYRO
  hash << VRAM_PIN_MD4_B.commit();    // PIN_28 <- ROFA
  hash << VRAM_PIN_MD4_D.commit();    // PIN_28 <- RUBE
  hash << VRAM_PIN_MD5_A.commit();    // PIN_27 <- REVU
  hash << VRAM_PIN_MD5_B.commit();    // PIN_27 <- ROFA
  hash << VRAM_PIN_MD5_D.commit();    // PIN_27 <- RUMU
  hash << VRAM_PIN_MD6_A.commit();    // PIN_26 <- REKU
  hash << VRAM_PIN_MD6_B.commit();    // PIN_26 <- ROFA
  hash << VRAM_PIN_MD6_D.commit();    // PIN_26 <- RYTY
  hash << VRAM_PIN_MD7_A.commit();    // PIN_25 <- RYZE
  hash << VRAM_PIN_MD7_B.commit();    // PIN_25 <- ROFA
  hash << VRAM_PIN_MD7_D.commit();    // PIN_25 <- RADY

  hash << VRM_TRI_D0.commit();
  hash << VRM_TRI_D1.commit();
  hash << VRM_TRI_D2.commit();
  hash << VRM_TRI_D3.commit();
  hash << VRM_TRI_D4.commit();
  hash << VRM_TRI_D5.commit();
  hash << VRM_TRI_D6.commit();
  hash << VRM_TRI_D7.commit();

  //----------
  // LCDC

  hash << VYXE_LCDC_BGEN.commit();
  hash << XYLO_LCDC_SPEN.commit();
  hash << XYMO_LCDC_SPSIZE.commit();
  hash << XAFO_LCDC_BGMAP.commit();
  hash << WEXU_LCDC_BGTILE.commit();
  hash << WYMO_LCDC_WINEN.commit();
  hash << WOKY_LCDC_WINMAP.commit();
  hash << XONA_LCDC_EN.commit();

  if (verbose) printf("SchematicTop::commit done\n");

  return hash;
}

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or(top.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, CPU_PIN_A05, CPU_PIN_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_reg.BEDO_Axxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /*p05.ANOC*/ not(top.GND),
  /*p05.ATAJ*/ not(top.GND),
  /*p05.AJEC*/ not(top.GND),
  /*p05.ASUZ*/ not(top.GND),
  /*p05.BENU*/ not(top.GND),
  /*p05.AKAJ*/ not(top.GND),
  /*p05.ARAR*/ not(top.GND),
  /*p05.BEDA*/ not(top.GND)
);

if (LECO_xBCDEFGH) set_data(
  /*p07.ROMY*/ top.GND,
  /*p07.RYNE*/ top.GND,
  /*p07.REJY*/ top.GND,
  /*p07.RASE*/ top.GND,
  /*p07.REKA*/ top.GND,
  /*p07.ROWE*/ top.GND,
  /*p07.RYKE*/ top.GND,
  /*p07.RARU*/ top.GND
);



//----------
// more debug stuff

/*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, clk.CPU_PIN_BOGA_AxCDEFGH);
/*p25.SOLE*/ wire SOLE = not(TUSO);

if (top.VYPO_GND) bus_out.set_data(
  /*p25.TOVU*/ SOLE,
  /*p25.SOSA*/ SOLE,
  /*p25.SEDU*/ SOLE,
  /*p25.TAXO*/ SOLE,
  /*p25.TAHY*/ SOLE,
  /*p25.TESU*/ SOLE,
  /*p25.TAZU*/ SOLE,
  /*p25.TEWA*/ SOLE
);

// FIXME
///*p05.KORE*/ wire P05_NC0 = nand(KERU_DBG_FF00_D7, FF60_0);
///*p05.KYWE*/ wire P05_NC1 = nor (KERU_DBG_FF00_D7, FF60_0o);

/*p08.LYRA*/ wire DBG_D_RDn = nand(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/*p08.TUTY*/ if (!DBG_D_RDn) CPU_TRI_D0 = not(/*p08.TOVO*/ not(pins.PIN_D0_C));
/*p08.SYWA*/ if (!DBG_D_RDn) CPU_TRI_D1 = not(/*p08.RUZY*/ not(pins.PIN_D1_C));
/*p08.SUGU*/ if (!DBG_D_RDn) CPU_TRI_D2 = not(/*p08.ROME*/ not(pins.PIN_D2_C));
/*p08.TAWO*/ if (!DBG_D_RDn) CPU_TRI_D3 = not(/*p08.SAZA*/ not(pins.PIN_D3_C));
/*p08.TUTE*/ if (!DBG_D_RDn) CPU_TRI_D4 = not(/*p08.TEHE*/ not(pins.PIN_D4_C));
/*p08.SAJO*/ if (!DBG_D_RDn) CPU_TRI_D5 = not(/*p08.RATU*/ not(pins.PIN_D5_C));
/*p08.TEMY*/ if (!DBG_D_RDn) CPU_TRI_D6 = not(/*p08.SOCA*/ not(pins.PIN_D6_C));
/*p08.ROPA*/ if (!DBG_D_RDn) CPU_TRI_D7 = not(/*p08.RYBA*/ not(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug reg
  /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, CPU_PIN_A05, CPU_PIN_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, CPU_TRI_D0);
  /*p07.AMUT*/ AMUT_FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, CPU_TRI_D1);

  ///*p05.KURA*/ wire FF60_0n = not(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not(FF60);
  /*p07.BURO*/ dbg_reg.BURO_FF60_0.set(1, rst_sig.ALUR_RSTn, CPU_TRI_D0);
  /*p07.AMUT*/ dbg_reg.AMUT_FF60_1.set(1, rst_sig.ALUR_RSTn, CPU_TRI_D1);
}
#endif


#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
// If we're in debug mode 2, drive external address bus onto internal address
// these may be backwards, probably don't want to drive external address onto bus normally...

/*p08.KOVA*/ wire A00_Cn = not(EXT_PIN_A00_C);
/*p08.CAMU*/ wire A01_Cn = not(EXT_PIN_A01_C);
/*p08.BUXU*/ wire A02_Cn = not(EXT_PIN_A02_C);
/*p08.BASE*/ wire A03_Cn = not(EXT_PIN_A03_C);
/*p08.AFEC*/ wire A04_Cn = not(EXT_PIN_A04_C);
/*p08.ABUP*/ wire A05_Cn = not(EXT_PIN_A05_C);
/*p08.CYGU*/ wire A06_Cn = not(EXT_PIN_A06_C);
/*p08.COGO*/ wire A07_Cn = not(EXT_PIN_A07_C);
/*p08.MUJY*/ wire A08_Cn = not(EXT_PIN_A08_C);
/*p08.NENA*/ wire A09_Cn = not(EXT_PIN_A09_C);
/*p08.SURA*/ wire A10_Cn = not(EXT_PIN_A10_C);
/*p08.MADY*/ wire A11_Cn = not(EXT_PIN_A11_C);
/*p08.LAHE*/ wire A12_Cn = not(EXT_PIN_A12_C);
/*p08.LURA*/ wire A13_Cn = not(EXT_PIN_A13_C);
/*p08.PEVO*/ wire A14_Cn = not(EXT_PIN_A14_C);
/*p08.RAZA*/ wire A15_Cn = not(EXT_PIN_A15_C);

/*p08.KEJO*/ top.CPU_PIN_A00.set_tribuf_10n(TOVA_MODE_DBG2n, A00_Cn);
/*p08.BYXE*/ top.CPU_PIN_A01.set_tribuf_10n(TOVA_MODE_DBG2n, A01_Cn);
/*p08.AKAN*/ top.CPU_PIN_A02.set_tribuf_10n(TOVA_MODE_DBG2n, A02_Cn);
/*p08.ANAR*/ top.CPU_PIN_A03.set_tribuf_10n(TOVA_MODE_DBG2n, A03_Cn);
/*p08.AZUV*/ top.CPU_PIN_A04.set_tribuf_10n(TOVA_MODE_DBG2n, A04_Cn);
/*p08.AJOV*/ top.CPU_PIN_A05.set_tribuf_10n(TOVA_MODE_DBG2n, A05_Cn);
/*p08.BYNE*/ top.CPU_PIN_A06.set_tribuf_10n(TOVA_MODE_DBG2n, A06_Cn);
/*p08.BYNA*/ top.CPU_PIN_A07.set_tribuf_10n(TOVA_MODE_DBG2n, A07_Cn);
/*p08.LOFA*/ top.CPU_PIN_A08.set_tribuf_10n(TOVA_MODE_DBG2n, A08_Cn);
/*p08.MAPU*/ top.CPU_PIN_A09.set_tribuf_10n(TOVA_MODE_DBG2n, A09_Cn);
/*p08.RALA*/ top.CPU_PIN_A10.set_tribuf_10n(TOVA_MODE_DBG2n, A10_Cn);
/*p08.LORA*/ top.CPU_PIN_A11.set_tribuf_10n(TOVA_MODE_DBG2n, A11_Cn);
/*p08.LYNA*/ top.CPU_PIN_A12.set_tribuf_10n(TOVA_MODE_DBG2n, A12_Cn);
/*p08.LEFY*/ top.CPU_PIN_A13.set_tribuf_10n(TOVA_MODE_DBG2n, A13_Cn);
/*p08.NEFE*/ top.CPU_PIN_A14.set_tribuf_10n(TOVA_MODE_DBG2n, A14_Cn);
/*p08.SYZU*/ top.CPU_PIN_A15.set_tribuf_10n(TOVA_MODE_DBG2n, A15_Cn);
}
#endif

//---------------------------------------------------------------------------

void SchematicTop::tick_vram_pins() {

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp = and (UNOR_MODE_DBG2p(), dbg_reg.SOTO_DBG.qn());
  /*p25.RACO*/ wire RACO_DBG_VRAMn = not(TUTO_DBG_VRAMp);

  // the logic here is kinda weird, still seems to select vram.
  /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp());
  /*p08.SORE*/ wire SORE_0000_7FFFp = not(CPU_PIN_A15);
  /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(CPU_PIN_A13, CPU_PIN_A14, SORE_0000_7FFFp);
  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (CPU_PIN_A15, TEFA_8000_9FFFp);

  /*p??.APAP*/ wire APAP_CPU_ADDR_VALIDp = not(CPU_PIN_ADDR_VALID); // Missing from schematic
  /*p01.AWOD*/ wire AWOD_CPU_ADDR_VALIDn = nor(UNOR_MODE_DBG2p(), APAP_CPU_ADDR_VALIDp);
  /*p01.ABUZ*/ wire ABUZ_CPU_ADDR_VALIDp = not(AWOD_CPU_ADDR_VALIDn);

  /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and (SOSE_8000_9FFFp, ABUZ_CPU_ADDR_VALIDp);
  /*p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not(VRAM_PIN_MCSn_C);
  /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, TUTO_DBG_VRAMp);
  /*p25.ROPY*/ wire ROPY_RENDERINGn    = not(XYMU_RENDERINGp());
  /*p25.SERE*/ wire SERE_CPU_VRM_RDp   = and (_TOLE_VRAM_RDp, ROPY_RENDERINGn);

  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(SOSE_8000_9FFFp, CPU_PIN_WRp); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.TAVY*/ wire TAVY_MOEp = not(VRAM_PIN_MOEn_C);
  /*p25.SALE*/ wire SALE_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, TUTO_DBG_VRAMp);

  {
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxxFGH = not(AREV_CPU_WRn_ABCDxxxH());
    /*p25.TUJA*/ wire TUJA_CPU_VRAM_WRp = and(SOSE_8000_9FFFp, APOV_CPU_WRp_xxxxxFGH);
    /*p25.SUDO*/ wire SUDO_MWRp_C = not(VRAM_PIN_MWRn_C);
    /*p25.TYJY*/ wire TYJY_DBG_VRAM_WRp = mux2_p(SUDO_MWRp_C, TUJA_CPU_VRAM_WRp, TUTO_DBG_VRAMp);
    /*p25.SOHY*/ wire SOHY_MWRn    = nand(TYJY_DBG_VRAM_WRp, SERE_CPU_VRM_RDp);
    /*p25.TAXY*/ wire TAXY_MWRn_A = and(SOHY_MWRn, RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire SOFY_MWRn_D = or (SOHY_MWRn, TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire SYSY_MWRp_A = not(TAXY_MWRn_A);
    /*p25.RAGU*/ wire RAGU_MWRp_D = not(SOFY_MWRn_D);
    /*p25.SYSY*/ VRAM_PIN_MWRn_A.set(SYSY_MWRp_A);
    /*p25.RAGU*/ VRAM_PIN_MWRn_D.set(RAGU_MWRp_D);
  }


  {
    /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn   = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_VRAM_RDp = not(MUHO_DMA_VRAM_RDn);
    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_VRAM_RDp, XYMU_RENDERINGp()); // def nor
    /*p25.RYLU*/ wire RYLU_DBG_VRAM_RDn = nand(SALE_VRAM_WRn, XANE_VRAM_LOCKn);
    /*p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not(SOHO_SPR_VRAM_RDp());
    /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp()); // this should be correct
    /*p25.APAM*/ wire APAM_DMA_VRAM_RDn = not(LUFA_DMA_VRAM_RDp);
    /*p25.RACU*/ wire RACU_MOEn   = and (RYLU_DBG_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAM_RDn); // def and
    /*p25.SEMA*/ wire SEMA_MOEn_A = and(RACU_MOEn, RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire RUTE_MOEn_D = or (RACU_MOEn, TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire REFO_MOEn_A = not(SEMA_MOEn_A);
    /*p25.SAHA*/ wire SAHA_MOEn_D = not(RUTE_MOEn_D);
    /*p25.REFO*/ VRAM_PIN_MOEn_A.set(REFO_MOEn_A);
    /*p25.SAHA*/ VRAM_PIN_MOEn_D.set(SAHA_MOEn_D);
  }

  {
    /*p04.MUHO*/ wire MUHO_DMA_VRM_RDn = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_VRM_RDp = not(MUHO_DMA_VRM_RDn);
    /*p29.ABON*/ wire ABON_SPR_VRM_RDn = not(TEXY_SPR_READ_VRAMp());

    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp());
    /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not(LUSU_BGW_VRAM_RDn);
    // Polarity issues here, ABON should be P
    // ABON = not(TEXY)
    // SUTU = nor(LENA, LUFA, ABON, SERE);
    /*p25.SUTU*/ wire SUTU_MCSn = nor(LENA_BGW_VRM_RDp,
                                      LUFA_DMA_VRM_RDp,
                                      ABON_SPR_VRM_RDn,
                                      SERE_CPU_VRM_RDp);
    /*p25.TODE*/ wire TODE_MCSn_A = and(SUTU_MCSn, RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire SEWO_MCSn_D = or (SUTU_MCSn, TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire SOKY_MCSp_A = not(TODE_MCSn_A);
    /*p25.SETY*/ wire SETY_MCSp_D = not(SEWO_MCSn_D);
    /*p25.SOKY*/ VRAM_PIN_MCSn_A.set(SOKY_MCSp_A);
    /*p25.SETY*/ VRAM_PIN_MCSn_D.set(SETY_MCSp_D);
  }

  {
    /*p25.MYFU*/ wire MYFU = not(VRM_TRI_A00.q());
    /*p25.MASA*/ wire MASA = not(VRM_TRI_A01.q());
    /*p25.MYRE*/ wire MYRE = not(VRM_TRI_A02.q());
    /*p25.MAVU*/ wire MAVU = not(VRM_TRI_A03.q());
    /*p25.MEPA*/ wire MEPA = not(VRM_TRI_A04.q());
    /*p25.MYSA*/ wire MYSA = not(VRM_TRI_A05.q());
    /*p25.MEWY*/ wire MEWY = not(VRM_TRI_A06.q());
    /*p25.MUME*/ wire MUME = not(VRM_TRI_A07.q());
    /*p25.VOVA*/ wire VOVA = not(VRM_TRI_A08.q());
    /*p25.VODE*/ wire VODE = not(VRM_TRI_A09.q());
    /*p25.RUKY*/ wire RUKY = not(VRM_TRI_A10.q());
    /*p25.RUMA*/ wire RUMA = not(VRM_TRI_A11.q());
    /*p25.REHO*/ wire REHO = not(VRM_TRI_A12.q());

    /*p25.LEXE*/ VRAM_PIN_MA00_AD.set(MYFU);
    /*p25.LOZU*/ VRAM_PIN_MA01_AD.set(MASA);
    /*p25.LACA*/ VRAM_PIN_MA02_AD.set(MYRE);
    /*p25.LUVO*/ VRAM_PIN_MA03_AD.set(MAVU);
    /*p25.LOLY*/ VRAM_PIN_MA04_AD.set(MEPA);
    /*p25.LALO*/ VRAM_PIN_MA05_AD.set(MYSA);
    /*p25.LEFA*/ VRAM_PIN_MA06_AD.set(MEWY);
    /*p25.LUBY*/ VRAM_PIN_MA07_AD.set(MUME);
    /*p25.TUJY*/ VRAM_PIN_MA08_AD.set(VOVA);
    /*p25.TAGO*/ VRAM_PIN_MA09_AD.set(VODE);
    /*p25.NUVA*/ VRAM_PIN_MA10_AD.set(RUKY);
    /*p25.PEDU*/ VRAM_PIN_MA11_AD.set(RUMA);
    /*p25.PONY*/ VRAM_PIN_MA12_AD.set(REHO);
  }

  {
    /*p25.RUVY*/ wire _RUVY_VRAM_WR = not(SALE_VRAM_WRn);
    /*p25.SAZO*/ wire _SAZO_VRAM_RD  = and (_RUVY_VRAM_WR, SERE_CPU_VRM_RDp);
    /*p25.RYJE*/ wire _RYJE_VRAM_RDn = not(_SAZO_VRAM_RD);
    /*p25.REVO*/ wire _REVO_VRAM_RDp = not(_RYJE_VRAM_RDn);

    /*p25.ROCY*/ wire _ROCY_CPU_TO_VRMp = and (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RAHU*/ wire _RAHU_CPU_TO_VRMn = not(_ROCY_CPU_TO_VRMp);
    /*p25.TEME*/ VRM_TRI_D0.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D0.q());
    /*p25.TEWU*/ VRM_TRI_D1.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D1.q());
    /*p25.TYGO*/ VRM_TRI_D2.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D2.q());
    /*p25.SOTE*/ VRM_TRI_D3.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D3.q());
    /*p25.SEKE*/ VRM_TRI_D4.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D4.q());
    /*p25.RUJO*/ VRM_TRI_D5.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D5.q());
    /*p25.TOFA*/ VRM_TRI_D6.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D6.q());
    /*p25.SUZA*/ VRM_TRI_D7.set_tribuf_10n(_RAHU_CPU_TO_VRMn, CPU_TRI_D7.q());


    {
      /*p25.SYNU*/ wire SYNU = or (VRM_TRI_D0.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SYMA*/ wire SYMA = or (VRM_TRI_D1.q(), _RAHU_CPU_TO_VRMn);
      /*p25.ROKO*/ wire ROKO = or (VRM_TRI_D2.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SYBU*/ wire SYBU = or (VRM_TRI_D3.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SAKO*/ wire SAKO = or (VRM_TRI_D4.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SEJY*/ wire SEJY = or (VRM_TRI_D5.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SEDO*/ wire SEDO = or (VRM_TRI_D6.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SAWU*/ wire SAWU = or (VRM_TRI_D7.q(), _RAHU_CPU_TO_VRMn);

      /*p25.RURA*/ wire RURA = not(SYNU);
      /*p25.RULY*/ wire RULY = not(SYMA);
      /*p25.RARE*/ wire RARE = not(ROKO);
      /*p25.RODU*/ wire RODU = not(SYBU);
      /*p25.RUBE*/ wire RUBE = not(SAKO);
      /*p25.RUMU*/ wire RUMU = not(SEJY);
      /*p25.RYTY*/ wire RYTY = not(SEDO);
      /*p25.RADY*/ wire RADY = not(SAWU);

      VRAM_PIN_MD0_D.set(RURA);
      VRAM_PIN_MD1_D.set(RULY);
      VRAM_PIN_MD2_D.set(RARE);
      VRAM_PIN_MD3_D.set(RODU);
      VRAM_PIN_MD4_D.set(RUBE);
      VRAM_PIN_MD5_D.set(RUMU);
      VRAM_PIN_MD6_D.set(RYTY);
      VRAM_PIN_MD7_D.set(RADY);
    }


    {
      /*p25.ROVE*/ wire _ROVE_CPU_TO_VRMp = not(_RAHU_CPU_TO_VRMn);
      /*p25.SEFA*/ wire SEFA = and(VRM_TRI_D0.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SOGO*/ wire SOGO = and(VRM_TRI_D1.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SEFU*/ wire SEFU = and(VRM_TRI_D2.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUNA*/ wire SUNA = and(VRM_TRI_D3.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUMO*/ wire SUMO = and(VRM_TRI_D4.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SAZU*/ wire SAZU = and(VRM_TRI_D5.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SAMO*/ wire SAMO = and(VRM_TRI_D6.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUKE*/ wire SUKE = and(VRM_TRI_D7.q(), _ROVE_CPU_TO_VRMp);

      /*p25.REGE*/ wire REGE = not(SEFA);
      /*p25.RYKY*/ wire RYKY = not(SOGO);
      /*p25.RAZO*/ wire RAZO = not(SEFU);
      /*p25.RADA*/ wire RADA = not(SUNA);
      /*p25.RYRO*/ wire RYRO = not(SUMO);
      /*p25.REVU*/ wire REVU = not(SAZU);
      /*p25.REKU*/ wire REKU = not(SAMO);
      /*p25.RYZE*/ wire RYZE = not(SUKE);

      VRAM_PIN_MD0_A.set(REGE);
      VRAM_PIN_MD1_A.set(RYKY);
      VRAM_PIN_MD2_A.set(RAZO);
      VRAM_PIN_MD3_A.set(RADA);
      VRAM_PIN_MD4_A.set(RYRO);
      VRAM_PIN_MD5_A.set(REVU);
      VRAM_PIN_MD6_A.set(REKU);
      VRAM_PIN_MD7_A.set(RYZE);
    }


    // this is ORing a signal with a delayed version of itself?
    /*p25.RELA*/ wire _RELA_VRM_TO_CPUn = or (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RENA*/ wire _RENA_VRM_TO_CPUp = not(_RELA_VRM_TO_CPUn);
    /*p25.RODY*/ VRM_TRI_D0.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD0_C);
    /*p25.REBA*/ VRM_TRI_D1.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD1_C);
    /*p25.RYDO*/ VRM_TRI_D2.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD2_C);
    /*p25.REMO*/ VRM_TRI_D3.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD3_C);
    /*p25.ROCE*/ VRM_TRI_D4.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD4_C);
    /*p25.ROPU*/ VRM_TRI_D5.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD5_C);
    /*p25.RETA*/ VRM_TRI_D6.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD6_C);
    /*p25.RAKU*/ VRM_TRI_D7.set_tribuf_6p(_RENA_VRM_TO_CPUp, VRAM_PIN_MD7_C);

    /*p25.ROFA*/ wire _ROFA_VRM_TO_CPUn = not(_RENA_VRM_TO_CPUp);
    VRAM_PIN_MD0_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD1_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD2_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD3_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD4_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD5_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD6_B.set(_ROFA_VRM_TO_CPUn);
    VRAM_PIN_MD7_B.set(_ROFA_VRM_TO_CPUn);
  }

  {
    /*p04.DECY*/ wire DECY_FROM_CPU5n = not(CPU_PIN5);
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp(), CATY_FROM_CPU5p);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);

    /*p25.TYVY*/ wire _TYVY_VRM_TO_CPUn = nand(SERE_CPU_VRM_RDp, LEKO_CPU_RDp);
    /*p25.SEBY*/ wire _SEBY_VRM_TO_CPUp = not(_TYVY_VRM_TO_CPUn);

    /*p25.RERY*/ wire RERY = not(VRM_TRI_D0.q());
    /*p25.RUNA*/ wire RUNA = not(VRM_TRI_D1.q());
    /*p25.RONA*/ wire RONA = not(VRM_TRI_D2.q());
    /*p25.RUNO*/ wire RUNO = not(VRM_TRI_D3.q());
    /*p25.SANA*/ wire SANA = not(VRM_TRI_D4.q());
    /*p25.RORO*/ wire RORO = not(VRM_TRI_D5.q());
    /*p25.RABO*/ wire RABO = not(VRM_TRI_D6.q());
    /*p25.SAME*/ wire SAME = not(VRM_TRI_D7.q());

    /*p25.RUGA*/ CPU_TRI_D0.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RERY);
    /*p25.ROTA*/ CPU_TRI_D1.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RUNA);
    /*p25.RYBU*/ CPU_TRI_D2.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RONA);
    /*p25.RAJU*/ CPU_TRI_D3.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RUNO);
    /*p25.TYJA*/ CPU_TRI_D4.set_tribuf_6p(_SEBY_VRM_TO_CPUp, SANA);
    /*p25.REXU*/ CPU_TRI_D5.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RORO);
    /*p25.RUPY*/ CPU_TRI_D6.set_tribuf_6p(_SEBY_VRM_TO_CPUp, RABO);
    /*p25.TOKU*/ CPU_TRI_D7.set_tribuf_6p(_SEBY_VRM_TO_CPUp, SAME);
  }

}

//-----------------------------------------------------------------------------
// Reset signals

wire SchematicTop::AVOR_SYS_RSTp() const {
  /*p01.AVOR*/ wire AVOR_SYS_RSTp = or(rst_reg.AFER_SYS_RSTp.q(), rst_reg.ASOL_POR_DONEn.q());
  return AVOR_SYS_RSTp;
}

wire SchematicTop::XAPO_VID_RSTn() const {
  /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not(XORE_SYS_RSTp());
  /*p01.XODO*/ wire _XODO_VID_RSTp = nand(_XEBE_SYS_RSTn, XONA_LCDC_EN.q());
  /*p01.XAPO*/ wire _XAPO_VID_RSTn = not(_XODO_VID_RSTp);
  return _XAPO_VID_RSTn;
}

//-----------------------------------------------------------------------------
// CPU signals

wire SchematicTop::UJYV_CPU_RDn() const {
  wire UNOR_MODE_DBG2p = this->UNOR_MODE_DBG2p();
  /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(EXT_PIN_RDp_C, CPU_PIN_RDp, UNOR_MODE_DBG2p);
  return UJYV_CPU_RDn;
}

wire SchematicTop::AREV_CPU_WRn_ABCDxxxH() const {
  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand(CPU_PIN_WRp, AFAS_xxxxEFGx());
  return AREV_CPU_WRn_ABCDxxxH;
}

wire SchematicTop::UBAL_CPU_WRn_ABCDxxxH() const {
  wire UNOR_MODE_DBG2p = this->UNOR_MODE_DBG2p();
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not(AREV_CPU_WRn_ABCDxxxH());

  // polarity of EXT_PIN_WRp_C wrong?
  /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(EXT_PIN_WRp_C, APOV_CPU_WRp_xxxxEFGx, UNOR_MODE_DBG2p);
  return UBAL_CPU_WRn_ABCDxxxH;
}

//-----------------------------------------------------------------------------
// Bus mux signals

//-----------------------------------------------------------------------------
// DMA signals

wire SchematicTop::MATU_DMA_RUNNINGp() const {
  /*p04.MATU*/ wire MATU_DMA_RUNNINGp = dma_reg.MATU_DMA_RUNNINGp.q();
  return MATU_DMA_RUNNINGp;
}

wire SchematicTop::MUDA_DMA_SRC_VRAMp() const {
  /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(dma_reg.DMA_A15.q());
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
  return MUDA_DMA_SRC_VRAMp;
}

//------------------------------------------------------------------------------
// LCD signals

wire SchematicTop::BYHA_VID_LINE_TRIG_d4() const {
  /*p28.ABAF*/ wire ABAF_LINE_END_Bn = not(lcd_reg.CATU_LINE_END.q());
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4 = and (or (lcd_reg.ANEL_LINE_END_D.q(), ABAF_LINE_END_Bn), ABEZ_VID_RSTn);
  return BYHA_VID_LINE_TRIG_d4;
}

//-----------------------------------------------------------------------------
// Address decoder signals

wire SchematicTop::TUNA_0000_FDFFp() const {
  /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12,
                                           CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
  return TUNA_0000_FDFFp;
}

wire SchematicTop::WERO_FF4Xp() const {
#if 0

  /*p22.WERO*/ wire WERO_FF4Xp = and( CPU_PIN_A15, CPU_PIN_A14,  CPU_PIN_A13,  CPU_PIN_A12,
                                      CPU_PIN_A11, CPU_PIN_A10,  CPU_PIN_A09,  CPU_PIN_A08,
                                     !CPU_PIN_A07, CPU_PIN_A06, !CPU_PIN_A05, !CPU_PIN_A04);

#endif

  /*p22.XALY*/ wire XALY_0x00xxxxp  = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp(), TONA_A08n());
  /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
  /*p22.WERO*/ wire WERO_FF4Xp = not(WUTU_FF4Xn);

  return WERO_FF4Xp;
}

//-----------------------------------------------------------------------------
// PPU signals

wire SchematicTop::XYMU_RENDERINGp() const { /*p21.XYMU*/ return ppu_reg.XYMU_RENDERINGp.q(); }

wire SchematicTop::WODU_RENDER_DONEp() const {
  /*p21.XUGU*/ wire _XUGU_X_167 = nand(ppu_reg.XEHO_X0.q(), ppu_reg.SAVY_X1.q(), ppu_reg.XODU_X2.q(), ppu_reg.TUKY_X5.q(), ppu_reg.SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire _XANO_X_167 = not(_XUGU_X_167);
  /*p21.XENA*/ wire _XENA_STORE_MATCHn = not(FEPO_STORE_MATCHp());
  /*p21.WODU*/ wire WODU_RENDER_DONEp = and (_XENA_STORE_MATCHn, _XANO_X_167);
  return WODU_RENDER_DONEp;
}

wire SchematicTop::TEVO_FINE_RSTp() const {
  
  /*p27.ROMO*/ wire ROMO_AFTER_PORCHn = not(tile_fetcher.POKY_PORCH_DONEp.q());
  /*p27.SUVU*/ wire SUVU_PORCH_ENDn = nand(XYMU_RENDERINGp(), ROMO_AFTER_PORCHn, tile_fetcher.NYKA_TILE_FETCH_DONE_Ap.q(), PORY_TILE_FETCH_DONE_Bp());
  /*p27.TAVE*/ wire TAVE_PORCH_DONE_TRIGp = not(SUVU_PORCH_ENDn);
  /*p27.TEVO*/ wire TEVO_FINE_RSTp = nor(win_reg.SEKO_WX_MATCHne, win_reg.SUZU_WIN_FIRST_TILEne, TAVE_PORCH_DONE_TRIGp);
  return TEVO_FINE_RSTp;
}

wire SchematicTop::NYXU_TILE_FETCHER_RSTn() const {
  /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WX_MATCHpe());
  /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);
  /*p27.NYXU*/ wire NYXU_TILE_FETCHER_RSTn = nor(AVAP_RENDER_START_RST(), MOSU_WIN_MODE_TRIGp, TEVO_FINE_RSTp());
  return NYXU_TILE_FETCHER_RSTn;
}

// so dma stops oam scan?
wire SchematicTop::ACYL_SCANNINGp() const {
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(MATU_DMA_RUNNINGp());
  /*p28.ACYL*/ wire ACYL_SCANNINGp = and (BOGE_DMA_RUNNINGn, BESU_SCANNINGp());
  return ACYL_SCANNINGp;
}

wire SchematicTop::LYRY_BFETCH_DONEp() const {
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(tile_fetcher.LAXU_BFETCH_S0.q(), tile_fetcher.NYVA_BFETCH_S2.q(), NYXU_TILE_FETCHER_RSTn());
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);
  return LYRY_BFETCH_DONEp;
}

wire SchematicTop::SEGU_CLKPIPEn() const {
  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor(sprite_store.FEPO_STORE_MATCHp, WODU_RENDER_DONEp(), MYVO_AxCxExGx());

  /*p27.SYLO*/ wire SYLO_WIN_HITn = not(win_reg.RYDY_WIN_FIRST_TILE_A.q());
  /*p24.TOMU*/ wire TOMU_WIN_HITp = not(SYLO_WIN_HITn);
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not(TOMU_WIN_HITp);
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and (_SOCY_WIN_HITn, tile_fetcher.POKY_PORCH_DONEp.q(), _VYBO_PIX_CLK_xBxDxFxH);

  /*p24.SEGU*/ wire SEGU_CLKPIPEn = not(_TYFA_CLKPIPEp_xBxDxFxH);
  return SEGU_CLKPIPEn;
}

wire SchematicTop::SACU_CLKPIPEp() const {
  /*p24.SACU*/ return nor(SEGU_CLKPIPEn(), ppu_reg.ROXY_FINE_MATCH_LATCHn.q());
}

wire SchematicTop::ROZE_FINE_COUNT_7n() const {
  /*p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand(ppu_reg.RYKU_FINE_CNT0.q(), ppu_reg.ROGA_FINE_CNT1.q(), ppu_reg.RUBU_FINE_CNT2.q());
  return ROZE_FINE_COUNT_7n;
}

wire SchematicTop::VOTY_INT_STATp() const {
  /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(RUTU_LINE_END());
  /*p21.SELA*/ wire SELA_NEW_LINE_d0p = not(PURE_NEW_LINE_d0n);
  /*p21.TOLU*/ wire TOLU_VBLANKn = not(PARU_VBLANKp_d4());
  /*p21.TAPA*/ wire TAPA_INT_OAM = and (TOLU_VBLANKn, SELA_NEW_LINE_d0p);
  /*p21.TARU*/ wire TARU_INT_HBL = and (TOLU_VBLANKn, WODU_RENDER_DONEp());
  /*p21.SUKO*/ wire SUKO_INT_STATb = amux4(ppu_reg.RUGU_INT_LYC_EN.q(), ROPO_LY_MATCH_SYNCp(),
                                           ppu_reg.REFE_INT_OAM_EN.q(), TAPA_INT_OAM,
                                           ppu_reg.RUFO_INT_VBL_EN.q(), PARU_VBLANKp_d4(), // polarity?
                                           ppu_reg.ROXE_INT_HBL_EN.q(), TARU_INT_HBL);
  /*p21.TUVA*/ wire TUVA_INT_STATn = not(SUKO_INT_STATb);
  /*p21.VOTY*/ wire VOTY_INT_STATp = not(TUVA_INT_STATn);
  return VOTY_INT_STATp;
}

// And this is the topmost "reset sprite fetcher" signal
wire SchematicTop::VEKU_SFETCH_RUNNING_RSTn() const {
  /*p29.WUTY*/ wire WUTY_PIPE_LOAD_SPRITEp = not(VUSA_SPRITE_DONEn());
  /*p27.ROMO*/ wire ROMO_AFTER_PORCHn = not(tile_fetcher.POKY_PORCH_DONEp.q());
  /*p27.SUVU*/ wire SUVU_PORCH_ENDn = nand(XYMU_RENDERINGp(), ROMO_AFTER_PORCHn, tile_fetcher.NYKA_TILE_FETCH_DONE_Ap.q(), tile_fetcher.PORY_TILE_FETCH_DONE_Bp.q());
  /*p27.TAVE*/ wire TAVE_PORCH_DONE_TRIGp = not(SUVU_PORCH_ENDn);
  /*p27.VEKU*/ wire VEKU_SFETCH_RUNNING_RSTn = nor(WUTY_PIPE_LOAD_SPRITEp, TAVE_PORCH_DONE_TRIGp); // def nor
  return VEKU_SFETCH_RUNNING_RSTn;
}

//-----------------------------------------------------------------------------
// Tile fetcher signals

wire SchematicTop::PORY_TILE_FETCH_DONE_Bp() const {
  /*p24.PORY*/ return tile_fetcher.PORY_TILE_FETCH_DONE_Bp.q();
}

wire SchematicTop::LONY_BG_READ_VRAM_LATCHp() const { return tile_fetcher.LONY_BG_READ_VRAM_LATCHp.q(); }

wire SchematicTop::DATY_SCX0() const { /*p23.DATY*/ return tile_fetcher.DATY_SCX0.q(); }
wire SchematicTop::DUZU_SCX1() const { /*p23.DUZU*/ return tile_fetcher.DUZU_SCX1.q(); }
wire SchematicTop::CYXU_SCX2() const { /*p23.CYXU*/ return tile_fetcher.CYXU_SCX2.q(); }

//-----------------------------------------------------------------------------
// Sprite scanner signals

wire SchematicTop::CEHA_SCANNINGp() const { /*p29.CEHA*/ return not(sprite_scanner.CENO_SCANNINGp.qn()); }
wire SchematicTop::BESU_SCANNINGp() const { /*p28.BESU*/ return sprite_scanner.BESU_SCANNINGp.q(); }
wire SchematicTop::CENO_SCANNINGp() const { /*p29.CENO*/ return sprite_scanner.CENO_SCANNINGp.q(); }

wire SchematicTop::AVAP_RENDER_START_RST() const {
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4());
  /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p, ATAR_VID_RSTp);
  /*p29.BALU*/ wire BALU_LINE_RSTp = not(ANOM_LINE_RSTn);
  /*p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or(BALU_LINE_RSTp, sprite_scanner.DOBA_SCAN_DONE_B.q(), !sprite_scanner.BYBA_SCAN_DONE_A.q());
  /*p29.AVAP*/ wire AVAP_RENDER_START_RST = not(BEBU_SCAN_DONE_TRIGn);
  return AVAP_RENDER_START_RST;
}

wire SchematicTop::DEGE_SPRITE_DELTA0() const { return sprite_scanner.DEGE_SPRITE_DELTA0; }
wire SchematicTop::DABY_SPRITE_DELTA1() const { return sprite_scanner.DABY_SPRITE_DELTA1; }
wire SchematicTop::DABU_SPRITE_DELTA2() const { return sprite_scanner.DABU_SPRITE_DELTA2; }
wire SchematicTop::GYSA_SPRITE_DELTA3() const { return sprite_scanner.GYSA_SPRITE_DELTA3; }
wire SchematicTop::CARE_STORE_ENp_ABxxEFxx() const { return sprite_scanner.CARE_STORE_ENp_ABxxEFxx; }

wire SchematicTop::YFEL_SCAN0() const { /*p28.YFEL*/ return sprite_scanner.YFEL_SCAN0.q(); }
wire SchematicTop::WEWY_SCAN1() const { /*p28.WEWY*/ return sprite_scanner.WEWY_SCAN1.q(); }
wire SchematicTop::GOSO_SCAN2() const { /*p28.GOSO*/ return sprite_scanner.GOSO_SCAN2.q(); }
wire SchematicTop::ELYN_SCAN3() const { /*p28.ELYN*/ return sprite_scanner.ELYN_SCAN3.q(); }
wire SchematicTop::FAHA_SCAN4() const { /*p28.FAHA*/ return sprite_scanner.FAHA_SCAN4.q(); }
wire SchematicTop::FONY_SCAN5() const { /*p28.FONY*/ return sprite_scanner.FONY_SCAN5.q(); }

//-----------------------------------------------------------------------------
// Sprite store signals

wire SchematicTop::FEPO_STORE_MATCHp() const { /*p29.FEPO*/ return sprite_store.FEPO_STORE_MATCHp; }

//-----------------------------------------------------------------------------
// Sprite fetcher signals

wire SchematicTop::WEFY_SPR_READp() const {
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1.q(), sprite_fetcher.TESE_SFETCH_S2.q());
  /*p28.WEFY*/ wire WEFY_SPR_READp = and(TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0_D1.q());
  return WEFY_SPR_READp;
}

wire SchematicTop::VAPE_FETCH_OAM_CLK() const {
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1.q(), sprite_fetcher.TESE_SFETCH_S2.q());
  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0.q());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);
  /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  return VAPE_FETCH_OAM_CLK;
}

wire SchematicTop::VUSA_SPRITE_DONEn() const {
  // TYNO_01 << TOXE_17
  // TYNO_02 << SEBA_17
  // TYNO_03 << VONU_17
  // TYNO_04 >> VUSA_02

  // VUSA_01 << TYFO_16
  // VUSA_02 << TYNO_04
  // VUSA_03 nc
  // VUSA_04 >>

  /*p29.TYNO*/ wire TYNO = nand(sprite_fetcher.TOXE_SFETCH_S0.q(), sprite_fetcher.SEBA_SFETCH_S1_D5.q(), sprite_fetcher.VONU_SFETCH_S1_D4.q());
  /*p29.VUSA*/ wire VUSA_SPRITE_DONEn = or(sprite_fetcher.TYFO_SFETCH_S0_D1.qn(), TYNO);
  return VUSA_SPRITE_DONEn;
}

wire SchematicTop::TEXY_SPR_READ_VRAMp() const {
#if 0

  if (XYMU_RENDERINGp) {
    /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp = or(TULY_SFETCH_S1, VONU_SFETCH_S1_D4);
  }
  else {
    /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp = 0;
  }

#endif
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.SAKY*/ wire SAKY = nor(sprite_fetcher.TULY_SFETCH_S1.q(), sprite_fetcher.VONU_SFETCH_S1_D4.q());
  /*p29.TYSO*/ wire TYSO_SPRITE_READn = or(SAKY, TEPA_RENDERINGn);
  /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp = not(TYSO_SPRITE_READn);
  return TEXY_SPR_READ_VRAMp;
}

wire SchematicTop::SOHO_SPR_VRAM_RDp() const {
  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0.q());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);
  /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(TEXY_SPR_READ_VRAMp());
  /*p25.SOHO*/ wire SOHO_SPR_VRAM_RDp = and (TACU_SPR_SEQ_5_TRIG, ABON_SPR_VRAM_RDp1);
  return SOHO_SPR_VRAM_RDp;
}

//-----------------------------------------------------------------------------
// Window signals

wire SchematicTop::RYDY_WIN_FIRST_TILE_A() const {
  wire RYDY_WIN_FIRST_TILE_A = win_reg.RYDY_WIN_FIRST_TILE_A.q();
  return RYDY_WIN_FIRST_TILE_A;
}

wire SchematicTop::NOCU_WIN_MODEn() const {
  /*p27.NOCU*/ return not(win_reg.PYNU_WIN_MODE_A.q());
}

wire SchematicTop::NUNY_WX_MATCHpe() const {
  /*p27.NUNY*/ return win_reg.NUNY_WX_MATCHpe;
}

//------------------------------------------------------------------------------
// Misc signals

wire SchematicTop::ASOK_INT_JOYPADp() const {
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp = and (joy_reg.APUG_JP_GLITCH3.q(), joy_reg.BATU_JP_GLITCH0.q());
  return ASOK_INT_JOYPADp;
}

wire SchematicTop::BOOT_BITn() const {
  /*p07.TEPU*/ return bootrom.BOOT_BITn.q();
}

wire SchematicTop::CALY_INT_SERIALp() const {
  return ser_reg.CALY_INT_SERIALp.q();
}

//------------------------------------------------------------------------------
































































//------------------------------------------------------------------------------

//void preset(bool EXT_PIN_RDn_A, bool EXT_PIN_CSn_A);

/*
void preset_d(bool oe, uint8_t d) {
PIN_D0_C.preset(oe, d & 0x01);
PIN_D1_C.preset(oe, d & 0x02);
PIN_D2_C.preset(oe, d & 0x04);
PIN_D3_C.preset(oe, d & 0x08);
PIN_D4_C.preset(oe, d & 0x10);
PIN_D5_C.preset(oe, d & 0x20);
PIN_D6_C.preset(oe, d & 0x40);
PIN_D7_C.preset(oe, d & 0x80);
}
*/

#if 0
void ExtPinsOut::preset() {
  EXT_PIN_WRp_C.preset(true, 0);   // -> P07.UBAL
  EXT_PIN_RDp_C.preset(true, 0);   // -> P07.UJYV
  EXT_PIN_A00_C.preset(true, 0);   // -> P08.KOVA
  EXT_PIN_A01_C.preset(true, 0);   // -> P08.CAMU
  EXT_PIN_A02_C.preset(true, 0);   // -> P08.BUXU
  EXT_PIN_A03_C.preset(true, 0);   // -> P08.BASE
  EXT_PIN_A04_C.preset(true, 0);   // -> P08.AFEC
  EXT_PIN_A05_C.preset(true, 0);   // -> P08.ABUP
  EXT_PIN_A06_C.preset(true, 0);   // -> P08.CYGU
  EXT_PIN_A07_C.preset(true, 0);   // -> P08.COGO
  EXT_PIN_A08_C.preset(true, 0);   // -> P08.MUJY
  EXT_PIN_A09_C.preset(true, 0);   // -> P08.NENA
  EXT_PIN_A10_C.preset(true, 0);   // -> P08.SURA
  EXT_PIN_A11_C.preset(true, 0);   // -> P08.MADY
  EXT_PIN_A12_C.preset(true, 0);   // -> P08.LAHE
  EXT_PIN_A13_C.preset(true, 0);   // -> P08.LURA
  EXT_PIN_A14_C.preset(true, 0);   // -> P08.PEVO
  EXT_PIN_A15_C.preset(true, 0);   // -> P08.RAZA

  if (!EXT_PIN_RDn_A && !EXT_PIN_CSn_A) {
    uint16_t ext_addr = get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    preset_d(false, 0);
  }
}
#endif





#if 0
inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& d) {
  dump_pin(text_painter, a.a, d.a);
}

inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& b, const PinIn& c, const PinOut& d) {
  dump_pin(text_painter, a.a, b.a, c.a, d.a);
}

void dump_regs(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf(" ----- EXT_BUS -----\n");

  text_painter.dprintf("ADDR_LATCH ");
  dump2(text_painter, EXT_ADDR_LATCH_14.a);
  dump2(text_painter, EXT_ADDR_LATCH_13.a);
  dump2(text_painter, EXT_ADDR_LATCH_12.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_11.a);
  dump2(text_painter, EXT_ADDR_LATCH_10.a);
  dump2(text_painter, EXT_ADDR_LATCH_09.a);
  dump2(text_painter, EXT_ADDR_LATCH_08.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_07.a);
  dump2(text_painter, EXT_ADDR_LATCH_06.a);
  dump2(text_painter, EXT_ADDR_LATCH_05.a);
  dump2(text_painter, EXT_ADDR_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_03.a);
  dump2(text_painter, EXT_ADDR_LATCH_02.a);
  dump2(text_painter, EXT_ADDR_LATCH_01.a);
  dump2(text_painter, EXT_ADDR_LATCH_00.a);
  text_painter.newline();

  text_painter.dprintf("DATA_LATCH ");
  dump2(text_painter, CART_DATA_LATCH_07.a);
  dump2(text_painter, CART_DATA_LATCH_06.a);
  dump2(text_painter, CART_DATA_LATCH_05.a);
  dump2(text_painter, CART_DATA_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, CART_DATA_LATCH_03.a);
  dump2(text_painter, CART_DATA_LATCH_02.a);
  dump2(text_painter, CART_DATA_LATCH_01.a);
  dump2(text_painter, CART_DATA_LATCH_00.a);
  text_painter.newline();
  */
}

void dump_pins(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf("----- EXT_PINS -----\n");

  text_painter.dprintf("EXT_PIN_CLK %d\n", EXT_PIN_CLK.a.val);
  text_painter.dprintf("WR  %d\n", EXT_PIN_WRn_A.a.val);
  text_painter.dprintf("RD  %d\n", EXT_PIN_RDn_A.a.val);
  text_painter.dprintf("CS  %d\n", EXT_PIN_CSn_A.a.val);

  text_painter.add_text("Axx ");
  dump_pins(text_painter, EXT_PIN_A15_A, EXT_PIN_A15_D);
  dump_pins(text_painter, EXT_PIN_A14_A, EXT_PIN_A14_D);
  dump_pins(text_painter, EXT_PIN_A13_A, EXT_PIN_A13_D);
  dump_pins(text_painter, EXT_PIN_A12_A, EXT_PIN_A12_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A11_A, EXT_PIN_A11_D);
  dump_pins(text_painter, EXT_PIN_A10_A, EXT_PIN_A10_D);
  dump_pins(text_painter, EXT_PIN_A09_A, EXT_PIN_A09_D);
  dump_pins(text_painter, EXT_PIN_A08_A, EXT_PIN_A08_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A07_A, EXT_PIN_A07_D);
  dump_pins(text_painter, EXT_PIN_A06_A, EXT_PIN_A06_D);
  dump_pins(text_painter, EXT_PIN_A05_A, EXT_PIN_A05_D);
  dump_pins(text_painter, EXT_PIN_A04_A, EXT_PIN_A04_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A03_A, EXT_PIN_A03_D);
  dump_pins(text_painter, EXT_PIN_A02_A, EXT_PIN_A02_D);
  dump_pins(text_painter, EXT_PIN_A01_A, EXT_PIN_A01_D);
  dump_pins(text_painter, EXT_PIN_A00_A, EXT_PIN_A00_D);
  text_painter.newline();

  text_painter.add_text("Dxx ");
  dump_pins(text_painter, EXT_PIN_D7_A, EXT_PIN_D7_B, PIN_D7_C, EXT_PIN_D7_D);
  dump_pins(text_painter, EXT_PIN_D6_A, EXT_PIN_D6_B, PIN_D6_C, EXT_PIN_D6_D);
  dump_pins(text_painter, EXT_PIN_D5_A, EXT_PIN_D5_B, PIN_D5_C, EXT_PIN_D5_D);
  dump_pins(text_painter, EXT_PIN_D4_A, EXT_PIN_D4_B, PIN_D4_C, EXT_PIN_D4_D);
  dump_pins(text_painter, EXT_PIN_D3_A, EXT_PIN_D3_B, PIN_D3_C, EXT_PIN_D3_D);
  dump_pins(text_painter, EXT_PIN_D2_A, EXT_PIN_D2_B, PIN_D2_C, EXT_PIN_D2_D);
  dump_pins(text_painter, EXT_PIN_D1_A, EXT_PIN_D1_B, PIN_D1_C, EXT_PIN_D1_D);
  dump_pins(text_painter, EXT_PIN_D0_A, EXT_PIN_D0_B, PIN_D0_C, EXT_PIN_D0_D);
  text_painter.newline();
  */
}

uint16_t get_addr() const {
  uint16_t ext_addr = (uint16_t)pack(
    EXT_PIN_A00_A, EXT_PIN_A01_A, EXT_PIN_A02_A, EXT_PIN_A03_A,
    EXT_PIN_A04_A, EXT_PIN_A05_A, EXT_PIN_A06_A, EXT_PIN_A07_A,
    EXT_PIN_A08_A, EXT_PIN_A09_A, EXT_PIN_A10_A, EXT_PIN_A11_A,
    EXT_PIN_A12_A, EXT_PIN_A13_A, EXT_PIN_A14_A, EXT_PIN_A15_A);

  return ext_addr;
}

/*
void dump_pins(TextPainter& text_painter) {
text_painter.dprintf("----- SYS_PINS -----\n");
text_painter.dprintf("PIN_RST      %d\n", PIN_RST.a.val);
text_painter.dprintf("PIN_CLK_GOOD %d\n", PIN_CLK_GOOD.a.val);
text_painter.dprintf("SYS_PIN_CLK_xBxDxFxH   %d\n", SYS_PIN_CLK_xBxDxFxH.a.val);
text_painter.dprintf("SYS_PIN_T1       %d\n", SYS_PIN_T1.a.val);
text_painter.dprintf("SYS_PIN_T2       %d\n", SYS_PIN_T2.a.val);
text_painter.newline();
}
*/

#endif



#if 0
void dump(TextPainter& text_painter) {
  text_painter.dprintf("----- VRAM_PINS -----\n");

  text_painter.dprintf("MCS  %d:x:%d:%d\n", VRAM_PIN_MCSn_A.prev().val, VRAM_PIN_MCSn_C.prev().val, VRAM_PIN_MCSn_D.prev().val);
  text_painter.dprintf("MOE  %d:x:%d:%d\n", VRAM_PIN_MOEn_A.prev().val, VRAM_PIN_MOEn_C.prev().val, VRAM_PIN_MOEn_D.prev().val);
  text_painter.dprintf("MWR  %d:x:%d:%d\n", VRAM_PIN_MWRn_A.prev().val, VRAM_PIN_MWRn_C.prev().val, VRAM_PIN_MWRn_D.prev().val);
  text_painter.dprintf("MAxx 0x%04x\n", pack(VRAM_PIN_MA00_AD, VRAM_PIN_MA01_AD, VRAM_PIN_MA02_AD, VRAM_PIN_MA03_AD, VRAM_PIN_MA04_AD, VRAM_PIN_MA05_AD, VRAM_PIN_MA06_AD,
    VRAM_PIN_MA07_AD, VRAM_PIN_MA08_AD, VRAM_PIN_MA09_AD, VRAM_PIN_MA10_AD, VRAM_PIN_MA11_AD, VRAM_PIN_MA12_AD));

  text_painter.dprintf("MDx_A 0x%02x\n", pack(VRAM_PIN_MD0_A, VRAM_PIN_MD1_A, VRAM_PIN_MD2_A, VRAM_PIN_MD3_A, VRAM_PIN_MD4_A, VRAM_PIN_MD5_A, VRAM_PIN_MD6_A, VRAM_PIN_MD7_A));
  text_painter.dprintf("MDx_B 0x%02x\n", pack(VRAM_PIN_MD0_B, VRAM_PIN_MD1_B, VRAM_PIN_MD2_B, VRAM_PIN_MD3_B, VRAM_PIN_MD4_B, VRAM_PIN_MD5_B, VRAM_PIN_MD6_B, VRAM_PIN_MD7_B));
  text_painter.dprintf("MDx_C 0x%02x\n", pack(VRAM_PIN_MD0_C, VRAM_PIN_MD1_C, VRAM_PIN_MD2_C, VRAM_PIN_MD3_C, VRAM_PIN_MD4_C, VRAM_PIN_MD5_C, VRAM_PIN_MD6_C, VRAM_PIN_MD7_C));
  text_painter.dprintf("MDx_D 0x%02x\n", pack(VRAM_PIN_MD0_D, VRAM_PIN_MD1_D, VRAM_PIN_MD2_D, VRAM_PIN_MD3_D, VRAM_PIN_MD4_D, VRAM_PIN_MD5_D, VRAM_PIN_MD6_D, VRAM_PIN_MD7_D));
  text_painter.newline();
}
#endif
