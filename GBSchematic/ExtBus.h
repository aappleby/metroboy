#pragma once

struct ExtBus {
  // all the address lines are bidir
  /* 01 */ bool A0_A,  A0_D,  A0_IN;
  /* 02 */ bool A1_A,  A1_D,  A1_IN;
  /* 03 */ bool A2_A,  A2_D,  A2_IN;
  /* 04 */ bool A3_A,  A3_D,  A3_IN;
  /* 05 */ bool A4_A,  A4_D,  A4_IN;
  /* 06 */ bool A5_A,  A5_D,  A5_IN;
  /* 07 */ bool A6_A,  A6_D,  A6_IN;
  /* 08 */ bool A7_A,  A7_D,  A7_IN;
  /* 09 */ bool A8_A,  A8_D,  A8_IN;
  /* 10 */ bool A9_A,  A9_D,  A9_IN;
  /* 11 */ bool A10_A, A10_D, A10_IN;
  /* 12 */ bool A11_A, A11_D, A11_IN;
  /* 13 */ bool A12_A, A12_D, A12_IN;
  /* 14 */ bool A13_A, A13_D, A13_IN;
  /* 15 */ bool A14_A, A14_D, A14_IN;
  /* 16 */ bool A15_A, A15_D, A15_IN, A15_C; // ???
};

extern ExtBus ext;

// package RD/WR
extern bool WR_A, WR_C, WR_IN; // is this supposed to be the input of the /WR gpio?
extern bool RD_A, RD_B, RD_C;  // what is the B for?
extern bool CS_OUT; // output chip select

// package data bus
extern bool D0_A, D0_D, D0_IN;
extern bool D1_A, D1_D, D1_IN;
extern bool D2_A, D2_D, D2_IN;
extern bool D3_A, D3_D, D3_IN;
extern bool D4_A, D4_D, D4_IN;
extern bool D5_A, D5_D, D5_IN;
extern bool D6_A, D6_D, D6_IN;
extern bool D7_A, D7_D, D7_IN;

extern bool CLKIN_A;
extern bool CLKIN_B;

extern bool RESET;
extern bool T1, T2;
extern bool T1nT2n;
extern bool T1nT2;
extern bool T1T2n;

extern bool NET01; // not(T1nT2)
extern bool NET02; // don't know where this is driven yet, but probably similar

extern bool PHI_OUT;

extern bool P10; // why are these not _ABCD
extern bool P12;

extern bool P10_B; // weirdly muxed with debug stuff
extern bool P10_C; // input
extern bool P10_D; // output
extern bool P11_B; // output
extern bool P11_C; // input
extern bool P11_D; // output
extern bool P12_A; // output?
extern bool P12_C; // input
extern bool P12_D; // output
extern bool P13_A; // output
extern bool P13_C; // input _and_ output? wat? i think there's a typo...
extern bool P14_A; // output
extern bool P14_B; // output
extern bool P15_A; // output

// lcd interface
extern bool LD1;
extern bool LD0;
extern bool PIN_CPG;
extern bool CP;
extern bool PIN_ST;
extern bool PIN_CPL;
extern bool PIN_FR;
extern bool PIN_S;

extern bool SCK_IN;  // input
extern bool SCK_DIR; // input?
extern bool SER_OUT; // input?
extern bool SIN_IN;  // input?

extern bool PIN_NC;

