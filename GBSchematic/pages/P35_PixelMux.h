#pragma once

struct Gameboy;

struct P35_PixelMux {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool POKA; bool BGPIXELn; };

private:

  bool WOXA,XULA,TADE,RAJY;
  bool WELE,VUMU,NULY,RUTA,RYFU;
  bool LAVA,WOLO,WYRU;
  bool VYRO,VATA,VOLO,VUGO;
  bool WUFU,WALY;
  bool MABY,LUKU,MEXA,LOZO,LYLE,LEDO,LYKY,LOPU,LARU;
  bool MOKA,MUFA;
  bool SOBA,VYCO;
  bool MUVE,NUPO,NALE,NYPO,NUXO,NUMA,POBU,NURA,NELO;
  bool PATY,PERO;
  bool RAVO,REMY;
};
