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

  /*p01.AFUR*/ Reg9 AFUR_ABCDxxxx;
  /*p01.ALEF*/ Reg9 ALEF_xBCDExxx;
  /*p01.APUK*/ Reg9 APUK_xxCDEFxx;
  /*p01.ADYK*/ Reg9 ADYK_xxxDEFGx;

  /*p29.WUVU*/ Reg17 WUVU_ABxxEFxx;
  /*p21.VENA*/ Reg17 VENA_ABxxxxGH;
  /*p29.WOSU*/ Reg17 WOSU_AxxDExxH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics