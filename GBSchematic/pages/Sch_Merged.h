#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct BusRegisters {

  /*p07.TEPU*/ Reg2 BOOT_BIT;

  /*p25.SOTO*/ Reg2 SOTO_DBG;

  /*p08.ALOR*/ Reg2 ADDR_LATCH_00;
  /*p08.APUR*/ Reg2 ADDR_LATCH_01;
  /*p08.ALYR*/ Reg2 ADDR_LATCH_02;
  /*p08.ARET*/ Reg2 ADDR_LATCH_03;
  /*p08.AVYS*/ Reg2 ADDR_LATCH_04;
  /*p08.ATEV*/ Reg2 ADDR_LATCH_05;
  /*p08.AROS*/ Reg2 ADDR_LATCH_06;
  /*p08.ARYM*/ Reg2 ADDR_LATCH_07;
  /*p08.LUNO*/ Reg2 ADDR_LATCH_08;
  /*p08.LYSA*/ Reg2 ADDR_LATCH_09;
  /*p08.PATE*/ Reg2 ADDR_LATCH_10;
  /*p08.LUMY*/ Reg2 ADDR_LATCH_11;
  /*p08.LOBU*/ Reg2 ADDR_LATCH_12;
  /*p08.LONU*/ Reg2 ADDR_LATCH_13;
  /*p08.NYRE*/ Reg2 ADDR_LATCH_14;

  /*p08.SOMA*/ Reg2 DATA_LATCH_00;
  /*p08.RONY*/ Reg2 DATA_LATCH_01;
  /*p08.RAXY*/ Reg2 DATA_LATCH_02;
  /*p08.SELO*/ Reg2 DATA_LATCH_03;
  /*p08.SODY*/ Reg2 DATA_LATCH_04;
  /*p08.SAGO*/ Reg2 DATA_LATCH_05;
  /*p08.RUPA*/ Reg2 DATA_LATCH_06;
  /*p08.SAZY*/ Reg2 DATA_LATCH_07;
};

//-----------------------------------------------------------------------------

struct ClkRegisters {
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ Reg2 PHAZ_ABCDxxxx;
  /*p01.ALEF*/ Reg2 PHAZ_xBCDExxx;
  /*p01.APUK*/ Reg2 PHAZ_xxCDEFxx;
  /*p01.ADYK*/ Reg2 PHAZ_xxxDEFGx;
};

//-----------------------------------------------------------------------------

struct ConfigRegisters {

  int get_lcdc() const {
    return pack(LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
                LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);
  }

  void set_lcdc(uint8_t x) {
    big_set2(x, LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
                LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);
  }

  int get_scy()  const { return pack(SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7); }
  int get_scx()  const { return pack(SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7); }
  int get_lyc()  const { return pack(LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7); }
  int get_bgp()  const { return pack(BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7); }
  int get_obp0() const { return pack(OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07); }
  int get_obp1() const { return pack(OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17); }
  int get_wy()   const { return pack(WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7); }
  int get_wx()   const { return pack(WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7); }

  int set_scy(uint8_t x)  { big_set2(x, SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7); }
  int set_scx(uint8_t x)  { big_set2(x, SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7); }
  int set_lyc(uint8_t x)  { big_set2(x, LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7); }
  int set_bgp(uint8_t x)  { big_set2(x, BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7); }
  int set_obp0(uint8_t x) { big_set2(x, OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07); }
  int set_obp1(uint8_t x) { big_set2(x, OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17); }
  int set_wy(uint8_t x)   { big_set2(x, WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7); }
  int set_wx(uint8_t x)   { big_set2(x, WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7); }

  // FF40 - LCDC
  /*p23.VYXE*/ Reg2 LCDC_BGEN;
  /*p23.XYLO*/ Reg2 LCDC_SPEN;
  /*p23.XYMO*/ Reg2 LCDC_SPSIZE;
  /*p23.XAFO*/ Reg2 LCDC_BGMAP;
  /*p23.WEXU*/ Reg2 LCDC_BGTILE;
  /*p23.WYMO*/ Reg2 LCDC_WINEN;
  /*p23.WOKY*/ Reg2 LCDC_WINMAP;
  /*p23.XONA*/ Reg2 LCDC_EN;

  // FF42 - SCY
  /*p23.GAVE*/ Reg2 SCY0;
  /*p23.FYMO*/ Reg2 SCY1;
  /*p23.FEZU*/ Reg2 SCY2;
  /*p23.FUJO*/ Reg2 SCY3;
  /*p23.DEDE*/ Reg2 SCY4;
  /*p23.FOTY*/ Reg2 SCY5;
  /*p23.FOHA*/ Reg2 SCY6;
  /*p23.FUNY*/ Reg2 SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg2 SCX0;
  /*p23.DUZU*/ Reg2 SCX1;
  /*p23.CYXU*/ Reg2 SCX2;
  /*p23.GUBO*/ Reg2 SCX3;
  /*p23.BEMY*/ Reg2 SCX4;
  /*p23.CUZY*/ Reg2 SCX5;
  /*p23.CABU*/ Reg2 SCX6;
  /*p23.BAKE*/ Reg2 SCX7;

  // FF45 - LYC
  /*p23.SYRY*/ Reg2 LYC0;
  /*p23.VUCE*/ Reg2 LYC1;
  /*p23.SEDY*/ Reg2 LYC2;
  /*p23.SALO*/ Reg2 LYC3;
  /*p23.SOTA*/ Reg2 LYC4;
  /*p23.VAFA*/ Reg2 LYC5;
  /*p23.VEVO*/ Reg2 LYC6;
  /*p23.RAHA*/ Reg2 LYC7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg2 BGP0;
  /*p36.NUSY*/ Reg2 BGP1;
  /*p36.PYLU*/ Reg2 BGP2;
  /*p36.MAXY*/ Reg2 BGP3;
  /*p36.MUKE*/ Reg2 BGP4;
  /*p36.MORU*/ Reg2 BGP5;
  /*p36.MOGY*/ Reg2 BGP6;
  /*p36.MENA*/ Reg2 BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg2 OBP00;
  /*p36.XUKY*/ Reg2 OBP01;
  /*p36.XOVA*/ Reg2 OBP02;
  /*p36.XALO*/ Reg2 OBP03;
  /*p36.XERU*/ Reg2 OBP04;
  /*p36.XYZE*/ Reg2 OBP05;
  /*p36.XUPO*/ Reg2 OBP06;
  /*p36.XANA*/ Reg2 OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg2 OBP10;
  /*p36.LAWO*/ Reg2 OBP11;
  /*p36.MOSA*/ Reg2 OBP12;
  /*p36.LOSE*/ Reg2 OBP13;
  /*p36.LUNE*/ Reg2 OBP14;
  /*p36.LUGU*/ Reg2 OBP15;
  /*p36.LEPU*/ Reg2 OBP16;
  /*p36.LUXO*/ Reg2 OBP17;

  // FF4A - WY
  /*p23.NESO*/ Reg2 WY0;
  /*p23.NYRO*/ Reg2 WY1;
  /*p23.NAGA*/ Reg2 WY2;
  /*p23.MELA*/ Reg2 WY3;
  /*p23.NULO*/ Reg2 WY4;
  /*p23.NENE*/ Reg2 WY5;
  /*p23.NUKA*/ Reg2 WY6;
  /*p23.NAFU*/ Reg2 WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg2 WX0;
  /*p23.NOFE*/ Reg2 WX1;
  /*p23.NOKE*/ Reg2 WX2;
  /*p23.MEBY*/ Reg2 WX3;
  /*p23.MYPU*/ Reg2 WX4;
  /*p23.MYCE*/ Reg2 WX5;
  /*p23.MUVO*/ Reg2 WX6;
  /*p23.NUKU*/ Reg2 WX7;
};

//-----------------------------------------------------------------------------

struct DebugRegisters {

  /*p07.BURO*/ Reg2 FF60_0;
  /*p07.AMUT*/ Reg2 FF60_1;
};

//-----------------------------------------------------------------------------

struct DmaRegisters {
  /*p04.MAKA*/ Reg2 FROM_CPU5_SYNC;

  /*p04.MATU*/ Reg2 REG_DMA_RUNNING; // -> p25,p28
  /*p04.MYTE*/ Reg2 DMA_DONE_SYNC;
  /*p04.LUVY*/ Reg2 REG_DMA_EN_d0;
  /*p04.LENE*/ Reg2 REG_DMA_EN_d4;

  /*p04.NAKY*/ Reg2 DMA_A00;
  /*p04.PYRO*/ Reg2 DMA_A01; 
  /*p04.NEFY*/ Reg2 DMA_A02; 
  /*p04.MUTY*/ Reg2 DMA_A03; 
  /*p04.NYKO*/ Reg2 DMA_A04; 
  /*p04.PYLO*/ Reg2 DMA_A05; 
  /*p04.NUTO*/ Reg2 DMA_A06; 
  /*p04.MUGU*/ Reg2 DMA_A07; 
  /*p04.NAFA*/ Reg2 DMA_A08; 
  /*p04.PYNE*/ Reg2 DMA_A09; 
  /*p04.PARA*/ Reg2 DMA_A10; 
  /*p04.NYDO*/ Reg2 DMA_A11; 
  /*p04.NYGY*/ Reg2 DMA_A12; 
  /*p04.PULA*/ Reg2 DMA_A13; 
  /*p04.POKU*/ Reg2 DMA_A14; 
  /*p04.MARU*/ Reg2 DMA_A15; 

  /*p04.LYXE*/ Reg2 LATCH_DMA_ENn_d0;
  /*p04.LOKY*/ Reg2 LATCH_DMA_EN_d4; // NAND latch
};

//-----------------------------------------------------------------------------

struct InterruptRegisters {

  /*
  Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
  Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
  Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
  Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
  Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
  */

  /*p02.LOPE*/ Reg2 FF0F_0;
  /*p02.UBUL*/ Reg2 FF0F_1;
  /*p02.ULAK*/ Reg2 FF0F_2;
  /*p02.LALU*/ Reg2 FF0F_3;
  /*p02.NYBO*/ Reg2 FF0F_4;

  /*p02.MATY*/ Reg2 FF0F_L0;
  /*p02.NEJY*/ Reg2 FF0F_L1;
  /*p02.NUTY*/ Reg2 FF0F_L2;
  /*p02.MOPO*/ Reg2 FF0F_L3;
  /*p02.PAVY*/ Reg2 FF0F_L4;

};

//-----------------------------------------------------------------------------

