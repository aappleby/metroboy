#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P32_BgPixelShifter::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p32.LESO = not(b.p27.MOFU);
  c.p32.AJAR = not(b.p32.LESO);
  c.p32.LABU = not(b.p32.AJAR);
  c.p32.METE = not(b.p27.NYDY);
  c.p32.LOMA = not(b.p32.METE);
  c.p32.LOZE = not(b.p27.NYXU);
  c.p32.LUXA = not(b.p27.NYXU);

  c.p32.RAWU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.RAWU, b.MD0);
  c.p32.POZO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POZO, b.MD1);
  c.p32.PYZO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PYZO, b.MD2);
  c.p32.POXA = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POXA, b.MD3);
  c.p32.PULO = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PULO, b.MD4);
  c.p32.POJU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POJU, b.MD5);
  c.p32.POWY = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.POWY, b.MD6);
  c.p32.PYJU = tock_pos(a.p32.LABU, b.p32.LABU, b.p27.P10_Bn, b.p32.PYJU, b.MD7);

  c.p32.LEGU = latch_pos(b.p32.LOMA, b.p32.LEGU, b.MD0);
  c.p32.NUDU = latch_pos(b.p32.LOMA, b.p32.NUDU, b.MD1);
  c.p32.MUKU = latch_pos(b.p32.LOMA, b.p32.MUKU, b.MD2);
  c.p32.LUZO = latch_pos(b.p32.LOMA, b.p32.LUZO, b.MD3);
  c.p32.MEGU = latch_pos(b.p32.LOMA, b.p32.MEGU, b.MD4);
  c.p32.MYJY = latch_pos(b.p32.LOMA, b.p32.MYJY, b.MD5);
  c.p32.NASA = latch_pos(b.p32.LOMA, b.p32.NASA, b.MD6);
  c.p32.NEFO = latch_pos(b.p32.LOMA, b.p32.NEFO, b.MD7);

  c.p32.TOSA = not(!b.p32.RAWU);
  c.p32.RUCO = not(!b.p32.POZO);
  c.p32.TYCE = not(!b.p32.PYZO);
  c.p32.REVY = not(!b.p32.POXA);
  c.p32.RYGA = not(!b.p32.PULO);
  c.p32.RYLE = not(!b.p32.POJU);
  c.p32.RAPU = not(!b.p32.POWY);
  c.p32.SOJA = not(!b.p32.PYJU);

  c.p32.LUHE = not(b.p32.LEGU);
  c.p32.NOLY = not(b.p32.NUDU);
  c.p32.LEKE = not(b.p32.MUKU);
  c.p32.LOMY = not(b.p32.LUZO);
  c.p32.LALA = not(b.p32.MEGU);
  c.p32.LOXA = not(b.p32.MYJY);
  c.p32.NEZE = not(b.p32.NASA);
  c.p32.NOBO = not(b.p32.NEFO);

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

  c.p32.TOMY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.TUXE, b.p32.SEJA, b.p32.TOMY, b.chip.P10_B);
  c.p32.TACA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.SOLY, b.p32.SENO, b.p32.TACA, b.p32.TOMY);
  c.p32.SADY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RUCE, b.p32.SURE, b.p32.SADY, b.p32.TACA);
  c.p32.RYSA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RYJA, b.p32.SEBO, b.p32.RYSA, b.p32.SADY);
  c.p32.SOBO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RUTO, b.p32.SUCA, b.p32.SOBO, b.p32.RYSA);
  c.p32.SETU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAJA, b.p32.SYWE, b.p32.SETU, b.p32.SOBO);
  c.p32.RALU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAJO, b.p32.SUPU, b.p32.RALU, b.p32.SETU);
  c.p32.SOHU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.RAGA, b.p32.RYJY, b.p32.SOHU, b.p32.RALU);

  c.p32.MYDE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LAKY, b.p32.LOTY, b.p32.MYDE, b.chip.P10_B);
  c.p32.NOZO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NYXO, b.p32.NEXA, b.p32.NOZO, b.p32.MYDE);
  c.p32.MOJU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LOTO, b.p32.LUTU, b.p32.MOJU, b.p32.NOZO);
  c.p32.MACU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LYDU, b.p32.LUJA, b.p32.MACU, b.p32.MOJU);
  c.p32.NEPO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.MYVY, b.p32.MOSY, b.p32.NEPO, b.p32.MACU);
  c.p32.MODU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.LODO, b.p32.LERU, b.p32.MODU, b.p32.NEPO);
  c.p32.NEDA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NUTE, b.p32.NYHA, b.p32.NEDA, b.p32.MODU);
  c.p32.PYBO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p32.NAJA, b.p32.NADY, b.p32.PYBO, b.p32.NEDA);
}