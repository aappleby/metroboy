#pragma once
#include "GateBoyLib/Cells.h"

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
    return pack_p(!VRAM_BUS_A00n.tp(), !VRAM_BUS_A01n.tp(), !VRAM_BUS_A02n.tp(), !VRAM_BUS_A03n.tp(),
                  !VRAM_BUS_A04n.tp(), !VRAM_BUS_A05n.tp(), !VRAM_BUS_A06n.tp(), !VRAM_BUS_A07n.tp(),
                  !VRAM_BUS_A08n.tp(), !VRAM_BUS_A09n.tp(), !VRAM_BUS_A10n.tp(), !VRAM_BUS_A11n.tp(),
                  !VRAM_BUS_A12n.tp(), 0, 0, 0);
  }

  int get_bus_data() const {
    return pack_p(VRAM_BUS_D0p.tp(), VRAM_BUS_D1p.tp(), VRAM_BUS_D2p.tp(), VRAM_BUS_D3p.tp(),
                  VRAM_BUS_D4p.tp(), VRAM_BUS_D5p.tp(), VRAM_BUS_D6p.tp(), VRAM_BUS_D7p.tp());
  }

  uint16_t get_pin_addr() const {
    return (uint16_t)pack_p(VRAM_PIN_A00p.qp(), VRAM_PIN_A01p.qp(), VRAM_PIN_A02p.qp(), VRAM_PIN_A03p.qp(),
                            VRAM_PIN_A04p.qp(), VRAM_PIN_A05p.qp(), VRAM_PIN_A06p.qp(), VRAM_PIN_A07p.qp(),
                            VRAM_PIN_A08p.qp(), VRAM_PIN_A09p.qp(), VRAM_PIN_A10p.qp(), VRAM_PIN_A11p.qp(),
                            VRAM_PIN_A12p.qp(), 0, 0, 0);
  }

  uint8_t get_pin_data_out() const {
    return (uint8_t)pack_p(VRAM_PIN_D00p.qp(), VRAM_PIN_D01p.qp(), VRAM_PIN_D02p.qp(), VRAM_PIN_D03p.qp(),
                           VRAM_PIN_D04p.qp(), VRAM_PIN_D05p.qp(), VRAM_PIN_D06p.qp(), VRAM_PIN_D07p.qp());
  }

  void set_pin_data_in(uint8_t data) {
    VRAM_PIN_D00p = (data & 0x01) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D01p = (data & 0x02) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D02p = (data & 0x04) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D03p = (data & 0x08) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D04p = (data & 0x10) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D05p = (data & 0x20) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D06p = (data & 0x40) ? DELTA_TRI1 : DELTA_TRI0;
    VRAM_PIN_D07p = (data & 0x80) ? DELTA_TRI1 : DELTA_TRI0;
  }

  void set_pin_data_z() {
    VRAM_PIN_D00p = DELTA_TRIZ;
    VRAM_PIN_D01p = DELTA_TRIZ;
    VRAM_PIN_D02p = DELTA_TRIZ;
    VRAM_PIN_D03p = DELTA_TRIZ;
    VRAM_PIN_D04p = DELTA_TRIZ;
    VRAM_PIN_D05p = DELTA_TRIZ;
    VRAM_PIN_D06p = DELTA_TRIZ;
    VRAM_PIN_D07p = DELTA_TRIZ;
  }

  //-----------------------------------------------------------------------------

  /*p32.LEGU*/ RegQN LEGU_TILE_DA0n = REG_D0C0; // def holds inverted pix
  /*p32.NUDU*/ RegQN NUDU_TILE_DA1n = REG_D0C0;
  /*p32.MUKU*/ RegQN MUKU_TILE_DA2n = REG_D0C0;
  /*p32.LUZO*/ RegQN LUZO_TILE_DA3n = REG_D0C0;
  /*p32.MEGU*/ RegQN MEGU_TILE_DA4n = REG_D0C0;
  /*p32.MYJY*/ RegQN MYJY_TILE_DA5n = REG_D0C0;
  /*p32.NASA*/ RegQN NASA_TILE_DA6n = REG_D0C0;
  /*p32.NEFO*/ RegQN NEFO_TILE_DA7n = REG_D0C0; // color wrong on die

  /*p32.RAWU*/ RegQN RAWU_TILE_DB0n = REG_D0C0; // def holds inverted pix, also holds tile index during fetch
  /*p32.POZO*/ RegQN POZO_TILE_DB1n = REG_D0C0;
  /*p32.PYZO*/ RegQN PYZO_TILE_DB2n = REG_D0C0; 
  /*p32.POXA*/ RegQN POXA_TILE_DB3n = REG_D0C0; 
  /*p32.PULO*/ RegQN PULO_TILE_DB4n = REG_D0C0; 
  /*p32.POJU*/ RegQN POJU_TILE_DB5n = REG_D0C0; 
  /*p32.POWY*/ RegQN POWY_TILE_DB6n = REG_D0C0; 
  /*p32.PYJU*/ RegQN PYJU_TILE_DB7n = REG_D0C0;

  /*p33.PEFO*/ RegQN PEFO_SPRITE_DB0n = REG_D0C0; // def holds inverted pix
  /*p33.ROKA*/ RegQN ROKA_SPRITE_DB1n = REG_D0C0;
  /*p33.MYTU*/ RegQN MYTU_SPRITE_DB2n = REG_D0C0;
  /*p33.RAMU*/ RegQN RAMU_SPRITE_DB3n = REG_D0C0;
  /*p33.SELE*/ RegQN SELE_SPRITE_DB4n = REG_D0C0;
  /*p33.SUTO*/ RegQN SUTO_SPRITE_DB5n = REG_D0C0;
  /*p33.RAMA*/ RegQN RAMA_SPRITE_DB6n = REG_D0C0;
  /*p33.RYDU*/ RegQN RYDU_SPRITE_DB7n = REG_D0C0;

  /*p33.REWO*/ RegQN REWO_SPRITE_DA0n = REG_D0C0; // def holds inverted pix
  /*p33.PEBA*/ RegQN PEBA_SPRITE_DA1n = REG_D0C0;
  /*p33.MOFO*/ RegQN MOFO_SPRITE_DA2n = REG_D0C0;
  /*p33.PUDU*/ RegQN PUDU_SPRITE_DA3n = REG_D0C0;
  /*p33.SAJA*/ RegQN SAJA_SPRITE_DA4n = REG_D0C0;
  /*p33.SUNY*/ RegQN SUNY_SPRITE_DA5n = REG_D0C0;
  /*p33.SEMO*/ RegQN SEMO_SPRITE_DA6n = REG_D0C0;
  /*p33.SEGA*/ RegQN SEGA_SPRITE_DA7n = REG_D0C0;

  //----------------------------------------
  
  // VRAM_BUS_D* must _not_ be inverting, see CBD->VBD->VPD chain

  Tri VRAM_BUS_D0p = TRI_D0NP;
  Tri VRAM_BUS_D1p = TRI_D0NP;
  Tri VRAM_BUS_D2p = TRI_D0NP;
  Tri VRAM_BUS_D3p = TRI_D0NP;
  Tri VRAM_BUS_D4p = TRI_D0NP;
  Tri VRAM_BUS_D5p = TRI_D0NP;
  Tri VRAM_BUS_D6p = TRI_D0NP;
  Tri VRAM_BUS_D7p = TRI_D0NP;

  Tri VRAM_BUS_A00n = TRI_D0NP;
  Tri VRAM_BUS_A01n = TRI_D0NP;
  Tri VRAM_BUS_A02n = TRI_D0NP;
  Tri VRAM_BUS_A03n = TRI_D0NP;
  Tri VRAM_BUS_A04n = TRI_D0NP;
  Tri VRAM_BUS_A05n = TRI_D0NP;
  Tri VRAM_BUS_A06n = TRI_D0NP;
  Tri VRAM_BUS_A07n = TRI_D0NP;
  Tri VRAM_BUS_A08n = TRI_D0NP;
  Tri VRAM_BUS_A09n = TRI_D0NP;
  Tri VRAM_BUS_A10n = TRI_D0NP;
  Tri VRAM_BUS_A11n = TRI_D0NP;
  Tri VRAM_BUS_A12n = TRI_D0NP;

  //----------------------------------------
  // VRAM bus

  Pin VRAM_PIN_CSn  = TRI_D0NP;
  Pin VRAM_PIN_OEn  = TRI_D0NP;
  Pin VRAM_PIN_WRn  = TRI_D0NP;

  Pin VRAM_PIN_A00p = TRI_D0NP; // PIN_34
  Pin VRAM_PIN_A01p = TRI_D0NP; // PIN_35
  Pin VRAM_PIN_A02p = TRI_D0NP; // PIN_36
  Pin VRAM_PIN_A03p = TRI_D0NP; // PIN_37
  Pin VRAM_PIN_A04p = TRI_D0NP; // PIN_38
  Pin VRAM_PIN_A05p = TRI_D0NP; // PIN_39
  Pin VRAM_PIN_A06p = TRI_D0NP; // PIN_40
  Pin VRAM_PIN_A07p = TRI_D0NP; // PIN_41
  Pin VRAM_PIN_A08p = TRI_D0NP; // PIN_48
  Pin VRAM_PIN_A09p = TRI_D0NP; // PIN_47
  Pin VRAM_PIN_A10p = TRI_D0NP; // PIN_44
  Pin VRAM_PIN_A11p = TRI_D0NP; // PIN_46
  Pin VRAM_PIN_A12p = TRI_D0NP; // PIN_42

  Pin VRAM_PIN_D00p = TRI_D0NP; // PIN_33
  Pin VRAM_PIN_D01p = TRI_D0NP; // PIN_31
  Pin VRAM_PIN_D02p = TRI_D0NP; // PIN_30
  Pin VRAM_PIN_D03p = TRI_D0NP; // PIN_29
  Pin VRAM_PIN_D04p = TRI_D0NP; // PIN_28
  Pin VRAM_PIN_D05p = TRI_D0NP; // PIN_27
  Pin VRAM_PIN_D06p = TRI_D0NP; // PIN_26
  Pin VRAM_PIN_D07p = TRI_D0NP; // PIN_25

  //----------------------------------------
  // Signals for debugging

  /*p26.FAFO*/ Sig FAFO_TILE_Y0S; 
  /*p26.FAFO*/ Sig FAFO_TILE_Y0C; 
  /*p26.EMUX*/ Sig EMUX_TILE_Y1S; 
  /*p26.EMUX*/ Sig EMUX_TILE_Y1C; 
  /*p26.ECAB*/ Sig ECAB_TILE_Y2S; 
  /*p26.ECAB*/ Sig ECAB_TILE_Y2C; 
  /*p26.ETAM*/ Sig ETAM_MAP_Y0S;
  /*p26.ETAM*/ Sig ETAM_MAP_Y0C;
  /*p26.DOTO*/ Sig DOTO_MAP_Y1S;
  /*p26.DOTO*/ Sig DOTO_MAP_Y1C;
  /*p26.DABA*/ Sig DABA_MAP_Y2S;
  /*p26.DABA*/ Sig DABA_MAP_Y2C;
  /*p26.EFYK*/ Sig EFYK_MAP_Y3S;
  /*p26.EFYK*/ Sig EFYK_MAP_Y3C;
  /*p26.EJOK*/ Sig EJOK_MAP_Y4S;
  /*p26.EJOK*/ Sig EJOK_MAP_Y4C;

  /*p26.ATAD*/ Sig ATAD_TILE_X0S; 
  /*p26.ATAD*/ Sig ATAD_TILE_X0C; 
  /*p26.BEHU*/ Sig BEHU_TILE_X1S; 
  /*p26.BEHU*/ Sig BEHU_TILE_X1C; 
  /*p26.APYH*/ Sig APYH_TILE_X2S; 
  /*p26.APYH*/ Sig APYH_TILE_X2C; 
  /*p26.BABE*/ Sig BABE_MAP_X0S;
  /*p26.BABE*/ Sig BABE_MAP_X0C;
  /*p26.ABOD*/ Sig ABOD_MAP_X1S;
  /*p26.ABOD*/ Sig ABOD_MAP_X1C;
  /*p26.BEWY*/ Sig BEWY_MAP_X2S;
  /*p26.BEWY*/ Sig BEWY_MAP_X2C;
  /*p26.BYCA*/ Sig BYCA_MAP_X3S;
  /*p26.BYCA*/ Sig BYCA_MAP_X3C;
  /*p26.ACUL*/ Sig ACUL_MAP_X4S;
  /*p26.ACUL*/ Sig ACUL_MAP_X4C;

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