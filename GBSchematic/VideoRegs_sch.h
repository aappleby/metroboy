#pragma once

extern bool RESET_VIDEO;

// screen y coordinate
extern bool V0, V1, V2, V3, V4, V5, V6, V7;

// screen x coordinate
extern bool ACAM, AZUB, AMEL, AHAL, APUX, ABEF, ADAZ, ASAH;

// the actual counter regs?
extern reg XEHO, SAVY, XODU, XYDO, TUHU, TUKY, TAKO, SYBE; // x counter
extern reg MUWY, MYRO, LEXA, LYDO, LOVU, LEMA, MATO, LAFO; // y counter


// FF40 LCDC
extern bool FF40_D0;
extern bool FF40_D1;
extern bool FF40_D2;
extern bool FF40_D3;
extern bool FF40_D4;
extern bool FF40_D5;
extern bool FF40_D6;
extern bool FF40_D7;

// FF42 SCY
extern bool FF42_D0;
extern bool FF42_D1;
extern bool FF42_D2;
extern bool FF42_D3;
extern bool FF42_D4;
extern bool FF42_D5;
extern bool FF42_D6;
extern bool FF42_D7;

// FF43 SCX
extern bool FF43_D0;
extern bool FF43_D1;
extern bool FF43_D2;
extern bool FF43_D3;
extern bool FF43_D4;
extern bool FF43_D5;
extern bool FF43_D6;
extern bool FF43_D7;

// FF45 LYC
extern bool FF45_D0;
extern bool FF45_D1;
extern bool FF45_D2;
extern bool FF45_D3;
extern bool FF45_D4;
extern bool FF45_D5;
extern bool FF45_D6;
extern bool FF45_D7;

// FF4A WY
extern bool FF4A_D0;
extern bool FF4A_D1;
extern bool FF4A_D2;
extern bool FF4A_D3;
extern bool FF4A_D4;
extern bool FF4A_D5;
extern bool FF4A_D6;
extern bool FF4A_D7;

// FF4B WX
extern bool FF4B_D0;
extern bool FF4B_D1;
extern bool FF4B_D2;
extern bool FF4B_D3;
extern bool FF4B_D4;
extern bool FF4B_D5;
extern bool FF4B_D6;
extern bool FF4B_D7;
