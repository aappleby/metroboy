#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyCpuSignals {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  wire AJAS_CPU_RDn_old      () const;
  wire DYKY_CPU_WRn_old      () const;

  wire ASOT_CPU_RDp_old      () const;
  wire CUPA_CPU_WRp_old      () const;
  wire MYNU_CPU_RDn_old      () const;
  wire LEKO_CPU_RDp_old      () const;
  wire REDU_CPU_RDn_old      () const;
  wire MEXO_CPU_WRn_old      () const;

  wire DECY_LATCH_EXTn_old   () const;
  wire CATY_LATCH_EXTp_old   () const;
  wire BOFE_LATCH_EXTn_old   () const;

  wire AJAS_CPU_RDn_new      () const;
  wire DYKY_CPU_WRn_new      () const;

  wire ASOT_CPU_RDp_new      () const;
  wire CUPA_CPU_WRp_new      () const;
  wire MYNU_CPU_RDn_new      () const;
  wire LEKO_CPU_RDp_new      () const;
  wire REDU_CPU_RDn_new      () const;
  wire MEXO_CPU_WRn_new      () const;

  wire DECY_LATCH_EXTn_new   () const;
  wire CATY_LATCH_EXTp_new   () const;
  wire BOFE_LATCH_EXTn_new   () const;

  //----------------------------------------

  Gate ABUZ_EXT_RAM_CS_CLK;

  /*_SIG_IN_CPU_RDp     */  SigIn  SIG_IN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*_SIG_IN_CPU_WRp     */  SigIn  SIG_IN_CPU_WRp;           // top right port PORTA_01: ->
  /*_SIG_IN_CPU_EXT_BUSp*/  SigIn  SIG_IN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  /*_SIG_IN_CPU_LATCH_EXT*/ SigIn  SIG_IN_CPU_DBUS_FREE;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

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

//-----------------------------------------------------------------------------

struct GateBoyCpuABus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();
  void set_addr(uint16_t bus_addr_new);

  //----------

  wire TOVY_A00n_old         () const;
  wire TOLA_A01n_old         () const;
  wire XOLA_A00n_old         () const;
  wire XENO_A01n_old         () const;
  wire XUSY_A02n_old         () const;
  wire XERA_A03n_old         () const;
  wire TONA_A08n_old         () const;
  wire SORE_A15n_old         () const;
  wire SEFY_A02n_old         () const;
  wire SOGY_A14n_old         () const;

  wire WADO_A00p_old         () const;
  wire WESA_A01p_old         () const;
  wire WALO_A02p_old         () const;
  wire WEPO_A03p_old         () const;

  wire WUTU_ADDR_PPUn_old    () const;
  wire WERO_ADDR_PPUp_old    () const;

  wire TUNA_0000_FDFF_old    () const;
  wire SYKE_ADDR_HIp_old     () const;

  wire XALY_0x00xxxx_old     () const;
  wire SARE_XX00_XX07p_old   () const;
  wire RYFO_FF04_FF07p_old   () const;

  wire WATE_FF46n_old        () const;
  wire XEDA_FF46p_old        () const;

  //----------

  wire TUNA_0000_FDFF_new    () const;
  wire RYCU_FE00_FFFF_new    () const;
  wire SYRO_FE00_FFFF_new    () const;

  wire TOVY_A00n_new         () const;
  wire TOLA_A01n_new         () const;
  wire XOLA_A00n_new         () const;
  wire XENO_A01n_new         () const;
  wire XUSY_A02n_new         () const;
  wire XERA_A03n_new         () const;
  wire TONA_A08n_new         () const;
  wire SORE_A15n_new         () const;
  wire SEFY_A02n_new         () const;
  wire SOGY_A14n_new         () const;

  wire WADO_A00p_new         () const;
  wire WESA_A01p_new         () const;
  wire WALO_A02p_new         () const;
  wire WEPO_A03p_new         () const;

  wire TYRO_XX_0x0x0000p_new () const;
  wire TUFA_XX_x1x1xxxxp_new () const;

  wire SYKE_ADDR_HIp_new     () const;
  wire SOHA_ADDR_HIn_new     () const;

  wire XALY_0x00xxxx_new     () const;
  wire SARE_XX00_XX07p_new   () const;
  wire RYFO_FF04_FF07p_new   () const;

  wire SAPA_XX_xxxx1111p_new () const;
  wire SEMY_XX_0000xxxxp_new () const;
  wire SANO_FF00_FF03p_new   () const;
  wire TUMA_A000_BFFFp_new   () const;
  wire TYNU_A000_FFFFp_new   () const;
  wire ADAH_FE00_FEFFn_new   () const;

  wire TULO_ADDR_BOOTROMp_new() const;

  wire ROPE_ADDR_OAMn_new    () const;
  wire SARO_ADDR_OAMp_new    () const;

  wire WORU_FF40n_new        () const;
  wire WOFA_FF41n_new        () const;
  wire WEBU_FF42n_new        () const;
  wire WAVU_FF43n_new        () const;
  wire WYLE_FF44n_new        () const;
  wire WETY_FF45n_new        () const;
  wire WATE_FF46n_new        () const;
  wire WYBO_FF47n_new        () const;
  wire WETA_FF48n_new        () const;
  wire VAMA_FF49n_new        () const;
  wire WYVO_FF4An_new        () const;
  wire WAGE_FF4Bn_new        () const;

  wire VOCA_FF40p_new        () const;
  wire VARY_FF41p_new        () const;
  wire XARO_FF42p_new        () const;
  wire XAVY_FF43p_new        () const;
  wire XOGY_FF44p_new        () const;
  wire XAYU_FF45p_new        () const;
  wire XEDA_FF46p_new        () const;
  wire WERA_FF47p_new        () const;
  wire XAYO_FF48p_new        () const;
  wire TEGO_FF49p_new        () const;
  wire VYGA_FF4Ap_new        () const;
  wire VUMY_FF4Bp_new        () const;

  wire WUTU_ADDR_PPUn_new    () const;
  wire WERO_ADDR_PPUp_new    () const;

  wire TEVY_ADDR_VRAMn_new   () const;

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
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();
  void set_data(bool OEp, uint8_t data);

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
