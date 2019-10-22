#include "Schematics.h"
#include "Register.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <functional>
#include <stdarg.h>

namespace {

void r(int i, int y, const char* name, int v) {
  if (i == 0) {
    printf("\033[%d;%dH%s", y, 0, name);
  }

  printf("\033[%d;%dH%c", y, i + 15 + (i/8), v ? 219 : 176);
}

void print_at(int x, int y, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\033[%d;%dH", y, x);
  vprintf(format, args);
  va_end(args);
}

//-----------------------------------------------------------------------------

struct P1_ClocksReset {

  struct Input {
    bool RESET; // low = reset, high = run
    bool CLK_GOOD;
    bool CLKIN_B;
    bool ABOL; // dunno

    bool FROM_CPU3;
    bool FROM_CPU4;

    bool T1;
    bool T2;
  };

  struct Output {
    bool ATAL_4MHZ;
    bool BOGA1MHZ;
    bool PHI_OUT;
    bool PHIn;
    bool CLK2;
    bool CLK1;
    bool RESET2;
    bool TO_CPU;
    bool CPU_RD_SYNC; 
    bool BUKE;
    bool ABUZ;
    bool AFAS;
  };

  void tick(int i);

  Input in;
  Output out;

  // 1mhz phase generator
  reg ADYK,AFUR,ALEF,APUK;

  // dunno, has something to do with holding the chip in reset for 1/16th of a sec
  reg AFER;

  // div
  reg UKUP,UFOR,UNER,TERO,UNYK,TAMA,UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;
};

//-----------------------------------------------------------------------------

void P1_ClocksReset::tick(int i)
{
  bool TAMA_Q = TAMA.q();
  bool UNYK_Q = UNYK.q();
  bool TERO_Q = TERO.q();
  bool UNER_Q = UNER.q();
  bool UFOR_Q = UFOR.q();
  bool UKUP_Q = UKUP.q();
  bool UGOT_Q = UGOT.q();
  bool TULU_Q = TULU.q();
  bool TUGO_Q = TUGO.q();
  bool TOFE_Q = TOFE.q();
  bool TERU_Q = TERU.q();
  bool SOLA_Q = SOLA.q();
  bool SUBU_Q = SUBU.q();
  bool TEKA_Q = TEKA.q();
  bool UKET_Q = UKET.q();
  bool RESET_TIMEOUT = UPOF.q();

  bool ADYK_Q = ADYK.q();
  bool AFUR_Q = AFUR.q();
  bool ALEF_Q = ALEF.q();
  bool APUK_Q = APUK.q();

  bool AFER_Q = AFER.q();

  //----------

  if (in.T1 || in.T2) {
    bool T1nT2  = and(in.T2, not(in.T1));

    out.AFAS = and(ADYK_Q, AFUR_Q);
    out.ABUZ = nor(T1nT2, unk3(AFUR_Q, not(APUK_Q), in.FROM_CPU4));
    out.CPU_RD_SYNC = and(in.FROM_CPU3, ADYK_Q, AFUR_Q);
    out.BOGA1MHZ = (ALEF_Q && AFUR_Q) || in.CLK_GOOD;

    out.TO_CPU = in.ABOL || (in.CLK_GOOD && (ALEF_Q || AFUR_Q));
    out.BUKE = nor(ALEF_Q, not(APUK_Q), in.ABOL);
    out.PHI_OUT = and(AFUR_Q, not(in.ABOL));

    bool ASOL;

    if (in.RESET) {
      ASOL = unk2(0, 1);
    }
    else {
      ASOL = unk2(1, 0);
    }

    if (ASOL) {
      out.RESET2 = 0;
      AFER.tock(not(out.BOGA1MHZ), 1, 1);
    }
    else {
      out.RESET2 = not(AFER_Q);
      AFER.tock(not(out.BOGA1MHZ), 1, 0);
    }

    ADYK.dtock(not(in.CLKIN_B), 1, APUK_Q);
    AFUR.dtock(not(in.CLKIN_B), 1, !ADYK_Q);
    ALEF.dtock(not(in.CLKIN_B), 1, AFUR_Q);
    APUK.dtock(not(in.CLKIN_B), 1, ALEF_Q);
  }
  else {

    out.AFAS = and(ADYK_Q, AFUR_Q);
    out.ABUZ = nor(false, unk3(AFUR_Q, not(APUK_Q), in.FROM_CPU4));
    out.CPU_RD_SYNC = and(in.FROM_CPU3, ADYK_Q, AFUR_Q);
    out.BOGA1MHZ = (ALEF_Q && AFUR_Q) || in.CLK_GOOD;

    if (in.ABOL) {
      out.TO_CPU  = 1;
      out.BUKE    = 0;
      out.PHI_OUT = 0;
    }
    else {
      out.TO_CPU  = (in.CLK_GOOD && (ALEF_Q || AFUR_Q));
      out.BUKE    = and(not(ALEF_Q), APUK_Q);
      out.PHI_OUT = AFUR_Q;
    }

    /*
    // as in schematic
    bool UPYF = or(in.RESET, not(in.CLK_GOOD));
    bool TUBO = unk2(in.ABOL, UPYF);
    bool UNUT = and(RESET_TIMEOUT, TUBO);
    bool TABA = UNUT;
    bool ALYP = not(TABA);
    bool AFAR = nor(ALYP, in.RESET);
    bool ASOL = unk2(AFAR, in.RESET);
    bool AVOR = or(AFER_Q, ASOL);
    bool ALUR = not(AVOR);
    out.RESET2 = ALUR;
    AFER.tock(not(out.BOGA1MHZ), !in.RESET, ASOL);

    // flattened down to unk2()s
    bool TUBO = unk2(in.ABOL, or(in.RESET, not(in.CLK_GOOD)));
    bool ASOL = unk2(nor(nand(RESET_TIMEOUT, TUBO), in.RESET), in.RESET);
    out.RESET2 = nor(AFER_Q, ASOL);
    AFER.tock(not(out.BOGA1MHZ), !in.RESET, ASOL);

    // or just one line, sort of?
    bool STOP = unk2(nor(nand(RESET_TIMEOUT, unk2(in.ABOL, or(in.RESET, not(in.CLK_GOOD)))), in.RESET), in.RESET);

    // the sane possibilities
    // unk2 0001 - runs during reset
    // unk2 0110 - if ABOL is high, we run when the clock is bad. probably wrong.
    // unk2 0111 - winner winner chicken dinner. this is just "or". :/
    // unk2 1000 - oscillates
    // unk2 1001 - oscillates
    // unk2 1110 - oscillates
    */

    bool STOP = false;
    if (in.ABOL || !in.CLK_GOOD) STOP = RESET_TIMEOUT;
    if (in.RESET) STOP = true;

    if (STOP) {
      out.RESET2 = 0;
      AFER.tock(not(out.BOGA1MHZ), 0, 1);
    }
    else {
      out.RESET2 = not(AFER_Q);
      AFER.tock(not(out.BOGA1MHZ), 1, 0);
    }


    ADYK.dtock(not(in.CLKIN_B),  !in.RESET, APUK_Q);
    AFUR.dtock(not(in.CLKIN_B),  !in.RESET, !ADYK_Q);
    ALEF.dtock(not(in.CLKIN_B),  !in.RESET, AFUR_Q);
    APUK.dtock(not(in.CLKIN_B),  !in.RESET, ALEF_Q);
  }

  //----------


  //----------

  bool RESET_DIV = in.RESET || not(in.CLK_GOOD);

  UKUP.tock(out.BOGA1MHZ, not(RESET_DIV), !UKUP_Q);
  UFOR.tock(!UKUP.q(),    not(RESET_DIV), !UFOR_Q);
  UNER.tock(!UFOR.q(),    not(RESET_DIV), !UNER_Q);
  TERO.tock(!UNER.q(),    not(RESET_DIV), !TERO_Q);
  UNYK.tock(!TERO.q(),    not(RESET_DIV), !UNYK_Q);
  TAMA.tock(!UNYK.q(),    not(RESET_DIV), !TAMA_Q);
  UGOT.tock(!TAMA.q(),    not(RESET_DIV), !UGOT_Q);
  TULU.tock(!UGOT.q(),    not(RESET_DIV), !TULU_Q);
  TUGO.tock(!TULU.q(),    not(RESET_DIV), !TUGO_Q);
  TOFE.tock(!TUGO.q(),    not(RESET_DIV), !TOFE_Q);
  TERU.tock(!TOFE.q(),    not(RESET_DIV), !TERU_Q);
  SOLA.tock(!TERU.q(),    not(RESET_DIV), !SOLA_Q);
  SUBU.tock(!SOLA.q(),    not(RESET_DIV), !SUBU_Q);
  TEKA.tock(!SUBU.q(),    not(RESET_DIV), !TEKA_Q);
  UKET.tock(!TEKA.q(),    not(RESET_DIV), !UKET_Q);
  UPOF.tock(!UKET.q(),    not(RESET_DIV), !RESET_TIMEOUT);

  //----------

  out.ATAL_4MHZ = not(in.CLKIN_B);
  out.PHIn      = not(out.PHI_OUT);
  out.CLK2      = not(in.CLKIN_B);
  out.CLK1      = in.CLKIN_B;

  //----------

  int y = 3;
  r(i, y++, "CLK_A", in.CLK_GOOD);
  r(i, y++, "CLK_B", in.CLKIN_B);
  y++;
}

//-----------------------------------------------------------------------------

void test_p1() {
  P1_ClocksReset sch;

  auto blah = [&sch]() { return sch.ADYK.q(); };

  sch.in.RESET = 1;
  sch.in.CLK_GOOD = 0;
  sch.in.CLKIN_B = 0;
  sch.in.ABOL = 0;

  sch.in.FROM_CPU3 = 1;
  sch.in.FROM_CPU4 = 0;

  sch.in.T1 = 0;
  sch.in.T2 = 0;

  for (int i = 0; i < 128; i++) {
    sch.in.CLK_GOOD = 1;
    sch.in.CLKIN_B = (~((i + 0) >> 0)) & 1;

    sch.tick(i);
  }

  print_at(0, 0, "done");
  print_at(0, 80, "done");
}

//-----------------------------------------------------------------------------

};