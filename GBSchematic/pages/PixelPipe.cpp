#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P32_BgPixelShifter_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

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

  {
    c.p32.LUXA = not(b.p27.NYXU);

    c.p32.SEJA = nand(b.p32.TOSA, b.p32.LUXA);
    c.p32.SENO = nand(b.p32.RUCO, b.p32.LUXA);
    c.p32.SURE = nand(b.p32.TYCE, b.p32.LUXA);
    c.p32.SEBO = nand(b.p32.REVY, b.p32.LUXA);
    c.p32.SUCA = nand(b.p32.RYGA, b.p32.LUXA);
    c.p32.SYWE = nand(b.p32.RYLE, b.p32.LUXA);
    c.p32.SUPU = nand(b.p32.RAPU, b.p32.LUXA);
    c.p32.RYJY = nand(b.p32.SOJA, b.p32.LUXA);

    c.p32.TUXE = nand(b.p32.LUXA, !b.p32.RAWU);
    c.p32.SOLY = nand(b.p32.LUXA, !b.p32.POZO);
    c.p32.RUCE = nand(b.p32.LUXA, !b.p32.PYZO);
    c.p32.RYJA = nand(b.p32.LUXA, !b.p32.POXA);
    c.p32.RUTO = nand(b.p32.LUXA, !b.p32.PULO);
    c.p32.RAJA = nand(b.p32.LUXA, !b.p32.POJU);
    c.p32.RAJO = nand(b.p32.LUXA, !b.p32.POWY);
    c.p32.RAGA = nand(b.p32.LUXA, !b.p32.PYJU);
  }

  {
    c.p32.LOZE = not(b.p27.NYXU);

    c.p32.LOTY = nand(b.p32.LUHE, b.p32.LOZE);
    c.p32.NEXA = nand(b.p32.NOLY, b.p32.LOZE);
    c.p32.LUTU = nand(b.p32.LEKE, b.p32.LOZE);
    c.p32.LUJA = nand(b.p32.LOMY, b.p32.LOZE);
    c.p32.MOSY = nand(b.p32.LALA, b.p32.LOZE);
    c.p32.LERU = nand(b.p32.LOXA, b.p32.LOZE);
    c.p32.NYHA = nand(b.p32.NEZE, b.p32.LOZE);
    c.p32.NADY = nand(b.p32.NOBO, b.p32.LOZE);

    c.p32.LAKY = nand(b.p32.LOZE, b.p32.LEGU);
    c.p32.NYXO = nand(b.p32.LOZE, b.p32.NUDU);
    c.p32.LOTO = nand(b.p32.LOZE, b.p32.MUKU);
    c.p32.LYDU = nand(b.p32.LOZE, b.p32.LUZO);
    c.p32.MYVY = nand(b.p32.LOZE, b.p32.MEGU);
    c.p32.LODO = nand(b.p32.LOZE, b.p32.MYJY);
    c.p32.NUTE = nand(b.p32.LOZE, b.p32.NASA);
    c.p32.NAJA = nand(b.p32.LOZE, b.p32.NEFO);
  }

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
  // P33

  // sprite x flip
  /*p29.XONO*/ c.p29.FLIP_X = and(!b.p31.OAM_A_D5, b.p29.TEXY);

  /*p33.PUTE*/ c.p33.PUTE = mux2(b.MD7, b.MD0, b.p29.FLIP_X);
  /*p33.PELO*/ c.p33.PELO = mux2(b.MD6, b.MD1, b.p29.FLIP_X);
  /*p33.PONO*/ c.p33.PONO = mux2(b.MD5, b.MD2, b.p29.FLIP_X);
  /*p33.POBE*/ c.p33.POBE = mux2(b.MD4, b.MD3, b.p29.FLIP_X);
  /*p33.PACY*/ c.p33.PACY = mux2(b.MD3, b.MD4, b.p29.FLIP_X);
  /*p33.PUGU*/ c.p33.PUGU = mux2(b.MD2, b.MD5, b.p29.FLIP_X);
  /*p33.PAWE*/ c.p33.PAWE = mux2(b.MD1, b.MD6, b.p29.FLIP_X);
  /*p33.PULY*/ c.p33.PULY = mux2(b.MD0, b.MD7, b.p29.FLIP_X);

  // the schematic layout is slightly inconsistent, might have some of these swapped

  /*p33.PEFO*/ c.p33.PEFO = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.PEFO, b.p33.PUTE);
  /*p33.PEBA*/ c.p33.PEBA = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.PEBA, b.p33.PELO);
  /*p33.MYTU*/ c.p33.MYTU = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.MYTU, b.p33.PONO);
  /*p33.PUDU*/ c.p33.PUDU = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.PUDU, b.p33.POBE);
  /*p33.SAJA*/ c.p33.SAJA = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SAJA, b.p33.PACY);
  /*p33.SUNY*/ c.p33.SUNY = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SUNY, b.p33.PUGU);
  /*p33.SEMO*/ c.p33.SEMO = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SEMO, b.p33.PAWE);
  /*p33.SEGA*/ c.p33.SEGA = latch_pos(b.p29.SPRITE_PIX_LATCH_A, b.p33.SEGA, b.p33.PULY);

  /*p33.REWO*/ c.p33.REWO = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.REWO, b.p33.PUTE);
  /*p33.ROKA*/ c.p33.ROKA = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.ROKA, b.p33.PELO);
  /*p33.MOFO*/ c.p33.MOFO = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.MOFO, b.p33.PONO);
  /*p33.RAMU*/ c.p33.RAMU = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.RAMU, b.p33.POBE);
  /*p33.SELE*/ c.p33.SELE = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SELE, b.p33.PACY);
  /*p33.SUTO*/ c.p33.SUTO = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.SUTO, b.p33.PUGU);
  /*p33.RAMA*/ c.p33.RAMA = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.RAMA, b.p33.PAWE);
  /*p33.RYDU*/ c.p33.RYDU = latch_pos(b.p29.SPRITE_PIX_LATCH_B, b.p33.RYDU, b.p33.PULY);

  /*p33.LOZA*/ c.p33.LOZA = not(b.p33.PEFO);
  /*p33.NUCA*/ c.p33.NUCA = not(b.p33.PEBA);
  /*p33.LUMO*/ c.p33.LUMO = not(b.p33.MYTU);
  /*p33.LUBO*/ c.p33.LUBO = not(b.p33.PUDU);
  /*p33.WERY*/ c.p33.WERY = not(b.p33.SAJA);
  /*p33.WURA*/ c.p33.WURA = not(b.p33.SUNY);
  /*p33.SULU*/ c.p33.SULU = not(b.p33.SEMO);
  /*p33.WAMY*/ c.p33.WAMY = not(b.p33.SEGA);

  /*p33.RATA*/ c.p33.RATA = not(b.p33.REWO);
  /*p33.SYBO*/ c.p33.SYBO = not(b.p33.ROKA);
  /*p33.LASE*/ c.p33.LASE = not(b.p33.MOFO);
  /*p33.SOLO*/ c.p33.SOLO = not(b.p33.RAMU);
  /*p33.VOBY*/ c.p33.VOBY = not(b.p33.SELE);
  /*p33.WYCO*/ c.p33.WYCO = not(b.p33.SUTO);
  /*p33.SERY*/ c.p33.SERY = not(b.p33.RAMA);
  /*p33.SELU*/ c.p33.SELU = not(b.p33.RYDU);

  /*p34.LESY*/ c.p34.SPRITE_MASK0 = not(b.p34.MEFU);
  /*p34.LOTA*/ c.p34.SPRITE_MASK1 = not(b.p34.MEVE);
  /*p34.LYKU*/ c.p34.SPRITE_MASK2 = not(b.p34.MYZO);
  /*p34.ROBY*/ c.p34.SPRITE_MASK3 = not(b.p34.RUDA);
  /*p34.TYTA*/ c.p34.SPRITE_MASK4 = not(b.p34.VOTO);
  /*p34.TYCO*/ c.p34.SPRITE_MASK5 = not(b.p34.VYSA);
  /*p34.SOKA*/ c.p34.SPRITE_MASK6 = not(b.p34.TORY);
  /*p34.XOVU*/ c.p34.SPRITE_MASK7 = not(b.p34.WOPE);

  /*p33.LUFY*/ c.p33.LUFY = nand(b.p33.LUBO, b.p34.SPRITE_MASK3);
  /*p33.MAME*/ c.p33.MAME = nand(b.p33.PUDU, b.p34.SPRITE_MASK3);
  /*p33.REHU*/ c.p33.REHU = nand(b.p33.SOLO, b.p34.SPRITE_MASK3);
  /*p33.RANO*/ c.p33.RANO = nand(b.p33.RAMU, b.p34.SPRITE_MASK3);

  /*p33.WAXO*/ c.p33.WAXO = nand(b.p33.VOBY, b.p34.SPRITE_MASK4);
  /*p33.TYGA*/ c.p33.TYGA = nand(b.p33.SELE, b.p34.SPRITE_MASK4);
  /*p33.XATO*/ c.p33.XATO = nand(b.p33.WERY, b.p34.SPRITE_MASK4);
  /*p33.VEXU*/ c.p33.VEXU = nand(b.p33.SAJA, b.p34.SPRITE_MASK4);

  /*p33.MAJO*/ c.p33.MAJO = nand(b.p33.LUMO, b.p34.SPRITE_MASK2);
  /*p33.MYXA*/ c.p33.MYXA = nand(b.p33.MYTU, b.p34.SPRITE_MASK2);
  /*p33.LYDE*/ c.p33.LYDE = nand(b.p33.LASE, b.p34.SPRITE_MASK2);
  /*p33.LELA*/ c.p33.LELA = nand(b.p33.MOFO, b.p34.SPRITE_MASK2);

  /*p33.XEXU*/ c.p33.XEXU = nand(b.p33.WURA, b.p34.SPRITE_MASK5);
  /*p33.VABY*/ c.p33.VABY = nand(b.p33.SUNY, b.p34.SPRITE_MASK5);
  /*p33.XOLE*/ c.p33.XOLE = nand(b.p33.WYCO, b.p34.SPRITE_MASK5);
  /*p33.VUME*/ c.p33.VUME = nand(b.p33.SUTO, b.p34.SPRITE_MASK5);

  /*p33.MOFY*/ c.p33.MOFY = nand(b.p33.LOZA, b.p34.SPRITE_MASK0);
  /*p33.MEZU*/ c.p33.MEZU = nand(b.p33.PEFO, b.p34.SPRITE_MASK0);
  /*p33.PYZU*/ c.p33.PYZU = nand(b.p33.RATA, b.p34.SPRITE_MASK0);
  /*p33.PABE*/ c.p33.PABE = nand(b.p33.REWO, b.p34.SPRITE_MASK0);

  /*p33.TULA*/ c.p33.TULA = nand(b.p33.SELU, b.p34.SPRITE_MASK7);
  /*p33.TESO*/ c.p33.TESO = nand(b.p33.RYDU, b.p34.SPRITE_MASK7);
  /*p33.XYVE*/ c.p33.XYVE = nand(b.p33.WAMY, b.p34.SPRITE_MASK7);
  /*p33.VUNE*/ c.p33.VUNE = nand(b.p33.SEGA, b.p34.SPRITE_MASK7);

  /*p33.MADA*/ c.p33.MADA = nand(b.p33.NUCA, b.p34.SPRITE_MASK1);
  /*p33.MYTO*/ c.p33.MYTO = nand(b.p33.PEBA, b.p34.SPRITE_MASK1);
  /*p33.RUCA*/ c.p33.RUCA = nand(b.p33.SYBO, b.p34.SPRITE_MASK1);
  /*p33.RUSY*/ c.p33.RUSY = nand(b.p33.ROKA, b.p34.SPRITE_MASK1);

  /*p33.TABY*/ c.p33.TABY = nand(b.p33.SERY, b.p34.SPRITE_MASK6);
  /*p33.TAPO*/ c.p33.TAPO = nand(b.p33.RAMA, b.p34.SPRITE_MASK6);
  /*p33.TUPE*/ c.p33.TUPE = nand(b.p33.SULU, b.p34.SPRITE_MASK6);
  /*p33.TUXA*/ c.p33.TUXA = nand(b.p33.SEMO, b.p34.SPRITE_MASK6);

  /*p33.NURO*/ c.p33.NURO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.PABE, b.p33.PYZU, b.p33.NURO, b.chip.P10_B);
  /*p33.MASO*/ c.p33.MASO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MYTO, b.p33.MADA, b.p33.MASO, b.p33.NURO);
  /*p33.LEFE*/ c.p33.LEFE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.LELA, b.p33.LYDE, b.p33.LEFE, b.p33.MASO);
  /*p33.LESU*/ c.p33.LESU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MAME, b.p33.LUFY, b.p33.LESU, b.p33.LEFE);
  /*p33.WYHO*/ c.p33.WYHO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VEXU, b.p33.XATO, b.p33.WYHO, b.p33.LESU);
  /*p33.WORA*/ c.p33.WORA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VABY, b.p33.XEXU, b.p33.WORA, b.p33.WYHO);
  /*p33.VAFO*/ c.p33.VAFO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TUXA, b.p33.TUPE, b.p33.VAFO, b.p33.WORA);
  /*p33.WUFY*/ c.p33.WUFY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VUNE, b.p33.XYVE, b.p33.WUFY, b.p33.VAFO);

  /*p33.NYLU*/ c.p33.NYLU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MEZU, b.p33.MOFY, b.p33.NYLU, b.chip.P10_B);
  /*p33.PEFU*/ c.p33.PEFU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.RUSY, b.p33.RUCA, b.p33.PEFU, b.p33.NYLU);
  /*p33.NATY*/ c.p33.NATY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MYXA, b.p33.MAJO, b.p33.NATY, b.p33.PEFU);
  /*p33.PYJO*/ c.p33.PYJO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.RANO, b.p33.REHU, b.p33.PYJO, b.p33.NATY);
  /*p33.VARE*/ c.p33.VARE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TYGA, b.p33.WAXO, b.p33.VARE, b.p33.PYJO);
  /*p33.WEBA*/ c.p33.WEBA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VUME, b.p33.XOLE, b.p33.WEBA, b.p33.VARE);
  /*p33.VANU*/ c.p33.VANU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TAPO, b.p33.TABY, b.p33.VANU, b.p33.WEBA);
  /*p33.VUPY*/ c.p33.VUPY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TESO, b.p33.TULA, b.p33.VUPY, b.p33.VANU);

  //----------
  // P34

  /*p34.MEFU*/ c.p34.MEFU = or(b.p29.XEFY, b.p33.SPR_PIX_A_0, b.p33.SPR_PIX_B_0);
  /*p34.MEVE*/ c.p34.MEVE = or(b.p29.XEFY, b.p33.SPR_PIX_A_1, b.p33.SPR_PIX_B_1);
  /*p34.MYZO*/ c.p34.MYZO = or(b.p29.XEFY, b.p33.SPR_PIX_A_2, b.p33.SPR_PIX_B_2);
  /*p34.RUDA*/ c.p34.RUDA = or(b.p29.XEFY, b.p33.SPR_PIX_A_3, b.p33.SPR_PIX_B_3);
  /*p34.VOTO*/ c.p34.VOTO = or(b.p29.XEFY, b.p33.SPR_PIX_A_4, b.p33.SPR_PIX_B_4);
  /*p34.VYSA*/ c.p34.VYSA = or(b.p29.XEFY, b.p33.SPR_PIX_A_5, b.p33.SPR_PIX_B_5);
  /*p34.TORY*/ c.p34.TORY = or(b.p29.XEFY, b.p33.SPR_PIX_A_6, b.p33.SPR_PIX_B_6);
  /*p34.WOPE*/ c.p34.WOPE = or(b.p29.XEFY, b.p33.SPR_PIX_A_7, b.p33.SPR_PIX_B_7);

  /*p34.LESY*/ c.p34.SPRITE_MASK0 = not(b.p34.MEFU);
  /*p34.LOTA*/ c.p34.SPRITE_MASK1 = not(b.p34.MEVE);
  /*p34.LYKU*/ c.p34.SPRITE_MASK2 = not(b.p34.MYZO);
  /*p34.ROBY*/ c.p34.SPRITE_MASK3 = not(b.p34.RUDA);
  /*p34.TYTA*/ c.p34.SPRITE_MASK4 = not(b.p34.VOTO);
  /*p34.TYCO*/ c.p34.SPRITE_MASK5 = not(b.p34.VYSA);
  /*p34.SOKA*/ c.p34.SPRITE_MASK6 = not(b.p34.TORY);
  /*p34.XOVU*/ c.p34.SPRITE_MASK7 = not(b.p34.WOPE);

  /*p34.SYPY*/ c.p34.SYPY = not(b.p31.OAM_A_D4);
  /*p34.TOTU*/ c.p34.TOTU = not(b.p31.OAM_A_D4);
  /*p34.NARO*/ c.p34.NARO = not(b.p31.OAM_A_D4);
  /*p34.WEXY*/ c.p34.WEXY = not(b.p31.OAM_A_D4);
  /*p34.RYZY*/ c.p34.RYZY = not(b.p31.OAM_A_D4);
  /*p34.RYFE*/ c.p34.RYFE = not(b.p31.OAM_A_D4);
  /*p34.LADY*/ c.p34.LADY = not(b.p31.OAM_A_D4);
  /*p34.LAFY*/ c.p34.LAFY = not(b.p31.OAM_A_D4);

  /*p34.PUME*/ c.p34.PUME = nand(b.p34.SPRITE_MASK0, b.p31.OAM_A_D4);
  /*p34.SUCO*/ c.p34.SUCO = nand(b.p34.SPRITE_MASK0, b.p34.SYPY);
  /*p34.SORO*/ c.p34.SORO = nand(b.p34.SPRITE_MASK1, b.p31.OAM_A_D4);
  /*p34.TAFA*/ c.p34.TAFA = nand(b.p34.SPRITE_MASK1, b.p34.TOTU);
  /*p34.PAMO*/ c.p34.PAMO = nand(b.p34.SPRITE_MASK2, b.p31.OAM_A_D4);
  /*p34.PYZY*/ c.p34.PYZY = nand(b.p34.SPRITE_MASK2, b.p34.NARO);
  /*p34.SUKY*/ c.p34.SUKY = nand(b.p34.SPRITE_MASK3, b.p31.OAM_A_D4);
  /*p34.TOWA*/ c.p34.TOWA = nand(b.p34.SPRITE_MASK3, b.p34.WEXY);
  /*p34.RORA*/ c.p34.RORA = nand(b.p34.SPRITE_MASK4, b.p31.OAM_A_D4);
  /*p34.RUDU*/ c.p34.RUDU = nand(b.p34.SPRITE_MASK4, b.p34.RYZY);
  /*p34.MENE*/ c.p34.MENE = nand(b.p34.SPRITE_MASK5, b.p31.OAM_A_D4);
  /*p34.PAZO*/ c.p34.PAZO = nand(b.p34.SPRITE_MASK5, b.p34.RYFE);
  /*p34.LUKE*/ c.p34.LUKE = nand(b.p34.SPRITE_MASK6, b.p31.OAM_A_D4);
  /*p34.LOWA*/ c.p34.LOWA = nand(b.p34.SPRITE_MASK6, b.p34.LADY);
  /*p34.LAMY*/ c.p34.LAMY = nand(b.p34.SPRITE_MASK7, b.p31.OAM_A_D4);
  /*p34.LUNU*/ c.p34.LUNU = nand(b.p34.SPRITE_MASK7, b.p34.LAFY);

  /*p34.RUGO*/ c.p34.RUGO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.PUME, b.p34.SUCO, b.p34.RUGO, b.chip.P10_B);
  /*p34.SATA*/ c.p34.SATA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SORO, b.p34.TAFA, b.p34.SATA, b.p34.RUGO);
  /*p34.ROSA*/ c.p34.ROSA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.PAMO, b.p34.PYZY, b.p34.ROSA, b.p34.SATA);
  /*p34.SOMY*/ c.p34.SOMY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SUKY, b.p34.TOWA, b.p34.SOMY, b.p34.ROSA);
  /*p34.PALU*/ c.p34.PALU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.RORA, b.p34.RUDU, b.p34.PALU, b.p34.SOMY);
  /*p34.NUKE*/ c.p34.NUKE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.MENE, b.p34.PAZO, b.p34.NUKE, b.p34.PALU);
  /*p34.MODA*/ c.p34.MODA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.LUKE, b.p34.LOWA, b.p34.MODA, b.p34.NUKE);
  /*p34.LYME*/ c.p34.LYME = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.LAMY, b.p34.LUNU, b.p34.LYME, b.p34.MODA);

  /*p34.LOME*/ c.p34.LOME = not(b.p34.LYME);

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

  /*p34.LAFU*/ c.p34.OBP0PIXELn = nand(b.p34.LOME, b.p35.BGPIXELn);
  /*p34.LEKA*/ c.p34.OBP1PIXELn = nand(b.p34.LYME, b.p35.BGPIXELn);

  /*p35.LAVA*/ c.p35.MASK_OPB0  = not(b.p34.OBP0PIXELn);
  /*p35.LUKU*/ c.p35.MASK_OBP1  = not(b.p34.OBP1PIXELn);
  /*p35.MUVE*/ c.p35.MASK_BGP   = not(b.p35.BGPIXELn);

  //----------

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
  // 2-to-4 decoder for palette lookup + mask out missing pixels + palette lookup

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
}