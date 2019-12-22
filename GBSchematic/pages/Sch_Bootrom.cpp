#include "Sch_Bootrom.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Bootrom_tick(const Bus& bus,
                  const BusControl& ctl,
                  const Debug& dbg,
                  const Decoder& dec,
                  const Bootrom& prev,
                  Bootrom& next,
                  Bus& bus_out) {



  {
    /*p07.ZORO*/ wire ADDR_0XXX = nor(bus.A15, bus.A14, bus.A13, bus.A12);
    /*p07.ZADU*/ wire ADDR_X0XX = nor(bus.A11, bus.A10, bus.A09, bus.A08);
    /*p07.ZUFA*/ wire ADDR_00XX2 = and(ADDR_0XXX, ADDR_X0XX);
    /*p07.YAZA*/ wire MODE_DBG1n = not(dbg.MODE_DBG1);
    /*p07.YULA*/ wire BOOT_RD    = and(ctl.CPU_RD, MODE_DBG1n, dec.ADDR_BOOT);
    /*p07.ZADO*/ wire BOOT_CSn   = nand(BOOT_RD, ADDR_00XX2);
    /*p07.ZERY*/ next.BOOT_CS    = not(BOOT_CSn);
  }

  {
    /*p07.ZYBA*/ wire ADDR_00n = not(bus.A00);
    /*p07.ZUVY*/ wire ADDR_01n = not(bus.A01);
    /*p07.ZUFY*/ wire ADDR_04n = not(bus.A04);
    /*p07.ZERA*/ wire ADDR_05n = not(bus.A05);
    /*p07.ZOLE*/ wire ADDR_00 = and(ADDR_01n, ADDR_00n);
    /*p07.ZAJE*/ wire ADDR_01 = and(ADDR_01n, bus.A00);
    /*p07.ZUBU*/ wire ADDR_10 = and(bus.A01, ADDR_00n);
    /*p07.ZAPY*/ wire ADDR_11 = and(bus.A01, bus.A00);

    /*p07.ZETE*/ next.BOOTROM_A1nA0n = not(ADDR_00);
    /*p07.ZEFU*/ next.BOOTROM_A1nA0  = not(ADDR_01);
    /*p07.ZYRO*/ next.BOOTROM_A1A0n  = not(ADDR_10);
    /*p07.ZAPA*/ next.BOOTROM_A1A0   = not(ADDR_11);
    /*p07.ZYGA*/ next.BOOTROM_A2n    = not(bus.A02);
    /*p07.ZYKY*/ next.BOOTROM_A3n    = not(bus.A03);
    /*p07.ZYKY*/ next.BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
    /*p07.ZYGA*/ next.BOOTROM_A5nA4  = and(ADDR_05n, bus.A04);
    /*p07.ZOVY*/ next.BOOTROM_A5A4n  = and(bus.A05, ADDR_04n);
    /*p07.ZUKO*/ next.BOOTROM_A5A4   = and(bus.A05, bus.A04);
    /*p07.ZAGE*/ next.BOOTROM_A6n    = not(bus.A06);
    /*p07.ZYRA*/ next.BOOTROM_A7n    = not(bus.A07);
  }

  {
    /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(bus.A07, bus.A05, bus.A03, bus.A02, bus.A01, bus.A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(bus.A04, bus.A06);
    /*p07.TEXE*/ wire FF50_RD = and(ctl.CPU_RD, dec.ADDR_FFXX, ADDR_x1x1xxxx, ADDR_0x0x0000);
    /*p07.SYPU*/ if (FF50_RD) bus_out.D0 = prev.BOOT_BIT;
  }
}

//-----------------------------------------------------------------------------

void Bootrom_tock(const Bus& bus,
                  const BusControl& ctl,
                  const Decoder& dec,
                  const Resets& rst,
                  const Bootrom& prev,
                  Bootrom& next) {

  /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(bus.A07, bus.A05, bus.A03, bus.A02, bus.A01, bus.A00);
  /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(bus.A04, bus.A06);
  /*p07.TUGE*/ wire FF50_WRn = nand(ctl.CPU_WR, dec.ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
  /*p07.SATO*/ wire BOOT_BIT_IN  = or(bus.D0, prev.BOOT_BIT);
  /*p07.TEPU*/ next.BOOT_BIT.tock(FF50_WRn, rst.SYS_RESETn, BOOT_BIT_IN);
}

//-----------------------------------------------------------------------------

};