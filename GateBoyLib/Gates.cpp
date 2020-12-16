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

#if defined(USE_DIRTY_BIT) || defined(USE_OLD_BIT) || defined(USE_HASH)

  uint8_t* base = (uint8_t*)blob;
#ifdef USE_DIRTY_BIT
  bool bad_bits = false;
#endif

  for (size_t i = 0; i < size; i++) {
    uint8_t s = base[i];

#ifdef USE_DIRTY_BIT
    if (!(s & 0x80)) {
      LOG_Y("Bit %d not dirty after sim pass!\n", i);
      bad_bits = true;
    }
    s &= 0x03;
#endif

#ifdef USE_OLD_BIT
    s |= (s << 2);
#endif

#ifdef USE_HASH
    combine_hash(h, s);
#endif

#if defined(USE_DIRTY_BIT) || defined(USE_OLD_BIT)
    base[i] = s;
#endif
  }

#ifdef USE_DIRTY_BIT
  ASSERT_N(bad_bits);
#endif

#endif
  return h;
}

//-----------------------------------------------------------------------------
