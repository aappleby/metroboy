#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void tick(SchematicTop& top);
  SignalHash commit();

//private:
  //friend SchematicTop;

  /*p01.AFUR*/ Reg9p AFUR_xBCDExxx; // these have positive reset
  /*p01.ALEF*/ Reg9p ALEF_xxCDEFxx; // these have positive reset
  /*p01.APUK*/ Reg9p APUK_xxxDEFGx; // these have positive reset
  /*p01.ADYK*/ Reg9p ADYK_xxxxEFGH; // these have positive reset

  /*p29.WUVU*/ Reg17 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg17 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg17 WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics