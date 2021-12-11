#pragma once
#include <tree_sitter/api.h>
#include "../Plait/TreeSymbols.h"

//------------------------------------------------------------------------------

struct Node {
  Node(TSNode tsnode, int depth, int index, uint32_t span_a, uint32_t span_b, int field_id)
    : tsnode(tsnode), depth(depth), index(index), span_a(span_a), span_b(span_b), field_id(field_id) {}

  Node(int depth, int index, uint32_t span_a, uint32_t span_b)
    : depth(depth), index(index), span_a(span_a), span_b(span_b) {}

  int child_count() const { return (int)ts_node_child_count(tsnode); }

  Node child(int i) const {
    TSNode c = ts_node_child(tsnode, i);
    return {
      c,
      depth + 1,
      (int)i,
      ts_node_start_byte(c),
      ts_node_end_byte(c),
      ts_node_field_id_for_child(tsnode, i)
    };
  }

  Node child_by_field_id(TSFieldId field_id) const {
    for (int i = 0; i < (int)ts_node_child_count(tsnode); i++) {
      if (field_id == ts_node_field_id_for_child(tsnode, i)) {
        return child(i);
      }
    }
  }

  Node next() const {
    TSNode s = ts_node_next_sibling(tsnode);
    return {
      s,
      depth,
      index + 1,
      ts_node_start_byte(s),
      ts_node_end_byte(s),
      -1 //ts_node_field_id_for_child(tsnode, i)
    };
  }

  Node get_type()       const { return child_by_field_id(field_type); }
  Node get_declarator() const { return child_by_field_id(field_declarator); }

  std::string body(const char* source) const {
    assert(!is_null());
    return std::string(&source[span_a], &source[span_b]);
  }

  bool operator==(Node b) const { return ts_node_eq(tsnode, b.tsnode); }

  int         line()       const { return ts_node_start_point(tsnode).row; }
  const char* type()       const { return ts_node_type(tsnode); }
  int         symbol()     const { return is_null() ? -1 : ts_node_symbol(tsnode); }


  bool is_null()       const { return ts_node_is_null(tsnode); }
  bool is_named()      const { return !is_null() && ts_node_is_named(tsnode); }
  bool is_missing()    const { return !is_null() && ts_node_is_missing(tsnode); }
  bool is_extra()      const { return !is_null() && ts_node_is_extra(tsnode); }
  bool is_leaf()       const { return !is_null() && is_named() && !child_count(); }
  bool is_branch()     const { return !is_null() && is_named() && child_count(); }

  bool is_identifier() const { return !is_null() && symbol() == sym_identifier; }
  bool is_comment()    const { return !is_null() && symbol() == sym_comment; }
  bool is_decl()       const { return !is_null() && symbol() == sym_declaration; }
  bool is_field_decl() const { return !is_null() && symbol() == sym_field_declaration; }
  bool is_func_decl()  const { return !is_null() && symbol() == sym_function_declarator; }
  bool is_expression() const { return !is_null() && symbol() == sym_expression_statement; }
  bool is_call()       const { return !is_null() && symbol() == sym_call_expression; }
  bool is_function()   const { return !is_null() && symbol() == sym_function_definition; }
  bool is_assignment() const { return !is_null() && symbol() == sym_assignment_expression; }
  bool is_field_id()   const { return !is_null() && symbol() == alias_sym_field_identifier; }
  bool is_field_expr() const { return !is_null() && symbol() == sym_field_expression; }
  bool is_call_expr()  const { return !is_null() && symbol() == sym_call_expression; }
  bool is_arglist()    const { return !is_null() && symbol() == sym_argument_list; }

  Node get_arglist_field() const { return child_by_field_id(field_arguments); }
  Node get_type_field()    const { return child_by_field_id(field_type); }
  Node get_decl_field()    const { return child_by_field_id(field_declarator); }

  TSNode tsnode = { 0 };
  int depth = 0;
  int index = 0;
  uint32_t span_a = 0;
  uint32_t span_b = 0;
  int field_id = -1;
};

//------------------------------------------------------------------------------
