#include "Joypad.h"

#include "AddressDecoder.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "System.h"
#include "Joypad.h"

//-----------------------------------------------------------------------------

void Joypad::tick(const Resets& rst, MemBus& mem, const Clocks& clk) {
  bool FF00 = mem.match(0xFF00);
  bool FF00WR = nand(cpu.CPU_WR, FF00);

  bool JOYP_RAQ = JOYP_RA.tock(FF00WR, rst.RESET2, mem.D0);
  bool JOYP_LBQ = JOYP_LB.tock(FF00WR, rst.RESET2, mem.D1);
  bool JOYP_UCQ = JOYP_UC.tock(FF00WR, rst.RESET2, mem.D2);
  bool JOYP_DSQ = JOYP_DS.tock(FF00WR, rst.RESET2, mem.D3);
  
  bool JOYP_DIRQ = JOYP_DIR.tock(FF00WR, rst.RESET2, mem.D4);
  bool JOYP_BTNQ = JOYP_BTN.tock(FF00WR, rst.RESET2, mem.D5);
  
  bool JOYP_D6Q = JOYP_D6.tock(FF00WR, rst.RESET2, mem.D6);
  //bool JOYP_D7Q = JOYP_D7.tock(FF00WR, rst.RESET2, mem.D7);

  // debug muxing a joypad register pin to serial?
  SOUT = mux2(JOYP_D6Q, ext.SER_OUT, sys.BURO.q());

  ext.P10_B = nand(JOYP_RAQ,     sys.FF60_D0);
  ext.P10_D = nor (JOYP_RAQ, not(sys.FF60_D0));

  ext.P11_B = nand(JOYP_LBQ,     sys.FF60_D0);
  ext.P11_D = nor (JOYP_LBQ, not(sys.FF60_D0));

  ext.P12_A = nand(JOYP_UCQ,     sys.FF60_D0);
  ext.P12_D = nor (JOYP_UCQ, not(sys.FF60_D0));

  ext.P13_A = nand(JOYP_DSQ,     sys.FF60_D0);
  ext.P13_C = nor (JOYP_DSQ, not(sys.FF60_D0));

  ext.P14_A = or(!JOYP_DIRQ, not(sys.FF60_D0));
  ext.P14_B = !JOYP_DIRQ;

  ext.P15_A = or(!JOYP_BTNQ, not(sys.FF60_D0));

  bool FF00RD = nand(cpu.CPU_RD, FF00);
  bool P10_Q = P10_L.latch(FF00RD, ext.P10_C);
  bool P11_Q = P11_L.latch(FF00RD, ext.P11_C);
  bool P12_Q = P12_L.latch(FF00RD, ext.P12_C);
  bool P13_Q = P13_L.latch(FF00RD, ext.P13_C);

  if (nand(cpu.CPU_RD, FF00)) {
    mem.D0 = P10_Q;
    mem.D1 = P11_Q;
    mem.D2 = P12_Q;
    mem.D3 = P13_Q;
    mem.D4 = JOYP_DIRQ;
    mem.D5 = JOYP_BTNQ;
  }

  // this is weird.  polarity?
  if (or(cpu.FROM_CPU, clk.BEDO)) {
    mem.D0 = ext.P10_B;
    mem.D2 = ext.P10_B;
    mem.D6 = ext.P10_B;
    mem.D4 = ext.P10_B;
    mem.D5 = ext.P10_B;
    mem.D3 = ext.P10_B;
    mem.D1 = ext.P10_B;
    mem.D7 = ext.P10_B;
  }

}