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

uint64_t commit_and_hash(void* blob, size_t size) {
  (void)blob;
  (void)size;

  uint64_t h = HASH_INIT;
#ifdef USE_HASH
  uint8_t* base = (uint8_t*)blob;
  bool bad_bits = false;

  for (size_t i = 0; i < size; i++) {
    uint8_t s = base[i];

#ifdef USE_DIRTY_BIT
    if (!(s & 0x80)) {
      LOG_Y("Bit %d not dirty after sim pass!\n", i);
      bad_bits = true;
    }
#endif
    s &= 0b00001011;

    combine_hash(h, s);

    base[i] = s;
  }

  ASSERT_N(bad_bits);
#endif

  return h;
}

//-----------------------------------------------------------------------------
