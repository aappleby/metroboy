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
    //d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
    d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", SIG_CPU_LATCH_EXT.state);
    d.dump_bitp   ("BOOT_BITn         : ", TEPU_BOOT_BITn_h.state);
    d.dump_bitp   ("SIG_CPU_BOOTp     : ", SIG_CPU_BOOTp.state);
    d.dump_bitp   ("TEDO_CPU_RDp      : ", TEDO_CPU_RDp.state);
    d.dump_bitp   ("APOV_CPU_WRp      : ", APOV_CPU_WRp.state);
    d.dump_bitp   ("TAPU_CPU_WRp      : ", TAPU_CPU_WRp.state);
    d.dump_slice2p("BUS_CPU_A : ", &BUS_CPU_A00p, 16);
    d.dump_slice2p("BUS_CPU_D : ", &BUS_CPU_D00p, 8);
  }

  Signal ABUZ_EXT_RAM_CS_CLK;

  /*BUS_CPU_A00p*/ Bus BUS_CPU_A00p;
  /*BUS_CPU_A01p*/ Bus BUS_CPU_A01p;
  /*BUS_CPU_A02p*/ Bus BUS_CPU_A02p;
  /*BUS_CPU_A03p*/ Bus BUS_CPU_A03p;
  /*BUS_CPU_A04p*/ Bus BUS_CPU_A04p;
  /*BUS_CPU_A05p*/ Bus BUS_CPU_A05p;
  /*BUS_CPU_A06p*/ Bus BUS_CPU_A06p;
  /*BUS_CPU_A07p*/ Bus BUS_CPU_A07p;
  /*BUS_CPU_A08p*/ Bus BUS_CPU_A08p;
  /*BUS_CPU_A09p*/ Bus BUS_CPU_A09p;
  /*BUS_CPU_A10p*/ Bus BUS_CPU_A10p;
  /*BUS_CPU_A11p*/ Bus BUS_CPU_A11p;
  /*BUS_CPU_A12p*/ Bus BUS_CPU_A12p;
  /*BUS_CPU_A13p*/ Bus BUS_CPU_A13p;
  /*BUS_CPU_A14p*/ Bus BUS_CPU_A14p;
  /*BUS_CPU_A15p*/ Bus BUS_CPU_A15p;

  /*BUS_CPU_D00p*/ Bus BUS_CPU_D00p;
  /*BUS_CPU_D01p*/ Bus BUS_CPU_D01p;
  /*BUS_CPU_D02p*/ Bus BUS_CPU_D02p;
  /*BUS_CPU_D03p*/ Bus BUS_CPU_D03p;
  /*BUS_CPU_D04p*/ Bus BUS_CPU_D04p;
  /*BUS_CPU_D05p*/ Bus BUS_CPU_D05p;
  /*BUS_CPU_D06p*/ Bus BUS_CPU_D06p;
  /*BUS_CPU_D07p*/ Bus BUS_CPU_D07p;

  /*SIG_CPU_RDp     */  Signal SIG_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*SIG_CPU_WRp     */  Signal SIG_CPU_WRp;           // top right port PORTA_01: ->
  /*SIG_CPU_UNOR_DBG*/  Signal SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  /*SIG_CPU_ADDR_HIp*/  Signal SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  /*SIG_CPU_UMUT_DBG*/  Signal SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  /*SIG_CPU_EXT_BUSp*/  Signal SIG_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  ///*SIG_CPU_6*/         Signal SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  /*SIG_CPU_LATCH_EXT*/ Signal SIG_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  /*SIG_CPU_BOOTp*/     Signal SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  /*SIG_BOOT_CSp*/      Signal SIG_BOOT_CSp;          // cs for bootrom

  /* p07.TEPU*/ DFF17 TEPU_BOOT_BITn_h;

  Signal TEDO_CPU_RDp;
  Signal APOV_CPU_WRp;
  Signal TAPU_CPU_WRp;

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(TAPU_CPU_WRp.qp_new()); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(TEDO_CPU_RDp.qp_new()); }
  /* p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(APOV_CPU_WRp.qp_new()); }


  /* p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(SIG_CPU_LATCH_EXT.qp_new()); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /* p07.TUNA*/ wire TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A15p.qp_new(), BUS_CPU_A14p.qp_new(), BUS_CPU_A13p.qp_new(), BUS_CPU_A12p.qp_new(), BUS_CPU_A11p.qp_new(), BUS_CPU_A10p.qp_new(), BUS_CPU_A09p.qp_new()); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ wire TOVY_A00n         () const { return not1(BUS_CPU_A00p.qp_new()); }
  /* p08.TOLA*/ wire TOLA_A01n         () const { return not1(BUS_CPU_A01p.qp_new()); }
  /* p22.XOLA*/ wire XOLA_A00n         () const { return not1(BUS_CPU_A00p.qp_new()); }
  /* p22.XENO*/ wire XENO_A01n         () const { return not1(BUS_CPU_A01p.qp_new()); }
  /* p22.XUSY*/ wire XUSY_A02n         () const { return not1(BUS_CPU_A02p.qp_new()); }
  /* p22.XERA*/ wire XERA_A03n         () const { return not1(BUS_CPU_A03p.qp_new()); }
  /* p07.TONA*/ wire TONA_A08n         () const { return not1(BUS_CPU_A08p.qp_new()); }
  /*#p08.SORE*/ wire SORE_A15n         () const { return not1(BUS_CPU_A15p.qp_new()); }
  /* p06.SEFY*/ wire SEFY_A02n         () const { return not1(BUS_CPU_A02p.qp_new()); }
  /* p10.BYKO*/ wire BYKO_A05n         () const { return not1(BUS_CPU_A05p.qp_new()); }
  /* p10.AKUG*/ wire AKUG_A06n         () const { return not1(BUS_CPU_A06p.qp_new()); }
  /* p08.SOGY*/ wire SOGY_A14n         () const { return not1(BUS_CPU_A14p.qp_new()); }

  /* p22.WADO*/ wire WADO_A00p         () const { return not1(XOLA_A00n()); }
  /* p22.WESA*/ wire WESA_A01p         () const { return not1(XENO_A01n()); }
  /* p22.WALO*/ wire WALO_A02p         () const { return not1(XUSY_A02n()); }
  /* p22.WEPO*/ wire WEPO_A03p         () const { return not1(XERA_A03n()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A07p.qp_new(), BUS_CPU_A05p.qp_new(), BUS_CPU_A03p.qp_new(), BUS_CPU_A02p.qp_new(), BUS_CPU_A01p.qp_new(), BUS_CPU_A00p.qp_new()); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A04p.qp_new(), BUS_CPU_A06p.qp_new()); }

  /*#p07.SYKE*/ wire SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ wire XALY_0x00xxxx     () const { return nor3(BUS_CPU_A07p.qp_new(), BUS_CPU_A05p.qp_new(), BUS_CPU_A04p.qp_new()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A07p.qp_new(), BUS_CPU_A06p.qp_new(), BUS_CPU_A05p.qp_new(), BUS_CPU_A04p.qp_new(), BUS_CPU_A03p.qp_new()); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A02p.qp_new(), SARE_XX00_XX07p()); }

  /* p03.RYFO*/ wire RYFO_FF04_FF07p_b () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A02p.qp_new(), SARE_XX00_XX07p()); }

  /* p10.AMUS*/ wire AMUS_XX_0xx00000p () const { return nor6(BUS_CPU_A00p.qp_new(), BUS_CPU_A01p.qp_new(), BUS_CPU_A02p.qp_new(), BUS_CPU_A03p.qp_new(), BUS_CPU_A04p.qp_new(), BUS_CPU_A07p.qp_new()); }
  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A00p.qp_new(), BUS_CPU_A01p.qp_new(), BUS_CPU_A02p.qp_new(), BUS_CPU_A03p.qp_new()); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A07p.qp_new(), BUS_CPU_A06p.qp_new(), BUS_CPU_A05p.qp_new(), BUS_CPU_A04p.qp_new()); }
  /* p06.SANO*/ wire SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p10.ANAP*/ wire ANAP_FF_0xx00000  () const { return and2(SYKE_ADDR_HIp(), AMUS_XX_0xx00000p()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A13p.qp_new(), SOGY_A14n(), BUS_CPU_A15p.qp_new()); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A15p.qp_new(), BUS_CPU_A14p.qp_new(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ wire TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A15p.qp_new(), BUS_CPU_A14p.qp_new(), BUS_CPU_A13p.qp_new(), BUS_CPU_A12p.qp_new(), BUS_CPU_A11p.qp_new(), BUS_CPU_A10p.qp_new(), BUS_CPU_A09p.qp_new(), BUS_CPU_A08p.qp_new()); }
  /* p07.TERA*/ wire TERA_BOOT_BITp    () const { return not1(TEPU_BOOT_BITn_h.qp_new()); }
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), TULO_ADDR_BOOTROMp()); }

  /* p22.WUTU*/ wire WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A06p.qp_new(), XALY_0x00xxxx()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A13p.qp_new(), BUS_CPU_A14p.qp_new(), SORE_A15n()); }
  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(SIG_CPU_EXT_BUSp.qp_new(), TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), BUS_CPU_A15p.qp_new()); }
  /* p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }

  /* p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), APOV_CPU_WRp.qp_new()); }

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

  wire TOLE_CPU_VRAM_RDp()
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK.qp_new());
    /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = not1(_TUCA_CPU_VRAM_RDp);

    return _TOLE_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn()
  {
    // Ignoring debug for now
    ///*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);

    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = not1(_TEGU_CPU_VRAM_WRn);

    return _SALE_CPU_VRAM_WRn;
  }

};