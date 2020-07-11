#include "Sandbox.h"
#include "Console.h"

#include "Cells.h"

#include <stdio.h>
#include <stdarg.h>

#pragma warning(disable:4100)

//void print_at(int x, int y, int color, const char* format, ...);

//void print_signal(int x, int y, const char* name, SignalState s) {
//  print_at(x, y, 0b1111, "%s %d\n", name, s.val);
//} 

//------------------------------------------------------------------------------

struct Sandbox {

  Sandbox() {
    SYS_PIN_CLK_A.set(0);
    SYS_PIN_T1.set(0);
    SYS_PIN_T2.set(0);

    CPU_PIN_READYp.set(0);
    CPU_PIN_RDp.set(0);
    CPU_PIN_WRp.set(0);
    CPU_PIN_ADDR_VALID.set(0);
    set_addr(0);

    EXT_PIN_RDn_C.set(1);
    EXT_PIN_WRp_C.set(1);
  }

  PinIn  SYS_PIN_CLK_A;
  PinIn  SYS_PIN_CLK_B;
  PinIn  SYS_PIN_T1;
  PinIn  SYS_PIN_T2;
  PinIn  SYS_PIN_RSTp;

  PinIn  EXT_PIN_RDn_C;   // PIN_79 -> P07.UJYV
  PinIn  EXT_PIN_WRp_C;   // PIN_78 -> P07.UBAL

  PinIn  CPU_PIN_READYp;
  PinIn  CPU_PIN_RDp;            // top right port PORTA_00: ->
  PinIn  CPU_PIN_WRp;            // top right port PORTA_01: ->
  PinIn  CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  PinOut CPU_PIN_SYS_RSTp;
  PinOut CPU_PIN_EXT_RST;
  PinOut CPU_PIN_EXT_CLKGOOD;
  PinOut CPU_PIN_POR_DONEp;

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

  /*p01.TUBO*/ NorLatch TUBO_CPU_READYn;
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg13    AFER_SYS_RSTp; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

  /*p01.AFUR*/ Reg9 AFUR_ABCDxxxx;
  /*p01.ALEF*/ Reg9 ALEF_xBCDExxx;
  /*p01.APUK*/ Reg9 APUK_xxCDEFxx;
  /*p01.ADYK*/ Reg9 ADYK_xxxDEFGx;

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

  //--------------------------------------------------------------------------------

