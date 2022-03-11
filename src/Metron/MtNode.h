#pragma once
#include <assert.h>

#include <functional>
#include <string>

#include "Platform.h"
#include "TreeSymbols.h"
#include "tree_sitter/api.h"

bool operator<(const TSNode& a, const TSNode& b);
bool operator==(const TSNode& a, const TSNode& b);
bool operator!=(const TSNode& a, const TSNode& b);
bool operator<(const TSTreeCursor& a, const TSTreeCursor& b);
bool operator==(const TSTreeCursor& a, const TSTreeCursor& b);
bool operator!=(const TSTreeCursor& a, const TSTreeCursor& b);

struct MtModule;
struct MtSourceFile;

//------------------------------------------------------------------------------

struct MtNode {
  MtNode();
  MtNode(TSNode node, int sym, int field, MtSourceFile* source);

  //----------

  void dump_node(int index, int depth) const;
  void dump_tree(int index, int depth, int maxdepth) const;
  void dump_tree() const { dump_tree(0, 0, 255); }
  void error() const {
    dump_tree(0, 0, 255);
    debugbreak();
  }

  operator bool() const { return !ts_node_is_null(node); }

  MtNode& check_null() {
    if (is_null()) debugbreak();
    return *this;
  }

  const char* type() const { return ts_node_type(node); }
  uint32_t start_byte() const { return ts_node_start_byte(node); }
  uint32_t end_byte() const { return ts_node_end_byte(node); }

  const char* start();
  const char* end();
  std::string text();
  bool match(const char* s);

  //----------

  int child_count() const { return (int)ts_node_child_count(node); }
  int named_child_count() const { return (int)ts_node_named_child_count(node); }

  MtNode child(int i) const;
  MtNode named_child(int i) const;
  MtNode first_named_child() const;

  bool is_static() const;
  bool is_const() const;

  bool is_null() const { return ts_node_is_null(node); }
  bool is_named() const { return !is_null() && ts_node_is_named(node); }

  //----------

  MtNode get_field(int field_id);

  //----------

  std::string node_to_name();
  std::string node_to_type();

  typedef std::function<void(MtNode)> NodeVisitor;

  void visit_tree(NodeVisitor cv);

  void check_sym(TSSymbol sym) {
    if (!is_null() && this->sym != sym) {
      dump_tree();
    }
    assert(is_null() || this->sym == sym);
  }

  //----------

  TSNode node;
  TSSymbol sym;
  int field;
  MtSourceFile* source;

  static const MtNode null;
};

//------------------------------------------------------------------------------

struct MtConstIterator {
  MtConstIterator(MtNode parent) {
    if (ts_node_is_null(parent.node)) {
      cursor = {0};
    } else {
      cursor = ts_tree_cursor_new(parent.node);

      if (!ts_tree_cursor_goto_first_child(&cursor)) {
        ts_tree_cursor_delete(&cursor);
        cursor = {0};
      }
    }
    this->source = parent.source;
  }

  ~MtConstIterator() {
    if (cursor.tree) {
      ts_tree_cursor_delete(&cursor);
    }
  }

  MtConstIterator& operator++() {
    if (!ts_tree_cursor_goto_next_sibling(&cursor)) {
      ts_tree_cursor_delete(&cursor);
      cursor = {0};
    }
    return *this;
  }

  bool operator<(const MtConstIterator& b) const { return cursor < b.cursor; }
  bool operator!=(const MtConstIterator& b) const { return cursor != b.cursor; }

  const MtNode operator*() const {
    auto child = ts_tree_cursor_current_node(&cursor);
    auto sym = ts_node_symbol(child);
    auto field = ts_tree_cursor_current_field_id(&cursor);

    return {child, sym, field, source};
  }

  TSTreeCursor cursor;
  MtSourceFile* source;
};

//------------------------------------------------------------------------------

struct MtIterator {
  MtIterator(MtNode parent) {
    if (ts_node_is_null(parent.node)) {
      cursor = {0};
    } else {
      cursor = ts_tree_cursor_new(parent.node);

      if (!ts_tree_cursor_goto_first_child(&cursor)) {
        ts_tree_cursor_delete(&cursor);
        cursor = {0};
      }
    }
    this->source = parent.source;
  }

  ~MtIterator() {
    if (cursor.tree) {
      ts_tree_cursor_delete(&cursor);
    }
  }

  MtIterator& operator++() {
    if (!ts_tree_cursor_goto_next_sibling(&cursor)) {
      ts_tree_cursor_delete(&cursor);
      cursor = {0};
    }
    return *this;
  }

  bool operator<(const MtIterator& b) const { return cursor < b.cursor; }
  bool operator!=(const MtIterator& b) const { return cursor != b.cursor; }

  MtNode operator*() const {
    auto child = ts_tree_cursor_current_node(&cursor);
    auto sym = ts_node_symbol(child);
    auto field = ts_tree_cursor_current_field_id(&cursor);

    return {child, sym, field, source};
  }

  TSTreeCursor cursor;
  MtSourceFile* source;
};

//------------------------------------------------------------------------------

inline MtConstIterator begin(const MtNode& parent) {
  return MtConstIterator(parent);
}

inline MtConstIterator end(const MtNode& parent) {
  return MtConstIterator(MtNode::null);
}

inline MtIterator begin(MtNode& parent) { return MtIterator(parent); }

inline MtIterator end(MtNode& parent) { return MtIterator(MtNode::null); }

//------------------------------------------------------------------------------

struct MtIdentifier : public MtNode {
  MtIdentifier(){};
  MtIdentifier(const MtNode& n) : MtNode(n) { check_sym(sym_identifier); }
};

//------------------------------------------------------------------------------

struct MtLiteral : public MtNode {
  MtLiteral(){};
  MtLiteral(const MtNode& n) : MtNode(n) { assert(!is_named()); }
};

//------------------------------------------------------------------------------

struct MtAssignmentExpr : public MtNode {
  MtAssignmentExpr(){};
  MtAssignmentExpr(const MtNode& n) : MtNode(n) {
    check_sym(sym_assignment_expression);
  }
  MtNode lhs() { return get_field(field_left); }
  MtLiteral op() { return MtLiteral(get_field(field_operator)); }
  MtNode rhs() { return get_field(field_right); }
};

//------------------------------------------------------------------------------

struct MtTypeIdentifier : public MtNode {
  MtTypeIdentifier(){};
  MtTypeIdentifier(const MtNode& n) : MtNode(n) {
    check_sym(alias_sym_type_identifier);
  }
};

//------------------------------------------------------------------------------

struct MtFieldDeclList : public MtNode {
  MtFieldDeclList(){};
  MtFieldDeclList(const MtNode& n) : MtNode(n) {
    check_sym(sym_field_declaration_list);
  }
};

//------------------------------------------------------------------------------

struct MtClassSpecifier : public MtNode {
  MtClassSpecifier(){};
  MtClassSpecifier(const MtNode& n) : MtNode(n) {
    check_sym(sym_class_specifier);
  }
  MtTypeIdentifier name() { return MtTypeIdentifier(get_field(field_name)); }
  MtFieldDeclList body() { return MtFieldDeclList(get_field(field_body)); }
};

//------------------------------------------------------------------------------

struct MtPrimitiveType : public MtNode {
  MtPrimitiveType(){};
  MtPrimitiveType(const MtNode& n) : MtNode(n) {
    check_sym(sym_primitive_type);
  }
};

//------------------------------------------------------------------------------

struct MtExprStatement : public MtNode {
  MtExprStatement(){};
  MtExprStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_expression_statement);
  }
};

//------------------------------------------------------------------------------

struct MtFieldName : public MtNode {
  MtFieldName(){};
  MtFieldName(const MtNode& n) : MtNode(n) {
    check_sym(alias_sym_field_identifier);
  }
};

//------------------------------------------------------------------------------

struct MtParameterList : public MtNode {
  MtParameterList(){};
  MtParameterList(const MtNode& n) : MtNode(n) {
    check_sym(sym_parameter_list);
  }
};

//------------------------------------------------------------------------------

struct MtFuncDeclarator : public MtNode {
  MtFuncDeclarator(){};
  MtFuncDeclarator(const MtNode& n) : MtNode(n) {
    check_sym(sym_function_declarator);
  }

  MtFieldName decl() { return MtFieldName(get_field(field_declarator)); }
  MtParameterList params() {
    return MtParameterList(get_field(field_parameters));
  }
};

//------------------------------------------------------------------------------

struct MtCompoundStatement : public MtNode {
  MtCompoundStatement(){};
  MtCompoundStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_compound_statement);
  }
};

//------------------------------------------------------------------------------

struct MtFuncDefinition : public MtNode {
  MtFuncDefinition(){};
  MtFuncDefinition(const MtNode& n) : MtNode(n) {
    check_sym(sym_function_definition);
  }

  MtPrimitiveType type() { return MtPrimitiveType(get_field(field_type)); }
  MtFuncDeclarator decl() {
    return MtFuncDeclarator(get_field(field_declarator));
  }
  MtCompoundStatement body() {
    return MtCompoundStatement(get_field(field_body));
  }
};

//------------------------------------------------------------------------------

struct MtArgList : public MtNode {
  MtArgList(){};
  MtArgList(const MtNode& n) : MtNode(n) { check_sym(sym_argument_list); }
};

//------------------------------------------------------------------------------

struct MtFieldExpr : public MtNode {
  MtFieldExpr(){};
  MtFieldExpr(const MtNode& n) : MtNode(n) { check_sym(sym_field_expression); }
};

//------------------------------------------------------------------------------

struct MtFieldIdentifier : public MtNode {
  MtFieldIdentifier(){};
  MtFieldIdentifier(const MtNode& n) : MtNode(n) {
    check_sym(alias_sym_field_identifier);
  }
};

//------------------------------------------------------------------------------

struct MtTemplateArgList : public MtNode {
  MtTemplateArgList(){};
  MtTemplateArgList(const MtNode& n) : MtNode(n) {
    check_sym(sym_template_argument_list);
  }
};

//------------------------------------------------------------------------------

struct MtTemplateFunc : public MtNode {
  MtTemplateFunc() {}
  MtTemplateFunc(const MtNode& n) : MtNode(n) {
    check_sym(sym_template_function);
  }

  MtTemplateArgList args() { return get_field(field_arguments); }
};

//------------------------------------------------------------------------------
// MtIdentifier || MtTemplateFunc

// primitive type because int(x) style casts

struct MtFunc : public MtNode {
  MtFunc() {}
  MtFunc(const MtNode& n) : MtNode(n) {
    if (is_null() || is_id() || is_templ() || is_field() || is_prim()) {
    } else {
      error();
    }
  }

  std::string name() {
    if (sym == sym_field_expression) {
      return as_field().node_to_name();
    } else {
      return node_to_name();
    }
  }

  bool is_id() { return sym == sym_identifier; }
  bool is_templ() { return sym == sym_template_function; }
  bool is_field() { return sym == sym_field_expression; }
  bool is_prim() { return sym == sym_primitive_type; }

  MtIdentifier as_id() { return MtIdentifier(*this); }
  MtTemplateFunc as_templ() { return MtTemplateFunc(*this); }
  MtFieldExpr as_field() { return MtFieldExpr(*this); }
};

//------------------------------------------------------------------------------

struct MtCallExpr : public MtNode {
  MtCallExpr(){};
  MtCallExpr(const MtNode& n) : MtNode(n) { check_sym(sym_call_expression); }

  // identifier or field expression or template function?
  MtFunc func() { return MtFunc(get_field(field_function)); }
  MtArgList args() { return MtArgList(get_field(field_arguments)); }
};

//------------------------------------------------------------------------------

struct MtInitDecl : public MtNode {
  MtInitDecl(){};
  MtInitDecl(const MtNode& n) : MtNode(n) { check_sym(sym_init_declarator); }
  MtIdentifier decl() { return MtIdentifier(get_field(field_declarator)); }
};

//------------------------------------------------------------------------------

struct MtTemplateType : public MtNode {
  MtTemplateType(){};
  MtTemplateType(const MtNode& n) : MtNode(n) { check_sym(sym_template_type); }

  MtTypeIdentifier name() { return MtTypeIdentifier(get_field(field_name)); }
  MtTemplateArgList args() {
    return MtTemplateArgList(get_field(field_arguments));
  }
};

//------------------------------------------------------------------------------

