#include "MtModule.h"
#include <assert.h>

#include "MtIterator.h"
#include "Platform.h"
#include "../Plait/TreeSymbols.h"

#include <assert.h>
#include <stdarg.h>

#pragma warning(disable:4996) // unsafe fopen()

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

//------------------------------------------------------------------------------

blob load_blob(const char* filename) {
  FILE* f = fopen(filename, "rb");
  assert(f);

  blob result;
  fseek(f, 0, SEEK_END);
  result.resize(ftell(f));
  fseek(f, 0, SEEK_SET);

  auto res = fread(result.data(), 1, result.size(), f);
  fclose(f);
  return result;
}

void print_escaped(char s) {
  if (s == '\n') printf("\\n");
  else if (s == '\r') printf("\\r");
  else if (s == '\t') printf("\\t");
  else if (s == '"')  printf("\\\"");
  else if (s == '\\') printf("\\\\");
  else                printf("%c", s);
}

void print_escaped(const char* source, uint32_t a, uint32_t b) {
  printf("\"");
  for (; a < b; a++) {
    print_escaped(source[a]);
  }
  printf("\"");
}

//------------------------------------------------------------------------------

MtModule::MtModule() {
}

MtModule::~MtModule() {

  ts_tree_delete(tree);
  ts_parser_delete(parser);

  src_blob.clear();
  lang = nullptr;
  parser = nullptr;
  tree = nullptr;
  source = nullptr;
}

void MtModule::load(const std::string& input_filename, const std::string& output_filename) {
  printf("loading %s\n", input_filename.c_str());
  this->input_filename = input_filename;
  this->output_filename = output_filename;

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);

  src_blob = load_blob(input_filename.c_str());

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  root = MtHandle::from_tree(tree, source);

  find_module();
  collect_moduleparams();
  collect_fields();

  // Verify that tick()/tock() obey read/write ordering rules.

  check_dirty_tick(node_tick);
  check_dirty_tock(node_tock);
}

//------------------------------------------------------------------------------

void MtModule::print_error(MtHandle n, const char* fmt, ...) {
  printf("\n########################################\n");

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  printf("@%04d: ", ts_node_start_point(n.node).row + 1);
  
  {
    auto start = &source[n.start_byte()];

    auto a = start;
    auto b = start;
    while (a > source     && *a != '\n' && *a != '\r') a--;
    while (b < source_end && *b != '\n' && *b != '\r') b++;

    if (*a == '\n' || *a == '\r') a++;

    while (a != b) {
      putc(*a++, stdout);
    }
  }

  printf("\n");

  dump_tree(n);

  printf("halting...\n");
  printf("########################################\n");
  debugbreak();
}

//------------------------------------------------------------------------------

MtHandle MtModule::get_by_id(std::vector<MtHandle>& handles, MtHandle id) {
  assert(id.sym == sym_identifier);
  auto name_a = node_to_name(id);

  for (auto& c : handles) {
    auto name_b = node_to_name(c);
    if (name_a == name_b) return c;
  }

  return MtHandle::null;
}

//------------------------------------------------------------------------------
// Node debugging

