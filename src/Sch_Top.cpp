#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void SchematicTop::preset_sys(wire CLK_GOOD, wire CLK, wire RST, wire T1, wire T2) {
  SYS_PIN_CLK_A.preset(CLK_GOOD);
  SYS_PIN_CLK_B.preset(CLK);
  SYS_PIN_RSTn.preset(RST);
  SYS_PIN_T1p.preset(T1);
  SYS_PIN_T2p.preset(T2);
}

//-----------------------------------------------------------------------------

void SchematicTop::preset_cpu(uint16_t addr, uint8_t data, bool read, bool write) {

  CPU_PIN_A00.preset(addr & 0x0001);
  CPU_PIN_A01.preset(addr & 0x0002);
  CPU_PIN_A02.preset(addr & 0x0004);
  CPU_PIN_A03.preset(addr & 0x0008);
  CPU_PIN_A04.preset(addr & 0x0010);
  CPU_PIN_A05.preset(addr & 0x0020);
  CPU_PIN_A06.preset(addr & 0x0040);
  CPU_PIN_A07.preset(addr & 0x0080);
  CPU_PIN_A08.preset(addr & 0x0100);
  CPU_PIN_A09.preset(addr & 0x0200);
  CPU_PIN_A10.preset(addr & 0x0400);
  CPU_PIN_A11.preset(addr & 0x0800);
  CPU_PIN_A12.preset(addr & 0x1000);
  CPU_PIN_A13.preset(addr & 0x2000);
  CPU_PIN_A14.preset(addr & 0x4000);
  CPU_PIN_A15.preset(addr & 0x8000);

  CPU_PIN_RD.preset(read);
  CPU_PIN_WRp.preset(write);

  if (write) {
    CPU_TRI_D0.preset(data & 0x01);
    CPU_TRI_D1.preset(data & 0x02);
    CPU_TRI_D2.preset(data & 0x04);
    CPU_TRI_D3.preset(data & 0x08);
    CPU_TRI_D4.preset(data & 0x10);
    CPU_TRI_D5.preset(data & 0x20);
    CPU_TRI_D6.preset(data & 0x40);
    CPU_TRI_D7.preset(data & 0x80);
  }
  else {
    CPU_TRI_D0.preset(HIZ);
    CPU_TRI_D1.preset(HIZ);
    CPU_TRI_D2.preset(HIZ);
    CPU_TRI_D3.preset(HIZ);
    CPU_TRI_D4.preset(HIZ);
    CPU_TRI_D5.preset(HIZ);
    CPU_TRI_D6.preset(HIZ);
    CPU_TRI_D7.preset(HIZ);
  }

  CPU_PIN5.preset(HIZ);
  CPU_PIN6.preset(HIZ);
  CPU_PIN_ADDR_VALIDn.preset(HIZ);
  CPU_PIN_CLKREQ.preset(HIZ);

  CPU_PIN_ACK_JOYPAD.preset(HIZ);
  CPU_PIN_ACK_SERIAL.preset(HIZ);
  CPU_PIN_ACK_STAT.preset(HIZ);
  CPU_PIN_ACK_TIMER.preset(HIZ);
  CPU_PIN_ACK_VBLANK.preset(HIZ);
}

//-----------------------------------------------------------------------------

void SchematicTop::preset_ext() {
  EXT_PIN_A00_C.preset(HIZ);
  EXT_PIN_A01_C.preset(HIZ);
  EXT_PIN_A02_C.preset(HIZ);
  EXT_PIN_A03_C.preset(HIZ);
  EXT_PIN_A04_C.preset(HIZ);
  EXT_PIN_A05_C.preset(HIZ);
  EXT_PIN_A06_C.preset(HIZ);
  EXT_PIN_A07_C.preset(HIZ);
  EXT_PIN_A08_C.preset(HIZ);
  EXT_PIN_A09_C.preset(HIZ);
  EXT_PIN_A10_C.preset(HIZ);
  EXT_PIN_A11_C.preset(HIZ);
  EXT_PIN_A12_C.preset(HIZ);
  EXT_PIN_A13_C.preset(HIZ);
  EXT_PIN_A14_C.preset(HIZ);
  EXT_PIN_A15_C.preset(HIZ);

  EXT_PIN_D0_C.preset(HIZ);
  EXT_PIN_D1_C.preset(HIZ);
  EXT_PIN_D2_C.preset(HIZ);
  EXT_PIN_D3_C.preset(HIZ);
  EXT_PIN_D4_C.preset(HIZ);
  EXT_PIN_D5_C.preset(HIZ);
  EXT_PIN_D6_C.preset(HIZ);
  EXT_PIN_D7_C.preset(HIZ);
  EXT_PIN_RDn_C.preset(HIZ);
  EXT_PIN_WRn_C.preset(HIZ);
}

//-----------------------------------------------------------------------------

void SchematicTop::preset_joy(uint8_t buttons) {
  JOY_PIN_P10_C.preset(buttons & 0x01);
  JOY_PIN_P11_C.preset(buttons & 0x02);
  JOY_PIN_P12_C.preset(buttons & 0x04);
  JOY_PIN_P13_C.preset(buttons & 0x08);
}

//-----------------------------------------------------------------------------

void SchematicTop::preset_vram(wire OE, uint8_t data) {
  VRAM_PIN_MCSn_C.preset(HIZ);
  VRAM_PIN_MOEn_C.preset(HIZ);
  VRAM_PIN_MWRn_C.preset(HIZ);

  if (OE) {
    VRAM_PIN_MD0_C.preset(data & 0x01);
    VRAM_PIN_MD1_C.preset(data & 0x02);
    VRAM_PIN_MD2_C.preset(data & 0x04);
    VRAM_PIN_MD3_C.preset(data & 0x08);
    VRAM_PIN_MD4_C.preset(data & 0x10);
    VRAM_PIN_MD5_C.preset(data & 0x20);
    VRAM_PIN_MD6_C.preset(data & 0x40);
    VRAM_PIN_MD7_C.preset(data & 0x80);
  }
  else {
    VRAM_PIN_MD0_C.preset(HIZ);
    VRAM_PIN_MD1_C.preset(HIZ);
    VRAM_PIN_MD2_C.preset(HIZ);
    VRAM_PIN_MD3_C.preset(HIZ);
    VRAM_PIN_MD4_C.preset(HIZ);
    VRAM_PIN_MD5_C.preset(HIZ);
    VRAM_PIN_MD6_C.preset(HIZ);
    VRAM_PIN_MD7_C.preset(HIZ);
  }
}

//-----------------------------------------------------------------------------

void SchematicTop::preset_oam() {
  OAM_PIN_DA0.preset(HIZ);
  OAM_PIN_DA1.preset(HIZ);
  OAM_PIN_DA2.preset(HIZ);
  OAM_PIN_DA3.preset(HIZ);
  OAM_PIN_DA4.preset(HIZ);
  OAM_PIN_DA5.preset(HIZ);
  OAM_PIN_DA6.preset(HIZ);
  OAM_PIN_DA7.preset(HIZ);

  OAM_PIN_DB0.preset(HIZ);
  OAM_PIN_DB1.preset(HIZ);
  OAM_PIN_DB2.preset(HIZ);
  OAM_PIN_DB3.preset(HIZ);
  OAM_PIN_DB4.preset(HIZ);
  OAM_PIN_DB5.preset(HIZ);
  OAM_PIN_DB6.preset(HIZ);
  OAM_PIN_DB7.preset(HIZ);
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::tick() {
  SignalHash hash;

  clk_reg.tick(*this);

  //dbg_reg.tick(*this);
  //rst_reg.tick(*this);
  //tim_reg.tick(*this);

  //tick_vram_pins();
  //tick_top_regs();

  /*
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);

  joy_reg.tick(rst_sig, clk_reg, cpu_bus, cpu_sig);

  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  //cpu_sig.tick(*this);
  cpu_pins_out.tick(*this);
  vram_pins.tick(*this);
  */

  //----------

  hash << commit_io();
  hash << clk_reg.commit();

  //hash << dbg_reg.commit();
  //hash << rst_reg.commit();
  //hash << tim_reg.commit();
  //hash << lcd_reg.commit();
  //hash << pxp_reg.commit();
  //hash << sst_reg.commit();
  //hash << ppu_reg.commit();
  //hash << win_reg.commit();
  //hash << lcd_reg.commit();
  //hash << ser_reg.commit();
  //hash << commit_ibus();
  //hash << commit_vbus();
  //hash << commit_ebus();
  //hash << commit_obus();
  //hash << commit_sys_pins();
  //hash << commit_lcd_pins();
  //hash << commit_joy_pins();
  //hash << commit_top_regs();

  return hash;
}

//------------------------------------------------------------------------------


// TEVY box color wrong on die trace, but schematic correct.

// Die trace:
// SORE = not(A15)
// TEVY = or(A13, A13, SORE) // A13 line not fully drawn
// TEXO = and(ADDR_VALIDx?, TEVY)

// UJYV01 << UNOR04
// UJYV02 << (mux 1) RD_C
// UJYV03 << (mux 0) CPU_RAW_RD
// UJYV04 nc
// UJYV05 >> TEDO01

// UBAL1 << UNOR04
// UBAL2 << (mux1) << WR_C
// UBAL3 << (mux0) << APOV04
// UBAL4 nc
// UBAL5 >>

void SchematicTop::tick_cpu_bus() {
  auto& top = *this;

  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(top.CPU_PIN_CLKREQ);
    /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);

    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!top.AFUR_xBCDExxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(!top.APUK_xxxDEFGx());
    /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(top.ALEF_xxCDEFxx());
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
    /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
    /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());

    /*p01.BUVU*/ wire BUVU_xBxxxxxx = and(BUTY_CLKREQ, BALY_xBxxxxxx);
    /*p01.BYXO*/ wire BYXO_AxCDEFGH = not(BUVU_xBxxxxxx);
    /*p01.BEDO*/ wire BEDO_xBxxxxxx = not(BYXO_AxCDEFGH);
    /*p01.BOWA*/ wire BOWA_AxCDEFGH = not(BEDO_xBxxxxxx);
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p01.BUGO*/ wire BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
    /*p01.BATE*/ wire BATE_ABxxxxxH = nor(ABOL_CLKREQn,  BUGO_xxCDEFxx, AROV_xxxDEFGx);
    /*p01.BASU*/ wire BASU_xxCDEFGx = not(BATE_ABxxxxxH);
    /*p01.BUKE*/ wire BUKE_ABxxxxxH = not(BASU_xxCDEFGx);

    /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);

    top.CPU_PIN_BOWA_AxCDEFGH.set(BOWA_AxCDEFGH);
    top.CPU_PIN_BEDO_xBxxxxxx.set(BEDO_xBxxxxxx);
    top.CPU_PIN_BEKO_xBCDExxx.set(BEKO_xBCDExxx);
    top.CPU_PIN_BUDE_AxxxxFGH.set(BUDE_AxxxxFGH);
    top.CPU_PIN_BOLO_xBCDEFGx.set(BOLO_xBCDEFGx);
    top.CPU_PIN_BUKE_ABxxxxxH.set(BUKE_ABxxxxxH);
    top.CPU_PIN_BOMA_xBxxxxxx.set(BOMA_xBxxxxxx);
    top.CPU_PIN_BOGA_AxCDEFGH.set(BOGA_AxCDEFGH);
  }

  {
    wire ABOL_CLKREQn  = not(top.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(top.AFUR_xBCDExxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /* PIN_75 */ top.EXT_PIN_CLK.set(BUDE_AxxxxFGH);
  }

  {
    top.CPU_PIN_EXT_RESET.set(top.SYS_PIN_RSTn);
  }

  {
    top.CPU_PIN_EXT_CLKGOOD.set(top.SYS_PIN_CLK_A);
  }

  {
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    top.CPU_PIN_ADDR_HI.set(SYRO_FE00_FFFFp);
  }
}

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or(top.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, CPU_PIN_A05, CPU_PIN_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_reg.BEDO_xBxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_reg.BEDO_xBxxxxxx, sys_sig.MODE_DBG2);

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
void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- CPU DBG/PIN_RST -----\n");
  text_painter.dprintf("CPU_PIN_EXT_RESET     %d\n", CPU_PIN_EXT_RESET.a.val);
  text_painter.dprintf("CPU_PIN_DBG     %d\n", CPU_PIN_DBG.a.val);
  text_painter.dprintf("CPU_PIN_EXT_CLKGOOD   %d\n", CPU_PIN_EXT_CLKGOOD.a.val);

  text_painter.dprintf("----- CPU CLOCKS -----\n");

  text_painter.dprintf("CPU_PIN_BOWA_AxCDEFGH %d\n", CPU_PIN_BOWA_AxCDEFGH.a.val);
  text_painter.dprintf("CPU_PIN_BEDO_xBxxxxxx %d\n", CPU_PIN_BEDO_xBxxxxxx.a.val);
  text_painter.dprintf("CPU_PIN_BEKO_xBCDExxx %d\n", CPU_PIN_BEKO_xBCDExxx.a.val);
  text_painter.dprintf("CPU_PIN_BUDE_AxxxxFGH %d\n", CPU_PIN_BUDE_AxxxxFGH.a.val);
  text_painter.dprintf("CPU_PIN_BOLO_xBCDEFGx %d\n", CPU_PIN_BOLO_xBCDEFGx.a.val);
  text_painter.dprintf("CPU_PIN_BUKE_ABxxxxxH %d\n", CPU_PIN_BUKE_ABxxxxxH.a.val);
  text_painter.dprintf("CPU_PIN_BOMA_xBxxxxxx %d\n", CPU_PIN_BOMA_xBxxxxxx.a.val);
  text_painter.dprintf("CPU_PIN_BOGA_AxCDEFGH %d\n", CPU_PIN_BOGA_AxCDEFGH.a.val);

  text_painter.dprintf("----- FROM CPU -----\n");
  //text_painter.dprintf("CPU_PIN_CLKREQ        %d\n", CPU_PIN_CLKREQ.a.val);
  //text_painter.dprintf("CPU_PIN5     %d\n", CPU_PIN5.a.val);
  //text_painter.dprintf("CPU_PIN6     %d\n", CPU_PIN6.a.val);

  text_painter.dprintf("----- TO CPU -----\n");
  text_painter.dprintf("CPU_PIN_PROD     %d\n", CPU_PIN_PROD.a.val);
  text_painter.dprintf("CPU_PIN_WAKE          %d\n", CPU_PIN_WAKE.a.val);
  text_painter.dprintf("CPU_PIN_ADDR_HI          %d\n", CPU_PIN_ADDR_HI.a.val);
  text_painter.dprintf("CPU_PIN_BOOTp    %d\n", CPU_PIN_BOOTp.a.val);

  /*
  text_painter.dprintf("----- CPU INT -----\n");
  text_painter.dprintf("CPU_PIN_INT_VBLANK    %d\n", CPU_PIN_INT_VBLANK.a.val);
  text_painter.dprintf("CPU_PIN_INT_STAT      %d\n", CPU_PIN_INT_STAT.a.val);
  text_painter.dprintf("CPU_PIN_INT_TIMER     %d\n", CPU_PIN_INT_TIMER.a.val);
  text_painter.dprintf("CPU_PIN_INT_SERIAL    %d\n", CPU_PIN_INT_SERIAL.a.val);
  text_painter.dprintf("CPU_PIN_INT_JOYPAD    %d\n", CPU_PIN_INT_JOYPAD.a.val);
  text_painter.dprintf("CPU_PIN_ACK_VBLANK    %d\n", CPU_PIN_ACK_VBLANK.a.val);
  text_painter.dprintf("CPU_PIN_ACK_STAT      %d\n", CPU_PIN_ACK_STAT.a.val);
  text_painter.dprintf("CPU_PIN_ACK_TIMER     %d\n", CPU_PIN_ACK_TIMER.a.val);
  text_painter.dprintf("CPU_PIN_ACK_SERIAL    %d\n", CPU_PIN_ACK_SERIAL.a.val);
  text_painter.dprintf("CPU_PIN_ACK_JOYPAD    %d\n", CPU_PIN_ACK_JOYPAD.a.val);
  */

  /*
  text_painter.dprintf("----- CPU BUS -----\n");
  text_painter.dprintf("CPU_PIN_RD    %d\n", CPU_PIN_RD.a.val);
  text_painter.dprintf("CPU_PIN_WR    %d\n", CPU_PIN_WR.a.val);
  text_painter.dprintf("CPU_PIN_ADDR_VALID    %d\n", CPU_PIN_ADDR_VALID.a.val);
  */

  /*
  text_painter.add_text("Axx ");
  dump2(text_painter, CPU_PIN_A15.a);
  dump2(text_painter, CPU_PIN_A14.a);
  dump2(text_painter, CPU_PIN_A13.a);
  dump2(text_painter, CPU_PIN_A12.a);
  text_painter.add_char(':');
  dump2(text_painter, CPU_PIN_A11.a);
  dump2(text_painter, CPU_PIN_A10.a);
  dump2(text_painter, CPU_PIN_A09.a);
  dump2(text_painter, CPU_PIN_A08.a);
  text_painter.add_char(':');
  dump2(text_painter, CPU_PIN_A07.a);
  dump2(text_painter, CPU_PIN_A06.a);
  dump2(text_painter, CPU_PIN_A05.a);
  dump2(text_painter, CPU_PIN_A04.a);
  text_painter.add_char(':');
  dump2(text_painter, CPU_PIN_A03.a);
  dump2(text_painter, CPU_PIN_A02.a);
  dump2(text_painter, CPU_PIN_A01.a);
  dump2(text_painter, CPU_PIN_A00.a);
  text_painter.newline();
  */

  /*
  text_painter.add_text("Dxx ");
  dump2(text_painter, BUS_CPU_D7.a);
  dump2(text_painter, BUS_CPU_D6.a);
  dump2(text_painter, BUS_CPU_D5.a);
  dump2(text_painter, BUS_CPU_D4.a);
  text_painter.add_char(':');
  dump2(text_painter, BUS_CPU_D3.a);
  dump2(text_painter, BUS_CPU_D2.a);
  dump2(text_painter, BUS_CPU_D1.a);
  dump2(text_painter, BUS_CPU_D0.a);
  text_painter.newline();
  */

  text_painter.newline();
}

/*
void preset_data(bool oe, uint8_t data) {
D0.preset(oe, data & 0x01);
D1.preset(oe, data & 0x02);
D2.preset(oe, data & 0x04);
D3.preset(oe, data & 0x08);
D4.preset(oe, data & 0x10);
D5.preset(oe, data & 0x20);
D6.preset(oe, data & 0x40);
D7.preset(oe, data & 0x80);
}
*/

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

/*p08.KEJO*/ top.CPU_PIN_A00.set(!TOVA_MODE_DBG2n, not(A00_Cn));
/*p08.BYXE*/ top.CPU_PIN_A01.set(!TOVA_MODE_DBG2n, not(A01_Cn));
/*p08.AKAN*/ top.CPU_PIN_A02.set(!TOVA_MODE_DBG2n, not(A02_Cn));
/*p08.ANAR*/ top.CPU_PIN_A03.set(!TOVA_MODE_DBG2n, not(A03_Cn));
/*p08.AZUV*/ top.CPU_PIN_A04.set(!TOVA_MODE_DBG2n, not(A04_Cn));
/*p08.AJOV*/ top.CPU_PIN_A05.set(!TOVA_MODE_DBG2n, not(A05_Cn));
/*p08.BYNE*/ top.CPU_PIN_A06.set(!TOVA_MODE_DBG2n, not(A06_Cn));
/*p08.BYNA*/ top.CPU_PIN_A07.set(!TOVA_MODE_DBG2n, not(A07_Cn));
/*p08.LOFA*/ top.CPU_PIN_A08.set(!TOVA_MODE_DBG2n, not(A08_Cn));
/*p08.MAPU*/ top.CPU_PIN_A09.set(!TOVA_MODE_DBG2n, not(A09_Cn));
/*p08.RALA*/ top.CPU_PIN_A10.set(!TOVA_MODE_DBG2n, not(A10_Cn));
/*p08.LORA*/ top.CPU_PIN_A11.set(!TOVA_MODE_DBG2n, not(A11_Cn));
/*p08.LYNA*/ top.CPU_PIN_A12.set(!TOVA_MODE_DBG2n, not(A12_Cn));
/*p08.LEFY*/ top.CPU_PIN_A13.set(!TOVA_MODE_DBG2n, not(A13_Cn));
/*p08.NEFE*/ top.CPU_PIN_A14.set(!TOVA_MODE_DBG2n, not(A14_Cn));
/*p08.SYZU*/ top.CPU_PIN_A15.set(!TOVA_MODE_DBG2n, not(A15_Cn));
}
#endif

//---------------------------------------------------------------------------

#if 0

  if (UNOR_MODE_DBG2p) {
    if (!dbg_reg.SOTO_DBG) {
      wire SERE_VRAM_RDp = and (!VRAM_PIN_MCSn_C, !XYMU_RENDERINGp);
    }
    else {
      wire SERE_VRAM_RDp = and (SOSE_8000_9FFFp, !XYMU_RENDERINGp);
    }
  }
  else {
    wire SERE_VRAM_RDp = and (SOSE_8000_9FFFp, !CPU_PIN_ADDR_VALIDn, !XYMU_RENDERINGp);
  }

#endif

#if 0

  if (TUTO_DBG_VRAMp) {
    // VRAM_PIN_MCSn hiz
    VRAM_PIN_MCSn_A.set(1);
    VRAM_PIN_MCSn_D.set(0);
  }
  else {
    if (SUTU_MCSn) {
      // VRAM_PIN_MCSn high
      VRAM_PIN_MCSn_A.set(0);
      VRAM_PIN_MCSn_D.set(0);
    }
    else {
      // VRAM_PIN_MCSn low
      VRAM_PIN_MCSn_A.set(1);
      VRAM_PIN_MCSn_D.set(1);
    }
  }

#endif


void SchematicTop::tick_vram_pins() {

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp = and (UNOR_MODE_DBG2p(), !dbg_reg.SOTO_DBG);
  /*p25.RACO*/ wire RACO_DBG_VRAMn = not(TUTO_DBG_VRAMp);

  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH());
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p01.AREV*/ nwire AREV_CPU_WRn_ABCDExxx = nand(CPU_PIN_WRp, AFAS_xxxxxFGH);
  /*p01.APOV*/ pwire APOV_CPU_WRp_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);



  // the logic here is kinda weird, still seems to select vram.
  /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
  /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
  /*p08.SORE*/ wire SORE_0000_7FFFp = not(CPU_PIN_A15);
  /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(CPU_PIN_A13, CPU_PIN_A14, SORE_0000_7FFFp);
  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (CPU_PIN_ADDR_VALIDn, TEVY_8000_9FFFn);
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (CPU_PIN_A15, TEFA_8000_9FFFp);

  /*p??.APAP*/ pwire APAP_CPU_ADDR_VALIDp = not(CPU_PIN_ADDR_VALIDn); // Missing from schematic
  /*p01.AWOD*/ nwire AWOD_CPU_ADDR_VALIDn = nor(UNOR_MODE_DBG2p(), APAP_CPU_ADDR_VALIDp);
  /*p01.ABUZ*/ pwire ABUZ_CPU_ADDR_VALIDp = not(AWOD_CPU_ADDR_VALIDn);

  /*p25.TUCA*/ pwire _TUCA_CPU_VRAM_RDp = and (SOSE_8000_9FFFp, ABUZ_CPU_ADDR_VALIDp);
  /*p25.TEFY*/ pwire _TEFY_VRAM_MCSp    = not(VRAM_PIN_MCSn_C);
  /*p25.TOLE*/ pwire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, TUTO_DBG_VRAMp);
  /*p25.ROPY*/ nwire ROPY_RENDERINGn    = not(XYMU_RENDERINGp());
  /*p25.SERE*/ pwire SERE_VRAM_RDp     = and (_TOLE_VRAM_RDp, ROPY_RENDERINGn);

  /*p25.TEGU*/ nwire TEGU_CPU_VRAM_WRn = nand(SOSE_8000_9FFFp, CPU_PIN_WRp); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.TAVY*/ pwire TAVY_MOEp = not(VRAM_PIN_MOEn_C);
  /*p25.SALE*/ nwire SALE_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, TUTO_DBG_VRAMp);

  /*p25.TUJA*/ pwire TUJA_CPU_VRAM_WRp = and(SOSE_8000_9FFFp, APOV_CPU_WRp_xxxxxFGH);
  /*p25.SUDO*/ pwire SUDO_MWRp_C = not(VRAM_PIN_MWRn_C);
  /*p25.TYJY*/ pwire TYJY_DBG_VRAM_WRp = mux2_p(SUDO_MWRp_C, TUJA_CPU_VRAM_WRp, TUTO_DBG_VRAMp);
  /*p25.SOHY*/ wire SOHY_MWRn    = nand(TYJY_DBG_VRAM_WRp, SERE_VRAM_RDp);

  /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn   = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
  /*p04.LUFA*/ wire LUFA_DMA_VRAM_RDp = not(MUHO_DMA_VRAM_RDn);
  /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_VRAM_RDp, XYMU_RENDERINGp()); // def nor

  /*p25.RYLU*/ wire RYLU_DBG_VRAM_RDn = nand(SALE_VRAM_WRn, XANE_VRAM_LOCKn);
  /*p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not(SOHO_SPR_VRAM_RDp());
  /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp()); // this should be correct
  /*p25.APAM*/ wire APAM_DMA_VRAM_RDn = not(LUFA_DMA_VRAM_RDp);
  /*p25.RACU*/ wire RACU_MOEn   = and (RYLU_DBG_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAM_RDn); // def and


  /*p04.MUHO*/ nwire MUHO_DMA_READ_VRAMn = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
  /*p04.LUFA*/ pwire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
  /*p29.ABON*/ nwire ABON_SPR_READ_VRAMn = not(TEXY_SPR_READ_VRAMp());

  // ABON = not(TEXY)
  // SUTU = nor(LENA, LUFA, ABON, SERE);

  // Polarity issues here, ABON should be P

  /*p27.LUSU*/ nwire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp());
  /*p27.LENA*/ pwire LENA_BGW_VRAM_RDp = not(LUSU_BGW_VRAM_RDn);
  /*p25.SUTU*/ wire SUTU_MCSn = nor(LENA_BGW_VRAM_RDp, LUFA_DMA_READ_VRAMp, ABON_SPR_READ_VRAMn, SERE_VRAM_RDp);

  {
    /*p25.TAXY*/ wire TAXY_MWRn_A = and(SOHY_MWRn, RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire SOFY_MWRn_D = or (SOHY_MWRn, TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire SYSY_MWRp_A = not(TAXY_MWRn_A);
    /*p25.RAGU*/ wire RAGU_MWRp_D = not(SOFY_MWRn_D);
    /*p25.SYSY*/ VRAM_PIN_MWRn_A.set(SYSY_MWRp_A);
    /*p25.RAGU*/ VRAM_PIN_MWRn_D.set(RAGU_MWRp_D);
  }

  {
    /*p25.SEMA*/ wire SEMA_MOEn_A = and(RACU_MOEn, RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire RUTE_MOEn_D = or (RACU_MOEn, TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire REFO_MOEn_A = not(SEMA_MOEn_A);
    /*p25.SAHA*/ wire SAHA_MOEn_D = not(RUTE_MOEn_D);
    /*p25.REFO*/ VRAM_PIN_MOEn_A.set(REFO_MOEn_A);
    /*p25.SAHA*/ VRAM_PIN_MOEn_D.set(SAHA_MOEn_D);
  }
  {
    /*p25.TODE*/ wire TODE_MCSn_A = and(SUTU_MCSn, RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire SEWO_MCSn_D = or (SUTU_MCSn, TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire SOKY_MCSp_A = not(TODE_MCSn_A);
    /*p25.SETY*/ wire SETY_MCSp_D = not(SEWO_MCSn_D);
    /*p25.SOKY*/ VRAM_PIN_MCSn_A.set(SOKY_MCSp_A);
    /*p25.SETY*/ VRAM_PIN_MCSn_D.set(SETY_MCSp_D);
  }

  {
    /*p25.LEXE*/ VRAM_PIN_MA00_AD.set(not(/*p25.MYFU*/ not(VRM_TRI_A00)));
    /*p25.LOZU*/ VRAM_PIN_MA01_AD.set(not(/*p25.MASA*/ not(VRM_TRI_A01)));
    /*p25.LACA*/ VRAM_PIN_MA02_AD.set(not(/*p25.MYRE*/ not(VRM_TRI_A02)));
    /*p25.LUVO*/ VRAM_PIN_MA03_AD.set(not(/*p25.MAVU*/ not(VRM_TRI_A03)));
    /*p25.LOLY*/ VRAM_PIN_MA04_AD.set(not(/*p25.MEPA*/ not(VRM_TRI_A04)));
    /*p25.LALO*/ VRAM_PIN_MA05_AD.set(not(/*p25.MYSA*/ not(VRM_TRI_A05)));
    /*p25.LEFA*/ VRAM_PIN_MA06_AD.set(not(/*p25.MEWY*/ not(VRM_TRI_A06)));
    /*p25.LUBY*/ VRAM_PIN_MA07_AD.set(not(/*p25.MUME*/ not(VRM_TRI_A07)));
    /*p25.TUJY*/ VRAM_PIN_MA08_AD.set(not(/*p25.VOVA*/ not(VRM_TRI_A08)));
    /*p25.TAGO*/ VRAM_PIN_MA09_AD.set(not(/*p25.VODE*/ not(VRM_TRI_A09)));
    /*p25.NUVA*/ VRAM_PIN_MA10_AD.set(not(/*p25.RUKY*/ not(VRM_TRI_A10)));
    /*p25.PEDU*/ VRAM_PIN_MA11_AD.set(not(/*p25.RUMA*/ not(VRM_TRI_A11)));
    /*p25.PONY*/ VRAM_PIN_MA12_AD.set(not(/*p25.REHO*/ not(VRM_TRI_A12)));
  }

  {
    /*p25.RUVY*/ wire _RUVY_VRAM_WR = not(SALE_VRAM_WRn);
    /*p25.SAZO*/ wire _SAZO_VRAM_RD  = and (_RUVY_VRAM_WR, SERE_VRAM_RDp);

    /*p25.RYJE*/ nwire _RYJE_VRAM_RDn = not(_SAZO_VRAM_RD);
    /*p25.REVO*/ pwire _REVO_VRAM_RDp = not(_RYJE_VRAM_RDn);

    // this is ORing a signal with a delayed version of itself?
    /*p25.RELA*/ wire _RELA_VBUS_DIR_OUT   = or (_REVO_VRAM_RDp, _SAZO_VRAM_RD);

    /*p25.ROCY*/ pwire _ROCY_VBUS_TRISTATEn = and (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RAHU*/ nwire _RAHU_VBUS_DIR_IN = not(_ROCY_VBUS_TRISTATEn);
    /*p25.ROVE*/ pwire _ROVE_VBUS_DIR_OUT = not(_RAHU_VBUS_DIR_IN);

    /*p25.SEFA*/ wire SEFA = and(VRM_TRI_D0, _ROVE_VBUS_DIR_OUT);
    /*p25.SOGO*/ wire SOGO = and(VRM_TRI_D1, _ROVE_VBUS_DIR_OUT);
    /*p25.SEFU*/ wire SEFU = and(VRM_TRI_D2, _ROVE_VBUS_DIR_OUT);
    /*p25.SUNA*/ wire SUNA = and(VRM_TRI_D3, _ROVE_VBUS_DIR_OUT);
    /*p25.SUMO*/ wire SUMO = and(VRM_TRI_D4, _ROVE_VBUS_DIR_OUT);
    /*p25.SAZU*/ wire SAZU = and(VRM_TRI_D5, _ROVE_VBUS_DIR_OUT);
    /*p25.SAMO*/ wire SAMO = and(VRM_TRI_D6, _ROVE_VBUS_DIR_OUT);
    /*p25.SUKE*/ wire SUKE = and(VRM_TRI_D7, _ROVE_VBUS_DIR_OUT);

    /*p25.SYNU*/ wire SYNU = or (VRM_TRI_D0, _RAHU_VBUS_DIR_IN);
    /*p25.SYMA*/ wire SYMA = or (VRM_TRI_D1, _RAHU_VBUS_DIR_IN);
    /*p25.ROKO*/ wire ROKO = or (VRM_TRI_D2, _RAHU_VBUS_DIR_IN);
    /*p25.SYBU*/ wire SYBU = or (VRM_TRI_D3, _RAHU_VBUS_DIR_IN);
    /*p25.SAKO*/ wire SAKO = or (VRM_TRI_D4, _RAHU_VBUS_DIR_IN);
    /*p25.SEJY*/ wire SEJY = or (VRM_TRI_D5, _RAHU_VBUS_DIR_IN);
    /*p25.SEDO*/ wire SEDO = or (VRM_TRI_D6, _RAHU_VBUS_DIR_IN);
    /*p25.SAWU*/ wire SAWU = or (VRM_TRI_D7, _RAHU_VBUS_DIR_IN);

    /*p25.REGE*/ wire REGE = not(SEFA);
    /*p25.RYKY*/ wire RYKY = not(SOGO);
    /*p25.RAZO*/ wire RAZO = not(SEFU);
    /*p25.RADA*/ wire RADA = not(SUNA);
    /*p25.RYRO*/ wire RYRO = not(SUMO);
    /*p25.REVU*/ wire REVU = not(SAZU);
    /*p25.REKU*/ wire REKU = not(SAMO);
    /*p25.RYZE*/ wire RYZE = not(SUKE);

    /*p25.RURA*/ wire RURA = not(SYNU);
    /*p25.RULY*/ wire RULY = not(SYMA);
    /*p25.RARE*/ wire RARE = not(ROKO);
    /*p25.RODU*/ wire RODU = not(SYBU);
    /*p25.RUBE*/ wire RUBE = not(SAKO);
    /*p25.RUMU*/ wire RUMU = not(SEJY);
    /*p25.RYTY*/ wire RYTY = not(SEDO);
    /*p25.RADY*/ wire RADY = not(SAWU);

    VRAM_PIN_MD0_A.set(REGE);
    VRAM_PIN_MD1_A.set(RYKY);
    VRAM_PIN_MD2_A.set(RAZO);
    VRAM_PIN_MD3_A.set(RADA);
    VRAM_PIN_MD4_A.set(RYRO);
    VRAM_PIN_MD5_A.set(REVU);
    VRAM_PIN_MD6_A.set(REKU);
    VRAM_PIN_MD7_A.set(RYZE);

    VRAM_PIN_MD0_D.set(RURA);
    VRAM_PIN_MD1_D.set(RULY);
    VRAM_PIN_MD2_D.set(RARE);
    VRAM_PIN_MD3_D.set(RODU);
    VRAM_PIN_MD4_D.set(RUBE);
    VRAM_PIN_MD5_D.set(RUMU);
    VRAM_PIN_MD6_D.set(RYTY);
    VRAM_PIN_MD7_D.set(RADY);

    /*p25.TEME*/ VRM_TRI_D0.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D0);
    /*p25.TEWU*/ VRM_TRI_D1.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D1);
    /*p25.TYGO*/ VRM_TRI_D2.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D2);
    /*p25.SOTE*/ VRM_TRI_D3.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D3);
    /*p25.SEKE*/ VRM_TRI_D4.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D4);
    /*p25.RUJO*/ VRM_TRI_D5.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D5);
    /*p25.TOFA*/ VRM_TRI_D6.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D6);
    /*p25.SUZA*/ VRM_TRI_D7.set_tribuf_10(_RAHU_VBUS_DIR_IN, CPU_TRI_D7);

    {
      /*p25.RENA*/ wire _RENA_VBUS_DIR_IN = not(_RELA_VBUS_DIR_OUT);
      /*p25.RODY*/ VRM_TRI_D0.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD0_C);
      /*p25.REBA*/ VRM_TRI_D1.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD1_C);
      /*p25.RYDO*/ VRM_TRI_D2.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD2_C);
      /*p25.REMO*/ VRM_TRI_D3.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD3_C);
      /*p25.ROCE*/ VRM_TRI_D4.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD4_C);
      /*p25.ROPU*/ VRM_TRI_D5.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD5_C);
      /*p25.RETA*/ VRM_TRI_D6.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD6_C);
      /*p25.RAKU*/ VRM_TRI_D7.set_tribuf(_RENA_VBUS_DIR_IN, VRAM_PIN_MD7_C);

      /*p25.ROFA*/ wire _ROFA_VBUS_DIR_OUT = not(_RENA_VBUS_DIR_IN);
      /*p25.ROFA*/ VRAM_PIN_MD0_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD1_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD2_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD3_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD4_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD5_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD6_B.set(_ROFA_VBUS_DIR_OUT);
      /*p25.ROFA*/ VRAM_PIN_MD7_B.set(_ROFA_VBUS_DIR_OUT);
    }
  }

  {
    /*p04.DECY*/ nwire DECY_FROM_CPU5n = not(CPU_PIN5);
    /*p04.CATY*/ pwire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n);
    /*p07.TEDO*/ pwire TEDO_CPU_RDp = not(UJYV_CPU_RDn());
    /*p08.REDU*/ nwire REDU_CPU_RDn = not(TEDO_CPU_RDp);
    /*p07.AJAS*/ nwire AJAS_CPU_RDn = not(TEDO_CPU_RDp);
    /*p07.ASOT*/ pwire ASOT_CPU_RDp = not(AJAS_CPU_RDn);
    /*p28.MYNU*/ nwire MYNU_CPU_RDn = nand(ASOT_CPU_RDp, CATY_FROM_CPU5p);
    /*p28.LEKO*/ pwire LEKO_CPU_RDp = not(MYNU_CPU_RDn);

    /*p25.TYVY*/ nwire TYVY_VRAMD_TO_CPUDn = nand(SERE_VRAM_RDp, LEKO_CPU_RDp);
    /*p25.SEBY*/ pwire SEBY_VRAMD_TO_CPUDp = not(TYVY_VRAMD_TO_CPUDn);

    /*p25.RERY*/ wire RERY = !not(VRM_TRI_D0);
    /*p25.RUNA*/ wire RUNA = !not(VRM_TRI_D1);
    /*p25.RONA*/ wire RONA = !not(VRM_TRI_D2);
    /*p25.RUNO*/ wire RUNO = !not(VRM_TRI_D3);
    /*p25.SANA*/ wire SANA = !not(VRM_TRI_D4);
    /*p25.RORO*/ wire RORO = !not(VRM_TRI_D5);
    /*p25.RABO*/ wire RABO = !not(VRM_TRI_D6);
    /*p25.SAME*/ wire SAME = !not(VRM_TRI_D7);

    /*p25.RUGA*/ CPU_TRI_D0.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RERY);
    /*p25.ROTA*/ CPU_TRI_D1.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RUNA);
    /*p25.RYBU*/ CPU_TRI_D2.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RONA);
    /*p25.RAJU*/ CPU_TRI_D3.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RUNO);
    /*p25.TYJA*/ CPU_TRI_D4.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, SANA);
    /*p25.REXU*/ CPU_TRI_D5.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RORO);
    /*p25.RUPY*/ CPU_TRI_D6.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, RABO);
    /*p25.TOKU*/ CPU_TRI_D7.set_tribuf_6p(SEBY_VRAMD_TO_CPUDp, SAME);
  }

}

//-----------------------------------------------------------------------------

void SchematicTop::tick_top_regs() {
  /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp());
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
  /*p01.XARE*/ wire XARE_RSTn = not(XORE_RSTp);

  /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_CPU_RDn());
  /*p07.AJAS*/ wire AJAS_CPU_RD = not(TEDO_CPU_RD);
  /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_CPU_RD);

  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_CPU_WRp_ABCDExxx());
  /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);

  // FF40 LCDC
  {
    /*p22.XOLA*/ wire XOLA_A00n = not(CPU_PIN_A00);
    /*p22.XENO*/ wire XENO_A01n = not(CPU_PIN_A01);
    /*p22.XUSY*/ wire XUSY_A02n = not(CPU_PIN_A02);
    /*p22.XERA*/ wire XERA_A03n = not(CPU_PIN_A03);
    /*p22.WORU*/ nwire WORU_FF40n = nand(WERO_FF4Xp(), XOLA_A00n, XENO_A01n, XUSY_A02n, XERA_A03n);
    /*p22.VOCA*/ pwire VOCA_FF40p = not(WORU_FF40n);

    /*p23.VYRE*/ pwire VYRE_FF40_RDp = and (VOCA_FF40p, ASOT_CPU_RD);
    /*p23.WYCE*/ nwire WYCE_FF40_RDn = not(VYRE_FF40_RDp);

    /*p23.WYPO*/ CPU_TRI_D0.set_tribuf_6n(WYCE_FF40_RDn, VYXE_LCDC_BGEN);
    /*p23.XERO*/ CPU_TRI_D1.set_tribuf_6n(WYCE_FF40_RDn, XYLO_LCDC_SPEN);
    /*p23.WYJU*/ CPU_TRI_D2.set_tribuf_6n(WYCE_FF40_RDn, XYMO_LCDC_SPSIZE);
    /*p23.WUKA*/ CPU_TRI_D3.set_tribuf_6n(WYCE_FF40_RDn, XAFO_LCDC_BGMAP);
    /*p23.VOKE*/ CPU_TRI_D4.set_tribuf_6n(WYCE_FF40_RDn, WEXU_LCDC_BGTILE);
    /*p23.VATO*/ CPU_TRI_D5.set_tribuf_6n(WYCE_FF40_RDn, WYMO_LCDC_WINEN);
    /*p23.VAHA*/ CPU_TRI_D6.set_tribuf_6n(WYCE_FF40_RDn, WOKY_LCDC_WINMAP);
    /*p23.XEBU*/ CPU_TRI_D7.set_tribuf_6n(WYCE_FF40_RDn, XONA_LCDC_EN);

    /*p23.WARU*/ wire WARU_FF40_WRp = and (VOCA_FF40p, CUPA_CPU_WR_xxxxxFGH);
    /*p23.XUBO*/ wire XUBO_FF40_WRn = not(WARU_FF40_WRp);

    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D0);
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D1);
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D2);
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D3);
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D4);
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D5);
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D6);
    /*p23.XONA*/ XONA_LCDC_EN     .set(XUBO_FF40_WRn, !XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D7);
  }
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_io() {
  SignalHash hash;

  hash << SYS_PIN_RSTn.clear_preset();          // PIN_71
  hash << SYS_PIN_CLK_B.clear_preset(); // PIN_74
  hash << SYS_PIN_CLK_A.clear_preset();     // PIN_74
  hash << SYS_PIN_T2p.clear_preset();           // PIN_76
  hash << SYS_PIN_T1p.clear_preset();           // PIN_77

  //----------------------------------------

  hash << CPU_PIN_RD.clear_preset();             // PORTA_00: -> UJYV, LAGU, LAVO
  hash << CPU_PIN_WRp.clear_preset();             // PORTA_01: -> AREV, LAGU.
  hash << CPU_PIN_ADDR_VALIDn.clear_preset();     // PORTA_06: -> APAP, TEXO

  hash << CPU_PIN_ACK_VBLANK.clear_preset();     // PORTB_01: -> LETY, vblank int ack
  hash << CPU_PIN_ACK_STAT.clear_preset();       // PORTB_05: -> LEJA, stat int ack
  hash << CPU_PIN_ACK_TIMER.clear_preset();      // PORTB_09: -> LESA, timer int ack
  hash << CPU_PIN_ACK_SERIAL.clear_preset();     // PORTB_13: -> LUFE, serial int ack
  hash << CPU_PIN_ACK_JOYPAD.clear_preset();     // PORTB_17: -> LAMO, joypad int ack
  hash << CPU_PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL
  hash << CPU_PIN6.clear_preset();               // PORTD_00: -> LEXY, doesn't do anything
  hash << CPU_PIN5.clear_preset();               // PORTD_06: -> FROM_CPU5

  hash << CPU_PIN_A00.clear_preset();
  hash << CPU_PIN_A01.clear_preset();
  hash << CPU_PIN_A02.clear_preset();
  hash << CPU_PIN_A03.clear_preset();
  hash << CPU_PIN_A04.clear_preset();
  hash << CPU_PIN_A05.clear_preset();
  hash << CPU_PIN_A06.clear_preset();
  hash << CPU_PIN_A07.clear_preset();
  hash << CPU_PIN_A08.clear_preset();
  hash << CPU_PIN_A09.clear_preset();
  hash << CPU_PIN_A10.clear_preset();
  hash << CPU_PIN_A11.clear_preset();
  hash << CPU_PIN_A12.clear_preset();
  hash << CPU_PIN_A13.clear_preset();
  hash << CPU_PIN_A14.clear_preset();
  hash << CPU_PIN_A15.clear_preset();

  hash << CPU_TRI_D0.commit_tribuf();
  hash << CPU_TRI_D1.commit_tribuf();
  hash << CPU_TRI_D2.commit_tribuf();
  hash << CPU_TRI_D3.commit_tribuf();
  hash << CPU_TRI_D4.commit_tribuf();
  hash << CPU_TRI_D5.commit_tribuf();
  hash << CPU_TRI_D6.commit_tribuf();
  hash << CPU_TRI_D7.commit_tribuf();

  //----------------------------------------

  hash << VRAM_PIN_MCSn_C.clear_preset();    // PIN_43 -> TEFY
  hash << VRAM_PIN_MOEn_C.clear_preset();    // PIN_45 -> TAVY
  hash << VRAM_PIN_MWRn_C.clear_preset();    // PIN_49 -> SUDO

  hash << VRAM_PIN_MD0_C.clear_preset();     // PIN_33 -> RODY
  hash << VRAM_PIN_MD1_C.clear_preset();     // PIN_31 -> REBA
  hash << VRAM_PIN_MD2_C.clear_preset();     // PIN_30 -> RYDO
  hash << VRAM_PIN_MD3_C.clear_preset();     // PIN_29 -> REMO
  hash << VRAM_PIN_MD4_C.clear_preset();     // PIN_28 -> ROCE
  hash << VRAM_PIN_MD5_C.clear_preset();     // PIN_27 -> ROPU
  hash << VRAM_PIN_MD6_C.clear_preset();     // PIN_26 -> RETA
  hash << VRAM_PIN_MD7_C.clear_preset();     // PIN_25 -> RAKU

  //----------------------------------------

  hash << EXT_PIN_RDn_C.clear_preset();     // PIN_79 -> UJYV
  hash << EXT_PIN_WRn_C.clear_preset();     // PIN_78 -> UBAL

  hash << EXT_PIN_A00_C.clear_preset();     // PIN_01 -> KOVA
  hash << EXT_PIN_A01_C.clear_preset();     // PIN_02 -> CAMU
  hash << EXT_PIN_A02_C.clear_preset();     // PIN_03 -> BUXU
  hash << EXT_PIN_A03_C.clear_preset();     // PIN_04 -> BASE
  hash << EXT_PIN_A04_C.clear_preset();     // PIN_05 -> AFEC
  hash << EXT_PIN_A05_C.clear_preset();     // PIN_06 -> ABUP
  hash << EXT_PIN_A06_C.clear_preset();     // PIN_07 -> CYGU
  hash << EXT_PIN_A07_C.clear_preset();     // PIN_08 -> COGO
  hash << EXT_PIN_A08_C.clear_preset();     // PIN_09 -> MUJY
  hash << EXT_PIN_A09_C.clear_preset();     // PIN_10 -> NENA
  hash << EXT_PIN_A10_C.clear_preset();     // PIN_11 -> SURA
  hash << EXT_PIN_A11_C.clear_preset();     // PIN_12 -> MADY
  hash << EXT_PIN_A12_C.clear_preset();     // PIN_13 -> LAHE
  hash << EXT_PIN_A13_C.clear_preset();     // PIN_14 -> LURA
  hash << EXT_PIN_A14_C.clear_preset();     // PIN_15 -> PEVO
  hash << EXT_PIN_A15_C.clear_preset();     // PIN_16 -> RAZA

  hash << EXT_PIN_D0_C.clear_preset();      // PIN_17 -> TOVO,SOMA
  hash << EXT_PIN_D1_C.clear_preset();      // PIN_18 -> RUZY,RONY
  hash << EXT_PIN_D2_C.clear_preset();      // PIN_19 -> ROME,RAXY
  hash << EXT_PIN_D3_C.clear_preset();      // PIN_20 -> SAZA,SELO
  hash << EXT_PIN_D4_C.clear_preset();      // PIN_21 -> TEHE,SODY
  hash << EXT_PIN_D5_C.clear_preset();      // PIN_22 -> RATU,SAGO
  hash << EXT_PIN_D6_C.clear_preset();      // PIN_23 -> SOCA,RUPA
  hash << EXT_PIN_D7_C.clear_preset();      // PIN_24 -> RYBA,SAZY

  //----------------------------------------

  hash << JOY_PIN_P10_C.clear_preset();     // PIN_67-> KERY, KEVU
  hash << JOY_PIN_P11_C.clear_preset();     // PIN_66-> KERY, P05.KAPA
  hash << JOY_PIN_P12_C.clear_preset();     // PIN_65-> KERY, P05.KEJA
  hash << JOY_PIN_P13_C.clear_preset();     // PIN_64-> KERY, P05.KOLO

  //----------------------------------------

  hash << OAM_PIN_DA0.commit_tribuf();
  hash << OAM_PIN_DA1.commit_tribuf();
  hash << OAM_PIN_DA2.commit_tribuf();
  hash << OAM_PIN_DA3.commit_tribuf();
  hash << OAM_PIN_DA4.commit_tribuf();
  hash << OAM_PIN_DA5.commit_tribuf();
  hash << OAM_PIN_DA6.commit_tribuf();
  hash << OAM_PIN_DA7.commit_tribuf();
  hash << OAM_PIN_DB0.commit_tribuf();
  hash << OAM_PIN_DB1.commit_tribuf();
  hash << OAM_PIN_DB2.commit_tribuf();
  hash << OAM_PIN_DB3.commit_tribuf();
  hash << OAM_PIN_DB4.commit_tribuf();
  hash << OAM_PIN_DB5.commit_tribuf();
  hash << OAM_PIN_DB6.commit_tribuf();
  hash << OAM_PIN_DB7.commit_tribuf();

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_ibus() {
  SignalHash hash;

  hash << CPU_PIN_WAKE.commit_pinout();          // <- P02.AWOB

  hash << CPU_PIN_UNOR_DBG.commit_pinout();      // PORTA_02: <- P07.UNOR_MODE_DBG2
  hash << CPU_PIN_ADDR_HI.commit_pinout();          // PORTA_03: <- SYRO
  hash << CPU_PIN_BOOTp.commit_pinout();         // PORTA_04: <- TUTU
  hash << CPU_PIN_UMUT_DBG.commit_pinout();      // PORTA_05: <- P07.UMUT_MODE_DBG1
  hash << CPU_PIN_INT_VBLANK.commit_pinout();    // PORTB_03: <- LOPE, vblank int
  hash << CPU_PIN_INT_STAT.commit_pinout();      // PORTB_07: <- LALU, stat int
  hash << CPU_PIN_INT_TIMER.commit_pinout();     // PORTB_11: <- NYBO, timer int
  hash << CPU_PIN_INT_SERIAL.commit_pinout();    // PORTB_15: <- UBUL, serial int
  hash << CPU_PIN_INT_JOYPAD.commit_pinout();    // PORTB_19: <- ULAK, joypad int

  hash << CPU_PIN_PROD.commit_pinout();     // PORTC_01: <- AFER
  hash << CPU_PIN_EXT_RESET.commit_pinout();     // PORTC_02: <- PIN_RESET directly connected to the pad
  hash << CPU_PIN_EXT_CLKGOOD.commit_pinout();   // PORTC_03: <- CLKIN_A
  hash << CPU_PIN_DBG.commit_pinout();     // PORTC_04: <- TABA

  hash << CPU_PIN_BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- BOWA
  hash << CPU_PIN_BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- BEDO _____fgh
  hash << CPU_PIN_BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  hash << CPU_PIN_BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- BUDE abcd____
  hash << CPU_PIN_BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO
  hash << CPU_PIN_BUKE_ABxxxxxH.commit_pinout(); // PORTD_07: <- BUKE _____f__
  hash << CPU_PIN_BOMA_xBxxxxxx.commit_pinout(); // PORTD_08: <- BOMA _____fgh
  hash << CPU_PIN_BOGA_AxCDEFGH.commit_pinout(); // PORTD_09: <- BOGA abcde___

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_vbus() {
  SignalHash hash;

  hash << VRAM_PIN_MCSn_A.commit_pinout();   // PIN_43 <- SOKY
  hash << VRAM_PIN_MCSn_D.commit_pinout();   // PIN_43 <- SETY

  hash << VRAM_PIN_MOEn_A.commit_pinout();   // PIN_45 <- REFO
  hash << VRAM_PIN_MOEn_D.commit_pinout();   // PIN_45 <- SAHA

  hash << VRAM_PIN_MWRn_A.commit_pinout();   // PIN_49 <- SYSY
  hash << VRAM_PIN_MWRn_D.commit_pinout();   // PIN_49 <- RAGU

  hash << VRAM_PIN_MA00_AD.commit_pinout();  // PIN_34 <- ECAL
  hash << VRAM_PIN_MA01_AD.commit_pinout();  // PIN_35 <- EGEZ
  hash << VRAM_PIN_MA02_AD.commit_pinout();  // PIN_36 <- FUHE
  hash << VRAM_PIN_MA03_AD.commit_pinout();  // PIN_37 <- FYZY
  hash << VRAM_PIN_MA04_AD.commit_pinout();  // PIN_38 <- DAMU
  hash << VRAM_PIN_MA05_AD.commit_pinout();  // PIN_39 <- DAVA
  hash << VRAM_PIN_MA06_AD.commit_pinout();  // PIN_40 <- ETEG
  hash << VRAM_PIN_MA07_AD.commit_pinout();  // PIN_41 <- EREW
  hash << VRAM_PIN_MA08_AD.commit_pinout();  // PIN_48 <- EVAX
  hash << VRAM_PIN_MA09_AD.commit_pinout();  // PIN_47 <- DUVE
  hash << VRAM_PIN_MA10_AD.commit_pinout();  // PIN_44 <- ERAF
  hash << VRAM_PIN_MA11_AD.commit_pinout();  // PIN_46 <- FUSY
  hash << VRAM_PIN_MA12_AD.commit_pinout();  // PIN_42 <- EXYF

  hash << VRAM_PIN_MD0_A.commit_pinout();    // PIN_33 <- REGE
  hash << VRAM_PIN_MD0_B.commit_pinout();    // PIN_33 <- ROFA
  hash << VRAM_PIN_MD0_D.commit_pinout();    // PIN_33 <- RURA
  hash << VRAM_PIN_MD1_A.commit_pinout();    // PIN_31 <- RYKY
  hash << VRAM_PIN_MD1_B.commit_pinout();    // PIN_31 <- ROFA
  hash << VRAM_PIN_MD1_D.commit_pinout();    // PIN_31 <- RULY
  hash << VRAM_PIN_MD2_A.commit_pinout();    // PIN_30 <- RAZO
  hash << VRAM_PIN_MD2_B.commit_pinout();    // PIN_30 <- ROFA
  hash << VRAM_PIN_MD2_D.commit_pinout();    // PIN_30 <- RARE
  hash << VRAM_PIN_MD3_A.commit_pinout();    // PIN_29 <- RADA
  hash << VRAM_PIN_MD3_B.commit_pinout();    // PIN_29 <- ROFA
  hash << VRAM_PIN_MD3_D.commit_pinout();    // PIN_29 <- RODU
  hash << VRAM_PIN_MD4_A.commit_pinout();    // PIN_28 <- RYRO
  hash << VRAM_PIN_MD4_B.commit_pinout();    // PIN_28 <- ROFA
  hash << VRAM_PIN_MD4_D.commit_pinout();    // PIN_28 <- RUBE
  hash << VRAM_PIN_MD5_A.commit_pinout();    // PIN_27 <- REVU
  hash << VRAM_PIN_MD5_B.commit_pinout();    // PIN_27 <- ROFA
  hash << VRAM_PIN_MD5_D.commit_pinout();    // PIN_27 <- RUMU
  hash << VRAM_PIN_MD6_A.commit_pinout();    // PIN_26 <- REKU
  hash << VRAM_PIN_MD6_B.commit_pinout();    // PIN_26 <- ROFA
  hash << VRAM_PIN_MD6_D.commit_pinout();    // PIN_26 <- RYTY
  hash << VRAM_PIN_MD7_A.commit_pinout();    // PIN_25 <- RYZE
  hash << VRAM_PIN_MD7_B.commit_pinout();    // PIN_25 <- ROFA
  hash << VRAM_PIN_MD7_D.commit_pinout();    // PIN_25 <- RADY

  hash << VRM_TRI_A00.commit_tribuf();
  hash << VRM_TRI_A01.commit_tribuf();
  hash << VRM_TRI_A02.commit_tribuf();
  hash << VRM_TRI_A03.commit_tribuf();
  hash << VRM_TRI_A04.commit_tribuf();
  hash << VRM_TRI_A05.commit_tribuf();
  hash << VRM_TRI_A06.commit_tribuf();
  hash << VRM_TRI_A07.commit_tribuf();
  hash << VRM_TRI_A08.commit_tribuf();
  hash << VRM_TRI_A09.commit_tribuf();
  hash << VRM_TRI_A10.commit_tribuf();
  hash << VRM_TRI_A11.commit_tribuf();
  hash << VRM_TRI_A12.commit_tribuf();

  hash << VRM_TRI_D0.commit_tribuf();
  hash << VRM_TRI_D1.commit_tribuf();
  hash << VRM_TRI_D2.commit_tribuf();
  hash << VRM_TRI_D3.commit_tribuf();
  hash << VRM_TRI_D4.commit_tribuf();
  hash << VRM_TRI_D5.commit_tribuf();
  hash << VRM_TRI_D6.commit_tribuf();
  hash << VRM_TRI_D7.commit_tribuf();

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_ebus() {
  SignalHash hash;

  hash << EXT_PIN_CLK.commit_pinout();      // PIN_75 <- BUDE/BEVA

  

  hash << EXT_PIN_RDn_A.commit_pinout();    // PIN_79 <- UGAC
  hash << EXT_PIN_RDn_D.commit_pinout();    // PIN_79 <- URUN

  hash << EXT_PIN_WRn_A.commit_pinout();    // PIN_78 <- UVER
  hash << EXT_PIN_WRn_D.commit_pinout();    // PIN_78 <- USUF

  hash << EXT_PIN_CSn_A.commit_pinout();    // PIN_80 <- TYHO

  hash << EXT_PIN_A00_A.commit_pinout();    // PIN_01 <- KUPO
  hash << EXT_PIN_A00_D.commit_pinout();    // PIN_01 <- KOTY
  hash << EXT_PIN_A01_A.commit_pinout();    // PIN_02 <- CABA
  hash << EXT_PIN_A01_D.commit_pinout();    // PIN_02 <- COTU
  hash << EXT_PIN_A02_A.commit_pinout();    // PIN_03 <- BOKU
  hash << EXT_PIN_A02_D.commit_pinout();    // PIN_03 <- BAJO
  hash << EXT_PIN_A03_A.commit_pinout();    // PIN_04 <- BOTY
  hash << EXT_PIN_A03_D.commit_pinout();    // PIN_04 <- BOLA
  hash << EXT_PIN_A04_A.commit_pinout();    // PIN_05 <- BYLA
  hash << EXT_PIN_A04_D.commit_pinout();    // PIN_05 <- BEVO
  hash << EXT_PIN_A05_A.commit_pinout();    // PIN_06 <- BADU
  hash << EXT_PIN_A05_D.commit_pinout();    // PIN_06 <- AJAV
  hash << EXT_PIN_A06_A.commit_pinout();    // PIN_07 <- CEPU
  hash << EXT_PIN_A06_D.commit_pinout();    // PIN_07 <- CYKA
  hash << EXT_PIN_A07_A.commit_pinout();    // PIN_08 <- DEFY
  hash << EXT_PIN_A07_D.commit_pinout();    // PIN_08 <- COLO
  hash << EXT_PIN_A08_A.commit_pinout();    // PIN_09 <- MYNY
  hash << EXT_PIN_A08_D.commit_pinout();    // PIN_09 <- MEGO
  hash << EXT_PIN_A09_A.commit_pinout();    // PIN_10 <- MUNE
  hash << EXT_PIN_A09_D.commit_pinout();    // PIN_10 <- MENY
  hash << EXT_PIN_A10_A.commit_pinout();    // PIN_11 <- ROXU
  hash << EXT_PIN_A10_D.commit_pinout();    // PIN_11 <- RORE
  hash << EXT_PIN_A11_A.commit_pinout();    // PIN_12 <- LEPY
  hash << EXT_PIN_A11_D.commit_pinout();    // PIN_12 <- LYNY
  hash << EXT_PIN_A12_A.commit_pinout();    // PIN_13 <- LUCE
  hash << EXT_PIN_A12_D.commit_pinout();    // PIN_13 <- LOSO
  hash << EXT_PIN_A13_A.commit_pinout();    // PIN_14 <- LABE
  hash << EXT_PIN_A13_D.commit_pinout();    // PIN_14 <- LEVA
  hash << EXT_PIN_A14_A.commit_pinout();    // PIN_15 <- PUHE
  hash << EXT_PIN_A14_D.commit_pinout();    // PIN_15 <- PAHY
  hash << EXT_PIN_A15_A.commit_pinout();    // PIN_16 <- SUZE
  hash << EXT_PIN_A15_D.commit_pinout();    // PIN_16 <- RULO

  hash << EXT_PIN_D0_A.commit_pinout();     // PIN_17 <- RUXA
  hash << EXT_PIN_D0_B.commit_pinout();     // PIN_17 <- LULA
  hash << EXT_PIN_D0_D.commit_pinout();     // PIN_17 <- RUNE
  hash << EXT_PIN_D1_A.commit_pinout();     // PIN_18 <- RUJA
  hash << EXT_PIN_D1_B.commit_pinout();     // PIN_18 <- LULA
  hash << EXT_PIN_D1_D.commit_pinout();     // PIN_18 <- RYPU
  hash << EXT_PIN_D2_A.commit_pinout();     // PIN_19 <- RABY
  hash << EXT_PIN_D2_B.commit_pinout();     // PIN_19 <- LULA
  hash << EXT_PIN_D2_D.commit_pinout();     // PIN_19 <- SULY
  hash << EXT_PIN_D3_A.commit_pinout();     // PIN_20 <- RERA
  hash << EXT_PIN_D3_B.commit_pinout();     // PIN_20 <- LULA
  hash << EXT_PIN_D3_D.commit_pinout();     // PIN_20 <- SEZE
  hash << EXT_PIN_D4_A.commit_pinout();     // PIN_21 <- RORY
  hash << EXT_PIN_D4_B.commit_pinout();     // PIN_21 <- LULA
  hash << EXT_PIN_D4_D.commit_pinout();     // PIN_21 <- RESY
  hash << EXT_PIN_D5_A.commit_pinout();     // PIN_22 <- RYVO
  hash << EXT_PIN_D5_B.commit_pinout();     // PIN_22 <- LULA
  hash << EXT_PIN_D5_D.commit_pinout();     // PIN_22 <- TAMU
  hash << EXT_PIN_D6_A.commit_pinout();     // PIN_23 <- RAFY
  hash << EXT_PIN_D6_B.commit_pinout();     // PIN_23 <- LULA
  hash << EXT_PIN_D6_D.commit_pinout();     // PIN_23 <- ROGY
  hash << EXT_PIN_D7_A.commit_pinout();     // PIN_24 <- RAVU
  hash << EXT_PIN_D7_B.commit_pinout();     // PIN_24 <- LULA
  hash << EXT_PIN_D7_D.commit_pinout();     // PIN_24 <- RYDA

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_obus() {
  SignalHash hash;

  hash << OAM_PIN_CLK.commit_pinout();
  hash << OAM_PIN_OE.commit_pinout();
  hash << OAM_PIN_WR_A.commit_pinout();
  hash << OAM_PIN_WR_B.commit_pinout();

  hash << OAM_PIN_A0.commit_pinout();
  hash << OAM_PIN_A1.commit_pinout();
  hash << OAM_PIN_A2.commit_pinout();
  hash << OAM_PIN_A3.commit_pinout();
  hash << OAM_PIN_A4.commit_pinout();
  hash << OAM_PIN_A5.commit_pinout();
  hash << OAM_PIN_A6.commit_pinout();
  hash << OAM_PIN_A7.commit_pinout();

  //----------
  // Sprite store tribufs

  hash << SPR_TRI_IDX_0.commit_tribuf();
  hash << SPR_TRI_IDX_1.commit_tribuf();
  hash << SPR_TRI_IDX_2.commit_tribuf();
  hash << SPR_TRI_IDX_3.commit_tribuf();
  hash << SPR_TRI_IDX_4.commit_tribuf();
  hash << SPR_TRI_IDX_5.commit_tribuf();

  hash << SPR_TRI_LINE_0.commit_tribuf();
  hash << SPR_TRI_LINE_1.commit_tribuf();
  hash << SPR_TRI_LINE_2.commit_tribuf();
  hash << SPR_TRI_LINE_3.commit_tribuf();

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_joy_pins() {
  SignalHash hash;

  hash << JOY_PIN_P10_A.commit_pinout();    // PIN_67<- KOLE
  hash << JOY_PIN_P10_B.commit_pinout();    // PIN_67
  hash << JOY_PIN_P10_D.commit_pinout();    // PIN_67<- KYBU
  hash << JOY_PIN_P11_A.commit_pinout();    // PIN_66<- KYTO
  hash << JOY_PIN_P11_B.commit_pinout();    // PIN_66
  hash << JOY_PIN_P11_D.commit_pinout();    // PIN_66<- KABU
  hash << JOY_PIN_P12_A.commit_pinout();    // PIN_65<- KYHU
  hash << JOY_PIN_P12_B.commit_pinout();    // PIN_65
  hash << JOY_PIN_P12_D.commit_pinout();    // PIN_65<- KASY
  hash << JOY_PIN_P13_A.commit_pinout();    // PIN_64<- KORY
  hash << JOY_PIN_P13_B.commit_pinout();    // PIN_64
  hash << JOY_PIN_P13_D.commit_pinout();    // PIN_64<- KALE
  hash << JOY_PIN_P14_A.commit_pinout();    // PIN_63<- KARU
  hash << JOY_PIN_P14_D.commit_pinout();    // PIN_63<- KELY
  hash << JOY_PIN_P15_A.commit_pinout();    // PIN_62<- CELA
  hash << JOY_PIN_P15_D.commit_pinout();    // PIN_62<- COFY

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_lcd_pins() {
  SignalHash hash;

  hash << LCD_PIN_LD1.commit_pinout();      // PIN_50 
  hash << LCD_PIN_LD0.commit_pinout();      // PIN_51 
  hash << LCD_PIN_CPG.commit_pinout();      // PIN_52 
  hash << LCD_PIN_CP.commit_pinout();       // PIN_53 
  hash << LCD_PIN_ST.commit_pinout();       // PIN_54 
  hash << LCD_PIN_CPL.commit_pinout();      // PIN_55 
  hash << LCD_PIN_FR.commit_pinout();       // PIN_56 
  hash << LCD_PIN_S.commit_pinout();        // PIN_57 

  return hash;
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_top_regs() {
  SignalHash hash;

  //----------
  // LCDC

  hash << VYXE_LCDC_BGEN.commit_reg();
  hash << XYLO_LCDC_SPEN.commit_reg();
  hash << XYMO_LCDC_SPSIZE.commit_reg();
  hash << XAFO_LCDC_BGMAP.commit_reg();
  hash << WEXU_LCDC_BGTILE.commit_reg();
  hash << WYMO_LCDC_WINEN.commit_reg();
  hash << WOKY_LCDC_WINMAP.commit_reg();
  hash << XONA_LCDC_EN.commit_reg();

  //----------
  // Tile fetcher regs

  hash << LEGU_BG_PIX_A0.commit_reg();
  hash << NUDU_BG_PIX_A1.commit_reg();
  hash << MUKU_BG_PIX_A2.commit_reg();
  hash << LUZO_BG_PIX_A3.commit_reg();
  hash << MEGU_BG_PIX_A4.commit_reg();
  hash << MYJY_BG_PIX_A5.commit_reg();
  hash << NASA_BG_PIX_A6.commit_reg();
  hash << NEFO_BG_PIX_A7.commit_reg();

  hash << RAWU_BG_PIX_B0.commit_reg();
  hash << POZO_BG_PIX_B1.commit_reg();
  hash << PYZO_BG_PIX_B2.commit_reg(); 
  hash << POXA_BG_PIX_B3.commit_reg(); 
  hash << PULO_BG_PIX_B4.commit_reg(); 
  hash << POJU_BG_PIX_B5.commit_reg(); 
  hash << POWY_BG_PIX_B6.commit_reg(); 
  hash << PYJU_BG_PIX_B7.commit_reg();

  return hash;
}

//-----------------------------------------------------------------------------
// Debug signals

pwire SchematicTop::UMUT_MODE_DBG1p() const {
  /*p07.UVAR*/ wire UVAR_T2n = not(SYS_PIN_T2p);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (SYS_PIN_T1p, UVAR_T2n);
  return UMUT_MODE_DBG1p;
}

pwire SchematicTop::UNOR_MODE_DBG2p() const {
  /*p07.UBET*/ wire UBET_T1n = not(SYS_PIN_T1p);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (SYS_PIN_T2p, UBET_T1n);
  return UNOR_MODE_DBG2p;
}

nwire SchematicTop::TOVA_MODE_DBG2n() const {
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not(UNOR_MODE_DBG2p());
  return TOVA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------
// Clock signals

wire SchematicTop::AFUR_xBCDExxx() const { return clk_reg.AFUR_xBCDExxx; }
wire SchematicTop::ALEF_xxCDEFxx() const { return clk_reg.ALEF_xxCDEFxx; }
wire SchematicTop::APUK_xxxDEFGx() const { return clk_reg.APUK_xxxDEFGx; }
wire SchematicTop::ADYK_xxxxEFGH() const { return clk_reg.ADYK_xxxxEFGH; }
wire SchematicTop::WUVU_AxxDExxH() const { return clk_reg.WUVU_AxxDExxH; }
wire SchematicTop::VENA_xBCDExxx() const { return clk_reg.VENA_xBCDExxx; }
wire SchematicTop::WOSU_xxCDxxGH() const { return clk_reg.WOSU_xxCDxxGH; }

wire SchematicTop::BELE_xBxxxxxx() const {
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(!APUK_xxxDEFGx());
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx());
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_CLKREQ);
  /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
  /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
  /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
  /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
  /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
  /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
  /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
  /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
  /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
  /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
  /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
  return BELE_xBxxxxxx;
}

wire SchematicTop::BYJU_AxCDEFGH() const {
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(SYS_PIN_CLK_A);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx(), ATEZ_CLKBAD);
  return BYJU_AxCDEFGH;
}

//-----------------------------------------------------------------------------
// Timer signals

wire SchematicTop::UVYN_DIV_05n() const    { return not(tim_reg.TAMA_DIV_05); }
wire SchematicTop::UMEK_DIV_06n() const    { return not(tim_reg.UGOT_DIV_06); }
wire SchematicTop::UREK_DIV_07n() const    { return not(tim_reg.TULU_DIV_07); }
wire SchematicTop::UPOF_DIV_15() const     { return tim_reg.UPOF_DIV_15; }

pwire SchematicTop::MOBA_INT_TIMERp() const { return (pwire)tim_reg.MOBA_INT_TIMERp; }

//-----------------------------------------------------------------------------
// Reset signals

pwire SchematicTop::AVOR_RSTp() const {
  /*p01.AVOR*/ pwire AVOR_RSTp = or(rst_reg.AFER.q(), rst_reg.ASOL.q());
  return AVOR_RSTp;
}

nwire SchematicTop::XAPO_VID_RSTn() const {
  /*p01.ALUR*/ nwire ALUR_RSTn = not(AVOR_RSTp());
  /*p01.DULA*/ pwire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ nwire CUNU_RSTn = not(DULA_RSTp);
  /*p01.XORE*/ pwire XORE_RSTp = not(CUNU_RSTn);
  /*p01.XEBE*/ nwire XEBE_RSTn = not(XORE_RSTp);

  /*p01.XODO*/ pwire XODO_VID_RSTp = nand(XEBE_RSTn, XONA_LCDC_EN);
  /*p01.XAPO*/ nwire XAPO_VID_RSTn = not(XODO_VID_RSTp);
  return XAPO_VID_RSTn;
}


//-----------------------------------------------------------------------------
// CPU signals

nwire SchematicTop::UJYV_CPU_RDn() const {
  /*p07.UJYV*/ nwire UJYV_CPU_RDn = mux2_n(EXT_PIN_RDn_C, CPU_PIN_RD, UNOR_MODE_DBG2p());
  return UJYV_CPU_RDn;
}

pwire SchematicTop::UBAL_CPU_WRp_ABCDExxx() const {
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH());
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p01.AREV*/ nwire AREV_CPU_WRn_ABCDExxx = nand(CPU_PIN_WRp, AFAS_xxxxxFGH);
  /*p01.APOV*/ pwire APOV_CPU_WRp_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);

  // polarity of EXT_PIN_WRn_C wrong?
  /*p07.UBAL*/ pwire UBAL_CPU_WRp_ABCDExxx = mux2_n(EXT_PIN_WRn_C, APOV_CPU_WRp_xxxxxFGH, UNOR_MODE_DBG2p());
  return UBAL_CPU_WRp_ABCDExxx;
}

int SchematicTop::get_addr() const {
  return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03, CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
    CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11, CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
}

int SchematicTop::get_data() const {
  return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3, CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
}

void SchematicTop::set_data(bool oe, uint8_t data) {
  CPU_TRI_D0.set_tribuf(oe, data & 0x01);
  CPU_TRI_D1.set_tribuf(oe, data & 0x02);
  CPU_TRI_D2.set_tribuf(oe, data & 0x04);
  CPU_TRI_D3.set_tribuf(oe, data & 0x08);
  CPU_TRI_D4.set_tribuf(oe, data & 0x10);
  CPU_TRI_D5.set_tribuf(oe, data & 0x20);
  CPU_TRI_D6.set_tribuf(oe, data & 0x40);
  CPU_TRI_D7.set_tribuf(oe, data & 0x80);
}

//-----------------------------------------------------------------------------
// Bus mux signals

/*p28.GEKA*/ pwire SchematicTop::GEKA_OAM_A0p() const { return (pwire)bus_mux.GEKA_OAM_A0p; }
/*p28.ZYFO*/ pwire SchematicTop::ZYFO_OAM_A1p() const { return (pwire)bus_mux.ZYFO_OAM_A1p; }
/*p28.YFOT*/ pwire SchematicTop::YFOT_OAM_A2p() const { return (pwire)bus_mux.YFOT_OAM_A2p; }
/*p28.YFOC*/ pwire SchematicTop::YFOC_OAM_A3p() const { return (pwire)bus_mux.YFOC_OAM_A3p; }
/*p28.YVOM*/ pwire SchematicTop::YVOM_OAM_A4p() const { return (pwire)bus_mux.YVOM_OAM_A4p; }
/*p28.YMEV*/ pwire SchematicTop::YMEV_OAM_A5p() const { return (pwire)bus_mux.YMEV_OAM_A5p; }
/*p28.XEMU*/ pwire SchematicTop::XEMU_OAM_A6p() const { return (pwire)bus_mux.XEMU_OAM_A6p; }
/*p28.YZET*/ pwire SchematicTop::YZET_OAM_A7p() const { return (pwire)bus_mux.YZET_OAM_A7p; }

/*p31.YLOR*/ pwire SchematicTop::YLOR_SPRITE_X0() const { return (pwire)bus_mux.YLOR_SPRITE_X0; }
/*p31.ZYTY*/ pwire SchematicTop::ZYTY_SPRITE_X1() const { return (pwire)bus_mux.ZYTY_SPRITE_X1; }
/*p31.ZYVE*/ pwire SchematicTop::ZYVE_SPRITE_X2() const { return (pwire)bus_mux.ZYVE_SPRITE_X2; }
/*p31.ZEZY*/ pwire SchematicTop::ZEZY_SPRITE_X3() const { return (pwire)bus_mux.ZEZY_SPRITE_X3; }
/*p31.GOMO*/ pwire SchematicTop::GOMO_SPRITE_X4() const { return (pwire)bus_mux.GOMO_SPRITE_X4; }
/*p31.BAXO*/ pwire SchematicTop::BAXO_SPRITE_X5() const { return (pwire)bus_mux.BAXO_SPRITE_X5; }
/*p31.YZOS*/ pwire SchematicTop::YZOS_SPRITE_X6() const { return (pwire)bus_mux.YZOS_SPRITE_X6; }
/*p31.DEPO*/ pwire SchematicTop::DEPO_SPRITE_X7() const { return (pwire)bus_mux.DEPO_SPRITE_X7; }

/*p29.XUSO*/ pwire SchematicTop::XUSO_SPRITE_Y0() const { return (pwire)bus_mux.XUSO_SPRITE_Y0; }
/*p29.XEGU*/ pwire SchematicTop::XEGU_SPRITE_Y1() const { return (pwire)bus_mux.XEGU_SPRITE_Y1; }
/*p29.YJEX*/ pwire SchematicTop::YJEX_SPRITE_Y2() const { return (pwire)bus_mux.YJEX_SPRITE_Y2; }
/*p29.XYJU*/ pwire SchematicTop::XYJU_SPRITE_Y3() const { return (pwire)bus_mux.XYJU_SPRITE_Y3; }
/*p29.YBOG*/ pwire SchematicTop::YBOG_SPRITE_Y4() const { return (pwire)bus_mux.YBOG_SPRITE_Y4; }
/*p29.WYSO*/ pwire SchematicTop::WYSO_SPRITE_Y5() const { return (pwire)bus_mux.WYSO_SPRITE_Y5; }
/*p29.XOTE*/ pwire SchematicTop::XOTE_SPRITE_Y6() const { return (pwire)bus_mux.XOTE_SPRITE_Y6; }
/*p29.YZAB*/ pwire SchematicTop::YZAB_SPRITE_Y7() const { return (pwire)bus_mux.YZAB_SPRITE_Y7; }

//-----------------------------------------------------------------------------
// DMA signals

pwire SchematicTop::MATU_DMA_RUNNINGp() const {
  /*p04.MATU*/ pwire MATU_DMA_RUNNINGp = dma_reg.MATU_DMA_RUNNINGp;
  return MATU_DMA_RUNNINGp;
}

pwire SchematicTop::MUDA_DMA_SRC_VRAMp() const {
  /*p04.LEBU*/ nwire LEBU_DMA_ADDR_A15n  = not(dma_reg.DMA_A15.q());
  /*p04.MUDA*/ pwire MUDA_DMA_SRC_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
  return MUDA_DMA_SRC_VRAMp;
}

pwire SchematicTop::DMA_A00() const { return (pwire)dma_reg.DMA_A00; }
pwire SchematicTop::DMA_A01() const { return (pwire)dma_reg.DMA_A01; }
pwire SchematicTop::DMA_A02() const { return (pwire)dma_reg.DMA_A02; }
pwire SchematicTop::DMA_A03() const { return (pwire)dma_reg.DMA_A03; }
pwire SchematicTop::DMA_A04() const { return (pwire)dma_reg.DMA_A04; }
pwire SchematicTop::DMA_A05() const { return (pwire)dma_reg.DMA_A05; }
pwire SchematicTop::DMA_A06() const { return (pwire)dma_reg.DMA_A06; }
pwire SchematicTop::DMA_A07() const { return (pwire)dma_reg.DMA_A07; }
pwire SchematicTop::DMA_A08() const { return (pwire)dma_reg.DMA_A08; }
pwire SchematicTop::DMA_A09() const { return (pwire)dma_reg.DMA_A09; }
pwire SchematicTop::DMA_A10() const { return (pwire)dma_reg.DMA_A10; }
pwire SchematicTop::DMA_A11() const { return (pwire)dma_reg.DMA_A11; }
pwire SchematicTop::DMA_A12() const { return (pwire)dma_reg.DMA_A12; }
pwire SchematicTop::DMA_A13() const { return (pwire)dma_reg.DMA_A13; }
pwire SchematicTop::DMA_A14() const { return (pwire)dma_reg.DMA_A14; }
pwire SchematicTop::DMA_A15() const { return (pwire)dma_reg.DMA_A15; }

//------------------------------------------------------------------------------
// LCD signals

wire SchematicTop::BYHA_VID_LINE_TRIG_d4() const {
  /*p28.ABAF*/ wire ABAF_LINE_END_Bn = not(lcd_reg.CATU_LINE_END_B.q());
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
  /*p28.BYHA*/ pwire BYHA_VID_LINE_TRIG_d4 = and (or (lcd_reg.ANEL_LINE_END_D, ABAF_LINE_END_Bn), ABEZ_VID_RSTn);
  return BYHA_VID_LINE_TRIG_d4;
}

wire SchematicTop::RUTU_LINE_END_F() const { return lcd_reg.RUTU_LINE_END_F; }
wire SchematicTop::CATU_LINE_END_B() const { return lcd_reg.CATU_LINE_END_B; }
wire SchematicTop::POPU_VBLANK_d4() const { return lcd_reg.POPU_VBLANK_d4; }
pwire SchematicTop::ROPO_LY_MATCH_SYNCp() const { return (pwire)lcd_reg.ROPO_LY_MATCH_SYNCp; }

pwire SchematicTop::MUWY_Y0() const { return (pwire)lcd_reg.MUWY_Y0; }
pwire SchematicTop::MYRO_Y1() const { return (pwire)lcd_reg.MYRO_Y1; }
pwire SchematicTop::LEXA_Y2() const { return (pwire)lcd_reg.LEXA_Y2; }
pwire SchematicTop::LYDO_Y3() const { return (pwire)lcd_reg.LYDO_Y3; }
pwire SchematicTop::LOVU_Y4() const { return (pwire)lcd_reg.LOVU_Y4; }
pwire SchematicTop::LEMA_Y5() const { return (pwire)lcd_reg.LEMA_Y5; }
pwire SchematicTop::MATO_Y6() const { return (pwire)lcd_reg.MATO_Y6; }
pwire SchematicTop::LAFO_Y7() const { return (pwire)lcd_reg.LAFO_Y7; }

//-----------------------------------------------------------------------------
// Address decoder signals

pwire SchematicTop::WERO_FF4Xp() const {
#if 0

  /*p22.WERO*/ wire WERO_FF4Xp = and( CPU_PIN_A15, CPU_PIN_A14,  CPU_PIN_A13,  CPU_PIN_A12,
                                      CPU_PIN_A11, CPU_PIN_A10,  CPU_PIN_A09,  CPU_PIN_A08,
                                     !CPU_PIN_A07, CPU_PIN_A06, !CPU_PIN_A05, !CPU_PIN_A04);

#endif

  /*p07.TONA*/ wire TONA_A08n       = not(CPU_PIN_A08);
  /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
  /*p22.XALY*/ wire XALY_0x00xxxxp  = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
  /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
  /*p22.WERO*/ wire WERO_FF4Xp = not(WUTU_FF4Xn);

  return WERO_FF4Xp;
}

//-----------------------------------------------------------------------------
// PPU signals

pwire SchematicTop::XYMU_RENDERINGp() const { /*p21.XYMU*/ return (pwire)ppu_reg.XYMU_RENDERINGp; }

pwire SchematicTop::XEHO_X0() const { /*p21.XEHO*/ return (pwire)ppu_reg.XEHO_X0; }
pwire SchematicTop::SAVY_X1() const { /*p21.SAVY*/ return (pwire)ppu_reg.SAVY_X1; }
pwire SchematicTop::XODU_X2() const { /*p21.XODU*/ return (pwire)ppu_reg.XODU_X2; }
pwire SchematicTop::XYDO_X3() const { /*p21.XYDO*/ return (pwire)ppu_reg.XYDO_X3; }
pwire SchematicTop::TUHU_X4() const { /*p21.TUHU*/ return (pwire)ppu_reg.TUHU_X4; }
pwire SchematicTop::TUKY_X5() const { /*p21.TUKY*/ return (pwire)ppu_reg.TUKY_X5; }
pwire SchematicTop::TAKO_X6() const { /*p21.TAKO*/ return (pwire)ppu_reg.TAKO_X6; }
pwire SchematicTop::SYBE_X7() const { /*p21.SYBE*/ return (pwire)ppu_reg.SYBE_X7; }

wire SchematicTop::TEVO_FINE_RSTp() const {
  
  /*p27.ROMO*/ wire ROMO_AFTER_PORCHn = not(tile_fetcher.POKY_PORCH_DONEp);
  /*p27.SUVU*/ wire SUVU_PORCH_ENDn = nand(XYMU_RENDERINGp(), ROMO_AFTER_PORCHn, tile_fetcher.NYKA_TILE_FETCH_DONE_Ap, PORY_TILE_FETCH_DONE_Bp());
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
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(tile_fetcher.LAXU_BFETCH_S0, tile_fetcher.NYVA_BFETCH_S2, NYXU_TILE_FETCHER_RSTn());
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);
  return LYRY_BFETCH_DONEp;
}

nwire SchematicTop::SEGU_CLKPIPEn() const {
  /*p01.ANOS*/ wire ANOS_AxCxExGx = not(SYS_PIN_CLK_B);
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_AxCxExGx = not(ALET_xBxDxFxH);
  /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor(sst_reg.FEPO_STORE_MATCHp, ppu_reg.WODU_RENDER_DONEp, MYVO_AxCxExGx);

  /*p27.SYLO*/ wire SYLO_WIN_HITn = not(win_reg.RYDY_WIN_FIRST_TILE_A);
  /*p24.TOMU*/ wire TOMU_WIN_HITp = not(SYLO_WIN_HITn);
  /*p24.SOCY*/ wire _SOCY_WIN_HITn = not(TOMU_WIN_HITp);
  /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and (_SOCY_WIN_HITn, tile_fetcher.POKY_PORCH_DONEp, _VYBO_PIX_CLK_xBxDxFxH);

  /*p24.SEGU*/ wire SEGU_CLKPIPEn = not(_TYFA_CLKPIPEp_xBxDxFxH);
  return SEGU_CLKPIPEn;
}

wire SchematicTop::SACU_CLKPIPEp() const {
  /*p24.SACU*/ return nor(SEGU_CLKPIPEn(), ppu_reg.ROXY_FINE_MATCH_LATCHn);
}

wire SchematicTop::ROZE_FINE_COUNT_7n() const {
  /*p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand(ppu_reg.RYKU_FINE_CNT0, ppu_reg.ROGA_FINE_CNT1, ppu_reg.RUBU_FINE_CNT2);
  return ROZE_FINE_COUNT_7n;
}

wire SchematicTop::VOTY_INT_STATp() const {
  /*p21.PURE*/ wire PURE_NEW_LINE_d0n = not(RUTU_LINE_END_F());
  /*p21.SELA*/ wire SELA_NEW_LINE_d0p = not(PURE_NEW_LINE_d0n);
  /*p21.PARU*/ wire PARU_VBLANKp = not(!POPU_VBLANK_d4());
  /*p21.TOLU*/ wire TOLU_VBLANKn = not(PARU_VBLANKp);
  /*p21.TAPA*/ wire TAPA_INT_OAM = and (TOLU_VBLANKn, SELA_NEW_LINE_d0p);
  /*p21.TARU*/ wire TARU_INT_HBL = and (TOLU_VBLANKn, ppu_reg.WODU_RENDER_DONEp);
  /*p21.SUKO*/ wire SUKO_INT_STATb = amux4(ppu_reg.RUGU_INT_LYC_EN, ROPO_LY_MATCH_SYNCp(),
                                           ppu_reg.REFE_INT_OAM_EN, TAPA_INT_OAM,
                                           ppu_reg.RUFO_INT_VBL_EN, PARU_VBLANKp, // polarity?
                                           ppu_reg.ROXE_INT_HBL_EN, TARU_INT_HBL);
  /*p21.TUVA*/ wire TUVA_INT_STATn = not(SUKO_INT_STATb);
  /*p21.VOTY*/ wire VOTY_INT_STATp = not(TUVA_INT_STATn);
  return VOTY_INT_STATp;
}

// And this is the topmost "reset sprite fetcher" signal
wire SchematicTop::VEKU_SFETCH_RUNNING_RSTn() const {
  /*p29.WUTY*/ wire WUTY_PIPE_LOAD_SPRITEp = not(VUSA_SPRITE_DONEn());
  /*p27.ROMO*/ wire ROMO_AFTER_PORCHn = not(tile_fetcher.POKY_PORCH_DONEp);
  /*p27.SUVU*/ wire SUVU_PORCH_ENDn = nand(XYMU_RENDERINGp(), ROMO_AFTER_PORCHn, tile_fetcher.NYKA_TILE_FETCH_DONE_Ap, tile_fetcher.PORY_TILE_FETCH_DONE_Bp);
  /*p27.TAVE*/ wire TAVE_PORCH_DONE_TRIGp = not(SUVU_PORCH_ENDn);
  /*p27.VEKU*/ wire VEKU_SFETCH_RUNNING_RSTn = nor(WUTY_PIPE_LOAD_SPRITEp, TAVE_PORCH_DONE_TRIGp); // def nor
  return VEKU_SFETCH_RUNNING_RSTn;
}

//-----------------------------------------------------------------------------
// Tile fetcher signals

wire SchematicTop::PORY_TILE_FETCH_DONE_Bp() const {
  /*p24.PORY*/ return tile_fetcher.PORY_TILE_FETCH_DONE_Bp;
}

wire SchematicTop::LONY_BG_READ_VRAM_LATCHp() const { return tile_fetcher.LONY_BG_READ_VRAM_LATCHp; }

pwire SchematicTop::DATY_SCX0() const { /*p23.DATY*/ return (pwire)tile_fetcher.DATY_SCX0; }
pwire SchematicTop::DUZU_SCX1() const { /*p23.DUZU*/ return (pwire)tile_fetcher.DUZU_SCX1; }
pwire SchematicTop::CYXU_SCX2() const { /*p23.CYXU*/ return (pwire)tile_fetcher.CYXU_SCX2; }

//-----------------------------------------------------------------------------
// Sprite scanner signals

pwire SchematicTop::CEHA_SCANNINGp() const { /*p29.CEHA*/ return (pwire)not(sprite_scanner.CENO_SCANNINGp.qn()); }
pwire SchematicTop::BESU_SCANNINGp() const { /*p28.BESU*/ return (pwire)sprite_scanner.BESU_SCANNINGp; }
pwire SchematicTop::CENO_SCANNINGp() const { /*p29.CENO*/ return (pwire)sprite_scanner.CENO_SCANNINGp; }

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

wire SchematicTop::YFEL_SCAN0() const { /*p28.YFEL*/ return sprite_scanner.YFEL_SCAN0; }
wire SchematicTop::WEWY_SCAN1() const { /*p28.WEWY*/ return sprite_scanner.WEWY_SCAN1; }
wire SchematicTop::GOSO_SCAN2() const { /*p28.GOSO*/ return sprite_scanner.GOSO_SCAN2; }
wire SchematicTop::ELYN_SCAN3() const { /*p28.ELYN*/ return sprite_scanner.ELYN_SCAN3; }
wire SchematicTop::FAHA_SCAN4() const { /*p28.FAHA*/ return sprite_scanner.FAHA_SCAN4; }
wire SchematicTop::FONY_SCAN5() const { /*p28.FONY*/ return sprite_scanner.FONY_SCAN5; }

//-----------------------------------------------------------------------------
// Sprite store signals

wire SchematicTop::FEPO_STORE_MATCHp() const { /*p29.FEPO*/ return sst_reg.FEPO_STORE_MATCHp; }

//-----------------------------------------------------------------------------
// Sprite fetcher signals

wire SchematicTop::WEFY_SPR_READp() const {
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1, sprite_fetcher.TESE_SFETCH_S2);
  /*p28.WEFY*/ wire WEFY_SPR_READp = and(TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0_D1);
  return WEFY_SPR_READp;
}

wire SchematicTop::VAPE_FETCH_OAM_CLK() const {
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1, sprite_fetcher.TESE_SFETCH_S2);
  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0);
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1, TYTU_SFETCH_S0_D0n);
  /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  return VAPE_FETCH_OAM_CLK;
}

nwire SchematicTop::VUSA_SPRITE_DONEn() const {
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

pwire SchematicTop::TEXY_SPR_READ_VRAMp() const {
#if 0

  if (XYMU_RENDERINGp) {
    /*p29.TEXY*/ pwire TEXY_SPR_READ_VRAMp = or(TULY_SFETCH_S1, VONU_SFETCH_S1_D4);
  }
  else {
    /*p29.TEXY*/ pwire TEXY_SPR_READ_VRAMp = 0;
  }

#endif
  /*p29.TEPA*/ nwire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
  /*p29.SAKY*/ nwire SAKY = nor(sprite_fetcher.TULY_SFETCH_S1.q(), sprite_fetcher.VONU_SFETCH_S1_D4.q());
  /*p29.TYSO*/ nwire TYSO_SPRITE_READn = or(SAKY, TEPA_RENDERINGn);
  /*p29.TEXY*/ pwire TEXY_SPR_READ_VRAMp = not(TYSO_SPRITE_READn);
  return TEXY_SPR_READ_VRAMp;
}

wire SchematicTop::SOHO_SPR_VRAM_RDp() const {
  /*p29.TYTU*/ nwire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0.q());
  /*p29.TACU*/ pwire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);
  /*p29.ABON*/ nwire ABON_SPR_VRAM_RDp1 = not(TEXY_SPR_READ_VRAMp());
  /*p25.SOHO*/ pwire SOHO_SPR_VRAM_RDp = and (TACU_SPR_SEQ_5_TRIG, ABON_SPR_VRAM_RDp1);
  return SOHO_SPR_VRAM_RDp;
}


wire SchematicTop::SPR_PIX_A0() const { /*p33.PEFO*/ return sprite_fetcher.SPR_PIX_A0; }
wire SchematicTop::SPR_PIX_A1() const { /*p33.ROKA*/ return sprite_fetcher.SPR_PIX_A1; }
wire SchematicTop::SPR_PIX_A2() const { /*p33.MYTU*/ return sprite_fetcher.SPR_PIX_A2; }
wire SchematicTop::SPR_PIX_A3() const { /*p33.RAMU*/ return sprite_fetcher.SPR_PIX_A3; }
wire SchematicTop::SPR_PIX_A4() const { /*p33.SELE*/ return sprite_fetcher.SPR_PIX_A4; }
wire SchematicTop::SPR_PIX_A5() const { /*p33.SUTO*/ return sprite_fetcher.SPR_PIX_A5; }
wire SchematicTop::SPR_PIX_A6() const { /*p33.RAMA*/ return sprite_fetcher.SPR_PIX_A6; }
wire SchematicTop::SPR_PIX_A7() const { /*p33.RYDU*/ return sprite_fetcher.SPR_PIX_A7; }

wire SchematicTop::SPR_PIX_B0() const { /*p33.REWO*/ return sprite_fetcher.SPR_PIX_B0; }
wire SchematicTop::SPR_PIX_B1() const { /*p33.PEBA*/ return sprite_fetcher.SPR_PIX_B1; }
wire SchematicTop::SPR_PIX_B2() const { /*p33.MOFO*/ return sprite_fetcher.SPR_PIX_B2; }
wire SchematicTop::SPR_PIX_B3() const { /*p33.PUDU*/ return sprite_fetcher.SPR_PIX_B3; }
wire SchematicTop::SPR_PIX_B4() const { /*p33.SAJA*/ return sprite_fetcher.SPR_PIX_B4; }
wire SchematicTop::SPR_PIX_B5() const { /*p33.SUNY*/ return sprite_fetcher.SPR_PIX_B5; }
wire SchematicTop::SPR_PIX_B6() const { /*p33.SEMO*/ return sprite_fetcher.SPR_PIX_B6; }
wire SchematicTop::SPR_PIX_B7() const { /*p33.SEGA*/ return sprite_fetcher.SPR_PIX_B7; }

//-----------------------------------------------------------------------------
// Window signals

wire SchematicTop::RYDY_WIN_FIRST_TILE_A() const {
  wire RYDY_WIN_FIRST_TILE_A = win_reg.RYDY_WIN_FIRST_TILE_A;
  return RYDY_WIN_FIRST_TILE_A;
}

wire SchematicTop::NOCU_WIN_MODEn() const {
  /*p27.NOCU*/ return not(win_reg.PYNU_WIN_MODE_A.q());
}

wire SchematicTop::NUNY_WX_MATCHpe() const {
  /*p27.NUNY*/ return win_reg.NUNY_WX_MATCHpe;
}

wire SchematicTop::WYKA_WIN_X3() const { return win_reg.WYKA_WIN_X3; }
wire SchematicTop::WODY_WIN_X4() const { return win_reg.WODY_WIN_X4; }
wire SchematicTop::WOBO_WIN_X5() const { return win_reg.WOBO_WIN_X5; }
wire SchematicTop::WYKO_WIN_X6() const { return win_reg.WYKO_WIN_X6; }
wire SchematicTop::XOLO_WIN_X7() const { return win_reg.XOLO_WIN_X7; }

wire SchematicTop::VYNO_WIN_Y0() const { return win_reg.VYNO_WIN_Y0; }
wire SchematicTop::VUJO_WIN_Y1() const { return win_reg.VUJO_WIN_Y1; }
wire SchematicTop::VYMU_WIN_Y2() const { return win_reg.VYMU_WIN_Y2; }
wire SchematicTop::TUFU_WIN_Y3() const { return win_reg.TUFU_WIN_Y3; }
wire SchematicTop::TAXA_WIN_Y4() const { return win_reg.TAXA_WIN_Y4; }
wire SchematicTop::TOZO_WIN_Y5() const { return win_reg.TOZO_WIN_Y5; }
wire SchematicTop::TATE_WIN_Y6() const { return win_reg.TATE_WIN_Y6; }
wire SchematicTop::TEKE_WIN_Y7() const { return win_reg.TEKE_WIN_Y7; }

//------------------------------------------------------------------------------
// Misc signals

wire SchematicTop::ASOK_INT_JOYPADp() const {
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp = and (joy_reg.APUG_JP_GLITCH3, joy_reg.BATU_JP_GLITCH0);
  return ASOK_INT_JOYPADp;
}

wire SchematicTop::BOOT_BITn() const {
  /*p07.TEPU*/ return bootrom.BOOT_BITn;
}

wire SchematicTop::CALY_INT_SERIALp() const {
  return ser_reg.CALY_INT_SERIALp;
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
  EXT_PIN_WRn_C.preset(true, 0);   // -> P07.UBAL
  EXT_PIN_RDn_C.preset(true, 0);   // -> P07.UJYV
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
