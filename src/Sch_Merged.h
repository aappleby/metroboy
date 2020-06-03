#pragma once
#include "Sch_Common.h"

#include "../src/TextPainter.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClkRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- CLK_REG -----\n");
    dump_long(text_painter, "PHAZ_xBCDExxx ", PHAZ_xBCDExxx.a);
    dump_long(text_painter, "PHAZ_xxCDEFxx ", PHAZ_xxCDEFxx.a);
    dump_long(text_painter, "PHAZ_xxxDEFGx ", PHAZ_xxxDEFGx.a);
    dump_long(text_painter, "PHAZ_xxxxEFGH ", PHAZ_xxxxEFGH.a);
    text_painter.newline();
  }

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ RegDuo PHAZ_xBCDExxx;
  /*p01.ALEF*/ RegDuo PHAZ_xxCDEFxx;
  /*p01.APUK*/ RegDuo PHAZ_xxxDEFGx;
  /*p01.ADYK*/ RegDuo PHAZ_xxxxEFGH;
};

//-----------------------------------------------------------------------------

struct BusRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- BUS_REG -----\n");
    
    dump_long(text_painter, "BOOT_BIT ", BOOT_BIT.a);
    dump_long(text_painter, "SOTO_DBG ", SOTO_DBG.a);

    text_painter.dprintf("ADDR_LATCH ");
    dump2(text_painter, INT_ADDR_LATCH_14.a);
    dump2(text_painter, INT_ADDR_LATCH_13.a);
    dump2(text_painter, INT_ADDR_LATCH_12.a);
    text_painter.add_char(':');
    dump2(text_painter, INT_ADDR_LATCH_11.a);
    dump2(text_painter, INT_ADDR_LATCH_10.a);
    dump2(text_painter, INT_ADDR_LATCH_09.a);
    dump2(text_painter, INT_ADDR_LATCH_08.a);
    text_painter.add_char(':');
    dump2(text_painter, INT_ADDR_LATCH_07.a);
    dump2(text_painter, INT_ADDR_LATCH_06.a);
    dump2(text_painter, INT_ADDR_LATCH_05.a);
    dump2(text_painter, INT_ADDR_LATCH_04.a);
    text_painter.add_char(':');
    dump2(text_painter, INT_ADDR_LATCH_03.a);
    dump2(text_painter, INT_ADDR_LATCH_02.a);
    dump2(text_painter, INT_ADDR_LATCH_01.a);
    dump2(text_painter, INT_ADDR_LATCH_00.a);
    text_painter.newline();

    text_painter.dprintf("DATA_LATCH ");
    dump2(text_painter, EXT_DATA_LATCH_07.a);
    dump2(text_painter, EXT_DATA_LATCH_06.a);
    dump2(text_painter, EXT_DATA_LATCH_05.a);
    dump2(text_painter, EXT_DATA_LATCH_04.a);
    text_painter.add_char(':');
    dump2(text_painter, EXT_DATA_LATCH_03.a);
    dump2(text_painter, EXT_DATA_LATCH_02.a);
    dump2(text_painter, EXT_DATA_LATCH_01.a);
    dump2(text_painter, EXT_DATA_LATCH_00.a);
    text_painter.newline();
    text_painter.newline();
  }

  /*p07.TEPU*/ Reg3 BOOT_BIT;
  /*p25.SOTO*/ Reg3 SOTO_DBG;

  /*p08.ALOR*/ Latch3 INT_ADDR_LATCH_00;
  /*p08.APUR*/ Latch3 INT_ADDR_LATCH_01;
  /*p08.ALYR*/ Latch3 INT_ADDR_LATCH_02;
  /*p08.ARET*/ Latch3 INT_ADDR_LATCH_03;
  /*p08.AVYS*/ Latch3 INT_ADDR_LATCH_04;
  /*p08.ATEV*/ Latch3 INT_ADDR_LATCH_05;
  /*p08.AROS*/ Latch3 INT_ADDR_LATCH_06;
  /*p08.ARYM*/ Latch3 INT_ADDR_LATCH_07;
  /*p08.LUNO*/ Latch3 INT_ADDR_LATCH_08;
  /*p08.LYSA*/ Latch3 INT_ADDR_LATCH_09;
  /*p08.PATE*/ Latch3 INT_ADDR_LATCH_10;
  /*p08.LUMY*/ Latch3 INT_ADDR_LATCH_11;
  /*p08.LOBU*/ Latch3 INT_ADDR_LATCH_12;
  /*p08.LONU*/ Latch3 INT_ADDR_LATCH_13;
  /*p08.NYRE*/ Latch3 INT_ADDR_LATCH_14;

  /*p08.SOMA*/ Latch3 EXT_DATA_LATCH_00;
  /*p08.RONY*/ Latch3 EXT_DATA_LATCH_01;
  /*p08.RAXY*/ Latch3 EXT_DATA_LATCH_02;
  /*p08.SELO*/ Latch3 EXT_DATA_LATCH_03;
  /*p08.SODY*/ Latch3 EXT_DATA_LATCH_04;
  /*p08.SAGO*/ Latch3 EXT_DATA_LATCH_05;
  /*p08.RUPA*/ Latch3 EXT_DATA_LATCH_06;
  /*p08.SAZY*/ Latch3 EXT_DATA_LATCH_07;
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
    dump_long(text_painter, "LCDC_EN     ", LCDC_EN.a    );

    dump(text_painter,      "SCY         ", SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
    dump(text_painter,      "SCX         ", SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
    dump(text_painter,      "LYC         ", LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
    dump(text_painter,      "BGP         ", BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
    dump(text_painter,      "OBP0        ", OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
    dump(text_painter,      "OBP1        ", OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);
    dump(text_painter,      "WY          ", WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
    dump(text_painter,      "WX          ", WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);

    text_painter.newline();
  }

  int get_lcdc() const {
    return pack(LCDC_BGEN,
                LCDC_SPEN,
                LCDC_SPSIZE,
                LCDC_BGMAP,
                LCDC_BGTILE,
                LCDC_WINEN,
                LCDC_WINMAP,
                LCDC_EN);
  }

  int get_scy()  const { return pack(SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7); }
  int get_scx()  const { return pack(SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7); }
  int get_lyc()  const { return pack(LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7); }
  int get_bgp()  const { return pack(BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7); }
  int get_obp0() const { return pack(OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07); }
  int get_obp1() const { return pack(OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17); }
  int get_wy()   const { return pack(WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7); }
  int get_wx()   const { return pack(WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7); }

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
  /*p23.VYXE*/ Reg3 LCDC_BGEN;
  /*p23.XYLO*/ Reg3 LCDC_SPEN;
  /*p23.XYMO*/ Reg3 LCDC_SPSIZE;
  /*p23.XAFO*/ Reg3 LCDC_BGMAP;
  /*p23.WEXU*/ Reg3 LCDC_BGTILE;
  /*p23.WYMO*/ Reg3 LCDC_WINEN;
  /*p23.WOKY*/ Reg3 LCDC_WINMAP;
  /*p23.XONA*/ Reg3 LCDC_EN;

  // FF42 - SCY
  /*p23.GAVE*/ Reg3 SCY0;
  /*p23.FYMO*/ Reg3 SCY1;
  /*p23.FEZU*/ Reg3 SCY2;
  /*p23.FUJO*/ Reg3 SCY3;
  /*p23.DEDE*/ Reg3 SCY4;
  /*p23.FOTY*/ Reg3 SCY5;
  /*p23.FOHA*/ Reg3 SCY6;
  /*p23.FUNY*/ Reg3 SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg3 SCX0;
  /*p23.DUZU*/ Reg3 SCX1;
  /*p23.CYXU*/ Reg3 SCX2;
  /*p23.GUBO*/ Reg3 SCX3;
  /*p23.BEMY*/ Reg3 SCX4;
  /*p23.CUZY*/ Reg3 SCX5;
  /*p23.CABU*/ Reg3 SCX6;
  /*p23.BAKE*/ Reg3 SCX7;

  // FF45 - LYC
  /*p23.SYRY*/ Reg3 LYC0;
  /*p23.VUCE*/ Reg3 LYC1;
  /*p23.SEDY*/ Reg3 LYC2;
  /*p23.SALO*/ Reg3 LYC3;
  /*p23.SOTA*/ Reg3 LYC4;
  /*p23.VAFA*/ Reg3 LYC5;
  /*p23.VEVO*/ Reg3 LYC6;
  /*p23.RAHA*/ Reg3 LYC7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg3 BGP0;
  /*p36.NUSY*/ Reg3 BGP1;
  /*p36.PYLU*/ Reg3 BGP2;
  /*p36.MAXY*/ Reg3 BGP3;
  /*p36.MUKE*/ Reg3 BGP4;
  /*p36.MORU*/ Reg3 BGP5;
  /*p36.MOGY*/ Reg3 BGP6;
  /*p36.MENA*/ Reg3 BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg3 OBP00;
  /*p36.XUKY*/ Reg3 OBP01;
  /*p36.XOVA*/ Reg3 OBP02;
  /*p36.XALO*/ Reg3 OBP03;
  /*p36.XERU*/ Reg3 OBP04;
  /*p36.XYZE*/ Reg3 OBP05;
  /*p36.XUPO*/ Reg3 OBP06;
  /*p36.XANA*/ Reg3 OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg3 OBP10;
  /*p36.LAWO*/ Reg3 OBP11;
  /*p36.MOSA*/ Reg3 OBP12;
  /*p36.LOSE*/ Reg3 OBP13;
  /*p36.LUNE*/ Reg3 OBP14;
  /*p36.LUGU*/ Reg3 OBP15;
  /*p36.LEPU*/ Reg3 OBP16;
  /*p36.LUXO*/ Reg3 OBP17;

  // FF4A - WY
  /*p23.NESO*/ Reg3 WY0;
  /*p23.NYRO*/ Reg3 WY1;
  /*p23.NAGA*/ Reg3 WY2;
  /*p23.MELA*/ Reg3 WY3;
  /*p23.NULO*/ Reg3 WY4;
  /*p23.NENE*/ Reg3 WY5;
  /*p23.NUKA*/ Reg3 WY6;
  /*p23.NAFU*/ Reg3 WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg3 WX0;
  /*p23.NOFE*/ Reg3 WX1;
  /*p23.NOKE*/ Reg3 WX2;
  /*p23.MEBY*/ Reg3 WX3;
  /*p23.MYPU*/ Reg3 WX4;
  /*p23.MYCE*/ Reg3 WX5;
  /*p23.MUVO*/ Reg3 WX6;
  /*p23.NUKU*/ Reg3 WX7;
};

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- DBG REG ----- \n");
    FF60_0.dump(text_painter, "FF60_0 ");
    FF60_1.dump(text_painter, "FF60_1 ");
    text_painter.newline();
  }

  /*p07.BURO*/ Reg3 FF60_0;
  /*p07.AMUT*/ Reg3 FF60_1;
};

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- DMA REG -----\n");
    FROM_CPU5_SYNC   .dump(text_painter, "FROM_CPU5_SYNC   ");
    REG_DMA_RUNNING  .dump(text_painter, "REG_DMA_RUNNING  ");
    DMA_DONE_SYNC    .dump(text_painter, "DMA_DONE_SYNC    ");
    REG_DMA_EN_d0    .dump(text_painter, "REG_DMA_EN_d0    ");  
    REG_DMA_EN_d4    .dump(text_painter, "REG_DMA_EN_d4    ");
    LATCH_DMA_ENn_d0 .dump(text_painter, "LATCH_DMA_ENn_d0 ");
    LATCH_DMA_EN_d4  .dump(text_painter, "LATCH_DMA_EN_d4  ");
    text_painter.dprintf("DMA ADDR LO      0x%02x\n", get_addr_lo());
    text_painter.dprintf("DMA ADDR HI      0x%02x\n", get_addr_hi());
    text_painter.newline();
  }

  /*p04.MAKA*/ Reg3 FROM_CPU5_SYNC;

  /*p04.MATU*/ Reg3 REG_DMA_RUNNING; // -> p25,p28
  /*p04.MYTE*/ Reg3 DMA_DONE_SYNC;
  /*p04.LUVY*/ Reg3 REG_DMA_EN_d0;
  /*p04.LENE*/ Reg3 REG_DMA_EN_d4;

  int get_addr_lo() {
    return pack(DMA_A00, DMA_A01, DMA_A02, DMA_A03, DMA_A04, DMA_A05, DMA_A06, DMA_A07);
  }
  int get_addr_hi() {
    return pack(DMA_A08, DMA_A09, DMA_A10, DMA_A11, DMA_A12, DMA_A13, DMA_A14, DMA_A15);
  }

  /*p04.NAKY*/ Reg3 DMA_A00;
  /*p04.PYRO*/ Reg3 DMA_A01; 
  /*p04.NEFY*/ Reg3 DMA_A02; 
  /*p04.MUTY*/ Reg3 DMA_A03; 
  /*p04.NYKO*/ Reg3 DMA_A04; 
  /*p04.PYLO*/ Reg3 DMA_A05; 
  /*p04.NUTO*/ Reg3 DMA_A06; 
  /*p04.MUGU*/ Reg3 DMA_A07; 
  /*p04.NAFA*/ Reg3 DMA_A08; 
  /*p04.PYNE*/ Reg3 DMA_A09; 
  /*p04.PARA*/ Reg3 DMA_A10; 
  /*p04.NYDO*/ Reg3 DMA_A11; 
  /*p04.NYGY*/ Reg3 DMA_A12; 
  /*p04.PULA*/ Reg3 DMA_A13; 
  /*p04.POKU*/ Reg3 DMA_A14; 
  /*p04.MARU*/ Reg3 DMA_A15; 

  /*p04.LYXE*/ Latch3 LATCH_DMA_ENn_d0;
  /*p04.LOKY*/ Latch3 LATCH_DMA_EN_d4; // NAND latch
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

  /*p02.LOPE*/ Reg3 FF0F_0;
  /*p02.UBUL*/ Reg3 FF0F_1;
  /*p02.ULAK*/ Reg3 FF0F_2;
  /*p02.LALU*/ Reg3 FF0F_3;
  /*p02.NYBO*/ Reg3 FF0F_4;

  /*p02.MATY*/ Latch3 FF0F_L0;
  /*p02.NEJY*/ Latch3 FF0F_L1;
  /*p02.NUTY*/ Latch3 FF0F_L2;
  /*p02.MOPO*/ Latch3 FF0F_L3;
  /*p02.PAVY*/ Latch3 FF0F_L4;
};

