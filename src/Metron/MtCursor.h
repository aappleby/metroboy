#pragma once

#include <assert.h>
#include "MtModLibrary.h"
#include "MtModule.h"

//------------------------------------------------------------------------------

struct MtCursor {

  MtCursor(MtModLibrary* mod_lib, MtModule* mod, FILE* out)
    : mod_lib(mod_lib), mod(mod), out(out) {
    indent_stack.push_back("");
    cursor = mod->source;
  }

  MtModLibrary* mod_lib;
  MtModule* mod;
  FILE* out = nullptr;
  const char* cursor = nullptr;
  std::vector<std::string> indent_stack;

  bool in_init = false;
  bool in_comb = false;
  bool in_seq = false;
  bool in_final = false;
  TSNode current_function_name = { 0 };

  void push_indent(TSNode n) {
    auto e = mod->start(n);
    auto b = e;
    while (*b != '\n') b--;
    indent_stack.push_back(std::string(b + 1, e));
  }

  void pop_indent() {
    indent_stack.pop_back();
  }

  void emit_newline() {
    emit("\n%s", indent_stack.back().c_str());
  }

  MtModule* field_identifier_to_submod(TSNode id);

  //----------

  bool match(TSNode n, const char* str) { return mod->match(n, str); }

  void visit_children(TSNode n, NodeVisitor cv);
  void emit_children(TSNode n);
  void emit_children(TSNode n, NodeVisitor3 cv);
  void emit_span(const char* a, const char* b);
  void emit(TSNode n);
  void emit(const char* fmt, ...);
  void emit_replacement(TSNode n, const char* fmt, ...);
  void skip_over(TSNode n);
  void advance_to(TSNode n);
  void comment_out(TSNode n);

  //----------

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
  void emit_field_expression(TSNode n);
  void emit_dispatch(TSNode n);
};

//------------------------------------------------------------------------------
