#pragma once

#include "tree-sitter/lib/include/tree_sitter/api.h"

#include <deque>
#include <string>
#include <span>

#include "metrolib/core/Check.h"
#include "metrolib/core/Blobs.h"
#include "TreeSymbols.h"

typedef std::span<const char> cspan;

//-----------------------------------------------------------------------------

/*
uint32_t ts_node_start_byte(TSNode);
TSPoint ts_node_start_point(TSNode);
uint32_t ts_node_end_byte(TSNode);
TSPoint ts_node_end_point(TSNode);

char *ts_node_string(TSNode);

TSNode ts_node_named_child(TSNode, uint32_t);
uint32_t ts_node_named_child_count(TSNode);

TSNode ts_node_child_by_field_name(TSNode self, const char *field_name, uint32_t
field_name_length); TSNode ts_node_child_by_field_id(TSNode, TSFieldId);

TSNode ts_node_first_child_for_byte(TSNode, uint32_t);
TSNode ts_node_first_named_child_for_byte(TSNode, uint32_t);

TSNode ts_node_descendant_for_byte_range(TSNode, uint32_t, uint32_t);
TSNode ts_node_descendant_for_point_range(TSNode, TSPoint, TSPoint);
TSNode ts_node_named_descendant_for_byte_range(TSNode, uint32_t, uint32_t);
TSNode ts_node_named_descendant_for_point_range(TSNode, TSPoint, TSPoint);
*/

//-----------------------------------------------------------------------------

struct PNode : public TSNode {
  PNode(const TSNode& b) : TSNode(b) {}

  bool operator==(PNode b) const { return ts_node_eq(*this, b); }

  const char* type() const { return ts_node_type(*this); }
  TSSymbol symbol() const { return ts_node_symbol(*this); }

  bool is_null() const { return ts_node_is_null(*this); }
  bool is_named() const { return ts_node_is_named(*this); }
  bool is_missing() const { return ts_node_is_missing(*this); }
  bool is_extra() const { return ts_node_is_extra(*this); }

  bool is_identifier() const { return symbol() == sym_identifier; }
  bool is_comment() const { return symbol() == sym_comment; }
  bool is_decl() const { return symbol() == sym_declaration; }
  bool is_field_decl() const { return symbol() == sym_field_declaration; }
  bool is_func_decl() const { return symbol() == sym_function_declarator; }
  bool is_expression() const { return symbol() == sym_expression_statement; }
  bool is_call() const { return symbol() == sym_call_expression; }
  bool is_function() const { return symbol() == sym_function_definition; }
  bool is_assignment() const { return symbol() == sym_assignment_expression; }
  bool is_field_id() const { return symbol() == alias_sym_field_identifier; }
  bool is_field_expr() const { return symbol() == sym_field_expression; }
  bool is_call_expr() const { return symbol() == sym_call_expression; }
  bool is_arglist() const { return symbol() == sym_argument_list; }
  bool is_return() const { return symbol() == sym_return_statement; }
  bool is_preproc_if() const { return symbol() == sym_preproc_if; }

  int line() const { return ts_node_start_point(*this).row; }

  PNode arglist() const {
    auto r = get_field(field_arguments);
    CHECK_P(r.is_arglist());
    return r;
  }

  PNode parent() const { return ts_node_parent(*this); }
  int child_count() const { return (int)ts_node_child_count(*this); }
  PNode child(int i) const { return ts_node_child(*this, i); }
  const char* field(int i) const {
    return ts_node_field_name_for_child(*this, i);
  }

  int named_child_count() const {
    return (int)ts_node_named_child_count(*this);
  }
  PNode named_child(int i) const { return ts_node_named_child(*this, i); }

  PNode prev() const { return ts_node_prev_sibling(*this); }
  PNode next() const { return ts_node_next_sibling(*this); }

  PNode prev_named() const { return ts_node_prev_named_sibling(*this); }
  PNode next_named() const { return ts_node_next_named_sibling(*this); }

  PNode get_field(TSFieldId field_id) const {
    return ts_node_child_by_field_id(*this, field_id);
  }

  PNode get_type_field() const { return get_field(field_type); }
  PNode get_declarator_field() const { return get_field(field_declarator); }

  void enqueue_children(std::deque<PNode>& queue) {
    for (auto i = 0; i < child_count(); i++) {
      queue.push_back(child(i));
    }
  }

  cspan span(const char* src) const {
    const char* a = &src[ts_node_start_byte(*this)];
    const char* b = &src[ts_node_end_byte(*this)];

    while (isspace(a[0]) || a[0] == '\r') a++;
    while (isspace(b[-1]) || b[-1] == '\r') b--;
    return cspan(a, b);
  }

  std::string body(const char* src) const {
    CHECK_N(is_null());
    auto s = span(src);
    return std::string(s.begin(), s.end());
  }

  //----------

  void print_source(const char* source, int max_len) {
    max_len = 999999999;
    const char* old_a = &source[ts_node_start_byte(*this)];
    const char* old_b = &source[ts_node_end_byte(*this)];
    const char* a = old_a;
    const char* b = old_b;

    while (isspace(a[0]) || a[0] == '\r') a++;
    while (isspace(b[-1]) || b[-1] == '\r') b--;
    auto len = (b - a) > max_len ? max_len : b - a;
    if (len > 0) {
      fwrite(a, len, 1, stdout);
    } else {
      fwrite("???", 3, 1, stdout);
    }
  }

  void print(const char* source, const char* field_name = nullptr) {
    printf("%04d %-20s %-20s | ", symbol(), field_name, type());
    print_source(source, 40);
    printf("\n");
  }

  void print(const char* source, int depth, const char* field_name = nullptr) {
    printf("%04d: ", symbol());
    for (int i = 0; i < depth; i++) printf("|  ");
    if (field_name) {
      printf("%s: ", field_name);
    }

    if (is_named()) {
      printf("%s ", type());
    } else {
      printf("lit ");
    }
    printf("'");
    print_source(source, 40);
    printf("'");
    printf("\n");
  }

  void dump_children(const char* source) {
    for (auto i = 0; i < child_count(); i++) {
      child(i).print(source, 0, nullptr);
    }
  }

  void dump(const char* source, int depth = 0,
            const char* field_name = nullptr) {
    print(source, depth, field_name);
    for (auto i = 0; i < child_count(); i++) {
      child(i).dump(source, depth + 1, field(i));
    }
  }

  bool is_wire_decl() const {
    if (symbol() != sym_comment) return false;
    // check for tag pattern
    if (next().is_null()) return false;
    if (next().symbol() != sym_declaration) return false;
    if (line() != next().line()) return false;
    return true;
  }
};

//-----------------------------------------------------------------------------

struct PTree {
  PTree(const char* filename);
  ~PTree();

  PNode root() const { return ts_tree_root_node(tree); }

  int count_nodes() const;
  int count_comments() const;

  const char* source() const { return (const char*)src_blob.data(); }

  TSTree* tree;
  blob src_blob;
};

//-----------------------------------------------------------------------------
