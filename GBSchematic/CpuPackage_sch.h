#pragma once

#if 0
// clock/reset/etc
extern bool RSTn;
extern bool X0;
extern bool X1;
extern bool T1;
extern bool T2;

// vram interface

extern bool MD0, MD1, MD2, MD3, MD4, MD5, MD6, MD7;
extern bool MA0, MA1, MA2, MA3, MA4, MA5, MA6, MA7, MA8, MA9, MA10, MA11, MA12;
extern bool MWRn, MCSn, MOEn;

// lcd interface

extern bool LD1;
extern bool LD0;
extern bool CPG;
extern bool CP;
extern bool ST;
extern bool CPL;
extern bool FR;
extern bool S;

// joypad interface

extern bool P10, P11, P12, P13, P14, P15;

// serial

extern bool S-OUT, S-IN, SCX;

// cart interface
bool CLK_OUT; // CLK?
bool WRn;
bool RDn;
bool CSn;

bool A0, A1, A2, A3, A4, A5, A6, A7;
bool A8, A9, A10, A11, A12, A13, A14, A15;
bool D0, D1, D2, D3, D4, D5, D6, D7;
#endif