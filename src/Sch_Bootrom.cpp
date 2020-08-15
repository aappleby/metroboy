#include "Sch_Bootrom.h"

#include "Sch_Top.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

using namespace Schematics;

//-----------------------------------------------------------------------------

static const uint8_t DMG_ROM_bin[] = {
  0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32,
  0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e,
  0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
  0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0,
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a,
  0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
  0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06,
  0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9,
  0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
  0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20,
  0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64,
  0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
  0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90,
  0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2,
  0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
  0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06,
  0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xf0, 0x42,
  0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
  0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04,
  0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17,
  0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,

  // the logo @ 0x00A8
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
  0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
  0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
  0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,

  0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c,
  0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13,
  0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
  0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
  0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50
};

//-----------------------------------------------------------------------------

void Bootrom::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  if (top.TEDO_CPU_RDp() && (top.cpu_bus.get_bus_addr() <= 0xFF)) {
    int x = 1;
    x++;
  }

  // FF50
  {
    /*p07.TYRO*/ wire _TYFO_ADDR_0x0x0000p = nor6(top.cpu_bus.CPU_BUS_A07.tp(), top.cpu_bus.CPU_BUS_A05.tp(), top.cpu_bus.CPU_BUS_A03.tp(),
                                                  top.cpu_bus.CPU_BUS_A02.tp(), top.cpu_bus.CPU_BUS_A01.tp(), top.cpu_bus.CPU_BUS_A00.tp());
    /*p07.TUFA*/ wire _TUFA_ADDR_x1x1xxxxp = and2(top.cpu_bus.CPU_BUS_A04.tp(), top.cpu_bus.CPU_BUS_A06.tp());

    /*p07.TEXE*/ wire _TEXE_FF50_RDp = and4(top.TEDO_CPU_RDp(), top.cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SYPU*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(_TEXE_FF50_RDp, BOOT_BITn.qp());

    /*p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx(), top.cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SATO*/ wire _SATO_BOOT_BIT_IN = or2(top.cpu_bus.CPU_BUS_D0p.tp(), BOOT_BITn.qp());

    /*p07.TEPU*/ BOOT_BITn = dff17_AB(_TUGE_FF50_WRn, top.clk_reg.ALUR_SYS_RSTn(), _SATO_BOOT_BIT_IN);
  }

  {
    // Bootrom -> CPU

    /*p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(top.clk_reg.UMUT_MODE_DBG1p());
    /*p07.YULA*/ wire _YULA_BOOT_RDp   = and3(top.TEDO_CPU_RDp(), _YAZA_MODE_DBG1n, top.TUTU_ADDR_BOOTp()); // def AND
    /*p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, top.cpu_bus.ZUFA_ADDR_00XX());
    /*p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

#if 0
    /*p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(top.cpu_bus.CPU_BUS_A00);
    /*p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(top.cpu_bus.CPU_BUS_A01);
    /*p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(top.cpu_bus.CPU_BUS_A04);
    /*p07.ZERA*/ wire ZERA_ADDR_05n = not1(top.cpu_bus.CPU_BUS_A05);
    /*p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
    /*p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, top.cpu_bus.CPU_BUS_A00);
    /*p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(top.cpu_bus.CPU_BUS_A01, ZYBA_ADDR_00n);
    /*p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(top.cpu_bus.CPU_BUS_A01, top.cpu_bus.CPU_BUS_A00);

    /*p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
    /*p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
    /*p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
    /*p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
    /*p07.ZYGA*/ wire BOOTROM_A2n    = not1(top.cpu_bus.CPU_BUS_A02);
    /*p07.ZYKY*/ wire BOOTROM_A3n    = not1(top.cpu_bus.CPU_BUS_A03);
    /*p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
    /*p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, top.cpu_bus.CPU_BUS_A04);
    /*p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(top.cpu_bus.CPU_BUS_A05, ZUFY_ADDR_04n);
    /*p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(top.cpu_bus.CPU_BUS_A05, top.cpu_bus.CPU_BUS_A04);
    /*p07.ZAGE*/ wire BOOTROM_A6n    = not1(top.cpu_bus.CPU_BUS_A06);
    /*p07.ZYRA*/ wire BOOTROM_A7n    = not1(top.cpu_bus.CPU_BUS_A07);
#endif

#if 1
    // this is kind of a hack
    uint16_t addr = (uint16_t)top.cpu_bus.get_bus_addr();
    uint8_t data = DMG_ROM_bin[addr & 0xFF];

    cpu_bus.CPU_BUS_D0p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x01));
    cpu_bus.CPU_BUS_D1p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x02));
    cpu_bus.CPU_BUS_D2p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x04));
    cpu_bus.CPU_BUS_D3p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x08));
    cpu_bus.CPU_BUS_D4p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x10));
    cpu_bus.CPU_BUS_D5p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x20));
    cpu_bus.CPU_BUS_D6p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x40));
    cpu_bus.CPU_BUS_D7p = tribuf_6pn(_ZERY_BOOT_CSp, !bool(data & 0x80));
#endif
  }
}

//-----------------------------------------------------------------------------
