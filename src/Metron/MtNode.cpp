#include "MtNode.h"
#include "Platform.h"

#include "MtNode.h"

#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtSourceFile.h"

void print_escaped(const char* source, uint32_t a, uint32_t b);

const MtNode MtNode::null;

//------------------------------------------------------------------------------

/*
MtNode MtNode::from_mod(MtModule* mod) {
  auto root = ts_tree_root_node(mod->source_file->tree);

  return MtNode(
    root,
    ts_node_symbol(root),
    0,
    mod
  );
}
*/

MtNode MtNode::get_field(int field_id) {
  if (is_null()) return MtNode::null;

  for (auto c : *this) {
    if (c.field == field_id) return c;
  }

  return MtNode::null;
}

//------------------------------------------------------------------------------

MtNode MtNode::child(int index) const {
  int i = 0;
  for (auto c : *this) {
    if (index == i) return c;
    i++;
  }
  return MtNode::null;
}

//----------

MtNode MtNode::named_child(int index) const {
  int i = 0;
  for (auto c : *this) {
    if (!c.is_named()) continue;
    if (index == i) return c;
    i++;
  }
  return MtNode::null;
}

//----------

MtNode MtNode::first_named_child() const {
  return named_child(0);
}

bool MtNode::is_static() const {
  for (auto c : *this) {
    if (c.sym == sym_storage_class_specifier && c.text() == "static") return true;
  }
  return false;
}

bool MtNode::is_const() const {
  for (auto c : *this) {
    if (c.sym == sym_type_qualifier && c.text() == "const") return true;
  }
  return false;
}


//------------------------------------------------------------------------------

std::string MtNode::text() {
  assert(!is_null());

  if (sym == anon_sym_SEMI) {
    int x = 0;
  }

  auto a = &source->source[start_byte()];
  auto b = &source->source[end_byte()];

  if (sym == anon_sym_LF) return a;

  while (a < b && isspace(a[0])) a++;
  while (b > a && isspace(b[-1])) b--;

  return std::string(a, b);
}

const char* MtNode::start() {
  assert(!is_null());
  auto a = &source->source[start_byte()];
  auto b = &source->source[end_byte()];

  if (sym == anon_sym_LF) return a;

  while (a < b && isspace(a[0])) a++;
  return a;
}

const char* MtNode::end() {
  assert(!is_null());
  auto a = &source->source[start_byte()];
  auto b = &source->source[end_byte()];

  if (sym == anon_sym_LF) return b;

  while (b > a && isspace(b[-1])) b--;
  return b;
}

bool MtNode::match(const char* s) {
  assert(!is_null());
  const char* a = start();
  const char* b = end();

  while (a != b) {
    if (*a++ != *s++)  return false;
  }
  return true;
}

//------------------------------------------------------------------------------


std::string MtNode::node_to_name() {
  assert(!is_null());

  switch (sym) {

  // BUG: Field tags wrong, just pull out last child.
  /*
  ========== tree dump begin
  [0] f15 s354 function.qualified_identifier:
  |   [0] f30 s321 scope.template_type:
  |   |   [0] f22 s395 name.type_identifier: "logic"
  |   |   [1] f3 s324 arguments.template_argument_list:
  |   |   |   [0] s36 lit: "<"
  |   |   |   [1] s264 type_descriptor:
  |   |   |   |   [0] f32 s395 type.type_identifier: "cycle_bits"
  |   |   |   [2] s33 lit: ">"
  |   [1] f22 s43 name.lit: "::"
  |   [2] s1 identifier: "coerce"
  ========== tree dump end
  */
  case sym_qualified_identifier:
    return child(child_count() - 1).node_to_name();

  case sym_field_expression:
    //return get_field(field_field).node_to_name();
    return text();

  case sym_call_expression:
    return get_field(field_function).node_to_name();

  case alias_sym_type_identifier:
  case sym_identifier:
  case alias_sym_field_identifier:
    return text();

  case sym_field_declaration: {
    auto node_type = get_field(field_type);
    if (node_type.sym == sym_enum_specifier) {
      return node_type.node_to_name();
    }
    else {
      return get_field(field_declarator).node_to_name();
    }
  }

  case sym_array_declarator:
  case sym_parameter_declaration:
  case sym_optional_parameter_declaration:
  case sym_function_definition:
  case sym_function_declarator:
    return get_field(field_declarator).node_to_name();

  case sym_enum_specifier: {
    auto name = get_field(field_name);
    if (name.is_null()) {
      return "<anonymous enum>";
    }
    else {
      return get_field(field_name).node_to_name();
    }
  }

  case sym_struct_specifier:
  case sym_class_specifier:
  case sym_template_function:
    return get_field(field_name).node_to_name();

  case sym_primitive_type:
    return text();

  default:
    error();
    return "";
  }
}

