#pragma once
#include "Platform.h"

#include "MtNode.h"
#include <map>

struct MtField;

//------------------------------------------------------------------------------

struct MtCursor {

  MtCursor(MtModule* mod, std::string* out);

  // Debugging
  
  void dump_node_line(MtNode n);
  void print_error(MtNode n, const char* fmt, ...);

  // Indentation

  void push_indent(MtNode n);
  void pop_indent(MtNode n);
  void emit_newline();

  // Generic emit()s.

  void emit_span(const char* a, const char* b);
  void emit_text(MtNode n);
  void emit(const char* fmt, ...);
  void emit_replacement(MtNode n, const char* fmt, ...);
  void skip_over(MtNode n);
  void skip_to_next_sibling(MtNode n);
  void skip_space();
  void advance_to(MtNode n);
  void comment_out(MtNode n);

  // Per-symbol emit()s.

  void emit(MtAssignmentExpr n);
  void emit(MtPreprocInclude n);
  void emit(MtCallExpr n);
  void emit(MtFuncDefinition n);
  void emit(MtReturnStatement n);
  void emit(MtNumberLiteral n, int size_cast = 0);
  void emit(MtPrimitiveType n);
  void emit(MtIdentifier n);
  void emit(MtTypeIdentifier n);
  void emit(MtFieldDecl decl);
  void emit(MtStructSpecifier n);
  void emit(MtCompoundStatement n);
  void emit(MtTemplateType n);
  void emit(MtTemplateDecl n);
  void emit(MtTranslationUnit n);
  void emit(MtFieldExpr n);

  void emit(MtTemplateParamList n);
  void emit(MtTemplateArgList n);
  void emit_field_decl_as_enum_class(MtFieldDecl n);

  void emit(MtEnumeratorList n);
  void emit_dispatch(MtNode n);
  void emit(MtCaseStatement n);
  void emit(MtSwitchStatement n);
  void emit(MtBreakStatement n);
  void emit(MtFieldDeclList n);
  void emit(MtCondExpr n);
  void emit(MtStorageSpec n);
  void emit(MtQualifiedId n);
  void emit(MtIfStatement n);
  void emit(MtEnumSpecifier n);
  void emit(MtUsingDecl n);
  void emit(MtDecl n);
  void emit(MtSizedTypeSpec n);
  void emit(MtNamespaceDef n);
  void emit(MtArgList n);

  void emit_static_bit_extract(MtCallExpr n, int bx_width);
  void emit_dynamic_bit_extract(MtCallExpr n, MtNode bx_node);
  void emit(MtComment n);
  void emit_function_body(MtCompoundStatement n);

  void emit_submodule_port_list(MtFieldDecl decl);

  void emit_glue_declaration(MtField f, const std::string& prefix);
  void emit_glue_declarations(MtFieldDecl decl);
  void emit_glue_assignments(MtFuncDefinition n);
  void emit_glue_assignment(MtCallExpr call_expr);


  // Special-purpose emit()s

  void emit_hoisted_decls(MtCompoundStatement n);
  void emit_init_declarator_as_decl(MtDecl n);
  void emit_init_declarator_as_assign(MtDecl n);

  void emit_port_list();
  void emit_sym_field_declaration_list(MtFieldDeclList class_body);
  void emit_input_ports(std::vector<MtField>& fields);
  void emit_output_ports(std::vector<MtField>& fields);

  void emit_decl_no_semi(MtNode n);

  //----------

  MtModule* mod;
  const char* cursor = nullptr;
  std::string current_function_name;
  std::vector<std::string> spacer_stack;
  
  void emit_char(char c);
  std::string* str_out;

  std::map<std::string, std::string> id_replacements;

  bool in_init = false;
  bool in_tick = false;
  bool in_tock = false;
  bool in_task = false;
  bool in_func = false;

  bool in_module_or_package = false;

  bool trim_namespaces = true;
  bool in_ports = false;

  int override_size = 0;
};

//------------------------------------------------------------------------------
