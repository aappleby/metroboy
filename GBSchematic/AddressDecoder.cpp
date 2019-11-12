#include "AddressDecoder.h"

#include "MemBus.h"

void AddressDecoder::tick(const MemBus& mem2) {
  bool SARE = nor(mem2.A7, mem2.A7, mem2.A7, mem2.A4, mem2.A3);
  A00_07 = SARE;
}