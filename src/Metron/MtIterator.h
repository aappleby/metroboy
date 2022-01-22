#pragma once

#include "tree_sitter/api.h"

//------------------------------------------------------------------------------

struct TSNodeIt {

  TSNodeIt& operator++() {
    index++;
    return *this;
  }

  bool operator != (const TSNodeIt& n) {
    if (parent.context[0] != n.parent.context[0]) return true;
    if (parent.context[1] != n.parent.context[1]) return true;
    if (parent.context[2] != n.parent.context[2]) return true;
    if (parent.context[3] != n.parent.context[3]) return true;
    if (parent.id != n.parent.id)         return true;
    if (parent.tree != n.parent.tree)       return true;

    if (index != n.index) return true;

    return false;
  }

  TSNode operator*() const {
    return ts_node_child(parent, index);
  }

  TSNode parent;
  int index;
};

inline TSNodeIt begin(TSNode& parent) {
  return { parent, 0 };
}

inline TSNodeIt end(TSNode& parent) {
  return { parent, (int)ts_node_child_count(parent) };
}

//------------------------------------------------------------------------------