struct JoypadRegisters {
  /*p02.BATU*/ Reg2 JP_GLITCH0;
  /*p02.ACEF*/ Reg2 JP_GLITCH1;
  /*p02.AGEM*/ Reg2 JP_GLITCH2;
  /*p02.APUG*/ Reg2 JP_GLITCH3;
  /*p05.JUTE*/ Reg2 JOYP_RA;
  /*p05.KECY*/ Reg2 JOYP_LB;
  /*p05.JALE*/ Reg2 JOYP_UC;
  /*p05.KYME*/ Reg2 JOYP_DS;
  /*p05.KELY*/ Reg2 JOYP_UDLR;
  /*p05.COFY*/ Reg2 JOYP_ABCS;
  /*p05.KUKO*/ Reg2 DBG_FF00_D6;
  /*p05.KERU*/ Reg2 DBG_FF00_D7;
  /*p05.KEVU*/ Reg2 JOYP_L0;
  /*p05.KAPA*/ Reg2 JOYP_L1;
  /*p05.KEJA*/ Reg2 JOYP_L2;
  /*p05.KOLO*/ Reg2 JOYP_L3;
  /*p02.AWOB*/ Reg2 WAKE_CPU;
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }

  /*p21.SAXO*/ Reg2 X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg2 X1;
  /*p21.VYZO*/ Reg2 X2;
  /*p21.TELU*/ Reg2 X3;
  /*p21.SUDE*/ Reg2 X4;
  /*p21.TAHA*/ Reg2 X5;
  /*p21.TYRY*/ Reg2 X6;

  /*p21.MUWY*/ Reg2 Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg2 Y1;
  /*p21.LEXA*/ Reg2 Y2;
  /*p21.LYDO*/ Reg2 Y3;
  /*p21.LOVU*/ Reg2 Y4;
  /*p21.LEMA*/ Reg2 Y5;
  /*p21.MATO*/ Reg2 Y6;
  /*p21.LAFO*/ Reg2 Y7;

  /*p21.RUTU*/ Reg2 RUTU_NEW_LINE_d0; // p909+8
  /*p29.CATU*/ Reg2 VID_LINE_d4;  // p001+8
  /*p21.NYPE*/ Reg2 NYPE_NEW_LINE_d4; // p001+8
  /*p28.ANEL*/ Reg2 VID_LINE_d6;  // p003+8
                                 
  /*p21.MYTA*/ Reg2 LINE_153_d4;  // p153:001 - p000:001
  /*p21.POPU*/ Reg2 POPU_IN_VBLANK_d4;    // p144:001 - p000:001

  /*p21.SYGU*/ Reg2 LINE_STROBE;
  /*p24.PAHO*/ Reg2 X_8_SYNC;

  /*p21.WUSA*/ Reg2 CPEN_LATCH;

  /*p24.POME*/ Reg2 POME; // nand latch with p24.RUJU
  /*p24.RUJU*/ Reg2 RUJU; // nand latch with p24.POME

  /*p24.MEDA*/ Reg2 VSYNC_OUTn;
  /*p24.LUCA*/ Reg2 LINE_EVEN;
  /*p21.NAPO*/ Reg2 FRAME_EVEN;
};

//-----------------------------------------------------------------------------

struct PixelPipeRegisters {

  /*p32.LEGU*/ Reg2 BG_PIX_A0;
  /*p32.NUDU*/ Reg2 BG_PIX_A1;
  /*p32.MUKU*/ Reg2 BG_PIX_A2;
  /*p32.LUZO*/ Reg2 BG_PIX_A3;
  /*p32.MEGU*/ Reg2 BG_PIX_A4;
  /*p32.MYJY*/ Reg2 BG_PIX_A5;
  /*p32.NASA*/ Reg2 BG_PIX_A6;
  /*p32.NEFO*/ Reg2 BG_PIX_A7;

  /*p32.RAWU*/ Reg2 BG_PIX_B0;
  /*p32.POZO*/ Reg2 BG_PIX_B1;
  /*p32.PYZO*/ Reg2 BG_PIX_B2;
  /*p32.POXA*/ Reg2 BG_PIX_B3;
  /*p32.PULO*/ Reg2 BG_PIX_B4;
  /*p32.POJU*/ Reg2 BG_PIX_B5;
  /*p32.POWY*/ Reg2 BG_PIX_B6;
  /*p32.PYJU*/ Reg2 BG_PIX_B7;

  /*p33.PEFO*/ Reg2 SPR_PIX_A0;
  /*p33.ROKA*/ Reg2 SPR_PIX_A1;
  /*p33.MYTU*/ Reg2 SPR_PIX_A2;
  /*p33.RAMU*/ Reg2 SPR_PIX_A3;
  /*p33.SELE*/ Reg2 SPR_PIX_A4;
  /*p33.SUTO*/ Reg2 SPR_PIX_A5;
  /*p33.RAMA*/ Reg2 SPR_PIX_A6;
  /*p33.RYDU*/ Reg2 SPR_PIX_A7;

  /*p33.REWO*/ Reg2 SPR_PIX_B0;
  /*p33.PEBA*/ Reg2 SPR_PIX_B1;
  /*p33.MOFO*/ Reg2 SPR_PIX_B2;
  /*p33.PUDU*/ Reg2 SPR_PIX_B3;
  /*p33.SAJA*/ Reg2 SPR_PIX_B4;
  /*p33.SUNY*/ Reg2 SPR_PIX_B5;
  /*p33.SEMO*/ Reg2 SPR_PIX_B6;
  /*p33.SEGA*/ Reg2 SPR_PIX_B7;

  /*p32.MYDE*/ Reg2 BG_PIPE_A0;
  /*p32.NOZO*/ Reg2 BG_PIPE_A1;
  /*p32.MOJU*/ Reg2 BG_PIPE_A2;
  /*p32.MACU*/ Reg2 BG_PIPE_A3;
  /*p32.NEPO*/ Reg2 BG_PIPE_A4;
  /*p32.MODU*/ Reg2 BG_PIPE_A5;
  /*p32.NEDA*/ Reg2 BG_PIPE_A6;
  /*p32.PYBO*/ Reg2 BG_PIPE_A7;

  /*p32.TOMY*/ Reg2 BG_PIPE_B0;
  /*p32.TACA*/ Reg2 BG_PIPE_B1;
  /*p32.SADY*/ Reg2 BG_PIPE_B2;
  /*p32.RYSA*/ Reg2 BG_PIPE_B3;
  /*p32.SOBO*/ Reg2 BG_PIPE_B4;
  /*p32.SETU*/ Reg2 BG_PIPE_B5;
  /*p32.RALU*/ Reg2 BG_PIPE_B6;
  /*p32.SOHU*/ Reg2 BG_PIPE_B7;

