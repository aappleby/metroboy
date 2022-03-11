#include "MtCursor.h"

#include <stdarg.h>

#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtNode.h"
#include "MtSourceFile.h"
#include "Platform.h"

void print_escaped(char s);

//------------------------------------------------------------------------------

MtCursor::MtCursor(MtModLibrary* lib, MtSourceFile* source_file,
                   std::string* out)
    : lib(lib), source_file(source_file), str_out(out) {
  indent_stack.push_back("");
  cursor = source_file->source;
}

//------------------------------------------------------------------------------

void MtCursor::push_indent(MtNode body) {
  assert(body.sym == sym_compound_statement ||
         body.sym == sym_field_declaration_list);

  auto n = body.first_named_child().node;
  if (ts_node_is_null(n)) {
    indent_stack.push_back("");
    return;
  }

  if (ts_node_symbol(n) == sym_access_specifier) {
    n = ts_node_next_sibling(n);
  }
  const char* begin = &source_file->source[ts_node_start_byte(n)] - 1;
  const char* end = &source_file->source[ts_node_start_byte(n)];

  std::string indent;

  while (*begin != '\n' && *begin != '{') begin--;
  if (*begin == '{') {
    // begin++;
    // indent = std::string(begin + 1, end);
    indent = "";
  } else {
    indent = std::string(begin + 1, end);
  }

  for (auto& c : indent) {
    assert(isspace(c));
    // c = '#';
  }

  indent_stack.push_back(indent);
}

void MtCursor::pop_indent(MtNode class_body) { indent_stack.pop_back(); }

void MtCursor::push_indent_of(MtNode n) {
  if (n) {
    auto begin = n.start() - 1;
    auto end = n.start();

    while (*begin != '\n' && *begin != '{') begin--;
    if (*begin == '{') begin++;

    std::string indent(begin + 1, end);
    for (auto c : indent) assert(isspace(c));

    indent_stack.push_back(indent);
  } else {
    indent_stack.push_back("");
  }
}

void MtCursor::pop_indent_of(MtNode class_body) { indent_stack.pop_back(); }

void MtCursor::emit_newline() { emit_char('\n'); }

void MtCursor::emit_indent() {
  for (auto c : indent_stack.back()) emit_char(c);
}

//------------------------------------------------------------------------------

void MtCursor::dump_node_line(MtNode n) {
  auto start = &(source_file->source[n.start_byte()]);

  auto a = start;
  auto b = start;
  while (a > source_file->source && *a != '\n' && *a != '\r') a--;
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
  if (c < 0 || !isspace(c)) {
    line_dirty = true;
  }

  if (c == '\n') {
    // putchar('}');
    /*
    if (!line_dirty) {
      putchar('?');
    }
    */

    line_dirty = false;
    line_elided = false;
  }

  if (!quiet) putchar(c);
  str_out->push_back(c);

  // if (c == '\n') putchar('{');
}

//----------------------------------------

void MtCursor::emit_ws() {
  while (cursor < source_file->source_end && isspace(*cursor)) {
    emit_char(*cursor++);
  }
}

//----------------------------------------

void MtCursor::emit_span(const char* a, const char* b) {
  assert(a != b);
  assert(cursor >= source_file->source);
  assert(cursor <= source_file->source_end);
  for (auto c = a; c < b; c++) emit_char(*c);
}

//----------------------------------------

void MtCursor::emit_text(MtNode n) {
  assert(cursor == n.start());
  emit_span(n.start(), n.end());
  cursor = n.end();
}

//----------------------------------------

void MtCursor::emit(const char* fmt, ...) {
  if (!quiet) printf("\u001b[38;2;128;255;128m");
  // emit_char('{');

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

  if (!quiet) printf("\u001b[0m");
  // emit_char('}');
}

//----------------------------------------

void MtCursor::emit_replacement(MtNode n, const char* fmt, ...) {
  assert(cursor == n.start());

  if (!quiet) printf("\u001b[38;2;255;255;128m");

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

  if (!quiet) printf("\u001b[0m");
}

//----------------------------------------

void MtCursor::skip_over(MtNode n) {
  assert(cursor == n.start());

  /*
  if (!quiet) {
    printf("\u001b[38;2;255;128;128m");

    for (auto c = n.start(); c < n.end(); c++) {
      if (*c == ' ') putchar('#');
      else putchar(*c);
    }

    printf("\u001b[0m");
  }
  */

  cursor = n.end();
}

//----------------------------------------

void MtCursor::skip_to_next_sibling(MtNode n) {
  assert(cursor == n.start());

  auto next_node = ts_node_next_sibling(n.node);
  auto a = &source_file->source[ts_node_start_byte(next_node)];

  /*
  if (!quiet) {
    printf("\u001b[38;2;255;128;128m");

    for (auto c = cursor; c < a; c++) {
      if (*c == ' ') putchar('#');
      else putchar(*c);
    }

    printf("\u001b[0m");
  }
  */

  cursor = a;
}

void MtCursor::skip_to_next_line() {
  // if (!quiet) printf("\u001b[38;2;255;128;128m");

  while (*cursor != '\n') {
    // if (!quiet) {
    //  if (*cursor == ' ') putchar('#');
    //  else putchar(*cursor);
    //}
    cursor++;
  }
  cursor++;

  // if (!quiet) printf("\u001b[0m");
}

void MtCursor::emit_to_newline() {
  while (*cursor != '\n') emit_char(*cursor++);
  emit_char(*cursor++);
}

//----------------------------------------

void MtCursor::skip_ws() {
  // if (!quiet) printf("\u001b[38;2;255;128;128m");

  while (*cursor && isspace(*cursor)) {
    /*
    if (!quiet) {
      if (*cursor == ' ') putchar('#');
      else putchar(*cursor);
    }
    */
    cursor++;
  }

  // if (!quiet) printf("\u001b[0m");
}

//----------------------------------------

void MtCursor::comment_out(MtNode n) {
  assert(cursor == n.start());
  emit("/*");
  emit_text(n);
  emit("*/");
}

//------------------------------------------------------------------------------
// Replace "#include" with "`include" and ".h" with ".sv"

void MtCursor::emit(MtPreprocInclude n) {
  assert(cursor == n.start());

  emit_replacement(n.child(0), "`include");
  emit_ws();
  emit_span(n.path_node().start(), n.path_node().end() - 1);
  emit(".sv\"");
  cursor = n.end();
}

//------------------------------------------------------------------------------
// Change '=' to '<=' if lhs is a field and we're inside a sequential block.

// sym_assignment_expression := { left: identifier, operator: lit, right : expr
// }

