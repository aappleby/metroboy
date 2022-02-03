#include "MtModule.h"
#include <assert.h>

#include "MtIterator.h"
#include "Platform.h"
#include "../Plait/TreeSymbols.h"

#include <assert.h>

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

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());
  root = ts_tree_root_node(tree);

  find_module();
  collect_moduleparams();
  collect_fields();
}

//------------------------------------------------------------------------------

MtHandle MtModule::get_by_id(std::vector<MtHandle>& handles, MtHandle id) {
  assert(id.sym == sym_identifier);
  auto name_a = node_to_name(id);

  for (auto& c : handles) {
    auto name_b = node_to_name(c);
    if (name_a == name_b) return c;
  }

  return MtHandle::null;
}

//------------------------------------------------------------------------------
// Node debugging

void MtModule::dump_node(MtHandle n, int index, int field, int depth) {
  if (!n) {
    printf("### NULL ###\n");
    return;
  }

  uint32_t color = 0x00000000;
  if (n.sym == sym_template_declaration) color = 0xAADDFF;
  if (n.sym == sym_struct_specifier)     color = 0xFFAAFF;
  if (n.sym == sym_class_specifier)      color = 0xFFAAFF;
  if (n.sym == sym_expression_statement) color = 0xAAFFFF;
  if (n.sym == sym_expression_statement) color = 0xAAFFFF;
  if (n.sym == sym_compound_statement)   color = 0xFFFFFF;
  if (n.sym == sym_function_definition)  color = 0xAAAAFF;
  if (n.sym == sym_field_declaration)    color = 0xFFAAAA;
  if (n.sym == sym_comment)              color = 0xAAFFAA;

  if (color) {
    printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
    for (int i = 0; i < depth; i++) printf("|---");
  }
  else {
    for (int i = 0; i < depth; i++) printf("|   ");
  }

  printf("[%d] ", index);

  if (field != -1) printf("f%d ", field);
  if (n.sym != -1) printf("s%d ", n.sym);

  if (field != -1) {
    printf("%s.", ts_language_field_name_for_id(lang, field));
  }

  if (n && n.is_named() && n.child_count()) {

    printf("%s: ", n.type());
  }
  else if (n && n.is_named() && !n.child_count()) {
    printf("%s: ", n.type());
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }
  else if (!n) {
    debugbreak();
    printf("text: ");
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }
  else {
    // Unnamed nodes usually have their node body as their "type",
    // and their symbol is something like "aux_sym_preproc_include_token1"
    printf("lit: ");
    ::print_escaped(source, n.start_byte(), n.end_byte());
  }

  printf("\n");
  printf("\u001b[0m");
}

//------------------------------------------------------------------------------

void MtModule::dump_tree(MtHandle n, int index, int field, int depth, int maxdepth) {
  if (depth == 0) {
    printf("\n========== tree dump begin\n");
  }
  dump_node(n, index, field, depth);

  if (depth < maxdepth) {
    if (n) {
      int index = 0;
      for (auto c : n) {
        dump_tree(c, index++, c.field, depth + 1, maxdepth);
      }
    }
  }
  if (depth == 0) printf("========== tree dump end\n");
}

//------------------------------------------------------------------------------
// Node traversal

void MtModule::visit_tree(MtHandle n, NodeVisitor cv) {
  cv(n);
  for (auto c : n) visit_tree(c, cv);
}

void MtModule::visit_tree2(MtHandle n, NodeVisitor2 cv) {
  for (auto c : n) {
    cv(n, c);
    visit_tree2(c, cv);
  }
}

//------------------------------------------------------------------------------
// Strip leading/trailing whitespace off non-SYM_LF nodes.

const char* MtModule::start(MtHandle n) {
  assert(n);

  auto a = &source[n.start_byte()];
  auto b = &source[n.end_byte()];

  if (n.sym == anon_sym_LF) return a;

  while (a < b && isspace(a[0])) a++;
  return a;
}

const char* MtModule::end(MtHandle n) {
  assert(n);

  auto a = &source[n.start_byte()];
  auto b = &source[n.end_byte()];

  if (n.sym == anon_sym_LF) return b;

  while (b > a && isspace(b[-1])) b--;
  return b;
}

//------------------------------------------------------------------------------

std::string MtModule::body(MtHandle n) {
  return std::string(start(n), end(n));
}

bool MtModule::match(MtHandle n, const char* s) {
  assert(n);

  const char* a = start(n);
  const char* b = end(n);

  while (a != b) {
    if (*a++ != *s++)  return false;
  }
  return true;
}

std::string MtModule::node_to_name(MtHandle n) {
  switch (n.sym) {
  
  case sym_field_expression:
  case alias_sym_type_identifier:
  case sym_identifier:
  case alias_sym_field_identifier:
    return body(n);

  case sym_array_declarator:
  case sym_parameter_declaration:
  case sym_field_declaration:
  case sym_optional_parameter_declaration:
  case sym_function_definition:
  case sym_function_declarator:
    return node_to_name(n.get_field(field_declarator));

  case sym_struct_specifier:
  case sym_class_specifier:
    return node_to_name(n.get_field(field_name));

  default:
    dump_tree(n);
    debugbreak();
    return "";
  }
}

