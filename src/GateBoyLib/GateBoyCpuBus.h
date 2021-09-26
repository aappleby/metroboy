#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyCpuSignals {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  wire AJAS_CPU_RDn      () const;
  wire DYKY_CPU_WRn      () const;

  wire ASOT_CPU_RDp      () const;
  wire CUPA_CPU_WRp      () const;
  wire MYNU_CPU_RDn      () const;
  wire LEKO_CPU_RDp      () const;
  wire REDU_CPU_RDn      () const;
  wire MEXO_CPU_WRn      () const;

  wire DECY_LATCH_EXTn   () const;
  wire CATY_LATCH_EXTp   () const;
  wire BOFE_LATCH_EXTn   () const;

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

  wire TUNA_0000_FDFF    () const;
  wire RYCU_FE00_FFFF    () const;
  wire SYRO_FE00_FFFF    () const;

  wire TOVY_A00n         () const;
  wire TOLA_A01n         () const;
  wire XOLA_A00n         () const;
  wire XENO_A01n         () const;
  wire XUSY_A02n         () const;
  wire XERA_A03n         () const;
  wire TONA_A08n         () const;
  wire SORE_A15n         () const;
  wire SEFY_A02n         () const;
  wire SOGY_A14n         () const;

  wire WADO_A00p         () const;
  wire WESA_A01p         () const;
  wire WALO_A02p         () const;
  wire WEPO_A03p         () const;

  wire TYRO_XX_0x0x0000p () const;
  wire TUFA_XX_x1x1xxxxp () const;

  wire SYKE_ADDR_HIp     () const;
  wire SOHA_ADDR_HIn     () const;

  wire XALY_0x00xxxx     () const;
  wire SARE_XX00_XX07p   () const;
  wire RYFO_FF04_FF07p   () const;

  wire SAPA_XX_xxxx1111p () const;
  wire SEMY_XX_0000xxxxp () const;
  wire SANO_FF00_FF03p   () const;
  wire TUMA_A000_BFFFp   () const;
  wire TYNU_A000_FFFFp   () const;
  wire ADAH_FE00_FEFFn   () const;

  wire TULO_ADDR_BOOTROMp() const;

  wire ROPE_ADDR_OAMn    () const;
  wire SARO_ADDR_OAMp    () const;

  wire WORU_FF40n        () const;
  wire WOFA_FF41n        () const;
  wire WEBU_FF42n        () const;
  wire WAVU_FF43n        () const;
  wire WYLE_FF44n        () const;
  wire WETY_FF45n        () const;
  wire WATE_FF46n        () const;
  wire WYBO_FF47n        () const;
  wire WETA_FF48n        () const;
  wire VAMA_FF49n        () const;
  wire WYVO_FF4An        () const;
  wire WAGE_FF4Bn        () const;

  wire VOCA_FF40p        () const;
  wire VARY_FF41p        () const;
  wire XARO_FF42p        () const;
  wire XAVY_FF43p        () const;
  wire XOGY_FF44p        () const;
  wire XAYU_FF45p        () const;
  wire XEDA_FF46p        () const;
  wire WERA_FF47p        () const;
  wire XAYO_FF48p        () const;
  wire TEGO_FF49p        () const;
  wire VYGA_FF4Ap        () const;
  wire VUMY_FF4Bp        () const;

  wire WUTU_ADDR_PPUn    () const;
  wire WERO_ADDR_PPUp    () const;

  wire TEVY_ADDR_VRAMn   () const;

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
