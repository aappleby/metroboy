#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct VramPins {
  void tick(TestGB& gb);
  bool commit();

  void dump(TextPainter& text_painter) {
    text_painter.dprintf("----- VRAM_PINS -----\n");

    text_painter.dprintf("MCS  %d:x:%d:%d\n", PIN_MCSn_A.a.val, PIN_MCSn_C.a.val, PIN_MCSn_D.a.val);
    text_painter.dprintf("MOE  %d:x:%d:%d\n", PIN_MOEn_A.a.val, PIN_MOEn_C.a.val, PIN_MOEn_D.a.val);
    text_painter.dprintf("MWR  %d:x:%d:%d\n", PIN_MWRn_A.a.val, PIN_MWRn_C.a.val, PIN_MWRn_D.a.val);
    text_painter.dprintf("MAxx 0x%04x\n", pack(PIN_MA00_AD, PIN_MA01_AD, PIN_MA02_AD, PIN_MA03_AD, PIN_MA04_AD, PIN_MA05_AD, PIN_MA06_AD,
                                               PIN_MA07_AD, PIN_MA08_AD, PIN_MA09_AD, PIN_MA10_AD, PIN_MA11_AD, PIN_MA12_AD));

    text_painter.dprintf("MDx_A 0x%02x\n", pack(PIN_MD0_A, PIN_MD1_A, PIN_MD2_A, PIN_MD3_A, PIN_MD4_A, PIN_MD5_A, PIN_MD6_A, PIN_MD7_A));
    text_painter.dprintf("MDx_B 0x%02x\n", pack(PIN_MD0_B, PIN_MD1_B, PIN_MD2_B, PIN_MD3_B, PIN_MD4_B, PIN_MD5_B, PIN_MD6_B, PIN_MD7_B));
    text_painter.dprintf("MDx_C 0x%02x\n", pack(PIN_MD0_C, PIN_MD1_C, PIN_MD2_C, PIN_MD3_C, PIN_MD4_C, PIN_MD5_C, PIN_MD6_C, PIN_MD7_C));
    text_painter.dprintf("MDx_D 0x%02x\n", pack(PIN_MD0_D, PIN_MD1_D, PIN_MD2_D, PIN_MD3_D, PIN_MD4_D, PIN_MD5_D, PIN_MD6_D, PIN_MD7_D));
    text_painter.newline();
  }

  /* PIN_43 */ PinIn  PIN_MCSn_C;  // -> P25.TEFY
  /* PIN_45 */ PinIn  PIN_MOEn_C;  // -> P25.TAVY
  /* PIN_49 */ PinIn  PIN_MWRn_C;  // -> P25.SUDO

  /* PIN_33 */ PinIn  PIN_MD0_C;   // -> P25.RODY
  /* PIN_31 */ PinIn  PIN_MD1_C;   // -> P25.REBA
  /* PIN_30 */ PinIn  PIN_MD2_C;   // -> P25.RYDO
  /* PIN_29 */ PinIn  PIN_MD3_C;   // -> P25.REMO
  /* PIN_28 */ PinIn  PIN_MD4_C;   // -> P25.ROCE
  /* PIN_27 */ PinIn  PIN_MD5_C;   // -> P25.ROPU
  /* PIN_26 */ PinIn  PIN_MD6_C;   // -> P25.RETA
  /* PIN_25 */ PinIn  PIN_MD7_C;   // -> P25.RAKU

private:

  //----------
  // VRAM control pins

  /* PIN_43 */ PinOut PIN_MCSn_A;   // <- P25.SOKY
  /* PIN_43 */ PinOut PIN_MCSn_D;   // <- P25.SETY

  /* PIN_45 */ PinOut PIN_MOEn_A;   // <- P25.REFO
  /* PIN_45 */ PinOut PIN_MOEn_D;   // <- P25.SAHA

  /* PIN_49 */ PinOut PIN_MWRn_A;   // <- P25.SYSY
  /* PIN_49 */ PinOut PIN_MWRn_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ PinOut PIN_MA00_AD;  // <- P04.ECAL
  /* PIN_35 */ PinOut PIN_MA01_AD;  // <- P04.EGEZ
  /* PIN_36 */ PinOut PIN_MA02_AD;  // <- P04.FUHE
  /* PIN_37 */ PinOut PIN_MA03_AD;  // <- P04.FYZY
  /* PIN_38 */ PinOut PIN_MA04_AD;  // <- P04.DAMU
  /* PIN_39 */ PinOut PIN_MA05_AD;  // <- P04.DAVA
  /* PIN_40 */ PinOut PIN_MA06_AD;  // <- P04.ETEG
  /* PIN_41 */ PinOut PIN_MA07_AD;  // <- P04.EREW
  /* PIN_48 */ PinOut PIN_MA08_AD;  // <- P04.EVAX
  /* PIN_47 */ PinOut PIN_MA09_AD;  // <- P04.DUVE
  /* PIN_44 */ PinOut PIN_MA10_AD;  // <- P04.ERAF
  /* PIN_46 */ PinOut PIN_MA11_AD;  // <- P04.FUSY
  /* PIN_42 */ PinOut PIN_MA12_AD;  // <- P04.EXYF

  //----------
  // VRAM data pins

  /* PIN_33 */ PinOut PIN_MD0_A;   // <- P25.REGE
  /* PIN_31 */ PinOut PIN_MD1_A;   // <- P25.RYKY
  /* PIN_30 */ PinOut PIN_MD2_A;   // <- P25.RAZO
  /* PIN_29 */ PinOut PIN_MD3_A;   // <- P25.RADA
  /* PIN_28 */ PinOut PIN_MD4_A;   // <- P25.RYRO
  /* PIN_27 */ PinOut PIN_MD5_A;   // <- P25.REVU
  /* PIN_26 */ PinOut PIN_MD6_A;   // <- P25.REKU
  /* PIN_25 */ PinOut PIN_MD7_A;   // <- P25.RYZE

  /* PIN_33 */ PinOut PIN_MD0_B;   // <- P25.ROFA
  /* PIN_31 */ PinOut PIN_MD1_B;   // <- P25.ROFA
  /* PIN_30 */ PinOut PIN_MD2_B;   // <- P25.ROFA
  /* PIN_29 */ PinOut PIN_MD3_B;   // <- P25.ROFA
  /* PIN_28 */ PinOut PIN_MD4_B;   // <- P25.ROFA
  /* PIN_27 */ PinOut PIN_MD5_B;   // <- P25.ROFA
  /* PIN_26 */ PinOut PIN_MD6_B;   // <- P25.ROFA
  /* PIN_25 */ PinOut PIN_MD7_B;   // <- P25.ROFA

  /* PIN_33 */ PinOut PIN_MD0_D;   // <- P25.RURA
  /* PIN_31 */ PinOut PIN_MD1_D;   // <- P25.RULY
  /* PIN_30 */ PinOut PIN_MD2_D;   // <- P25.RARE
  /* PIN_29 */ PinOut PIN_MD3_D;   // <- P25.RODU
  /* PIN_28 */ PinOut PIN_MD4_D;   // <- P25.RUBE
  /* PIN_27 */ PinOut PIN_MD5_D;   // <- P25.RUMU
  /* PIN_26 */ PinOut PIN_MD6_D;   // <- P25.RYTY
  /* PIN_25 */ PinOut PIN_MD7_D;   // <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics