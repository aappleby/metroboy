#include "MtIterator.h"

const MtHandle MtHandle::null;

MtHandle MtHandle::get_field(int field_id) {

  for (auto c : *this) {
    if (c.field == field_id) return c;
  }

  return MtHandle::null;
}


//------------------------------------------------------------------------------
// Field introspection

bool MtHandle::field_is_primitive() {

  auto node_type = get_field(field_type);
  auto node_decl = get_field(field_declarator);

  // Primitive types are primitive types.
  if (node_type.sym == sym_primitive_type) return true;

  // Logic arrays are primitive types.
  if (node_type.sym == sym_template_type) {
    auto templ_name = node_type.get_field(field_name);
    if (templ_name.match("logic")) return true;
  }

  // Bits are primitive types.
  if (node_type.match("bit")) return true;

  return false;
}

bool MtHandle::field_is_module() {
  return !field_is_primitive();
}

bool MtHandle::field_is_static() {
  for (auto c : *this) {
    if (c.sym == sym_storage_class_specifier) {
      if (c.match("static")) return true;
    }
  }
  return false;
}

bool MtHandle::field_is_const() {
  for (auto c : *this) {
    if (c.sym == sym_type_qualifier) {
      if (c.match("const")) return true;
    }
  }
  return false;
}

bool MtHandle::field_is_param() {
  return field_is_static() && field_is_const();
}

bool MtHandle::field_is_input() {
  if (field_is_static() || field_is_const()) return false;

  auto name = get_field(field_declarator);
  return name.body().starts_with("i_");
}

bool MtHandle::field_is_output() {
  if (field_is_static() || field_is_const()) return false;

  auto name = get_field(field_declarator);
  return name.body().starts_with("o_");
}

