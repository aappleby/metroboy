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
    return pack(_VRAM_BUS_A00.q(), _VRAM_BUS_A01.q(), _VRAM_BUS_A02.q(), _VRAM_BUS_A03.q(),
                _VRAM_BUS_A04.q(), _VRAM_BUS_A05.q(), _VRAM_BUS_A06.q(), _VRAM_BUS_A07.q(),
                _VRAM_BUS_A08.q(), _VRAM_BUS_A09.q(), _VRAM_BUS_A10.q(), _VRAM_BUS_A11.q(),
                _VRAM_BUS_A12.q(), 0, 0, 0);
  }

  int get_bus_data() const {
    return pack(_VRAM_BUS_D0.q(), _VRAM_BUS_D1.q(), _VRAM_BUS_D2.q(), _VRAM_BUS_D3.q(),
                _VRAM_BUS_D4.q(), _VRAM_BUS_D5.q(), _VRAM_BUS_D6.q(), _VRAM_BUS_D7.q());
  }

  uint16_t get_pin_addr() const {
    return (uint16_t)pack(_VRAM_PIN_A00.qn(), _VRAM_PIN_A01.qn(), _VRAM_PIN_A02.qn(), _VRAM_PIN_A03.qn(),
                          _VRAM_PIN_A04.qn(), _VRAM_PIN_A05.qn(), _VRAM_PIN_A06.qn(), _VRAM_PIN_A07.qn(),
                          _VRAM_PIN_A08.qn(), _VRAM_PIN_A09.qn(), _VRAM_PIN_A10.qn(), _VRAM_PIN_A11.qn(),
                          _VRAM_PIN_A12.qn(), 0, 0, 0);
  }

  uint8_t get_pin_data_out() const {
    return (uint8_t)pack(_VRAM_PIN_D0_A.qn(), _VRAM_PIN_D1_A.qn(), _VRAM_PIN_D2_A.qn(), _VRAM_PIN_D3_A.qn(),
                         _VRAM_PIN_D4_A.qn(), _VRAM_PIN_D5_A.qn(), _VRAM_PIN_D6_A.qn(), _VRAM_PIN_D7_A.qn());
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

  int get_map_x(const SchematicTop& top) const;
  int get_map_y(const SchematicTop& top) const;
  void dump(Dumper& d, const SchematicTop& top) const;

  //----------------------------------------

  // -> oam data tri
  /*p25.RERY*/ wire VRM_BUS_D0() const { return _VRAM_BUS_D0.q(); }
  /*p25.RUNA*/ wire VRM_BUS_D1() const { return _VRAM_BUS_D1.q(); }
  /*p25.RONA*/ wire VRM_BUS_D2() const { return _VRAM_BUS_D2.q(); }
  /*p25.RUNO*/ wire VRM_BUS_D3() const { return _VRAM_BUS_D3.q(); }
  /*p25.SANA*/ wire VRM_BUS_D4() const { return _VRAM_BUS_D4.q(); }
  /*p25.RORO*/ wire VRM_BUS_D5() const { return _VRAM_BUS_D5.q(); }
  /*p25.RABO*/ wire VRM_BUS_D6() const { return _VRAM_BUS_D6.q(); }
  /*p25.SAME*/ wire VRM_BUS_D7() const { return _VRAM_BUS_D7.q(); }

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

  //-----------------------------------------------------------------------------
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