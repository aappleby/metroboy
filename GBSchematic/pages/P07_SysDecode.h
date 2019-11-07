#pragma once
struct Gameboy;

struct P07_SysDecode {
#if 0
  struct Input {
    bool BEDO;
    bool P10_B;
    bool FFXX;
    bool RESET2;
    bool RESET;
    bool CPU_WR_RAW;
    bool ANAP;
    bool NET02; // FIXME this should be T1nT2
    bool WR_IN;
    bool CPU_RD_SYNC;
    bool CPU_RAW_RD;
    bool RD_B;
    bool FROM_CPU6;

    bool T1;
    bool T2;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool CPU_RD;
    bool CPU_WR;
    bool CPU_RD2;
    bool CPU_WR2;

    bool PIN_NC;

    bool BOOT_CS;
    bool HRAM_CS;
    bool T1nT2n;
    bool T1nT2;
    bool T1T2n;

    bool FF60_D0;
    bool FF60_D1;

    bool FFXX;
    bool FFXXn;
    bool SARO;
    bool FEXXFFXXn;
    bool FF0F_RD;
    bool FF0F_WR;

    bool BOOTROM_A1A0;
    bool BOOTROM_A1A0n;
    bool BOOTROM_A1nA0;
    bool BOOTROM_A1nA0n;
    bool BOOTROM_A2n;
    bool BOOTROM_A3n;

    bool BOOTROM_A5AA4;
    bool BOOTROM_A5A4n;
    bool BOOTROM_A5nA4;
    bool BOOTROM_A5nA4n;
    bool BOOTROM_A6n;
    bool BOOTROM_A7n;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };
#endif

  // FF50 - the "enable bootrom" register
  bool TEPU;

  // FF60 - secret debug register
  bool APET,APER,BURO_00,AMUT_01;

  bool UBET,UVAR,UPOJ,UNOR,UMUT;

  bool TYRO,TUFA,TUGE,SATO,TEXE,SYPU,TERA,TULO;
  bool TUTU,ZORO,ZADU,YAZA,YULA,ZUFA,ZADO,ZERY;

  bool SEMY,SAPA,ROLO,REFA;

  bool WALE,WOLY,WUTA;

  bool TONA,TUNA,SYKE,RYCU,SOHA,ROPE,BAKO,SARO;

  bool LECO;
  bool ROMY_00,RYNE_01,REJY_02,RASE_03,REKA_04,ROWE_05,RYKE_06,RARU_07;

  bool UBAL,TAPU,DYKY,CUPA,UJYV,TEDO,AJAS,ASOT,LEXY;

  bool ZYRA,ZAGE,ZABU,ZOKE,ZERA,ZUFY,ZYKY,ZYGA,ZOVY,ZUKO;
  bool ZUVY,ZYBA,ZOLE,ZAJE,ZUBU,ZAPY,ZETE,ZEFU,ZYRO,ZAPA;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};



