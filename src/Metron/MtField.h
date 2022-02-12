#pragma once

#include "MtNode.h"

//------------------------------------------------------------------------------

struct MtField {
  MtField() {}
  MtField(MtNode _decl, MtNode _type, MtNode _name);

  MtNode decl; // "int x, y, z;"
  MtNode type; // "int"
  MtNode name; // "x", "y", "z"

  void sanity_check();

  bool is_primitive();
  bool is_enum();
  bool is_module();
  bool is_static();
  bool is_const();
  bool is_param();
  bool is_input();
  bool is_output();

  std::string node_to_name() { return name.node_to_name(); }
  operator bool() const { return bool(decl); }

  static const MtField null;
};

//------------------------------------------------------------------------------