#if 0
void MtModule::dump_node(TSNode n, int index, int field, int depth) {
  if (ts_node_is_null(n)) {
    printf("### NULL ###\n");
    return;
  }

  auto sym = ts_node_symbol(n);

  uint32_t color = 0x00000000;
  if (sym == sym_template_declaration) color = 0xAADDFF;
  if (sym == sym_struct_specifier)     color = 0xFFAAFF;
  if (sym == sym_class_specifier)      color = 0xFFAAFF;
  if (sym == sym_expression_statement) color = 0xAAFFFF;
  if (sym == sym_expression_statement) color = 0xAAFFFF;
  if (sym == sym_compound_statement)   color = 0xFFFFFF;
  if (sym == sym_function_definition)  color = 0xAAAAFF;
  if (sym == sym_field_declaration)    color = 0xFFAAAA;
  if (sym == sym_comment)              color = 0xAAFFAA;

  if (color) {
    printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
    for (int i = 0; i < depth; i++) printf("|---");
  }
  else {
    for (int i = 0; i < depth; i++) printf("|   ");
  }

  printf("[%d] ", index);

  if (field > 0) printf("f%d ", field);
  if (sym) printf("s%d ", sym);

  if (field > 0) {
    printf("%s.", ts_language_field_name_for_id(lang, field));
  }

  if (ts_node_is_named(n) && ts_node_child_count(n)) {
    printf("%s: ", ts_node_type(n));
  }
  else if (ts_node_is_named(n) && !ts_node_child_count(n)) {
    printf("%s: ", ts_node_type(n));
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

void MtModule::dump_tree(TSNode n, int index, int field, int depth, int maxdepth) {
  if (depth == 0) {
    printf("\n========== tree dump begin\n");
  }
  dump_node(n, index, field, depth);

  if (!ts_node_is_null(n) && depth < maxdepth) {
    for (int i = 0; i < (int)ts_node_child_count(n); i++) {
      dump_tree(
        ts_node_child(n, i),
        i,
        ts_node_field_id_for_child(n, i),
        depth + 1,
        maxdepth
      );
    }
  }
  if (depth == 0) printf("========== tree dump end\n");
}
#endif

//------------------------------------------------------------------------------

void MtModule::dump_node(MtHandle n, int index, int depth) {
  if (!n) {
    printf("### NULL ###\n");
    return;
  }

  uint32_t color = 0x00000000;
  if (n.sym == sym_template_declaration) color = 0xAADDFF;
  if (n.sym == sym_struct_specifier)     color = 0xFFAAFF;
  if (n.sym == sym_class_specifier)      color = 0xFFAAFF;
  if (n.sym == sym_expression_statement) color = 0xAAFFFF;
  if (n.sym == sym_expression_statement) color = 0xAAFFFF;
  if (n.sym == sym_compound_statement)   color = 0xFFFFFF;
  if (n.sym == sym_function_definition)  color = 0xAAAAFF;
  if (n.sym == sym_field_declaration)    color = 0xFFAAAA;
  if (n.sym == sym_comment)              color = 0xAAFFAA;

  if (color) {
    printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
    for (int i = 0; i < depth; i++) printf("|---");
  }
  else {
    for (int i = 0; i < depth; i++) printf("|   ");
  }

  printf("[%d] ", index);

  if (n.field > 0) printf("f%d ", n.field);
  if (n.sym) printf("s%d ", n.sym);

  if (n.field > 0) {
    printf("%s.", ts_language_field_name_for_id(lang, n.field));
  }

  if (n && n.is_named() && n.child_count()) {
    printf("%s: ", n.type());
  }
  else if (n && n.is_named() && !n.child_count()) {
    printf("%s: ", n.type());
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }
  else if (!n) {
    debugbreak();
    printf("text: ");
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }
  else {
    // Unnamed nodes usually have their node body as their "type",
    // and their symbol is something like "aux_sym_preproc_include_token1"
    printf("lit: ");
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }

  printf("\n");
  printf("\u001b[0m");
}

//------------------------------------------------------------------------------

void MtModule::dump_tree(MtHandle n, int index, int depth, int maxdepth) {
  if (depth == 0) {
    printf("\n========== tree dump begin\n");
  }
  dump_node(n, index, depth);

  if (n && depth < maxdepth) {
    for (int i = 0; i < n.child_count(); i++) {
      dump_tree(n.child(i), i, depth + 1, maxdepth);
    }
  }
  if (depth == 0) printf("========== tree dump end\n");
}

//------------------------------------------------------------------------------
// Node traversal

void MtModule::visit_tree(MtHandle n, NodeVisitor cv) {
  cv(n);
  for (auto c : n) visit_tree(c, cv);
}

void MtModule::visit_tree2(MtHandle n, NodeVisitor2 cv) {
  for (auto c : n) {
    cv(n, c);
    visit_tree2(c, cv);
  }
}

//------------------------------------------------------------------------------

std::string MtModule::node_to_name(MtHandle n) {

  switch (n.sym) {
  
  case sym_field_expression:
  case alias_sym_type_identifier:
  case sym_identifier:
  case alias_sym_field_identifier:
    return n.body();

  case sym_field_declaration:
  case sym_array_declarator:
  case sym_parameter_declaration:
  case sym_optional_parameter_declaration:
  case sym_function_definition:
  case sym_function_declarator:
    return node_to_name(n.get_field(field_declarator));

  case sym_struct_specifier:
  case sym_class_specifier:
    return node_to_name(n.get_field(field_name));

  default:
    dump_tree(n);
    debugbreak();
    return "";
  }
}

std::string MtModule::node_to_type(MtHandle n) {
  switch (n.sym) {
  case alias_sym_type_identifier:
    return n.body();

  case sym_field_declaration:
    return node_to_type(n.get_field(field_type));

  case sym_template_type:
    return node_to_type(n.get_field(field_name));

  default:
    dump_tree(n);
    debugbreak();
    return "";
  }
}

//------------------------------------------------------------------------------
// Scanner

void MtModule::find_module() {
  module_template = MtHandle();
  module_class = MtHandle();

  visit_tree2(root, [&](MtHandle parent, MtHandle child) {
    if (child.sym == sym_struct_specifier || child.sym == sym_class_specifier) {
      if (parent.sym == sym_template_declaration) module_template = parent;
      module_class = child;

      auto name_node = module_class.get_field(field_name);
      module_name = name_node.body();
    }
    });

}

void MtModule::collect_moduleparams() {
  if (!module_template) return;

  if (module_template.sym != sym_template_declaration) debugbreak();

  auto params = module_template.get_field(field_parameters);

  for (auto child : params) {
    if (child.sym == sym_parameter_declaration ||
        child.sym == sym_optional_parameter_declaration) {
      moduleparams.push_back(child);
    }
  }
}


void MtModule::collect_fields() {
  visit_tree(module_class, [&](MtHandle n) {
    if (n.sym == sym_function_definition) {
      auto func_name = n.get_field(field_declarator).get_field(field_declarator);
      auto func_args = n.get_field(field_declarator).get_field(field_parameters);

      if (func_name.match("tick")) {
        visit_tree(func_args, [&](MtHandle func_arg) {
          if (func_arg.sym == sym_parameter_declaration) {
            auto arg_name = func_arg.get_field(field_declarator);

            if (!arg_name.match("rst_n")) {
              inputs.push_back(func_arg);
            }
          }
        });
      }
    }
  });

  visit_tree(module_class, [&](MtHandle n) {
    if (n.sym == sym_field_declaration) {
      if      (n.field_is_input())  inputs.push_back(n);
      else if (n.field_is_output()) outputs.push_back(n);
      else if (n.field_is_param()) {
        localparams.push_back(n);
      }
      else if (n.field_is_module()) submodules.push_back(n);
      else                         fields.push_back(n);
    }

    if (n.sym == sym_function_definition) {
      auto func_def = n;

      auto func_type = func_def.get_field(field_type);
      auto func_decl = func_def.get_field(field_declarator);

      bool is_task = false;
      bool is_init = false;
      bool is_tock = false;
      bool is_tick = false;

      //----------

      is_task = func_type.match("void");

      //----------

      auto current_function_name = func_decl.get_field(field_declarator);
      is_init = is_task && current_function_name.match("init");
      is_tick = is_task && current_function_name.match("tick");
      is_tock = is_task && current_function_name.match("tock");

      if (is_init) {
        node_init = func_def;
      }
      else if (is_tick) {
        node_tick = func_def;
      }
      else if (is_tock) {
        node_tock = func_def;
      }
      else {
        if (is_task) {
          tasks.push_back(func_def);
        }
        else {
          functions.push_back(func_def);
        }
      }
    }
  });
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_tick(MtHandle func_def) {
  std::set<MtHandle> dirty_fields;
  check_dirty_dispatch(func_def, true, dirty_fields, 0);
}

void MtModule::check_dirty_tock(MtHandle func_def) {
  std::set<MtHandle> dirty_fields;
  check_dirty_dispatch(func_def, false, dirty_fields, 0);
}

//----------------------------------------

void MtModule::check_dirty_read(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {

  // Reading from a dirty field in a seq block is forbidden.
  if (is_seq) {
    auto field = get_field_by_id(n);
    if (field && dirty_fields.contains(field)) {
      print_error(n, "seq read dirty field - %s\n", node_to_name(field).c_str());
    }
  }

  // Reading from a clean output in a comb block is forbidden.
  if (!is_seq) {
    auto output = get_output_by_id(n);
    if (output && !dirty_fields.contains(output)) {
      print_error(n, "comb read clean output - %s\n", node_to_name(output).c_str());
    }
  }
}

//----------------------------------------

void MtModule::check_dirty_write(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {
  // Writing to an already-dirty field in a seq block is forbidden.
  if (is_seq) {
    auto field = get_field_by_id(n);
    if (field) {
      if (dirty_fields.contains(field)) {
        print_error(n, "seq wrote dirty field - %s\n", node_to_name(field).c_str());
      }
      dirty_fields.insert(field);
    }
  }

  // Writing to any field in a comb block is forbidden.
  if (!is_seq) {
    auto field = get_field_by_id(n);
    if (field) {
      print_error(n, "comb wrote field - %s\n", node_to_name(field).c_str());
    }
  }

  // Writing to an already-dirty field in a comb block is forbidden.
  if (!is_seq) {
    auto output = get_output_by_id(n);
    if (output) {
      if (dirty_fields.contains(output)) {
        print_error(n, "comb wrote dirty output - %s\n", node_to_name(output).c_str());
      }
      dirty_fields.insert(output);
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_dispatch(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {
  if (!n || !n.is_named()) return;

  switch (n.sym) {
  case sym_identifier:            check_dirty_read(n, is_seq, dirty_fields, depth); break;
  case sym_assignment_expression: check_dirty_assign(n, is_seq, dirty_fields, depth); break;
  case sym_if_statement:          check_dirty_if(n, is_seq, dirty_fields, depth); break;
  case sym_call_expression:       check_dirty_call(n, is_seq, dirty_fields, depth); break;
  case sym_switch_statement:      check_dirty_switch(n, is_seq, dirty_fields, depth); break;
  default:                        for (auto c : n) check_dirty_dispatch(c, is_seq, dirty_fields, depth + 1); break;
  }
}

//------------------------------------------------------------------------------
// Check for reads on the RHS of an assignment, then check the write on the left.

void MtModule::check_dirty_assign(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {
  auto lhs = n.get_field(field_left);
  auto rhs = n.get_field(field_right);

  check_dirty_dispatch(rhs, is_seq, dirty_fields, depth + 1);
  check_dirty_write(lhs, is_seq, dirty_fields, depth + 1);
}

//----------------------------------------
// Check the "if" branch and the "else" branch independently and then merge the results.

void MtModule::check_dirty_if(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {
  check_dirty_dispatch(n.get_field(field_condition), is_seq, dirty_fields, depth + 1);

  std::set<MtHandle> if_set = dirty_fields;
  std::set<MtHandle> else_set = dirty_fields;

  check_dirty_dispatch(n.get_field(field_consequence), is_seq, if_set, depth + 1);
  check_dirty_dispatch(n.get_field(field_alternative), is_seq, else_set, depth + 1);

  dirty_fields.merge(if_set);
  dirty_fields.merge(else_set);
}

//----------------------------------------
// Follow member function calls.

void MtModule::check_dirty_call(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {
  auto node_func = n.get_field(field_function);
  auto node_args = n.get_field(field_arguments);

  if (node_func.is_identifier()) {
    // local function call, traverse args and then function body
    check_dirty_dispatch(node_args, is_seq, dirty_fields, depth + 1);

    auto task = get_task_by_id(node_func);
    if (task) check_dirty_dispatch(task, is_seq, dirty_fields, depth + 1);

    auto func = get_function_by_id(node_func);
    if (func) check_dirty_dispatch(func, is_seq, dirty_fields, depth + 1);
  }
}

//----------------------------------------
// Check the condition of a switch statement, then check each case independently.

void MtModule::check_dirty_switch(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth) {

  auto cond = n.get_field(field_condition);
  auto body = n.get_field(field_body);

  check_dirty_dispatch(cond, is_seq, dirty_fields, depth + 1);

  auto old_dirty_fields = dirty_fields;

  for (auto c : body) {
    if (c.sym == sym_case_statement) {
      auto temp = old_dirty_fields;
      check_dirty_dispatch(c, is_seq, temp, depth + 1);
      dirty_fields.merge(temp);
    }
  }
}

//------------------------------------------------------------------------------
