#pragma once

//#include <assert.h>
//#include "MtModLibrary.h"
//#include "MtModule.h"
//#include "MtIterator.h"
//#include <set>
//#include <string.h>

struct MtModLibrary;
struct MtModule;
struct MtHandle;

#include <stdio.h>
#include <vector>
#include <string>

//------------------------------------------------------------------------------

struct MtCursor {

  MtCursor(MtModule* mod, FILE* out);

  MtModule* mod;
  const char* cursor = nullptr;
  std::vector<std::string> indent_stack;
  FILE* out;

  bool in_init = false;
  bool in_comb = false;
  bool in_seq = false;
  bool in_final = false;

  //MtHandle current_function_name;
  std::string current_function_name;

  void push_indent(MtHandle n);
  void pop_indent(MtHandle n);
  void emit_newline();

  void dump_node_line(MtHandle n);
  void print_error(MtHandle n, const char* fmt, ...);

  // Generic emit()s.

  void emit_span(const char* a, const char* b);
  void emit(MtHandle n);
  void emit(const char* fmt, ...);
  void emit_replacement(MtHandle n, const char* fmt, ...);
  void skip_over(MtHandle n);
  void skip_whitespace();
  void advance_to(MtHandle n);
  void comment_out(MtHandle n);

  // Per-symbol emit()s.

  void emit_number_literal(MtHandle n);
  void emit_primitive_type(MtHandle n);
  void emit_type_identifier(MtHandle n);
  void emit_preproc_include(MtHandle n);
  void emit_return_statement(MtHandle n);
  void emit_assignment_expression(MtHandle n);
  void emit_call_expression(MtHandle n);
  void emit_function_definition(MtHandle n);
  void emit_glue_declaration(MtHandle decl, const std::string& prefix);
  void emit_field_declaration(MtHandle decl);
  void emit_class_specifier(MtHandle n);
  void emit_compound_statement(MtHandle n);
  void emit_template_type(MtHandle n);
  void emit_module_parameters(MtHandle n);
  void emit_template_declaration(MtHandle n);
  void emit_template_argument_list(MtHandle n);
  void emit_enumerator_list(MtHandle n);
  void emit_translation_unit(MtHandle n);
  void emit_flat_field_expression(MtHandle n);
  void emit_dispatch(MtHandle n);

  // Special-purpose emit()s

  void emit_hoisted_decls(MtHandle n);
  void emit_init_declarator_as_decl(MtHandle n);
  void emit_init_declarator_as_assign(MtHandle n);
};

//------------------------------------------------------------------------------
