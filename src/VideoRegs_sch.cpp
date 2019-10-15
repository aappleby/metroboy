// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

extern bool CPU_RD2;
extern bool CPU_WR2;
extern bool RESET7;

bool WARU;
bool XARE;

// this is literally just all registers

// LCDC
bool FF40;
bool FF40_D0;
bool FF40_D1;
bool FF40_D2;
bool FF40_D3;
bool FF40_D4;
bool FF40_D5;
bool FF40_D6;
bool FF40_D7;

// SCY
bool FF42;
bool FF42_D0;
bool FF42_D1;
bool FF42_D2;
bool FF42_D3;
bool FF42_D4;
bool FF42_D5;
bool FF42_D6;
bool FF42_D7;

// SCX
bool FF43;
bool FF43_D0;
bool FF43_D1;
bool FF43_D2;
bool FF43_D3;
bool FF43_D4;
bool FF43_D5;
bool FF43_D6;
bool FF43_D7;

// LY is actually the "VX" bus
bool FF44;

// LYC
bool FF45;
bool FF45_D0;
bool FF45_D1;
bool FF45_D2;
bool FF45_D3;
bool FF45_D4;
bool FF45_D5;
bool FF45_D6;
bool FF45_D7;

// WY
bool FF4A;
bool FF4A_D0;
bool FF4A_D1;
bool FF4A_D2;
bool FF4A_D3;
bool FF4A_D4;
bool FF4A_D5;
bool FF4A_D6;
bool FF4A_D7;

// WX
bool FF4B;
bool FF4B_D0;
bool FF4B_D1;
bool FF4B_D2;
bool FF4B_D3;
bool FF4B_D4;
bool FF4B_D5;
bool FF4B_D6;
bool FF4B_D7;

void tick_videoregs() {
  WARU = and(FF40, CPU_WR2);
  XARE = not(RESET7);

  // not going to write out all the clocks right now... too tedious

}