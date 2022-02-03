#pragma once

#include "tree_sitter/api.h"
#include "../Plait/TreeSymbols.h"
#include <compare>
#include <algorithm>

//------------------------------------------------------------------------------

inline std::strong_ordering operator<=>(const TSNode& a, const TSNode& b) {
  constexpr auto eq = std::strong_ordering::equal;
  if (auto x = (a.context[0] <=> b.context[0]); x != eq) return x;
  if (auto x = (a.context[1] <=> b.context[1]); x != eq) return x;
  if (auto x = (a.context[2] <=> b.context[2]); x != eq) return x;
  if (auto x = (a.context[3] <=> b.context[3]); x != eq) return x;
  if (auto x = (a.id         <=> b.id);         x != eq) return x;
  if (auto x = (a.tree       <=> b.tree);       x != eq) return x;
  return eq;
}

inline std::strong_ordering operator<=>(const TSTreeCursor& a, const TSTreeCursor& b) {
  constexpr auto eq = std::strong_ordering::equal;
  if (auto x = a.context[0] <=> b.context[0]; x != eq) return x;
  if (auto x = a.context[1] <=> b.context[1]; x != eq) return x;
  if (auto x = a.tree       <=> b.tree;       x != eq) return x;
  if (auto x = a.id         <=> b.id;         x != eq) return x;
  return eq;
}


//------------------------------------------------------------------------------

struct MtHandle {
  TSNode node;
  TSSymbol sym;
  int field;
  static const MtHandle null;

  //----------

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

  //----------

  operator bool() const { return !ts_node_is_null(node); }
  std::strong_ordering operator<=>(const MtHandle& b) const { return node <=> b.node; }

  const char* type() const { return ts_node_type(node); }
  uint32_t start_byte() const { return ts_node_start_byte(node); }
  uint32_t end_byte()   const { return ts_node_end_byte(node); }

  //----------

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

  //----------

  MtHandle get_field(int field_id) {
    auto child = ts_node_child_by_field_id(node, field_id);
    if (ts_node_is_null(child)) {
      return MtHandle::null;
    }
    else {
      return MtHandle(child, ts_node_symbol(child), field_id);
    }
  }

  //----------

  int child_count() const { return (int)ts_node_child_count(node); }

  MtHandle child(int i) const {
    auto n = ts_node_child(node, i);
    if (ts_node_is_null(n)) {
      return null;
    }
    else {
      auto s = ts_node_symbol(n);
      auto f = ts_node_field_id_for_child(node, i);
      return { n, s, f };
    }
  }

  //----------

  int named_child_count() const { return (int)ts_node_named_child_count(node); }

  MtHandle named_child(int i) const {
    auto n = ts_node_named_child(node, i);
    if (ts_node_is_null(n)) {
      return null;
    }
    else {
      auto s = ts_node_symbol(n);
      TSTreeCursor cursor = ts_tree_cursor_new(n);
      auto f = ts_tree_cursor_current_field_id(&cursor);
      ts_tree_cursor_delete(&cursor);
      return { n, s, f };
    }
  }
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

  std::strong_ordering operator<=>(const MtIterator& b) const { return cursor <=> b.cursor; }
  bool operator != (const MtIterator& b) const { return (*this <=> b) != std::strong_ordering::equal; }

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
  return MtIterator(parent.node);
}

inline MtIterator end(MtHandle parent) {
  return MtIterator({ 0 });
}

//------------------------------------------------------------------------------
