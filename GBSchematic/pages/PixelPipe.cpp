#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P32_BgPixelShifter_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // BGP

  /*p36.VUSO*/ c.p36.FF47_RD  = and(b.sys.CPU_RD2, b.p22.FF47); // polarity?
  /*p36.VELY*/ c.p36.FF47_WR  = and(b.sys.CPU_WR2, b.p22.FF47);
  /*p36.TEPY*/ c.p36.FF47_RDn = not(b.p36.FF47_RD);
  /*p36.TEPO*/ c.p36.FF47_WRn = not(b.p36.FF47_WR);

  /*p36.MENA*/ c.p36.BGP_D7 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D7, b.D7);
  /*p36.MORU*/ c.p36.BGP_D5 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D5, b.D5);
  /*p36.MAXY*/ c.p36.BGP_D3 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D3, b.D3);
  /*p36.NUSY*/ c.p36.BGP_D1 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D1, b.D1);
  /*p36.MOGY*/ c.p36.BGP_D6 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D6, b.D6);
  /*p36.MUKE*/ c.p36.BGP_D4 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D4, b.D4);
  /*p36.PYLU*/ c.p36.BGP_D2 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D2, b.D2);
  /*p36.PAVO*/ c.p36.BGP_D0 = tock_pos(a.p36.FF47_WRn, b.p36.FF47_WRn, 0, b.p36.BGP_D0, b.D0);

  /*p36.LARY*/ if (b.p36.FF47_RDn) c.D7 = b.p36.BGP_D7;
  /*p36.LYKA*/ if (b.p36.FF47_RDn) c.D5 = b.p36.BGP_D5;
  /*p36.LOBE*/ if (b.p36.FF47_RDn) c.D3 = b.p36.BGP_D3;
  /*p36.PABA*/ if (b.p36.FF47_RDn) c.D1 = b.p36.BGP_D1;
  /*p36.LODY*/ if (b.p36.FF47_RDn) c.D6 = b.p36.BGP_D6;
  /*p36.LACE*/ if (b.p36.FF47_RDn) c.D4 = b.p36.BGP_D4;
  /*p36.REDO*/ if (b.p36.FF47_RDn) c.D2 = b.p36.BGP_D2;
  /*p36.RARO*/ if (b.p36.FF47_RDn) c.D0 = b.p36.BGP_D0;

  //----------
  // OBP0

  /*p36.XUFY*/ c.p36.FF48_RD  = and(b.sys.CPU_RD2, b.p22.FF48);
  /*p36.XOMA*/ c.p36.FF48_WR  = and(b.sys.CPU_WR2, b.p22.FF48);
  /*p36.XOZY*/ c.p36.FF48_RDn = not(b.p36.FF48_RD);
  /*p36.XELO*/ c.p36.FF48_WRn = not(b.p36.FF48_WR);

  /*p36.XANA*/ c.p36.OBP0_D7 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D7, b.D7);
  /*p36.XYZE*/ c.p36.OBP0_D5 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D5, b.D5);
  /*p36.XALO*/ c.p36.OBP0_D3 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D3, b.D3);
  /*p36.XUKY*/ c.p36.OBP0_D1 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D1, b.D1);
  /*p36.XUPO*/ c.p36.OBP0_D6 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D6, b.D6);
  /*p36.XERU*/ c.p36.OBP0_D4 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D4, b.D4);
  /*p36.XOVA*/ c.p36.OBP0_D2 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D2, b.D2);
  /*p36.XUFU*/ c.p36.OBP0_D0 = tock_pos(a.p36.FF48_WRn, b.p36.FF48_WRn, 0, b.p36.OBP0_D0, b.D0);

  /*p36.XAWO*/ if (b.p36.FF48_RDn) c.D7 = b.p36.OBP0_D7;
  /*p36.XOBO*/ if (b.p36.FF48_RDn) c.D5 = b.p36.OBP0_D5;
  /*p36.XUBY*/ if (b.p36.FF48_RDn) c.D3 = b.p36.OBP0_D3;
  /*p36.XOKE*/ if (b.p36.FF48_RDn) c.D1 = b.p36.OBP0_D1;
  /*p36.XAXA*/ if (b.p36.FF48_RDn) c.D6 = b.p36.OBP0_D6;
  /*p36.XAJU*/ if (b.p36.FF48_RDn) c.D4 = b.p36.OBP0_D4;
  /*p36.XUNO*/ if (b.p36.FF48_RDn) c.D2 = b.p36.OBP0_D2;
  /*p36.XARY*/ if (b.p36.FF48_RDn) c.D0 = b.p36.OBP0_D0;

  //----------
  // OBP1

  /*p36.MUMY*/ c.p36.FF49_RD  = and(b.sys.CPU_RD2, b.p22.FF49);
  /*p36.MYXE*/ c.p36.FF49_WR  = and(b.sys.CPU_WR2, b.p22.FF49);
  /*p36.LOTE*/ c.p36.FF49_RDn = not(b.p36.FF49_RD);
  /*p36.LEHO*/ c.p36.FF49_WRn = not(b.p36.FF49_WR);

  /*p36.LUXO*/ c.p36.OBP1_D7 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D7, b.D7);
  /*p36.LUGU*/ c.p36.OBP1_D5 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D5, b.D5);
  /*p36.LOSE*/ c.p36.OBP1_D3 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D3, b.D3);
  /*p36.LAWO*/ c.p36.OBP1_D1 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D1, b.D1);
  /*p36.LEPU*/ c.p36.OBP1_D6 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D6, b.D6);
  /*p36.LUNE*/ c.p36.OBP1_D4 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D4, b.D4);
  /*p36.MOSA*/ c.p36.OBP1_D2 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D2, b.D2);
  /*p36.MOXY*/ c.p36.OBP1_D0 = tock_pos(a.p36.FF49_WRn, b.p36.FF49_WRn, 0, b.p36.OBP1_D0, b.D0);

  /*p36.LELU*/ if (b.p36.FF49_RDn) c.D7 = b.p36.OBP1_D7;
  /*p36.LUGA*/ if (b.p36.FF49_RDn) c.D5 = b.p36.OBP1_D5;
  /*p36.LYZA*/ if (b.p36.FF49_RDn) c.D3 = b.p36.OBP1_D3;
  /*p36.LEPA*/ if (b.p36.FF49_RDn) c.D1 = b.p36.OBP1_D1;
  /*p36.LEBA*/ if (b.p36.FF49_RDn) c.D6 = b.p36.OBP1_D6;
  /*p36.LUKY*/ if (b.p36.FF49_RDn) c.D4 = b.p36.OBP1_D4;
  /*p36.LODE*/ if (b.p36.FF49_RDn) c.D2 = b.p36.OBP1_D2;
  /*p36.LAJU*/ if (b.p36.FF49_RDn) c.D0 = b.p36.OBP1_D0;

  //----------
  // P32

  /*p32.LESO*/ c.p32.LESO = not(b.p27.MOFU);
  /*p32.AJAR*/ c.p32.AJAR = not(b.p32.LESO);
  /*p32.LABU*/ c.p32.LABU = not(b.p32.AJAR);

  /*p32.RAWU*/ c.p32.RAWU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.RAWU, b.MD0);
  /*p32.POZO*/ c.p32.POZO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POZO, b.MD1);
  /*p32.PYZO*/ c.p32.PYZO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PYZO, b.MD2);
  /*p32.POXA*/ c.p32.POXA = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POXA, b.MD3);
  /*p32.PULO*/ c.p32.PULO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PULO, b.MD4);
  /*p32.POJU*/ c.p32.POJU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POJU, b.MD5);
  /*p32.POWY*/ c.p32.POWY = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POWY, b.MD6);
  /*p32.PYJU*/ c.p32.PYJU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PYJU, b.MD7);

  /*p32.METE*/ c.p32.METE = not(b.p27.NYDY);
  /*p32.LOMA*/ c.p32.LOMA = not(b.p32.METE);

  /*p32.LEGU*/ c.p32.LEGU = latch_pos(b.p32.LOMA, b.p32.LEGU, b.MD0);
  /*p32.NUDU*/ c.p32.NUDU = latch_pos(b.p32.LOMA, b.p32.NUDU, b.MD1);
  /*p32.MUKU*/ c.p32.MUKU = latch_pos(b.p32.LOMA, b.p32.MUKU, b.MD2);
  /*p32.LUZO*/ c.p32.LUZO = latch_pos(b.p32.LOMA, b.p32.LUZO, b.MD3);
  /*p32.MEGU*/ c.p32.MEGU = latch_pos(b.p32.LOMA, b.p32.MEGU, b.MD4);
  /*p32.MYJY*/ c.p32.MYJY = latch_pos(b.p32.LOMA, b.p32.MYJY, b.MD5);
  /*p32.NASA*/ c.p32.NASA = latch_pos(b.p32.LOMA, b.p32.NASA, b.MD6);
  /*p32.NEFO*/ c.p32.NEFO = latch_pos(b.p32.LOMA, b.p32.NEFO, b.MD7);

  /*p32.TOSA*/ c.p32.TOSA = not(!b.p32.RAWU);
  /*p32.RUCO*/ c.p32.RUCO = not(!b.p32.POZO);
  /*p32.TYCE*/ c.p32.TYCE = not(!b.p32.PYZO);
  /*p32.REVY*/ c.p32.REVY = not(!b.p32.POXA);
  /*p32.RYGA*/ c.p32.RYGA = not(!b.p32.PULO);
  /*p32.RYLE*/ c.p32.RYLE = not(!b.p32.POJU);
  /*p32.RAPU*/ c.p32.RAPU = not(!b.p32.POWY);
  /*p32.SOJA*/ c.p32.SOJA = not(!b.p32.PYJU);

  /*p32.LUHE*/ c.p32.LUHE = not(b.p32.LEGU);
  /*p32.NOLY*/ c.p32.NOLY = not(b.p32.NUDU);
  /*p32.LEKE*/ c.p32.LEKE = not(b.p32.MUKU);
  /*p32.LOMY*/ c.p32.LOMY = not(b.p32.LUZO);
  /*p32.LALA*/ c.p32.LALA = not(b.p32.MEGU);
  /*p32.LOXA*/ c.p32.LOXA = not(b.p32.MYJY);
  /*p32.NEZE*/ c.p32.NEZE = not(b.p32.NASA);
  /*p32.NOBO*/ c.p32.NOBO = not(b.p32.NEFO);

  /*p32.LUXA*/ c.p32.LUXA = not(b.p27.NYXU);
  /*p32.SEJA*/ c.p32.SEJA = nand(b.p32.TOSA, b.p32.LUXA);
  /*p32.SENO*/ c.p32.SENO = nand(b.p32.RUCO, b.p32.LUXA);
  /*p32.SURE*/ c.p32.SURE = nand(b.p32.TYCE, b.p32.LUXA);
  /*p32.SEBO*/ c.p32.SEBO = nand(b.p32.REVY, b.p32.LUXA);
  /*p32.SUCA*/ c.p32.SUCA = nand(b.p32.RYGA, b.p32.LUXA);
  /*p32.SYWE*/ c.p32.SYWE = nand(b.p32.RYLE, b.p32.LUXA);
  /*p32.SUPU*/ c.p32.SUPU = nand(b.p32.RAPU, b.p32.LUXA);
  /*p32.RYJY*/ c.p32.RYJY = nand(b.p32.SOJA, b.p32.LUXA);
  /*p32.TUXE*/ c.p32.TUXE = nand(b.p32.LUXA, !b.p32.RAWU);
  /*p32.SOLY*/ c.p32.SOLY = nand(b.p32.LUXA, !b.p32.POZO);
  /*p32.RUCE*/ c.p32.RUCE = nand(b.p32.LUXA, !b.p32.PYZO);
  /*p32.RYJA*/ c.p32.RYJA = nand(b.p32.LUXA, !b.p32.POXA);
  /*p32.RUTO*/ c.p32.RUTO = nand(b.p32.LUXA, !b.p32.PULO);
  /*p32.RAJA*/ c.p32.RAJA = nand(b.p32.LUXA, !b.p32.POJU);
  /*p32.RAJO*/ c.p32.RAJO = nand(b.p32.LUXA, !b.p32.POWY);
  /*p32.RAGA*/ c.p32.RAGA = nand(b.p32.LUXA, !b.p32.PYJU);

  /*p32.LOZE*/ c.p32.LOZE = not(b.p27.NYXU);
  /*p32.LOTY*/ c.p32.LOTY = nand(b.p32.LUHE, b.p32.LOZE);
  /*p32.NEXA*/ c.p32.NEXA = nand(b.p32.NOLY, b.p32.LOZE);
  /*p32.LUTU*/ c.p32.LUTU = nand(b.p32.LEKE, b.p32.LOZE);
  /*p32.LUJA*/ c.p32.LUJA = nand(b.p32.LOMY, b.p32.LOZE);
  /*p32.MOSY*/ c.p32.MOSY = nand(b.p32.LALA, b.p32.LOZE);
  /*p32.LERU*/ c.p32.LERU = nand(b.p32.LOXA, b.p32.LOZE);
  /*p32.NYHA*/ c.p32.NYHA = nand(b.p32.NEZE, b.p32.LOZE);
  /*p32.NADY*/ c.p32.NADY = nand(b.p32.NOBO, b.p32.LOZE);
  /*p32.LAKY*/ c.p32.LAKY = nand(b.p32.LOZE, b.p32.LEGU);
  /*p32.NYXO*/ c.p32.NYXO = nand(b.p32.LOZE, b.p32.NUDU);
  /*p32.LOTO*/ c.p32.LOTO = nand(b.p32.LOZE, b.p32.MUKU);
  /*p32.LYDU*/ c.p32.LYDU = nand(b.p32.LOZE, b.p32.LUZO);
  /*p32.MYVY*/ c.p32.MYVY = nand(b.p32.LOZE, b.p32.MEGU);
  /*p32.LODO*/ c.p32.LODO = nand(b.p32.LOZE, b.p32.MYJY);
  /*p32.NUTE*/ c.p32.NUTE = nand(b.p32.LOZE, b.p32.NASA);
  /*p32.NAJA*/ c.p32.NAJA = nand(b.p32.LOZE, b.p32.NEFO);

  /*p32.TOMY*/ c.p32.TOMY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.TUXE, b.p32.SEJA, b.p32.TOMY, b.chip.P10_B);
  /*p32.TACA*/ c.p32.TACA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.SOLY, b.p32.SENO, b.p32.TACA, b.p32.TOMY);
  /*p32.SADY*/ c.p32.SADY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RUCE, b.p32.SURE, b.p32.SADY, b.p32.TACA);
  /*p32.RYSA*/ c.p32.RYSA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RYJA, b.p32.SEBO, b.p32.RYSA, b.p32.SADY);
  /*p32.SOBO*/ c.p32.SOBO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RUTO, b.p32.SUCA, b.p32.SOBO, b.p32.RYSA);
  /*p32.SETU*/ c.p32.SETU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAJA, b.p32.SYWE, b.p32.SETU, b.p32.SOBO);
  /*p32.RALU*/ c.p32.RALU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAJO, b.p32.SUPU, b.p32.RALU, b.p32.SETU);
  /*p32.SOHU*/ c.p32.BG_PIX_B_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAGA, b.p32.RYJY, b.p32.BG_PIX_B_7, b.p32.RALU);

  /*p32.MYDE*/ c.p32.MYDE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LAKY, b.p32.LOTY, b.p32.MYDE, b.chip.P10_B);
  /*p32.NOZO*/ c.p32.NOZO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NYXO, b.p32.NEXA, b.p32.NOZO, b.p32.MYDE);
  /*p32.MOJU*/ c.p32.MOJU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LOTO, b.p32.LUTU, b.p32.MOJU, b.p32.NOZO);
  /*p32.MACU*/ c.p32.MACU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LYDU, b.p32.LUJA, b.p32.MACU, b.p32.MOJU);
  /*p32.NEPO*/ c.p32.NEPO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.MYVY, b.p32.MOSY, b.p32.NEPO, b.p32.MACU);
  /*p32.MODU*/ c.p32.MODU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LODO, b.p32.LERU, b.p32.MODU, b.p32.NEPO);
  /*p32.NEDA*/ c.p32.NEDA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NUTE, b.p32.NYHA, b.p32.NEDA, b.p32.MODU);
  /*p32.PYBO*/ c.p32.BG_PIX_A_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NAJA, b.p32.NADY, b.p32.BG_PIX_A_7, b.p32.NEDA);

  //----------
  // Sprite x flip

  /*p29.XONO*/ c.p29.FLIP_X = and(!b.p31.OAM_A_D5, b.p29.TEXY);
  /*p33.PUTE*/ c.p33.SPR_PIX_FLIP0 = mux2(b.MD7, b.MD0, b.p29.FLIP_X);
  /*p33.PELO*/ c.p33.SPR_PIX_FLIP1 = mux2(b.MD6, b.MD1, b.p29.FLIP_X);
  /*p33.PONO*/ c.p33.SPR_PIX_FLIP2 = mux2(b.MD5, b.MD2, b.p29.FLIP_X);
  /*p33.POBE*/ c.p33.SPR_PIX_FLIP3 = mux2(b.MD4, b.MD3, b.p29.FLIP_X);
  /*p33.PACY*/ c.p33.SPR_PIX_FLIP4 = mux2(b.MD3, b.MD4, b.p29.FLIP_X);
  /*p33.PUGU*/ c.p33.SPR_PIX_FLIP5 = mux2(b.MD2, b.MD5, b.p29.FLIP_X);
  /*p33.PAWE*/ c.p33.SPR_PIX_FLIP6 = mux2(b.MD1, b.MD6, b.p29.FLIP_X);
  /*p33.PULY*/ c.p33.SPR_PIX_FLIP7 = mux2(b.MD0, b.MD7, b.p29.FLIP_X);

  //----------
  // Sprite pipe B

  // Sprite latch might be wrong

  /*p33.REWO*/ c.p33.SPR_PIX_B0     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B0, b.p33.SPR_PIX_FLIP0);
  /*p33.PEBA*/ c.p33.SPR_PIX_B1     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B1, b.p33.SPR_PIX_FLIP1);
  /*p33.MOFO*/ c.p33.SPR_PIX_B2     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B2, b.p33.SPR_PIX_FLIP2);
  /*p33.PUDU*/ c.p33.SPR_PIX_B3     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B3, b.p33.SPR_PIX_FLIP3);
  /*p33.SAJA*/ c.p33.SPR_PIX_B4     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B4, b.p33.SPR_PIX_FLIP4);
  /*p33.SUNY*/ c.p33.SPR_PIX_B5     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B5, b.p33.SPR_PIX_FLIP5);
  /*p33.SEMO*/ c.p33.SPR_PIX_B6     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B6, b.p33.SPR_PIX_FLIP6);
  /*p33.SEGA*/ c.p33.SPR_PIX_B7     = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SPR_PIX_B7, b.p33.SPR_PIX_FLIP7);

  /*p33.RATA*/ c.p33.SPR_PIX_B0n    = not(b.p33.SPR_PIX_B0);
  /*p33.NUCA*/ c.p33.SPR_PIX_B1n    = not(b.p33.SPR_PIX_B1);
  /*p33.LASE*/ c.p33.SPR_PIX_B2n    = not(b.p33.SPR_PIX_B2);
  /*p33.LUBO*/ c.p33.SPR_PIX_B3n    = not(b.p33.SPR_PIX_B3);
  /*p33.WERY*/ c.p33.SPR_PIX_B4n    = not(b.p33.SPR_PIX_B4);
  /*p33.WURA*/ c.p33.SPR_PIX_B5n    = not(b.p33.SPR_PIX_B5);
  /*p33.SULU*/ c.p33.SPR_PIX_B6n    = not(b.p33.SPR_PIX_B6);
  /*p33.WAMY*/ c.p33.SPR_PIX_B7n    = not(b.p33.SPR_PIX_B7);

  /*p33.PABE*/ c.p33.SPR_PIX_B_SET0 = nand(b.p33.SPR_PIX_B0, b.p34.SPRITE_MASK0);
  /*p33.MYTO*/ c.p33.SPR_PIX_B_SET1 = nand(b.p33.SPR_PIX_B1, b.p34.SPRITE_MASK1);
  /*p33.LELA*/ c.p33.SPR_PIX_B_SET2 = nand(b.p33.SPR_PIX_B2, b.p34.SPRITE_MASK2);
  /*p33.MAME*/ c.p33.SPR_PIX_B_SET3 = nand(b.p33.SPR_PIX_B3, b.p34.SPRITE_MASK3);
  /*p33.VEXU*/ c.p33.SPR_PIX_B_SET4 = nand(b.p33.SPR_PIX_B4, b.p34.SPRITE_MASK4);
  /*p33.VABY*/ c.p33.SPR_PIX_B_SET5 = nand(b.p33.SPR_PIX_B5, b.p34.SPRITE_MASK5);
  /*p33.TUXA*/ c.p33.SPR_PIX_B_SET6 = nand(b.p33.SPR_PIX_B6, b.p34.SPRITE_MASK6);
  /*p33.VUNE*/ c.p33.SPR_PIX_B_SET7 = nand(b.p33.SPR_PIX_B7, b.p34.SPRITE_MASK7);

  /*p33.PYZU*/ c.p33.SPR_PIX_B_RST0 = nand(b.p33.SPR_PIX_B0n, b.p34.SPRITE_MASK0);
  /*p33.MADA*/ c.p33.SPR_PIX_B_RST1 = nand(b.p33.SPR_PIX_B1n, b.p34.SPRITE_MASK1);
  /*p33.LYDE*/ c.p33.SPR_PIX_B_RST2 = nand(b.p33.SPR_PIX_B2n, b.p34.SPRITE_MASK2);
  /*p33.LUFY*/ c.p33.SPR_PIX_B_RST3 = nand(b.p33.SPR_PIX_B3n, b.p34.SPRITE_MASK3);
  /*p33.XATO*/ c.p33.SPR_PIX_B_RST4 = nand(b.p33.SPR_PIX_B4n, b.p34.SPRITE_MASK4);
  /*p33.XEXU*/ c.p33.SPR_PIX_B_RST5 = nand(b.p33.SPR_PIX_B5n, b.p34.SPRITE_MASK5);
  /*p33.TUPE*/ c.p33.SPR_PIX_B_RST6 = nand(b.p33.SPR_PIX_B6n, b.p34.SPRITE_MASK6);
  /*p33.XYVE*/ c.p33.SPR_PIX_B_RST7 = nand(b.p33.SPR_PIX_B7n, b.p34.SPRITE_MASK7);

  /*p33.NURO*/ c.p33.SPR_PIX_B_0    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET0, b.p33.SPR_PIX_B_RST0, b.p33.SPR_PIX_B_0, b.chip.P10_B);
  /*p33.MASO*/ c.p33.SPR_PIX_B_1    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET1, b.p33.SPR_PIX_B_RST1, b.p33.SPR_PIX_B_1, b.p33.SPR_PIX_B_0);
  /*p33.LEFE*/ c.p33.SPR_PIX_B_2    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET2, b.p33.SPR_PIX_B_RST2, b.p33.SPR_PIX_B_2, b.p33.SPR_PIX_B_1);
  /*p33.LESU*/ c.p33.SPR_PIX_B_3    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET3, b.p33.SPR_PIX_B_RST3, b.p33.SPR_PIX_B_3, b.p33.SPR_PIX_B_2);
  /*p33.WYHO*/ c.p33.SPR_PIX_B_4    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET4, b.p33.SPR_PIX_B_RST4, b.p33.SPR_PIX_B_4, b.p33.SPR_PIX_B_3);
  /*p33.WORA*/ c.p33.SPR_PIX_B_5    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET5, b.p33.SPR_PIX_B_RST5, b.p33.SPR_PIX_B_5, b.p33.SPR_PIX_B_4);
  /*p33.VAFO*/ c.p33.SPR_PIX_B_6    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET6, b.p33.SPR_PIX_B_RST6, b.p33.SPR_PIX_B_6, b.p33.SPR_PIX_B_5);
  /*p33.WUFY*/ c.p33.SPR_PIX_B_7    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_B_SET7, b.p33.SPR_PIX_B_RST7, b.p33.SPR_PIX_B_7, b.p33.SPR_PIX_B_6);

  //----------
  // Sprite pipe A

  // Sprite latch might be wrong

  /*p33.PEFO*/ c.p33.SPR_PIX_A0 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A0, b.p33.SPR_PIX_FLIP0);
  /*p33.ROKA*/ c.p33.SPR_PIX_A1 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A1, b.p33.SPR_PIX_FLIP1);
  /*p33.MYTU*/ c.p33.SPR_PIX_A2 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A2, b.p33.SPR_PIX_FLIP2);
  /*p33.RAMU*/ c.p33.SPR_PIX_A3 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A3, b.p33.SPR_PIX_FLIP3);
  /*p33.SELE*/ c.p33.SPR_PIX_A4 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A4, b.p33.SPR_PIX_FLIP4);
  /*p33.SUTO*/ c.p33.SPR_PIX_A5 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A5, b.p33.SPR_PIX_FLIP5);
  /*p33.RAMA*/ c.p33.SPR_PIX_A6 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A6, b.p33.SPR_PIX_FLIP6);
  /*p33.RYDU*/ c.p33.SPR_PIX_A7 = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SPR_PIX_A7, b.p33.SPR_PIX_FLIP7);

  /*p33.LOZA*/ c.p33.SPR_PIX_A0n = not(b.p33.SPR_PIX_A0);
  /*p33.SYBO*/ c.p33.SPR_PIX_A1n = not(b.p33.SPR_PIX_A1);
  /*p33.LUMO*/ c.p33.SPR_PIX_A2n = not(b.p33.SPR_PIX_A2);
  /*p33.SOLO*/ c.p33.SPR_PIX_A3n = not(b.p33.SPR_PIX_A3);
  /*p33.VOBY*/ c.p33.SPR_PIX_A4n = not(b.p33.SPR_PIX_A4);
  /*p33.WYCO*/ c.p33.SPR_PIX_A5n = not(b.p33.SPR_PIX_A5);
  /*p33.SERY*/ c.p33.SPR_PIX_A6n = not(b.p33.SPR_PIX_A6);
  /*p33.SELU*/ c.p33.SPR_PIX_A7n = not(b.p33.SPR_PIX_A7);

  /*p33.MEZU*/ c.p33.SPR_PIX_A_SET0 = nand(b.p33.SPR_PIX_A0, b.p34.SPRITE_MASK0);
  /*p33.RUSY*/ c.p33.SPR_PIX_A_SET1 = nand(b.p33.SPR_PIX_A1, b.p34.SPRITE_MASK1);
  /*p33.MYXA*/ c.p33.SPR_PIX_A_SET2 = nand(b.p33.SPR_PIX_A2, b.p34.SPRITE_MASK2);
  /*p33.RANO*/ c.p33.SPR_PIX_A_SET3 = nand(b.p33.SPR_PIX_A3, b.p34.SPRITE_MASK3);
  /*p33.TYGA*/ c.p33.SPR_PIX_A_SET4 = nand(b.p33.SPR_PIX_A4, b.p34.SPRITE_MASK4);
  /*p33.VUME*/ c.p33.SPR_PIX_A_SET5 = nand(b.p33.SPR_PIX_A5, b.p34.SPRITE_MASK5);
  /*p33.TAPO*/ c.p33.SPR_PIX_A_SET6 = nand(b.p33.SPR_PIX_A6, b.p34.SPRITE_MASK6);
  /*p33.TESO*/ c.p33.SPR_PIX_A_SET7 = nand(b.p33.SPR_PIX_A7, b.p34.SPRITE_MASK7);

  /*p33.MOFY*/ c.p33.SPR_PIX_A_RST0 = nand(b.p33.SPR_PIX_A0n, b.p34.SPRITE_MASK0);
  /*p33.RUCA*/ c.p33.SPR_PIX_A_RST1 = nand(b.p33.SPR_PIX_A1n, b.p34.SPRITE_MASK1);
  /*p33.MAJO*/ c.p33.SPR_PIX_A_RST2 = nand(b.p33.SPR_PIX_A2n, b.p34.SPRITE_MASK2);
  /*p33.REHU*/ c.p33.SPR_PIX_A_RST3 = nand(b.p33.SPR_PIX_A3n, b.p34.SPRITE_MASK3);
  /*p33.WAXO*/ c.p33.SPR_PIX_A_RST4 = nand(b.p33.SPR_PIX_A4n, b.p34.SPRITE_MASK4);
  /*p33.XOLE*/ c.p33.SPR_PIX_A_RST5 = nand(b.p33.SPR_PIX_A5n, b.p34.SPRITE_MASK5);
  /*p33.TABY*/ c.p33.SPR_PIX_A_RST6 = nand(b.p33.SPR_PIX_A6n, b.p34.SPRITE_MASK6);
  /*p33.TULA*/ c.p33.SPR_PIX_A_RST7 = nand(b.p33.SPR_PIX_A7n, b.p34.SPRITE_MASK7);

  /*p33.NYLU*/ c.p33.SPR_PIX_A_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET0, b.p33.SPR_PIX_A_RST0, b.p33.SPR_PIX_A_0, b.chip.P10_B);
  /*p33.PEFU*/ c.p33.SPR_PIX_A_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET1, b.p33.SPR_PIX_A_RST1, b.p33.SPR_PIX_A_1, b.p33.SPR_PIX_A_0);
  /*p33.NATY*/ c.p33.SPR_PIX_A_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET2, b.p33.SPR_PIX_A_RST2, b.p33.SPR_PIX_A_2, b.p33.SPR_PIX_A_1);
  /*p33.PYJO*/ c.p33.SPR_PIX_A_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET3, b.p33.SPR_PIX_A_RST3, b.p33.SPR_PIX_A_3, b.p33.SPR_PIX_A_2);
  /*p33.VARE*/ c.p33.SPR_PIX_A_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET4, b.p33.SPR_PIX_A_RST4, b.p33.SPR_PIX_A_4, b.p33.SPR_PIX_A_3);
  /*p33.WEBA*/ c.p33.SPR_PIX_A_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET5, b.p33.SPR_PIX_A_RST5, b.p33.SPR_PIX_A_5, b.p33.SPR_PIX_A_4);
  /*p33.VANU*/ c.p33.SPR_PIX_A_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET6, b.p33.SPR_PIX_A_RST6, b.p33.SPR_PIX_A_6, b.p33.SPR_PIX_A_5);
  /*p33.VUPY*/ c.p33.SPR_PIX_A_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.SPR_PIX_A_SET7, b.p33.SPR_PIX_A_RST7, b.p33.SPR_PIX_A_7, b.p33.SPR_PIX_A_6);

  //----------
  // P34

  /*p34.MEFU*/ c.p34.SPRITE_MASK0 = or(b.p29.XEFY, b.p33.SPR_PIX_A_0, b.p33.SPR_PIX_B_0);
  /*p34.MEVE*/ c.p34.SPRITE_MASK1 = or(b.p29.XEFY, b.p33.SPR_PIX_A_1, b.p33.SPR_PIX_B_1);
  /*p34.MYZO*/ c.p34.SPRITE_MASK2 = or(b.p29.XEFY, b.p33.SPR_PIX_A_2, b.p33.SPR_PIX_B_2);
  /*p34.RUDA*/ c.p34.SPRITE_MASK3 = or(b.p29.XEFY, b.p33.SPR_PIX_A_3, b.p33.SPR_PIX_B_3);
  /*p34.VOTO*/ c.p34.SPRITE_MASK4 = or(b.p29.XEFY, b.p33.SPR_PIX_A_4, b.p33.SPR_PIX_B_4);
  /*p34.VYSA*/ c.p34.SPRITE_MASK5 = or(b.p29.XEFY, b.p33.SPR_PIX_A_5, b.p33.SPR_PIX_B_5);
  /*p34.TORY*/ c.p34.SPRITE_MASK6 = or(b.p29.XEFY, b.p33.SPR_PIX_A_6, b.p33.SPR_PIX_B_6);
  /*p34.WOPE*/ c.p34.SPRITE_MASK7 = or(b.p29.XEFY, b.p33.SPR_PIX_A_7, b.p33.SPR_PIX_B_7);

  /*p34.LESY*/ c.p34.SPRITE_MASK0n = not(b.p34.SPRITE_MASK0);
  /*p34.LOTA*/ c.p34.SPRITE_MASK1n = not(b.p34.SPRITE_MASK1);
  /*p34.LYKU*/ c.p34.SPRITE_MASK2n = not(b.p34.SPRITE_MASK2);
  /*p34.ROBY*/ c.p34.SPRITE_MASK3n = not(b.p34.SPRITE_MASK3);
  /*p34.TYTA*/ c.p34.SPRITE_MASK4n = not(b.p34.SPRITE_MASK4);
  /*p34.TYCO*/ c.p34.SPRITE_MASK5n = not(b.p34.SPRITE_MASK5);
  /*p34.SOKA*/ c.p34.SPRITE_MASK6n = not(b.p34.SPRITE_MASK6);
  /*p34.XOVU*/ c.p34.SPRITE_MASK7n = not(b.p34.SPRITE_MASK7);

  /*p34.SYPY*/ c.p34.OAM_A_D4n = not(b.p31.OAM_A_D4); // sprite palette
  /*p34.TOTU*/ c.p34.OAM_A_D4o = not(b.p31.OAM_A_D4);
  /*p34.NARO*/ c.p34.OAM_A_D4p = not(b.p31.OAM_A_D4);
  /*p34.WEXY*/ c.p34.OAM_A_D4q = not(b.p31.OAM_A_D4);
  /*p34.RYZY*/ c.p34.OAM_A_D4r = not(b.p31.OAM_A_D4);
  /*p34.RYFE*/ c.p34.OAM_A_D4s = not(b.p31.OAM_A_D4);
  /*p34.LADY*/ c.p34.OAM_A_D4t = not(b.p31.OAM_A_D4);
  /*p34.LAFY*/ c.p34.OAM_A_D4u = not(b.p31.OAM_A_D4);

  /*p34.PUME*/ c.p34.SPRITE_PAL_PIPE_SET0n = nand(b.p34.SPRITE_MASK0n, b.p31.OAM_A_D4);
  /*p34.SORO*/ c.p34.SPRITE_PAL_PIPE_SET1n = nand(b.p34.SPRITE_MASK1n, b.p31.OAM_A_D4);
  /*p34.PAMO*/ c.p34.SPRITE_PAL_PIPE_SET2n = nand(b.p34.SPRITE_MASK2n, b.p31.OAM_A_D4);
  /*p34.SUKY*/ c.p34.SPRITE_PAL_PIPE_SET3n = nand(b.p34.SPRITE_MASK3n, b.p31.OAM_A_D4);
  /*p34.RORA*/ c.p34.SPRITE_PAL_PIPE_SET4n = nand(b.p34.SPRITE_MASK4n, b.p31.OAM_A_D4);
  /*p34.MENE*/ c.p34.SPRITE_PAL_PIPE_SET5n = nand(b.p34.SPRITE_MASK5n, b.p31.OAM_A_D4);
  /*p34.LUKE*/ c.p34.SPRITE_PAL_PIPE_SET6n = nand(b.p34.SPRITE_MASK6n, b.p31.OAM_A_D4);
  /*p34.LAMY*/ c.p34.SPRITE_PAL_PIPE_SET7n = nand(b.p34.SPRITE_MASK7n, b.p31.OAM_A_D4);

  /*p34.SUCO*/ c.p34.SPRITE_PAL_PIPE_RST0n = nand(b.p34.SPRITE_MASK0n, b.p34.OAM_A_D4n);
  /*p34.TAFA*/ c.p34.SPRITE_PAL_PIPE_RST1n = nand(b.p34.SPRITE_MASK1n, b.p34.OAM_A_D4o);
  /*p34.PYZY*/ c.p34.SPRITE_PAL_PIPE_RST2n = nand(b.p34.SPRITE_MASK2n, b.p34.OAM_A_D4p);
  /*p34.TOWA*/ c.p34.SPRITE_PAL_PIPE_RST3n = nand(b.p34.SPRITE_MASK3n, b.p34.OAM_A_D4q);
  /*p34.RUDU*/ c.p34.SPRITE_PAL_PIPE_RST4n = nand(b.p34.SPRITE_MASK4n, b.p34.OAM_A_D4r);
  /*p34.PAZO*/ c.p34.SPRITE_PAL_PIPE_RST5n = nand(b.p34.SPRITE_MASK5n, b.p34.OAM_A_D4s);
  /*p34.LOWA*/ c.p34.SPRITE_PAL_PIPE_RST6n = nand(b.p34.SPRITE_MASK6n, b.p34.OAM_A_D4t);
  /*p34.LUNU*/ c.p34.SPRITE_PAL_PIPE_RST7n = nand(b.p34.SPRITE_MASK7n, b.p34.OAM_A_D4u);

  /*p34.RUGO*/ c.p34.SPRITE_PAL_PIPE_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET0n, b.p34.SPRITE_PAL_PIPE_RST0n, b.p34.SPRITE_PAL_PIPE_0, b.chip.P10_B);
  /*p34.SATA*/ c.p34.SPRITE_PAL_PIPE_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET1n, b.p34.SPRITE_PAL_PIPE_RST1n, b.p34.SPRITE_PAL_PIPE_1, b.p34.SPRITE_PAL_PIPE_0);
  /*p34.ROSA*/ c.p34.SPRITE_PAL_PIPE_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET2n, b.p34.SPRITE_PAL_PIPE_RST2n, b.p34.SPRITE_PAL_PIPE_2, b.p34.SPRITE_PAL_PIPE_1);
  /*p34.SOMY*/ c.p34.SPRITE_PAL_PIPE_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET3n, b.p34.SPRITE_PAL_PIPE_RST3n, b.p34.SPRITE_PAL_PIPE_3, b.p34.SPRITE_PAL_PIPE_2);
  /*p34.PALU*/ c.p34.SPRITE_PAL_PIPE_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET4n, b.p34.SPRITE_PAL_PIPE_RST4n, b.p34.SPRITE_PAL_PIPE_4, b.p34.SPRITE_PAL_PIPE_3);
  /*p34.NUKE*/ c.p34.SPRITE_PAL_PIPE_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET5n, b.p34.SPRITE_PAL_PIPE_RST5n, b.p34.SPRITE_PAL_PIPE_5, b.p34.SPRITE_PAL_PIPE_4);
  /*p34.MODA*/ c.p34.SPRITE_PAL_PIPE_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET6n, b.p34.SPRITE_PAL_PIPE_RST6n, b.p34.SPRITE_PAL_PIPE_6, b.p34.SPRITE_PAL_PIPE_5);
  /*p34.LYME*/ c.p34.SPRITE_PAL_PIPE_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SPRITE_PAL_PIPE_SET7n, b.p34.SPRITE_PAL_PIPE_RST7n, b.p34.SPRITE_PAL_PIPE_7, b.p34.SPRITE_PAL_PIPE_6);
  
  //----------

  /*p26.XOGA*/ c.p26.OAM_A_D7n = not(b.p31.OAM_A_D7); // sprite priority
  /*p26.XURA*/ c.p26.OAM_A_D7o = not(b.p31.OAM_A_D7);
  /*p26.TAJO*/ c.p26.OAM_A_D7p = not(b.p31.OAM_A_D7);
  /*p26.XENU*/ c.p26.OAM_A_D7q = not(b.p31.OAM_A_D7);
  /*p26.XYKE*/ c.p26.OAM_A_D7r = not(b.p31.OAM_A_D7);
  /*p26.XABA*/ c.p26.OAM_A_D7s = not(b.p31.OAM_A_D7);
  /*p26.TAFU*/ c.p26.OAM_A_D7t = not(b.p31.OAM_A_D7);
  /*p26.XUHO*/ c.p26.OAM_A_D7u = not(b.p31.OAM_A_D7);

  /*p26.TEDE*/ c.p26.MASK_PIPE_SET0 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK0);
  /*p26.XALA*/ c.p26.MASK_PIPE_SET1 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK1);
  /*p26.TYRA*/ c.p26.MASK_PIPE_SET2 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK2);
  /*p26.XYRU*/ c.p26.MASK_PIPE_SET3 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK3);
  /*p26.XUKU*/ c.p26.MASK_PIPE_SET4 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK4);
  /*p26.XELY*/ c.p26.MASK_PIPE_SET5 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK5);
  /*p26.TYKO*/ c.p26.MASK_PIPE_SET6 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK6);
  /*p26.TUWU*/ c.p26.MASK_PIPE_SET7 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK7);

  /*p26.WOKA*/ c.p26.MASK_PIPE_RST0 = nand(b.p26.OAM_A_D7n, b.p34.SPRITE_MASK0);
  /*p26.WEDE*/ c.p26.MASK_PIPE_RST1 = nand(b.p26.OAM_A_D7o, b.p34.SPRITE_MASK1);
  /*p26.TUFO*/ c.p26.MASK_PIPE_RST2 = nand(b.p26.OAM_A_D7p, b.p34.SPRITE_MASK2);
  /*p26.WEVO*/ c.p26.MASK_PIPE_RST3 = nand(b.p26.OAM_A_D7q, b.p34.SPRITE_MASK3);
  /*p26.WEDY*/ c.p26.MASK_PIPE_RST4 = nand(b.p26.OAM_A_D7r, b.p34.SPRITE_MASK4);
  /*p26.WUJA*/ c.p26.MASK_PIPE_RST5 = nand(b.p26.OAM_A_D7s, b.p34.SPRITE_MASK5);
  /*p26.TENA*/ c.p26.MASK_PIPE_RST6 = nand(b.p26.OAM_A_D7t, b.p34.SPRITE_MASK6);
  /*p26.WUBU*/ c.p26.MASK_PIPE_RST7 = nand(b.p26.OAM_A_D7u, b.p34.SPRITE_MASK7);

  /*p26.VEZO*/ c.p26.MASK_PIPE_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET0, b.p26.MASK_PIPE_RST0, b.p26.MASK_PIPE_0, 0);
  /*p26.WURU*/ c.p26.MASK_PIPE_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET1, b.p26.MASK_PIPE_RST1, b.p26.MASK_PIPE_1, b.p26.MASK_PIPE_0);
  /*p26.VOSA*/ c.p26.MASK_PIPE_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET2, b.p26.MASK_PIPE_RST2, b.p26.MASK_PIPE_2, b.p26.MASK_PIPE_1);
  /*p26.WYFU*/ c.p26.MASK_PIPE_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET3, b.p26.MASK_PIPE_RST3, b.p26.MASK_PIPE_3, b.p26.MASK_PIPE_2);
  /*p26.XETE*/ c.p26.MASK_PIPE_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET4, b.p26.MASK_PIPE_RST4, b.p26.MASK_PIPE_4, b.p26.MASK_PIPE_3);
  /*p26.WODA*/ c.p26.MASK_PIPE_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET5, b.p26.MASK_PIPE_RST5, b.p26.MASK_PIPE_5, b.p26.MASK_PIPE_4);
  /*p26.VUMO*/ c.p26.MASK_PIPE_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET6, b.p26.MASK_PIPE_RST6, b.p26.MASK_PIPE_6, b.p26.MASK_PIPE_5);
  /*p26.VAVA*/ c.p26.MASK_PIPE_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET7, b.p26.MASK_PIPE_RST7, b.p26.MASK_PIPE_7, b.p26.MASK_PIPE_6);

  //----------
  // P35

  // Input background and sprite pixels

  /*p35.RAJY*/ c.p35.PIX_BG0  = and(b.p23.LCDC_BGEN, b.p32.BG_PIX_A_7);
  /*p35.TADE*/ c.p35.PIX_BG1  = and(b.p23.LCDC_BGEN, b.p32.BG_PIX_B_7);

  /*p35.WOXA*/ c.p35.PIX_SP0 = and(b.p23.LCDC_SPEN, b.p33.SPR_PIX_A_7);
  /*p35.XULA*/ c.p35.PIX_SP1 = and(b.p23.LCDC_SPEN, b.p33.SPR_PIX_B_7);
  /*p35.NULY*/ c.p35.PIX_SPn = nor(b.p35.PIX_SP0, b.p35.PIX_SP1);

  //----------
  // Sprite-vs-background masking

  /*p35.RYFU*/ c.p35.MASK_BG0   = and(b.p35.PIX_BG0, b.p26.MASK_PIPE_7);
  /*p35.RUTA*/ c.p35.MASK_BG1   = and(b.p35.PIX_BG1, b.p26.MASK_PIPE_7);
  /*p35.POKA*/ c.p35.BGPIXELn   = nor(b.p35.PIX_SPn, b.p35.MASK_BG0, b.p35.MASK_BG1);

  /*p34.LOME*/ c.p34.SPRITE_PAL_PIPE_7n = not(b.p34.SPRITE_PAL_PIPE_7);
  /*p34.LAFU*/ c.p34.OBP0PIXELn = nand(b.p34.SPRITE_PAL_PIPE_7n, b.p35.BGPIXELn);
  /*p34.LEKA*/ c.p34.OBP1PIXELn = nand(b.p34.SPRITE_PAL_PIPE_7n, b.p35.BGPIXELn);

  /*p35.LAVA*/ c.p35.MASK_OPB0  = not(b.p34.OBP0PIXELn);
  /*p35.LUKU*/ c.p35.MASK_OBP1  = not(b.p34.OBP1PIXELn);
  /*p35.MUVE*/ c.p35.MASK_BGP   = not(b.p35.BGPIXELn);

  //----------
  // Sprite palette 0 lookup

  /*p35.VUMU*/ c.p35.PIX_SP0n = not(b.p35.PIX_SP0);
  /*p35.WYRU*/ c.p35.PIX_SP0a = not(b.p35.PIX_SP0n);
  /*p35.WELE*/ c.p35.PIX_SP1n = not(b.p35.PIX_SP1);
  /*p35.WOLO*/ c.p35.PIX_SP1a = not(b.p35.PIX_SP1n);

  /*p35.VYRO*/ c.p35.PAL_OBP0D = and(b.p35.PIX_SP0a, b.p35.PIX_SP1a, b.p35.MASK_OPB0);
  /*p35.VATA*/ c.p35.PAL_OBP0C = and(b.p35.PIX_SP0a, b.p35.PIX_SP1n, b.p35.MASK_OPB0);
  /*p35.VOLO*/ c.p35.PAL_OBP0B = and(b.p35.PIX_SP0n, b.p35.PIX_SP1a, b.p35.MASK_OPB0);
  /*p35.VUGO*/ c.p35.PAL_OBP0A = and(b.p35.PIX_SP0n, b.p35.PIX_SP1n, b.p35.MASK_OPB0);

  /*p35.WUFU*/ c.p35.COL_OBP00 = amux4(b.p36.OBP0_D7, b.p35.PAL_OBP0D,
                                       b.p36.OBP0_D5, b.p35.PAL_OBP0C,
                                       b.p36.OBP0_D3, b.p35.PAL_OBP0B,
                                       b.p36.OBP0_D1, b.p35.PAL_OBP0A);

  /*p35.WALY*/ c.p35.COL_OBP01 = amux4(b.p36.OBP0_D6, b.p35.PAL_OBP0D,
                                       b.p36.OBP0_D4, b.p35.PAL_OBP0C,
                                       b.p36.OBP0_D2, b.p35.PAL_OBP0B,
                                       b.p36.OBP0_D0, b.p35.PAL_OBP0A);

  //----------
  // Sprite palette 1 lookup

  /*p35.MEXA*/ c.p35.PIX_SP0o = not(b.p35.PIX_SP0);
  /*p35.LOZO*/ c.p35.PIX_SP0b = not(b.p35.PIX_SP0o);
  /*p35.MABY*/ c.p35.PIX_SP1o = not(b.p35.PIX_SP1);
  /*p35.LYLE*/ c.p35.PIX_SP1b = not(b.p35.PIX_SP1o);

  /*p35.LEDO*/ c.p35.PAL_OBP0D = and(b.p35.PIX_SP1b, b.p35.PIX_SP0b, b.p35.MASK_OBP1);
  /*p35.LYKY*/ c.p35.PAL_OBP0C = and(b.p35.PIX_SP1b, b.p35.PIX_SP0o, b.p35.MASK_OBP1);
  /*p35.LARU*/ c.p35.PAL_OBP0B = and(b.p35.PIX_SP1o, b.p35.PIX_SP0b, b.p35.MASK_OBP1);
  /*p35.LOPU*/ c.p35.PAL_OBP0A = and(b.p35.PIX_SP1o, b.p35.PIX_SP0o, b.p35.MASK_OBP1);

  /*p35.MOKA*/ c.p35.COL_OBP10 = amux4(b.p36.OBP1_D7, b.p35.PAL_OBP0D,
                                       b.p36.OBP1_D5, b.p35.PAL_OBP0C,
                                       b.p36.OBP1_D3, b.p35.PAL_OBP0B,
                                       b.p36.OBP1_D1, b.p35.PAL_OBP0A);

  /*p35.MUFA*/ c.p35.COL_OBP11 = amux4(b.p36.OBP1_D6, b.p35.PAL_OBP0D,
                                       b.p36.OBP1_D4, b.p35.PAL_OBP0C,
                                       b.p36.OBP1_D2, b.p35.PAL_OBP0B,
                                       b.p36.OBP1_D0, b.p35.PAL_OBP0A);

  //----------
  // Background/window palette lookup

  /*p35.SOBA*/ c.p35.PIX_BG0n = not(b.p35.PIX_BG0);
  /*p35.VYCO*/ c.p35.PIX_BG1n = not(b.p35.PIX_BG1);
  /*p35.NUPO*/ c.p35.PIX_BG0a = not(b.p35.PIX_BG0n);
  /*p35.NALE*/ c.p35.PIX_BG1a = not(b.p35.PIX_BG1n);

  /*p35.POBU*/ c.p35.PAL_BGPA = and(b.p35.PIX_BG1n, b.p35.PIX_BG0n, b.p35.MASK_BGP);
  /*p35.NUMA*/ c.p35.PAL_BGPB = and(b.p35.PIX_BG1a, b.p35.PIX_BG0n, b.p35.MASK_BGP);
  /*p35.NUXO*/ c.p35.PAL_BGPC = and(b.p35.PIX_BG1n, b.p35.PIX_BG0a, b.p35.MASK_BGP);
  /*p35.NYPO*/ c.p35.PAL_BGPD = and(b.p35.PIX_BG1a, b.p35.PIX_BG0a, b.p35.MASK_BGP);

  /*p35.NURA*/ c.p35.COL_BGP1 = amux4(b.p36.BGP_D7, b.p35.PAL_BGPD,
                                      b.p36.BGP_D5, b.p35.PAL_BGPC,
                                      b.p36.BGP_D3, b.p35.PAL_BGPB,
                                      b.p36.BGP_D1, b.p35.PAL_BGPA);

  /*p35.NELO*/ c.p35.COL_BGP0 = amux4(b.p36.BGP_D6, b.p35.PAL_BGPD,
                                      b.p36.BGP_D4, b.p35.PAL_BGPC,
                                      b.p36.BGP_D2, b.p35.PAL_BGPB,
                                      b.p36.BGP_D0, b.p35.PAL_BGPA);

  //----------
  // Pixel merge and send

  // bits 0 and 1 swapped somewhere...

  /*p35.PATY*/ c.p35.LD0 = or(b.p35.COL_BGP1, b.p35.COL_OBP00, b.p35.COL_OBP10);
  /*p35.PERO*/ c.p35.LD1 = or(b.p35.COL_BGP0, b.p35.COL_OBP01, b.p35.COL_OBP11);

  /*p35.REMY*/ c.p35.LD0n = not(b.p35.LD0);
  /*p35.RAVO*/ c.p35.LD1n = not(b.p35.LD1);

  c.chip.LD0 = not(b.p35.LD0n);
  c.chip.LD1 = not(b.p35.LD1n);

  //----------
  // P36
}