#include "MtModule.h"

#include "../CoreLib/Log.h"
#include "MtCursor.h"
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

MtMethod *MtModule::get_method(const std::string &name) {
  for (auto &n : *tick_methods)
    if (n.name == name) return &n;
  for (auto &n : *tock_methods)
    if (n.name == name) return &n;
  return nullptr;
}

//----------------------------------------

bool MtModule::has_enum(const std::string &name) {
  for (auto &n : *enums) {
    if (n.name() == name) return true;
  }
  return false;
}

//----------------------------------------

bool MtModule::has_field(const std::string &name) {
  return get_field(name) != nullptr;
}

MtField *MtModule::get_field(const std::string &name) {
  for (auto &f : *all_fields) {
    if (f.name() == name) return &f;
  }
  return nullptr;
}

//----------------------------------------

bool MtModule::has_input(const std::string &name) {
  for (auto &f : *inputs)
    if (f.name() == name) return true;
  return false;
}

//----------------------------------------

bool MtModule::has_output(const std::string &name) {
  assert(outputs);
  return get_output(name) != nullptr;
}

MtField *MtModule::get_output(const std::string &name) {
  for (auto &n : *outputs)
    if (n.name() == name) return &n;
  return nullptr;
}
//----------------------------------------

bool MtModule::has_register(const std::string &name) {
  for (auto &f : *registers)
    if (f.name() == name) return true;
  return false;
}

//----------------------------------------

bool MtModule::has_submod(const std::string &name) {
  return get_submod(name) != nullptr;
}

MtSubmod *MtModule::get_submod(const std::string &name) {
  for (auto &n : *submods) {
    if (n.name() == name) return &n;
  }
  return nullptr;
}

//------------------------------------------------------------------------------

void MtModule::dump_method_list(std::vector<MtMethod> &methods) {
  for (auto &n : methods) {
    LOG_INDENT_SCOPE();
    LOG_R("%s(", n.name.c_str());

    if (n.params) {
      int param_count = int(n.params->size());
      int param_index = 0;

      for (auto &param : *n.params) {
        LOG_R("%s", param.c_str());
        if (param_index++ != param_count - 1) LOG_C(", ");
      }
    }
    LOG_R(")\n");
  }
}

//------------------------------------------------------------------------------

