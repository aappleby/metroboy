#pragma once
#include "Sch_Common.h"

#include "../src/TextPainter.h"

namespace Schematics {

struct TestGB;

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

struct BusRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- BUS_REG -----\n");
    
    dump_long(text_painter, "BOOT_BIT ", BOOT_BIT.a);
    dump_long(text_painter, "SOTO_DBG ", SOTO_DBG.a);

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

  /*p07.TEPU*/ Reg3 BOOT_BIT; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
  /*p25.SOTO*/ Reg3 SOTO_DBG; // 17-rung

  /*p08.ALOR*/ Latch3 CPU_ADDR_LATCH_00;
  /*p08.APUR*/ Latch3 CPU_ADDR_LATCH_01;
  /*p08.ALYR*/ Latch3 CPU_ADDR_LATCH_02;
  /*p08.ARET*/ Latch3 CPU_ADDR_LATCH_03;
  /*p08.AVYS*/ Latch3 CPU_ADDR_LATCH_04;
  /*p08.ATEV*/ Latch3 CPU_ADDR_LATCH_05;
  /*p08.AROS*/ Latch3 CPU_ADDR_LATCH_06;
  /*p08.ARYM*/ Latch3 CPU_ADDR_LATCH_07;
  /*p08.LUNO*/ Latch3 CPU_ADDR_LATCH_08;
  /*p08.LYSA*/ Latch3 CPU_ADDR_LATCH_09;
  /*p08.PATE*/ Latch3 CPU_ADDR_LATCH_10;
  /*p08.LUMY*/ Latch3 CPU_ADDR_LATCH_11;
  /*p08.LOBU*/ Latch3 CPU_ADDR_LATCH_12;
  /*p08.LONU*/ Latch3 CPU_ADDR_LATCH_13;
  /*p08.NYRE*/ Latch3 CPU_ADDR_LATCH_14;

  /*p08.SOMA*/ Latch3 CART_DATA_LATCH_00;
  /*p08.RONY*/ Latch3 CART_DATA_LATCH_01;
  /*p08.RAXY*/ Latch3 CART_DATA_LATCH_02;
  /*p08.SELO*/ Latch3 CART_DATA_LATCH_03;
  /*p08.SODY*/ Latch3 CART_DATA_LATCH_04;
  /*p08.SAGO*/ Latch3 CART_DATA_LATCH_05;
  /*p08.RUPA*/ Latch3 CART_DATA_LATCH_06;
  /*p08.SAZY*/ Latch3 CART_DATA_LATCH_07;
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
  /*p23.VYXE*/ Reg3 LCDC_BGEN;
  /*p23.XYLO*/ Reg3 LCDC_SPEN;
  /*p23.XYMO*/ Reg3 LCDC_SPSIZE;
  /*p23.XAFO*/ Reg3 LCDC_BGMAP;
  /*p23.WEXU*/ Reg3 LCDC_BGTILE;
  /*p23.WYMO*/ Reg3 LCDC_WINEN;
  /*p23.WOKY*/ Reg3 LCDC_WINMAP;
  /*p23.XONA*/ Reg3 XONA_LCDC_EN;

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
  /*p36.MOSA*/ Reg3 OBP12; // 9-rung, bottom rung is passthrough?
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
    return (X0.q() << 0) | (X1.q() << 1) | (X2.q() << 2) | (X3.q() << 3) | (X4.q() << 4) | (X5.q() << 5) | (X6.q() << 6);
  }

  uint32_t y() const {
    return (Y0.q() << 0) | (Y1.q() << 1) | (Y2.q() << 2) | (Y3.q() << 3) | (Y4.q() << 4) | (Y5.q() << 5) | (Y6.q() << 6) | (Y7.q() << 7);
  }

  void tick(
    bool CUNU_RSTn,
    bool VENA_xBCDExxx,
    bool WUVU_AxxDExxH,
    bool UGOT_DIV_06,
    bool TULU_DIV_07,
    bool XONA_LCDC_EN,
    bool RYPO_LCD_CP,
    bool PATY_PIX_OUT_LO,
    bool PERO_PIX_OUT_HI,
    bool SEPA_FF41_WR);

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- LCD REG -----\n");

    dump(text_painter, "LCD X ", X0, X1, X2, X3, X4, X5, X6);
    dump(text_painter, "LCD Y ", Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7);

    dump_long(text_painter, "RUTU_NEW_LINE_d0   ", RUTU_NEW_LINE_d0.a);
    dump_long(text_painter, "CATU_VID_LINE_d4   ", CATU_VID_LINE_d4.a);
    dump_long(text_painter, "NYPE_NEW_LINE_d4   ", NYPE_NEW_LINE_d4.a);
    dump_long(text_painter, "ANEL_VID_LINE_d6   ", ANEL_VID_LINE_d6.a);
    dump_long(text_painter, "MYTA_LINE_153_d4   ", MYTA_LINE_153_d4.a);
    dump_long(text_painter, "POPU_IN_VBLANK_d4  ", POPU_IN_VBLANK_d4.a);
    dump_long(text_painter, "SYGU_LINE_STROBE   ", SYGU_LINE_STROBE.a);
    dump_long(text_painter, "PAHO_X_8_SYNC ", PAHO_X_8_SYNC.a);
    dump_long(text_painter, "WUSA_CPEN_LATCH    ", WUSA_CPEN_LATCH.a);
    dump_long(text_painter, "POFY_ST_LATCH ", POFY_ST_LATCH.a);
    dump_long(text_painter, "MEDA_VSYNC_OUTn    ", MEDA_VSYNC_OUTn.a);
    dump_long(text_painter, "LUCA_LINE_EVEN     ", LUCA_LINE_EVEN.a);
    dump_long(text_painter, "NAPO_FRAME_EVEN    ", NAPO_FRAME_EVEN.a);
    text_painter.newline();
  }

  // 17-rung regs. Output order QN/Q
  /*p21.SAXO*/ Reg3 X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg3 X1;
  /*p21.VYZO*/ Reg3 X2; 
  /*p21.TELU*/ Reg3 X3;
  /*p21.SUDE*/ Reg3 X4;
  /*p21.TAHA*/ Reg3 X5;
  /*p21.TYRY*/ Reg3 X6;

  // 17-rung regs. Output order QN/Q
  /*p21.MUWY*/ Reg3 Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg3 Y1;
  /*p21.LEXA*/ Reg3 Y2;
  /*p21.LYDO*/ Reg3 Y3;
  /*p21.LOVU*/ Reg3 Y4;
  /*p21.LEMA*/ Reg3 Y5;
  /*p21.MATO*/ Reg3 Y6;
  /*p21.LAFO*/ Reg3 Y7;

  /*p21.RUTU*/ Reg3 RUTU_NEW_LINE_d0; // p909+8
  /*p29.CATU*/ Reg3 CATU_VID_LINE_d4; // p001+8
  /*p21.NYPE*/ Reg3 NYPE_NEW_LINE_d4; // p001+8
  /*p28.ANEL*/ Reg3 ANEL_VID_LINE_d6; // p003+8
                                 
  /*p21.MYTA*/ Reg3 MYTA_LINE_153_d4;  // p153:001 - p000:001
  /*p21.POPU*/ Reg3 POPU_IN_VBLANK_d4; // p144:001 - p000:001

  /*p21.SYGU*/ Reg3 SYGU_LINE_STROBE;
  /*p24.PAHO*/ Reg3 PAHO_X_8_SYNC;

  /*p21.WUSA*/ Latch3 WUSA_CPEN_LATCH;

  /*p24.RUJU*/ Latch3 POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME

  /*p24.MEDA*/ Reg3 MEDA_VSYNC_OUTn;
  /*p24.LUCA*/ Reg3 LUCA_LINE_EVEN;
  /*p21.NAPO*/ Reg3 NAPO_FRAME_EVEN;

  // FF45 - LYC
  /*p23.SYRY*/ Reg3 LYC0;
  /*p23.VUCE*/ Reg3 LYC1;
  /*p23.SEDY*/ Reg3 LYC2;
  /*p23.SALO*/ Reg3 LYC3;
  /*p23.SOTA*/ Reg3 LYC4;
  /*p23.VAFA*/ Reg3 LYC5;
  /*p23.VEVO*/ Reg3 LYC6;
  /*p23.RAHA*/ Reg3 LYC7;

  /*p21.ROPO*/ Reg3 LY_MATCH_SYNC;
  /*p21.RUPO*/ Latch3 LYC_MATCH_LATCHn;

  bool commit() {
    bool changed = false;
    /*p21.SAXO*/ changed |= X0.commit_reg(); // increments at phase 1, reset to 0 at p909.
    /*p21.TYPO*/ changed |= X1.commit_reg();
    /*p21.VYZO*/ changed |= X2.commit_reg();
    /*p21.TELU*/ changed |= X3.commit_reg();
    /*p21.SUDE*/ changed |= X4.commit_reg();
    /*p21.TAHA*/ changed |= X5.commit_reg();
    /*p21.TYRY*/ changed |= X6.commit_reg();
    /*p21.MUWY*/ changed |= Y0.commit_reg(); // increments at p909, reset to 0 at p153:001
    /*p21.MYRO*/ changed |= Y1.commit_reg();
    /*p21.LEXA*/ changed |= Y2.commit_reg();
    /*p21.LYDO*/ changed |= Y3.commit_reg();
    /*p21.LOVU*/ changed |= Y4.commit_reg();
    /*p21.LEMA*/ changed |= Y5.commit_reg();
    /*p21.MATO*/ changed |= Y6.commit_reg();
    /*p21.LAFO*/ changed |= Y7.commit_reg();
    /*p21.RUTU*/ changed |= RUTU_NEW_LINE_d0.commit_reg(); // p909+8
    /*p29.CATU*/ changed |= CATU_VID_LINE_d4.commit_reg();  // p001+8
    /*p21.NYPE*/ changed |= NYPE_NEW_LINE_d4.commit_reg(); // p001+8
    /*p28.ANEL*/ changed |= ANEL_VID_LINE_d6.commit_reg();  // p003+8
    /*p21.MYTA*/ changed |= MYTA_LINE_153_d4.commit_reg();  // p153:001 - p000:001
    /*p21.POPU*/ changed |= POPU_IN_VBLANK_d4.commit_reg();    // p144:001 - p000:001
    /*p21.SYGU*/ changed |= SYGU_LINE_STROBE.commit_reg();
    /*p24.PAHO*/ changed |= PAHO_X_8_SYNC.commit_reg();
    /*p21.WUSA*/ changed |= WUSA_CPEN_LATCH.commit_latch();
    /*p24.RUJU*/ changed |= POFY_ST_LATCH.commit_latch(); // nor latch with p24.RUJU, p24.POME
    /*p24.MEDA*/ changed |= MEDA_VSYNC_OUTn.commit_reg();
    /*p24.LUCA*/ changed |= LUCA_LINE_EVEN.commit_reg();
    /*p21.NAPO*/ changed |= NAPO_FRAME_EVEN.commit_reg();

    /*p23.SYRY*/ changed |= LYC0.commit_reg();
    /*p23.VUCE*/ changed |= LYC1.commit_reg();
    /*p23.SEDY*/ changed |= LYC2.commit_reg();
    /*p23.SALO*/ changed |= LYC3.commit_reg();
    /*p23.SOTA*/ changed |= LYC4.commit_reg();
    /*p23.VAFA*/ changed |= LYC5.commit_reg();
    /*p23.VEVO*/ changed |= LYC6.commit_reg();
    /*p23.RAHA*/ changed |= LYC7.commit_reg();

    /*p21.ROPO*/ changed |= LY_MATCH_SYNC.commit_reg();
    /*p21.RUPO*/ changed |= LYC_MATCH_LATCHn.commit_latch();

    return changed;
  }

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- LCD_PINS -----\n");
    LD1.dump(text_painter, "LD1 ");
    LD0.dump(text_painter, "LD0 ");
    CPG.dump(text_painter, "CPG ");
    CP.dump(text_painter, "CP  ");
    ST.dump(text_painter, "ST  ");
    CPL.dump(text_painter, "CPL ");
    FR.dump(text_painter, "FR  ");
    S.dump(text_painter, "S   ");
    text_painter.newline();
  }

  /* PIN_50 */ PinOut LD1;
  /* PIN_51 */ PinOut LD0;
  /* PIN_52 */ PinOut CPG;
  /* PIN_53 */ PinOut CP;
  /* PIN_54 */ PinOut ST;
  /* PIN_55 */ PinOut CPL;
  /* PIN_56 */ PinOut FR;
  /* PIN_57 */ PinOut S;

  bool commit_pins() {
    bool changed = false;
    /* PIN_50 */ changed |= LD1.commit_pinout();
    /* PIN_51 */ changed |= LD0.commit_pinout();
    /* PIN_52 */ changed |= CPG.commit_pinout();
    /* PIN_53 */ changed |= CP.commit_pinout();
    /* PIN_54 */ changed |= ST.commit_pinout();
    /* PIN_55 */ changed |= CPL.commit_pinout();
    /* PIN_56 */ changed |= FR.commit_pinout();
    /* PIN_57 */ changed |= S.commit_pinout();
    return changed;
  }
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

  uint8_t bg_pix_a()   { return (uint8_t)pack(BG_PIX_A0.q(),   BG_PIX_A1.q(),   BG_PIX_A2.q(),   BG_PIX_A3.q(),   BG_PIX_A4.q(),   BG_PIX_A5.q(),   BG_PIX_A6.q(),   BG_PIX_A7.q());   }
  uint8_t bg_pix_b()   { return (uint8_t)pack(BG_PIX_B0.q(),   BG_PIX_B1.q(),   BG_PIX_B2.q(),   BG_PIX_B3.q(),   BG_PIX_B4.q(),   BG_PIX_B5.q(),   BG_PIX_B6.q(),   BG_PIX_B7.q());   }
  uint8_t spr_pix_a()  { return (uint8_t)pack(SPR_PIX_A0.q(),  SPR_PIX_A1.q(),  SPR_PIX_A2.q(),  SPR_PIX_A3.q(),  SPR_PIX_A4.q(),  SPR_PIX_A5.q(),  SPR_PIX_A6.q(),  SPR_PIX_A7.q());  }
  uint8_t spr_pix_b()  { return (uint8_t)pack(SPR_PIX_B0.q(),  SPR_PIX_B1.q(),  SPR_PIX_B2.q(),  SPR_PIX_B3.q(),  SPR_PIX_B4.q(),  SPR_PIX_B5.q(),  SPR_PIX_B6.q(),  SPR_PIX_B7.q());  }
  uint8_t bg_pipe_a()  { return (uint8_t)pack(BG_PIPE_A0.q(),  BG_PIPE_A1.q(),  BG_PIPE_A2.q(),  BG_PIPE_A3.q(),  BG_PIPE_A4.q(),  BG_PIPE_A5.q(),  BG_PIPE_A6.q(),  BG_PIPE_A7.q());  }
  uint8_t bg_pipe_b()  { return (uint8_t)pack(BG_PIPE_B0.q(),  BG_PIPE_B1.q(),  BG_PIPE_B2.q(),  BG_PIPE_B3.q(),  BG_PIPE_B4.q(),  BG_PIPE_B5.q(),  BG_PIPE_B6.q(),  BG_PIPE_B7.q());  }
  uint8_t spr_pipe_a() { return (uint8_t)pack(SPR_PIPE_A0.q(), SPR_PIPE_A1.q(), SPR_PIPE_A2.q(), SPR_PIPE_A3.q(), SPR_PIPE_A4.q(), SPR_PIPE_A5.q(), SPR_PIPE_A6.q(), SPR_PIPE_A7.q()); }
  uint8_t spr_pipe_b() { return (uint8_t)pack(SPR_PIPE_B0.q(), SPR_PIPE_B1.q(), SPR_PIPE_B2.q(), SPR_PIPE_B3.q(), SPR_PIPE_B4.q(), SPR_PIPE_B5.q(), SPR_PIPE_B6.q(), SPR_PIPE_B7.q()); }
  uint8_t pal_pipe()   { return (uint8_t)pack(PAL_PIPE_0.q(),  PAL_PIPE_1.q(),  PAL_PIPE_2.q(),  PAL_PIPE_3.q(),  PAL_PIPE_4.q(),  PAL_PIPE_5.q(),  PAL_PIPE_6.q(),  PAL_PIPE_7.q());  }
  uint8_t mask_pipe()  { return (uint8_t)pack(MASK_PIPE_0.q(), MASK_PIPE_1.q(), MASK_PIPE_2.q(), MASK_PIPE_3.q(), MASK_PIPE_4.q(), MASK_PIPE_5.q(), MASK_PIPE_6.q(), MASK_PIPE_7.q()); }

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
    TUBO_CLKREQn_LATCH.dump(text_painter, "TUBO_CLKREQn_LATCH ");
    RESET_REGp        .dump(text_painter, "RESET_REGp          ");
    text_painter.newline();
  }

  /*p01.TUBO*/ Latch3 TUBO_CLKREQn_LATCH;
  /*p01.ASOL*/ Latch3 ASOL_RST_LATCHp; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg3   RESET_REGp;
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

  uint8_t ser_cnt()  { return (uint8_t)pack(SER_CNT0.q(),SER_CNT1.q(),SER_CNT2.q(),SER_CNT3.q()); }
  uint8_t ser_data() { return (uint8_t)pack(SER_DATA0.q(),SER_DATA1.q(),SER_DATA2.q(),SER_DATA3.q(),SER_DATA4.q(),SER_DATA5.q(),SER_DATA6.q(),SER_DATA7.q()); }

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

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- SER_PINS -----\n");
    text_painter.dprintf("SCK  %d:%d:%d:%d\n", SCK_A.a.val, SCK_B.a.val, SCK_C.a.val, SCK_D.a.val);
    text_painter.dprintf("SIN  %d:%d:%d:%d\n", SIN_A.a.val, SIN_B.a.val, SIN_C.a.val, SIN_D.a.val);
    text_painter.dprintf("SOUT %d\n", SOUT.a.val);
    text_painter.newline();
  }

  //----------
  // Serial pins

  /* PIN_68 */ PinOut SCK_A;   // <- P06.KEXU
  /* PIN_68 */ PinOut SCK_B;   // <- P06.CULY
  /* PIN_68 */ PinIn  SCK_C;   // -> P06.CAVE
  /* PIN_68 */ PinOut SCK_D;   // <- P06.KUJO

  /* PIN_69 */ PinOut SIN_A;   // nc?
  /* PIN_69 */ PinOut SIN_B;   // nc?
  /* PIN_69 */ PinIn  SIN_C;   // -> P06.CAGE
  /* PIN_69 */ PinOut SIN_D;   // nc?

  /* PIN_70 */ PinOut SOUT;    // <- P05.KENA

  bool commit_pins() {
    bool changed = false;
    /* PIN_68 */ changed |= SCK_A.commit_pinout();   // <- P06.KEXU
    /* PIN_68 */ changed |= SCK_B.commit_pinout();   // <- P06.CULY
    /* PIN_68 */ changed |= SCK_C.clear_preset();   // -> P06.CAVE
    /* PIN_68 */ changed |= SCK_D.commit_pinout();   // <- P06.KUJO
    ///* PIN_69 */ changed |= SIN_A.commit();   // nc?
    ///* PIN_69 */ changed |= SIN_B.commit();   // nc?
    /* PIN_69 */ changed |= SIN_C.clear_preset();   // -> P06.CAGE
    ///* PIN_69 */ changed |= SIN_D.commit();   // nc?
    /* PIN_70 */ changed |= SOUT.commit_pinout();    // <- P05.KENA
    return changed;
  }

};

//-----------------------------------------------------------------------------

struct SpriteRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_REG -----\n");

    TOXE_SFETCH_S0_D0    .dump(text_painter, "TOXE_SFETCH_S0_D0    ");
    TULY_SFETCH_S1    .dump(text_painter, "TULY_SFETCH_S1    ");
    TESE_SFETCH_S2    .dump(text_painter, "TESE_SFETCH_S2    ");
    TOBU_SFETCH_S1_D2  .dump(text_painter, "TOBU_SFETCH_S1_D2  ");
    VONU_SFETCH_S1_D4 .dump(text_painter, "VONU_SFETCH_S1_D4 ");
    SEBA_SFETCH_S1_D5 .dump(text_painter, "SEBA_SFETCH_S1_D5 ");
    TYFO_SFETCH_S0_D1     .dump(text_painter, "TYFO_SFETCH_S0_D1     ");
    //CENO_SCANNINGp.dump(text_painter, "CENO_SCANNINGp");

    //text_painter.dprintf("SCAN    %d\n", scan());
    //SCAN_DONE_d4.dump(text_painter, "SCAN_DONE_d4 ");
    //SCAN_DONE_d5.dump(text_painter, "SCAN_DONE_d5 ");

    //text_painter.dprintf("SPR_IDX %d\n", spr_idx());
    //text_painter.dprintf("TS_IDX  %d\n", ts_idx());
    //text_painter.dprintf("TS_LINE %d\n", ts_line());
    text_painter.newline();
  }

  //int scan()    { return pack(SCAN0.q(), SCAN1.q(), SCAN2.q(), SCAN3.q(), SCAN4.q()); }
  //int spr_idx() { return pack(XADU_SPRITE_IDX0.q(),XEDY_SPRITE_IDX1.q(),ZUZE_SPRITE_IDX2.q(),XOBE_SPRITE_IDX3.q(),YDUF_SPRITE_IDX4.q(),XECU_SPRITE_IDX5.q()); }
  //int ts_idx()  { return pack(WEZA_TS_IDX_5,WUCO_TS_IDX_4,WYDA_TS_IDX_3,ZYSU_TS_IDX_2,WYSE_TS_IDX_1,WUZY_TS_IDX_0); }
  //int ts_line() { return pack(WENU_TS_LINE_0,CUCU_TS_LINE_1,CUCA_TS_LINE_2,CEGA_TS_LINE_3); }

  /*p29.TOXE*/ Reg3 TOXE_SFETCH_S0_D0;
  /*p29.TULY*/ Reg3 TULY_SFETCH_S1;
  /*p29.TESE*/ Reg3 TESE_SFETCH_S2;
  
  /*p29.TOBU*/ Reg3 TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg3 VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg3 SEBA_SFETCH_S1_D5;
  /*p29.TYFO*/ Reg3 TYFO_SFETCH_S0_D1;

};

//-----------------------------------------------------------------------------

struct TimerRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- TIM_REG -----\n");
    text_painter.dprintf("DIV       %d\n", get_div());
    text_painter.dprintf("TIMA      %d\n", get_tima());
    text_painter.dprintf("TMA       %d\n", get_tma());
    text_painter.newline();

    UKUP_DIV_00   .dump(text_painter, "UKUP_DIV_00    ");
    UFOR_DIV_01   .dump(text_painter, "UFOR_DIV_01    ");
    UNER_DIV_02   .dump(text_painter, "UNER_DIV_02    ");
    TERO_DIV_03   .dump(text_painter, "TERO_DIV_03    ");
    UNYK_DIV_04   .dump(text_painter, "UNYK_DIV_04    ");
    TAMA_DIV_05   .dump(text_painter, "TAMA_DIV_05    ");
    UGOT_DIV_06   .dump(text_painter, "UGOT_DIV_06    ");
    TULU_DIV_07   .dump(text_painter, "TULU_DIV_07    ");
    TUGO_DIV_08   .dump(text_painter, "TUGO_DIV_08    ");
    TOFE_DIV_09   .dump(text_painter, "TOFE_DIV_09    ");
    TERU_DIV_10   .dump(text_painter, "TERU_DIV_10    ");
    SOLA_DIV_11   .dump(text_painter, "SOLA_DIV_11    ");
    SUBU_DIV_12   .dump(text_painter, "SUBU_DIV_12    ");
    TEKA_DIV_13   .dump(text_painter, "TEKA_DIV_13    ");
    UKET_DIV_14   .dump(text_painter, "UKET_DIV_14    ");
    UPOF_DIV_15   .dump(text_painter, "UPOF_DIV_15    ");
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
    return pack(UKUP_DIV_00.q(), UFOR_DIV_01.q(), UNER_DIV_02.q(), TERO_DIV_03.q(), UNYK_DIV_04.q(), TAMA_DIV_05.q(), UGOT_DIV_06.q(), TULU_DIV_07.q(),
                TUGO_DIV_08.q(), TOFE_DIV_09.q(), TERU_DIV_10.q(), SOLA_DIV_11.q(), SUBU_DIV_12.q(), TEKA_DIV_13.q(), UKET_DIV_14.q(), UPOF_DIV_15.q());
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
    return pack(TIMA_0.q(), TIMA_1.q(), TIMA_2.q(), TIMA_3.q(), TIMA_4.q(), TIMA_5.q(), TIMA_6.q(), TIMA_7.q());
  }

  /*
  void set_tima(uint8_t x) {
    TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
    TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
  }
  */

  wire get_tima_max()  { return TIMA_MAX.q(); }
  wire get_int_timer() { return INT_TIMER.q(); }

  int get_tma() const {
    return pack(TMA_0.q(), TMA_1.q(), TMA_2.q(), TMA_3.q(), TMA_4.q(), TMA_5.q(), TMA_6.q(), TMA_7.q());
  }

  /*
  void set_tma(uint8_t x) {
    TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
    TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
  }
  */

  int get_tac() const {
    return pack(TAC_0.q(), TAC_1.q(), TAC_2.q());
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

  /*p01.UKUP*/ Reg3 UKUP_DIV_00;
  /*p01.UFOR*/ Reg3 UFOR_DIV_01;
  /*p01.UNER*/ Reg3 UNER_DIV_02;
  /*p01.TERO*/ Reg3 TERO_DIV_03;
  /*p01.UNYK*/ Reg3 UNYK_DIV_04;
  /*p01.TAMA*/ Reg3 TAMA_DIV_05;
  /*p01.UGOT*/ Reg3 UGOT_DIV_06;
  /*p01.TULU*/ Reg3 TULU_DIV_07;
  /*p01.TUGO*/ Reg3 TUGO_DIV_08;
  /*p01.TOFE*/ Reg3 TOFE_DIV_09;
  /*p01.TERU*/ Reg3 TERU_DIV_10;
  /*p01.SOLA*/ Reg3 SOLA_DIV_11;
  /*p01.SUBU*/ Reg3 SUBU_DIV_12;
  /*p01.TEKA*/ Reg3 TEKA_DIV_13;
  /*p01.UKET*/ Reg3 UKET_DIV_14;
  /*p01.UPOF*/ Reg3 UPOF_DIV_15;

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

    text_painter.dprintf("PIX X      %d\n", pack(X0.q(), X1.q(), X2.q(), X3.q(), X4.q(), X5.q(), X6.q(), X7.q()));
    text_painter.dprintf("WIN MAP X  %d\n", pack(WIN_X3.q(), WIN_X4.q(), WIN_X5.q(), WIN_X6.q(), WIN_X7.q()));
    text_painter.dprintf("WIN MAP Y  %d\n", pack(WIN_Y3.q(), WIN_Y4.q(), WIN_Y5.q(), WIN_Y6.q(), WIN_Y7.q()));
    text_painter.dprintf("WIN TILE Y %d\n", pack(WIN_Y0.q(), WIN_Y1.q(), WIN_Y2.q()));
    text_painter.dprintf("FINE_CNT   %d\n", pack(FINE_CNT0.q(), FINE_CNT1.q(), FINE_CNT2.q()));

    text_painter.dprintf("MAxx     0x%04x\n", pack(MA00.q(), MA01.q(), MA02.q(), MA03.q(), MA04.q(), MA05.q(), MA06.q(), MA07.q(), MA08.q(), MA09.q(), MA10.q(), MA11.q(), MA12.q()));
    text_painter.dprintf("MDxx     0x%02x\n", pack(MD0.q(), MD1.q(), MD2.q(), MD3.q(), MD4.q(), MD5.q(), MD6.q(), MD7.q()));

    ROXY_FINE_MATCH_LATCH         .dump(text_painter, "ROXY_FINE_MATCH_LATCH          ");
    PUXA_FINE_MATCH_SYNC1        .dump(text_painter, "PUXA_FINE_MATCH_SYNC1         ");
    NYZE_FINE_MATCH_SYNC2        .dump(text_painter, "NYZE_FINE_MATCH_SYNC2         ");
    NOPA_WIN_MODE_SYNC      .dump(text_painter, "NOPA_WIN_MODE_SYNC       ");
    SOVY_WIN_HIT_SYNC      .dump(text_painter, "SOVY_WIN_HIT_SYNC       ");
    XYMU_RENDERINGp    .dump(text_painter, "RENDERING_LATCH     ");
    RENDER_DONE_SYNC        .dump(text_painter, "RENDER_DONE_SYNC         ");
    PYNU_WIN_MODE_LATCH     .dump(text_painter, "PYNU_WIN_MODE_LATCH      ");
    RYDY_WIN_HIT_LATCH     .dump(text_painter, "RYDY_WIN_HIT_LATCH      ");
    INT_HBL_EN              .dump(text_painter, "INT_HBL_EN               ");
    INT_VBL_EN              .dump(text_painter, "INT_VBL_EN               ");
    INT_OAM_EN              .dump(text_painter, "INT_OAM_EN               ");
    INT_LYC_EN              .dump(text_painter, "INT_LYC_EN               ");
    //LY_MATCH_SYNC      .dump(text_painter, "LY_MATCH_SYNC       ");
    //LYC_MATCH_LATCHn    .dump(text_painter, "LYC_MATCH_LATCHn     ");
    WY_MATCH_SYNC           .dump(text_painter, "WY_MATCH_SYNC            ");
    WIN_MATCH_ONSCREEN_SYNC1.dump(text_painter, "WIN_MATCH_ONSCREEN_SYNC1 ");
    WIN_MATCH_ONSCREEN_SYNC2.dump(text_painter, "WIN_MATCH_ONSCREEN_SYNC2 ");
    WIN_MATCH_SYNC1         .dump(text_painter, "WIN_MATCH_SYNC1          ");
    WIN_MATCH_SYNC2         .dump(text_painter, "WIN_MATCH_SYNC2          ");
    BG_READ_VRAM_LATCHp     .dump(text_painter, "BG_READ_VRAM_LATCH      ");
    BFETCH_S0         .dump(text_painter, "BFETCH_S0          ");
    BFETCH_S1         .dump(text_painter, "BFETCH_S1          ");
    BFETCH_S2         .dump(text_painter, "BFETCH_S2          ");
    LOVY_BG_SEQ5_SYNC            .dump(text_painter, "BG_SEQ5_SYNC             ");
    BFETCH_DONE_SYNC                .dump(text_painter, "BFETCH_DONE_SYNC                 ");
    BFETCH_DONE_SYNC_DELAY                .dump(text_painter, "BFETCH_DONE_SYNC_DELAY                 ");
    BFETCH_S0_DELAY   .dump(text_painter, "BFETCH_S0_DELAY    ");
    PYGO_TILE_DONE          .dump(text_painter, "PYGO_TILE_DONE           ");
    POKY_FRONT_PORCH_LATCH .dump(text_painter, "POKY_FRONT_PORCH_LATCH  ");
    WY_MATCH_LATCH          .dump(text_painter, "WY_MATCH_LATCH           ");
    SFETCH_RUN_LATCH     .dump(text_painter, "SFETCH_RUN_LATCH      ");
    SOBU_SPRITE_FETCH_SYNC1     .dump(text_painter, "SOBU_SPRITE_FETCH_SYNC1      ");
    SUDA_SPRITE_FETCH_SYNC2     .dump(text_painter, "SUDA_SPRITE_FETCH_SYNC2      ");
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

  /*p27.WYKA*/ Reg3 WIN_X3;
  /*p27.WODY*/ Reg3 WIN_X4;
  /*p27.WOBO*/ Reg3 WIN_X5;
  /*p27.WYKO*/ Reg3 WIN_X6;
  /*p27.XOLO*/ Reg3 WIN_X7;

  /*p27.TUFU*/ Reg3 WIN_Y3;
  /*p27.TAXA*/ Reg3 WIN_Y4;
  /*p27.TOZO*/ Reg3 WIN_Y5;
  /*p27.TATE*/ Reg3 WIN_Y6;
  /*p27.TEKE*/ Reg3 WIN_Y7;

  /*p27.VYNO*/ Reg3 WIN_Y0;
  /*p27.VUJO*/ Reg3 WIN_Y1;
  /*p27.VYMU*/ Reg3 WIN_Y2;

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

  /*p??.ROXY*/ Latch3 ROXY_FINE_MATCH_LATCH;
  /*p??.PUXA*/ Reg3   PUXA_FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg3   NYZE_FINE_MATCH_SYNC2;

  /*p27.NOPA*/ Reg3 NOPA_WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg3 SOVY_WIN_HIT_SYNC;

  /*p21.XYMU*/ Latch3 XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.VOGA*/ Reg3 RENDER_DONE_SYNC;

  /*p27.PYNU*/ Latch3 PYNU_WIN_MODE_LATCH;
  /*p27.RYDY*/ Latch3 RYDY_WIN_HIT_LATCH;

  /*p21.ROXE*/ Reg3 INT_HBL_EN;
  /*p21.RUFO*/ Reg3 INT_VBL_EN;
  /*p21.REFE*/ Reg3 INT_OAM_EN;
  /*p21.RUGU*/ Reg3 INT_LYC_EN;

  /*p27.SARY*/ Reg3 WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg3 WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg3 WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg3 WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg3 WIN_MATCH_SYNC2;

  /*p27.LONY*/ Latch3 BG_READ_VRAM_LATCHp; // LONY has "arms" on the VCC side

  /*p27.LAXU*/ Reg3 BFETCH_S0;
  /*p27.MESU*/ Reg3 BFETCH_S1;
  /*p27.NYVA*/ Reg3 BFETCH_S2;

  /*p27.LOVY*/ Reg3 LOVY_BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg3 BFETCH_DONE_SYNC;
  /*p24.PORY*/ Reg3 BFETCH_DONE_SYNC_DELAY;
  /*p27.LYZU*/ Reg3 BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg3 PYGO_TILE_DONE;
  /*p24.POKY*/ Latch3 POKY_FRONT_PORCH_LATCH;

  /*p27.REJO*/ Latch3 WY_MATCH_LATCH;

  /*p27.TAKA*/ Latch3 SFETCH_RUN_LATCH;
  /*p27.SOBU*/ Reg3 SOBU_SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg3 SUDA_SPRITE_FETCH_SYNC2;



};

