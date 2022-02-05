#include "MtModule.h"
#include "Platform.h"

#include "MtIterator.h"

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

  out_file = fopen(output_filename.c_str(), "wb");

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  root = MtHandle::from_mod(this);

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

  n.dump_tree();

  printf("halting...\n");
  printf("########################################\n");
  debugbreak();
}

//------------------------------------------------------------------------------

MtHandle MtModule::get_by_id(std::vector<MtHandle>& handles, MtHandle id) {
  assert(id.sym == sym_identifier);
  auto name_a = id.node_to_name();

  for (auto& c : handles) {
    auto name_b = c.node_to_name();
    if (name_a == name_b) return c;
  }

  return MtHandle::null;
}

//------------------------------------------------------------------------------
// Scanner

void MtModule::find_module() {
  module_template = MtHandle();
  module_class = MtHandle();

  root.visit_tree2([&](MtHandle parent, MtHandle child) {
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

  for (auto child : module_template.get_field(field_parameters)) {
    if (child.sym == sym_parameter_declaration ||
        child.sym == sym_optional_parameter_declaration) {
      moduleparams.push_back(child);
    }
  }
}


void MtModule::collect_fields() {
  module_class.visit_tree([&](MtHandle n) {
    if (n.sym == sym_function_definition) {
      auto func_name = n.get_field(field_declarator).get_field(field_declarator);
      auto func_args = n.get_field(field_declarator).get_field(field_parameters);

      if (func_name.match("tick")) {
        func_args.visit_tree([&](MtHandle func_arg) {
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

  module_class.visit_tree([&](MtHandle n) {
    if (n.sym == sym_field_declaration) {
      if      (n.field_is_input())  inputs.push_back(n);
      else if (n.field_is_output()) outputs.push_back(n);
      else if (n.field_is_param())  localparams.push_back(n);
      else if (n.field_is_module()) submodules.push_back(n);
      else                          fields.push_back(n);
    }

    if (n.sym == sym_function_definition) {
      auto func_type = n.get_field(field_type);
      auto func_decl = n.get_field(field_declarator);
      auto func_name = func_decl.get_field(field_declarator).node_to_name();

      bool is_task = func_type.match("void");
      bool is_init = is_task && func_name == "init";
      bool is_tick = is_task && func_name == "tick";
      bool is_tock = is_task && func_name == "tock";

      if      (is_init) node_init = n;
      else if (is_tick) node_tick = n;
      else if (is_tock) node_tock = n;
      else if (is_task) tasks.push_back(n);
      else              functions.push_back(n);
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
      print_error(n, "seq read dirty field - %s\n", field.node_to_name().c_str());
    }
  }

  // Reading from a clean output in a comb block is forbidden.
  if (!is_seq) {
    auto output = get_output_by_id(n);
    if (output && !dirty_fields.contains(output)) {
      print_error(n, "comb read clean output - %s\n", output.node_to_name().c_str());
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
        print_error(n, "seq wrote dirty field - %s\n", field.node_to_name().c_str());
      }
      dirty_fields.insert(field);
    }
  }

  // Writing to any field in a comb block is forbidden.
  if (!is_seq) {
    auto field = get_field_by_id(n);
    if (field) {
      print_error(n, "comb wrote field - %s\n", field.node_to_name().c_str());
    }
  }

  // Writing to an already-dirty field in a comb block is forbidden.
  if (!is_seq) {
    auto output = get_output_by_id(n);
    if (output) {
      if (dirty_fields.contains(output)) {
        print_error(n, "comb wrote dirty output - %s\n", output.node_to_name().c_str());
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
