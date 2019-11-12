#include "Schematics.h"
#include "Clocks.h"

#if 0
// mystery
bool GASE;

reg AZET;
reg BUSA;
reg BANO;
reg AZUS;

void tick_waveram(const Clocks& clk, const Resets& rst) {

  bool AZUS_Q = AZUS.q();

  bool BAMA = not(rst.APU_RESET);
  bool ARUC = not(clk.CLK_4M_B); // check this phase
  bool CYBO = not(clk.CLK_4M_B);
  AZET.tock(ARUC, BAMA, AZUS_Q);
  BUSA.tock(clk.CLK_4M_B, BAMA, GASE);
  bool COZY = not(clk.CLK_4M_B);

  bool ABUR = not(clk.BUKE);
  bool BORY = not(ABUR);

}
#endif