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

  void error() {
    dump_tree(0, 0, 255);
    debugbreak();
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

  /*
  MtNode child_by_sym(TSSymbol child_sym) {
    for (auto c : *this) {
      if (c.sym == child_sym) return c;
    }
    return MtNode::null;
  }
  */

  bool is_static() const;
  bool is_const() const;

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

struct MtIdentifier        : public MtNode { MtIdentifier        () {}; MtIdentifier        (const MtNode& n) : MtNode(n) { assert(sym == sym_identifier); } };
struct MtLiteral           : public MtNode { MtLiteral           () {}; MtLiteral           (const MtNode& n) : MtNode(n) { assert(!is_named());} };

//------------------------------------------------------------------------------

struct MtAssignmentExpr : public MtNode {
  MtAssignmentExpr() {};
  MtAssignmentExpr(const MtNode& n) : MtNode(n) {
    assert(sym == sym_assignment_expression);
  }
  MtIdentifier lhs() { return MtIdentifier(get_field(field_left)); }
  MtLiteral    op()  { return MtLiteral(get_field(field_operator)); }
  MtNode       rhs() { return get_field(field_right); }
};

//------------------------------------------------------------------------------

struct MtTypeIdentifier : public MtNode {
  MtTypeIdentifier() {};
  MtTypeIdentifier(const MtNode& n) : MtNode(n) {
    assert(sym == alias_sym_type_identifier);
  }
};

//------------------------------------------------------------------------------

struct MtFieldDeclList : public MtNode {
  MtFieldDeclList() {};
  MtFieldDeclList(const MtNode& n) : MtNode(n) {
    assert(sym == sym_field_declaration_list);
  }
};

//------------------------------------------------------------------------------

struct MtStructSpecifier : public MtNode {
  MtStructSpecifier() {};
  MtStructSpecifier(const MtNode& n) : MtNode(n) {
    assert(sym == sym_struct_specifier);
  }
  MtTypeIdentifier name() { return MtTypeIdentifier(get_field(field_name)); }
  MtFieldDeclList  body() { return MtFieldDeclList(get_field(field_body)); }
};

//------------------------------------------------------------------------------

struct MtPrimitiveType     : public MtNode {
  MtPrimitiveType() {};
  MtPrimitiveType(const MtNode& n) : MtNode(n) {
    assert(sym == sym_primitive_type);
  }
};

//------------------------------------------------------------------------------

struct MtFuncDefinition : public MtNode {
  MtFuncDefinition() {};
  MtFuncDefinition(const MtNode& n) : MtNode(n) {
    assert(sym == sym_function_definition);
  }
  
  MtPrimitiveType type() { return MtPrimitiveType(get_field(field_type)); }
};

//------------------------------------------------------------------------------

struct MtFieldDecl         : public MtNode { /*MtFieldDecl         () {}; MtFieldDecl         (const MtNode& n) : MtNode(n) { assert(sym == sym_field_declaration); }      */ };
struct MtNumberLiteral     : public MtNode { /*MtNumberLiteral     () {}; MtNumberLiteral     (const MtNode& n) : MtNode(n) { assert(sym == sym_number_literal); }         */ };
struct MtFieldExpression   : public MtNode { /*MtFieldExpression   () {}; MtFieldExpression   (const MtNode& n) : MtNode(n) { assert(sym == sym_field_expression); }       */ };
struct MtReturnStatement   : public MtNode { /*MtReturnStatement   () {}; MtReturnStatement   (const MtNode& n) : MtNode(n) { assert(sym == sym_return_statement); }       */ };
struct MtTemplateDecl      : public MtNode { /*MtTemplateDecl      () {}; MtTemplateDecl      (const MtNode& n) : MtNode(n) { assert(sym == sym_template_declaration); }   */ };
struct MtPreprocInclude    : public MtNode { /*MtPreprocInclude    () {}; MtPreprocInclude    (const MtNode& n) : MtNode(n) { assert(sym == sym_preproc_include); }        */ };
struct MtCompoundStatement : public MtNode { /*MtCompoundStatement () {}; MtCompoundStatement (const MtNode& n) : MtNode(n) { assert(sym == sym_compound_statement); }     */ };
struct MtTemplateType      : public MtNode { /*MtTemplateType      () {}; MtTemplateType      (const MtNode& n) : MtNode(n) { assert(sym == sym_template_type); }          */ };
struct MtTranslationUnit   : public MtNode { /*MtTranslationUnit   () {}; MtTranslationUnit   (const MtNode& n) : MtNode(n) { assert(sym == sym_translation_unit); }       */ };
struct MtCallExpression    : public MtNode { /*MtCallExpression    () {}; MtCallExpression    (const MtNode& n) : MtNode(n) { assert(sym == sym_call_expression); }        */ };
struct MtTemplateParamList : public MtNode { /*MtTemplateParamList () {}; MtTemplateParamList (const MtNode& n) : MtNode(n) { assert(sym == sym_template_parameter_list); }*/ };
struct MtTemplateArgList   : public MtNode { /*MtTemplateArgList   () {}; MtTemplateArgList   (const MtNode& n) : MtNode(n) { assert(sym == sym_template_argument_list); } */ };
struct MtEnumeratorList    : public MtNode { /*MtEnumeratorList    () {}; MtEnumeratorList    (const MtNode& n) : MtNode(n) { assert(sym == sym_enumerator_list); }        */ };
struct MtDeclaration       : public MtNode { /*MtDeclaration       () {}; MtDeclaration       (const MtNode& n) : MtNode(n) { assert(sym == sym_declaration); }            */ };
struct MtComment           : public MtNode { /*MtComment           () {}; MtComment           (const MtNode& n) : MtNode(n) { assert(sym == sym_comment); }                */ };
struct MtCaseStatement     : public MtNode { /*MtCaseStatement     () {}; MtCaseStatement     (const MtNode& n) : MtNode(n) { assert(sym == sym_case_statement); }         */ };
struct MtSwitchStatement   : public MtNode { /*MtSwitchStatement   () {}; MtSwitchStatement   (const MtNode& n) : MtNode(n) { assert(sym == sym_switch_statement); }       */ };
struct MtUsingDecl         : public MtNode { /*MtUsingDecl         () {}; MtUsingDecl         (const MtNode& n) : MtNode(n) { assert(sym == sym_using_declaration); }      */ };
struct MtBreakStatement    : public MtNode { /*MtBreakStatement    () {}; MtBreakStatement    (const MtNode& n) : MtNode(n) { assert(sym == sym_break_statement); }        */ };
struct MtCondExpression    : public MtNode { /*MtCondExpression    () {}; MtCondExpression    (const MtNode& n) : MtNode(n) { assert(sym == sym_conditional_expression); } */ };
struct MtStorageSpec       : public MtNode { /*MtStorageSpec       () {}; MtStorageSpec       (const MtNode& n) : MtNode(n) { assert(sym == sym_storage_class_specifier); }*/ };
struct MtQualifiedId       : public MtNode { /*MtQualifiedId       () {}; MtQualifiedId       (const MtNode& n) : MtNode(n) { assert(sym == sym_qualified_identifier); }   */ };
struct MtIfStatement       : public MtNode { /*MtIfStatement       () {}; MtIfStatement       (const MtNode& n) : MtNode(n) { assert(sym == sym_if_statement); }           */ };
struct MtEnumSpecifier     : public MtNode { /*MtEnumSpecifier     () {}; MtEnumSpecifier     (const MtNode& n) : MtNode(n) { assert(sym == sym_enum_specifier); }         */ };
struct MtSizedTypeSpec     : public MtNode { /*MtSizedTypeSpec     () {}; MtSizedTypeSpec     (const MtNode& n) : MtNode(n) { assert(sym == sym_sized_type_specifier); }   */ };
struct MtNamespaceDef      : public MtNode { /*MtNamespaceDef      () {}; MtNamespaceDef      (const MtNode& n) : MtNode(n) { assert(sym == sym_namespace_definition); }   */ };

//------------------------------------------------------------------------------

struct MtConstIterator {

  MtConstIterator(MtNode parent) {
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

  ~MtConstIterator() {
    if (cursor.tree) {
      ts_tree_cursor_delete(&cursor);
    }
  }

  MtConstIterator& operator++() {
    if (!ts_tree_cursor_goto_next_sibling(&cursor)) {
      ts_tree_cursor_delete(&cursor);
      cursor = { 0 };
    }
    return *this;
  }

  //std::strong_ordering operator<=>(const MtIterator& b) const { return cursor <=> b.cursor; }
  //bool operator != (const MtIterator& b) const { return (*this <=> b) != std::strong_ordering::equal; }
  bool operator <  (const MtConstIterator& b) const { return cursor < b.cursor; }
  bool operator != (const MtConstIterator& b) const { return cursor != b.cursor; }

  const MtNode operator*() const {
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

inline MtConstIterator begin(const MtNode& parent) {
  return MtConstIterator(parent);
}

inline MtConstIterator end(const MtNode& parent) {
  return MtConstIterator(MtNode::null);
}

inline MtIterator begin(MtNode& parent) {
  return MtIterator(parent);
}

inline MtIterator end(MtNode& parent) {
  return MtIterator(MtNode::null);
}

//------------------------------------------------------------------------------
