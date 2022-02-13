#if 0

//------------------------------------------------------------------------------

template<int N>
struct blogic {
  typedef typename bitsize_to_basetype<N>::basetype basetype;

  blogic() {}

  blogic& operator=(uint64_t y) { return *this; }

  operator uint64_t() const { return 0; }

  template<int M>
  blogic(logic<M> b) { static_assert(false); }

  template<int M>
  blogic& operator=(const logic<M> b) { static_assert(false); }
  template<int M>
  blogic& operator=(const logic<M>& b) { static_assert(false); }

private:

  basetype x;

};

void blogic_test() {
  blogic<7> a;
  blogic<8> b;
  b = a;
}


//------------------------------------------------------------------------------

__declspec(noinline) uint32_t test_bitproxy(int offset) {
  logic<32> x = 0;
  x.slice<10>(offset) = 0b1111111111;
  return uint32_t(x);
}

template<int N>
void test_reduce_xor() {
  for (uint64_t i = 0; i < (1ull << N); i++) {
    int parity1 = 0;
    for (int j = 0; j < N; j++) {
      parity1 ^= (i >> j) & 1;
    }
    int parity2 = reduce_xor<N>(i);
    if (parity1 != parity2) {
      printf("%d %d %d\n", i, parity1, parity2);
    }
  }
}


#endif