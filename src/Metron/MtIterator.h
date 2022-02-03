#pragma once

#include "tree_sitter/api.h"

#if 0
TSTreeCursor ts_tree_cursor_new(TSNode);
void ts_tree_cursor_delete(TSTreeCursor*);
void ts_tree_cursor_reset(TSTreeCursor*, TSNode);
TSNode ts_tree_cursor_current_node(const TSTreeCursor*);
const char* ts_tree_cursor_current_field_name(const TSTreeCursor*);
TSFieldId ts_tree_cursor_current_field_id(const TSTreeCursor*);
bool ts_tree_cursor_goto_parent(TSTreeCursor*);
bool ts_tree_cursor_goto_next_sibling(TSTreeCursor*);
bool ts_tree_cursor_goto_first_child(TSTreeCursor*);
#endif

//------------------------------------------------------------------------------

struct MtHandle {
  TSNode node;
  TSSymbol sym;
  int field;

  operator TSNode() const { return node; }
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
