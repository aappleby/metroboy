#include "MtIterator.h"

#include "MtModule.h"


void print_escaped(const char* source, uint32_t a, uint32_t b);

const MtHandle MtHandle::null;

//------------------------------------------------------------------------------

MtHandle MtHandle::from_mod(MtModule* mod) {
  auto root = ts_tree_root_node(mod->tree);

  return MtHandle(
    root,
    ts_node_symbol(root),
    0,
    mod
  );
}


MtHandle MtHandle::get_field(int field_id) {

  for (auto c : *this) {
    if (c.field == field_id) return c;
  }

  return MtHandle::null;
}

//------------------------------------------------------------------------------

MtHandle MtHandle::child(int i) const {
  auto child = ts_node_child(node, i);
  if (ts_node_is_null(child)) {
    return MtHandle::null;
  }
  else {
    auto sym = ts_node_symbol(child);
    auto field = ts_node_field_id_for_child(node, i);
    return { child, sym, field, mod };
  }
}

//----------

MtHandle MtHandle::first_named_child() const {
  for (int i = 0; i < child_count(); i++) {
    auto c = child(i);
    if (c.is_named()) return c;
  }
  return MtHandle::null;
}

std::string MtHandle::body() {
  assert(!is_null());

  auto a = &mod->source[start_byte()];
  auto b = &mod->source[end_byte()];

  if (sym == anon_sym_LF) return a;

  while (a < b && isspace(a[0])) a++;
  while (b > a && isspace(b[-1])) b--;

  return std::string(a, b);
}

const char* MtHandle::start() {
  auto a = &mod->source[start_byte()];
  auto b = &mod->source[end_byte()];

  if (sym == anon_sym_LF) return a;

  while (a < b && isspace(a[0])) a++;
  return a;
}

const char* MtHandle::end() {
  auto a = &mod->source[start_byte()];
  auto b = &mod->source[end_byte()];

  if (sym == anon_sym_LF) return b;

  while (b > a && isspace(b[-1])) b--;
  return b;
}

bool MtHandle::match(const char* s) {
  const char* a = start();
  const char* b = end();

  while (a != b) {
    if (*a++ != *s++)  return false;
  }
  return true;
}

//------------------------------------------------------------------------------
// Field introspection

bool MtHandle::field_is_primitive() {

  auto node_type = get_field(field_type);
  auto node_decl = get_field(field_declarator);

  // Primitive types are primitive types.
  if (node_type.sym == sym_primitive_type) return true;

  // Logic arrays are primitive types.
  if (node_type.sym == sym_template_type) {
    auto templ_name = node_type.get_field(field_name);
    if (templ_name.match("logic")) return true;
  }

  // Bits are primitive types.
  if (node_type.match("bit")) return true;

  return false;
}

bool MtHandle::field_is_module() {
  return !field_is_primitive();
}

bool MtHandle::field_is_static() {
  for (auto c : *this) {
    if (c.sym == sym_storage_class_specifier) {
      if (c.match("static")) return true;
    }
  }
  return false;
}

bool MtHandle::field_is_const() {
  for (auto c : *this) {
    if (c.sym == sym_type_qualifier) {
      if (c.match("const")) return true;
    }
  }
  return false;
}

bool MtHandle::field_is_param() {
  return field_is_static() && field_is_const();
}

bool MtHandle::field_is_input() {
  if (field_is_static() || field_is_const()) return false;

  auto name = get_field(field_declarator);
  return name.body().starts_with("i_");
}

bool MtHandle::field_is_output() {
  if (field_is_static() || field_is_const()) return false;

  auto name = get_field(field_declarator);
  return name.body().starts_with("o_");
}



void MtHandle::visit_tree(NodeVisitor cv) {
  cv(*this);
  for (auto c : *this) c.visit_tree(cv);
}

void MtHandle::visit_tree2(NodeVisitor2 cv) {
  for (auto c : *this) {
    cv(*this, c);
    c.visit_tree2(cv);
  }
}

//------------------------------------------------------------------------------
// Node debugging

void MtHandle::dump_node(int index, int depth) {
  if (is_null()) {
    printf("### NULL ###\n");
    return;
  }

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

  //if (field > 0) {
  //  printf("%s.", ts_language_field_name_for_id(lang, field));
  //}

  if (is_named() && child_count()) {
    printf("%s: ", type());
  }
  else if (is_named() && !child_count()) {
    printf("%s: ", type());
    ::print_escaped(mod->source, start_byte(), end_byte());
  }
  else {
    // Unnamed nodes usually have their node body as their "type",
    // and their symbol is something like "aux_sym_preproc_include_token1"
    printf("lit: ");
    ::print_escaped(mod->source, start_byte(), end_byte());
  }

  printf("\n");
  printf("\u001b[0m");
}

//------------------------------------------------------------------------------

void MtHandle::dump_tree(int index, int depth, int maxdepth) {
  if (depth == 0) {
    printf("\n========== tree dump begin\n");
  }
  dump_node(index, depth);

  if (!is_null() && depth < maxdepth) {
    for (int i = 0; i < child_count(); i++) {
      child(i).dump_tree(i, depth + 1, maxdepth);
    }
  }
  if (depth == 0) printf("========== tree dump end\n");
}

//------------------------------------------------------------------------------
