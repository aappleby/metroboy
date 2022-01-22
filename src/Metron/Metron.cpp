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

struct Module {

  Module() {
  }

  ~Module() {

    ts_tree_delete(tree);
    ts_parser_delete(parser);

    src_blob.clear();
    lang = nullptr;
    parser = nullptr;
    tree = nullptr;
    source = nullptr;
  }

  void load(const std::string& input_filename, const std::string& output_filename) {
    printf("loading %s\n", input_filename.c_str());
    this->input_filename = input_filename;
    this->output_filename = output_filename;

    parser = ts_parser_new();
    lang = tree_sitter_cpp();
    ts_parser_set_language(parser, lang);

    src_blob = load_blob(input_filename.c_str());
    src_blob.push_back(0);

    source = (const char*)src_blob.data();
    tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
    root = ts_tree_root_node(tree);

    find_module();
    collect_moduleparams();
    collect_fields();
  }

  std::string input_filename;
  std::string output_filename;

  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;

  blob src_blob;
  TSTree* tree = nullptr;
  const char* source = nullptr;
  TSNode root;

  std::vector<TSNode> moduleparams;
  std::vector<TSNode> inputs;
  std::vector<TSNode> outputs;
  std::vector<TSNode> localparams;
  std::vector<TSNode> fields;
  std::vector<TSNode> submodules;

  TSNode module_template = { 0 };
  TSNode module_class = { 0 };
  TSNode module_param_list = { 0 };
  std::string module_name;

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
    
    if (field != -1) {
      printf("%s.", ts_language_field_name_for_id(lang, field));
    }

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

  std::string body(TSNode n) {
    return std::string(start(n), end(n));
  }

  bool match(TSNode n, const char* s) {
    const char* a = start(n);
    const char* b = end(n);

    while (a != b) {
      if (*a++ != *s++)  return false;
    }
    return true;
  }

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Field introspection

  std::string class_to_name(TSNode n) {
    auto name = ts_node_child_by_field_id(n, field_name);
    return body(name);
  }

  std::string decl_to_name(TSNode decl) {
    auto s = ts_node_symbol(decl);

    if (s == sym_identifier) {
      return body(decl);
    }
    if (s == alias_sym_field_identifier) {
      return body(decl);
    }
    else if (s == sym_array_declarator) {
      return decl_to_name(ts_node_child_by_field_id(decl, field_declarator));
    }
    else {
      __debugbreak();
      return "?";
    }
  }

  std::string field_to_name(TSNode n) {
    auto decl = ts_node_child_by_field_id(n, field_declarator);
    return decl_to_name(decl);
  }

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
    return body(name).starts_with("i_");
  }

  bool field_is_output(TSNode n) {
    if (field_is_static(n) || field_is_const(n)) return false;

    auto name = ts_node_child_by_field_id(n, field_declarator);
    return body(name).starts_with("o_");
  }

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Scanner

  void find_module() {
    module_template = { 0 };
    module_class = { 0 };

    visit_tree2(root, [&](TSNode parent, TSNode child) {
      auto sp = ts_node_symbol(parent);
      auto sc = ts_node_symbol(child);

      if (sc == sym_struct_specifier || sc == sym_class_specifier) {
        if (sp == sym_template_declaration) module_template = parent;
        module_class = child;

        //dump_tree(module_class, 0, -1, 0, 2);
        auto name_node = ts_node_child_by_field_id(module_class, field_name);
        module_name = body(name_node);
      }
      });

  }

  void collect_moduleparams() {
    if (ts_node_is_null(module_template)) return;

    if (ts_node_symbol(module_template) != sym_template_declaration) __debugbreak();

    auto params = ts_node_child_by_field_id(module_template, field_parameters);
    for (const auto& child : params) {
      auto sc = ts_node_symbol(child);
      if (sc == sym_parameter_declaration || sc == sym_optional_parameter_declaration) moduleparams.push_back(child);
    }
  }


  void collect_fields() {
    visit_tree(module_class, [&](TSNode n) {
      if (ts_node_symbol(n) == sym_field_declaration) {
        if      (field_is_input(n))  inputs.push_back(n);
        else if (field_is_output(n)) outputs.push_back(n);
        else if (field_is_param(n))  localparams.push_back(n);
        else if (field_is_module(n)) submodules.push_back(n);
        else                         fields.push_back(n);
      }
      });
  }
};











