#include "MtCursor.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include <stdarg.h>
#include <regex>


//------------------------------------------------------------------------------
// Generic emit() methods

void ModCursor::emit_span(const char* a, const char* b) {
  if (out) fwrite(a, 1, b - a, out);
  fwrite(a, 1, b - a, stdout);
}

void ModCursor::emit(TSNode n) {
  emit_span(mod->start(n), mod->end(n));
}

void ModCursor::emit(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  if (out) vfprintf(out, fmt, args);
  vfprintf(stdout, fmt, args);
  va_end(args);
}

void ModCursor::print_escaped(TSNode n) {
  ::print_escaped(mod->source, ts_node_start_byte(n), ts_node_end_byte(n));
}

void ModCursor::skip_over(TSNode n) {
  if (cursor < mod->start(n)) {
    emit_span(cursor, mod->start(n));
  }
  cursor = mod->end(n);
}

void ModCursor::advance_to(TSNode n) {
  assert(cursor <= mod->start(n));
  emit_span(cursor, mod->start(n));
  cursor = mod->start(n);
}

void ModCursor::advance_past(TSNode n) {
  assert(cursor >= mod->start(n));
  assert(cursor <= mod->end(n));
  emit_span(cursor, mod->end(n));
  cursor = mod->end(n);
}

void ModCursor::comment_out(TSNode n) {
  advance_to(n);
  emit("/* ");
  emit_body(n);
  emit(" */");
}

void ModCursor::emit_body(TSNode n) {
  assert(cursor <= mod->start(n));
  emit_span(cursor, mod->start(n));
  emit(n);
  cursor = mod->end(n);
}

void ModCursor::emit_leaf(TSNode n) {
  assert(!ts_node_is_null(n) && !ts_node_child_count(n));
  emit_body(n);
}

void ModCursor::emit_anon(TSNode n) {
  assert(!ts_node_is_named(n) && !ts_node_is_null(n) && !ts_node_child_count(n));

  switch (ts_node_symbol(n)) {
  case anon_sym_template:
    comment_out(n);
    break;
  default:
    emit_body(n);
    break;
  }
}

void ModCursor::emit_replacement(TSNode n, const char* fmt, ...) {
  assert(cursor == mod->start(n));

  advance_to(n);

  va_list args;
  va_start(args, fmt);
  if (out) vfprintf(out, fmt, args);
  vfprintf(stdout, fmt, args);
  va_end(args);

  skip_over(n);

  assert(cursor == mod->end(n));
}

//------------------------------------------------------------------------------

void ModCursor::emit_error(TSNode n) {
  assert(cursor == mod->start(n));
  printf("\n");
  printf("########################################\n");
  mod->dump_tree(n);
  printf("########################################\n");
  __debugbreak();
}

//------------------------------------------------------------------------------

























//------------------------------------------------------------------------------
// Reformat #includes and replace .h with .sv.

void ModCursor::emit_include(TSNode n) {
  assert(ts_node_symbol(n) == sym_preproc_include);

  auto node_path = ts_node_child_by_field_id(n, field_path);

  auto path = mod->body(node_path);
  static regex rx_trim(R"(\.h)");
  path = std::regex_replace(path, rx_trim, ".sv");
  emit("`include %s", path.c_str());
  cursor = mod->end(node_path);
}

//------------------------------------------------------------------------------
// FIXME - change '=' to '<=' if lhs is a field

void ModCursor::emit_assignment_expression(TSNode n) {
  auto exp_lv = ts_node_child_by_field_id(n, field_left);
  auto exp_op = ts_node_child_by_field_id(n, field_operator);
  auto exp_rv = ts_node_child_by_field_id(n, field_right);

  emit_dispatch(exp_lv);

  // need to check if lhs is a field reference
  if (in_seq) {
    if (ts_node_symbol(exp_lv) == sym_identifier) {
      std::string id = mod->body(exp_lv);

      bool has_field = false;
      for (auto f : mod->fields) {
        if (mod->field_to_name(f) == id) {
          has_field = true;
          break;
        }
      }

      if (has_field) {
        advance_to(exp_op);
        emit("<", id.c_str());
      }
    }
  }

  emit_leaf(exp_op);
  emit_dispatch(exp_rv);
}

//------------------------------------------------------------------------------
// Replace function names with macro names where needed, comment out explicit
// tick/tock calls.

void ModCursor::emit_call_expression(TSNode n) {
  auto call_func = ts_node_child_by_field_id(n, field_function);
  auto call_args = ts_node_child_by_field_id(n, field_arguments);

  if (mod->match(call_func, "clog2")) {
    emit_replacement(call_func, "$clog2");
    emit_dispatch(call_args);
  }
  else if (mod->match(call_func, "readmemh")) {
    emit_replacement(call_func, "$readmemh");
    emit_dispatch(call_args);
  }
  else {
    comment_out(n);
  }

  cursor = mod->end(n);
}

