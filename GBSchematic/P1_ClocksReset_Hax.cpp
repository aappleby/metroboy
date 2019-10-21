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
    bool RESET;
    bool CLKIN_A;
    bool CLKIN_B;
    bool ABOL_1MHZ; // dunno

    bool FROM_CPU3;
    bool FROM_CPU4;

    bool T1T2n;   // true for debugging
    bool T1nT2;   // true for debugging
    bool T1nT2n;  // true for normal mode
  };

  struct Output {
    bool ATAL_4MHZ;
    bool BOGA1MHZ;
    bool PHI_OUT;
    bool PHIn;
    bool CLKIN_An;
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

wire unk3_test(wire a, wire b, wire c) {
  return a ^ b ^ c;
}


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
  bool UPOF_Q = UPOF.q();

  bool ADYK_Q = ADYK.q();
  bool AFUR_Q = AFUR.q();
  bool ALEF_Q = ALEF.q();
  bool APUK_Q = APUK.q();

  bool AFER_Q = AFER.q();

  //----------
  // bottom left

  bool UCOB = not(in.CLKIN_A);
  bool CLKIN_An = UCOB;

  bool ATAL = not(in.CLKIN_B);
  bool ATAL_4MHZ = ATAL;
  bool AZOF = not(ATAL);

  bool ZAXY = not(AZOF);

  bool ZEME = not(ZAXY);

  bool ALET = not(ZEME);

  bool CLK1 = ZEME;
  bool CLK2 = ALET;

  //----------
  // bottom right

  bool UPYF = or(in.RESET, CLKIN_An);
  bool TUBO = unk2(in.ABOL_1MHZ, UPYF);
  bool UNUT = and(TUBO, UPOF_Q);
  bool TABA = or(in.T1nT2, in.T1T2n, UNUT);

  //----------
  // top left

  bool ADAR = not(ADYK_Q);
  bool ATYP = not(AFUR_Q);
  bool AFEP = not(ALEF_Q);
  bool AROV = not(APUK_Q);

  bool AFAS = nor(ADAR, ATYP);
  bool AJAX = not(ATYP);
  bool BUGO = not(AFEP);

  bool AREV = nand(in.FROM_CPU3, AFAS);
  bool AGUT = unk3_test(AJAX, AROV, in.FROM_CPU4);
  bool BATE = nor(BUGO, AROV, in.ABOL_1MHZ);

  bool APOV = not(AREV);
  bool CPU_RD_SYNC = APOV;
  bool AWOD = or(in.T1nT2, AGUT);
  bool BASU = not(BATE);

  bool ABUZ = not(AWOD);
  bool BUKE = not(BASU);

  bool BAPY = nor(in.ABOL_1MHZ, AROV, ATYP);
  bool NULE = nor(ATYP, in.ABOL_1MHZ);

  bool BERU = not(BAPY);
  bool BYRY = not(NULE);

  bool BUFA = not(BERU);
  bool BYLY = not(BERU);
  bool BUDE = not(BYRY);
  bool BEVA = not(BYRY);
  bool PHI_OUT = BEVA;
  bool DOVA = not(BEVA);
  bool PHIn = DOVA;

  bool BOLO = not(BUFA);
  bool BYDA = not(BYLY);
  bool BEKO = not(BUDE);
  bool BAVY = not(BEVA);

  bool BEJA = nand(BOLO, BYDA, BEKO, BAVY);
  bool BANE = not(BEJA);
  bool BELO = not(BANE);
  bool BAZE = not(BELO);
  bool BUTO = nand(AFEP, ATYP, BAZE);

  bool BELE = not(BUTO);
  bool ATEZ = not(in.CLKIN_A);

  bool BYJU = nor(BELE, ATEZ);
  bool ALYP = not(TABA);

  bool BUTY = not(in.ABOL_1MHZ);
  bool BALY = not(BYJU);
  bool AFAR = nor(ALYP, in.RESET);

  bool BUVU = and(BUTY, BALY);
  bool BYXO = not(BUVU);
  bool BEDO = not(BYXO);
  bool BOWA = not(BEDO);
  bool TO_CPU = BOWA;

  bool BOGA = not(BALY);
  bool BOGA1MHZ = BOGA;
  bool BOMA = not(BOGA);

  bool ASOL = unk2(AFAR, in.RESET);
  bool AVOR = or(AFER_Q, ASOL);
  bool ALUR = not(AVOR);
  bool RESET2 = ALUR;

  //----------

  bool RESET_DIVn = nor(CLKIN_An, in.RESET);

  //----------

  UKUP.tock(BOGA1MHZ,   RESET_DIVn, !UKUP_Q);
  UFOR.tock(!UKUP.q(),  RESET_DIVn, !UFOR_Q);
  UNER.tock(!UFOR.q(),  RESET_DIVn, !UNER_Q);
  TERO.tock(!UNER.q(),  RESET_DIVn, !TERO_Q);
  UNYK.tock(!TERO.q(),  RESET_DIVn, !UNYK_Q);
  TAMA.tock(!UNYK.q(),  RESET_DIVn, !TAMA_Q);
  UGOT.tock(BOGA1MHZ,   RESET_DIVn, !UGOT_Q);
  TULU.tock(!UGOT.q(),  RESET_DIVn, !TULU_Q);
  TUGO.tock(!TULU.q(),  RESET_DIVn, !TUGO_Q);
  TOFE.tock(!TUGO.q(),  RESET_DIVn, !TOFE_Q);
  TERU.tock(!TOFE.q(),  RESET_DIVn, !TERU_Q);
  SOLA.tock(!TERU.q(),  RESET_DIVn, !SOLA_Q);
  SUBU.tock(!SOLA.q(),  RESET_DIVn, !SUBU_Q);
  TEKA.tock(!SUBU.q(),  RESET_DIVn, !TEKA_Q);
  UKET.tock(!TEKA.q(),  RESET_DIVn, !UKET_Q);
  UPOF.tock(!UKET.q(),  RESET_DIVn, !UPOF_Q);


  ADYK.dtock(ATAL_4MHZ, in.T1nT2n, APUK_Q);
  AFUR.dtock(ATAL_4MHZ, in.T1nT2n, !ADYK_Q);
  ALEF.dtock(ATAL_4MHZ, in.T1nT2n, AFUR_Q);
  APUK.dtock(ATAL_4MHZ, in.T1nT2n, ALEF_Q);

  AFER.tock(BOMA, in.T1nT2n, ASOL);

  //r("ADYK", i, 2, ADYK_Q);
  int y = 3;
  r(i, y++, "CLK_A", in.CLKIN_A);
  r(i, y++, "CLK_B", in.CLKIN_B);
  y++;

  r(i, y++, "ADYK", ADYK_Q);
  r(i, y++, "AFUR", AFUR_Q);
  r(i, y++, "ALEF", ALEF_Q);
  r(i, y++, "APUK", APUK_Q);
  y++;

  r(i, y++, "ADAR", ADAR);
  r(i, y++, "ATYP", ATYP);
  r(i, y++, "AFEP", AFEP);
  r(i, y++, "AROV", AROV);
  y++;

  r(i, y++, "AFAS", AFAS);
  r(i, y++, "AJAX", AJAX);
  r(i, y++, "BUGO", BUGO);
  y++;

  r(i, y++, "AREV", AREV);
  r(i, y++, "AGUT", AGUT);
  r(i, y++, "BATE", BATE);
  y++;

  r(i, y++, "BELE", BELE);
  r(i, y++, "ATEZ", ATEZ);
  r(i, y++, "BYJU", BYJU);
  r(i, y++, "BALY", BALY);
  r(i, y++, "BOGA", BOGA);
  y++;

  r(i, y++, "UKUP", UKUP_Q);
  r(i, y++, "UFOR", UFOR_Q);
  r(i, y++, "UNER", UNER_Q);
  r(i, y++, "TERO", TERO_Q);
  y++;

  r(i, y++, "PHI_OUT", PHI_OUT);

  //----------

  out.ATAL_4MHZ = ATAL_4MHZ;
  out.BOGA1MHZ = BOGA1MHZ;
  out.PHI_OUT = PHI_OUT;
  out.PHIn = PHIn;
  out.CLKIN_An = CLKIN_An;
  out.CLK2 = CLK2;
  out.CLK1 = CLK1;
  out.RESET2 = RESET2;
  out.TO_CPU = TO_CPU;
  out.CPU_RD_SYNC = CPU_RD_SYNC;
  out.BUKE = BUKE;
  out.ABUZ = ABUZ;
  out.AFAS = AFAS;
}

//-----------------------------------------------------------------------------

void test_p1() {
  P1_ClocksReset sch;

  auto blah = [&sch]() { return sch.ADYK.q(); };

  sch.in.RESET = 0;
  sch.in.CLKIN_A = 0;
  sch.in.CLKIN_B = 0;
  sch.in.ABOL_1MHZ = 0;

  sch.in.FROM_CPU3 = 1;
  sch.in.FROM_CPU4 = 0;

  sch.in.T1T2n = 0;
  sch.in.T1nT2 = 0;
  sch.in.T1nT2n = 1;

  for (int i = 0; i < 128; i++) {
    sch.in.CLKIN_A = 1;
    sch.in.CLKIN_B = (~((i + 0) >> 0)) & 1;

    sch.tick(i);
  }

  print_at(0, 0, "done");
  print_at(0, 80, "done");
}

//-----------------------------------------------------------------------------

};