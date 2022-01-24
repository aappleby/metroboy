#include "MtCursor.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include <stdarg.h>
#include <regex>

//------------------------------------------------------------------------------
// Traversal methods

void MtCursor::visit_children(TSNode n, NodeVisitor cv) {
  for (int i = 0; i < (int)ts_node_child_count(n); i++) {
    auto child = ts_node_child(n, i);
    cv(child);
  }
}

void MtCursor::emit_children(TSNode n, NodeVisitor3 cv) {
  if (cursor < mod->start(n)) {
    emit_span(cursor, mod->start(n));
    cursor = mod->start(n);
  }

  for (int i = 0; i < (int)ts_node_child_count(n); i++) {
    auto child = ts_node_child(n, i);
    auto sym = ts_node_symbol(child);
    auto field = ts_node_field_id_for_child(n, i);

    advance_to(child);
    cv(child, field, sym);
    assert(cursor == mod->end(child));
  }

  if (cursor < mod->end(n)) {
    emit_span(cursor, mod->end(n));
    cursor = mod->end(n);
  }
}

void MtCursor::emit_children(TSNode n) {
  emit_children(n, [&](TSNode n, int field, TSSymbol sym) {
    emit_dispatch(n);
  });
}

//------------------------------------------------------------------------------
// Generic emit() methods

void MtCursor::emit_span(const char* a, const char* b) {
  assert(cursor >= mod->source);
  assert(cursor <  mod->source_end);

  if (out) fwrite(a, 1, b - a, out);
  fwrite(a, 1, b - a, stdout);
}

void MtCursor::emit(TSNode n) {
  assert(cursor == mod->start(n));
  emit_span(mod->start(n), mod->end(n));
  cursor = mod->end(n);
}

void MtCursor::emit(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  if (out) vfprintf(out, fmt, args);
  vfprintf(stdout, fmt, args);
  va_end(args);
}

void MtCursor::skip_over(TSNode n) {
  assert(cursor == mod->start(n));
  cursor = mod->end(n);
}

void MtCursor::advance_to(TSNode n) {
  assert(cursor <= mod->start(n));
  emit_span(cursor, mod->start(n));
  cursor = mod->start(n);
}

void MtCursor::comment_out(TSNode n) {
  assert(cursor == mod->start(n));
  emit("/*");
  emit(n);
  emit("*/");
}

void MtCursor::emit_anon(TSNode n) {
  assert(cursor == mod->start(n));
  assert(!ts_node_is_named(n) && !ts_node_is_null(n) && !ts_node_child_count(n));

  switch (ts_node_symbol(n)) {
  case anon_sym_template:
    comment_out(n);
    break;
  default:
    emit(n);
    break;
  }
}

void MtCursor::emit_replacement(TSNode n, const char* fmt, ...) {
  assert(cursor == mod->start(n));
  va_list args;
  va_start(args, fmt);
  if (out) vfprintf(out, fmt, args);
  vfprintf(stdout, fmt, args);
  va_end(args);
  cursor = mod->end(n);
}

void MtCursor::emit_error(TSNode n) {
  assert(cursor == mod->start(n));
  printf("\n");
  printf("########################################\n");
  mod->dump_tree(n);
  printf("########################################\n");
  __debugbreak();
}

//------------------------------------------------------------------------------
// Replace "#include" with "`include" and ".h" with ".sv"

void MtCursor::emit_preproc_include(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    if (sym == aux_sym_preproc_include_token1) {
      emit_replacement(child, "`include");
    }
    else if (sym == sym_string_literal) {
      auto path = mod->body(child);
      static regex rx_trim(R"(\.h)");
      path = std::regex_replace(path, rx_trim, ".h.sv");
      emit_replacement(child, "%s", path.c_str());
    }
    else {
      emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Change '=' to '<=' if lhs is a field and we're inside a sequential block.

void MtCursor::emit_assignment_expression(TSNode n) {
  if (!in_seq) return emit_children(n);

  bool lvalue_is_field = false;

  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (field) {

    case field_left: {
      if (sym == sym_identifier) {
        std::string lhs_name = mod->body(child);
        for (auto f : mod->fields) {
          if (mod->field_to_name(f) == lhs_name) {
            lvalue_is_field = true;
            break;
          }
        }
      }
      return emit_dispatch(child);
    }

    case field_operator:
      if (lvalue_is_field) emit("<");
      return emit_dispatch(child);
    
    case field_right:
      return emit_dispatch(child);
    
    default:
      __debugbreak();
    }
  });
}