//-----------------------------------------------------------------------------

struct JoypadRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- JOY REG -----\n");
    JP_GLITCH0 .dump(text_painter, "JP_GLITCH0  ");
    JP_GLITCH1 .dump(text_painter, "JP_GLITCH1  ");
    JP_GLITCH2 .dump(text_painter, "JP_GLITCH2  ");
    JP_GLITCH3 .dump(text_painter, "JP_GLITCH3  ");
    JOYP_RA    .dump(text_painter, "JOYP_RA     ");
    JOYP_LB    .dump(text_painter, "JOYP_LB     ");
    JOYP_UC    .dump(text_painter, "JOYP_UC     ");
    JOYP_DS    .dump(text_painter, "JOYP_DS     ");
    JOYP_UDLR  .dump(text_painter, "JOYP_UDLR   ");
    JOYP_ABCS  .dump(text_painter, "JOYP_ABCS   ");
    DBG_FF00_D6.dump(text_painter, "DBG_FF00_D6 ");
    DBG_FF00_D7.dump(text_painter, "DBG_FF00_D7 ");
    JOYP_L0    .dump(text_painter, "JOYP_L0     ");
    JOYP_L1    .dump(text_painter, "JOYP_L1     ");
    JOYP_L2    .dump(text_painter, "JOYP_L2     ");
    JOYP_L3    .dump(text_painter, "JOYP_L3     ");
    WAKE_CPU   .dump(text_painter, "WAKE_CPU    ");
    text_painter.newline();
  }

  /*p02.BATU*/ Reg3 JP_GLITCH0;
  /*p02.ACEF*/ Reg3 JP_GLITCH1;
  /*p02.AGEM*/ Reg3 JP_GLITCH2;
  /*p02.APUG*/ Reg3 JP_GLITCH3;
  /*p05.JUTE*/ Reg3 JOYP_RA;
  /*p05.KECY*/ Reg3 JOYP_LB;
  /*p05.JALE*/ Reg3 JOYP_UC;
  /*p05.KYME*/ Reg3 JOYP_DS;
  /*p05.KELY*/ Reg3 JOYP_UDLR;
  /*p05.COFY*/ Reg3 JOYP_ABCS;
  /*p05.KUKO*/ Reg3 DBG_FF00_D6;
  /*p05.KERU*/ Reg3 DBG_FF00_D7;
  /*p05.KEVU*/ Reg3 JOYP_L0;
  /*p05.KAPA*/ Reg3 JOYP_L1;
  /*p05.KEJA*/ Reg3 JOYP_L2;
  /*p05.KOLO*/ Reg3 JOYP_L3;
  /*p02.AWOB*/ Reg3 WAKE_CPU;
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- LCD REG -----\n");

    dump(text_painter, "LCD X ", X0, X1, X2, X3, X4, X5, X6);
    dump(text_painter, "LCD Y ", Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7);

    dump_long(text_painter, "RUTU_NEW_LINE_d0  ", RUTU_NEW_LINE_d0.a );
    dump_long(text_painter, "VID_LINE_d4       ", VID_LINE_d4.a      );
    dump_long(text_painter, "NYPE_NEW_LINE_d4  ", NYPE_NEW_LINE_d4.a );
    dump_long(text_painter, "VID_LINE_d6       ", VID_LINE_d6.a      );
    dump_long(text_painter, "LINE_153_d4       ", LINE_153_d4.a      );
    dump_long(text_painter, "POPU_IN_VBLANK_d4 ", POPU_IN_VBLANK_d4.a);
    dump_long(text_painter, "LINE_STROBE       ", LINE_STROBE.a      );
    dump_long(text_painter, "X_8_SYNC          ", X_8_SYNC.a         );
    dump_long(text_painter, "CPEN_LATCH        ", CPEN_LATCH.a       );
    dump_long(text_painter, "POME              ", POME.a             );
    dump_long(text_painter, "RUJU              ", RUJU.a             );
    dump_long(text_painter, "VSYNC_OUTn        ", VSYNC_OUTn.a       );
    dump_long(text_painter, "LINE_EVEN         ", LINE_EVEN.a        );
    dump_long(text_painter, "FRAME_EVEN        ", FRAME_EVEN.a       );
    text_painter.newline();
  }

  /*p21.SAXO*/ Reg3 X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg3 X1;
  /*p21.VYZO*/ Reg3 X2;
  /*p21.TELU*/ Reg3 X3;
  /*p21.SUDE*/ Reg3 X4;
  /*p21.TAHA*/ Reg3 X5;
  /*p21.TYRY*/ Reg3 X6;

  /*p21.MUWY*/ Reg3 Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg3 Y1;
  /*p21.LEXA*/ Reg3 Y2;
  /*p21.LYDO*/ Reg3 Y3;
  /*p21.LOVU*/ Reg3 Y4;
  /*p21.LEMA*/ Reg3 Y5;
  /*p21.MATO*/ Reg3 Y6;
  /*p21.LAFO*/ Reg3 Y7;

  /*p21.RUTU*/ Reg3 RUTU_NEW_LINE_d0;     // p909+8
  /*p29.CATU*/ Reg3 VID_LINE_d4;          // p001+8
  /*p21.NYPE*/ Reg3 NYPE_NEW_LINE_d4;     // p001+8
  /*p28.ANEL*/ Reg3 VID_LINE_d6;          // p003+8
                                 
  /*p21.MYTA*/ Reg3 LINE_153_d4;          // p153:001 - p000:001
  /*p21.POPU*/ Reg3 POPU_IN_VBLANK_d4;    // p144:001 - p000:001

  /*p21.SYGU*/ Reg3 LINE_STROBE;
  /*p24.PAHO*/ Reg3 X_8_SYNC;

  /*p21.WUSA*/ Latch3 CPEN_LATCH;

  /*p24.POME*/ Latch3 POME; // nand latch with p24.RUJU
  /*p24.RUJU*/ Latch3 RUJU; // nand latch with p24.POME

  /*p24.MEDA*/ Reg3 VSYNC_OUTn;
  /*p24.LUCA*/ Reg3 LINE_EVEN;
  /*p21.NAPO*/ Reg3 FRAME_EVEN;
};

