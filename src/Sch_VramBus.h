#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct VramBus {
  VramBus();
  void tock(SchematicTop& top);
  SignalHash commit();

  void set_vram_data(uint8_t data) {
    _VRAM_PIN_MD0_C.set_pin_in(data & 0x01);
    _VRAM_PIN_MD1_C.set_pin_in(data & 0x02);
    _VRAM_PIN_MD2_C.set_pin_in(data & 0x04);
    _VRAM_PIN_MD3_C.set_pin_in(data & 0x08);
    _VRAM_PIN_MD4_C.set_pin_in(data & 0x10);
    _VRAM_PIN_MD5_C.set_pin_in(data & 0x20);
    _VRAM_PIN_MD6_C.set_pin_in(data & 0x40);
    _VRAM_PIN_MD7_C.set_pin_in(data & 0x80);
  }

  // -> oam data tri
  /*p25.RERY*/ wire VRM_TRI_D0() const { return _VRM_TRI_D0.q(); }
  /*p25.RUNA*/ wire VRM_TRI_D1() const { return _VRM_TRI_D1.q(); }
  /*p25.RONA*/ wire VRM_TRI_D2() const { return _VRM_TRI_D2.q(); }
  /*p25.RUNO*/ wire VRM_TRI_D3() const { return _VRM_TRI_D3.q(); }
  /*p25.SANA*/ wire VRM_TRI_D4() const { return _VRM_TRI_D4.q(); }
  /*p25.RORO*/ wire VRM_TRI_D5() const { return _VRM_TRI_D5.q(); }
  /*p25.RABO*/ wire VRM_TRI_D6() const { return _VRM_TRI_D6.q(); }
  /*p25.SAME*/ wire VRM_TRI_D7() const { return _VRM_TRI_D7.q(); }

