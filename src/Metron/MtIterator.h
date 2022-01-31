#pragma once

#include "tree_sitter/api.h"

//------------------------------------------------------------------------------

struct MtHandle {
  TSNode node;
  TSSymbol sym;
  int field;
  int index;

  operator TSNode() const { return node; }
};

//------------------------------------------------------------------------------

struct MtIterator {

  MtIterator& operator++() {
    child_index++;
    return *this;
  }

  bool operator != (const MtIterator& n) {
    if (node.context[0] != n.node.context[0]) return true;
    if (node.context[1] != n.node.context[1]) return true;
    if (node.context[2] != n.node.context[2]) return true;
    if (node.context[3] != n.node.context[3]) return true;
    if (node.id != n.node.id)         return true;
    if (node.tree != n.node.tree)       return true;

    if (child_index != n.child_index) return true;

    return false;
  }

  MtHandle operator*() const {
    auto child = ts_node_child(node, child_index);
    return {
      child,
      ts_node_symbol(child),
      ts_node_field_id_for_child(node, child_index),
      child_index
    };
  }

  TSNode node;
  int child_index;
};

inline MtIterator begin(TSNode& parent) {
  return { parent, 0 };
}

inline MtIterator end(TSNode& parent) {
  return { parent, (int)ts_node_child_count(parent) };
}

inline MtIterator begin(MtIterator& it) {
  auto child = ts_node_child(it.node, it.child_index);
  return { child, 0 };
}

inline MtIterator end(MtIterator& it) {
  auto child = ts_node_child(it.node, it.child_index);
  return { child, (int)ts_node_child_count(child) };
}

//------------------------------------------------------------------------------
