#pragma once

#include "Platform.h"
#include "tree_sitter/api.h"
#include "../Plait/TreeSymbols.h"
#include <compare>
#include <algorithm>
#include <assert.h>
#include <string>
#include <functional>

struct MtHandle;
struct MtModule;

typedef std::function<void(MtHandle)> NodeVisitor;
typedef std::function<void(MtHandle parent, MtHandle child)> NodeVisitor2;

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
  MtModule* mod;

  static const MtHandle null;

  //----------

  MtHandle() {
    this->node = { 0 };
    this->sym = 0;
    this->field = 0;
    this->mod = nullptr;
  }

  MtHandle(TSNode node, int sym, int field, MtModule* mod) {
    this->node = node;
    this->sym = sym;
    this->field = field;
    this->mod = mod;
  }

  static MtHandle from_mod(MtModule* mod);

  //----------

  void dump_node(int index, int depth);
  void dump_tree(int index, int depth, int maxdepth);
  void dump_tree() { dump_tree(0, 0, 255); }

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

  MtHandle child(int i) const;
  MtHandle first_named_child() const;
  std::string body();
  const char* start();
  const char* end();
  bool match(const char* s);

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
      dump_tree();
      debugbreak();
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
      dump_tree();
      debugbreak();
      return "";
    }
  }


  void visit_tree(NodeVisitor cv);
  void visit_tree2(NodeVisitor2 cv);
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
    this->mod = parent.mod;
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
      mod
    };
  }

  TSTreeCursor cursor;
  MtModule* mod;
};

inline MtIterator begin(MtHandle parent) {
  return MtIterator(parent);
}

inline MtIterator end(MtHandle parent) {
  return MtIterator(MtHandle::null);
}

//------------------------------------------------------------------------------
