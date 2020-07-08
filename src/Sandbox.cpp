#include <stdio.h>

#include "Cells.h"

//------------------------------------------------------------------------------

struct Sandbox {

  Sandbox() {
    SYS_PIN_CLK_A.set(0);
    SYS_PIN_T1.set(0);
    SYS_PIN_T2.set(0);

    CPU_PIN_CLKREQ.set(0);
    CPU_PIN_RD.set(0);
    CPU_PIN_WR.set(0);
    CPU_PIN_ADDR_VALID.set(0);
    set_addr(0);

    EXT_PIN_RDn_C.set(1);
    EXT_PIN_WRn_C.set(1);
  }

  PinIn  SYS_PIN_CLK_A;
  PinIn  SYS_PIN_CLK_B;
  PinIn  SYS_PIN_T1;
  PinIn  SYS_PIN_T2;
  PinIn  SYS_PIN_RST;

  PinIn  CPU_PIN_CLKREQ;
  PinIn  CPU_PIN_RD;            // top right port PORTA_00: ->
  PinIn  CPU_PIN_WR;            // top right port PORTA_01: ->
  PinIn  CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  PinIn  CPU_PIN_A00;           // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01;           // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02;           // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03;           // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04;           // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05;           // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06;           // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07;           // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08;           // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09;           // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10;           // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11;           // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12;           // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13;           // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14;           // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15;           // bottom right port PORTB_30: -> A15

  PinIn  EXT_PIN_RDn_C;   // PIN_79 -> P07.UJYV
  PinIn  EXT_PIN_WRn_C;   // PIN_78 -> P07.UBAL

  PinOut CPU_PIN_PROD;
  PinOut CPU_PIN_EXT_RESET;
  PinOut CPU_PIN_EXT_CLKGOOD;
  PinOut CPU_PIN_DBG;

  /*p01.TUBO*/ NorLatch TUBO;
  /*p01.ASOL*/ NorLatch ASOL; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg13    AFER; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

  /*p01.AFUR*/ Reg9p AFUR_xBCDExxx; // these have positive reset
  /*p01.ALEF*/ Reg9p ALEF_xxCDEFxx; // these have positive reset
  /*p01.APUK*/ Reg9p APUK_xxxDEFGx; // these have positive reset
  /*p01.ADYK*/ Reg9p ADYK_xxxxEFGH; // these have positive reset

  /*p01.UKUP*/ Reg17 UKUP_DIV_00;
  /*p01.UFOR*/ Reg17 UFOR_DIV_01;
  /*p01.UNER*/ Reg17 UNER_DIV_02;
  /*p01.TERO*/ Reg17 TERO_DIV_03;
  /*p01.UNYK*/ Reg17 UNYK_DIV_04;
  /*p01.TAMA*/ Reg17 TAMA_DIV_05;
  /*p01.UGOT*/ Reg17 UGOT_DIV_06;
  /*p01.TULU*/ Reg17 TULU_DIV_07;
  /*p01.TUGO*/ Reg17 TUGO_DIV_08;
  /*p01.TOFE*/ Reg17 TOFE_DIV_09;
  /*p01.TERU*/ Reg17 TERU_DIV_10;
  /*p01.SOLA*/ Reg17 SOLA_DIV_11;
  /*p01.SUBU*/ Reg17 SUBU_DIV_12;
  /*p01.TEKA*/ Reg17 TEKA_DIV_13;
  /*p01.UKET*/ Reg17 UKET_DIV_14;
  /*p01.UPOF*/ Reg17 UPOF_DIV_15;

  void set_addr(uint16_t addr) {
    CPU_PIN_A00.set(addr & 0x0001);
    CPU_PIN_A01.set(addr & 0x0002);
    CPU_PIN_A02.set(addr & 0x0004);
    CPU_PIN_A03.set(addr & 0x0008);
    CPU_PIN_A04.set(addr & 0x0010);
    CPU_PIN_A05.set(addr & 0x0020);
    CPU_PIN_A06.set(addr & 0x0040);
    CPU_PIN_A07.set(addr & 0x0080);
    CPU_PIN_A08.set(addr & 0x0100);
    CPU_PIN_A09.set(addr & 0x0200);
    CPU_PIN_A10.set(addr & 0x0400);
    CPU_PIN_A11.set(addr & 0x0800);
    CPU_PIN_A12.set(addr & 0x1000);
    CPU_PIN_A13.set(addr & 0x2000);
    CPU_PIN_A14.set(addr & 0x4000);
    CPU_PIN_A15.set(addr & 0x8000);
  }