  /*p33.NURO*/ Reg2 SPR_PIPE_B0;
  /*p33.MASO*/ Reg2 SPR_PIPE_B1;
  /*p33.LEFE*/ Reg2 SPR_PIPE_B2;
  /*p33.LESU*/ Reg2 SPR_PIPE_B3;
  /*p33.WYHO*/ Reg2 SPR_PIPE_B4;
  /*p33.WORA*/ Reg2 SPR_PIPE_B5;
  /*p33.VAFO*/ Reg2 SPR_PIPE_B6;
  /*p33.WUFY*/ Reg2 SPR_PIPE_B7;

  /*p33.NYLU*/ Reg2 SPR_PIPE_A0;
  /*p33.PEFU*/ Reg2 SPR_PIPE_A1;
  /*p33.NATY*/ Reg2 SPR_PIPE_A2;
  /*p33.PYJO*/ Reg2 SPR_PIPE_A3;
  /*p33.VARE*/ Reg2 SPR_PIPE_A4;
  /*p33.WEBA*/ Reg2 SPR_PIPE_A5;
  /*p33.VANU*/ Reg2 SPR_PIPE_A6;
  /*p33.VUPY*/ Reg2 SPR_PIPE_A7;

  /*p34.RUGO*/ Reg2 PAL_PIPE_0;
  /*p34.SATA*/ Reg2 PAL_PIPE_1;
  /*p34.ROSA*/ Reg2 PAL_PIPE_2;
  /*p34.SOMY*/ Reg2 PAL_PIPE_3;
  /*p34.PALU*/ Reg2 PAL_PIPE_4;
  /*p34.NUKE*/ Reg2 PAL_PIPE_5;
  /*p34.MODA*/ Reg2 PAL_PIPE_6;
  /*p34.LYME*/ Reg2 PAL_PIPE_7;

  /*p26.VEZO*/ Reg2 MASK_PIPE_0;
  /*p26.WURU*/ Reg2 MASK_PIPE_1;
  /*p26.VOSA*/ Reg2 MASK_PIPE_2;
  /*p26.WYFU*/ Reg2 MASK_PIPE_3;
  /*p26.XETE*/ Reg2 MASK_PIPE_4;
  /*p26.WODA*/ Reg2 MASK_PIPE_5;
  /*p26.VUMO*/ Reg2 MASK_PIPE_6;
  /*p26.VAVA*/ Reg2 MASK_PIPE_7;
};

//-----------------------------------------------------------------------------

struct RstRegisters {
  /*p01.TUBO*/ Reg2 WAITING_FOR_CLKREQ;
  /*p01.AFER*/ Reg2 RESET_REG;
};

//-----------------------------------------------------------------------------

struct SerialRegisters {

  /*p06.ETAF*/ Reg2 XFER_START;
  /*p06.CULY*/ Reg2 XFER_DIR;

  /*p06.COTY*/ Reg2 SER_CLK;

  /*p06.CAFA*/ Reg2 SER_CNT0;
  /*p06.CYLO*/ Reg2 SER_CNT1;
  /*p06.CYDE*/ Reg2 SER_CNT2;
  /*p06.CALY*/ Reg2 SER_CNT3;

  /*p06.CUBA*/ Reg2 SER_DATA0;
  /*p06.DEGU*/ Reg2 SER_DATA1;
  /*p06.DYRA*/ Reg2 SER_DATA2;
  /*p06.DOJO*/ Reg2 SER_DATA3;
  /*p06.DOVU*/ Reg2 SER_DATA4;
  /*p06.EJAB*/ Reg2 SER_DATA5;
  /*p06.EROD*/ Reg2 SER_DATA6;
  /*p06.EDER*/ Reg2 SER_DATA7;

  /*p06.ELYS*/ Reg2 SER_OUT;
};

//-----------------------------------------------------------------------------

struct SpriteRegisters {

  /*p29.CENO*/ Reg2 STORE_SPRITE_IDXn;

  /*p28.YFEL*/ Reg2 SCAN0;
  /*p28.WEWY*/ Reg2 SCAN1;
  /*p28.GOSO*/ Reg2 SCAN2;
  /*p28.ELYN*/ Reg2 SCAN3;
  /*p28.FAHA*/ Reg2 SCAN4;
  /*p28.FONY*/ Reg2 SCAN5;

  /*p30.XADU*/ Reg2 SPRITE_IDX0;
  /*p30.XEDY*/ Reg2 SPRITE_IDX1;
  /*p30.ZUZE*/ Reg2 SPRITE_IDX2;
  /*p30.XOBE*/ Reg2 SPRITE_IDX3;
  /*p30.YDUF*/ Reg2 SPRITE_IDX4;
  /*p30.XECU*/ Reg2 SPRITE_IDX5;

  /*p29.BYBA*/ Reg2 SCAN_DONE_d4;
  /*p29.DOBA*/ Reg2 SCAN_DONE_d5;

  /*p29.TOXE*/ Reg2 TOXE_SPR_SEQ0;
  /*p29.TULY*/ Reg2 TULY_SPR_SEQ1;
  /*p29.TESE*/ Reg2 TESE_SPR_SEQ2;
  
  /*p29.TOBU*/ Reg2 TOBU_SEQ_xx23xx;
  /*p29.VONU*/ Reg2 VONU_SEQ_xxx34xn;
  /*p29.SEBA*/ Reg2 SEBA_SEQ_xxxx45n;
  /*p29.TYFO*/ Reg2 TYFO_SEQ_B0d;

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

  /*p29.DEZY*/ Reg2 STORE_EN_SYNC;
  /*p29.BESE*/ Reg2 SPRITE_COUNT0;
  /*p29.CUXY*/ Reg2 SPRITE_COUNT1;
  /*p29.BEGO*/ Reg2 SPRITE_COUNT2;
  /*p29.DYBE*/ Reg2 SPRITE_COUNT3;

  /*p29.EBOJ*/ Reg2 SPRITE0_GET_SYNC;
  /*p30.YGUS*/ Reg2 STORE0_IDX0;
  /*p30.YSOK*/ Reg2 STORE0_IDX1;
  /*p30.YZEP*/ Reg2 STORE0_IDX2;
  /*p30.WYTE*/ Reg2 STORE0_IDX3;
  /*p30.ZONY*/ Reg2 STORE0_IDX4;
  /*p30.YWAK*/ Reg2 STORE0_IDX5;
  /*p30.FYHY*/ Reg2 STORE0_LINE0;
  /*p30.GYHO*/ Reg2 STORE0_LINE1;
  /*p30.BOZU*/ Reg2 STORE0_LINE2;
  /*p30.CUFO*/ Reg2 STORE0_LINE3;
  /*p31.XEPE*/ Reg2 STORE0_X0;
  /*p31.YLAH*/ Reg2 STORE0_X1;
  /*p31.ZOLA*/ Reg2 STORE0_X2;
  /*p31.ZULU*/ Reg2 STORE0_X3;
  /*p31.WELO*/ Reg2 STORE0_X4;
  /*p31.XUNY*/ Reg2 STORE0_X5;
  /*p31.WOTE*/ Reg2 STORE0_X6;
  /*p31.XAKO*/ Reg2 STORE0_X7;

  /*p29.CEDY*/ Reg2 SPRITE1_GET_SYNCn;
  /*p30.CAJY*/ Reg2 STORE1_IDX0;
  /*p30.CUZA*/ Reg2 STORE1_IDX1;
  /*p30.COMA*/ Reg2 STORE1_IDX2;
  /*p30.CUFA*/ Reg2 STORE1_IDX3;
  /*p30.CEBO*/ Reg2 STORE1_IDX4;
  /*p30.CADU*/ Reg2 STORE1_IDX5;
  /*p30.ABUG*/ Reg2 STORE1_LINE0;
  /*p30.AMES*/ Reg2 STORE1_LINE1;
  /*p30.ABOP*/ Reg2 STORE1_LINE2;
  /*p30.AROF*/ Reg2 STORE1_LINE3;
  /*p31.DANY*/ Reg2 STORE1_X0;
  /*p31.DUKO*/ Reg2 STORE1_X1;
  /*p31.DESU*/ Reg2 STORE1_X2;
  /*p31.DAZO*/ Reg2 STORE1_X3;
  /*p31.DAKE*/ Reg2 STORE1_X4;
  /*p31.CESO*/ Reg2 STORE1_X5;
  /*p31.DYFU*/ Reg2 STORE1_X6;
  /*p31.CUSY*/ Reg2 STORE1_X7;

  /*p29.EGAV*/ Reg2 SPRITE2_GET_SYNCn;
  /*p30.BOXA*/ Reg2 STORE2_IDX0;
  /*p30.BUNA*/ Reg2 STORE2_IDX1;
  /*p30.BULU*/ Reg2 STORE2_IDX2;
  /*p30.BECA*/ Reg2 STORE2_IDX3;
  /*p30.BYHU*/ Reg2 STORE2_IDX4;
  /*p30.BUHE*/ Reg2 STORE2_IDX5;
  /*p30.YKUK*/ Reg2 STORE2_LINE0;
  /*p30.YLOV*/ Reg2 STORE2_LINE1;
  /*p30.XAZY*/ Reg2 STORE2_LINE2;
  /*p30.XOSY*/ Reg2 STORE2_LINE3;
  /*p31.FOKA*/ Reg2 STORE2_X0;
  /*p31.FYTY*/ Reg2 STORE2_X1;
  /*p31.FUBY*/ Reg2 STORE2_X2;
  /*p31.GOXU*/ Reg2 STORE2_X3;
  /*p31.DUHY*/ Reg2 STORE2_X4;
  /*p31.EJUF*/ Reg2 STORE2_X5;
  /*p31.ENOR*/ Reg2 STORE2_X6;
  /*p31.DEPY*/ Reg2 STORE2_X7;

  /*p29.GOTA*/ Reg2 SPRITE3_GET_SYNCn;
  /*p30.DAFU*/ Reg2 DAFU;
  /*p30.DEBA*/ Reg2 DEBA;
  /*p30.DUHA*/ Reg2 DUHA;
  /*p30.DUNY*/ Reg2 DUNY;
  /*p30.DESE*/ Reg2 DESE;
  /*p30.DEVY*/ Reg2 DEVY;
  /*p30.ZURY*/ Reg2 ZURY;
  /*p30.ZURO*/ Reg2 ZURO;
  /*p30.ZENE*/ Reg2 ZENE;
  /*p30.ZYLU*/ Reg2 ZYLU;
  /*p31.XOLY*/ Reg2 XOLY;
  /*p31.XYBA*/ Reg2 XYBA;
  /*p31.XABE*/ Reg2 XABE;
  /*p31.XEKA*/ Reg2 XEKA;
  /*p31.XOMY*/ Reg2 XOMY;
  /*p31.WUHA*/ Reg2 WUHA;
  /*p31.WYNA*/ Reg2 WYNA;
  /*p31.WECO*/ Reg2 WECO;

