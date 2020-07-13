#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

SchematicTop::SchematicTop() {
  // HACK preset these so we don't read hiz

  JOY_PIN_P10_C.set(0);
  JOY_PIN_P11_C.set(0);
  JOY_PIN_P12_C.set(0);
  JOY_PIN_P13_C.set(0);

  int_reg.CPU_PIN_ACK_JOYPAD.set(0);
  int_reg.CPU_PIN_ACK_SERIAL.set(0);
  int_reg.CPU_PIN_ACK_STAT.set(0);
  int_reg.CPU_PIN_ACK_TIMER.set(0);
  int_reg.CPU_PIN_ACK_VBLANK.set(0);
}

//-----------------------------------------------------------------------------

void SchematicTop::set_buttons(uint8_t buttons) {
  if (joypad.KELY_JOYP_UDLR.q()) {
    JOY_PIN_P10_C.set(buttons & 0x01);
    JOY_PIN_P11_C.set(buttons & 0x02);
    JOY_PIN_P12_C.set(buttons & 0x04);
    JOY_PIN_P13_C.set(buttons & 0x08);
  }
  else if (joypad.COFY_JOYP_ABCS.q()) {
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

SignalHash SchematicTop::tick() {
  SignalHash hash;

  bool verbose = false;

  if (verbose) printf("SchematicTop::tick\n");

  rst_reg.tick(*this);
  lcd_reg.tick(*this);
  sprite_scanner.tick(*this);
  sprite_store.tick_match(*this);  // after bus mux
  win_reg.tick(*this); // after sprite store
  tile_fetcher.tick(*this); // after window
  clk_reg.tick(*this);
  dbg_reg.tick(*this);
  tim_reg.tick(*this);
  bootrom.tick(*this);
  dma_reg.tick(*this);
  ser_reg.tick(*this);
  joypad.tick(*this);
  bus_mux.tick(*this);
  ppu_reg.tick(*this); // after window
  sprite_fetcher.tick(*this); // after window
  pix_pipe.tick(*this); // after window
  int_reg.tick(*this);

  clk_reg.tock(*this);
  dbg_reg.tock(*this);
  rst_reg.tock(*this);
  tim_reg.tock(*this);
  bootrom.tock(*this);
  dma_reg.tock(*this);
  ser_reg.tock(*this);
  joypad.tock(*this);
  sprite_scanner.tock(*this);
  lcd_reg.tock(*this);
  bus_mux.tock(*this);
  sprite_store.tock(*this);  // after bus mux
  win_reg.tock(*this); // after sprite store
  ppu_reg.tock(*this); // after window
  sprite_fetcher.tock(*this); // after window
  pix_pipe.tock(*this); // after window
  tile_fetcher.tock(*this); // after window
  int_reg.tock(*this);

  //----------
  // FF40 LCDC
  {
    /*p22.WORU*/ wire _WORU_FF40n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.XOLA_A00n(), top.int_bus.XENO_A01n(), top.int_bus.XUSY_A02n(), top.int_bus.XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not(_WORU_FF40n);

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and (_VOCA_FF40p, ASOT_CPU_RDp());
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not(_VYRE_FF40_RDp);

    /*p23.WYPO*/ int_bus.INT_TRI_D0.set_tribuf_6n(_WYCE_FF40_RDn, VYXE_LCDC_BGEN.q());
    /*p23.XERO*/ int_bus.INT_TRI_D1.set_tribuf_6n(_WYCE_FF40_RDn, XYLO_LCDC_SPEN.q());
    /*p23.WYJU*/ int_bus.INT_TRI_D2.set_tribuf_6n(_WYCE_FF40_RDn, XYMO_LCDC_SPSIZE.q());
    /*p23.WUKA*/ int_bus.INT_TRI_D3.set_tribuf_6n(_WYCE_FF40_RDn, XAFO_LCDC_BGMAP.q());
    /*p23.VOKE*/ int_bus.INT_TRI_D4.set_tribuf_6n(_WYCE_FF40_RDn, WEXU_LCDC_BGTILE.q());
    /*p23.VATO*/ int_bus.INT_TRI_D5.set_tribuf_6n(_WYCE_FF40_RDn, WYMO_LCDC_WINEN.q());
    /*p23.VAHA*/ int_bus.INT_TRI_D6.set_tribuf_6n(_WYCE_FF40_RDn, WOKY_LCDC_WINMAP.q());
    /*p23.XEBU*/ int_bus.INT_TRI_D7.set_tribuf_6n(_WYCE_FF40_RDn, XONA_LCDC_EN.q());

    /*p23.WARU*/ wire _WARU_FF40_WRp = and (_VOCA_FF40p, CUPA_CPU_WRp_xxxxEFGx());
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not(rst_reg.XORE_SYS_RSTp());
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D0.q());
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D1.q());
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D2.q());
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D3.q());
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D4.q());
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D5.q());
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D6.q());
    /*p23.XONA*/ XONA_LCDC_EN     .set(_XUBO_FF40_WRn, !_XUBO_FF40_WRn, _XARE_RSTn, top.int_bus.INT_TRI_D7.q());
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
  hash << joypad.commit(*this);
  hash << sprite_scanner.commit();
  hash << lcd_reg.commit(*this);
  hash << bus_mux.commit(*this);
 
  hash << sprite_store.commit(*this); // after bus mux
  hash << win_reg.commit(); // after sprite store
  hash << ppu_reg.commit(*this); // after sprite store
  hash << sprite_fetcher.commit(); // after window
  hash << pix_pipe.commit(*this); // after window
  hash << tile_fetcher.commit(*this); // after window
  hash << int_reg.commit();

  hash << SYS_PIN_RSTp.commit();
  hash << SYS_PIN_T2n.commit();
  hash << SYS_PIN_T1n.commit();

  //----------------------------------------

  hash << JOY_PIN_P10_C.commit();     // PIN_67-> KERY, KEVU
  hash << JOY_PIN_P11_C.commit();     // PIN_66-> KERY, P05.KAPA
  hash << JOY_PIN_P12_C.commit();     // PIN_65-> KERY, P05.KEJA
  hash << JOY_PIN_P13_C.commit();     // PIN_64-> KERY, P05.KOLO

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
/*p08.TUTY*/ if (!DBG_D_RDn) INT_TRI_D0 = not(/*p08.TOVO*/ not(pins.PIN_D0_C));
/*p08.SYWA*/ if (!DBG_D_RDn) INT_TRI_D1 = not(/*p08.RUZY*/ not(pins.PIN_D1_C));
/*p08.SUGU*/ if (!DBG_D_RDn) INT_TRI_D2 = not(/*p08.ROME*/ not(pins.PIN_D2_C));
/*p08.TAWO*/ if (!DBG_D_RDn) INT_TRI_D3 = not(/*p08.SAZA*/ not(pins.PIN_D3_C));
/*p08.TUTE*/ if (!DBG_D_RDn) INT_TRI_D4 = not(/*p08.TEHE*/ not(pins.PIN_D4_C));
/*p08.SAJO*/ if (!DBG_D_RDn) INT_TRI_D5 = not(/*p08.RATU*/ not(pins.PIN_D5_C));
/*p08.TEMY*/ if (!DBG_D_RDn) INT_TRI_D6 = not(/*p08.SOCA*/ not(pins.PIN_D6_C));
/*p08.ROPA*/ if (!DBG_D_RDn) INT_TRI_D7 = not(/*p08.RYBA*/ not(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug reg
  /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, CPU_PIN_A05, CPU_PIN_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, INT_TRI_D0);
  /*p07.AMUT*/ AMUT_FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, INT_TRI_D1);

  ///*p05.KURA*/ wire FF60_0n = not(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not(FF60);
  /*p07.BURO*/ dbg_reg.BURO_FF60_0.set(1, rst_sig.ALUR_RSTn, INT_TRI_D0);
  /*p07.AMUT*/ dbg_reg.AMUT_FF60_1.set(1, rst_sig.ALUR_RSTn, INT_TRI_D1);
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

