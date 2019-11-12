#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

  // 4M clocks
  union { bool APUV; bool CLK_AxCxExGx1; }; // a_c_e_g_
  union { bool ARYF; bool CLK_AxCxExGx2; }; // a_c_e_g_
  union { bool ATAL; bool CLK_AxCxExGx3; }; // a_c_e_g_
  union { bool ALET; bool CLK_AxCxExGx4; }; // a_c_e_g_
  union { bool ARYS; bool CLK_AxCxExGx5; }; // a_c_e_g_
  union { bool ANOS; bool CLK_AxCxExGx6; }; // a_c_e_g_
  union { bool CYBO; bool CLK_AxCxExGx7; }; // a_c_e_g_
  union { bool ATAG; bool CLK_AxCxExGx8; }; // a_c_e_g_
  union { bool ZAXY; bool CLK_AxCxExGx9; }; // a_c_e_g_
  union { bool TAVA; bool CLK_AxCxExGx10;}; // a_c_e_g_

  union { bool AMUK; bool CLK_xBxDxFxH1; }; // _b_d_f_h
  union { bool ZEME; bool CLK_xBxDxFxH2; }; // _b_d_f_h
  union { bool AVET; bool CLK_xBxDxFxH3; }; // _b_d_f_h
  union { bool AZOF; bool CLK_xBxDxFxH4; }; // _b_d_f_h
  union { bool LAPE; bool CLK_xBxDxFxH5; }; // _b_d_f_h

  // 2M clocks
  union { bool CERY; bool CLK_ABxxEFxx1; }; // ab__ef__

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  union { bool AFUR; bool CLK_ABCDxxxx1; }; // abcd____
  union { bool ALEF; bool CLK_xBCDExxx1; }; // _bcde___
  union { bool APUK; bool CLK_xxCDEFxx1; }; // __cdef__
  union { bool ADYK; bool CLK_xxxDEFGx1; }; // ___defg_
  union { bool ATYP; bool CLK_xxxxEFGH1; }; // ____efgh
  union { bool AFEP; bool CLK_AxxxxFGH1; }; // a____fgh
  union { bool AROV; bool CLK_ABxxxxGH1; }; // ab____gh
  union { bool ADAR; bool CLK_ABCxxxxH1; }; // abc____h

  union { bool UVYT; bool CLK_xxxxEFGH3; bool PHI_OUTn; };      // ____efgh
  union { bool DOVA; bool CLK_xxxxEFGH2; bool PHIn; };          // ____efgh
  union { bool BEDO; bool CLK_xxxxxFGH2; }; // _____fgh -> PORTD_02
  union { bool BOWA; bool CLK_ABCDExxx2; }; // abcde___ -> PORTD_01
  union { bool BOGA; bool CLK_ABCDExxx1; }; // abcde___ -> PORTD_08
  union { bool BAVU; bool BAVU_1M; };
  union { bool BUKE; bool CLK_xxxxxFxx; };  // _____f__ -> PORTD_06
  union { bool BOMA; bool CLK_xxxxxFGH1; }; // _____fgh -> PORTD_07

  union { bool AJAX; bool CLK_ABCDxxxx3; };

  //----------
  // clocks for the cpu

  union { bool BAPY; bool CPUCLK_xxDExxxx1; };
  union { bool BUFA; bool CPUCLK_xxDExxxx2; };
  union { bool BYLY; bool CPUCLK_xxDExxxx3; };

  union { bool BERU; bool CPUCLK_ABxxEFGH1; };
  union { bool BOLO; bool CPUCLK_ABxxEFGH2; };
  union { bool BYDA; bool CPUCLK_ABxxEFGH3; };

  union { bool NULE; bool CPUCLK_ABCDxxxx1; };
  union { bool BUDE; bool CPUCLK_ABCDxxxx2; };  // -> PORTD_04
  union { bool BEVA; bool CPUCLK_ABCDxxxx3; bool PHI_OUT; };
  union { bool BEJA; bool CPUCLK_ABCDxxxx4; };
  union { bool BELO; bool CPUCLK_ABCDxxxx5; };

  union { bool BYRY; bool CPUCLK_xxxxEFGH1; };
  union { bool BEKO; bool CPUCLK_xxxxEFGH2; };  // ____efgh -> PORTD_03
  union { bool BAVY; bool CPUCLK_xxxxEFGH3; };
  union { bool BANE; bool CPUCLK_xxxxEFGH4; };
  union { bool BAZE; bool CPUCLK_xxxxEFGH5; };

  bool AREV; // abc_efgh
  bool BUTO; // abcdef__
  bool BYJU; // abcde___
  bool BUVU; // _____fgh
  bool AGUT; // abcd__gh
  bool AWOD; // abcd__gh

  bool AVOK;

  // Other clocks

  union { bool HAMA;    bool HAMA_512Kn; };
  union { bool JESO;    bool JESO_512K; };

  //----------
  // DIV

  union { bool UFOL; bool DIV_RSTn; };
  union { bool TAGY; bool DIV_RD; };
  union { bool TAPE; bool DIV_WR; };

  union { bool UKUP_00; bool DIV_00; };
  union { bool UFOR_01; bool DIV_01; bool CLK_256K; };
  union { bool UNER_02; bool DIV_02; };
  union { bool TERO_03; bool DIV_03; bool CLK_64K; };
  union { bool UNYK_04; bool DIV_04; };
  union { bool TAMA_05; bool DIV_05; bool TAMA_16Kn; };
  union { bool UGOT_06; bool DIV_06; };
  union { bool TULU_07; bool DIV_07; };
  union { bool TUGO_08; bool DIV_08; };
  union { bool TOFE_09; bool DIV_09; };
  union { bool TERU_10; bool DIV_10; };
  union { bool SOLA_11; bool DIV_11; };
  union { bool SUBU_12; bool DIV_12; };
  union { bool TEKA_13; bool DIV_13; };
  union { bool UKET_14; bool DIV_14; };
  union { bool UPOF_15; bool DIV_15; };

  union { bool UMEK; bool DIV_06n; };
  union { bool UREK; bool DIV_07n; };
  union { bool UTOK; bool DIV_08n; };
  union { bool SAPY; bool DIV_09n; };
  union { bool UMER; bool DIV_10n; };
  union { bool RAVE; bool DIV_11n; };
  union { bool RYSO; bool DIV_12n; };
  union { bool UDOR; bool DIV_13n; };

  union { bool TAWU; bool DIV_D0; };
  union { bool TAKU; bool DIV_D1; };
  union { bool TEMU; bool DIV_D2; };
  union { bool TUSE; bool DIV_D3; };
  union { bool UPUG; bool DIV_D4; };
  union { bool SEPU; bool DIV_D5; };
  union { bool SAWA; bool DIV_D6; };
  union { bool TATU; bool DIV_D7; };

  //----------

  union { bool UVYN;    bool CLK_16K; };

  // Slow clocks for APU sequencer
  union { bool HORU;    bool HORU_512; };
  union { bool BUFY;    bool BUFY_256; };
  union { bool BYFE;    bool BYFE_128; };

  // Gated rd/wr signals
  union { bool ABUZ; bool CPU_RD_SYNC; }; // ____ef__ -> CPU_RD_SYNC
  union { bool APOV; bool CPU_WR_SYNC; }; // ___d____ -> CPU_WR_SYNC

  union { bool UCOB; bool CLKIN_An; };

  // Resets
  union { bool ALUR; bool RESET2; };
  union { bool CUNU; bool RESET6; };
  union { bool XORE; bool RESET7; };
  union { bool XEBE; bool RESET7n; };
  union { bool WALU; bool RESET8; };
  union { bool WESY; bool RESET9; };
  union { bool XAPO; bool RESET_VIDEO; };
  union { bool DULA; bool RESETn; };

  union { bool AFAS; bool CLK_xxxDxxxx1; }; // ___d____

  union { bool COKE; }; // ? clock

  union { bool ABOL; bool CPUCLK_REQn; };
  union { bool BUTY; bool CPUCLK_REQ;  };

private:

  //----------
  // registers


  // dividers for apu
  bool BARA,CARU,BYLU;
  bool ATYK;

  // reset reg
  bool AFER; 

  //----------
  // cells

  union { bool BUGO; bool CLK_xBCDExxx2; };

  bool ALYP; // x
  bool AFAR; // x
  bool ASOL; // x

  bool ULUR;
  bool BATE,BASU;
  bool BELE,ATEZ,BALY,AVOR;
  bool BYXO;
  bool XODO;
  bool BELA;
  bool UPYF,TUBO,UNUT,TABA;
  bool BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU;
  
  union { bool BOPO; bool APU_RESETn1; };
  union { bool ATUS; bool APU_RESETn2; };
};
