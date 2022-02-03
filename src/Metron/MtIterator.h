#pragma once

#include "tree_sitter/api.h"

//------------------------------------------------------------------------------

struct MtHandle {
  TSNode node;
  TSSymbol sym;
  int field;

  MtHandle(TSNode n, int sym, int field) {
    this->node = n;
    this->sym = sym;
    this->field = field;
  }

  MtHandle(TSNode n) {
    if (ts_node_is_null(n)) {
      this->node = { 0 };
      this->sym = -1;
      this->field = -1;
    }
    else {
      TSTreeCursor cursor = ts_tree_cursor_new(n);
      this->node = n;
      this->sym = ts_node_symbol(n);
      this->field = ts_tree_cursor_current_field_id(&cursor);
      ts_tree_cursor_delete(&cursor);
    }
  }

  MtHandle get_field(int field_id) {
    auto child = ts_node_child_by_field_id(node, field_id);
    if (ts_node_is_null(child)) {
      return MtHandle::null;
    }
    else {
      return MtHandle(child, ts_node_symbol(child), field_id);
    }
  }

  operator bool() const {
    return ts_node_is_null(node);
  }

  operator TSNode() const { return node; }

  static const MtHandle null;
};

//------------------------------------------------------------------------------

struct MtIterator {

  MtIterator(TSNode parent) {
    if (ts_node_is_null(parent)) {
      cursor = { 0 };
    }
    else {
      cursor = ts_tree_cursor_new(parent);

      if (!ts_tree_cursor_goto_first_child(&cursor)) {
        ts_tree_cursor_delete(&cursor);
        cursor = { 0 };
      }
    }
  }

  ~MtIterator() {
    if (cursor.tree) {
      ts_tree_cursor_delete(&cursor);
    }
  }

  MtIterator& operator++() {
    if (!ts_tree_cursor_goto_next_sibling(&cursor)) {
      ts_tree_cursor_delete(&cursor);
      cursor = { 0 };
    }
    return *this;
  }

  bool operator != (const MtIterator& n) {
    if (cursor.tree != n.cursor.tree) return true;
    if (cursor.id != n.cursor.id) return true;
    if (cursor.context[0] != n.cursor.context[0]) return true;
    if (cursor.context[1] != n.cursor.context[1]) return true;
    return false;
  }

  MtHandle operator*() const {
    auto child = ts_tree_cursor_current_node(&cursor);
    auto sym = ts_node_symbol(child);
    auto field = ts_tree_cursor_current_field_id(&cursor);

    return {
      child,
      sym,
      field
    };
  }

  TSTreeCursor cursor;
};

inline MtIterator begin(TSNode parent) {
  return MtIterator(parent);
}

inline MtIterator end(TSNode parent) {
  return MtIterator({ 0 });
}

inline MtIterator begin(MtHandle parent) {
  return MtIterator(parent);
}

inline MtIterator end(MtHandle parent) {
  return MtIterator({ 0 });
}

//------------------------------------------------------------------------------
