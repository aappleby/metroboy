#include "ExtBus.h"

ExtBus ext;

// all the vram data lines are bidir
// vram address lines are output-only
// mcs, moe, mwr are bidir

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

bool NET01; // not(T1nT2)
bool NET02; // something to do with external buses
bool T1nT2n;
bool T1nT2;
bool T1T2n;

bool PIN_NC;

