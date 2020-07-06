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

  /*p01.AFUR*/ Reg9 AFUR_xBCDExxx;
  /*p01.ALEF*/ Reg9 ALEF_xxCDEFxx;
  /*p01.APUK*/ Reg9 APUK_xxxDEFGx;
  /*p01.ADYK*/ Reg9 ADYK_xxxxEFGH;

  /*p29.WUVU*/ Reg17 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg17 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg17 WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics