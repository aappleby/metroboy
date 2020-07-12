#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

SchematicTop::SchematicTop() {
  // HACK preset these so we don't read hiz

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

  int_reg.CPU_PIN_ACK_JOYPAD.set(0);
  int_reg.CPU_PIN_ACK_SERIAL.set(0);
  int_reg.CPU_PIN_ACK_STAT.set(0);
  int_reg.CPU_PIN_ACK_TIMER.set(0);
  int_reg.CPU_PIN_ACK_VBLANK.set(0);
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
  bus_mux.EXT_PIN_D0_C.set(data & 0x01);
  bus_mux.EXT_PIN_D1_C.set(data & 0x02);
  bus_mux.EXT_PIN_D2_C.set(data & 0x04);
  bus_mux.EXT_PIN_D3_C.set(data & 0x08);
  bus_mux.EXT_PIN_D4_C.set(data & 0x10);
  bus_mux.EXT_PIN_D5_C.set(data & 0x20);
  bus_mux.EXT_PIN_D6_C.set(data & 0x40);
  bus_mux.EXT_PIN_D7_C.set(data & 0x80);
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

  sprite_scanner.tick_ymatch(*this);
  sprite_scanner.tock(*this);

  lcd_reg.tick(*this);
  lcd_reg.tock(*this);

  bus_mux.tick(*this);
  bus_mux.tock(*this);

  sprite_store.tick_match(*this);  // after bus mux
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

  CPU_PIN_EXT_RST.set(SYS_PIN_RSTp);

  {
    CPU_PIN_ADDR_HI.set(SYRO_FE00_FFFFp());
  }

  //----------
  // FF40 LCDC
  {
    /*p22.WORU*/ wire _WORU_FF40n = nand(WERO_FF4Xp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not(_WORU_FF40n);

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and (_VOCA_FF40p, ASOT_CPU_RDp());
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not(_VYRE_FF40_RDp);

    /*p23.WYPO*/ CPU_TRI_D0.set_tribuf_6n(_WYCE_FF40_RDn, VYXE_LCDC_BGEN.q());
    /*p23.XERO*/ CPU_TRI_D1.set_tribuf_6n(_WYCE_FF40_RDn, XYLO_LCDC_SPEN.q());
    /*p23.WYJU*/ CPU_TRI_D2.set_tribuf_6n(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZE.q());
    /*p23.WUKA*/ CPU_TRI_D3.set_tribuf_6n(_WYCE_FF40_RDn, XAFO_LCDC_BGMAP.q());
    /*p23.VOKE*/ CPU_TRI_D4.set_tribuf_6n(_WYCE_FF40_RDn, WEXU_LCDC_BGTILE.q());
    /*p23.VATO*/ CPU_TRI_D5.set_tribuf_6n(_WYCE_FF40_RDn, WYMO_LCDC_WINEN.q());
    /*p23.VAHA*/ CPU_TRI_D6.set_tribuf_6n(_WYCE_FF40_RDn, WOKY_LCDC_WINMAP.q());
    /*p23.XEBU*/ CPU_TRI_D7.set_tribuf_6n(_WYCE_FF40_RDn, XONA_LCDC_EN.q());

    /*p23.WARU*/ wire _WARU_FF40_WRp = and (_VOCA_FF40p, CUPA_CPU_WRp_xxxxEFGx());
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not(rst_reg.XORE_SYS_RSTp());
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D0.q());
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D1.q());
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D2.q());
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D3.q());
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D4.q());
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D5.q());
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D6.q());
    /*p23.XONA*/ XONA_LCDC_EN     .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, CPU_TRI_D7.q());
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
  hash << SYS_PIN_T2n.commit();
  hash << SYS_PIN_T1n.commit();

  //----------------------------------------
  // SOC-to-CPU

  hash << CPU_PIN_SYS_RSTp.commit();
  hash << CPU_PIN_EXT_RST.commit();       // PORTC_02: <- PIN_RESET directly connected to the pad
  hash << CPU_PIN_STARTp.commit();
  hash << CPU_PIN_BOOTp.commit();         // PORTA_04: <- TUTU
  hash << CPU_PIN_ADDR_HI.commit();       // PORTA_03: <- SYRO

  //----------------------------------------
  // CPU-to-SOC

  hash << CPU_PIN6.commit();               // PORTD_00: -> LEXY, doesn't do anything
  hash << CPU_PIN5.commit();               // PORTD_06: -> FROM_CPU5

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

  /*p25.TUTO*/ wire _TUTO_DBG_VRAMp = and (top.UNOR_MODE_DBG2p(), dbg_reg.SOTO_DBG.qn());
  /*p25.RACO*/ wire _RACO_DBG_VRAMn = not(_TUTO_DBG_VRAMp);

  // the logic here is kinda weird, still seems to select vram.

  /*p25.TEFA*/ wire _TEFA_8000_9FFFp = nor(top.SYRO_FE00_FFFFp(), top.TEXO_8000_9FFFn());
  /*p25.SOSE*/ wire _SOSE_8000_9FFFp = and (CPU_PIN_A15, _TEFA_8000_9FFFp);

  /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and (_SOSE_8000_9FFFp, top.ABUZ_CPU_ADDR_VALIDp());
  /*p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not(VRAM_PIN_MCSn_C);
  /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, _TUTO_DBG_VRAMp);
  /*p25.SERE*/ wire _SERE_CPU_VRM_RDp   = and (_TOLE_VRAM_RDp, ppu_reg.ROPY_RENDERINGn());

  /*p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand(_SOSE_8000_9FFFp, CPU_PIN_WRp); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.TAVY*/ wire _TAVY_MOEp = not(VRAM_PIN_MOEn_C);
  /*p25.SALE*/ wire _SALE_VRAM_WRn = mux2_p(_TAVY_MOEp, _TEGU_CPU_VRAM_WRn, _TUTO_DBG_VRAMp);

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and(_SOSE_8000_9FFFp, APOV_CPU_WRp_xxxxEFGx());
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not(VRAM_PIN_MWRn_C);
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2_p(_SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp, _TUTO_DBG_VRAMp);
    /*p25.SOHY*/ wire _SOHY_MWRn    = nand(_TYJY_DBG_VRAM_WRp, _SERE_CPU_VRM_RDp);
    /*p25.TAXY*/ wire _TAXY_MWRn_A = and(_SOHY_MWRn, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or (_SOHY_MWRn, _TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not(_SOFY_MWRn_D);
    VRAM_PIN_MWRn_A.set(_SYSY_MWRp_A);
    VRAM_PIN_MWRn_D.set(_RAGU_MWRp_D);
  }


  {
    /*p25.RYLU*/ wire _RYLU_DBG_VRAM_RDn = nand(_SALE_VRAM_WRn, XANE_VRAM_LOCKn());
    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and(sprite_fetcher.TACU_SPR_SEQ_5_TRIG(), ABON_SPR_VRM_RDn());
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not(_SOHO_SPR_VRAM_RDp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not(tile_fetcher.LONY_BG_READ_VRAM_LATCHp.q()); // this should be correct

    /*p25.RACU*/ wire _RACU_MOEn   = and (_RYLU_DBG_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, top.dma_reg.APAM_DMA_VRAM_RDn()); // def and
    /*p25.SEMA*/ wire _SEMA_MOEn_A = and(_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or (_RACU_MOEn, _TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not(_RUTE_MOEn_D);
    VRAM_PIN_MOEn_A.set(_REFO_MOEn_A);
    VRAM_PIN_MOEn_D.set(_SAHA_MOEn_D);
  }

  {
    // Polarity issues here, ABON should be P
    // ABON = not(TEXY)
    // SUTU = nor(LENA, LUFA, ABON, SERE);
    /*p25.SUTU*/ wire _SUTU_MCSn = nor(top.tile_fetcher.LENA_BGW_VRM_RDp(),
                                       top.dma_reg.LUFA_DMA_VRAM_RDp(),
                                       ABON_SPR_VRM_RDn(),
                                       _SERE_CPU_VRM_RDp);
    /*p25.TODE*/ wire _TODE_MCSn_A = and(_SUTU_MCSn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or (_SUTU_MCSn, _TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not(_SEWO_MCSn_D);
    VRAM_PIN_MCSn_A.set(_SOKY_MCSp_A);
    VRAM_PIN_MCSn_D.set(_SETY_MCSp_D);
  }

  {
    /*p25.MYFU*/ wire _MYFU = not(VRM_TRI_A00.q());
    /*p25.MASA*/ wire _MASA = not(VRM_TRI_A01.q());
    /*p25.MYRE*/ wire _MYRE = not(VRM_TRI_A02.q());
    /*p25.MAVU*/ wire _MAVU = not(VRM_TRI_A03.q());
    /*p25.MEPA*/ wire _MEPA = not(VRM_TRI_A04.q());
    /*p25.MYSA*/ wire _MYSA = not(VRM_TRI_A05.q());
    /*p25.MEWY*/ wire _MEWY = not(VRM_TRI_A06.q());
    /*p25.MUME*/ wire _MUME = not(VRM_TRI_A07.q());
    /*p25.VOVA*/ wire _VOVA = not(VRM_TRI_A08.q());
    /*p25.VODE*/ wire _VODE = not(VRM_TRI_A09.q());
    /*p25.RUKY*/ wire _RUKY = not(VRM_TRI_A10.q());
    /*p25.RUMA*/ wire _RUMA = not(VRM_TRI_A11.q());
    /*p25.REHO*/ wire _REHO = not(VRM_TRI_A12.q());

    /*p25.LEXE*/ VRAM_PIN_MA00_AD.set(_MYFU);
    /*p25.LOZU*/ VRAM_PIN_MA01_AD.set(_MASA);
    /*p25.LACA*/ VRAM_PIN_MA02_AD.set(_MYRE);
    /*p25.LUVO*/ VRAM_PIN_MA03_AD.set(_MAVU);
    /*p25.LOLY*/ VRAM_PIN_MA04_AD.set(_MEPA);
    /*p25.LALO*/ VRAM_PIN_MA05_AD.set(_MYSA);
    /*p25.LEFA*/ VRAM_PIN_MA06_AD.set(_MEWY);
    /*p25.LUBY*/ VRAM_PIN_MA07_AD.set(_MUME);
    /*p25.TUJY*/ VRAM_PIN_MA08_AD.set(_VOVA);
    /*p25.TAGO*/ VRAM_PIN_MA09_AD.set(_VODE);
    /*p25.NUVA*/ VRAM_PIN_MA10_AD.set(_RUKY);
    /*p25.PEDU*/ VRAM_PIN_MA11_AD.set(_RUMA);
    /*p25.PONY*/ VRAM_PIN_MA12_AD.set(_REHO);
  }

  {
    /*p25.RUVY*/ wire _RUVY_VRAM_WR = not(_SALE_VRAM_WRn);
    /*p25.SAZO*/ wire _SAZO_VRAM_RD  = and (_RUVY_VRAM_WR, _SERE_CPU_VRM_RDp);
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
      /*p25.SYNU*/ wire _SYNU = or (VRM_TRI_D0.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SYMA*/ wire _SYMA = or (VRM_TRI_D1.q(), _RAHU_CPU_TO_VRMn);
      /*p25.ROKO*/ wire _ROKO = or (VRM_TRI_D2.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SYBU*/ wire _SYBU = or (VRM_TRI_D3.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SAKO*/ wire _SAKO = or (VRM_TRI_D4.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SEJY*/ wire _SEJY = or (VRM_TRI_D5.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SEDO*/ wire _SEDO = or (VRM_TRI_D6.q(), _RAHU_CPU_TO_VRMn);
      /*p25.SAWU*/ wire _SAWU = or (VRM_TRI_D7.q(), _RAHU_CPU_TO_VRMn);

      /*p25.RURA*/ wire _RURA = not(_SYNU);
      /*p25.RULY*/ wire _RULY = not(_SYMA);
      /*p25.RARE*/ wire _RARE = not(_ROKO);
      /*p25.RODU*/ wire _RODU = not(_SYBU);
      /*p25.RUBE*/ wire _RUBE = not(_SAKO);
      /*p25.RUMU*/ wire _RUMU = not(_SEJY);
      /*p25.RYTY*/ wire _RYTY = not(_SEDO);
      /*p25.RADY*/ wire _RADY = not(_SAWU);

      VRAM_PIN_MD0_D.set(_RURA);
      VRAM_PIN_MD1_D.set(_RULY);
      VRAM_PIN_MD2_D.set(_RARE);
      VRAM_PIN_MD3_D.set(_RODU);
      VRAM_PIN_MD4_D.set(_RUBE);
      VRAM_PIN_MD5_D.set(_RUMU);
      VRAM_PIN_MD6_D.set(_RYTY);
      VRAM_PIN_MD7_D.set(_RADY);
    }


    {
      /*p25.ROVE*/ wire _ROVE_CPU_TO_VRMp = not(_RAHU_CPU_TO_VRMn);
      /*p25.SEFA*/ wire _SEFA = and(VRM_TRI_D0.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SOGO*/ wire _SOGO = and(VRM_TRI_D1.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SEFU*/ wire _SEFU = and(VRM_TRI_D2.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUNA*/ wire _SUNA = and(VRM_TRI_D3.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUMO*/ wire _SUMO = and(VRM_TRI_D4.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SAZU*/ wire _SAZU = and(VRM_TRI_D5.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SAMO*/ wire _SAMO = and(VRM_TRI_D6.q(), _ROVE_CPU_TO_VRMp);
      /*p25.SUKE*/ wire _SUKE = and(VRM_TRI_D7.q(), _ROVE_CPU_TO_VRMp);

      /*p25.REGE*/ wire _REGE = not(_SEFA);
      /*p25.RYKY*/ wire _RYKY = not(_SOGO);
      /*p25.RAZO*/ wire _RAZO = not(_SEFU);
      /*p25.RADA*/ wire _RADA = not(_SUNA);
      /*p25.RYRO*/ wire _RYRO = not(_SUMO);
      /*p25.REVU*/ wire _REVU = not(_SAZU);
      /*p25.REKU*/ wire _REKU = not(_SAMO);
      /*p25.RYZE*/ wire _RYZE = not(_SUKE);

      VRAM_PIN_MD0_A.set(_REGE);
      VRAM_PIN_MD1_A.set(_RYKY);
      VRAM_PIN_MD2_A.set(_RAZO);
      VRAM_PIN_MD3_A.set(_RADA);
      VRAM_PIN_MD4_A.set(_RYRO);
      VRAM_PIN_MD5_A.set(_REVU);
      VRAM_PIN_MD6_A.set(_REKU);
      VRAM_PIN_MD7_A.set(_RYZE);
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
    /*p25.TYVY*/ wire _TYVY_VRM_TO_CPUn = nand(_SERE_CPU_VRM_RDp, LEKO_CPU_RDp());
    /*p25.SEBY*/ wire _SEBY_VRM_TO_CPUp = not(_TYVY_VRM_TO_CPUn);

    /*p25.RERY*/ wire _RERY = not(VRM_TRI_D0.q());
    /*p25.RUNA*/ wire _RUNA = not(VRM_TRI_D1.q());
    /*p25.RONA*/ wire _RONA = not(VRM_TRI_D2.q());
    /*p25.RUNO*/ wire _RUNO = not(VRM_TRI_D3.q());
    /*p25.SANA*/ wire _SANA = not(VRM_TRI_D4.q());
    /*p25.RORO*/ wire _RORO = not(VRM_TRI_D5.q());
    /*p25.RABO*/ wire _RABO = not(VRM_TRI_D6.q());
    /*p25.SAME*/ wire _SAME = not(VRM_TRI_D7.q());

    /*p25.RUGA*/ CPU_TRI_D0.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RERY);
    /*p25.ROTA*/ CPU_TRI_D1.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RUNA);
    /*p25.RYBU*/ CPU_TRI_D2.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RONA);
    /*p25.RAJU*/ CPU_TRI_D3.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RUNO);
    /*p25.TYJA*/ CPU_TRI_D4.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _SANA);
    /*p25.REXU*/ CPU_TRI_D5.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RORO);
    /*p25.RUPY*/ CPU_TRI_D6.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RABO);
    /*p25.TOKU*/ CPU_TRI_D7.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _SAME);
  }

}

//-----------------------------------------------------------------------------
































































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
