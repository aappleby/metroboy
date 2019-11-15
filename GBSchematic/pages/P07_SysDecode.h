#pragma once

struct Gameboy;

struct P07_SysDecode {
public:

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  //----------
  // outputs

  /*p07.UBET*/ bool T1n;
  /*p07.UVAR*/ bool T2n;
  /*p07.UPOJ*/ bool MODE_PROD;
  /*p07.UMUT*/ bool MODE_DBG1;
  /*p07.YAZA*/ bool MODE_DBG1n;
  /*p07.UNOR*/ bool MODE_DBG2;
  /*p07.APET*/ bool MODE_DEBUG;

  /*p07.LEXY*/ bool PIN_NC;


  /*p07.ZYRA*/ bool BOOTROM_A7n;
  /*p07.ZAGE*/ bool BOOTROM_A6n;
  /*p07.ZYKY*/ bool BOOTROM_A5nA4n;
  /*p07.ZYGA*/ bool BOOTROM_A5nA4;
  /*p07.ZOVY*/ bool BOOTROM_A5A4n;
  /*p07.ZUKO*/ bool BOOTROM_A5A4;
  /*p07.ZABU*/ bool BOOTROM_A3n;
  /*p07.ZOKE*/ bool BOOTROM_A2n;
  /*p07.ZETE*/ bool BOOTROM_A1nA0n;
  /*p07.ZEFU*/ bool BOOTROM_A1nA0;
  /*p07.ZYRO*/ bool BOOTROM_A1A0n;
  /*p07.ZAPA*/ bool BOOTROM_A1A0;

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;
  /*p07.ASOT*/ bool CPU_RD2;
  /*p07.CUPA*/ bool CPU_WR2;
  /*p07.AJAS*/ bool CPU_RDn;
  /*p07.UBAL*/ bool CPU_WR_MUX;
  /*p07.UJYV*/ bool CPU_RD_MUX;
  /*p07.DYKY*/ bool CPU_WRn;

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p07.TEXE*/ bool BOOT_BIT_RD;
  /*p07.TUGE*/ bool BOOT_BIT_WRn;
  /*p07.TEPU*/ bool BOOT_BIT;
  /*p07.SATO*/ bool BOOT_BIT2;
  /*p07.SYPU*/ bool BOOT_BIT_D0;
  /*p07.TERA*/ bool BOOT_EN;
  /*p07.YULA*/ bool BOOT_RD;
  /*p07.ZERY*/ bool BOOT_CS;
  /*p07.ZADO*/ bool BOOT_CSn;

  /*p07.WALE*/ bool ADDR_xxxxxxxxx1111111n;
  /*p07.WOLY*/ bool HRAM_CSn;
  /*p07.WUTA*/ bool HRAM_CS;

  /*p07.ROLO*/ bool FF0F_RD;
  /*p07.REFA*/ bool FF0F_WR;

  // FF60 - secret debug register
  /*p07.APER*/ bool FF60_WRn;
  /*p07.BURO*/ bool FF60_0;
  /*p07.AMUT*/ bool FF60_1;

  /*p07.BAKO*/ bool ADDR_FFXXn1;
  /*p07.SOHA*/ bool ADDR_FFXXn2;
  /*p07.SARO*/ bool ADDR_OAM;
  /*p07.ZYBA*/ bool ADDR_00n;
  /*p07.ZUVY*/ bool ADDR_01n;
  /*p07.ZUFY*/ bool ADDR_04n;
  /*p07.ZERA*/ bool ADDR_05n;
  /*p07.TONA*/ bool ADDR_08n;
  /*p07.TULO*/ bool ADDR_00XX;
  /*p07.ZUFA*/ bool ADDR_00XX2;
  /*p07.ZORO*/ bool ADDR_0XXX;
  /*p07.ZADU*/ bool ADDR_X0XX;
  /*p07.SEMY*/ bool ADDR_XX0X;
  /*p07.SAPA*/ bool ADDR_XXXF;
  /*p07.RYCU*/ bool ADDR_FE00_FFFF;
  /*p07.ROPE*/ bool ADDR_FEXXn;
  /*p07.SYKE*/ bool ADDR_FFXX;
  /*p07.TUNA*/ bool ADDR_0000_FE00;

  /*p07.LECO*/ bool LECO;
  /*p07.ROMY*/ bool DBG_0;
  /*p07.RYNE*/ bool DBG_1;
  /*p07.REJY*/ bool DBG_2;
  /*p07.RASE*/ bool DBG_3;
  /*p07.REKA*/ bool DBG_4;
  /*p07.ROWE*/ bool DBG_5;
  /*p07.RYKE*/ bool DBG_6;
  /*p07.RARU*/ bool DBG_7;


  /*p07.TYRO*/ bool ADDR_xxxxxxxx0x0x0000;
  /*p07.TUFA*/ bool ADDR_xxxxxxxxx1x1xxxx;

  /*p07.ZOLE*/ bool ADDR_xxxxxxxxxxxxxx00;
  /*p07.ZAJE*/ bool ADDR_xxxxxxxxxxxxxx01;
  /*p07.ZUBU*/ bool ADDR_xxxxxxxxxxxxxx10;
  /*p07.ZAPY*/ bool ADDR_xxxxxxxxxxxxxx11;

};