  void tick_simplified() {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = SYS_PIN_CLK_B;

    /*p01.ADYK*/ ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, nand(not(SYS_PIN_T1), not(SYS_PIN_T2), SYS_PIN_RSTp), APUK_xxCDEFxx.q());
    /*p01.APUK*/ APUK_xxCDEFxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, nand(not(SYS_PIN_T1), not(SYS_PIN_T2), SYS_PIN_RSTp), ALEF_xBCDExxx.q());
    /*p01.ALEF*/ ALEF_xBCDExxx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, nand(not(SYS_PIN_T1), not(SYS_PIN_T2), SYS_PIN_RSTp), AFUR_ABCDxxxx.q());
    /*p01.AFUR*/ AFUR_ABCDxxxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, nand(not(SYS_PIN_T1), not(SYS_PIN_T2), SYS_PIN_RSTp), ADYK_xxxDEFGx.qn());

    /*p01.BOLO*/ wire BOLO_xBCDEFGx = or(not(CPU_PIN_READYp),  APUK_xxCDEFxx, AFUR_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_xBCDExxx = and(BOLO_xBCDEFGx, nand(CPU_PIN_READYp,  !AFUR_ABCDxxxx));
    /*p01.BELE*/ wire BELE_Axxxxxxx = and(!ALEF_xBCDExxx, AFUR_ABCDxxxx, BAZE_xBCDExxx);
    /*p01.BOGA*/ wire BOGA_xBCDEFGH = or(BELE_Axxxxxxx, not(SYS_PIN_CLK_A));
    /*p01.BOMA*/ wire BOMA_Axxxxxxx = !or(BELE_Axxxxxxx, not(SYS_PIN_CLK_A));

    /*p01.TUBO*/ TUBO_CPU_READYn.nor_latch(or(SYS_PIN_RSTp, not(SYS_PIN_CLK_A)), CPU_PIN_READYp);

    /*p01.TABA*/ wire TABA_RSTp = or(and(SYS_PIN_T2, not(SYS_PIN_T1)),
                                     and(SYS_PIN_T1, not(SYS_PIN_T2)),
                                     and(TUBO_CPU_READYn, UPOF_DIV_15));

    /*p01.ASOL*/ ASOL_POR_DONEn.nor_latch(SYS_PIN_RSTp, and(TABA_RSTp, !SYS_PIN_RSTp));

    /*p01.AFER*/ AFER_SYS_RSTp.set(BOGA_xBCDEFGH,
                                         BOMA_Axxxxxxx,
                                         nand(not(SYS_PIN_T1), not(SYS_PIN_T2), SYS_PIN_RSTp),
                                         ASOL_POR_DONEn);


    /*p01.AVOR*/ wire AVOR_RST = or(AFER_SYS_RSTp, ASOL_POR_DONEn);
    (void)AVOR_RST;

    wire ADDR_FF04p = false;

    /*p07.TAPU*/ wire TAPU_CPU_WRp = mux2_p(EXT_PIN_WRp_C, and(CPU_PIN_WRp, ADYK_xxxDEFGx, !AFUR_ABCDxxxx), and(SYS_PIN_T2, not(SYS_PIN_T1)));
    /*p01.UFOL*/ wire UFOL_DIV_RST = and(SYS_PIN_CLK_A, !SYS_PIN_RSTp, !and(TAPU_CPU_WRp, ADDR_FF04p));

    /*p01.UPOF*/ UPOF_DIV_15.set(UKET_DIV_14.qn(), UFOL_DIV_RST, UPOF_DIV_15.qn());
    /*p01.UKET*/ UKET_DIV_14.set(TEKA_DIV_13.qn(), UFOL_DIV_RST, UKET_DIV_14.qn());
    /*p01.TEKA*/ TEKA_DIV_13.set(SUBU_DIV_12.qn(), UFOL_DIV_RST, TEKA_DIV_13.qn());
    /*p01.SUBU*/ SUBU_DIV_12.set(SOLA_DIV_11.qn(), UFOL_DIV_RST, SUBU_DIV_12.qn());
    /*p01.SOLA*/ SOLA_DIV_11.set(TERU_DIV_10.qn(), UFOL_DIV_RST, SOLA_DIV_11.qn());
    /*p01.TERU*/ TERU_DIV_10.set(TOFE_DIV_09.qn(), UFOL_DIV_RST, TERU_DIV_10.qn());
    /*p01.TOFE*/ TOFE_DIV_09.set(TUGO_DIV_08.qn(), UFOL_DIV_RST, TOFE_DIV_09.qn());
    /*p01.TUGO*/ TUGO_DIV_08.set(TULU_DIV_07.qn(), UFOL_DIV_RST, TUGO_DIV_08.qn());
    /*p01.TULU*/ TULU_DIV_07.set(UGOT_DIV_06.qn(), UFOL_DIV_RST, TULU_DIV_07.qn());
    /*p01.UGOT*/ UGOT_DIV_06.set(TAMA_DIV_05.qn(), UFOL_DIV_RST, UGOT_DIV_06.qn());
    /*p01.TAMA*/ TAMA_DIV_05.set(UNYK_DIV_04.qn(), UFOL_DIV_RST, TAMA_DIV_05.qn());
    /*p01.UNYK*/ UNYK_DIV_04.set(TERO_DIV_03.qn(), UFOL_DIV_RST, UNYK_DIV_04.qn());
    /*p01.TERO*/ TERO_DIV_03.set(UNER_DIV_02.qn(), UFOL_DIV_RST, TERO_DIV_03.qn());
    /*p01.UNER*/ UNER_DIV_02.set(UFOR_DIV_01.qn(), UFOL_DIV_RST, UNER_DIV_02.qn());
    /*p01.UFOR*/ UFOR_DIV_01.set(UKUP_DIV_00.qn(), UFOL_DIV_RST, UFOR_DIV_01.qn());
    /*p01.UKUP*/ UKUP_DIV_00.set(BOGA_xBCDEFGH,    UFOL_DIV_RST, UKUP_DIV_00.qn());

    CPU_PIN_EXT_RST.set(SYS_PIN_RSTp);
    CPU_PIN_POR_DONEp.set(TABA_RSTp);
    CPU_PIN_SYS_RSTp.set(AFER_SYS_RSTp);
    CPU_PIN_EXT_CLKGOOD.set(SYS_PIN_CLK_A);
  }


  //--------------------------------------------------------------------------------

  void tick() {

    /*p01.ATAL*/ wire ATAL_xBxDxFxH = SYS_PIN_CLK_B;

    // UBET - tadpole points at VCC
    // UVAR - tadpole points at VCC

    /*p07.UBET*/ wire UBET_T1n = not(SYS_PIN_T1);
    /*p07.UVAR*/ wire UVAR_T2n = not(SYS_PIN_T2);
    /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and(SYS_PIN_T1, UVAR_T2n);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and(SYS_PIN_T2, UBET_T1n);

    {
      // FIXME polarity issues again?
      /*p07.UPOJ*/ wire UPOJ_STOP_CLOCKn = nand(UBET_T1n, UVAR_T2n, SYS_PIN_RSTp);
      /*p01.ADYK*/ ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_STOP_CLOCKn, APUK_xxCDEFxx.q());
      /*p01.APUK*/ APUK_xxCDEFxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_STOP_CLOCKn, ALEF_xBCDExxx.q());
      /*p01.ALEF*/ ALEF_xBCDExxx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_STOP_CLOCKn, AFUR_ABCDxxxx.q());
      /*p01.AFUR*/ AFUR_ABCDxxxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_STOP_CLOCKn, ADYK_xxxDEFGx.qn());
    }

    bool BELE_Axxxxxxx;
    {
#if 0
      // CPU_PIN_CLKREQ does not actually affect BELE

      if (CPU_PIN_READYp) {
        /*p01.BELE*/ BELE_Axxxxxxx = and(!ALEF_xxCDEFx,
                                         AFUR_ABCDxxxx,
                                         or(APUK_xxCDEFxx, AFUR_ABCDxxxx)
                                         AFUR_ABCDxxxx);
      }
      else {

        /*p01.BELE*/ BELE_Axxxxxxx = and(!ALEF_xBCDExxx, AFUR_ABCDxxxx);
      }
      ///*p01.BELE*/ BELE_xBxxxxxx = xBxxxxxx;

#endif

      /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
      /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_ABCDxxxx.qn());
      /*p01.AROV*/ wire AROV_xxxDEFGx = not(APUK_xxCDEFxx.qn());
      /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xBCDExxx.q());

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
      /*p01.BELE*/ BELE_Axxxxxxx = not(BUTO_AxCDEFGH);

    }



    {

      /*p01.UCOB*/ wire UCOB_CLKBAD  = not(SYS_PIN_CLK_A);
      /*p01.UPYF*/ wire UPYF_CPU_READY_RSTp = or(SYS_PIN_RSTp, UCOB_CLKBAD);
      /*p01.TUBO*/ TUBO_CPU_READYn.nor_latch(UPYF_CPU_READY_RSTp, CPU_PIN_READYp);
    }



    {
      /*p01.UNUT*/ wire UNUT_POR_DONEp      = and(TUBO_CPU_READYn, UPOF_DIV_15);
      /*p01.TABA*/ wire TABA_POR_DONEp      = or(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, UNUT_POR_DONEp);
      /*p01.ALYP*/ wire ALYP_POR_DONEn      = not(TABA_POR_DONEp);
      /*p01.AFAR*/ wire AFAR_POR_DONE_RSTp  = nor(ALYP_POR_DONEn, SYS_PIN_RSTp);
      /*p01.ASOL*/ ASOL_POR_DONEn.nor_latch(SYS_PIN_RSTp, AFAR_POR_DONE_RSTp);

      CPU_PIN_POR_DONEp.set(TABA_POR_DONEp);
    }

    {
      /*p07.UPOJ*/ wire UPOJ = nand(UBET_T1n, UVAR_T2n, SYS_PIN_RSTp);

      /*p01.ATEZ*/ wire ATEZ_CLKBAD  = not(SYS_PIN_CLK_A);
      /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, ATEZ_CLKBAD);
      /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
      /*p01.BOGA*/ wire BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
      /*p01.BOMA*/ wire BOMA_Axxxxxxx = not(BOGA_xBCDEFGH);
      /*p01.AFER*/ AFER_SYS_RSTp.set(BOGA_xBCDEFGH, BOMA_Axxxxxxx, UPOJ, ASOL_POR_DONEn);
      /*p01.AVOR*/ wire AVOR_TOP_RST = or(AFER_SYS_RSTp.q(), ASOL_POR_DONEn.q());
      (void)AVOR_TOP_RST;

      CPU_PIN_SYS_RSTp.set(AFER_SYS_RSTp);
    }

    CPU_PIN_EXT_RST.set(SYS_PIN_RSTp);

    {
      /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_ABCDxxxx.qn());
      /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxDEFGx.q());
      /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);  // outline black on overlay

      /*p01.ATEZ*/ wire ATEZ_CLKBAD  = not(SYS_PIN_CLK_A);
      /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, ATEZ_CLKBAD);
      /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
      /*p01.BOGA*/ wire BOGA_xBCDEFGH = not(BALY_Axxxxxxx);

      /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
      /*p07.TONA*/ wire TONA_A08n = not(CPU_PIN_A08);
      /*p06.SARE*/ wire SARE_XX00_XX07p = nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03);
      /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
      /*p03.RYFO*/ wire RYFO_FF04_FF07p = and (CPU_PIN_A02, SARE_XX00_XX07p, SYKE_FF00_FFFFp);
      /*p03.TOVY*/ wire TOVY_A00n = not(CPU_PIN_A00);
      /*p08.TOLA*/ wire TOLA_A01n = not(CPU_PIN_A01);

      // And this makes no sense either - CPU_PIN_WR must gate the write clock when _not_ writing, which means it has to be WRp

      /*p01.AREV*/ wire AREV_CPU_WRn = nand(CPU_PIN_WRp, AFAS_xxxxEFGx);
      /*p01.APOV*/ wire APOV_CPU_WRp = not(AREV_CPU_WRn);

      /*p07.UBAL*/ wire UBAL_CPU_WRn = mux2_n(EXT_PIN_WRp_C, APOV_CPU_WRp, UNOR_MODE_DBG2p);
      /*p07.TAPU*/ wire TAPU_CPU_WRp = not(UBAL_CPU_WRn);

      /*p01.TAPE*/ wire TAPE_FF04_WR = and(TAPU_CPU_WRp, RYFO_FF04_FF07p, TOLA_A01n, TOVY_A00n);
      /*p01.UCOB*/ wire UCOB_CLKBAD  = not(SYS_PIN_CLK_A);
      /*p01.UFOL*/ wire UFOL_DIV_RST = nor(UCOB_CLKBAD, SYS_PIN_RSTp, TAPE_FF04_WR);
      /*p01.UKUP*/ UKUP_DIV_00.set(BOGA_xBCDEFGH, UFOL_DIV_RST, UKUP_DIV_00.qn());

      /*p01.UPOF*/ UPOF_DIV_15.set(UKET_DIV_14.qn(), UFOL_DIV_RST, UPOF_DIV_15.qn());
      /*p01.UKET*/ UKET_DIV_14.set(TEKA_DIV_13.qn(), UFOL_DIV_RST, UKET_DIV_14.qn());
      /*p01.TEKA*/ TEKA_DIV_13.set(SUBU_DIV_12.qn(), UFOL_DIV_RST, TEKA_DIV_13.qn());
      /*p01.SUBU*/ SUBU_DIV_12.set(SOLA_DIV_11.qn(), UFOL_DIV_RST, SUBU_DIV_12.qn());
      /*p01.SOLA*/ SOLA_DIV_11.set(TERU_DIV_10.qn(), UFOL_DIV_RST, SOLA_DIV_11.qn());
      /*p01.TERU*/ TERU_DIV_10.set(TOFE_DIV_09.qn(), UFOL_DIV_RST, TERU_DIV_10.qn());
      /*p01.TOFE*/ TOFE_DIV_09.set(TUGO_DIV_08.qn(), UFOL_DIV_RST, TOFE_DIV_09.qn());
      /*p01.TUGO*/ TUGO_DIV_08.set(TULU_DIV_07.qn(), UFOL_DIV_RST, TUGO_DIV_08.qn());
      /*p01.TULU*/ TULU_DIV_07.set(UGOT_DIV_06.qn(), UFOL_DIV_RST, TULU_DIV_07.qn());
      /*p01.UGOT*/ UGOT_DIV_06.set(TAMA_DIV_05.qn(), UFOL_DIV_RST, UGOT_DIV_06.qn());
      /*p01.TAMA*/ TAMA_DIV_05.set(UNYK_DIV_04.qn(), UFOL_DIV_RST, TAMA_DIV_05.qn());
      /*p01.UNYK*/ UNYK_DIV_04.set(TERO_DIV_03.qn(), UFOL_DIV_RST, UNYK_DIV_04.qn());
      /*p01.TERO*/ TERO_DIV_03.set(UNER_DIV_02.qn(), UFOL_DIV_RST, TERO_DIV_03.qn());
      /*p01.UNER*/ UNER_DIV_02.set(UFOR_DIV_01.qn(), UFOL_DIV_RST, UNER_DIV_02.qn());
      /*p01.UFOR*/ UFOR_DIV_01.set(UKUP_DIV_00.qn(), UFOL_DIV_RST, UFOR_DIV_01.qn());
    }

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
    hash << SYS_PIN_RSTp;

    hash << CPU_PIN_READYp;
    hash << CPU_PIN_RDp;
    hash << CPU_PIN_WRp;
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
    hash << EXT_PIN_WRp_C;

    hash << CPU_PIN_SYS_RSTp.commit();
    hash << CPU_PIN_EXT_RST.commit();
    hash << CPU_PIN_EXT_CLKGOOD.commit();
    hash << CPU_PIN_POR_DONEp.commit();

    hash << TUBO_CPU_READYn.commit();
    hash << ASOL_POR_DONEn.commit();
    hash << AFER_SYS_RSTp.commit();

    hash << AFUR_ABCDxxxx.commit();
    hash << ALEF_xBCDExxx.commit();
    hash << APUK_xxCDEFxx.commit();
    hash << ADYK_xxxDEFGx.commit();

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

    /*
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
    */
  }
};

