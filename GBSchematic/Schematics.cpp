#include "Schematics.h"

// mystery signals 

bool ANAP; // no idea, apu?
bool FERO_Q; // probably apu clock select in debug mode

bool NET01; // not(T1nT2)
bool NET02; // something to do with external buses

bool CYBO_4MHZ; // who drives this? is it hiding in clocks_reset?
bool AJER_2MHZ; // ditto
bool ABOL_1MHZ; // ditto

bool APU_RESET;
bool APU_RESET5n;

// mystery signal that's written but unused - ram something
bool MD_B;

bool CPU_WR_RAW;
bool CPU_WR_WAT;
bool CPU_RAW_RD;
bool CPU; // wat?
bool WR_IN; // is this supposed to be MWR_IN?
bool RD_B; // no idea

bool A15_C; // ???

bool FF00RD;
bool FF00WR;
bool FF04_FF07;
bool FF0F;

bool FROM_CPU;
// no FROM_CPU2 - maybe in APU?
bool FROM_CPU3;
bool FROM_CPU4;
bool FROM_CPU5;
bool FROM_CPU6;
bool FROM_CPU7;
bool FROM_CPU8;
bool FROM_CPU9;
bool FROM_CPU10;
