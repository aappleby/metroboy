#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== SpriteScan =====\001\n");

  d("SCAN INDEX       %02d\n", pack_p(YFEL_SCAN0.qp17(), WEWY_SCAN1.qp17(), GOSO_SCAN2.qp17(), ELYN_SCAN3.qp17(),
                                     FAHA_SCAN4.qp17(), FONY_SCAN5.qp17(), 0, 0));
  d("SPRITE INDEX     %02d\n", pack_p(XADU_SPRITE_IDX0p.qn12(), XEDY_SPRITE_IDX1p.qn12(), ZUZE_SPRITE_IDX2p.qn12(), XOBE_SPRITE_IDX3p.qn12(),
                                      YDUF_SPRITE_IDX4p.qn12(), XECU_SPRITE_IDX5p.qn12(), 0, 0));

  d("BESU_SCANNINGp   %c\n", BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", DOBA_SCAN_DONE_B.c());
  d("\n");
  d("LCD Y      %03d\n", top.lcd_reg.get_ly());
  d("Y DIFF     %03d\n", pack_p(DEGE_SPRITE_DELTA0, DABY_SPRITE_DELTA1, DABU_SPRITE_DELTA2, GYSA_SPRITE_DELTA3,
                                GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7));
  d("CARE_STORE_ENp_ABxxEFxx %d\n", (wire)CARE_STORE_ENp_ABxxEFxx);

  d("\n");
}

//------------------------------------------------------------------------------
