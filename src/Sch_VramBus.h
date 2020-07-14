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
    _VRAM_PIN_MD0_C.set(data & 0x01);
    _VRAM_PIN_MD1_C.set(data & 0x02);
    _VRAM_PIN_MD2_C.set(data & 0x04);
    _VRAM_PIN_MD3_C.set(data & 0x08);
    _VRAM_PIN_MD4_C.set(data & 0x10);
    _VRAM_PIN_MD5_C.set(data & 0x20);
    _VRAM_PIN_MD6_C.set(data & 0x40);
    _VRAM_PIN_MD7_C.set(data & 0x80);
  }

  /*p25.TEFY*/ wire TEFY_VRAM_MCSp() const { return not(_VRAM_PIN_MCSn_C); }

  wire VRAM_PIN_MWRn_C() const { return _VRAM_PIN_MWRn_C.get(); }
  wire VRAM_PIN_MOEn_C() const { return _VRAM_PIN_MOEn_C.get(); }
  wire VRAM_PIN_MCSn_C() const { return _VRAM_PIN_MCSn_C.get(); }

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

  Tribuf _VRM_TRI_D0;
  Tribuf _VRM_TRI_D1;
  Tribuf _VRM_TRI_D2;
  Tribuf _VRM_TRI_D3;
  Tribuf _VRM_TRI_D4;
  Tribuf _VRM_TRI_D5;
  Tribuf _VRM_TRI_D6;
  Tribuf _VRM_TRI_D7;

  Tribuf _VRM_TRI_A00;
  Tribuf _VRM_TRI_A01;
  Tribuf _VRM_TRI_A02;
  Tribuf _VRM_TRI_A03;
  Tribuf _VRM_TRI_A04;
  Tribuf _VRM_TRI_A05;
  Tribuf _VRM_TRI_A06;
  Tribuf _VRM_TRI_A07;
  Tribuf _VRM_TRI_A08;
  Tribuf _VRM_TRI_A09;
  Tribuf _VRM_TRI_A10;
  Tribuf _VRM_TRI_A11;
  Tribuf _VRM_TRI_A12;

  //-----------------------------------------------------------------------------
  // VRAM bus

  ExtPinOut _VRAM_PIN_MCSn_A;   // PIN_43 <- P25.SOKY
  ExtPinIn  _VRAM_PIN_MCSn_C;   // PIN_43 -> P25.TEFY
  ExtPinOut _VRAM_PIN_MCSn_D;   // PIN_43 <- P25.SETY

  ExtPinOut _VRAM_PIN_MOEn_A;   // PIN_45 <- P25.REFO
  ExtPinIn  _VRAM_PIN_MOEn_C;   // PIN_45 -> P25.TAVY
  ExtPinOut _VRAM_PIN_MOEn_D;   // PIN_45 <- P25.SAHA

  ExtPinOut _VRAM_PIN_MWRn_A;   // PIN_49 <- P25.SYSY
  ExtPinIn  _VRAM_PIN_MWRn_C;   // PIN_49 -> P25.SUDO
  ExtPinOut _VRAM_PIN_MWRn_D;   // PIN_49 <- P25.RAGU

  ExtPinOut _VRAM_PIN_MA00_AD;  // PIN_34 <- P04.ECAL
  ExtPinOut _VRAM_PIN_MA01_AD;  // PIN_35 <- P04.EGEZ
  ExtPinOut _VRAM_PIN_MA02_AD;  // PIN_36 <- P04.FUHE
  ExtPinOut _VRAM_PIN_MA03_AD;  // PIN_37 <- P04.FYZY
  ExtPinOut _VRAM_PIN_MA04_AD;  // PIN_38 <- P04.DAMU
  ExtPinOut _VRAM_PIN_MA05_AD;  // PIN_39 <- P04.DAVA
  ExtPinOut _VRAM_PIN_MA06_AD;  // PIN_40 <- P04.ETEG
  ExtPinOut _VRAM_PIN_MA07_AD;  // PIN_41 <- P04.EREW
  ExtPinOut _VRAM_PIN_MA08_AD;  // PIN_48 <- P04.EVAX
  ExtPinOut _VRAM_PIN_MA09_AD;  // PIN_47 <- P04.DUVE
  ExtPinOut _VRAM_PIN_MA10_AD;  // PIN_44 <- P04.ERAF
  ExtPinOut _VRAM_PIN_MA11_AD;  // PIN_46 <- P04.FUSY
  ExtPinOut _VRAM_PIN_MA12_AD;  // PIN_42 <- P04.EXYF

  ExtPinOut _VRAM_PIN_MD0_A;    // PIN_33 <- P25.REGE
  ExtPinOut _VRAM_PIN_MD1_A;    // PIN_31 <- P25.RYKY
  ExtPinOut _VRAM_PIN_MD2_A;    // PIN_30 <- P25.RAZO
  ExtPinOut _VRAM_PIN_MD3_A;    // PIN_29 <- P25.RADA
  ExtPinOut _VRAM_PIN_MD4_A;    // PIN_28 <- P25.RYRO
  ExtPinOut _VRAM_PIN_MD5_A;    // PIN_27 <- P25.REVU
  ExtPinOut _VRAM_PIN_MD6_A;    // PIN_26 <- P25.REKU
  ExtPinOut _VRAM_PIN_MD7_A;    // PIN_25 <- P25.RYZE

  ExtPinOut _VRAM_PIN_MD0_B;    // PIN_33 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD1_B;    // PIN_31 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD2_B;    // PIN_30 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD3_B;    // PIN_29 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD4_B;    // PIN_28 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD5_B;    // PIN_27 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD6_B;    // PIN_26 <- P25.ROFA
  ExtPinOut _VRAM_PIN_MD7_B;    // PIN_25 <- P25.ROFA

  ExtPinIn  _VRAM_PIN_MD0_C;    // PIN_33 -> P25.RODY
  ExtPinIn  _VRAM_PIN_MD1_C;    // PIN_31 -> P25.REBA
  ExtPinIn  _VRAM_PIN_MD2_C;    // PIN_30 -> P25.RYDO
  ExtPinIn  _VRAM_PIN_MD3_C;    // PIN_29 -> P25.REMO
  ExtPinIn  _VRAM_PIN_MD4_C;    // PIN_28 -> P25.ROCE
  ExtPinIn  _VRAM_PIN_MD5_C;    // PIN_27 -> P25.ROPU
  ExtPinIn  _VRAM_PIN_MD6_C;    // PIN_26 -> P25.RETA
  ExtPinIn  _VRAM_PIN_MD7_C;    // PIN_25 -> P25.RAKU

  ExtPinOut _VRAM_PIN_MD0_D;    // PIN_33 <- P25.RURA
  ExtPinOut _VRAM_PIN_MD1_D;    // PIN_31 <- P25.RULY
  ExtPinOut _VRAM_PIN_MD2_D;    // PIN_30 <- P25.RARE
  ExtPinOut _VRAM_PIN_MD3_D;    // PIN_29 <- P25.RODU
  ExtPinOut _VRAM_PIN_MD4_D;    // PIN_28 <- P25.RUBE
  ExtPinOut _VRAM_PIN_MD5_D;    // PIN_27 <- P25.RUMU
  ExtPinOut _VRAM_PIN_MD6_D;    // PIN_26 <- P25.RYTY
  ExtPinOut _VRAM_PIN_MD7_D;    // PIN_25 <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics