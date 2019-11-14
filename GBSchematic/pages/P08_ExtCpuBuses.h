#pragma once
#include "Common.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P08_ExtCpuBuses {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { /*bool TOLA;*/ bool A1n; };
  union { /*bool TOVA;*/ bool MODE_DBG2n1; bool NET01; };
  union { /*bool RYCA;*/ bool MODE_DBG2n2; };

  // True if the internal data bus is driving the external data bus
  union { /*bool LULA;*/ bool DBUS_OUT; }; 
  union { /*bool RORU;*/ bool DBUS_OUTn; };

  bool TEXO;

//private:

  bool SOBY_15;

  // Address latch
  union { /*bool ALOR;*/ bool ADDR_LATCH_00; };
  union { /*bool APUR;*/ bool ADDR_LATCH_01; };
  union { /*bool ALYR;*/ bool ADDR_LATCH_02; };
  union { /*bool ARET;*/ bool ADDR_LATCH_03; };
  union { /*bool AVYS;*/ bool ADDR_LATCH_04; };
  union { /*bool ATEV;*/ bool ADDR_LATCH_05; };
  union { /*bool AROS;*/ bool ADDR_LATCH_06; };
  union { /*bool ARYM;*/ bool ADDR_LATCH_07; };
  union { /*bool LUNO;*/ bool ADDR_LATCH_08; };
  union { /*bool LYSA;*/ bool ADDR_LATCH_09; };
  union { /*bool PATE;*/ bool ADDR_LATCH_10; };
  union { /*bool LUMY;*/ bool ADDR_LATCH_11; };
  union { /*bool LOBU;*/ bool ADDR_LATCH_12; };
  union { /*bool LONU;*/ bool ADDR_LATCH_13; };
  union { /*bool NYRE;*/ bool ADDR_LATCH_14; };
  union { /*bool SEPY;*/ bool ADDR_LATCH_15; };

  // Addres mux
  union { /*bool AMET;*/ bool ADDR_MUX_00; };
  union { /*bool ATOL;*/ bool ADDR_MUX_01; };
  union { /*bool APOK;*/ bool ADDR_MUX_02; };
  union { /*bool AMER;*/ bool ADDR_MUX_03; };
  union { /*bool ATEM;*/ bool ADDR_MUX_04; };
  union { /*bool ATOV;*/ bool ADDR_MUX_05; };
  union { /*bool ATYR;*/ bool ADDR_MUX_06; };
  union { /*bool ASUR;*/ bool ADDR_MUX_07; };
  union { /*bool MANO;*/ bool ADDR_MUX_08; };
  union { /*bool MASU;*/ bool ADDR_MUX_09; };
  union { /*bool PAMY;*/ bool ADDR_MUX_10; };
  union { /*bool MALE;*/ bool ADDR_MUX_11; };
  union { /*bool MOJY;*/ bool ADDR_MUX_12; };
  union { /*bool MUCE;*/ bool ADDR_MUX_13; };
  union { /*bool PEGE;*/ bool ADDR_MUX_14; };
  union { /*bool TAZY;*/ bool ADDR_MUX_15; };


  // AXX_A
  union { /*bool KUPO;*/ bool PIN_A00_A; };
  union { /*bool CABA;*/ bool PIN_A01_A; };
  union { /*bool BOKU;*/ bool PIN_A02_A; };
  union { /*bool BOTY;*/ bool PIN_A03_A; };
  union { /*bool BYLA;*/ bool PIN_A04_A; };
  union { /*bool BADU;*/ bool PIN_A05_A; };
  union { /*bool CEPU;*/ bool PIN_A06_A; };
  union { /*bool DEFY;*/ bool PIN_A07_A; };
  union { /*bool MYNY;*/ bool PIN_A08_A; };
  union { /*bool MUNE;*/ bool PIN_A09_A; };
  union { /*bool ROXU;*/ bool PIN_A10_A; };
  union { /*bool LEPY;*/ bool PIN_A11_A; };
  union { /*bool LUCE;*/ bool PIN_A12_A; };
  union { /*bool LABE;*/ bool PIN_A13_A; };
  union { /*bool PUHE;*/ bool PIN_A14_A; };
  union { /*bool SUZE;*/ bool PIN_A15_A; };

  // AXX_D
  union { /*bool KOTY;*/ bool PIN_A00_D; };
  union { /*bool COTU;*/ bool PIN_A01_D; };
  union { /*bool BAJO;*/ bool PIN_A02_D; };
  union { /*bool BOLA;*/ bool PIN_A03_D; };
  union { /*bool BEVO;*/ bool PIN_A04_D; };
  union { /*bool AJAV;*/ bool PIN_A05_D; };
  union { /*bool CYKA;*/ bool PIN_A06_D; };
  union { /*bool COLO;*/ bool PIN_A07_D; };
  union { /*bool MEGO;*/ bool PIN_A08_D; };
  union { /*bool MENY;*/ bool PIN_A09_D; };
  union { /*bool RORE;*/ bool PIN_A10_D; };
  union { /*bool LYNY;*/ bool PIN_A11_D; };
  union { /*bool LOSO;*/ bool PIN_A12_D; };
  union { /*bool LEVA;*/ bool PIN_A13_D; };
  union { /*bool PAHY;*/ bool PIN_A14_D; };
  union { /*bool RULO;*/ bool PIN_A15_D; };

  // Data output driver
  union { /*bool RUXA_00;*/ bool D0_A; };
  union { /*bool RUJA_01;*/ bool D1_A; };
  union { /*bool RABY_02;*/ bool D2_A; };
  union { /*bool RERA_03;*/ bool D3_A; };
  union { /*bool RORY_04;*/ bool D4_A; };
  union { /*bool RYVO_05;*/ bool D5_A; };
  union { /*bool RAFY_06;*/ bool D6_A; };
  union { /*bool RAVU_07;*/ bool D7_A; };

  union { /*bool RUNE_00;*/ bool D0_D; };
  union { /*bool RYPU_01;*/ bool D1_D; };
  union { /*bool SULY_02;*/ bool D2_D; };
  union { /*bool SEZE_03;*/ bool D3_D; };
  union { /*bool RESY_04;*/ bool D4_D; };
  union { /*bool TAMU_05;*/ bool D5_D; };
  union { /*bool ROGY_06;*/ bool D6_D; };
  union { /*bool RYDA_07;*/ bool D7_D; };

  // DX_C
  union {
    // TOVO,RUZY,ROME,SAZA,TEHE,RATU,SOCA,RYBA
    struct
    {
      bool D0_Cn : 1;
      bool D1_Cn : 1;
      bool D2_Cn : 1;
      bool D3_Cn : 1;
      bool D4_Cn : 1;
      bool D5_Cn : 1;
      bool D6_Cn : 1;
      bool D7_Cn : 1;
    };
  };

  union {
    // TUTY,SYWA,SUGU,TAWO,TUTE,SAJO,TEMY,ROPA
    struct
    {
      bool D0_C : 1;
      bool D1_C : 1;
      bool D2_C : 1;
      bool D3_C : 1;
      bool D4_C : 1;
      bool D5_C : 1;
      bool D6_C : 1;
      bool D7_C : 1;
    };
  };


  union { /*bool SOMA;*/ bool LATCH_D0; };
  union { /*bool RONY;*/ bool LATCH_D1; };
  union { /*bool RAXY;*/ bool LATCH_D2; };
  union { /*bool SELO;*/ bool LATCH_D3; };
  union { /*bool SODY;*/ bool LATCH_D4; };
  union { /*bool SAGO;*/ bool LATCH_D5; };
  union { /*bool RUPA;*/ bool LATCH_D6; };
  union { /*bool SAZY;*/ bool LATCH_D7; };
  
  union { /*bool RYMA;*/ bool BUS_D0; };
  union { /*bool RUVO;*/ bool BUS_D1; };
  union { /*bool RYKO;*/ bool BUS_D2; };
  union { /*bool TAVO;*/ bool BUS_D3; };
  union { /*bool TEPE;*/ bool BUS_D4; };
  union { /*bool SAFO;*/ bool BUS_D5; };
  union { /*bool SEVU;*/ bool BUS_D6; };
  union { /*bool TAJU;*/ bool BUS_D7; };

  // Debug, drives external address onto internal address
  union { /*bool KOVA;*/ bool A00_Cn; };
  union { /*bool CAMU;*/ bool A01_Cn; };
  union { /*bool BUXU;*/ bool A02_Cn; };
  union { /*bool BASE;*/ bool A03_Cn; };
  union { /*bool AFEC;*/ bool A04_Cn; };
  union { /*bool ABUP;*/ bool A05_Cn; };
  union { /*bool CYGU;*/ bool A06_Cn; };
  union { /*bool COGO;*/ bool A07_Cn; };
  union { /*bool MUJY;*/ bool A08_Cn; };
  union { /*bool NENA;*/ bool A09_Cn; };
  union { /*bool SURA;*/ bool A10_Cn; };
  union { /*bool MADY;*/ bool A11_Cn; };
  union { /*bool LAHE;*/ bool A12_Cn; };
  union { /*bool LURA;*/ bool A13_Cn; };
  union { /*bool PEVO;*/ bool A14_Cn; };
  union { /*bool RAZA;*/ bool A15_Cn; };

  union { /*bool KEJO;*/ bool A00_C; };
  union { /*bool BYXE;*/ bool A01_C; };
  union { /*bool AKAN;*/ bool A02_C; };
  union { /*bool ANAR;*/ bool A03_C; };
  union { /*bool AZUV;*/ bool A04_C; };
  union { /*bool AJOV;*/ bool A05_C; };
  union { /*bool BYNE;*/ bool A06_C; };
  union { /*bool BYNA;*/ bool A07_C; };
  union { /*bool LOFA;*/ bool A08_C; };
  union { /*bool MAPU;*/ bool A09_C; };
  union { /*bool RALA;*/ bool A10_C; };
  union { /*bool LORA;*/ bool A11_C; };
  union { /*bool LYNA;*/ bool A12_C; };
  union { /*bool LEFY;*/ bool A13_C; };
  union { /*bool NEFE;*/ bool A14_C; };
  union { /*bool SYZU;*/ bool A15_C; };

  union { /*bool SORE;*/ bool ADDR_0000_7FFF; };
  union { /*bool TEVY;*/ bool ADDR_NOT_VRAM; };

  bool LEVO,LAGU,LYWE,MOCA,MEXO,NEVY,MOTY,PUVA,TYMU;
  bool LOXO,LASY,MATE;
  bool SOGY,TYNU,TOZA;

  union { bool TUMA; bool CART_RAM; };

  bool LAVO;
  bool LYRA;

  union { bool TYHO; bool CS_A; };
  union { bool UVER; bool WR_A; };
  union { bool USUF; bool WR_D; };
  union { bool UGAC; bool RD_A; };
  union { bool URUN; bool RD_D; };

  union { bool MULE; bool MODE_DBG1n; };

  union { /*bool REDU;*/ bool CPU_RDn; };
};
