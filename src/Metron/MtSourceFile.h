#pragma once
#include <string>
#include <vector>

#include "MtNode.h"
#include "Platform.h"
#include "tree_sitter/api.h"

struct MtTranslationUnit;
struct MtModule;
struct MtModLibrary;
typedef std::vector<uint8_t> blob;

//------------------------------------------------------------------------------

struct MtSourceFile {
  MtSourceFile(const std::string& _full_path, const std::string& _src_blob);
  ~MtSourceFile();

  MtModule* get_module(const std::string& name);

  MtModLibrary* lib = nullptr;

  const std::string full_path;
  const std::string src_blob;
  bool use_utf8_bom = false;

  MtTranslationUnit mt_root;

  const char* source = nullptr;
  const char* source_end = nullptr;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;

  std::vector<MtModule*> modules;
};

//------------------------------------------------------------------------------
