#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct VramBus {
  void tock(SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  int get_map_x(const SchematicTop& top) const;
  int get_map_y(const SchematicTop& top) const;

  int get_bus_addr() const {
    return pack_p(_VRAM_BUS_A00.qp(), _VRAM_BUS_A01.qp(), _VRAM_BUS_A02.qp(), _VRAM_BUS_A03.qp(),
                _VRAM_BUS_A04.qp(), _VRAM_BUS_A05.qp(), _VRAM_BUS_A06.qp(), _VRAM_BUS_A07.qp(),
                _VRAM_BUS_A08.qp(), _VRAM_BUS_A09.qp(), _VRAM_BUS_A10.qp(), _VRAM_BUS_A11.qp(),
                _VRAM_BUS_A12.qp(), 0, 0, 0);
  }

  int get_bus_data() const {
    return pack_p(_VRAM_BUS_D0n.qp(), _VRAM_BUS_D1n.qp(), _VRAM_BUS_D2n.qp(), _VRAM_BUS_D3n.qp(),
                  _VRAM_BUS_D4n.qp(), _VRAM_BUS_D5n.qp(), _VRAM_BUS_D6n.qp(), _VRAM_BUS_D7n.qp());
  }

  uint16_t get_pin_addr() const {
    return (uint16_t)pack_p(_VRAM_PIN_A00.qp(), _VRAM_PIN_A01.qp(), _VRAM_PIN_A02.qp(), _VRAM_PIN_A03.qp(),
                            _VRAM_PIN_A04.qp(), _VRAM_PIN_A05.qp(), _VRAM_PIN_A06.qp(), _VRAM_PIN_A07.qp(),
                            _VRAM_PIN_A08.qp(), _VRAM_PIN_A09.qp(), _VRAM_PIN_A10.qp(), _VRAM_PIN_A11.qp(),
                            _VRAM_PIN_A12.qp(), 0, 0, 0);
  }

  uint8_t get_pin_data_out() const {
    return ~(uint8_t)pack_p(_VRAM_PIN_D0_A.qp(), _VRAM_PIN_D1_A.qp(), _VRAM_PIN_D2_A.qp(), _VRAM_PIN_D3_A.qp(),
                            _VRAM_PIN_D4_A.qp(), _VRAM_PIN_D5_A.qp(), _VRAM_PIN_D6_A.qp(), _VRAM_PIN_D7_A.qp());
  }

  void preset_pin_data_in(uint8_t data) {
    _VRAM_PIN_D0n_C.preset(!(data & 0x01));
    _VRAM_PIN_D1n_C.preset(!(data & 0x02));
    _VRAM_PIN_D2n_C.preset(!(data & 0x04));
    _VRAM_PIN_D3n_C.preset(!(data & 0x08));
    _VRAM_PIN_D4n_C.preset(!(data & 0x10));
    _VRAM_PIN_D5n_C.preset(!(data & 0x20));
    _VRAM_PIN_D6n_C.preset(!(data & 0x40));
    _VRAM_PIN_D7n_C.preset(!(data & 0x80));
  }

  void preset_pin_data_z() {
    _VRAM_PIN_D0n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D1n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D2n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D3n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D4n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D5n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D6n_C.preset(DELTA_TRIZ);
    _VRAM_PIN_D7n_C.preset(DELTA_TRIZ);
  }

  //-----------------------------------------------------------------------------

  /*p32.LEGU*/ RegQN LEGU_TILE_DA0n = REG_D0C0;
  /*p32.NUDU*/ RegQN NUDU_TILE_DA1n = REG_D0C0;
  /*p32.MUKU*/ RegQN MUKU_TILE_DA2n = REG_D0C0;
  /*p32.LUZO*/ RegQN LUZO_TILE_DA3n = REG_D0C0;
  /*p32.MEGU*/ RegQN MEGU_TILE_DA4n = REG_D0C0;
  /*p32.MYJY*/ RegQN MYJY_TILE_DA5n = REG_D0C0;
  /*p32.NASA*/ RegQN NASA_TILE_DA6n = REG_D0C0;
  /*p32.NEFO*/ RegQN NEFO_TILE_DA7n = REG_D0C0; // color wrong on die

  /*p32.RAWU*/ RegQN RAWU_TILE_DB0n = REG_D0C0; // also holds tile index during fetch
  /*p32.POZO*/ RegQN POZO_TILE_DB1n = REG_D0C0;
  /*p32.PYZO*/ RegQN PYZO_TILE_DB2n = REG_D0C0; 
  /*p32.POXA*/ RegQN POXA_TILE_DB3n = REG_D0C0; 
  /*p32.PULO*/ RegQN PULO_TILE_DB4n = REG_D0C0; 
  /*p32.POJU*/ RegQN POJU_TILE_DB5n = REG_D0C0; 
  /*p32.POWY*/ RegQN POWY_TILE_DB6n = REG_D0C0; 
  /*p32.PYJU*/ RegQN PYJU_TILE_DB7n = REG_D0C0;

  /*p33.PEFO*/ RegQN PEFO_SPRITE_DB0n = REG_D0C0;
  /*p33.ROKA*/ RegQN ROKA_SPRITE_DB1n = REG_D0C0;
  /*p33.MYTU*/ RegQN MYTU_SPRITE_DB2n = REG_D0C0;
  /*p33.RAMU*/ RegQN RAMU_SPRITE_DB3n = REG_D0C0;
  /*p33.SELE*/ RegQN SELE_SPRITE_DB4n = REG_D0C0;
  /*p33.SUTO*/ RegQN SUTO_SPRITE_DB5n = REG_D0C0;
  /*p33.RAMA*/ RegQN RAMA_SPRITE_DB6n = REG_D0C0;
  /*p33.RYDU*/ RegQN RYDU_SPRITE_DB7n = REG_D0C0;

  /*p33.REWO*/ RegQN REWO_SPRITE_DA0n = REG_D0C0;
  /*p33.PEBA*/ RegQN PEBA_SPRITE_DA1n = REG_D0C0;
  /*p33.MOFO*/ RegQN MOFO_SPRITE_DA2n = REG_D0C0;
  /*p33.PUDU*/ RegQN PUDU_SPRITE_DA3n = REG_D0C0;
  /*p33.SAJA*/ RegQN SAJA_SPRITE_DA4n = REG_D0C0;
  /*p33.SUNY*/ RegQN SUNY_SPRITE_DA5n = REG_D0C0;
  /*p33.SEMO*/ RegQN SEMO_SPRITE_DA6n = REG_D0C0;
  /*p33.SEGA*/ RegQN SEGA_SPRITE_DA7n = REG_D0C0;

  //----------------------------------------
  
  // OK, this is super screwy - VRAM_BUS_D* is positive on writes, inverted on reads. WTF.

  Tri _VRAM_BUS_D0n = TRI_HZNP;
  Tri _VRAM_BUS_D1n = TRI_HZNP;
  Tri _VRAM_BUS_D2n = TRI_HZNP;
  Tri _VRAM_BUS_D3n = TRI_HZNP;
  Tri _VRAM_BUS_D4n = TRI_HZNP;
  Tri _VRAM_BUS_D5n = TRI_HZNP;
  Tri _VRAM_BUS_D6n = TRI_HZNP;
  Tri _VRAM_BUS_D7n = TRI_HZNP;

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

  //----------------------------------------
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

  Tri _VRAM_PIN_D0n_C = TRI_HZPU;    // PIN_33 -> P25.RODY
  Tri _VRAM_PIN_D1n_C = TRI_HZPU;    // PIN_31 -> P25.REBA
  Tri _VRAM_PIN_D2n_C = TRI_HZPU;    // PIN_30 -> P25.RYDO
  Tri _VRAM_PIN_D3n_C = TRI_HZPU;    // PIN_29 -> P25.REMO
  Tri _VRAM_PIN_D4n_C = TRI_HZPU;    // PIN_28 -> P25.ROCE
  Tri _VRAM_PIN_D5n_C = TRI_HZPU;    // PIN_27 -> P25.ROPU
  Tri _VRAM_PIN_D6n_C = TRI_HZPU;    // PIN_26 -> P25.RETA
  Tri _VRAM_PIN_D7n_C = TRI_HZPU;    // PIN_25 -> P25.RAKU

  Tri _VRAM_PIN_D0_D = TRI_HZNP;    // PIN_33 <- P25.RURA
  Tri _VRAM_PIN_D1_D = TRI_HZNP;    // PIN_31 <- P25.RULY
  Tri _VRAM_PIN_D2_D = TRI_HZNP;    // PIN_30 <- P25.RARE
  Tri _VRAM_PIN_D3_D = TRI_HZNP;    // PIN_29 <- P25.RODU
  Tri _VRAM_PIN_D4_D = TRI_HZNP;    // PIN_28 <- P25.RUBE
  Tri _VRAM_PIN_D5_D = TRI_HZNP;    // PIN_27 <- P25.RUMU
  Tri _VRAM_PIN_D6_D = TRI_HZNP;    // PIN_26 <- P25.RYTY
  Tri _VRAM_PIN_D7_D = TRI_HZNP;    // PIN_25 <- P25.RADY

  //----------------------------------------
  // Signals for debugging

  /*p26.FAFO*/ Sig _FAFO_TILE_Y0S; 
  /*p26.FAFO*/ Sig _FAFO_TILE_Y0C; 
  /*p26.EMUX*/ Sig _EMUX_TILE_Y1S; 
  /*p26.EMUX*/ Sig _EMUX_TILE_Y1C; 
  /*p26.ECAB*/ Sig _ECAB_TILE_Y2S; 
  /*p26.ECAB*/ Sig _ECAB_TILE_Y2C; 
  /*p26.ETAM*/ Sig _ETAM_MAP_Y0S;
  /*p26.ETAM*/ Sig _ETAM_MAP_Y0C;
  /*p26.DOTO*/ Sig _DOTO_MAP_Y1S;
  /*p26.DOTO*/ Sig _DOTO_MAP_Y1C;
  /*p26.DABA*/ Sig _DABA_MAP_Y2S;
  /*p26.DABA*/ Sig _DABA_MAP_Y2C;
  /*p26.EFYK*/ Sig _EFYK_MAP_Y3S;
  /*p26.EFYK*/ Sig _EFYK_MAP_Y3C;
  /*p26.EJOK*/ Sig _EJOK_MAP_Y4S;
  /*p26.EJOK*/ Sig _EJOK_MAP_Y4C;

  /*p26.ATAD*/ Sig _ATAD_TILE_X0S; 
  /*p26.ATAD*/ Sig _ATAD_TILE_X0C; 
  /*p26.BEHU*/ Sig _BEHU_TILE_X1S; 
  /*p26.BEHU*/ Sig _BEHU_TILE_X1C; 
  /*p26.APYH*/ Sig _APYH_TILE_X2S; 
  /*p26.APYH*/ Sig _APYH_TILE_X2C; 
  /*p26.BABE*/ Sig _BABE_MAP_X0S;
  /*p26.BABE*/ Sig _BABE_MAP_X0C;
  /*p26.ABOD*/ Sig _ABOD_MAP_X1S;
  /*p26.ABOD*/ Sig _ABOD_MAP_X1C;
  /*p26.BEWY*/ Sig _BEWY_MAP_X2S;
  /*p26.BEWY*/ Sig _BEWY_MAP_X2C;
  /*p26.BYCA*/ Sig _BYCA_MAP_X3S;
  /*p26.BYCA*/ Sig _BYCA_MAP_X3C;
  /*p26.ACUL*/ Sig _ACUL_MAP_X4S;
  /*p26.ACUL*/ Sig _ACUL_MAP_X4C;

  /*p25.XEDU*/ Sig XEDU_CPU_VRAM_RDn;
  /*p04.AHOC*/ Sig AHOC_DMA_VRAM_RDn;
  /*p29.ABON*/ Sig ABON_SPR_VRM_RDn;
  /*p26.BAFY*/ Sig BAFY_BG_MAP_READn;
  /*p25.WUKO*/ Sig WUKO_WIN_MAP_READn;

  /*p27.NETA*/ Sig NETA_TILE_READp;
  /*p26.ASUL*/ Sig ASUL_TILE_READp;
  /*p26.BEJE*/ Sig BEJE_BGD_TILE_READn;
  /*p25.XUCY*/ Sig XUCY_WIN_TILE_READn;
  /*p25.VUZA*/ Sig VUZA_TILE_BANKp;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics