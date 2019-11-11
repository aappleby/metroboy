#pragma once

struct P27_WindowMapLookup {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool PYRY; bool RESET_VIDEOn; };
  union { bool LENA; };
  union { bool MYMA; };
  union { bool XUHA; };
  union { bool VYNO; };
  union { bool VUJO; };
  union { bool VYMU; };
  union { bool MOSU; };
  union { bool LYRY; };
  union { bool MYVO; };
  union { bool SYLO; };
  union { bool ROXY; };
  union { bool POVA; };
  union { bool NETA; };
  union { bool POTU; };
  union { bool PORE; };
  union { bool VYPO; };

  union { bool MOFU; };
  union { bool NYDY; };
  union { bool NYXU; };

  bool SECA;

private:

  bool NAZE,PEBO,POMO,NEVU,NOJO,PAGA,PEZO,NUPA;
  bool PALO,NELE,PAFU,ROGE;
  bool SARY;
  bool REPU,REJO;
  bool NEZO,NORY,NONO,PASE,MYLO,PUWU,PUHO,NYTU;
  bool PUKY,NUFA,NOGY,NUKO;
  bool ROZE,PAHA,SUHA,SYBY,SOZU,PECU,RONE,POHU,MOXE,PANY,SEKO,ROMO,SUVU,TAVE,XAHY,XOFO,XACO,PYNU,NUNY,NOCU;
  bool PUKU,RYDY;
  bool TUXY,SUZU,TEVO,PASO,VETU,ROCO,MEHE,NYFO,WAZY,SYNY;
  bool NOPA,PYCO,NUNU;
  bool RYKU,ROGA,RUBU;
  bool NYZE,PUXA;
  bool RYFA,RENE,SOVY;
  bool MOCE,LEBO,LAXE,MYSO,NAKO,NOFU,NOGU,NENY,LURY,LONY,LUSU;
  bool LYZU,LAXU,MESU,NYVA,LOVY;
  bool WYKA,WODY,WOBO,WYKO,XOLO;
  bool TUFU,TAXA,TOZO,TATE,TEKE;
  bool XEJA,XAMO,XAHE,XULO,WUJU;
  bool VYTO,VEHA,VACE,VOVO,VULO;
  bool VEVY,VEZA,VOGU;
  bool RYCE,ROSY,VEKU,TAKA,TUKU,SOWO,TEKY,TAVA,SOBU,SUDA;
};



