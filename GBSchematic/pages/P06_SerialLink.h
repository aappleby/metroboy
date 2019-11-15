#pragma once

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P06_SerialLink {

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  union { /*bool SARE;*/ bool ADDR_XX00_XX07; };

  // counter that triggers INT_SERIAL
  union { /*bool CAFA;*/ bool SER_CNT0; }; 
  union { /*bool CYLO;*/ bool SER_CNT1; }; 
  union { /*bool CYDE;*/ bool SER_CNT2; }; 
  union { /*bool CALY;*/ bool SER_CNT3; bool INT_SERIAL; };
  union { /*bool COBA;*/ bool SER_CNT3n; };

  union { /*bool COTY;*/ bool SER_CLK; };
  union { /*bool CAVE;*/ bool SER_CLK_MUXn; };
  union { /*bool DAWA;*/ bool SER_TICK; };
  union { /*bool EDYL;*/ bool SER_TICKn; };
  union { /*bool EPYT;*/ bool SER_TICK2; };
  union { /*bool DEHO;*/ bool SER_TICKn2; };
  union { /*bool DAWE;*/ bool SER_TICK3; };

  union { /*bool ELYS;*/ bool SER_OUT; };

  // FF01 SB
  union { /*bool URYS;*/ bool FF01_WRn; };
  union { /*bool DAKU;*/ bool FF01_WR; };
  union { /*bool UFEG;*/ bool FF01_RD; };

  union { /*bool SEFY;*/ bool A02n; };

  union { /*bool SANO;*/ bool ADDR_FF00_FF03; };


  union { /*bool CAGE;*/ bool SIN_Cn; };

  union { /*bool CUFU;*/ bool SER_DATA0_SETn; };
  union { /*bool DOCU;*/ bool SER_DATA1_SETn; };
  union { /*bool DELA;*/ bool SER_DATA2_SETn; };
  union { /*bool DYGE;*/ bool SER_DATA3_SETn; };
  union { /*bool DOLA;*/ bool SER_DATA4_SETn; };
  union { /*bool ELOK;*/ bool SER_DATA5_SETn; };
  union { /*bool EDEL;*/ bool SER_DATA6_SETn; };
  union { /*bool EFEF;*/ bool SER_DATA7_SETn; };

  union { /*bool COHY;*/ bool SER_DATA0_RSTn; };
  union { /*bool DUMO;*/ bool SER_DATA1_RSTn; };
  union { /*bool DYBO;*/ bool SER_DATA2_RSTn; };
  union { /*bool DAJU;*/ bool SER_DATA3_RSTn; };
  union { /*bool DYLY;*/ bool SER_DATA4_RSTn; };
  union { /*bool EHUJ;*/ bool SER_DATA5_RSTn; };
  union { /*bool EFAK;*/ bool SER_DATA6_RSTn; };
  union { /*bool EGUV;*/ bool SER_DATA7_RSTn; };
  
  union { /*bool CUBA;*/ bool SER_DATA0; };
  union { /*bool DEGU;*/ bool SER_DATA1; };
  union { /*bool DYRA;*/ bool SER_DATA2; };
  union { /*bool DOJO;*/ bool SER_DATA3; };
  union { /*bool DOVU;*/ bool SER_DATA4; };
  union { /*bool EJAB;*/ bool SER_DATA5; };
  union { /*bool EROD;*/ bool SER_DATA6; };
  union { /*bool EDER;*/ bool SER_DATA7; };
  
  union { /*bool CUGY;*/ bool FF01_D0; };
  union { /*bool DUDE;*/ bool FF01_D1; };
  union { /*bool DETU;*/ bool FF01_D2; };
  union { /*bool DASO;*/ bool FF01_D3; };
  union { /*bool DAME;*/ bool FF01_D4; };
  union { /*bool EVOK;*/ bool FF01_D5; };
  union { /*bool EFAB;*/ bool FF01_D6; };
  union { /*bool ETAK;*/ bool FF01_D7; };

  // FF02 SC
  union { /*bool CULY;*/ bool SCK_B; };
  union { /*bool ETAF;*/ bool XFER_START; };
  union { /*bool CORE;*/ bool FF02_D0; };
  union { /*bool ELUV;*/ bool FF02_D7; };

  union { /*bool UWAM;*/ bool FF02_WRn; };
  union { /*bool UCOM;*/ bool FF02_RD; };
  
  union { /*bool KEXU;*/ bool SCK_A; };
  union { /*bool JAGO;*/ bool SCK_Bn; };
  union { /*bool KUJO;*/ bool SCK_D; };

  union { /*bool CARO;*/ bool SER_RST; };
  union { /*bool CABY;*/ bool XFER_RESET; };

};
