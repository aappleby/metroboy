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

struct Clocks clk;

//-----------------------------------------------------------------------------

void Clocks::tick(const APU& apu, Resets& rst, const AddressDecoder& dec) {

  //----------
  // FF04 DIV

  bool RESET_DIVn = nor(clk.CLKIN_An, ext.RESET, and(dec.FF04_FF07, cpu.CPU_WR, mem.TOLA_A1n, mem.TOVY_A0n));

  wire DIV_Q0  = DIV_R0. flip(clk.BOGA1MHZ, RESET_DIVn);
  wire DIV_Q1  = DIV_R1. flip(!DIV_Q0,      RESET_DIVn);
  wire DIV_Q2  = DIV_R2. flip(!DIV_Q1,      RESET_DIVn);
  wire DIV_Q3  = DIV_R3. flip(!DIV_Q2,      RESET_DIVn);
  wire DIV_Q4  = DIV_R4. flip(!DIV_Q3,      RESET_DIVn);
  wire DIV_Q5  = DIV_R5. flip(!DIV_Q4,      RESET_DIVn);

  wire ULUR = mux2(clk.BOGA1MHZ, !DIV_Q5, sys.FF60_D1);

  wire DIV_Q6  = DIV_R6. flip(ULUR,         RESET_DIVn);
  wire DIV_Q7  = DIV_R7. flip(!DIV_Q6,      RESET_DIVn);
  wire DIV_Q8  = DIV_R8. flip(!DIV_Q7,      RESET_DIVn);
  wire DIV_Q9  = DIV_R9. flip(!DIV_Q8,      RESET_DIVn);
  wire DIV_Q10 = DIV_R10.flip(!DIV_Q9,      RESET_DIVn);
  wire DIV_Q11 = DIV_R11.flip(!DIV_Q10,     RESET_DIVn);
  wire DIV_Q12 = DIV_R12.flip(!DIV_Q11,     RESET_DIVn);
  wire DIV_Q13 = DIV_R13.flip(!DIV_Q12,     RESET_DIVn);
  wire DIV_Q14 = DIV_R14.flip(!DIV_Q13,     RESET_DIVn);
  wire DIV_Q15 = DIV_R15.flip(!DIV_Q14,     RESET_DIVn);

  if (and(dec.FF04_FF07, cpu.CPU_RD, mem.TOLA_A1n, mem.TOVY_A0n)) {
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

  clk.CLK_16k  = not(DIV_Q5); // is this really inverted?
  clk.CLK_64k  = DIV_Q3;
  clk.CLK_256k = DIV_Q1;
  clk.FF04_D0n = not(DIV_Q6);
  clk.FF04_D1n = not(DIV_Q7);

  wire CLK_1M_AQ = CLK_1M_A.q();
  wire CLK_1M_BQ = CLK_1M_B.q();
  wire CLK_1M_CQ = CLK_1M_C.q();
  wire CLK_1M_DQ = CLK_1M_D.q();

  wire AFER_Q = AFER.q();

  // combi clock deglitcher on CLKIN_B, skipping it

  clk.CLKIN_An  = not(ext.CLKIN_A);
  clk.CLK_4M_A  = ext.CLKIN_B;
  clk.CLK_4M_B  = not(ext.CLKIN_B);
  clk.APUV_4MHZ = not(ext.CLKIN_B);

  // guess this is used in the apu somewhere? still dunno who drives CYBO_4MHZ
  clk.CERY_2MHZ = CERY.flip(clk.CYBO_4MHZ, not(rst.APU_RESET));

  // comment about clock rates on schematic is wrong

  // makin' funny clocks
  clk.AFAS = and(CLK_1M_AQ, CLK_1M_BQ);

  clk.ABUZ = nor(dbg.T1nT2, unk3(not(not(CLK_1M_BQ)), not(CLK_1M_DQ), cpu.FROM_CPU4));
  clk.BUKE = and(not(CLK_1M_CQ), CLK_1M_DQ, not(clk.ABOL_1MHZ));

  cpu.CPU_RD_SYNC = and(cpu.FROM_CPU3, CLK_1M_AQ, CLK_1M_BQ);

  wire BAPY = nor(clk.ABOL_1MHZ, not(CLK_1M_DQ), not(CLK_1M_BQ));
  wire BEJA = nand(not(BAPY), not(nor(not(CLK_1M_BQ), clk.ABOL_1MHZ)));
  wire BALY = or(and(not(CLK_1M_CQ), not(CLK_1M_BQ), not(BEJA)), not(ext.CLKIN_A));
  clk.BOGA1MHZ = not(BALY);

  clk.BEDO = and(not(clk.ABOL_1MHZ), BALY);
  
  // this is probably the CPU's 1 mhz clock?
  cpu.TO_CPU = not(and(not(clk.ABOL_1MHZ), BALY));

  clk.PHI_OUTn = not(ext.PHI_OUT);

  {
    wire TUBO = unk2(clk.ABOL_1MHZ, or(ext.RESET, clk.CLKIN_An));
    wire TABA = or(dbg.T1nT2, dbg.T1nT2n, and(TUBO, DIV_Q15));
    wire ASOL = unk2(nor(not(TABA), ext.RESET), ext.RESET);
    rst.RESET2 = not(or(AFER_Q, ASOL));
    AFER.tock(not(clk.BOGA1MHZ), dbg.T1nT2n, ASOL);
  }

  clk.PHIn = not(nor(not(CLK_1M_BQ), clk.ABOL_1MHZ));

  rst.RESET7 = not(rst.RESET2);
  rst.RESET7n = rst.RESET2;
  rst.WESY    = rst.RESET2;
  rst.RESET9  = rst.RESET2;
  rst.RESET8  = rst.RESET2;
  rst.RESET6  = rst.RESET2;
  rst.XARE = not(rst.RESET7);
  rst.RESET_VIDEO  = nand(ppu.LCDC_LCDEN, not(rst.RESET7));
  rst.RESET_VIDEOn = and(ppu.LCDC_LCDEN, not(rst.RESET7));
  rst.RESET_VIDEO2n = not(rst.RESET_VIDEO);

  wire BARA_Q = BARA.q();
  wire CARU_Q = CARU.q();
  wire BYLU_Q = BYLU.q();

  clk.HORU_512 = mux2(clk.HAMA_512Kn, !BARA_Q, apu.FERO_Q);
  clk.BUFY_256 = mux2(clk.HAMA_512Kn, not(!CARU_Q), apu.FERO_Q);
  clk.BYFE_128 = mux2(clk.HAMA_512Kn, not(!BYLU_Q), apu.FERO_Q);

  wire ATYK_Q = ATYK.flip(not(ext.CLKIN_B),   not(rst.APU_RESET));
  wire AVOK_Q = AVOK.flip(ATYK_Q,      not(rst.APU_RESET));
  wire JESO_Q = JESO.flip(not(AVOK_Q), rst.APU_RESET5n);
  clk.JESO_512K = JESO_Q;
  clk.BAVU_1MHZ = not(AVOK_Q);
  clk.HAMA_512Kn = not(JESO_Q);

  BARA.tock(not(clk.AJER_2MHZ), not(rst.APU_RESET), not(DIV_Q10));
  CARU.flip(BARA_Q, not(rst.APU_RESET));
  BYLU.flip(!CARU_Q, not(rst.APU_RESET));

  //----------
  // registers

  CLK_1M_A.tock(not(ext.CLKIN_B), dbg.T1nT2n, CLK_1M_DQ);
  CLK_1M_B.tock(not(ext.CLKIN_B), dbg.T1nT2n, !CLK_1M_AQ);
  CLK_1M_C.tock(not(ext.CLKIN_B), dbg.T1nT2n, !CLK_1M_BQ);
  CLK_1M_D.tock(not(ext.CLKIN_B), dbg.T1nT2n, !CLK_1M_CQ);
}