//-----------------------------------------------------------------------------

struct PixelPipeRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- PXP_REG -----\n");
    text_painter.dprintf("BG_PIX_A   0x%02x\n", bg_pix_a());
    text_painter.dprintf("BG_PIX_B   0x%02x\n", bg_pix_b());
    text_painter.dprintf("SPR_PIX_A  0x%02x\n", spr_pix_a());
    text_painter.dprintf("SPR_PIX_B  0x%02x\n", spr_pix_b());
    text_painter.dprintf("BG_PIPE_A  0x%02x\n", bg_pipe_a());
    text_painter.dprintf("BG_PIPE_B  0x%02x\n", bg_pipe_b());
    text_painter.dprintf("SPR_PIPE_A 0x%02x\n", spr_pipe_a());
    text_painter.dprintf("SPR_PIPE_B 0x%02x\n", spr_pipe_b());
    text_painter.dprintf("PAL_PIPE   0x%02x\n", pal_pipe());
    text_painter.dprintf("MASK_PIPE  0x%02x\n", mask_pipe());
    text_painter.newline();
  }

  uint8_t bg_pix_a()   { return (uint8_t)pack(BG_PIX_A0,   BG_PIX_A1,   BG_PIX_A2,   BG_PIX_A3,   BG_PIX_A4,   BG_PIX_A5,   BG_PIX_A6,   BG_PIX_A7);   }
  uint8_t bg_pix_b()   { return (uint8_t)pack(BG_PIX_B0,   BG_PIX_B1,   BG_PIX_B2,   BG_PIX_B3,   BG_PIX_B4,   BG_PIX_B5,   BG_PIX_B6,   BG_PIX_B7);   }
  uint8_t spr_pix_a()  { return (uint8_t)pack(SPR_PIX_A0,  SPR_PIX_A1,  SPR_PIX_A2,  SPR_PIX_A3,  SPR_PIX_A4,  SPR_PIX_A5,  SPR_PIX_A6,  SPR_PIX_A7);  }
  uint8_t spr_pix_b()  { return (uint8_t)pack(SPR_PIX_B0,  SPR_PIX_B1,  SPR_PIX_B2,  SPR_PIX_B3,  SPR_PIX_B4,  SPR_PIX_B5,  SPR_PIX_B6,  SPR_PIX_B7);  }
  uint8_t bg_pipe_a()  { return (uint8_t)pack(BG_PIPE_A0,  BG_PIPE_A1,  BG_PIPE_A2,  BG_PIPE_A3,  BG_PIPE_A4,  BG_PIPE_A5,  BG_PIPE_A6,  BG_PIPE_A7);  }
  uint8_t bg_pipe_b()  { return (uint8_t)pack(BG_PIPE_B0,  BG_PIPE_B1,  BG_PIPE_B2,  BG_PIPE_B3,  BG_PIPE_B4,  BG_PIPE_B5,  BG_PIPE_B6,  BG_PIPE_B7);  }
  uint8_t spr_pipe_a() { return (uint8_t)pack(SPR_PIPE_A0, SPR_PIPE_A1, SPR_PIPE_A2, SPR_PIPE_A3, SPR_PIPE_A4, SPR_PIPE_A5, SPR_PIPE_A6, SPR_PIPE_A7); }
  uint8_t spr_pipe_b() { return (uint8_t)pack(SPR_PIPE_B0, SPR_PIPE_B1, SPR_PIPE_B2, SPR_PIPE_B3, SPR_PIPE_B4, SPR_PIPE_B5, SPR_PIPE_B6, SPR_PIPE_B7); }
  uint8_t pal_pipe()   { return (uint8_t)pack(PAL_PIPE_0,  PAL_PIPE_1,  PAL_PIPE_2,  PAL_PIPE_3,  PAL_PIPE_4,  PAL_PIPE_5,  PAL_PIPE_6,  PAL_PIPE_7);  }
  uint8_t mask_pipe()  { return (uint8_t)pack(MASK_PIPE_0, MASK_PIPE_1, MASK_PIPE_2, MASK_PIPE_3, MASK_PIPE_4, MASK_PIPE_5, MASK_PIPE_6, MASK_PIPE_7); }

  /*p32.LEGU*/ Latch3 BG_PIX_A0;
  /*p32.NUDU*/ Latch3 BG_PIX_A1;
  /*p32.MUKU*/ Latch3 BG_PIX_A2;
  /*p32.LUZO*/ Latch3 BG_PIX_A3;
  /*p32.MEGU*/ Latch3 BG_PIX_A4;
  /*p32.MYJY*/ Latch3 BG_PIX_A5;
  /*p32.NASA*/ Latch3 BG_PIX_A6;
  /*p32.NEFO*/ Latch3 BG_PIX_A7;

  /*p32.RAWU*/ Reg3 BG_PIX_B0;
  /*p32.POZO*/ Reg3 BG_PIX_B1;
  /*p32.PYZO*/ Reg3 BG_PIX_B2;
  /*p32.POXA*/ Reg3 BG_PIX_B3;
  /*p32.PULO*/ Reg3 BG_PIX_B4;
  /*p32.POJU*/ Reg3 BG_PIX_B5;
  /*p32.POWY*/ Reg3 BG_PIX_B6;
  /*p32.PYJU*/ Reg3 BG_PIX_B7;

  /*p33.PEFO*/ Latch3 SPR_PIX_A0;
  /*p33.ROKA*/ Latch3 SPR_PIX_A1;
  /*p33.MYTU*/ Latch3 SPR_PIX_A2;
  /*p33.RAMU*/ Latch3 SPR_PIX_A3;
  /*p33.SELE*/ Latch3 SPR_PIX_A4;
  /*p33.SUTO*/ Latch3 SPR_PIX_A5;
  /*p33.RAMA*/ Latch3 SPR_PIX_A6;
  /*p33.RYDU*/ Latch3 SPR_PIX_A7;

  /*p33.REWO*/ Latch3 SPR_PIX_B0;
  /*p33.PEBA*/ Latch3 SPR_PIX_B1;
  /*p33.MOFO*/ Latch3 SPR_PIX_B2;
  /*p33.PUDU*/ Latch3 SPR_PIX_B3;
  /*p33.SAJA*/ Latch3 SPR_PIX_B4;
  /*p33.SUNY*/ Latch3 SPR_PIX_B5;
  /*p33.SEMO*/ Latch3 SPR_PIX_B6;
  /*p33.SEGA*/ Latch3 SPR_PIX_B7;

  /*p32.MYDE*/ Reg3 BG_PIPE_A0;
  /*p32.NOZO*/ Reg3 BG_PIPE_A1;
  /*p32.MOJU*/ Reg3 BG_PIPE_A2;
  /*p32.MACU*/ Reg3 BG_PIPE_A3;
  /*p32.NEPO*/ Reg3 BG_PIPE_A4;
  /*p32.MODU*/ Reg3 BG_PIPE_A5;
  /*p32.NEDA*/ Reg3 BG_PIPE_A6;
  /*p32.PYBO*/ Reg3 BG_PIPE_A7;

  /*p32.TOMY*/ Reg3 BG_PIPE_B0;
  /*p32.TACA*/ Reg3 BG_PIPE_B1;
  /*p32.SADY*/ Reg3 BG_PIPE_B2;
  /*p32.RYSA*/ Reg3 BG_PIPE_B3;
  /*p32.SOBO*/ Reg3 BG_PIPE_B4;
  /*p32.SETU*/ Reg3 BG_PIPE_B5;
  /*p32.RALU*/ Reg3 BG_PIPE_B6;
  /*p32.SOHU*/ Reg3 BG_PIPE_B7;

  /*p33.NURO*/ Reg3 SPR_PIPE_B0;
  /*p33.MASO*/ Reg3 SPR_PIPE_B1;
  /*p33.LEFE*/ Reg3 SPR_PIPE_B2;
  /*p33.LESU*/ Reg3 SPR_PIPE_B3;
  /*p33.WYHO*/ Reg3 SPR_PIPE_B4;
  /*p33.WORA*/ Reg3 SPR_PIPE_B5;
  /*p33.VAFO*/ Reg3 SPR_PIPE_B6;
  /*p33.WUFY*/ Reg3 SPR_PIPE_B7;

  /*p33.NYLU*/ Reg3 SPR_PIPE_A0;
  /*p33.PEFU*/ Reg3 SPR_PIPE_A1;
  /*p33.NATY*/ Reg3 SPR_PIPE_A2;
  /*p33.PYJO*/ Reg3 SPR_PIPE_A3;
  /*p33.VARE*/ Reg3 SPR_PIPE_A4;
  /*p33.WEBA*/ Reg3 SPR_PIPE_A5;
  /*p33.VANU*/ Reg3 SPR_PIPE_A6;
  /*p33.VUPY*/ Reg3 SPR_PIPE_A7;

  /*p34.RUGO*/ Reg3 PAL_PIPE_0;
  /*p34.SATA*/ Reg3 PAL_PIPE_1;
  /*p34.ROSA*/ Reg3 PAL_PIPE_2;
  /*p34.SOMY*/ Reg3 PAL_PIPE_3;
  /*p34.PALU*/ Reg3 PAL_PIPE_4;
  /*p34.NUKE*/ Reg3 PAL_PIPE_5;
  /*p34.MODA*/ Reg3 PAL_PIPE_6;
  /*p34.LYME*/ Reg3 PAL_PIPE_7;

  /*p26.VEZO*/ Reg3 MASK_PIPE_0;
  /*p26.WURU*/ Reg3 MASK_PIPE_1;
  /*p26.VOSA*/ Reg3 MASK_PIPE_2;
  /*p26.WYFU*/ Reg3 MASK_PIPE_3;
  /*p26.XETE*/ Reg3 MASK_PIPE_4;
  /*p26.WODA*/ Reg3 MASK_PIPE_5;
  /*p26.VUMO*/ Reg3 MASK_PIPE_6;
  /*p26.VAVA*/ Reg3 MASK_PIPE_7;
};

