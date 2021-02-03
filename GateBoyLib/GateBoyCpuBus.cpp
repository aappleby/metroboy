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

void GateBoy::set_pins()
{
  rst.PIN_71_RST.reset_for_pass();
  pins.PIN_74_CLK.reset_for_pass();
  rst.PIN_76_T2.reset_for_pass();
  rst.PIN_77_T1.reset_for_pass();

  rst.PIN_71_RST.pin_in_dp(bit(~sys_rst));
  pins.PIN_74_CLK.pin_clk(!(phase_total & 1) && sys_clken, bit(~sys_clkgood));
  rst.PIN_76_T2.pin_in_dp(bit(~sys_t2));
  rst.PIN_77_T1.pin_in_dp(bit(~sys_t1));

  interrupts.SIG_CPU_ACK_VBLANK.sig_in(bit(int_ack_latch, BIT_VBLANK));
  interrupts.SIG_CPU_ACK_STAT  .sig_in(bit(int_ack_latch, BIT_STAT));
  interrupts.SIG_CPU_ACK_TIMER .sig_in(bit(int_ack_latch, BIT_TIMER));
  interrupts.SIG_CPU_ACK_SERIAL.sig_in(bit(int_ack_latch, BIT_SERIAL));
  interrupts.SIG_CPU_ACK_JOYPAD.sig_in(bit(int_ack_latch, BIT_JOYPAD));

  clk.SIG_CPU_CLKREQ.sig_in(sys_clkreq);

  cpu_signals.SIG_CPU_RDp.sig_in(DELTA_HA ? 0 : bus_req_new.read);
  cpu_signals.SIG_CPU_WRp.sig_in(DELTA_HA ? 0 : bus_req_new.write);

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req_new.read && (bus_req_new.addr < 0xFF00))) {
    cpu_signals.SIG_CPU_LATCH_EXT.sig_in(1);
  }
  else {
    cpu_signals.SIG_CPU_LATCH_EXT.sig_in(0);
  }

  //SIG_CPU_6.set(0);

  // FIXME yeeeeeech this is nasty. probably not right.

  uint16_t bus_addr_new = bus_req_new.addr;
  bool addr_ext_new = (bus_req_new.read || bus_req_new.write) && (bus_addr_new < 0xFE00);
  if (bus_addr_new <= 0x00FF && bit(~cpu_signals.TEPU_BOOT_BITn_h.qp_old())) addr_ext_new = false;
  if (DELTA_HA) {
    if ((bus_addr_new >= 0x8000) && (bus_addr_new < 0x9FFF)) addr_ext_new = false;
  }
  cpu_signals.SIG_CPU_EXT_BUSp.sig_in(addr_ext_new);

  // Data has to be driven on EFGH or we fail the wave tests

  /*SIG_CPU_ADDR_HIp*/ cpu_signals.SIG_CPU_ADDR_HIp.sig_out(new_bus.SYRO_FE00_FFFF());
  /*SIG_CPU_UNOR_DBG*/ cpu_signals.SIG_CPU_UNOR_DBG.sig_out(UNOR_MODE_DBG2p());
  /*SIG_CPU_UMUT_DBG*/ cpu_signals.SIG_CPU_UMUT_DBG.sig_out(UMUT_MODE_DBG1p());

  ///* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_79_EXT_RDn.qn_new()*/ 0, SIG_CPU_RDp.qp_new()); // Ignoring debug stuff for now
  ///* p07.UBAL*/ wire _UBAL_CPU_WRn = mux2n(rst.UNOR_MODE_DBG2p(), /*PIN_78_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp); // Ignoring debug stuff for now
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_bootrom() {
  /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(cpu_signals.TAPU_CPU_WRp.qp_new(), new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  // FF50 - disable bootrom bit

  /* p07.TEPU*/ cpu_signals.TEPU_BOOT_BITn_h.dff17(_TUGE_FF50_WRn, ALUR_SYS_RSTn(), SATO_BOOT_BITn.qp_old());

  // BOOT -> CBD
  // this is kind of a hack
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, &new_bus.BUS_CPU_A00p);
  wire bootrom_data = boot_buf[cpu_addr & 0xFF];

  /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1(cpu_signals.TEPU_BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, new_bus.TULO_ADDR_BOOTROMp());
  /*SIG_CPU_BOOTp*/ cpu_signals.SIG_CPU_BOOTp.sig_out(_TUTU_READ_BOOTROMp);

  /* p07.ZORO*/ wire _ZORO_0000xxxx_XX = nor4(new_bus.BUS_CPU_A15p.qp_new(), new_bus.BUS_CPU_A14p.qp_new(), new_bus.BUS_CPU_A13p.qp_new(), new_bus.BUS_CPU_A12p.qp_new());
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XX = nor4(new_bus.BUS_CPU_A11p.qp_new(), new_bus.BUS_CPU_A10p.qp_new(), new_bus.BUS_CPU_A09p.qp_new(), new_bus.BUS_CPU_A08p.qp_new());
  /* p07.ZUFA*/ wire _ZUFA_0000_00FF  = and2(_ZORO_0000xxxx_XX, _ZADU_xxxx0000_XX);
  /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(UMUT_MODE_DBG1p());
  /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(cpu_signals.TEDO_CPU_RDp.qp_new(), _YAZA_MODE_DBG1n, _TUTU_READ_BOOTROMp); // def AND
  /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FF);
  /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

  /*SIG_BOOT_CSp*/ cpu_signals.SIG_BOOT_CSp.sig_out(_ZERY_BOOT_CSp);

  new_bus.BUS_CPU_D00p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 0));
  new_bus.BUS_CPU_D01p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 1));
  new_bus.BUS_CPU_D02p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 2));
  new_bus.BUS_CPU_D03p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 3));
  new_bus.BUS_CPU_D04p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 4));
  new_bus.BUS_CPU_D05p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 5));
  new_bus.BUS_CPU_D06p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 6));
  new_bus.BUS_CPU_D07p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 7));

  /* p07.TEXE*/ wire _TEXE_FF50_RDp =  and4(cpu_signals.TEDO_CPU_RDp.qp_new(), new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  /* p07.SYPU_BOOT_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TEXE_FF50_RDp, cpu_signals.TEPU_BOOT_BITn_h.qp_new());

  /* p07.SATO*/ SATO_BOOT_BITn = or2(new_bus.BUS_CPU_D00p.qp_new(), cpu_signals.TEPU_BOOT_BITn_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
