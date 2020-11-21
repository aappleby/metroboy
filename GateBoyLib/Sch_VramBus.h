#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramBus {

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

  //DelayGlitch latch_sprite_a_delay;
  //DelayGlitch latch_sprite_b_delay;
};

//-----------------------------------------------------------------------------
