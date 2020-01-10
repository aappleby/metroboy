#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct VidSignals2 {
};

struct ConfigRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();
  
  VidSignals2 tick(const SysSignals& sys_sig,
                   const RstSignals& rst_sig,
                   const BusSignals& bus_sig,
                   const DecoderSignals& dec_sig,
                   BusTristates & bus_out);

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

};