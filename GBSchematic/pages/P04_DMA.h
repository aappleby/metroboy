#pragma once
#include "../Schematics.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P04_DMA {

  // DMA counter
  bool NAKY_00,PYRO_01,NEFY_02,MUTY_03,NYKO_04,PYLO_05,NUTO_06,MUGU_07;
  bool ECAL_00,EGEZ_01,FUHE_02,FYZY_03,DAMU_04,DAVA_05,ETEG_06,EREW_07;

  // FF46 DMA
  bool NAFA_00,PYNE_01,PARA_02,NYDO_03,NYGY_04,PULA_05,POKU_06,MARU_07;
  bool POLY_00,ROFO_01,REMA_02,PANE_03,PARE_04,RALY_05,RESU_06,NUVY_07;
  bool EVAX_08,DUVE_09,ERAF_10,FUSY_11,EXYF_12;

  // Misc registers
  bool MAKA;
  bool LUVY;
  bool MYTE;
  bool LENE;
  bool MATU;

  bool DECY,CATY,NAXY,POWU,LUPA,MOLU,LAVY,NYGO,LORU,PUSY;
  bool AHOC,LOKO,LAPA,META;
  bool MOPA,NAVO,NOLO,LARA,LOKY,MORY,LUMA,LOGO,DUGA,LUFA,MUHO,MUDA,LEBU;
  bool LYXE,WYJA;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};
