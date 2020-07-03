#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramPins {
  void tick(SchematicTop& gb);
  SignalHash commit();

  void dump(TextPainter& text_painter) {
    text_painter.dprintf("----- VRAM_PINS -----\n");

    text_painter.dprintf("MCS  %d:x:%d:%d\n", EXT_PIN_MCSn_A.prev().val, EXT_PIN_MCSn_C.prev().val, EXT_PIN_MCSn_D.prev().val);
    text_painter.dprintf("MOE  %d:x:%d:%d\n", EXT_PIN_MOEn_A.prev().val, EXT_PIN_MOEn_C.prev().val, EXT_PIN_MOEn_D.prev().val);
    text_painter.dprintf("MWR  %d:x:%d:%d\n", EXT_PIN_MWRn_A.prev().val, EXT_PIN_MWRn_C.prev().val, EXT_PIN_MWRn_D.prev().val);
    text_painter.dprintf("MAxx 0x%04x\n", pack(EXT_PIN_MA00_AD, EXT_PIN_MA01_AD, EXT_PIN_MA02_AD, EXT_PIN_MA03_AD, EXT_PIN_MA04_AD, EXT_PIN_MA05_AD, EXT_PIN_MA06_AD,
                                               EXT_PIN_MA07_AD, EXT_PIN_MA08_AD, EXT_PIN_MA09_AD, EXT_PIN_MA10_AD, EXT_PIN_MA11_AD, EXT_PIN_MA12_AD));

    text_painter.dprintf("MDx_A 0x%02x\n", pack(EXT_PIN_MD0_A, EXT_PIN_MD1_A, EXT_PIN_MD2_A, EXT_PIN_MD3_A, EXT_PIN_MD4_A, EXT_PIN_MD5_A, EXT_PIN_MD6_A, EXT_PIN_MD7_A));
    text_painter.dprintf("MDx_B 0x%02x\n", pack(EXT_PIN_MD0_B, EXT_PIN_MD1_B, EXT_PIN_MD2_B, EXT_PIN_MD3_B, EXT_PIN_MD4_B, EXT_PIN_MD5_B, EXT_PIN_MD6_B, EXT_PIN_MD7_B));
    text_painter.dprintf("MDx_C 0x%02x\n", pack(EXT_PIN_MD0_C, EXT_PIN_MD1_C, EXT_PIN_MD2_C, EXT_PIN_MD3_C, EXT_PIN_MD4_C, EXT_PIN_MD5_C, EXT_PIN_MD6_C, EXT_PIN_MD7_C));
    text_painter.dprintf("MDx_D 0x%02x\n", pack(EXT_PIN_MD0_D, EXT_PIN_MD1_D, EXT_PIN_MD2_D, EXT_PIN_MD3_D, EXT_PIN_MD4_D, EXT_PIN_MD5_D, EXT_PIN_MD6_D, EXT_PIN_MD7_D));
    text_painter.newline();
  }

  /* PIN_45 */ PinIn  EXT_PIN_MOEn_C;  // -> P25.TAVY
  /* PIN_43 */ PinIn  EXT_PIN_MCSn_C;  // -> P25.TEFY

