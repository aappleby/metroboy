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

  /* p07.AJAS*/ uint8_t AJAS_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p07.DYKY*/ uint8_t DYKY_CPU_WRn      () const { return not1(TAPU_CPU_WRp.qp_new()); }
  /* p07.ASOT*/ uint8_t ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ uint8_t LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ uint8_t CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ uint8_t MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ uint8_t REDU_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p08.MEXO*/ uint8_t MEXO_CPU_WRn      () const { return not1(APOV_CPU_WRp.qp_new()); }


  /* p04.DECY*/ uint8_t DECY_LATCH_EXTn   () const { return not1(SIG_CPU_LATCH_EXT.qp_new()); }
  /* p04.CATY*/ uint8_t CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ uint8_t BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /* p07.TUNA*/ uint8_t TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), BUS_CPU_A[13].qp_new(), BUS_CPU_A[12].qp_new(), BUS_CPU_A[11].qp_new(), BUS_CPU_A[10].qp_new(), BUS_CPU_A[9].qp_new()); }
  /* p07.RYCU*/ uint8_t RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ uint8_t SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ uint8_t TOVY_A00n         () const { return not1(BUS_CPU_A[0].qp_new()); }
  /* p08.TOLA*/ uint8_t TOLA_A01n         () const { return not1(BUS_CPU_A[1].qp_new()); }
  /* p22.XOLA*/ uint8_t XOLA_A00n         () const { return not1(BUS_CPU_A[0].qp_new()); }
  /* p22.XENO*/ uint8_t XENO_A01n         () const { return not1(BUS_CPU_A[1].qp_new()); }
  /* p22.XUSY*/ uint8_t XUSY_A02n         () const { return not1(BUS_CPU_A[2].qp_new()); }
  /* p22.XERA*/ uint8_t XERA_A03n         () const { return not1(BUS_CPU_A[3].qp_new()); }
  /* p07.TONA*/ uint8_t TONA_A08n         () const { return not1(BUS_CPU_A[8].qp_new()); }
  /*#p08.SORE*/ uint8_t SORE_A15n         () const { return not1(BUS_CPU_A[15].qp_new()); }
  /* p06.SEFY*/ uint8_t SEFY_A02n         () const { return not1(BUS_CPU_A[2].qp_new()); }
  /* p10.BYKO*/ uint8_t BYKO_A05n         () const { return not1(BUS_CPU_A[5].qp_new()); }
  /* p10.AKUG*/ uint8_t AKUG_A06n         () const { return not1(BUS_CPU_A[6].qp_new()); }
  /* p08.SOGY*/ uint8_t SOGY_A14n         () const { return not1(BUS_CPU_A[14].qp_new()); }

  /* p22.WADO*/ uint8_t WADO_A00p         () const { return not1(XOLA_A00n()); }
  /* p22.WESA*/ uint8_t WESA_A01p         () const { return not1(XENO_A01n()); }
  /* p22.WALO*/ uint8_t WALO_A02p         () const { return not1(XUSY_A02n()); }
  /* p22.WEPO*/ uint8_t WEPO_A03p         () const { return not1(XERA_A03n()); }

  /* p07.TYRO*/ uint8_t TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 3].qp_new(), BUS_CPU_A[ 2].qp_new(), BUS_CPU_A[ 1].qp_new(), BUS_CPU_A[ 0].qp_new()); }
  /* p07.TUFA*/ uint8_t TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A[ 4].qp_new(), BUS_CPU_A[ 6].qp_new()); }

  /*#p07.SYKE*/ uint8_t SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ uint8_t SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ uint8_t XALY_0x00xxxx     () const { return nor3(BUS_CPU_A[7].qp_new(), BUS_CPU_A[5].qp_new(), BUS_CPU_A[4].qp_new()); }
  /* p06.SARE*/ uint8_t SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A[7].qp_new(), BUS_CPU_A[6].qp_new(), BUS_CPU_A[5].qp_new(), BUS_CPU_A[4].qp_new(), BUS_CPU_A[3].qp_new()); }
  /* p03.RYFO*/ uint8_t RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new(), SARE_XX00_XX07p()); }

  /* p03.RYFO*/ uint8_t RYFO_FF04_FF07p_b () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A[2].qp_new(), SARE_XX00_XX07p()); }

  /* p10.AMUS*/ uint8_t AMUS_XX_0xx00000p () const { return nor6(BUS_CPU_A[0].qp_new(), BUS_CPU_A[1].qp_new(), BUS_CPU_A[2].qp_new(), BUS_CPU_A[3].qp_new(), BUS_CPU_A[4].qp_new(), BUS_CPU_A[7].qp_new()); }
  /* p07.SAPA*/ uint8_t SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A[0].qp_new(), BUS_CPU_A[1].qp_new(), BUS_CPU_A[2].qp_new(), BUS_CPU_A[3].qp_new()); }
  /* p07.SEMY*/ uint8_t SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A[7].qp_new(), BUS_CPU_A[6].qp_new(), BUS_CPU_A[5].qp_new(), BUS_CPU_A[4].qp_new()); }
  /* p06.SANO*/ uint8_t SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ uint8_t ANAP_FF_0xx00000  () const { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ uint8_t TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A[13].qp_new(), SOGY_A14n(), BUS_CPU_A[15].qp_new()); }
  /* p08.TYNU*/ uint8_t TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ uint8_t ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ uint8_t TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), BUS_CPU_A[13].qp_new(), BUS_CPU_A[12].qp_new(),
                                                              BUS_CPU_A[11].qp_new(), BUS_CPU_A[10].qp_new(), BUS_CPU_A[ 9].qp_new(), BUS_CPU_A[ 8].qp_new()); }
  /* p07.TERA*/ uint8_t TERA_BOOT_BITp    () const { return not1(BOOT_BITn_h.qp_new()); }
  /* p07.TUTU*/ uint8_t TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), TULO_ADDR_BOOTROMp()); }

  /* p22.WUTU*/ uint8_t WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A[6].qp_new(), XALY_0x00xxxx()); }
  /* p22.WERO*/ uint8_t WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ uint8_t TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A[13].qp_new(), BUS_CPU_A[14].qp_new(), SORE_A15n()); }
  /*#p08.TEXO*/ uint8_t TEXO_ADDR_VRAMn   () const { return and2(SIG_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ uint8_t TEFA_ADDR_VRAMp   () const { return nor2(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ uint8_t SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A[15].qp_new()); }
  /* p08.LEVO*/ uint8_t LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }

  /* p25.TUJA*/ uint8_t TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), APOV_CPU_WRp.qp_new()); }

  /*#p07.ROPE*/ uint8_t ROPE_ADDR_OAMn    () const { return nand2(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
  /*#p07.SARO*/ uint8_t SARO_ADDR_OAMp    () const { return not1(ROPE_ADDR_OAMn()); }

  /* p22.WORU*/ uint8_t WORU_FF40n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WOFA*/ uint8_t WOFA_FF41n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WEBU*/ uint8_t WEBU_FF42n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WAVU*/ uint8_t WAVU_FF43n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /* p22.WYLE*/ uint8_t WYLE_FF44n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETY*/ uint8_t WETY_FF45n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ uint8_t WATE_FF46n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WYBO*/ uint8_t WYBO_FF47n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /* p22.WETA*/ uint8_t WETA_FF48n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.VAMA*/ uint8_t VAMA_FF49n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WYVO*/ uint8_t WYVO_FF4An        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /* p22.WAGE*/ uint8_t WAGE_FF4Bn        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /* p22.VOCA*/ uint8_t VOCA_FF40p        () const { return not1(WORU_FF40n()); }
  /* p22.VARY*/ uint8_t VARY_FF41p        () const { return not1(WOFA_FF41n()); }
  /* p22.XARO*/ uint8_t XARO_FF42p        () const { return not1(WEBU_FF42n()); }
  /* p22.XAVY*/ uint8_t XAVY_FF43p        () const { return not1(WAVU_FF43n()); }
  /* p22.XOGY*/ uint8_t XOGY_FF44p        () const { return not1(WYLE_FF44n()); }
  /* p22.XAYU*/ uint8_t XAYU_FF45p        () const { return not1(WETY_FF45n()); }
  /*#p22.XEDA*/ uint8_t XEDA_FF46p        () const { return not1(WATE_FF46n()); }
  /* p22.WERA*/ uint8_t WERA_FF47p        () const { return not1(WYBO_FF47n()); }
  /* p22.XAYO*/ uint8_t XAYO_FF48p        () const { return not1(WETA_FF48n()); }
  /* p22.TEGO*/ uint8_t TEGO_FF49p        () const { return not1(VAMA_FF49n()); }
  /* p22.VYGA*/ uint8_t VYGA_FF4Ap        () const { return not1(WYVO_FF4An()); }
  /* p22.VUMY*/ uint8_t VUMY_FF4Bp        () const { return not1(WAGE_FF4Bn()); }

  uint8_t TOLE_CPU_VRAM_RDp(uint8_t ABUZ_EXT_RAM_CS_CLK)
  {
    /*#p25.TUCA*/ uint8_t _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ uint8_t _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new()); // Ignoring debug for now
    ///*#p25.TEFY*/ uint8_t _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ uint8_t _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);
    /*#p25.TOLE*/ uint8_t _TOLE_CPU_VRAM_RDp = _TUCA_CPU_VRAM_RDp;;
    return _TOLE_CPU_VRAM_RDp;
  }

  uint8_t SALE_CPU_VRAM_WRn()
  {
    /*#p25.TEGU*/ uint8_t _TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ uint8_t _TAVY_MOEp         = not1(vram_bus.PIN45_VRAM_OEn.qn_new()); // Ignoring debug for now
    ///*#p25.TEFY*/ uint8_t _TEFY_VRAM_MCSp    = not1(vram_bus.PIN43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ uint8_t _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
    /*#p25.SALE*/ uint8_t _SALE_CPU_VRAM_WRn = _TEGU_CPU_VRAM_WRn;
    return _SALE_CPU_VRAM_WRn;
  }

};