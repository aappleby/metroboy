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
    d.dump_bitp   ("BOOT_BITn         : ", BOOT_BITn_h.state);
    d.dump_bitp   ("SIG_CPU_BOOTp     : ", SIG_CPU_BOOTp.state);
    d.dump_bitp   ("TEDO_CPU_RDp      : ", TEDO_CPU_RDp.state);
    d.dump_bitp   ("APOV_CPU_WRp      : ", APOV_CPU_WRp.state);
    d.dump_bitp   ("TAPU_CPU_WRp      : ", TAPU_CPU_WRp.state);
    d.dump_slice2p("BUS_CPU_A : ", BUS_CPU_A, 16);
    d.dump_slice2p("BUS_CPU_D : ", BUS_CPU_D, 8);
  }

  Signal _XYMU_RENDERINGp;

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

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
  Signal SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

  /* p07.TEDO*/ Signal TEDO_CPU_RDp;
  /* p01.APOV*/ Signal APOV_CPU_WRp;
  /* p07.TAPU*/ Signal TAPU_CPU_WRp;

  /* p07.AJAS*/ wire2 AJAS_CPU_RDn      () const { return not1b(TEDO_CPU_RDp.qp_new2()); }
  /* p07.DYKY*/ wire2 DYKY_CPU_WRn      () const { return not1b(TAPU_CPU_WRp.qp_new2()); }
  /* p07.ASOT*/ wire2 ASOT_CPU_RDp      () const { return not1b(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire2 LEKO_CPU_RDp      () const { return not1b(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire2 CUPA_CPU_WRp      () const { return not1b(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire2 MYNU_CPU_RDn      () const { return nand2b(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire2 REDU_CPU_RDn      () const { return not1b(TEDO_CPU_RDp.qp_new2()); }
  /* p08.MEXO*/ wire2 MEXO_CPU_WRn      () const { return not1b(APOV_CPU_WRp.qp_new2()); }


  /* p04.DECY*/ wire2 DECY_LATCH_EXTn   () const { return not1b(SIG_CPU_LATCH_EXT.qp_new2()); }
  /* p04.CATY*/ wire2 CATY_LATCH_EXTp   () const { return not1b(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire2 BOFE_LATCH_EXTn   () const { return not1b(CATY_LATCH_EXTp()); }

  /* p07.TUNA*/ wire2 TUNA_0000_FDFF    () const { return nand7b(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), BUS_CPU_A[13].qp_new2(), BUS_CPU_A[12].qp_new2(), BUS_CPU_A[11].qp_new2(), BUS_CPU_A[10].qp_new2(), BUS_CPU_A[9].qp_new2()); }
  /* p07.RYCU*/ wire2 RYCU_FE00_FFFF    () const { return not1b(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire2 SYRO_FE00_FFFF    () const { return not1b(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ wire2 TOVY_A00n         () const { return not1b(BUS_CPU_A[0].qp_new2()); }
  /* p08.TOLA*/ wire2 TOLA_A01n         () const { return not1b(BUS_CPU_A[1].qp_new2()); }
  /* p22.XOLA*/ wire2 XOLA_A00n         () const { return not1b(BUS_CPU_A[0].qp_new2()); }
  /* p22.XENO*/ wire2 XENO_A01n         () const { return not1b(BUS_CPU_A[1].qp_new2()); }
  /* p22.XUSY*/ wire2 XUSY_A02n         () const { return not1b(BUS_CPU_A[2].qp_new2()); }
  /* p22.XERA*/ wire2 XERA_A03n         () const { return not1b(BUS_CPU_A[3].qp_new2()); }
  /* p07.TONA*/ wire2 TONA_A08n         () const { return not1b(BUS_CPU_A[8].qp_new2()); }
  /*#p08.SORE*/ wire2 SORE_A15n         () const { return not1b(BUS_CPU_A[15].qp_new2()); }
  /* p06.SEFY*/ wire2 SEFY_A02n         () const { return not1b(BUS_CPU_A[2].qp_new2()); }
  /* p10.BYKO*/ wire2 BYKO_A05n         () const { return not1b(BUS_CPU_A[5].qp_new2()); }
  /* p10.AKUG*/ wire2 AKUG_A06n         () const { return not1b(BUS_CPU_A[6].qp_new2()); }
  /* p08.SOGY*/ wire2 SOGY_A14n         () const { return not1b(BUS_CPU_A[14].qp_new2()); }

  /* p22.WADO*/ wire2 WADO_A00p         () const { return not1b(XOLA_A00n()); }
  /* p22.WESA*/ wire2 WESA_A01p         () const { return not1b(XENO_A01n()); }
  /* p22.WALO*/ wire2 WALO_A02p         () const { return not1b(XUSY_A02n()); }
  /* p22.WEPO*/ wire2 WEPO_A03p         () const { return not1b(XERA_A03n()); }

  /* p07.TYRO*/ wire2 TYRO_XX_0x0x0000p () const { return nor6b(BUS_CPU_A[ 7].qp_new2(), BUS_CPU_A[ 5].qp_new2(), BUS_CPU_A[ 3].qp_new2(), BUS_CPU_A[ 2].qp_new2(), BUS_CPU_A[ 1].qp_new2(), BUS_CPU_A[ 0].qp_new2()); }
  /* p07.TUFA*/ wire2 TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A[ 4].qp_new2(), BUS_CPU_A[ 6].qp_new2()); }

  /*#p07.SYKE*/ wire2 SYKE_ADDR_HIp     () const { return nor2b(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire2 SOHA_ADDR_HIn     () const { return not1b(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ wire2 XALY_0x00xxxx     () const { return nor3b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2()); }
  /* p06.SARE*/ wire2 SARE_XX00_XX07p   () const { return nor5b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[6].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2(), BUS_CPU_A[3].qp_new2()); }
  /* p03.RYFO*/ wire2 RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new2(), SARE_XX00_XX07p()); }

  /* p03.RYFO*/ wire2 RYFO_FF04_FF07p_b () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new2(), SARE_XX00_XX07p()); }

  /* p10.AMUS*/ wire2 AMUS_XX_0xx00000p () const { return nor6b(BUS_CPU_A[0].qp_new2(), BUS_CPU_A[1].qp_new2(), BUS_CPU_A[2].qp_new2(), BUS_CPU_A[3].qp_new2(), BUS_CPU_A[4].qp_new2(), BUS_CPU_A[7].qp_new2()); }
  /* p07.SAPA*/ wire2 SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A[0].qp_new2(), BUS_CPU_A[1].qp_new2(), BUS_CPU_A[2].qp_new2(), BUS_CPU_A[3].qp_new2()); }
  /* p07.SEMY*/ wire2 SEMY_XX_0000xxxxp () const { return nor4b(BUS_CPU_A[7].qp_new2(), BUS_CPU_A[6].qp_new2(), BUS_CPU_A[5].qp_new2(), BUS_CPU_A[4].qp_new2()); }
  /* p06.SANO*/ wire2 SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ wire2 ANAP_FF_0xx00000  () const { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ wire2 TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A[13].qp_new2(), SOGY_A14n(), BUS_CPU_A[15].qp_new2()); }
  /* p08.TYNU*/ wire2 TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire2 ADAH_FE00_FEFFn   () const { return not1b(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ wire2 TULO_ADDR_BOOTROMp() const { return nor8b(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), BUS_CPU_A[13].qp_new2(), BUS_CPU_A[12].qp_new2(),
                                                               BUS_CPU_A[11].qp_new2(), BUS_CPU_A[10].qp_new2(), BUS_CPU_A[ 9].qp_new2(), BUS_CPU_A[ 8].qp_new2()); }
  /* p07.TERA*/ wire2 TERA_BOOT_BITp    () const { return not1b(BOOT_BITn_h.qp_new2()); }
  /* p07.TUTU*/ wire2 TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), TULO_ADDR_BOOTROMp()); }

  /* p22.WUTU*/ wire2 WUTU_ADDR_PPUn    () const { return nand3b(SYKE_ADDR_HIp(), BUS_CPU_A[6].qp_new2(), XALY_0x00xxxx()); }
  /* p22.WERO*/ wire2 WERO_ADDR_PPUp    () const { return not1b(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire2 TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A[13].qp_new2(), BUS_CPU_A[14].qp_new2(), SORE_A15n()); }
  /*#p08.TEXO*/ wire2 TEXO_ADDR_VRAMn   () const { return and2(SIG_CPU_EXT_BUSp.qp_new2(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire2 TEFA_ADDR_VRAMp   () const { return nor2b(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire2 SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A[15].qp_new2()); }
  /* p08.LEVO*/ wire2 LEVO_ADDR_VRAMn   () const { return not1b(TEXO_ADDR_VRAMn()); }

  /* p25.TUJA*/ wire2 TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), APOV_CPU_WRp.qp_new2()); }

  /*#p07.ROPE*/ wire2 ROPE_ADDR_OAMn    () const { return nand2b(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
  /*#p07.SARO*/ wire2 SARO_ADDR_OAMp    () const { return not1b(ROPE_ADDR_OAMn()); }

  /* p22.WORU*/ wire2 WORU_FF40n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WOFA*/ wire2 WOFA_FF41n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WEBU*/ wire2 WEBU_FF42n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WAVU*/ wire2 WAVU_FF43n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WYLE*/ wire2 WYLE_FF44n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETY*/ wire2 WETY_FF45n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ wire2 WATE_FF46n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WYBO*/ wire2 WYBO_FF47n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETA*/ wire2 WETA_FF48n        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.VAMA*/ wire2 VAMA_FF49n        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WYVO*/ wire2 WYVO_FF4An        () const { return nand5b(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WAGE*/ wire2 WAGE_FF4Bn        () const { return nand5b(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /* p22.VOCA*/ wire2 VOCA_FF40p        () const { return not1b(WORU_FF40n()); }
  /* p22.VARY*/ wire2 VARY_FF41p        () const { return not1b(WOFA_FF41n()); }
  /* p22.XARO*/ wire2 XARO_FF42p        () const { return not1b(WEBU_FF42n()); }
  /* p22.XAVY*/ wire2 XAVY_FF43p        () const { return not1b(WAVU_FF43n()); }
  /* p22.XOGY*/ wire2 XOGY_FF44p        () const { return not1b(WYLE_FF44n()); }
  /* p22.XAYU*/ wire2 XAYU_FF45p        () const { return not1b(WETY_FF45n()); }
  /*#p22.XEDA*/ wire2 XEDA_FF46p        () const { return not1b(WATE_FF46n()); }
  /* p22.WERA*/ wire2 WERA_FF47p        () const { return not1b(WYBO_FF47n()); }
  /* p22.XAYO*/ wire2 XAYO_FF48p        () const { return not1b(WETA_FF48n()); }
  /* p22.TEGO*/ wire2 TEGO_FF49p        () const { return not1b(VAMA_FF49n()); }
  /* p22.VYGA*/ wire2 VYGA_FF4Ap        () const { return not1b(WYVO_FF4An()); }
  /* p22.VUMY*/ wire2 VUMY_FF4Bp        () const { return not1b(WAGE_FF4Bn()); }

  wire2 TOLE_CPU_VRAM_RDp(wire2 ABUZ_EXT_RAM_CS_CLK)
  {
    /*#p25.TUCA*/ wire2 _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire2 _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new2()); // Ignoring debug for now
    ///*#p25.TEFY*/ wire2 _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new2());
    ///*#p25.TOLE*/ wire2 _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);
    /*#p25.TOLE*/ wire2 _TOLE_CPU_VRAM_RDp = _TUCA_CPU_VRAM_RDp;;
    return _TOLE_CPU_VRAM_RDp;
  }

  wire2 SALE_CPU_VRAM_WRn()
  {
    /*#p25.TEGU*/ wire2 _TEGU_CPU_VRAM_WRn = nand2b(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ wire2 _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new2()); // Ignoring debug for now
    ///*#p25.TEFY*/ wire2 _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new2());
    ///*#p25.SALE*/ wire2 _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
    /*#p25.SALE*/ wire2 _SALE_CPU_VRAM_WRn = _TEGU_CPU_VRAM_WRn;
    return _SALE_CPU_VRAM_WRn;
  }

};