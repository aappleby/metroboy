#include "MtCursor.h"

#include "Platform.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtNode.h"
#include <stdarg.h>
#include "MtSourceFile.h"

void print_escaped(char s);

//------------------------------------------------------------------------------

MtCursor::MtCursor(MtModLibrary* lib, MtSourceFile* source_file, std::string* out) : lib(lib), str_out(out) {
  spacer_stack.push_back("\n");
  cursor = source_file->source;
}

//------------------------------------------------------------------------------

void MtCursor::push_indent(MtNode body) {
  auto n = body.first_named_child();
  assert(body.sym == sym_compound_statement || body.sym == sym_field_declaration_list);

  if (n) {
    auto begin = n.start() - 1;
    auto end = n.start();

    while (*begin != '\n' && *begin != '{') begin--;
    if (*begin == '{') begin++;

    std::string spacer(begin + 1, end);
    for (auto c : spacer) assert(isspace(c));

    spacer_stack.push_back("\n" + spacer);
  }
  else {
    spacer_stack.push_back("\n");
  }
}

void MtCursor::pop_indent(MtNode class_body) {
  spacer_stack.pop_back();
}

void MtCursor::emit_newline() {
  for (auto c : spacer_stack.back()) emit_char(c);
}

//------------------------------------------------------------------------------

void MtCursor::dump_node_line(MtNode n) {
  auto start = &(source_file->source[n.start_byte()]);

  auto a = start;
  auto b = start;
  while (a > source_file->source     && *a != '\n' && *a != '\r') a--;
  while (b < source_file->source_end && *b != '\n' && *b != '\r') b++;

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

  n.error();

  emit("halting...\n");
  emit("########################################\n");
  debugbreak();
}

//------------------------------------------------------------------------------
// Generic emit() methods

void MtCursor::emit_char(char c) {
  putchar(c);
  str_out->push_back(c);
}

void MtCursor::emit_span(const char* a, const char* b) {
  assert(a != b);
  assert(cursor >= source_file->source);
  assert(cursor <= source_file->source_end);
  for (auto c = a; c < b; c++) emit_char(*c);
}

//----------

void MtCursor::emit_text(MtNode n) {
  assert(cursor == n.start());
  emit_span(n.start(), n.end());
  cursor = n.end();
}

//----------

void MtCursor::emit(const char* fmt, ...) {
  printf("\u001b[38;2;128;255;128m");
  //emit_char('{');

  va_list args;
  va_start(args, fmt);
  int size = vsnprintf(nullptr, 0, fmt, args);
  va_end(args);

  auto buf = new char[size + 1];

  va_start(args, fmt);
  vsnprintf(buf, size + 1, fmt, args);
  va_end(args);

  for (int i = 0; i < size; i++) emit_char(buf[i]);
  delete buf;

  printf("\u001b[0m");
  //emit_char('}');
}

//----------

void MtCursor::emit_replacement(MtNode n, const char* fmt, ...) {
  printf("\u001b[38;2;255;255;128m");
  //emit_char('{');

  assert(cursor == n.start());
  advance_to(n);
  cursor = n.end();

  va_list args;
  va_start(args, fmt);
  int size = vsnprintf(nullptr, 0, fmt, args);
  va_end(args);

  auto buf = new char[size + 1];

  va_start(args, fmt);
  vsnprintf(buf, size + 1, fmt, args);
  va_end(args);

  for (int i = 0; i < size; i++) emit_char(buf[i]);
  delete buf;

  printf("\u001b[0m");
  //emit_char('}');
}

//----------

void MtCursor::skip_over(MtNode n) {
  assert(cursor == n.start());
  cursor = n.end();
}

void MtCursor::skip_to_next_sibling(MtNode n) {
  assert(!n.is_null());
  auto next_node = ts_node_next_sibling(n.node);
  auto a = &source_file->source[ts_node_start_byte(next_node)];
  auto b = &source_file->source[ts_node_end_byte(next_node)];
  while (a < b && isspace(a[0])) a++;
  cursor = a;
}

void MtCursor::skip_space() {
  while(*cursor && (*cursor == ' ')) {
    cursor++;
  }
}

void MtCursor::advance_to(MtNode n) {
  assert(cursor <= n.start());
  if (cursor < n.start()) {
    emit_span(cursor, n.start());
    cursor = n.start();
  }
}

void MtCursor::comment_out(MtNode n) {
  assert(cursor == n.start());
  emit("/*");
  emit_text(n);
  emit("*/");
}

//------------------------------------------------------------------------------
// Replace "#include" with "`include" and ".h" with ".sv"

void MtCursor::emit(MtPreprocInclude n) {
  advance_to(n.child(0));
  emit_replacement(n.child(0), "`include");
  advance_to(n.path_node());
  emit_span(n.path_node().start(), n.path_node().end() - 1);
  emit(".sv\"");
  cursor = n.end();
}

//------------------------------------------------------------------------------
// Change '=' to '<=' if lhs is a field and we're inside a sequential block.

// sym_assignment_expression := { left: identifier, operator: lit, right : expr }

void MtCursor::emit(MtAssignmentExpr n) {
  bool lhs_is_field = false;
  if (n.lhs().sym == sym_identifier) {
    std::string lhs_name = n.lhs().text();
    for (auto& f : current_mod->fields) {
      if (f.name == lhs_name) {
        lhs_is_field = true;
        break;
      }
    }
  }

  emit_dispatch(n.lhs());
  advance_to(n.op());
  if (in_tick && lhs_is_field) emit("<");
  emit_text(n.op());
  emit_dispatch(n.rhs());
}

//------------------------------------------------------------------------------

