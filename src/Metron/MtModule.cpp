#include "MtModule.h"
#include "Platform.h"

#include "MtNode.h"
#include "MtModLibrary.h"
#include "../CoreLib/Log.h"

#pragma warning(disable:4996) // unsafe fopen()

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

//-----------------------------------------------------------------------------
// alwaysOut = alwaysA && alwaysB
// maybeOut = maybeA || maybeB || (alwaysA && !alwaysB) || (alwaysB && !alwaysA)

template<typename T>
void fold_parallel(const std::set<T>& always_a, const std::set<T>& maybe_a,
                   const std::set<T>& always_b, const std::set<T>& maybe_b,
                   std::set<T>& always_out, std::set<T>& maybe_out) {
  always_out.clear();
  maybe_out.clear();

  for (const auto& d : always_a) {
    if (always_b.contains(d)) always_out.insert(d);
  }

  for (const auto& d : always_a) {
    if (!always_b.contains(d)) maybe_out.insert(d);
  }

  for (const auto& d : always_b) {
    if (!always_a.contains(d)) maybe_out.insert(d);
  }

  maybe_out.insert(maybe_a.begin(), maybe_a.end());
  maybe_out.insert(maybe_b.begin(), maybe_b.end());
}

//------------------------------------------------------------------------------
// alwaysOut = alwaysA || alwaysB
// maybeOut  = (maybeA || maybeB) && !(alwaysA || alwaysB);

