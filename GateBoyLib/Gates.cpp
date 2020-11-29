#include "GateBoyLib/Gates.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "CoreLib/Tests.h"

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b) {
  a = _byteswap_uint64((a ^ b) * 0xff51afd7ed558ccd);
}

//-----------------------------------------------------------------------------

uint64_t commit_and_hash(void* blob, int size) {
  uint64_t h = HASH_INIT;

  uint8_t* base = (uint8_t*)blob;

  for (int i = 0; i < size; i++) {
    uint8_t s = base[i];
    if (!(s & 0x80)) {
      LOG_Y("Bit %d not dirty after sim pass!\n", i);
      ASSERT_P(false);
    }
    s &= 0x03;
    combine_hash(h, s);
    base[i] = s;
  }

  return h;
}

//-----------------------------------------------------------------------------
