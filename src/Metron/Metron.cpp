#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "tree_sitter/api.h"
#include <vector>
#include <deque>
#include <regex>
//#include <span>
#include "../Plait/TreeSymbols.h"
#include <functional>
#include "Node.h"
#include "Utils.h"
#include <stdarg.h>

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

struct CodeEmitter;

//------------------------------------------------------------------------------

struct TSNodeIt {

  TSNodeIt& operator++() {
    index++;
    return *this;
  }

  bool operator != (const TSNodeIt& n) {
    if (parent.context[0] != n.parent.context[0]) return true;
    if (parent.context[1] != n.parent.context[1]) return true;
    if (parent.context[2] != n.parent.context[2]) return true;
    if (parent.context[3] != n.parent.context[3]) return true;
    if (parent.id != n.parent.id)         return true;
    if (parent.tree != n.parent.tree)       return true;

    if (index != n.index) return true;

    return false;
  }

  TSNode operator*() const {
    return ts_node_child(parent, index);
  }

  TSNode parent;
  int index;
};

TSNodeIt begin(TSNode& parent) {
  return { parent, 0 };
}

TSNodeIt end(TSNode& parent) {
  return { parent, (int)ts_node_child_count(parent) };
}

//------------------------------------------------------------------------------

struct CodeEmitter {
  typedef CodeEmitter self;

  blob src_blob;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;
  const char* source = nullptr;
  TSNode root;
  const char* cursor = nullptr;
  FILE* out = nullptr;


  const char* get_field_name(int field_id) {
    return field_id == -1 ? nullptr : ts_language_field_name_for_id(lang, field_id);
  }

  //----------------------------------------

  CodeEmitter(const char* input_filename, const char* output_filename) {
    out = fopen(output_filename, "wb");

    src_blob = load_blob(input_filename);
    src_blob.push_back(0);

    source = (const char*)src_blob.data();
    cursor = source;
    parser = ts_parser_new();
    lang = tree_sitter_cpp();

    ts_parser_set_language(parser, lang);
    tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());