  void tick() {

    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(SYS_PIN_CLK_B);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);

    /*p07.UBET*/ wire UBET_T1n = not(SYS_PIN_T1);
    /*p07.UVAR*/ wire UVAR_T2n = not(SYS_PIN_T2);
    /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and(SYS_PIN_T1, UVAR_T2n);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and(SYS_PIN_T2, UBET_T1n);

    {
      // FIXME polarity issues again?
      /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, SYS_PIN_RST);
      /*p01.ADYK*/ ADYK_xxxxEFGH.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn, APUK_xxxDEFGx.q());
      /*p01.APUK*/ APUK_xxxDEFGx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn, ALEF_xxCDEFxx.q());
      /*p01.ALEF*/ ALEF_xxCDEFxx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn, AFUR_xBCDExxx.q());
      /*p01.AFUR*/ AFUR_xBCDExxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn, ADYK_xxxxEFGH.qn());
    }

    /*p01.ATEZ*/ wire ATEZ_CLKBAD  = not(SYS_PIN_CLK_A);
    /*p01.UCOB*/ wire UCOB_CLKBAD  = not(SYS_PIN_CLK_A);

    /*p01.BOGA*/ bool BOGA_AxCDEFGH;
    /*p01.BOMA*/ bool BOMA_xBxxxxxx;

    {
      /*p01.ABOL*/ wire ABOL_CLKREQn = not(CPU_PIN_CLKREQ);

      /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_xBCDExxx.qn());
      /*p01.AROV*/ wire AROV_xxxDEFGx = not(APUK_xxxDEFGx.qn());
      /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx.q());

      /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
      /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
      /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
      /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);

      /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
      /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
      /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
      /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    
      /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
      /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
      /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
      /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
      /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
      /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
      /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
      /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
      /*p01.BOGA*/ BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
      /*p01.BOMA*/ BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);
    }

    {
      /*p01.UPYF*/ wire UPYF = or(SYS_PIN_RST, UCOB_CLKBAD);
      /*p01.UNUT*/ wire UNUT = and(TUBO, UPOF_DIV_15);

      // Are we _sure_ this is a nor latch?
      /*p01.TUBO*/ TUBO.nor_latch(UPYF, CPU_PIN_CLKREQ);

      /*p01.TABA*/ wire TABA = or(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, UNUT);
      /*p01.ALYP*/ wire ALYP_RSTn = not(TABA);
      /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, SYS_PIN_RST);

      /*p07.UPOJ*/ wire UPOJ = nand(UBET_T1n, UVAR_T2n, SYS_PIN_RST);
      /*p01.AFER*/ AFER.set(BOGA_AxCDEFGH, BOMA_xBxxxxxx, UPOJ, ASOL);
      /*p01.ASOL*/ ASOL.nor_latch(SYS_PIN_RST, AFAR_RST); // Schematic wrong, this is a latch.

      CPU_PIN_DBG.set(TABA);
    }

    bool TAPE_FF04_WRp;
    {
      /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_xBCDExxx.qn());
      /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH.q());
      /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
      /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(CPU_PIN_WR, AFAS_xxxxxFGH);
      /*p01.APOV*/ wire APOV_CPU_WRp_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);
      /*p07.UBAL*/ wire UBAL_CPU_WRp_ABCDExxx = mux2_n(EXT_PIN_WRn_C, APOV_CPU_WRp_xxxxxFGH, UNOR_MODE_DBG2p);
      /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_CPU_WRp_ABCDExxx);
      /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
      /*p07.TONA*/ wire TONA_A08n = not(CPU_PIN_A08);
      /*p06.SARE*/ wire SARE_XX00_XX07p = nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03);
      /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
      /*p03.RYFO*/ wire RYFO_FF04_FF07p = and (CPU_PIN_A02, SARE_XX00_XX07p, SYKE_FF00_FFFFp);
      /*p03.TOVY*/ wire TOVY_A00n = not(CPU_PIN_A00);
      /*p08.TOLA*/ wire TOLA_A01n = not(CPU_PIN_A01);
      /*p01.TAPE*/ TAPE_FF04_WRp =  and(TAPU_CPU_WR_xxxxxFGH, RYFO_FF04_FF07p, TOLA_A01n, TOVY_A00n);
    }
    
    /*p01.UFOL*/ wire UFOL_DIV_RSTn = nor(UCOB_CLKBAD, SYS_PIN_RST, TAPE_FF04_WRp);

    /*p01.UPOF*/ UPOF_DIV_15.set(UKET_DIV_14.qn(), UFOL_DIV_RSTn, UPOF_DIV_15.qn());
    /*p01.UKET*/ UKET_DIV_14.set(TEKA_DIV_13.qn(), UFOL_DIV_RSTn, UKET_DIV_14.qn());
    /*p01.TEKA*/ TEKA_DIV_13.set(SUBU_DIV_12.qn(), UFOL_DIV_RSTn, TEKA_DIV_13.qn());
    /*p01.SUBU*/ SUBU_DIV_12.set(SOLA_DIV_11.qn(), UFOL_DIV_RSTn, SUBU_DIV_12.qn());
    /*p01.SOLA*/ SOLA_DIV_11.set(TERU_DIV_10.qn(), UFOL_DIV_RSTn, SOLA_DIV_11.qn());
    /*p01.TERU*/ TERU_DIV_10.set(TOFE_DIV_09.qn(), UFOL_DIV_RSTn, TERU_DIV_10.qn());
    /*p01.TOFE*/ TOFE_DIV_09.set(TUGO_DIV_08.qn(), UFOL_DIV_RSTn, TOFE_DIV_09.qn());
    /*p01.TUGO*/ TUGO_DIV_08.set(TULU_DIV_07.qn(), UFOL_DIV_RSTn, TUGO_DIV_08.qn());
    /*p01.TULU*/ TULU_DIV_07.set(UGOT_DIV_06.qn(), UFOL_DIV_RSTn, TULU_DIV_07.qn());
    /*p01.UGOT*/ UGOT_DIV_06.set(TAMA_DIV_05.qn(), UFOL_DIV_RSTn, UGOT_DIV_06.qn());
    /*p01.TAMA*/ TAMA_DIV_05.set(UNYK_DIV_04.qn(), UFOL_DIV_RSTn, TAMA_DIV_05.qn());
    /*p01.UNYK*/ UNYK_DIV_04.set(TERO_DIV_03.qn(), UFOL_DIV_RSTn, UNYK_DIV_04.qn());
    /*p01.TERO*/ TERO_DIV_03.set(UNER_DIV_02.qn(), UFOL_DIV_RSTn, TERO_DIV_03.qn());
    /*p01.UNER*/ UNER_DIV_02.set(UFOR_DIV_01.qn(), UFOL_DIV_RSTn, UNER_DIV_02.qn());
    /*p01.UFOR*/ UFOR_DIV_01.set(UKUP_DIV_00.qn(), UFOL_DIV_RSTn, UFOR_DIV_01.qn());
    /*p01.UKUP*/ UKUP_DIV_00.set(BOGA_AxCDEFGH,    UFOL_DIV_RSTn, UKUP_DIV_00.qn());

    CPU_PIN_PROD.set(AFER);
    CPU_PIN_EXT_RESET.set(SYS_PIN_RST);
    CPU_PIN_EXT_CLKGOOD.set(SYS_PIN_CLK_A);
  }

  int get_addr() {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03,
                CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
                CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11,
                CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  int get_div() {
    return pack(UKUP_DIV_00, UFOR_DIV_01, UNER_DIV_02, TERO_DIV_03,
                UNYK_DIV_04, TAMA_DIV_05, UGOT_DIV_06, TULU_DIV_07,
                TUGO_DIV_08, TOFE_DIV_09, TERU_DIV_10, SOLA_DIV_11,
                SUBU_DIV_12, TEKA_DIV_13, UKET_DIV_14, UPOF_DIV_15);
  }

  SignalHash commit() {
    SignalHash hash;

    hash << SYS_PIN_CLK_A;
    hash << SYS_PIN_CLK_B;
    hash << SYS_PIN_T1;
    hash << SYS_PIN_T2;
    hash << SYS_PIN_RST;

    hash << CPU_PIN_CLKREQ;
    hash << CPU_PIN_RD;
    hash << CPU_PIN_WR;
    hash << CPU_PIN_ADDR_VALID;

    hash << CPU_PIN_A00;
    hash << CPU_PIN_A01;
    hash << CPU_PIN_A02;
    hash << CPU_PIN_A03;
    hash << CPU_PIN_A04;
    hash << CPU_PIN_A05;
    hash << CPU_PIN_A06;
    hash << CPU_PIN_A07;
    hash << CPU_PIN_A08;
    hash << CPU_PIN_A09;
    hash << CPU_PIN_A10;
    hash << CPU_PIN_A11;
    hash << CPU_PIN_A12;
    hash << CPU_PIN_A13;
    hash << CPU_PIN_A14;
    hash << CPU_PIN_A15;

    hash << EXT_PIN_RDn_C;
    hash << EXT_PIN_WRn_C;

    hash << CPU_PIN_PROD.commit();
    hash << CPU_PIN_EXT_RESET.commit();
    hash << CPU_PIN_EXT_CLKGOOD.commit();
    hash << CPU_PIN_DBG.commit();

    hash << TUBO.commit();
    hash << ASOL.commit();
    hash << AFER.commit();

    hash << AFUR_xBCDExxx.commit();
    hash << ALEF_xxCDEFxx.commit();
    hash << APUK_xxxDEFGx.commit();
    hash << ADYK_xxxxEFGH.commit();

    hash << UKUP_DIV_00.commit();
    hash << UFOR_DIV_01.commit();
    hash << UNER_DIV_02.commit();
    hash << TERO_DIV_03.commit();
    hash << UNYK_DIV_04.commit();
    hash << TAMA_DIV_05.commit();
    hash << UGOT_DIV_06.commit();
    hash << TULU_DIV_07.commit();
    hash << TUGO_DIV_08.commit();
    hash << TOFE_DIV_09.commit();
    hash << TERU_DIV_10.commit();
    hash << SOLA_DIV_11.commit();
    hash << SUBU_DIV_12.commit();
    hash << TEKA_DIV_13.commit();
    hash << UKET_DIV_14.commit();
    hash << UPOF_DIV_15.commit();

    return hash;
  }

  void dump(SignalHash hash, int passes) {

    auto dec = [](SignalState s) {
      if (s.error) return "ERR";
      if (s.hiz)   return "HIZ";
      if (s.clk)   return s.val ? "^1^" : "^0^";
      else         return s.val ? ".1." : ".0.";
    };

    printf("0x%016llx %2d   %s  %s  %s  %s  %s  ", hash.h, passes, dec(SYS_PIN_CLK_A), dec(SYS_PIN_CLK_B), dec(SYS_PIN_T1), dec(SYS_PIN_T2), dec(SYS_PIN_RST));
    printf("%s ", dec(TUBO.a));
    printf("%d ", get_div());

    printf("\n");
  }
};

