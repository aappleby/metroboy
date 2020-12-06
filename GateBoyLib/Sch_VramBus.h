#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramBus {

  void reset_cart() {
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
