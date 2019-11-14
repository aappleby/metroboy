#pragma once

struct Gameboy;

struct P07_SysDecode {
public:

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  //----------
  // outputs

  union { /*bool UBET;*/ bool T1n; };
  union { /*bool UVAR;*/ bool T2n; };
  union { /*bool UPOJ; bool T1nT2n; */ bool MODE_PROD; };
  union { /*bool UMUT; bool T1T2n;  */ bool MODE_DBG1; };
  union { /*bool UNOR; bool T1nT2;  */ bool MODE_DBG2; };
  union { /*bool APET;*/ bool MODE_DEBUG; };

  union { /*bool ZETE;*/ bool BOOTROM_A1nA0n; };
  union { /*bool ZEFU;*/ bool BOOTROM_A1nA0;  };
  union { /*bool ZYRO;*/ bool BOOTROM_A1A0n;  };
  union { /*bool ZAPA;*/ bool BOOTROM_A1A0;   };
  union { /*bool ZOKE;*/ bool BOOTROM_A2n;    };
  union { /*bool ZABU;*/ bool BOOTROM_A3n;    };
  union { /*bool ZYKY;*/ bool BOOTROM_A5nA4n; };
  union { /*bool ZYGA;*/ bool BOOTROM_A5nA4;  };
  union { /*bool ZOVY;*/ bool BOOTROM_A5A4n;  };
  union { /*bool ZUKO;*/ bool BOOTROM_A5A4;   };
  union { /*bool ZAGE;*/ bool BOOTROM_A6n;    };
  union { /*bool ZYRA;*/ bool BOOTROM_A7n;    };

  
  union { /*bool TEDO;*/ bool CPU_RD;  };
  union { /*bool TAPU;*/ bool CPU_WR;  };
  union { /*bool ASOT;*/ bool CPU_RD2; };
  union { /*bool CUPA;*/ bool CPU_WR2; };
  
  union { /*bool SYKE;*/ bool ADDR_FFXX;      };
  union { /*bool TUTU;*/ bool READ_BOOTROM; };
  union { /*bool ZERY;*/ bool BOOT_CS;   };
  union { /*bool WUTA;*/ bool HRAM_CS;   };
  union { /*bool TUNA;*/ bool ADDR_0000_FE00; };

  union { /*bool BAKO;*/ bool FFXXn1; };
  union { /*bool SOHA;*/ bool FFXXn2; };

  union { /*bool SARO;*/ bool ADDR_OAM; };

  union { /*bool ROLO;*/ bool FF0F_RD; };
  union { /*bool REFA;*/ bool FF0F_WR; };

  //----------
  // registers

  // FF60 - secret debug register
  union { /*bool BURO;*/ bool FF60_D0; };
  union { /*bool AMUT;*/ bool FF60_D1; };

  union { /*bool AJAS;*/ bool CPU_RDn; };
  union { /*bool UBAL;*/ bool CPU_WR_MUX; };
  union { /*bool UJYV;*/ bool CPU_RD_MUX; };
  union { /*bool DYKY;*/ bool CPU_WRn; };

private:

  union { /*bool TEPU;*/ bool DISABLE_BOOTROM1; };
  union { /*bool SYPU;*/ bool DISABLE_BOOTROM2; };
  union { /*bool TERA;*/ bool ENABLE_BOOTROM1; };

  union { /*bool APER;*/ bool FF60_WRn; };

  union { /*bool ZYBA;*/ bool ADDR_00n; };
  union { /*bool ZUVY;*/ bool ADDR_01n; };
  union { /*bool ZUFY;*/ bool ADDR_04n; };
  union { /*bool ZERA;*/ bool ADDR_05n; };
  union { /*bool TONA;*/ bool ADDR_08n; };

  union { /*bool TULO;*/ bool ADDR_00XX; };
  union { /*bool ZUFA;*/ bool ADDR_00XX2; };
  union { /*bool ZORO;*/ bool ADDR_0XXX; };
  union { /*bool ZADU;*/ bool ADDR_X0XX; };
  union { /*bool SEMY;*/ bool ADDR_XX0X; };
  union { /*bool SAPA;*/ bool ADDR_XXXF; };
  union { /*bool RYCU;*/ bool ADDR_FE00_FFFF; };
  union { /*bool ROPE;*/ bool ADDR_FEXXn; };

  bool TYRO,TUFA,TUGE,SATO,TEXE;
  bool YAZA,YULA,ZADO;
  bool WALE,WOLY;
  bool LECO;
  bool ROMY_00,RYNE_01,REJY_02,RASE_03,REKA_04,ROWE_05,RYKE_06,RARU_07;
  bool LEXY;
  bool ZOLE,ZAJE,ZUBU,ZAPY;

};
