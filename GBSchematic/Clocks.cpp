#include "Clocks.h"
#include "Schematics.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "PPU.h"
#include "APU.h"
#include "Clocks.h"
#include "AddressDecoder.h"
#include "Debug.h"
#include "System.h"

//-----------------------------------------------------------------------------

void Clocks::tick(const APU& apu, Resets& rst, MemBus& mem) {

  //----------
  // FF04 DIV

  // not sure why the clock's getting nor'd with it
  //bool RESET_DIV = nor(CLK_1357, ext.RESET, cpu.CPU_WR, mem.match(0xFF04));

  bool RESET_DIV = ext.RESET || (cpu.CPU_WR && mem.match(0xFF04));

  wire DIV_Q0  = DIV_R0. flip(BOGA1MHZ, !RESET_DIV);
  wire DIV_Q1  = DIV_R1. flip(!DIV_Q0,  !RESET_DIV);
  wire DIV_Q2  = DIV_R2. flip(!DIV_Q1,  !RESET_DIV);
  wire DIV_Q3  = DIV_R3. flip(!DIV_Q2,  !RESET_DIV);
  wire DIV_Q4  = DIV_R4. flip(!DIV_Q3,  !RESET_DIV);
  wire DIV_Q5  = DIV_R5. flip(!DIV_Q4,  !RESET_DIV);

  wire ULUR = mux2(BOGA1MHZ, !DIV_Q5, sys.FF60_D1);

  wire DIV_Q6  = DIV_R6. flip(ULUR,     !RESET_DIV);
  wire DIV_Q7  = DIV_R7. flip(!DIV_Q6,  !RESET_DIV);
  wire DIV_Q8  = DIV_R8. flip(!DIV_Q7,  !RESET_DIV);
  wire DIV_Q9  = DIV_R9. flip(!DIV_Q8,  !RESET_DIV);
  wire DIV_Q10 = DIV_R10.flip(!DIV_Q9,  !RESET_DIV);
  wire DIV_Q11 = DIV_R11.flip(!DIV_Q10, !RESET_DIV);
  wire DIV_Q12 = DIV_R12.flip(!DIV_Q11, !RESET_DIV);
  wire DIV_Q13 = DIV_R13.flip(!DIV_Q12, !RESET_DIV);
  wire DIV_Q14 = DIV_R14.flip(!DIV_Q13, !RESET_DIV);
  wire DIV_Q15 = DIV_R15.flip(!DIV_Q14, !RESET_DIV);

  if (cpu.CPU_RD && mem.match(0xFF04)) {
    mem.D0 = DIV_Q6;
    mem.D1 = DIV_Q7;
    mem.D2 = DIV_Q8;
    mem.D3 = DIV_Q9;
    mem.D4 = DIV_Q10;
    mem.D5 = DIV_Q11;
    mem.D6 = DIV_Q12;
    mem.D7 = DIV_Q13;
  }

  //----------

  CLK_16k  = not(DIV_Q5); // is this really inverted?
  CLK_64k  = DIV_Q3;
  CLK_256k = DIV_Q1;
  FF04_D0n = not(DIV_Q6);
  FF04_D1n = not(DIV_Q7);

  wire CLK_1M_AQ = CLK_1M_A.q();
  wire CLK_1M_BQ = CLK_1M_B.q();
  wire CLK_1M_CQ = CLK_1M_C.q();
  wire CLK_1M_DQ = CLK_1M_D.q();

  wire AFER_Q = AFER.q();

  // combi clock deglitcher on CLKIN_B, skipping it

  CLK_1357  = ext.CLKIN_B;
  CLK_0246  = not(ext.CLKIN_B);
  APUV_4MHZ = not(ext.CLKIN_B);

  // guess this is used in the apu somewhere? not sure if phase is right
  CERY_2MHZ = CERY.flip(ext.CLKIN_B, not(rst.APU_RESET));

  // comment about clock rates on schematic is wrong

  // makin' funny clocks
  AFAS = and(CLK_1M_AQ, CLK_1M_BQ);

  ABUZ = nor(dbg.T1nT2, unk3(not(not(CLK_1M_BQ)), not(CLK_1M_DQ), cpu.FROM_CPU4));
  BUKE = and(not(CLK_1M_CQ), CLK_1M_DQ, not(ABOL_1MHZ));

  cpu.CPU_RD_SYNC = and(cpu.FROM_CPU3, CLK_1M_AQ, CLK_1M_BQ);

  wire BAPY = nor(ABOL_1MHZ, not(CLK_1M_DQ), not(CLK_1M_BQ));
  wire BEJA = nand(not(BAPY), not(nor(not(CLK_1M_BQ), ABOL_1MHZ)));
  wire BALY = or(and(not(CLK_1M_CQ), not(CLK_1M_BQ), not(BEJA)), not(ext.CLKIN_A));
  BOGA1MHZ = not(BALY);

  BEDO = and(not(ABOL_1MHZ), BALY);
  
  // this is probably the CPU's 1 mhz clock?
  cpu.TO_CPU = not(and(not(ABOL_1MHZ), BALY));

  PHI_OUTn = not(ext.PHI_OUT);

  {
    wire TUBO = unk2(ABOL_1MHZ, or(ext.RESET, CLK_1357));
    wire TABA = or(dbg.T1nT2, dbg.T1nT2n, and(TUBO, DIV_Q15));
    wire ASOL = unk2(nor(not(TABA), ext.RESET), ext.RESET);
    rst.RESET2 = not(or(AFER_Q, ASOL));
    AFER.tock(not(BOGA1MHZ), dbg.T1nT2n, ASOL);
  }

  PHIn = not(nor(not(CLK_1M_BQ), ABOL_1MHZ));

  rst.RESET_VIDEO = nand(ppu.LCDC_LCDEN, rst.RESET2);

  wire BARA_Q = BARA.q();
  wire CARU_Q = CARU.q();
  wire BYLU_Q = BYLU.q();

  HORU_512 = mux2(HAMA_512Kn, !BARA_Q, apu.FERO_Q);
  BUFY_256 = mux2(HAMA_512Kn, not(!CARU_Q), apu.FERO_Q);
  BYFE_128 = mux2(HAMA_512Kn, not(!BYLU_Q), apu.FERO_Q);

  wire ATYK_Q = ATYK.flip(not(ext.CLKIN_B),   not(rst.APU_RESET));
  wire AVOK_Q = AVOK.flip(ATYK_Q,      not(rst.APU_RESET));
  wire JESO_Q = JESO.flip(not(AVOK_Q), rst.APU_RESET5n);
  JESO_512K = JESO_Q;
  BAVU_1MHZ = not(AVOK_Q);
  HAMA_512Kn = not(JESO_Q);

  BARA.tock(not(AJER_2MHZ), not(rst.APU_RESET), not(DIV_Q10));
  CARU.flip(BARA_Q, not(rst.APU_RESET));
  BYLU.flip(!CARU_Q, not(rst.APU_RESET));

  //----------
  // registers

  CLK_1M_A.tock(CLK_0246, dbg.T1nT2n,  CLK_1M_DQ);
  CLK_1M_B.tock(CLK_0246, dbg.T1nT2n, !CLK_1M_AQ);
  CLK_1M_C.tock(CLK_0246, dbg.T1nT2n,  CLK_1M_BQ);
  CLK_1M_D.tock(CLK_0246, dbg.T1nT2n,  CLK_1M_CQ);
}
