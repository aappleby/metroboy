#include "Sch_Registers.h"

#include "Sch_Decoder.h"
#include "Sch_Resets.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Registers_tick(const RegistersIn& in,
                    RegistersOut& out,
                    const Decoder& dec,
                    const Registers& prev) {

  /*p23.VYRE*/ wire FF40_RD = and(dec.FF40, in.CPU_RD2);
  /*p23.WYCE*/ wire FF40_RDn = not(FF40_RD);

  /*p23.WYPO*/ if (FF40_RDn) out.D0 = prev.LCDC_BGEN;
  /*p23.XERO*/ if (FF40_RDn) out.D1 = prev.LCDC_SPEN;
  /*p23.WYJU*/ if (FF40_RDn) out.D2 = prev.LCDC_SPSIZE;
  /*p23.WUKA*/ if (FF40_RDn) out.D3 = prev.LCDC_BGMAP;
  /*p23.VOKE*/ if (FF40_RDn) out.D4 = prev.LCDC_BGTILE;
  /*p23.VATO*/ if (FF40_RDn) out.D5 = prev.LCDC_WINEN;
  /*p23.VAHA*/ if (FF40_RDn) out.D6 = prev.LCDC_WINMAP;
  /*p23.XEBU*/ if (FF40_RDn) out.D7 = prev.LCDC_EN;

  /*p23.ANYP*/ wire FF42_RD = and(dec.FF42, in.CPU_RD2);
  /*p23.BUWY*/ wire FF42_RDn = not(FF42_RD);

  /*p23.WARE*/ if (FF42_RDn) out.D0 = prev.SCY0;
  /*p23.GOBA*/ if (FF42_RDn) out.D1 = prev.SCY1;
  /*p23.GONU*/ if (FF42_RDn) out.D2 = prev.SCY2;
  /*p23.GODO*/ if (FF42_RDn) out.D3 = prev.SCY3;
  /*p23.CUSA*/ if (FF42_RDn) out.D4 = prev.SCY4;
  /*p23.GYZO*/ if (FF42_RDn) out.D5 = prev.SCY5;
  /*p23.GUNE*/ if (FF42_RDn) out.D6 = prev.SCY6;
  /*p23.GYZA*/ if (FF42_RDn) out.D7 = prev.SCY7;

  /*p23.AVOG*/ wire FF43_RD = and(dec.FF43, in.CPU_RD2);
  /*p23.BEBA*/ wire FF43_RDn = not(FF43_RD);

  /*p23.EDOS*/ if (FF43_RDn) out.D0 = prev.SCX0;
  /*p23.EKOB*/ if (FF43_RDn) out.D1 = prev.SCX1;
  /*p23.CUGA*/ if (FF43_RDn) out.D2 = prev.SCX2;
  /*p23.WONY*/ if (FF43_RDn) out.D3 = prev.SCX3;
  /*p23.CEDU*/ if (FF43_RDn) out.D4 = prev.SCX4;
  /*p23.CATA*/ if (FF43_RDn) out.D5 = prev.SCX5;
  /*p23.DOXE*/ if (FF43_RDn) out.D6 = prev.SCX6;
  /*p23.CASY*/ if (FF43_RDn) out.D7 = prev.SCX7;

  /*p23.XYLY*/ wire FF45_RD = and(in.CPU_RD2, dec.FF45);
  /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);

  /*p23.RETU*/ if (FF45_RDn) out.D0 = prev.LYC0;
  /*p23.VOJO*/ if (FF45_RDn) out.D1 = prev.LYC1;
  /*p23.RAZU*/ if (FF45_RDn) out.D2 = prev.LYC2;
  /*p23.REDY*/ if (FF45_RDn) out.D3 = prev.LYC3;
  /*p23.RACE*/ if (FF45_RDn) out.D4 = prev.LYC4;
  /*p23.VAZU*/ if (FF45_RDn) out.D5 = prev.LYC5;
  /*p23.VAFE*/ if (FF45_RDn) out.D6 = prev.LYC6;
  /*p23.PUFY*/ if (FF45_RDn) out.D7 = prev.LYC7;

  /*p36.VUSO*/ wire FF47_RD = and(in.CPU_RD2, dec.FF47);
  /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);

  /*p36.RARO*/ if (FF47_RDn) out.D0 = prev.BGP_D0;
  /*p36.REDO*/ if (FF47_RDn) out.D2 = prev.BGP_D2;
  /*p36.LACE*/ if (FF47_RDn) out.D4 = prev.BGP_D4;
  /*p36.LODY*/ if (FF47_RDn) out.D6 = prev.BGP_D6;
  /*p36.PABA*/ if (FF47_RDn) out.D1 = prev.BGP_D1;
  /*p36.LOBE*/ if (FF47_RDn) out.D3 = prev.BGP_D3;
  /*p36.LYKA*/ if (FF47_RDn) out.D5 = prev.BGP_D5;
  /*p36.LARY*/ if (FF47_RDn) out.D7 = prev.BGP_D7;

  /*p36.XUFY*/ wire FF48_RD  = and(in.CPU_RD2, dec.FF48);
  /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);

  /*p36.XARY*/ if (FF48_RDn) out.D0 = prev.OBP0_D0;
  /*p36.XUNO*/ if (FF48_RDn) out.D2 = prev.OBP0_D2;
  /*p36.XAJU*/ if (FF48_RDn) out.D4 = prev.OBP0_D4;
  /*p36.XAXA*/ if (FF48_RDn) out.D6 = prev.OBP0_D6;
  /*p36.XOKE*/ if (FF48_RDn) out.D1 = prev.OBP0_D1;
  /*p36.XUBY*/ if (FF48_RDn) out.D3 = prev.OBP0_D3;
  /*p36.XOBO*/ if (FF48_RDn) out.D5 = prev.OBP0_D5;
  /*p36.XAWO*/ if (FF48_RDn) out.D7 = prev.OBP0_D7;

  /*p36.MUMY*/ wire FF49_RD  = and(in.CPU_RD2, dec.FF49);
  /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD);

  /*p36.LAJU*/ if (FF49_RDn) out.D0 = prev.OBP1_D0;
  /*p36.LODE*/ if (FF49_RDn) out.D2 = prev.OBP1_D2;
  /*p36.LUKY*/ if (FF49_RDn) out.D4 = prev.OBP1_D4;
  /*p36.LEBA*/ if (FF49_RDn) out.D6 = prev.OBP1_D6;
  /*p36.LEPA*/ if (FF49_RDn) out.D1 = prev.OBP1_D1;
  /*p36.LYZA*/ if (FF49_RDn) out.D3 = prev.OBP1_D3;
  /*p36.LUGA*/ if (FF49_RDn) out.D5 = prev.OBP1_D5;
  /*p36.LELU*/ if (FF49_RDn) out.D7 = prev.OBP1_D7;

  /*p23.WAXU*/ wire FF4A_RD = and(in.CPU_RD2, dec.FF4A);
  /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);

  /*p23.PUNU*/ if (FF4A_RDn) out.D0 = prev.WY0;
  /*p23.PODA*/ if (FF4A_RDn) out.D1 = prev.WY1;
  /*p23.PYGU*/ if (FF4A_RDn) out.D2 = prev.WY2;
  /*p23.LOKA*/ if (FF4A_RDn) out.D3 = prev.WY3;
  /*p23.MEGA*/ if (FF4A_RDn) out.D4 = prev.WY4;
  /*p23.PELA*/ if (FF4A_RDn) out.D5 = prev.WY5;
  /*p23.POLO*/ if (FF4A_RDn) out.D6 = prev.WY6;
  /*p23.MERA*/ if (FF4A_RDn) out.D7 = prev.WY7;

  /*p23.WYZE*/ wire FF4B_RD = and(in.CPU_RD2, dec.FF4B);
  /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

  /*p23.LOVA*/ if (FF4B_RDn) out.D0 = prev.WX0;
  /*p23.MUKA*/ if (FF4B_RDn) out.D1 = prev.WX1;
  /*p23.MOKO*/ if (FF4B_RDn) out.D2 = prev.WX2;
  /*p23.LOLE*/ if (FF4B_RDn) out.D3 = prev.WX3;
  /*p23.MELE*/ if (FF4B_RDn) out.D4 = prev.WX4;
  /*p23.MUFE*/ if (FF4B_RDn) out.D5 = prev.WX5;
  /*p23.MULY*/ if (FF4B_RDn) out.D6 = prev.WX6;
  /*p23.MARA*/ if (FF4B_RDn) out.D7 = prev.WX7;
}

