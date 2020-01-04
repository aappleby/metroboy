#include "TestGB.h"
#include "../pages/Sch_Decoder.h"
#include "../pages/Sch_System.h"
#include "../pages/Sch_Timer.h"
#include "../pages/Sch_Registers.h"

using namespace Schematics;

void test_timer() {
  printf("test_timer:\n");

  TestGB gb;
  gb.reset();

  gb.timer.set_tma(0xBA);
  gb.timer.set_tac(0b101);

  int max_passes = 0;

  for (int i = 0; i < 8*1024*1024; i++) {

    gb.bus_tri.set_addr((uint16_t)0x0000);
    int passes = gb.sim_phase(1);
    if (passes > max_passes) {
      max_passes = passes;
      printf("max_passes %d\n", max_passes);
    }

    /*
    printf("phase:%05d passes:%d div:%05d tima:%03d c:%d tma:%03d tac:%02x M:%d I:%d lcdc:0x%02x scy:0x%02x addr:0x%04x data:0x%02x\n",
           gb.sys_reg.phase_count,
           passes,
           gb.timer.get_div(),
           gb.timer.get_tima(),
           gb.timer.TIMA_7.c(),
           gb.timer.get_tma(),
           gb.timer.get_tac(),
           gb.timer.get_tima_max(),
           gb.timer.get_int_timer(),
           gb.vid_reg.get_lcdc(),
           gb.vid_reg.get_scy(),
           gb.bus_tri.get_addr(),
           gb.bus_tri.get_data());

    if ((gb.sys_reg.phase_count % 8) == 7) printf("\n");
    */
  }

  printf("test_timer: pass\n");
}