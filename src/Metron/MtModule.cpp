#include "MtModule.h"
#include "Platform.h"

#include "MtNode.h"
#include "MtModLibrary.h"
#include "../CoreLib/Log.h"

#pragma warning(disable:4996) // unsafe fopen()

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
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

void MtModule::dump_method_list(std::vector<MtMethod>& methods) {
  for (auto& n : methods) {
    LOG_INDENT_SCOPE();
    LOG_R("%s(", n.name.c_str());
    for (int i = 0; i < n.params.size(); i++) {
      auto& param = n.params[i];
      LOG_R("%s", param.c_str());
      if (i != n.params.size() - 1) LOG_C(", ");
    }
    LOG_R(")\n");
  }
}

void MtModule::dump_call_list(std::vector<MtCall>& calls) {
  for (auto& call : calls) {
    LOG_INDENT_SCOPE();
    LOG_C("%s.%s(", call.submod->name.c_str(), call.method->name.c_str());
    if (call.args.size()) {
      LOG_C("\n");
      LOG_INDENT_SCOPE();
      for (auto i = 0; i < call.args.size(); i++) {
        LOG_C("%s = %s", call.method->params[i].c_str(), call.args[i].c_str());
        if (i != call.args.size() - 1) LOG_C(",\n");
      }
    }
    LOG_C(")\n");
  }
}

//------------------------------------------------------------------------------

void MtModule::dump_banner() {
  LOG_Y("//----------------------------------------\n");
  if (mod_struct.is_null()) {
    LOG_Y("// Package %s\n", full_path.c_str());
    LOG_Y("\n");
    return;
  }

  LOG_Y("// Module %s\n", mod_name.c_str());
  LOG_Y("\n");

  if (modparams.size()) {
    LOG_B("Modparams:\n")
    LOG_INDENT_SCOPE();
    for (auto& param : modparams) LOG_G("%s\n", param.name.c_str());
  }

  if (localparams.size()) {
    LOG_B("Localparams:\n");
    LOG_INDENT_SCOPE();
    for (auto& param : localparams) LOG_G("%s\n", param.name.c_str());
  }

  if (enums.size()) {
    LOG_B("Enums:\n");
    LOG_INDENT_SCOPE();
    for (auto& n : enums) LOG_G("%s\n", n.name.c_str());
  }

  if (inputs.size()) {
    LOG_B("Inputs:\n");
    LOG_INDENT_SCOPE();
    for (auto& n : inputs) LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (outputs.size()) {
    LOG_B("Outputs:\n");
    LOG_INDENT_SCOPE();
    for (auto& n : outputs) LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (fields.size()) {
    LOG_B("Fields:\n");
    LOG_INDENT_SCOPE();
    for (auto& n : fields) LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (submods.size()) {
    LOG_B("Submods:\n");
    LOG_INDENT_SCOPE();
    for (auto& submod : submods) LOG_G("%s:%s\n", submod.name.c_str(), submod.mod->mod_name.c_str());
  }

  if (init_methods.size()) {
    LOG_B("Init methods:\n");
    dump_method_list(init_methods);
  }

  if (tick_methods.size()) {
    LOG_B("Tick methods:\n");
    dump_method_list(tick_methods);
  }

  if (tock_methods.size()) {
    LOG_B("Tock methods:\n");
    dump_method_list(tock_methods);
  }

  if (task_methods.size()) {
    LOG_B("Tasks:\n");
    dump_method_list(task_methods);
  }

  if (func_methods.size()) {
    LOG_B("Functions:\n");
    dump_method_list(func_methods);
  }

  if (tick_calls.size()) {
    LOG_B("Tick calls:\n");
    dump_call_list(tick_calls);
  }

  if (tock_calls.size()) {
    LOG_B("Tock calls:\n");
    dump_call_list(tock_calls);
  }

  if (port_map.size()) {
    LOG_B("Port map:\n");
    LOG_INDENT_SCOPE();
    for (auto& kv : port_map) LOG_G("%s = %s\n", kv.first.c_str(), kv.second.c_str());
  }

  LOG_B("\n");
}

//------------------------------------------------------------------------------

void MtModule::load_pass1(const char* _full_path, blob& _src_blob) {
  this->full_path = _full_path;
  this->src_blob = _src_blob;

  if (src_blob[0] == 239 && src_blob[1] == 187 && src_blob[2] == 191) {
    use_utf8_bom = true;
    src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
  }

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();

  // Parse the module and find the root class/struct/template.

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  mod_root = MtTranslationUnit(MtNode::from_mod(this));

  for (auto n : (MtNode)mod_root) {
    if (n.sym == sym_template_declaration) {
      mod_template = MtTemplateDecl(n);
      mod_param_list = MtTemplateParamList(n.child(1));
      mod_struct = MtStructSpecifier(n.child(2));
      break;
    }
    if (n.sym == sym_struct_specifier) {
      mod_struct = MtStructSpecifier(n);
      break;
    }
  }

  if (mod_struct.is_null()) return;

  // We've loaded and parsed a module, we can start pulling stuff out of it.

  mod_name = mod_struct.get_field(field_name).text();

  auto mod_name = mod_struct.get_field(field_name).check_null();
  auto mod_body = mod_struct.get_field(field_body).check_null();

  // Collect the template parameters if there are any.

  if (mod_template) {
    auto params = mod_template.get_field(field_parameters);

    for (int i = 0; i < params.named_child_count(); i++) {
      auto child = params.named_child(i);
      if (child.sym == sym_parameter_declaration ||
          child.sym == sym_optional_parameter_declaration) {
        modparams.push_back(MtParam(child));
      }
      else {
        debugbreak();
      }
    }
  }

  // Collect and sort all function definitions in the module.

  for (auto n : mod_body) {
    if (n.sym != sym_function_definition) continue;
    auto func_decl = n.get_field(field_declarator);
    auto func_name = func_decl.get_field(field_declarator).node_to_name();
    auto func_args = n.get_field(field_declarator).get_field(field_parameters);
    auto func_type = n.get_field(field_type);

    bool is_task = func_type.match("void");
    bool is_init = is_task && func_name.starts_with("init");
    bool is_tick = is_task && func_name.starts_with("tick");
    bool is_tock = is_task && func_name.starts_with("tock");

    if      (is_init) init_methods.push_back(node_to_method(n));
    else if (is_tick) tick_methods.push_back(node_to_method(n));
    else if (is_tock) tock_methods.push_back(node_to_method(n));
    else if (is_task) task_methods.push_back(node_to_method(n));
    else              func_methods.push_back(node_to_method(n));
  }

  // Collect all inputs to all tick and tock methods and merge them into a list
  // of input ports. Input ports can be declared in multiple tick/tock methods,
  // but we don't want duplicates in the Verilog port list.

  std::set<std::string> input_dedup;

  for (auto n : tick_methods) {
    for (auto tick_arg : n.get_field(field_declarator).get_field(field_parameters)) {
      if (tick_arg.sym == sym_parameter_declaration) {
        auto arg_type = tick_arg.get_field(field_type);
        auto arg_name = tick_arg.get_field(field_declarator);

        if (arg_name.is_null()) n.error();

        if (!input_dedup.contains(arg_name.text())) {
          inputs.push_back({tick_arg, arg_type.node_to_type(), arg_name.text()});
          input_dedup.insert(arg_name.text());
        }
      }
    }
  }

  for (auto n : tock_methods) {
    for (auto tock_arg : n.get_field(field_declarator).get_field(field_parameters)) {
      if (tock_arg.sym == sym_parameter_declaration) {
        auto arg_type = tock_arg.get_field(field_type);
        auto arg_name = tock_arg.get_field(field_declarator);

        if (arg_name.is_null()) n.error();

        if (!input_dedup.contains(arg_name.text())) {
          inputs.push_back({tock_arg, arg_type.node_to_type(), arg_name.text()});
          input_dedup.insert(arg_name.text());
        }
      }
    }
  }

  // Collecting fields requires the ability to resolve the names of other
  // modules, we'll do that in pass 2.
}

//------------------------------------------------------------------------------
// All modules are now in the library, we can resolve references to other
// modules when we're collecting fields.

void MtModule::load_pass2() {
  if (mod_struct.is_null()) return;

  auto mod_name = mod_struct.get_field(field_name).check_null();
  auto mod_body = mod_struct.get_field(field_body).check_null();

  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;

    auto field_type = n.get_field(::field_type);

    // enum class
    if (field_type.sym == sym_enum_specifier) {
      enums.push_back(MtEnum(n));
      continue;
    }

    // localparam = static const int
    if (n.is_static() && n.is_const()) {
      localparams.push_back(MtParam(n));
      continue;
    }

    // Everything not an enum shoul have 1 or more declarator fields that
    // contain the field name(s).

    for (auto c : n) {
      if (c.field == field_declarator) {
        auto name = c.sym == sym_array_declarator ? c.get_field(field_declarator) : c;


        MtField f = { c, field_type.node_to_type(), name.text() };

        if (name.text().starts_with("o_")) {
          outputs.push_back(f);
        }
        else {
          auto type_name = n.node_to_type();
          if (lib->has_mod(type_name)) {
            MtSubmod submod(n);
            submod.mod = lib->find_mod(type_name);
            submod.name = n.get_field(field_declarator).text();
            submods.push_back(submod);
          }
          else {
            fields.push_back(f);
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// All modules have populated their fields, match up tick/tock calls with their
// corresponding methods.

void MtModule::load_pass3() {
  if (mod_struct.is_null()) return;

  for (auto n : tick_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_call_expression) return;
      if (child.get_field(field_function).sym != sym_field_expression) return;
      tick_calls.push_back(node_to_submod_call(child));
    });
  }

  for (auto n : tock_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_call_expression) return;
      if (child.get_field(field_function).sym != sym_field_expression) return;
      tock_calls.push_back(node_to_submod_call(child));
    });
  }

  for (auto& call : tick_calls) {
    for (auto i = 0; i < call.args.size(); i++) {
      auto key = call.submod->name + "." + call.method->params[i];
      auto val = call.args[i];
      auto it = port_map.find(key);
      if (it != port_map.end()) {
        assert((*it).second == val);
      }
      else {
        port_map.insert({key, val});
      }
    }
  }

  for (auto& call : tock_calls) {
    for (auto i = 0; i < call.args.size(); i++) {
      auto key = call.submod->name + "." + call.method->params[i];
      auto val = call.args[i];
      auto it = port_map.find(key);
      if (it != port_map.end()) {
        assert((*it).second == val);
      }
      else {
        port_map.insert({key, val});
      }
    }
  }
}

//------------------------------------------------------------------------------

MtMethod MtModule::node_to_method(MtNode n) {
  MtMethod result(n);

  auto method_name = n.get_field(field_declarator).get_field(field_declarator).text();
  auto method_params = n.get_field(field_declarator).get_field(field_parameters);

  result.name = method_name;

  for (int i = 0; i < method_params.child_count(); i++) {
    auto param = method_params.child(i);
    if (param.sym != sym_parameter_declaration) continue;

    auto param_name = param.get_field(field_declarator).text();
    result.params.push_back(param_name);
  }

  return result;
}

//------------------------------------------------------------------------------

MtCall MtModule::node_to_submod_call(MtNode n) {
  MtCall result(n);

  auto call = MtCallExpr(n);
  auto call_this = call.get_field(field_function).get_field(field_argument);
  auto call_func = call.get_field(field_function).get_field(field_field);
  auto call_args = call.get_field(field_arguments);

  auto submod = get_submod(call_this.text());
  assert(submod);

  result.submod = submod;
  result.method = submod->mod->get_method(call_func.text());
  for (int i = 0; i < call_args.named_child_count(); i++) {
    result.args.push_back(call_args.named_child(i).text());
  }
  return result;
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_ticks() {
  for (auto& tick : tick_methods) {
    std::set<MtField> dirty_fields;
    check_dirty_dispatch(tick, true, dirty_fields, 0);
  }
}

void MtModule::check_dirty_tocks() {
  for (auto& tock : tock_methods) {
    std::set<MtField> dirty_fields;
    check_dirty_dispatch(tock, false, dirty_fields, 0);
  }
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_dispatch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
  if (n.is_null() || !n.is_named()) return;

  switch (n.sym) {
    case sym_identifier: {
      check_dirty_read(n, is_seq, dirty_fields, depth);
      break;
    }

    case sym_assignment_expression: {
      check_dirty_assign(n, is_seq, dirty_fields, depth);
      break;
    }

    case sym_if_statement: {
      check_dirty_if(n, is_seq, dirty_fields, depth);
      break;
    }

    case sym_call_expression: {
      check_dirty_call(n, is_seq, dirty_fields, depth);
      break;
    }

    case sym_switch_statement: {
      check_dirty_switch(n, is_seq, dirty_fields, depth);
      break;
    }

    default: {
      for (auto c : n) {
        check_dirty_dispatch(c, is_seq, dirty_fields, depth + 1); break;
      }
    }
  }
}

//----------------------------------------

void MtModule::check_dirty_read(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {

#if 0
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

#endif
}

//----------------------------------------

void MtModule::check_dirty_write(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
#if 0

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

#endif
}

//------------------------------------------------------------------------------
// Check for reads on the RHS of an assignment, then check the write on the left.

void MtModule::check_dirty_assign(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
#if 0
  auto lhs = n.get_field(field_left);
  auto rhs = n.get_field(field_right);

  check_dirty_dispatch(rhs, is_seq, dirty_fields, depth + 1);
  check_dirty_write(lhs, is_seq, dirty_fields, depth + 1);
#endif
}

//----------------------------------------
// Check the "if" branch and the "else" branch independently and then merge the results.

void MtModule::check_dirty_if(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
#if 0
  check_dirty_dispatch(n.get_field(field_condition), is_seq, dirty_fields, depth + 1);

  std::set<MtField> if_set = dirty_fields;
  std::set<MtField> else_set = dirty_fields;

  check_dirty_dispatch(n.get_field(field_consequence), is_seq, if_set, depth + 1);
  check_dirty_dispatch(n.get_field(field_alternative), is_seq, else_set, depth + 1);

  dirty_fields.merge(if_set);
  dirty_fields.merge(else_set);
#endif
}

//----------------------------------------
// Follow member function calls.

void MtModule::check_dirty_call(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
#if 0
  auto node_func = n.get_field(field_function);
  auto node_args = n.get_field(field_arguments);

  if (node_func.is_identifier()) {
    // local function call, traverse args and then function body
    check_dirty_dispatch(node_args, is_seq, dirty_fields, depth + 1);

    // FIXME 
    /*
    auto task = get_task_by_id(node_func);
    if (task) check_dirty_dispatch(task, is_seq, dirty_fields, depth + 1);

    auto func = get_function_by_id(node_func);
    if (func) check_dirty_dispatch(func, is_seq, dirty_fields, depth + 1);
    */
  }
  else {
    debugbreak();
  }
#endif
}

//----------------------------------------
// Check the condition of a switch statement, then check each case independently.

void MtModule::check_dirty_switch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth) {
#if 0
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
#endif
}

//------------------------------------------------------------------------------
