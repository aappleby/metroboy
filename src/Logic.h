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

__forceinline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

__forceinline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

