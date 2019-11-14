#pragma once
#include "../Schematics.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P04_DMA {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { /*bool DECY;*/ bool FROM_CPU5n; };
  union { /*bool CATY;*/ bool FROM_CPU5; };
  union { /*bool MAKA;*/ bool FROM_CPU5_SYNC; };

  union { bool WYJA; };
  union { /*bool MOPA;*/ bool PHI_OUT; };
  union { /*bool LUMA;*/ bool DO_DMA; }; // if true, addr bus = dma addr
  union { /*bool DUGA;*/ bool OAM_ADDR_DMA; };

  union { /*bool LUFA;*/ bool VRAM_TO_OAM; };
  union { /*bool AHOC;*/ bool VRAM_TO_OAMn; };

  union { /*bool NAKY;*/ bool DMA_A00; };
  union { /*bool PYRO;*/ bool DMA_A01; }; 
  union { /*bool NEFY;*/ bool DMA_A02; }; 
  union { /*bool MUTY;*/ bool DMA_A03; }; 
  union { /*bool NYKO;*/ bool DMA_A04; }; 
  union { /*bool PYLO;*/ bool DMA_A05; }; 
  union { /*bool NUTO;*/ bool DMA_A06; }; 
  union { /*bool MUGU;*/ bool DMA_A07; }; 
  union { /*bool NAFA;*/ bool DMA_A08; }; 
  union { /*bool PYNE;*/ bool DMA_A09; }; 
  union { /*bool PARA;*/ bool DMA_A10; }; 
  union { /*bool NYDO;*/ bool DMA_A11; }; 
  union { /*bool NYGY;*/ bool DMA_A12; }; 
  union { /*bool PULA;*/ bool DMA_A13; }; 
  union { /*bool POKU;*/ bool DMA_A14; }; 
  union { /*bool MARU;*/ bool DMA_A15; }; 

  union { bool MATU; }; // -> p25,p28

private:
  
  union { /*bool ECAL;*/ bool MA00; };
  union { /*bool EGEZ;*/ bool MA01; };
  union { /*bool FUHE;*/ bool MA02; };
  union { /*bool FYZY;*/ bool MA03; };
  union { /*bool DAMU;*/ bool MA04; };
  union { /*bool DAVA;*/ bool MA05; };
  union { /*bool ETEG;*/ bool MA06; };
  union { /*bool EREW;*/ bool MA07; };
  union { /*bool EVAX;*/ bool MA08; };
  union { /*bool DUVE;*/ bool MA09; };
  union { /*bool ERAF;*/ bool MA10; };
  union { /*bool FUSY;*/ bool MA11; };
  union { /*bool EXYF;*/ bool MA12; };

  union { /*bool POLY;*/ bool FF46_D0; };
  union { /*bool ROFO;*/ bool FF46_D1; };
  union { /*bool REMA;*/ bool FF46_D2; };
  union { /*bool PANE;*/ bool FF46_D3; };
  union { /*bool PARE;*/ bool FF46_D4; };
  union { /*bool RALY;*/ bool FF46_D5; };
  union { /*bool RESU;*/ bool FF46_D6; };
  union { /*bool NUVY;*/ bool FF46_D7; };

  union { bool MOLU; bool FF46_RDn1; };
  union { bool NYGO; bool FF46_RD; };
  union { bool PUSY; bool FF46_RDn2; };
  union { bool LAVY; bool FF46_WRn; };
  union { bool LORU; bool FF46_WR; };
  
  union { bool META; bool DMA_CLK; };
  union { bool LOKY; bool DMA_RUNNING; };
  union { bool NAVO; bool DMA_DONE; };
  union { bool NOLO; bool DMA_DONEn; };

  bool LUVY;
  bool MYTE;
  bool LENE;

  bool NAXY,POWU,LUPA;
  bool LOKO,LAPA;
  bool LARA,MORY,LOGO,MUHO,MUDA;

  union { bool LEBU; bool DMA_A15n; };

  bool LYXE;
};
 