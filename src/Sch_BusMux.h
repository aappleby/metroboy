#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct BusMux {
  BusMux();

  void tick(SchematicTop& top);
  void tock(SchematicTop& top);
  SignalHash commit(SchematicTop& top);

  //-----------------------------------------------------------------------------

private:
};

//-----------------------------------------------------------------------------

}; // namespace Schematics