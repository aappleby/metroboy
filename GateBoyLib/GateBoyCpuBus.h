#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyCpuBus {
  Signal BUS_CPU_A[16];
  Signal BUS_CPU_D[8];
  BusOut BUS_CPU_D_out[8];

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  /* p07.TEDO*/ Signal TEDO_CPU_RDp;
  /* p01.APOV*/ Signal APOV_CPU_WRp;
  /* p07.TAPU*/ Signal TAPU_CPU_WRp;

  /* p07.TUNA*/ wire TUNA_0000_FDFF_ext    () { return nand7(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12], BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[9]); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF_ext    () { return not1(TUNA_0000_FDFF_ext()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF_ext    () { return not1(TUNA_0000_FDFF_ext()); }

  /* p03.TOVY*/ wire TOVY_A00n_ext         () { return not1(BUS_CPU_A[0]); }
  /* p08.TOLA*/ wire TOLA_A01n_ext         () { return not1(BUS_CPU_A[1]); }
  /* p22.XOLA*/ wire XOLA_A00n_ext         () { return not1(BUS_CPU_A[0]); }
  /* p22.XENO*/ wire XENO_A01n_ext         () { return not1(BUS_CPU_A[1]); }
  /* p22.XUSY*/ wire XUSY_A02n_ext         () { return not1(BUS_CPU_A[2]); }
  /* p22.XERA*/ wire XERA_A03n_ext         () { return not1(BUS_CPU_A[3]); }
  /* p07.TONA*/ wire TONA_A08n_ext         () { return not1(BUS_CPU_A[8]); }
  /*#p08.SORE*/ wire SORE_A15n_ext         () { return not1(BUS_CPU_A[15]); }
  /* p06.SEFY*/ wire SEFY_A02n_ext         () { return not1(BUS_CPU_A[2]); }
  /* p10.BYKO*/ wire BYKO_A05n_ext         () { return not1(BUS_CPU_A[5]); }
  /* p10.AKUG*/ wire AKUG_A06n_ext         () { return not1(BUS_CPU_A[6]); }
  /* p08.SOGY*/ wire SOGY_A14n_ext         () { return not1(BUS_CPU_A[14]); }

  /* p22.WADO*/ wire WADO_A00p_ext         () { return not1(XOLA_A00n_ext()); }
  /* p22.WESA*/ wire WESA_A01p_ext         () { return not1(XENO_A01n_ext()); }
  /* p22.WALO*/ wire WALO_A02p_ext         () { return not1(XUSY_A02n_ext()); }
  /* p22.WEPO*/ wire WEPO_A03p_ext         () { return not1(XERA_A03n_ext()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p     () { return nor6(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 3], BUS_CPU_A[ 2], BUS_CPU_A[ 1], BUS_CPU_A[ 0]); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp     () { return and2(BUS_CPU_A[ 4], BUS_CPU_A[ 6]); }

  /* p07.SYKE*/ wire SYKE_ADDR_HIp_ext     () { return nor2(TUNA_0000_FDFF_ext(), TONA_A08n_ext()); }
  /* p07.SOHA*/ wire SOHA_ADDR_HIn_ext     () { return not1(SYKE_ADDR_HIp_ext()); }
  /* p07.ROPE*/ wire ROPE_ADDR_OAMn_ext    () { return nand2(SOHA_ADDR_HIn_ext(), RYCU_FE00_FFFF_ext()); }
  /* p07.SARO*/ wire SARO_ADDR_OAMp_ext    () { return not1(ROPE_ADDR_OAMn_ext()); }
  /* p22.XALY*/ wire XALY_0x00xxxx_ext     () { return nor3(BUS_CPU_A[7], BUS_CPU_A[5], BUS_CPU_A[4]); }
  /* p22.WUTU*/ wire WUTU_ADDR_PPUn_ext    () { return nand3(SYKE_ADDR_HIp_ext(), BUS_CPU_A[6], XALY_0x00xxxx_ext()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp_ext    () { return not1(WUTU_ADDR_PPUn_ext()); }
  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn_ext   () { return or3(BUS_CPU_A[13], BUS_CPU_A[14], SORE_A15n_ext()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p_ext   () { return nor5 (BUS_CPU_A[7], BUS_CPU_A[6], BUS_CPU_A[5], BUS_CPU_A[4], BUS_CPU_A[3]); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p_ext   () { return and3(SYKE_ADDR_HIp_ext(), BUS_CPU_A[2], SARE_XX00_XX07p_ext()); }
  /* p10.AMUS*/ wire AMUS_XX_0xx00000p_ext () { return nor6(BUS_CPU_A[0], BUS_CPU_A[1], BUS_CPU_A[2], BUS_CPU_A[3], BUS_CPU_A[4], BUS_CPU_A[7]); }
  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p_ext () { return and4(BUS_CPU_A[0], BUS_CPU_A[1], BUS_CPU_A[2], BUS_CPU_A[3]); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp_ext () { return nor4(BUS_CPU_A[7], BUS_CPU_A[6], BUS_CPU_A[5], BUS_CPU_A[4]); }
  /* p06.SANO*/ wire SANO_FF00_FF03p_ext   () { return and3(SARE_XX00_XX07p_ext(), SEFY_A02n_ext(), SYKE_ADDR_HIp_ext()); }
  /* p10.ANAP*/ wire ANAP_FF_0xx00000_ext  () { return and2(SYKE_ADDR_HIp_ext(), AMUS_XX_0xx00000p_ext()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp_ext   () { return and3(BUS_CPU_A[13], SOGY_A14n_ext(), BUS_CPU_A[15]); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp_ext   () { return and_or3(BUS_CPU_A[15], BUS_CPU_A[14], TUMA_A000_BFFFp_ext()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn_ext   () { return not1(SARO_ADDR_OAMp_ext()); }

  /* p22.WORU*/ wire WORU_FF40n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), XENO_A01n_ext(), XUSY_A02n_ext(), XERA_A03n_ext()); }
  /* p22.WOFA*/ wire WOFA_FF41n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), XENO_A01n_ext(), XUSY_A02n_ext(), XERA_A03n_ext()); }
  /* p22.WEBU*/ wire WEBU_FF42n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), WESA_A01p_ext(), XUSY_A02n_ext(), XERA_A03n_ext()); }
  /* p22.WAVU*/ wire WAVU_FF43n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), WESA_A01p_ext(), XUSY_A02n_ext(), XERA_A03n_ext()); }
  /* p22.WYLE*/ wire WYLE_FF44n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), XENO_A01n_ext(), WALO_A02p_ext(), XERA_A03n_ext()); }
  /* p22.WETY*/ wire WETY_FF45n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), XENO_A01n_ext(), WALO_A02p_ext(), XERA_A03n_ext()); }
  /*#p22.WATE*/ wire WATE_FF46n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), WESA_A01p_ext(), WALO_A02p_ext(), XERA_A03n_ext()); }
  /* p22.WYBO*/ wire WYBO_FF47n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), WESA_A01p_ext(), WALO_A02p_ext(), XERA_A03n_ext()); }
  /* p22.WETA*/ wire WETA_FF48n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), XENO_A01n_ext(), XUSY_A02n_ext(), WEPO_A03p_ext()); }
  /* p22.VAMA*/ wire VAMA_FF49n_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), XENO_A01n_ext(), XUSY_A02n_ext(), WEPO_A03p_ext()); }
  /* p22.WYVO*/ wire WYVO_FF4An_ext        () { return nand5(WERO_ADDR_PPUp_ext(), XOLA_A00n_ext(), WESA_A01p_ext(), XUSY_A02n_ext(), WEPO_A03p_ext()); }
  /* p22.WAGE*/ wire WAGE_FF4Bn_ext        () { return nand5(WERO_ADDR_PPUp_ext(), WADO_A00p_ext(), WESA_A01p_ext(), XUSY_A02n_ext(), WEPO_A03p_ext()); }

  /* p22.VOCA*/ wire VOCA_FF40p_ext        () { return not1(WORU_FF40n_ext()); }
  /* p22.VARY*/ wire VARY_FF41p_ext        () { return not1(WOFA_FF41n_ext()); }
  /* p22.XARO*/ wire XARO_FF42p_ext        () { return not1(WEBU_FF42n_ext()); }
  /* p22.XAVY*/ wire XAVY_FF43p_ext        () { return not1(WAVU_FF43n_ext()); }
  /* p22.XOGY*/ wire XOGY_FF44p_ext        () { return not1(WYLE_FF44n_ext()); }
  /* p22.XAYU*/ wire XAYU_FF45p_ext        () { return not1(WETY_FF45n_ext()); }
  /*#p22.XEDA*/ wire XEDA_FF46p_ext        () { return not1(WATE_FF46n_ext()); }
  /* p22.WERA*/ wire WERA_FF47p_ext        () { return not1(WYBO_FF47n_ext()); }
  /* p22.XAYO*/ wire XAYO_FF48p_ext        () { return not1(WETA_FF48n_ext()); }
  /* p22.TEGO*/ wire TEGO_FF49p_ext        () { return not1(VAMA_FF49n_ext()); }
  /* p22.VYGA*/ wire VYGA_FF4Ap_ext        () { return not1(WYVO_FF4An_ext()); }
  /* p22.VUMY*/ wire VUMY_FF4Bp_ext        () { return not1(WAGE_FF4Bn_ext()); }
};