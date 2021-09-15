#include "GateBoyLib/Gates.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "CoreLib/Tests.h"

inline uint64_t fmix64(uint64_t k)
{
  k ^= k >> 33;
  k *= 0xff51afd7ed558ccd;
  k ^= k >> 33;
  k *= 0xc4ceb9fe1a85ec53;
  k ^= k >> 33;

  return k;
}

uint64_t hash_all_bits(const void * key, const int len, uint64_t seed)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  const uint64_t c1 = 0x87c37b91114253d5;
  const uint64_t c2 = 0x4cf5ad432745937f;

  //----------
  // body

  const uint64_t * blocks = (const uint64_t *)(data);

  for(int i = 0; i < nblocks; i++)
  {
    uint64_t k1 = blocks[i*2+0];
    uint64_t k2 = blocks[i*2+1];

    k1 *= c1; k1  = _rotl64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = _rotl64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = _rotl64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = _rotl64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

  uint64_t k1 = 0;
  uint64_t k2 = 0;

  switch(len & 15)
  {
  case 15: k2 ^= ((uint64_t)tail[14]) << 48; [[fallthrough]];
  case 14: k2 ^= ((uint64_t)tail[13]) << 40; [[fallthrough]];
  case 13: k2 ^= ((uint64_t)tail[12]) << 32; [[fallthrough]];
  case 12: k2 ^= ((uint64_t)tail[11]) << 24; [[fallthrough]];
  case 11: k2 ^= ((uint64_t)tail[10]) << 16; [[fallthrough]];
  case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;  [[fallthrough]];
  case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;  k2 *= c2; k2  = _rotl64(k2,33); k2 *= c1; h2 ^= k2;  [[fallthrough]];
  case  8: k1 ^= ((uint64_t)tail[ 7]) << 56; [[fallthrough]];
  case  7: k1 ^= ((uint64_t)tail[ 6]) << 48; [[fallthrough]];
  case  6: k1 ^= ((uint64_t)tail[ 5]) << 40; [[fallthrough]];
  case  5: k1 ^= ((uint64_t)tail[ 4]) << 32; [[fallthrough]];
  case  4: k1 ^= ((uint64_t)tail[ 3]) << 24; [[fallthrough]];
  case  3: k1 ^= ((uint64_t)tail[ 2]) << 16; [[fallthrough]];
  case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;  [[fallthrough]];
  case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;  k1 *= c1; k1  = _rotl64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len; h2 ^= len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  return h1;
}

//-----------------------------------------------------------------------------

uint64_t hash_low_bit(const void * key, const int len, uint64_t seed)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  const uint64_t c1 = 0x87c37b91114253d5;
  const uint64_t c2 = 0x4cf5ad432745937f;

  //----------
  // body

  const uint64_t * blocks = (const uint64_t *)(data);

  for(int i = 0; i < nblocks; i++)
  {
    uint64_t k1 = blocks[i*2+0] & 0x0101010101010101;
    uint64_t k2 = blocks[i*2+1] & 0x0101010101010101;;

    k1 *= c1; k1  = _rotl64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = _rotl64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = _rotl64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = _rotl64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

  uint64_t k1 = 0;
  uint64_t k2 = 0;

  switch(len & 15)
  {
  case 15: k2 ^= ((uint64_t)tail[14] & 0x01) << 48; [[fallthrough]];
  case 14: k2 ^= ((uint64_t)tail[13] & 0x01) << 40; [[fallthrough]];
  case 13: k2 ^= ((uint64_t)tail[12] & 0x01) << 32; [[fallthrough]];
  case 12: k2 ^= ((uint64_t)tail[11] & 0x01) << 24; [[fallthrough]];
  case 11: k2 ^= ((uint64_t)tail[10] & 0x01) << 16; [[fallthrough]];
  case 10: k2 ^= ((uint64_t)tail[ 9] & 0x01) << 8;  [[fallthrough]];
  case  9: k2 ^= ((uint64_t)tail[ 8] & 0x01) << 0;  k2 *= c2; k2  = _rotl64(k2,33); k2 *= c1; h2 ^= k2;  [[fallthrough]];
  case  8: k1 ^= ((uint64_t)tail[ 7] & 0x01) << 56; [[fallthrough]];
  case  7: k1 ^= ((uint64_t)tail[ 6] & 0x01) << 48; [[fallthrough]];
  case  6: k1 ^= ((uint64_t)tail[ 5] & 0x01) << 40; [[fallthrough]];
  case  5: k1 ^= ((uint64_t)tail[ 4] & 0x01) << 32; [[fallthrough]];
  case  4: k1 ^= ((uint64_t)tail[ 3] & 0x01) << 24; [[fallthrough]];
  case  3: k1 ^= ((uint64_t)tail[ 2] & 0x01) << 16; [[fallthrough]];
  case  2: k1 ^= ((uint64_t)tail[ 1] & 0x01) << 8;  [[fallthrough]];
  case  1: k1 ^= ((uint64_t)tail[ 0] & 0x01) << 0;  k1 *= c1; k1  = _rotl64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len; h2 ^= len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  return h1;
}

//-----------------------------------------------------------------------------