//------------------------------------------------------------------------------
// Change "init/tick/tock" to "initial begin / always_comb / always_ff", change
// void methods to tasks, and change const methods to functions.

void ModCursor::emit_function_definition(TSNode n) {

  auto func_type = ts_node_child_by_field_id(n, field_type);
  auto func_decl = ts_node_child_by_field_id(n, field_declarator);
  auto func_body = ts_node_child_by_field_id(n, field_body);
  auto func_name = ts_node_child_by_field_id(func_decl, field_declarator);

  current_function_name = func_name;

  advance_to(func_type);

  bool is_task = mod->match(func_type, "void");

  // FIXME check if method is const

  //----------
  // Special task/function

  bool is_tick = is_task && mod->match(func_name, "tick");
  bool is_tock = is_task && mod->match(func_name, "tock");
  bool is_init = is_task && mod->match(func_name, "initial");

  if (is_tick) {
    emit("always_comb");
    cursor = mod->end(func_decl);

    in_comb = true;
    emit_dispatch(func_body);
    in_comb = false;

    current_function_name = { 0 };
    return;
  }
  else if (is_tock) {
    cursor = mod->start(func_decl);
    emit_replacement(func_decl, "always_ff @(posedge clk, negedge rst_n)");

    in_seq = true;
    emit_dispatch(func_body);
    in_seq = false;

    current_function_name = { 0 };
    return;
  }
  else if (is_init) {
    emit("initial");
    cursor = mod->end(func_decl);

    in_init = true;
    emit_dispatch(func_body);
    in_init = false;

    current_function_name = { 0 };
    return;
  }

  //----------
  // Generic task/function

  if (is_task) {
    skip_over(func_type);
    emit("task");
    emit_body(func_name);
  }
  else {
    emit("function ");
    emit_body(func_type);
    emit_body(func_name);
  }

  auto func_args = ts_node_child_by_field_id(func_decl, field_parameters);
  emit_dispatch(func_args);
  emit(";");

  for (int i = 0; i < (int)ts_node_child_count(func_body); i++) {
    auto c = ts_node_child(func_body, i);
    auto s = ts_node_symbol(c);

    if (s == anon_sym_LBRACE) {
      skip_over(c);
    }
    else if (s == anon_sym_RBRACE) {
      advance_to(c);
      emit_replacement(c, is_task ? "endtask" : "endfunction");
    }
    else {
      emit_dispatch(c);
    }
  }

  current_function_name = { 0 };
}

//------------------------------------------------------------------------------
// Change static const to localparam.

