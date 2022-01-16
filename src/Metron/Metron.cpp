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










  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Scanner

  std::vector<TSNode> moduleparams;
  std::vector<TSNode> inputs;
  std::vector<TSNode> outputs;
  std::vector<TSNode> localparams;
  std::vector<TSNode> fields;
  std::vector<TSNode> submodules;

  TSNode module_template;
  TSNode module_class;

  bool in_comb = false;
  bool in_seq = false;


  void find_module() {
    module_template = { 0 };
    module_class = { 0 };

    visit_tree2(root, [&](TSNode parent, TSNode child) {
      auto sp = ts_node_symbol(parent);
      auto sc = ts_node_symbol(child);

      if (sc == sym_struct_specifier || sc == sym_class_specifier) {
        if (sp == sym_template_declaration) module_template = parent;
        module_class = child;
      }
      });

  }

  void collect_moduleparams(TSNode template_node) {
    if (ts_node_is_null(template_node)) return;

    if (ts_node_symbol(template_node) != sym_template_declaration) __debugbreak();

    auto params = ts_node_child_by_field_id(template_node, field_parameters);
    for (const auto& child : params) {
      auto sc = ts_node_symbol(child);
      if (sc == sym_parameter_declaration || sc == sym_optional_parameter_declaration) moduleparams.push_back(child);
    }
  }


  void collect_fields(TSNode class_node) {
    visit_tree(class_node, [&](TSNode n) {
      if (ts_node_symbol(n) == sym_field_declaration) {
        if (field_is_input(n))  inputs.push_back(n);
        else if (field_is_output(n)) outputs.push_back(n);
        else if (field_is_param(n))  localparams.push_back(n);
        else if (field_is_module(n)) submodules.push_back(n);
        else                         fields.push_back(n);
      }
      });
  }


























  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Text handling

  const char* start(TSNode n) {
    return &source[ts_node_start_byte(n)];
  }

  const char* end(TSNode n) {
    return &source[ts_node_end_byte(n)];
  }

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

  std::string node_to_string(TSNode n) {
    return std::string(start(n), end(n));
  }

  std::string class_to_name(TSNode n) {
    auto name = ts_node_child_by_field_id(n, field_name);
    return node_to_string(name);
  }

  std::string decl_to_name(TSNode decl) {
    auto s = ts_node_symbol(decl);

    if (s == sym_identifier) {
      return node_to_string(decl);
    }
    if (s == alias_sym_field_identifier) {
      return node_to_string(decl);
    }
    else if (s == sym_array_declarator) {
      return decl_to_name(ts_node_child_by_field_id(decl, field_declarator));
    }
    else {
      printf("\n\n\n########################################\n");
      dump_tree(decl);
      printf("\n########################################\n\n\n");
      __debugbreak();
      return "?";
    }
  }

  std::string field_to_name(TSNode n) {
    auto decl = ts_node_child_by_field_id(n, field_declarator);
    return decl_to_name(decl);
  }












  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Node traversal

  typedef std::function<void(TSNode)> NodeVisitor;
  typedef std::function<void(TSNode parent, TSNode child)> NodeVisitor2;

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

  void visit_tree2(TSNode parent, NodeVisitor2 cv) {
    for (const auto& child : parent) {
      cv(parent, child);
      visit_tree2(child, cv);
    }
  }

  void dispatch_children(TSNode n) {
    visit_children(n, [&](TSNode c) { emit_dispatch(c); });
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

  void comment_out(TSNode n) {
    advance_to(n);
    emit("/* ");
    emit_body(n);
    emit(" */");
  }







  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Node debugging

  void dump_node(TSNode n, int index = 0, int field = -1, int depth = 0) {
    if (ts_node_is_null(n)) {
      printf("### NULL ###\n");
      return;
    }

    auto s = ts_node_symbol(n);

    uint32_t color = 0x00000000;
    if (s == sym_template_declaration) color = 0xAADDFF;
    if (s == sym_struct_specifier)     color = 0xFFAAFF;
    if (s == sym_class_specifier)      color = 0xFFAAFF;
    if (s == sym_expression_statement) color = 0xAAFFFF;
    if (s == sym_expression_statement) color = 0xAAFFFF;
    if (s == sym_compound_statement)   color = 0xFFFFFF;
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
  }

  void dump_tree(TSNode n, int index = 0, int field = -1, int depth = 0, int maxdepth = 255) {
    dump_node(n, index, field, depth);


    if (depth < maxdepth) {
      if (!ts_node_is_null(n)) {
        for (int i = 0; i < (int)ts_node_child_count(n); i++) {
          auto c = ts_node_child(n, i);
          dump_tree(c, i, ts_node_field_id_for_child(n, i), depth + 1, maxdepth);
        }
      }
    }
  }















  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Field introspection

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
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Generic emit() methods

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

  void emit_anon(TSNode n) {
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

  void emit_replacement(TSNode n, const char* fmt, ...) {
    assert(cursor == start(n));

    advance_to(n);

    va_list args;
    va_start(args, fmt);
    if (out) vfprintf(out, fmt, args);
    vfprintf(stdout, fmt, args);
    va_end(args);

    skip_over(n);

    assert(cursor == end(n));
  }

  //------------------------------------------------------------------------------

  void emit_error(TSNode n) {
    assert(cursor == start(n));

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
    dump_tree(n);
    emit("\n");

    cursor = end(n);

    __debugbreak();
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
    dump_tree(n);
    emit("\n");

    cursor = end(n);

    __debugbreak();
  }
























  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Typed emit() methods


  //------------------------------------------------------------------------------
  // Reformat #includes and replace .h with .sv.

  void emit_include(TSNode n) {
    assert(ts_node_symbol(n) == sym_preproc_include);

    auto node_path = ts_node_child_by_field_id(n, field_path);

    auto path = std::string(start(node_path), end(node_path));
    static regex rx_trim(R"(\.h)");
    path = std::regex_replace(path, rx_trim, ".sv");
    emit("`include %s", path.c_str());
    cursor = end(node_path);
  }

  //------------------------------------------------------------------------------
  // FIXME - change '=' to '<=' if lhs is a field

  void emit_assignment_expression(TSNode n) {
    auto exp_lv = ts_node_child(n, 0);
    auto exp_op = ts_node_child(n, 1);
    auto exp_rv = ts_node_child(n, 2);

    emit_dispatch(exp_lv);

    // need to check if lhs is a field reference
    /*
    if (in_seq) {
      advance_to(exp_op);
      emit("<");
    }
    */

    emit_leaf(exp_op);
    emit_dispatch(exp_rv);
  }

  //------------------------------------------------------------------------------
  // Replace function names with macro names where needed, comment out explicit
  // tick/tock calls.

  void emit_call_expression(TSNode n) {
    auto call_func = ts_node_child_by_field_id(n, field_function);
    auto call_args = ts_node_child_by_field_id(n, field_arguments);

    if (match(call_func, "clog2")) {
      emit_replacement(call_func, "$clog2");
      emit_dispatch(call_args);
    }
    else {
      comment_out(n);
    }

    cursor = end(n);
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

      in_comb = true;
      emit_dispatch(func_body);
      in_comb = false;

      return;
    }

    else if (is_tock) {
      skip_over(func_type);

      // don't want the space after the type
      //advance_to(func_decl);
      cursor = start(func_decl);
      
      emit_replacement(func_decl, "always_ff @(posedge clk, negedge rst_n)");

      in_seq = true;
      emit_dispatch(func_body);
      in_seq = false;

      return;
    }
    else {
      emit_replacement(n, "{function_definition}");
    }

    /*
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
    emit_dispatch(func_args);
    emit(";");

    for (int i = 0; i < (int)ts_node_child_count(func_body); i++) {
      auto c = ts_node_child(func_body, i);
      auto s = ts_node_symbol(c);

      if (s == anon_sym_LBRACE) {
        skip_over(c);
      }
      else if (s == anon_sym_RBRACE) {
        emit_replacement(c, is_task ? "endtask" : "endfunction");
      }
      else {
        emit_dispatch(c);
      }
    }
    */
  }


  //------------------------------------------------------------------------------
  // The "type" and "declarator" field tags don't seem to be on the right nodes.

  /*
  void emit_localparam(TSNode n) {
    emit("localparam");

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
  */

  //------------------------------------------------------------------------------
  // Change static const to localparam.

  void emit_field_declaration(TSNode field_decl) {
    advance_to(field_decl);

    if (field_is_input(field_decl)) {
      emit("input  ");
    }

    if (field_is_output(field_decl)) {
      emit("output ");
    }

    visit_children(field_decl, [&](TSNode child) {
      switch (ts_node_symbol(child)) {
      case sym_storage_class_specifier:
        if (match(child, "static")) {
          advance_to(child);
          emit_replacement(child, "localparam");
        }
        break;

      case sym_type_qualifier:
        if (match(child, "const")) {
          advance_to(child);
          emit_replacement(child, "/*const*/");
        }
        break;

      default:
        emit_dispatch(child);
        break;
      }
    });

    skip_over(field_decl);
  }

  //------------------------------------------------------------------------------
  // Change class/struct to module, add default clk/rst inputs, add input and
  // ouptut ports to module param list.

  void emit_class_specifier(TSNode n) {
    auto node_class = ts_node_child(n, 0);
    auto node_name = ts_node_child(n, 1);
    auto node_body = ts_node_child(n, 2);

    emit_replacement(node_class, "module");
    emit_body(node_name);
    emit("\n");
    emit("(clk, rst_n");

    for (auto i : inputs) {
      emit(", %s", field_to_name(i).c_str());
    }
      
    for (auto o : outputs) {
      emit(", %s", field_to_name(o).c_str());
    }

    emit(");\n");

    emit("  input  logic clk;\n");
    emit("  input  logic rst_n;\n");

    // FIXME add input/output placeholders here

    visit_children(node_body, [&](TSNode child) {
      auto sc = ts_node_symbol(child);
      if (sc == anon_sym_LBRACE) {
        skip_over(child);
      }
      else if (sc == anon_sym_SEMI) {
        skip_over(child);
      }
      else if (sc == anon_sym_RBRACE) {
        advance_to(child);
        emit_replacement(child, "endmodule");
      }
      else {
        emit_dispatch(child);
      }
    });
  }

  //------------------------------------------------------------------------------
  // Change "{ blah(); }" to "begin blah(); end"

  void emit_compound_statement(TSNode n) {
    visit_children(n, [&](TSNode child) {
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

  void emit_template_type(TSNode n) {
    auto node_name = ts_node_child(n, 0);
    auto node_args = ts_node_child(n, 1);

    if (match(node_name, "logic")) {
      auto template_arg = ts_node_named_child(node_args, 0);

      if (ts_node_symbol(template_arg) == sym_type_descriptor) {
        emit_replacement(node_name, "logic[");
        emit_span(start(template_arg), end(template_arg));
        emit("-1:0]");
        skip_over(n);
      }
      else if (ts_node_symbol(template_arg) == sym_number_literal) {
        if (match(template_arg, "1")) {
          emit_replacement(node_name, "logic");
          cursor = end(n);
        }
        else {
          int width = atoi(start(template_arg));
          emit_replacement(node_name, "logic[%d:0]", width-1);
          skip_over(n);
        }
      }
      else {
        emit_replacement(node_name, "logic[");
        emit("(");
        emit_span(start(template_arg), end(template_arg));
        emit(")");
        emit("-1:0]");
        skip_over(n);
      }

    }
    else {
      // FIXME change to #(param, param)
      comment_out(n);
    }
  }

  //------------------------------------------------------------------------------
  // Change <param, param> to #(param, param)

  void emit_module_parameters(TSNode n) {
    visit_children(n, [&](TSNode child) {
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

  void emit_enumerator_list(TSNode n) {
    // Enum lists do _not_ turn braces into begin/end.
    visit_children(n, [&](TSNode child) {
      auto sc = ts_node_symbol(child);
      if (sc == anon_sym_LBRACE || sc == anon_sym_RBRACE) {
        emit_body(child);
      }
      else {
        emit_dispatch(child);
      }
    });
  }

  //------------------------------------------------------------------------------

  void emit_translation_unit(TSNode n) {
    emit("/* verilator lint_off WIDTH */\n");
    emit("`default_nettype none\n");
    dispatch_children(n);
  }

  //------------------------------------------------------------------------------

  void emit_field_declaration_list(TSNode n) {
    visit_children(n, [&](TSNode child) {
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

  void emit_dispatch(TSNode n) {
    assert(cursor <= start(n));
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
    case sym_field_expression: // This needs to be flattened to module_o_blah instad of module.o_blah
    case sym_argument_list:
      dispatch_children(n);
      return;

    // For some reason the class's trailing semicolon ends up with the template decl, so we prune it here.
    case sym_template_declaration:
      visit_children(n, [&](TSNode child) {
        auto sc = ts_node_symbol(child);
        sc == anon_sym_SEMI ? skip_over(child) : emit_dispatch(child);
      });
      return;

    case alias_sym_field_identifier:
    case sym_identifier:
    case sym_true:
    case sym_false:
    case sym_number_literal:
    case sym_comment:
      emit_leaf(n);
      return;

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
      emit_module_parameters(n);
    }
    else if (s == sym_enumerator_list) {
      emit_enumerator_list(n);
    }
    else if (ts_node_has_error(n)) {
    }
    else {
      printf("\n\n\n########################################\n");
      dump_tree(n);
      printf("\n########################################\n\n\n");
      __debugbreak();

    }
  }
};


//------------------------------------------------------------------------------


int main(int argc, char** argv) {
  std::vector<std::string> inputs = {
    //"src/uart_test/uart_top.h",
    //"src/uart_test/uart_hello.h",
    //"src/uart_test/uart_tx.h",
    "src/uart_test/uart_rx.h",
  };


  for (auto& input : inputs) {

    CodeEmitter e(input.c_str(), (input + ".sv").c_str());

    e.find_module();
    e.collect_moduleparams(e.module_template);
    e.collect_fields(e.module_class);

    e.emit("//--------------------------------------------------------------------------------\n");
    e.emit("// MODULE:       ");
    e.emit("%s\n", e.class_to_name(e.module_class).c_str());

    e.emit("// MODULEPARAMS: ");
    for (auto f : e.moduleparams) {
      e.emit("%s, ", e.field_to_name(f).c_str());
    }
    e.emit("\n");

    e.emit("// INPUTS:       ");
    for (auto f : e.inputs) e.emit("%s, ", e.field_to_name(f).c_str());
    e.emit("\n");

    e.emit("// OUTPUTS:      ");
    for (auto f : e.outputs) e.emit("%s, ", e.field_to_name(f).c_str());
    e.emit("\n");

    e.emit("// LOCALPARAMS:  ");
    for (auto f : e.localparams) e.emit("%s, ", e.field_to_name(f).c_str());
    e.emit("\n");

    e.emit("// FIELDS:       ");
    for (auto f : e.fields) {
      e.emit("%s, ", e.field_to_name(f).c_str());
    }
    e.emit("\n");

    e.emit("// SUBMODULES:   ");
    for (auto f : e.submodules) e.emit("%s, ", e.field_to_name(f).c_str());
    e.emit("\n");

    e.emit("\n");
    e.emit_dispatch(e.root);
  }

  return 0;
}

//------------------------------------------------------------------------------
