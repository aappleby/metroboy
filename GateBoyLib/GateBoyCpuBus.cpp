#include "GateBoyLib/GateBoyCpuBus.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyCpuSignals::reset_to_bootrom() {
}

void GateBoyCpuSignals::reset_to_cart() {
  TEPU_BOOT_BITn_h.state = 0b00011011;
  SIG_CPU_BOOTp.state = 0b00011000;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_bootrom() {
  /* p07.TUGE*/ wire TUGE_FF50_WRn = nand4(cpu_signals.TAPU_CPU_WRp.out_new(), new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  // FF50 - disable bootrom bit

  /* p07.TEPU*/ cpu_signals.TEPU_BOOT_BITn_h.dff17(TUGE_FF50_WRn, ALUR_SYS_RSTn(), SATO_BOOT_BITn.out_old());

  // BOOT -> CBD
  // this is kind of a hack
  uint16_t cpu_addr = (uint16_t)pack_new(16, (BitBase*)&new_bus.BUS_CPU_A00p);
  wire bootrom_data = boot_buf[cpu_addr & 0xFF];

  /* p07.TERA*/ wire TERA_BOOT_BITp  = not1(cpu_signals.TEPU_BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp = and2(TERA_BOOT_BITp, new_bus.TULO_ADDR_BOOTROMp());
  /*SIG_CPU_BOOTp*/ cpu_signals.SIG_CPU_BOOTp.sig_out(TUTU_READ_BOOTROMp);

  /* p07.ZORO*/ wire ZORO_0000xxxx_XX = nor4(new_bus.BUS_CPU_A15p.out_new(), new_bus.BUS_CPU_A14p.out_new(), new_bus.BUS_CPU_A13p.out_new(), new_bus.BUS_CPU_A12p.out_new());
  /* p07.ZADU*/ wire ZADU_xxxx0000_XX = nor4(new_bus.BUS_CPU_A11p.out_new(), new_bus.BUS_CPU_A10p.out_new(), new_bus.BUS_CPU_A09p.out_new(), new_bus.BUS_CPU_A08p.out_new());
  /* p07.ZUFA*/ wire ZUFA_0000_00FF  = and2(ZORO_0000xxxx_XX, ZADU_xxxx0000_XX);
  /* p07.YAZA*/ wire YAZA_MODE_DBG1n = not1(UMUT_MODE_DBG1p());
  /* p07.YULA*/ wire YULA_BOOT_RDp   = and3(cpu_signals.TEDO_CPU_RDp.out_new(), YAZA_MODE_DBG1n, TUTU_READ_BOOTROMp); // def AND
  /* p07.ZADO*/ wire ZADO_BOOT_CSn   = nand2(YULA_BOOT_RDp, ZUFA_0000_00FF);
  /* p07.ZERY*/ wire ZERY_BOOT_CSp   = not1(ZADO_BOOT_CSn);

  /*SIG_BOOT_CSp*/ cpu_signals.SIG_BOOT_CSp.sig_out(ZERY_BOOT_CSp);

  wire boot_d0 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 0));
  wire boot_d1 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 1));
  wire boot_d2 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 2));
  wire boot_d3 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 3));
  wire boot_d4 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 4));
  wire boot_d5 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 5));
  wire boot_d6 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 6));
  wire boot_d7 = tri6_pn(ZERY_BOOT_CSp, bit(~bootrom_data, 7));

  new_bus.BUS_CPU_D00p.tri_bus(boot_d0);
  new_bus.BUS_CPU_D01p.tri_bus(boot_d1);
  new_bus.BUS_CPU_D02p.tri_bus(boot_d2);
  new_bus.BUS_CPU_D03p.tri_bus(boot_d3);
  new_bus.BUS_CPU_D04p.tri_bus(boot_d4);
  new_bus.BUS_CPU_D05p.tri_bus(boot_d5);
  new_bus.BUS_CPU_D06p.tri_bus(boot_d6);
  new_bus.BUS_CPU_D07p.tri_bus(boot_d7);

  /* p07.TEXE*/ wire TEXE_FF50_RDp =  and4(cpu_signals.TEDO_CPU_RDp.out_new(), new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  /* p07.SYPU*/ wire SYPU_BOOT_TO_CD0 = tri6_pn(TEXE_FF50_RDp, cpu_signals.TEPU_BOOT_BITn_h.qp_new());
  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(SYPU_BOOT_TO_CD0);

  /* p07.SATO*/ SATO_BOOT_BITn = or2(new_bus.BUS_CPU_D00p.out_new(), cpu_signals.TEPU_BOOT_BITn_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
