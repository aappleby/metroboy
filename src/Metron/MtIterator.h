#pragma once

#include "tree_sitter/api.h"
#include "../Plait/TreeSymbols.h"

//------------------------------------------------------------------------------

struct MtHandle {
  TSNode node;
  TSSymbol sym;
  int field;

  MtHandle() {
    this->node = { 0 };
    this->sym = 0;
    this->field = 0;
  }

  MtHandle(TSNode n, int sym, int field) {
    this->node = n;
    this->sym = sym;
    this->field = field;
  }

  MtHandle(TSNode n) {
    if (ts_node_is_null(n)) {
      this->node = { 0 };
      this->sym = 0;
      this->field = 0;
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

  int child_count() const { return (int)ts_node_child_count(node); }

  MtHandle child(int i) const {
    auto n = ts_node_child(node, i);
    auto s = ts_node_symbol(n);
    auto f = ts_node_field_id_for_child(node, i);
    return { n, s, f };
  }

  int named_child_count() const { return (int)ts_node_named_child_count(node); }

  MtHandle named_child(int i) const {
    auto n = ts_node_named_child(node, i);
    auto s = ts_node_symbol(n);
    TSTreeCursor cursor = ts_tree_cursor_new(n);
    auto f = ts_tree_cursor_current_field_id(&cursor);
    ts_tree_cursor_delete(&cursor);
    return { n, s, f };
  }

  bool is_null()       const { return ts_node_is_null(node); }
  bool is_named()      const { return !is_null() && ts_node_is_named(node); }
  bool is_missing()    const { return !is_null() && ts_node_is_missing(node); }
  bool is_extra()      const { return !is_null() && ts_node_is_extra(node); }
  bool is_leaf()       const { return !is_null() && is_named() && !child_count(); }
  bool is_branch()     const { return !is_null() && is_named() && child_count(); }

  bool is_identifier() const { return !is_null() && sym == sym_identifier; }
  bool is_comment()    const { return !is_null() && sym == sym_comment; }
  bool is_decl()       const { return !is_null() && sym == sym_declaration; }
  bool is_field_decl() const { return !is_null() && sym == sym_field_declaration; }
  bool is_func_decl()  const { return !is_null() && sym == sym_function_declarator; }
  bool is_expression() const { return !is_null() && sym == sym_expression_statement; }
  bool is_call()       const { return !is_null() && sym == sym_call_expression; }
  bool is_init_decl()  const { return !is_null() && sym == sym_init_declarator; }
  bool is_function()   const { return !is_null() && sym == sym_function_definition; }
  bool is_assignment() const { return !is_null() && sym == sym_assignment_expression; }
  bool is_field_id()   const { return !is_null() && sym == alias_sym_field_identifier; }
  bool is_field_expr() const { return !is_null() && sym == sym_field_expression; }
  bool is_call_expr()  const { return !is_null() && sym == sym_call_expression; }
  bool is_arglist()    const { return !is_null() && sym == sym_argument_list; }


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
