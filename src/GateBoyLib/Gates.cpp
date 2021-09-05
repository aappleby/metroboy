#include "GateBoyLib/Gates.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "CoreLib/Tests.h"

//-----------------------------------------------------------------------------

uint64_t hash_blob(void* blob, size_t size, uint8_t mask) {
  uint64_t h = HASH_INIT;
  for (size_t i = 0; i < size; i++) {
    combine_hash(h, ((uint8_t*)blob)[i] & mask);
  }
  return h;
}

void commit_blob(void* blob, size_t size) {
  //static_assert(config_drive_flags || config_oldnew_flags);

  uint8_t* base = (uint8_t*)blob;
  bool bad_bits = false;
  for (size_t i = 0; i < size; i++) {
    uint8_t s = base[i];

    if (config_drive_flags) {
      if (bool(s & BIT_DRIVEN) && bool(s & BIT_PULLED)) {
        LOG_Y("Bit %d both driven and pulled up!\n", i);
        bad_bits = true;
      }

      if (!bool(s & BIT_DRIVEN) && !bool(s & BIT_PULLED)) {
        LOG_Y("Bit %d floating!\n", i);
        bad_bits = true;
      }

    }

    if (config_oldnew_flags) {
      if ((s & (BIT_OLD | BIT_NEW)) != BIT_NEW) {
        LOG_Y("Bit %d not dirty after sim pass!\n", i);
        bad_bits = true;
      }
    }

    base[i] = (s & 0b00001111) | BIT_OLD;
  }
  ASSERT_N(bad_bits);
}

//-----------------------------------------------------------------------------

int diff_blob(void* blob_a, int start_a, int end_a, void* blob_b, int start_b, int end_b, uint8_t mask) {
  int failures = 0;
  int size_a = end_a - start_a;
  int size_b = end_b - start_b;

  if (size_a != size_b) {
    LOG_R("diff() : Size mismatch %d vs %d\n", size_a, size_b);
    return false;
  }

  uint8_t* bytes_a = (uint8_t*)blob_a;
  uint8_t* bytes_b = (uint8_t*)blob_b;

  for (int i = 0; i < size_a; i++) {
    int ia = start_a + i;
    int ib = start_b + i;

    int byte_a = bytes_a[ia] & mask;
    int byte_b = bytes_b[ib] & mask;

    EXPECT_EQ(byte_a, byte_b, "@ %5d : [%5d] = 0x%02x, [%5d] = 0x%02x\n", i, ia, byte_a, ib, byte_b);
  }
  return failures;
}


//-----------------------------------------------------------------------------
