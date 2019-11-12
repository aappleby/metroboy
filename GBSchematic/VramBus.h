#pragma once
#include "Schematics.h"

struct OAM;
struct AddressDecoder;
struct Window;
struct Vram;
struct ExtCpuBuses_sch;
struct MemBus;

struct Vram {
public:

  void tick(OAM& oam, const AddressDecoder& dec, const Window& win, Vram& vram, const ExtCpuBuses_sch& ext_sch, MemBus& mem, const Clocks& clk);

  bool MA0;
  bool MA1;
  bool MA2;
  bool MA3;
  bool MA4;
  bool MA5;
  bool MA7;
  bool MA8;
  bool MA9;
  bool MA6;
  bool MA10;
  bool MA11;
  bool MA12;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;

private:

  reg SOTO;
};
