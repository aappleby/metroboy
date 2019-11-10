#pragma once
#include "../Schematics.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P04_DMA {

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  union { bool CATY; };
  union { bool WYJA; };
  union { bool MOPA; bool MOPA_PHI; };
  union { bool LUMA; };
  union { bool DUGA; bool OAM_ADDR_DMA; };
  union { bool LUFA; bool VRAM_TO_OAM; };

  union { bool NAKY_00; bool DMA_A00; };
  union { bool PYRO_01; bool DMA_A01; }; 
  union { bool NEFY_02; bool DMA_A02; }; 
  union { bool MUTY_03; bool DMA_A03; }; 
  union { bool NYKO_04; bool DMA_A04; }; 
  union { bool PYLO_05; bool DMA_A05; }; 
  union { bool NUTO_06; bool DMA_A06; }; 
  union { bool MUGU_07; bool DMA_A07; }; 
  union { bool NAFA_00; bool DMA_A08; }; 
  union { bool PYNE_01; bool DMA_A09; }; 
  union { bool PARA_02; bool DMA_A10; }; 
  union { bool NYDO_03; bool DMA_A11; }; 
  union { bool NYGY_04; bool DMA_A12; }; 
  union { bool PULA_05; bool DMA_A13; }; 
  union { bool POKU_06; bool DMA_A14; }; 
  union { bool MARU_07; bool DMA_A15; }; 

private:
  
  // DMA counter
  bool ECAL_00,EGEZ_01,FUHE_02,FYZY_03,DAMU_04,DAVA_05,ETEG_06,EREW_07;

  // FF46 DMA
  bool POLY_00,ROFO_01,REMA_02,PANE_03,PARE_04,RALY_05,RESU_06,NUVY_07;
  bool EVAX_08,DUVE_09,ERAF_10,FUSY_11,EXYF_12;

  // Misc registers
  bool MAKA;
  bool LUVY;
  bool MYTE;
  bool LENE;
  bool MATU;

  bool DECY,NAXY,POWU,LUPA,MOLU,LAVY,NYGO,LORU,PUSY;
  bool AHOC,LOKO,LAPA,META;
  bool NAVO,NOLO,LARA,LOKY,MORY,LOGO,MUHO,MUDA,LEBU;
  bool LYXE;
};
