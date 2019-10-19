#pragma once
#include "Schematics.h"

struct ExtCpuBuses_sch {
public:

  //----------
  // outputs

  bool LULA;
  bool RORU;
  bool TEXO; // used by vram interface

  void tick(bool ABUZ, bool LUMA);

private:

  //----------
  // registers

  reg NYRE;
  reg LONU;
  reg LOBU;
  reg LUMY;
  reg PATE;
  reg LYSA;
  reg LUNO;
  reg ARYM;
  reg AROS;
  reg ATEV;
  reg AVYS;
  reg ARET;
  reg ALYR;
  reg APUR;
  reg ALOR;

  // DX_IN latch
  reg SODY;
  reg SELO;
  reg RONY;
  reg SOMA;
  reg RAXY;
  reg RUPA;
  reg SAGO;
  reg SAZY;

};

extern ExtCpuBuses_sch ext_sch;
