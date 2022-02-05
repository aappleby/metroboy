#include "MtIterator.h"

const MtHandle MtHandle::null;

MtHandle MtHandle::get_field(int field_id) {

  for (auto c : *this) {
    if (c.field == field_id) return c;
  }

  return MtHandle::null;
}
