#pragma once
#include "Platform.h"

#include "MtNode.h"

//------------------------------------------------------------------------------

struct MtEnum : public MtNode {
  MtEnum(MtNode n) : MtNode(n) {

    if (n.sym == sym_field_declaration) {
      auto enum_type = n.get_field(field_type);
      auto enum_name = enum_type.get_field(field_name);
      name = enum_name.text();
    }
    else {
      n.dump_tree();
      debugbreak();
    }
  }

  std::string name;
};

//------------------------------------------------------------------------------
