#pragma once
#include "Platform.h"

#include "tree_sitter/api.h"

#include <vector>
#include <string>

struct MtModule;
struct MtModLibrary;
typedef std::vector<uint8_t> blob;

//------------------------------------------------------------------------------

struct MtSourceFile {

  MtSourceFile();
  ~MtSourceFile();

  void parse_source(const char* _full_path, blob& _src_blob);

  MtModule* get_module(const std::string& name);

  MtModLibrary* lib;
  std::string full_path;

  blob src_blob;
  bool use_utf8_bom = false;

  const char* source = nullptr;
  const char* source_end = nullptr;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;

  std::vector<MtModule*> modules;
};

//------------------------------------------------------------------------------
