#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyOamBus.h"

//------------------------------------------------------------------------------------------------------------------------

void SpriteScanner::tock(wire2 XUPY_ABxxEFxx, wire2 ANOM_LINE_RSTn) {

  for (int feedback = 0; feedback < 2; feedback++) {
    /* p28.GAVA*/ wire2 GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp_any(), XUPY_ABxxEFxx);
    /* p28.YFEL*/ YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,    ANOM_LINE_RSTn, YFEL_SCAN0.qn_any2());
    /* p28.WEWY*/ WEWY_SCAN1.dff17_any(YFEL_SCAN0.qn_any2(), ANOM_LINE_RSTn, WEWY_SCAN1.qn_any2());
    /* p28.GOSO*/ GOSO_SCAN2.dff17_any(WEWY_SCAN1.qn_any2(), ANOM_LINE_RSTn, GOSO_SCAN2.qn_any2());
    /* p28.ELYN*/ ELYN_SCAN3.dff17_any(GOSO_SCAN2.qn_any2(), ANOM_LINE_RSTn, ELYN_SCAN3.qn_any2());
    /* p28.FAHA*/ FAHA_SCAN4.dff17_any(ELYN_SCAN3.qn_any2(), ANOM_LINE_RSTn, FAHA_SCAN4.qn_any2());
    /* p28.FONY*/ FONY_SCAN5.dff17_any(FAHA_SCAN4.qn_any2(), ANOM_LINE_RSTn, FONY_SCAN5.qn_any2());
  }
}

//------------------------------------------------------------------------------------------------------------------------