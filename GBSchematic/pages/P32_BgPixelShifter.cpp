#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P32_BgPixelShifter {
  struct Input {
    bool CLKPIPE;

    bool VYPO;
    bool MOFU;
    bool NYDY;
    bool NYXU;
    bool P10_B;

    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;
  };

  struct Output {
    bool BG_PIX_A_7;
    bool BG_PIX_B_7;
  };

  // tile reg?
  reg RAWU,POZO,PYZO,POXA,PULO,POJU,POWY,PYJU;

  // tile latch?
  reg LEGU,NUDU,MUKU,LUZO,MEGU,MYJY,NASA,NEFO;

  // bg_pix_b
  reg TOMY,TACA,SADY,RYSA,SOBO,SETU,RALU,SOHU;

  // bg_pix_a
  reg MYDE,NOZO,MOJU,MACU,NEPO,MODU,NEDA,PYBO;

  void tick(const Input& in, Output& out) {

    wire LESO = not(in.MOFU);
    wire AJAR = not(LESO);
    wire LABU = not(AJAR);
    wire METE = not(in.NYDY);
    wire LOMA = not(METE);
    wire LOZE = not(in.NYXU);
    wire LUXA = not(in.NYXU);

    wire RAWU_Q = RAWU.tock(LABU, in.VYPO, in.MD0);
    wire POZO_Q = POZO.tock(LABU, in.VYPO, in.MD1);
    wire PYZO_Q = PYZO.tock(LABU, in.VYPO, in.MD2);
    wire POXA_Q = POXA.tock(LABU, in.VYPO, in.MD3);
    wire PULO_Q = PULO.tock(LABU, in.VYPO, in.MD4);
    wire POJU_Q = POJU.tock(LABU, in.VYPO, in.MD5);
    wire POWY_Q = POWY.tock(LABU, in.VYPO, in.MD6);
    wire PYJU_Q = PYJU.tock(LABU, in.VYPO, in.MD7);

    wire LEGU_Q = LEGU.latch(LOMA, in.MD0);
    wire NUDU_Q = NUDU.latch(LOMA, in.MD1);
    bool MUKU_Q = MUKU.latch(LOMA, in.MD2);
    bool LUZO_Q = LUZO.latch(LOMA, in.MD3);
    bool MEGU_Q = MEGU.latch(LOMA, in.MD4);
    bool MYJY_Q = MYJY.latch(LOMA, in.MD5);
    bool NASA_Q = NASA.latch(LOMA, in.MD6);
    bool NEFO_Q = NEFO.latch(LOMA, in.MD7);

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

    bool TOMY_Q = TOMY.srtock(in.CLKPIPE, TUXE, SEJA, in.P10_B);
    bool TACA_Q = TACA.srtock(in.CLKPIPE, SOLY, SENO, TOMY_Q);
    bool SADY_Q = SADY.srtock(in.CLKPIPE, RUCE, SURE, TACA_Q);
    bool RYSA_Q = RYSA.srtock(in.CLKPIPE, RYJA, SEBO, SADY_Q);
    bool SOBO_Q = SOBO.srtock(in.CLKPIPE, RUTO, SUCA, RYSA_Q);
    bool SETU_Q = SETU.srtock(in.CLKPIPE, RAJA, SYWE, SOBO_Q);
    bool RALU_Q = RALU.srtock(in.CLKPIPE, RAJO, SUPU, SETU_Q);
    bool SOHU_Q = SOHU.srtock(in.CLKPIPE, RAGA, RYJY, RALU_Q);

    out.BG_PIX_B_7 = SOHU_Q;

    bool MYDE_Q = MYDE.srtock(in.CLKPIPE, LAKY, LOTY, in.P10_B);
    bool NOZO_Q = NOZO.srtock(in.CLKPIPE, NYXO, NEXA, MYDE_Q);
    bool MOJU_Q = MOJU.srtock(in.CLKPIPE, LOTO, LUTU, NOZO_Q);
    bool MACU_Q = MACU.srtock(in.CLKPIPE, LYDU, LUJA, MOJU_Q);
    bool NEPO_Q = NEPO.srtock(in.CLKPIPE, MYVY, MOSY, MACU_Q);
    bool MODU_Q = MODU.srtock(in.CLKPIPE, LODO, LERU, NEPO_Q);
    bool NEDA_Q = NEDA.srtock(in.CLKPIPE, NUTE, NYHA, MODU_Q);
    bool PYBO_Q = PYBO.srtock(in.CLKPIPE, NAJA, NADY, NEDA_Q);

    out.BG_PIX_A_7 = PYBO_Q;
  }
};