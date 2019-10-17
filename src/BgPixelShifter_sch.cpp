// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CLKPIPE;
extern bool P10_B;
extern bool MOFU;
extern bool NYDY;
extern bool NYXU;
extern bool VYPO;

//----------
// outputs

bool BG_PIX_A_7;
bool BG_PIX_B_7;

//----------
// registers

// tile reg?
reg RAWU;
reg POZO;
reg PYZO;
reg POXA;
reg PULO;
reg POJU;
reg POWY;
reg PYJU;

// tile latch?
reg LEGU;
reg NUDU;
reg MUKU;
reg LUZO;
reg MEGU;
reg MYJY;
reg NASA;
reg NEFO;

// bg_pix_b
reg TOMY;
reg TACA;
reg SADY;
reg RYSA;
reg SOBO;
reg SETU;
reg RALU;
reg SOHU;

// bg_pix_a
reg MYDE;
reg NOZO;
reg MOJU;
reg MACU;
reg NEPO;
reg MODU;
reg NEDA;
reg PYBO;

//-----------------------------------------------------------------------------

void tick_bgpixelshifter() {

  wire LESO = not(MOFU);
  wire AJAR = not(LESO);
  wire LABU = not(AJAR);
  wire METE = not(NYDY);
  wire LOMA = not(METE);
  wire LOZE = not(NYXU);
  wire LUXA = not(NYXU);

  wire RAWU_Q = RAWU.tock(LABU, VYPO, MD0);
  wire POZO_Q = POZO.tock(LABU, VYPO, MD1);
  wire PYZO_Q = PYZO.tock(LABU, VYPO, MD2);
  wire POXA_Q = POXA.tock(LABU, VYPO, MD3);
  wire PULO_Q = POXA.tock(LABU, VYPO, MD4);
  wire POJU_Q = POXA.tock(LABU, VYPO, MD5);
  wire POWY_Q = POXA.tock(LABU, VYPO, MD6);
  wire PYJU_Q = POXA.tock(LABU, VYPO, MD7);

  wire LEGU_Q = LEGU.latch(LOMA, MD0);
  wire NUDU_Q = LEGU.latch(LOMA, MD1);
  bool MUKU_Q = MUKU.latch(LOMA, MD2);
  bool LUZO_Q = MUKU.latch(LOMA, MD3);
  bool MEGU_Q = MEGU.latch(LOMA, MD4);
  bool MYJY_Q = MYJY.latch(LOMA, MD5);
  bool NASA_Q = NASA.latch(LOMA, MD6);
  bool NEFO_Q = NEFO.latch(LOMA, MD7);

  bool TOSA = not(!RAWU_Q);
  bool RUCO = not(!POZO_Q);
  bool TYCE = not(!PYZO_Q);
  bool REVY = not(!POXA_Q);
  bool RYGA = not(!PULO_Q);
  bool RYLE = not(!POJU_Q);
  bool RAPU = not(!POWY_Q);
  bool SOJA = not(!PYJU_Q);

  bool LUHE = not(LEGU_Q);
  bool NOLY = not(NUDU_Q);
  bool LEKE = not(MUKU_Q);
  bool LOMY = not(LUZO_Q);
  bool LALA = not(MEGU_Q);
  bool LOXA = not(MYJY_Q);
  bool NEZE = not(NASA_Q);
  bool NOBO = not(NEFO_Q);

  bool SEJA = nand(TOSA, LUXA);
  bool SENO = nand(RUCO, LUXA);
  bool SURE = nand(TYCE, LUXA);
  bool SEBO = nand(REVY, LUXA);
  bool SUCA = nand(RYGA, LUXA);
  bool SYWE = nand(RYLE, LUXA);
  bool SUPU = nand(RAPU, LUXA);
  bool RYJY = nand(SOJA, LUXA);

  bool TUXE = nand(LUXA, !RAWU_Q);
  bool SOLY = nand(LUXA, !POZO_Q);
  bool RUCE = nand(LUXA, !PYZO_Q);
  bool RYJA = nand(LUXA, !POXA_Q);
  bool RUTO = nand(LUXA, !PULO_Q);
  bool RAJA = nand(LUXA, !POJU_Q);
  bool RAJO = nand(LUXA, !POWY_Q);
  bool RAGA = nand(LUXA, !PYJU_Q);

  bool LOTY = nand(LUHE, LOZE);
  bool NEXA = nand(NOLY, LOZE);
  bool LUTU = nand(LEKE, LOZE);
  bool LUJA = nand(LOMY, LOZE);
  bool MOSY = nand(LALA, LOZE);
  bool LERU = nand(LOXA, LOZE);
  bool NYHA = nand(NEZE, LOZE);
  bool NADY = nand(NOBO, LOZE);

  bool LAKY = nand(LOZE, LEGU_Q);
  bool NYXO = nand(LOZE, NUDU_Q);
  bool LOTO = nand(LOZE, MUKU_Q);
  bool LYDU = nand(LOZE, LUZO_Q);
  bool MYVY = nand(LOZE, MEGU_Q);
  bool LODO = nand(LOZE, MYJY_Q);
  bool NUTE = nand(LOZE, NASA_Q);
  bool NAJA = nand(LOZE, NEFO_Q);

  bool TOMY_Q = TOMY.srtock(CLKPIPE, TUXE, SEJA, P10_B);
  bool TACA_Q = TACA.srtock(CLKPIPE, SOLY, SENO, TOMY_Q);
  bool SADY_Q = SADY.srtock(CLKPIPE, RUCE, SURE, TACA_Q);
  bool RYSA_Q = RYSA.srtock(CLKPIPE, RYJA, SEBO, SADY_Q);
  bool SOBO_Q = SOBO.srtock(CLKPIPE, RUTO, SUCA, RYSA_Q);
  bool SETU_Q = SETU.srtock(CLKPIPE, RAJA, SYWE, SOBO_Q);
  bool RALU_Q = RALU.srtock(CLKPIPE, RAJO, SUPU, SETU_Q);
  bool SOHU_Q = SOHU.srtock(CLKPIPE, RAGA, RYJY, RALU_Q);

  BG_PIX_B_7 = SOHU_Q;

  bool MYDE_Q = MYDE.srtock(CLKPIPE, LAKY, LOTY, P10_B);
  bool NOZO_Q = NOZO.srtock(CLKPIPE, NYXO, NEXA, MYDE_Q);
  bool MOJU_Q = MOJU.srtock(CLKPIPE, LOTO, LUTU, NOZO_Q);
  bool MACU_Q = MACU.srtock(CLKPIPE, LYDU, LUJA, MOJU_Q);
  bool NEPO_Q = NEPO.srtock(CLKPIPE, MYVY, MOSY, MACU_Q);
  bool MODU_Q = MODU.srtock(CLKPIPE, LODO, LERU, NEPO_Q);
  bool NEDA_Q = NEDA.srtock(CLKPIPE, NUTE, NYHA, MODU_Q);
  bool PYBO_Q = PYBO.srtock(CLKPIPE, NAJA, NADY, NEDA_Q);

  BG_PIX_A_7 = PYBO_Q;
}