  /*p29.XUDY*/ Reg2 SPRITE4_GET_SYNC;
  /*p31.WEDU*/ Reg2 WEDU;
  /*p31.YGAJ*/ Reg2 YGAJ;
  /*p31.ZYJO*/ Reg2 ZYJO;
  /*p31.XURY*/ Reg2 XURY;
  /*p31.YBED*/ Reg2 YBED;
  /*p31.ZALA*/ Reg2 ZALA;
  /*p31.WYDE*/ Reg2 WYDE;
  /*p31.XEPA*/ Reg2 XEPA;
  /*p30.XYNU*/ Reg2 XYNU;
  /*p30.XEGE*/ Reg2 XEGE;
  /*p30.XABO*/ Reg2 XABO;
  /*p30.WANU*/ Reg2 WANU;
  /*p30.XEFE*/ Reg2 XEFE;
  /*p30.XAVE*/ Reg2 XAVE;
  /*p30.CUMU*/ Reg2 CUMU;
  /*p30.CAPO*/ Reg2 CAPO;
  /*p30.CONO*/ Reg2 CONO;
  /*p30.CAJU*/ Reg2 CAJU;

  // store 5
  /*p29.WAFY*/ Reg2 SPRITE5_GET_SYNC;
  /*p31.FUSA*/ Reg2 FUSA;
  /*p31.FAXA*/ Reg2 FAXA;
  /*p31.FOZY*/ Reg2 FOZY;
  /*p31.FESY*/ Reg2 FESY;
  /*p31.CYWE*/ Reg2 CYWE;
  /*p31.DYBY*/ Reg2 DYBY;
  /*p31.DURY*/ Reg2 DURY;
  /*p31.CUVY*/ Reg2 CUVY;
  /*p30.EKAP*/ Reg2 EKAP;
  /*p30.ETAV*/ Reg2 ETAV;
  /*p30.EBEX*/ Reg2 EBEX;
  /*p30.GORU*/ Reg2 GORU;
  /*p30.ETYM*/ Reg2 ETYM;
  /*p30.EKOP*/ Reg2 EKOP;
  /*p30.ANED*/ Reg2 ANED;
  /*p30.ACEP*/ Reg2 ACEP;
  /*p30.ABUX*/ Reg2 ABUX;
  /*p30.ABEG*/ Reg2 ABEG;

  // sprite store 6
  /*p29.WOMY*/ Reg2 SPRITE6_GET_SYNCn;
  /*p31.YCOL*/ Reg2 YCOL;
  /*p31.YRAC*/ Reg2 YRAC;
  /*p31.YMEM*/ Reg2 YMEM;
  /*p31.YVAG*/ Reg2 YVAG;
  /*p31.ZOLY*/ Reg2 ZOLY;
  /*p31.ZOGO*/ Reg2 ZOGO;
  /*p31.ZECU*/ Reg2 ZECU;
  /*p31.ZESA*/ Reg2 ZESA;
  /*p30.GECU*/ Reg2 GECU;
  /*p30.FOXY*/ Reg2 FOXY;
  /*p30.GOHU*/ Reg2 GOHU;
  /*p30.FOGO*/ Reg2 FOGO;
  /*p30.GACY*/ Reg2 GACY;
  /*p30.GABO*/ Reg2 GABO;
  /*p30.ZUBE*/ Reg2 ZUBE;
  /*p30.ZUMY*/ Reg2 ZUMY;
  /*p30.ZEXO*/ Reg2 ZEXO;
  /*p30.ZAFU*/ Reg2 ZAFU;

  // sprite store 7
  /*p29.WAPO*/ Reg2 SPRITE7_GET_SYNC;
  /*p31.ERAZ*/ Reg2 STORE7_X0;
  /*p31.EPUM*/ Reg2 STORE7_X1;
  /*p31.EROL*/ Reg2 STORE7_X2;
  /*p31.EHYN*/ Reg2 STORE7_X3;
  /*p31.FAZU*/ Reg2 STORE7_X4;
  /*p31.FAXE*/ Reg2 STORE7_X5;
  /*p31.EXUK*/ Reg2 STORE7_X6;
  /*p31.FEDE*/ Reg2 STORE7_X7;
  /*p30.FUZO*/ Reg2 FUZO;
  /*p30.GESY*/ Reg2 GESY;
  /*p30.FYSU*/ Reg2 FYSU;
  /*p30.FEFA*/ Reg2 FEFA;
  /*p30.GYNO*/ Reg2 GYNO;
  /*p30.GULE*/ Reg2 GULE;
  /*p30.XYGO*/ Reg2 XYGO;
  /*p30.XYNA*/ Reg2 XYNA;
  /*p30.XAKU*/ Reg2 XAKU;
  /*p30.YGUM*/ Reg2 YGUM;

  // sprite store 8
  /*p29.EXUQ*/ Reg2 SPRITE8_GET_SYNCn;
  /*p31.GAVY*/ Reg2 GAVY;
  /*p31.GYPU*/ Reg2 GYPU;
  /*p31.GADY*/ Reg2 GADY;
  /*p31.GAZA*/ Reg2 GAZA;
  /*p31.EZUF*/ Reg2 EZUF;
  /*p31.ENAD*/ Reg2 ENAD;
  /*p31.EBOW*/ Reg2 EBOW;
  /*p31.FYCA*/ Reg2 FYCA;
  /*p30.AXUV*/ Reg2 AXUV;
  /*p30.BADA*/ Reg2 BADA;
  /*p30.APEV*/ Reg2 APEV;
  /*p30.BADO*/ Reg2 BADO;
  /*p30.BEXY*/ Reg2 BEXY;
  /*p30.BYHE*/ Reg2 BYHE;
  /*p30.AFYM*/ Reg2 AFYM;
  /*p30.AZAP*/ Reg2 AZAP;
  /*p30.AFUT*/ Reg2 AFUT;
  /*p30.AFYX*/ Reg2 AFYX;