//-----------------------------------------------------------------------------

struct RstRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- RST_REG -----\n");
    WAITING_FOR_CLKREQ.dump(text_painter, "WAITING_FOR_CLKREQ ");
    RESET_REG         .dump(text_painter, "RESET_REG          ");
    text_painter.newline();
  }

  /*p01.TUBO*/ Latch3 WAITING_FOR_CLKREQ;
  /*p01.AFER*/ Reg3 RESET_REG;
};

//-----------------------------------------------------------------------------

struct SerialRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SER_REG -----\n");
    text_painter.dprintf("SER_CLK    %d\n", SER_CLK.a.val);
    text_painter.dprintf("XFER_START %d\n", XFER_START.a.val);
    text_painter.dprintf("XFER_DIR   %d\n", XFER_DIR.a.val);
    text_painter.dprintf("SER_OUT    %d\n", SER_OUT.a.val);
    text_painter.dprintf("SER_CNT    %d\n", ser_cnt());
    text_painter.dprintf("SER_DATA   %d\n", ser_data());
    text_painter.newline();
  }

  uint8_t ser_cnt()  { return (uint8_t)pack(SER_CNT0,SER_CNT1,SER_CNT2,SER_CNT3); }
  uint8_t ser_data() { return (uint8_t)pack(SER_DATA0,SER_DATA1,SER_DATA2,SER_DATA3,SER_DATA4,SER_DATA5,SER_DATA6,SER_DATA7); }

  /*p06.ETAF*/ Reg3 XFER_START;
  /*p06.CULY*/ Reg3 XFER_DIR;

  /*p06.COTY*/ Reg3 SER_CLK;

  /*p06.CAFA*/ Reg3 SER_CNT0;
  /*p06.CYLO*/ Reg3 SER_CNT1;
  /*p06.CYDE*/ Reg3 SER_CNT2;
  /*p06.CALY*/ Reg3 SER_CNT3;

  /*p06.CUBA*/ Reg3 SER_DATA0;
  /*p06.DEGU*/ Reg3 SER_DATA1;
  /*p06.DYRA*/ Reg3 SER_DATA2;
  /*p06.DOJO*/ Reg3 SER_DATA3;
  /*p06.DOVU*/ Reg3 SER_DATA4;
  /*p06.EJAB*/ Reg3 SER_DATA5;
  /*p06.EROD*/ Reg3 SER_DATA6;
  /*p06.EDER*/ Reg3 SER_DATA7;

  /*p06.ELYS*/ Reg3 SER_OUT;
};

//-----------------------------------------------------------------------------

struct SpriteRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_REG -----\n");

    TOXE_SPR_SEQ0    .dump(text_painter, "TOXE_SPR_SEQ0    ");
    TULY_SPR_SEQ1    .dump(text_painter, "TULY_SPR_SEQ1    ");
    TESE_SPR_SEQ2    .dump(text_painter, "TESE_SPR_SEQ2    ");
    TOBU_SEQ_xx23xx  .dump(text_painter, "TOBU_SEQ_xx23xx  ");
    VONU_SEQ_xxx34xn .dump(text_painter, "VONU_SEQ_xxx34xn ");
    SEBA_SEQ_xxxx45n .dump(text_painter, "SEBA_SEQ_xxxx45n ");
    TYFO_SEQ_B0d     .dump(text_painter, "TYFO_SEQ_B0d     ");
    STORE_SPRITE_IDXn.dump(text_painter, "STORE_SPRITE_IDXn");

    text_painter.dprintf("SCAN    %d\n", scan());
    SCAN_DONE_d4.dump(text_painter, "SCAN_DONE_d4 ");
    SCAN_DONE_d5.dump(text_painter, "SCAN_DONE_d5 ");

    text_painter.dprintf("SPR_IDX %d\n", spr_idx());
    text_painter.dprintf("TS_IDX  %d\n", ts_idx());
    text_painter.dprintf("TS_LINE %d\n", ts_line());
    text_painter.newline();
  }

  int scan()    { return pack(SCAN0, SCAN1, SCAN2, SCAN3, SCAN4); }
  int spr_idx() { return pack(SPRITE_IDX0,SPRITE_IDX1,SPRITE_IDX2,SPRITE_IDX3,SPRITE_IDX4,SPRITE_IDX5); }
  int ts_idx()  { return pack(TS_IDX_0,TS_IDX_1,TS_IDX_2,TS_IDX_3,TS_IDX_4,TS_IDX_5); }
  int ts_line() { return pack(TS_LINE_0,TS_LINE_1,TS_LINE_2,TS_LINE_3); }

  /*p29.TOXE*/ Reg3 TOXE_SPR_SEQ0;
  /*p29.TULY*/ Reg3 TULY_SPR_SEQ1;
  /*p29.TESE*/ Reg3 TESE_SPR_SEQ2;
  
  /*p29.TOBU*/ Reg3 TOBU_SEQ_xx23xx;
  /*p29.VONU*/ Reg3 VONU_SEQ_xxx34xn;
  /*p29.SEBA*/ Reg3 SEBA_SEQ_xxxx45n;
  /*p29.TYFO*/ Reg3 TYFO_SEQ_B0d;

  /*p29.CENO*/ Reg3 STORE_SPRITE_IDXn;

  /*p28.YFEL*/ Reg3 SCAN0;
  /*p28.WEWY*/ Reg3 SCAN1;
  /*p28.GOSO*/ Reg3 SCAN2;
  /*p28.ELYN*/ Reg3 SCAN3;
  /*p28.FAHA*/ Reg3 SCAN4;
  /*p28.FONY*/ Reg3 SCAN5;
  /*p29.BYBA*/ Reg3 SCAN_DONE_d4;
  /*p29.DOBA*/ Reg3 SCAN_DONE_d5;

  /*p30.XADU*/ Reg3 SPRITE_IDX0;
  /*p30.XEDY*/ Reg3 SPRITE_IDX1;
  /*p30.ZUZE*/ Reg3 SPRITE_IDX2;
  /*p30.XOBE*/ Reg3 SPRITE_IDX3;
  /*p30.YDUF*/ Reg3 SPRITE_IDX4;
  /*p30.XECU*/ Reg3 SPRITE_IDX5;
  
  Tribuf TS_IDX_0;
  Tribuf TS_IDX_1;
  Tribuf TS_IDX_2;
  Tribuf TS_IDX_3;
  Tribuf TS_IDX_4;
  Tribuf TS_IDX_5;

  Tribuf TS_LINE_0;
  Tribuf TS_LINE_1;
  Tribuf TS_LINE_2;
  Tribuf TS_LINE_3;
};

