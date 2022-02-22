#include "MtField.h"
#include "MtModule.h"
#include "MtModLibrary.h"

const MtField MtField::null;

//------------------------------------------------------------------------------

MtField::MtField(MtNode _decl, MtNode _type, MtNode _name)
  : decl(_decl), type(_type), name(_name) {
  sanity_check();
}

//------------------------------------------------------------------------------

void MtField::sanity_check() {
  if (decl.is_null()) {
    decl.error();
  }

  bool type_ok = false;
  if (type.sym == sym_template_type) type_ok = true;
  if (type.sym == sym_qualified_identifier) type_ok = true;
  if (type.sym == alias_sym_type_identifier) type_ok = true;
  if (type.sym == sym_enum_specifier) type_ok = true;
  if (type.sym == sym_primitive_type) type_ok = true;
  if (!type_ok) {
    type.error();
  }

  if (!name.is_null()) {
    bool name_ok = false;
    if (name.sym == alias_sym_field_identifier) name_ok = true;
    if (name.sym == sym_identifier) name_ok = true; // for tick/tock parameters
    if (name.sym == sym_array_declarator) name_ok = true;
    if (!name_ok) {
      name.error();
    }
  }

}

//------------------------------------------------------------------------------

bool MtField::is_primitive() {

  // Primitive types are primitive types.
  if (type.sym == sym_primitive_type) return true;

  // Logic arrays are primitive types.
  if (type.sym == sym_template_type) {
    auto templ_name = type.get_field(field_name);
    if (templ_name.match("logic")) return true;
  }

  // Bits are primitive types.
  if (type.match("bit")) return true;

  return false;
}

bool MtField::is_enum() {

  /*
  if (type.child_count() >= 3 &&
      type.child(0).body() == "enum" &&
      type.child(1).body() == "class" && 
      type.child(2).sym == alias_sym_type_identifier) {
    emit_sym_field_declaration_as_enum_class(decl);
  */

  return type.sym == sym_enum_specifier;
}

bool MtField::is_module() {
  return decl.mod->lib->find_module(name.body());

}

bool MtField::is_static() {
  for (auto c : decl) {
    if (c.sym == sym_storage_class_specifier) {
      if (c.match("static")) return true;
    }
  }
  return false;
}

bool MtField::is_const() {
  for (auto c : decl) {
    if (c.sym == sym_type_qualifier) {
      if (c.match("const")) return true;
    }
  }
  return false;
}

bool MtField::is_param() {
  return is_static() && is_const();
}

bool MtField::is_input() {  
  if (is_static() || is_const() || is_enum()) return false;

  auto base_name = name.sym == sym_array_declarator ? name.get_field(field_declarator).body() : name.body();
  return base_name.starts_with("i_") || base_name.ends_with("_i");
}

bool MtField::is_output() {
  if (is_static() || is_const() || is_enum()) return false;
  auto base_name = name.sym == sym_array_declarator ? name.get_field(field_declarator).body() : name.body();
  return base_name.starts_with("o_") || base_name.ends_with("_o");
}

//------------------------------------------------------------------------------