std::string MtModule::node_to_type(MtHandle n) {
  switch (n.sym) {
  case alias_sym_type_identifier:
    return body(n);

  case sym_field_declaration:
    return node_to_type(n.get_field(field_type));

  case sym_template_type:
    return node_to_type(n.get_field(field_name));

  default:
    dump_tree(n);
    debugbreak();
    return "";
  }
}

//------------------------------------------------------------------------------
// Field introspection

bool MtModule::field_is_primitive(MtHandle n) {

  auto node_type = n.get_field(field_type);
  auto node_decl = n.get_field(field_declarator);

  // Primitive types are primitive types.
  if (node_type.sym == sym_primitive_type) return true;

  // Logic arrays are primitive types.
  if (node_type.sym == sym_template_type) {
    auto templ_name = node_type.get_field(field_name);
    if (match(templ_name, "logic")) return true;
  }

  // Bits are primitive types.
  if (match(node_type, "bit")) return true;

  return false;
}

bool MtModule::field_is_module(MtHandle n) {
  return !field_is_primitive(n);
}

bool MtModule::field_is_static(MtHandle n) {
  for (auto c : n) {
    if (c.sym == sym_storage_class_specifier) {
      if (match(c, "static")) return true;
    }
  }
  return false;
}

bool MtModule::field_is_const(MtHandle n) {
  for (auto c : n) {
    if (c.sym == sym_type_qualifier) {
      if (match(c, "const")) return true;
    }
  }
  return false;
}

bool MtModule::field_is_param(MtHandle n) {
  return field_is_static(n) && field_is_const(n);
}

bool MtModule::field_is_input(MtHandle n) {
  if (field_is_static(n) || field_is_const(n)) return false;

  auto name = n.get_field(field_declarator);
  return body(name).starts_with("i_");
}

bool MtModule::field_is_output(MtHandle n) {
  if (field_is_static(n) || field_is_const(n)) return false;

  auto name = n.get_field(field_declarator);
  return body(name).starts_with("o_");
}

//------------------------------------------------------------------------------
// Scanner

void MtModule::find_module() {
  module_template = MtHandle();
  module_class = MtHandle();

  visit_tree2(root, [&](MtHandle parent, MtHandle child) {
    if (child.sym == sym_struct_specifier || child.sym == sym_class_specifier) {
      if (parent.sym == sym_template_declaration) module_template = parent;
      module_class = child;

      auto name_node = module_class.get_field(field_name);
      module_name = body(name_node);
    }
    });

}

void MtModule::collect_moduleparams() {
  if (!module_template) return;

  if (module_template.sym != sym_template_declaration) debugbreak();

  auto params = module_template.get_field(field_parameters);

  for (auto child : params) {
    if (child.sym == sym_parameter_declaration ||
        child.sym == sym_optional_parameter_declaration) {
      moduleparams.push_back(child);
    }
  }
}


void MtModule::collect_fields() {
  visit_tree(module_class, [&](MtHandle n) {
    if (n.sym == sym_function_definition) {
      auto func_name = n.get_field(field_declarator).get_field(field_declarator);
      auto func_args = n.get_field(field_declarator).get_field(field_parameters);

      if (match(func_name, "tick")) {
        visit_tree(func_args, [&](MtHandle func_arg) {
          if (func_arg.sym == sym_parameter_declaration) {
            auto arg_name = func_arg.get_field(field_declarator);

            if (!match(arg_name, "rst_n")) {
              inputs.push_back(func_arg);
            }
          }
        });
      }
    }
  });

  visit_tree(module_class, [&](MtHandle n) {
    if (n.sym == sym_field_declaration) {
      if      (field_is_input(n))  inputs.push_back(n);
      else if (field_is_output(n)) outputs.push_back(n);
      else if (field_is_param(n))  localparams.push_back(n);
      else if (field_is_module(n)) submodules.push_back(n);
      else                         fields.push_back(n);
    }

    if (n.sym == sym_function_definition) {
      auto func_def = n;

      auto func_type = func_def.get_field(field_type);
      auto func_decl = func_def.get_field(field_declarator);

      bool is_task = false;
      bool is_init = false;
      bool is_tock = false;
      bool is_tick = false;

      //----------

      is_task = match(func_type, "void");

      //----------

      auto current_function_name = func_decl.get_field(field_declarator);
      is_init = is_task && match(current_function_name, "init");
      is_tick = is_task && match(current_function_name, "tick");
      is_tock = is_task && match(current_function_name, "tock");

      if (is_init) {
        node_init = func_def;
      }
      else if (is_tick) {
        node_tick = func_def;
      }
      else if (is_tock) {
        node_tock = func_def;
      }
      else {
        if (is_task) {
          tasks.push_back(func_def);
        }
        else {
          functions.push_back(func_def);
        }
      }
    }
  });
}

//------------------------------------------------------------------------------
