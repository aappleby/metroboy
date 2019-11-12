#pragma once

struct Gameboy;

struct P24_LcdControl {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool SACU; bool CLKPIPE; };
  union { bool TOFU; };
  union { bool ROXO; };
  union { bool POKY; };
  union { bool PORY; };
  union { bool TOMU; };
  union { bool NYKA; };
  union { bool SEGU; };
  union { bool LOBY; };

private:

  bool NAFY,PYGO,SOCY,VYBO,TYFA,PAHO;
  bool POME,RUJU,POFY,RUZE;
  bool NERU,MEDA,MURE;
  bool LOFU,LUCA,MAGU,LEBE,MECO,KEBO,KASA,UMOB,USEC,KAHE,KUPA,KYMO,KOFO;

  union { bool KEDY; bool LCD_OFF; };
};


