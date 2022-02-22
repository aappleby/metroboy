#include "MtModule.h"
#include "Platform.h"

#include "MtNode.h"
#include "MtModLibrary.h"

#pragma warning(disable:4996) // unsafe fopen()

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

//------------------------------------------------------------------------------

blob load_blob(const char* filename) {
  //printf("load_blob {%s}\n", filename);
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

std::string get_field_type_name(MtNode n) {
  assert(n.sym == sym_field_declaration);

  auto node_type = n.get_field(field_type);
  if (node_type.sym == sym_primitive_type) {
    return node_type.body();
  }
  else if (node_type.sym == sym_identifier) {
    return node_type.body();
  }
  else if (node_type.sym == alias_sym_type_identifier) {
    return node_type.body();
  }
  else {
    auto type_name = node_type.get_field(field_name);
    if (type_name.is_null()) {
      n.error();
    }
    else {
      return type_name.body();
    }
  }

  debugbreak();
  return "";
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

//------------------------------------------------------------------------------

void MtModule::load(const std::string& full_path) {
  this->full_path = full_path;

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);

  src_blob = load_blob(full_path.c_str());

  if (src_blob[0] == 239 && src_blob[1] == 187 && src_blob[2] == 191) {
    use_utf8_bom = true;
    src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
  }

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  mod_root = MtTranslationUnit(MtNode::from_mod(this));

  find_module();
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
  if (id.sym == sym_subscript_expression) {
    return get_by_id(handles, id.get_field(field_argument));
  }

  assert(id.sym == sym_identifier);
  auto name_a = id.node_to_name();

  for (auto& c : handles) {
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
    auto name_b = c.name.node_to_name();
    if (name_a == name_b) return c;
  }

  return MtField::null;
}

//------------------------------------------------------------------------------
// Scanner

void MtModule::find_module() {
  //mod_template = MtNode::null;
  //mod_class = MtNode::null;

  mod_root.visit_tree2([&](MtNode parent, MtNode child) {
    if (child.sym == sym_struct_specifier) {
      if (parent.sym == sym_template_declaration) mod_template = MtTemplateDecl(parent);

      //child.dump_tree(0, 0, 1);

      mod_class = MtStructSpecifier(child);
    }

    if (child.sym == sym_template_parameter_list) {
      mod_param_list = MtTemplateParamList(child);
    }
  });

  mod_name = mod_class.get_field(field_name).body();
}

//------------------------------------------------------------------------------

void MtModule::collect_modparams() {
  if (!mod_template) return;

  for (auto child : mod_template.get_field(field_parameters)) {
    if (child.sym == sym_parameter_declaration ||
        child.sym == sym_optional_parameter_declaration) {
      modparams.push_back(child);
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::collect_localparams() {
}

//------------------------------------------------------------------------------

void MtModule::collect_functions() {

  for (auto n : (MtNode&)mod_class.body()) {
    if (n.sym == sym_function_definition) {
      n.dump_tree(0, 0, 1);

      auto func_decl = n.get_field(field_declarator);
      auto func_name = func_decl.get_field(field_declarator).node_to_name();
      auto func_args = n.get_field(field_declarator).get_field(field_parameters);
      auto func_type = n.get_field(field_type);

      bool is_task = func_type.match("void");
      bool is_init = is_task && func_name.starts_with("init");
      bool is_tick = is_task && func_name.starts_with("tick");
      bool is_tock = is_task && func_name.starts_with("tock");

      if      (is_init) inits.push_back(n);
      else if (is_tick) ticks.push_back(n);
      else if (is_tock) tocks.push_back(n);
      else if (is_task) tasks.push_back(n);
      else              funcs.push_back(n);
    }
  }
}


//------------------------------------------------------------------------------

void MtModule::collect_ports() {
  for (auto n : ticks) {
    for (auto tick_arg : n.get_field(field_declarator).get_field(field_parameters)) {
      if (tick_arg.sym == sym_parameter_declaration) {
        auto arg_type = tick_arg.get_field(field_type);
        auto arg_name = tick_arg.get_field(field_declarator);

        if (arg_name.is_null()) {
          n.error();
        }

        if (!arg_name.match("rst_n")) {
          inputs.push_back({tick_arg, arg_type, arg_name});
        }
      }
    }
  }

  for (auto n : tocks) {
    for (auto tock_arg : n.get_field(field_declarator).get_field(field_parameters)) {
      if (tock_arg.sym == sym_parameter_declaration) {
        auto arg_type = tock_arg.get_field(field_type);
        auto arg_name = tock_arg.get_field(field_declarator);

        if (arg_name.is_null()) {
          n.error();
        }

        if (!arg_name.match("rst_n")) {
          inputs.push_back({tock_arg, arg_type, arg_name});
        }
      }
    }
  }
}

//------------------------------------------------------------------------------

/*
enum field
submodule
submodule with template arg
*/

void MtModule::collect_fields() {
  auto mod_name = mod_class.get_field(field_name).check_null();
  auto mod_body = mod_class.get_field(field_body).check_null();

  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;

    //n.dump_tree();
    //continue;

    auto field_type = n.get_field(::field_type);

    // enum class
    if (field_type.sym == sym_enum_specifier) {
      //printf("enum\n");
      //n.dump_tree();
      enums.push_back(n);
      continue;
    }

    // localparam = static const int
    if (n.is_static() && n.is_const()) {
      localparams.push_back(n);
      continue;
    }

    // Everything not an enum shoul have 1 or more declarator fields that
    // contain the field name(s).

    std::vector<MtNode> decls;
    for (auto c : n) {
      if (c.field == field_declarator) {
        auto name = c.sym == sym_array_declarator ? c.get_field(field_declarator) : c;

        MtField f = { n, field_type, name };

        if (name.body().starts_with("o_")) {
          outputs.push_back(f);
        }
        else {
          auto type_name = get_field_type_name(n);
          if (lib->has_mod(type_name)) {
            printf("SUBMOD %s\n", type_name.c_str());
            submodules.push_back(n);
          }
          else {
            printf("FIELD  %s\n", type_name.c_str());
            fields.push_back(f);
          }

          //n.dump_tree();
          //fields.push_back(f);
        }
      }
    }

    //if (node_type.sym == sym_template_type) {
    //  if (node_type.get_field(field_name).body() == "logic") {
    //    //logic with literal template arg
    //    //logic with identifier template arg
    //    //logic array
    //  }
    //}

    /*
    int id_count = 0;

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
    */


    //if (id_count == 0) {
    //  n.dump_tree();
    //  debugbreak();
    //}
  }
}

//------------------------------------------------------------------------------
// Input ports can be declared in multiple tick/tock methods, but we don't want
// duplicates in the Verilog port list.

void MtModule::dedup_inputs() {

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

void MtModule::check_dirty_ticks() {
  /*
  std::set<MtField> dirty_fields;
  check_dirty_dispatch(func_def, true, dirty_fields, 0);
  */
}

void MtModule::check_dirty_tocks() {
  /*
  std::set<MtField> dirty_fields;
  check_dirty_dispatch(func_def, false, dirty_fields, 0);
  */
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
