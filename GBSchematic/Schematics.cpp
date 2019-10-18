#include "Schematics.h"

// mystery signals 

bool ANAP; // no idea, apu?
bool FERO_Q; // probably apu clock select in debug mode

bool CYBO_4MHZ; // who drives this? is it hiding in clocks_reset?
bool AJER_2MHZ; // ditto
bool ABOL_1MHZ; // ditto

bool APU_RESET;
bool APU_RESET5n;

// mystery signal that's written but unused - ram something
bool MD_B;

bool CPU; // wat?
bool WR_IN; // is this supposed to be MWR_IN? input in of the /WR gpio?
bool RD_B; // no idea

bool A15_C; // ???

bool FF00RD;
bool FF00WR;
bool FF04_FF07;
bool FF0F;

