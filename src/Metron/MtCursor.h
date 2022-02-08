#pragma once
#include "Platform.h"

//------------------------------------------------------------------------------

struct MtCursor {

  MtCursor(MtModule* mod, FILE* out);

  // Debugging
  
  void dump_node_line(MtNode n);
  void print_error(MtNode n, const char* fmt, ...);

  // Indentation

  void push_indent(MtNode n);
  void pop_indent(MtNode n);
  void emit_newline();

  // Generic emit()s.

  void emit_span(const char* a, const char* b);
  void emit(MtNode n);
  void emit(const char* fmt, ...);
  void emit_replacement(MtNode n, const char* fmt, ...);
  void skip_over(MtNode n);
  void skip_space();
  void advance_to(MtNode n);
  void comment_out(MtNode n);

  // Per-symbol emit()s.

  void emit_number_literal(MtNode n);
  void emit_primitive_type(MtNode n);
  void emit_identifier(MtNode n);
  void emit_type_identifier(MtNode n);
  void emit_preproc_include(MtNode n);
  void emit_return_statement(MtNode n);
  void emit_assignment_expression(MtNode n);
  void emit_call_expression(MtNode n);
  void emit_function_definition(MtNode n);
  void emit_template_glue_declaration(MtNode decl, const std::string& prefix);
  void emit_glue_declaration(MtNode decl, const std::string& prefix);
  void emit_field_declaration(MtNode decl);
  void emit_class_specifier(MtNode n);
  void emit_compound_statement(MtNode n);
  void emit_template_type(MtNode n);
  void emit_module_parameters(MtNode n);
  void emit_template_declaration(MtNode n);
  void emit_template_argument_list(MtNode n);
  void emit_enumerator_list(MtNode n);
  void emit_translation_unit(MtNode n);
  void emit_flat_field_expression(MtNode n);
  void emit_dispatch(MtNode n);
  void emit_case(MtNode n);
  void emit_switch(MtNode n);
  void emit_bit_extract(MtNode n, int bx_width);

  // Special-purpose emit()s

  void emit_hoisted_decls(MtNode n);
  void emit_init_declarator_as_decl(MtNode n);
  void emit_init_declarator_as_assign(MtNode n);

  //----------

  MtModule* mod;
  const char* cursor = nullptr;
  std::string current_function_name;
  std::vector<std::string> indent_stack;
  FILE* out;

  std::map<std::string, std::string> id_replacements;

  bool in_init = false;
  bool in_comb = false;
  bool in_seq = false;
  bool in_final = false;
};

//------------------------------------------------------------------------------
