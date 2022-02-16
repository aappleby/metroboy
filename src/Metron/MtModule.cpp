#include "MtModule.h"
#include "Platform.h"

#include "MtNode.h"

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
  this->input_filename = input_filename;
  this->output_filename = output_filename;

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);

  src_blob = load_blob(input_filename.c_str());

  out_file = fopen(output_filename.c_str(), "wb");

  // Copy the BOM over if needed.
  if (src_blob[0] == 239 && src_blob[1] == 187 && src_blob[2] == 191) {
    fwrite(src_blob.data(), 1, 3, out_file);
    src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
  }

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  mod_root = MtNode::from_mod(this);

  find_module();
  collect_moduleparams();
  collect_fields();

  // Verify that tick()/tock() obey read/write ordering rules.

  check_dirty_tick(mod_tick);
  check_dirty_tock(mod_tock);
}

//------------------------------------------------------------------------------

void MtModule::print_error(MtNode n, const char* fmt, ...) {
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

MtNode MtModule::get_by_id(std::vector<MtNode>& handles, MtNode id) {
  //id.dump_tree();

  if (id.sym == sym_subscript_expression) {
    return get_by_id(handles, id.get_field(field_argument));
  }

  /*
  [0] f19 s266 left.subscript_expression:
  |   [0] f2 s1 argument.identifier: "gnt_tree"
  |   [1] s61 lit: "["
  |   [2] f16 s112 index.number_literal: "0"
  |   [3] s62 lit: "]"
  ========== tree dump end
  */

  assert(id.sym == sym_identifier);
  auto name_a = id.node_to_name();

  for (auto& c : handles) {
    //c.dump_tree();
    auto name_b = c.node_to_name();
    if (name_a == name_b) return c;
  }

  return MtNode::null;
}

MtField MtModule::get_by_id(std::vector<MtField>& handles, MtNode id) {

  if (id.sym == sym_subscript_expression) {
    return get_by_id(handles, id.get_field(field_argument));
  }

  assert(id.sym == sym_identifier);
  auto name_a = id.node_to_name();

  for (auto& c : handles) {
    //c.dump_tree();
    auto name_b = c.name.node_to_name();
    if (name_a == name_b) return c;
  }

  return MtField::null;
}

//------------------------------------------------------------------------------
// Scanner

void MtModule::find_module() {
  mod_template = MtNode::null;
  mod_class = MtNode::null;

  mod_root.visit_tree2([&](MtNode parent, MtNode child) {
    if (child.sym == sym_struct_specifier || child.sym == sym_class_specifier) {
      if (parent.sym == sym_template_declaration) mod_template = parent;
      mod_class = child;
    }

    if (child.sym == sym_template_parameter_list) {
      mod_param_list = child;
    }
  });

  mod_name = mod_class.get_field(field_name).body();
}

//------------------------------------------------------------------------------

void MtModule::collect_moduleparams() {
  if (!mod_template) return;

  if (mod_template.sym != sym_template_declaration) debugbreak();

  for (auto child : mod_template.get_field(field_parameters)) {
    if (child.sym == sym_parameter_declaration ||
        child.sym == sym_optional_parameter_declaration) {
      modparams.push_back(child);
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::collect_fields() {
  if (mod_class.is_null()) {
    return;
  }
  
  auto mod_name = mod_class.get_field(field_name).check_null();
  auto mod_body = mod_class.get_field(field_body).check_null();

  for (auto n : mod_body) {
    if (n.sym == sym_function_definition) {
      auto func_name = n.get_field(field_declarator).get_field(field_declarator).body();
      auto func_args = n.get_field(field_declarator).get_field(field_parameters);

      if (func_name.starts_with("tick") || func_name.starts_with("tock")) {
        for (auto func_arg : func_args) {
          if (func_arg.sym == sym_parameter_declaration) {
            auto arg_type = func_arg.get_field(field_type);
            auto arg_name = func_arg.get_field(field_declarator);

            if (arg_name.is_null()) {
              n.dump_tree();
              arg_type.dump_tree();
              debugbreak();
            }

            if (!arg_name.match("rst_n")) {
              inputs.push_back({func_arg, arg_type, arg_name});
            }
          }
        }
      }
    }
  }

  for (auto n : mod_body) {
    if (n.sym == sym_field_declaration) {

      int id_count = 0;
      auto node_type = n.get_field(field_type);

      for (auto c : n) {
        if (c.sym == alias_sym_field_identifier || c.sym == sym_array_declarator) {
          id_count++;
          auto node_name = c;
          MtField f = { n, node_type, node_name };
          if (f.is_input()) {
            inputs.push_back(f);
          }
          else if (f.is_output()) {
            outputs.push_back(f);
          }
          else {
            fields.push_back(f);
          }
        }
      }


      if (id_count == 0) {
        if (node_type.sym == sym_enum_specifier) {
          enums.push_back(n);
        }
        else {
          n.dump_tree();
          debugbreak();
        }
      }
    }

    if (n.sym == sym_function_definition) {
      auto func_type = n.get_field(field_type);
      auto func_decl = n.get_field(field_declarator);
      auto func_name = func_decl.get_field(field_declarator).node_to_name();

      bool is_task = func_type.match("void");
      bool is_init = is_task && func_name == "init";
      bool is_tick = is_task && func_name == "tick";
      bool is_tock = is_task && func_name == "tock";

      if      (is_init) mod_init = n;
      else if (is_tick) mod_tick = n;
      else if (is_tock) mod_tock = n;
      else if (is_task) tasks.push_back(n);
      else              functions.push_back(n);
    }
  }

  //----------
  // Dedup the input ports

  std::set<std::string> input_dedup;
  std::vector<MtField> inputs2;
  for (auto input : inputs) {
    auto input_name = input.name.body();
    if (!input_dedup.contains(input_name)) {
      input_dedup.insert(input_name);
      inputs2.push_back(input);
    }
  }
  inputs = inputs2;
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_tick(MtNode func_def) {
  std::set<MtField> dirty_fields;
  check_dirty_dispatch(func_def, true, dirty_fields, 0);
}

void MtModule::check_dirty_tock(MtNode func_def) {
  std::set<MtField> dirty_fields;
  check_dirty_dispatch(func_def, false, dirty_fields, 0);
}

//----------------------------------------

void MtModule::check_dirty_read(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {

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

void MtModule::check_dirty_write(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
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

      // We should generally not be assigning to fields in tock(), but
      // prim_arbiter_fixed does a big comb tree thing.

      //print_error(n, "comb wrote field - %s\n", field.node_to_name().c_str());
    }
  }

  // Writing to an already-dirty field in a comb block is forbidden.
  if (!is_seq) {
    auto output = get_output_by_id(n);
    if (output) {
      if (dirty_fields.contains(output)) {
        // we need this for ibex_compressed_decoder
        //print_error(n, "comb wrote dirty output - %s\n", output.node_to_name().c_str());
      }
      dirty_fields.insert(output);
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_dispatch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
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

void MtModule::check_dirty_assign(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
  auto lhs = n.get_field(field_left);
  auto rhs = n.get_field(field_right);

  check_dirty_dispatch(rhs, is_seq, dirty_fields, depth + 1);
  check_dirty_write(lhs, is_seq, dirty_fields, depth + 1);
}

//----------------------------------------
// Check the "if" branch and the "else" branch independently and then merge the results.

void MtModule::check_dirty_if(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
  check_dirty_dispatch(n.get_field(field_condition), is_seq, dirty_fields, depth + 1);

  std::set<MtField> if_set = dirty_fields;
  std::set<MtField> else_set = dirty_fields;

  check_dirty_dispatch(n.get_field(field_consequence), is_seq, if_set, depth + 1);
  check_dirty_dispatch(n.get_field(field_alternative), is_seq, else_set, depth + 1);

  dirty_fields.merge(if_set);
  dirty_fields.merge(else_set);
}

//----------------------------------------
// Follow member function calls.

void MtModule::check_dirty_call(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
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

void MtModule::check_dirty_switch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {

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