//-----------------------------------------------------------------------------

struct SpriteStoreRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_STORE -----\n");
    text_painter.dprintf("SPRITE_COUNT %d\n", pack(SPRITE_COUNT0, SPRITE_COUNT1, SPRITE_COUNT2, SPRITE_COUNT3));

    text_painter.dprintf("STORE0 %d %d %d %d\n",
                 SPRITE0_GET_SYNC.a.val,
                 pack(STORE0_X0,    STORE0_X1,    STORE0_X2,    STORE0_X3,   STORE0_X4,   STORE0_X5, STORE0_X6, STORE0_X7),
                 pack(STORE0_IDX0,  STORE0_IDX1,  STORE0_IDX2,  STORE0_IDX3, STORE0_IDX4, STORE0_IDX5),
                 pack(STORE0_LINE0, STORE0_LINE1, STORE0_LINE2, STORE0_LINE3));

    text_painter.dprintf("STORE1 %d %d %d %d\n",
                 SPRITE1_GET_SYNCn.a.val,
                 pack(STORE1_X0,    STORE1_X1,    STORE1_X2,    STORE1_X3,   STORE1_X4,   STORE1_X5, STORE1_X6, STORE1_X7),
                 pack(STORE1_IDX0,  STORE1_IDX1,  STORE1_IDX2,  STORE1_IDX3, STORE1_IDX4, STORE1_IDX5),
                 pack(STORE1_LINE0, STORE1_LINE1, STORE1_LINE2, STORE1_LINE3));

    text_painter.dprintf("STORE2 %d %d %d %d\n",
                 SPRITE2_GET_SYNCn.a.val,
                 pack(STORE2_X0,    STORE2_X1,    STORE2_X2,    STORE2_X3,   STORE2_X4,   STORE2_X5, STORE2_X6, STORE2_X7),
                 pack(STORE2_IDX0,  STORE2_IDX1,  STORE2_IDX2,  STORE2_IDX3, STORE2_IDX4, STORE2_IDX5),
                 pack(STORE2_LINE0, STORE2_LINE1, STORE2_LINE2, STORE2_LINE3));
    text_painter.dprintf("...\n");
    text_painter.newline();
  }

  /*p29.DEZY*/ Reg3 STORE_EN_SYNC;
  /*p29.BESE*/ Reg3 SPRITE_COUNT0;
  /*p29.CUXY*/ Reg3 SPRITE_COUNT1;
  /*p29.BEGO*/ Reg3 SPRITE_COUNT2;
  /*p29.DYBE*/ Reg3 SPRITE_COUNT3;

  /*p29.EBOJ*/ Reg3 SPRITE0_GET_SYNC;
  /*p30.YGUS*/ Reg3 STORE0_IDX0;
  /*p30.YSOK*/ Reg3 STORE0_IDX1;
  /*p30.YZEP*/ Reg3 STORE0_IDX2;
  /*p30.WYTE*/ Reg3 STORE0_IDX3;
  /*p30.ZONY*/ Reg3 STORE0_IDX4;
  /*p30.YWAK*/ Reg3 STORE0_IDX5;
  /*p30.FYHY*/ Reg3 STORE0_LINE0;
  /*p30.GYHO*/ Reg3 STORE0_LINE1;
  /*p30.BOZU*/ Reg3 STORE0_LINE2;
  /*p30.CUFO*/ Reg3 STORE0_LINE3;
  /*p31.XEPE*/ Reg3 STORE0_X0;
  /*p31.YLAH*/ Reg3 STORE0_X1;
  /*p31.ZOLA*/ Reg3 STORE0_X2;
  /*p31.ZULU*/ Reg3 STORE0_X3;
  /*p31.WELO*/ Reg3 STORE0_X4;
  /*p31.XUNY*/ Reg3 STORE0_X5;
  /*p31.WOTE*/ Reg3 STORE0_X6;
  /*p31.XAKO*/ Reg3 STORE0_X7;

  /*p29.CEDY*/ Reg3 SPRITE1_GET_SYNCn;
  /*p30.CAJY*/ Reg3 STORE1_IDX0;
  /*p30.CUZA*/ Reg3 STORE1_IDX1;
  /*p30.COMA*/ Reg3 STORE1_IDX2;
  /*p30.CUFA*/ Reg3 STORE1_IDX3;
  /*p30.CEBO*/ Reg3 STORE1_IDX4;
  /*p30.CADU*/ Reg3 STORE1_IDX5;
  /*p30.ABUG*/ Reg3 STORE1_LINE0;
  /*p30.AMES*/ Reg3 STORE1_LINE1;
  /*p30.ABOP*/ Reg3 STORE1_LINE2;
  /*p30.AROF*/ Reg3 STORE1_LINE3;
  /*p31.DANY*/ Reg3 STORE1_X0;
  /*p31.DUKO*/ Reg3 STORE1_X1;
  /*p31.DESU*/ Reg3 STORE1_X2;
  /*p31.DAZO*/ Reg3 STORE1_X3;
  /*p31.DAKE*/ Reg3 STORE1_X4;
  /*p31.CESO*/ Reg3 STORE1_X5;
  /*p31.DYFU*/ Reg3 STORE1_X6;
  /*p31.CUSY*/ Reg3 STORE1_X7;

  /*p29.EGAV*/ Reg3 SPRITE2_GET_SYNCn;
  /*p30.BOXA*/ Reg3 STORE2_IDX0;
  /*p30.BUNA*/ Reg3 STORE2_IDX1;
  /*p30.BULU*/ Reg3 STORE2_IDX2;
  /*p30.BECA*/ Reg3 STORE2_IDX3;
  /*p30.BYHU*/ Reg3 STORE2_IDX4;
  /*p30.BUHE*/ Reg3 STORE2_IDX5;
  /*p30.YKUK*/ Reg3 STORE2_LINE0;
  /*p30.YLOV*/ Reg3 STORE2_LINE1;
  /*p30.XAZY*/ Reg3 STORE2_LINE2;
  /*p30.XOSY*/ Reg3 STORE2_LINE3;
  /*p31.FOKA*/ Reg3 STORE2_X0;
  /*p31.FYTY*/ Reg3 STORE2_X1;
  /*p31.FUBY*/ Reg3 STORE2_X2;
  /*p31.GOXU*/ Reg3 STORE2_X3;
  /*p31.DUHY*/ Reg3 STORE2_X4;
  /*p31.EJUF*/ Reg3 STORE2_X5;
  /*p31.ENOR*/ Reg3 STORE2_X6;
  /*p31.DEPY*/ Reg3 STORE2_X7;

  /*p29.GOTA*/ Reg3 SPRITE3_GET_SYNCn;
  /*p30.DAFU*/ Reg3 DAFU;
  /*p30.DEBA*/ Reg3 DEBA;
  /*p30.DUHA*/ Reg3 DUHA;
  /*p30.DUNY*/ Reg3 DUNY;
  /*p30.DESE*/ Reg3 DESE;
  /*p30.DEVY*/ Reg3 DEVY;
  /*p30.ZURY*/ Reg3 ZURY;
  /*p30.ZURO*/ Reg3 ZURO;
  /*p30.ZENE*/ Reg3 ZENE;
  /*p30.ZYLU*/ Reg3 ZYLU;
  /*p31.XOLY*/ Reg3 XOLY;
  /*p31.XYBA*/ Reg3 XYBA;
  /*p31.XABE*/ Reg3 XABE;
  /*p31.XEKA*/ Reg3 XEKA;
  /*p31.XOMY*/ Reg3 XOMY;
  /*p31.WUHA*/ Reg3 WUHA;
  /*p31.WYNA*/ Reg3 WYNA;
  /*p31.WECO*/ Reg3 WECO;

  /*p29.XUDY*/ Reg3 SPRITE4_GET_SYNC;
  /*p31.WEDU*/ Reg3 WEDU;
  /*p31.YGAJ*/ Reg3 YGAJ;
  /*p31.ZYJO*/ Reg3 ZYJO;
  /*p31.XURY*/ Reg3 XURY;
  /*p31.YBED*/ Reg3 YBED;
  /*p31.ZALA*/ Reg3 ZALA;
  /*p31.WYDE*/ Reg3 WYDE;
  /*p31.XEPA*/ Reg3 XEPA;
  /*p30.XYNU*/ Reg3 XYNU;
  /*p30.XEGE*/ Reg3 XEGE;
  /*p30.XABO*/ Reg3 XABO;
  /*p30.WANU*/ Reg3 WANU;
  /*p30.XEFE*/ Reg3 XEFE;
  /*p30.XAVE*/ Reg3 XAVE;
  /*p30.CUMU*/ Reg3 CUMU;
  /*p30.CAPO*/ Reg3 CAPO;
  /*p30.CONO*/ Reg3 CONO;
  /*p30.CAJU*/ Reg3 CAJU;

  // store 5
  /*p29.WAFY*/ Reg3 SPRITE5_GET_SYNC;
  /*p31.FUSA*/ Reg3 FUSA;
  /*p31.FAXA*/ Reg3 FAXA;
  /*p31.FOZY*/ Reg3 FOZY;
  /*p31.FESY*/ Reg3 FESY;
  /*p31.CYWE*/ Reg3 CYWE;
  /*p31.DYBY*/ Reg3 DYBY;
  /*p31.DURY*/ Reg3 DURY;
  /*p31.CUVY*/ Reg3 CUVY;
  /*p30.EKAP*/ Reg3 EKAP;
  /*p30.ETAV*/ Reg3 ETAV;
  /*p30.EBEX*/ Reg3 EBEX;
  /*p30.GORU*/ Reg3 GORU;
  /*p30.ETYM*/ Reg3 ETYM;
  /*p30.EKOP*/ Reg3 EKOP;
  /*p30.ANED*/ Reg3 ANED;
  /*p30.ACEP*/ Reg3 ACEP;
  /*p30.ABUX*/ Reg3 ABUX;
  /*p30.ABEG*/ Reg3 ABEG;

  // sprite store 6
  /*p29.WOMY*/ Reg3 SPRITE6_GET_SYNCn;
  /*p31.YCOL*/ Reg3 YCOL;
  /*p31.YRAC*/ Reg3 YRAC;
  /*p31.YMEM*/ Reg3 YMEM;
  /*p31.YVAG*/ Reg3 YVAG;
  /*p31.ZOLY*/ Reg3 ZOLY;
  /*p31.ZOGO*/ Reg3 ZOGO;
  /*p31.ZECU*/ Reg3 ZECU;
  /*p31.ZESA*/ Reg3 ZESA;
  /*p30.GECU*/ Reg3 GECU;
  /*p30.FOXY*/ Reg3 FOXY;
  /*p30.GOHU*/ Reg3 GOHU;
  /*p30.FOGO*/ Reg3 FOGO;
  /*p30.GACY*/ Reg3 GACY;
  /*p30.GABO*/ Reg3 GABO;
  /*p30.ZUBE*/ Reg3 ZUBE;
  /*p30.ZUMY*/ Reg3 ZUMY;
  /*p30.ZEXO*/ Reg3 ZEXO;
  /*p30.ZAFU*/ Reg3 ZAFU;

  // sprite store 7
  /*p29.WAPO*/ Reg3 SPRITE7_GET_SYNC;
  /*p31.ERAZ*/ Reg3 STORE7_X0;
  /*p31.EPUM*/ Reg3 STORE7_X1;
  /*p31.EROL*/ Reg3 STORE7_X2;
  /*p31.EHYN*/ Reg3 STORE7_X3;
  /*p31.FAZU*/ Reg3 STORE7_X4;
  /*p31.FAXE*/ Reg3 STORE7_X5;
  /*p31.EXUK*/ Reg3 STORE7_X6;
  /*p31.FEDE*/ Reg3 STORE7_X7;
  /*p30.FUZO*/ Reg3 FUZO;
  /*p30.GESY*/ Reg3 GESY;
  /*p30.FYSU*/ Reg3 FYSU;
  /*p30.FEFA*/ Reg3 FEFA;
  /*p30.GYNO*/ Reg3 GYNO;
  /*p30.GULE*/ Reg3 GULE;
  /*p30.XYGO*/ Reg3 XYGO;
  /*p30.XYNA*/ Reg3 XYNA;
  /*p30.XAKU*/ Reg3 XAKU;
  /*p30.YGUM*/ Reg3 YGUM;

  // sprite store 8
  /*p29.EXUQ*/ Reg3 SPRITE8_GET_SYNCn;
  /*p31.GAVY*/ Reg3 GAVY;
  /*p31.GYPU*/ Reg3 GYPU;
  /*p31.GADY*/ Reg3 GADY;
  /*p31.GAZA*/ Reg3 GAZA;
  /*p31.EZUF*/ Reg3 EZUF;
  /*p31.ENAD*/ Reg3 ENAD;
  /*p31.EBOW*/ Reg3 EBOW;
  /*p31.FYCA*/ Reg3 FYCA;
  /*p30.AXUV*/ Reg3 AXUV;
  /*p30.BADA*/ Reg3 BADA;
  /*p30.APEV*/ Reg3 APEV;
  /*p30.BADO*/ Reg3 BADO;
  /*p30.BEXY*/ Reg3 BEXY;
  /*p30.BYHE*/ Reg3 BYHE;
  /*p30.AFYM*/ Reg3 AFYM;
  /*p30.AZAP*/ Reg3 AZAP;
  /*p30.AFUT*/ Reg3 AFUT;
  /*p30.AFYX*/ Reg3 AFYX;

  // sprite store 9
  /*p29.FONO*/ Reg3 SPRITE9_GET_SYNC;
  /*p31.XUVY*/ Reg3 STORE9_X0;
  /*p31.XERE*/ Reg3 STORE9_X1;
  /*p31.XUZO*/ Reg3 STORE9_X2;
  /*p31.XEXA*/ Reg3 STORE9_X3;
  /*p31.YPOD*/ Reg3 STORE9_X4;
  /*p31.YROP*/ Reg3 STORE9_X5;
  /*p31.YNEP*/ Reg3 STORE9_X6;
  /*p31.YZOF*/ Reg3 STORE9_X7;
  /*p30.XUFO*/ Reg3 STORE9_IDX0;
  /*p30.XUTE*/ Reg3 STORE9_IDX1;
  /*p30.XOTU*/ Reg3 STORE9_IDX2;
  /*p30.XYFE*/ Reg3 STORE9_IDX3;
  /*p30.YZOR*/ Reg3 STORE9_IDX4;
  /*p30.YBER*/ Reg3 STORE9_IDX5;
  /*p30.DEWU*/ Reg3 STORE9_LINE0;
  /*p30.CANA*/ Reg3 STORE9_LINE1;
  /*p30.DYSY*/ Reg3 STORE9_LINE2;
  /*p30.FOFO*/ Reg3 STORE9_LINE3;
};

