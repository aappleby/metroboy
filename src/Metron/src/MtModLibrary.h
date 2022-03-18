#pragma once
#include <string>
#include <vector>

#include "Platform.h"

struct MtModule;
struct MtSourceFile;

//------------------------------------------------------------------------------

struct MtModLibrary {
  void reset();
  void add_search_path(const std::string& path);
  void add_source(MtSourceFile* source_file);
  MtSourceFile* find_source(const std::string& filename);

  bool load_source(const char* name);
  void load_blob(const std::string& filename, const std::string& full_path,
                 const std::string& src_blob, bool use_utf8_bom = false);

  void process_sources();

  MtModule* get_mod(const std::string& module_name);
  bool has_mod(const std::string& name);

  //----------

  std::vector<std::string> search_paths;
  std::vector<MtSourceFile*> source_files;
  std::vector<MtModule*> modules;

  bool sources_loaded = false;
  bool sources_processed = false;
};

//------------------------------------------------------------------------------
