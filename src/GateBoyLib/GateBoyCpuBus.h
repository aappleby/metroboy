#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyCpuSignals {
  void reset_to_bootrom();
  void reset_to_cart();

  Gate ABUZ_EXT_RAM_CS_CLK;

  /*_SIG_IN_CPU_RDp     */  SigIn  SIG_IN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*_SIG_IN_CPU_WRp     */  SigIn  SIG_IN_CPU_WRp;           // top right port PORTA_01: ->
  /*_SIG_IN_CPU_EXT_BUSp*/  SigIn  SIG_IN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  /*_SIG_IN_CPU_LATCH_EXT*/ SigIn  SIG_IN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  /*_SIG_CPU_UNOR_DBG*/  SigOut SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  /*_SIG_CPU_ADDR_HIp*/  SigOut SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  /*_SIG_CPU_UMUT_DBG*/  SigOut SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  /*_SIG_CPU_BOOTp*/     SigOut SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  /*_SIG_BOOT_CSp*/      SigOut SIG_BOOT_CSp;          // cs for bootrom

  /*_p07.TEPU*/ DFF17 TEPU_BOOT_BITn;

  Gate TEDO_CPU_RDp;
  Gate APOV_CPU_WRp;
  Gate TAPU_CPU_WRp;

  ///*_SIG_CPU_6*/         SigOut SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? Drives an unwired pad.
};

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyCpuABus {
  void reset_to_cart_new() {
    BUS_CPU_A00p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A01p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A02p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A03p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A04p.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_CPU_A05p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A06p.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_CPU_A07p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A08p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A09p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A10p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A11p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A12p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A13p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A14p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_CPU_A15p.state = BIT_OLD | BIT_DRIVEN | 0;
  }

  void reset_to_bootrom() {
    BUS_CPU_A00p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A01p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A02p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A03p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A04p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A05p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A06p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A07p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A08p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A09p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A10p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A11p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A12p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A13p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A14p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_A15p.state = BIT_OLD | BIT_PULLED | 1;
  }

  void set_addr(uint16_t bus_addr_new)
  {
    triwire EXT_bus_addr00 = tri_pp(1, bit(bus_addr_new, 0));
    triwire EXT_bus_addr01 = tri_pp(1, bit(bus_addr_new, 1));
    triwire EXT_bus_addr02 = tri_pp(1, bit(bus_addr_new, 2));
    triwire EXT_bus_addr03 = tri_pp(1, bit(bus_addr_new, 3));
    triwire EXT_bus_addr04 = tri_pp(1, bit(bus_addr_new, 4));
    triwire EXT_bus_addr05 = tri_pp(1, bit(bus_addr_new, 5));
    triwire EXT_bus_addr06 = tri_pp(1, bit(bus_addr_new, 6));
    triwire EXT_bus_addr07 = tri_pp(1, bit(bus_addr_new, 7));
    triwire EXT_bus_addr08 = tri_pp(1, bit(bus_addr_new, 8));
    triwire EXT_bus_addr09 = tri_pp(1, bit(bus_addr_new, 9));
    triwire EXT_bus_addr10 = tri_pp(1, bit(bus_addr_new, 10));
    triwire EXT_bus_addr11 = tri_pp(1, bit(bus_addr_new, 11));
    triwire EXT_bus_addr12 = tri_pp(1, bit(bus_addr_new, 12));
    triwire EXT_bus_addr13 = tri_pp(1, bit(bus_addr_new, 13));
    triwire EXT_bus_addr14 = tri_pp(1, bit(bus_addr_new, 14));
    triwire EXT_bus_addr15 = tri_pp(1, bit(bus_addr_new, 15));

    /*_BUS_CPU_A00p*/ BUS_CPU_A00p.tri_bus(EXT_bus_addr00);
    /*_BUS_CPU_A01p*/ BUS_CPU_A01p.tri_bus(EXT_bus_addr01);
    /*_BUS_CPU_A02p*/ BUS_CPU_A02p.tri_bus(EXT_bus_addr02);
    /*_BUS_CPU_A03p*/ BUS_CPU_A03p.tri_bus(EXT_bus_addr03);
    /*_BUS_CPU_A04p*/ BUS_CPU_A04p.tri_bus(EXT_bus_addr04);
    /*_BUS_CPU_A05p*/ BUS_CPU_A05p.tri_bus(EXT_bus_addr05);
    /*_BUS_CPU_A06p*/ BUS_CPU_A06p.tri_bus(EXT_bus_addr06);
    /*_BUS_CPU_A07p*/ BUS_CPU_A07p.tri_bus(EXT_bus_addr07);
    /*_BUS_CPU_A08p*/ BUS_CPU_A08p.tri_bus(EXT_bus_addr08);
    /*_BUS_CPU_A09p*/ BUS_CPU_A09p.tri_bus(EXT_bus_addr09);
    /*_BUS_CPU_A10p*/ BUS_CPU_A10p.tri_bus(EXT_bus_addr10);
    /*_BUS_CPU_A11p*/ BUS_CPU_A11p.tri_bus(EXT_bus_addr11);
    /*_BUS_CPU_A12p*/ BUS_CPU_A12p.tri_bus(EXT_bus_addr12);
    /*_BUS_CPU_A13p*/ BUS_CPU_A13p.tri_bus(EXT_bus_addr13);
    /*_BUS_CPU_A14p*/ BUS_CPU_A14p.tri_bus(EXT_bus_addr14);
    /*_BUS_CPU_A15p*/ BUS_CPU_A15p.tri_bus(EXT_bus_addr15);
  }

  /*_p07.TUNA*/ wire TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A15p.out_any(), BUS_CPU_A14p.out_any(), BUS_CPU_A13p.out_any(), BUS_CPU_A12p.out_any(), BUS_CPU_A11p.out_any(), BUS_CPU_A10p.out_any(), BUS_CPU_A09p.out_any()); }
  /*_p07.RYCU*/ wire RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
  /*_p25.SYRO*/ wire SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

  /*_p03.TOVY*/ wire TOVY_A00n         () const { return not1(BUS_CPU_A00p.out_any()); }
  /*_p08.TOLA*/ wire TOLA_A01n         () const { return not1(BUS_CPU_A01p.out_any()); }
  /*_p22.XOLA*/ wire XOLA_A00n         () const { return not1(BUS_CPU_A00p.out_any()); }
  /*_p22.XENO*/ wire XENO_A01n         () const { return not1(BUS_CPU_A01p.out_any()); }
  /*_p22.XUSY*/ wire XUSY_A02n         () const { return not1(BUS_CPU_A02p.out_any()); }
  /*_p22.XERA*/ wire XERA_A03n         () const { return not1(BUS_CPU_A03p.out_any()); }
  /*_p07.TONA*/ wire TONA_A08n         () const { return not1(BUS_CPU_A08p.out_any()); }
  /*#p08.SORE*/ wire SORE_A15n         () const { return not1(BUS_CPU_A15p.out_any()); }
  /*_p06.SEFY*/ wire SEFY_A02n         () const { return not1(BUS_CPU_A02p.out_any()); }
  /*_p08.SOGY*/ wire SOGY_A14n         () const { return not1(BUS_CPU_A14p.out_any()); }

  /*_p22.WADO*/ wire WADO_A00p         () const { return not1(XOLA_A00n()); }
  /*_p22.WESA*/ wire WESA_A01p         () const { return not1(XENO_A01n()); }
  /*_p22.WALO*/ wire WALO_A02p         () const { return not1(XUSY_A02n()); }
  /*_p22.WEPO*/ wire WEPO_A03p         () const { return not1(XERA_A03n()); }

  /*_p07.TYRO*/ wire TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A07p.out_any(), BUS_CPU_A05p.out_any(), BUS_CPU_A03p.out_any(), BUS_CPU_A02p.out_any(), BUS_CPU_A01p.out_any(), BUS_CPU_A00p.out_any()); }
  /*_p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A04p.out_any(), BUS_CPU_A06p.out_any()); }

  /*#p07.SYKE*/ wire SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

  /*_p22.XALY*/ wire XALY_0x00xxxx     () const { return nor3(BUS_CPU_A07p.out_any(), BUS_CPU_A05p.out_any(), BUS_CPU_A04p.out_any()); }
  /*_p06.SARE*/ wire SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A07p.out_any(), BUS_CPU_A06p.out_any(), BUS_CPU_A05p.out_any(), BUS_CPU_A04p.out_any(), BUS_CPU_A03p.out_any()); }
  /*_p03.RYFO*/ wire RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A02p.out_any(), SARE_XX00_XX07p()); }

  /*_p07.SAPA*/ wire SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A00p.out_any(), BUS_CPU_A01p.out_any(), BUS_CPU_A02p.out_any(), BUS_CPU_A03p.out_any()); }
  /*_p07.SEMY*/ wire SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A07p.out_any(), BUS_CPU_A06p.out_any(), BUS_CPU_A05p.out_any(), BUS_CPU_A04p.out_any()); }
  /*_p06.SANO*/ wire SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /*_p08.TUMA*/ wire TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A13p.out_any(), SOGY_A14n(), BUS_CPU_A15p.out_any()); }
  /*_p08.TYNU*/ wire TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A15p.out_any(), BUS_CPU_A14p.out_any(), TUMA_A000_BFFFp()); }
  /*_p28.ADAH*/ wire ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

  /*_p07.TULO*/ wire TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A15p.out_any(), BUS_CPU_A14p.out_any(), BUS_CPU_A13p.out_any(), BUS_CPU_A12p.out_any(), BUS_CPU_A11p.out_any(), BUS_CPU_A10p.out_any(), BUS_CPU_A09p.out_any(), BUS_CPU_A08p.out_any()); }

  /*#p07.ROPE*/ wire ROPE_ADDR_OAMn    () const { return nand2(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
  /*#p07.SARO*/ wire SARO_ADDR_OAMp    () const { return not1(ROPE_ADDR_OAMn()); }

  /*_p22.WORU*/ wire WORU_FF40n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /*_p22.WOFA*/ wire WOFA_FF41n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
  /*_p22.WEBU*/ wire WEBU_FF42n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /*_p22.WAVU*/ wire WAVU_FF43n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
  /*_p22.WYLE*/ wire WYLE_FF44n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*_p22.WETY*/ wire WETY_FF45n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
  /*#p22.WATE*/ wire WATE_FF46n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /*_p22.WYBO*/ wire WYBO_FF47n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
  /*_p22.WETA*/ wire WETA_FF48n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /*_p22.VAMA*/ wire VAMA_FF49n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
  /*_p22.WYVO*/ wire WYVO_FF4An        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
  /*_p22.WAGE*/ wire WAGE_FF4Bn        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

  /*_p22.VOCA*/ wire VOCA_FF40p        () const { return not1(WORU_FF40n()); }
  /*_p22.VARY*/ wire VARY_FF41p        () const { return not1(WOFA_FF41n()); }
  /*_p22.XARO*/ wire XARO_FF42p        () const { return not1(WEBU_FF42n()); }
  /*_p22.XAVY*/ wire XAVY_FF43p        () const { return not1(WAVU_FF43n()); }
  /*_p22.XOGY*/ wire XOGY_FF44p        () const { return not1(WYLE_FF44n()); }
  /*_p22.XAYU*/ wire XAYU_FF45p        () const { return not1(WETY_FF45n()); }
  /*#p22.XEDA*/ wire XEDA_FF46p        () const { return not1(WATE_FF46n()); }
  /*_p22.WERA*/ wire WERA_FF47p        () const { return not1(WYBO_FF47n()); }
  /*_p22.XAYO*/ wire XAYO_FF48p        () const { return not1(WETA_FF48n()); }
  /*_p22.TEGO*/ wire TEGO_FF49p        () const { return not1(VAMA_FF49n()); }
  /*_p22.VYGA*/ wire VYGA_FF4Ap        () const { return not1(WYVO_FF4An()); }
  /*_p22.VUMY*/ wire VUMY_FF4Bp        () const { return not1(WAGE_FF4Bn()); }

  /*_p22.WUTU*/ wire WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A06p.out_any(), XALY_0x00xxxx()); }
  /*_p22.WERO*/ wire WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A13p.out_any(), BUS_CPU_A14p.out_any(), SORE_A15n()); }

  /*_BUS_CPU_A00p*/ Bus BUS_CPU_A00p;
  /*_BUS_CPU_A01p*/ Bus BUS_CPU_A01p;
  /*_BUS_CPU_A02p*/ Bus BUS_CPU_A02p;
  /*_BUS_CPU_A03p*/ Bus BUS_CPU_A03p;
  /*_BUS_CPU_A04p*/ Bus BUS_CPU_A04p;
  /*_BUS_CPU_A05p*/ Bus BUS_CPU_A05p;
  /*_BUS_CPU_A06p*/ Bus BUS_CPU_A06p;
  /*_BUS_CPU_A07p*/ Bus BUS_CPU_A07p;
  /*_BUS_CPU_A08p*/ Bus BUS_CPU_A08p;
  /*_BUS_CPU_A09p*/ Bus BUS_CPU_A09p;
  /*_BUS_CPU_A10p*/ Bus BUS_CPU_A10p;
  /*_BUS_CPU_A11p*/ Bus BUS_CPU_A11p;
  /*_BUS_CPU_A12p*/ Bus BUS_CPU_A12p;
  /*_BUS_CPU_A13p*/ Bus BUS_CPU_A13p;
  /*_BUS_CPU_A14p*/ Bus BUS_CPU_A14p;
  /*_BUS_CPU_A15p*/ Bus BUS_CPU_A15p;
};

//---------------------------------------------------------------------------------------------------------------------

struct GateBoyCpuDBus {
  void reset_to_cart_new() {
    BUS_CPU_D00p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D01p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D02p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D03p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D04p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D05p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D06p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D07p.state = BIT_OLD | BIT_PULLED | 1;
  }

  void reset_to_bootrom() {
    BUS_CPU_D00p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D01p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D02p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D03p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D04p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D05p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D06p.state = BIT_OLD | BIT_PULLED | 1;
    BUS_CPU_D07p.state = BIT_OLD | BIT_PULLED | 1;
  }

  void set_data(bool OEp, uint8_t data) {
    triwire tri0 = tri_pp(OEp, bit(data, 0));
    triwire tri1 = tri_pp(OEp, bit(data, 1));
    triwire tri2 = tri_pp(OEp, bit(data, 2));
    triwire tri3 = tri_pp(OEp, bit(data, 3));
    triwire tri4 = tri_pp(OEp, bit(data, 4));
    triwire tri5 = tri_pp(OEp, bit(data, 5));
    triwire tri6 = tri_pp(OEp, bit(data, 6));
    triwire tri7 = tri_pp(OEp, bit(data, 7));

    BUS_CPU_D00p.tri_bus(tri0);
    BUS_CPU_D01p.tri_bus(tri1);
    BUS_CPU_D02p.tri_bus(tri2);
    BUS_CPU_D03p.tri_bus(tri3);
    BUS_CPU_D04p.tri_bus(tri4);
    BUS_CPU_D05p.tri_bus(tri5);
    BUS_CPU_D06p.tri_bus(tri6);
    BUS_CPU_D07p.tri_bus(tri7);
  }

  /*_BUS_CPU_D00p*/ Bus BUS_CPU_D00p;
  /*_BUS_CPU_D01p*/ Bus BUS_CPU_D01p;
  /*_BUS_CPU_D02p*/ Bus BUS_CPU_D02p;
  /*_BUS_CPU_D03p*/ Bus BUS_CPU_D03p;
  /*_BUS_CPU_D04p*/ Bus BUS_CPU_D04p;
  /*_BUS_CPU_D05p*/ Bus BUS_CPU_D05p;
  /*_BUS_CPU_D06p*/ Bus BUS_CPU_D06p;
  /*_BUS_CPU_D07p*/ Bus BUS_CPU_D07p;
};

//---------------------------------------------------------------------------------------------------------------------
