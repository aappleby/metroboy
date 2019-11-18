#pragma once

struct Gameboy;

struct P34_SpritePaletteShifter {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool LAFU; bool OBP0PIXELn; };
  union { bool LEKA; bool OBP1PIXELn; };

  bool LESY,LOTA,LYKU,ROBY,TYTA,TYCO,SOKA,XOVU;

  bool MEFU,MEVE,MYZO,RUDA,VOTO,VYSA,TORY,WOPE;
  bool SYPY,TOTU,NARO,WEXY,RYZY,RYFE,LADY,LAFY;
  bool PUME,SUCO,SORO,TAFA,PAMO,PYZY,SUKY,TOWA,RORA,RUDU,MENE,PAZO,LUKE,LOWA,LAMY,LUNU;
  bool RUGO,SATA,ROSA,SOMY,PALU,NUKE,MODA,LYME;
  bool LOME;
};