//------------------------------------------------------------------------------

struct ModLibrary {
  void reset() {
    for (auto module : modules) delete module;
    modules.clear();
  }


  Module* find_module(const std::string& module_name) {
    for (auto mod : modules) {
      if (mod->module_name == module_name) return mod;
    }
    return nullptr;
  }

  /*
  Module* find_submodule(const std::string& name) {
    for (auto submod : modules) {
      if (submod->module_name == name) {
        return submod;
      }
    }
    return nullptr;
  }
  */

  void load(const std::string& input_filename, const std::string& output_filename) {
    auto mod = new Module();
    mod->load(input_filename, output_filename);
    modules.push_back(mod);
  }

  std::vector<Module*> modules;
};











struct ModCursor {
  ModLibrary* mod_lib;
  Module* mod;
  const char* cursor = nullptr;
  FILE* out = nullptr;
  bool in_init = false;
  bool in_comb = false;
  bool in_seq = false;

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // Generic emit() methods

  void emit_span(const char* a, const char* b) {
    if (out) fwrite(a, 1, b - a, out);
    fwrite(a, 1, b - a, stdout);
  }

  void emit(TSNode n) {
    emit_span(mod->start(n), mod->end(n));
  }

  void emit(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (out) vfprintf(out, fmt, args);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }

  void print_escaped(TSNode n) {
    ::print_escaped(mod->source, ts_node_start_byte(n), ts_node_end_byte(n));
  }

  void skip_over(TSNode n) {
    if (cursor < mod->start(n)) {
      emit_span(cursor, mod->start(n));
    }
    cursor = mod->end(n);
  }

  void advance_to(TSNode n) {
    assert(cursor <= mod->start(n));
    emit_span(cursor, mod->start(n));
    cursor = mod->start(n);
  }

  void advance_past(TSNode n) {
    assert(cursor >= mod->start(n));
    assert(cursor <= mod->end(n));
    emit_span(cursor, mod->end(n));
    cursor = mod->end(n);
  }

  void comment_out(TSNode n) {
    advance_to(n);
    emit("/* ");
    emit_body(n);
    emit(" */");
  }

  void emit_body(TSNode n) {
    assert(cursor <= mod->start(n));
    emit_span(cursor, mod->start(n));
    emit(n);
    cursor = mod->end(n);
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

  void emit_error(TSNode n) {
    assert(cursor == mod->start(n));
    printf("\n");
    printf("########################################\n");
    mod->dump_tree(n);
    printf("########################################\n");
    __debugbreak();
  }

  //------------------------------------------------------------------------------

























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

    auto path = mod->body(node_path);
    static regex rx_trim(R"(\.h)");
    path = std::regex_replace(path, rx_trim, ".sv");
    emit("`include %s", path.c_str());
    cursor = mod->end(node_path);
  }

  //------------------------------------------------------------------------------
  // FIXME - change '=' to '<=' if lhs is a field

