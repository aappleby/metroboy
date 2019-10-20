#pragma once
#include "Schematics.h"

struct AddressDecoder;

struct ExtCpuBuses_sch {
public:

  void tick(bool ABUZ, bool LUMA, const AddressDecoder& dec);

  //----------
  // outputs

  bool LULA;
  bool RORU;
  bool TEXO; // used by vram interface

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

