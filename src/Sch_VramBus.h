#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct VramBus {
  void tock(SchematicTop& top);

  //----------------------------------------

  int get_bus_addr() const {
    return pack(_VRAM_BUS_A00, _VRAM_BUS_A01, _VRAM_BUS_A02, _VRAM_BUS_A03,
                _VRAM_BUS_A04, _VRAM_BUS_A05, _VRAM_BUS_A06, _VRAM_BUS_A07,
                _VRAM_BUS_A08, _VRAM_BUS_A09, _VRAM_BUS_A10, _VRAM_BUS_A11,
                _VRAM_BUS_A12, 0, 0, 0);
  }

  int get_bus_data() const {
    return pack(_VRAM_BUS_D0, _VRAM_BUS_D1, _VRAM_BUS_D2, _VRAM_BUS_D3,
                _VRAM_BUS_D4, _VRAM_BUS_D5, _VRAM_BUS_D6, _VRAM_BUS_D7);
  }

  uint16_t get_pin_addr() const {
    return (uint16_t)pack(!_VRAM_PIN_A00, !_VRAM_PIN_A01, !_VRAM_PIN_A02, !_VRAM_PIN_A03,
                          !_VRAM_PIN_A04, !_VRAM_PIN_A05, !_VRAM_PIN_A06, !_VRAM_PIN_A07,
                          !_VRAM_PIN_A08, !_VRAM_PIN_A09, !_VRAM_PIN_A10, !_VRAM_PIN_A11,
                          !_VRAM_PIN_A12, 0, 0, 0);
  }

  uint8_t get_pin_data_out() const {
    return (uint8_t)pack(!_VRAM_PIN_D0_A, !_VRAM_PIN_D1_A, !_VRAM_PIN_D2_A, !_VRAM_PIN_D3_A,
                         !_VRAM_PIN_D4_A, !_VRAM_PIN_D5_A, !_VRAM_PIN_D6_A, !_VRAM_PIN_D7_A);
  }

  void preset_pin_data_in(uint8_t data) {
    _VRAM_PIN_D0_C.preset(!(data & 0x01));
    _VRAM_PIN_D1_C.preset(!(data & 0x02));
    _VRAM_PIN_D2_C.preset(!(data & 0x04));
    _VRAM_PIN_D3_C.preset(!(data & 0x08));
    _VRAM_PIN_D4_C.preset(!(data & 0x10));
    _VRAM_PIN_D5_C.preset(!(data & 0x20));
    _VRAM_PIN_D6_C.preset(!(data & 0x40));
    _VRAM_PIN_D7_C.preset(!(data & 0x80));
  }

  void preset_pin_data_z() {
    _VRAM_PIN_D0_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D1_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D2_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D3_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D4_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D5_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D6_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D7_C.preset(DELTA_TRIZ);
  }

  //----------------------------------------

  void dump(Dumper& d) const {
    /*
    int addr = pack(_VRAM_PIN_A00_AD, _VRAM_PIN_A01_AD, _VRAM_PIN_A02_AD, _VRAM_PIN_A03_AD,
                    _VRAM_PIN_A04_AD, _VRAM_PIN_A05_AD, _VRAM_PIN_A06_AD, _VRAM_PIN_A07_AD,
                    _VRAM_PIN_A08_AD, _VRAM_PIN_A09_AD, _VRAM_PIN_A10_AD, _VRAM_PIN_A11_AD,
                    _VRAM_PIN_A12_AD, 0, 0, 0);
    int data_a = pack(_VRAM_PIN_D0_A, _VRAM_PIN_D1_A, _VRAM_PIN_D2_A, _VRAM_PIN_D3_A,
                      _VRAM_PIN_D4_A, _VRAM_PIN_D5_A, _VRAM_PIN_D6_A, _VRAM_PIN_D7_A);
    int data_b = pack(_VRAM_PIN_D0_B, _VRAM_PIN_D1_B, _VRAM_PIN_D2_B, _VRAM_PIN_D3_B,
                      _VRAM_PIN_D4_B, _VRAM_PIN_D5_B, _VRAM_PIN_D6_B, _VRAM_PIN_D7_B);
    int data_c = pack(_VRAM_PIN_D0_C, _VRAM_PIN_D1_C, _VRAM_PIN_D2_C, _VRAM_PIN_D3_C,
                      _VRAM_PIN_D4_C, _VRAM_PIN_D5_C, _VRAM_PIN_D6_C, _VRAM_PIN_D7_C);
    int data_d = pack(_VRAM_PIN_D0_D, _VRAM_PIN_D1_D, _VRAM_PIN_D2_D, _VRAM_PIN_D3_D,
                      _VRAM_PIN_D4_D, _VRAM_PIN_D5_D, _VRAM_PIN_D6_D, _VRAM_PIN_D7_D);
    addr = (~addr) & 0x1FFF;
    */


    d("---------- VRAM Bus ----------\n");
    d("VRAM BUS ADDR    : %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      _VRAM_BUS_A12.c(), _VRAM_BUS_A11.c(), _VRAM_BUS_A10.c(), _VRAM_BUS_A09.c(),
      _VRAM_BUS_A08.c(), _VRAM_BUS_A07.c(), _VRAM_BUS_A06.c(), _VRAM_BUS_A05.c(),
      _VRAM_BUS_A04.c(), _VRAM_BUS_A03.c(), _VRAM_BUS_A02.c(), _VRAM_BUS_A01.c(),
      _VRAM_BUS_A00.c());
    d("VRAM BUS DATA    : %c%c%c%c%c%c%c%c\n",
      _VRAM_BUS_D7.c(), _VRAM_BUS_D6.c(), _VRAM_BUS_D5.c(), _VRAM_BUS_D4.c(),
      _VRAM_BUS_D3.c(), _VRAM_BUS_D2.c(), _VRAM_BUS_D1.c(), _VRAM_BUS_D0.c());

    d("VRAM PIN MCS     : %c%c%c\n", _VRAM_PIN_CS_A.c(), _VRAM_PIN_CS_C.c(), _VRAM_PIN_CS_D.c());
    d("VRAM PIN MOE     : %c%c%c\n", _VRAM_PIN_OE_A.c(), _VRAM_PIN_OE_C.c(), _VRAM_PIN_OE_D.c());
    d("VRAM PIN MWR     : %c%c%c\n", _VRAM_PIN_WR_A.c(), _VRAM_PIN_WR_C.c(), _VRAM_PIN_WR_D.c());

    d("VRAM PIN ADDR    : %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_A12.c(), _VRAM_PIN_A11.c(), _VRAM_PIN_A10.c(), _VRAM_PIN_A09.c(),
      _VRAM_PIN_A08.c(), _VRAM_PIN_A07.c(), _VRAM_PIN_A06.c(), _VRAM_PIN_A05.c(),
      _VRAM_PIN_A04.c(), _VRAM_PIN_A03.c(), _VRAM_PIN_A02.c(), _VRAM_PIN_A01.c(),
      _VRAM_PIN_A00.c());

    d("VRAM PIN DATA A  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_D7_A.c(), _VRAM_PIN_D6_A.c(), _VRAM_PIN_D5_A.c(), _VRAM_PIN_D4_A.c(),
      _VRAM_PIN_D3_A.c(), _VRAM_PIN_D2_A.c(), _VRAM_PIN_D1_A.c(), _VRAM_PIN_D0_A.c());
    d("VRAM PIN DATA B  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_D7_B.c(), _VRAM_PIN_D6_B.c(), _VRAM_PIN_D5_B.c(), _VRAM_PIN_D4_B.c(),
      _VRAM_PIN_D3_B.c(), _VRAM_PIN_D2_B.c(), _VRAM_PIN_D1_B.c(), _VRAM_PIN_D0_B.c());
    d("VRAM PIN DATA C  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_D7_C.c(), _VRAM_PIN_D6_C.c(), _VRAM_PIN_D5_C.c(), _VRAM_PIN_D4_C.c(),
      _VRAM_PIN_D3_C.c(), _VRAM_PIN_D2_C.c(), _VRAM_PIN_D1_C.c(), _VRAM_PIN_D0_C.c());
    d("VRAM PIN DATA D  : %c%c%c%c%c%c%c%c\n",
      _VRAM_PIN_D7_D.c(), _VRAM_PIN_D6_D.c(), _VRAM_PIN_D5_D.c(), _VRAM_PIN_D4_D.c(),
      _VRAM_PIN_D3_D.c(), _VRAM_PIN_D2_D.c(), _VRAM_PIN_D1_D.c(), _VRAM_PIN_D0_D.c());
    d("\n");
  }

  //----------------------------------------

  // -> oam data tri
  /*p25.RERY*/ wire VRM_BUS_D0() const { return _VRAM_BUS_D0; }
  /*p25.RUNA*/ wire VRM_BUS_D1() const { return _VRAM_BUS_D1; }
  /*p25.RONA*/ wire VRM_BUS_D2() const { return _VRAM_BUS_D2; }
  /*p25.RUNO*/ wire VRM_BUS_D3() const { return _VRAM_BUS_D3; }
  /*p25.SANA*/ wire VRM_BUS_D4() const { return _VRAM_BUS_D4; }
  /*p25.RORO*/ wire VRM_BUS_D5() const { return _VRAM_BUS_D5; }
  /*p25.RABO*/ wire VRM_BUS_D6() const { return _VRAM_BUS_D6; }
  /*p25.SAME*/ wire VRM_BUS_D7() const { return _VRAM_BUS_D7; }

  /*p32.LEGU*/ Reg LEGU_TILE_DA0 = REG_D0C0;
  /*p32.NUDU*/ Reg NUDU_TILE_DA1 = REG_D0C0;
  /*p32.MUKU*/ Reg MUKU_TILE_DA2 = REG_D0C0;
  /*p32.LUZO*/ Reg LUZO_TILE_DA3 = REG_D0C0;
  /*p32.MEGU*/ Reg MEGU_TILE_DA4 = REG_D0C0;
  /*p32.MYJY*/ Reg MYJY_TILE_DA5 = REG_D0C0;
  /*p32.NASA*/ Reg NASA_TILE_DA6 = REG_D0C0;
  /*p32.NEFO*/ Reg NEFO_TILE_DA7 = REG_D0C0; // color wrong on die

  /*p32.RAWU*/ Reg RAWU_TILE_DB0 = REG_D0C0; // also holds tile index during fetch
  /*p32.POZO*/ Reg POZO_TILE_DB1 = REG_D0C0;
  /*p32.PYZO*/ Reg PYZO_TILE_DB2 = REG_D0C0; 
  /*p32.POXA*/ Reg POXA_TILE_DB3 = REG_D0C0; 
  /*p32.PULO*/ Reg PULO_TILE_DB4 = REG_D0C0; 
  /*p32.POJU*/ Reg POJU_TILE_DB5 = REG_D0C0; 
  /*p32.POWY*/ Reg POWY_TILE_DB6 = REG_D0C0; 
  /*p32.PYJU*/ Reg PYJU_TILE_DB7 = REG_D0C0;

  /*p33.PEFO*/ Reg PEFO_SPRITE_DA0 = REG_D0C0;
  /*p33.ROKA*/ Reg ROKA_SPRITE_DA1 = REG_D0C0;
  /*p33.MYTU*/ Reg MYTU_SPRITE_DA2 = REG_D0C0;
  /*p33.RAMU*/ Reg RAMU_SPRITE_DA3 = REG_D0C0;
  /*p33.SELE*/ Reg SELE_SPRITE_DA4 = REG_D0C0;
  /*p33.SUTO*/ Reg SUTO_SPRITE_DA5 = REG_D0C0;
  /*p33.RAMA*/ Reg RAMA_SPRITE_DA6 = REG_D0C0;
  /*p33.RYDU*/ Reg RYDU_SPRITE_DA7 = REG_D0C0;

  /*p33.REWO*/ Reg REWO_SPRITE_DB0 = REG_D0C0;
  /*p33.PEBA*/ Reg PEBA_SPRITE_DB1 = REG_D0C0;
  /*p33.MOFO*/ Reg MOFO_SPRITE_DB2 = REG_D0C0;
  /*p33.PUDU*/ Reg PUDU_SPRITE_DB3 = REG_D0C0;
  /*p33.SAJA*/ Reg SAJA_SPRITE_DB4 = REG_D0C0;
  /*p33.SUNY*/ Reg SUNY_SPRITE_DB5 = REG_D0C0;
  /*p33.SEMO*/ Reg SEMO_SPRITE_DB6 = REG_D0C0;
  /*p33.SEGA*/ Reg SEGA_SPRITE_DB7 = REG_D0C0;

  //----------------------------------------

//private:

  // bus looks inverted on reads
  Tri _VRAM_BUS_D0 = TRI_HZNP;
  Tri _VRAM_BUS_D1 = TRI_HZNP;
  Tri _VRAM_BUS_D2 = TRI_HZNP;
  Tri _VRAM_BUS_D3 = TRI_HZNP;
  Tri _VRAM_BUS_D4 = TRI_HZNP;
  Tri _VRAM_BUS_D5 = TRI_HZNP;
  Tri _VRAM_BUS_D6 = TRI_HZNP;
  Tri _VRAM_BUS_D7 = TRI_HZNP;

  Tri _VRAM_BUS_A00 = TRI_HZNP;
  Tri _VRAM_BUS_A01 = TRI_HZNP;
  Tri _VRAM_BUS_A02 = TRI_HZNP;
  Tri _VRAM_BUS_A03 = TRI_HZNP;
  Tri _VRAM_BUS_A04 = TRI_HZNP;
  Tri _VRAM_BUS_A05 = TRI_HZNP;
  Tri _VRAM_BUS_A06 = TRI_HZNP;
  Tri _VRAM_BUS_A07 = TRI_HZNP;
  Tri _VRAM_BUS_A08 = TRI_HZNP;
  Tri _VRAM_BUS_A09 = TRI_HZNP;
  Tri _VRAM_BUS_A10 = TRI_HZNP;
  Tri _VRAM_BUS_A11 = TRI_HZNP;
  Tri _VRAM_BUS_A12 = TRI_HZNP;

  //-----------------------------------------------------------------------------
  // VRAM bus

  Tri _VRAM_PIN_CS_A = TRI_HZNP;   // PIN_43 <- P25.SOKY
  Tri _VRAM_PIN_CS_C = TRI_HZNP;   // PIN_43 -> P25.TEFY
  Tri _VRAM_PIN_CS_D = TRI_HZNP;   // PIN_43 <- P25.SETY

  Tri _VRAM_PIN_OE_A = TRI_HZNP;   // PIN_45 <- P25.REFO
  Tri _VRAM_PIN_OE_C = TRI_HZNP;   // PIN_45 -> P25.TAVY
  Tri _VRAM_PIN_OE_D = TRI_HZNP;   // PIN_45 <- P25.SAHA

  Tri _VRAM_PIN_WR_A = TRI_HZNP;   // PIN_49 <- P25.SYSY
  Tri _VRAM_PIN_WR_C = TRI_HZNP;   // PIN_49 -> P25.SUDO
  Tri _VRAM_PIN_WR_D = TRI_HZNP;   // PIN_49 <- P25.RAGU

  Tri _VRAM_PIN_A00  = TRI_HZNP;  // PIN_34 <- P04.ECAL
  Tri _VRAM_PIN_A01  = TRI_HZNP;  // PIN_35 <- P04.EGEZ
  Tri _VRAM_PIN_A02  = TRI_HZNP;  // PIN_36 <- P04.FUHE
  Tri _VRAM_PIN_A03  = TRI_HZNP;  // PIN_37 <- P04.FYZY
  Tri _VRAM_PIN_A04  = TRI_HZNP;  // PIN_38 <- P04.DAMU
  Tri _VRAM_PIN_A05  = TRI_HZNP;  // PIN_39 <- P04.DAVA
  Tri _VRAM_PIN_A06  = TRI_HZNP;  // PIN_40 <- P04.ETEG
  Tri _VRAM_PIN_A07  = TRI_HZNP;  // PIN_41 <- P04.EREW
  Tri _VRAM_PIN_A08  = TRI_HZNP;  // PIN_48 <- P04.EVAX
  Tri _VRAM_PIN_A09  = TRI_HZNP;  // PIN_47 <- P04.DUVE
  Tri _VRAM_PIN_A10  = TRI_HZNP;  // PIN_44 <- P04.ERAF
  Tri _VRAM_PIN_A11  = TRI_HZNP;  // PIN_46 <- P04.FUSY
  Tri _VRAM_PIN_A12  = TRI_HZNP;  // PIN_42 <- P04.EXYF

  Tri _VRAM_PIN_D0_A = TRI_HZNP;    // PIN_33 <- P25.REGE
  Tri _VRAM_PIN_D1_A = TRI_HZNP;    // PIN_31 <- P25.RYKY
  Tri _VRAM_PIN_D2_A = TRI_HZNP;    // PIN_30 <- P25.RAZO
  Tri _VRAM_PIN_D3_A = TRI_HZNP;    // PIN_29 <- P25.RADA
  Tri _VRAM_PIN_D4_A = TRI_HZNP;    // PIN_28 <- P25.RYRO
  Tri _VRAM_PIN_D5_A = TRI_HZNP;    // PIN_27 <- P25.REVU
  Tri _VRAM_PIN_D6_A = TRI_HZNP;    // PIN_26 <- P25.REKU
  Tri _VRAM_PIN_D7_A = TRI_HZNP;    // PIN_25 <- P25.RYZE

  Tri _VRAM_PIN_D0_B = TRI_HZNP;    // PIN_33 <- P25.ROFA
  Tri _VRAM_PIN_D1_B = TRI_HZNP;    // PIN_31 <- P25.ROFA
  Tri _VRAM_PIN_D2_B = TRI_HZNP;    // PIN_30 <- P25.ROFA
  Tri _VRAM_PIN_D3_B = TRI_HZNP;    // PIN_29 <- P25.ROFA
  Tri _VRAM_PIN_D4_B = TRI_HZNP;    // PIN_28 <- P25.ROFA
  Tri _VRAM_PIN_D5_B = TRI_HZNP;    // PIN_27 <- P25.ROFA
  Tri _VRAM_PIN_D6_B = TRI_HZNP;    // PIN_26 <- P25.ROFA
  Tri _VRAM_PIN_D7_B = TRI_HZNP;    // PIN_25 <- P25.ROFA

  Tri _VRAM_PIN_D0_C = TRI_HZPU;    // PIN_33 -> P25.RODY
  Tri _VRAM_PIN_D1_C = TRI_HZPU;    // PIN_31 -> P25.REBA
  Tri _VRAM_PIN_D2_C = TRI_HZPU;    // PIN_30 -> P25.RYDO
  Tri _VRAM_PIN_D3_C = TRI_HZPU;    // PIN_29 -> P25.REMO
  Tri _VRAM_PIN_D4_C = TRI_HZPU;    // PIN_28 -> P25.ROCE
  Tri _VRAM_PIN_D5_C = TRI_HZPU;    // PIN_27 -> P25.ROPU
  Tri _VRAM_PIN_D6_C = TRI_HZPU;    // PIN_26 -> P25.RETA
  Tri _VRAM_PIN_D7_C = TRI_HZPU;    // PIN_25 -> P25.RAKU

  Tri _VRAM_PIN_D0_D = TRI_HZNP;    // PIN_33 <- P25.RURA
  Tri _VRAM_PIN_D1_D = TRI_HZNP;    // PIN_31 <- P25.RULY
  Tri _VRAM_PIN_D2_D = TRI_HZNP;    // PIN_30 <- P25.RARE
  Tri _VRAM_PIN_D3_D = TRI_HZNP;    // PIN_29 <- P25.RODU
  Tri _VRAM_PIN_D4_D = TRI_HZNP;    // PIN_28 <- P25.RUBE
  Tri _VRAM_PIN_D5_D = TRI_HZNP;    // PIN_27 <- P25.RUMU
  Tri _VRAM_PIN_D6_D = TRI_HZNP;    // PIN_26 <- P25.RYTY
  Tri _VRAM_PIN_D7_D = TRI_HZNP;    // PIN_25 <- P25.RADY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics