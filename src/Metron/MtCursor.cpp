#include "MtCursor.h"

#include "Platform.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtNode.h"

//------------------------------------------------------------------------------

MtCursor::MtCursor(MtModule* mod, FILE* out) : mod(mod), out(out) {
  indent_stack.push_back("");
  cursor = mod->source;
}

//------------------------------------------------------------------------------

void MtCursor::push_indent(MtNode n) {
  if (n) {
    auto e = n.start();
    auto b = e;
    while (*b != '\n') b--;
    indent_stack.push_back(std::string(b + 1, e));
  }
}

void MtCursor::pop_indent(MtNode n) {
  if (n) indent_stack.pop_back();
}

void MtCursor::emit_newline() {
  emit("\n%s", indent_stack.back().c_str());
}

//------------------------------------------------------------------------------

void MtCursor::dump_node_line(MtNode n) {
  auto start = &mod->source[n.start_byte()];

  auto a = start;
  auto b = start;
  while (a > mod->source     && *a != '\n' && *a != '\r') a--;
  while (b < mod->source_end && *b != '\n' && *b != '\r') b++;

  if (*a == '\n' || *a == '\r') a++;

  while (a != b) {
    fputc(*a++, stdout);
  }
}

//------------------------------------------------------------------------------

void MtCursor::print_error(MtNode n, const char* fmt, ...) {
  emit("\n########################################\n");

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  emit("@%04d: ", ts_node_start_point(n.node).row + 1);
  dump_node_line(n);
  printf("\n");

  n.dump_tree(0, 0, 255);

  emit("halting...\n");
  emit("########################################\n");
  debugbreak();
}

//------------------------------------------------------------------------------
// Generic emit() methods

void MtCursor::emit_span(const char* a, const char* b) {
  assert(cursor >= mod->source);
  assert(cursor <= mod->source_end);
  if (out) fwrite(a, 1, b - a, out);
  fwrite(a, 1, b - a, stdout);
}

void MtCursor::emit(MtNode n) {
  emit_span(cursor, n.end());
  cursor = n.end();
}

void MtCursor::emit(const char* fmt, ...) {
  {
    va_list args;
    va_start(args, fmt);
    if (out) vfprintf(out, fmt, args);
    va_end(args);
  }
  {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }
}

void MtCursor::emit_replacement(MtNode n, const char* fmt, ...) {
  advance_to(n);
  {
    va_list args;
    va_start(args, fmt);
    if (out) vfprintf(out, fmt, args);
    va_end(args);
  }
  {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }
  cursor = n.end();
}

void MtCursor::skip_over(MtNode n) {
  advance_to(n);
  cursor = n.end();
}

void MtCursor::skip_space() {
  while(*cursor && (*cursor == ' ')) {
    cursor++;
  }
}

void MtCursor::advance_to(MtNode n) {
  assert(cursor <= n.start());
  emit_span(cursor, n.start());
  cursor = n.start();
}

void MtCursor::comment_out(MtNode n) {
  advance_to(n);
  emit("/*");
  emit(n);
  emit("*/");
}

//------------------------------------------------------------------------------
// Replace "#include" with "`include" and ".h" with ".sv"

void MtCursor::emit_preproc_include(MtNode n) {
  for (auto c : n) {
    switch (c.sym) {
    case aux_sym_preproc_include_token1: emit_replacement(c, "`include"); break;
    case sym_string_literal: {
      auto path = c.body();
      path.pop_back();
      path.append(".sv\"");
      emit_replacement(c, "%s", path.c_str());
      break;
    }
    default: emit_dispatch(c); break;
    }
  }
}

//------------------------------------------------------------------------------
// Change '=' to '<=' if lhs is a field and we're inside a sequential block.

void MtCursor::emit_assignment_expression(MtNode n) {
  auto lhs = n.get_field(field_left);
  auto op  = n.get_field(field_operator);
  auto rhs = n.get_field(field_right);

  bool lhs_is_field = false;
  if (lhs.sym == sym_identifier) {
    std::string lhs_name = lhs.body();
    for (auto& f : mod->fields) {
      if (f.node_to_name() == lhs_name) {
        lhs_is_field = true;
        break;
      }
    }
  }

  emit_dispatch(lhs);

  advance_to(op);
  if (in_seq && lhs_is_field) emit("<");
  emit_dispatch(op);

  emit_dispatch(rhs);
}

//------------------------------------------------------------------------------

void MtCursor::emit_bit_extract(MtNode n, int bx_width) {
  advance_to(n);

  auto call_args = n.get_field(field_arguments);

  int arg_count = call_args.named_child_count();

  auto arg0 = call_args.named_child(0);
  auto arg1 = call_args.named_child(1);
  auto arg2 = call_args.named_child(2);

  if (arg_count == 1) {
    if (arg0.sym == sym_identifier) {
      // Truncate
      emit_replacement(n, "%s[%d:0]", arg0.body().c_str(), bx_width - 1);
    }
    else if (arg0.sym == sym_number_literal) {
      // Truncate literal

      emit("%d", bx_width);

      //emit_replacement(n);
      cursor = arg0.start();
      emit_number_literal(arg0);
      cursor = n.end();

      //emit_replacement(n, "lskjdf");
    }
    else {
      debugbreak();
    }
  }
  else if (arg_count == 2) {
    if (arg0.sym != sym_identifier) debugbreak();
    if (arg1.sym != sym_number_literal) debugbreak();
    int offset = atoi(arg1.start());

    // Slice at offset
    if (bx_width == 1) {
      emit_replacement(n, "%s[%d]", arg0.body().c_str(), offset);
    }
    else {
      emit_replacement(n, "%s[%d:%d]", arg0.body().c_str(), bx_width - 1 + offset, offset);
    }
  }
  else if (arg_count == 3) {
    if (arg0.sym != sym_identifier) debugbreak();
    if (arg1.sym != sym_number_literal) debugbreak();

    int b = atoi(arg1.start());
    int e = atoi(arg2.start());

    // Slice at offset
    emit_replacement(n, "%s[%d:%d]", arg0.body().c_str(), b, e);
  }
  else {
    debugbreak();
  }
}


//------------------------------------------------------------------------------
// Replace function names with macro names where needed, comment out explicit
// init/final/tick/tock calls.

/*
[2] f34 s267 value.call_expression:
|   [0] f15 s323 function.template_function:
|   |   [0] f22 s1 name.identifier: "bx"
|   |   [1] f3 s324 arguments.template_argument_list:
|   |   |   [0] s36 lit: "<"
|   |   |   [1] s112 number_literal: "11"
|   |   |   [2] s33 lit: ">"
|   [1] f3 s268 arguments.argument_list:
|   |   [0] s5 lit: "("
|   |   [1] s1 identifier: "blah"
|   |   [2] s8 lit: ")"
*/

