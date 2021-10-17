#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_bootrom_gates(const GateBoyState& reg_old) {
  GateBoyState& reg_new = gb_state;

  /*_p07.TUGE*/ wire TUGE_FF50_WRn = nand4(reg_old.cpu_signals.TAPU_CPU_WRp.out_old(), reg_old.cpu_abus.SYKE_ADDR_HIp(), reg_old.cpu_abus.TYRO_XX_0x0x0000p(), reg_old.cpu_abus.TUFA_XX_x1x1xxxxp());
  // FF50 - disable bootrom bit

  /*_p07.SATO*/ wire SATO_BOOT_BITn_old = or2(reg_old.cpu_dbus.BUS_CPU_D00p.out_old(), reg_old.cpu_signals.TEPU_BOOT_BITn.qp_old());
  /*_p07.TEPU*/ reg_new.cpu_signals.TEPU_BOOT_BITn.dff17(TUGE_FF50_WRn, reg_new.sys_rst.ALUR_SYS_RSTn(), SATO_BOOT_BITn_old);

  // BOOT -> CBD
  // this is kind of a hack
  auto cpu_addr_old = bit_pack(reg_old.cpu_abus);
  wire bootrom_data = DMG_ROM_blob[cpu_addr_old & 0xFF];

  /*_p07.TERA*/ wire TERA_BOOT_BITp  = not1(reg_new.cpu_signals.TEPU_BOOT_BITn.qp_new());
  /*_p07.TUTU*/ wire TUTU_READ_BOOTROMp = and2(TERA_BOOT_BITp, reg_old.cpu_abus.TULO_ADDR_BOOTROMp());
  /*_SIG_CPU_BOOTp*/ reg_new.cpu_signals.SIG_CPU_BOOTp.sig_out(TUTU_READ_BOOTROMp);

  /*_p07.ZORO*/ wire ZORO_0000xxxx_XX = nor4(reg_old.cpu_abus.BUS_CPU_A15p.out_old(), reg_old.cpu_abus.BUS_CPU_A14p.out_old(), reg_old.cpu_abus.BUS_CPU_A13p.out_old(), reg_old.cpu_abus.BUS_CPU_A12p.out_old());
  /*_p07.ZADU*/ wire ZADU_xxxx0000_XX = nor4(reg_old.cpu_abus.BUS_CPU_A11p.out_old(), reg_old.cpu_abus.BUS_CPU_A10p.out_old(), reg_old.cpu_abus.BUS_CPU_A09p.out_old(), reg_old.cpu_abus.BUS_CPU_A08p.out_old());
  /*_p07.ZUFA*/ wire ZUFA_0000_00FF  = and2(ZORO_0000xxxx_XX, ZADU_xxxx0000_XX);
  /*_p07.YAZA*/ wire YAZA_MODE_DBG1n = not1(pins.sys.UMUT_MODE_DBG1p());
  /*_p07.YULA*/ wire YULA_BOOT_RDp   = and3(reg_old.cpu_signals.TEDO_CPU_RDp.out_old(), YAZA_MODE_DBG1n, TUTU_READ_BOOTROMp); // def AND
  /*_p07.ZADO*/ wire ZADO_BOOT_CSn   = nand2(YULA_BOOT_RDp, ZUFA_0000_00FF);
  /*_p07.ZERY*/ wire ZERY_BOOT_CSp   = not1(ZADO_BOOT_CSn);

  /*_SIG_BOOT_CSp*/ reg_new.cpu_signals.SIG_BOOT_CSp.sig_out(ZERY_BOOT_CSp);

  triwire boot_d0 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 0));
  triwire boot_d1 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 1));
  triwire boot_d2 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 2));
  triwire boot_d3 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 3));
  triwire boot_d4 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 4));
  triwire boot_d5 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 5));
  triwire boot_d6 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 6));
  triwire boot_d7 = tri_pp(ZERY_BOOT_CSp, get_bit(bootrom_data, 7));

  reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(boot_d0);
  reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(boot_d1);
  reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(boot_d2);
  reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(boot_d3);
  reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(boot_d4);
  reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(boot_d5);
  reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(boot_d6);
  reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(boot_d7);

  /*_p07.TEXE*/ wire TEXE_FF50_RDp =  and4(reg_old.cpu_signals.TEDO_CPU_RDp.out_old(), reg_old.cpu_abus.SYKE_ADDR_HIp(), reg_old.cpu_abus.TYRO_XX_0x0x0000p(), reg_old.cpu_abus.TUFA_XX_x1x1xxxxp());
  /*_p07.SYPU*/ triwire SYPU_BOOT_TO_CD0 = tri6_pn(TEXE_FF50_RDp, reg_new.cpu_signals.TEPU_BOOT_BITn.qp_new());
  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(SYPU_BOOT_TO_CD0);
}

//-----------------------------------------------------------------------------

void GateBoyCpuSignals::reset_to_poweron() {
  //memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void GateBoyCpuSignals::reset_to_bootrom() {
  ABUZ_EXT_RAM_CS_CLK.state  = 0b00011000;
  SIG_IN_CPU_RDp.state       = 0b00011001;
  SIG_IN_CPU_WRp.state       = 0b00011000;
  SIG_IN_CPU_EXT_BUSp.state  = 0b00011000;
  SIG_IN_CPU_DBUS_FREE.state = 0b00011001;
  SIG_CPU_UNOR_DBG.state     = 0b00011000;
  SIG_CPU_ADDR_HIp.state     = 0b00011000;
  SIG_CPU_UMUT_DBG.state     = 0b00011000;
  SIG_CPU_BOOTp.state        = 0b00011001;
  SIG_BOOT_CSp.state         = 0b00011001;
  TEPU_BOOT_BITn.state       = 0b00011010;
  TEDO_CPU_RDp.state         = 0b00011001;
  APOV_CPU_WRp.state         = 0b00011000;
  TAPU_CPU_WRp.state         = 0b00011000;
}

void GateBoyCpuSignals::reset_to_cart() {
  ABUZ_EXT_RAM_CS_CLK.state  = 0b00011000;
  SIG_IN_CPU_RDp.state       = 0b00011000;
  SIG_IN_CPU_WRp.state       = 0b00011001;
  SIG_IN_CPU_EXT_BUSp.state  = 0b00011000;
  SIG_IN_CPU_DBUS_FREE.state = 0b00011000;
  SIG_CPU_UNOR_DBG.state     = 0b00011000;
  SIG_CPU_ADDR_HIp.state     = 0b00011001;
  SIG_CPU_UMUT_DBG.state     = 0b00011000;
  SIG_CPU_BOOTp.state        = 0b00011000;
  SIG_BOOT_CSp.state         = 0b00011000;
  TEPU_BOOT_BITn.state       = 0b00011011;
  TEDO_CPU_RDp.state         = 0b00011000;
  APOV_CPU_WRp.state         = 0b00011000;
  TAPU_CPU_WRp.state         = 0b00011000;
}

/*_p07.AJAS*/ wire GateBoyCpuSignals::AJAS_CPU_RDn      () const { return not1(TEDO_CPU_RDp.out_new()); }
/*_p07.DYKY*/ wire GateBoyCpuSignals::DYKY_CPU_WRn      () const { return not1(TAPU_CPU_WRp.out_new()); }

/*_p07.ASOT*/ wire GateBoyCpuSignals::ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
/*_p07.CUPA*/ wire GateBoyCpuSignals::CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
/*_p28.MYNU*/ wire GateBoyCpuSignals::MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
/*_p28.LEKO*/ wire GateBoyCpuSignals::LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
/*_p08.REDU*/ wire GateBoyCpuSignals::REDU_CPU_RDn      () const { return not1(TEDO_CPU_RDp.out_new()); }
/*_p08.MEXO*/ wire GateBoyCpuSignals::MEXO_CPU_WRn      () const { return not1(APOV_CPU_WRp.out_new()); }

/*_p04.DECY*/ wire GateBoyCpuSignals::DECY_LATCH_EXTn   () const { return not1(SIG_IN_CPU_DBUS_FREE.out_new()); }
/*_p04.CATY*/ wire GateBoyCpuSignals::CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
/*#p28.BOFE*/ wire GateBoyCpuSignals::BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

//-----------------------------------------------------------------------------

void GateBoyCpuABus::reset_to_poweron() {
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

void GateBoyCpuABus::reset_to_bootrom() {
  BUS_CPU_A00p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A01p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A02p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A03p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A04p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A05p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A06p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A07p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A08p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A09p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A10p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A11p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A12p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A13p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A14p.state = BIT_OLD | BIT_DRIVEN;
  BUS_CPU_A15p.state = BIT_OLD | BIT_DRIVEN;
}

void GateBoyCpuABus::reset_to_cart() {
  BUS_CPU_A00p.state = 0b00011000;
  BUS_CPU_A01p.state = 0b00011000;
  BUS_CPU_A02p.state = 0b00011000;
  BUS_CPU_A03p.state = 0b00011000;
  BUS_CPU_A04p.state = 0b00011001;
  BUS_CPU_A05p.state = 0b00011000;
  BUS_CPU_A06p.state = 0b00011001;
  BUS_CPU_A07p.state = 0b00011000;
  BUS_CPU_A08p.state = 0b00011001;
  BUS_CPU_A09p.state = 0b00011001;
  BUS_CPU_A10p.state = 0b00011001;
  BUS_CPU_A11p.state = 0b00011001;
  BUS_CPU_A12p.state = 0b00011001;
  BUS_CPU_A13p.state = 0b00011001;
  BUS_CPU_A14p.state = 0b00011001;
  BUS_CPU_A15p.state = 0b00011001;
}

void GateBoyCpuABus::set_addr(uint16_t bus_addr_new)
{
  triwire EXT_bus_addr00 = tri_pp(1, get_bit(bus_addr_new, 0));
  triwire EXT_bus_addr01 = tri_pp(1, get_bit(bus_addr_new, 1));
  triwire EXT_bus_addr02 = tri_pp(1, get_bit(bus_addr_new, 2));
  triwire EXT_bus_addr03 = tri_pp(1, get_bit(bus_addr_new, 3));
  triwire EXT_bus_addr04 = tri_pp(1, get_bit(bus_addr_new, 4));
  triwire EXT_bus_addr05 = tri_pp(1, get_bit(bus_addr_new, 5));
  triwire EXT_bus_addr06 = tri_pp(1, get_bit(bus_addr_new, 6));
  triwire EXT_bus_addr07 = tri_pp(1, get_bit(bus_addr_new, 7));
  triwire EXT_bus_addr08 = tri_pp(1, get_bit(bus_addr_new, 8));
  triwire EXT_bus_addr09 = tri_pp(1, get_bit(bus_addr_new, 9));
  triwire EXT_bus_addr10 = tri_pp(1, get_bit(bus_addr_new, 10));
  triwire EXT_bus_addr11 = tri_pp(1, get_bit(bus_addr_new, 11));
  triwire EXT_bus_addr12 = tri_pp(1, get_bit(bus_addr_new, 12));
  triwire EXT_bus_addr13 = tri_pp(1, get_bit(bus_addr_new, 13));
  triwire EXT_bus_addr14 = tri_pp(1, get_bit(bus_addr_new, 14));
  triwire EXT_bus_addr15 = tri_pp(1, get_bit(bus_addr_new, 15));

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

/*_p07.TUNA*/ wire GateBoyCpuABus::TUNA_0000_FDFF    () const { return nand7(BUS_CPU_A15p.out_new(), BUS_CPU_A14p.out_new(), BUS_CPU_A13p.out_new(), BUS_CPU_A12p.out_new(), BUS_CPU_A11p.out_new(), BUS_CPU_A10p.out_new(), BUS_CPU_A09p.out_new()); }
/*_p07.RYCU*/ wire GateBoyCpuABus::RYCU_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }
/*_p25.SYRO*/ wire GateBoyCpuABus::SYRO_FE00_FFFF    () const { return not1(TUNA_0000_FDFF()); }

/*_p03.TOVY*/ wire GateBoyCpuABus::TOVY_A00n         () const { return not1(BUS_CPU_A00p.out_new()); }
/*_p08.TOLA*/ wire GateBoyCpuABus::TOLA_A01n         () const { return not1(BUS_CPU_A01p.out_new()); }
/*_p22.XOLA*/ wire GateBoyCpuABus::XOLA_A00n         () const { return not1(BUS_CPU_A00p.out_new()); }
/*_p22.XENO*/ wire GateBoyCpuABus::XENO_A01n         () const { return not1(BUS_CPU_A01p.out_new()); }
/*_p22.XUSY*/ wire GateBoyCpuABus::XUSY_A02n         () const { return not1(BUS_CPU_A02p.out_new()); }
/*_p22.XERA*/ wire GateBoyCpuABus::XERA_A03n         () const { return not1(BUS_CPU_A03p.out_new()); }
/*_p07.TONA*/ wire GateBoyCpuABus::TONA_A08n         () const { return not1(BUS_CPU_A08p.out_new()); }
/*#p08.SORE*/ wire GateBoyCpuABus::SORE_A15n         () const { return not1(BUS_CPU_A15p.out_new()); }
/*_p06.SEFY*/ wire GateBoyCpuABus::SEFY_A02n         () const { return not1(BUS_CPU_A02p.out_new()); }
/*_p08.SOGY*/ wire GateBoyCpuABus::SOGY_A14n         () const { return not1(BUS_CPU_A14p.out_new()); }

/*_p22.WADO*/ wire GateBoyCpuABus::WADO_A00p         () const { return not1(XOLA_A00n()); }
/*_p22.WESA*/ wire GateBoyCpuABus::WESA_A01p         () const { return not1(XENO_A01n()); }
/*_p22.WALO*/ wire GateBoyCpuABus::WALO_A02p         () const { return not1(XUSY_A02n()); }
/*_p22.WEPO*/ wire GateBoyCpuABus::WEPO_A03p         () const { return not1(XERA_A03n()); }

/*_p07.TYRO*/ wire GateBoyCpuABus::TYRO_XX_0x0x0000p () const { return nor6(BUS_CPU_A07p.out_new(), BUS_CPU_A05p.out_new(), BUS_CPU_A03p.out_new(), BUS_CPU_A02p.out_new(), BUS_CPU_A01p.out_new(), BUS_CPU_A00p.out_new()); }
/*_p07.TUFA*/ wire GateBoyCpuABus::TUFA_XX_x1x1xxxxp () const { return and2(BUS_CPU_A04p.out_new(), BUS_CPU_A06p.out_new()); }

/*#p07.SYKE*/ wire GateBoyCpuABus::SYKE_ADDR_HIp     () const { return nor2(TUNA_0000_FDFF(), TONA_A08n()); }
/*#p07.SOHA*/ wire GateBoyCpuABus::SOHA_ADDR_HIn     () const { return not1(SYKE_ADDR_HIp()); }

/*_p22.XALY*/ wire GateBoyCpuABus::XALY_0x00xxxx     () const { return nor3(BUS_CPU_A07p.out_new(), BUS_CPU_A05p.out_new(), BUS_CPU_A04p.out_new()); }
/*_p06.SARE*/ wire GateBoyCpuABus::SARE_XX00_XX07p   () const { return nor5(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BUS_CPU_A05p.out_new(), BUS_CPU_A04p.out_new(), BUS_CPU_A03p.out_new()); }
/*_p03.RYFO*/ wire GateBoyCpuABus::RYFO_FF04_FF07p   () const { return and3(SYKE_ADDR_HIp(), BUS_CPU_A02p.out_new(), SARE_XX00_XX07p()); }

/*_p07.SAPA*/ wire GateBoyCpuABus::SAPA_XX_xxxx1111p () const { return and4(BUS_CPU_A00p.out_new(), BUS_CPU_A01p.out_new(), BUS_CPU_A02p.out_new(), BUS_CPU_A03p.out_new()); }
/*_p07.SEMY*/ wire GateBoyCpuABus::SEMY_XX_0000xxxxp () const { return nor4(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BUS_CPU_A05p.out_new(), BUS_CPU_A04p.out_new()); }
/*_p06.SANO*/ wire GateBoyCpuABus::SANO_FF00_FF03p   () const { return and3(SARE_XX00_XX07p(), SEFY_A02n(), SYKE_ADDR_HIp()); }
/*_p08.TUMA*/ wire GateBoyCpuABus::TUMA_A000_BFFFp   () const { return and3(BUS_CPU_A13p.out_new(), SOGY_A14n(), BUS_CPU_A15p.out_new()); }
/*_p08.TYNU*/ wire GateBoyCpuABus::TYNU_A000_FFFFp   () const { return and_or3(BUS_CPU_A15p.out_new(), BUS_CPU_A14p.out_new(), TUMA_A000_BFFFp()); }
/*_p28.ADAH*/ wire GateBoyCpuABus::ADAH_FE00_FEFFn   () const { return not1(SARO_ADDR_OAMp()); }

/*_p07.TULO*/ wire GateBoyCpuABus::TULO_ADDR_BOOTROMp() const { return nor8(BUS_CPU_A15p.out_new(), BUS_CPU_A14p.out_new(), BUS_CPU_A13p.out_new(), BUS_CPU_A12p.out_new(), BUS_CPU_A11p.out_new(), BUS_CPU_A10p.out_new(), BUS_CPU_A09p.out_new(), BUS_CPU_A08p.out_new()); }

/*#p07.ROPE*/ wire GateBoyCpuABus::ROPE_ADDR_OAMn    () const { return nand2(SOHA_ADDR_HIn(), RYCU_FE00_FFFF()); } // looks like nand?
/*#p07.SARO*/ wire GateBoyCpuABus::SARO_ADDR_OAMp    () const { return not1(ROPE_ADDR_OAMn()); }

/*_p22.WORU*/ wire GateBoyCpuABus::WORU_FF40n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
/*_p22.WOFA*/ wire GateBoyCpuABus::WOFA_FF41n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), XERA_A03n()); }
/*_p22.WEBU*/ wire GateBoyCpuABus::WEBU_FF42n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
/*_p22.WAVU*/ wire GateBoyCpuABus::WAVU_FF43n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), XERA_A03n()); }
/*_p22.WYLE*/ wire GateBoyCpuABus::WYLE_FF44n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
/*_p22.WETY*/ wire GateBoyCpuABus::WETY_FF45n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), WALO_A02p(), XERA_A03n()); }
/*#p22.WATE*/ wire GateBoyCpuABus::WATE_FF46n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
/*_p22.WYBO*/ wire GateBoyCpuABus::WYBO_FF47n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), WALO_A02p(), XERA_A03n()); }
/*_p22.WETA*/ wire GateBoyCpuABus::WETA_FF48n        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
/*_p22.VAMA*/ wire GateBoyCpuABus::VAMA_FF49n        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), XENO_A01n(), XUSY_A02n(), WEPO_A03p()); }
/*_p22.WYVO*/ wire GateBoyCpuABus::WYVO_FF4An        () const { return nand5(WERO_ADDR_PPUp(), XOLA_A00n(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }
/*_p22.WAGE*/ wire GateBoyCpuABus::WAGE_FF4Bn        () const { return nand5(WERO_ADDR_PPUp(), WADO_A00p(), WESA_A01p(), XUSY_A02n(), WEPO_A03p()); }

/*_p22.VOCA*/ wire GateBoyCpuABus::VOCA_FF40p        () const { return not1(WORU_FF40n()); }
/*_p22.VARY*/ wire GateBoyCpuABus::VARY_FF41p        () const { return not1(WOFA_FF41n()); }
/*_p22.XARO*/ wire GateBoyCpuABus::XARO_FF42p        () const { return not1(WEBU_FF42n()); }
/*_p22.XAVY*/ wire GateBoyCpuABus::XAVY_FF43p        () const { return not1(WAVU_FF43n()); }
/*_p22.XOGY*/ wire GateBoyCpuABus::XOGY_FF44p        () const { return not1(WYLE_FF44n()); }
/*_p22.XAYU*/ wire GateBoyCpuABus::XAYU_FF45p        () const { return not1(WETY_FF45n()); }
/*#p22.XEDA*/ wire GateBoyCpuABus::XEDA_FF46p        () const { return not1(WATE_FF46n()); }
/*_p22.WERA*/ wire GateBoyCpuABus::WERA_FF47p        () const { return not1(WYBO_FF47n()); }
/*_p22.XAYO*/ wire GateBoyCpuABus::XAYO_FF48p        () const { return not1(WETA_FF48n()); }
/*_p22.TEGO*/ wire GateBoyCpuABus::TEGO_FF49p        () const { return not1(VAMA_FF49n()); }
/*_p22.VYGA*/ wire GateBoyCpuABus::VYGA_FF4Ap        () const { return not1(WYVO_FF4An()); }
/*_p22.VUMY*/ wire GateBoyCpuABus::VUMY_FF4Bp        () const { return not1(WAGE_FF4Bn()); }

/*_p22.WUTU*/ wire GateBoyCpuABus::WUTU_ADDR_PPUn    () const { return nand3(SYKE_ADDR_HIp(), BUS_CPU_A06p.out_new(), XALY_0x00xxxx()); }
/*_p22.WERO*/ wire GateBoyCpuABus::WERO_ADDR_PPUp    () const { return not1(WUTU_ADDR_PPUn()); }

/*#p08.TEVY*/ wire GateBoyCpuABus::TEVY_ADDR_VRAMn   () const { return or3(BUS_CPU_A13p.out_new(), BUS_CPU_A14p.out_new(), SORE_A15n()); }


//-----------------------------------------------------------------------------

void GateBoyCpuDBus::reset_to_poweron() {
  BUS_CPU_D00p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D01p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D02p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D03p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D04p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D05p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D06p.state = BIT_OLD | BIT_PULLED | 1;
  BUS_CPU_D07p.state = BIT_OLD | BIT_PULLED | 1;
}

void GateBoyCpuDBus::reset_to_bootrom() {
  BUS_CPU_D00p.state = 0b00011001;
  BUS_CPU_D01p.state = 0b00011000;
  BUS_CPU_D02p.state = 0b00011000;
  BUS_CPU_D03p.state = 0b00011000;
  BUS_CPU_D04p.state = 0b00011001;
  BUS_CPU_D05p.state = 0b00011001;
  BUS_CPU_D06p.state = 0b00011000;
  BUS_CPU_D07p.state = 0b00011000;
}

void GateBoyCpuDBus::reset_to_cart() {
  BUS_CPU_D00p.state = 0b00011001;
  BUS_CPU_D01p.state = 0b00011000;
  BUS_CPU_D02p.state = 0b00011000;
  BUS_CPU_D03p.state = 0b00011000;
  BUS_CPU_D04p.state = 0b00011000;
  BUS_CPU_D05p.state = 0b00011000;
  BUS_CPU_D06p.state = 0b00011000;
  BUS_CPU_D07p.state = 0b00011000;
}

void GateBoyCpuDBus::set_data(bool OEp, uint8_t data) {
  triwire tri0 = tri_pp(OEp, get_bit(data, 0));
  triwire tri1 = tri_pp(OEp, get_bit(data, 1));
  triwire tri2 = tri_pp(OEp, get_bit(data, 2));
  triwire tri3 = tri_pp(OEp, get_bit(data, 3));
  triwire tri4 = tri_pp(OEp, get_bit(data, 4));
  triwire tri5 = tri_pp(OEp, get_bit(data, 5));
  triwire tri6 = tri_pp(OEp, get_bit(data, 6));
  triwire tri7 = tri_pp(OEp, get_bit(data, 7));

  BUS_CPU_D00p.tri_bus(tri0);
  BUS_CPU_D01p.tri_bus(tri1);
  BUS_CPU_D02p.tri_bus(tri2);
  BUS_CPU_D03p.tri_bus(tri3);
  BUS_CPU_D04p.tri_bus(tri4);
  BUS_CPU_D05p.tri_bus(tri5);
  BUS_CPU_D06p.tri_bus(tri6);
  BUS_CPU_D07p.tri_bus(tri7);
}

//-----------------------------------------------------------------------------