    root = ts_tree_root_node(tree);
  }

  //----------------------------------------

  ~CodeEmitter() {
    fclose(out);
    out = nullptr;

    ts_tree_delete(tree);
    ts_parser_delete(parser);

    src_blob.clear();
    lang = nullptr;
    parser = nullptr;
    tree = nullptr;
    source = nullptr;
  }

  //----------------------------------------

  bool match(TSNode n, const char* s) {
    const char* a = start(n);
    const char* b = end(n);

    while (a != b) {
      if (*a++ != *s++)  return false;
    }
    return true;
  }

  void emit_span(const char* a, const char* b) {
    if (out) fwrite(a, 1, b - a, out);
    fwrite(a, 1, b - a, stdout);
  }

  void emit(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (out) vfprintf(out, fmt, args);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }

  void emit_escaped(char s) {
    if      (s == '\n') emit("\\n");
    else if (s == '\r') emit("\\r");
    else if (s == '\t') emit("\\t");
    else if (s == '"')  emit("\\\"");
    else if (s == '\\') emit("\\\\");
    else                emit("%c", s);
  }

  void emit_escaped(const char* source, uint32_t a, uint32_t b) {
    emit("\"");
    for (; a < b; a++) {
      emit_escaped(source[a]);
    }
    emit("\"");
  }

  void emit_escaped(const std::string& s) {
    for (auto c : s) emit_escaped(c);
  }

  void print_escaped(TSNode n) {
    ::print_escaped(source, ts_node_start_byte(n), ts_node_end_byte(n));
  }


  void skip_over(TSNode n) {
    if (cursor < start(n)) {
      emit_span(cursor, start(n));
    }
    cursor = end(n);
  }

  void advance_to(TSNode n) {
    assert(cursor <= start(n));
    emit_span(cursor, start(n));
    cursor = start(n);
  }

  void advance_past(TSNode n) {
    assert(cursor >= start(n));
    assert(cursor <= end(n));
    emit_span(cursor, end(n));
    cursor = end(n);
  }

  //----------------------------------------

  void dump_node(TSNode n, int index = 0, int field = -1, int depth = 0) {
    auto s = ts_node_symbol(n);

    uint32_t color = 0x00000000;
    if (s == sym_expression_statement) color = 0xAAFFFF;
    if (s == sym_compound_statement )  color = 0xFFFFFF;
    if (s == sym_function_definition)  color = 0xAAAAFF;
    if (s == sym_field_declaration)    color = 0xFFAAAA;
    if (s == sym_comment)              color = 0xAAFFAA;

    if (color) {
      printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
      for (int i = 0; i < depth; i++) printf("|---");
    }
    else {
      for (int i = 0; i < depth; i++) printf("|   ");
    }

    printf("[%d] ", index);

    if (field != -1) printf("f%d ", field);
    if (s != -1) printf("s%d ", s);
    if (field != -1) printf("%s.", get_field_name(field));

    if (!ts_node_is_null(n) && ts_node_is_named(n) && ts_node_child_count(n)) {

      printf("%s: ", ts_node_type(n));
    }
    else if (!ts_node_is_null(n) && ts_node_is_named(n) && !ts_node_child_count(n)) {
      printf("%s: ", ts_node_type(n));
      ::print_escaped(source, ts_node_start_byte(n), ts_node_end_byte(n));
    }
    else if (ts_node_is_null(n)) {
      __debugbreak();
      printf("text: ");
      ::print_escaped(source, ts_node_start_byte(n), ts_node_end_byte(n));
    }
    else {
      // Unnamed nodes usually have their node body as their "type",
      // and their symbol is something like "aux_sym_preproc_include_token1"
      printf("lit: ");
      ::print_escaped(source, ts_node_start_byte(n), ts_node_end_byte(n));
    }

    printf("\n");

    printf("\u001b[0m");
    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);
      dump_node(c, i, ts_node_field_id_for_child(n, i), depth + 1);
    }
  }

  //------------------------------------------------------------------------------

  typedef std::function<void(TSNode)> NodeVisitor;

  void visit_children(TSNode n, NodeVisitor cv) {
    for (const auto& c : n) {
      cv(c);
    }
  }

  void visit_tree(TSNode n, NodeVisitor cv) {
    cv(n);
    for (const auto& c : n) {
      visit_tree(c, cv);
    }
  }

  void dispatch_children(TSNode n) {
    visit_children(n, [&](TSNode c) { emit_dispatch(c); });
  }

  //------------------------------------------------------------------------------

  const char* start(TSNode n) {
    return &source[ts_node_start_byte(n)];
  }

  const char* end(TSNode n) {
    return &source[ts_node_end_byte(n)];
  }

  void emit_body(TSNode n) {
    assert(cursor <= start(n));
    emit_span(cursor, start(n));
    emit_span(start(n), end(n));
    cursor = end(n);
  }

  void emit_leaf(TSNode n) {
    assert(!ts_node_is_null(n) && !ts_node_child_count(n));
    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_error(TSNode n) {
    if (ts_node_is_named(n)) {
      emit("XXX %s ", ts_node_type(n));
      emit("XXX");
      emit_body(n);
      emit("XXX");
    }
    else {
      emit("XXXXXXX");
      emit_body(n);
      emit("XXXXXXX");
    }
    emit("\n");
    emit("\n");
    dump_node(n);
    emit("\n");

    cursor = end(n);
  }

  //------------------------------------------------------------------------------

  void emit_placeholder(TSNode n) {
    if (ts_node_is_named(n)) {
      emit("%s ", ts_node_type(n));
      emit("<<<");
      emit_body(n);
      emit(">>>");
    }
    else {
      emit("<<<");
      emit_body(n);
      emit(">>>");
    }

    emit("\n");
    emit("\n");
    dump_node(n);
    emit("\n");

    cursor = end(n);
  }

  //------------------------------------------------------------------------------

  void emit_access_specifier(TSNode n) {
    advance_to(n);
    emit("// ");
    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_comment(TSNode n) {
    if (cursor > start(n)) {
      int x = 0;
      x++;
    }

    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_include(TSNode n) {
    assert(ts_node_symbol(n) == sym_preproc_include);

    auto node_path = ts_node_child_by_field_id(n, field_path);

    auto path = std::string(start(node_path), end(node_path));
    static regex rx_trim(R"(\.h)");
    path = std::regex_replace(path, rx_trim, ".sv");
    emit("`include %s\n", path.c_str());
    cursor = end(node_path);
  }

  //------------------------------------------------------------------------------

  void emit_binary_expression(TSNode n) {
    auto exp_l = ts_node_child(n, 0);
    auto exp_o = ts_node_child(n, 1);
    auto exp_r = ts_node_child(n, 2);

    emit_dispatch(exp_l);
    emit_leaf(exp_o);
    emit_dispatch(exp_r);
  }

  //------------------------------------------------------------------------------

  void emit_lvalue(TSNode n) {
    if (ts_node_symbol(n) == sym_identifier) {
      emit_leaf(n);
    }
    else {
      emit_error(n);
    }
  }

  //------------------------------------------------------------------------------

  void emit_rvalue(TSNode n) {
    emit_dispatch(n);
  }

  //------------------------------------------------------------------------------

  void emit_assignment_expression(TSNode n) {
    auto exp_lv = ts_node_child(n, 0);
    auto exp_op = ts_node_child(n, 1);
    auto exp_rv = ts_node_child(n, 2);

    emit_lvalue(exp_lv);
    emit_leaf(exp_op);
    emit_rvalue(exp_rv);
  }

  //------------------------------------------------------------------------------

  void emit_replacement(TSNode n, const char* r) {
    advance_to(n);
    printf("%s", r);
    skip_over(n);
  }

  //------------------------------------------------------------------------------

  void emit_call_expression(TSNode n) {

    auto call_func = ts_node_child(n, 0);
    auto call_args = ts_node_child(n, 1);

    if (match(call_func, "clog2")) {
      emit_replacement(call_func, "$clog2");
    }
    else {
      emit_leaf(call_func);
    }

    dispatch_children(call_args);
  }

  //------------------------------------------------------------------------------

  void emit_number_literal(TSNode n) {
    emit_leaf(n);
  }

  //------------------------------------------------------------------------------

  void emit_return_statement(TSNode n) {
    auto ret_keyword = ts_node_child(n, 0);
    auto ret_expr = ts_node_child(n, 1);
    auto ret_tail = ts_node_child(n, 2);

    emit_leaf(ret_keyword);
    emit_dispatch(ret_expr);
    emit_leaf(ret_tail);
  }

  //------------------------------------------------------------------------------

  void emit_expression_statement(TSNode n) {
    auto statement_body = ts_node_child(n, 0);
    auto statement_tail = ts_node_child(n, 1);

    emit_dispatch(statement_body);
    emit_leaf(statement_tail);
  }

  //------------------------------------------------------------------------------

  void emit_function_arg(TSNode n) {
    auto param_type = ts_node_child_by_field_id(n, field_type);
    auto param_decl = ts_node_child_by_field_id(n, field_declarator);

    emit_leaf(param_type);
    emit_leaf(param_decl);
  }

  //------------------------------------------------------------------------------

  void emit_function_definition(TSNode n) {

    auto func_type = ts_node_child_by_field_id(n, field_type);
    auto func_decl = ts_node_child_by_field_id(n, field_declarator);
    auto func_body = ts_node_child_by_field_id(n, field_body);
    auto task_name = ts_node_child_by_field_id(func_decl, field_declarator);

    advance_to(func_type);

    bool is_task = match(func_type, "void");
    bool is_tick = is_task && match(task_name, "tick");
    bool is_tock = is_task && match(task_name, "tock");

    if (is_tick) {
      emit("always_comb");
      cursor = end(func_decl);
      for (int i = 0; i < (int)ts_node_child_count(func_body); i++) {
        auto c = ts_node_child(func_body, i);
        auto s = ts_node_symbol(c);

        if (s == anon_sym_LBRACE) {
          //emit_replacement(c, "bakjdsf");
          //skip_over(c);
          emit_dispatch(c);

        }
        else if (s == anon_sym_RBRACE) {
          emit_dispatch(c);
        }
        else {
          emit_dispatch(c);
        }
      }
      skip_over(n);
      return;
    }

    if (is_tock) {
      emit("always_ff @(posedge clk, negedge rst_n) begin : tock");
      cursor = end(func_decl);
      for (int i = 0; i < (int)ts_node_child_count(func_body); i++) {
        auto c = ts_node_child(func_body, i);
        auto s = ts_node_symbol(c);

        if (s == anon_sym_LBRACE) {
          skip_over(c);
        }
        else if (s == anon_sym_RBRACE) {
          skip_over(c);
          emit("end");
        }
        else {
          emit_dispatch(c);
        }
      }
      skip_over(n);
      return;
    }


    if (is_task) {
      skip_over(func_type);
      emit("task");
      emit_body(task_name);
    }
    else {
      emit("function ");
      emit_body(func_type);
      emit_body(task_name);
    }


    auto func_args = ts_node_child_by_field_id(func_decl, field_parameters);
    visit_children(func_args, [&](TSNode n) {
      advance_to(n);
      if (ts_node_symbol(n) == sym_parameter_declaration) {
        emit_function_arg(n);
      }
      else {
        emit_leaf(n);
      }
    });

    emit(";");

    for (int i = 0; i < (int)ts_node_child_count(func_body); i++) {
      auto c = ts_node_child(func_body, i);
      auto s = ts_node_symbol(c);

      if (s == anon_sym_LBRACE) {
        skip_over(c);
      }
      else if (s == anon_sym_RBRACE) {
        skip_over(c);
        if (is_task) {
          emit("endtask");
        }
        else {
          emit("endfunction");
        }
      }
      else {
        emit_dispatch(c);
      }
    }
  }

  //------------------------------------------------------------------------------
  
  /*
  [0] s236 field_declaration:
  |   [0] f32 s78 type.primitive_type: "int"
  |   [1] f9 s392 declarator.field_identifier: "o_serial"
  |   [2] s39 lit: ";"

  [0] s236 field_declaration:
  |   [0] f32 s395 type.type_identifier: "bit"
  |   [1] f9 s392 declarator.field_identifier: "o_done"
  |   [2] s39 lit: ";"

  [0] s236 field_declaration:
  |   [0] f32 s321 type.template_type:
  |   |   [0] f22 s395 name.type_identifier: "logic"
  |   |   [1] f3 s324 arguments.template_argument_list:
  |   |   |   [0] s36 lit: "<"
  |   |   |   [1] s112 number_literal: "32"
  |   |   |   [2] s33 lit: ">"
  |   [1] f9 s392 declarator.field_identifier: "o_sum"
  |   [2] s39 lit: ";"

  FIELDS:
  [0] s236 field_declaration:
  |   [0] f32 s395 type.type_identifier: "uart_hello"
  |   [1] f9 s392 declarator.field_identifier: "hello"
  | [3] s39 lit : ";"
  | [3] s39 lit : ";"
  */

  bool field_is_primitive(TSNode n) {

    auto node_type = ts_node_child_by_field_id(n, field_type);
    auto node_decl = ts_node_child_by_field_id(n, field_declarator);

    // Primitive types are primitive types.
    if (ts_node_symbol(node_type) == sym_primitive_type) return true;

    // Logic arrays are primitive types.
    if (ts_node_symbol(node_type) == sym_template_type) {
      auto templ_name = ts_node_child_by_field_id(node_type, field_name);
      if (match(templ_name, "logic")) return true;
    }

    // Bits are primitive types.
    if (match(node_type, "bit")) return true;

    return false;
  }

  bool field_is_module(TSNode n) {
    return !field_is_primitive(n);
  }


  bool field_is_static(TSNode n) {
    for (auto c : n) {
      if (ts_node_symbol(c) == sym_storage_class_specifier) {
        if (match(c, "static")) return true;
      }
    }
    return false;
  }

  bool field_is_const(TSNode n) {
    for (auto c : n) {
      if (ts_node_symbol(c) == sym_type_qualifier) {
        if (match(c, "const")) return true;
      }
    }
    return false;
  }

  bool field_is_param(TSNode n) {
    return field_is_static(n) && field_is_const(n);
  }

  bool field_is_input(TSNode n) {
    if (field_is_static(n) || field_is_const(n)) return false;

    auto name = ts_node_child_by_field_id(n, field_declarator);
    std::string text(start(name), end(name));
    return text.starts_with("i_");
  }

  bool field_is_output(TSNode n) {
    if (field_is_static(n) || field_is_const(n)) return false;

    auto name = ts_node_child_by_field_id(n, field_declarator);
    std::string text(start(name), end(name));
    return text.starts_with("o_");
  }

  //------------------------------------------------------------------------------

  std::vector<TSNode> moduleparams;
  std::vector<TSNode> inputs;
  std::vector<TSNode> outputs;
  std::vector<TSNode> localparams;
  std::vector<TSNode> fields;
  std::vector<TSNode> modules;

  /*
  void collect_moduleparams(TSNode template_node) {

    auto params = ts_node_child_by_field_id(template_node, field_parameters);
    for (auto c : params) {
      if (ts_node_symbol(c) == sym_parameter_declaration) moduleparams.push_back(c);
    }
  }
  */

  void collect_fields(TSNode class_node) {
    visit_tree(class_node, [&] (TSNode n) {
      if (ts_node_symbol(n) == sym_field_declaration) {
        if      (field_is_input(n))  inputs.push_back(n);
        else if (field_is_output(n)) outputs.push_back(n);
        else if (field_is_param(n))  localparams.push_back(n);
        else if (field_is_module(n)) modules.push_back(n);
        else                         fields.push_back(n);
      }
    });
  }

  //------------------------------------------------------------------------------
  // The "type" and "declarator" field tags don't seem to be on the right nodes.

  void emit_param(TSNode n) {
    printf("localparam");

    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);
      switch (ts_node_symbol(c)) {
      case sym_storage_class_specifier:
      case sym_type_qualifier:
        skip_over(c);
        continue;
      default:
        emit_dispatch(c);
        continue;
      }
    }

    skip_over(n);
  }


  void emit_field_declaration(TSNode n) {
    bool is_static = false;
    bool is_input_port = false;
    bool is_output_port = false;

    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);
      if (ts_node_symbol(c) == alias_sym_field_identifier) {
        std::string name(start(c), end(c));
        if (name.starts_with("o_")) {
          //printf("\nOUTPUT PORT %s\n", name.c_str());
          is_output_port = true;
        }
        if (name.starts_with("i_")) {
          //printf("\nINPUT PORT %s\n", name.c_str());
          is_input_port = true;
        }
      }
    }

    //if (is_input_port || is_output_port) {
    //  skip_over(n);
    //  return;
    //}

    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);

      switch (ts_node_symbol(c)) {
      case sym_storage_class_specifier:
        if (match(c, "static")) {
          is_static = true;
          advance_to(c);
          printf("localparam");
          skip_over(c);
        }
        else {
          emit_error(c);
          skip_over(c);
        }
        continue;

      case sym_type_qualifier:
        if (match(c, "const")) emit_replacement(c, "/*const*/");
        skip_over(c);
        continue;

      case sym_primitive_type:
      case alias_sym_type_identifier:
      case alias_sym_field_identifier:
      case sym_number_literal:
      case sym_binary_expression:
      case sym_call_expression:
      case anon_sym_SEMI:
      case anon_sym_EQ:
      case sym_template_type:
        emit_dispatch(c);
        continue;

      default:
        dump_node(c);
        __debugbreak();
        continue;
      }
    }

    skip_over(n);
  }

  //------------------------------------------------------------------------------

  void emit_declaration(TSNode n) {
    emit_placeholder(n);
  }

  //------------------------------------------------------------------------------

  void emit_named_children(TSNode n) {
    for (uint32_t i = 0; i < ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);

      if (!ts_node_is_named(c)) {
        skip_over(c);
      }
      else {
        emit_dispatch(c);
      }
    }
  }

  //------------------------------------------------------------------------------
  // class_specifier : class_head '{' [ member_specification ] '}'

  void emit_class_specifier(TSNode n) {
    auto node_class = ts_node_child(n, 0);
    auto node_name = ts_node_child(n, 1);
    auto node_body = ts_node_child(n, 2);

    emit_replacement(node_class, "module");
    emit_body(node_name);
    emit("(input logic clk, input logic rst); ");

    dispatch_children(node_body);

    emit("endmodule");
  }

  //------------------------------------------------------------------------------

  int get_indent(TSNode n) {
    return ts_node_start_point(n).column;
  }

  //------------------------------------------------------------------------------

  void emit_if_statement(TSNode n) {
    dispatch_children(n);
  }

  //------------------------------------------------------------------------------

  void emit_for_statement(TSNode n) {
    dispatch_children(n);
  }

  //------------------------------------------------------------------------------

  void emit_compound_statement(TSNode n) {
    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      auto c = ts_node_child(n, i);
      auto s = ts_node_symbol(c);

      if (s == anon_sym_LBRACE) {
        emit_replacement(c, "begin");
      }
      else if (s == anon_sym_RBRACE) {
        emit_replacement(c, "end");
      }
      else {
        emit_dispatch(c);
      }
    }
  }

  //------------------------------------------------------------------------------

  void emit_template_type(TSNode n) {
    auto node_name = ts_node_child(n, 0);
    auto node_args = ts_node_child(n, 1);

    if (match(node_name, "logic")) {
      emit_replacement(node_name, "logic[");
      auto template_arg = ts_node_named_child(node_args, 0);

      if (ts_node_symbol(template_arg) == sym_type_descriptor) {
        emit_span(start(template_arg), end(template_arg));
      }
      else if (ts_node_symbol(template_arg) == sym_number_literal) {
        emit_span(start(template_arg), end(template_arg));
      }
      else {
        emit("(");
        emit_span(start(template_arg), end(template_arg));
        emit(")");
      }

      emit("-1:0]");
    }
    else {
      assert(false);
    }

    skip_over(n);
  }

  //------------------------------------------------------------------------------

  void emit_dispatch(TSNode n) {
    advance_to(n);
    auto s = ts_node_symbol(n);

    if (s == anon_sym_LBRACE) {
      emit_replacement(n, "begin");
    }
    else if (s == anon_sym_RBRACE) {
      emit_replacement(n, "end");
    }
    else if (s == sym_translation_unit) {
      emit("/* verilator lint_off WIDTH */\n");
      emit("`default_nettype none\n");
      dispatch_children(n);
    }
    else if (s == sym_primitive_type) {
      // FIXME translate types here
      emit_body(n);
    }
    else if (s == alias_sym_type_identifier) {
      // FIXME translate types here
      emit_body(n);
    }
    else if (s == alias_sym_field_identifier) {
      emit_leaf(n);
    }
    else if (s == sym_field_declaration) {
      emit_field_declaration(n);
    }
    else if (s == sym_access_specifier) {
      emit_access_specifier(n);
    }
    else if (s == sym_if_statement) {
      emit_if_statement(n);
    }
    else if (s == sym_class_specifier || s == sym_struct_specifier) {
      emit_class_specifier(n);
    }
    else if (s == sym_comment) {
      emit_comment(n);
    }
    else if (s == sym_expression_statement) {
      emit_expression_statement(n);
    }
    else if (s == sym_function_definition) {
      emit_function_definition(n);
    }
    else if (s == sym_preproc_if) {
      // FIXME check preprocessor condition?
    }
    else if (s == sym_preproc_include) {
      emit_include(n);
    }
    else if (s == sym_return_statement) {
      emit_return_statement(n);
    }
    else if (s == sym_declaration) {
      emit_declaration(n);
    }
    else if (s == sym_for_statement) {
      emit_for_statement(n);
    }
    else if (s == sym_number_literal) {
      emit_number_literal(n);
    }
    else if (s == sym_identifier) {
      emit_leaf(n);
    }
    else if (s == sym_call_expression) {
      emit_call_expression(n);
    }
    else if (s == sym_binary_expression) {
      emit_binary_expression(n);
    }
    else if (s == sym_assignment_expression) {
      emit_assignment_expression(n);
    }
    else if (s == sym_parenthesized_expression) {
      dispatch_children(n);
    }
    else if (s == sym_true) {
      emit_leaf(n);
    }
    else if (s == sym_number_literal) {
      emit_leaf(n);
    }
    else if (s == sym_compound_statement) {
      emit_compound_statement(n);
    }
    else if (s == sym_template_type) {
      emit_template_type(n);
    }
    else if (s == sym_parameter_declaration) {
      dispatch_children(n);
    }
    else if (s == sym_optional_parameter_declaration) {
      dispatch_children(n);
    }
    else if (s == sym_template_parameter_list) {
      for (int i = 0; i < (int)ts_node_child_count(n); i++) {
        auto c = ts_node_child(n, i);
        auto s = ts_node_symbol(c);

        if (s == anon_sym_LT) {
          emit_replacement(c, "#(");
        }
        else if (s == anon_sym_GT) {
          emit_replacement(c, ")");
        }
        else if (s == anon_sym_COMMA) {
          emit_dispatch(c);
        }
        else if (s == sym_parameter_declaration) {
          advance_to(c);
          emit("parameter ");
          emit_dispatch(c);
        }
        else if (s == sym_optional_parameter_declaration) {
          advance_to(c);
          emit("parameter ");
          emit_dispatch(c);
        }
        else {
          __debugbreak();
        }
      }
    }
    else if (s == sym_template_declaration) {
      for (int i = 0; i < (int)ts_node_child_count(n); i++) {
        auto c = ts_node_child(n, i);
        auto s = ts_node_symbol(c);

        if (s == anon_sym_template) {
          skip_over(c);
        }
        else if (s == sym_template_parameter_list) {
          emit_dispatch(c);
        }
        else if (s == sym_class_specifier || s == sym_struct_specifier) {
          emit_dispatch(c);
        }
        else {
          skip_over(c);
        }
      }
    }
    else if (s == sym_condition_clause) {
      dispatch_children(n);
    }
    else if (s == sym_unary_expression) {
      dispatch_children(n);
    }
    else if (s == sym_preproc_call) {
      skip_over(n);
    }
    else if (ts_node_has_error(n)) {
    }
    else if (!ts_node_is_named(n)) {
      emit_leaf(n);
    }
    else {
      emit_error(n);
    }
  }
};


