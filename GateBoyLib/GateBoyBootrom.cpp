#include "GateBoyLib/GateBoyBootrom.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

//--------------------------------------------------------------------------------

void GateBoyBootrom::reset_cart() {
  BOOT_BITn_h.reset(REG_D1C1);
}

//--------------------------------------------------------------------------------

void GateBoyBootrom::tock(
  GateBoyResetDebug& rst,
  GateBoyCpuBus& cpu_bus,
  uint8_t* boot_buf)
{
  {
    /* p07.TEXE*/ wire _TEXE_FF50_RDp =  and4(cpu_bus.TEDO_CPU_RDp, cpu_bus.SYKE_ADDR_HIp(), cpu_bus.TYRO_XX_0x0x0000p(), cpu_bus.TUFA_XX_x1x1xxxxp());
    /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(cpu_bus.TAPU_CPU_WRp, cpu_bus.SYKE_ADDR_HIp(), cpu_bus.TYRO_XX_0x0x0000p(), cpu_bus.TUFA_XX_x1x1xxxxp());
    // FF50 - disable bootrom bit
    /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(cpu_bus.BUS_CPU_D[0], BOOT_BITn_h.qp_old());
    /* p07.TEPU*/ BOOT_BITn_h.dff17(_TUGE_FF50_WRn, rst.ALUR_SYS_RSTn(), _SATO_BOOT_BITn_old);
    /* p07.SYPU*/ cpu_bus.BUS_CPU_D_out[0].tri6_pn(_TEXE_FF50_RDp, BOOT_BITn_h.qp_new());
  }

  /* BOOT -> CBD */
#if 0
/* p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(BUS_CPU_A[ 0]);
/* p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(BUS_CPU_A[ 1]);
/* p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(BUS_CPU_A[ 4]);
/* p07.ZERA*/ wire ZERA_ADDR_05n = not1(BUS_CPU_A[ 5]);
/* p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
/* p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, BUS_CPU_A[ 0]);
/* p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(BUS_CPU_A[ 1], ZYBA_ADDR_00n);
/* p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(BUS_CPU_A[ 1], BUS_CPU_A[ 0]);

/* p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
/* p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
/* p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
/* p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
/* p07.ZYGA*/ wire BOOTROM_A2n    = not1(BUS_CPU_A[ 2]);
/* p07.ZYKY*/ wire BOOTROM_A3n    = not1(BUS_CPU_A[ 3]);
/* p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
/* p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, BUS_CPU_A[ 4]);
/* p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(BUS_CPU_A[ 5], ZUFY_ADDR_04n);
/* p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
/* p07.ZAGE*/ wire BOOTROM_A6n    = not1(BUS_CPU_A[ 6]);
/* p07.ZYRA*/ wire BOOTROM_A7n    = not1(BUS_CPU_A[ 7]);
#endif

  // this is kind of a hack
  uint16_t cpu_addr = pack_u16p(16, cpu_bus.BUS_CPU_A);
  uint8_t bootrom_data = boot_buf[cpu_addr & 0xFF];

  /* p07.TULO*/ wire _TULO_ADDR_BOOTROMp = nor8(cpu_bus.BUS_CPU_A[15], cpu_bus.BUS_CPU_A[14], cpu_bus.BUS_CPU_A[13], cpu_bus.BUS_CPU_A[12],
                                                    cpu_bus.BUS_CPU_A[11], cpu_bus.BUS_CPU_A[10], cpu_bus.BUS_CPU_A[ 9], cpu_bus.BUS_CPU_A[ 8]);
  /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1(BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, _TULO_ADDR_BOOTROMp);

  /* p07.ZORO*/ wire _ZORO_0000xxxx_XX = nor4(cpu_bus.BUS_CPU_A[15], cpu_bus.BUS_CPU_A[14], cpu_bus.BUS_CPU_A[13], cpu_bus.BUS_CPU_A[12]);
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XX = nor4(cpu_bus.BUS_CPU_A[11], cpu_bus.BUS_CPU_A[10], cpu_bus.BUS_CPU_A[ 9], cpu_bus.BUS_CPU_A[ 8]);
  /* p07.ZUFA*/ wire _ZUFA_0000_00FF  = and2(_ZORO_0000xxxx_XX, _ZADU_xxxx0000_XX);
  /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(rst.UMUT_MODE_DBG1p());
  /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(cpu_bus.TEDO_CPU_RDp, _YAZA_MODE_DBG1n, _TUTU_READ_BOOTROMp); // def AND
  /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FF);
  /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);
  cpu_bus.BUS_CPU_D_out[0].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x01));
  cpu_bus.BUS_CPU_D_out[1].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x02));
  cpu_bus.BUS_CPU_D_out[2].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x04));
  cpu_bus.BUS_CPU_D_out[3].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x08));
  cpu_bus.BUS_CPU_D_out[4].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x10));
  cpu_bus.BUS_CPU_D_out[5].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x20));
  cpu_bus.BUS_CPU_D_out[6].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x40));
  cpu_bus.BUS_CPU_D_out[7].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x80));
}

//--------------------------------------------------------------------------------

wire GateBoyBootrom::TUTU_READ_BOOTROMp(Signal BUS_CPU_A[16])
{
  /* p07.TULO*/ wire _TULO_ADDR_BOOTROMp = nor8(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12],
                                                BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
  /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1(BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, _TULO_ADDR_BOOTROMp);
  return _TUTU_READ_BOOTROMp;
}

//--------------------------------------------------------------------------------
