#pragma once

struct Gameboy;

struct P36_Palettes {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool MENA; bool BGP_D7; };
  union { bool MORU; bool BGP_D5; };
  union { bool MAXY; bool BGP_D3; };
  union { bool NUSY; bool BGP_D1; };
  union { bool MOGY; bool BGP_D6; };
  union { bool MUKE; bool BGP_D4; };
  union { bool PYLU; bool BGP_D2; };
  union { bool PAVO; bool BGP_D0; };

  union { bool XANA; bool OBP0_D7; };
  union { bool XYZE; bool OBP0_D5; };
  union { bool XALO; bool OBP0_D3; };
  union { bool XUKY; bool OBP0_D1; };
  union { bool XUPO; bool OBP0_D6; };
  union { bool XERU; bool OBP0_D4; };
  union { bool XOVA; bool OBP0_D2; };
  union { bool XUFU; bool OBP0_D0; };

  union { bool LUXO; bool OBP1_D7; };
  union { bool LUGU; bool OBP1_D5; };
  union { bool LOSE; bool OBP1_D3; };
  union { bool LAWO; bool OBP1_D1; };
  union { bool LEPU; bool OBP1_D6; };
  union { bool LUNE; bool OBP1_D4; };
  union { bool MOSA; bool OBP1_D2; };
  union { bool MOXY; bool OBP1_D0; };

  bool VUSO,VELY,TEPY,TEPO;
  bool LARY,LYKA,LOBE,PABA,LODY,LACE,REDO,RARO;
  bool XUFY,XOMA,XOZY,XELO;
  bool XAWO,XOBO,XUBY,XOKE,XAXA,XAJU,XUNO,XARY;
  bool MUMY,MYXE,LOTE,LEHO;
  bool LELU,LUGA,LYZA,LEPA,LEBA,LUKY,LODE,LAJU;
};
