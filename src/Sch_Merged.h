#pragma once
#include "Sch_Common.h"

#include "../src/TextPainter.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- BUS_REG -----\n");
    
    dump_long(text_painter, "BOOT_BITn ", BOOT_BITn.a);
    //dump_long(text_painter, "SOTO_DBG ", SOTO_DBG.a);

    text_painter.dprintf("ADDR_LATCH ");
    dump2(text_painter, CPU_ADDR_LATCH_14.a);
    dump2(text_painter, CPU_ADDR_LATCH_13.a);
    dump2(text_painter, CPU_ADDR_LATCH_12.a);
    text_painter.add_char(':');
    dump2(text_painter, CPU_ADDR_LATCH_11.a);
    dump2(text_painter, CPU_ADDR_LATCH_10.a);
    dump2(text_painter, CPU_ADDR_LATCH_09.a);
    dump2(text_painter, CPU_ADDR_LATCH_08.a);
    text_painter.add_char(':');
    dump2(text_painter, CPU_ADDR_LATCH_07.a);
    dump2(text_painter, CPU_ADDR_LATCH_06.a);
    dump2(text_painter, CPU_ADDR_LATCH_05.a);
    dump2(text_painter, CPU_ADDR_LATCH_04.a);
    text_painter.add_char(':');
    dump2(text_painter, CPU_ADDR_LATCH_03.a);
    dump2(text_painter, CPU_ADDR_LATCH_02.a);
    dump2(text_painter, CPU_ADDR_LATCH_01.a);
    dump2(text_painter, CPU_ADDR_LATCH_00.a);
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
    text_painter.newline();
  }

  /*p07.TEPU*/ Reg BOOT_BITn; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.

  /*p08.ALOR*/ TpLatch CPU_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch CPU_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch CPU_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch CPU_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch CPU_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch CPU_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch CPU_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch CPU_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch CPU_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch CPU_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch CPU_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch CPU_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch CPU_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch CPU_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch CPU_ADDR_LATCH_14;

  /*p08.SOMA*/ TpLatch CART_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch CART_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch CART_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch CART_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch CART_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch CART_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch CART_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch CART_DATA_LATCH_07;
};

//-----------------------------------------------------------------------------

struct ConfigRegisters {

  ConfigRegisters() {
    SCX0.a.val = SCX0.b.val = 1;
  }

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- PPU CFG -----\n");
    dump_long(text_painter, "LCDC_BGEN   ", LCDC_BGEN.a  );
    dump_long(text_painter, "LCDC_SPEN   ", LCDC_SPEN.a  );   
    dump_long(text_painter, "LCDC_SPSIZE ", LCDC_SPSIZE.a);
    dump_long(text_painter, "LCDC_BGMAP  ", LCDC_BGMAP.a );
    dump_long(text_painter, "LCDC_BGTILE ", LCDC_BGTILE.a);
    dump_long(text_painter, "LCDC_WINEN  ", LCDC_WINEN.a );
    dump_long(text_painter, "LCDC_WINMAP ", LCDC_WINMAP.a);
    dump_long(text_painter, "XONA_LCDC_EN     ", XONA_LCDC_EN.a    );

