#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

inline bool posedge(wire a, wire b) {
  return !bit(a) && bit(b);
}

inline bool negedge(wire a, wire b) {
  return bit(a) && !bit(b);
}

//-----------------------------------------------------------------------------

inline void combine_hash(uint64_t& a, uint64_t b) {
  a = swap((a ^ b) * 0xff51afd7ed558ccd);
}

uint64_t hash_all_bits(const void* key, const int len, uint64_t seed);
uint64_t hash_low_bit(const void* key, const int len, uint64_t seed);

//-----------------------------------------------------------------------------

inline uint8_t pack(wire a, wire b) {
  return (bit(a) << 0) | (bit(b) << 1);
}

inline uint8_t pack(wire a, wire b, wire c, wire d) {
  return (bit(a) << 0) | (bit(b) << 1) | (bit(c) << 2) | (bit(d) << 3);
}

inline uint8_t pack(wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) {
  return (bit(a) << 0) | (bit(b) << 1) | (bit(c) << 2) | (bit(d) << 3) | (bit(e) << 4) | (bit(f) << 5) | (bit(g) << 6) | (bit(h) << 7);
}

//-----------------------------------------------------------------------------

template<typename SRC>
inline uint32_t bit_pack(const SRC* psrc, int c) {
  uint8_t* src = (uint8_t*)psrc;
  uint32_t result = 0;
  for (int i = 0; i < c; i++) result |= (bit(src[i]) << i);
  return result;
}

template<typename SRC>
inline uint32_t bit_pack(const SRC& src) {
  return bit_pack(&src, sizeof(SRC));
}

//-----------------------------------------------------------------------------

template<typename SRC>
inline uint32_t bit_pack_inv(const SRC* psrc, int c) {
  uint8_t* src = (uint8_t*)psrc;
  uint32_t result = 0;
  for (int i = 0; i < c; i++) result |= (!bit(src[i]) << i);
  return result;
}

template<class SRC>
inline uint32_t bit_pack_inv(const SRC& src) {
  return bit_pack_inv(&src, sizeof(SRC));
}

//-----------------------------------------------------------------------------

template<class DST>
inline void bit_unpack(DST* pdst, int c, const uint32_t src) {
  uint8_t* dst = (uint8_t*)pdst;
  for (int i = 0; i < c; i++) {
    dst[i] &= ~1;
    dst[i] |= get_bit(src, i);
  }
}

template<class DST>
inline void bit_unpack(DST& dst, const uint32_t src) {
  bit_unpack(&dst, sizeof(DST), src);
}

//-----------------------------------------------------------------------------

template<class DST>
inline void bit_init(DST* pdst, int c, const uint8_t flags, const uint32_t src) {
  uint8_t* dst = (uint8_t*)pdst;
  for (int i = 0; i < c; i++) {
    dst[i] = flags | get_bit(src, i);
  }
}

template<class DST>
inline void bit_init(DST& dst, const uint8_t flags, const uint32_t src) {
  bit_init(&dst, sizeof(DST), flags, src);
}

//-----------------------------------------------------------------------------

template<class DST>
inline void bit_unpack_inv(DST* pdst, int c, const uint32_t src) {
  uint8_t* dst = (uint8_t*)pdst;
  for (int i = 0; i < c; i++) {
    dst[i] &= ~1;
    dst[i] |= !get_bit(src, i);
  }
}

template<class DST>
inline void bit_unpack_inv(DST& dst, const uint32_t src) {
  bit_unpack_inv(&dst, sizeof(DST), src);
}

//-----------------------------------------------------------------------------

template<typename T>
inline void bit_cat(T& dst, int bit_min, int bit_max, uint32_t src) {

  int mask = (1 << (bit_max + 1)) - (1 << bit_min);

  dst = T((dst & ~mask) | ((src << bit_min) & mask));
}

//-----------------------------------------------------------------------------

template<typename T>
bool bit_cmp(const T& a, const T& b, uint8_t mask = 0xFF, FieldInfo* field_info = nullptr) {
  const uint8_t* pa = (const uint8_t*)&a;
  const uint8_t* pb = (const uint8_t*)&b;
  bool result = true;
  for (size_t i = 0; i < sizeof(T); i++) {
    auto ba = pa[i] & mask;
    auto bb = pb[i] & mask;
    if (ba != bb) {
      LOG_R("bit_cmp mismatch at offset %3d - 0x%02x 0x%02x, mask 0x%02x",  (int)i, ba, bb, mask);
      if (field_info) {
        LOG_R(" : ");
        print_field_at((int)i, field_info);
      }
      LOG_R("\n");
      result = false;
    }
  }
  return result;
}

//-----------------------------------------------------------------------------

template<typename T>
T bit_purge(const T& a) {
  T b;
  auto src = (const uint8_t*)&a;
  auto dst = (uint8_t*)&b;
  for (auto i = 0; i < sizeof(T); i++) {
    dst[i] = src[i] & 1;
  }
  return b;
}

//-----------------------------------------------------------------------------

template<typename T>
void bit_check(T& a) {
  uint8_t* pa = (uint8_t*)&a;
  for (size_t i = 0; i < sizeof(T); i++) {
    if (pa[i] & ~1) {
      LOG_R("bit_check found non-bit at offset %d - 0x%02x\n",  (int)i, pa);
      //debugbreak();
    }
  }
}

//-----------------------------------------------------------------------------