//-----------------------------------------------------------------------------

struct TimerRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- TIM_REG -----\n");
    text_painter.dprintf("DIV       %d\n", get_div());
    text_painter.dprintf("TIMA      %d\n", get_tima());
    text_painter.dprintf("TMA       %d\n", get_tma());
    text_painter.newline();

    DIV_00   .dump(text_painter, "DIV_00    ");
    DIV_01   .dump(text_painter, "DIV_01    ");
    DIV_02   .dump(text_painter, "DIV_02    ");
    DIV_03   .dump(text_painter, "DIV_03    ");
    DIV_04   .dump(text_painter, "DIV_04    ");
    DIV_05   .dump(text_painter, "DIV_05    ");
    DIV_06   .dump(text_painter, "DIV_06    ");
    DIV_07   .dump(text_painter, "DIV_07    ");
    DIV_08   .dump(text_painter, "DIV_08    ");
    DIV_09   .dump(text_painter, "DIV_09    ");
    DIV_10   .dump(text_painter, "DIV_10    ");
    DIV_11   .dump(text_painter, "DIV_11    ");
    DIV_12   .dump(text_painter, "DIV_12    ");
    DIV_13   .dump(text_painter, "DIV_13    ");
    DIV_14   .dump(text_painter, "DIV_14    ");
    DIV_15   .dump(text_painter, "DIV_15    ");
    text_painter.newline();

    TIMA_0   .dump(text_painter, "TIMA_0    ");
    TIMA_1   .dump(text_painter, "TIMA_1    ");
    TIMA_2   .dump(text_painter, "TIMA_2    ");
    TIMA_3   .dump(text_painter, "TIMA_3    ");
    TIMA_4   .dump(text_painter, "TIMA_4    ");
    TIMA_5   .dump(text_painter, "TIMA_5    ");
    TIMA_6   .dump(text_painter, "TIMA_6    ");
    TIMA_7   .dump(text_painter, "TIMA_7    ");
    text_painter.newline();

    TMA_0    .dump(text_painter, "TMA_0     ");
    TMA_1    .dump(text_painter, "TMA_1     ");
    TMA_2    .dump(text_painter, "TMA_2     ");
    TMA_3    .dump(text_painter, "TMA_3     ");
    TMA_4    .dump(text_painter, "TMA_4     ");
    TMA_5    .dump(text_painter, "TMA_5     ");
    TMA_6    .dump(text_painter, "TMA_6     ");
    TMA_7    .dump(text_painter, "TMA_7     ");
    text_painter.newline();

    TAC_0    .dump(text_painter, "TAC_0     ");
    TAC_1    .dump(text_painter, "TAC_1     ");
    TAC_2    .dump(text_painter, "TAC_2     ");
    text_painter.newline();

    TIMA_MAX .dump(text_painter, "TIMA_MAX  ");
    INT_TIMER.dump(text_painter, "INT_TIMER ");
    text_painter.newline();
  }

  int get_div() const {
    return pack(DIV_00, DIV_01, DIV_02, DIV_03, DIV_04, DIV_05, DIV_06, DIV_07,
                DIV_08, DIV_09, DIV_10, DIV_11, DIV_12, DIV_13, DIV_14, DIV_15);
  }

  /*
  void set_div(uint16_t x) {
    DIV_00.set2(x & 0x0001); DIV_01.set2(x & 0x0002); DIV_02.set2(x & 0x0004); DIV_03.set2(x & 0x0008);
    DIV_04.set2(x & 0x0010); DIV_05.set2(x & 0x0020); DIV_06.set2(x & 0x0040); DIV_07.set2(x & 0x0080);
    DIV_08.set2(x & 0x0100); DIV_09.set2(x & 0x0200); DIV_10.set2(x & 0x0400); DIV_11.set2(x & 0x0800);
    DIV_12.set2(x & 0x1000); DIV_13.set2(x & 0x2000); DIV_14.set2(x & 0x4000); DIV_15.set2(x & 0x8000);
  }
  */

  int get_tima() const {
    return pack(TIMA_0.v(), TIMA_1.v(), TIMA_2.v(), TIMA_3.v(), TIMA_4.v(), TIMA_5.v(), TIMA_6.v(), TIMA_7.v());
  }

  /*
  void set_tima(uint8_t x) {
    TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
    TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
  }
  */

  wire get_tima_max()  { return TIMA_MAX; }
  wire get_int_timer() { return INT_TIMER; }

  int get_tma() const {
    return pack(TMA_0, TMA_1, TMA_2, TMA_3, TMA_4, TMA_5, TMA_6, TMA_7);
  }

  /*
  void set_tma(uint8_t x) {
    TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
    TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
  }
  */

  int get_tac() const {
    return pack(TAC_0, TAC_1, TAC_2);
  }

  /*
  void set_tac(uint8_t x) {
    TAC_0.set2(x & 1);
    TAC_1.set2(x & 2);
    TAC_2.set2(x & 4);
  }
  */

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg3 DIV_00;
  /*p01.UFOR*/ Reg3 DIV_01;
  /*p01.UNER*/ Reg3 DIV_02;
  /*p01.TERO*/ Reg3 DIV_03;
  /*p01.UNYK*/ Reg3 DIV_04;
  /*p01.TAMA*/ Reg3 DIV_05;
  /*p01.UGOT*/ Reg3 DIV_06;
  /*p01.TULU*/ Reg3 DIV_07;
  /*p01.TUGO*/ Reg3 DIV_08;
  /*p01.TOFE*/ Reg3 DIV_09;
  /*p01.TERU*/ Reg3 DIV_10;
  /*p01.SOLA*/ Reg3 DIV_11;
  /*p01.SUBU*/ Reg3 DIV_12;
  /*p01.TEKA*/ Reg3 DIV_13;
  /*p01.UKET*/ Reg3 DIV_14;
  /*p01.UPOF*/ Reg3 DIV_15;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Counter TIMA_0;
  /*p03.POVY*/ Counter TIMA_1;
  /*p03.PERU*/ Counter TIMA_2;
  /*p03.RATE*/ Counter TIMA_3;
  /*p03.RUBY*/ Counter TIMA_4;
  /*p03.RAGE*/ Counter TIMA_5;
  /*p03.PEDA*/ Counter TIMA_6;
  /*p03.NUGA*/ Counter TIMA_7;

  /*p03.NYDU*/ Reg3 TIMA_MAX;
  /*p03.MOBA*/ Reg3 INT_TIMER;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg3 TMA_0;
  /*p03.NYKE*/ Reg3 TMA_1;
  /*p03.MURU*/ Reg3 TMA_2;
  /*p03.TYVA*/ Reg3 TMA_3;
  /*p03.TYRU*/ Reg3 TMA_4;
  /*p03.SUFY*/ Reg3 TMA_5;
  /*p03.PETO*/ Reg3 TMA_6;
  /*p03.SETA*/ Reg3 TMA_7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg3 TAC_0;
  /*p03.SAMY*/ Reg3 TAC_1;
  /*p03.SABO*/ Reg3 TAC_2;
};

