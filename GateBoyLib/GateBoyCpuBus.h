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

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () { return and2(PIN_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () { return nor2(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A[15]); }

  /* p07.TUNA*/ wire TUNA_0000_FDFF    () { return nand7(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12], BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[9]); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF    () { return not1(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF    () { return not1(TUNA_0000_FDFF()); }

  /* p04.DECY*/ wire DECY_LATCH_EXTn   () { return not1(PIN_CPU_LATCH_EXT.qp_new()); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () { return not1(DECY_LATCH_EXTn()); }

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () { return not1(TEDO_CPU_RDp); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () { return not1(AJAS_CPU_RDn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () { return not1(MYNU_CPU_RDn()); }

  /* p03.TOVY*/ wire TOVY_A00n         () { return not1(BUS_CPU_A[0]); }
  /* p08.TOLA*/ wire TOLA_A01n         () { return not1(BUS_CPU_A[1]); }
  /* p22.XOLA*/ wire XOLA_A00n         () { return not1(BUS_CPU_A[0]); }
  /* p22.XENO*/ wire XENO_A01n         () { return not1(BUS_CPU_A[1]); }
  /* p22.XUSY*/ wire XUSY_A02n         () { return not1(BUS_CPU_A[2]); }
  /* p22.XERA*/ wire XERA_A03n         () { return not1(BUS_CPU_A[3]); }
  /* p07.TONA*/ wire TONA_A08n         () { return not1(BUS_CPU_A[8]); }
  /*#p08.SORE*/ wire SORE_A15n         () { return not1(BUS_CPU_A[15]); }
  /* p06.SEFY*/ wire SEFY_A02n         () { return not1(BUS_CPU_A[2]); }
  /* p10.BYKO*/ wire BYKO_A05n         () { return not1(BUS_CPU_A[5]); }
  /* p10.AKUG*/ wire AKUG_A06n         () { return not1(BUS_CPU_A[6]); }
  /* p08.SOGY*/ wire SOGY_A14n         () { return not1(BUS_CPU_A[14]); }

  /* p22.WADO*/ wire WADO_A00p         () { return not1(XOLA_A00n()); }
  /* p22.WESA*/ wire WESA_A01p         () { return not1(XENO_A01n()); }
  /* p22.WALO*/ wire WALO_A02p         () { return not1(XUSY_A02n()); }
  /* p22.WEPO*/ wire WEPO_A03p         () { return not1(XERA_A03n()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p () { return nor6(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 3], BUS_CPU_A[ 2], BUS_CPU_A[ 1], BUS_CPU_A[ 0]); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () { return and2(BUS_CPU_A[ 4], BUS_CPU_A[ 6]); }

  /* p07.SYKE*/ wire SYKE_ADDR_HIp     () { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /* p07.SOHA*/ wire SOHA_ADDR_HIn     () { return not1(SYKE_ADDR_HIp()); }
  /* p07.ROPE*/ wire ROPE_ADDR_OAMn    () { return nand2(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); }
  /* p07.SARO*/ wire SARO_ADDR_OAMp    () { return not1(ROPE_ADDR_OAMn()); }
  /* p22.XALY*/ wire XALY_0x00xxxx     () { return nor3(BUS_CPU_A[7], BUS_CPU_A[5], BUS_CPU_A[4]); }
  /* p22.WUTU*/ wire WUTU_ADDR_PPUn    () { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A[6], XALY_0x00xxxx()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp    () { return not1(WUTU_ADDR_PPUn()); }
  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () { return or3(BUS_CPU_A[13], BUS_CPU_A[14], SORE_A15n()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p   () { return nor5 (BUS_CPU_A[7], BUS_CPU_A[6], BUS_CPU_A[5], BUS_CPU_A[4], BUS_CPU_A[3]); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p   () { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2], SARE_XX00_XX07p()); }
  /* p10.AMUS*/ wire AMUS_XX_0xx00000p () { return nor6(BUS_CPU_A[0], BUS_CPU_A[1], BUS_CPU_A[2], BUS_CPU_A[3], BUS_CPU_A[4], BUS_CPU_A[7]); }
  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p () { return and4(BUS_CPU_A[0], BUS_CPU_A[1], BUS_CPU_A[2], BUS_CPU_A[3]); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp () { return nor4(BUS_CPU_A[7], BUS_CPU_A[6], BUS_CPU_A[5], BUS_CPU_A[4]); }
  /* p06.SANO*/ wire SANO_FF00_FF03p   () { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ wire ANAP_FF_0xx00000  () { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp   () { return and3(BUS_CPU_A[13], SOGY_A14n(), BUS_CPU_A[15]); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp   () { return and_or3(BUS_CPU_A[15], BUS_CPU_A[14], TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn   () { return not1(SARO_ADDR_OAMp()); }

  /* p22.WORU*/ wire WORU_FF40n        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WOFA*/ wire WOFA_FF41n        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WEBU*/ wire WEBU_FF42n        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WAVU*/ wire WAVU_FF43n        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WYLE*/ wire WYLE_FF44n        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETY*/ wire WETY_FF45n        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ wire WATE_FF46n        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WYBO*/ wire WYBO_FF47n        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETA*/ wire WETA_FF48n        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.VAMA*/ wire VAMA_FF49n        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WYVO*/ wire WYVO_FF4An        () { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WAGE*/ wire WAGE_FF4Bn        () { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /* p22.VOCA*/ wire VOCA_FF40p        () { return not1(WORU_FF40n()); }
  /* p22.VARY*/ wire VARY_FF41p        () { return not1(WOFA_FF41n()); }
  /* p22.XARO*/ wire XARO_FF42p        () { return not1(WEBU_FF42n()); }
  /* p22.XAVY*/ wire XAVY_FF43p        () { return not1(WAVU_FF43n()); }
  /* p22.XOGY*/ wire XOGY_FF44p        () { return not1(WYLE_FF44n()); }
  /* p22.XAYU*/ wire XAYU_FF45p        () { return not1(WETY_FF45n()); }
  /*#p22.XEDA*/ wire XEDA_FF46p        () { return not1(WATE_FF46n()); }
  /* p22.WERA*/ wire WERA_FF47p        () { return not1(WYBO_FF47n()); }
  /* p22.XAYO*/ wire XAYO_FF48p        () { return not1(WETA_FF48n()); }
  /* p22.TEGO*/ wire TEGO_FF49p        () { return not1(VAMA_FF49n()); }
  /* p22.VYGA*/ wire VYGA_FF4Ap        () { return not1(WYVO_FF4An()); }
  /* p22.VUMY*/ wire VUMY_FF4Bp        () { return not1(WAGE_FF4Bn()); }
};