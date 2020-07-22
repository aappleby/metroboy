#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct VramBus {
  void tock(SchematicTop& top);

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
  /*p25.RERY*/ wire VRM_TRI_D0() const { return _VRAM_TRI_D0.q(); }
  /*p25.RUNA*/ wire VRM_TRI_D1() const { return _VRAM_TRI_D1.q(); }
  /*p25.RONA*/ wire VRM_TRI_D2() const { return _VRAM_TRI_D2.q(); }
  /*p25.RUNO*/ wire VRM_TRI_D3() const { return _VRAM_TRI_D3.q(); }
  /*p25.SANA*/ wire VRM_TRI_D4() const { return _VRAM_TRI_D4.q(); }
  /*p25.RORO*/ wire VRM_TRI_D5() const { return _VRAM_TRI_D5.q(); }
  /*p25.RABO*/ wire VRM_TRI_D6() const { return _VRAM_TRI_D6.q(); }
  /*p25.SAME*/ wire VRM_TRI_D7() const { return _VRAM_TRI_D7.q(); }

  /*p32.LEGU*/ Reg2 LEGU_TILE_DA0 = Reg2::D0C0;
  /*p32.NUDU*/ Reg2 NUDU_TILE_DA1 = Reg2::D0C0;
  /*p32.MUKU*/ Reg2 MUKU_TILE_DA2 = Reg2::D0C0;
  /*p32.LUZO*/ Reg2 LUZO_TILE_DA3 = Reg2::D0C0;
  /*p32.MEGU*/ Reg2 MEGU_TILE_DA4 = Reg2::D0C0;
  /*p32.MYJY*/ Reg2 MYJY_TILE_DA5 = Reg2::D0C0;
  /*p32.NASA*/ Reg2 NASA_TILE_DA6 = Reg2::D0C0;
  /*p32.NEFO*/ Reg2 NEFO_TILE_DA7 = Reg2::D0C0; // color wrong on die

  /*p32.RAWU*/ Reg2 RAWU_TILE_DB0 = Reg2::D0C0; // also holds tile index during fetch
  /*p32.POZO*/ Reg2 POZO_TILE_DB1 = Reg2::D0C0;
  /*p32.PYZO*/ Reg2 PYZO_TILE_DB2 = Reg2::D0C0; 
  /*p32.POXA*/ Reg2 POXA_TILE_DB3 = Reg2::D0C0; 
  /*p32.PULO*/ Reg2 PULO_TILE_DB4 = Reg2::D0C0; 
  /*p32.POJU*/ Reg2 POJU_TILE_DB5 = Reg2::D0C0; 
  /*p32.POWY*/ Reg2 POWY_TILE_DB6 = Reg2::D0C0; 
  /*p32.PYJU*/ Reg2 PYJU_TILE_DB7 = Reg2::D0C0;

  /*p33.PEFO*/ Reg2 PEFO_SPRITE_DA0 = Reg2::D0C0;
  /*p33.ROKA*/ Reg2 ROKA_SPRITE_DA1 = Reg2::D0C0;
  /*p33.MYTU*/ Reg2 MYTU_SPRITE_DA2 = Reg2::D0C0;
  /*p33.RAMU*/ Reg2 RAMU_SPRITE_DA3 = Reg2::D0C0;
  /*p33.SELE*/ Reg2 SELE_SPRITE_DA4 = Reg2::D0C0;
  /*p33.SUTO*/ Reg2 SUTO_SPRITE_DA5 = Reg2::D0C0;
  /*p33.RAMA*/ Reg2 RAMA_SPRITE_DA6 = Reg2::D0C0;
  /*p33.RYDU*/ Reg2 RYDU_SPRITE_DA7 = Reg2::D0C0;

  /*p33.REWO*/ Reg2 REWO_SPRITE_DB0 = Reg2::D0C0;
  /*p33.PEBA*/ Reg2 PEBA_SPRITE_DB1 = Reg2::D0C0;
  /*p33.MOFO*/ Reg2 MOFO_SPRITE_DB2 = Reg2::D0C0;
  /*p33.PUDU*/ Reg2 PUDU_SPRITE_DB3 = Reg2::D0C0;
  /*p33.SAJA*/ Reg2 SAJA_SPRITE_DB4 = Reg2::D0C0;
  /*p33.SUNY*/ Reg2 SUNY_SPRITE_DB5 = Reg2::D0C0;
  /*p33.SEMO*/ Reg2 SEMO_SPRITE_DB6 = Reg2::D0C0;
  /*p33.SEGA*/ Reg2 SEGA_SPRITE_DB7 = Reg2::D0C0;

  int bus_addr() const {
    return pack(_VRAM_TRI_A00, _VRAM_TRI_A01, _VRAM_TRI_A02, _VRAM_TRI_A03,
                _VRAM_TRI_A04, _VRAM_TRI_A05, _VRAM_TRI_A06, _VRAM_TRI_A07,
                _VRAM_TRI_A08, _VRAM_TRI_A09, _VRAM_TRI_A10, _VRAM_TRI_A11,
                _VRAM_TRI_A12, 0, 0, 0);
  }

  int bus_data() const {
    return pack(_VRAM_TRI_D0, _VRAM_TRI_D1, _VRAM_TRI_D2, _VRAM_TRI_D3,
                _VRAM_TRI_D4, _VRAM_TRI_D5, _VRAM_TRI_D6, _VRAM_TRI_D7);
  }

  void dump(Dumper& d) {
    /*
    int addr = pack(_VRAM_PIN_MA00_AD, _VRAM_PIN_MA01_AD, _VRAM_PIN_MA02_AD, _VRAM_PIN_MA03_AD,
                    _VRAM_PIN_MA04_AD, _VRAM_PIN_MA05_AD, _VRAM_PIN_MA06_AD, _VRAM_PIN_MA07_AD,
                    _VRAM_PIN_MA08_AD, _VRAM_PIN_MA09_AD, _VRAM_PIN_MA10_AD, _VRAM_PIN_MA11_AD,
                    _VRAM_PIN_MA12_AD, 0, 0, 0);
    int data_a = pack(_VRAM_PIN_MD0_A, _VRAM_PIN_MD1_A, _VRAM_PIN_MD2_A, _VRAM_PIN_MD3_A,
                      _VRAM_PIN_MD4_A, _VRAM_PIN_MD5_A, _VRAM_PIN_MD6_A, _VRAM_PIN_MD7_A);
    int data_b = pack(_VRAM_PIN_MD0_B, _VRAM_PIN_MD1_B, _VRAM_PIN_MD2_B, _VRAM_PIN_MD3_B,
                      _VRAM_PIN_MD4_B, _VRAM_PIN_MD5_B, _VRAM_PIN_MD6_B, _VRAM_PIN_MD7_B);
    int data_c = pack(_VRAM_PIN_MD0_C, _VRAM_PIN_MD1_C, _VRAM_PIN_MD2_C, _VRAM_PIN_MD3_C,
                      _VRAM_PIN_MD4_C, _VRAM_PIN_MD5_C, _VRAM_PIN_MD6_C, _VRAM_PIN_MD7_C);
    int data_d = pack(_VRAM_PIN_MD0_D, _VRAM_PIN_MD1_D, _VRAM_PIN_MD2_D, _VRAM_PIN_MD3_D,
                      _VRAM_PIN_MD4_D, _VRAM_PIN_MD5_D, _VRAM_PIN_MD6_D, _VRAM_PIN_MD7_D);
    addr = (~addr) & 0x1FFF;
    */


    d("---------- VRAM Bus ----------\n");
    d("VRAM BUS ADDR    : %c%c%c%c%c%c%c%c%c%c%c%c%c\n",
      _VRAM_TRI_A12.c(), _VRAM_TRI_A11.c(), _VRAM_TRI_A10.c(), _VRAM_TRI_A09.c(),
      _VRAM_TRI_A08.c(), _VRAM_TRI_A07.c(), _VRAM_TRI_A06.c(), _VRAM_TRI_A05.c(),
      _VRAM_TRI_A04.c(), _VRAM_TRI_A03.c(), _VRAM_TRI_A02.c(), _VRAM_TRI_A01.c(),
      _VRAM_TRI_A00.c());
    d("VRAM BUS DATA    : %c%c%c%c%c%c%c%c\n",
      _VRAM_TRI_D7.c(), _VRAM_TRI_D6.c(), _VRAM_TRI_D5.c(), _VRAM_TRI_D4.c(),
      _VRAM_TRI_D3.c(), _VRAM_TRI_D2.c(), _VRAM_TRI_D1.c(), _VRAM_TRI_D0.c());

    d("VRAM PIN MCS     : %c%c%c\n", _VRAM_PIN_MCSn_A.c(), _VRAM_PIN_MCSn_C.c(), _VRAM_PIN_MCSn_D.c());
    d("VRAM PIN MOE     : %c%c%c\n", _VRAM_PIN_MOEn_A.c(), _VRAM_PIN_MOEn_C.c(), _VRAM_PIN_MOEn_D.c());
    d("VRAM PIN MWR     : %c%c%c\n", _VRAM_PIN_MWRn_A.c(), _VRAM_PIN_MWRn_C.c(), _VRAM_PIN_MWRn_D.c());

    d("VRAM PIN ADDR    : %c%c%c%c%c%c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_MA12_AD.c(), _VRAM_PIN_MA11_AD.c(), _VRAM_PIN_MA10_AD.c(), _VRAM_PIN_MA09_AD.c(),
      _VRAM_PIN_MA08_AD.c(), _VRAM_PIN_MA07_AD.c(), _VRAM_PIN_MA06_AD.c(), _VRAM_PIN_MA05_AD.c(),
      _VRAM_PIN_MA04_AD.c(), _VRAM_PIN_MA03_AD.c(), _VRAM_PIN_MA02_AD.c(), _VRAM_PIN_MA01_AD.c(),
      _VRAM_PIN_MA00_AD.c());

    d("VRAM PIN DATA A  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_MD7_A.c(), _VRAM_PIN_MD6_A.c(), _VRAM_PIN_MD5_A.c(), _VRAM_PIN_MD4_A.c(),
      _VRAM_PIN_MD3_A.c(), _VRAM_PIN_MD2_A.c(), _VRAM_PIN_MD1_A.c(), _VRAM_PIN_MD0_A.c());
    d("VRAM PIN DATA B  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_MD7_B.c(), _VRAM_PIN_MD6_B.c(), _VRAM_PIN_MD5_B.c(), _VRAM_PIN_MD4_B.c(),
      _VRAM_PIN_MD3_B.c(), _VRAM_PIN_MD2_B.c(), _VRAM_PIN_MD1_B.c(), _VRAM_PIN_MD0_B.c());
    d("VRAM PIN DATA C  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_MD7_C.c(), _VRAM_PIN_MD6_C.c(), _VRAM_PIN_MD5_C.c(), _VRAM_PIN_MD4_C.c(),
      _VRAM_PIN_MD3_C.c(), _VRAM_PIN_MD2_C.c(), _VRAM_PIN_MD1_C.c(), _VRAM_PIN_MD0_C.c());
    d("VRAM PIN DATA D  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_MD7_D.c(), _VRAM_PIN_MD6_D.c(), _VRAM_PIN_MD5_D.c(), _VRAM_PIN_MD4_D.c(),
      _VRAM_PIN_MD3_D.c(), _VRAM_PIN_MD2_D.c(), _VRAM_PIN_MD1_D.c(), _VRAM_PIN_MD0_D.c());
    d("\n");
  }


private:

  Pin2 _VRAM_TRI_D0 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D1 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D2 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D3 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D4 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D5 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D6 = Pin2::HIZ_PU;
  Pin2 _VRAM_TRI_D7 = Pin2::HIZ_PU;

  Pin2 _VRAM_TRI_A00 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A01 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A02 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A03 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A04 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A05 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A06 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A07 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A08 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A09 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A10 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A11 = Pin2::HIZ_NP;
  Pin2 _VRAM_TRI_A12 = Pin2::HIZ_NP;

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