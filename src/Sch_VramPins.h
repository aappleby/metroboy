#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct VramPins {
  void tick(SchematicTop& gb);
};

//-----------------------------------------------------------------------------

}; // namespace Schematics