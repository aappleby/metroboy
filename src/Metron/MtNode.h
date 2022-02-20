#pragma once
#include "Platform.h"


//------------------------------------------------------------------------------

struct MtNode {

  MtNode() {
    this->node = { 0 };
    this->sym = 0;
    this->field = 0;
    this->mod = nullptr;
  }

  MtNode(TSNode node, int sym, int field, MtModule* mod) {
    this->node = node;
    this->sym = sym;
    this->field = field;
    this->mod = mod;
  }

  static MtNode from_mod(MtModule* mod);

  //----------

  void dump_node(int index, int depth);
  void dump_tree(int index, int depth, int maxdepth);
  void dump_tree() {
    dump_tree(0, 0, 255);
  }

  operator bool() const { return !ts_node_is_null(node); }
  //std::strong_ordering operator<=>(const MtNode& b) const { return node <=> b.node; }

  bool operator<(const MtNode& b) const {
    return node < b.node;
  }

  const char* type() const { return ts_node_type(node); }
  uint32_t start_byte() const { return ts_node_start_byte(node); }
  uint32_t end_byte()   const { return ts_node_end_byte(node); }

  MtNode& check_null() {
    if (is_null()) debugbreak();
    return *this;
  }

  //----------

  int child_count() const { return (int)ts_node_child_count(node); }
  int named_child_count() const { return (int)ts_node_named_child_count(node); }

  MtNode child(int i) const;
  MtNode named_child(int i) const;
  MtNode first_named_child() const;

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

  MtNode get_field(int field_id);

  //----------

  std::string body();
  const char* start();
  const char* end();
  bool match(const char* s);

  std::string node_to_name();
  std::string node_to_type();

  typedef std::function<void(MtNode)> NodeVisitor;
  typedef std::function<void(MtNode parent, MtNode child)> NodeVisitor2;

  void visit_tree(NodeVisitor cv);
  void visit_tree2(NodeVisitor2 cv);

  //----------

  TSNode node;
  TSSymbol sym;
  int field;
  MtModule* mod;

  static const MtNode null;
};

//------------------------------------------------------------------------------

struct MtIterator {

  MtIterator(MtNode parent) {
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

  //std::strong_ordering operator<=>(const MtIterator& b) const { return cursor <=> b.cursor; }
  //bool operator != (const MtIterator& b) const { return (*this <=> b) != std::strong_ordering::equal; }
  bool operator <  (const MtIterator& b) const { return cursor < b.cursor; }
  bool operator != (const MtIterator& b) const { return cursor != b.cursor; }

  MtNode operator*() const {
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

inline MtIterator begin(MtNode parent) {
  return MtIterator(parent);
}

inline MtIterator end(MtNode parent) {
  return MtIterator(MtNode::null);
}

//------------------------------------------------------------------------------
