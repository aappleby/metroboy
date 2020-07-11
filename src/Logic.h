#pragma once

//typedef const bool wire;
typedef bool wire;

__forceinline wire not (wire a) { return !a; }

__forceinline wire and (wire a) { return a; }
__forceinline wire and (wire a, wire b) { return a & b; }
__forceinline wire and (wire a, wire b, wire c) { return  (a & b & c); }
__forceinline wire and (wire a, wire b, wire c, wire d) { return  (a & b & c & d); }
__forceinline wire and (wire a, wire b, wire c, wire d, wire e) { return  (a & b & c & d & e); }
__forceinline wire and (wire a, wire b, wire c, wire d, wire e, wire f) { return  (a & b & c & d & e & f); }
__forceinline wire and (wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return  (a & b & c & d & e & f & g); }

__forceinline wire or  (wire a) { return a; }
__forceinline wire or  (wire a, wire b) { return a | b; }
__forceinline wire or  (wire a, wire b, wire c) { return  (a | b | c); }
__forceinline wire or  (wire a, wire b, wire c, wire d) { return  (a | b | c | d); }
__forceinline wire or  (wire a, wire b, wire c, wire d, wire e) { return  (a | b | c | d | e); }

__forceinline wire xor (wire a, wire b) { return a ^ b; }
__forceinline wire xnor(wire a, wire b) { return a == b; }

__forceinline wire nor (wire a) { return !a; }
__forceinline wire nor (wire a, wire b) { return !(a | b); }
__forceinline wire nor (wire a, wire b, wire c) { return !(a | b | c); }
__forceinline wire nor (wire a, wire b, wire c, wire d) { return !(a | b | c | d); }
__forceinline wire nor (wire a, wire b, wire c, wire d, wire e) { return !(a | b | c | d | e); }
__forceinline wire nor (wire a, wire b, wire c, wire d, wire e, wire f) { return !(a | b | c | d | e | f); }
__forceinline wire nor (wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return !(a | b | c | d | e | f | g | h); }

__forceinline wire nand(wire a) { return !a; }
__forceinline wire nand(wire a, wire b) { return !(a & b); }
__forceinline wire nand(wire a, wire b, wire c) { return !(a & b & c); }
__forceinline wire nand(wire a, wire b, wire c, wire d) { return !(a & b & c & d); }
__forceinline wire nand(wire a, wire b, wire c, wire d, wire e) { return !(a & b & c & d & e); }
__forceinline wire nand(wire a, wire b, wire c, wire d, wire e, wire f) { return !(a & b & c & d & e & f); }
__forceinline wire nand(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return !(a & b & c & d & e & f & g); }

//-----------------------------------------------------------------------------

__forceinline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

__forceinline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

