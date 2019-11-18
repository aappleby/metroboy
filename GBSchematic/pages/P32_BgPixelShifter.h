#pragma once

struct Gameboy;

struct P32_BgPixelShifter {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool SOHU; bool BG_PIX_B_7; };
  union { bool PYBO; bool BG_PIX_A_7; };

  bool RAWU,POZO,PYZO,POXA,PULO,POJU,POWY,PYJU;

  bool LESO,AJAR,LABU,METE,LOMA,LOZE,LUXA;
  bool LEGU,NUDU,MUKU,LUZO,MEGU,MYJY,NASA,NEFO;
  bool TOSA,RUCO,TYCE,REVY,RYGA,RYLE,RAPU,SOJA;
  bool LUHE,NOLY,LEKE,LOMY,LALA,LOXA,NEZE,NOBO;
  bool SEJA,SENO,SURE,SEBO,SUCA,SYWE,SUPU,RYJY;
  bool TUXE,SOLY,RUCE,RYJA,RUTO,RAJA,RAJO,RAGA;
  bool LOTY,NEXA,LUTU,LUJA,MOSY,LERU,NYHA,NADY;
  bool LAKY,NYXO,LOTO,LYDU,MYVY,LODO,NUTE,NAJA;
  bool TOMY,TACA,SADY,RYSA,SOBO,SETU,RALU;
  bool MYDE,NOZO,MOJU,MACU,NEPO,MODU,NEDA;
};

