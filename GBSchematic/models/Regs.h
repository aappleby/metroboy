#pragma once

typedef unsigned char uint8_t;

//-----------------------------------------------------------------------------

struct Reg {
  void reset();
  void tick();
  void tock(bool clk_in, bool rst_in, bool reg_in);
  operator bool() const;

private:

  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct Reg8 {
  void reset();
  void tick();
  void tock(bool clk_in, bool rst_in, uint8_t reg_in);
  operator uint8_t() const;

private:

  uint8_t val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct RegSR {
  void reset();
  void tick();
  void tock(bool clk_in, bool set_in, bool rst_in, bool reg_in);
  operator bool() const;

private:

  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct Counter {
  void reset();
  void tick();
  void tock(bool clk_in, bool load_in, bool reg_in);
  operator bool() const;

private:

  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct DuoReg {
  void reset();
  void tick();
  void tock(bool clk_in, bool rst_in, bool reg_in);
  operator bool() const;

private:

  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct Latch {
  bool latch(bool clk_in, bool latch_in);
  operator bool() const;

private:
  bool val = false;
};

//-----------------------------------------------------------------------------

inline bool _not(bool a) { return !a; }
inline bool _and (bool a, bool b) { return a & b; }
inline bool _and (bool a, bool b, bool c) { return  (a & b & c); }
inline bool _and (bool a, bool b, bool c, bool d) { return  (a & b & c & d); }
inline bool _and (bool a, bool b, bool c, bool d, bool e, bool f) { return  (a & b & c & d & e & f); }
inline bool _or  (bool a, bool b) { return a | b; }
inline bool _or  (bool a, bool b, bool c) { return  (a | b | c); }
inline bool _or  (bool a, bool b, bool c, bool d) { return  (a | b | c | d); }
inline bool _xor (bool a, bool b) { return a ^ b; }
inline bool _nor (bool a, bool b) { return !(a | b); }
inline bool _nor (bool a, bool b, bool c) { return !(a | b | c); }
inline bool _nor (bool a, bool b, bool c, bool d) { return !(a | b | c | d); }
inline bool _nor (bool a, bool b, bool c, bool d, bool e) { return !(a | b | c | d | e); }
inline bool _nor (bool a, bool b, bool c, bool d, bool e, bool f) { return !(a | b | c | d | e | f); }
inline bool _nor (bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) { return !(a | b | c | d | e | f | g | h); }
inline bool _nand(bool a, bool b) { return !(a & b); }
inline bool _nand(bool a, bool b, bool c) { return !(a & b & c); }
inline bool _nand(bool a, bool b, bool c, bool d) { return !(a & b & c & d); }
inline bool _nand(bool a, bool b, bool c, bool d, bool e) { return !(a & b & c & d & e); }
inline bool _nand(bool a, bool b, bool c, bool d, bool e, bool f) { return !(a & b & c & d & e & f); }
inline bool _nand(bool a, bool b, bool c, bool d, bool e, bool f, bool g) { return !(a & b & c & d & e & f & g); }
inline bool mux2 (bool a, bool b, bool m) { return m ? a : b; }
inline bool mux2n (bool a, bool b, bool m) { return m ? !a : !b; }
inline bool amux2(bool a0, bool b0, bool a1, bool b1) { return (b0 ? a0 : 0) | (b1 ? a1 : 0); }
inline bool amux3(bool a0, bool b0, bool a1, bool b1, bool a2, bool b2) { return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0); }
inline bool amux4(bool a0, bool b0, bool a1, bool b1, bool a2, bool b2, bool a3, bool b3) { return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0); }
inline bool amux6(bool a0, bool b0, bool a1, bool b1, bool a2, bool b2, bool a3, bool b3, bool a4, bool b4, bool a5, bool b5) { return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0) | (b4 ? a4 : 0) | (b5 ? a5 : 0); }
