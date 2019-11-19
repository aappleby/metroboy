#pragma once

struct Gameboy;

struct P35_PixelMux {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool POKA; bool BGPIXELn; };

  /*p35.WOXA*/ bool WOXA;
  /*p35.XULA*/ bool XULA;
  /*p35.TADE*/ bool TADE;
  /*p35.RAJY*/ bool RAJY;

  /*p35.WELE*/ bool WELE;
  /*p35.VUMU*/ bool VUMU;
  /*p35.NULY*/ bool NULY;
  /*p35.RUTA*/ bool RUTA;
  /*p35.RYFU*/ bool RYFU;

  /*p35.LAVA*/ bool LAVA;
  /*p35.WOLO*/ bool WOLO;
  /*p35.WYRU*/ bool WYRU;

  /*p35.VYRO*/ bool VYRO;
  /*p35.VATA*/ bool VATA;
  /*p35.VOLO*/ bool VOLO;
  /*p35.VUGO*/ bool VUGO;

  /*p35.WUFU*/ bool WUFU;
  /*p35.WALY*/ bool WALY;

  /*p35.MABY*/ bool MABY;
  /*p35.LUKU*/ bool LUKU;
  /*p35.MEXA*/ bool MEXA;
  /*p35.LOZO*/ bool LOZO;
  /*p35.LYLE*/ bool LYLE;
  /*p35.LEDO*/ bool LEDO;
  /*p35.LYKY*/ bool LYKY;
  /*p35.LOPU*/ bool LOPU;
  /*p35.LARU*/ bool LARU;

  /*p35.MOKA*/ bool MOKA;
  /*p35.MUFA*/ bool MUFA;

  /*p35.SOBA*/ bool SOBA;
  /*p35.VYCO*/ bool VYCO;

  /*p35.MUVE*/ bool MUVE;
  /*p35.NUPO*/ bool NUPO;
  /*p35.NALE*/ bool NALE;
  /*p35.NYPO*/ bool NYPO;
  /*p35.NUXO*/ bool NUXO;
  /*p35.NUMA*/ bool NUMA;
  /*p35.POBU*/ bool POBU;
  /*p35.NURA*/ bool NURA;
  /*p35.NELO*/ bool NELO;

  /*p35.PATY*/ bool PATY;
  /*p35.PERO*/ bool PERO;

  /*p35.RAVO*/ bool RAVO;
  /*p35.REMY*/ bool REMY;
};
