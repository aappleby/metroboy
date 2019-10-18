#include "ExtBus.h"

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
/* 16 */ bool A15_A, A15_D, A15_IN;

/* 17 */ bool D0_A, D0_D, D0_IN;
/* 18 */ bool D1_A, D1_D, D1_IN;
/* 19 */ bool D2_A, D2_D, D2_IN;
/* 20 */ bool D3_A, D3_D, D3_IN;
/* 21 */ bool D4_A, D4_D, D4_IN;
/* 22 */ bool D5_A, D5_D, D5_IN;
/* 23 */ bool D6_A, D6_D, D6_IN;
/* 24 */ bool D7_A, D7_D, D7_IN;

// (vram bus)

/* 50 */ bool LD0;
/* 51 */ bool LD1;
/* 52 */ bool PIN_CPG;
/* 53 */ bool CP; // lcd clock signal
/* 54 */ bool PIN_ST;
/* 55 */ bool PIN_CPL;
/* 56 */ bool PIN_FR;
/* 57 */ bool PIN_S;

/* 62 */
bool P15_A; // output
//bool P15_B;
//bool P15_C;
//bool P15_D;

/* 63 */
bool P14_A; // output
bool P14_B; // output
//bool P14_C;
//bool P14_D;

/* 64 */
bool P13_A; // output ?
//bool P13_B;
bool P13_C; // input _and_ output? wat? i think there's a typo...
//bool P13_D;

/* 65 */
bool P12;
bool P12_A; // output?
//bool P12_B;
bool P12_C; // input
bool P12_D; // output

/* 66 */
// bool P11_A;
bool P11_B; // output
bool P11_C; // input
bool P11_D; // output

/* 67 - the 1997 schematic is wrooooong */ 
bool P10; // used for interrupt
//bool P10_A;
bool P10_B; // weirdly muxed with debug stuff
bool P10_C; // input
bool P10_D; // output

/* 68 */
/* 69 */
/* 70 */
bool SCK_IN;  // input
bool SCK_DIR; // input?
bool SER_OUT; // input?
bool SIN_IN;  // input?

/* 71 */
bool RESET;
/* 73 */
bool CLKIN_A;
/* 74 */
bool CLKIN_B;

/* 75 */
// CLK? goes here

/* 76 */
bool T2;

/* 77 */
bool T1;

/* 78 */
bool WR_A, WR_C;

/* 79 */
bool RD_A, RD_C;

/* 80 */
bool CS_OUT;

bool NET01; // not(T1nT2)
bool NET02; // something to do with external buses
bool T1nT2n;
bool T1nT2;
bool T1T2n;

bool PIN_NC;

