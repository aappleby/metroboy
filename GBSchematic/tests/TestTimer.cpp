#include "TestGB.h"
#include "../pages/Sch_Decoder.h"
#include "../pages/Sch_System.h"
#include "../pages/Sch_Timer.h"
#include "../pages/Sch_Registers.h"

using namespace Schematics;

void test_timer() {
  printf("test_timer:\n");

  SysRegisters sys_reg;
  ClkRegisters clk_reg;
  RstRegisters rst_reg;
  VidRegisters vid_reg;
  BusTristates bus_tri;
  Timer timer;

  sys_reg.reset();
  clk_reg.reset();
  rst_reg.reset();
  vid_reg.reset();

  bus_tri.reset();
  bus_tri.set_addr(0xFF40);
  bus_tri.set_data(0x80);
  sys_reg.CPU_RAW_WR = true;

  timer.reset();
  timer.set_tma(0xBA);
  timer.set_tac(0b101);

  //vid_reg.set_scy(0xCD);

  /*
  for (int i = 0; i < 8000; i++) {
    SysSignals sys_sig =  sys_reg.next_phase();

    for (int pass = 0; pass < 32; pass++) {
      ClkSignals clk_sig = clk_reg.tick_slow(sys_sig);
      RstSignals rst_sig = rst_reg.tick_slow(sys_sig, clk_sig);
      BusSignals bus_sig = BusSignals::tick(sys_sig, clk_sig);
      Decoder    dec_sig = Decoder::tick(sys_sig, clk_sig, bus_tri);

      TimerSignals tim_sig = timer.tick(sys_sig, clk_sig, rst_sig, bus_sig, dec_sig, bus_tri);

      sys_reg.commit();
      clk_reg.commit();
      rst_reg.commit();
      timer.commit();
    }
  }
  */

  for (int i = 0; i < 512; i++) {

    for (int j = 0; j < 1; j++) {
      SysSignals sys_sig =  sys_reg.next_phase();

      for (int pass = 0; pass < 32; pass++) {
        ClkSignals clk_sig = clk_reg.tick_slow(sys_sig);
        RstSignals rst_sig = rst_reg.tick_slow(sys_sig, clk_sig);
        BusSignals bus_sig = BusSignals::tick(sys_sig, clk_sig);
        Decoder    dec_sig = Decoder::tick(sys_sig, clk_sig, bus_tri);
        
        vid_reg.tick(sys_sig, rst_sig, bus_sig, dec_sig, bus_tri);

        TimerSignals tim_sig = timer.tick(sys_sig, clk_sig, rst_sig, bus_sig, dec_sig, bus_tri);

        sys_reg.commit();
        clk_reg.commit();
        rst_reg.commit();
        timer.commit();
        vid_reg.commit();
      }
    }

    printf("phase:%05d div:%05d tima:%03d c:%d tma:%03d tac:%02x M:%d I:%d lcdc:0x%02x scy:0x%02x addr:0x%04x data:0x%02x\n",
           sys_reg.phase_count,
           timer.get_div(),
           timer.get_tima(),
           timer.TIMA_7.c(),
           timer.get_tma(),
           timer.get_tac(),
           timer.get_tima_max(),
           timer.get_int_timer(),
           vid_reg.get_lcdc(),
           vid_reg.get_scy(),
           bus_tri.get_addr(),
           bus_tri.get_data());

    if ((sys_reg.phase_count % 8) == 7) printf("\n");
  }


  printf("test_timer: pass\n");
}