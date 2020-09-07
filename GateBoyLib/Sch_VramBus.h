#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramBus {
  void tock(SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  int get_bus_addr() const {
    return pack_p(!BUS_VRAM_A00n.tp(), !BUS_VRAM_A01n.tp(), !BUS_VRAM_A02n.tp(), !BUS_VRAM_A03n.tp(),
                  !BUS_VRAM_A04n.tp(), !BUS_VRAM_A05n.tp(), !BUS_VRAM_A06n.tp(), !BUS_VRAM_A07n.tp(),
                  !BUS_VRAM_A08n.tp(), !BUS_VRAM_A09n.tp(), !BUS_VRAM_A10n.tp(), !BUS_VRAM_A11n.tp(),
                  !BUS_VRAM_A12n.tp(), 0, 0, 0);
  }

  int get_bus_data() const {
    return pack_p(BUS_VRAM_D0p.tp(), BUS_VRAM_D1p.tp(), BUS_VRAM_D2p.tp(), BUS_VRAM_D3p.tp(),
                  BUS_VRAM_D4p.tp(), BUS_VRAM_D5p.tp(), BUS_VRAM_D6p.tp(), BUS_VRAM_D7p.tp());
  }

  int get_pin_addr() const {
    return pack_p(PIN_VRAM_A00p.qp(), PIN_VRAM_A01p.qp(), PIN_VRAM_A02p.qp(), PIN_VRAM_A03p.qp(),
                  PIN_VRAM_A04p.qp(), PIN_VRAM_A05p.qp(), PIN_VRAM_A06p.qp(), PIN_VRAM_A07p.qp(),
                  PIN_VRAM_A08p.qp(), PIN_VRAM_A09p.qp(), PIN_VRAM_A10p.qp(), PIN_VRAM_A11p.qp(),
                  PIN_VRAM_A12p.qp(), 0, 0, 0);
  }

  int get_pin_data() const {
    return pack_p(PIN_VRAM_D00p.qp(), PIN_VRAM_D01p.qp(), PIN_VRAM_D02p.qp(), PIN_VRAM_D03p.qp(),
                  PIN_VRAM_D04p.qp(), PIN_VRAM_D05p.qp(), PIN_VRAM_D06p.qp(), PIN_VRAM_D07p.qp());
  }

  void set_pin_data_in(uint8_t data) {
    PIN_VRAM_D00p = (data & 0x01) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D01p = (data & 0x02) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D02p = (data & 0x04) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D03p = (data & 0x08) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D04p = (data & 0x10) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D05p = (data & 0x20) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D06p = (data & 0x40) ? DELTA_TRI1 : DELTA_TRI0;
    PIN_VRAM_D07p = (data & 0x80) ? DELTA_TRI1 : DELTA_TRI0;
  }

  void set_pin_data_z() {
    PIN_VRAM_D00p = DELTA_TRIZ;
    PIN_VRAM_D01p = DELTA_TRIZ;
    PIN_VRAM_D02p = DELTA_TRIZ;
    PIN_VRAM_D03p = DELTA_TRIZ;
    PIN_VRAM_D04p = DELTA_TRIZ;
    PIN_VRAM_D05p = DELTA_TRIZ;
    PIN_VRAM_D06p = DELTA_TRIZ;
    PIN_VRAM_D07p = DELTA_TRIZ;
  }

  //-----------------------------------------------------------------------------

  /*p32.LEGU*/ DFF8 LEGU_TILE_DA0n;
  /*p32.NUDU*/ DFF8 NUDU_TILE_DA1n;
  /*p32.MUKU*/ DFF8 MUKU_TILE_DA2n;
  /*p32.LUZO*/ DFF8 LUZO_TILE_DA3n;
  /*p32.MEGU*/ DFF8 MEGU_TILE_DA4n;
  /*p32.MYJY*/ DFF8 MYJY_TILE_DA5n;
  /*p32.NASA*/ DFF8 NASA_TILE_DA6n;
  /*p32.NEFO*/ DFF8 NEFO_TILE_DA7n; // color wrong on die

  /*p32.RAWU*/ RegQN RAWU_TILE_DB0n = REG_D0C0; // def holds inverted pix, also holds tile index during fetch
  /*p32.POZO*/ RegQN POZO_TILE_DB1n = REG_D0C0;
  /*p32.PYZO*/ RegQN PYZO_TILE_DB2n = REG_D0C0; 
  /*p32.POXA*/ RegQN POXA_TILE_DB3n = REG_D0C0; 
  /*p32.PULO*/ RegQN PULO_TILE_DB4n = REG_D0C0; 
  /*p32.POJU*/ RegQN POJU_TILE_DB5n = REG_D0C0; 
  /*p32.POWY*/ RegQN POWY_TILE_DB6n = REG_D0C0; 
  /*p32.PYJU*/ RegQN PYJU_TILE_DB7n = REG_D0C0;

  /*p33.PEFO*/ DFF8 PEFO_SPRITE_DB0n;
  /*p33.ROKA*/ DFF8 ROKA_SPRITE_DB1n;
  /*p33.MYTU*/ DFF8 MYTU_SPRITE_DB2n;
  /*p33.RAMU*/ DFF8 RAMU_SPRITE_DB3n;
  /*p33.SELE*/ DFF8 SELE_SPRITE_DB4n;
  /*p33.SUTO*/ DFF8 SUTO_SPRITE_DB5n;
  /*p33.RAMA*/ DFF8 RAMA_SPRITE_DB6n;
  /*p33.RYDU*/ DFF8 RYDU_SPRITE_DB7n;

  /*p33.REWO*/ RegQN REWO_SPRITE_DA0n = REG_D0C0; // def holds inverted pix
  /*p33.PEBA*/ RegQN PEBA_SPRITE_DA1n = REG_D0C0;
  /*p33.MOFO*/ RegQN MOFO_SPRITE_DA2n = REG_D0C0;
  /*p33.PUDU*/ RegQN PUDU_SPRITE_DA3n = REG_D0C0;
  /*p33.SAJA*/ RegQN SAJA_SPRITE_DA4n = REG_D0C0;
  /*p33.SUNY*/ RegQN SUNY_SPRITE_DA5n = REG_D0C0;
  /*p33.SEMO*/ RegQN SEMO_SPRITE_DA6n = REG_D0C0;
  /*p33.SEGA*/ RegQN SEGA_SPRITE_DA7n = REG_D0C0;

  //----------------------------------------
  // BUS_VRAM_D* must _not_ be inverting, see CBD->VBD->VPD chain

  Bus BUS_VRAM_D0p = TRI_D0NP;
  Bus BUS_VRAM_D1p = TRI_D0NP;
  Bus BUS_VRAM_D2p = TRI_D0NP;
  Bus BUS_VRAM_D3p = TRI_D0NP;
  Bus BUS_VRAM_D4p = TRI_D0NP;
  Bus BUS_VRAM_D5p = TRI_D0NP;
  Bus BUS_VRAM_D6p = TRI_D0NP;
  Bus BUS_VRAM_D7p = TRI_D0NP;

  Bus BUS_VRAM_A00n = TRI_HZPU; // This bus isn't driven between tile fetches while rendering; where's the pullup?
  Bus BUS_VRAM_A01n = TRI_HZPU;
  Bus BUS_VRAM_A02n = TRI_HZPU;
  Bus BUS_VRAM_A03n = TRI_HZPU;
  Bus BUS_VRAM_A04n = TRI_HZPU;
  Bus BUS_VRAM_A05n = TRI_HZPU;
  Bus BUS_VRAM_A06n = TRI_HZPU;
  Bus BUS_VRAM_A07n = TRI_HZPU;
  Bus BUS_VRAM_A08n = TRI_HZPU;
  Bus BUS_VRAM_A09n = TRI_HZPU;
  Bus BUS_VRAM_A10n = TRI_HZPU;
  Bus BUS_VRAM_A11n = TRI_HZPU;
  Bus BUS_VRAM_A12n = TRI_HZPU;

  //----------------------------------------
  // VRAM pins

  Pin PIN_VRAM_CSn  = TRI_D0NP; // PIN_43
  Pin PIN_VRAM_OEn  = TRI_D0NP; // PIN_45
  Pin PIN_VRAM_WRn  = TRI_D0NP; // PIN_49

  Pin PIN_VRAM_A00p = TRI_D0NP; // PIN_34
  Pin PIN_VRAM_A01p = TRI_D0NP; // PIN_35
  Pin PIN_VRAM_A02p = TRI_D0NP; // PIN_36
  Pin PIN_VRAM_A03p = TRI_D0NP; // PIN_37
  Pin PIN_VRAM_A04p = TRI_D0NP; // PIN_38
  Pin PIN_VRAM_A05p = TRI_D0NP; // PIN_39
  Pin PIN_VRAM_A06p = TRI_D0NP; // PIN_40
  Pin PIN_VRAM_A07p = TRI_D0NP; // PIN_41
  Pin PIN_VRAM_A08p = TRI_D0NP; // PIN_48
  Pin PIN_VRAM_A09p = TRI_D0NP; // PIN_47
  Pin PIN_VRAM_A10p = TRI_D0NP; // PIN_44
  Pin PIN_VRAM_A11p = TRI_D0NP; // PIN_46
  Pin PIN_VRAM_A12p = TRI_D0NP; // PIN_42

  Pin PIN_VRAM_D00p = TRI_HZPU; // PIN_33 // This pin isn't driven between tile fetches while rendering; where's the pullup?
  Pin PIN_VRAM_D01p = TRI_HZPU; // PIN_31
  Pin PIN_VRAM_D02p = TRI_HZPU; // PIN_30
  Pin PIN_VRAM_D03p = TRI_HZPU; // PIN_29
  Pin PIN_VRAM_D04p = TRI_HZPU; // PIN_28
  Pin PIN_VRAM_D05p = TRI_HZPU; // PIN_27
  Pin PIN_VRAM_D06p = TRI_HZPU; // PIN_26
  Pin PIN_VRAM_D07p = TRI_HZPU; // PIN_25
};

//-----------------------------------------------------------------------------

}; // namespace Schematics