    dump(text_painter,      "SCY         ", SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
    dump(text_painter,      "SCX         ", SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
    //dump(text_painter,      "LYC         ", LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
    dump(text_painter,      "BGP         ", BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
    dump(text_painter,      "OBP0        ", OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
    dump(text_painter,      "OBP1        ", OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);
    dump(text_painter,      "WY          ", WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
    dump(text_painter,      "WX          ", WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);

    text_painter.newline();
  }

  int get_lcdc() const {
    return pack(LCDC_BGEN.q(),
                LCDC_SPEN.q(),
                LCDC_SPSIZE.q(),
                LCDC_BGMAP.q(),
                LCDC_BGTILE.q(),
                LCDC_WINEN.q(),
                LCDC_WINMAP.q(),
                XONA_LCDC_EN.q());
  }

  int get_scy()  const { return pack(SCY0.q(),  SCY1.q(),  SCY2.q(),  SCY3.q(),  SCY4.q(),  SCY5.q(),  SCY6.q(),  SCY7.q()); }
  int get_scx()  const { return pack(SCX0.q(),  SCX1.q(),  SCX2.q(),  SCX3.q(),  SCX4.q(),  SCX5.q(),  SCX6.q(),  SCX7.q()); }
  //int get_lyc()  const { return pack(LYC0.q(),  LYC1.q(),  LYC2.q(),  LYC3.q(),  LYC4.q(),  LYC5.q(),  LYC6.q(),  LYC7.q()); }
  int get_bgp()  const { return pack(BGP0.q(),  BGP1.q(),  BGP2.q(),  BGP3.q(),  BGP4.q(),  BGP5.q(),  BGP6.q(),  BGP7.q()); }
  int get_obp0() const { return pack(OBP00.q(), OBP01.q(), OBP02.q(), OBP03.q(), OBP04.q(), OBP05.q(), OBP06.q(), OBP07.q()); }
  int get_obp1() const { return pack(OBP10.q(), OBP11.q(), OBP12.q(), OBP13.q(), OBP14.q(), OBP15.q(), OBP16.q(), OBP17.q()); }
  int get_wy()   const { return pack(WY0.q(),   WY1.q(),   WY2.q(),   WY3.q(),   WY4.q(),   WY5.q(),   WY6.q(),   WY7.q()); }
  int get_wx()   const { return pack(WX0.q(),   WX1.q(),   WX2.q(),   WX3.q(),   WX4.q(),   WX5.q(),   WX6.q(),   WX7.q()); }

  /*
  void set_lcdc(uint8_t x) {
    big_set2(x, LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
                LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);
  }

  int set_scy(uint8_t x)  { big_set2(x, SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7); }
  int set_scx(uint8_t x)  { big_set2(x, SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7); }
  int set_lyc(uint8_t x)  { big_set2(x, LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7); }
  int set_bgp(uint8_t x)  { big_set2(x, BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7); }
  int set_obp0(uint8_t x) { big_set2(x, OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07); }
  int set_obp1(uint8_t x) { big_set2(x, OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17); }
  int set_wy(uint8_t x)   { big_set2(x, WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7); }
  int set_wx(uint8_t x)   { big_set2(x, WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7); }
  */

  // FF40 - LCDC
  /*p23.VYXE*/ Reg LCDC_BGEN;
  /*p23.XYLO*/ Reg LCDC_SPEN;
  /*p23.XYMO*/ Reg LCDC_SPSIZE;
  /*p23.XAFO*/ Reg LCDC_BGMAP;
  /*p23.WEXU*/ Reg LCDC_BGTILE;
  /*p23.WYMO*/ Reg LCDC_WINEN;
  /*p23.WOKY*/ Reg LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  // FF42 - SCY
  /*p23.GAVE*/ Reg SCY0;
  /*p23.FYMO*/ Reg SCY1;
  /*p23.FEZU*/ Reg SCY2;
  /*p23.FUJO*/ Reg SCY3;
  /*p23.DEDE*/ Reg SCY4;
  /*p23.FOTY*/ Reg SCY5;
  /*p23.FOHA*/ Reg SCY6;
  /*p23.FUNY*/ Reg SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg SCX0;
  /*p23.DUZU*/ Reg SCX1;
  /*p23.CYXU*/ Reg SCX2;
  /*p23.GUBO*/ Reg SCX3;
  /*p23.BEMY*/ Reg SCX4;
  /*p23.CUZY*/ Reg SCX5;
  /*p23.CABU*/ Reg SCX6;
  /*p23.BAKE*/ Reg SCX7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg BGP0;
  /*p36.NUSY*/ Reg BGP1;
  /*p36.PYLU*/ Reg BGP2;
  /*p36.MAXY*/ Reg BGP3;
  /*p36.MUKE*/ Reg BGP4;
  /*p36.MORU*/ Reg BGP5;
  /*p36.MOGY*/ Reg BGP6;
  /*p36.MENA*/ Reg BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg OBP00;
  /*p36.XUKY*/ Reg OBP01;
  /*p36.XOVA*/ Reg OBP02;
  /*p36.XALO*/ Reg OBP03;
  /*p36.XERU*/ Reg OBP04;
  /*p36.XYZE*/ Reg OBP05;
  /*p36.XUPO*/ Reg OBP06;
  /*p36.XANA*/ Reg OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg OBP10;
  /*p36.LAWO*/ Reg OBP11;
  /*p36.MOSA*/ Reg OBP12; // 9-rung, bottom rung is passthrough?
  /*p36.LOSE*/ Reg OBP13;
  /*p36.LUNE*/ Reg OBP14;
  /*p36.LUGU*/ Reg OBP15;
  /*p36.LEPU*/ Reg OBP16;
  /*p36.LUXO*/ Reg OBP17;

  // FF4A - WY
  /*p23.NESO*/ Reg WY0;
  /*p23.NYRO*/ Reg WY1;
  /*p23.NAGA*/ Reg WY2;
  /*p23.MELA*/ Reg WY3;
  /*p23.NULO*/ Reg WY4;
  /*p23.NENE*/ Reg WY5;
  /*p23.NUKA*/ Reg WY6;
  /*p23.NAFU*/ Reg WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg WX0;
  /*p23.NOFE*/ Reg WX1;
  /*p23.NOKE*/ Reg WX2;
  /*p23.MEBY*/ Reg WX3;
  /*p23.MYPU*/ Reg WX4;
  /*p23.MYCE*/ Reg WX5;
  /*p23.MUVO*/ Reg WX6;
  /*p23.NUKU*/ Reg WX7;
};

//-----------------------------------------------------------------------------
// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)

struct InterruptRegisters {

  void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- INT REG -----\n");
    FF0F_0 .dump(text_painter, "FF0F_0  ");
    FF0F_1 .dump(text_painter, "FF0F_1  ");
    FF0F_2 .dump(text_painter, "FF0F_2  ");
    FF0F_3 .dump(text_painter, "FF0F_3  ");
    FF0F_4 .dump(text_painter, "FF0F_4  ");
    FF0F_L0.dump(text_painter, "FF0F_L0 ");
    FF0F_L1.dump(text_painter, "FF0F_L1 ");
    FF0F_L2.dump(text_painter, "FF0F_L2 ");
    FF0F_L3.dump(text_painter, "FF0F_L3 ");
    FF0F_L4.dump(text_painter, "FF0F_L4 ");
    text_painter.newline();
  }

  /*p02.LOPE*/ Reg FF0F_0;
  /*p02.UBUL*/ Reg FF0F_1;
  /*p02.ULAK*/ Reg FF0F_2;
  /*p02.LALU*/ Reg FF0F_3;
  /*p02.NYBO*/ Reg FF0F_4;

  /*p02.MATY*/ TpLatch FF0F_L0;
  /*p02.NEJY*/ TpLatch FF0F_L1;
  /*p02.NUTY*/ TpLatch FF0F_L2;
  /*p02.MOPO*/ TpLatch FF0F_L3;
  /*p02.PAVY*/ TpLatch FF0F_L4;
};

//-----------------------------------------------------------------------------

};