//-----------------------------------------------------------------------------

struct OamRegisters {

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_REG -----\n");
    text_painter.dprintf("LATCH_A 0x%02x\n", pack(LATCH_OAM_A0.q(),LATCH_OAM_A1.q(),LATCH_OAM_A2.q(),LATCH_OAM_A3.q(),LATCH_OAM_A4.q(),LATCH_OAM_A5.q(),LATCH_OAM_A6.q(),LATCH_OAM_A7.q()));
    text_painter.dprintf("LATCH_B 0x%02x\n", pack(LATCH_OAM_B0.q(),LATCH_OAM_B1.q(),LATCH_OAM_B2.q(),LATCH_OAM_B3.q(),LATCH_OAM_B4.q(),LATCH_OAM_B5.q(),LATCH_OAM_B6.q(),LATCH_OAM_B7.q()));
    text_painter.dprintf("REG_A   0x%02x\n", pack(REG_OAM_A0.q(),REG_OAM_A1.q(),REG_OAM_A2.q(),REG_OAM_A3.q(),REG_OAM_A4.q(),REG_OAM_A5.q(),REG_OAM_A6.q(),REG_OAM_A7.q()));
    text_painter.dprintf("REG_B   0x%02x\n", pack(REG_OAM_B0.q(),REG_OAM_B1.q(),REG_OAM_B2.q(),REG_OAM_B3.q(),REG_OAM_B4.q(),REG_OAM_B5.q(),REG_OAM_B6.q(),REG_OAM_B7.q()));
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