void MtCursor::emit_call_expression(MtNode n) {
  auto call_func = n.get_field(field_function);
  auto call_args = n.get_field(field_arguments);

  // If we're calling a member function, look at the name of the member
  // function and not the whole foo.bar().

  std::string func_name;

  if (call_func.sym == sym_field_expression) {
    func_name = call_func.get_field(field_field).node_to_name();
  }
  else {
    func_name = call_func.node_to_name();
  }

  if (func_name == "clog2") {
    emit_replacement(call_func, "$clog2");
    emit_dispatch(call_args);
  }
  else if (func_name == "readmemh") {
    emit_replacement(call_func, "$readmemh");
    emit_dispatch(call_args);
  }
  else if (func_name == "printf") {
    emit_replacement(call_func, "$write");
    emit_dispatch(call_args);
  }
  else if (func_name == "init") {
    comment_out(n);
  }
  else if (func_name == "final") {
    comment_out(n);
  }
  else if (func_name == "tick") {
    comment_out(n);
  }
  else if (func_name == "tock") {
    comment_out(n);
  }
  else if (func_name == "bx") {
    advance_to(n);

    // Bit extract.

    assert(call_func.sym == sym_template_function);
    auto template_args = call_func.get_field(field_arguments);
    auto template_arg = template_args.named_child(0);

    int bx_width = atoi(template_arg.start());

    int arg_count = call_args.named_child_count();

    auto arg0 = call_args.named_child(0);
    auto arg1 = call_args.named_child(1);
    auto arg2 = call_args.named_child(2);

    if (arg_count == 1) {
      if (arg0.sym == sym_identifier) {
        // Truncate
        emit_replacement(n, "%s[%d:0]", arg0.body().c_str(), bx_width - 1);
      }
      else if (arg0.sym == sym_number_literal) {
        // Truncate literal

        advance_to(n);
        emit("%d", bx_width);

        //emit_replacement(n);
        cursor = arg0.start();
        emit_number_literal(arg0);
        cursor = n.end();

        //emit_replacement(n, "lskjdf");
      }
      else {
        debugbreak();
      }
    }
    else if (arg_count == 2) {
      if (arg0.sym != sym_identifier) debugbreak();
      if (arg1.sym != sym_number_literal) debugbreak();
      int offset = atoi(arg1.start());

      // Slice at offset
      if (bx_width == 1) {
        emit_replacement(n, "%s[%d]", arg0.body().c_str(), offset);
      }
      else {
        emit_replacement(n, "%s[%d:%d]", arg0.body().c_str(), bx_width - 1 + offset, offset);
      }
    }
    else if (arg_count == 3) {
      if (arg0.sym != sym_identifier) debugbreak();
      if (arg1.sym != sym_number_literal) debugbreak();

      int b = atoi(arg1.start());
      int e = atoi(arg2.start());

      // Slice at offset
      emit_replacement(n, "%s[%d:%d]", arg0.body().c_str(), b, e);
    }
    else {
      debugbreak();
    }
  }
  else if (func_name == "b1")  emit_bit_extract(n, 1);
  else if (func_name == "b2")  emit_bit_extract(n, 2);
  else if (func_name == "b3")  emit_bit_extract(n, 3);
  else if (func_name == "b4")  emit_bit_extract(n, 4);
  else if (func_name == "b5")  emit_bit_extract(n, 5);
  else if (func_name == "b6")  emit_bit_extract(n, 6);
  else if (func_name == "b7")  emit_bit_extract(n, 7);
  else if (func_name == "b8")  emit_bit_extract(n, 8);
  else if (func_name == "b9")  emit_bit_extract(n, 9);
  else if (func_name == "b10") emit_bit_extract(n, 10);
  else if (func_name == "b11") emit_bit_extract(n, 11);
  else if (func_name == "b12") emit_bit_extract(n, 12);
  else if (func_name == "b13") emit_bit_extract(n, 13);
  else if (func_name == "b14") emit_bit_extract(n, 14);
  else if (func_name == "b15") emit_bit_extract(n, 15);
  else if (func_name == "b16") emit_bit_extract(n, 16);
  else if (func_name == "b17") emit_bit_extract(n, 17);
  else if (func_name == "b18") emit_bit_extract(n, 18);
  else if (func_name == "b19") emit_bit_extract(n, 19);
  else if (func_name == "b20") emit_bit_extract(n, 20);
  else if (func_name == "b21") emit_bit_extract(n, 21);
  else if (func_name == "b22") emit_bit_extract(n, 22);
  else if (func_name == "b23") emit_bit_extract(n, 23);
  else if (func_name == "b24") emit_bit_extract(n, 24);
  else if (func_name == "b25") emit_bit_extract(n, 25);
  else if (func_name == "b26") emit_bit_extract(n, 26);
  else if (func_name == "b27") emit_bit_extract(n, 27);
  else if (func_name == "b28") emit_bit_extract(n, 28);
  else if (func_name == "b29") emit_bit_extract(n, 29);
  else if (func_name == "b30") emit_bit_extract(n, 30);
  else if (func_name == "b31") emit_bit_extract(n, 31);
  else if (func_name == "b32") emit_bit_extract(n, 32);
  else if (func_name == "cat") {
    // Remove "cat" and replace parens with brackets

    skip_over(call_func);
    for (const auto& arg : call_args) switch (arg.sym) {
    case anon_sym_LPAREN: emit_replacement(arg, "{"); break;
    case anon_sym_RPAREN: emit_replacement(arg, "}"); break;
    default: emit_dispatch(arg); break;
    }
  }
  else if (func_name == "dup") {
    // 15 {instr_i[12]}}

    assert(call_func.sym == sym_template_function);
    auto template_args = call_func.get_field(field_arguments);
    auto template_arg = template_args.named_child(0);

    int dup_count = atoi(template_arg.start());
    int arg_count = call_args.named_child_count();

    auto arg0 = call_args.named_child(0);

    if (arg_count == 1) {
      skip_over(call_func);
      emit("{%d ", dup_count);
      emit("{");
      cursor = arg0.start();
      emit_dispatch(arg0);
      emit("}}");
      cursor = n.end();
    }
    else {
      debugbreak();
    }

  }
  else {
    // All other function/task calls go through normally.
    for (auto c : n) emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "logic blah;"

void MtCursor::emit_init_declarator_as_decl(MtNode n) {

  for (auto c : n) switch (c.field) {
  case field_declarator:
    for (auto gc : c) switch (gc.field) {
    case field_declarator: emit(gc); skip_space(); break;
    default: skip_over(gc); skip_space(); break;
    }
    break;
  default: emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "blah = x;"

void MtCursor::emit_init_declarator_as_assign(MtNode n) {

  auto node_decl = n.get_field(field_declarator);

  if (node_decl.is_init_decl()) {
    for (auto c : n) switch (c.field) {
    case field_type: skip_over(c); skip_space(); break;
    default: emit_dispatch(c); break;
    }
  }
  else {
    skip_over(n);
    skip_space();
  }
}

//------------------------------------------------------------------------------
// Emit local variable declarations at the top of the block scope.

void MtCursor::emit_hoisted_decls(MtNode n) {
  MtCursor old_cursor = *this;
  for (auto c : n) {
    if (c.sym == sym_declaration) {
      cursor = c.start();
      emit_newline();
      emit_init_declarator_as_decl(c);
    }
  }
  *this = old_cursor;
}

//------------------------------------------------------------------------------
// Change "init/tick/tock" to "initial begin / always_comb / always_ff", change
// void methods to tasks, and change const methods to functions.

void MtCursor::emit_function_definition(MtNode func_def) {

  auto func_type = func_def.get_field(field_type);
  auto func_decl = func_def.get_field(field_declarator);
  auto func_body = func_def.get_field(field_body);

  bool is_task = false;
  bool is_init = false;
  bool is_tock = false;
  bool is_tick = false;

  //----------

  is_task = func_type.match("void");
  skip_over(func_type);
  skip_space();

  //----------

  in_init = false;
  in_comb = false;
  in_seq = false;

  current_function_name = func_decl.get_field(field_declarator).node_to_name();
  is_init = is_task && current_function_name == "init";
  is_tick = is_task && current_function_name == "tick";
  is_tock = is_task && current_function_name == "tock";

  if (is_init) {
    emit_replacement(func_decl, "initial");
    in_init = true;
  }
  else if (is_tick) {
    emit_replacement(func_decl, "always_ff @(posedge clk, negedge rst_n)");
    in_seq = true;
  }
  else if (is_tock) {
    emit_replacement(func_decl, "always_comb");
    in_comb = true;
  }
  else {
    advance_to(func_decl);
    if (is_task) {
      emit("task ");
    }
    else {
      emit("function %s ", func_type.body().c_str());
    }

    emit_dispatch(func_decl);
    skip_space();
    emit(";");

    in_seq = is_task;
    in_comb = !is_task;
  }

  //----------
  // Emit the module body with the correct type of "begin/end" pair,
  // hoisting locals to the top of the body scope.

  push_indent(func_body.first_named_child());

  for (auto c : func_body) switch (c.sym) {
  case anon_sym_LBRACE:
    if      (is_init) emit_replacement(c, "begin : INIT");
    else if (is_tick) emit_replacement(c, "begin : TICK");
    else if (is_tock) emit_replacement(c, "begin : TOCK");
    else if (is_task) emit_replacement(c, "");
    else              emit_replacement(c, "");
    emit_hoisted_decls(func_body);
    break;

  case anon_sym_RBRACE:
    if      (is_init) emit_replacement(c, "end");
    else if (is_tick) emit_replacement(c, "end");
    else if (is_tock) emit_replacement(c, "end");
    else if (is_task) emit_replacement(c, "endtask");
    else              emit_replacement(c, "endfunction");
    break;

  case sym_declaration: emit_init_declarator_as_assign(c); break;

  default: emit_dispatch(c); break;
  }

  pop_indent(func_body.first_named_child());

  //----------
  // For each call to {submodule}.tick() in module::tick(), emit glue assignments.

  current_function_name = "";
  in_init = false;
  in_comb = false;
  in_seq  = false;

  if (is_tick && !mod->submodules.empty()) {
    emit_newline();

    std::vector<MtNode> submod_call_nodes;

    func_def.visit_tree([&](MtNode child) {
      if (child.sym == sym_call_expression) {
        auto call_func = child.get_field(field_function);

        if (call_func.sym == sym_field_expression) {
          auto call_args = child.get_field(field_arguments);
          auto call_this = call_func.get_field(field_argument);
          auto func_name = call_func.get_field(field_field);

          if (func_name.is_null()) {
            printf("FUNC NAME NULL\n");
          }

          if (func_name.match("tick")) {
            submod_call_nodes.push_back(child);
          }
        }
      }
    });

    for (auto& submod_call : submod_call_nodes) {
      auto call_func = submod_call.get_field(field_function);
      auto call_args = submod_call.get_field(field_arguments);
      auto call_this = call_func.get_field(field_argument);
      auto func_name = call_func.get_field(field_field);

      for (auto& sm : mod->submodules) {
        auto submod_type = sm.node_to_type();
        auto submod_name = sm.node_to_name();
        if (submod_name == call_this.node_to_name()) {
          auto submod = mod->lib->find_module(submod_type);

          std::vector<std::string> call_src;
          std::vector<std::string> call_dst;

          for (auto arg : call_args) {
            if (!arg.is_named()) continue;
            auto src = arg.node_to_name();
            for (auto& c : src) if (c == '.') c = '_';
            if (src != "rst_n") call_src.push_back(src);
          }

          for (auto& input : submod->inputs) {
            call_dst.push_back(input.node_to_name());
          }

          assert(call_src.size() == call_dst.size());

          for (int i = 0; i < call_src.size(); i++) {
            
            emit_newline();
            emit("assign %s_%s = %s;",
              submod_name.c_str(),
              call_dst[i].c_str(),
              call_src[i].c_str());
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Emit "<type> <submod_name>_<param_name>;" glue declarations because we can't
// directly pass arguments to submodules.

void MtCursor::emit_glue_declaration(MtNode decl, const std::string& prefix) {

  assert(decl.sym == sym_field_declaration ||
         decl.sym == sym_parameter_declaration);

  auto node_type = decl.get_field(field_type);
  auto node_name = decl.get_field(field_declarator);

  std::string type_name;

  if (node_type.sym == alias_sym_type_identifier || node_type.sym == sym_primitive_type) {
    type_name = node_type.body();
  }
  else if (node_type.sym == sym_template_type) {
    type_name = node_type.get_field(field_name).body();
  }
  else {
    debugbreak();
  }

  emit_dispatch(node_type);
  advance_to(node_name);
  emit("%s_", prefix.c_str());
  emit_dispatch(node_name);
  emit(";");
  emit_newline();
}

//------------------------------------------------------------------------------
// Emit field declarations. For submodules, also emit glue declarations and
// append the glue parameter list to the field.

void MtCursor::emit_field_declaration(MtNode decl) {

  // Check if this field is a submodule by looking up its type name in our
  // module list.

  auto node_type = decl.get_field(field_type);
  std::string type_name;

  switch (node_type.sym) {
  case alias_sym_type_identifier: type_name = node_type.body(); break;
  case sym_primitive_type:        type_name = node_type.body(); break;
  case sym_template_type:         type_name = node_type.get_field(field_name).body(); break;
  
  case sym_enum_specifier: {
    //decl.dump_tree();

    auto name = node_type.get_field(field_name);
    if (name.is_null()) {
      type_name = "<anonymous enum>";
    }
    else {
      type_name = node_type.get_field(field_name).body(); break;
    }
    break;
  }
  
  default: {
    decl.dump_tree();
    debugbreak();
  }
  }

  auto submod = mod->lib->find_module(type_name);


  // If this isn't a submodule, just tack on "input" and "output" annotations.
  if (!submod) {
    if (decl.field_is_input()) {
      advance_to(decl);
      emit("input ");
    }
    if (decl.field_is_output()) {
      advance_to(decl);
      emit("output ");
    }

    if (node_type.sym == sym_enum_specifier) {
      //decl.dump_tree();

      advance_to(decl);
      auto node_value = decl.get_field(field_default_value);
      emit("typedef enum ");
      cursor = node_value.start();
      emit_dispatch(node_value);

      auto name = node_type.get_field(field_name);
      cursor = name.start();
      emit(" ");
      emit_dispatch(name);
      cursor = decl.end();
      emit(";");
      return;
    }
    else {
      for (auto c : decl) {
        emit_dispatch(c);
      }
    }

    return;
  }

  // If this is a submodule, emit glue parameters and patch the glue parameter
  // list into the submodule declaration.

  advance_to(decl);
  std::string inst_name = decl.node_to_name();

  for (auto& input : submod->inputs) {
    MtCursor sub_cursor(submod, out);
    sub_cursor.cursor = input.start();
    sub_cursor.indent_stack = indent_stack;
    sub_cursor.emit_glue_declaration(input, inst_name);
  }

  for (auto& output : submod->outputs) {
    MtCursor sub_cursor(submod, out);
    sub_cursor.cursor = output.start();
    sub_cursor.indent_stack = indent_stack;
    sub_cursor.emit_glue_declaration(output, inst_name);
  }

  for (auto c : decl) {
    emit_dispatch(c);
    if (c.field == field_declarator) {
      emit("(clk, rst_n");
      for (auto& input : submod->inputs) {
        emit(", %s_%s", inst_name.c_str(), input.node_to_name().c_str());
      }
      for (auto& output : submod->outputs) {
        emit(", %s_%s", inst_name.c_str(), output.node_to_name().c_str());
      }
      emit(")");
    }
  }

  emit_newline();
}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void MtCursor::emit_class_specifier(MtNode n) {

  for (auto c : n) {
    if (c.sym == anon_sym_class || c.sym == anon_sym_struct) {
      emit_replacement(c, "module");
    }
    else if (c.field == field_name) {
      emit_dispatch(c);

      // Patch the template parameter list in after the module declaration
      if (mod->mod_param_list) {
        emit_newline();
        MtCursor sub_cursor = *this;
        sub_cursor.cursor = mod->mod_param_list.start();
        sub_cursor.emit_module_parameters(mod->mod_param_list);
      }

      // Emit an old-style port list
      emit_newline();
      emit("(clk, rst_n");
      for (auto input : mod->inputs) {
        emit(", %s", input.node_to_name().c_str());
      }
      for (auto output : mod->outputs) {
        emit(", %s", output.node_to_name().c_str());
      }
      emit(");");

    }
    else if (c.field == field_body) {
      // And the declaration of the ports will be in the module body along with
      // the rest of the module.

      push_indent(c.first_named_child());

      emit_newline();
      emit("/*verilator public_module*/");
      emit_newline();

      emit_newline();
      emit("input logic clk;");

      emit_newline();
      emit("input logic rst_n;");

      for (auto input : mod->inputs) {
        MtCursor sub_cursor = *this;
        sub_cursor.cursor = input.start();
        emit_newline();
        emit("input ");
        sub_cursor.emit_dispatch(input);
        emit(";");
      }

      // Emit the module body, with a few modifications.
      // Discard the opening brace
      // Replace the closing brace with "endmodule"
      // Discard the seimcolon at the end of class{};"

      for (auto gc : c) switch (gc.sym) {
      case anon_sym_LBRACE: emit_replacement(gc, ""); break;
      case anon_sym_RBRACE: emit_replacement(gc, "endmodule"); break;
      case anon_sym_SEMI:   emit_replacement(gc, ""); break;
      default:              emit_dispatch(gc); break;
      }

      pop_indent(c.first_named_child());
    }
    else {
      c.dump_tree(0, 0, 1);
      debugbreak();
    }
  }
}

//------------------------------------------------------------------------------
// Change "{ blah(); foo(); int x = 1; }" to "begin blah(); ... end"

void MtCursor::emit_compound_statement(MtNode body) {
  push_indent(body.first_named_child());

  for (auto c : body) switch (c.sym) {
  case anon_sym_LBRACE:
    emit_replacement(c, "begin");
    emit_hoisted_decls(body);
    break;
  case sym_declaration: emit_init_declarator_as_assign(c); break;
  case anon_sym_RBRACE: emit_replacement(c, "end"); break;
  default: emit_dispatch(c); break;
  }

  pop_indent(body.first_named_child());
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void MtCursor::emit_template_type(MtNode n) {
  auto node_name = n.get_field(field_name);
  auto node_args = n.get_field(field_arguments);

  bool is_logic = node_name.match("logic");

  if (!is_logic) {
    emit_dispatch(node_name);
    emit_dispatch(node_args);
    return;
  }

  emit_dispatch(node_name);
  for (auto c : node_args) switch (c.sym) {
  case anon_sym_LT: skip_over(c); break;
  case anon_sym_GT: skip_over(c); break;
  case sym_number_literal: {
    int width = atoi(c.start());
    if (width > 1) emit("[%d:0]", width - 1);
    skip_over(c);
    break;
  }
  case sym_type_descriptor: {
    emit("[");
    emit(c);
    emit("-1:0]");
    break;
  }
  default: debugbreak();
  }
}

//------------------------------------------------------------------------------
// Change (template)<int param, int param> to
// #(parameter int param, parameter int param)

void MtCursor::emit_module_parameters(MtNode n) {
  for (auto c : n) switch (c.sym) {
  case anon_sym_LT: emit_replacement(c, "#("); break;
  case anon_sym_GT: emit_replacement(c, ")"); break;

  // intentional fallthrough, we're just appending "parameter "
  case sym_parameter_declaration:
  case sym_optional_parameter_declaration:
    advance_to(c);
    emit("parameter ");
  default:
    emit_dispatch(c);
    break;
  }
}

//------------------------------------------------------------------------------
// Change <param, param> to #(param, param)

void MtCursor::emit_template_argument_list(MtNode n) {
  for (auto c : n) switch (c.sym) {
  case anon_sym_LT: emit_replacement(c, " #("); break;
  case anon_sym_GT: emit_replacement(c, ")"); break;
  default:          emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void MtCursor::emit_enumerator_list(MtNode n) {
  for (auto c : n) switch (c.sym) {
  case anon_sym_LBRACE: emit(c); break;
  case anon_sym_RBRACE: emit(c); break;
  default: {
    emit_dispatch(c);
    break;
  }
  }
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void MtCursor::emit_translation_unit(MtNode n) {
  emit("/* verilator lint_off WIDTH */\n");
  emit("`default_nettype none\n");

  for (auto c : n) switch (c.sym) {
  case anon_sym_SEMI: skip_over(c); break;
  default:            emit_dispatch(c); break;
  }

  emit_span(cursor, mod->source_end);
}

//------------------------------------------------------------------------------
// Replace "0x" prefixes with "'h"
// Replace "0b" prefixes with "'b"

void MtCursor::emit_number_literal(MtNode n) {
  std::string body = n.body();
  if (body.starts_with("0x")) {
    emit_replacement(n, "'h%s", body.c_str() + 2);
  }
  else if (body.starts_with("0b")) {
    emit_replacement(n, "'b%s", body.c_str() + 2);
  }
  else {
    advance_to(n);
    emit("'d");
    emit(n);
  }
}

//------------------------------------------------------------------------------
// Change "return x" to "(funcname) = x" to match old Verilog return style.

void MtCursor::emit_return_statement(MtNode n) {
  auto func_name = current_function_name;
  for (auto c : n) switch (c.sym) {
  case anon_sym_return: emit_replacement(c, "%s =", func_name.c_str()); break;
  default: emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit_primitive_type(MtNode n) {
  emit(n);
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit_type_identifier(MtNode n) {
  emit(n);
}

//------------------------------------------------------------------------------
// For some reason the class's trailing semicolon ends up with the template decl, so we prune it here.

void MtCursor::emit_template_declaration(MtNode n) {
  for (auto c : n) switch (c.sym) {
  case anon_sym_template: skip_over(c); skip_space(); break;
  case anon_sym_SEMI: skip_over(c); break;
  default: emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// Replace foo.bar.baz with foo_bar_baz, so that a field expression instead
// refers to a glue expression.

void MtCursor::emit_flat_field_expression(MtNode n) {
  auto field = n.body();
  for (auto& c : field) if (c == '.') c = '_';
  emit_replacement(n, field.c_str());
}

void MtCursor::emit_case(MtNode n) {
  /*
  [3] s247 case_statement:
  |   [0] s87 lit: "case"
  |   [1] f34 s112 value.number_literal: "0b110"
  |   [2] s83 lit: ":"
  |   [3] s225 compound_statement:
  |   |   [0] s59 lit: "{"
  |   |   [1] s252 break_statement:
  |   |   |   [0] s93 lit: "break"
  |   |   |   [1] s39 lit: ";"
  |   |   [2] s60 lit: "}"
  */

  /*
  [4] s247 case_statement:
  |   |   |   [0] s87 lit: "case"
  |   |   |   [1] f34 s112 value.number_literal: "0b001"
  |   |   |   [2] s83 lit: ":"
  */

  /*
  [9] s247 case_statement:
  |   [0] s88 lit: "default"
  |   [1] s83 lit: ":"
  |---[2] s225 compound_statement:
  |   |   [0] s59 lit: "{"
  |---|---[1] s244 expression_statement:
  |   |   |   [0] s258 assignment_expression:
  |   |   |   |   [0] f19 s1 left.identifier: "illegal_instr_o"
  |   |   |   |   [1] f23 s63 operator.lit: "="
  |   |   |   |   [2] f29 s112 right.number_literal: "0b1"
  |   |   |   [1] s39 lit: ";"
  |   |   [2] s60 lit: "}"
  */

  auto tag = n.child(0);

  if (tag.sym != anon_sym_default && n.child_count() == 3) {
    skip_over(n.child(0));
    skip_space();
    cursor = n.child(1).start();
    emit_dispatch(n.child(1));
    emit(",");
    cursor = n.end();
    return;
  }

  for (auto c : n) {
    if (c.sym == anon_sym_case) {
      skip_over(c);
      skip_space();
    }
    else emit_dispatch(c);
  }
}

void MtCursor::emit_switch(MtNode n) {
  //n.dump_tree();

  for (auto c : n) {
    if (c.sym == anon_sym_switch) {
      emit_replacement(c, "case");
    }
    else if (c.field == field_body) {
      for (auto gc : c) {
        if (gc.sym == anon_sym_LBRACE) skip_over(gc);
        else if (gc.sym == anon_sym_RBRACE) emit_replacement(gc, "endcase");
        else emit_dispatch(gc);
      }

    }
    else {
      emit_dispatch(c);
    }
  }
}

//------------------------------------------------------------------------------

/*
========== tree dump begin
[0] s236 field_declaration:
|   [0] f32 s230 type.enum_specifier:
|   |   [0] s79 lit: "enum"
|   |   [1] f22 s395 name.type_identifier: "opcode_e"
|   [1] f11 s272 default_value.initializer_list:
|   |   [0] s59 lit: "{"
|   |   [1] s258 assignment_expression:
|   |   |   [0] f19 s1 left.identifier: "OPCODE_LOAD"
|   |   |   [1] f23 s63 operator.lit: "="
|   |   |   [2] f29 s112 right.number_literal: "0x03"
|   |   [2] s7 lit: ","
|   |   [3] s60 lit: "}"
|   [2] s39 lit: ";"
========== tree dump end

========== tree dump begin
[0] s236 field_declaration:
|   [0] f32 s230 type.enum_specifier:
|   |   [0] s79 lit: "enum"
|   |   [1] f5 s231 body.enumerator_list:
|   |   |   [0] s59 lit: "{"
|   |   |   [1] s238 enumerator:
|   |   |   |   [0] f22 s1 name.identifier: "OPCODE_LOAD"
|   |   |   [2] s7 lit: ","
|   |   |   [3] s60 lit: "}"
|   [1] s39 lit: ";"
========== tree dump end
*/

void MtCursor::emit_dispatch(MtNode n) {
  assert(cursor <= n.start());

  switch (n.sym) {

  case sym_storage_class_specifier:
    n.match("static") ? emit_replacement(n, "localparam") : comment_out(n);
    break;

  case sym_break_statement:
    //emit_replacement(n, "/*break;*/");
    comment_out(n);
    break;

  case sym_access_specifier:
  case sym_type_qualifier:
  case sym_preproc_call:
  case sym_preproc_if:
  case sym_template_parameter_list:
    skip_over(n);
    skip_space();
    break;

  case sym_enum_specifier:
    for (auto c : n) emit_dispatch(c);
    break;

  case sym_parameter_list:
  case sym_if_statement:
  case sym_for_statement:
  case sym_parenthesized_expression:
  case sym_parameter_declaration:
  case sym_optional_parameter_declaration:
  case sym_condition_clause:
  case sym_unary_expression:
  case sym_subscript_expression:
  case sym_enumerator:
  case sym_type_definition:
  case sym_expression_statement:
  case sym_declaration:
  case sym_binary_expression:
  case sym_argument_list:
  case sym_array_declarator:
  case sym_type_descriptor:
  case sym_function_declarator:
  case sym_init_declarator:
  case sym_initializer_list:
    for (auto c : n) emit_dispatch(c);
    break;

  case sym_number_literal:         emit_number_literal(n); break;
  case sym_field_expression:       emit_flat_field_expression(n); break;
  case sym_return_statement:       emit_return_statement(n); break;
  case sym_template_declaration:   emit_template_declaration(n); break;
  case sym_preproc_include:        emit_preproc_include(n);      break;
  case sym_field_declaration:      emit_field_declaration(n);  break;
  case sym_compound_statement:     emit_compound_statement(n); break;
  case sym_template_type:          emit_template_type(n);      break;
  case sym_translation_unit:       emit_translation_unit(n); break;
  case sym_primitive_type:         emit_primitive_type(n); break;
  case alias_sym_type_identifier:  emit_type_identifier(n); break;
  case sym_class_specifier:        emit_class_specifier(n); break;
  case sym_struct_specifier:       emit_class_specifier(n); break;
  case sym_function_definition:    emit_function_definition(n); break;
  case sym_call_expression:        emit_call_expression(n); break;
  case sym_assignment_expression:  emit_assignment_expression(n); break;
  case sym_template_argument_list: emit_template_argument_list(n); break;
  case sym_enumerator_list:        emit_enumerator_list(n); break;
  case sym_case_statement:         emit_case(n); break;
  case sym_switch_statement:       emit_switch(n); break;

  default:
    emit(n);
    break;
  }
}

//------------------------------------------------------------------------------
