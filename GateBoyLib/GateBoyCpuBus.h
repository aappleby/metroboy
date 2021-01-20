#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyBuses.h"

struct GateBoyCpuBus {
  void reset_to_bootrom();
  void reset_to_cart();

  void dump(Dumper& d) {
    d.dump_bitp   ("SIG_CPU_RDp       : ", SIG_CPU_RDp.get_state());
    d.dump_bitp   ("SIG_CPU_WRp       : ", SIG_CPU_WRp.get_state());
    d.dump_bitp   ("SIG_CPU_UNOR_DBG  : ", SIG_CPU_UNOR_DBG.get_state());
    d.dump_bitp   ("SIG_CPU_ADDR_HIp  : ", SIG_CPU_ADDR_HIp.get_state());
    d.dump_bitp   ("SIG_CPU_UMUT_DBG  : ", SIG_CPU_UMUT_DBG.get_state());
    d.dump_bitp   ("SIG_CPU_EXT_BUSp  : ", SIG_CPU_EXT_BUSp.get_state());
    //d.dump_bitp   ("SIG_CPU_6         : ", SIG_CPU_6.state);
    d.dump_bitp   ("SIG_CPU_LATCH_EXT : ", SIG_CPU_LATCH_EXT.get_state());
    d.dump_bitp   ("BOOT_BITn         : ", TEPU_BOOT_BITn_h.state);
    d.dump_bitp   ("SIG_CPU_BOOTp     : ", SIG_CPU_BOOTp.get_state());
    d.dump_bitp   ("TEDO_CPU_RDp      : ", TEDO_CPU_RDp.get_state());
    d.dump_bitp   ("APOV_CPU_WRp      : ", APOV_CPU_WRp.get_state());
    d.dump_bitp   ("TAPU_CPU_WRp      : ", TAPU_CPU_WRp.get_state());
    //d.dump_slice2p("BUS_CPU_A : ", &new_bus.BUS_CPU_A00p, 16);
    //d.dump_slice2p("BUS_CPU_D : ", &new_bus.BUS_CPU_D00p, 8);
  }

  Gate ABUZ_EXT_RAM_CS_CLK;

  /*SIG_CPU_RDp     */  SigIn  SIG_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*SIG_CPU_WRp     */  SigIn  SIG_CPU_WRp;           // top right port PORTA_01: ->
  /*SIG_CPU_UNOR_DBG*/  SigOut SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  /*SIG_CPU_ADDR_HIp*/  SigOut SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  /*SIG_CPU_UMUT_DBG*/  SigOut SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  /*SIG_CPU_EXT_BUSp*/  SigIn  SIG_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  ///*SIG_CPU_6*/         SigOut SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  /*SIG_CPU_LATCH_EXT*/ SigIn  SIG_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  /*SIG_CPU_BOOTp*/     SigOut SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  /*SIG_BOOT_CSp*/      SigOut SIG_BOOT_CSp;          // cs for bootrom

  /* p07.TEPU*/ DFF17 TEPU_BOOT_BITn_h;

  Gate TEDO_CPU_RDp;
  Gate APOV_CPU_WRp;
  Gate TAPU_CPU_WRp;

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(TEDO_CPU_RDp); }
  /* p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(TAPU_CPU_WRp); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(TEDO_CPU_RDp); }
  /* p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(APOV_CPU_WRp); }


  /* p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(SIG_CPU_LATCH_EXT); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /* p07.TERA*/ wire TERA_BOOT_BITp    () const { return not1(TEPU_BOOT_BITn_h.qp_new()); }
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp(const GateBoyBuses& new_bus) const { return and2(TERA_BOOT_BITp(), new_bus.TULO_ADDR_BOOTROMp()); }

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   (const GateBoyBuses& new_bus) const { return and2(SIG_CPU_EXT_BUSp, new_bus.TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   (const GateBoyBuses& new_bus) const { return nor2(new_bus.SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn(new_bus)); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   (const GateBoyBuses& new_bus) const { return and2(TEFA_ADDR_VRAMp(new_bus), new_bus.BUS_CPU_A15p.qp_new()); }
  /* p08.LEVO*/ wire LEVO_ADDR_VRAMn   (const GateBoyBuses& new_bus) const { return not1(TEXO_ADDR_VRAMn(new_bus)); }
  /* p25.TUJA*/ wire TUJA_CPU_VRAM_WRp (const GateBoyBuses& new_bus) const { return and2(SOSE_ADDR_VRAMp(new_bus), APOV_CPU_WRp); }

  wire TOLE_CPU_VRAM_RDp(GateBoyBuses& new_bus)
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(new_bus), ABUZ_EXT_RAM_CS_CLK);
    /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = not1(_TUCA_CPU_VRAM_RDp);

    return _TOLE_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn(GateBoyBuses& new_bus)
  {
    // Ignoring debug for now
    ///*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);

    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(new_bus), SIG_CPU_WRp);  // Schematic wrong, second input is SIG_CPU_WRp
    /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = not1(_TEGU_CPU_VRAM_WRn);

    return _SALE_CPU_VRAM_WRn;
  }
};