//-----------------------------------------------------------------------------

void Registers_tock(const RegistersIn& in,
                    const ResetSignals& rst_sig,
                    const Decoder& dec,
                    Registers& next) {

  /*p23.WARU*/ wire FF40_WR = and(dec.FF40, in.CPU_WR2);
  /*p23.XUBO*/ wire FF40_WRn = not(FF40_WR);

  /*p23.VYXE*/ next.LCDC_BGEN    .tock(FF40_WRn, rst_sig.XARE_RESET, in.D0);
  /*p23.XYLO*/ next.LCDC_SPEN    .tock(FF40_WRn, rst_sig.XARE_RESET, in.D1);
  /*p23.XYMO*/ next.LCDC_SPSIZE  .tock(FF40_WRn, rst_sig.XARE_RESET, in.D2);
  /*p23.XAFO*/ next.LCDC_BGMAP   .tock(FF40_WRn, rst_sig.XARE_RESET, in.D3);
  /*p23.WEXU*/ next.LCDC_BGTILE  .tock(FF40_WRn, rst_sig.XARE_RESET, in.D4);
  /*p23.WYMO*/ next.LCDC_WINEN   .tock(FF40_WRn, rst_sig.XARE_RESET, in.D5);
  /*p23.WOKY*/ next.LCDC_WINMAP  .tock(FF40_WRn, rst_sig.XARE_RESET, in.D6);
  /*p23.XONA*/ next.LCDC_EN      .tock(FF40_WRn, rst_sig.XARE_RESET, in.D7);

  /*p23.BEDY*/ wire FF42_WR = and(dec.FF42, in.CPU_WR2);
  /*p23.CAVO*/ wire FF42_WRn = not(FF42_WR);

  /*p23.GAVE*/ next.SCY0.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D0);
  /*p23.FYMO*/ next.SCY1.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D1);
  /*p23.FEZU*/ next.SCY2.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D2);
  /*p23.FUJO*/ next.SCY3.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D3);
  /*p23.DEDE*/ next.SCY4.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D4);
  /*p23.FOTY*/ next.SCY5.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D5);
  /*p23.FOHA*/ next.SCY6.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D6);
  /*p23.FUNY*/ next.SCY7.tock(FF42_WRn, rst_sig.CUNU_RESETn, in.D7);

  /*p23.ARUR*/ wire FF43_WR = and(dec.FF43, in.CPU_WR2);
  /*p23.AMUN*/ wire FF43_WRn = not(FF43_WR);

  /*p23.DATY*/ next.SCX0.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D0);
  /*p23.DUZU*/ next.SCX1.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D1);
  /*p23.CYXU*/ next.SCX2.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D2);
  /*p23.GUBO*/ next.SCX3.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D3);
  /*p23.BEMY*/ next.SCX4.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D4);
  /*p23.CUZY*/ next.SCX5.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D5);
  /*p23.CABU*/ next.SCX6.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D6);
  /*p23.BAKE*/ next.SCX7.tock(FF43_WRn, rst_sig.CUNU_RESETn, in.D7);

  /*p23.XUFA*/ wire FF45_WR = and(in.CPU_WR2, dec.FF45);
  /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

  /*p23.SYRY*/ next.LYC0.tock(FF45_WRn, rst_sig.WESY_RESET, in.D0);
  /*p23.VUCE*/ next.LYC1.tock(FF45_WRn, rst_sig.WESY_RESET, in.D1);
  /*p23.SEDY*/ next.LYC2.tock(FF45_WRn, rst_sig.WESY_RESET, in.D2);
  /*p23.SALO*/ next.LYC3.tock(FF45_WRn, rst_sig.WESY_RESET, in.D3);
  /*p23.SOTA*/ next.LYC4.tock(FF45_WRn, rst_sig.WESY_RESET, in.D4);
  /*p23.VAFA*/ next.LYC5.tock(FF45_WRn, rst_sig.WESY_RESET, in.D5);
  /*p23.VEVO*/ next.LYC6.tock(FF45_WRn, rst_sig.WESY_RESET, in.D6);
  /*p23.RAHA*/ next.LYC7.tock(FF45_WRn, rst_sig.WESY_RESET, in.D7);

  /*p36.VELY*/ wire FF47_WR = and(in.CPU_WR2, dec.FF47);
  /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

  /*p36.PAVO*/ next.BGP_D0.tock(FF47_WRn, 1, in.D0);
  /*p36.PYLU*/ next.BGP_D2.tock(FF47_WRn, 1, in.D2);
  /*p36.MUKE*/ next.BGP_D4.tock(FF47_WRn, 1, in.D4);
  /*p36.MOGY*/ next.BGP_D6.tock(FF47_WRn, 1, in.D6);
  /*p36.NUSY*/ next.BGP_D1.tock(FF47_WRn, 1, in.D1);
  /*p36.MAXY*/ next.BGP_D3.tock(FF47_WRn, 1, in.D3);
  /*p36.MORU*/ next.BGP_D5.tock(FF47_WRn, 1, in.D5);
  /*p36.MENA*/ next.BGP_D7.tock(FF47_WRn, 1, in.D7);

  /*p36.XOMA*/ wire FF48_WR  = and(in.CPU_WR2, dec.FF48);
  /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

  /*p36.XUFU*/ next.OBP0_D0.tock(FF48_WRn, 1, in.D0);
  /*p36.XOVA*/ next.OBP0_D2.tock(FF48_WRn, 1, in.D2);
  /*p36.XERU*/ next.OBP0_D4.tock(FF48_WRn, 1, in.D4);
  /*p36.XUPO*/ next.OBP0_D6.tock(FF48_WRn, 1, in.D6);
  /*p36.XUKY*/ next.OBP0_D1.tock(FF48_WRn, 1, in.D1);
  /*p36.XALO*/ next.OBP0_D3.tock(FF48_WRn, 1, in.D3);
  /*p36.XYZE*/ next.OBP0_D5.tock(FF48_WRn, 1, in.D5);
  /*p36.XANA*/ next.OBP0_D7.tock(FF48_WRn, 1, in.D7);

  /*p36.MYXE*/ wire FF49_WR  = and(in.CPU_WR2, dec.FF49);
  /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

  /*p36.MOXY*/ next.OBP1_D0.tock(FF49_WRn, 1, in.D0);
  /*p36.MOSA*/ next.OBP1_D2.tock(FF49_WRn, 1, in.D2);
  /*p36.LUNE*/ next.OBP1_D4.tock(FF49_WRn, 1, in.D4);
  /*p36.LEPU*/ next.OBP1_D6.tock(FF49_WRn, 1, in.D6);
  /*p36.LAWO*/ next.OBP1_D1.tock(FF49_WRn, 1, in.D1);
  /*p36.LOSE*/ next.OBP1_D3.tock(FF49_WRn, 1, in.D3);
  /*p36.LUGU*/ next.OBP1_D5.tock(FF49_WRn, 1, in.D5);
  /*p36.LUXO*/ next.OBP1_D7.tock(FF49_WRn, 1, in.D7);

  /*p23.WEKO*/ wire FF4A_WR = and(in.CPU_WR2, dec.FF4A);
  /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

  /*p23.NESO*/ next.WY0.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D0);
  /*p23.NYRO*/ next.WY1.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D1);
  /*p23.NAGA*/ next.WY2.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D2);
  /*p23.MELA*/ next.WY3.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D3);
  /*p23.NULO*/ next.WY4.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D4);
  /*p23.NENE*/ next.WY5.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D5);
  /*p23.NUKA*/ next.WY6.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D6);
  /*p23.NAFU*/ next.WY7.tock(FF4A_WRn, rst_sig.WALU_RESET, in.D7);

  /*p23.WUZA*/ wire FF4B_WR = and(in.CPU_WR2, dec.FF4B);
  /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

  /*p23.MYPA*/ next.WX0.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D0);
  /*p23.NOFE*/ next.WX1.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D1);
  /*p23.NOKE*/ next.WX2.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D2);
  /*p23.MEBY*/ next.WX3.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D3);
  /*p23.MYPU*/ next.WX4.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D4);
  /*p23.MYCE*/ next.WX5.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D5); 
  /*p23.MUVO*/ next.WX6.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D6);
  /*p23.NUKU*/ next.WX7.tock(FF4B_WRn, rst_sig.WALU_RESET, in.D7);
}

//-----------------------------------------------------------------------------

};