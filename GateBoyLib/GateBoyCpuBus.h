#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyCpuBus {
  void reset_to_cart();

  void set_addr(int phase_total, Req bus_req_new);
  void set_data(int phase_total, Req bus_req_new);
  void set_pins(
    const GateBoyResetDebug& rst,
    const GateBoyClock& clk,
    int phase_total,
    Req bus_req_new);

  void dump(Dumper& d) {
    d.dump_slice2p("BUS_CPU_A        ", BUS_CPU_A, 16);
    d.dump_slice2p("BUS_CPU_D        ", BUS_CPU_D, 8);
    d.dump_slice2p("BUS_CPU_D_out    ", BUS_CPU_D, 8);
    d.dump_bitp   ("PIN_CPU_RDp      ", PIN_CPU_RDp.state);
    d.dump_bitp   ("PIN_CPU_WRp      ", PIN_CPU_WRp.state);
    //d.dump_bitp   ("PIN_CPU_UNOR_DBG ", PIN_CPU_UNOR_DBG.state);
    d.dump_bitp   ("PIN_CPU_ADDR_HIp ", PIN_CPU_ADDR_HIp.state);
    //d.dump_bitp   ("PIN_CPU_UMUT_DBG ", PIN_CPU_UMUT_DBG.state);
    d.dump_bitp   ("PIN_CPU_EXT_BUSp ", PIN_CPU_EXT_BUSp.state);
    //d.dump_bitp   ("PIN_CPU_6        ", PIN_CPU_6.state);
    d.dump_bitp   ("PIN_CPU_LATCH_EXT", PIN_CPU_LATCH_EXT.state);
    //d.dump_bitp   ("BOOT_BITn        ", BOOT_BITn_h.state);
    //d.dump_bitp   ("PIN_CPU_BOOTp    ", PIN_CPU_BOOTp.state);
    //d.dump_bitp   ("TEDO_CPU_RDp     ", TEDO_CPU_RDp.state);
    //d.dump_bitp   ("APOV_CPU_WRp     ", APOV_CPU_WRp.state);
    //d.dump_bitp   ("TAPU_CPU_WRp     ", TAPU_CPU_WRp.state);
  }

  Signal BUS_CPU_A[16];
  Bus BUS_CPU_D[8];

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

  /* p07.TEDO*/ Signal TEDO_CPU_RDp;
  /* p01.APOV*/ Signal APOV_CPU_WRp;
  /* p07.TAPU*/ Signal TAPU_CPU_WRp;

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(TAPU_CPU_WRp.qp_new()); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(APOV_CPU_WRp.qp_new()); }


  /* p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(PIN_CPU_LATCH_EXT.qp_new()); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /* p07.TUNA*/ wire TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A[15].qp(), BUS_CPU_A[14].qp(), BUS_CPU_A[13].qp(), BUS_CPU_A[12].qp(), BUS_CPU_A[11].qp(), BUS_CPU_A[10].qp(), BUS_CPU_A[9].qp()); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ wire TOVY_A00n         () const { return not1(BUS_CPU_A[0].qp()); }
  /* p08.TOLA*/ wire TOLA_A01n         () const { return not1(BUS_CPU_A[1].qp()); }
  /* p22.XOLA*/ wire XOLA_A00n         () const { return not1(BUS_CPU_A[0].qp()); }
  /* p22.XENO*/ wire XENO_A01n         () const { return not1(BUS_CPU_A[1].qp()); }
  /* p22.XUSY*/ wire XUSY_A02n         () const { return not1(BUS_CPU_A[2].qp()); }
  /* p22.XERA*/ wire XERA_A03n         () const { return not1(BUS_CPU_A[3].qp()); }
  /* p07.TONA*/ wire TONA_A08n         () const { return not1(BUS_CPU_A[8].qp()); }
  /*#p08.SORE*/ wire SORE_A15n         () const { return not1(BUS_CPU_A[15].qp()); }
  /* p06.SEFY*/ wire SEFY_A02n         () const { return not1(BUS_CPU_A[2].qp()); }
  /* p10.BYKO*/ wire BYKO_A05n         () const { return not1(BUS_CPU_A[5].qp()); }
  /* p10.AKUG*/ wire AKUG_A06n         () const { return not1(BUS_CPU_A[6].qp()); }
  /* p08.SOGY*/ wire SOGY_A14n         () const { return not1(BUS_CPU_A[14].qp()); }

  /* p22.WADO*/ wire WADO_A00p         () const { return not1(XOLA_A00n()); }
  /* p22.WESA*/ wire WESA_A01p         () const { return not1(XENO_A01n()); }
  /* p22.WALO*/ wire WALO_A02p         () const { return not1(XUSY_A02n()); }
  /* p22.WEPO*/ wire WEPO_A03p         () const { return not1(XERA_A03n()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A[ 7].qp(), BUS_CPU_A[ 5].qp(), BUS_CPU_A[ 3].qp(), BUS_CPU_A[ 2].qp(), BUS_CPU_A[ 1].qp(), BUS_CPU_A[ 0].qp()); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A[ 4].qp(), BUS_CPU_A[ 6].qp()); }

  /*#p07.SYKE*/ wire SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ wire XALY_0x00xxxx     () const { return nor3(BUS_CPU_A[7].qp(), BUS_CPU_A[5].qp(), BUS_CPU_A[4].qp()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A[7].qp(), BUS_CPU_A[6].qp(), BUS_CPU_A[5].qp(), BUS_CPU_A[4].qp(), BUS_CPU_A[3].qp()); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp(), SARE_XX00_XX07p()); }
  /* p10.AMUS*/ wire AMUS_XX_0xx00000p () const { return nor6(BUS_CPU_A[0].qp(), BUS_CPU_A[1].qp(), BUS_CPU_A[2].qp(), BUS_CPU_A[3].qp(), BUS_CPU_A[4].qp(), BUS_CPU_A[7].qp()); }
  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A[0].qp(), BUS_CPU_A[1].qp(), BUS_CPU_A[2].qp(), BUS_CPU_A[3].qp()); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A[7].qp(), BUS_CPU_A[6].qp(), BUS_CPU_A[5].qp(), BUS_CPU_A[4].qp()); }
  /* p06.SANO*/ wire SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ wire ANAP_FF_0xx00000  () const { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A[13].qp(), SOGY_A14n(), BUS_CPU_A[15].qp()); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A[15].qp(), BUS_CPU_A[14].qp(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ wire TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A[15].qp(), BUS_CPU_A[14].qp(), BUS_CPU_A[13].qp(), BUS_CPU_A[12].qp(),
                                                              BUS_CPU_A[11].qp(), BUS_CPU_A[10].qp(), BUS_CPU_A[ 9].qp(), BUS_CPU_A[ 8].qp()); }
  /* p07.TERA*/ wire TERA_BOOT_BITp    () const { return not1(BOOT_BITn_h.qp_new()); }
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), TULO_ADDR_BOOTROMp()); }

  /* p22.WUTU*/ wire WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A[6].qp(), XALY_0x00xxxx()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A[13].qp(), BUS_CPU_A[14].qp(), SORE_A15n()); }
  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(PIN_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A[15].qp()); }
  /* p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }

  /* p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), APOV_CPU_WRp.qp()); }

  /*#p07.ROPE*/ wire ROPE_ADDR_OAMn    () const { return nand2(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
  /*#p07.SARO*/ wire SARO_ADDR_OAMp    () const { return not1(ROPE_ADDR_OAMn()); }

  /* p22.WORU*/ wire WORU_FF40n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WOFA*/ wire WOFA_FF41n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WEBU*/ wire WEBU_FF42n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WAVU*/ wire WAVU_FF43n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WYLE*/ wire WYLE_FF44n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETY*/ wire WETY_FF45n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ wire WATE_FF46n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WYBO*/ wire WYBO_FF47n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETA*/ wire WETA_FF48n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.VAMA*/ wire VAMA_FF49n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WYVO*/ wire WYVO_FF4An        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WAGE*/ wire WAGE_FF4Bn        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /* p22.VOCA*/ wire VOCA_FF40p        () const { return not1(WORU_FF40n()); }
  /* p22.VARY*/ wire VARY_FF41p        () const { return not1(WOFA_FF41n()); }
  /* p22.XARO*/ wire XARO_FF42p        () const { return not1(WEBU_FF42n()); }
  /* p22.XAVY*/ wire XAVY_FF43p        () const { return not1(WAVU_FF43n()); }
  /* p22.XOGY*/ wire XOGY_FF44p        () const { return not1(WYLE_FF44n()); }
  /* p22.XAYU*/ wire XAYU_FF45p        () const { return not1(WETY_FF45n()); }
  /*#p22.XEDA*/ wire XEDA_FF46p        () const { return not1(WATE_FF46n()); }
  /* p22.WERA*/ wire WERA_FF47p        () const { return not1(WYBO_FF47n()); }
  /* p22.XAYO*/ wire XAYO_FF48p        () const { return not1(WETA_FF48n()); }
  /* p22.TEGO*/ wire TEGO_FF49p        () const { return not1(VAMA_FF49n()); }
  /* p22.VYGA*/ wire VYGA_FF4Ap        () const { return not1(WYVO_FF4An()); }
  /* p22.VUMY*/ wire VUMY_FF4Bp        () const { return not1(WAGE_FF4Bn()); }
};