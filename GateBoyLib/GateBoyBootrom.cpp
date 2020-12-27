#include "GateBoyLib/GateBoyBootrom.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

//--------------------------------------------------------------------------------

void GateBoyBootrom::read_boot_bit(GateBoyCpuBus& cpu_bus)
{
  /* p07.TEXE*/ wire _TEXE_FF50_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new2(), cpu_bus.SYKE_ADDR_HIp(), cpu_bus.TYRO_XX_0x0x0000p(), cpu_bus.TUFA_XX_x1x1xxxxp());
  /* p07.SYPU*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TEXE_FF50_RDp, cpu_bus.TEPU_BOOT_BITn_h.qp_new2());
}

void GateBoyBootrom::write_boot_bit_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4b(cpu_bus.TAPU_CPU_WRp.qp_new2(), cpu_bus.SYKE_ADDR_HIp(), cpu_bus.TYRO_XX_0x0x0000p(), cpu_bus.TUFA_XX_x1x1xxxxp());
  // FF50 - disable bootrom bit

  /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(cpu_bus.BUS_CPU_D[0].qp_old2(), cpu_bus.TEPU_BOOT_BITn_h.qp_old2());
  /* p07.TEPU*/ cpu_bus.TEPU_BOOT_BITn_h.dff17(_TUGE_FF50_WRn, rst.ALUR_SYS_RSTn(), _SATO_BOOT_BITn_old);
}

void GateBoyBootrom::read_bootrom(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, uint8_t* boot_buf)
{
  /* BOOT -> CBD */
  // this is kind of a hack
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, cpu_bus.BUS_CPU_A);
  wire bootrom_data = boot_buf[cpu_addr & 0xFF];

  /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1b(cpu_bus.TEPU_BOOT_BITn_h.qp_new2());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, cpu_bus.TULO_ADDR_BOOTROMp());
  cpu_bus.SIG_CPU_BOOTp.set(_TUTU_READ_BOOTROMp);

  /* p07.ZORO*/ wire _ZORO_0000xxxx_XX = nor4b(cpu_bus.BUS_CPU_A[15].qp_new2(), cpu_bus.BUS_CPU_A[14].qp_new2(), cpu_bus.BUS_CPU_A[13].qp_new2(), cpu_bus.BUS_CPU_A[12].qp_new2());
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XX = nor4b(cpu_bus.BUS_CPU_A[11].qp_new2(), cpu_bus.BUS_CPU_A[10].qp_new2(), cpu_bus.BUS_CPU_A[ 9].qp_new2(), cpu_bus.BUS_CPU_A[ 8].qp_new2());
  /* p07.ZUFA*/ wire _ZUFA_0000_00FF  = and2(_ZORO_0000xxxx_XX, _ZADU_xxxx0000_XX);
  /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1b(rst.UMUT_MODE_DBG1p());
  /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(cpu_bus.TEDO_CPU_RDp.qp_new2(), _YAZA_MODE_DBG1n, _TUTU_READ_BOOTROMp); // def AND
  /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2b(_YULA_BOOT_RDp, _ZUFA_0000_00FF);
  /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1b(_ZADO_BOOT_CSn);
  cpu_bus.BUS_CPU_D[0].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 0));
  cpu_bus.BUS_CPU_D[1].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 1));
  cpu_bus.BUS_CPU_D[2].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 2));
  cpu_bus.BUS_CPU_D[3].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 3));
  cpu_bus.BUS_CPU_D[4].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 4));
  cpu_bus.BUS_CPU_D[5].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 5));
  cpu_bus.BUS_CPU_D[6].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 6));
  cpu_bus.BUS_CPU_D[7].tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 7));
}

//--------------------------------------------------------------------------------