private:

  /* PIN_49 */ PinIn  EXT_PIN_MWRn_C;  // -> P25.SUDO

  /* PIN_33 */ PinIn  EXT_PIN_MD0_C;   // -> P25.RODY
  /* PIN_31 */ PinIn  EXT_PIN_MD1_C;   // -> P25.REBA
  /* PIN_30 */ PinIn  EXT_PIN_MD2_C;   // -> P25.RYDO
  /* PIN_29 */ PinIn  EXT_PIN_MD3_C;   // -> P25.REMO
  /* PIN_28 */ PinIn  EXT_PIN_MD4_C;   // -> P25.ROCE
  /* PIN_27 */ PinIn  EXT_PIN_MD5_C;   // -> P25.ROPU
  /* PIN_26 */ PinIn  EXT_PIN_MD6_C;   // -> P25.RETA
  /* PIN_25 */ PinIn  EXT_PIN_MD7_C;   // -> P25.RAKU

  //----------
  // VRAM control pins

  /* PIN_43 */ PinOut EXT_PIN_MCSn_A;   // <- P25.SOKY
  /* PIN_43 */ PinOut EXT_PIN_MCSn_D;   // <- P25.SETY

  /* PIN_45 */ PinOut EXT_PIN_MOEn_A;   // <- P25.REFO
  /* PIN_45 */ PinOut EXT_PIN_MOEn_D;   // <- P25.SAHA

  /* PIN_49 */ PinOut EXT_PIN_MWRn_A;   // <- P25.SYSY
  /* PIN_49 */ PinOut EXT_PIN_MWRn_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ PinOut EXT_PIN_MA00_AD;  // <- P04.ECAL
  /* PIN_35 */ PinOut EXT_PIN_MA01_AD;  // <- P04.EGEZ
  /* PIN_36 */ PinOut EXT_PIN_MA02_AD;  // <- P04.FUHE
  /* PIN_37 */ PinOut EXT_PIN_MA03_AD;  // <- P04.FYZY
  /* PIN_38 */ PinOut EXT_PIN_MA04_AD;  // <- P04.DAMU
  /* PIN_39 */ PinOut EXT_PIN_MA05_AD;  // <- P04.DAVA
  /* PIN_40 */ PinOut EXT_PIN_MA06_AD;  // <- P04.ETEG
  /* PIN_41 */ PinOut EXT_PIN_MA07_AD;  // <- P04.EREW
  /* PIN_48 */ PinOut EXT_PIN_MA08_AD;  // <- P04.EVAX
  /* PIN_47 */ PinOut EXT_PIN_MA09_AD;  // <- P04.DUVE
  /* PIN_44 */ PinOut EXT_PIN_MA10_AD;  // <- P04.ERAF
  /* PIN_46 */ PinOut EXT_PIN_MA11_AD;  // <- P04.FUSY
  /* PIN_42 */ PinOut EXT_PIN_MA12_AD;  // <- P04.EXYF

  //----------
  // VRAM data pins

  /* PIN_33 */ PinOut EXT_PIN_MD0_A;   // <- P25.REGE
  /* PIN_31 */ PinOut EXT_PIN_MD1_A;   // <- P25.RYKY
  /* PIN_30 */ PinOut EXT_PIN_MD2_A;   // <- P25.RAZO
  /* PIN_29 */ PinOut EXT_PIN_MD3_A;   // <- P25.RADA
  /* PIN_28 */ PinOut EXT_PIN_MD4_A;   // <- P25.RYRO
  /* PIN_27 */ PinOut EXT_PIN_MD5_A;   // <- P25.REVU
  /* PIN_26 */ PinOut EXT_PIN_MD6_A;   // <- P25.REKU
  /* PIN_25 */ PinOut EXT_PIN_MD7_A;   // <- P25.RYZE

  /* PIN_33 */ PinOut EXT_PIN_MD0_B;   // <- P25.ROFA
  /* PIN_31 */ PinOut EXT_PIN_MD1_B;   // <- P25.ROFA
  /* PIN_30 */ PinOut EXT_PIN_MD2_B;   // <- P25.ROFA
  /* PIN_29 */ PinOut EXT_PIN_MD3_B;   // <- P25.ROFA
  /* PIN_28 */ PinOut EXT_PIN_MD4_B;   // <- P25.ROFA
  /* PIN_27 */ PinOut EXT_PIN_MD5_B;   // <- P25.ROFA
  /* PIN_26 */ PinOut EXT_PIN_MD6_B;   // <- P25.ROFA
  /* PIN_25 */ PinOut EXT_PIN_MD7_B;   // <- P25.ROFA

  /* PIN_33 */ PinOut EXT_PIN_MD0_D;   // <- P25.RURA
  /* PIN_31 */ PinOut EXT_PIN_MD1_D;   // <- P25.RULY
  /* PIN_30 */ PinOut EXT_PIN_MD2_D;   // <- P25.RARE
  /* PIN_29 */ PinOut EXT_PIN_MD3_D;   // <- P25.RODU
  /* PIN_28 */ PinOut EXT_PIN_MD4_D;   // <- P25.RUBE
  /* PIN_27 */ PinOut EXT_PIN_MD5_D;   // <- P25.RUMU
  /* PIN_26 */ PinOut EXT_PIN_MD6_D;   // <- P25.RYTY
  /* PIN_25 */ PinOut EXT_PIN_MD7_D;   // <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics