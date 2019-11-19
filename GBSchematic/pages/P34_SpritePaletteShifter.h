#pragma once

struct Gameboy;

struct P34_SpritePaletteShifter {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool LAFU; bool OBP0PIXELn; };
  union { bool LEKA; bool OBP1PIXELn; };

  /*p34.LESY*/ bool LESY;
  /*p34.LOTA*/ bool LOTA;
  /*p34.LYKU*/ bool LYKU;
  /*p34.ROBY*/ bool ROBY;
  /*p34.TYTA*/ bool TYTA;
  /*p34.TYCO*/ bool TYCO;
  /*p34.SOKA*/ bool SOKA;
  /*p34.XOVU*/ bool XOVU;


  /*p34.MEFU*/ bool MEFU;
  /*p34.MEVE*/ bool MEVE;
  /*p34.MYZO*/ bool MYZO;
  /*p34.RUDA*/ bool RUDA;
  /*p34.VOTO*/ bool VOTO;
  /*p34.VYSA*/ bool VYSA;
  /*p34.TORY*/ bool TORY;
  /*p34.WOPE*/ bool WOPE;

  /*p34.SYPY*/ bool SYPY;
  /*p34.TOTU*/ bool TOTU;
  /*p34.NARO*/ bool NARO;
  /*p34.WEXY*/ bool WEXY;
  /*p34.RYZY*/ bool RYZY;
  /*p34.RYFE*/ bool RYFE;
  /*p34.LADY*/ bool LADY;
  /*p34.LAFY*/ bool LAFY;

  /*p34.PUME*/ bool PUME;
  /*p34.SUCO*/ bool SUCO;
  /*p34.SORO*/ bool SORO;
  /*p34.TAFA*/ bool TAFA;
  /*p34.PAMO*/ bool PAMO;
  /*p34.PYZY*/ bool PYZY;
  /*p34.SUKY*/ bool SUKY;
  /*p34.TOWA*/ bool TOWA;
  /*p34.RORA*/ bool RORA;
  /*p34.RUDU*/ bool RUDU;
  /*p34.MENE*/ bool MENE;
  /*p34.PAZO*/ bool PAZO;
  /*p34.LUKE*/ bool LUKE;
  /*p34.LOWA*/ bool LOWA;
  /*p34.LAMY*/ bool LAMY;
  /*p34.LUNU*/ bool LUNU;

  /*p34.RUGO*/ bool RUGO;
  /*p34.SATA*/ bool SATA;
  /*p34.ROSA*/ bool ROSA;
  /*p34.SOMY*/ bool SOMY;
  /*p34.PALU*/ bool PALU;
  /*p34.NUKE*/ bool NUKE;
  /*p34.MODA*/ bool MODA;
  /*p34.LYME*/ bool LYME;

  /*p34.LOME*/ bool LOME;
};



