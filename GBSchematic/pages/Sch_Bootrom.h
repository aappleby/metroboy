#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bootrom {

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p07.TUGE*/ bool BOOT_BIT_CLK;
  /*p07.TEPU*/ bool BOOT_BIT;

  /*p07.ZETE*/ bool BOOTROM_A1nA0n;
  /*p07.ZEFU*/ bool BOOTROM_A1nA0;
  /*p07.ZYRO*/ bool BOOTROM_A1A0n;
  /*p07.ZAPA*/ bool BOOTROM_A1A0;
  /*p07.ZYGA*/ bool BOOTROM_A2n;
  /*p07.ZYKY*/ bool BOOTROM_A3n;
  /*p07.ZYKY*/ bool BOOTROM_A5nA4n;
  /*p07.ZYGA*/ bool BOOTROM_A5nA4;
  /*p07.ZOVY*/ bool BOOTROM_A5A4n;
  /*p07.ZUKO*/ bool BOOTROM_A5A4;
  /*p07.ZAGE*/ bool BOOTROM_A6n;
  /*p07.ZYRA*/ bool BOOTROM_A7n;

  /*p07.ZERY*/ bool BOOT_CS;
};

//-----------------------------------------------------------------------------

};