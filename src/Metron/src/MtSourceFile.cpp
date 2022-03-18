#include "MtSourceFile.h"

#include "MtModLibrary.h"
#include "MtModule.h"

#pragma warning(disable : 4996)

extern "C" {
extern const TSLanguage* tree_sitter_cpp();
}

//------------------------------------------------------------------------------

MtSourceFile::MtSourceFile(const std::string& _filename,
                           const std::string& _full_path,
                           const std::string& _src_blob)
    : filename(_filename), full_path(_full_path), src_blob(_src_blob) {
  assert(src_blob.back() != 0);

  auto blob_size = src_blob.size();
  source = (const char*)src_blob.data();
  source_end = source + blob_size;

  // Parse the source file.

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)blob_size);

  // Pull out all modules from the top level of the source.

  TSNode ts_root = ts_tree_root_node(tree);
  auto root_sym = ts_node_symbol(ts_root);
  mt_root = MtTranslationUnit(MtNode(ts_root, root_sym, 0, this));

  modules = new std::vector<MtModule*>();
  find_modules(mt_root);
}

//------------------------------------------------------------------------------

MtSourceFile::~MtSourceFile() {
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  for (auto m : *modules) delete m;
  delete modules;
  modules = nullptr;

  lang = nullptr;
  parser = nullptr;
  tree = nullptr;
  source = nullptr;
}

//------------------------------------------------------------------------------

void MtSourceFile::find_modules(MtNode toplevel) {
  for (auto c : toplevel) {
    switch (c.sym) {
      case sym_template_declaration: {
        MtNode mod_root(c.node, c.sym, 0, this);
        MtModule* mod = new MtModule(this, MtTemplateDecl(mod_root));
        modules->push_back(mod);
        break;
      }
      case sym_class_specifier: {
        MtNode mod_root(c.node, c.sym, 0, this);
        MtModule* mod = new MtModule(this, MtClassSpecifier(mod_root));
        modules->push_back(mod);
        break;
      }
      case sym_preproc_ifdef: {
        find_modules(c);
        break;
      }
    }
  }
}

//------------------------------------------------------------------------------

MtModule* MtSourceFile::get_module(const std::string& name) {
  for (auto n : *modules) {
    if (n->mod_name == name) return n;
  }
  return nullptr;
}

//------------------------------------------------------------------------------
