#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bootrom {
  /*p07.TEPU*/ Reg BOOT_BIT;

  /*p07.ZERY*/ bool BOOT_CS;
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
};

//-----------------------------------------------------------------------------

};