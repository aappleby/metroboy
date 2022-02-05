#pragma once

#include "tree_sitter/api.h"
#include "../Plait/TreeSymbols.h"
#include <compare>
#include <algorithm>
#include <assert.h>
#include <string>

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
  const char* source;

  static const MtHandle null;

  //----------

  MtHandle() {
    this->node = { 0 };
    this->sym = 0;
    this->field = 0;
    this->source = nullptr;
  }

  MtHandle(TSNode node, int sym, int field, const char* source) {
    this->node = node;
    this->sym = sym;
    this->field = field;
    this->source = source;
  }

  static MtHandle from_tree(TSTree* tree, const char* source) {
    auto root = ts_tree_root_node(tree);

    return MtHandle(
      root,
      ts_node_symbol(root),
      0,
      source
    );
  }

  //----------

  operator bool() const { return !ts_node_is_null(node); }
  std::strong_ordering operator<=>(const MtHandle& b) const { return node <=> b.node; }

  const char* type() const { return ts_node_type(node); }
  uint32_t start_byte() const { return ts_node_start_byte(node); }
  uint32_t end_byte()   const { return ts_node_end_byte(node); }

  //----------

  int child_count() const { return (int)ts_node_child_count(node); }

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

  MtHandle get_field(int field_id);

  //----------

  MtHandle child(int i) const {
    auto child = ts_node_child(node, i);
    if (ts_node_is_null(child)) {
      return MtHandle::null;
    }
    else {
      auto sym = ts_node_symbol(child);
      auto field = ts_node_field_id_for_child(node, i);
      return { child, sym, field, source };
    }
  }

  //----------

  MtHandle first_named_child() const {
    for (int i = 0; i < child_count(); i++) {
      auto c = child(i);
      if (c.is_named()) return c;
    }
    return MtHandle::null;
  }

  std::string body() {
    assert(!is_null());

    auto a = &source[start_byte()];
    auto b = &source[end_byte()];

    if (sym == anon_sym_LF) return a;

    while (a < b && isspace(a[0])) a++;
    while (b > a && isspace(b[-1])) b--;

    return std::string(a, b);
  }

  const char* start() {
    auto a = &source[start_byte()];
    auto b = &source[end_byte()];

    if (sym == anon_sym_LF) return a;

    while (a < b && isspace(a[0])) a++;
    return a;
  }

  const char* end() {
    auto a = &source[start_byte()];
    auto b = &source[end_byte()];

    if (sym == anon_sym_LF) return b;

    while (b > a && isspace(b[-1])) b--;
    return b;
  }

  bool match(const char* s) {
    const char* a = start();
    const char* b = end();

    while (a != b) {
      if (*a++ != *s++)  return false;
    }
    return true;
  }

  // Field introspection
  bool field_is_primitive();
  bool field_is_module();
  bool field_is_static();
  bool field_is_const();
  bool field_is_param();
  bool field_is_input();
  bool field_is_output();


  std::string node_to_name() {

    switch (sym) {

    case sym_field_expression:
    case alias_sym_type_identifier:
    case sym_identifier:
    case alias_sym_field_identifier:
      return body();

    case sym_field_declaration:
    case sym_array_declarator:
    case sym_parameter_declaration:
    case sym_optional_parameter_declaration:
    case sym_function_definition:
    case sym_function_declarator:
      return get_field(field_declarator).node_to_name();

    case sym_struct_specifier:
    case sym_class_specifier:
      return get_field(field_name).node_to_name();

    default:
      //dump_tree(n);
      //debugbreak();
      return "";
    }
  }

  std::string node_to_type() {
    switch (sym) {
    case alias_sym_type_identifier:
      return body();

    case sym_field_declaration:
      return get_field(field_type).node_to_type();

    case sym_template_type:
      return get_field(field_name).node_to_type();

    default:
      //dump_tree(n);
      //debugbreak();
      return "";
    }
  }


};

//------------------------------------------------------------------------------

struct MtIterator {

  MtIterator(MtHandle parent) {
    if (ts_node_is_null(parent.node)) {
      cursor = { 0 };
    }
    else {
      cursor = ts_tree_cursor_new(parent.node);

      if (!ts_tree_cursor_goto_first_child(&cursor)) {
        ts_tree_cursor_delete(&cursor);
        cursor = { 0 };
      }
    }
    source = parent.source;
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
      field,
      source
    };
  }

  TSTreeCursor cursor;
  const char* source;
};

inline MtIterator begin(MtHandle parent) {
  return MtIterator(parent);
}

inline MtIterator end(MtHandle parent) {
  return MtIterator(MtHandle::null);
}

//------------------------------------------------------------------------------