//------------------------------------------------------------------------------
// Replace function names with macro names where needed, comment out explicit
// init/final/tick/tock calls.

void MtCursor::emit_call_expression(TSNode n) {
  assert(ts_node_child_count(n) == 2);
  assert(ts_node_field_id_for_child(n, 0) == field_function);
  assert(ts_node_field_id_for_child(n, 1) == field_arguments);

  auto call_func = ts_node_child_by_field_id(n, field_function);
  auto call_args = ts_node_child_by_field_id(n, field_arguments);

  // If we're calling a member function, look at the name of the member
  // function and not the whole foo.bar().
  if (ts_node_symbol(call_func) == sym_field_expression) {
    call_func = ts_node_child_by_field_id(call_func, field_field);
  }

  if (mod->match(call_func, "clog2")) {
    emit_replacement(call_func, "$clog2");
    emit_dispatch(call_args);
  }
  else if (mod->match(call_func, "readmemh")) {
    emit_replacement(call_func, "$readmemh");
    emit_dispatch(call_args);
  }
  else if (mod->match(call_func, "init")) {
    comment_out(n);
  }
  else if (mod->match(call_func, "final")) {
    comment_out(n);
  }
  else if (mod->match(call_func, "tick")) {
    comment_out(n);
  }
  else if (mod->match(call_func, "tock")) {
    comment_out(n);
  }
  else {
    comment_out(n);
    mod->dump_tree(call_func);
    __debugbreak();
  }
}

//------------------------------------------------------------------------------
// Change "init/tick/tock" to "initial begin / always_comb / always_ff", change
// void methods to tasks, and change const methods to functions.

void MtCursor::emit_function_definition(TSNode n) {
  assert(ts_node_child_count(n) == 3);
  assert(ts_node_field_id_for_child(n, 0) == field_type);
  assert(ts_node_field_id_for_child(n, 1) == field_declarator);
  assert(ts_node_field_id_for_child(n, 2) == field_body);

  auto func_type = ts_node_child_by_field_id(n, field_type);
  auto func_decl = ts_node_child_by_field_id(n, field_declarator);
  auto func_body = ts_node_child_by_field_id(n, field_body);

  bool is_task = false;
  bool is_init = false;
  bool is_tick = false;
  bool is_tock = false;

  //----------

  advance_to(func_type);
  is_task = mod->match(func_type, "void");
  emit("/*");
  emit(func_type);
  emit("*/");

  //----------

  advance_to(func_decl);
  in_init = false;
  in_comb = false;
  in_seq = false;

  current_function_name = ts_node_child_by_field_id(func_decl, field_declarator);
  is_init = is_task && mod->match(current_function_name, "init");
  is_tick = is_task && mod->match(current_function_name, "tick");
  is_tock = is_task && mod->match(current_function_name, "tock");

  if (is_init) {
    emit_replacement(func_decl, "initial");
    in_init = true;
  }
  else if (is_tick) {
    emit_replacement(func_decl, "always_comb");
    in_comb = true;
  }
  else if (is_tock) {
    emit_replacement(func_decl, "always_ff @(posedge clk, negedge rst_n)");
    in_seq = true;
  }
  else {
    if (is_task) {
      emit("task ");
    }
    else {
      emit("function %s ", mod->body(func_type).c_str());
    }

    emit_dispatch(func_decl);
    emit(";");

    in_seq = is_task;
    in_comb = !is_task;
  }

  //----------

  advance_to(func_body);
  emit_children(func_body, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LBRACE:
      if      (is_init) return emit_replacement(child, "begin");
      else if (is_tick) return emit_replacement(child, "begin");
      else if (is_tock) return emit_replacement(child, "begin");
      else if (is_task) return emit_replacement(child, "");
      else              return emit_replacement(child, "");
    case anon_sym_RBRACE:
      if      (is_init) return emit_replacement(child, "end");
      else if (is_tick) return emit_replacement(child, "end");
      else if (is_tock) return emit_replacement(child, "end");
      else if (is_task) return emit_replacement(child, "endtask");
      else              return emit_replacement(child, "endfunction");
    default: return emit_dispatch(child);
    }
  });

  current_function_name = { 0 };
  in_init = false;
  in_comb = false;
  in_seq  = false;
}

