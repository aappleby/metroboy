#include "MtCursor.h"

#include <stdarg.h>

#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtNode.h"
#include "MtSourceFile.h"
#include "Platform.h"

#include "../CoreLib/Log.h"

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
    indent = "";
  } else {
    indent = std::string(begin + 1, end);
  }

  for (auto& c : indent) {
    assert(isspace(c));
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
    line_dirty = false;
    line_elided = false;
  }

  if (!quiet) putchar(c);
  str_out->push_back(c);

  at_newline = c == '\n';
}

//----------------------------------------

void MtCursor::emit_ws() {
  while (cursor < source_file->source_end && isspace(*cursor)) {
    emit_char(*cursor++);
  }
}

void MtCursor::emit_ws_to_newline() {
  while (cursor < source_file->source_end && isspace(*cursor)) {
    auto c = *cursor++;
    emit_char(c);
    if (c == '\n') return;
  }
}

//----------------------------------------

void MtCursor::skip_over(MtNode n) {
  assert(cursor == n.start());
  cursor = n.end();
  line_elided = true;
}

//----------------------------------------

void MtCursor::skip_ws() {
  while (*cursor && isspace(*cursor)) {
    cursor++;
  }
}

//----------------------------------------

void MtCursor::comment_out(MtNode n) {
  assert(cursor == n.start());
  emit("/*");
  emit_text(n);
  emit("*/");
  assert(cursor == n.end());
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

//------------------------------------------------------------------------------
// Replace "#include" with "`include" and ".h" with ".sv"

void MtCursor::emit(MtPreprocInclude n) {
  assert(cursor == n.start());

  emit_replacement(n.child(0), "`include");
  emit_ws();
  emit_span(n.path_node().start(), n.path_node().end() - 1);
  emit(".sv\"");
  cursor = n.end();
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change '=' to '<=' if lhs is a field and we're inside a sequential block.

// sym_assignment_expression := { left: identifier, operator: lit, right : expr
// }

void MtCursor::emit(MtAssignmentExpr n) {
  assert(cursor == n.start());

  auto lhs = n.lhs();
  auto rhs = n.rhs();

  bool lhs_is_reg = false;
  if (lhs.sym == sym_identifier) {
    std::string lhs_name = lhs.text();
    for (auto& f : *current_mod->registers) {
      if (f.name() == lhs_name) {
        lhs_is_reg = true;
        break;
      }
    }
  }

  std::string lhs_name = lhs.text();

  if (lhs.sym == sym_identifier) {
    emit(MtIdentifier(lhs));
  } else if (lhs.sym == sym_subscript_expression) {
    emit_children(lhs);
  } else {
    lhs.dump_tree();
    debugbreak();
  }
  emit_ws();

  if (in_tick && lhs_is_reg) emit("<");
  emit_text(n.op());

  // Emit_dispatch makes sense here, as we really could have anything on the
  // rhs.
  emit_ws();
  emit_dispatch(rhs);

  assert(cursor == n.end());
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
    } else {
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

void MtCursor::emit(MtCallExpr n) {
  assert(cursor == n.start());

  MtFunc func = n.func();
  MtArgList args = n.args();

  // If we're calling a member function, look at the name of the member
  // function and not the whole foo.bar().

  std::string func_name = func.name();

  if (func_name == "coerce") {
    // Convert to cast? We probably shouldn't be calling coerce() directly.
    n.error();
  }
  else if (func_name == "sra") {
    auto lhs = args.named_child(0);
    auto rhs = args.named_child(1);

    emit("($signed(");
    cursor = lhs.start();
    emit_dispatch(lhs);
    emit(") >>> ");
    cursor = rhs.start();
    emit_dispatch(rhs);
    emit(")");
    cursor = n.end();
    
    //call.dump_tree();

  }
  else if (func_name == "signed") {
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
  } else if (func_name == "value_plusargs") {
    emit_replacement(func, "$value$plusargs");
    emit(args);
  } else if (func_name == "write") {
    emit_replacement(func, "$write");
    emit(args);
  } else if (func_name.starts_with("init")) {
    comment_out(n);
  } else if (func_name.starts_with("final")) {
    comment_out(n);
  } else if (func_name.starts_with("tick")) {
    comment_out(n);
  } else if (func_name.starts_with("tock")) {
    comment_out(n);
  } else if (func_name == "bx") {
    // Bit extract.
    auto template_arg = func.as_templ().args().named_child(0);
    emit_dynamic_bit_extract(n, template_arg);
  } else if (func_name == "b1")
    emit_static_bit_extract(n, 1);
  else if (func_name == "b2")
    emit_static_bit_extract(n, 2);
  else if (func_name == "b3")
    emit_static_bit_extract(n, 3);
  else if (func_name == "b4")
    emit_static_bit_extract(n, 4);
  else if (func_name == "b5")
    emit_static_bit_extract(n, 5);
  else if (func_name == "b6")
    emit_static_bit_extract(n, 6);
  else if (func_name == "b7")
    emit_static_bit_extract(n, 7);
  else if (func_name == "b8")
    emit_static_bit_extract(n, 8);
  else if (func_name == "b9")
    emit_static_bit_extract(n, 9);

  else if (func_name == "b10")
    emit_static_bit_extract(n, 10);
  else if (func_name == "b11")
    emit_static_bit_extract(n, 11);
  else if (func_name == "b12")
    emit_static_bit_extract(n, 12);
  else if (func_name == "b13")
    emit_static_bit_extract(n, 13);
  else if (func_name == "b14")
    emit_static_bit_extract(n, 14);
  else if (func_name == "b15")
    emit_static_bit_extract(n, 15);
  else if (func_name == "b16")
    emit_static_bit_extract(n, 16);
  else if (func_name == "b17")
    emit_static_bit_extract(n, 17);
  else if (func_name == "b18")
    emit_static_bit_extract(n, 18);
  else if (func_name == "b19")
    emit_static_bit_extract(n, 19);

  else if (func_name == "b20")
    emit_static_bit_extract(n, 20);
  else if (func_name == "b21")
    emit_static_bit_extract(n, 21);
  else if (func_name == "b22")
    emit_static_bit_extract(n, 22);
  else if (func_name == "b23")
    emit_static_bit_extract(n, 23);
  else if (func_name == "b24")
    emit_static_bit_extract(n, 24);
  else if (func_name == "b25")
    emit_static_bit_extract(n, 25);
  else if (func_name == "b26")
    emit_static_bit_extract(n, 26);
  else if (func_name == "b27")
    emit_static_bit_extract(n, 27);
  else if (func_name == "b28")
    emit_static_bit_extract(n, 28);
  else if (func_name == "b29")
    emit_static_bit_extract(n, 29);

  else if (func_name == "b30")
    emit_static_bit_extract(n, 30);
  else if (func_name == "b31")
    emit_static_bit_extract(n, 31);
  else if (func_name == "b32")
    emit_static_bit_extract(n, 32);
  else if (func_name == "b33")
    emit_static_bit_extract(n, 33);
  else if (func_name == "b34")
    emit_static_bit_extract(n, 34);
  else if (func_name == "b35")
    emit_static_bit_extract(n, 35);
  else if (func_name == "b36")
    emit_static_bit_extract(n, 36);
  else if (func_name == "b37")
    emit_static_bit_extract(n, 37);
  else if (func_name == "b38")
    emit_static_bit_extract(n, 38);
  else if (func_name == "b39")
    emit_static_bit_extract(n, 39);

  else if (func_name == "b40")
    emit_static_bit_extract(n, 40);
  else if (func_name == "b41")
    emit_static_bit_extract(n, 41);
  else if (func_name == "b42")
    emit_static_bit_extract(n, 42);
  else if (func_name == "b43")
    emit_static_bit_extract(n, 43);
  else if (func_name == "b44")
    emit_static_bit_extract(n, 44);
  else if (func_name == "b45")
    emit_static_bit_extract(n, 45);
  else if (func_name == "b46")
    emit_static_bit_extract(n, 46);
  else if (func_name == "b47")
    emit_static_bit_extract(n, 47);
  else if (func_name == "b48")
    emit_static_bit_extract(n, 48);
  else if (func_name == "b49")
    emit_static_bit_extract(n, 49);

  else if (func_name == "b50")
    emit_static_bit_extract(n, 50);
  else if (func_name == "b51")
    emit_static_bit_extract(n, 51);
  else if (func_name == "b52")
    emit_static_bit_extract(n, 52);
  else if (func_name == "b53")
    emit_static_bit_extract(n, 53);
  else if (func_name == "b54")
    emit_static_bit_extract(n, 54);
  else if (func_name == "b55")
    emit_static_bit_extract(n, 55);
  else if (func_name == "b56")
    emit_static_bit_extract(n, 56);
  else if (func_name == "b57")
    emit_static_bit_extract(n, 57);
  else if (func_name == "b58")
    emit_static_bit_extract(n, 58);
  else if (func_name == "b59")
    emit_static_bit_extract(n, 59);

  else if (func_name == "b60")
    emit_static_bit_extract(n, 60);
  else if (func_name == "b61")
    emit_static_bit_extract(n, 61);
  else if (func_name == "b62")
    emit_static_bit_extract(n, 62);
  else if (func_name == "b63")
    emit_static_bit_extract(n, 63);
  else if (func_name == "b64")
    emit_static_bit_extract(n, 64);

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
    cursor = n.end();
  } else {
    // All other function/task calls go through normally.
    emit_children(n);
  }

  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Replace "logic blah = x;" with "logic blah;"

void MtCursor::emit_init_declarator_as_decl(MtDecl n) {
  assert(cursor == n.start());

  emit(n._type());
  emit_ws();
  emit(n._init_decl().decl());
  emit(";");
  cursor = n.end();

  assert(cursor == n.end());
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

  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Emit local variable declarations at the top of the block scope.

void MtCursor::emit_hoisted_decls(MtCompoundStatement n) {
  bool any_to_hoist = false;

  for (auto c : (MtNode&)n) {
    if (c.sym == sym_declaration) {
      bool is_localparam = c.sym == sym_declaration && c.child_count() >= 4 &&
                           c.child(0).text() == "static" &&
                           c.child(1).text() == "const";

      if (is_localparam) {
      } else {
        any_to_hoist = true;
        break;
      }
    }
  }

  if (!any_to_hoist) return;

  if (!at_newline) {
    LOG_R("We're in some weird one-liner with a local variable?");
    debugbreak();
    emit_newline();
    emit_indent();
  }

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

        if (d.is_init_decl()) {
          emit_indent();
          emit_init_declarator_as_decl(MtDecl(c));
          emit_newline();
        } else {
          emit_indent();
          emit_dispatch(d);
          emit_newline();
        }
      }
    }
  }
  *this = old_cursor;

  assert(at_newline);
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

void MtCursor::emit(MtFuncDefinition n) {
  assert(cursor == n.start());

  current_function_name = n.decl().decl().node_to_name();
  in_task = n.type().match("void");
  in_func = !in_task;
  in_init = in_task && current_function_name.starts_with("init");
  in_tick = in_task && current_function_name.starts_with("tick");
  in_tock = in_task && current_function_name.starts_with("tock");

  //----------
  // Emit a block declaration for the type of function we're in.

  skip_over(n.type());
  skip_ws();

  if (in_init) {
    emit_replacement(n.decl(), "initial");
  } else if (in_tick) {
    emit_replacement(n.decl(), "always_ff @(posedge clock)");
  } else if (in_tock) {
    emit_replacement(n.decl(), "always_comb");
  } else if (in_task) {
    emit("task ");
    emit_dispatch(n.decl());
    emit(";");
  } else if (in_func) {
    emit("function %s ", n.type().text().c_str());
    emit_dispatch(n.decl());
    emit(";");
  } else {
    debugbreak();
  }

  emit_ws();

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

  auto func_body = n.body();
  push_indent(func_body);

  auto body_count = func_body.child_count();
  for (int i = 0; i < body_count; i++) {
    auto c = func_body.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE:
        skip_over(c);

        // while (*cursor != '\n') emit_char(*cursor++);
        // emit_char(*cursor++);
        emit_ws_to_newline();
        emit_hoisted_decls(func_body);
        emit_ws();
        break;

      case sym_declaration: {
        MtDecl d(c);
        if (d.is_init_decl()) {
          emit_init_declarator_as_assign(c);
        } else {
          skip_over(c);
          //comment_out(c);
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

  assert(cursor == n.end());

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

void MtCursor::emit_field_as_enum_class(MtFieldDecl n) {
  assert(cursor == n.start());

  assert(n.sym == sym_field_declaration);

  std::string enum_name;
  MtNode node_values;
  int bit_width = 0;
  std::string enum_type = "";

  if (n.sym == sym_enum_specifier &&
      n.get_field(field_body).sym == sym_enumerator_list) {
    auto node_name = n.get_field(field_name);
    auto node_base = n.get_field(field_base);
    enum_name = node_name.is_null() ? "" : node_name.text();
    enum_type = node_base.is_null() ? "" : node_base.text();
    node_values = n.get_field(field_body);
    bit_width = 0;
  } else if (n.sym == sym_field_declaration &&
             n.get_field(field_type).sym == sym_enum_specifier &&
             n.get_field(field_type).get_field(field_body).sym ==
                 sym_enumerator_list) {
    // Anonymous enums have "body" nested under "type"
    node_values = n.get_field(field_type).get_field(field_body);
    bit_width = 0;
  } else if (n.sym == sym_field_declaration &&
             n.get_field(field_type).sym == sym_enum_specifier &&
             n.get_field(field_default_value).sym ==
                 sym_initializer_list) {
    // TreeSitterCPP BUG - "enum class foo : int = {}" misinterpreted as
    // default_value

    enum_name = n.get_field(field_type).get_field(field_name).text();
    node_values = n.get_field(field_default_value);
    bit_width = 0;
  } else if (n.sym == sym_field_declaration &&
             n.child_count() == 3 &&
             n.child(0).sym == sym_enum_specifier &&
             n.child(1).sym == sym_bitfield_clause) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" misinterpreted as
    // bitfield
    auto node_bitfield = n.child(1);
    auto node_compound = node_bitfield.child(1);
    auto node_basetype = node_compound.get_field(field_type);
    auto node_scope = node_basetype.get_field(field_scope);
    auto node_args = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name = n.get_field(field_type).get_field(field_name).text();
    node_values = node_compound.get_field(field_value);
    bit_width = atoi(node_bitwidth.start());
  } else if (n.sym == sym_declaration &&
             n.child_count() == 3 &&
             n.child(0).sym == sym_enum_specifier &&
             n.child(1).sym == sym_init_declarator) {
    // TreeSitterCPP BUG - "enum class foo : logic<2> = {}" in namespace
    // misinterpreted as declarator
    auto node_decl1 = n.get_field(field_declarator);
    auto node_decl2 = node_decl1.get_field(field_declarator);
    auto node_scope = node_decl2.get_field(field_scope);
    auto node_args = node_scope.get_field(field_arguments);
    auto node_bitwidth = node_args.child(1);
    assert(node_bitwidth.sym == sym_number_literal);

    enum_name = n.get_field(field_type).get_field(field_name).text();
    node_values = node_decl1.get_field(field_value);
    bit_width = atoi(node_bitwidth.start());
  } else {
    n.error();
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
  cursor = n.end();

  // BUG: Trailing semicolons are inconsistent.
  if (n.text().back() == ';') emit(";");
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------

void MtCursor::emit_field_as_submod(MtFieldDecl n) {
  std::string type_name = n.type().node_to_type();
  auto submod_mod = lib->get_mod(type_name);

  auto node_type = n.child(0);  // type
  auto node_decl = n.child(1);  // decl
  auto node_semi = n.child(2);  // semi

  auto inst_name = node_decl.text();

  // Swap template arguments with the values from the template instantiation.
  std::map<std::string, std::string> replacements;

  auto args = node_type.get_field(field_arguments);
  if (args) {
    int arg_count = args.named_child_count();
    for (int i = 0; i < arg_count; i++) {
      auto key = submod_mod->modparams->at(i).name();
      auto val = args.named_child(i).text();
      replacements[key] = val;
    }
  }

  cursor = node_type.start();
  emit_dispatch(node_type);
  emit_ws();
  emit_dispatch(node_decl);
  emit_ws();

  emit("(");

  indent_stack.push_back(indent_stack.back() + "  ");

  emit_newline();
  emit_indent();
  emit("// Inputs");

  emit_newline();
  emit_indent();
  emit(".clock(clock),");

  int port_count =
      int(submod_mod->inputs->size() + submod_mod->outputs->size());
  int port_index = 0;

  for (auto& n : *submod_mod->inputs) {
    auto key = inst_name + "." + n.name();
    auto it = current_mod->port_map->find(key);
    assert(it != current_mod->port_map->end());

    emit_newline();
    emit_indent();
    emit(".%s(%s)", n.name().c_str(), (*it).second.c_str());

    if (port_index++ < port_count - 1) emit(", ");
  }

  emit_newline();
  emit_indent();
  emit("// Outputs");

  for (auto& n : *submod_mod->outputs) {
    emit_newline();
    emit_indent();
    emit(".%s(%s_%s)", n.name().c_str(), inst_name.c_str(), n.name().c_str());

    if (port_index++ < port_count - 1) emit(", ");
  }

  indent_stack.pop_back();

  emit_newline();
  emit_indent();
  emit(")");
  emit_dispatch(node_semi);
  emit_newline();

  cursor = n.end();

  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Emit field declarations. For submodules, also emit glue declarations and
// append the glue parameter list to the field.

// field_declaration = { type:type_identifier, declarator:field_identifier+ }
// field_declaration = { type:template_type,   declarator:field_identifier+ }
// field_declaration = { type:enum_specifier,  bitfield_clause (TREESITTER BUG)
// }

void MtCursor::emit(MtFieldDecl n) {
  assert(cursor == n.start());

  // Handle "enum class", which is broken a bit in TreeSitterCpp
  if (n.type().child_count() >= 3 &&
      n.type().child(0).text() == "enum" &&
      n.type().child(1).text() == "class" &&
      n.type().child(2).sym == alias_sym_type_identifier) {
    emit_field_as_enum_class(n);
    return;
  }

  std::string type_name = n.type().node_to_type();

  if (lib->has_mod(type_name)) {
    emit_field_as_submod(n);
  } else if (n.type().is_enum()) {
    emit_field_as_enum_class(n);
  } else if (current_mod->has_output(n.name().text())) {
    if (!in_ports) {
      //skip_to_next_sibling(n);
      //skip_over(n);
      comment_out(n);
    } else {
      emit_children(n);
    }
  } else if (n.is_static() && n.is_const()) {
    emit_children(n);
  } else if (current_mod->has_enum(type_name)) {
    emit_children(n);
  } else if (type_name == "logic") {
    emit_children(n);
  } else {
    n.dump_tree();
    debugbreak();
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change class/struct to module, add default clk/rst inputs, add input and
// ouptut ports to module param list.

void MtCursor::emit(MtClassSpecifier n) {
  assert(cursor == n.start());

  auto struct_lit = n.child(0);
  auto struct_name = n.get_field(field_name);
  auto struct_body = n.get_field(field_body);

  //----------

  if (!in_module_or_package) {
    assert(!current_mod);
    for (auto& mod : *source_file->modules) {
      if (mod->mod_name == struct_name.text()) {
        current_mod = mod;
        break;
      }
    }
    assert(current_mod);
  }

  in_module_or_package++;

  emit_indent();
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
          sub_cursor.emit_ws();
          break;
        case anon_sym_GT:
          sub_cursor.emit_replacement(c, ")");
          sub_cursor.skip_ws();
          break;

        case sym_parameter_declaration:
          sub_cursor.emit("parameter ");
          sub_cursor.emit_dispatch(c);
          sub_cursor.emit_ws();
          break;

        case sym_optional_parameter_declaration:
          sub_cursor.emit("parameter ");
          sub_cursor.emit_dispatch(c);
          sub_cursor.emit_ws();
          break;

        case anon_sym_COMMA:
          sub_cursor.emit_text(c);
          sub_cursor.emit_ws();
          break;

        default:
          c.dump_tree();
          assert(false);
          break;
      }
    }
    emit_newline();
  }

  emit_indent();
  emit("(");
  emit_newline();

  {
    // Save the indentation level of the struct body so we can use it in the
    // port list.
    push_indent(struct_body);

    in_ports = true;
    trim_namespaces = false;

    emit_indent();
    emit("input logic clock,");
    emit_newline();

    int port_count =
        int(current_mod->inputs->size() + current_mod->outputs->size());
    int port_index = 0;

    for (auto& input : *current_mod->inputs) {
      emit_indent();
      emit("input ");

      auto node_type = input.child(0);  // type
      auto node_decl = input.child(1);  // decl
      auto node_semi = input.child(2);  // semi

      MtCursor sub_cursor = *this;
      sub_cursor.cursor = input.start();
      sub_cursor.emit_dispatch(node_type);
      sub_cursor.emit_ws();
      sub_cursor.emit_dispatch(node_decl);

      if (port_index++ < port_count - 1) emit(",");
      emit_newline();
    }

    for (auto& output : *current_mod->outputs) {
      emit_indent();
      emit("output ");

      auto node_type = output.child(0);  // type
      auto node_decl = output.child(1);  // decl
      auto node_semi = output.child(2);  // semi

      MtCursor sub_cursor = *this;
      sub_cursor.cursor = output.start();
      sub_cursor.emit_dispatch(node_type);
      sub_cursor.emit_ws();
      sub_cursor.emit_dispatch(node_decl);

      if (port_index++ < port_count - 1) emit(",");
      emit_newline();
    }

    pop_indent(struct_body);
    emit_indent();
    emit(");");
    trim_namespaces = true;
    in_ports = false;
  }

  // Whitespace between the end of the port list and the module body.
  skip_ws();

  // Emit the module body, with a few modifications.
  // Discard the opening brace
  // Replace the closing brace with "endmodule"
  // Discard the seimcolon at the end of class{};"

  assert(struct_body.sym == sym_field_declaration_list);

  push_indent(struct_body);

  auto body_size = struct_body.child_count();
  for (int i = 0; i < body_size; i++) {
    auto c = struct_body.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE:
        skip_over(c);
        emit_ws_to_newline();
        emit_output_ports();
        emit_ws();
        break;
      case anon_sym_RBRACE:
        emit_replacement(c, "endmodule");
        break;
      case anon_sym_SEMI:
        emit_replacement(c, "");
        break;
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
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------

void MtCursor::emit_output_ports() {
  if (current_mod->submods->empty()) return;

  assert(at_newline);

  emit_indent();
  emit("// Submodule output port bindings");
  emit_newline();

  int output_count = 0;

  for (auto& submod : *current_mod->submods) {
    auto submod_type = submod.child(0);
    std::string type_name = submod_type.node_to_type();
    auto submod_mod = lib->get_mod(type_name);
    output_count += (int)submod_mod->outputs->size();
  }


  int output_index = 0;

  for (auto& submod : *current_mod->submods) {
    auto submod_type = submod.child(0);  // type
    auto submod_decl = submod.child(1);  // decl
    auto submod_semi = submod.child(2);  // semi

    std::string type_name = submod_type.node_to_type();

    auto submod_mod = lib->get_mod(type_name);

    auto inst_name = submod_decl.text();

    // Swap template arguments with the values from the template
    // instantiation.
    std::map<std::string, std::string> replacements;

    auto args = submod_type.get_field(field_arguments);

    if (args) {
      int arg_count = args.named_child_count();
      for (int i = 0; i < arg_count; i++) {
        auto key = submod_mod->modparams->at(i).name();
        auto val = args.named_child(i).text();
        replacements[key] = val;
      }
    }

    for (auto& n : *submod_mod->outputs) {
      MtCursor subcursor(lib, submod_mod->source_file, str_out);
      subcursor.quiet = quiet;
      subcursor.in_ports = true;
      subcursor.id_replacements = replacements;

      // field_declaration
      auto output_type = n.get_field(field_type);
      auto output_decl = n.get_field(field_declarator);

      subcursor.cursor = output_type.start();

      emit_indent();
      subcursor.emit_dispatch(output_type);
      subcursor.emit_ws();
      emit("%s_", inst_name.c_str());
      subcursor.emit_dispatch(output_decl);
      emit(";");
      emit_newline();

      output_index++;
    }
  }

  emit_newline();
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtExprStatement n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) {
    switch (c.sym) {
      case sym_call_expression:
        emit(MtCallExpr(c));
        break;
      case sym_assignment_expression:
        emit(MtAssignmentExpr(c));
        break;
      case sym_conditional_expression:
        emit_children(c);
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
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change "{ blah(); foo(); int x = 1; }" to "begin blah(); ... end"

void MtCursor::emit(MtCompoundStatement n) {
  assert(cursor == n.start());

  push_indent(n);

  auto body_count = n.child_count();
  for (int i = 0; i < body_count; i++) {
    auto c = n.child(i);
    switch (c.sym) {
      case anon_sym_LBRACE:
        emit_replacement(c, "begin");
        emit_ws_to_newline();
        emit_hoisted_decls(n);
        emit_ws();
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

  pop_indent(n);
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change logic<N> to logic[N-1:0]

void MtCursor::emit(MtTemplateType n) {
  assert(cursor == n.start());

  emit(n.name());
  emit_ws();

  auto args = n.args();

  bool is_logic = n.name().match("logic");
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

  cursor = n.end();
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change (template)<int param, int param> to
// #(parameter int param, parameter int param)

void MtCursor::emit(MtTemplateParamList n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) {
    switch (c.sym) {
      case anon_sym_LT:
        emit_replacement(c, "#(");
        break;
      case anon_sym_GT:
        emit_replacement(c, ")");
        break;
      case sym_parameter_declaration:
        emit("parameter ");
        emit_dispatch(c);
        break;
      case sym_optional_parameter_declaration:
        emit("parameter ");
        emit_dispatch(c);
        break;
      default:
        debugbreak();
        break;
    }
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change <param, param> to #(param, param)

void MtCursor::emit(MtTemplateArgList n) {
  assert(cursor == n.start());

  auto child_count = n.child_count();
  for (int i = 0; i < child_count; i++) {
    auto c = n.child(i);
    switch (c.sym) {
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
    if (i != child_count - 1) emit_ws();
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Enum lists do _not_ turn braces into begin/end.

void MtCursor::emit(MtEnumeratorList n) {
  assert(cursor == n.start());

  for (auto c : (MtNode&)n) {
    switch (c.sym) {
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
    emit_ws();
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Discard any trailing semicolons in the translation unit.

void MtCursor::emit(MtTranslationUnit n) {
  assert(cursor == n.start());

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
  assert(cursor == n.end());
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
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Change "return x" to "(funcname) = x" to match old Verilog return style.

void MtCursor::emit(MtReturnStatement n) {
  assert(cursor == n.start());
  auto func_name = current_function_name;
  for (auto c : (MtNode&)n) {
    switch (c.sym) {
      case anon_sym_return:
        emit_replacement(c, "%s =", func_name.c_str());
        break;
      default:
        emit_dispatch(c);
        break;
    }
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// FIXME translate types here

void MtCursor::emit(MtPrimitiveType n) {
  assert(cursor == n.start());
  emit_text(n);
  assert(cursor == n.end());
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
  assert(cursor == n.end());
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
  assert(cursor == n.end());
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
    for (auto& mod : *source_file->modules) {
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
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Replace foo.bar.baz with foo_bar_baz, so that a field expression instead
// refers to a glue expression.

void MtCursor::emit(MtFieldExpr n) {
  assert(cursor == n.start());

  auto field = n.text();
  for (auto& c : field) {
    if (c == '.') c = '_';
  }
  emit_replacement(n, field.c_str());
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtCaseStatement n) {
  assert(cursor == n.start());

  auto child_count = n.child_count();
  for (int i = 0; i < child_count; i++) {
    auto c = n.child(i);
    if (c.sym == anon_sym_case) {
      //skip_to_next_sibling(c);
      //skip_over(c);
      comment_out(c);
    } else {
      emit_dispatch(c);
    }
    if (i != child_count - 1) emit_ws();
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtSwitchStatement n) {
  assert(cursor == n.start());

  auto child_count = n.child_count();
  for (int i = 0; i < child_count; i++) {
    auto c = n.child(i);
    if (c.sym == anon_sym_switch) {
      emit_replacement(c, "case");
    } else if (c.field == field_body) {


      auto gc_count = c.child_count();
      for (int j = 0; j < gc_count; j++) {
        auto gc = c.child(j);
        if (gc.sym == anon_sym_LBRACE) {
          skip_over(gc);
        } else if (gc.sym == anon_sym_RBRACE) {
          emit_replacement(gc, "endcase");
        } else {
          emit_dispatch(gc);
        }
        if (j != gc_count - 1) emit_ws();
      }

    } else {
      emit_dispatch(c);
    }

    if (i != child_count - 1) emit_ws();
  }
  assert(cursor == n.end());
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
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Verilog doesn't use "break"

void MtCursor::emit(MtBreakStatement n) {
  assert(cursor == n.start());
  comment_out(n);
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------

void MtCursor::emit(MtFieldDeclList n) {
  assert(cursor == n.start());
  emit_children(n);
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// TreeSitter nodes slightly broken for "a = b ? c : d;"...

void MtCursor::emit(MtCondExpr n) {
  assert(cursor == n.start());
  emit_children(n);
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// Static variables become localparams at module level.

void MtCursor::emit(MtStorageSpec n) {
  assert(cursor == n.start());
  n.match("static") ? emit_replacement(n, "localparam") : comment_out(n);
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// ...er, this was something about namespace resolution?
// so this is chopping off the std:: in std::string...

void MtCursor::emit(MtQualifiedId n) {
  assert(cursor == n.start());

  if (n.text() == "std::string") {
    emit_replacement(n, "string");
    return;
  }

  if (trim_namespaces) {
    // Chop "enum::" off off "enum::enum_value"
    auto last_child = n.child(n.child_count() - 1);
    cursor = last_child.start();
    emit_dispatch(last_child);
    cursor = n.end();
  } else {
    emit_children(n);
  }
  assert(cursor == n.end());
}

//------------------------------------------------------------------------------
// If statements are basically the same.

void MtCursor::emit(MtIfStatement n) {
  assert(cursor == n.start());
  emit_children(n);
  assert(cursor == n.end());
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
  assert(cursor == n.end());
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
  if (n.is_static() && n.is_const()) {
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
    emit_ws();
    emit_dispatch(c);
  }

  assert(cursor == n.end());
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
      if (!value.is_null()) {
        emit_ws();
        emit_dispatch(value);
      }
      break;
    }

    case sym_preproc_ifdef: {
      emit_children(n);
      break;
    }

    case sym_preproc_else: {
      emit_children(n);
      break;
    }

    case sym_preproc_arg: {
      emit_text(n);
      break;
    }

    case sym_type_qualifier:
      comment_out(n);
      break;

    case sym_preproc_call:
    case sym_access_specifier:
    case sym_preproc_if:
      skip_over(n);
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

    case alias_sym_namespace_identifier:
      debugbreak();
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
  assert(cursor == n.end());
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