//------------------------------------------------------------------------------

void phase(Sandbox& sandbox) {
  SignalHash hash;
  int pass = 0;
  for (pass = 0; pass < 100; pass++) {
    sandbox.tick();
    SignalHash new_hash = sandbox.commit();
    //printf("0x%016llx\n", new_hash.h);
    if (new_hash.h == hash.h) break;
    hash = new_hash;
  }

  sandbox.dump(hash, pass);
}

void tphase(Sandbox& sandbox) {
  sandbox.SYS_PIN_CLK_B.set(0);
  phase(sandbox);
  sandbox.SYS_PIN_CLK_B.set(1);
  phase(sandbox);
}

void tphase_rst(Sandbox& sandbox) {
  sandbox.SYS_PIN_RST.set(0);
  tphase(sandbox);
}

void tphase_run(Sandbox& sandbox) {
  sandbox.SYS_PIN_RST.set(1);
  tphase(sandbox);
}

//------------------------------------------------------------------------------

int main() {
  printf("Hello World\n");

  Sandbox sandbox;

  printf("-------HASH------- PASS CLKA CLKB T1   T2   RST  TUBO\n");

  SignalHash hash;
  sandbox.dump(hash, 0);
  printf("-----------------------------------------------------\n");

  tphase_rst(sandbox);

  sandbox.SYS_PIN_CLK_A.set(1);

  for (int i = 0; i < 20; i++) {
    tphase_run(sandbox);
  }
}

//------------------------------------------------------------------------------