  // sprite store 9
  /*p29.FONO*/ Reg2 SPRITE9_GET_SYNC;
  /*p31.XUVY*/ Reg2 STORE9_X0;
  /*p31.XERE*/ Reg2 STORE9_X1;
  /*p31.XUZO*/ Reg2 STORE9_X2;
  /*p31.XEXA*/ Reg2 STORE9_X3;
  /*p31.YPOD*/ Reg2 STORE9_X4;
  /*p31.YROP*/ Reg2 STORE9_X5;
  /*p31.YNEP*/ Reg2 STORE9_X6;
  /*p31.YZOF*/ Reg2 STORE9_X7;
  /*p30.XUFO*/ Reg2 STORE9_IDX0;
  /*p30.XUTE*/ Reg2 STORE9_IDX1;
  /*p30.XOTU*/ Reg2 STORE9_IDX2;
  /*p30.XYFE*/ Reg2 STORE9_IDX3;
  /*p30.YZOR*/ Reg2 STORE9_IDX4;
  /*p30.YBER*/ Reg2 STORE9_IDX5;
  /*p30.DEWU*/ Reg2 STORE9_LINE0;
  /*p30.CANA*/ Reg2 STORE9_LINE1;
  /*p30.DYSY*/ Reg2 STORE9_LINE2;
  /*p30.FOFO*/ Reg2 STORE9_LINE3;
};

//-----------------------------------------------------------------------------

struct TimerRegisters {

  int get_div() const {
    return pack(DIV_00, DIV_01, DIV_02, DIV_03, DIV_04, DIV_05, DIV_06, DIV_07,
                DIV_08, DIV_09, DIV_10, DIV_11, DIV_12, DIV_13, DIV_14, DIV_15);
  }

  void set_div(uint16_t x) {
    DIV_00.set2(x & 0x0001); DIV_01.set2(x & 0x0002); DIV_02.set2(x & 0x0004); DIV_03.set2(x & 0x0008);
    DIV_04.set2(x & 0x0010); DIV_05.set2(x & 0x0020); DIV_06.set2(x & 0x0040); DIV_07.set2(x & 0x0080);
    DIV_08.set2(x & 0x0100); DIV_09.set2(x & 0x0200); DIV_10.set2(x & 0x0400); DIV_11.set2(x & 0x0800);
    DIV_12.set2(x & 0x1000); DIV_13.set2(x & 0x2000); DIV_14.set2(x & 0x4000); DIV_15.set2(x & 0x8000);
  }

  int get_tima() const {
    return pack(TIMA_0.v(), TIMA_1.v(), TIMA_2.v(), TIMA_3.v(),
                TIMA_4.v(), TIMA_5.v(), TIMA_6.v(), TIMA_7.v());
  }

  void set_tima(uint8_t x) {
    TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
    TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
  }

  wire get_tima_max()  { return TIMA_MAX; }
  wire get_int_timer() { return INT_TIMER; }

  int get_tma() const {
    return pack(TMA_0, TMA_1, TMA_2, TMA_3, TMA_4, TMA_5, TMA_6, TMA_7);
  }

  void set_tma(uint8_t x) {
    TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
    TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
  }

  int get_tac() const {
    return pack(TAC_0, TAC_1, TAC_2);
  }

  void set_tac(uint8_t x) {
    TAC_0.set2(x & 1);
    TAC_1.set2(x & 2);
    TAC_2.set2(x & 4);
  }

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg2 DIV_00;
  /*p01.UFOR*/ Reg2 DIV_01;
  /*p01.UNER*/ Reg2 DIV_02;
  /*p01.TERO*/ Reg2 DIV_03;
  /*p01.UNYK*/ Reg2 DIV_04;
  /*p01.TAMA*/ Reg2 DIV_05;
  /*p01.UGOT*/ Reg2 DIV_06;
  /*p01.TULU*/ Reg2 DIV_07;
  /*p01.TUGO*/ Reg2 DIV_08;
  /*p01.TOFE*/ Reg2 DIV_09;
  /*p01.TERU*/ Reg2 DIV_10;
  /*p01.SOLA*/ Reg2 DIV_11;
  /*p01.SUBU*/ Reg2 DIV_12;
  /*p01.TEKA*/ Reg2 DIV_13;
  /*p01.UKET*/ Reg2 DIV_14;
  /*p01.UPOF*/ Reg2 DIV_15;

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

  /*p03.NYDU*/ Reg2 TIMA_MAX;
  /*p03.MOBA*/ Reg2 INT_TIMER;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg2 TMA_0;
  /*p03.NYKE*/ Reg2 TMA_1;
  /*p03.MURU*/ Reg2 TMA_2;
  /*p03.TYVA*/ Reg2 TMA_3;
  /*p03.TYRU*/ Reg2 TMA_4;
  /*p03.SUFY*/ Reg2 TMA_5;
  /*p03.PETO*/ Reg2 TMA_6;
  /*p03.SETA*/ Reg2 TMA_7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg2 TAC_0;
  /*p03.SAMY*/ Reg2 TAC_1;
  /*p03.SABO*/ Reg2 TAC_2;
};

//-----------------------------------------------------------------------------

struct VidRegisters {

  /*p??.ROXY*/ Reg2 FINE_MATCH_DUMP;
  /*p??.PUXA*/ Reg2 FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg2 FINE_MATCH_SYNC2;
  /*p27.RYKU*/ Reg2 FINE_CNT0;
  /*p27.ROGA*/ Reg2 FINE_CNT1;
  /*p27.RUBU*/ Reg2 FINE_CNT2;

  //----------

  // x counter
  /*p21.XEHO*/ Reg2 X0;
  /*p21.SAVY*/ Reg2 X1;
  /*p21.XODU*/ Reg2 X2;
  /*p21.XYDO*/ Reg2 X3;
  /*p21.TUHU*/ Reg2 X4;
  /*p21.TUKY*/ Reg2 X5;
  /*p21.TAKO*/ Reg2 X6;
  /*p21.SYBE*/ Reg2 X7;


