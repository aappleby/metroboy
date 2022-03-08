#pragma once
#include <map>

#include "MtNode.h"
#include "Platform.h"

struct MtModule;
struct MtField;
struct MtSourceFile;
struct MtModLibrary;

//------------------------------------------------------------------------------

struct MtCursor {
  MtCursor(MtModLibrary* lib, MtSourceFile* source_file, std::string* out);

  // Debugging

  void dump_node_line(MtNode n);
  void print_error(MtNode n, const char* fmt, ...);

  // Indentation

  void push_indent(MtNode n);
  void pop_indent(MtNode n);

  void push_indent_of(MtNode n);
  void pop_indent_of(MtNode n);

  void emit_newline();
  void emit_indent();

  // Generic emit()s.

  void emit_char(char c);
  void emit_ws();
  void emit_span(const char* a, const char* b);
  void emit_text(MtNode n);
  void emit(const char* fmt, ...);
  void emit_replacement(MtNode n, const char* fmt, ...);
  void skip_over(MtNode n);
  void skip_to_next_sibling(MtNode n);
  void skip_to_next_line();
  void emit_to_newline();
  void skip_ws();
  void comment_out(MtNode n);

  // Per-symbol emit()s.

  void emit_dispatch(MtNode n);
  void emit_children(MtNode n);

  void emit(MtArgList n);
  void emit(MtAssignmentExpr n);
  void emit(MtBreakStatement n);
  void emit(MtCallExpr n);
  void emit(MtCaseStatement n);
  void emit(MtComment n);
  void emit(MtCompoundStatement n);
  void emit(MtCondExpr n);
  void emit(MtDecl n);
  void emit(MtEnumeratorList n);
  void emit(MtEnumSpecifier n);
  void emit(MtExprStatement n);
  void emit(MtFieldDecl decl);
  void emit(MtFieldDeclList n);
  void emit(MtFieldExpr n);
  void emit(MtFieldIdentifier n);
  void emit(MtIdentifier n);
  void emit(MtIfStatement n);
  void emit(MtNamespaceDef n);
  void emit(MtNumberLiteral n, int size_cast = 0);
  void emit(MtParameterList n);
  void emit(MtPreprocInclude n);
  void emit(MtPrimitiveType n);
  void emit(MtQualifiedId n);
  void emit(MtReturnStatement n);
  void emit(MtSizedTypeSpec n);
  void emit(MtStorageSpec n);
  void emit(MtClassSpecifier n);
  void emit(MtSwitchStatement n);
  void emit(MtTemplateArgList n);
  void emit(MtTemplateDecl n);
  void emit(MtTemplateParamList n);
  void emit(MtTemplateType n);
  void emit(MtTranslationUnit n);
  void emit(MtTypeIdentifier n);
  void emit(MtUsingDecl n);

  // Special-purpose emit()s

  void emit_field_as_localparam(MtFieldDecl field_decl);
  void emit_func_decl(MtFuncDeclarator n);
  void emit(MtFuncDefinition n);
  void emit_field_as_enum_class(MtFieldDecl n);
  void emit_static_bit_extract(MtCallExpr n, int bx_width);
  void emit_dynamic_bit_extract(MtCallExpr n, MtNode bx_node);
  void emit_hoisted_decls(MtCompoundStatement n);
  void emit_init_declarator_as_decl(MtDecl n);
  void emit_init_declarator_as_assign(MtDecl n);

  //----------

  MtModLibrary* lib = nullptr;
  MtSourceFile* source_file = nullptr;
  MtModule* current_mod = nullptr;
  const char* cursor = nullptr;
  std::string current_function_name;
  std::vector<std::string> indent_stack;

  std::string* str_out;

  std::map<std::string, std::string> id_replacements;

  bool quiet = true;
  bool in_init = false;
  bool in_tick = false;
  bool in_tock = false;
  bool in_task = false;
  bool in_func = false;

  int in_module_or_package = 0;

  bool trim_namespaces = true;
  bool in_ports = false;

  bool line_dirty = false;
  bool line_elided = false;

  int override_size = 0;
};

//------------------------------------------------------------------------------
