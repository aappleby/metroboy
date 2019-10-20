#pragma once
#include "Schematics.h"

struct MemBus;
struct Resets;
struct AddressDecoder;

struct Serial {

  bool SER_TICKn;

  void tick(const Resets& rst, const AddressDecoder& dec, MemBus& mem);

private:

  //----------
  // registers

  bool COTY_Q, COTY_CLK;
  bool ETAF_Q, ETAF_CLK;
  bool CULY_Q, CULY_CLK;
  bool CAFA_Q, CAFA_CLK;
  bool CYLO_Q, CYLO_CLK;
  bool CYDE_Q, CYDE_CLK;
  bool CALY_Q, CALY_CLK;

  bool CUBA_Q, CUBA_CLK;
  bool DEGU_Q, DEGU_CLK;
  bool DYRA_Q, DYRA_CLK;
  bool DOJO_Q, DOJO_CLK;
  bool DOVU_Q, DOVU_CLK;
  bool EJAB_Q, EJAB_CLK;
  bool EROD_Q, EROD_CLK;
  bool EDER_Q, EDER_CLK;

  bool ELYS_Q, ELYS_CLK;
};

extern Serial ser;
