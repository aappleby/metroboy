#pragma once
#include "Platform.h"

#include "MtNode.h"

//------------------------------------------------------------------------------

struct MtParam : public MtNode {
  MtParam(MtNode n) : MtNode(n) {
    assert(sym == sym_parameter_declaration ||
           sym == sym_optional_parameter_declaration || 
           sym == sym_field_declaration);

    if (sym == sym_parameter_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else if (sym == sym_optional_parameter_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else if (sym == sym_field_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else {
      n.dump_tree();
      debugbreak();
    }
  }
  std::string name;
};

//------------------------------------------------------------------------------
