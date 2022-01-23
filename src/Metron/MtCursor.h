#pragma once

#include "MtModLibrary.h"
#include "MtModule.h"

//------------------------------------------------------------------------------

struct MtCursor {

  ModLibrary* mod_lib;
  Module* mod;
  const char* cursor = nullptr;
  FILE* out = nullptr;
  bool in_init = false;
  bool in_comb = false;
  bool in_seq = false;
  bool in_final = false;
  TSNode current_function_name = { 0 };

  void visit_children(TSNode n, NodeVisitor cv);

  //----------

  void emit_span(const char* a, const char* b);
  void emit(TSNode n);
  void emit(const char* fmt, ...);
  void skip_over(TSNode n);
  void advance_to(TSNode n);
  void comment_out(TSNode n);
  void emit_anon(TSNode n);
  void emit_replacement(TSNode n, const char* fmt, ...);
  void emit_error(TSNode n);

  //----------

  void emit_basic_replacements(TSNode n);
  void emit_number_literal(TSNode n);
  void emit_primitive_type(TSNode n);
  void emit_type_identifier(TSNode n);

  void emit_preproc_include(TSNode n);
  void emit_return_statement(TSNode n);
  void emit_assignment_expression(TSNode n);
  void emit_call_expression(TSNode n);
  void emit_function_definition(TSNode n);
  void emit_glue_declaration(TSNode decl, const std::string& prefix);
  void emit_field_declaration(TSNode decl);
  void emit_class_specifier(TSNode n);
  void emit_compound_statement(TSNode n);
  void emit_template_type(TSNode n);
  void emit_module_parameters(TSNode n);
  void emit_template_declaration(TSNode n);
  void emit_template_argument_list(TSNode n);
  void emit_enumerator_list(TSNode n);
  void emit_translation_unit(TSNode n);
  void emit_field_declaration_list(TSNode n);
  void emit_field_expression(TSNode n);
  void emit_dispatch(TSNode n);
};


//------------------------------------------------------------------------------
