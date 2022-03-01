#include "MtMethod.h"

#include "MtModule.h"
#include "../CoreLib/Log.h"

void log_error(MtNode n, const char* fmt, ...);

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

MtMethod::MtMethod(MtNode n, MtModule* _mod) : MtNode(n), mod(_mod) {
}

void MtMethod::check_dirty() {
  if (!dirty_check_done) {
    auto body = get_field(field_body);
    //body.dump_tree();
    check_dirty_dispatch(body);
    dirty_check_done = true;
  }
}

//------------------------------------------------------------------------------

void MtMethod::check_dirty_dispatch(MtNode n) {
  //n.dump_tree();

  if (n.is_null() || !n.is_named()) return;

  //LOG_B("check_dirty_dispatch %s\n", ts_node_type(n.node));
  //LOG_INDENT_SCOPE();

  switch (n.sym) {

    case sym_field_expression:      check_dirty_read_submod(n); break;
    case sym_identifier:            check_dirty_read_identifier(n); break;
    case sym_assignment_expression: check_dirty_assign(n); break;
    case sym_if_statement:          check_dirty_if(n); break;
    case sym_call_expression: {
      if (n.get_field(field_function).sym == sym_field_expression) {
        // submod.tick()/tock()
      }
      else if (n.get_field(field_function).sym == sym_template_function) {
        // foo = bx<x>(bar);
      }
      else {
        // cat() etc
        //n.dump_tree();
      }
      check_dirty_call(n);
      break;
    }
    case sym_switch_statement:      check_dirty_switch(n); break;

    default: {
      for (auto c : n) check_dirty_dispatch(c);
    }
  }
}

//----------------------------------------

void MtMethod::check_dirty_read_identifier(MtNode n) {
  //LOG_G("check_dirty_read %s\n", ts_node_type(n.node));

  assert(n.sym == sym_identifier);
  auto field = n.text();

  //printf("field %s\n", field.c_str());

  // Reading from a dirty field in tick() is forbidden.
  if (is_tick && mod->has_field(field) && (maybe_dirty.contains(field) || always_dirty.contains(field))) {
    log_error(n, "%s() read dirty field - %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  // Reading from a clean output in tock() is forbidden.
  if (is_tock && mod->has_output(field) && !always_dirty.contains(field)) {
    log_error(n, "%s() read clean output - %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }
}

//----------------------------------------

void MtMethod::check_dirty_read_submod(MtNode n) {
  //LOG_G("check_dirty_read %s\n", ts_node_type(n.node));

  assert(n.sym == sym_field_expression);
  auto field = n.text();

  if (field.find(".o_") == std::string::npos) {
    log_error(n, "%s() read non-output from submodule - %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  //printf("field %s\n", field.c_str());

  // Reading from a dirty field in tick() is forbidden.
  if (is_tick && (maybe_dirty.contains(field) || always_dirty.contains(field))) {
    log_error(n, "%s() read dirty field - %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  // Reading from a clean output in tock() is forbidden.
  if (is_tock && !always_dirty.contains(field)) {
    log_error(n, "%s() read clean output - %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }
}

//----------------------------------------

void MtMethod::check_dirty_write(MtNode n) {
  //LOG_R("check_dirty_write %s\n", ts_node_type(n.node));

  if (n.sym == sym_subscript_expression) {
    return check_dirty_write(n.get_field(field_argument));
  }

  if (n.sym == sym_call_expression) {
    // assign to slice of a logic
    auto func_name = n.get_field(field_function).text();
    for (int i = 0; i < func_name.size(); i++) {
      if (i) {
        assert(isdigit(func_name[i]));
      }
      else {
        assert(func_name[i] == 's');
      }
    }
    auto field_node = n.get_field(field_arguments).named_child(0);
    field_node.dump_tree();
    return check_dirty_write(field_node);
  }

  std::string field = "";

  if (n.sym == sym_identifier) {
    field = n.text();
  }
  else {
    n.dump_tree();
    assert(false);
  }

  // Writing to a field twice is forbidden.
  if (maybe_dirty.contains(field) || always_dirty.contains(field)) {
    log_error(n, "%s() wrote dirty field %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  // Writing to an output in tick() is forbidden.
  if (is_tick && mod->has_output(field)) {
    log_error(n, "%s() wrote output %s\n", field.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  // Writing to a field in tock() is forbidden.
  if (is_tock && mod->has_field(field)) {
    log_error(n, "%s() wrote non-output %s\n", name.c_str(), field.c_str());
    dirty_check_fail = true;
  }

  if (mod->has_field(field) || mod->has_output(field)) {
    always_dirty.insert(field);
    maybe_dirty.erase(field);
  }
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
  auto call = mod->node_to_call(n);

  //LOG_C("check_dirty_call %s\n", ts_node_type(n.node));

  auto node_args = call.get_field(field_arguments);
  assert(node_args.sym == sym_argument_list);
  check_dirty_dispatch(node_args);

  auto node_func = call.get_field(field_function);

  if (node_func.is_identifier()) {
    // local function call, traverse args and then function body
    // TODO - traverse function body
    //printf("%s\n", node_func.text().c_str());
  }
  else if (node_func.is_field_expr()) {
    //assert(call.method);
    //call.method->check_dirty();

    {
      LOG_G("%s.%s\n", call.submod->name.c_str(), call.method->name.c_str());
      LOG_INDENT_SCOPE();
      call.method->check_dirty();
    }

    name_set call_always;
    name_set call_maybe;

    for (auto always : call.method->always_dirty) {
      auto field_name = call.submod->name + "." + always;
      //printf("%s always dirty\n", field_name.c_str());
      call_always.insert(field_name);
    }
    
    for (auto maybe : call.method->maybe_dirty) {
      auto field_name = call.submod->name + "." + maybe;
      //printf("%s maybe dirty\n", field_name.c_str());
      call_maybe.insert(field_name);
    }

    name_set temp_always;
    name_set temp_maybe;

    temp_always.swap(always_dirty);
    temp_maybe.swap(maybe_dirty);

    fold_series(call_always, call_maybe,
                temp_always, temp_maybe,
                always_dirty, maybe_dirty);

    //call.submod->get_

    //node_func.dump_tree();
    // submod function call, traverse args and then function body
    // TODO - traverse function body
    //printf("%s\n", node_func.text().c_str());
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
  name_set accum_always;
  name_set accum_maybe;

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
                      accum_always, accum_maybe);
        always_dirty.swap(accum_always);
        maybe_dirty.swap(accum_maybe);
        accum_always.clear();
        accum_maybe.clear();
      }
    }
  }
}

//------------------------------------------------------------------------------