//------------------------------------------------------------------------------
// Emit "<type> <submod_name>_<param_name>;" glue declarations because we can't
// directly pass arguments to submodules.

void MtCursor::emit_glue_declaration(TSNode decl, const std::string& prefix) {
  assert(ts_node_symbol(decl) == sym_field_declaration);

  auto node_type = ts_node_child_by_field_id(decl, field_type);
  auto node_name = ts_node_child_by_field_id(decl, field_declarator);

  std::string type_name;
  std::string inst_name = mod->field_to_name(decl);

  if (ts_node_symbol(node_type) == alias_sym_type_identifier || ts_node_symbol(node_type) == sym_primitive_type) {
    type_name = mod->body(node_type);
  }
  else if (ts_node_symbol(node_type) == sym_template_type) {
    type_name = mod->body(ts_node_child_by_field_id(node_type, field_name));
  }
  else {
    __debugbreak();
  }

  emit_dispatch(node_type);
  advance_to(node_name);
  emit("%s_", prefix.c_str());
  emit_dispatch(node_name);
  emit(";\n");
  emit("  ");
}

//------------------------------------------------------------------------------
// Emit field declarations. For submodules, also emit glue declarations and
// append the glue parameter list to the field.

void MtCursor::emit_field_declaration(TSNode decl) {

  // Check if this field is a submodule by looking up its type name in our
  // module list.

  auto node_type = ts_node_child_by_field_id(decl, field_type);
  std::string type_name;

  switch (ts_node_symbol(node_type)) {
  case alias_sym_type_identifier: type_name = mod->body(node_type); break;
  case sym_primitive_type:        type_name = mod->body(node_type); break;
  case sym_template_type:         type_name = mod->body(ts_node_child_by_field_id(node_type, field_name)); break;
  default:                        __debugbreak();
  }

  auto submod = mod_lib->find_module(type_name);

  // If this isn't a submodule, just tack on "input" and "output" annotations.
  if (!submod) {
    if (mod->field_is_input(decl)) emit("input ");
    if (mod->field_is_output(decl)) emit("output ");
    return emit_children(decl);
  }

  // If this is a submodule, emit glue parameters and patch the glue parameter
  // list into the submodule declaration.

  std::string inst_name = mod->field_to_name(decl);

  for (auto& input : submod->inputs) {
    MtCursor sub_cursor = { mod_lib, submod, submod->start(input), out };
    sub_cursor.emit_glue_declaration(input, inst_name);
  }

  for (auto& output : submod->outputs) {
    MtCursor sub_cursor = { mod_lib, submod, submod->start(output), out };
    sub_cursor.emit_glue_declaration(output, inst_name);
  }

  emit_children(decl, [&](TSNode child, int field, TSSymbol sym) {
    emit_dispatch(child);
    if (field == field_declarator) {
      emit("(clk, rst_n");
      for (auto& input : submod->inputs) {
        emit(", %s_%s", inst_name.c_str(), submod->field_to_name(input).c_str());
      }
      for (auto& output : submod->outputs) {
        emit(", %s_%s", inst_name.c_str(), submod->field_to_name(output).c_str());
      }
      emit(")");
    }
  });
}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void MtCursor::emit_class_specifier(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    if (sym == anon_sym_class || sym == anon_sym_struct) {
      emit_replacement(child, "module");
    }
    else if (field == field_name) {
      emit_dispatch(child);

      // Patch the template parameter list in after the module declaration
      if (!ts_node_is_null(mod->module_param_list)) {
        emit("\n");
        MtCursor sub_cursor = *this;
        sub_cursor.cursor = mod->start(mod->module_param_list);
        sub_cursor.emit_module_parameters(mod->module_param_list);
      }

      // Emit an old-style port list
      emit("\n");
      emit("(clk, rst_n");
      for (auto i : mod->inputs) {
        emit(", %s", mod->field_to_name(i).c_str());
      }
      for (auto o : mod->outputs) {
        emit(", %s", mod->field_to_name(o).c_str());
      }
      emit(");");
    }
    else if (field == field_body) {
      // And the declaration of the ports will be in the module body along with
      // the rest of the module.
      emit("\n  input logic clk;");
      emit("\n  input logic rst_n;");

      // Emit the module body, with a few modifications.
      emit_children(child, [&](TSNode child, int field, TSSymbol sym) {
        switch (sym) {
          // Discard the opening brace
        case anon_sym_LBRACE: return skip_over(child);
          // Replace the closing brace with "endmodule"
        case anon_sym_RBRACE: return emit_replacement(child, "endmodule");
          // Discard the seimcolon at the end of class{};"
        case anon_sym_SEMI:   return skip_over(child);
        default:              return emit_dispatch(child);
        }
        });

    }
    else {
      __debugbreak();
    }
  });
}

