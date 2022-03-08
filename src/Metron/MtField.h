#pragma once
#include "MtNode.h"
#include "Platform.h"

//------------------------------------------------------------------------------

struct MtField : public MtNode {
  MtField(MtNode n, std::string _type_name, std::string _name)
      : MtNode(n), name(_name), type_name(_type_name) {
    for (auto c : (MtNode) * this) {
      if (c.sym == sym_storage_class_specifier) {
        if (c.match("static")) is_static = true;
        ;
      }
      if (c.sym == sym_type_qualifier) {
        if (c.match("const")) is_const = true;
      }
    }
  }

  std::string name;
  std::string type_name;
  int index = 0;  // if this is part of a multi-decl, index of the sub-decl. not
                  // hooked up yet.
  bool is_static = false;
  bool is_const = false;
};

//------------------------------------------------------------------------------
