#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramBus {
  void reset() {}

  void reset_bootrom() {
    LEGU_TILE_DA0n.reset(REG_D0C0);
    NUDU_TILE_DA1n.reset(REG_D0C0);
    MUKU_TILE_DA2n.reset(REG_D0C0);
    LUZO_TILE_DA3n.reset(REG_D0C0);
    MEGU_TILE_DA4n.reset(REG_D0C0);
    MYJY_TILE_DA5n.reset(REG_D0C0);
    NASA_TILE_DA6n.reset(REG_D0C0);
    NEFO_TILE_DA7n.reset(REG_D0C0);
    RAWU_TILE_DB0p.reset(REG_D0C0);
    POZO_TILE_DB1p.reset(REG_D0C0);
    PYZO_TILE_DB2p.reset(REG_D0C0);
    POXA_TILE_DB3p.reset(REG_D0C0);
    PULO_TILE_DB4p.reset(REG_D0C0);
    POJU_TILE_DB5p.reset(REG_D0C0);
    POWY_TILE_DB6p.reset(REG_D0C0);
    PYJU_TILE_DB7p.reset(REG_D0C0);

    PEFO_SPRITE_DB0n.reset(REG_D0C0);
    ROKA_SPRITE_DB1n.reset(REG_D0C0);
    MYTU_SPRITE_DB2n.reset(REG_D0C0);
    RAMU_SPRITE_DB3n.reset(REG_D0C0);
    SELE_SPRITE_DB4n.reset(REG_D0C0);
    SUTO_SPRITE_DB5n.reset(REG_D0C0);
    RAMA_SPRITE_DB6n.reset(REG_D0C0);
    RYDU_SPRITE_DB7n.reset(REG_D0C0);
    REWO_SPRITE_DA0n.reset(REG_D0C0);
    PEBA_SPRITE_DA1n.reset(REG_D0C0);
    MOFO_SPRITE_DA2n.reset(REG_D0C0);
    PUDU_SPRITE_DA3n.reset(REG_D0C0);
    SAJA_SPRITE_DA4n.reset(REG_D0C0);
    SUNY_SPRITE_DA5n.reset(REG_D0C0);
    SEMO_SPRITE_DA6n.reset(REG_D0C0);
    SEGA_SPRITE_DA7n.reset(REG_D0C0);

    BUS_VRAM_D0p .reset(TRI_D0NP);
    BUS_VRAM_D1p .reset(TRI_D0NP);
    BUS_VRAM_D2p .reset(TRI_D0NP);
    BUS_VRAM_D3p .reset(TRI_D0NP);
    BUS_VRAM_D4p .reset(TRI_D0NP);
    BUS_VRAM_D5p .reset(TRI_D0NP);
    BUS_VRAM_D6p .reset(TRI_D0NP);
    BUS_VRAM_D7p .reset(TRI_D0NP);
    BUS_VRAM_A00n.reset(TRI_HZPU);
    BUS_VRAM_A01n.reset(TRI_HZPU);
    BUS_VRAM_A02n.reset(TRI_HZPU);
    BUS_VRAM_A03n.reset(TRI_HZPU);
    BUS_VRAM_A04n.reset(TRI_HZPU);
    BUS_VRAM_A05n.reset(TRI_HZPU);
    BUS_VRAM_A06n.reset(TRI_HZPU);
    BUS_VRAM_A07n.reset(TRI_HZPU);
    BUS_VRAM_A08n.reset(TRI_HZPU);
    BUS_VRAM_A09n.reset(TRI_HZPU);
    BUS_VRAM_A10n.reset(TRI_HZPU);
    BUS_VRAM_A11n.reset(TRI_HZPU);
    BUS_VRAM_A12n.reset(TRI_HZPU);
    PIN_VRAM_CSn .reset(TRI_D0NP);
    PIN_VRAM_OEn .reset(TRI_D0NP);
    PIN_VRAM_WRn .reset(TRI_D0NP);
    PIN_VRAM_A00p.reset(TRI_D0NP);
    PIN_VRAM_A01p.reset(TRI_D0NP);
    PIN_VRAM_A02p.reset(TRI_D0NP);
    PIN_VRAM_A03p.reset(TRI_D0NP);
    PIN_VRAM_A04p.reset(TRI_D0NP);
    PIN_VRAM_A05p.reset(TRI_D0NP);
    PIN_VRAM_A06p.reset(TRI_D0NP);
    PIN_VRAM_A07p.reset(TRI_D0NP);
    PIN_VRAM_A08p.reset(TRI_D0NP);
    PIN_VRAM_A09p.reset(TRI_D0NP);
    PIN_VRAM_A10p.reset(TRI_D0NP);
    PIN_VRAM_A11p.reset(TRI_D0NP);
    PIN_VRAM_A12p.reset(TRI_D0NP);
    PIN_VRAM_D00p.reset(TRI_HZPU);
    PIN_VRAM_D01p.reset(TRI_HZPU);
    PIN_VRAM_D02p.reset(TRI_HZPU);
    PIN_VRAM_D03p.reset(TRI_HZPU);
    PIN_VRAM_D04p.reset(TRI_HZPU);
    PIN_VRAM_D05p.reset(TRI_HZPU);
    PIN_VRAM_D06p.reset(TRI_HZPU);
    PIN_VRAM_D07p.reset(TRI_HZPU);
  }

  void tock(SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  int get_bus_addr() const {
    return pack_p(!BUS_VRAM_A00n.qp(), !BUS_VRAM_A01n.qp(), !BUS_VRAM_A02n.qp(), !BUS_VRAM_A03n.qp(),
                  !BUS_VRAM_A04n.qp(), !BUS_VRAM_A05n.qp(), !BUS_VRAM_A06n.qp(), !BUS_VRAM_A07n.qp(),
                  !BUS_VRAM_A08n.qp(), !BUS_VRAM_A09n.qp(), !BUS_VRAM_A10n.qp(), !BUS_VRAM_A11n.qp(),
                  !BUS_VRAM_A12n.qp(), 0, 0, 0);
  }

  int get_bus_data() const {
    return pack_p(BUS_VRAM_D0p.qp(), BUS_VRAM_D1p.qp(), BUS_VRAM_D2p.qp(), BUS_VRAM_D3p.qp(),
                  BUS_VRAM_D4p.qp(), BUS_VRAM_D5p.qp(), BUS_VRAM_D6p.qp(), BUS_VRAM_D7p.qp());
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

  /*p32.LEGU*/ DFF8 LEGU_TILE_DA0n = ERR_XXXX;
  /*p32.NUDU*/ DFF8 NUDU_TILE_DA1n = ERR_XXXX;
  /*p32.MUKU*/ DFF8 MUKU_TILE_DA2n = ERR_XXXX;
  /*p32.LUZO*/ DFF8 LUZO_TILE_DA3n = ERR_XXXX;
  /*p32.MEGU*/ DFF8 MEGU_TILE_DA4n = ERR_XXXX;
  /*p32.MYJY*/ DFF8 MYJY_TILE_DA5n = ERR_XXXX;
  /*p32.NASA*/ DFF8 NASA_TILE_DA6n = ERR_XXXX;
  /*p32.NEFO*/ DFF8 NEFO_TILE_DA7n = ERR_XXXX; // color wrong on die

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p = ERR_XXXX; // def holds inverted pix, also holds tile index during fetch
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p = ERR_XXXX;
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p = ERR_XXXX;
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p = ERR_XXXX;
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p = ERR_XXXX;
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p = ERR_XXXX;
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p = ERR_XXXX;
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p = ERR_XXXX;

  /*p33.PEFO*/ DFF8 PEFO_SPRITE_DB0n = ERR_XXXX;
  /*p33.ROKA*/ DFF8 ROKA_SPRITE_DB1n = ERR_XXXX;
  /*p33.MYTU*/ DFF8 MYTU_SPRITE_DB2n = ERR_XXXX;
  /*p33.RAMU*/ DFF8 RAMU_SPRITE_DB3n = ERR_XXXX;
  /*p33.SELE*/ DFF8 SELE_SPRITE_DB4n = ERR_XXXX;
  /*p33.SUTO*/ DFF8 SUTO_SPRITE_DB5n = ERR_XXXX;
  /*p33.RAMA*/ DFF8 RAMA_SPRITE_DB6n = ERR_XXXX;
  /*p33.RYDU*/ DFF8 RYDU_SPRITE_DB7n = ERR_XXXX;

  /*p33.REWO*/ DFF8 REWO_SPRITE_DA0n = ERR_XXXX;
  /*p33.PEBA*/ DFF8 PEBA_SPRITE_DA1n = ERR_XXXX;
  /*p33.MOFO*/ DFF8 MOFO_SPRITE_DA2n = ERR_XXXX;
  /*p33.PUDU*/ DFF8 PUDU_SPRITE_DA3n = ERR_XXXX;
  /*p33.SAJA*/ DFF8 SAJA_SPRITE_DA4n = ERR_XXXX;
  /*p33.SUNY*/ DFF8 SUNY_SPRITE_DA5n = ERR_XXXX;
  /*p33.SEMO*/ DFF8 SEMO_SPRITE_DA6n = ERR_XXXX;
  /*p33.SEGA*/ DFF8 SEGA_SPRITE_DA7n = ERR_XXXX;

  //----------------------------------------
  // BUS_VRAM_D* must _not_ be inverting, see CBD->VBD->VPD chain

  Bus BUS_VRAM_D0p = ERR_XXXX;
  Bus BUS_VRAM_D1p = ERR_XXXX;
  Bus BUS_VRAM_D2p = ERR_XXXX;
  Bus BUS_VRAM_D3p = ERR_XXXX;
  Bus BUS_VRAM_D4p = ERR_XXXX;
  Bus BUS_VRAM_D5p = ERR_XXXX;
  Bus BUS_VRAM_D6p = ERR_XXXX;
  Bus BUS_VRAM_D7p = ERR_XXXX;

  Bus BUS_VRAM_A00n = ERR_XXXX; // This bus isn't driven between tile fetches while rendering; where's the pullup?
  Bus BUS_VRAM_A01n = ERR_XXXX;
  Bus BUS_VRAM_A02n = ERR_XXXX;
  Bus BUS_VRAM_A03n = ERR_XXXX;
  Bus BUS_VRAM_A04n = ERR_XXXX;
  Bus BUS_VRAM_A05n = ERR_XXXX;
  Bus BUS_VRAM_A06n = ERR_XXXX;
  Bus BUS_VRAM_A07n = ERR_XXXX;
  Bus BUS_VRAM_A08n = ERR_XXXX;
  Bus BUS_VRAM_A09n = ERR_XXXX;
  Bus BUS_VRAM_A10n = ERR_XXXX;
  Bus BUS_VRAM_A11n = ERR_XXXX;
  Bus BUS_VRAM_A12n = ERR_XXXX;

  //----------------------------------------
  // VRAM pins

  Pin PIN_VRAM_CSn  = ERR_XXXX; // PIN_43
  Pin PIN_VRAM_OEn  = ERR_XXXX; // PIN_45
  Pin PIN_VRAM_WRn  = ERR_XXXX; // PIN_49

  Pin PIN_VRAM_A00p = ERR_XXXX; // PIN_34
  Pin PIN_VRAM_A01p = ERR_XXXX; // PIN_35
  Pin PIN_VRAM_A02p = ERR_XXXX; // PIN_36
  Pin PIN_VRAM_A03p = ERR_XXXX; // PIN_37
  Pin PIN_VRAM_A04p = ERR_XXXX; // PIN_38
  Pin PIN_VRAM_A05p = ERR_XXXX; // PIN_39
  Pin PIN_VRAM_A06p = ERR_XXXX; // PIN_40
  Pin PIN_VRAM_A07p = ERR_XXXX; // PIN_41
  Pin PIN_VRAM_A08p = ERR_XXXX; // PIN_48
  Pin PIN_VRAM_A09p = ERR_XXXX; // PIN_47
  Pin PIN_VRAM_A10p = ERR_XXXX; // PIN_44
  Pin PIN_VRAM_A11p = ERR_XXXX; // PIN_46
  Pin PIN_VRAM_A12p = ERR_XXXX; // PIN_42

  Pin PIN_VRAM_D00p = ERR_XXXX; // PIN_33 // This pin isn't driven between tile fetches while rendering; where's the pullup?
  Pin PIN_VRAM_D01p = ERR_XXXX; // PIN_31
  Pin PIN_VRAM_D02p = ERR_XXXX; // PIN_30
  Pin PIN_VRAM_D03p = ERR_XXXX; // PIN_29
  Pin PIN_VRAM_D04p = ERR_XXXX; // PIN_28
  Pin PIN_VRAM_D05p = ERR_XXXX; // PIN_27
  Pin PIN_VRAM_D06p = ERR_XXXX; // PIN_26
  Pin PIN_VRAM_D07p = ERR_XXXX; // PIN_25
};

//-----------------------------------------------------------------------------

}; // namespace Schematics