/*
========== tree dump begin
[00:000:187] declaration =
[00:032:321] |  type: template_type =
[00:022:395] |  |  name: type_identifier = "logic"
[01:003:324] |  |  arguments: template_argument_list =
[00:000:036] |  |  |  lit = "<"
[01:000:112] |  |  |  number_literal = "5"
[02:000:033] |  |  |  lit = ">"
[01:009:001] |  declarator: identifier = "default_funct"
[02:000:039] |  lit = ";"
========== tree dump end

========== tree dump begin
[00:000:187] declaration =
[00:032:321] |  type: template_type =
[00:022:395] |  |  name: type_identifier = "logic"
[01:003:324] |  |  arguments: template_argument_list =
[00:000:036] |  |  |  lit = "<"
[01:000:112] |  |  |  number_literal = "5"
[02:000:033] |  |  |  lit = ">"
[01:009:224] |  declarator: init_declarator =
[00:009:001] |  |  declarator: identifier = "default_funct"
[01:000:063] |  |  lit = "="
[02:034:112] |  |  value: number_literal = "22"
[02:000:039] |  lit = ";"
========== tree dump end
*/

struct MtDecl : public MtNode {
  MtDecl(){};
  MtDecl(const MtNode& n) : MtNode(n) { check_sym(sym_declaration); }

  bool is_init_decl() {
    assert(!is_null());
    return get_field(field_declarator).sym == sym_init_declarator;
  }

  bool is_static2() {
    for (auto c : (MtNode&)*this) {
      if (c.sym == sym_storage_class_specifier) {
        if (c.match("static")) return true;
      }
    }
    return false;
  }

  bool is_const2() {
    for (auto c : (MtNode&)*this) {
      if (c.sym == sym_type_qualifier) {
        if (c.match("const")) return true;
      }
    }
    return false;
  }

  MtTemplateType _type() { return MtTemplateType(get_field(field_type)); }
  MtIdentifier _decl() { return MtIdentifier(get_field(field_declarator)); }
  MtInitDecl _init_decl() { return MtInitDecl(get_field(field_declarator)); }
};

//------------------------------------------------------------------------------

struct MtEnumSpecifier : public MtNode {
  MtEnumSpecifier(){};
  MtEnumSpecifier(const MtNode& n) : MtNode(n) {
    check_sym(sym_enum_specifier);
  }
};

//------------------------------------------------------------------------------
// MtIdentifier || MtTemplateType || MtEnumSpecifier || MtPrimitiveType

struct MtType : public MtNode {
  MtType() {}
  MtType(const MtNode& n) : MtNode(n) {
    assert(is_id() || is_templ() || is_enum() || is_prim());
  }

  bool is_id() { return sym == alias_sym_type_identifier; }
  bool is_templ() { return sym == sym_template_type; }
  bool is_enum() { return sym == sym_enum_specifier; }
  bool is_prim() { return sym == sym_primitive_type; }

  MtIdentifier as_id() { return MtIdentifier(*this); }
  MtTemplateType as_templ() { return MtTemplateType(*this); }
  MtEnumSpecifier as_enum() { return MtEnumSpecifier(*this); }
  MtPrimitiveType as_prim() { return MtPrimitiveType(*this); }
};

//------------------------------------------------------------------------------

struct MtFieldDecl : public MtNode {
  MtFieldDecl(){};
  MtFieldDecl(const MtNode& n) : MtNode(n) { check_sym(sym_field_declaration); }

  bool is_primitive() {
    // Primitive types are primitive types.
    if (type().sym == sym_primitive_type) return true;

    // Logic arrays are primitive types.
    if (type().sym == sym_template_type) {
      auto templ_name = type().get_field(field_name);
      if (templ_name.match("logic")) return true;
    }

    // Bits are primitive types.
    if (type().match("bit")) return true;

    return false;
  }

  bool is_enum() { return type().sym == sym_enum_specifier; }

  bool is_static() {
    MtNode n = name();
    for (auto c : n) {
      if (c.sym == sym_storage_class_specifier) {
        if (c.match("static")) return true;
      }
    }
    return false;
  }

  bool is_const() {
    for (auto c : (MtNode)name()) {
      if (c.sym == sym_type_qualifier) {
        if (c.match("const")) return true;
      }
    }
    return false;
  }

  bool is_static2() {
    for (auto c : (MtNode&)*this) {
      if (c.sym == sym_storage_class_specifier) {
        if (c.match("static")) return true;
      }
    }
    return false;
  }

  bool is_const2() {
    for (auto c : (MtNode&)*this) {
      if (c.sym == sym_type_qualifier) {
        if (c.match("const")) return true;
      }
    }
    return false;
  }

  bool is_param() { return is_static() && is_const(); }

  bool is_input() {
    if (is_static() || is_const() || is_enum()) return false;

    auto base_name = name().sym == sym_array_declarator
                         ? name().get_field(field_declarator).text()
                         : name().text();
    return base_name.starts_with("i_") || base_name.ends_with("_i");
  }

  bool is_output() {
    if (is_static() || is_const() || is_enum()) return false;
    auto base_name = name().sym == sym_array_declarator
                         ? name().get_field(field_declarator).text()
                         : name().text();
    return base_name.starts_with("o_") || base_name.ends_with("_o");
  }

  MtType type() { return MtType(get_field(field_type)); }
  MtFieldName name() {
    auto decl = get_field(field_declarator);
    if (decl.sym == sym_array_declarator) {
      decl = decl.get_field(field_declarator);
    }
    return MtFieldName(decl);
  }
  MtCallExpr value() { return MtCallExpr(get_field(field_default_value)); }
};

//------------------------------------------------------------------------------

struct MtPreprocInclude : public MtNode {
  MtPreprocInclude(){};
  MtPreprocInclude(const MtNode& n) : MtNode(n) {
    check_sym(sym_preproc_include);
  }

  MtNode path_node() { return get_field(field_path); }

  std::string path() { return get_field(field_path).text(); }
};

//------------------------------------------------------------------------------

struct MtNumberLiteral : public MtNode {
  MtNumberLiteral(){};
  MtNumberLiteral(const MtNode& n) : MtNode(n) {
    check_sym(sym_number_literal);
  }
};
struct MtReturnStatement : public MtNode {
  MtReturnStatement(){};
  MtReturnStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_return_statement);
  }
};
struct MtTemplateDecl : public MtNode {
  MtTemplateDecl(){};
  MtTemplateDecl(const MtNode& n) : MtNode(n) {
    check_sym(sym_template_declaration);
  }
};
struct MtTranslationUnit : public MtNode {
  MtTranslationUnit(){};
  MtTranslationUnit(const MtNode& n) : MtNode(n) {
    check_sym(sym_translation_unit);
  }
};
struct MtTemplateParamList : public MtNode {
  MtTemplateParamList(){};
  MtTemplateParamList(const MtNode& n) : MtNode(n) {
    check_sym(sym_template_parameter_list);
  }
};
struct MtEnumeratorList : public MtNode {
  MtEnumeratorList(){};
  MtEnumeratorList(const MtNode& n) : MtNode(n) {
    check_sym(sym_enumerator_list);
  }
};
struct MtComment : public MtNode {
  MtComment(){};
  MtComment(const MtNode& n) : MtNode(n) { check_sym(sym_comment); }
};
struct MtCaseStatement : public MtNode {
  MtCaseStatement(){};
  MtCaseStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_case_statement);
  }
};
struct MtSwitchStatement : public MtNode {
  MtSwitchStatement(){};
  MtSwitchStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_switch_statement);
  }
};
struct MtUsingDecl : public MtNode {
  MtUsingDecl(){};
  MtUsingDecl(const MtNode& n) : MtNode(n) { check_sym(sym_using_declaration); }
};
struct MtBreakStatement : public MtNode {
  MtBreakStatement(){};
  MtBreakStatement(const MtNode& n) : MtNode(n) {
    check_sym(sym_break_statement);
  }
};
struct MtCondExpr : public MtNode {
  MtCondExpr(){};
  MtCondExpr(const MtNode& n) : MtNode(n) {
    check_sym(sym_conditional_expression);
  }
};
struct MtStorageSpec : public MtNode {
  MtStorageSpec(){};
  MtStorageSpec(const MtNode& n) : MtNode(n) {
    check_sym(sym_storage_class_specifier);
  }
};
struct MtQualifiedId : public MtNode {
  MtQualifiedId(){};
  MtQualifiedId(const MtNode& n) : MtNode(n) {
    check_sym(sym_qualified_identifier);
  }
};
struct MtIfStatement : public MtNode {
  MtIfStatement(){};
  MtIfStatement(const MtNode& n) : MtNode(n) { check_sym(sym_if_statement); }
};
struct MtSizedTypeSpec : public MtNode {
  MtSizedTypeSpec(){};
  MtSizedTypeSpec(const MtNode& n) : MtNode(n) {
    check_sym(sym_sized_type_specifier);
  }
};
struct MtNamespaceDef : public MtNode {
  MtNamespaceDef(){};
  MtNamespaceDef(const MtNode& n) : MtNode(n) {
    check_sym(sym_namespace_definition);
  }
};

//------------------------------------------------------------------------------