void MtModule::dump_call_list(std::vector<MtCall> &calls) {
  for (auto &call : calls) {
    LOG_INDENT_SCOPE();
    LOG_C("%s.%s(", call.submod->name().c_str(), call.method->name.c_str());
    if (call.args && call.args->size()) {
      LOG_C("\n");
      LOG_INDENT_SCOPE();

      assert(call.method->params->size() == call.args->size());
      int arg_count = int(call.args->size());
      int arg_index = 0;

      for (auto i = 0; i < arg_count; i++) {
        LOG_C("%s = %s", call.method->params->at(i).c_str(),
              call.args->at(i).c_str());

        if (arg_index++ < arg_count - 1) LOG_C(",\n");
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

  //----------

  LOG_B("Modparams:\n")
  for (auto &param : *modparams) LOG_G("  %s\n", param.name().c_str());
  LOG_B("Localparams:\n");
  for (auto &param : *localparams) LOG_G("  %s\n", param.name().c_str());
  LOG_B("Enums:\n");
  for (auto &n : *enums) LOG_G("  %s\n", n.name().c_str());
  LOG_B("Inputs:\n");
  for (auto &n : *inputs)
    LOG_G("  %s:%s\n", n.name().c_str(), n.type_name().c_str());
  LOG_B("Outputs:\n");
  for (auto &n : *outputs)
    LOG_G("  %s:%s\n", n.name().c_str(), n.type_name().c_str());
  LOG_B("Regs:\n");
  for (auto &n : *registers)
    LOG_G("  %s:%s\n", n.name().c_str(), n.type_name().c_str());
  LOG_B("Submods:\n");
  for (auto &submod : *submods)
    LOG_G("  %s:%s\n", submod.name().c_str(), submod.mod->mod_name.c_str());

  //----------

  LOG_B("Init methods:\n");
  dump_method_list(*init_methods);
  LOG_B("Tick methods:\n");
  dump_method_list(*tick_methods);
  LOG_B("Tock methods:\n");
  dump_method_list(*tock_methods);
  LOG_B("Tasks:\n");
  dump_method_list(*task_methods);
  LOG_B("Functions:\n");
  dump_method_list(*func_methods);
  LOG_B("Tick calls:\n");
  dump_call_list(*tick_calls);
  LOG_B("Tock calls:\n");
  dump_call_list(*tock_calls);

  //----------

  LOG_B("Port map:\n");
  for (auto &kv : *port_map)
    LOG_G("  %s = %s\n", kv.first.c_str(), kv.second.c_str());

  LOG_B("\n");
}

//------------------------------------------------------------------------------

void MtModule::dump_deltas() {
  if (mod_struct.is_null()) return;

  LOG_G("%s\n", mod_name.c_str());
  {
    LOG_INDENT_SCOPE();

    for (auto &tick : *tick_methods) {
      LOG_G("%s error %d\n", tick.name.c_str(), tick.delta->error);
      LOG_INDENT_SCOPE();
      {
        for (auto &s : tick.delta->state_old) {
          LOG_G("%s", s.first.c_str());
          LOG_W(" : ");
          log_field_state(s.second);

          if (tick.delta->state_new.contains(s.first)) {
            auto s2 = tick.delta->state_new[s.first];
            if (s2 != s.second) {
              LOG_W(" -> ");
              log_field_state(s2);
            }
          }

          LOG_G("\n");
        }
      }
    }

    for (auto &tock : *tock_methods) {
      LOG_G("%s error %d\n", tock.name.c_str(), tock.delta->error);
      LOG_INDENT_SCOPE();
      {
        for (auto &s : tock.delta->state_old) {
          LOG_G("%s", s.first.c_str());
          LOG_W(" : ");
          log_field_state(s.second);

          if (tock.delta->state_new.contains(s.first)) {
            auto s2 = tock.delta->state_new[s.first];
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
// All modules are now in the library, we can resolve references to other
// modules when we're collecting fields.

void MtModule::load_pass1() {
  if (mod_struct.is_null()) return;

  collect_params();
  collect_fields();
  collect_methods();
  collect_inputs();
  collect_outputs();
  collect_registers();
  collect_submods();

  enums = new std::vector<MtEnum>();

  auto mod_body = mod_struct.get_field(field_body).check_null();
  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;

    MtField f(n);

    // enum class
    if (n.get_field(::field_type).sym == sym_enum_specifier) {
      enums->push_back(MtEnum(n));
      continue;
    }

    // Everything not an enum shoul have 1 or more declarator fields that
    // contain the field name(s).

    // int field_count = 0;

    /*
    for (auto c : n) {
      if (c.field != field_declarator) continue;
      field_count++;
      auto name =
          c.sym == sym_array_declarator ? c.get_field(field_declarator) : c;

      MtField f(n);

      auto type_name = n.node_to_type();
      if (source_file->lib->has_mod(type_name)) {
        MtSubmod submod(n);
        submod.mod = source_file->lib->get_mod(type_name);
        submod.name = n.get_field(field_declarator).text();
        submods.push_back(submod);
      }
    }
    */
  }

  sanity_check();
}

//------------------------------------------------------------------------------

void MtModule::collect_params() {
  assert(modparams == nullptr);
  modparams = new std::vector<MtParam>();

  if (mod_template) {
    auto params = mod_template.get_field(field_parameters);

    for (int i = 0; i < params.named_child_count(); i++) {
      auto child = params.named_child(i);
      if (child.sym == sym_parameter_declaration ||
          child.sym == sym_optional_parameter_declaration) {
        modparams->push_back(MtParam(child));
      } else {
        debugbreak();
      }
    }
  }

  // localparam = static const int
  assert(localparams == nullptr);
  localparams = new std::vector<MtParam>();
  auto mod_body = mod_struct.get_field(field_body).check_null();
  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;

    if (n.is_static() && n.is_const()) {
      localparams->push_back(MtParam(n));
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::collect_fields() {
  assert(all_fields == nullptr);
  all_fields = new std::vector<MtField>();

  auto mod_body = mod_struct.get_field(field_body).check_null();
  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;
    if (n.get_field(field_type).sym == sym_enum_specifier) continue;
    all_fields->push_back(MtField(n));
  }
}

//------------------------------------------------------------------------------

void MtModule::collect_methods() {
  assert(init_methods == nullptr);
  assert(tick_methods == nullptr);
  assert(tock_methods == nullptr);
  assert(task_methods == nullptr);
  assert(func_methods == nullptr);

  init_methods = new std::vector<MtMethod>();
  tick_methods = new std::vector<MtMethod>();
  tock_methods = new std::vector<MtMethod>();
  task_methods = new std::vector<MtMethod>();
  func_methods = new std::vector<MtMethod>();

  auto mod_body = mod_struct.get_field(field_body).check_null();
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

    if (is_init) {
      init_methods->push_back(node_to_method(n));
    } else if (is_tick) {
      tick_methods->push_back(node_to_method(n));
    } else if (is_tock) {
      tock_methods->push_back(node_to_method(n));
    } else if (is_task) {
      task_methods->push_back(node_to_method(n));
    } else {
      func_methods->push_back(node_to_method(n));
    }
  }

  for (auto &n : *tick_methods) n.is_tick = true;
  for (auto &n : *tock_methods) n.is_tock = true;
}

//------------------------------------------------------------------------------
// Collect all inputs to all tick and tock methods and merge them into a list of
// input ports. Input ports can be declared in multiple tick/tock methods, but
// we don't want duplicates in the Verilog port list.

void MtModule::collect_inputs() {
  assert(inputs == nullptr);
  inputs = new std::vector<MtField>();

  std::set<std::string> dedup;

  for (auto n : *tick_methods) {
    auto params = n.get_field(field_declarator).get_field(field_parameters);
    for (auto param : params) {
      if (param.sym != sym_parameter_declaration) continue;
      MtField f(param);
      if (!dedup.contains(f.name())) {
        inputs->push_back(f);
        dedup.insert(f.name());
      }
    }
  }

  for (auto n : *tock_methods) {
    auto params = n.get_field(field_declarator).get_field(field_parameters);
    for (auto param : params) {
      if (param.sym != sym_parameter_declaration) continue;
      MtField f(param);
      if (!dedup.contains(f.name())) {
        inputs->push_back(f);
        dedup.insert(f.name());
      }
    }
  }
}

//------------------------------------------------------------------------------
// All fields written to in a tock method are outputs.

void MtModule::collect_outputs() {
  assert(outputs == nullptr);
  outputs = new std::vector<MtField>();

  std::set<std::string> dedup;

  for (auto n : *tock_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_assignment_expression) return;

      auto lhs = child.get_field(field_left);
      assert(lhs.sym == sym_identifier);
      auto lhs_name = lhs.text();

      if (dedup.contains(lhs_name)) return;
      dedup.insert(lhs_name);

      auto field = get_field(lhs_name);
      if (field) outputs->push_back(*field);
    });
  }
}

//------------------------------------------------------------------------------
// All fields written to in a tick method are registers.

void MtModule::collect_registers() {
  assert(registers == nullptr);
  registers = new std::vector<MtField>();

  std::set<std::string> dedup;

  for (auto n : *tick_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_assignment_expression) return;

      auto lhs = child.get_field(field_left);
      std::string lhs_name;

      if (lhs.sym == sym_identifier) {
        lhs_name = lhs.text();
      }
      else if (lhs.sym == sym_subscript_expression) {
        lhs_name = lhs.get_field(field_argument).text();
      }
      else {
        lhs.dump_tree();
        debugbreak();
      }

      if (dedup.contains(lhs_name)) return;
      dedup.insert(lhs_name);

      auto field = get_field(lhs_name);
      if (field) registers->push_back(*field);
    });
  }
}

//------------------------------------------------------------------------------

void MtModule::collect_submods() {
  assert(submods == nullptr);
  submods = new std::vector<MtSubmod>();

  auto mod_body = mod_struct.get_field(field_body).check_null();
  for (auto n : mod_body) {
    if (n.sym != sym_field_declaration) continue;

    MtField f(n);

    if (source_file->lib->has_mod(f.type_name())) {
      MtSubmod submod(n);
      submod.mod = source_file->lib->get_mod(f.type_name());
      submods->push_back(submod);
    }
  }
}

//------------------------------------------------------------------------------
// All modules have populated their fields, match up tick/tock calls with their
// corresponding methods.

void MtModule::load_pass2() {
  if (mod_struct.is_null()) return;

  collect_submod_calls();
  build_port_map();
  sanity_check();
}

//------------------------------------------------------------------------------

void MtModule::collect_submod_calls() {
  assert(tick_calls == nullptr);
  tick_calls = new std::vector<MtCall>();

  for (auto n : *tick_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_call_expression) return;
      if (child.get_field(field_function).sym != sym_field_expression) return;
      tick_calls->push_back(node_to_call(child));
    });
  }

  assert(tock_calls == nullptr);
  tock_calls = new std::vector<MtCall>();

  for (auto n : *tock_methods) {
    n.visit_tree([&](MtNode child) {
      if (child.sym != sym_call_expression) return;
      if (child.get_field(field_function).sym != sym_field_expression) return;
      tock_calls->push_back(node_to_call(child));
    });
  }
}

//------------------------------------------------------------------------------

void MtModule::build_port_map() {
  assert(port_map == nullptr);

  port_map = new std::map<std::string, std::string>();

  for (auto &call : *tick_calls) {
    for (auto i = 0; i < call.args->size(); i++) {
      auto key = call.submod->name() + "." + call.method->params->at(i);
      auto val = call.args->at(i);
      auto it = port_map->find(key);
      if (it != port_map->end()) {
        assert((*it).second == val);
      } else {
        port_map->insert({key, val});
      }
    }
  }

  for (auto &call : *tock_calls) {
    for (auto i = 0; i < call.args->size(); i++) {
      auto key = call.submod->name() + "." + call.method->params->at(i);
      auto val = call.args->at(i);
      auto it = port_map->find(key);
      if (it != port_map->end()) {
        assert((*it).second == val);
      } else {
        port_map->insert({key, val});
      }
    }
  }
}

//------------------------------------------------------------------------------

void MtModule::sanity_check() {
  // Inputs, outputs, registers, and submods must not overlap.

  std::set<std::string> field_names;

  for (auto &n : *inputs) {
    assert(!field_names.contains(n.name()));
    field_names.insert(n.name());
  }

  for (auto &n : *outputs) {
    assert(!field_names.contains(n.name()));
    field_names.insert(n.name());
  }

  for (auto &n : *registers) {
    auto name = n.name();
    assert(!field_names.contains(name));
    field_names.insert(n.name());
  }

  for (auto &n : *submods) {
    assert(!field_names.contains(n.name()));
    field_names.insert(n.name());
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
  result.params = new std::vector<std::string>();

  for (int i = 0; i < method_params.child_count(); i++) {
    auto param = method_params.child(i);
    if (param.sym != sym_parameter_declaration) continue;

    auto param_name = param.get_field(field_declarator).text();
    result.params->push_back(param_name);
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

  result.args = new std::vector<std::string>();

  if (call_args.named_child_count()) {
    for (int i = 0; i < call_args.named_child_count(); i++) {
      auto arg_node = call_args.named_child(i);

      std::string out_string;
      MtCursor cursor(source_file->lib, source_file, &out_string);
      cursor.cursor = arg_node.start();
      cursor.emit_dispatch(arg_node);
      result.args->push_back(out_string);
    }
  }

  return result;
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_ticks() {
  for (auto &tick : *tick_methods) {
    tick.update_delta();
    dirty_check_fail |= tick.delta->error;
  }
}

//------------------------------------------------------------------------------

void MtModule::check_dirty_tocks() {
  for (auto &tock : *tock_methods) {
    tock.update_delta();
    dirty_check_fail |= tock.delta->error;
  }
}

//------------------------------------------------------------------------------