/*p08.KEJO*/ top.int_bus.CPU_PIN_A00.set_tribuf_10n(TOVA_MODE_DBG2n, A00_Cn);
/*p08.BYXE*/ top.int_bus.CPU_PIN_A01.set_tribuf_10n(TOVA_MODE_DBG2n, A01_Cn);
/*p08.AKAN*/ top.int_bus.CPU_PIN_A02.set_tribuf_10n(TOVA_MODE_DBG2n, A02_Cn);
/*p08.ANAR*/ top.int_bus.CPU_PIN_A03.set_tribuf_10n(TOVA_MODE_DBG2n, A03_Cn);
/*p08.AZUV*/ top.int_bus.CPU_PIN_A04.set_tribuf_10n(TOVA_MODE_DBG2n, A04_Cn);
/*p08.AJOV*/ top.int_bus.CPU_PIN_A05.set_tribuf_10n(TOVA_MODE_DBG2n, A05_Cn);
/*p08.BYNE*/ top.int_bus.CPU_PIN_A06.set_tribuf_10n(TOVA_MODE_DBG2n, A06_Cn);
/*p08.BYNA*/ top.int_bus.CPU_PIN_A07.set_tribuf_10n(TOVA_MODE_DBG2n, A07_Cn);
/*p08.LOFA*/ top.int_bus.CPU_PIN_A08.set_tribuf_10n(TOVA_MODE_DBG2n, A08_Cn);
/*p08.MAPU*/ top.int_bus.CPU_PIN_A09.set_tribuf_10n(TOVA_MODE_DBG2n, A09_Cn);
/*p08.RALA*/ top.int_bus.CPU_PIN_A10.set_tribuf_10n(TOVA_MODE_DBG2n, A10_Cn);
/*p08.LORA*/ top.int_bus.CPU_PIN_A11.set_tribuf_10n(TOVA_MODE_DBG2n, A11_Cn);
/*p08.LYNA*/ top.int_bus.CPU_PIN_A12.set_tribuf_10n(TOVA_MODE_DBG2n, A12_Cn);
/*p08.LEFY*/ top.int_bus.CPU_PIN_A13.set_tribuf_10n(TOVA_MODE_DBG2n, A13_Cn);
/*p08.NEFE*/ top.int_bus.CPU_PIN_A14.set_tribuf_10n(TOVA_MODE_DBG2n, A14_Cn);
/*p08.SYZU*/ top.int_bus.CPU_PIN_A15.set_tribuf_10n(TOVA_MODE_DBG2n, A15_Cn);
}
#endif

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
