#pragma once

struct ExtBus {

  //---------------------------------------------------------------------------
  // PINS

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

  // all the data lines are bidir
  /* 17 */ bool D0_A, D0_D, D0_IN;
  /* 18 */ bool D1_A, D1_D, D1_IN;
  /* 19 */ bool D2_A, D2_D, D2_IN;
  /* 20 */ bool D3_A, D3_D, D3_IN;
  /* 21 */ bool D4_A, D4_D, D4_IN;
  /* 22 */ bool D5_A, D5_D, D5_IN;
  /* 23 */ bool D6_A, D6_D, D6_IN;
  /* 24 */ bool D7_A, D7_D, D7_IN;

  /* 25 */ bool MD7_A, MD7_OUT, MD7_IN;
  /* 26 */ bool MD6_A, MD6_OUT, MD6_IN;
  /* 27 */ bool MD5_A, MD5_OUT, MD5_IN;
  /* 28 */ bool MD4_A, MD4_OUT, MD4_IN;
  /* 29 */ bool MD3_A, MD3_OUT, MD3_IN;
  /* 30 */ bool MD2_A, MD2_OUT, MD2_IN;
  /* 31 */ bool MD1_A, MD1_OUT, MD1_IN;
  /* 32 GND */ 
  /* 33 */ bool MD0_A, MD0_OUT, MD0_IN;

  /* 34 MA0 */
  /* 35 MA1 */
  /* 36 MA2 */
  /* 37 MA3 */
  /* 38 MA4 */
  /* 39 MA5 */
  /* 40 MA6 */
  /* 41 MA7 */
  /* 42 MA12 */

  /* 43 MCS */
  bool MCS_A, MCS_D, MCS_IN;

  /* 44 MA10 */
  /* 45 MOE */
  bool MOE_A, MOE_D, MOE_IN;

  /* 46 MA11 */
  /* 47 MA9 */
  /* 48 MA8 */
  /* 49 MWR */
  bool MWR_A, MWR_D, MWR_IN;

  // lcd pins are output
  /* 50 */ bool LD0;
  /* 51 */ bool LD1;
  /* 52 */ bool PIN_CPG;
  /* 53 */ bool CP; // lcd clock signal
  /* 54 */ bool PIN_ST;
  /* 55 */ bool PIN_CPL;
  /* 56 */ bool PIN_FR;
  /* 57 */ bool PIN_S;

  /* 58 VCC */
  /* 59 ROUT */
  /* 60 LOUT */
  /* 61 VIN */

  /* 62 OUTPUT */
  bool P15_A; // output
  //bool P15_B;
  //bool P15_C;
  //bool P15_D;

  /* 63 OUTPUT */
  bool P14_A; // output
  bool P14_B; // output
  //bool P14_C;
  //bool P14_D;

  /* 64 BIDIR */
  bool P13_A; // output ?
  //bool P13_B;
  bool P13_C; // input _and_ output? wat? i think there's a typo...
  //bool P13_D;

  /* 65 BIDIR */
  bool P12;
  bool P12_A; // output?
  //bool P12_B;
  bool P12_C; // input
  bool P12_D; // output

  /* 66 BIDIR */
  // bool P11_A;
  bool P11_B; // output
  bool P11_C; // input
  bool P11_D; // output

  /* 67 BIDIR */ 
  bool P10; // used for interrupt
  //bool P10_A;
  bool P10_B; // weirdly muxed with debug stuff
  bool P10_C; // input
  bool P10_D; // output

  /* 68 BIDIR */
  bool SCK_IN;  // input
  bool SCK_DIR; // input?

  /* 69 BIDIR */
  bool SIN_IN;  // input?

  /* 70 OUTPUT */
  bool SER_OUT; // input?

  /* 71 INPUT */
  bool RESET;
  /* 72 GND */
  /* 73 OUTPUT */
  bool CLKIN_A;
  /* 74 INPUT */
  bool CLKIN_B;

  /* 75 OUTPUT */
  bool PHI_OUT;

  /* 76 INPUT */
  bool T2;

  /* 77 INPUT */
  bool T1;

  /* 78 OUTPUT.. but it has more pins? */
  bool WR_A, WR_C, WR_IN; // ???

  /* 79 OUTPUT... but it has more pins?*/
  bool RD_A, RD_B, RD_C;

  /* 80 OUTPUT*/
  bool CS_OUT;

  // not sure if this is connected to anything...
  bool PIN_NC;

  // this has gotta be one of the vram wr/cs/oe pins or something
  bool MD_B; 
};

extern ExtBus ext;

extern bool NET02; // don't know where this is driven yet, but probably similar