void MtCursor::emit(MtAssignmentExpr n) {
  assert(cursor == n.start());

  auto lhs = n.lhs();
  auto rhs = n.rhs();

  bool lhs_is_field = false;
  if (lhs.sym == sym_identifier) {
    std::string lhs_name = lhs.text();
    for (auto& f : current_mod->fields) {
      if (f.name == lhs_name) {
        lhs_is_field = true;
        break;
      }
    }
  }

  if (lhs.sym == sym_identifier) {
    emit(MtIdentifier(lhs));
  }
  else if (lhs.sym == sym_subscript_expression) {
    emit_children(lhs);
  }
  else {
    lhs.dump_tree();
    debugbreak();
  }
  emit_ws();

  if (in_tick && lhs_is_field) emit("<");
  emit_text(n.op());

  // Emit_dispatch makes sense here, as we really could have anything on the
  // rhs.
  emit_ws();
  emit_dispatch(rhs);
}

//------------------------------------------------------------------------------

void MtCursor::emit_static_bit_extract(MtCallExpr call, int bx_width) {
  assert(cursor == call.start());

  int arg_count = call.args().named_child_count();

  auto arg0 = call.args().named_child(0);
  auto arg1 = call.args().named_child(1);

  if (arg_count == 1) {
    if (arg0.sym == sym_number_literal) {
      // Explicitly sized literal - 8'd10

      cursor = arg0.start();
      emit(MtNumberLiteral(arg0), bx_width);
      cursor = call.end();
    } else if (arg0.sym == sym_identifier ||
               arg0.sym == sym_subscript_expression) {
      if (arg0.text() == "DONTCARE") {
        // Size-casting expression
        emit("%d'", bx_width);
        emit("x");
        cursor = call.end();
      } else {
        // Size-casting expression
        cursor = arg0.start();
        emit("%d'", bx_width);
        emit("(");
        emit_dispatch(arg0);
        emit(")");
        cursor = call.end();
      }

    } else {
      // Size-casting expression
      cursor = arg0.start();
      emit("%d'", bx_width);
      emit("(");
      emit_dispatch(arg0);
      emit(")");
      cursor = call.end();
    }
  } else if (arg_count == 2) {
    // Slicing logic array - foo[7:2]

    if (arg1.sym == sym_number_literal) {
      // Slice at offset
      if (bx_width == 1) {
        emit_replacement(call, "%s[%s]", arg0.text().c_str(),
                         arg1.text().c_str());
      } else {
        int offset = atoi(arg1.start());
        emit_replacement(call, "%s[%d:%d]", arg0.text().c_str(),
                         bx_width - 1 + offset, offset);
      }
    } else {
      if (bx_width == 1) {
        emit_replacement(call, "%s[%s]", arg0.text().c_str(),
                         arg1.text().c_str());
        ;
      } else {
        emit_replacement(call, "%s[%d + %s : %s]", arg0.text().c_str(),
                         bx_width - 1, arg1.text().c_str(),
                         arg1.text().c_str());
      }
    }

  } else {
    debugbreak();
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_dynamic_bit_extract(MtCallExpr call, MtNode bx_node) {
  assert(cursor == call.start());

  int arg_count = call.args().named_child_count();

  auto arg0 = call.args().named_child(0);
  auto arg1 = call.args().named_child(1);

  if (arg_count == 1) {
    // Non-literal size-casting expression - bits'(expression)
    if (arg0.text() == "DONTCARE") {
      cursor = bx_node.start();
      emit_dispatch(bx_node);
      emit("'(1'bx)");
      cursor = call.end();
    }
    else {
      cursor = bx_node.start();
      emit_dispatch(bx_node);
      emit("'(");
      cursor = arg0.start();
      emit_dispatch(arg0);
      emit(")");
      cursor = call.end();
    }

  } else if (arg_count == 2) {
    // Non-literal slice expression - expression[bits+offset-1:offset];

    cursor = arg0.start();
    emit_dispatch(arg0);

    if (arg1.sym != sym_number_literal) debugbreak();
    int offset = atoi(arg1.start());

    emit("[%s+%d:%d]", bx_node.text().c_str(), offset - 1, offset);
    cursor = call.end();
  } else {
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
  assert(cursor == call.start());

  MtFunc func = call.func();
  MtArgList args = call.args();

  // If we're calling a member function, look at the name of the member
  // function and not the whole foo.bar().

  std::string func_name = func.name();

  if (func_name == "coerce") {
    // Convert to cast? We probably shouldn't be calling coerce() directly.
    call.error();
  } else if (func_name == "signed") {
    emit_replacement(func, "$signed");
    emit(args);
  } else if (func_name == "unsigned") {
    emit_replacement(func, "$unsigned");
    emit(args);
  } else if (func_name == "clog2") {
    emit_replacement(func, "$clog2");
    emit(args);
  } else if (func_name == "pow2") {
    emit_replacement(func, "2**");
    emit(args);
  } else if (func_name == "readmemh") {
    emit_replacement(func, "$readmemh");
    emit(args);
  } else if (func_name == "write") {
    emit_replacement(func, "$write");
    emit(args);
  } else if (func_name.starts_with("init")) {
    comment_out(call);
  } else if (func_name.starts_with("final")) {
    comment_out(call);
  } else if (func_name.starts_with("tick")) {
    comment_out(call);
  } else if (func_name.starts_with("tock")) {
    comment_out(call);
  } else if (func_name == "bx") {
    // Bit extract.
    auto template_arg = func.as_templ().args().named_child(0);
    // emit_static_bit_extract(n, atoi(template_arg.start()));
    emit_dynamic_bit_extract(call, template_arg);
  } else if (func_name == "b1")
    emit_static_bit_extract(call, 1);
  else if (func_name == "b2")
    emit_static_bit_extract(call, 2);
  else if (func_name == "b3")
    emit_static_bit_extract(call, 3);
  else if (func_name == "b4")
    emit_static_bit_extract(call, 4);
  else if (func_name == "b5")
    emit_static_bit_extract(call, 5);
  else if (func_name == "b6")
    emit_static_bit_extract(call, 6);
  else if (func_name == "b7")
    emit_static_bit_extract(call, 7);
  else if (func_name == "b8")
    emit_static_bit_extract(call, 8);
  else if (func_name == "b9")
    emit_static_bit_extract(call, 9);

  else if (func_name == "b10")
    emit_static_bit_extract(call, 10);
  else if (func_name == "b11")
    emit_static_bit_extract(call, 11);
  else if (func_name == "b12")
    emit_static_bit_extract(call, 12);
  else if (func_name == "b13")
    emit_static_bit_extract(call, 13);
  else if (func_name == "b14")
    emit_static_bit_extract(call, 14);
  else if (func_name == "b15")
    emit_static_bit_extract(call, 15);
  else if (func_name == "b16")
    emit_static_bit_extract(call, 16);
  else if (func_name == "b17")
    emit_static_bit_extract(call, 17);
  else if (func_name == "b18")
    emit_static_bit_extract(call, 18);
  else if (func_name == "b19")
    emit_static_bit_extract(call, 19);

  else if (func_name == "b20")
    emit_static_bit_extract(call, 20);
  else if (func_name == "b21")
    emit_static_bit_extract(call, 21);
  else if (func_name == "b22")
    emit_static_bit_extract(call, 22);
  else if (func_name == "b23")
    emit_static_bit_extract(call, 23);
  else if (func_name == "b24")
    emit_static_bit_extract(call, 24);
  else if (func_name == "b25")
    emit_static_bit_extract(call, 25);
  else if (func_name == "b26")
    emit_static_bit_extract(call, 26);
  else if (func_name == "b27")
    emit_static_bit_extract(call, 27);
  else if (func_name == "b28")
    emit_static_bit_extract(call, 28);
  else if (func_name == "b29")
    emit_static_bit_extract(call, 29);

  else if (func_name == "b30")
    emit_static_bit_extract(call, 30);
  else if (func_name == "b31")
    emit_static_bit_extract(call, 31);
  else if (func_name == "b32")
    emit_static_bit_extract(call, 32);
  else if (func_name == "b33")
    emit_static_bit_extract(call, 33);
  else if (func_name == "b34")
    emit_static_bit_extract(call, 34);
  else if (func_name == "b35")
    emit_static_bit_extract(call, 35);
  else if (func_name == "b36")
    emit_static_bit_extract(call, 36);
  else if (func_name == "b37")
    emit_static_bit_extract(call, 37);
  else if (func_name == "b38")
    emit_static_bit_extract(call, 38);
  else if (func_name == "b39")
    emit_static_bit_extract(call, 39);

  else if (func_name == "b40")
    emit_static_bit_extract(call, 40);
  else if (func_name == "b41")
    emit_static_bit_extract(call, 41);
  else if (func_name == "b42")
    emit_static_bit_extract(call, 42);
  else if (func_name == "b43")
    emit_static_bit_extract(call, 43);
  else if (func_name == "b44")
    emit_static_bit_extract(call, 44);
  else if (func_name == "b45")
    emit_static_bit_extract(call, 45);
  else if (func_name == "b46")
    emit_static_bit_extract(call, 46);
  else if (func_name == "b47")
    emit_static_bit_extract(call, 47);
  else if (func_name == "b48")
    emit_static_bit_extract(call, 48);
  else if (func_name == "b49")
    emit_static_bit_extract(call, 49);

  else if (func_name == "b50")
    emit_static_bit_extract(call, 50);
  else if (func_name == "b51")
    emit_static_bit_extract(call, 51);
  else if (func_name == "b52")
    emit_static_bit_extract(call, 52);
  else if (func_name == "b53")
    emit_static_bit_extract(call, 53);
  else if (func_name == "b54")
    emit_static_bit_extract(call, 54);
  else if (func_name == "b55")
    emit_static_bit_extract(call, 55);
  else if (func_name == "b56")
    emit_static_bit_extract(call, 56);
  else if (func_name == "b57")
    emit_static_bit_extract(call, 57);
  else if (func_name == "b58")
    emit_static_bit_extract(call, 58);
  else if (func_name == "b59")
    emit_static_bit_extract(call, 59);

  else if (func_name == "b60")
    emit_static_bit_extract(call, 60);
  else if (func_name == "b61")
    emit_static_bit_extract(call, 61);
  else if (func_name == "b62")
    emit_static_bit_extract(call, 62);
  else if (func_name == "b63")
    emit_static_bit_extract(call, 63);
  else if (func_name == "b64")
    emit_static_bit_extract(call, 64);

  else if (func_name == "cat") {
    // Remove "cat" and replace parens with brackets
    skip_over(func);
    for (const auto& arg : (MtNode&)args) {
      switch (arg.sym) {
        case anon_sym_LPAREN: {
          emit_replacement(arg, "{");
          break;
        }
        case anon_sym_RPAREN: {
          emit_replacement(arg, "}");
          break;
        }
        default:
          emit_dispatch(arg);
          break;
      }
      emit_ws();
    }
  } else if (func_name == "dup") {
    // Convert "dup<15>(b12(instr_i))" to "15 {instr_i[12]}}"

    assert(args.named_child_count() == 1);

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
  } else {
    // All other function/task calls go through normally.
    emit_children(call);
  }
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "logic blah;"

void MtCursor::emit_init_declarator_as_decl(MtDecl n) {
  assert(cursor == n.start());

  // if (!n.is_init_decl()) {
  //  n.dump_tree();
  //}
  // assert(n.is_init_decl());

  emit(n._type());
  emit_ws();
  emit(n._init_decl().decl());
  emit(";");
  cursor = n.end();
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "blah = x;"

void MtCursor::emit_init_declarator_as_assign(MtDecl n) {
  assert(cursor == n.start());

  assert(n.is_init_decl());
  cursor = n._init_decl().start();
  emit_dispatch(n._init_decl());
  emit(";");
  cursor = n.end();
}

//------------------------------------------------------------------------------
// Emit local variable declarations at the top of the block scope.

void MtCursor::emit_hoisted_decls(MtCompoundStatement n) {
  MtCursor old_cursor = *this;
  for (auto c : (MtNode&)n) {
    if (c.sym == sym_declaration) {
      bool is_localparam = c.sym == sym_declaration && c.child_count() >= 4 &&
                           c.child(0).text() == "static" &&
                           c.child(1).text() == "const";

      if (is_localparam) {
      } else {
        cursor = c.start();

        auto d = MtDecl(c);

        emit_indent();
        if (d.is_init_decl()) {
          emit_init_declarator_as_decl(MtDecl(c));
        } else {
          emit_dispatch(d);
        }
        emit_newline();
      }
    }
  }
  *this = old_cursor;
}

//------------------------------------------------------------------------------

void MtCursor::emit_func_decl(MtFuncDeclarator n) {
  assert(cursor == n.start());

  emit(MtFieldIdentifier(n.get_field(field_declarator)));
  emit(MtParameterList(n.get_field(field_parameters)));
}

//------------------------------------------------------------------------------
// Change "init/tick/tock" to "initial begin / always_comb / always_ff", change
// void methods to tasks, and change const methods to funcs.

// func_def = { field_type, field_declarator, field_body }

void MtCursor::emit(MtFuncDefinition func) {
  assert(cursor == func.start());

  skip_over(func.type());
  skip_ws();

  current_function_name = func.decl().decl().node_to_name();
  in_task = func.type().match("void");
  in_func = !in_task;
  in_init = in_task && current_function_name.starts_with("init");
  in_tick = in_task && current_function_name.starts_with("tick");
  in_tock = in_task && current_function_name.starts_with("tock");

  //----------
  // Emit a block declaration for the type of function we're in.

  if (in_init) {
    emit_replacement(func.decl(), "initial");
    emit_ws();
  } else if (in_tick) {
    emit_replacement(func.decl(), "always_ff @(posedge clk)");
    emit_ws();
  } else if (in_tock) {
    emit_replacement(func.decl(), "always_comb");
    emit_ws();
  } else if (in_task) {
    emit("task ");
    emit_dispatch(func.decl());
    emit(";");
    skip_ws();
  } else if (in_func) {
    emit("function %s ", func.type().text().c_str());
    emit_dispatch(func.decl());
    emit(";");
    skip_ws();
  } else {
    debugbreak();
  }

  if (in_init)
    emit("begin : %s", current_function_name.c_str());
  else if (in_tick)
    emit("begin : %s", current_function_name.c_str());
  else if (in_tock)
    emit("begin : %s", current_function_name.c_str());
  else if (in_task)
    emit("");
  else if (in_func)
    emit("");
  else
    debugbreak();

  //----------
  // Emit the function body with the correct type of "begin/end" pair,
  // hoisting locals to the top of the body scope.

  auto func_body = func.body();
  push_indent(func_body);

  auto body_count = func_body.child_count();
  for (int i = 0; i < body_count; i++) {
    auto c = func_body.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE:
        skip_over(c);
        // emit_to_newline();

        // while (*cursor != '\n') emit_char(*cursor++);
        // emit_char(*cursor++);
        emit_ws();

        emit_hoisted_decls(func_body);
        break;

      case sym_declaration: {
        MtDecl d(c);
        if (d.is_init_decl()) {
          emit_init_declarator_as_assign(c);
        } else {
          skip_over(c);
        }
        break;
      }

      case sym_expression_statement:
        if (c.child(0).sym == sym_call_expression &&
            c.child(0).child(0).sym == sym_field_expression) {
          // Calls to submodules get commented out.
          comment_out(c);
        } else {
          // Other calls get translated.
          emit_dispatch(c);
        }
        break;

      case anon_sym_RBRACE:
        skip_over(c);
        break;

      default:
        emit_dispatch(c);
        break;
    }
    if (i != body_count - 1) emit_ws();
  }

  pop_indent(func_body);

  //----------

  if (in_init)
    emit("end");
  else if (in_tick)
    emit("end");
  else if (in_tock)
    emit("end");
  else if (in_task)
    emit("endtask");
  else if (in_func)
    emit("endfunction");
  else
    debugbreak();

  emit_to_newline();

  //----------

  current_function_name = "";
  in_init = false;
  in_tick = false;
  in_tock = false;
  in_task = false;
  in_func = false;
}

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

void MtCursor::emit_field_as_enum_class(MtFieldDecl field_decl) {
  assert(cursor == field_decl.start());

  assert(field_decl.sym == sym_field_declaration);

  std::string enum_name;
  MtNode node_values;
  int bit_width = 0;
  std::string enum_type = "";

  if (field_decl.sym == sym_enum_specifier &&
      field_decl.get_field(field_body).sym == sym_enumerator_list) {
    auto node_name = field_decl.get_field(field_name);
    auto node_base = field_decl.get_field(field_base);
    enum_name = node_name.is_null() ? "" : node_name.text();
    enum_type = node_base.is_null() ? "" : node_base.text();
    node_values = field_decl.get_field(field_body);
    bit_width = 0;
  } else if (field_decl.sym == sym_field_declaration &&
             field_decl.get_field(field_type).sym == sym_enum_specifier &&
             field_decl.get_field(field_type).get_field(field_body).sym ==
                 sym_enumerator_list) {
    // Anonymous enums have "body" nested under "type"
    node_values = field_decl.get_field(field_type).get_field(field_body);
    bit_width = 0;
  } else if (field_decl.sym == sym_field_declaration &&
             field_decl.get_field(field_type).sym == sym_enum_specifier &&
             field_decl.get_field(field_default_value).sym ==
                 sym_initializer_list) {
    // TreeSitterCPP BUG - "enum class foo : int = {}" misinterpreted as
    // default_value

    enum_name = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = field_decl.get_field(field_default_value);
    bit_width = 0;
  } else if (field_decl.sym == sym_field_declaration &&
             field_decl.child_count() == 3 &&
             field_decl.child(0).sym == sym_enum_specifier &&
             field_decl.child(1).sym == sym_bitfield_clause) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" misinterpreted as
    // bitfield
    auto node_bitfield = field_decl.child(1);
    auto node_compound = node_bitfield.child(1);
    auto node_basetype = node_compound.get_field(field_type);
    auto node_scope = node_basetype.get_field(field_scope);
    auto node_args = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = node_compound.get_field(field_value);
    bit_width = atoi(node_bitwidth.start());
  } else if (field_decl.sym == sym_declaration &&
             field_decl.child_count() == 3 &&
             field_decl.child(0).sym == sym_enum_specifier &&
             field_decl.child(1).sym == sym_init_declarator) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" in namespace
    // misinterpreted as declarator
    auto node_decl1 = field_decl.get_field(field_declarator);
    auto node_decl2 = node_decl1.get_field(field_declarator);
    auto node_scope = node_decl2.get_field(field_scope);
    auto node_args = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name = field_decl.get_field(field_type).get_field(field_name).text();
    node_values = node_decl1.get_field(field_value);
    bit_width = atoi(node_bitwidth.start());
  } else {
    field_decl.error();
  }

  emit("typedef enum ");
  if (bit_width == 1) {
    emit("logic ", bit_width - 1);
  } else if (bit_width > 1) {
    emit("logic[%d:0] ", bit_width - 1);
  } else if (enum_type.size()) {
    if (enum_type == "int") emit("integer ");
  } else {
    // emit("integer ");
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

void MtCursor::emit_field_as_localparam(MtFieldDecl field_decl) {
  assert(cursor == field_decl.start());
  emit_children(field_decl);
}

//------------------------------------------------------------------------------
// Emit field declarations. For submodules, also emit glue declarations and
// append the glue parameter list to the field.

// field_declaration = { type:type_identifier, declarator:field_identifier+ }
// field_declaration = { type:template_type,   declarator:field_identifier+ }
// field_declaration = { type:enum_specifier,  bitfield_clause (TREESITTER BUG)
// }

void MtCursor::emit(MtFieldDecl field_decl) {
  assert(cursor == field_decl.start());

  // Handle "enum class", which is broken a bit in TreeSitterCpp
  if (field_decl.type().child_count() >= 3 &&
      field_decl.type().child(0).text() == "enum" &&
      field_decl.type().child(1).text() == "class" &&
      field_decl.type().child(2).sym == alias_sym_type_identifier) {
    emit_field_as_enum_class(field_decl);
    return;
  }

  std::string type_name = field_decl.type().node_to_type();

  if (lib->has_mod(type_name)) {
    auto c0 = field_decl.child(0);  // type
    auto c1 = field_decl.child(1);  // decl
    auto c2 = field_decl.child(2);  // semi

    auto inst_name = c1.text();

    auto mod = lib->get_mod(type_name);

    for (auto& n : mod->inputs) {
      MtCursor subcursor(lib, mod->source_file, str_out);
      subcursor.quiet = quiet;
      subcursor.in_ports = true;

      // parameter_declaration
      auto input_type = n.get_field(field_type);
      auto input_decl = n.get_field(field_declarator);

      subcursor.cursor = input_type.start();
      subcursor.emit_dispatch(input_type);
      subcursor.emit_ws();
      emit("%s_", inst_name.c_str());
      subcursor.emit_dispatch(input_decl);

      emit(";\n");
      emit_indent();
    }

    for (auto& n : mod->outputs) {
      MtCursor subcursor(lib, mod->source_file, str_out);
      subcursor.quiet = quiet;
      subcursor.in_ports = true;

      // field_declaration
      auto output_type = n.get_field(field_type);
      auto output_decl = n.get_field(field_declarator);

      subcursor.cursor = output_type.start();
      subcursor.emit_dispatch(output_type);
      subcursor.emit_ws();
      emit("%s_", inst_name.c_str());
      subcursor.emit_dispatch(output_decl);

      emit(";\n");
      emit_indent();
    }

    cursor = c0.start();
    emit_dispatch(c0);
    emit_ws();
    emit_dispatch(c1);
    emit_ws();

    // FIXME patch in the params before the semicolon
    emit("(clk, ");

    for (auto& n : mod->inputs) {
      emit("%s_%s, ", inst_name.c_str(), n.name.c_str());
    }

    for (int i = 0; i < mod->outputs.size(); i++) {
      auto& n = mod->outputs[i];
      emit("%s_%s", inst_name.c_str(), n.name.c_str());
      if (i != mod->outputs.size() - 1) emit(", ");
    }

    emit(")");

    emit_dispatch(c2);
    cursor = field_decl.end();

  } else if (field_decl.type().is_enum()) {
    emit_field_as_enum_class(field_decl);
  } else if (field_decl.is_output()) {
    if (!in_ports) {
      skip_to_next_sibling(field_decl);
    } else {
      emit_children(field_decl);
    }
  } else if (field_decl.is_static2() && field_decl.is_const2()) {
    emit_field_as_localparam(field_decl);
  } else if (current_mod->has_enum(type_name)) {
    emit_children(field_decl);
  } else if (type_name == "logic") {
    emit_children(field_decl);
  } else {
    debugbreak();
  }

#if 0
  /*
  std::string type_name = field_decl.type().node_to_type();
  auto submod = lib->find_mod(type_name);

  if (submod) {
    field_decl.dump_tree();
    auto field_name = field_decl.node_to_name();

    // Emit glue parameters and patch the glue parameter list into the submodule declaration.
    //emit_glue_declarations(field_decl);
    {
      auto type_name = decl.get_field(field_type).node_to_type();

      MtField field(decl,
                    decl.get_field(field_type),
                    type_name,
                    decl.get_field(field_declarator).text());

      auto submod = lib->find_mod(type_name);

      advance_to(decl);
      std::string inst_name = decl.node_to_name();

      if (submod->modparams.size()) {
        auto templ_args = decl.get_field(field_type).get_field(field_arguments);

        for (int i = 0; i < templ_args.named_child_count(); i++) {
          auto param_name = submod->modparams[i].node_to_name();
          id_replacements[param_name] = templ_args.named_child(i).text();
        }
      }

      //------------------------------------------------------------------------------
      // Emit "<type> <submod_name>_<output_name>;" glue declarations because we can't
      // directly pass arguments to submodules.

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
  
      for (auto& input : submod->inputs) {
        MtCursor sub_cursor(lib, source_file, str_out);
        sub_cursor.spacer_stack = spacer_stack;
        sub_cursor.id_replacements = id_replacements;
        sub_cursor.emit_glue_declaration(input, inst_name);
      }

      for (auto& output : submod->outputs) {
        MtCursor sub_cursor(lib, source_file, str_out);
        sub_cursor.spacer_stack = spacer_stack;
        sub_cursor.id_replacements = id_replacements;
        sub_cursor.emit_glue_declaration(output, inst_name);
      }

      id_replacements.clear();
    }
  
    /
    //emit_submodule_port_list(field_decl);
    {
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

    emit_newline();
  }
  else {
    // If this isn't a submodule, just tack on "input" and "output" annotations.
  }
  */
#endif
}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void MtCursor::emit(MtClassSpecifier n) {
  assert(cursor == n.start());

  auto struct_lit = n.child(0);
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

  emit_replacement(struct_lit, "module");
  emit_ws();
  emit_dispatch(struct_name);
  emit_newline();

  // Patch the template parameter list in after the module declaration, before
  // the port list.

  if (current_mod->mod_param_list) {
    emit_indent();
    MtCursor sub_cursor = *this;
    sub_cursor.cursor = current_mod->mod_param_list.start();
    for (auto c : (MtNode&)current_mod->mod_param_list) {
      switch (c.sym) {
        case anon_sym_LT:
          sub_cursor.emit_replacement(c, "#(");
          break;
        case anon_sym_GT:
          sub_cursor.emit_replacement(c, ")");
          break;

        case sym_parameter_declaration:
          sub_cursor.emit("parameter ");
          sub_cursor.emit_dispatch(c);
          break;

        case sym_optional_parameter_declaration:
          sub_cursor.emit("parameter ");
          sub_cursor.emit_dispatch(c);
          break;

        case anon_sym_COMMA:
          sub_cursor.emit_text(c);
          break;

        default:
          c.dump_tree();
          assert(false);
          break;
      }
      sub_cursor.emit_ws();
    }
    emit_newline();
  }

  // Save the indentation level of the struct body so we can use it in the port
  // list.
  push_indent(struct_body);

  {
    /*
    (
    input logic clk,
    input logic rst_n,
    output logic o_serial,
    output logic[7:0] o_data,
    output logic o_valid,
    output logic o_done,
    output logic[31:0] o_sum
    );
    */
    in_ports = true;
    trim_namespaces = false;
    emit("(");
    emit_newline();

    emit_indent();
    emit("input logic clk,");
    emit_newline();

    for (auto& input : current_mod->inputs) {
      MtCursor sub_cursor = *this;
      emit_indent();
      emit("input ");
      sub_cursor.cursor = input.start();

      auto c0 = input.child(0);  // type
      auto c1 = input.child(1);  // decl
      auto c2 = input.child(2);  // semi

      sub_cursor.emit_dispatch(c0);
      sub_cursor.emit_ws();
      sub_cursor.emit_dispatch(c1);

      emit(",");
      emit_newline();
    }

    for (int i = 0; i < current_mod->outputs.size(); i++) {
      auto& output = current_mod->outputs[i];
      MtCursor sub_cursor = *this;
      emit_indent();
      emit("output ");
      sub_cursor.cursor = output.start();

      auto c0 = output.child(0);  // type
      auto c1 = output.child(1);  // decl
      auto c2 = output.child(2);  // semi

      sub_cursor.emit_dispatch(c0);
      sub_cursor.emit_ws();
      sub_cursor.emit_dispatch(c1);

      if (i != current_mod->outputs.size() - 1) emit(",");
      emit_newline();
    }
    emit(");");
    trim_namespaces = true;
    in_ports = false;
  }

  // Whitespace between the end of the port list and the module body.
  // emit_ws();
  skip_ws();

  // Emit the module body, with a few modifications.
  // Discard the opening brace
  // Replace the closing brace with "endmodule"
  // Discard the seimcolon at the end of class{};"

  assert(struct_body.sym == sym_field_declaration_list);

  auto body_size = struct_body.child_count();
  // for (auto c : (MtNode&)struct_body) {
  for (int i = 0; i < body_size; i++) {
    auto c = struct_body.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE: {
        skip_over(c);
        break;
      }
      case anon_sym_RBRACE: {
        if (current_mod->port_map.size()) {
          emit_newline();
          emit_indent();
          emit("// Port map:\n");
          for (auto& p : current_mod->port_map) {
            emit_indent();

            auto a = p.first;
            auto b = p.second;
            for (auto& c : a)
              if (c == '.') c = '_';
            for (auto& c : b)
              if (c == '.') c = '_';

            emit("assign %s = %s;\n", a.c_str(), b.c_str());
          }
          emit_newline();
        }

        emit_replacement(c, "endmodule");
        break;
      }
      case anon_sym_SEMI: {
        emit_replacement(c, "");
        break;
      }
      case sym_comment:
        emit(MtComment(c));
        break;
      case sym_field_declaration:
        emit(MtFieldDecl(c));
        break;
      case sym_function_definition:
        emit(MtFuncDefinition(c));
        break;

      case sym_access_specifier:
        comment_out(c);
        // skip_to_next_line();
        break;

      case sym_preproc_ifdef:
        emit_dispatch(c);
        break;

      default:
        c.dump_tree();
        debugbreak();
        break;
    }
    if (i != body_size - 1) emit_ws();
  }

  pop_indent(struct_body);

  cursor = n.end();

  in_module_or_package--;

  if (!in_module_or_package) {
    current_mod = nullptr;
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtExprStatement n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) switch (c.sym) {
      case sym_call_expression:
        emit(MtCallExpr(c));
        break;

      case sym_assignment_expression:
        emit(MtAssignmentExpr(c));
        break;

      case anon_sym_SEMI:
        emit_text(c);
        break;

      default:
        c.dump_tree();
        debugbreak();
        break;
    }
}

//------------------------------------------------------------------------------
// Change "{ blah(); foo(); int x = 1; }" to "begin blah(); ... end"

void MtCursor::emit(MtCompoundStatement body) {
  assert(cursor == body.start());

  push_indent(body);

  auto body_count = body.child_count();
  for (int i = 0; i < body_count; i++) {
    auto c = body.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE:
        emit_replacement(c, "begin");
        emit_to_newline();
        emit_hoisted_decls(body);
        break;
      case sym_declaration:
        emit_init_declarator_as_assign(c);
        break;
      case anon_sym_RBRACE: {
        emit_replacement(c, "end");
        break;
      }
      case sym_expression_statement:
        emit(MtExprStatement(c));
        break;

      case sym_if_statement:
        emit(MtIfStatement(c));
        break;

      case sym_comment:
        emit(MtComment(c));
        break;

      case sym_break_statement:
        emit(MtBreakStatement(c));
        break;

      default:
        c.dump_tree();
        debugbreak();
        break;
    }
    if (i != body_count - 1) emit_ws();
  }

  pop_indent(body);
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void MtCursor::emit(MtTemplateType templ_type) {
  assert(cursor == templ_type.start());

  emit(templ_type.name());
  emit_ws();

  auto args = templ_type.args();

  bool is_logic = templ_type.name().match("logic");
  if (is_logic) {
    auto logic_size = args.first_named_child();
    switch (logic_size.sym) {
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
  } else {
    emit(args);
  }

  cursor = templ_type.end();
}

//------------------------------------------------------------------------------
// Change (template)<int param, int param> to
// #(parameter int param, parameter int param)

void MtCursor::emit(MtTemplateParamList n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) switch (c.sym) {
      case anon_sym_LT:
        emit_replacement(c, "#(");
        break;
      case anon_sym_GT:
        emit_replacement(c, ")");
        break;

      // intentional fallthrough, we're just appending "parameter "
      case sym_parameter_declaration:
      case sym_optional_parameter_declaration:
        emit("parameter ");
        emit_dispatch(c);
        break;

      default:
        debugbreak();
        break;
    }
}

//------------------------------------------------------------------------------
// Change <param, param> to #(param, param)

void MtCursor::emit(MtTemplateArgList n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) switch (c.sym) {
      case anon_sym_LT:
        emit_replacement(c, " #(");
        break;
      case anon_sym_GT:
        emit_replacement(c, ")");
        break;
      default:
        emit_dispatch(c);
        break;
    }
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void MtCursor::emit(MtEnumeratorList n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) switch (c.sym) {
      case anon_sym_LBRACE:
        emit_text(c);
        break;
      case anon_sym_RBRACE:
        emit_text(c);
        break;
      default:
        emit_dispatch(c);
        break;
    }
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void MtCursor::emit(MtTranslationUnit n) {
  assert(cursor == n.start());

  emit("`default_nettype none\n");

  auto child_count = n.child_count();
  for (int i = 0; i < child_count; i++) {
    auto c = n.child(i);
    switch (c.sym) {
      case anon_sym_SEMI:
        skip_over(c);
        break;
      default:
        emit_dispatch(c);
        break;
    }
    if (i != child_count - 1) emit_ws();
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
  assert(cursor == n.start());

  assert(!override_size || !size_cast);
  if (override_size) size_cast = override_size;

  std::string body = n.text();

  // Count how many 's are in the number
  int spacer_count = 0;
  int prefix_count = 0;

  for (auto& c : body)
    if (c == '\'') {
      c = '_';
      spacer_count++;
    }

  if (body.starts_with("0x")) {
    prefix_count = 2;
    if (!size_cast) size_cast = ((int)body.size() - 2 - spacer_count) * 4;
    emit("%d'h", size_cast);
  } else if (body.starts_with("0b")) {
    prefix_count = 2;
    if (!size_cast) size_cast = (int)body.size() - 2 - spacer_count;
    emit("%d'b", size_cast);
  } else {
    if (size_cast) emit("%d'd", size_cast);
  }

  if (spacer_count) {
    emit(body.c_str() + prefix_count);
  } else {
    emit_span(n.start() + prefix_count, n.end());
  }

  cursor = n.end();
}

//------------------------------------------------------------------------------
// Change "return x" to "(funcname) = x" to match old Verilog return style.

void MtCursor::emit(MtReturnStatement n) {
  assert(cursor == n.start());
  auto func_name = current_function_name;
  for (auto c : (MtNode&)n) switch (c.sym) {
      case anon_sym_return:
        emit_replacement(c, "%s =", func_name.c_str());
        break;
      default:
        emit_dispatch(c);
        break;
    }
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit(MtPrimitiveType n) {
  assert(cursor == n.start());
  emit_text(n);
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit(MtIdentifier n) {
  assert(cursor == n.start());

  auto name = n.node_to_name();
  auto it = id_replacements.find(name);
  if (it != id_replacements.end()) {
    emit_replacement(n, it->second.c_str());
  } else {
    emit_text(n);
  }
}

void MtCursor::emit(MtTypeIdentifier n) {
  assert(cursor == n.start());

  auto name = n.node_to_name();
  auto it = id_replacements.find(name);
  if (it != id_replacements.end()) {
    emit_replacement(n, it->second.c_str());
  } else {
    emit_text(n);
  }
}

//------------------------------------------------------------------------------
// For some reason the class's trailing semicolon ends up with the template
// field_decl, so we prune it here.

void MtCursor::emit(MtTemplateDecl n) {
  assert(cursor == n.start());

  auto struct_specifier = MtClassSpecifier(n.child(2));
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
  assert(cursor == n.start());

  auto field = n.text();
  for (auto& c : field)
    if (c == '.') c = '_';
  emit_replacement(n, field.c_str());
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtCaseStatement n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) {
    if (c.sym == anon_sym_case) {
      skip_to_next_sibling(c);
    } else {
      emit_dispatch(c);
    }
    emit_ws();
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtSwitchStatement n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) {
    if (c.sym == anon_sym_switch) {
      emit_replacement(c, "case");
    } else if (c.field == field_body) {
      for (auto gc : c) {
        if (gc.sym == anon_sym_LBRACE) {
          skip_over(gc);
        } else if (gc.sym == anon_sym_RBRACE) {
          emit_replacement(gc, "endcase");
        } else {
          emit_dispatch(gc);
        }
        emit_ws();
      }

    } else {
      emit_dispatch(c);
    }
    emit_ws();
  }
}

//------------------------------------------------------------------------------
// Unwrap magic /*#foo#*/ comments to pass arbitrary text to Verilog.

void MtCursor::emit(MtComment n) {
  assert(cursor == n.start());

  auto body = n.text();
  if (body.starts_with("/*#") && body.ends_with("#*/")) {
    body.erase(body.size() - 3, 3);
    body.erase(0, 3);
    emit_replacement(n, body.c_str());
  } else {
    emit_text(n);
  }
}

//------------------------------------------------------------------------------
// Verilog doesn't use "break"

void MtCursor::emit(MtBreakStatement n) {
  assert(cursor == n.start());
  comment_out(n);
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtFieldDeclList n) {
  assert(cursor == n.start());
  emit_children(n);
}

//------------------------------------------------------------------------------
// TreeSitter nodes slightly broken for "a = b ? c : d;"...

void MtCursor::emit(MtCondExpr n) {
  assert(cursor == n.start());
  emit_children(n);
}

//------------------------------------------------------------------------------
// Static variables become localparams at module level.

void MtCursor::emit(MtStorageSpec n) {
  assert(cursor == n.start());
  n.match("static") ? emit_replacement(n, "localparam") : comment_out(n);
}

//------------------------------------------------------------------------------
// ...er, this was something about namespace resolution?

void MtCursor::emit(MtQualifiedId n) {
  assert(cursor == n.start());
  if (trim_namespaces) {
    auto last_child = n.child(n.child_count() - 1);
    cursor = last_child.start();
    emit_dispatch(last_child);
    cursor = n.end();
  } else {
    emit_children(n);
  }
}

//------------------------------------------------------------------------------
// If statements are basically the same.

void MtCursor::emit(MtIfStatement n) {
  assert(cursor == n.start());
  emit_children(n);
}

//------------------------------------------------------------------------------
// Enums are broken.

void MtCursor::emit(MtEnumSpecifier n) {
  assert(cursor == n.start());
  // emit_sym_field_declaration_as_enum_class(MtFieldDecl(n));
  // for (auto c : n) emit_dispatch(c);
  debugbreak();
}

//------------------------------------------------------------------------------
// FIXME - are we using this anywhere?

void MtCursor::emit(MtUsingDecl n) {
  assert(cursor == n.start());
  auto name = n.child(2).text();
  emit_replacement(n, "import %s::*;", name.c_str());
}

//------------------------------------------------------------------------------
// FIXME - When do we hit this? It doesn't look finished.
// It's for namespace decls

// FIXME need to handle const char* string declarations

/*
========== tree dump begin
[00:000:187] declaration =
[00:000:226] |  storage_class_specifier =
[00:000:064] |  |  lit = "static"
[01:000:227] |  type_qualifier =
[00:000:068] |  |  lit = "const"
[02:032:078] |  type: primitive_type = "char"
[03:009:224] |  declarator: init_declarator =
[00:009:212] |  |  declarator: pointer_declarator =
[00:000:023] |  |  |  lit = "*"
[01:009:001] |  |  |  declarator: identifier = "TEXT_HEX"
[01:000:063] |  |  lit = "="
[02:034:278] |  |  value: string_literal =
[00:000:123] |  |  |  lit = "\""
[01:000:123] |  |  |  lit = "\""
[04:000:039] |  lit = ";"
========== tree dump end
*/

void MtCursor::emit(MtDecl n) {
  //n.dump_tree();

  assert(cursor == n.start());

  // Check for "static const char"
  if (n.is_static2() && n.is_const2()) {
    auto node_type = n.get_field(field_type);
    if (node_type.text() == "char") {
      emit("localparam string ");
      auto init_decl = n.get_field(field_declarator);
      auto pointer_decl = init_decl.get_field(field_declarator);
      auto name = pointer_decl.get_field(field_declarator);
      cursor = name.start();
      emit_text(name);
      emit(" = ");

      auto val = init_decl.get_field(field_value);
      cursor = val.start();
      emit_text(val);
      emit(";");
      cursor = n.end();
      return;
    }
  }


  // Check for enum classes, which are broken.
  auto node_type = n.get_field(field_type);
  if (node_type.child_count() >= 2 && node_type.child(0).text() == "enum" &&
      node_type.child(1).text() == "class") {
    debugbreak();
    // emit_field_decl_as_enum_class(MtFieldDecl(n));
    return;
  }

  if (n.child_count() >= 5 && n.child(0).text() == "static" &&
      n.child(1).text() == "const") {
    emit("parameter ");
    cursor = n.child(2).start();
    emit_dispatch(n.child(2));
    emit_ws();
    emit_dispatch(n.child(3));
    emit_ws();
    emit_dispatch(n.child(4));

    cursor = n.end();
    return;
  }

  // Regular boring local variable declaration?
  for (auto c : (MtNode)n) {
    emit_dispatch(c);
    emit_ws();
  }
}

//------------------------------------------------------------------------------
// "unsigned int" -> "int unsigned"

void MtCursor::emit(MtSizedTypeSpec n) {
  assert(cursor == n.start());

  assert(n.child_count() == 2);

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
  assert(cursor == n.start());

  in_module_or_package++;
  auto node_name = n.get_field(field_name);
  auto node_body = n.get_field(field_body);

  emit("package %s;", node_name.text().c_str());
  cursor = node_body.start();

  for (auto c : node_body) {
    if (c.sym == anon_sym_LBRACE) {
      emit_replacement(c, "");
    } else if (c.sym == anon_sym_RBRACE) {
      emit_replacement(c, "");
    } else {
      emit_dispatch(c);
    }
    emit_ws();
  }

  emit("endpackage");

  emit_indent();
  cursor = n.end();
  in_module_or_package++;
}

//------------------------------------------------------------------------------
// Arg lists are the same in C and Verilog.

void MtCursor::emit(MtArgList n) {
  assert(cursor == n.start());
  emit_children(n);
}

void MtCursor::emit(MtParameterList n) {
  assert(cursor == n.start());
  emit_children(n);
}

void MtCursor::emit(MtFieldIdentifier n) {
  assert(cursor == n.start());
  emit_text(n);
}

//------------------------------------------------------------------------------
// Call the correct emit() method based on the node type.

void MtCursor::emit_dispatch(MtNode n) {
  if (cursor != n.start()) {
    n.dump_tree();
    assert(cursor == n.start());
  }

  switch (n.sym) {
    case sym_preproc_def: {
      auto lit = n.child(0);
      auto name = n.get_field(field_name);
      auto value = n.get_field(field_value);

      emit_replacement(lit, "`define");
      emit_ws();
      emit_dispatch(name);
      emit_ws();
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
      emit_children(n);
      break;
    }

      // case aux_sym_preproc_if_token2:

    case sym_preproc_else: {
      emit_children(n);
      break;
    }

    case sym_preproc_arg: {
      emit_text(n);
      break;
    }

    case sym_preproc_call:
    case sym_access_specifier:
    case sym_type_qualifier:
    case sym_preproc_if:
      skip_to_next_sibling(n);
      break;

    case sym_for_statement:
    case sym_parenthesized_expression:
    case sym_parameter_declaration:
    case sym_optional_parameter_declaration:
    case sym_condition_clause:
    case sym_unary_expression:
    case sym_subscript_expression:
    case sym_enumerator:
    case sym_type_definition:
    case sym_binary_expression:
    case sym_array_declarator:
    case sym_type_descriptor:
    case sym_init_declarator:
    case sym_initializer_list:
    case sym_declaration_list:
      emit_children(n);
      break;

    case alias_sym_field_identifier:
      emit(MtFieldIdentifier(n));
      break;
    case sym_parameter_list:
      emit(MtParameterList(n));
      break;
    case sym_function_declarator:
      emit_func_decl(MtFuncDeclarator(n));
      break;
    case sym_expression_statement:
      emit(MtExprStatement(n));
      break;
    case sym_argument_list:
      emit(MtArgList(n));
      break;
    case sym_enum_specifier:
      emit(MtEnumSpecifier(n));
      break;
    case sym_if_statement:
      emit(MtIfStatement(n));
      break;
    case sym_qualified_identifier:
      emit(MtQualifiedId(n));
      break;
    case sym_storage_class_specifier:
      emit(MtStorageSpec(n));
      break;
    case sym_conditional_expression:
      emit(MtCondExpr(n));
      break;
    case sym_field_declaration_list:
      emit(MtFieldDeclList(n));
      break;
    case sym_break_statement:
      emit(MtBreakStatement(n));
      break;
    case sym_identifier:
      emit(MtIdentifier(n));
      break;
    case sym_class_specifier:
      emit(MtClassSpecifier(n));
      break;
    case sym_number_literal:
      emit(MtNumberLiteral(n));
      break;
    case sym_field_expression:
      emit(MtFieldExpr(n));
      break;
    case sym_return_statement:
      emit(MtReturnStatement(n));
      break;
    case sym_template_declaration:
      emit(MtTemplateDecl(n));
      break;
    case sym_preproc_include:
      emit(MtPreprocInclude(n));
      break;
    case sym_field_declaration:
      emit(MtFieldDecl(n));
      break;
    case sym_compound_statement:
      emit(MtCompoundStatement(n));
      break;
    case sym_template_type:
      emit(MtTemplateType(n));
      break;
    case sym_translation_unit:
      emit(MtTranslationUnit(n));
      break;
    case sym_primitive_type:
      emit(MtPrimitiveType(n));
      break;
    case alias_sym_type_identifier:
      emit(MtTypeIdentifier(n));
      break;
    case sym_function_definition:
      emit(MtFuncDefinition(n));
      break;
    case sym_call_expression:
      emit(MtCallExpr(n));
      break;
    case sym_assignment_expression:
      emit(MtAssignmentExpr(n));
      break;
    case sym_template_argument_list:
      emit(MtTemplateArgList(n));
      break;
    case sym_comment:
      emit(MtComment(n));
      break;
    case sym_enumerator_list:
      emit(MtEnumeratorList(n));
      break;
    case sym_case_statement:
      emit(MtCaseStatement(n));
      break;
    case sym_switch_statement:
      emit(MtSwitchStatement(n));
      break;
    case sym_using_declaration:
      emit(MtUsingDecl(n));
      break;
    case sym_sized_type_specifier:
      emit(MtSizedTypeSpec(n));
      break;
    case sym_declaration:
      emit(MtDecl(n));
      break;
    case sym_namespace_definition:
      emit(MtNamespaceDef(n));
      break;

    default:
      static std::set<int> passthru_syms = {
          alias_sym_namespace_identifier, alias_sym_field_identifier,
          sym_sized_type_specifier, sym_string_literal};

      if (!n.is_named()) {
        auto text = n.text();
        if (text == "#ifdef")
          emit_replacement(n, "`ifdef");
        else if (text == "#ifndef")
          emit_replacement(n, "`ifndef");
        else if (text == "#else")
          emit_replacement(n, "`else");
        else if (text == "#endif")
          emit_replacement(n, "`endif");
        else {
          // FIXME TreeSitter bug - we get a anon_sym_SEMI with no text in
          // alu_control.h
          // FIXME TreeSitter - #ifdefs in if/else trees break things
          if (n.start() != n.end()) emit_text(n);
        }
      }

      else if (passthru_syms.contains(n.sym)) {
        emit_text(n);
      } else {
        printf("Don't know what to do with %d %s\n", n.sym, n.type());
        n.error();
      }
      break;
  }
}

//------------------------------------------------------------------------------

void MtCursor::emit_children(MtNode n) {
  assert(cursor == n.start());
  auto count = n.child_count();
  for (auto i = 0; i < count; i++) {
    emit_dispatch(n.child(i));
    if (i != count - 1) emit_ws();
  }
}

//------------------------------------------------------------------------------