  /*p27.NOPA*/ Reg2 NOPA_WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg2 SOVY_WIN_MODE_SYNC;

  /*p21.XYMU*/ Reg2 XYMU_RENDERING_LATCH;

  /*p21.VOGA*/ Reg2 RENDER_DONE_SYNC;

  /*p27.PYNU*/ Reg2 PYNU_WIN_MODE_LATCH;
  /*p27.RYDY*/ Reg2 RYDY_WIN_MODE_LATCH;

  /*p21.ROXE*/ Reg2 INT_HBL_EN;
  /*p21.RUFO*/ Reg2 INT_VBL_EN;
  /*p21.REFE*/ Reg2 INT_OAM_EN;
  /*p21.RUGU*/ Reg2 INT_LYC_EN;

  /*p21.ROPO*/ Reg2 ROPO_LY_MATCH_SYNC;
  /*p21.RUPO*/ Reg2 RUPO_LATCH_LYC_MATCH;

  /*p27.SARY*/ Reg2 WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg2 WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg2 WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg2 WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg2 WIN_MATCH_SYNC2;

  /*p27.LONY*/ Reg2 BG_READ_VRAM_LATCHn;

  /*p27.LAXU*/ Reg2 BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ Reg2 BG_SEQ_xx23xx67;
  /*p27.NYVA*/ Reg2 BG_SEQ_xxxx4567;

  /*p27.LOVY*/ Reg2 BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg2 BG_SEQ_6;
  /*p24.PORY*/ Reg2 BG_SEQ_7;
  /*p27.LYZU*/ Reg2 BG_SEQ_x1x3x5x7_DELAY;

  /*p27.WYKA*/ Reg2 MAP_X0;
  /*p27.WODY*/ Reg2 MAP_X1;
  /*p27.WOBO*/ Reg2 MAP_X2;
  /*p27.WYKO*/ Reg2 MAP_X3;
  /*p27.XOLO*/ Reg2 MAP_X4;

  /*p27.VYNO*/ Reg2 TILE_Y0;
  /*p27.VUJO*/ Reg2 TILE_Y1;
  /*p27.VYMU*/ Reg2 TILE_Y2;

  /*p27.TUFU*/ Reg2 MAP_Y0;
  /*p27.TAXA*/ Reg2 MAP_Y1;
  /*p27.TOZO*/ Reg2 MAP_Y2;
  /*p27.TATE*/ Reg2 MAP_Y3;
  /*p27.TEKE*/ Reg2 MAP_Y4;

  /*p24.PYGO*/ Reg2 PYGO_TILE_DONE;
  /*p24.POKY*/ Reg2 POKY_FRONT_PORCH_LATCHn;

  /*p27.REJO*/ Reg2 WY_MATCH_LATCH;

  /*p27.TAKA*/ Reg2 _SPRITE_FETCH_LATCH;
  /*p27.SOBU*/ Reg2 _SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg2 _SPRITE_FETCH_SYNC2;



  // vram bus

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
};

//-----------------------------------------------------------------------------
// Video clocks

struct VclkRegisters {
  /*p29.WUVU*/ Reg2 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg2 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg2 WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

struct OamRegisters {

  /*p31.XYKY*/ Reg2 LATCH_OAM_A0;
  /*p31.YRUM*/ Reg2 LATCH_OAM_A1;
  /*p31.YSEX*/ Reg2 LATCH_OAM_A2;
  /*p31.YVEL*/ Reg2 LATCH_OAM_A3;
  /*p31.WYNO*/ Reg2 LATCH_OAM_A4;
  /*p31.CYRA*/ Reg2 LATCH_OAM_A5;
  /*p31.ZUVE*/ Reg2 LATCH_OAM_A6;
  /*p31.ECED*/ Reg2 LATCH_OAM_A7;

  /*p29.YDYV*/ Reg2 LATCH_OAM_B0;
  /*p29.YCEB*/ Reg2 LATCH_OAM_B1;
  /*p29.ZUCA*/ Reg2 LATCH_OAM_B2;
  /*p29.WONE*/ Reg2 LATCH_OAM_B3;
  /*p29.ZAXE*/ Reg2 LATCH_OAM_B4;
  /*p29.XAFU*/ Reg2 LATCH_OAM_B5;
  /*p29.YSES*/ Reg2 LATCH_OAM_B6;
  /*p29.ZECA*/ Reg2 LATCH_OAM_B7;

  /*p31.YLOR*/ Reg2 REG_OAM_A0;
  /*p31.ZYTY*/ Reg2 REG_OAM_A1;
  /*p31.ZYVE*/ Reg2 REG_OAM_A2;
  /*p31.ZEZY*/ Reg2 REG_OAM_A3;
  /*p31.GOMO*/ Reg2 REG_OAM_A4;
  /*p31.BAXO*/ Reg2 REG_OAM_A5;
  /*p31.YZOS*/ Reg2 REG_OAM_A6;
  /*p31.DEPO*/ Reg2 REG_OAM_A7;

  /*p29.XUSO*/ Reg2 REG_OAM_B0;
  /*p29.XEGU*/ Reg2 REG_OAM_B1;
  /*p29.YJEX*/ Reg2 REG_OAM_B2;
  /*p29.XYJU*/ Reg2 REG_OAM_B3;
  /*p29.YBOG*/ Reg2 REG_OAM_B4;
  /*p29.WYSO*/ Reg2 REG_OAM_B5;
  /*p29.XOTE*/ Reg2 REG_OAM_B6;
  /*p29.YZAB*/ Reg2 REG_OAM_B7;
};

//-----------------------------------------------------------------------------

};