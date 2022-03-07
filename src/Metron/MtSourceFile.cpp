#include "MtSourceFile.h"

#include "MtModule.h"
#include "MtModLibrary.h"

#pragma warning(disable:4996)

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

MtSourceFile::MtSourceFile(
  const std::string& _full_path,
  const std::string& _src_blob) : full_path(_full_path), src_blob(_src_blob) {

  assert(src_blob.back() != 0);

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();

  // Parse the source file.

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());

  // Pull out all modules from the top level of the source.

  TSNode ts_root = ts_tree_root_node(tree);

  mt_root = MtTranslationUnit(MtNode(ts_root, ts_node_symbol(ts_root), 0, this));

  for (int i = 0; i < (int)ts_node_child_count(ts_root); i++) {
    auto child = ts_node_child(ts_root, i);
    if (ts_node_symbol(child) == sym_template_declaration) {
      MtNode mod_root(child, ts_node_symbol(child), 0, this);
      MtModule* mod = new MtModule(this, MtTemplateDecl(mod_root));
      modules.push_back(mod);
    }
    else if (ts_node_symbol(child) == sym_class_specifier) {
      MtNode mod_root(child, ts_node_symbol(child), 0, this);
      MtModule* mod = new MtModule(this, MtClassSpecifier(mod_root));
      modules.push_back(mod);
    }
  }
}

MtSourceFile::~MtSourceFile() {
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  lang = nullptr;
  parser = nullptr;
  tree = nullptr;
  source = nullptr;
}


MtModule* MtSourceFile::get_module(const std::string& name) {
  for (auto n : modules) {
    if (n->mod_name == name) return n;
  }
  return nullptr;
}