  void emit_assignment_expression(TSNode n) {
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

  void emit_call_expression(TSNode n) {
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

  TSNode current_function_name = { 0 };

  void emit_function_definition(TSNode n) {

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

  /*
  |---|---|---|---[8] s236 field_declaration:
  |   |   |   |   |   [0] f32 s395 type.type_identifier: "uart_hello"
  |   |   |   |   |   [1] f9 s392 declarator.field_identifier: "hello"
  |   |   |   |   |   [2] s39 lit: ";"
  |---|---|---|---[9] s236 field_declaration:
  |   |   |   |   |   [0] f32 s321 type.template_type:
  |   |   |   |   |   |   [0] f22 s395 name.type_identifier: "uart_tx"
  |   |   |   |   |   |   [1] f3 s324 arguments.template_argument_list:
  |   |   |   |   |   |   |   [0] s36 lit: "<"
  |   |   |   |   |   |   |   [1] s264 type_descriptor:
  |   |   |   |   |   |   |   |   [0] f32 s395 type.type_identifier: "cycles_per_bit"
  |   |   |   |   |   |   |   [2] s33 lit: ">"
  |   |   |   |   |   [1] f9 s392 declarator.field_identifier: "tx"
  |   |   |   |   |   [2] s39 lit: ";"
  |---|---|---|---[10] s236 field_declaration:
  |   |   |   |   |   [0] f32 s321 type.template_type:
  |   |   |   |   |   |   [0] f22 s395 name.type_identifier: "uart_rx"
  |   |   |   |   |   |   [1] f3 s324 arguments.template_argument_list:
  |   |   |   |   |   |   |   [0] s36 lit: "<"
  |   |   |   |   |   |   |   [1] s264 type_descriptor:
  |   |   |   |   |   |   |   |   [0] f32 s395 type.type_identifier: "cycles_per_bit"
  |   |   |   |   |   |   |   [2] s33 lit: ">"
  |   |   |   |   |   [1] f9 s392 declarator.field_identifier: "rx"
  |   |   |   |   |   [2] s39 lit: ";"
  */

  void emit_glue_declaration(TSNode decl, const std::string& prefix) {
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


  void emit_field_declaration(TSNode decl) {
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

  void emit_class_specifier(TSNode n) {
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

  void emit_compound_statement(TSNode n) {
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

  void emit_template_type(TSNode n) {
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
          emit_replacement(type_name, "logic[%d:0]", width-1);
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

  void emit_module_parameters(TSNode n) {
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

  void emit_template_parameters(TSNode n) {
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

  void emit_enumerator_list(TSNode n) {
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

  void emit_translation_unit(TSNode n) {
    emit("/* verilator lint_off WIDTH */\n");
    emit("`default_nettype none\n");
    
    mod->visit_children(n, [&](TSNode child) {
      auto sc = ts_node_symbol(child);
      sc == anon_sym_SEMI ? skip_over(child) : emit_dispatch(child);
    });
  }

  //------------------------------------------------------------------------------
  // Structs/classes get "begin/end" instead of {}.

  void emit_field_declaration_list(TSNode n) {
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

  void emit_dispatch(TSNode n) {
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
};





















//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  std::vector<std::string> inputs = {
    "src/uart_test/uart_top.h",
    "src/uart_test/uart_hello.h",
    "src/uart_test/uart_tx.h",
    "src/uart_test/uart_rx.h",
  };


  ModLibrary lib;
  for (auto& input : inputs) {
    lib.load(input.c_str(), (input + ".sv").c_str());
  }

  for (auto& module : lib.modules) {

    ModCursor cursor = {
      &lib,
      module,
      module->source,
      fopen(module->output_filename.c_str(), "wb"),
    };

    cursor.emit("//--------------------------------------------------------------------------------\n");
    cursor.emit("// MODULE:       ");
    cursor.emit("%s\n", module->class_to_name(module->module_class).c_str());

    cursor.emit("// MODULEPARAMS: ");
    for (auto f : module->moduleparams) {
      cursor.emit("%s, ", module->field_to_name(f).c_str());
    }
    cursor.emit("\n");

    cursor.emit("// INPUTS:       ");
    for (auto f : module->inputs) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// OUTPUTS:      ");
    for (auto f : module->outputs) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// LOCALPARAMS:  ");
    for (auto f : module->localparams) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// FIELDS:       ");
    for (auto f : module->fields) {
      cursor.emit("%s, ", module->field_to_name(f).c_str());
    }
    cursor.emit("\n");

    cursor.emit("// SUBMODULES:   ");
    for (auto f : module->submodules) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("\n");
    cursor.cursor = module->source;
    cursor.emit_dispatch(module->root);
    printf("\n\n\n");

    //e.mod->dump_tree(e.mod->root);
    //printf("\n\n");
  }


  return 0;
}

//------------------------------------------------------------------------------