void ModCursor::emit_glue_declaration(TSNode decl, const std::string& prefix) {
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


void ModCursor::emit_field_declaration(TSNode decl) {
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

  auto submod = mod_lib->find_module(type_name);

  if (submod) {
    ModCursor sub_cursor = {
      mod_lib,
      submod,
      nullptr,
      out,
    };

    for (size_t i = 0; i < submod->inputs.size(); i++) {
      auto input = submod->inputs[i];
      sub_cursor.cursor = submod->start(input);
      sub_cursor.emit_glue_declaration(input, inst_name);
    }
    for (size_t i = 0; i < submod->outputs.size(); i++) {
      auto input = submod->outputs[i];
      sub_cursor.cursor = submod->start(input);
      sub_cursor.emit_glue_declaration(input, inst_name);
    }

    emit_dispatch(node_type);
    emit_dispatch(node_name);
    emit("(clk, rst_n");
    for (size_t i = 0; i < submod->inputs.size(); i++) {
      emit(", %s_", inst_name.c_str());
      emit(submod->field_to_name(submod->inputs[i]).c_str());
    }
    for (size_t i = 0; i < submod->outputs.size(); i++) {
      emit(", %s_", inst_name.c_str());
      emit(submod->field_to_name(submod->outputs[i]).c_str());
    }
    emit(");");
    cursor = mod->end(decl);
  }
  else {
    if (mod->field_is_input(decl)) {
      emit("input ");
    }

    if (mod->field_is_output(decl)) {
      emit("output ");
    }

    mod->visit_children(decl, [&](TSNode child) {
      emit_dispatch(child);
      });
  }

}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void ModCursor::emit_class_specifier(TSNode n) {
  auto node_class = ts_node_child(n, 0);
  auto node_name = ts_node_child(n, 1);
  auto node_body = ts_node_child(n, 2);

  emit_replacement(node_class, "module");
  emit_leaf(node_name);
  emit("\n");

  // Patch the template parameter list in after the module declaration
  if (!ts_node_is_null(mod->module_param_list)) {
    ModCursor sub_cursor = *this;
    sub_cursor.cursor = mod->start(mod->module_param_list);
    sub_cursor.emit_module_parameters(mod->module_param_list);
    emit("\n");
  }

  // Emit an old-style port list
  emit("(clk, rst_n");
  for (auto i : mod->inputs) {
    emit(", %s", mod->field_to_name(i).c_str());
  }
  for (auto o : mod->outputs) {
    emit(", %s", mod->field_to_name(o).c_str());
  }
  emit(");\n");

  // And the declaration of the ports will be in the module body along with
  // the rest of the module.
  emit("  input logic clk;\n");
  emit("  input logic rst_n;\n");

  // Emit the module body, with a few modifications.
  cursor = mod->start(ts_node_child(node_body, 0));
  mod->visit_children(node_body, [&](TSNode child) {
    auto sc = ts_node_symbol(child);
    if (sc == anon_sym_LBRACE) {
      // Discard the opening brace
      skip_over(child);
    }
    else if (sc == anon_sym_RBRACE) {
      // Replace the closing brace with "endmodule"
      advance_to(child);
      emit_replacement(child, "endmodule");
    }
    else if (sc == anon_sym_SEMI) {
      // Discard the seimcolon at the end of class{};"
      skip_over(child);
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------
// Change "{ blah(); }" to "begin blah(); end"

void ModCursor::emit_compound_statement(TSNode n) {
  mod->visit_children(n, [&](TSNode child) {
    auto sc = ts_node_symbol(child);
    if (sc == anon_sym_LBRACE) {
      advance_to(child);
      emit_replacement(child, "begin");
    }
    else if (sc == anon_sym_RBRACE) {
      advance_to(child);
      emit_replacement(child, "end");
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void ModCursor::emit_template_type(TSNode n) {
  auto type_name = ts_node_child_by_field_id(n, field_name);
  auto type_args = ts_node_child_by_field_id(n, field_arguments);

  std::string name(mod->start(type_name), mod->end(type_name));
  bool is_submod = false;

  if (mod->match(type_name, "logic")) {
    auto template_arg = ts_node_named_child(type_args, 0);

    if (ts_node_symbol(template_arg) == sym_type_descriptor) {
      emit_replacement(type_name, "logic[");
      emit(template_arg);
      emit("-1:0]");
      skip_over(n);
    }
    else if (ts_node_symbol(template_arg) == sym_number_literal) {
      if (mod->match(template_arg, "1")) {
        emit_replacement(type_name, "logic");
        cursor = mod->end(n);
      }
      else {
        int width = atoi(mod->start(template_arg));
        emit_replacement(type_name, "logic[%d:0]", width - 1);
        skip_over(n);
      }
    }
    else {
      emit_replacement(type_name, "logic[");
      emit("(");
      emit(template_arg);
      emit(")");
      emit("-1:0]");
      skip_over(n);
    }
  }
  else if (is_submod) {
    emit_dispatch(type_name);
    emit_dispatch(type_args);
  }
  else {
    emit_dispatch(type_name);
    emit_dispatch(type_args);
  }
}

//------------------------------------------------------------------------------
// Change <int param, int param> to #(parameter int param, parameter int param)

void ModCursor::emit_module_parameters(TSNode n) {
  mod->visit_children(n, [&](TSNode child) {
    auto s = ts_node_symbol(child);

    if (s == anon_sym_LT) {
      emit_replacement(child, "#(");
    }
    else if (s == anon_sym_GT) {
      emit_replacement(child, ")");
    }
    else if (s == sym_parameter_declaration) {
      advance_to(child);
      emit("parameter ");
      emit_dispatch(child);
    }
    else if (s == sym_optional_parameter_declaration) {
      advance_to(child);
      emit("parameter ");
      emit_dispatch(child);
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------
// Change <param, param> to #(param, param)

void ModCursor::emit_template_parameters(TSNode n) {
  mod->visit_children(n, [&](TSNode child) {
    auto s = ts_node_symbol(child);

    if (s == anon_sym_LT) {
      emit_replacement(child, " #(");
    }
    else if (s == anon_sym_GT) {
      emit_replacement(child, ")");
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void ModCursor::emit_enumerator_list(TSNode n) {
  mod->visit_children(n, [&](TSNode child) {
    auto sc = ts_node_symbol(child);
    if (sc == anon_sym_LBRACE || sc == anon_sym_RBRACE) {
      emit_leaf(child);
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void ModCursor::emit_translation_unit(TSNode n) {
  emit("/* verilator lint_off WIDTH */\n");
  emit("`default_nettype none\n");

  mod->visit_children(n, [&](TSNode child) {
    auto sc = ts_node_symbol(child);
    sc == anon_sym_SEMI ? skip_over(child) : emit_dispatch(child);
    });
}

//------------------------------------------------------------------------------
// Structs/classes get "begin/end" instead of {}.

void ModCursor::emit_field_declaration_list(TSNode n) {
  mod->visit_children(n, [&](TSNode child) {
    auto sc = ts_node_symbol(child);
    if (sc == anon_sym_LBRACE) {
      emit_replacement(child, "begin");
    }
    else if (sc == anon_sym_RBRACE) {
      advance_to(child);
      emit_replacement(child, "end");
    }
    else {
      emit_dispatch(child);
    }
    });
}

//------------------------------------------------------------------------------

void ModCursor::emit_dispatch(TSNode n) {
  assert(cursor <= mod->start(n));
  advance_to(n);

  if (!ts_node_is_named(n)) {
    if (ts_node_symbol(n) == anon_sym_template) {
      skip_over(n);
    }
    else {
      emit_anon(n);
    }

    return;
  }

  auto s = ts_node_symbol(n);

  switch (s) {
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
    mod->visit_children(n, [&](TSNode c) { emit_dispatch(c); });
    return;

  case sym_storage_class_specifier:
    if (mod->match(n, "static")) {
      advance_to(n);
      emit_replacement(n, "localparam");
    }
    return;

  case sym_type_qualifier:
    if (mod->match(n, "const")) {
      advance_to(n);
      emit_replacement(n, "/*const*/");
    }
    return;

  case sym_return_statement: {
    if (ts_node_is_null(current_function_name)) emit_error(n);
    auto ret_literal = ts_node_child(n, 0);
    emit("%s =", mod->body(current_function_name).c_str());
    emit_span(mod->end(ret_literal), mod->end(n));
    cursor = mod->end(n);
    return;
  }

  case sym_field_expression: {
    auto blah = mod->body(n);
    for (auto& c : blah) if (c == '.') c = '_';
    emit_replacement(n, blah.c_str());
    return;
  }

                            // For some reason the class's trailing semicolon ends up with the template decl, so we prune it here.
  case sym_template_declaration:
    mod->visit_children(n, [&](TSNode child) {
      auto sc = ts_node_symbol(child);
      sc == anon_sym_SEMI ? skip_over(child) : emit_dispatch(child);
      });
    return;

  case alias_sym_field_identifier:
  case sym_identifier:
  case sym_true:
  case sym_false:
  case sym_comment:
    emit_leaf(n);
    return;

  case sym_string_literal:
    emit_body(n);
    return;

  case sym_number_literal: {
    std::string body = mod->body(n);
    if (body.starts_with("0x")) {
      emit("'h%s", body.c_str() + 2);
      skip_over(n);
    }
    else {
      emit_leaf(n);
    }
    return;
  }

  case sym_preproc_call:
  case sym_preproc_if:
    skip_over(n);
    return;

  case sym_access_specifier:
    comment_out(n);
    return;

  case sym_preproc_include:    emit_include(n);            return;
  case sym_field_declaration:  emit_field_declaration(n);  return;
  case sym_compound_statement: emit_compound_statement(n); return;
  case sym_template_type:      emit_template_type(n);      return;
  }

  //----------

  if (s == sym_field_declaration_list) {
    emit_field_declaration_list(n);
  }
  else if (s == sym_translation_unit) {
    emit_translation_unit(n);
  }
  else if (s == sym_primitive_type) {
    // FIXME translate types here
    emit_body(n);
  }
  else if (s == alias_sym_type_identifier) {
    // FIXME translate types here
    emit_body(n);
  }
  else if (s == sym_class_specifier || s == sym_struct_specifier) {
    emit_class_specifier(n);
  }
  else if (s == sym_function_definition) {
    emit_function_definition(n);
  }
  else if (s == sym_call_expression) {
    emit_call_expression(n);
  }
  else if (s == sym_assignment_expression) {
    emit_assignment_expression(n);
  }
  else if (s == sym_template_parameter_list) {
    //emit_module_parameters(n);
    mod->module_param_list = n;
    skip_over(n);
  }
  else if (s == sym_template_argument_list) {
    emit_template_parameters(n);
  }
  else if (s == sym_enumerator_list) {
    emit_enumerator_list(n);
  }
  else if (ts_node_has_error(n)) {
  }
  else {
    printf("\n\n\n########################################\n");
    mod->dump_tree(n);
    printf("\n########################################\n\n\n");
    __debugbreak();

  }
}








