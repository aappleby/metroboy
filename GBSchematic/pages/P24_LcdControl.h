#pragma once

struct Gameboy;

struct P24_LcdControl {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool SACU; bool CLKPIPE; };
  union { bool TOFU; };

private:

  bool LOBY,NAFY,NYKA,PORY,PYGO,TOMU,POKY,SOCY,VYBO,TYFA,SEGU,ROXO,PAHO;
  bool POME,RUJU,POFY,RUZE;
  bool NERU,MEDA,MURE;
  bool LOFU,LUCA,MAGU,LEBE,MECO,KEBO,KASA,UMOB,USEC,KEDY,KAHE,KUPA,KYMO,KOFO;
};