private:

  Reg _VRM_TRI_D0;
  Reg _VRM_TRI_D1;
  Reg _VRM_TRI_D2;
  Reg _VRM_TRI_D3;
  Reg _VRM_TRI_D4;
  Reg _VRM_TRI_D5;
  Reg _VRM_TRI_D6;
  Reg _VRM_TRI_D7;

  Reg _VRM_TRI_A00;
  Reg _VRM_TRI_A01;
  Reg _VRM_TRI_A02;
  Reg _VRM_TRI_A03;
  Reg _VRM_TRI_A04;
  Reg _VRM_TRI_A05;
  Reg _VRM_TRI_A06;
  Reg _VRM_TRI_A07;
  Reg _VRM_TRI_A08;
  Reg _VRM_TRI_A09;
  Reg _VRM_TRI_A10;
  Reg _VRM_TRI_A11;
  Reg _VRM_TRI_A12;

  //-----------------------------------------------------------------------------
  // VRAM bus

  Reg _VRAM_PIN_MCSn_A;   // PIN_43 <- P25.SOKY
  ExtPinIn  _VRAM_PIN_MCSn_C;   // PIN_43 -> P25.TEFY
  Reg _VRAM_PIN_MCSn_D;   // PIN_43 <- P25.SETY

  Reg _VRAM_PIN_MOEn_A;   // PIN_45 <- P25.REFO
  ExtPinIn  _VRAM_PIN_MOEn_C;   // PIN_45 -> P25.TAVY
  Reg _VRAM_PIN_MOEn_D;   // PIN_45 <- P25.SAHA

  Reg _VRAM_PIN_MWRn_A;   // PIN_49 <- P25.SYSY
  ExtPinIn  _VRAM_PIN_MWRn_C;   // PIN_49 -> P25.SUDO
  Reg _VRAM_PIN_MWRn_D;   // PIN_49 <- P25.RAGU

  Reg _VRAM_PIN_MA00_AD;  // PIN_34 <- P04.ECAL
  Reg _VRAM_PIN_MA01_AD;  // PIN_35 <- P04.EGEZ
  Reg _VRAM_PIN_MA02_AD;  // PIN_36 <- P04.FUHE
  Reg _VRAM_PIN_MA03_AD;  // PIN_37 <- P04.FYZY
  Reg _VRAM_PIN_MA04_AD;  // PIN_38 <- P04.DAMU
  Reg _VRAM_PIN_MA05_AD;  // PIN_39 <- P04.DAVA
  Reg _VRAM_PIN_MA06_AD;  // PIN_40 <- P04.ETEG
  Reg _VRAM_PIN_MA07_AD;  // PIN_41 <- P04.EREW
  Reg _VRAM_PIN_MA08_AD;  // PIN_48 <- P04.EVAX
  Reg _VRAM_PIN_MA09_AD;  // PIN_47 <- P04.DUVE
  Reg _VRAM_PIN_MA10_AD;  // PIN_44 <- P04.ERAF
  Reg _VRAM_PIN_MA11_AD;  // PIN_46 <- P04.FUSY
  Reg _VRAM_PIN_MA12_AD;  // PIN_42 <- P04.EXYF

  Reg _VRAM_PIN_MD0_A;    // PIN_33 <- P25.REGE
  Reg _VRAM_PIN_MD1_A;    // PIN_31 <- P25.RYKY
  Reg _VRAM_PIN_MD2_A;    // PIN_30 <- P25.RAZO
  Reg _VRAM_PIN_MD3_A;    // PIN_29 <- P25.RADA
  Reg _VRAM_PIN_MD4_A;    // PIN_28 <- P25.RYRO
  Reg _VRAM_PIN_MD5_A;    // PIN_27 <- P25.REVU
  Reg _VRAM_PIN_MD6_A;    // PIN_26 <- P25.REKU
  Reg _VRAM_PIN_MD7_A;    // PIN_25 <- P25.RYZE

  Reg _VRAM_PIN_MD0_B;    // PIN_33 <- P25.ROFA
  Reg _VRAM_PIN_MD1_B;    // PIN_31 <- P25.ROFA
  Reg _VRAM_PIN_MD2_B;    // PIN_30 <- P25.ROFA
  Reg _VRAM_PIN_MD3_B;    // PIN_29 <- P25.ROFA
  Reg _VRAM_PIN_MD4_B;    // PIN_28 <- P25.ROFA
  Reg _VRAM_PIN_MD5_B;    // PIN_27 <- P25.ROFA
  Reg _VRAM_PIN_MD6_B;    // PIN_26 <- P25.ROFA
  Reg _VRAM_PIN_MD7_B;    // PIN_25 <- P25.ROFA

  ExtPinIn  _VRAM_PIN_MD0_C;    // PIN_33 -> P25.RODY
  ExtPinIn  _VRAM_PIN_MD1_C;    // PIN_31 -> P25.REBA
  ExtPinIn  _VRAM_PIN_MD2_C;    // PIN_30 -> P25.RYDO
  ExtPinIn  _VRAM_PIN_MD3_C;    // PIN_29 -> P25.REMO
  ExtPinIn  _VRAM_PIN_MD4_C;    // PIN_28 -> P25.ROCE
  ExtPinIn  _VRAM_PIN_MD5_C;    // PIN_27 -> P25.ROPU
  ExtPinIn  _VRAM_PIN_MD6_C;    // PIN_26 -> P25.RETA
  ExtPinIn  _VRAM_PIN_MD7_C;    // PIN_25 -> P25.RAKU

  Reg _VRAM_PIN_MD0_D;    // PIN_33 <- P25.RURA
  Reg _VRAM_PIN_MD1_D;    // PIN_31 <- P25.RULY
  Reg _VRAM_PIN_MD2_D;    // PIN_30 <- P25.RARE
  Reg _VRAM_PIN_MD3_D;    // PIN_29 <- P25.RODU
  Reg _VRAM_PIN_MD4_D;    // PIN_28 <- P25.RUBE
  Reg _VRAM_PIN_MD5_D;    // PIN_27 <- P25.RUMU
  Reg _VRAM_PIN_MD6_D;    // PIN_26 <- P25.RYTY
  Reg _VRAM_PIN_MD7_D;    // PIN_25 <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics