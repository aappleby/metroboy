#include "GateBoyLib/GateBoyBootrom.h"

#include "GateBoyLib/GateBoy.h"

//--------------------------------------------------------------------------------

void GateBoy::tock_bootrom()
{
  /* p07.TEXE*/ wire _TEXE_FF50_RDp =  and4(cpu_bus.TEDO_CPU_RDp, new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  /* p07.SYPU_BOOT_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TEXE_FF50_RDp, cpu_bus.TEPU_BOOT_BITn_h.qp_new());

  /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(cpu_bus.TAPU_CPU_WRp, new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
  // FF50 - disable bootrom bit

  /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(old_bus.BUS_CPU_D00p.qp_old(), cpu_bus.TEPU_BOOT_BITn_h.qp_old());
  /* p07.TEPU*/ cpu_bus.TEPU_BOOT_BITn_h.dff17(_TUGE_FF50_WRn, ALUR_SYS_RSTn(), _SATO_BOOT_BITn_old);

  // BOOT -> CBD
  // this is kind of a hack
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, &new_bus.BUS_CPU_A00p);
  wire bootrom_data = boot_buf[cpu_addr & 0xFF];

  /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1(cpu_bus.TEPU_BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, new_bus.TULO_ADDR_BOOTROMp());
  /*SIG_CPU_BOOTp*/ cpu_bus.SIG_CPU_BOOTp.sig_out(_TUTU_READ_BOOTROMp);

  /* p07.ZORO*/ wire _ZORO_0000xxxx_XX = nor4(new_bus.BUS_CPU_A15p.qp_new(), new_bus.BUS_CPU_A14p.qp_new(), new_bus.BUS_CPU_A13p.qp_new(), new_bus.BUS_CPU_A12p.qp_new());
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XX = nor4(new_bus.BUS_CPU_A11p.qp_new(), new_bus.BUS_CPU_A10p.qp_new(), new_bus.BUS_CPU_A09p.qp_new(), new_bus.BUS_CPU_A08p.qp_new());
  /* p07.ZUFA*/ wire _ZUFA_0000_00FF  = and2(_ZORO_0000xxxx_XX, _ZADU_xxxx0000_XX);
  /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(UMUT_MODE_DBG1p());
  /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(cpu_bus.TEDO_CPU_RDp, _YAZA_MODE_DBG1n, _TUTU_READ_BOOTROMp); // def AND
  /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FF);
  /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

  /*SIG_BOOT_CSp*/ cpu_bus.SIG_BOOT_CSp.sig_out(_ZERY_BOOT_CSp);

  new_bus.BUS_CPU_D00p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 0));
  new_bus.BUS_CPU_D01p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 1));
  new_bus.BUS_CPU_D02p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 2));
  new_bus.BUS_CPU_D03p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 3));
  new_bus.BUS_CPU_D04p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 4));
  new_bus.BUS_CPU_D05p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 5));
  new_bus.BUS_CPU_D06p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 6));
  new_bus.BUS_CPU_D07p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 7));
}

//--------------------------------------------------------------------------------