//-----------------------------------------------------------------------------

struct VidRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- VID_REG -----\n");

    text_painter.dprintf("PIX X    %d\n", pack(X0, X1, X2, X3, X4, X5, X6, X7));
    text_painter.dprintf("MAP X    %d\n", pack(MAP_X0, MAP_X1, MAP_X2, MAP_X3, MAP_X4));
    text_painter.dprintf("MAP Y    %d\n", pack(MAP_Y0, MAP_Y1, MAP_Y2, MAP_Y3, MAP_Y4));
    text_painter.dprintf("TILE Y   %d\n", pack(TILE_Y0, TILE_Y1, TILE_Y2));
    text_painter.dprintf("FINE_CNT %d\n", pack(FINE_CNT0, FINE_CNT1, FINE_CNT2));

    text_painter.dprintf("MAxx     0x%04x\n", pack(MA00, MA01, MA02, MA03, MA04, MA05, MA06, MA07, MA08, MA09, MA10, MA11, MA12));
    text_painter.dprintf("MDxx     0x%02x\n", pack(MD0, MD1, MD2, MD3, MD4, MD5, MD6, MD7));

    FINE_MATCH_DUMP         .dump(text_painter, "FINE_MATCH_DUMP          ");
    FINE_MATCH_SYNC1        .dump(text_painter, "FINE_MATCH_SYNC1         ");
    FINE_MATCH_SYNC2        .dump(text_painter, "FINE_MATCH_SYNC2         ");
    NOPA_WIN_MODE_SYNC      .dump(text_painter, "NOPA_WIN_MODE_SYNC       ");
    SOVY_WIN_MODE_SYNC      .dump(text_painter, "SOVY_WIN_MODE_SYNC       ");
    XYMU_RENDERING_LATCH    .dump(text_painter, "XYMU_RENDERING_LATCH     ");
    RENDER_DONE_SYNC        .dump(text_painter, "RENDER_DONE_SYNC         ");
    PYNU_WIN_MODE_LATCH     .dump(text_painter, "PYNU_WIN_MODE_LATCH      ");
    RYDY_WIN_MODE_LATCH     .dump(text_painter, "RYDY_WIN_MODE_LATCH      ");
    INT_HBL_EN              .dump(text_painter, "INT_HBL_EN               ");
    INT_VBL_EN              .dump(text_painter, "INT_VBL_EN               ");
    INT_OAM_EN              .dump(text_painter, "INT_OAM_EN               ");
    INT_LYC_EN              .dump(text_painter, "INT_LYC_EN               ");
    ROPO_LY_MATCH_SYNC      .dump(text_painter, "ROPO_LY_MATCH_SYNC       ");
    RUPO_LATCH_LYC_MATCH    .dump(text_painter, "RUPO_LATCH_LYC_MATCH     ");
    WY_MATCH_SYNC           .dump(text_painter, "WY_MATCH_SYNC            ");
    WIN_MATCH_ONSCREEN_SYNC1.dump(text_painter, "WIN_MATCH_ONSCREEN_SYNC1 ");
    WIN_MATCH_ONSCREEN_SYNC2.dump(text_painter, "WIN_MATCH_ONSCREEN_SYNC2 ");
    WIN_MATCH_SYNC1         .dump(text_painter, "WIN_MATCH_SYNC1          ");
    WIN_MATCH_SYNC2         .dump(text_painter, "WIN_MATCH_SYNC2          ");
    BG_READ_VRAM_LATCHn     .dump(text_painter, "BG_READ_VRAM_LATCHn      ");
    BG_SEQ_x1x3x5x7         .dump(text_painter, "BG_SEQ_x1x3x5x7          ");
    BG_SEQ_xx23xx67         .dump(text_painter, "BG_SEQ_xx23xx67          ");
    BG_SEQ_xxxx4567         .dump(text_painter, "BG_SEQ_xxxx4567          ");
    BG_SEQ5_SYNC            .dump(text_painter, "BG_SEQ5_SYNC             ");
    BG_SEQ_6                .dump(text_painter, "BG_SEQ_6                 ");
    BG_SEQ_7                .dump(text_painter, "BG_SEQ_7                 ");
    BG_SEQ_x1x3x5x7_DELAY   .dump(text_painter, "BG_SEQ_x1x3x5x7_DELAY    ");
    PYGO_TILE_DONE          .dump(text_painter, "PYGO_TILE_DONE           ");
    POKY_FRONT_PORCH_LATCHn .dump(text_painter, "POKY_FRONT_PORCH_LATCHn  ");
    WY_MATCH_LATCH          .dump(text_painter, "WY_MATCH_LATCH           ");
    _SPRITE_FETCH_LATCH     .dump(text_painter, "_SPRITE_FETCH_LATCH      ");
    _SPRITE_FETCH_SYNC1     .dump(text_painter, "_SPRITE_FETCH_SYNC1      ");
    _SPRITE_FETCH_SYNC2     .dump(text_painter, "_SPRITE_FETCH_SYNC2      ");
    text_painter.newline();
  }

  /*p21.XEHO*/ Reg3 X0;
  /*p21.SAVY*/ Reg3 X1;
  /*p21.XODU*/ Reg3 X2;
  /*p21.XYDO*/ Reg3 X3;
  /*p21.TUHU*/ Reg3 X4;
  /*p21.TUKY*/ Reg3 X5;
  /*p21.TAKO*/ Reg3 X6;
  /*p21.SYBE*/ Reg3 X7;

  /*p27.WYKA*/ Reg3 MAP_X0;
  /*p27.WODY*/ Reg3 MAP_X1;
  /*p27.WOBO*/ Reg3 MAP_X2;
  /*p27.WYKO*/ Reg3 MAP_X3;
  /*p27.XOLO*/ Reg3 MAP_X4;

  /*p27.TUFU*/ Reg3 MAP_Y0;
  /*p27.TAXA*/ Reg3 MAP_Y1;
  /*p27.TOZO*/ Reg3 MAP_Y2;
  /*p27.TATE*/ Reg3 MAP_Y3;
  /*p27.TEKE*/ Reg3 MAP_Y4;

  /*p27.VYNO*/ Reg3 TILE_Y0;
  /*p27.VUJO*/ Reg3 TILE_Y1;
  /*p27.VYMU*/ Reg3 TILE_Y2;

  /*p27.RYKU*/ Reg3 FINE_CNT0;
  /*p27.ROGA*/ Reg3 FINE_CNT1;
  /*p27.RUBU*/ Reg3 FINE_CNT2;

  Tribuf MA00;
  Tribuf MA01;
  Tribuf MA02;
  Tribuf MA03;
  Tribuf MA04;
  Tribuf MA05;
  Tribuf MA06;
  Tribuf MA07;
  Tribuf MA08;
  Tribuf MA09;
  Tribuf MA10;
  Tribuf MA11;
  Tribuf MA12;

  Tribuf MD0;
  Tribuf MD1;
  Tribuf MD2;
  Tribuf MD3;
  Tribuf MD4;
  Tribuf MD5;
  Tribuf MD6;
  Tribuf MD7;

  /*p??.ROXY*/ Latch3 FINE_MATCH_DUMP;
  /*p??.PUXA*/ Reg3 FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg3 FINE_MATCH_SYNC2;

  /*p27.NOPA*/ Reg3 NOPA_WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg3 SOVY_WIN_MODE_SYNC;

  /*p21.XYMU*/ Latch3 XYMU_RENDERING_LATCH;

  /*p21.VOGA*/ Reg3 RENDER_DONE_SYNC;

  /*p27.PYNU*/ Latch3 PYNU_WIN_MODE_LATCH;
  /*p27.RYDY*/ Latch3 RYDY_WIN_MODE_LATCH;

  /*p21.ROXE*/ Reg3 INT_HBL_EN;
  /*p21.RUFO*/ Reg3 INT_VBL_EN;
  /*p21.REFE*/ Reg3 INT_OAM_EN;
  /*p21.RUGU*/ Reg3 INT_LYC_EN;

  /*p21.ROPO*/ Reg3 ROPO_LY_MATCH_SYNC;
  /*p21.RUPO*/ Latch3 RUPO_LATCH_LYC_MATCH;

  /*p27.SARY*/ Reg3 WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg3 WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg3 WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg3 WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg3 WIN_MATCH_SYNC2;

  /*p27.LONY*/ Latch3 BG_READ_VRAM_LATCHn;

  /*p27.LAXU*/ Reg3 BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ Reg3 BG_SEQ_xx23xx67;
  /*p27.NYVA*/ Reg3 BG_SEQ_xxxx4567;

  /*p27.LOVY*/ Reg3 BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg3 BG_SEQ_6;
  /*p24.PORY*/ Reg3 BG_SEQ_7;
  /*p27.LYZU*/ Reg3 BG_SEQ_x1x3x5x7_DELAY;

  /*p24.PYGO*/ Reg3 PYGO_TILE_DONE;
  /*p24.POKY*/ Latch3 POKY_FRONT_PORCH_LATCHn;

  /*p27.REJO*/ Latch3 WY_MATCH_LATCH;

  /*p27.TAKA*/ Latch3 _SPRITE_FETCH_LATCH;
  /*p27.SOBU*/ Reg3 _SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg3 _SPRITE_FETCH_SYNC2;



};

