#pragma once
#include "CoreLib/Types.h"
#include "CoreLib/Tests.h"

#include "GateBoyLib/Utils.h"

//-----------------------------------------------------------------------------

/*__attribute__((always_inline))*/ inline wire not1(wire a) { return ~a; }

inline wire and2(wire a, wire b) { return a & b; }
inline wire and3(wire a, wire b, wire c) { return  (a & b & c); }

inline wire and4(wire a, wire b, wire c, wire d) { return  (a & b & c & d); }
inline wire and5(wire a, wire b, wire c, wire d, wire e) { return  (a & b & c & d & e); }
inline wire and6(wire a, wire b, wire c, wire d, wire e, wire f) { return  (a & b & c & d & e & f); }
inline wire and7(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return  (a & b & c & d & e & f & g); }

inline wire or2(wire a, wire b) { return a | b; }
inline wire or3(wire a, wire b, wire c) { return  (a | b | c); }
inline wire or4(wire a, wire b, wire c, wire d) { return  (a | b | c | d); }
inline wire or5(wire a, wire b, wire c, wire d, wire e) { return  (a | b | c | d | e); }
inline wire or6(wire a, wire b, wire c, wire d, wire e, wire f) { return a | b | c | d | e | f; }

inline wire xor2(wire a, wire b) { return a ^ b; }

// XNOR01 >> D
// XNOR02 << A
// XNOR03 << B
// XNOR04
// XNOR05
// XNOR06
inline wire xnor2(wire a, wire b) { return ~(a ^ b); }

inline wire nor2(wire a, wire b) { return ~(a | b); }
inline wire nor3(wire a, wire b, wire c) { return ~(a | b | c); }
inline wire nor4(wire a, wire b, wire c, wire d) { return ~(a | b | c | d); }
inline wire nor5(wire a, wire b, wire c, wire d, wire e) { return ~(a | b | c | d | e); }
inline wire nor6(wire a, wire b, wire c, wire d, wire e, wire f) { return ~(a | b | c | d | e | f); }
inline wire nor8(wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return ~(a | b | c | d | e | f | g | h); }

inline wire nand2(wire a, wire b) { return ~(a & b); }
inline wire nand3(wire a, wire b, wire c) { return ~(a & b & c); }
inline wire nand4(wire a, wire b, wire c, wire d) { return ~(a & b & c & d); }
inline wire nand5(wire a, wire b, wire c, wire d, wire e) { return ~(a & b & c & d & e); }
inline wire nand6(wire a, wire b, wire c, wire d, wire e, wire f) { return ~(a & b & c & d & e & f); }
inline wire nand7(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return ~(a & b & c & d & e & f & g); }

inline wire and_or3(wire a, wire b, wire c) { return (a & b) | c; }
inline wire or_and3(wire a, wire b, wire c) { return (a | b) & c; }

inline wire not_or_and3(wire a, wire b, wire c) { return ~or_and3(a, b, c); }

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline wire mux2p(wire m, wire a, wire b) {
  return bit0(m) ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire mux2n(wire m, wire a, wire b) {
  return ~(bit0(m) ? a : b);
}

inline wire amux2(wire a0, wire b0, wire a1, wire b1) {
  return (b0 & a0) | (b1 & a1);
}

inline wire amux3(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2);
}

inline wire amux4(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2, wire a3, wire b3) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3);
}

inline wire amux6(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2, wire a3, wire b3, wire a4, wire b4, wire a5, wire b5) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3) | (b4 & a4) | (b5 & a5);
}

//-----------------------------------------------------------------------------
