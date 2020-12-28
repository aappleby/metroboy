#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyCpuBus {
  void reset_to_bootrom();
  void reset_to_cart();

  void set_addr(int phase_total, Req bus_req_new);
  void set_data(int phase_total, Req bus_req_new);
  void set_pins(
    const GateBoyResetDebug& rst,
    const GateBoyClock& clk,
    int phase_total,
    Req bus_req_new);

  void dump(Dumper& d) {
    d.dump_bitp   ("SIG_CPU_RDp       : ", SIG_CPU_RDp.state);
    d.dump_bitp   ("SIG_CPU_WRp       : ", SIG_CPU_WRp.state);
    d.dump_bitp   ("SIG_CPU_UNOR_DBG  : ", SIG_CPU_UNOR_DBG.state);
    d.dump_bitp   ("SIG_CPU_ADDR_HIp  : ", SIG_CPU_ADDR_HIp.state);
    d.dump_bitp   ("SIG_CPU_UMUT_DBG  : ", SIG_CPU_UMUT_DBG.state);
    d.dump_bitp   ("SIG_CPU_EXT_BUSp  : ", SIG_CPU_EXT_BUSp.state);
    d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
    d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", SIG_CPU_LATCH_EXT.state);
    d.dump_bitp   ("BOOT_BITn         : ", TEPU_BOOT_BITn_h.state);
    d.dump_bitp   ("SIG_CPU_BOOTp     : ", SIG_CPU_BOOTp.state);
    d.dump_bitp   ("TEDO_CPU_RDp      : ", TEDO_CPU_RDp.state);
    d.dump_bitp   ("APOV_CPU_WRp      : ", APOV_CPU_WRp.state);
    d.dump_bitp   ("TAPU_CPU_WRp      : ", TAPU_CPU_WRp.state);
    d.dump_slice2p("BUS_CPU_A : ", BUS_CPU_A, 16);
    d.dump_slice2p("BUS_CPU_D : ", BUS_CPU_D, 8);
  }

  Signal BUS_CPU_A[16];
  Bus BUS_CPU_D[8];

  Signal SIG_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Signal SIG_CPU_WRp;           // top right port PORTA_01: ->
  Signal SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Signal SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  Signal SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  Signal SIG_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  Signal SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  Signal SIG_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  /*p07.TEPU*/ DFF17 TEPU_BOOT_BITn_h;
  Signal SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

  /* p07.TEDO*/ Signal TEDO_CPU_RDp;
  /* p01.APOV*/ Signal APOV_CPU_WRp;
  /* p07.TAPU*/ Signal TAPU_CPU_WRp;

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1b(TEDO_CPU_RDp.qp_new2()); }
  /* p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1b(TAPU_CPU_WRp.qp_new2()); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1b(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1b(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1b(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2b(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire REDU_CPU_RDn      () const { return not1b(TEDO_CPU_RDp.qp_new2()); }
  /* p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1b(APOV_CPU_WRp.qp_new2()); }


  /* p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1b(SIG_CPU_LATCH_EXT.qp_new2()); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1b(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1b(CATY_LATCH_EXTp()); }

  /* p07.TUNA*/ wire TUNA_0000_FDFF    () const { return nand7b(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), BUS_CPU_A[13].qp_new2(), BUS_CPU_A[12].qp_new2(), BUS_CPU_A[11].qp_new2(), BUS_CPU_A[10].qp_new2(), BUS_CPU_A[9].qp_new2()); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF    () const { return not1b(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF    () const { return not1b(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ wire TOVY_A00n         () const { return not1b(BUS_CPU_A[0].qp_new2()); }
  /* p08.TOLA*/ wire TOLA_A01n         () const { return not1b(BUS_CPU_A[1].qp_new2()); }
  /* p22.XOLA*/ wire XOLA_A00n         () const { return not1b(BUS_CPU_A[0].qp_new2()); }
  /* p22.XENO*/ wire XENO_A01n         () const { return not1b(BUS_CPU_A[1].qp_new2()); }
  /* p22.XUSY*/ wire XUSY_A02n         () const { return not1b(BUS_CPU_A[2].qp_new2()); }
  /* p22.XERA*/ wire XERA_A03n         () const { return not1b(BUS_CPU_A[3].qp_new2()); }
  /* p07.TONA*/ wire TONA_A08n         () const { return not1b(BUS_CPU_A[8].qp_new2()); }
  /*#p08.SORE*/ wire SORE_A15n         () const { return not1b(BUS_CPU_A[15].qp_new2()); }
  /* p06.SEFY*/ wire SEFY_A02n         () const { return not1b(BUS_CPU_A[2].qp_new2()); }
  /* p10.BYKO*/ wire BYKO_A05n         () const { return not1b(BUS_CPU_A[5].qp_new2()); }
  /* p10.AKUG*/ wire AKUG_A06n         () const { return not1b(BUS_CPU_A[6].qp_new2()); }
  /* p08.SOGY*/ wire SOGY_A14n         () const { return not1b(BUS_CPU_A[14].qp_new2()); }

  /* p22.WADO*/ wire WADO_A00p         () const { return not1b(XOLA_A00n()); }
  /* p22.WESA*/ wire WESA_A01p         () const { return not1b(XENO_A01n()); }
  /* p22.WALO*/ wire WALO_A02p         () const { return not1b(XUSY_A02n()); }
  /* p22.WEPO*/ wire WEPO_A03p         () const { return not1b(XERA_A03n()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p () const { return nor6b(BUS_CPU_A[ 7].qp_new2(), BUS_CPU_A[ 5].qp_new2(), BUS_CPU_A[ 3].qp_new2(), BUS_CPU_A[ 2].qp_new2(), BUS_CPU_A[ 1].qp_new2(), BUS_CPU_A[ 0].qp_new2()); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A[ 4].qp_new2(), BUS_CPU_A[ 6].qp_new2()); }

  /*#p07.SYKE*/ wire SYKE_ADDR_HIp     () const { return nor2b(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire SOHA_ADDR_HIn     () const { return not1b(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ wire XALY_0x00xxxx     () const { return nor3b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p   () const { return nor5b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[6].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2(), BUS_CPU_A[3].qp_new2()); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new2(), SARE_XX00_XX07p()); }

  /* p03.RYFO*/ wire RYFO_FF04_FF07p_b () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new2(), SARE_XX00_XX07p()); }

  /* p10.AMUS*/ wire AMUS_XX_0xx00000p () const { return nor6b(BUS_CPU_A[0].qp_new2(), BUS_CPU_A[1].qp_new2(), BUS_CPU_A[2].qp_new2(), BUS_CPU_A[3].qp_new2(), BUS_CPU_A[4].qp_new2(), BUS_CPU_A[7].qp_new2()); }
  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A[0].qp_new2(), BUS_CPU_A[1].qp_new2(), BUS_CPU_A[2].qp_new2(), BUS_CPU_A[3].qp_new2()); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp () const { return nor4b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[6].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2()); }
  /* p06.SANO*/ wire SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ wire ANAP_FF_0xx00000  () const { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A[13].qp_new2(), SOGY_A14n(), BUS_CPU_A[15].qp_new2()); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn   () const { return not1b(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ wire TULO_ADDR_BOOTROMp() const { return nor8b(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), BUS_CPU_A[13].qp_new2(), BUS_CPU_A[12].qp_new2(), BUS_CPU_A[11].qp_new2(), BUS_CPU_A[10].qp_new2(), BUS_CPU_A[ 9].qp_new2(), BUS_CPU_A[ 8].qp_new2()); }
  /* p07.TERA*/ wire TERA_BOOT_BITp    () const { return not1b(TEPU_BOOT_BITn_h.qp_new2()); }
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), TULO_ADDR_BOOTROMp()); }

  /* p22.WUTU*/ wire WUTU_ADDR_PPUn    () const { return nand3b(SYKE_ADDR_HIp(), BUS_CPU_A[6].qp_new2(), XALY_0x00xxxx()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp    () const { return not1b(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A[13].qp_new2(), BUS_CPU_A[14].qp_new2(), SORE_A15n()); }
  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(SIG_CPU_EXT_BUSp.qp_new2(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2b(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A[15].qp_new2()); }
  /* p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1b(TEXO_ADDR_VRAMn()); }

  /* p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), APOV_CPU_WRp.qp_new2()); }

  /*#p07.ROPE*/ wire ROPE_ADDR_OAMn    () const { return nand2b(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
  /*#p07.SARO*/ wire SARO_ADDR_OAMp    () const { return not1b(ROPE_ADDR_OAMn()); }

  /* p22.WORU*/ wire WORU_FF40n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WOFA*/ wire WOFA_FF41n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WEBU*/ wire WEBU_FF42n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WAVU*/ wire WAVU_FF43n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WYLE*/ wire WYLE_FF44n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETY*/ wire WETY_FF45n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ wire WATE_FF46n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WYBO*/ wire WYBO_FF47n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETA*/ wire WETA_FF48n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.VAMA*/ wire VAMA_FF49n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WYVO*/ wire WYVO_FF4An        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WAGE*/ wire WAGE_FF4Bn        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /* p22.VOCA*/ wire VOCA_FF40p        () const { return not1b(WORU_FF40n()); }
  /* p22.VARY*/ wire VARY_FF41p        () const { return not1b(WOFA_FF41n()); }
  /* p22.XARO*/ wire XARO_FF42p        () const { return not1b(WEBU_FF42n()); }
  /* p22.XAVY*/ wire XAVY_FF43p        () const { return not1b(WAVU_FF43n()); }
  /* p22.XOGY*/ wire XOGY_FF44p        () const { return not1b(WYLE_FF44n()); }
  /* p22.XAYU*/ wire XAYU_FF45p        () const { return not1b(WETY_FF45n()); }
  /*#p22.XEDA*/ wire XEDA_FF46p        () const { return not1b(WATE_FF46n()); }
  /* p22.WERA*/ wire WERA_FF47p        () const { return not1b(WYBO_FF47n()); }
  /* p22.XAYO*/ wire XAYO_FF48p        () const { return not1b(WETA_FF48n()); }
  /* p22.TEGO*/ wire TEGO_FF49p        () const { return not1b(VAMA_FF49n()); }
  /* p22.VYGA*/ wire VYGA_FF4Ap        () const { return not1b(WYVO_FF4An()); }
  /* p22.VUMY*/ wire VUMY_FF4Bp        () const { return not1b(WAGE_FF4Bn()); }

  wire TOLE_CPU_VRAM_RDp(wire ABUZ_EXT_RAM_CS_CLK)
  {
   // Ignoring debug for now
    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new2());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new2());
    ///*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);
    return _TUCA_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn()
  {
    // Ignoring debug for now
    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2b(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new2());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new2());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new2());
    ///*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
    return _TEGU_CPU_VRAM_WRn;
  }

};