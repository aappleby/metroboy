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
    _VRAM_PIN_MD0_C.hold(data & 0x01);
    _VRAM_PIN_MD1_C.hold(data & 0x02);
    _VRAM_PIN_MD2_C.hold(data & 0x04);
    _VRAM_PIN_MD3_C.hold(data & 0x08);
    _VRAM_PIN_MD4_C.hold(data & 0x10);
    _VRAM_PIN_MD5_C.hold(data & 0x20);
    _VRAM_PIN_MD6_C.hold(data & 0x40);
    _VRAM_PIN_MD7_C.hold(data & 0x80);
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

  Pin2 _VRAM_PIN_MCSn_A = Pin2::HIZ_NP;   // PIN_43 <- P25.SOKY
  Pin2 _VRAM_PIN_MCSn_C = Pin2::HOLD_0;   // PIN_43 -> P25.TEFY
  Pin2 _VRAM_PIN_MCSn_D = Pin2::HIZ_NP;   // PIN_43 <- P25.SETY

  Pin2 _VRAM_PIN_MOEn_A = Pin2::HIZ_NP;   // PIN_45 <- P25.REFO
  Pin2 _VRAM_PIN_MOEn_C = Pin2::HOLD_0;   // PIN_45 -> P25.TAVY
  Pin2 _VRAM_PIN_MOEn_D = Pin2::HIZ_NP;   // PIN_45 <- P25.SAHA

  Pin2 _VRAM_PIN_MWRn_A = Pin2::HIZ_NP;   // PIN_49 <- P25.SYSY
  Pin2 _VRAM_PIN_MWRn_C = Pin2::HOLD_0;   // PIN_49 -> P25.SUDO
  Pin2 _VRAM_PIN_MWRn_D = Pin2::HIZ_NP;   // PIN_49 <- P25.RAGU

  Pin2 _VRAM_PIN_MA00_AD = Pin2::HIZ_NP;  // PIN_34 <- P04.ECAL
  Pin2 _VRAM_PIN_MA01_AD = Pin2::HIZ_NP;  // PIN_35 <- P04.EGEZ
  Pin2 _VRAM_PIN_MA02_AD = Pin2::HIZ_NP;  // PIN_36 <- P04.FUHE
  Pin2 _VRAM_PIN_MA03_AD = Pin2::HIZ_NP;  // PIN_37 <- P04.FYZY
  Pin2 _VRAM_PIN_MA04_AD = Pin2::HIZ_NP;  // PIN_38 <- P04.DAMU
  Pin2 _VRAM_PIN_MA05_AD = Pin2::HIZ_NP;  // PIN_39 <- P04.DAVA
  Pin2 _VRAM_PIN_MA06_AD = Pin2::HIZ_NP;  // PIN_40 <- P04.ETEG
  Pin2 _VRAM_PIN_MA07_AD = Pin2::HIZ_NP;  // PIN_41 <- P04.EREW
  Pin2 _VRAM_PIN_MA08_AD = Pin2::HIZ_NP;  // PIN_48 <- P04.EVAX
  Pin2 _VRAM_PIN_MA09_AD = Pin2::HIZ_NP;  // PIN_47 <- P04.DUVE
  Pin2 _VRAM_PIN_MA10_AD = Pin2::HIZ_NP;  // PIN_44 <- P04.ERAF
  Pin2 _VRAM_PIN_MA11_AD = Pin2::HIZ_NP;  // PIN_46 <- P04.FUSY
  Pin2 _VRAM_PIN_MA12_AD = Pin2::HIZ_NP;  // PIN_42 <- P04.EXYF

  Pin2 _VRAM_PIN_MD0_A = Pin2::HIZ_NP;    // PIN_33 <- P25.REGE
  Pin2 _VRAM_PIN_MD1_A = Pin2::HIZ_NP;    // PIN_31 <- P25.RYKY
  Pin2 _VRAM_PIN_MD2_A = Pin2::HIZ_NP;    // PIN_30 <- P25.RAZO
  Pin2 _VRAM_PIN_MD3_A = Pin2::HIZ_NP;    // PIN_29 <- P25.RADA
  Pin2 _VRAM_PIN_MD4_A = Pin2::HIZ_NP;    // PIN_28 <- P25.RYRO
  Pin2 _VRAM_PIN_MD5_A = Pin2::HIZ_NP;    // PIN_27 <- P25.REVU
  Pin2 _VRAM_PIN_MD6_A = Pin2::HIZ_NP;    // PIN_26 <- P25.REKU
  Pin2 _VRAM_PIN_MD7_A = Pin2::HIZ_NP;    // PIN_25 <- P25.RYZE

  Pin2 _VRAM_PIN_MD0_B = Pin2::HIZ_NP;    // PIN_33 <- P25.ROFA
  Pin2 _VRAM_PIN_MD1_B = Pin2::HIZ_NP;    // PIN_31 <- P25.ROFA
  Pin2 _VRAM_PIN_MD2_B = Pin2::HIZ_NP;    // PIN_30 <- P25.ROFA
  Pin2 _VRAM_PIN_MD3_B = Pin2::HIZ_NP;    // PIN_29 <- P25.ROFA
  Pin2 _VRAM_PIN_MD4_B = Pin2::HIZ_NP;    // PIN_28 <- P25.ROFA
  Pin2 _VRAM_PIN_MD5_B = Pin2::HIZ_NP;    // PIN_27 <- P25.ROFA
  Pin2 _VRAM_PIN_MD6_B = Pin2::HIZ_NP;    // PIN_26 <- P25.ROFA
  Pin2 _VRAM_PIN_MD7_B = Pin2::HIZ_NP;    // PIN_25 <- P25.ROFA

  Pin2 _VRAM_PIN_MD0_C = Pin2::HOLD_0;    // PIN_33 -> P25.RODY
  Pin2 _VRAM_PIN_MD1_C = Pin2::HOLD_0;    // PIN_31 -> P25.REBA
  Pin2 _VRAM_PIN_MD2_C = Pin2::HOLD_0;    // PIN_30 -> P25.RYDO
  Pin2 _VRAM_PIN_MD3_C = Pin2::HOLD_0;    // PIN_29 -> P25.REMO
  Pin2 _VRAM_PIN_MD4_C = Pin2::HOLD_0;    // PIN_28 -> P25.ROCE
  Pin2 _VRAM_PIN_MD5_C = Pin2::HOLD_0;    // PIN_27 -> P25.ROPU
  Pin2 _VRAM_PIN_MD6_C = Pin2::HOLD_0;    // PIN_26 -> P25.RETA
  Pin2 _VRAM_PIN_MD7_C = Pin2::HOLD_0;    // PIN_25 -> P25.RAKU

  Pin2 _VRAM_PIN_MD0_D = Pin2::HIZ_NP;    // PIN_33 <- P25.RURA
  Pin2 _VRAM_PIN_MD1_D = Pin2::HIZ_NP;    // PIN_31 <- P25.RULY
  Pin2 _VRAM_PIN_MD2_D = Pin2::HIZ_NP;    // PIN_30 <- P25.RARE
  Pin2 _VRAM_PIN_MD3_D = Pin2::HIZ_NP;    // PIN_29 <- P25.RODU
  Pin2 _VRAM_PIN_MD4_D = Pin2::HIZ_NP;    // PIN_28 <- P25.RUBE
  Pin2 _VRAM_PIN_MD5_D = Pin2::HIZ_NP;    // PIN_27 <- P25.RUMU
  Pin2 _VRAM_PIN_MD6_D = Pin2::HIZ_NP;    // PIN_26 <- P25.RYTY
  Pin2 _VRAM_PIN_MD7_D = Pin2::HIZ_NP;    // PIN_25 <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics