#include "MtModule.h"

#include "../CoreLib/Log.h"
#include "MtMethod.h"
#include "MtModLibrary.h"
#include "MtNode.h"
#include "MtSourceFile.h"
#include "Platform.h"
#include "tree_sitter/api.h"

#pragma warning(disable : 4996)  // unsafe fopen()

extern "C" {
extern const TSLanguage *tree_sitter_cpp();
}

void log_field_state(FieldState s) {
  switch (s) {
    case CLEAN:
      LOG_G("clean");
      break;
    case MAYBE:
      LOG_Y("maybe");
      break;
    case DIRTY:
      LOG_R("dirty");
      break;
    case ERROR:
      LOG_M("error");
      break;
  }
}

// FIXME
void log_error(MtNode n, const char *fmt, ...) {
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

  // n.error();
  n.dump_tree();

  // printf("halting...\n");
  printf("########################################\n");
}

//------------------------------------------------------------------------------

MtModule::MtModule(MtSourceFile *source_file, MtTemplateDecl node)
    : source_file(source_file) {
  mod_template = node;
  mod_param_list = MtTemplateParamList(mod_template.child(1));
  mod_struct = MtClassSpecifier(mod_template.child(2));
  mod_name = mod_struct.get_field(field_name).text();
}

//------------------------------------------------------------------------------

MtModule::MtModule(MtSourceFile *source_file, MtClassSpecifier node)
    : source_file(source_file) {
  mod_template = MtNode::null;
  mod_param_list = MtNode::null;
  mod_struct = node;
  mod_name = mod_struct.get_field(field_name).text();
}

//------------------------------------------------------------------------------

MtSubmod *MtModule::get_submod(const std::string &name) {
  for (auto &n : submods) {
    if (n.name == name) return &n;
  }
  return nullptr;
}

MtMethod *MtModule::get_method(const std::string &name) {
  for (auto &n : tick_methods)
    if (n.name == name) return &n;
  for (auto &n : tock_methods)
    if (n.name == name) return &n;
  return nullptr;
}

MtField *MtModule::get_output(const std::string &name) {
  for (auto &n : outputs)
    if (n.name == name) return &n;
  return nullptr;
}

bool MtModule::has_submod(const std::string &name) {
  for (auto &n : submods) {
    if (n.name == name) return true;
  }
  return false;
}

bool MtModule::has_enum(const std::string &name) {
  for (auto &n : enums) {
    if (n.name == name) return true;
  }
  return false;
}

bool MtModule::has_field(const std::string &name) {
  for (auto &f : fields)
    if (f.name == name) return true;
  return false;
}

bool MtModule::has_output(const std::string &name) {
  for (auto &f : outputs)
    if (f.name == name) return true;
  return false;
}

//------------------------------------------------------------------------------

void MtModule::dump_method_list(std::vector<MtMethod> &methods) {
  for (auto &n : methods) {
    LOG_INDENT_SCOPE();
    LOG_R("%s(", n.name.c_str());
    for (int i = 0; i < n.params.size(); i++) {
      auto &param = n.params[i];
      LOG_R("%s", param.c_str());
      if (i != n.params.size() - 1) LOG_C(", ");
    }
    LOG_R(")\n");
  }
}

//------------------------------------------------------------------------------

void MtModule::dump_call_list(std::vector<MtCall> &calls) {
  for (auto &call : calls) {
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
    LOG_Y("// Package %s\n", source_file->full_path.c_str());
    LOG_Y("\n");
    return;
  }

  LOG_Y("// Module %s\n", mod_name.c_str());
  LOG_Y("\n");

  if (modparams.size()) {
    LOG_B("Modparams:\n")
    LOG_INDENT_SCOPE();
    for (auto &param : modparams) LOG_G("%s\n", param.name.c_str());
  }

  if (localparams.size()) {
    LOG_B("Localparams:\n");
    LOG_INDENT_SCOPE();
    for (auto &param : localparams) LOG_G("%s\n", param.name.c_str());
  }

  if (enums.size()) {
    LOG_B("Enums:\n");
    LOG_INDENT_SCOPE();
    for (auto &n : enums) LOG_G("%s\n", n.name.c_str());
  }

  if (inputs.size()) {
    LOG_B("Inputs:\n");
    LOG_INDENT_SCOPE();
    for (auto &n : inputs)
      LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (outputs.size()) {
    LOG_B("Outputs:\n");
    LOG_INDENT_SCOPE();
    for (auto &n : outputs)
      LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (fields.size()) {
    LOG_B("Fields:\n");
    LOG_INDENT_SCOPE();
    for (auto &n : fields)
      LOG_G("%s:%s\n", n.name.c_str(), n.type_name.c_str());
  }

  if (submods.size()) {
    LOG_B("Submods:\n");
    LOG_INDENT_SCOPE();
    for (auto &submod : submods)
      LOG_G("%s:%s\n", submod.name.c_str(), submod.mod->mod_name.c_str());
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
    for (auto &kv : port_map)
      LOG_G("%s = %s\n", kv.first.c_str(), kv.second.c_str());
  }

  LOG_B("\n");
}

//------------------------------------------------------------------------------

void MtModule::dump_deltas() {
  if (mod_struct.is_null()) return;

  LOG_G("%s\n", mod_name.c_str());
  {
    LOG_INDENT_SCOPE();

    for (auto &tick : tick_methods) {
      LOG_G("%s delta valid %d error %d\n", tick.name.c_str(),
            tick.delta2.valid, tick.delta2.error);
      LOG_INDENT_SCOPE();
      {
        for (auto &s : tick.delta2.state_old) {
          LOG_G("%s", s.first.c_str());
          LOG_W(" : ");
          log_field_state(s.second);

          if (tick.delta2.state_new.contains(s.first)) {
            auto s2 = tick.delta2.state_new[s.first];
            if (s2 != s.second) {
              LOG_W(" -> ");
              log_field_state(s2);
            }
          }

          LOG_G("\n");
        }
      }
    }

    for (auto &tock : tock_methods) {
      LOG_G("%s delta valid %d error %d\n", tock.name.c_str(),
            tock.delta2.valid, tock.delta2.error);
      LOG_INDENT_SCOPE();
      {
        for (auto &s : tock.delta2.state_old) {
          LOG_G("%s", s.first.c_str());
          LOG_W(" : ");
          log_field_state(s.second);

          if (tock.delta2.state_new.contains(s.first)) {
            auto s2 = tock.delta2.state_new[s.first];
            if (s2 != s.second) {
              LOG_W(" -> ");
              log_field_state(s2);
            }
          }

          LOG_G("\n");
        }
      }
    }
  }
  LOG_G("\n");
}

//------------------------------------------------------------------------------

void MtModule::load_pass1() {
  if (mod_struct.is_null()) return;

  // We've loaded and parsed a module, we can start pulling stuff out of it.

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
      } else {
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

    if (is_init)
      init_methods.push_back(node_to_method(n));
    else if (is_tick)
      tick_methods.push_back(node_to_method(n));
    else if (is_tock)
      tock_methods.push_back(node_to_method(n));
    else if (is_task)
      task_methods.push_back(node_to_method(n));
    else
      func_methods.push_back(node_to_method(n));
  }

  for (auto &n : tick_methods) n.is_tick = true;
  for (auto &n : tock_methods) n.is_tock = true;

  // Collect all inputs to all tick and tock methods and merge them into a list
  // of input ports. Input ports can be declared in multiple tick/tock methods,
  // but we don't want duplicates in the Verilog port list.

  std::set<std::string> input_dedup;

  for (auto n : tick_methods) {
    n.is_tick = true;

    for (auto tick_arg :
         n.get_field(field_declarator).get_field(field_parameters)) {
      if (tick_arg.sym == sym_parameter_declaration) {
        auto arg_type = tick_arg.get_field(field_type);
        auto arg_name = tick_arg.get_field(field_declarator);

        if (arg_name.is_null()) n.error();

        if (!input_dedup.contains(arg_name.text())) {
          inputs.push_back(
              {tick_arg, arg_type.node_to_type(), arg_name.text()});
          input_dedup.insert(arg_name.text());
        }
      }
    }
  }

  for (auto n : tock_methods) {
    for (auto tock_arg :
         n.get_field(field_declarator).get_field(field_parameters)) {
      if (tock_arg.sym == sym_parameter_declaration) {
        auto arg_type = tock_arg.get_field(field_type);
        auto arg_name = tock_arg.get_field(field_declarator);

        if (arg_name.is_null()) n.error();

        if (!input_dedup.contains(arg_name.text())) {
          inputs.push_back(
              {tock_arg, arg_type.node_to_type(), arg_name.text()});
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

    int field_count = 0;

    for (auto c : n) {
      if (c.field == field_declarator) {
        field_count++;
        auto name =
            c.sym == sym_array_declarator ? c.get_field(field_declarator) : c;

        MtField f = {n, field_type.node_to_type(), name.text()};

        if (name.text().starts_with("o_")) {
          outputs.push_back(f);
        } else {
          auto type_name = n.node_to_type();
          if (source_file->lib->has_mod(type_name)) {
            MtSubmod submod(n);
            submod.mod = source_file->lib->get_mod(type_name);
            submod.name = n.get_field(field_declarator).text();
            submods.push_back(submod);
          } else {
            fields.push_back(f);
          }
        }
      }
    }

    assert(field_count == 1);
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
      tick_calls.push_back(node_to_call(child));
    });
  }

  for (auto n : tock_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_call_expression) return;
      if (child.get_field(field_function).sym != sym_field_expression) return;
      tock_calls.push_back(node_to_call(child));
    });
  }

  for (auto &call : tick_calls) {
    for (auto i = 0; i < call.args.size(); i++) {
      auto key = call.submod->name + "." + call.method->params[i];
      auto val = call.args[i];
      auto it = port_map.find(key);
      if (it != port_map.end()) {
        assert((*it).second == val);
      } else {
        port_map.insert({key, val});
      }
    }
  }

  for (auto &call : tock_calls) {
    for (auto i = 0; i < call.args.size(); i++) {
      auto key = call.submod->name + "." + call.method->params[i];
      auto val = call.args[i];
      auto it = port_map.find(key);
      if (it != port_map.end()) {
        assert((*it).second == val);
      } else {
        port_map.insert({key, val});
      }
    }
  }
}

//------------------------------------------------------------------------------

MtMethod MtModule::node_to_method(MtNode n) {
  assert(n.sym == sym_function_definition);

  MtMethod result(n, this);

  auto method_name =
      n.get_field(field_declarator).get_field(field_declarator).text();
  auto method_params =
      n.get_field(field_declarator).get_field(field_parameters);

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

MtCall MtModule::node_to_call(MtNode n) {
  MtCall result(n);

  auto call = MtCallExpr(n);
  auto call_func = call.get_field(field_function);
  auto call_args = call.get_field(field_arguments);

  if (call_func.sym == sym_field_expression) {
    auto call_this = call_func.get_field(field_argument);
    auto call_method = call_func.get_field(field_field);

    auto submod = get_submod(call_this.text());
    assert(submod);

    result.submod = submod;
    result.method = submod->mod->get_method(call_method.text());
  }

  for (int i = 0; i < call_args.named_child_count(); i++) {
    result.args.push_back(call_args.named_child(i).text());
  }

  return result;
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_ticks() {
  for (auto &tick : tick_methods) {
    tick.update_delta();
    dirty_check_fail |= tick.delta2.error;
  }
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_tocks() {
  for (auto &tock : tock_methods) {
    tock.update_delta();
    dirty_check_fail |= tock.delta2.error;
  }
}

//------------------------------------------------------------------------------