//------------------------------------------------------------------------------
// Change "{ blah(); }" to "begin blah(); end"

void MtCursor::emit_compound_statement(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LBRACE: return emit_replacement(child, "begin");
    case anon_sym_RBRACE: return emit_replacement(child, "end");
    default:              return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void MtCursor::emit_template_type(TSNode n) {
  bool is_logic = false;

  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    if (sym == alias_sym_type_identifier) {
      if (mod->match(child, "logic")) is_logic = true;
      emit_dispatch(child);
    }
    else if (is_logic && sym == sym_template_argument_list) {
      
      emit_children(child, [&](TSNode child, int field, TSSymbol sym) {
        switch (sym) {
        case anon_sym_LT: return skip_over(child);
        case anon_sym_GT: return skip_over(child);
        case sym_number_literal: {
          int width = atoi(mod->start(child));
          if (width > 1) emit("[%d:0]", width - 1);
          return skip_over(child);
        }
        case sym_type_descriptor: {
          emit("[");
          emit(child);
          emit("-1:0]");
          return;
        }
        default: __debugbreak();
        }
      });

    }
    else {
      emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Change (template)<int param, int param> to
// #(parameter int param, parameter int param)

void MtCursor::emit_module_parameters(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LT: return emit_replacement(child, "#(");
    case anon_sym_GT: return emit_replacement(child, ")");

    // intentional fallthrough, we're just appending "parameter "
    case sym_parameter_declaration:
    case sym_optional_parameter_declaration:
      emit("parameter ");
    default:
      return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Change <param, param> to #(param, param)

void MtCursor::emit_template_argument_list(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LT: return emit_replacement(child, " #(");
    case anon_sym_GT: return emit_replacement(child, ")");
    default:          return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void MtCursor::emit_enumerator_list(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LBRACE: return emit(child);
    case anon_sym_RBRACE: return emit(child);
    default:              return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void MtCursor::emit_translation_unit(TSNode n) {
  emit("/* verilator lint_off WIDTH */\n");
  emit("`default_nettype none\n");

  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_SEMI: return skip_over(child);
    default:            return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Structs/classes get "begin/end" instead of {}.

void MtCursor::emit_field_declaration_list(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_LBRACE: return emit_replacement(child, "begin");
    case anon_sym_RBRACE: return emit_replacement(child, "end");
    default:              return emit_dispatch(child);

    }
  });
}

//------------------------------------------------------------------------------
// Replace "0x" prefixes with "'h"

void MtCursor::emit_number_literal(TSNode n) {
  std::string body = mod->body(n);
  if (body.starts_with("0x")) {
    emit_replacement(n, "'h%s", body.c_str() + 2);
  }
  else {
    emit(n);
  }
}

//------------------------------------------------------------------------------
// Change "return x" to "(funcname) = x" to match old Verilog return style.

void MtCursor::emit_return_statement(TSNode n) {
  auto func_name = mod->body(current_function_name);
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_return: return emit_replacement(child, "%s =", func_name.c_str());
    default:              return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit_primitive_type(TSNode n) {
  emit(n);
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit_type_identifier(TSNode n) {
  emit(n);
}

//------------------------------------------------------------------------------
// For some reason the class's trailing semicolon ends up with the template decl, so we prune it here.

void MtCursor::emit_template_declaration(TSNode n) {
  emit_children(n, [&](TSNode child, int field, TSSymbol sym) {
    switch (sym) {
    case anon_sym_SEMI: return skip_over(child);
    default:            return emit_dispatch(child);
    }
  });
}

//------------------------------------------------------------------------------
// Replace foo.bar.baz with foo_bar_baz, so that a field expression instead
// refers to a glue expression.

void MtCursor::emit_field_expression(TSNode n) {
  auto field = mod->body(n);
  for (auto& c : field) if (c == '.') c = '_';
  emit_replacement(n, field.c_str());
}

//------------------------------------------------------------------------------

void MtCursor::emit_dispatch(TSNode n) {
  assert(cursor <= mod->start(n));

  auto s = ts_node_symbol(n);

  switch (s) {
  case anon_sym_template:
  case anon_sym_if:
  case anon_sym_else:
  case anon_sym_typedef:
  case anon_sym_enum:
  case anon_sym_LF:
  case anon_sym_EQ:
  case anon_sym_SEMI:
  case anon_sym_COMMA:
  case anon_sym_LPAREN:
  case anon_sym_RPAREN:
  case anon_sym_LBRACK:
  case anon_sym_RBRACK:
  case anon_sym_BANG:
  case anon_sym_EQ_EQ:
  case anon_sym_AMP:
  case anon_sym_AMP_AMP:
  case anon_sym_DASH:
  case anon_sym_PLUS:
  case anon_sym_PIPE:
  case anon_sym_PIPE_PIPE:
  case anon_sym_LT:
  case anon_sym_LT_LT:
  case anon_sym_LT_EQ:
  case anon_sym_GT_GT:
  case anon_sym_BANG_EQ:
  case aux_sym_preproc_include_token1:
    emit_anon(n);
    return;

  case alias_sym_field_identifier:
  case sym_identifier:
  case sym_true:
  case sym_false:
  case sym_comment:
  case sym_string_literal:
    emit(n);
    return;

  case sym_if_statement:
  case sym_for_statement:
  case sym_parenthesized_expression:
  case sym_parameter_declaration:
  case sym_optional_parameter_declaration:
  case sym_condition_clause:
  case sym_unary_expression:
  case sym_subscript_expression:
  case sym_enum_specifier:
  case sym_enumerator:
  case sym_type_definition:
  case sym_expression_statement:
  case sym_declaration:
  case sym_binary_expression:
  case sym_argument_list:
  case sym_array_declarator:
  case sym_parameter_list:
  case sym_type_descriptor:
  case sym_function_declarator:
    emit_children(n);
    return;

  case sym_number_literal:         emit_number_literal(n); return;
  case sym_field_expression:       emit_field_expression(n); return;
  case sym_return_statement:       emit_return_statement(n); return;
  case sym_template_declaration:   emit_template_declaration(n); return;
  case sym_preproc_include:        emit_preproc_include(n);            return;
  case sym_field_declaration:      emit_field_declaration(n);  return;
  case sym_compound_statement:     emit_compound_statement(n); return;
  case sym_template_type:          emit_template_type(n);      return;
  case sym_field_declaration_list: emit_field_declaration_list(n); return;
  case sym_translation_unit:       emit_translation_unit(n); return;
  case sym_primitive_type:         emit_primitive_type(n); return;
  case alias_sym_type_identifier:  emit_type_identifier(n); return;
  case sym_class_specifier:        emit_class_specifier(n); return;
  case sym_struct_specifier:       emit_class_specifier(n); return;
  case sym_function_definition:    emit_function_definition(n); return;
  case sym_call_expression:        emit_call_expression(n); return;
  case sym_assignment_expression:  emit_assignment_expression(n); return;
  case sym_template_argument_list: emit_template_argument_list(n); return;
  case sym_enumerator_list:        emit_enumerator_list(n); return;

  case sym_storage_class_specifier: {
    if (mod->match(n, "static")) {
      emit_replacement(n, "localparam");
    }
    else {
      comment_out(n);
    }
    return;
  }

  case sym_access_specifier:
  case sym_type_qualifier:
    comment_out(n);
    return;

  case sym_preproc_call:
  case sym_preproc_if:
    skip_over(n);
    return;

  case sym_template_parameter_list:
    mod->module_param_list = n;
    skip_over(n);
    return;

  default:
    printf("\n\n\n########################################\n");
    mod->dump_tree(n);
    printf("\n########################################\n\n\n");
    __debugbreak();
  }
}