//------------------------------------------------------------------------------

void phase(Sandbox& sandbox) {
  SignalHash hash;
  int pass = 0;
  for (pass = 0; pass < 100; pass++) {
    sandbox.tick();
    SignalHash new_hash = sandbox.commit();
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
  sandbox.SYS_PIN_RSTp.set(0);
  tphase(sandbox);
}

void tphase_run(Sandbox& sandbox) {
  sandbox.SYS_PIN_RSTp.set(1);
  tphase(sandbox);
}

//------------------------------------------------------------------------------

int main() {

  Console con;
  con.init();

  while(1) {
    con.begin_frame();

    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        int sx = x + ((con.frame / 10) & 0x1F);
        int sy = y + ((con.frame / 10) & 0x1F);
        con.plot(sx, sy, 0b1100, 'R');
      }
    }

    con.print_at(con.mouse_x, con.mouse_y, 0b1111, "X");
    con.print_at(30, 30, 0b1110, "Hello World");

    con.end_frame();
  }
 
  return 0;


  /*
  Sandbox sandbox;

  SignalHash hash;
  sandbox.dump(hash, 0);

  tphase_rst(sandbox);

  sandbox.SYS_PIN_CLK_A.set(1);

  for (int i = 0; i < 20; i++) {
    tphase_run(sandbox);
    printf("x\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  */

  /*
  int x = 0;
  while(1) {
    printf("\033[2J");
    print_signal(0, 0, "SYS_PIN_CLK_A", VAL);
    print_signal(0, 1, "SYS_PIN_CLK_B", HIZ);
    print_signal(0, 2, "SYS_PIN_CLK_C", CLK);
    print_signal(0, 3, "SYS_PIN_CLK_D", SET);
    print_signal(0, 4, "SYS_PIN_CLK_E", RST);
    print_signal(0, 5, "SYS_PIN_CLK_F", ERROR);
    printf("char %d\n", x);
    printf("\n");
  }
  */

  /*
  while(1) {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    printf("\033[38;2;%d;%d;%dm", 0x88, 0x88, 0xFF);
    printf("top line?");
    printf("\033[m");
    //printf("\n");
    //printf("next line\n");
  }
  */
}

//------------------------------------------------------------------------------