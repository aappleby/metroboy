#include "MtSourceFile.h"

#include "MtModule.h"

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

MtSourceFile::MtSourceFile() {
}

MtSourceFile::~MtSourceFile() {
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  src_blob.clear();
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

void MtSourceFile::parse_source(const char* _full_path, blob& _src_blob) {
  this->full_path = _full_path;
  this->src_blob = _src_blob;

  if (src_blob[0] == 239 && src_blob[1] == 187 && src_blob[2] == 191) {
    use_utf8_bom = true;
    src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
  }
  src_blob.push_back(0);
 

  source = (const char*)src_blob.data();
  source_end = source + src_blob.size();

  // Parse the module and find the root class/struct/template.

  parser = ts_parser_new();
  lang = tree_sitter_cpp();
  ts_parser_set_language(parser, lang);
  tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size() - 1);
}