//------------------------------------------------------------------------------


int main(int argc, char** argv) {
  //const char* input_filename = "src/uart_test/uart_tx.h";
  //const char* output_filename = "src/uart_test/uart_tx.gen.sv";

  //const char* input_filename = "src/uart_test/uart_top.h";
  //const char* output_filename = "src/uart_test/uart_top.gen.sv";

  const char* input_filename = "src/uart_test/uart_rx.h";
  const char* output_filename = "src/uart_test/uart_rx.gen.sv";

  CodeEmitter e(input_filename, output_filename);

  e.dump_node(e.root);

  e.collect_fields(e.root);

  printf("LOCALPARAMS:\n");
  for (auto f : e.localparams) { e.dump_node(f); printf("\n"); }
  printf("\n");

  printf("INPUTS:\n");
  for (auto f : e.inputs) { e.dump_node(f); printf("\n"); }
  printf("\n");

  printf("OUTPUTS:\n");
  for (auto f : e.outputs) { e.dump_node(f); printf("\n"); }
  printf("\n");

  printf("FIELDS:\n");
  for (auto f : e.fields) { e.dump_node(f); printf("\n"); }
  printf("\n");

  printf("MODULES:\n");
  for (auto f : e.modules) { e.dump_node(f); printf("\n"); }
  printf("\n");

  //e.emit_dispatch(e.root);
  //e.print_tsnode(e.root, 0, -1, 0);

  return 0;
}

//------------------------------------------------------------------------------
