#pragma once

//typedef const bool wire;
typedef bool wire;

inline wire not (wire a) { return !a; }

inline wire and (wire a) { return a; }
inline wire and (wire a, wire b) { return a & b; }
inline wire and (wire a, wire b, wire c) { return  (a & b & c); }
inline wire and (wire a, wire b, wire c, wire d) { return  (a & b & c & d); }
inline wire and (wire a, wire b, wire c, wire d, wire e) { return  (a & b & c & d & e); }
inline wire and (wire a, wire b, wire c, wire d, wire e, wire f) { return  (a & b & c & d & e & f); }
inline wire and (wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return  (a & b & c & d & e & f & g); }

inline wire or  (wire a) { return a; }
inline wire or  (wire a, wire b) { return a | b; }
inline wire or  (wire a, wire b, wire c) { return  (a | b | c); }
inline wire or  (wire a, wire b, wire c, wire d) { return  (a | b | c | d); }
inline wire or  (wire a, wire b, wire c, wire d, wire e) { return  (a | b | c | d | e); }

inline wire xor (wire a, wire b) { return a ^ b; }
inline wire xnor(wire a, wire b) { return a == b; }

inline wire nor (wire a) { return !a; }
inline wire nor (wire a, wire b) { return !(a | b); }
inline wire nor (wire a, wire b, wire c) { return !(a | b | c); }
inline wire nor (wire a, wire b, wire c, wire d) { return !(a | b | c | d); }
inline wire nor (wire a, wire b, wire c, wire d, wire e) { return !(a | b | c | d | e); }
inline wire nor (wire a, wire b, wire c, wire d, wire e, wire f) { return !(a | b | c | d | e | f); }
inline wire nor (wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return !(a | b | c | d | e | f | g | h); }

inline wire nand(wire a) { return !a; }
inline wire nand(wire a, wire b) { return !(a & b); }
inline wire nand(wire a, wire b, wire c) { return !(a & b & c); }
inline wire nand(wire a, wire b, wire c, wire d) { return !(a & b & c & d); }
inline wire nand(wire a, wire b, wire c, wire d, wire e) { return !(a & b & c & d & e); }
inline wire nand(wire a, wire b, wire c, wire d, wire e, wire f) { return !(a & b & c & d & e & f); }
inline wire nand(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return !(a & b & c & d & e & f & g); }


//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input _ZERO_
inline const wire mux2_p(wire a, wire b, wire m) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input _ZERO_
inline const wire mux2_n(wire a, wire b, wire m) {
  return !(m ? a : b);
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

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

inline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