//-----------------------------------------------------------------------------
// Video clocks

struct VclkRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- VCLK_REG -----\n");
    WUVU_AxxDExxH.dump(text_painter, "WUVU_AxxDExxH ");
    VENA_xBCDExxx.dump(text_painter, "VENA_xBCDExxx ");
    WOSU_xxCDxxGH.dump(text_painter, "WOSU_xxCDxxGH ");
    text_painter.newline();
  }

  /*p29.WUVU*/ Reg3 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg3 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg3 WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

struct OamRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_REG -----\n");
    text_painter.dprintf("LATCH_A 0x%02x\n", pack(LATCH_OAM_A0,LATCH_OAM_A1,LATCH_OAM_A2,LATCH_OAM_A3,LATCH_OAM_A4,LATCH_OAM_A5,LATCH_OAM_A6,LATCH_OAM_A7));
    text_painter.dprintf("LATCH_B 0x%02x\n", pack(LATCH_OAM_B0,LATCH_OAM_B1,LATCH_OAM_B2,LATCH_OAM_B3,LATCH_OAM_B4,LATCH_OAM_B5,LATCH_OAM_B6,LATCH_OAM_B7));
    text_painter.dprintf("REG_A   0x%02x\n", pack(REG_OAM_A0,REG_OAM_A1,REG_OAM_A2,REG_OAM_A3,REG_OAM_A4,REG_OAM_A5,REG_OAM_A6,REG_OAM_A7));
    text_painter.dprintf("REG_B   0x%02x\n", pack(REG_OAM_B0,REG_OAM_B1,REG_OAM_B2,REG_OAM_B3,REG_OAM_B4,REG_OAM_B5,REG_OAM_B6,REG_OAM_B7));
    text_painter.newline();
  }

  /*p31.XYKY*/ Latch3 LATCH_OAM_A0;
  /*p31.YRUM*/ Latch3 LATCH_OAM_A1;
  /*p31.YSEX*/ Latch3 LATCH_OAM_A2;
  /*p31.YVEL*/ Latch3 LATCH_OAM_A3;
  /*p31.WYNO*/ Latch3 LATCH_OAM_A4;
  /*p31.CYRA*/ Latch3 LATCH_OAM_A5;
  /*p31.ZUVE*/ Latch3 LATCH_OAM_A6;
  /*p31.ECED*/ Latch3 LATCH_OAM_A7;

  /*p29.YDYV*/ Latch3 LATCH_OAM_B0;
  /*p29.YCEB*/ Latch3 LATCH_OAM_B1;
  /*p29.ZUCA*/ Latch3 LATCH_OAM_B2;
  /*p29.WONE*/ Latch3 LATCH_OAM_B3;
  /*p29.ZAXE*/ Latch3 LATCH_OAM_B4;
  /*p29.XAFU*/ Latch3 LATCH_OAM_B5;
  /*p29.YSES*/ Latch3 LATCH_OAM_B6;
  /*p29.ZECA*/ Latch3 LATCH_OAM_B7;

  /*p31.YLOR*/ Reg3 REG_OAM_A0;
  /*p31.ZYTY*/ Reg3 REG_OAM_A1;
  /*p31.ZYVE*/ Reg3 REG_OAM_A2;
  /*p31.ZEZY*/ Reg3 REG_OAM_A3;
  /*p31.GOMO*/ Reg3 REG_OAM_A4;
  /*p31.BAXO*/ Reg3 REG_OAM_A5;
  /*p31.YZOS*/ Reg3 REG_OAM_A6;
  /*p31.DEPO*/ Reg3 REG_OAM_A7;

  /*p29.XUSO*/ Reg3 REG_OAM_B0;
  /*p29.XEGU*/ Reg3 REG_OAM_B1;
  /*p29.YJEX*/ Reg3 REG_OAM_B2;
  /*p29.XYJU*/ Reg3 REG_OAM_B3;
  /*p29.YBOG*/ Reg3 REG_OAM_B4;
  /*p29.WYSO*/ Reg3 REG_OAM_B5;
  /*p29.XOTE*/ Reg3 REG_OAM_B6;
  /*p29.YZAB*/ Reg3 REG_OAM_B7;
};

//-----------------------------------------------------------------------------

};