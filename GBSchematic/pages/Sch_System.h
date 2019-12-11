#pragma once

namespace Schematics {

struct CpuIn;
struct ChipIn;
struct Gameboy;

//-----------------------------------------------------------------------------

struct System {

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.AFER*/ bool RESET_REG;

  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.XAPO*/ bool VID_RESETn;

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;
  /*p07.ASOT*/ bool CPU_RD2;
  /*p07.CUPA*/ bool CPU_WR2;

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  /*p01.ANOS*/ bool ROOTCLK_AxCxExGx;
  /*p01.AVET*/ bool ROOTCLK_xBxDxFxH;

  /*p01.ALET*/ bool CLK_AxCxExGx4; // video
  /*p01.ZEME*/ bool CLK_xBxDxFxH2; // sprites, video


  // Phase generator
  /*p01.ATAL*/ bool PHASE_CLK;
  /*p01.AFUR*/ bool PHASE_ABCDxxxx1;
  /*p01.ALEF*/ bool PHASE_xBCDExxx1;
  /*p01.APUK*/ bool PHASE_xxCDEFxx1;
  /*p01.ADYK*/ bool PHASE_xxxDEFGx1;

  /*p01.AFAS*/ bool PHASE_xxxxEFGx3; // sys, vid
  /*p01.UVYT*/ bool PHASE_ABCDxxxx7b; // sys
  /*p01.DOVA*/ bool PHASE_ABCDxxxx7c; // apu
  /*p04.MOPA*/ bool PHASE_xxxxEFGH8b; // sys, sprites
  /*p01.BOGA*/ bool BOGA_xBCDEFGH; // sys, PORTD_08?
  /*p01.BEDO*/ bool BEDO_Axxxxxxx; // sys, PORTD_02

  //----------

  /*p01.ABUZ*/ bool ADDR_VALID_ABxxxxxx;
  /*p07.SYKE*/ bool ADDR_FFXX; // used by a bunch of stuff

  /*p02.AWOB*/ bool TO_CPU2;
};

//-----------------------------------------------------------------------------

};