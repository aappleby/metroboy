#include "MtIterator.h"

const MtHandle MtHandle::null;

MtHandle MtHandle::get_field(int field_id) {

  /*
  for (int i = 0; i < child_count(); i++) {
    auto c = child(i);
    if (c.field == field_id) return c;
  }
  */

  for (auto c : *this) {
    if (c.field == field_id) return c;
  }

  return MtHandle::null;
}