std::string MtNode::node_to_type() {
  switch (sym) {
  case alias_sym_type_identifier: return text();
  case sym_primitive_type:        return text();
  case sym_field_declaration:     return get_field(field_type).node_to_type();
  case sym_template_type:         return get_field(field_name).node_to_type();
  case sym_enum_specifier:        return get_field(field_name).node_to_type();
  default:                        error(); return "";
  }
}

//------------------------------------------------------------------------------

void MtNode::visit_tree(NodeVisitor cv) {
  cv(*this);
  for (auto c : *this) {
    if (c.is_null()) {
      printf("NULL!\n");
      debugbreak();
    }
    c.visit_tree(cv);
  }
}

//------------------------------------------------------------------------------
// Node debugging

/*
int cprintf(uint32_t color, const char *format, ...) {
  printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
  va_list args;
  va_start (args, format);
  auto r = vprintf (format, args);
  va_end (args);
  printf("\u001b[0m");
  return r;
}
*/

/*
void print_escaped(char s) {
  if      (s == '\n') printf("\\n");
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
*/

void MtNode::dump_node(int index, int depth) const {
  if (is_null()) {
    printf("### NULL ###\n");
    return;
  }

  uint32_t color = 0x888888;
  if (sym == sym_template_declaration) color = 0xAADDFF;
  if (sym == sym_struct_specifier)     color = 0xFFAAFF;
  if (sym == sym_class_specifier)      color = 0xFFAAFF;
  if (sym == sym_expression_statement) color = 0xAAFFFF;
  if (sym == sym_expression_statement) color = 0xAAFFFF;
  if (sym == sym_compound_statement)   color = 0xFFFFFF;
  if (sym == sym_function_definition)  color = 0xAAAAFF;
  if (sym == sym_field_declaration)    color = 0xFFAAAA;
  if (sym == sym_comment)              color = 0xAAFFAA;

  printf("[%02d:%03d:%03d] ", index, int16_t(field), int16_t(sym));

  for (int i = 0; i < depth; i++) printf(color != 0x888888 ? "|--" : "|  ");

  if (field) printf("%s: ", ts_language_field_name_for_id(source->lang, field));

  printf("%s = ", is_named() ? type() : "lit" );

  if (!child_count()) print_escaped(source->source, start_byte(), end_byte());

  printf("\n");
}

//------------------------------------------------------------------------------

void MtNode::dump_tree(int index, int depth, int maxdepth) const {
  if (depth == 0) {
    printf("\n========== tree dump begin\n");
  }
  dump_node(index, depth);

  if (!is_null() && depth < maxdepth) {
    /*
    for (int i = 0; i < child_count(); i++) {
      child(i).dump_tree(i, depth + 1, maxdepth);
    }
    */
    int i = 0;
    for (auto c : *this) {
      c.dump_tree(i, depth + 1, maxdepth);
      i++;
    }
  }
  if (depth == 0) printf("========== tree dump end\n");
}

//------------------------------------------------------------------------------
