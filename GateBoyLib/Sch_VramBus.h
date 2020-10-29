#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramBus {
  void dump(Dumper& d, const SchematicTop& /*top*/) const {
    d("\002===== VRAM Bus =====\001\n");

    d("VRAM BUS ADDR : %04x %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      get_bus_addr() | 0x8000,
      BUS_VRAM_A12n.c(), BUS_VRAM_A11n.c(), BUS_VRAM_A10n.c(), BUS_VRAM_A09n.c(),
      BUS_VRAM_A08n.c(), BUS_VRAM_A07n.c(), BUS_VRAM_A06n.c(), BUS_VRAM_A05n.c(),
      BUS_VRAM_A04n.c(), BUS_VRAM_A03n.c(), BUS_VRAM_A02n.c(), BUS_VRAM_A01n.c(),
      BUS_VRAM_A00n.c());
    d("VRAM BUS DATA : %c%c%c%c%c%c%c%c\n",
      BUS_VRAM_D7p.c(), BUS_VRAM_D6p.c(), BUS_VRAM_D5p.c(), BUS_VRAM_D4p.c(),
      BUS_VRAM_D3p.c(), BUS_VRAM_D2p.c(), BUS_VRAM_D1p.c(), BUS_VRAM_D0p.c());
    d("\n");

    d("VRAM PIN MCSn : %c\n", PIN_VRAM_CSn.c());
    d("VRAM PIN MOEn : %c\n", PIN_VRAM_OEn.c());
    d("VRAM PIN MWRn : %c\n", PIN_VRAM_WRn.c());
    d("VRAM PIN ADDR : 0x%04x\n", get_pin_addr() | 0x8000);
    d("VRAM PIN DATA : %02x %c%c%c%c%c%c%c%c\n",
      get_bus_data(),
      PIN_VRAM_D07p.c(), PIN_VRAM_D06p.c(), PIN_VRAM_D05p.c(), PIN_VRAM_D04p.c(),
      PIN_VRAM_D03p.c(), PIN_VRAM_D02p.c(), PIN_VRAM_D01p.c(), PIN_VRAM_D00p.c());
    d("\n");

    int TILE_DA = pack_p(LEGU_TILE_DA0n.qn07(), NUDU_TILE_DA1n.qn07(), MUKU_TILE_DA2n.qn07(), LUZO_TILE_DA3n.qn07(),
                         MEGU_TILE_DA4n.qn07(), MYJY_TILE_DA5n.qn07(), NASA_TILE_DA6n.qn07(), NEFO_TILE_DA7n.qn07());
    int TILE_DB = pack_p(RAWU_TILE_DB0p.q11p(), POZO_TILE_DB1p.q11p(), PYZO_TILE_DB2p.q11p(), POXA_TILE_DB3p.q11p(),
                         PULO_TILE_DB4p.q11p(), POJU_TILE_DB5p.q11p(), POWY_TILE_DB6p.q11p(), PYJU_TILE_DB7p.q11p());
    int SPRITE_DA = pack_p(PEFO_SPRITE_DB0n.qn07(), ROKA_SPRITE_DB1n.qn07(), MYTU_SPRITE_DB2n.qn07(), RAMU_SPRITE_DB3n.qn07(),
                           SELE_SPRITE_DB4n.qn07(), SUTO_SPRITE_DB5n.qn07(), RAMA_SPRITE_DB6n.qn07(), RYDU_SPRITE_DB7n.qn07());
    int SPRITE_DB = pack_p(REWO_SPRITE_DA0n.qn07(), PEBA_SPRITE_DA1n.qn07(), MOFO_SPRITE_DA2n.qn07(), PUDU_SPRITE_DA3n.qn07(),
                           SAJA_SPRITE_DA4n.qn07(), SUNY_SPRITE_DA5n.qn07(), SEMO_SPRITE_DA6n.qn07(), SEGA_SPRITE_DA7n.qn07());

    d("TILE_DA       : 0x%02x\n", TILE_DA);
    d("TILE_DB       : 0x%02x\n", TILE_DB);
    d("SPRITE_DA     : 0x%02x\n", SPRITE_DA);
    d("SPRITE_DB     : 0x%02x\n", SPRITE_DB);
    d("\n");
  }

  void reset_cart() {
    LEGU_TILE_DA0n.reset(REG_D1C1);
    NUDU_TILE_DA1n.reset(REG_D1C1);
    MUKU_TILE_DA2n.reset(REG_D1C1);
    LUZO_TILE_DA3n.reset(REG_D1C1);
    MEGU_TILE_DA4n.reset(REG_D1C1);
    MYJY_TILE_DA5n.reset(REG_D1C1);
    NASA_TILE_DA6n.reset(REG_D1C1);
    NEFO_TILE_DA7n.reset(REG_D1C1);
    RAWU_TILE_DB0p.reset(REG_D0C1);
    POZO_TILE_DB1p.reset(REG_D0C1);
    PYZO_TILE_DB2p.reset(REG_D0C1);
    POXA_TILE_DB3p.reset(REG_D0C1);
    PULO_TILE_DB4p.reset(REG_D0C1);
    POJU_TILE_DB5p.reset(REG_D0C1);
    POWY_TILE_DB6p.reset(REG_D0C1);
    PYJU_TILE_DB7p.reset(REG_D0C1);

    PEFO_SPRITE_DB0n.reset(REG_D1C0);
    ROKA_SPRITE_DB1n.reset(REG_D1C0);
    MYTU_SPRITE_DB2n.reset(REG_D1C0);
    RAMU_SPRITE_DB3n.reset(REG_D1C0);
    SELE_SPRITE_DB4n.reset(REG_D1C0);
    SUTO_SPRITE_DB5n.reset(REG_D1C0);
    RAMA_SPRITE_DB6n.reset(REG_D1C0);
    RYDU_SPRITE_DB7n.reset(REG_D1C0);
    REWO_SPRITE_DA0n.reset(REG_D1C0);
    PEBA_SPRITE_DA1n.reset(REG_D1C0);
    MOFO_SPRITE_DA2n.reset(REG_D1C0);
    PUDU_SPRITE_DA3n.reset(REG_D1C0);
    SAJA_SPRITE_DA4n.reset(REG_D1C0);
    SUNY_SPRITE_DA5n.reset(REG_D1C0);
    SEMO_SPRITE_DA6n.reset(REG_D1C0);
    SEGA_SPRITE_DA7n.reset(REG_D1C0);

    BUS_VRAM_D0p .reset(TRI_D0NP);
    BUS_VRAM_D1p .reset(TRI_D0NP);
    BUS_VRAM_D2p .reset(TRI_D0NP);
    BUS_VRAM_D3p .reset(TRI_D0NP);
    BUS_VRAM_D4p .reset(TRI_D0NP);
    BUS_VRAM_D5p .reset(TRI_D0NP);
    BUS_VRAM_D6p .reset(TRI_D0NP);
    BUS_VRAM_D7p .reset(TRI_D0NP);

    BUS_VRAM_A00n.reset(TRI_D1PU);
    BUS_VRAM_A01n.reset(TRI_D1PU);
    BUS_VRAM_A02n.reset(TRI_D1PU);
    BUS_VRAM_A03n.reset(TRI_D1PU);
    BUS_VRAM_A04n.reset(TRI_D0PU);
    BUS_VRAM_A05n.reset(TRI_D1PU);
    BUS_VRAM_A06n.reset(TRI_D0PU);
    BUS_VRAM_A07n.reset(TRI_D1PU);
    BUS_VRAM_A08n.reset(TRI_D1PU);
    BUS_VRAM_A09n.reset(TRI_D1PU);
    BUS_VRAM_A10n.reset(TRI_D1PU);
    BUS_VRAM_A11n.reset(TRI_D1PU);
    BUS_VRAM_A12n.reset(TRI_D1PU);

    PIN_VRAM_CSn .reset(TRI_D1NP);
    PIN_VRAM_OEn .reset(TRI_D0NP);
    PIN_VRAM_WRn .reset(TRI_D1NP);

    PIN_VRAM_A00p.reset(TRI_D0NP);
    PIN_VRAM_A01p.reset(TRI_D0NP);
    PIN_VRAM_A02p.reset(TRI_D0NP);
    PIN_VRAM_A03p.reset(TRI_D0NP);
    PIN_VRAM_A04p.reset(TRI_D1NP);
    PIN_VRAM_A05p.reset(TRI_D0NP);
    PIN_VRAM_A06p.reset(TRI_D1NP);
    PIN_VRAM_A07p.reset(TRI_D0NP);
    PIN_VRAM_A08p.reset(TRI_D0NP);
    PIN_VRAM_A09p.reset(TRI_D0NP);
    PIN_VRAM_A10p.reset(TRI_D0NP);
    PIN_VRAM_A11p.reset(TRI_D0NP);
    PIN_VRAM_A12p.reset(TRI_D0NP);

    PIN_VRAM_D00p.reset(TRI_D0PU);
    PIN_VRAM_D01p.reset(TRI_D0PU);
    PIN_VRAM_D02p.reset(TRI_D0PU);
    PIN_VRAM_D03p.reset(TRI_D0PU);
    PIN_VRAM_D04p.reset(TRI_D0PU);
    PIN_VRAM_D05p.reset(TRI_D0PU);
    PIN_VRAM_D06p.reset(TRI_D0PU);
    PIN_VRAM_D07p.reset(TRI_D0PU);

    //latch_sprite_a_delay.reset(TRI_D1NP);
    //latch_sprite_b_delay.reset(TRI_D1NP);
  }

  void reset_boot() {
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

    //latch_sprite_a_delay.reset();
    //latch_sprite_b_delay.reset();
  }

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

  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n;
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n;
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n;
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n;
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n;
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n;
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n;
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n; // color wrong on die

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p; // def holds inverted pix, also holds tile index during fetch
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p;
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p;
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p;
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p;
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p;
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p;
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p;

  // switching these to DFF8n breaks sprites

  /*p33.PEFO*/ DFF8n PEFO_SPRITE_DB0n;
  /*p33.ROKA*/ DFF8n ROKA_SPRITE_DB1n;
  /*p33.MYTU*/ DFF8n MYTU_SPRITE_DB2n;
  /*p33.RAMU*/ DFF8n RAMU_SPRITE_DB3n;
  /*p33.SELE*/ DFF8n SELE_SPRITE_DB4n;
  /*p33.SUTO*/ DFF8n SUTO_SPRITE_DB5n;
  /*p33.RAMA*/ DFF8n RAMA_SPRITE_DB6n;
  /*p33.RYDU*/ DFF8n RYDU_SPRITE_DB7n;

  /*p33.REWO*/ DFF8n REWO_SPRITE_DA0n;
  /*p33.PEBA*/ DFF8n PEBA_SPRITE_DA1n;
  /*p33.MOFO*/ DFF8n MOFO_SPRITE_DA2n;
  /*p33.PUDU*/ DFF8n PUDU_SPRITE_DA3n;
  /*p33.SAJA*/ DFF8n SAJA_SPRITE_DA4n;
  /*p33.SUNY*/ DFF8n SUNY_SPRITE_DA5n;
  /*p33.SEMO*/ DFF8n SEMO_SPRITE_DA6n;
  /*p33.SEGA*/ DFF8n SEGA_SPRITE_DA7n;

  //----------------------------------------
  // BUS_VRAM_D* must _not_ be inverting, see CBD->VBD->VPD chain

  Bus BUS_VRAM_D0p;
  Bus BUS_VRAM_D1p;
  Bus BUS_VRAM_D2p;
  Bus BUS_VRAM_D3p;
  Bus BUS_VRAM_D4p;
  Bus BUS_VRAM_D5p;
  Bus BUS_VRAM_D6p;
  Bus BUS_VRAM_D7p;

  Bus BUS_VRAM_A00n; // This bus isn't driven between tile fetches while rendering; where's the pullup?
  Bus BUS_VRAM_A01n;
  Bus BUS_VRAM_A02n;
  Bus BUS_VRAM_A03n;
  Bus BUS_VRAM_A04n;
  Bus BUS_VRAM_A05n;
  Bus BUS_VRAM_A06n;
  Bus BUS_VRAM_A07n;
  Bus BUS_VRAM_A08n;
  Bus BUS_VRAM_A09n;
  Bus BUS_VRAM_A10n;
  Bus BUS_VRAM_A11n;
  Bus BUS_VRAM_A12n;

  //----------------------------------------
  // VRAM pins

  Pin PIN_VRAM_CSn ; // PIN_43
  Pin PIN_VRAM_OEn ; // PIN_45
  Pin PIN_VRAM_WRn ; // PIN_49

  Pin PIN_VRAM_A00p; // PIN_34
  Pin PIN_VRAM_A01p; // PIN_35
  Pin PIN_VRAM_A02p; // PIN_36
  Pin PIN_VRAM_A03p; // PIN_37
  Pin PIN_VRAM_A04p; // PIN_38
  Pin PIN_VRAM_A05p; // PIN_39
  Pin PIN_VRAM_A06p; // PIN_40
  Pin PIN_VRAM_A07p; // PIN_41
  Pin PIN_VRAM_A08p; // PIN_48
  Pin PIN_VRAM_A09p; // PIN_47
  Pin PIN_VRAM_A10p; // PIN_44
  Pin PIN_VRAM_A11p; // PIN_46
  Pin PIN_VRAM_A12p; // PIN_42

  Pin PIN_VRAM_D00p; // PIN_33 // This pin isn't driven between tile fetches while rendering; where's the pullup?
  Pin PIN_VRAM_D01p; // PIN_31
  Pin PIN_VRAM_D02p; // PIN_30
  Pin PIN_VRAM_D03p; // PIN_29
  Pin PIN_VRAM_D04p; // PIN_28
  Pin PIN_VRAM_D05p; // PIN_27
  Pin PIN_VRAM_D06p; // PIN_26
  Pin PIN_VRAM_D07p; // PIN_25

  //DelayGlitch latch_sprite_a_delay;
  //DelayGlitch latch_sprite_b_delay;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics