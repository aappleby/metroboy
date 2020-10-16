#include "GateBoyLib/Sch_Bootrom.h"
#include "GateBoyLib/Sch_Top.h"

#include "CoreLib/Constants.h" // for DMG_ROM_bin

#pragma warning(disable:4100)
#pragma warning(disable:4189)

using namespace Schematics;

//-----------------------------------------------------------------------------

void Bootrom::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  // FF50 - disable bootrom bit
  {
    /*p07.TYRO*/ wire _TYFO_ADDR_0x0x0000p = nor6(top.cpu_bus.BUS_CPU_A07.qp(), top.cpu_bus.BUS_CPU_A05.qp(), top.cpu_bus.BUS_CPU_A03.qp(),
                                                  top.cpu_bus.BUS_CPU_A02.qp(), top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.BUS_CPU_A00.qp());
    /*p07.TUFA*/ wire _TUFA_ADDR_x1x1xxxxp = and2(top.cpu_bus.BUS_CPU_A04.qp(), top.cpu_bus.BUS_CPU_A06.qp());

    /*p07.TEXE*/ wire _TEXE_FF50_RDp = and4(top.TEDO_CPU_RDp, top.cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SYPU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TEXE_FF50_RDp, BOOT_BITn.qp17());

    /*p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, top.cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SATO*/ wire _SATO_BOOT_BIT_IN = or2(top.cpu_bus.BUS_CPU_D0p.qp(), BOOT_BITn.qp17());

    /*p07.TEPU*/ BOOT_BITn.dff17(_TUGE_FF50_WRn, top.clk_reg.ALUR_SYS_RSTn, _SATO_BOOT_BIT_IN);
  }

  {
    // Bootrom -> CPU

    /*p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(top.UMUT_MODE_DBG1p);
    /*p07.YULA*/ wire _YULA_BOOT_RDp   = and3(top.TEDO_CPU_RDp, _YAZA_MODE_DBG1n, top.TUTU_ADDR_BOOTp); // def AND
    /*p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, top.cpu_bus.ZUFA_ADDR_00XX());
    /*p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

#if 0
    /*p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(top.cpu_bus.BUS_CPU_A00);
    /*p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(top.cpu_bus.BUS_CPU_A01);
    /*p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(top.cpu_bus.BUS_CPU_A04);
    /*p07.ZERA*/ wire ZERA_ADDR_05n = not1(top.cpu_bus.BUS_CPU_A05);
    /*p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
    /*p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, top.cpu_bus.BUS_CPU_A00);
    /*p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(top.cpu_bus.BUS_CPU_A01, ZYBA_ADDR_00n);
    /*p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(top.cpu_bus.BUS_CPU_A01, top.cpu_bus.BUS_CPU_A00);

    /*p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
    /*p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
    /*p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
    /*p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
    /*p07.ZYGA*/ wire BOOTROM_A2n    = not1(top.cpu_bus.BUS_CPU_A02);
    /*p07.ZYKY*/ wire BOOTROM_A3n    = not1(top.cpu_bus.BUS_CPU_A03);
    /*p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
    /*p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, top.cpu_bus.BUS_CPU_A04);
    /*p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(top.cpu_bus.BUS_CPU_A05, ZUFY_ADDR_04n);
    /*p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(top.cpu_bus.BUS_CPU_A05, top.cpu_bus.BUS_CPU_A04);
    /*p07.ZAGE*/ wire BOOTROM_A6n    = not1(top.cpu_bus.BUS_CPU_A06);
    /*p07.ZYRA*/ wire BOOTROM_A7n    = not1(top.cpu_bus.BUS_CPU_A07);
#endif

#if 1
    // this is kind of a hack
    uint16_t addr = (uint16_t)top.cpu_bus.get_bus_addr();
    uint8_t data = DMG_ROM_bin[addr & 0xFF];

    cpu_bus.BUS_CPU_D0p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x01));
    cpu_bus.BUS_CPU_D1p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x02));
    cpu_bus.BUS_CPU_D2p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x04));
    cpu_bus.BUS_CPU_D3p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x08));
    cpu_bus.BUS_CPU_D4p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x10));
    cpu_bus.BUS_CPU_D5p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x20));
    cpu_bus.BUS_CPU_D6p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x40));
    cpu_bus.BUS_CPU_D7p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x80));
#endif
  }
}

//-----------------------------------------------------------------------------
