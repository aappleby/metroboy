#include "GateBoyLib/Gates.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "CoreLib/Tests.h"

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b) {
  a = swap((a ^ b) * 0xff51afd7ed558ccd);
}

//-----------------------------------------------------------------------------

uint64_t hash_blob2(void* blob, size_t size) {
  (void)blob;
  (void)size;

  uint64_t h = HASH_INIT;
  uint8_t* base = (uint8_t*)blob;
  for (size_t i = 0; i < size; i++) {
    combine_hash(h, base[i]);
  }
  return h;
}

void commit_blob(void* blob, size_t size) {
  (void)blob;
  (void)size;

  uint8_t* base = (uint8_t*)blob;
  bool bad_bits = false;
  for (size_t i = 0; i < size; i++) {
    uint8_t s = base[i];

    if (bool(s & BIT_DRIVEN) && bool(s & BIT_PULLED)) {
      LOG_Y("Bit %d both driven and pulled up!\n", i);
      bad_bits = true;
    }

    if (!bool(s & BIT_DRIVEN) && !bool(s & BIT_PULLED)) {
      LOG_Y("Bit %d floating!\n", i);
      bad_bits = true;
    }

    if ((s & (BIT_OLD | BIT_NEW)) != BIT_NEW) {
      LOG_Y("Bit %d not dirty after sim pass!\n", i);
      bad_bits = true;
    }

    base[i] = (s & 0b00001111) | BIT_OLD;
  }
  ASSERT_N(bad_bits);
}

//-----------------------------------------------------------------------------
