#pragma once

#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyBuses {
  void reset_to_cart_new() {
    BUS_CPU_A00p.state = 0b00011100;
    BUS_CPU_A01p.state = 0b00011100;
    BUS_CPU_A02p.state = 0b00011100;
    BUS_CPU_A03p.state = 0b00011100;
    BUS_CPU_A04p.state = 0b00011101;
    BUS_CPU_A05p.state = 0b00011100;
    BUS_CPU_A06p.state = 0b00011101;
    BUS_CPU_A07p.state = 0b00011100;
    BUS_CPU_A08p.state = 0b00011100;
    BUS_CPU_A09p.state = 0b00011100;
    BUS_CPU_A10p.state = 0b00011100;
    BUS_CPU_A11p.state = 0b00011100;
    BUS_CPU_A12p.state = 0b00011100;
    BUS_CPU_A13p.state = 0b00011100;
    BUS_CPU_A14p.state = 0b00011100;
    BUS_CPU_A15p.state = 0b00011100;
    BUS_CPU_D00p.state = 0b00010101;
    BUS_CPU_D01p.state = 0b00010101;
    BUS_CPU_D02p.state = 0b00010101;
    BUS_CPU_D03p.state = 0b00010101;
    BUS_CPU_D04p.state = 0b00010101;
    BUS_CPU_D05p.state = 0b00010101;
    BUS_CPU_D06p.state = 0b00010101;
    BUS_CPU_D07p.state = 0b00010101;

    BUS_SPR_I0.state = 0b00011100;
    BUS_SPR_I1.state = 0b00011100;
    BUS_SPR_I2.state = 0b00011101;
    BUS_SPR_I3.state = 0b00011100;
    BUS_SPR_I4.state = 0b00011101;
    BUS_SPR_I5.state = 0b00011100;
    BUS_SPR_L0.state = 0b00011101;
    BUS_SPR_L1.state = 0b00011101;
    BUS_SPR_L2.state = 0b00011101;
    BUS_SPR_L3.state = 0b00011101;

    BUS_OAM_A00n.state  = 0b00011101;
    BUS_OAM_A01n.state  = 0b00011101;
    BUS_OAM_A02n.state  = 0b00011101;
    BUS_OAM_A03n.state  = 0b00011101;
    BUS_OAM_A04n.state  = 0b00011100;
    BUS_OAM_A05n.state  = 0b00011101;
    BUS_OAM_A06n.state  = 0b00011100;
    BUS_OAM_A07n.state  = 0b00011101;
    BUS_OAM_DA00n.state = 0b00011100;
    BUS_OAM_DA01n.state = 0b00011100;
    BUS_OAM_DA02n.state = 0b00011100;
    BUS_OAM_DA03n.state = 0b00011100;
    BUS_OAM_DA04n.state = 0b00011100;
    BUS_OAM_DA05n.state = 0b00011100;
    BUS_OAM_DA06n.state = 0b00011100;
    BUS_OAM_DA07n.state = 0b00011100;
    BUS_OAM_DB00n.state = 0b00011100;
    BUS_OAM_DB01n.state = 0b00011100;
    BUS_OAM_DB02n.state = 0b00011100;
    BUS_OAM_DB03n.state = 0b00011100;
    BUS_OAM_DB04n.state = 0b00011100;
    BUS_OAM_DB05n.state = 0b00011100;
    BUS_OAM_DB06n.state = 0b00011100;
    BUS_OAM_DB07n.state = 0b00011100;

    BUS_VRAM_A00n.state = 0b00011101;
    BUS_VRAM_A01n.state = 0b00011101;
    BUS_VRAM_A02n.state = 0b00011101;
    BUS_VRAM_A03n.state = 0b00011101;
    BUS_VRAM_A04n.state = 0b00011100;
    BUS_VRAM_A05n.state = 0b00011101;
    BUS_VRAM_A06n.state = 0b00011100;
    BUS_VRAM_A07n.state = 0b00011101;
    BUS_VRAM_A08n.state = 0b00011101;
    BUS_VRAM_A09n.state = 0b00011101;
    BUS_VRAM_A10n.state = 0b00011101;
    BUS_VRAM_A11n.state = 0b00011101;
    BUS_VRAM_A12n.state = 0b00011101;
    BUS_VRAM_D00p.state = 0b00011100;
    BUS_VRAM_D01p.state = 0b00011100;
    BUS_VRAM_D02p.state = 0b00011100;
    BUS_VRAM_D03p.state = 0b00011100;
    BUS_VRAM_D04p.state = 0b00011100;
    BUS_VRAM_D05p.state = 0b00011100;
    BUS_VRAM_D06p.state = 0b00011100;
    BUS_VRAM_D07p.state = 0b00011100;
  }

  void reset_for_pass() {
    BUS_CPU_A00p.reset_for_pass();
    BUS_CPU_A01p.reset_for_pass();
    BUS_CPU_A02p.reset_for_pass();
    BUS_CPU_A03p.reset_for_pass();
    BUS_CPU_A04p.reset_for_pass();
    BUS_CPU_A05p.reset_for_pass();
    BUS_CPU_A06p.reset_for_pass();
    BUS_CPU_A07p.reset_for_pass();
    BUS_CPU_A08p.reset_for_pass();
    BUS_CPU_A09p.reset_for_pass();
    BUS_CPU_A10p.reset_for_pass();
    BUS_CPU_A11p.reset_for_pass();
    BUS_CPU_A12p.reset_for_pass();
    BUS_CPU_A13p.reset_for_pass();
    BUS_CPU_A14p.reset_for_pass();
    BUS_CPU_A15p.reset_for_pass();

    BUS_CPU_D00p.reset_for_pass();
    BUS_CPU_D01p.reset_for_pass();
    BUS_CPU_D02p.reset_for_pass();
    BUS_CPU_D03p.reset_for_pass();
    BUS_CPU_D04p.reset_for_pass();
    BUS_CPU_D05p.reset_for_pass();
    BUS_CPU_D06p.reset_for_pass();
    BUS_CPU_D07p.reset_for_pass();

    BUS_SPR_I0.reset_for_pass();
    BUS_SPR_I1.reset_for_pass();
    BUS_SPR_I2.reset_for_pass();
    BUS_SPR_I3.reset_for_pass();
    BUS_SPR_I4.reset_for_pass();
    BUS_SPR_I5.reset_for_pass();

    BUS_SPR_L0.reset_for_pass();
    BUS_SPR_L1.reset_for_pass();
    BUS_SPR_L2.reset_for_pass();
    BUS_SPR_L3.reset_for_pass();

    BUS_OAM_A00n.reset_for_pass();
    BUS_OAM_A01n.reset_for_pass();
    BUS_OAM_A02n.reset_for_pass();
    BUS_OAM_A03n.reset_for_pass();
    BUS_OAM_A04n.reset_for_pass();
    BUS_OAM_A05n.reset_for_pass();
    BUS_OAM_A06n.reset_for_pass();
    BUS_OAM_A07n.reset_for_pass();

    BUS_OAM_DA00n.reset_for_pass();
    BUS_OAM_DA01n.reset_for_pass();
    BUS_OAM_DA02n.reset_for_pass();
    BUS_OAM_DA03n.reset_for_pass();
    BUS_OAM_DA04n.reset_for_pass();
    BUS_OAM_DA05n.reset_for_pass();
    BUS_OAM_DA06n.reset_for_pass();
    BUS_OAM_DA07n.reset_for_pass();

    BUS_OAM_DB00n.reset_for_pass();
    BUS_OAM_DB01n.reset_for_pass();
    BUS_OAM_DB02n.reset_for_pass();
    BUS_OAM_DB03n.reset_for_pass();
    BUS_OAM_DB04n.reset_for_pass();
    BUS_OAM_DB05n.reset_for_pass();
    BUS_OAM_DB06n.reset_for_pass();
    BUS_OAM_DB07n.reset_for_pass();

    BUS_VRAM_A00n.reset_for_pass();
    BUS_VRAM_A01n.reset_for_pass();
    BUS_VRAM_A02n.reset_for_pass();
    BUS_VRAM_A03n.reset_for_pass();
    BUS_VRAM_A04n.reset_for_pass();
    BUS_VRAM_A05n.reset_for_pass();
    BUS_VRAM_A06n.reset_for_pass();
    BUS_VRAM_A07n.reset_for_pass();
    BUS_VRAM_A08n.reset_for_pass();
    BUS_VRAM_A09n.reset_for_pass();
    BUS_VRAM_A10n.reset_for_pass();
    BUS_VRAM_A11n.reset_for_pass();
    BUS_VRAM_A12n.reset_for_pass();

    BUS_VRAM_D00p.reset_for_pass();
    BUS_VRAM_D01p.reset_for_pass();
    BUS_VRAM_D02p.reset_for_pass();
    BUS_VRAM_D03p.reset_for_pass();
    BUS_VRAM_D04p.reset_for_pass();
    BUS_VRAM_D05p.reset_for_pass();
    BUS_VRAM_D06p.reset_for_pass();
    BUS_VRAM_D07p.reset_for_pass();
  }

  void set_addr(int phase_total, Req bus_req_new)
  {
    uint16_t bus_addr_new = DELTA_HA ? bus_req_new.addr & 0x00FF : bus_req_new.addr;
    BUS_CPU_A00p.set((bus_addr_new >>  0) & 1);
    BUS_CPU_A01p.set((bus_addr_new >>  1) & 1);
    BUS_CPU_A02p.set((bus_addr_new >>  2) & 1);
    BUS_CPU_A03p.set((bus_addr_new >>  3) & 1);
    BUS_CPU_A04p.set((bus_addr_new >>  4) & 1);
    BUS_CPU_A05p.set((bus_addr_new >>  5) & 1);
    BUS_CPU_A06p.set((bus_addr_new >>  6) & 1);
    BUS_CPU_A07p.set((bus_addr_new >>  7) & 1);
    BUS_CPU_A08p.set((bus_addr_new >>  8) & 1);
    BUS_CPU_A09p.set((bus_addr_new >>  9) & 1);
    BUS_CPU_A10p.set((bus_addr_new >> 10) & 1);
    BUS_CPU_A11p.set((bus_addr_new >> 11) & 1);
    BUS_CPU_A12p.set((bus_addr_new >> 12) & 1);
    BUS_CPU_A13p.set((bus_addr_new >> 13) & 1);
    BUS_CPU_A14p.set((bus_addr_new >> 14) & 1);
    BUS_CPU_A15p.set((bus_addr_new >> 15) & 1);
  }

  //------------------------------------------------------------------------------------------------------------------------

  void set_data(int phase_total, Req bus_req_new) {
    wire bus_oe_new = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req_new.write;
    BUS_CPU_D00p.tri(bus_oe_new, (bus_req_new.data_lo >> 0) & 1);
    BUS_CPU_D01p.tri(bus_oe_new, (bus_req_new.data_lo >> 1) & 1);
    BUS_CPU_D02p.tri(bus_oe_new, (bus_req_new.data_lo >> 2) & 1);
    BUS_CPU_D03p.tri(bus_oe_new, (bus_req_new.data_lo >> 3) & 1);
    BUS_CPU_D04p.tri(bus_oe_new, (bus_req_new.data_lo >> 4) & 1);
    BUS_CPU_D05p.tri(bus_oe_new, (bus_req_new.data_lo >> 5) & 1);
    BUS_CPU_D06p.tri(bus_oe_new, (bus_req_new.data_lo >> 6) & 1);
    BUS_CPU_D07p.tri(bus_oe_new, (bus_req_new.data_lo >> 7) & 1);
  }


  /* p07.TUNA*/ wire TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A15p.qp_any(), BUS_CPU_A14p.qp_any(), BUS_CPU_A13p.qp_any(), BUS_CPU_A12p.qp_any(), BUS_CPU_A11p.qp_any(), BUS_CPU_A10p.qp_any(), BUS_CPU_A09p.qp_any()); }
  /* p07.RYCU*/ wire RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
  /* p25.SYRO*/ wire SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

  /* p03.TOVY*/ wire TOVY_A00n         () const { return not1(BUS_CPU_A00p.qp_any()); }
  /* p08.TOLA*/ wire TOLA_A01n         () const { return not1(BUS_CPU_A01p.qp_any()); }
  /* p22.XOLA*/ wire XOLA_A00n         () const { return not1(BUS_CPU_A00p.qp_any()); }
  /* p22.XENO*/ wire XENO_A01n         () const { return not1(BUS_CPU_A01p.qp_any()); }
  /* p22.XUSY*/ wire XUSY_A02n         () const { return not1(BUS_CPU_A02p.qp_any()); }
  /* p22.XERA*/ wire XERA_A03n         () const { return not1(BUS_CPU_A03p.qp_any()); }
  /* p07.TONA*/ wire TONA_A08n         () const { return not1(BUS_CPU_A08p.qp_any()); }
  /*#p08.SORE*/ wire SORE_A15n         () const { return not1(BUS_CPU_A15p.qp_any()); }
  /* p06.SEFY*/ wire SEFY_A02n         () const { return not1(BUS_CPU_A02p.qp_any()); }
  /* p08.SOGY*/ wire SOGY_A14n         () const { return not1(BUS_CPU_A14p.qp_any()); }

  /* p22.WADO*/ wire WADO_A00p         () const { return not1(XOLA_A00n()); }
  /* p22.WESA*/ wire WESA_A01p         () const { return not1(XENO_A01n()); }
  /* p22.WALO*/ wire WALO_A02p         () const { return not1(XUSY_A02n()); }
  /* p22.WEPO*/ wire WEPO_A03p         () const { return not1(XERA_A03n()); }

  /* p07.TYRO*/ wire TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A07p.qp_any(), BUS_CPU_A05p.qp_any(), BUS_CPU_A03p.qp_any(), BUS_CPU_A02p.qp_any(), BUS_CPU_A01p.qp_any(), BUS_CPU_A00p.qp_any()); }
  /* p07.TUFA*/ wire TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A04p.qp_any(), BUS_CPU_A06p.qp_any()); }

  /*#p07.SYKE*/ wire SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
  /*#p07.SOHA*/ wire SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

  /* p22.XALY*/ wire XALY_0x00xxxx     () const { return nor3(BUS_CPU_A07p.qp_any(), BUS_CPU_A05p.qp_any(), BUS_CPU_A04p.qp_any()); }
  /* p06.SARE*/ wire SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A07p.qp_any(), BUS_CPU_A06p.qp_any(), BUS_CPU_A05p.qp_any(), BUS_CPU_A04p.qp_any(), BUS_CPU_A03p.qp_any()); }
  /* p03.RYFO*/ wire RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A02p.qp_any(), SARE_XX00_XX07p()); }

  /* p07.SAPA*/ wire SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A00p.qp_any(), BUS_CPU_A01p.qp_any(), BUS_CPU_A02p.qp_any(), BUS_CPU_A03p.qp_any()); }
  /* p07.SEMY*/ wire SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A07p.qp_any(), BUS_CPU_A06p.qp_any(), BUS_CPU_A05p.qp_any(), BUS_CPU_A04p.qp_any()); }
  /* p06.SANO*/ wire SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
  /* p08.TUMA*/ wire TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A13p.qp_any(), SOGY_A14n(), BUS_CPU_A15p.qp_any()); }
  /* p08.TYNU*/ wire TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A15p.qp_any(), BUS_CPU_A14p.qp_any(), TUMA_A000_BFFFp()); }
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

  /* p07.TULO*/ wire TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A15p.qp_any(), BUS_CPU_A14p.qp_any(), BUS_CPU_A13p.qp_any(), BUS_CPU_A12p.qp_any(), BUS_CPU_A11p.qp_any(), BUS_CPU_A10p.qp_any(), BUS_CPU_A09p.qp_any(), BUS_CPU_A08p.qp_any()); }

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

  /* p22.WUTU*/ wire WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A06p.qp_any(), XALY_0x00xxxx()); }
  /* p22.WERO*/ wire WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A13p.qp_any(), BUS_CPU_A14p.qp_any(), SORE_A15n()); }

  //----------------------------------------

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

  //----------------------------------------

  /*BUS_SPR_I0*/ Bus BUS_SPR_I0; // AxCxExGx
  /*BUS_SPR_I1*/ Bus BUS_SPR_I1; // AxCxExGx
  /*BUS_SPR_I2*/ Bus BUS_SPR_I2; // AxCxExGx
  /*BUS_SPR_I3*/ Bus BUS_SPR_I3; // AxCxExGx
  /*BUS_SPR_I4*/ Bus BUS_SPR_I4; // AxCxExGx
  /*BUS_SPR_I5*/ Bus BUS_SPR_I5; // AxCxExGx

  /*BUS_SPR_L0*/ Bus BUS_SPR_L0; // AxCxExGx
  /*BUS_SPR_L1*/ Bus BUS_SPR_L1; // AxCxExGx
  /*BUS_SPR_L2*/ Bus BUS_SPR_L2; // AxCxExGx
  /*BUS_SPR_L3*/ Bus BUS_SPR_L3; // AxCxExGx

  //----------------------------------------

  /*BUS_OAM_A00n*/ Bus BUS_OAM_A00n;  // ABCDEFGH
  /*BUS_OAM_A01n*/ Bus BUS_OAM_A01n;  // ABCDEFGH
  /*BUS_OAM_A02n*/ Bus BUS_OAM_A02n;  // ABCDEFGH
  /*BUS_OAM_A03n*/ Bus BUS_OAM_A03n;  // ABCDEFGH
  /*BUS_OAM_A04n*/ Bus BUS_OAM_A04n;  // ABCDEFGH
  /*BUS_OAM_A05n*/ Bus BUS_OAM_A05n;  // ABCDEFGH
  /*BUS_OAM_A06n*/ Bus BUS_OAM_A06n;  // ABCDEFGH
  /*BUS_OAM_A07n*/ Bus BUS_OAM_A07n;  // ABCDEFGH

  /*BUS_OAM_DA00n*/ Bus BUS_OAM_DA00n;
  /*BUS_OAM_DA01n*/ Bus BUS_OAM_DA01n;
  /*BUS_OAM_DA02n*/ Bus BUS_OAM_DA02n;
  /*BUS_OAM_DA03n*/ Bus BUS_OAM_DA03n;
  /*BUS_OAM_DA04n*/ Bus BUS_OAM_DA04n;
  /*BUS_OAM_DA05n*/ Bus BUS_OAM_DA05n;
  /*BUS_OAM_DA06n*/ Bus BUS_OAM_DA06n;
  /*BUS_OAM_DA07n*/ Bus BUS_OAM_DA07n;

  /*BUS_OAM_DB00n*/ Bus BUS_OAM_DB00n;
  /*BUS_OAM_DB01n*/ Bus BUS_OAM_DB01n;
  /*BUS_OAM_DB02n*/ Bus BUS_OAM_DB02n;
  /*BUS_OAM_DB03n*/ Bus BUS_OAM_DB03n;
  /*BUS_OAM_DB04n*/ Bus BUS_OAM_DB04n;
  /*BUS_OAM_DB05n*/ Bus BUS_OAM_DB05n;
  /*BUS_OAM_DB06n*/ Bus BUS_OAM_DB06n;
  /*BUS_OAM_DB07n*/ Bus BUS_OAM_DB07n;

  //----------------------------------------

  /*BUS_VRAM_A00n*/ Bus BUS_VRAM_A00n;
  /*BUS_VRAM_A01n*/ Bus BUS_VRAM_A01n;
  /*BUS_VRAM_A02n*/ Bus BUS_VRAM_A02n;
  /*BUS_VRAM_A03n*/ Bus BUS_VRAM_A03n;
  /*BUS_VRAM_A04n*/ Bus BUS_VRAM_A04n;
  /*BUS_VRAM_A05n*/ Bus BUS_VRAM_A05n;
  /*BUS_VRAM_A06n*/ Bus BUS_VRAM_A06n;
  /*BUS_VRAM_A07n*/ Bus BUS_VRAM_A07n;
  /*BUS_VRAM_A08n*/ Bus BUS_VRAM_A08n;
  /*BUS_VRAM_A09n*/ Bus BUS_VRAM_A09n;
  /*BUS_VRAM_A10n*/ Bus BUS_VRAM_A10n;
  /*BUS_VRAM_A11n*/ Bus BUS_VRAM_A11n;
  /*BUS_VRAM_A12n*/ Bus BUS_VRAM_A12n;

  /*BUS_VRAM_D00p*/ Bus BUS_VRAM_D00p;
  /*BUS_VRAM_D01p*/ Bus BUS_VRAM_D01p;
  /*BUS_VRAM_D02p*/ Bus BUS_VRAM_D02p;
  /*BUS_VRAM_D03p*/ Bus BUS_VRAM_D03p;
  /*BUS_VRAM_D04p*/ Bus BUS_VRAM_D04p;
  /*BUS_VRAM_D05p*/ Bus BUS_VRAM_D05p;
  /*BUS_VRAM_D06p*/ Bus BUS_VRAM_D06p;
  /*BUS_VRAM_D07p*/ Bus BUS_VRAM_D07p;
};