void MtCursor::emit_static_bit_extract(MtCallExpr call, int bx_width) {
  advance_to(call);

  int arg_count = call.args().named_child_count();

  auto arg0 = call.args().named_child(0);
  auto arg1 = call.args().named_child(1);

  if (arg_count == 1) {
  
    if (arg0.sym == sym_number_literal) {
      // Explicitly sized literal - 8'd10

      cursor = arg0.start();
      emit(MtNumberLiteral(arg0), bx_width);
      cursor = call.end();
    }
    else if (arg0.sym == sym_identifier || arg0.sym == sym_subscript_expression) {
      // Size-casting expression
      cursor = arg0.start();
      emit("%d'", bx_width);
      emit("(");
      emit_dispatch(arg0);
      emit(")");
      cursor = call.end();
    }
    else {
      // Size-casting expression
      cursor = arg0.start();
      emit("%d'", bx_width);
      emit("(");
      emit_dispatch(arg0);
      emit(")");
      cursor = call.end();
    }
  }
  else if (arg_count == 2) {
    // Slicing logic array - foo[7:2]

    if (arg1.sym == sym_number_literal)
    {
      // Slice at offset
      if (bx_width == 1) {
        emit_replacement(call, "%s[%s]", arg0.text().c_str(), arg1.text().c_str());
      }
      else {
        int offset = atoi(arg1.start());
        emit_replacement(call, "%s[%d:%d]", arg0.text().c_str(), bx_width - 1 + offset, offset);
      }
    }
    else
    {
      if (bx_width == 1) {
        emit_replacement(call, "%s[%s]", arg0.text().c_str(), arg1.text().c_str());;
      }
      else {
        emit_replacement(call, "%s[%d + %s : %s]",
          arg0.text().c_str(),
          bx_width - 1,
          arg1.text().c_str(),
          arg1.text().c_str());
      }
    }

  }
  else {
    debugbreak();
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_dynamic_bit_extract(MtCallExpr call, MtNode bx_node) {
  advance_to(call);

  int arg_count = call.args().named_child_count();

  auto arg0 = call.args().named_child(0);
  auto arg1 = call.args().named_child(1);

  if (arg_count == 1) {
    // Non-literal size-casting expression - bits'(expression)
    cursor = bx_node.start();
    emit_dispatch(bx_node);
    emit("'(");
    cursor = arg0.start();
    emit_dispatch(arg0);
    emit(")");
    cursor = call.end();
  }
  else if (arg_count == 2) {
    // Non-literal slice expression - expression[bits+offset-1:offset];

    cursor = arg0.start();
    emit_dispatch(arg0);

    if (arg1.sym != sym_number_literal) debugbreak();
    int offset = atoi(arg1.start());

    emit("[%s+%d:%d]", bx_node.text().c_str(), offset - 1, offset);
    cursor = call.end();
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

void MtCursor::emit(MtCallExpr call) {
  MtFunc func = call.func();
  MtArgList args = call.args();

  // If we're calling a member function, look at the name of the member
  // function and not the whole foo.bar().

  std::string func_name = func.name();

  if (func_name == "coerce") {
    // Convert to cast? We probably shouldn't be calling coerce() directly.
    call.error();
  }
  else if (func_name == "signed") {
    advance_to(func);
    emit_replacement(func, "$signed");
    emit(args);
  }
  else if (func_name == "unsigned") {
    advance_to(func);
    emit_replacement(func, "$unsigned");
    emit(args);
  }
  else if (func_name == "clog2") {
    advance_to(func);
    emit_replacement(func, "$clog2");
    emit(args);
  }
  else if (func_name == "pow2") {
    advance_to(func);
    emit_replacement(func, "2**");
    emit(args);
  }
  else if (func_name == "readmemh") {
    advance_to(func);
    emit_replacement(func, "$readmemh");
    emit(args);
  }
  else if (func_name == "printf") {
    advance_to(func);
    emit_replacement(func, "$write");
    emit(args);
  }
  else if (func_name.starts_with("init")) {
    comment_out(call);
  }
  else if (func_name.starts_with("final")) {
    comment_out(call);
  }
  else if (func_name.starts_with("tick")) {
    comment_out(call);
  }
  else if (func_name.starts_with("tock")) {
    comment_out(call);
  }
  else if (func_name == "bx") {
    // Bit extract.
    auto template_arg = func.as_templ().args().named_child(0);
    //emit_static_bit_extract(n, atoi(template_arg.start()));
    emit_dynamic_bit_extract(call, template_arg);
  }
  else if (func_name == "b1")  emit_static_bit_extract(call, 1);
  else if (func_name == "b2")  emit_static_bit_extract(call, 2);
  else if (func_name == "b3")  emit_static_bit_extract(call, 3);
  else if (func_name == "b4")  emit_static_bit_extract(call, 4);
  else if (func_name == "b5")  emit_static_bit_extract(call, 5);
  else if (func_name == "b6")  emit_static_bit_extract(call, 6);
  else if (func_name == "b7")  emit_static_bit_extract(call, 7);
  else if (func_name == "b8")  emit_static_bit_extract(call, 8);
  else if (func_name == "b9")  emit_static_bit_extract(call, 9);

  else if (func_name == "b10") emit_static_bit_extract(call, 10);
  else if (func_name == "b11") emit_static_bit_extract(call, 11);
  else if (func_name == "b12") emit_static_bit_extract(call, 12);
  else if (func_name == "b13") emit_static_bit_extract(call, 13);
  else if (func_name == "b14") emit_static_bit_extract(call, 14);
  else if (func_name == "b15") emit_static_bit_extract(call, 15);
  else if (func_name == "b16") emit_static_bit_extract(call, 16);
  else if (func_name == "b17") emit_static_bit_extract(call, 17);
  else if (func_name == "b18") emit_static_bit_extract(call, 18);
  else if (func_name == "b19") emit_static_bit_extract(call, 19);

  else if (func_name == "b20") emit_static_bit_extract(call, 20);
  else if (func_name == "b21") emit_static_bit_extract(call, 21);
  else if (func_name == "b22") emit_static_bit_extract(call, 22);
  else if (func_name == "b23") emit_static_bit_extract(call, 23);
  else if (func_name == "b24") emit_static_bit_extract(call, 24);
  else if (func_name == "b25") emit_static_bit_extract(call, 25);
  else if (func_name == "b26") emit_static_bit_extract(call, 26);
  else if (func_name == "b27") emit_static_bit_extract(call, 27);
  else if (func_name == "b28") emit_static_bit_extract(call, 28);
  else if (func_name == "b29") emit_static_bit_extract(call, 29);

  else if (func_name == "b30") emit_static_bit_extract(call, 30);
  else if (func_name == "b31") emit_static_bit_extract(call, 31);
  else if (func_name == "b32") emit_static_bit_extract(call, 32);
  else if (func_name == "b33") emit_static_bit_extract(call, 33);
  else if (func_name == "b34") emit_static_bit_extract(call, 34);
  else if (func_name == "b35") emit_static_bit_extract(call, 35);
  else if (func_name == "b36") emit_static_bit_extract(call, 36);
  else if (func_name == "b37") emit_static_bit_extract(call, 37);
  else if (func_name == "b38") emit_static_bit_extract(call, 38);
  else if (func_name == "b39") emit_static_bit_extract(call, 39);

  else if (func_name == "b40") emit_static_bit_extract(call, 40);
  else if (func_name == "b41") emit_static_bit_extract(call, 41);
  else if (func_name == "b42") emit_static_bit_extract(call, 42);
  else if (func_name == "b43") emit_static_bit_extract(call, 43);
  else if (func_name == "b44") emit_static_bit_extract(call, 44);
  else if (func_name == "b45") emit_static_bit_extract(call, 45);
  else if (func_name == "b46") emit_static_bit_extract(call, 46);
  else if (func_name == "b47") emit_static_bit_extract(call, 47);
  else if (func_name == "b48") emit_static_bit_extract(call, 48);
  else if (func_name == "b49") emit_static_bit_extract(call, 49);

  else if (func_name == "b50") emit_static_bit_extract(call, 50);
  else if (func_name == "b51") emit_static_bit_extract(call, 51);
  else if (func_name == "b52") emit_static_bit_extract(call, 52);
  else if (func_name == "b53") emit_static_bit_extract(call, 53);
  else if (func_name == "b54") emit_static_bit_extract(call, 54);
  else if (func_name == "b55") emit_static_bit_extract(call, 55);
  else if (func_name == "b56") emit_static_bit_extract(call, 56);
  else if (func_name == "b57") emit_static_bit_extract(call, 57);
  else if (func_name == "b58") emit_static_bit_extract(call, 58);
  else if (func_name == "b59") emit_static_bit_extract(call, 59);

  else if (func_name == "b60") emit_static_bit_extract(call, 60);
  else if (func_name == "b61") emit_static_bit_extract(call, 61);
  else if (func_name == "b62") emit_static_bit_extract(call, 62);
  else if (func_name == "b63") emit_static_bit_extract(call, 63);
  else if (func_name == "b64") emit_static_bit_extract(call, 64);

  else if (func_name == "cat") {
    // Remove "cat" and replace parens with brackets
    advance_to(func);
    skip_over(func);
    for (const auto& arg : (MtNode&)args) switch (arg.sym) {
    case anon_sym_LPAREN: {
      advance_to(arg);
      emit_replacement(arg, "{"); break;
    }
    case anon_sym_RPAREN: {
      advance_to(arg);
      emit_replacement(arg, "}"); break;
    }
    default: emit_dispatch(arg); break;
    }
  }
  else if (func_name == "dup") {
    // Convert "dup<15>(b12(instr_i))" to "15 {instr_i[12]}}"

    assert(args.named_child_count() == 1);

    advance_to(func);
    skip_over(func);

    auto template_arg = func.as_templ().args().named_child(0);
    int dup_count = atoi(template_arg.start());
    emit("{%d ", dup_count);
    emit("{");

    auto func_arg = args.named_child(0);
    cursor = func_arg.start();
    emit_dispatch(func_arg);

    emit("}}");
    cursor = call.end();
  }
  else {
    // All other function/task calls go through normally.
    for (auto c : (MtNode&)call) emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "logic blah;"

void MtCursor::emit_init_declarator_as_decl(MtDecl n) {

  if (n.is_init_decl()) {
    emit(n.type());
    emit(n._init_decl().decl());
    emit(";"); // FIXME where did the semi go?
    cursor = n._init_decl().end();
  }
  else {
    emit_dispatch(n);
  }

}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "blah = x;"

void MtCursor::emit_init_declarator_as_assign(MtDecl n) {
  bool is_localparam =
    n.sym == sym_declaration &&
    n.child_count() >= 4 &&
    n.child(0).text() == "static" &&
    n.child(1).text() == "const";

  if (is_localparam) {
    emit_dispatch(n);
    return;
  }

  if (n.is_init_decl()) {
    advance_to(n._type());
    skip_to_next_sibling(n._type());
    emit_dispatch(n._init_decl());
  }
  else {
    advance_to(n);
    skip_to_next_sibling(n);
  }
}

//------------------------------------------------------------------------------
// Emit local variable declarations at the top of the block scope.

void MtCursor::emit_hoisted_decls(MtCompoundStatement n) {
  MtCursor old_cursor = *this;
  for (auto c : (MtNode&)n) {
    if (c.sym == sym_declaration) {
      bool is_localparam =
        c.sym == sym_declaration &&
        c.child_count() >= 4 &&
        c.child(0).text() == "static" &&
        c.child(1).text() == "const";

      if (is_localparam) {
      }
      else {
        cursor = c.start();
        emit_newline();
        emit_init_declarator_as_decl(MtDecl(c));
      }
    }
  }
  *this = old_cursor;
}

//------------------------------------------------------------------------------

void MtCursor::emit_glue_assignment(MtCallExpr call) {
  auto call_this = call.func().get_field(field_argument);
  auto func_name = call.func().get_field(field_field);

  for (auto& submod : current_mod->submods) {
    if (submod.name == call_this.node_to_name()) {
      std::vector<std::string> call_src;
      std::vector<std::string> call_dst;

      for (auto arg : (MtNode)call.args()) {
        if (!arg.is_named()) continue;
        auto src = arg.node_to_name();
        for (auto& c : src) if (c == '.') c = '_';
        call_src.push_back(src);
      }

      for (auto& input : submod.mod->inputs) {
        call_dst.push_back(input.node_to_name());
      }

      assert(call_src.size() == call_dst.size());

      for (int i = 0; i < call_src.size(); i++) {
            
        emit_newline();
        emit("assign %s_%s = %s;",
          submod.name.c_str(),
          call_dst[i].c_str(),
          call_src[i].c_str());
      }
    }
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_glue_assignments(MtFuncDefinition func_def) {
  emit_newline();

  std::vector<MtNode> submod_call_nodes;

  func_def.body().visit_tree([&](MtNode child) {
    if (child.sym == sym_call_expression) {
      auto call = MtCallExpr(child);

      if (call.func().sym == sym_field_expression) {
        auto call_args = child.get_field(field_arguments);
        auto call_this = call.func().get_field(field_argument);
        auto func_name = call.func().get_field(field_field);

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
    emit_glue_assignment(submod_call);
  }
}

//------------------------------------------------------------------------------
// Emit the module body with the correct type of "begin/end" pair,
// hoisting locals to the top of the body scope.

void MtCursor::emit_function_body(MtCompoundStatement func_body) {
  push_indent(func_body);

  for (auto it = begin(func_body); it != end(func_body); ++it) {
    auto c = *it;

  //for (auto c : (MtNode&)func_body) {
    switch (c.sym) {
      case anon_sym_LBRACE:
        advance_to(c);
        if      (in_init) emit_replacement(c, "begin : %s", current_function_name.c_str());
        else if (in_tick) emit_replacement(c, "begin : %s", current_function_name.c_str());
        else if (in_tock) emit_replacement(c, "begin : %s", current_function_name.c_str());
        else if (in_task) emit_replacement(c, "");
        else if (in_func) emit_replacement(c, "");
        else              debugbreak();

        emit_hoisted_decls(func_body);
        break;

      case sym_declaration:
        emit_init_declarator_as_assign(c);
        break;

      case sym_expression_statement:
        if (c.child(0).sym == sym_call_expression && in_tick) {
          c.dump_tree();
          advance_to(c);
          //emit("CALL EXPRESSION ");
          comment_out(c);
          //emit_glue_assignment(c.child(0));
        }
        else {
          emit_dispatch(c);
        }
        break;

      case anon_sym_RBRACE:
        advance_to(c);
        if      (in_init) emit_replacement(c, "end");
        else if (in_tick) emit_replacement(c, "end");
        else if (in_tock) emit_replacement(c, "end");
        else if (in_task) emit_replacement(c, "endtask");
        else if (in_func) emit_replacement(c, "endfunction");
        else              debugbreak();
        break;

      default: emit_dispatch(c); break;
    }
  }

  pop_indent(func_body);
}

//------------------------------------------------------------------------------
// Change "init/tick/tock" to "initial begin / always_comb / always_ff", change
// void methods to tasks, and change const methods to funcs.

// func_def = { field_type, field_declarator, field_body }

void MtCursor::emit(MtFuncDefinition func) {
  advance_to(func);
  skip_over(func.type());
  skip_space();

  current_function_name = func.decl().decl().node_to_name();
  in_task = func.type().match("void");
  in_func = !in_task;
  in_init = in_task && current_function_name.starts_with("init");
  in_tick = in_task && current_function_name.starts_with("tick");
  in_tock = in_task && current_function_name.starts_with("tock");

  //----------

  if (in_init) {
    emit_replacement(func.decl(), "initial");
  }
  else if (in_tick) {
    emit_replacement(func.decl(), "always_ff @(posedge clk, negedge rst_n)");
  }
  else if (in_tock) {
    emit_replacement(func.decl(), "always_comb");
  }
  else if (in_task) {
    advance_to(func.decl());
    emit("task ");
    emit_dispatch(func.decl());
    skip_space();
    emit(";");
  }
  else if (in_func) {
    advance_to(func.decl());
    emit("function %s ", func.type().text().c_str());
    emit_dispatch(func.decl());
    skip_space();
    emit(";");
  }
  else {
    debugbreak();
  }

  emit_function_body(func.body());

  //----------
  // For each call to {submodule}.tick() in module::tick(), emit glue assignments.

  // FIXME needs to go in its own always_comb otherwise we get dupes

  /*
  if (in_tick && !mod->submodules.empty()) {
    emit_glue_assignments(func);
  }
  */

  //----------

  current_function_name = "";
  in_init = false;
  in_tick = false;
  in_tock = false;
  in_task = false;
  in_func = false;
}

//------------------------------------------------------------------------------
// Emit "<type> <submod_name>_<output_name>;" glue declarations because we can't
// directly pass arguments to submodules.

/*
void MtCursor::emit_glue_declaration(MtField f, const std::string& prefix) {
  cursor = f.foo_type.start();

  auto decl = f.get_field(field_declarator);

  emit_dispatch(f.foo_type);
  advance_to(decl);
  emit("%s_", prefix.c_str());
  emit_dispatch(decl);
  emit(";");
  emit_newline();
}
*/

//------------------------------------------------------------------------------
// TreeSitterCPP support for enums is SUPER BROKEN and produces different parse
// trees in different contexts. :/
// So, we're going to dig the info we need out of it but it may be flaky.

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

void MtCursor::emit_field_decl_as_enum_class(MtFieldDecl field_decl) {
  assert(field_decl.sym == sym_field_declaration);

  std::string enum_name;
  MtNode node_values;
  int bit_width = 0;
  std::string enum_type = "";

  if (field_decl.sym == sym_enum_specifier &&
      field_decl.get_field(field_body).sym == sym_enumerator_list) {
    auto node_name = field_decl.get_field(field_name);
    auto node_base = field_decl.get_field(field_base);
    enum_name   = node_name.is_null() ? "" : node_name.text();
    enum_type   = node_base.is_null() ? "" : node_base.text();
    node_values = field_decl.get_field(field_body);
    bit_width = 0;
  }
  else if (field_decl.sym == sym_field_declaration &&
           field_decl.get_field(field_type).sym == sym_enum_specifier &&
           field_decl.get_field(field_type).get_field(field_body).sym == sym_enumerator_list) {
    // Anonymous enums have "body" nested under "type"
    node_values = field_decl.get_field(field_type).get_field(field_body);
    bit_width = 0;
  }
  else if (field_decl.sym == sym_field_declaration &&
      field_decl.get_field(field_type).sym == sym_enum_specifier &&
      field_decl.get_field(field_default_value).sym == sym_initializer_list) {
    // TreeSitterCPP BUG - "enum class foo : int = {}" misinterpreted as default_value

    enum_name   = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = field_decl.get_field(field_default_value);
    bit_width   = 0;
  }
  else if (field_decl.sym == sym_field_declaration &&
      field_decl.child_count() == 3 &&
      field_decl.child(0).sym == sym_enum_specifier &&
      field_decl.child(1).sym == sym_bitfield_clause) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" misinterpreted as bitfield
    auto node_bitfield = field_decl.child(1);
    auto node_compound = node_bitfield.child(1);
    auto node_basetype = node_compound.get_field(field_type);
    auto node_scope    = node_basetype.get_field(field_scope);
    auto node_args     = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name   = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = node_compound.get_field(field_value);
    bit_width   = atoi(node_bitwidth.start());
  }
  else if (field_decl.sym == sym_declaration &&
      field_decl.child_count() == 3 &&
      field_decl.child(0).sym == sym_enum_specifier &&
      field_decl.child(1).sym == sym_init_declarator) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" in namespace misinterpreted as declarator
    auto node_decl1    = field_decl.get_field(field_declarator);
    auto node_decl2    = node_decl1.get_field(field_declarator);
    auto node_scope    = node_decl2.get_field(field_scope);
    auto node_args     = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name   = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = node_decl1.get_field(field_value);
    bit_width   = atoi(node_bitwidth.start());
  }
  else {
    field_decl.error();
  }

  advance_to(field_decl);
  emit("typedef enum ");
  if (bit_width == 1) {
    emit("logic ", bit_width - 1);
  }
  else if (bit_width > 1) {
    emit("logic[%d:0] ", bit_width - 1);
  }
  else if (enum_type.size()) {
    if (enum_type == "int") emit("integer ");
  }
  else {
    //emit("integer ");
  }

  override_size = bit_width;
  cursor = node_values.start();
  emit_dispatch(node_values);
  if (enum_name.size()) emit(" %s", enum_name.c_str());
  override_size = 0;
  cursor = field_decl.end();

  // BUG: Trailing semicolons are inconsistent.
  if (field_decl.text().back() == ';') emit(";");
}

//------------------------------------------------------------------------------
// If this field field_decl is a submodule, emit glue parameters and patch the glue
// parameter list into the submodule declaration.

/*
void MtCursor::emit_glue_declarations(MtFieldDecl decl) {

  auto type_name = decl.get_field(field_type).node_to_type();

  MtField field(decl,
                decl.get_field(field_type),
                type_name,
                decl.get_field(field_declarator).text());

  auto submod = mod->lib->find_mod(type_name);

  advance_to(decl);
  std::string inst_name = decl.node_to_name();

  if (submod->modparams.size()) {
    auto templ_args = decl.get_field(field_type).get_field(field_arguments);

    for (int i = 0; i < templ_args.named_child_count(); i++) {
      auto param_name = submod->modparams[i].node_to_name();
      id_replacements[param_name] = templ_args.named_child(i).text();
    }
  }

  for (auto& input : submod->inputs) {
    MtCursor sub_cursor(submod, str_out);
    sub_cursor.spacer_stack = spacer_stack;
    sub_cursor.id_replacements = id_replacements;
    sub_cursor.emit_glue_declaration(input, inst_name);
  }

  for (auto& output : submod->outputs) {
    MtCursor sub_cursor(submod, str_out);
    sub_cursor.spacer_stack = spacer_stack;
    sub_cursor.id_replacements = id_replacements;
    sub_cursor.emit_glue_declaration(output, inst_name);
  }

  id_replacements.clear();
}
*/

//------------------------------------------------------------------------------
// Emit submodule port list:
// 
// my_module foo(clk, rst_n, foo_i_bar, foo_i_baz);
//              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


void MtCursor::emit_submodule_port_list(MtFieldDecl field_decl) {

  auto field_type = field_decl.get_field(::field_type);
  auto field_name = field_decl.get_field(field_declarator);

  std::string type_name = field_type.node_to_type();

  auto submod = lib->find_mod(type_name);

  {
    std::string inst_name = field_decl.node_to_name();

    for (auto c : MtNode(field_decl)) {
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
  }
}

//------------------------------------------------------------------------------
// Emit field declarations. For submodules, also emit glue declarations and
// append the glue parameter list to the field.

// field_declaration = { type:type_identifier, declarator:field_identifier+ }
// field_declaration = { type:template_type,   declarator:field_identifier+ }
// field_declaration = { type:enum_specifier,  bitfield_clause (TREESITTER BUG) }

void MtCursor::emit(MtFieldDecl field_decl) {
  // Handle "enum class", which is broken a bit in TreeSitterCpp
  if (field_decl.type().child_count() >= 3 &&
      field_decl.type().child(0).text() == "enum" &&
      field_decl.type().child(1).text() == "class" && 
      field_decl.type().child(2).sym == alias_sym_type_identifier) {
    emit_field_decl_as_enum_class(field_decl);
    return;
  }

  // If this isn't a submodule, just tack on "input" and "output" annotations.
  std::string type_name = field_decl.type().node_to_type();
  auto submod = lib->find_mod(type_name);

  if (submod) {
    // Input and output fields go in the port list, not in the module body.
    if (!in_ports && (field_decl.is_input() || field_decl.is_output())) {
      skip_over(field_decl);
      return;
    }

    if (field_decl.type().is_enum()) {
      emit_field_decl_as_enum_class(field_decl);
      return;
    }

    // Check if this field is a submodule by looking up its type name in our
    // module list.

    //emit_glue_declarations(field_decl);
    emit_submodule_port_list(field_decl);
    emit_newline();
  }
  else {
    if (!in_ports && field_decl.is_output()) {
      advance_to(field_decl);
      skip_to_next_sibling(field_decl);
    }
    else {
      for (auto c : (MtNode&)(field_decl)) {
        emit_dispatch(c);
      }
    }

  }


}

//------------------------------------------------------------------------------

void MtCursor::emit_decl_no_semi(MtNode n) {
  advance_to(n);
  for (auto c : n) {
    if (c.sym == anon_sym_SEMI) {
      skip_over(c);
    }
    else {
      emit_dispatch(c);
    }
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_input_ports(std::vector<MtField>& fields) {
  for (auto& input : fields) {
    MtCursor sub_cursor = *this;
    emit_newline();
    emit("input ");
    //sub_cursor.cursor = input.type.start();
    //sub_cursor.emit_dispatch(input.type);
    //emit(" ");
    //sub_cursor.cursor = input.name.start();
    //sub_cursor.emit_dispatch(input.name);
    sub_cursor.cursor = input.start();
    //sub_cursor.emit_dispatch(input.decl);
    sub_cursor.emit_decl_no_semi(input);
    emit(",");
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_output_ports(std::vector<MtField>& fields) {
  for (int i = 0; i < fields.size(); i++)  {
    auto& output = fields[i];
    MtCursor sub_cursor = *this;
    emit_newline();
    emit("output ");
    //sub_cursor.cursor = output.type.start();
    //sub_cursor.emit_dispatch(output.type);
    //emit(" ");
    //sub_cursor.cursor = output.name.start();
    //sub_cursor.emit_dispatch(output.name);
    sub_cursor.cursor = output.start();
    //sub_cursor.emit_dispatch(output.decl);
    sub_cursor.emit_decl_no_semi(output);

    if (i != fields.size() - 1) emit(",");
  }
}

//------------------------------------------------------------------------------
/*
input logic clk,
input logic rst_n,

output logic o_serial,
output logic[7:0] o_data,
output logic o_valid,
output logic o_done,
output logic[31:0] o_sum
*/

void MtCursor::emit_port_list() {
  // Emit an old-style port list
  in_ports = true;
  trim_namespaces = false;

  emit_newline();
  emit("("); emit_newline();
  emit("input logic clk,"); emit_newline();
  emit("input logic rst_n,"); emit_newline();

  emit_input_ports(current_mod->inputs);
  emit_output_ports(current_mod->outputs);

  emit_newline();
  emit(");");

  trim_namespaces = true;
  in_ports = false;
}

//------------------------------------------------------------------------------
// Emit the module body, with a few modifications.
// Discard the opening brace
// Replace the closing brace with "endmodule"
// Discard the seimcolon at the end of class{};"

void MtCursor::emit_sym_field_declaration_list(MtFieldDeclList class_body) {
  assert(class_body.sym == sym_field_declaration_list);

  push_indent(class_body);

  for (auto c : (MtNode&)class_body) switch (c.sym) {
  case anon_sym_LBRACE: {
    advance_to(c);
    emit_replacement(c, "");
    emit_newline();
    //emit("// Submod outputs go here");
    //emit_newline();
    //for (auto& submod : current_mod->submods) {
    //  emit(submod.node_to_name().c_str());
    //  emit_newline();
    //}

    break;
  }
  case anon_sym_RBRACE: {
    advance_to(c);
    emit_replacement(c, "endmodule");
    break;
  }
  case anon_sym_SEMI: {
    advance_to(c);
    emit_replacement(c, "");
    break;
  }
  default: {
    emit_dispatch(c);
    break;
  }
  }

  pop_indent(class_body);
}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void MtCursor::emit(MtStructSpecifier n) {
  // FIXME enter mod here

  auto struct_lit  = n.child(0);
  auto struct_name = n.get_field(field_name);
  auto struct_body = n.get_field(field_body);

  /*
  if (in_module_or_package) {
    // Don't turn nested structs into modules.
    advance_to(n);
    emit("typedef struct packed");
    cursor = struct_name.end();
    emit_dispatch(struct_body);

    cursor = struct_name.start();
    emit(" ");
    emit_text(struct_name);
    cursor = n.end();
    return;
  }
  */

  //----------

  if (!in_module_or_package) {
    assert(!current_mod);
    for (auto& mod : source_file->modules) {
      if (mod->mod_name == struct_name.text()) {
        current_mod = mod;
        break;
      }
    }
    assert(current_mod);
  }

  in_module_or_package++;

  advance_to(struct_lit);
  emit_replacement(struct_lit, "module");

  advance_to(struct_name);
  emit_dispatch(struct_name);

  // Patch the template parameter list in after the module declaration, before
  // the port list.
  if (current_mod->mod_param_list) {
    emit_newline();
    MtCursor sub_cursor = *this;
    sub_cursor.cursor = current_mod->mod_param_list.start();
    sub_cursor.emit(current_mod->mod_param_list);
  }

  emit_port_list();
  emit_sym_field_declaration_list(MtFieldDeclList(struct_body));

  cursor = n.end();

  in_module_or_package--;

  if (!in_module_or_package) {
    current_mod = nullptr;
  }
}

//------------------------------------------------------------------------------
// Change "{ blah(); foo(); int x = 1; }" to "begin blah(); ... end"

void MtCursor::emit(MtCompoundStatement body) {
  push_indent(body);

  for (auto c : (MtNode&)body) switch (c.sym) {
  case anon_sym_LBRACE:
    advance_to(c);
    emit_replacement(c, "begin");
    emit_hoisted_decls(body);
    break;
  case sym_declaration: emit_init_declarator_as_assign(c); break;
  case anon_sym_RBRACE: {
    advance_to(c);
    emit_replacement(c, "end");
    break;
  }
  default: emit_dispatch(c); break;
  }

  pop_indent(body);
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void MtCursor::emit(MtTemplateType templ_type) {
  bool is_logic = templ_type.name().match("logic");

  if (!is_logic) {
    emit(templ_type.name());
    emit(templ_type.args());
    return;
  }

  emit(templ_type.name());
  /*
  for (auto c : (MtNode)templ_type.args()) switch (c.sym) {
    case anon_sym_LT: skip_over(c); break;
    case anon_sym_GT: skip_over(c); break;
    case sym_number_literal: {
      int width = atoi(c.start());
      if (width > 1) emit("[%d:0]", width - 1);
      skip_over(c);
      break;
    }
    default:
      emit("[");
      emit_dispatch(c);
      emit("-1:0]");
      break;
  }
  */

  auto args = templ_type.args();
  advance_to(args);

  auto logic_size = args.first_named_child();
  switch(logic_size.sym) {
    case sym_number_literal: {
      int width = atoi(logic_size.start());
      if (width > 1) emit_replacement(args, "[%d:0]", width - 1);
      cursor = args.end();
      break;
    }
    default:
      emit("[");
      cursor = logic_size.start();
      emit_dispatch(logic_size);
      emit("-1:0]");
      break;
  }
  cursor = templ_type.end();
}

//------------------------------------------------------------------------------
// Change (template)<int param, int param> to
// #(parameter int param, parameter int param)

void MtCursor::emit(MtTemplateParamList n) {
  for (auto c : (MtNode&)n) switch (c.sym) {
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

void MtCursor::emit(MtTemplateArgList n) {
  for (auto c : (MtNode&)n) switch (c.sym) {
  case anon_sym_LT: emit_replacement(c, " #("); break;
  case anon_sym_GT: emit_replacement(c, ")"); break;
  default:          emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void MtCursor::emit(MtEnumeratorList n) {
  for (auto c : (MtNode&)n) switch (c.sym) {
    case anon_sym_LBRACE: emit_text(c); break;
    case anon_sym_RBRACE: emit_text(c); break;
    default: emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void MtCursor::emit(MtTranslationUnit n) {
  emit("`default_nettype none\n");

  for (auto c : (MtNode&)n) switch (c.sym) {
    case anon_sym_SEMI: skip_over(c); break;
    default:            emit_dispatch(c); break;
  }

  if (cursor < source_file->source_end) {
    emit_span(cursor, source_file->source_end);
  }
}

//------------------------------------------------------------------------------
// Replace "0x" prefixes with "'h"
// Replace "0b" prefixes with "'b"
// Add an explicit size prefix if needed.

void MtCursor::emit(MtNumberLiteral n, int size_cast) {
  advance_to(n);

  assert(!override_size || !size_cast);
  if (override_size) size_cast = override_size;

  std::string body = n.text();

  // Count how many 's are in the number
  int spacer_count = 0;
  int prefix_count = 0;
  
  for(auto& c : body) if (c == '\'') {
    c = '_';
    spacer_count++;
  }
  
  if (body.starts_with("0x")) {
    prefix_count = 2;
    if (!size_cast) size_cast = ((int)body.size() - 2 - spacer_count) * 4;
    emit("%d'h", size_cast);
  }
  else if (body.starts_with("0b")) {
    prefix_count = 2;
    if (!size_cast) size_cast = (int)body.size() - 2 - spacer_count;
    emit("%d'b", size_cast);
  }
  else {
    if (size_cast) emit("%d'd", size_cast);
  }

  if (spacer_count) {
    emit(body.c_str() + prefix_count);
  }
  else {
    emit_span(n.start() + prefix_count, n.end());
  }

  cursor = n.end();
}

//------------------------------------------------------------------------------
// Change "return x" to "(funcname) = x" to match old Verilog return style.

void MtCursor::emit(MtReturnStatement n) {
  auto func_name = current_function_name;
  for (auto c : (MtNode&)n) switch (c.sym) {
    case anon_sym_return: emit_replacement(c, "%s =", func_name.c_str()); break;
    default: emit_dispatch(c); break;
  }
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit(MtPrimitiveType n) {
  advance_to(n);
  emit_text(n);
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit(MtIdentifier n) {
  auto name = n.node_to_name();
  auto it = id_replacements.find(name);
  if (it != id_replacements.end()) {
    emit_replacement(n, it->second.c_str());
  }
  else {
    advance_to(n);
    emit_text(n);
  }
}

void MtCursor::emit(MtTypeIdentifier n) {
  auto name = n.node_to_name();
  auto it = id_replacements.find(name);
  if (it != id_replacements.end()) {
    emit_replacement(n, it->second.c_str());
  }
  else {
    advance_to(n);
    emit_text(n);
  }
}

//------------------------------------------------------------------------------
// For some reason the class's trailing semicolon ends up with the template field_decl, so we prune it here.

void MtCursor::emit(MtTemplateDecl n) {
  auto struct_specifier = MtStructSpecifier(n.child(2));
  std::string struct_name = struct_specifier.get_field(field_name).text();

  if (!in_module_or_package) {
    assert(!current_mod);
    for (auto& mod : source_file->modules) {
      if (mod->mod_name == struct_name) {
        current_mod = mod;
        break;
      }
    }
    assert(current_mod);
  }

  in_module_or_package++;

  advance_to(n);
  cursor = struct_specifier.start();
  emit(struct_specifier);
  cursor = n.end();

  in_module_or_package--;

  if (!in_module_or_package) {
    current_mod = nullptr;
  }
}

//------------------------------------------------------------------------------
// Replace foo.bar.baz with foo_bar_baz, so that a field expression instead
// refers to a glue expression.

void MtCursor::emit(MtFieldExpr n) {
  auto field = n.text();
  for (auto& c : field) if (c == '.') c = '_';
  advance_to(n);
  emit_replacement(n, field.c_str());
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtCaseStatement n) {
  auto tag = n.child(0);

  /*
  if (tag.sym != anon_sym_default && n.child_count() == 3) {
    debugbreak();
    emit_replacement(n.child(0), "/ * * /");
    //skip_over(n.child(0));
    //skip_space();
    cursor = n.child(1).start();
    emit_dispatch(n.child(1));
    emit(",");
    cursor = n.end();
    return;
  }
*/

  for (auto c : (MtNode&)n) {
    if (c.sym == anon_sym_case) {
      advance_to(c);
      //emit_replacement(c, "/**/");
      skip_to_next_sibling(c);
    }
    else emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtSwitchStatement n) {
  for (auto c : (MtNode&)n) {
    if (c.sym == anon_sym_switch) {
      advance_to(c);
      emit_replacement(c, "case");
    }
    else if (c.field == field_body) {
      for (auto gc : c) {
        if (gc.sym == anon_sym_LBRACE) {
          advance_to(gc);
          skip_over(gc);
        }
        else if (gc.sym == anon_sym_RBRACE) {
          advance_to(gc);
          emit_replacement(gc, "endcase");
        }
        else emit_dispatch(gc);
      }

    }
    else {
      emit_dispatch(c);
    }
  }
}

//------------------------------------------------------------------------------
// Unwrap magic /*#foo#*/ comments to pass arbitrary text to Verilog.

void MtCursor::emit(MtComment n) {
  auto body = n.text();
  if (body.starts_with("/*#")) {
    body.erase(body.size() - 3, 3);
    body.erase(0, 3);
    emit_replacement(n, body.c_str());
  }
  else {
    advance_to(n);
    emit_text(n);
  }
}

//------------------------------------------------------------------------------
// Verilog doesn't use "break"

void MtCursor::emit(MtBreakStatement n) {
  advance_to(n);
  emit_replacement(n, "/*-*/;");
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtFieldDeclList n) {
  for (auto c : (MtNode&)n) {
    emit_dispatch(c);
  }
  cursor = n.end();
}

//------------------------------------------------------------------------------
// TreeSitter nodes slightly broken for "a = b ? c : d;"...

void MtCursor::emit(MtCondExpr n) {
  for (auto c : (MtNode&)n) {
    emit_dispatch(c);
  }
  cursor = n.end();
}

//------------------------------------------------------------------------------
// Static variables become localparams at module level.

void MtCursor::emit(MtStorageSpec n) {
  advance_to(n);
  n.match("static") ? emit_replacement(n, "localparam") : comment_out(n);
}

//------------------------------------------------------------------------------
// ...er, this was something about namespace resolution?

void MtCursor::emit(MtQualifiedId n) {
  if (trim_namespaces) {
    auto last_child = n.child(n.child_count() - 1);
    advance_to(n);
    cursor = last_child.start();
    emit_dispatch(last_child);
    cursor = n.end();
  } else {
    for (auto c : (MtNode&)n) emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------
// If statements are basically the same.

void MtCursor::emit(MtIfStatement n) {
  for (auto c : (MtNode&)n) {
    emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------
// Enums are broken.

void MtCursor::emit(MtEnumSpecifier n) {
  //emit_sym_field_declaration_as_enum_class(MtFieldDecl(n));
  //for (auto c : n) emit_dispatch(c);
  debugbreak();
}

//------------------------------------------------------------------------------
// FIXME - are we using this anywhere?

void MtCursor::emit(MtUsingDecl n) {
  auto name = n.child(2).text();
  emit_replacement(n, "import %s::*;", name.c_str());
}

//------------------------------------------------------------------------------
// FIXME - When do we hit this? It doesn't look finished.

void MtCursor::emit(MtDecl n) {

  // Check for enum classes, which are broken.
  auto node_type = n.get_field(field_type);
  if (node_type.child_count() >= 2 &&
      node_type.child(0).text() == "enum" &&
      node_type.child(1).text() == "class") {
    debugbreak();
    //emit_field_decl_as_enum_class(MtFieldDecl(n));
    return;
  }

  if (n.child_count() >= 5 &&
      n.child(0).text() == "static" &&
      n.child(1).text() == "const") {

    advance_to(n);
    emit("parameter ");
    cursor = n.child(2).start();
    emit_dispatch(n.child(2));
    emit_dispatch(n.child(3));
    emit_dispatch(n.child(4));

    cursor = n.end();
    return;
  }

  // Regular boring local variable declaration?
  for(auto c : (MtNode)n) emit_dispatch(c);
}

//------------------------------------------------------------------------------
// "unsigned int" -> "int unsigned"

void MtCursor::emit(MtSizedTypeSpec n) {
  assert(n.child_count() == 2);
  advance_to(n);

  cursor = n.child(1).start();
  emit_dispatch(n.child(1));
    
  emit_span(n.child(0).end(), n.child(1).start());

  cursor = n.child(0).start();
  emit_dispatch(n.child(0));

  cursor = n.end();
}

//------------------------------------------------------------------------------
// FIXME - Do we have a test case for namespaces?

void MtCursor::emit(MtNamespaceDef n) {
  in_module_or_package++;
  auto node_name = n.get_field(field_name);
  auto node_body = n.get_field(field_body);
  advance_to(n);
  emit("package %s;", node_name.text().c_str());
  cursor = node_body.start();
    
  for (auto c : node_body) {
    if      (c.sym == anon_sym_LBRACE) emit_replacement(c, "");
    else if (c.sym == anon_sym_RBRACE) emit_replacement(c, "");
    else                               emit_dispatch(c);
  }
    
  emit("endpackage");
  emit_newline();
  cursor = n.end();
  in_module_or_package++;
}

//------------------------------------------------------------------------------
// Arg lists are the same in C and Verilog.

void MtCursor::emit(MtArgList n) {
  for (auto c : (MtNode&)n) {
    emit_dispatch(c);
  }
}

//------------------------------------------------------------------------------
// Call the correct emit() method based on the node type.

void MtCursor::emit_dispatch(MtNode n) {
  assert(cursor <= n.start());

  switch (n.sym) {

  case sym_preproc_def: {
    auto lit = n.child(0);
    auto name = n.get_field(field_name);
    auto value = n.get_field(field_value);

    advance_to(lit);
    emit_replacement(lit, "`define");
    emit_dispatch(name);
    if (!value.is_null()) emit_dispatch(value);
    break;
  }

  case sym_preproc_ifdef: {
    /*
    auto child0 = n.child(0);
    auto text = child0.text();
    advance_to(child0);
    if (text == "#ifdef")  emit_replacement(child0, "`ifdef");
    else if (text == "#ifndef") emit_replacement(child0, "`ifndef");
    //if (text == "#define") emit_replacement(child0, "`define");
    else if (text == "#endif")  emit_replacement(child0, "`endif");
    else debugbreak();
    cursor = child0.end();
    for (int i = 1; i < n.child_count(); i++) {
      emit_dispatch(n.child(i));
    }
    */
    for (auto c : n) {
      emit_dispatch(c);
    }
    break;
  }

  //case aux_sym_preproc_if_token2:

  case sym_preproc_else: {
    for (auto c : n) {
      emit_dispatch(c);
    }
    break;
  }

  case sym_preproc_arg: {
    advance_to(n);
    emit_text(n);
    break;
  }

  case sym_preproc_call:
  case sym_access_specifier:
  case sym_type_qualifier:
  case sym_preproc_if:
    advance_to(n);
    skip_to_next_sibling(n);
    break;

  case sym_parameter_list:
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
  case sym_binary_expression:
  case sym_array_declarator:
  case sym_type_descriptor:
  case sym_function_declarator:
  case sym_init_declarator:
  case sym_initializer_list:
  case sym_declaration_list:
    for (auto c : n) {
      emit_dispatch(c);
    }
    break;

  case sym_argument_list:           emit(MtArgList(n));           break;
  case sym_enum_specifier:          emit(MtEnumSpecifier(n));     break;
  case sym_if_statement:            emit(MtIfStatement(n));       break;
  case sym_qualified_identifier:    emit(MtQualifiedId(n));       break;
  case sym_storage_class_specifier: emit(MtStorageSpec(n));       break;
  case sym_conditional_expression:  emit(MtCondExpr(n));          break;
  case sym_field_declaration_list:  emit(MtFieldDeclList(n));     break;
  case sym_break_statement:         emit(MtBreakStatement(n));    break;
  case sym_identifier:              emit(MtIdentifier(n));        break;
  case sym_class_specifier:         emit(MtStructSpecifier(n));   break;
  case sym_struct_specifier:        emit(MtStructSpecifier(n));   break;
  case sym_number_literal:          emit(MtNumberLiteral(n));     break;
  case sym_field_expression:        emit(MtFieldExpr(n));         break;
  case sym_return_statement:        emit(MtReturnStatement(n));   break;
  case sym_template_declaration:    emit(MtTemplateDecl(n));      break;
  case sym_preproc_include:         emit(MtPreprocInclude(n));    break;
  case sym_field_declaration:       emit(MtFieldDecl(n));         break;
  case sym_compound_statement:      emit(MtCompoundStatement(n)); break;
  case sym_template_type:           emit(MtTemplateType(n));      break;
  case sym_translation_unit:        emit(MtTranslationUnit(n));   break;
  case sym_primitive_type:          emit(MtPrimitiveType(n));     break;
  case alias_sym_type_identifier:   emit(MtTypeIdentifier(n));    break;
  case sym_function_definition:     emit(MtFuncDefinition(n));    break;
  case sym_call_expression:         emit(MtCallExpr(n));          break;
  case sym_assignment_expression:   emit(MtAssignmentExpr(n));    break;
  case sym_template_argument_list:  emit(MtTemplateArgList(n));   break;
  case sym_comment:                 emit(MtComment(n));           break;
  case sym_enumerator_list:         emit(MtEnumeratorList(n));    break;
  case sym_case_statement:          emit(MtCaseStatement(n));     break;
  case sym_switch_statement:        emit(MtSwitchStatement(n));   break;
  case sym_using_declaration:       emit(MtUsingDecl(n));         break;
  case sym_sized_type_specifier:    emit(MtSizedTypeSpec(n));     break;
  case sym_declaration:             emit(MtDecl(n));              break;
  case sym_namespace_definition:    emit(MtNamespaceDef(n));      break;

  default:
    static std::set<int> passthru_syms = {
      alias_sym_namespace_identifier,
      alias_sym_field_identifier,
      sym_sized_type_specifier,
      sym_string_literal
    };

    if (!n.is_named()) {
      advance_to(n);
      auto text = n.text();
      if      (text == "#ifdef")  emit_replacement(n, "`ifdef");
      else if (text == "#ifndef") emit_replacement(n, "`ifndef");
      else if (text == "#else")   emit_replacement(n, "`else");
      else if (text == "#endif")  emit_replacement(n, "`endif");
      else {
        // FIXME TreeSitter bug - we get a anon_sym_SEMI with no text in alu_control.h
        // FIXME TreeSitter - #ifdefs in if/else trees break things
        if (n.start() != n.end()) emit_text(n);
      }
    }

    else if (passthru_syms.contains(n.sym)) {
      advance_to(n);
      emit_text(n);
    } else {
      printf("Don't know what to do with %d %s\n", n.sym, n.type());
      n.error();
    }
    break;
  }
}

//------------------------------------------------------------------------------
