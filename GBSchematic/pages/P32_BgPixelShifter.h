#pragma once

struct Gameboy;

struct P32_BgPixelShifter {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool SOHU; bool BG_PIX_B_7; };
  union { bool PYBO; bool BG_PIX_A_7; };

  /*p32.RAWU*/ bool RAWU;
  /*p32.POZO*/ bool POZO;
  /*p32.PYZO*/ bool PYZO;
  /*p32.POXA*/ bool POXA;
  /*p32.PULO*/ bool PULO;
  /*p32.POJU*/ bool POJU;
  /*p32.POWY*/ bool POWY;
  /*p32.PYJU*/ bool PYJU;


  /*p32.LESO*/ bool LESO;
  /*p32.AJAR*/ bool AJAR;
  /*p32.LABU*/ bool LABU;
  /*p32.METE*/ bool METE;
  /*p32.LOMA*/ bool LOMA;
  /*p32.LOZE*/ bool LOZE;
  /*p32.LUXA*/ bool LUXA;

  /*p32.LEGU*/ bool LEGU;
  /*p32.NUDU*/ bool NUDU;
  /*p32.MUKU*/ bool MUKU;
  /*p32.LUZO*/ bool LUZO;
  /*p32.MEGU*/ bool MEGU;
  /*p32.MYJY*/ bool MYJY;
  /*p32.NASA*/ bool NASA;
  /*p32.NEFO*/ bool NEFO;

  /*p32.TOSA*/ bool TOSA;
  /*p32.RUCO*/ bool RUCO;
  /*p32.TYCE*/ bool TYCE;
  /*p32.REVY*/ bool REVY;
  /*p32.RYGA*/ bool RYGA;
  /*p32.RYLE*/ bool RYLE;
  /*p32.RAPU*/ bool RAPU;
  /*p32.SOJA*/ bool SOJA;

  /*p32.LUHE*/ bool LUHE;
  /*p32.NOLY*/ bool NOLY;
  /*p32.LEKE*/ bool LEKE;
  /*p32.LOMY*/ bool LOMY;
  /*p32.LALA*/ bool LALA;
  /*p32.LOXA*/ bool LOXA;
  /*p32.NEZE*/ bool NEZE;
  /*p32.NOBO*/ bool NOBO;

  /*p32.SEJA*/ bool SEJA;
  /*p32.SENO*/ bool SENO;
  /*p32.SURE*/ bool SURE;
  /*p32.SEBO*/ bool SEBO;
  /*p32.SUCA*/ bool SUCA;
  /*p32.SYWE*/ bool SYWE;
  /*p32.SUPU*/ bool SUPU;
  /*p32.RYJY*/ bool RYJY;

  /*p32.TUXE*/ bool TUXE;
  /*p32.SOLY*/ bool SOLY;
  /*p32.RUCE*/ bool RUCE;
  /*p32.RYJA*/ bool RYJA;
  /*p32.RUTO*/ bool RUTO;
  /*p32.RAJA*/ bool RAJA;
  /*p32.RAJO*/ bool RAJO;
  /*p32.RAGA*/ bool RAGA;

  /*p32.LOTY*/ bool LOTY;
  /*p32.NEXA*/ bool NEXA;
  /*p32.LUTU*/ bool LUTU;
  /*p32.LUJA*/ bool LUJA;
  /*p32.MOSY*/ bool MOSY;
  /*p32.LERU*/ bool LERU;
  /*p32.NYHA*/ bool NYHA;
  /*p32.NADY*/ bool NADY;

  /*p32.LAKY*/ bool LAKY;
  /*p32.NYXO*/ bool NYXO;
  /*p32.LOTO*/ bool LOTO;
  /*p32.LYDU*/ bool LYDU;
  /*p32.MYVY*/ bool MYVY;
  /*p32.LODO*/ bool LODO;
  /*p32.NUTE*/ bool NUTE;
  /*p32.NAJA*/ bool NAJA;

  /*p32.TOMY*/ bool TOMY;
  /*p32.TACA*/ bool TACA;
  /*p32.SADY*/ bool SADY;
  /*p32.RYSA*/ bool RYSA;
  /*p32.SOBO*/ bool SOBO;
  /*p32.SETU*/ bool SETU;
  /*p32.RALU*/ bool RALU;

  /*p32.MYDE*/ bool MYDE;
  /*p32.NOZO*/ bool NOZO;
  /*p32.MOJU*/ bool MOJU;
  /*p32.MACU*/ bool MACU;
  /*p32.NEPO*/ bool NEPO;
  /*p32.MODU*/ bool MODU;
  /*p32.NEDA*/ bool NEDA;
};