template<typename T>
void fold_series(const std::set<T>& always_a, const std::set<T>& maybe_a,
                 const std::set<T>& always_b, const std::set<T>& maybe_b,
                 std::set<T>& always_out, std::set<T>& maybe_out) {
  always_out.insert(always_a.begin(), always_a.end());
  always_out.insert(always_b.begin(), always_b.end());

  for (const auto& d : maybe_a) {
    if (!always_out.contains(d)) maybe_out.insert(d);
  }

  for (const auto& d : maybe_b) {
    if (!always_out.contains(d)) maybe_out.insert(d);
  }
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

void log_error(MtNode n, const char* fmt, ...) {
  printf("\n########################################\n");

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  printf("@%04d: ", ts_node_start_point(n.node).row + 1);
  
  /*
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
  */

  //n.error();
  n.dump_tree();

  //printf("halting...\n");
  printf("########################################\n");

  //load_error = true;

  //debugbreak();
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

  for (auto& n : tick_methods) n.is_tick = true;
  for (auto& n : tock_methods) n.is_tock = true;

  // Collect all inputs to all tick and tock methods and merge them into a list
  // of input ports. Input ports can be declared in multiple tick/tock methods,
  // but we don't want duplicates in the Verilog port list.

  std::set<std::string> input_dedup;

  for (auto n : tick_methods) {
    n.is_tick = true;

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

void MtMethod::check_dirty() {
  dirty_check_done = false;
  dirty_check_pass = true;
  check_dirty_dispatch((MtNode)*this);
  dirty_check_done = false;
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_ticks() {
  for (auto& tick : tick_methods) {
    tick.check_dirty();
  }
}

void MtModule::check_dirty_tocks() {
  for (auto& tock : tock_methods) {
    tock.check_dirty();
  }
}

//------------------------------------------------------------------------------

void MtMethod::check_dirty_dispatch(MtNode n) {
  //n.dump_tree();

  if (n.is_null() || !n.is_named()) return;

  //LOG_B("check_dirty_dispatch %s\n", ts_node_type(n.node));
  //LOG_INDENT_SCOPE();

  switch (n.sym) {
    case sym_identifier:            check_dirty_read(n); break;
    case sym_assignment_expression: check_dirty_assign(n); break;
    case sym_if_statement:          check_dirty_if(n); break;
    case sym_call_expression:       check_dirty_call(n); break;
    case sym_switch_statement:      check_dirty_switch(n); break;

    default: {
      for (auto c : n) check_dirty_dispatch(c);
    }
  }
}

//----------------------------------------

void MtMethod::check_dirty_read(MtNode n) {
  //LOG_G("check_dirty_read %s\n", ts_node_type(n.node));

  assert(n.sym == sym_identifier);
  auto field_name = n.text();

  // Reading from a dirty field in tick() is forbidden.
  if (is_tick && (maybe_dirty.contains(field_name) || always_dirty.contains(field_name))) {
    log_error(n, "%s() read dirty field - %s\n", field_name.c_str(), field_name.c_str());
    dirty_check_pass = false;
  }

  // Reading from a clean output in tock() is forbidden.
  if (is_tock && field_name.starts_with("o_") && !always_dirty.contains(field_name)) {
    log_error(n, "%s() read clean output - %s\n", name.c_str(), field_name.c_str());
    dirty_check_pass = false;
  }
}

//----------------------------------------

void MtMethod::check_dirty_write(MtNode n) {
  //LOG_R("check_dirty_write %s\n", ts_node_type(n.node));

  assert(n.sym == sym_identifier);
  auto field_name = n.text();

  // Writing to an output in tick() is forbidden.
  if (is_tick && field_name.starts_with("o_")) {
    log_error(n, "%s() wrote output %s\n", field_name.c_str(), field_name.c_str());
    dirty_check_pass = false;
  }

  // Writing to a field twice is forbidden.
  if (maybe_dirty.contains(field_name) || always_dirty.contains(field_name)) {
    log_error(n, "%s() wrote dirty field %s\n", field_name.c_str(), field_name.c_str());
    dirty_check_pass = false;
  }

  // Writing to a non-output field in tock() is forbidden.
  if (is_tock && !field_name.starts_with("o_")) {
    log_error(n, "%s() wrote non-output %s\n", name.c_str(), field_name.c_str());
    dirty_check_pass = false;
  }

  always_dirty.insert(field_name);
}

//------------------------------------------------------------------------------
// Check for reads on the RHS of an assignment, then check the write on the left.

void MtMethod::check_dirty_assign(MtNode n) {
  //LOG_Y("check_dirty_assign %s\n", ts_node_type(n.node));

  auto lhs = n.get_field(field_left);
  auto rhs = n.get_field(field_right);

  check_dirty_dispatch(rhs);
  check_dirty_write(lhs);
}

//----------------------------------------
// Check the "if" branch and the "else" branch independently and then merge the results.

void MtMethod::check_dirty_if(MtNode n) {
  //LOG_M("check_dirty_if %s\n", ts_node_type(n.node));

  check_dirty_dispatch(n.get_field(field_condition));

  MtMethod if_branch = *this;
  MtMethod else_branch = *this;

  if_branch.check_dirty_dispatch(n.get_field(field_consequence));
  else_branch.check_dirty_dispatch(n.get_field(field_alternative));

  fold_parallel(
    if_branch.always_dirty, if_branch.maybe_dirty,
    else_branch.always_dirty, else_branch.maybe_dirty,
    always_dirty, maybe_dirty);
}

//----------------------------------------
// Traverse function calls.

void MtMethod::check_dirty_call(MtNode n) {
  //LOG_C("check_dirty_call %s\n", ts_node_type(n.node));

  auto node_args = n.get_field(field_arguments);
  assert(node_args.sym == sym_argument_list);
  check_dirty_dispatch(node_args);

  auto node_func = n.get_field(field_function);
  if (node_func.is_identifier()) {
    // local function call, traverse args and then function body
    // TODO - traverse function body
  }
  else if (node_func.is_field_expr()) {
    // submod function call, traverse args and then function body
    // TODO - traverse function body
  }
  else if (node_func.sym == sym_template_function) {
  }
  else {
    n.dump_tree();
    debugbreak();
  }
}

//----------------------------------------
// Check the condition of a switch statement, then check each case independently.

void MtMethod::check_dirty_switch(MtNode n) {
  //LOG_W("check_dirty_switch %s\n", ts_node_type(n.node));

  check_dirty_dispatch(n.get_field(field_condition));

  MtMethod old_method = *this;
  MtMethod accum;

  bool first_branch = true;

  auto body = n.get_field(field_body);
  for (auto c : body) {
    if (c.sym == sym_case_statement) {
      MtMethod case_branch = old_method;
      case_branch.check_dirty_dispatch(c);

      if (first_branch) {
        always_dirty = case_branch.always_dirty;
        maybe_dirty = case_branch.maybe_dirty;
        first_branch = false;
      }
      else {
        fold_parallel(always_dirty, maybe_dirty,
                      case_branch.always_dirty, case_branch.maybe_dirty,
                      accum.always_dirty, accum.maybe_dirty);
        always_dirty.swap(accum.always_dirty);
        maybe_dirty.swap(accum.maybe_dirty);
        accum.always_dirty.clear();
        accum.maybe_dirty.clear();
      }
    }
  }
}

//------------------------------------------------------------------------------
