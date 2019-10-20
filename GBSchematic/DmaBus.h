#pragma once
#include "Schematics.h"

struct OAM;
struct Resets;
struct AddressDecoder;
struct Vram;
struct MemBus;

struct DmaBus {
  void tick(OAM& oam, const Resets& rst, const AddressDecoder& dec, Vram& vram, MemBus& mem);

  bool DMA_A0;
  bool DMA_A1;
  bool DMA_A2;
  bool DMA_A3;
  bool DMA_A4;
  bool DMA_A5;
  bool DMA_A6;
  bool DMA_A7;
  bool DMA_A8;
  bool DMA_A9;
  bool DMA_A10;
  bool DMA_A11;
  bool DMA_A12;
  bool DMA_A13;
  bool DMA_A14;
  bool DMA_A15;

  bool CATY;
  bool LUMA;
  bool WYJA;
  bool MOPA_PHI;

  bool VRAM_TO_OAM;

  reg MATU;

private:
  //----------
  // registers

  reg MAKA;
  reg LUVY;
  reg DMA_DONE; // {MYTE}
  reg LENE;

  // DMA base reg
  reg NAFA;
  reg NYGY;
  reg PARA;
  reg PYNE;
  reg PULA;
  reg NYDO;
  reg POKU;
  reg MARU;

  // DMA counter reg
  reg NAKY;
  reg PYRO;
  reg NEFY;
  reg MUTY;
  reg NYKO;
  reg PYLO;
  reg NUTO;
  reg MUGU;


};

extern DmaBus dma;