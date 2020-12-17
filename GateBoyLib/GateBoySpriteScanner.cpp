#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyOamBus.h"

//------------------------------------------------------------------------------------------------------------------------

void SpriteScanner::tock(wire XUPY_ABxxEFxx, wire ANOM_LINE_RSTn) {
/* p28.YFEL*/ YFEL_SCAN0.RSTn(ANOM_LINE_RSTn);
/* p28.WEWY*/ WEWY_SCAN1.RSTn(ANOM_LINE_RSTn);
/* p28.GOSO*/ GOSO_SCAN2.RSTn(ANOM_LINE_RSTn);
/* p28.ELYN*/ ELYN_SCAN3.RSTn(ANOM_LINE_RSTn);
/* p28.FAHA*/ FAHA_SCAN4.RSTn(ANOM_LINE_RSTn);
/* p28.FONY*/ FONY_SCAN5.RSTn(ANOM_LINE_RSTn);

/* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp(), XUPY_ABxxEFxx);
/* p28.YFEL*/ YFEL_SCAN0.dff17(GAVA_SCAN_CLOCKp,    ANOM_LINE_RSTn, YFEL_SCAN0.qn());
/* p28.WEWY*/ WEWY_SCAN1.dff17(YFEL_SCAN0.qn(), ANOM_LINE_RSTn, WEWY_SCAN1.qn());
/* p28.GOSO*/ GOSO_SCAN2.dff17(WEWY_SCAN1.qn(), ANOM_LINE_RSTn, GOSO_SCAN2.qn());
/* p28.ELYN*/ ELYN_SCAN3.dff17(GOSO_SCAN2.qn(), ANOM_LINE_RSTn, ELYN_SCAN3.qn());
/* p28.FAHA*/ FAHA_SCAN4.dff17(ELYN_SCAN3.qn(), ANOM_LINE_RSTn, FAHA_SCAN4.qn());
/* p28.FONY*/ FONY_SCAN5.dff17(FAHA_SCAN4.qn(), ANOM_LINE_RSTn, FONY_SCAN5.qn());
}

//------------------------------------------------------------------------------------------------------------------------