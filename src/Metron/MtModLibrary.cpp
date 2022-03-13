#include "MtModLibrary.h"

#include <sys/stat.h>

#include "../CoreLib/Log.h"
#include "MtModule.h"
#include "MtSourceFile.h"

#pragma warning(disable : 4996)

//------------------------------------------------------------------------------

void MtModLibrary::reset() {
  for (auto module : modules) delete module;
  modules.clear();
}

//------------------------------------------------------------------------------

void MtModLibrary::add_search_path(const std::string& path) {
  assert(!sources_loaded);
  search_paths.push_back(path);
}

//------------------------------------------------------------------------------

void MtModLibrary::add_source(MtSourceFile* source_file) {
  assert(!sources_loaded);
  source_file->lib = this;
  source_files.push_back(source_file);
}

//------------------------------------------------------------------------------

MtSourceFile* MtModLibrary::find_source(const std::string& filename) {
  for (auto& s : source_files) {
    if (s->filename == filename) {
      return s;
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------

bool MtModLibrary::load_source(const char* filename) {
  assert(!sources_loaded);
  bool found = false;
  for (auto& path : search_paths) {
    auto full_path = path.size() ? path + "/" + filename : filename;
    struct stat s;
    auto stat_result = stat(full_path.c_str(), &s);
    if (stat_result == 0) {
      found = true;
      LOG_B("Loading %s from %s\n", filename, full_path.c_str());
      LOG_INDENT_SCOPE();

      std::string src_blob;
      src_blob.resize(s.st_size);

      auto f = fopen(full_path.c_str(), "rb");
      fread(src_blob.data(), 1, src_blob.size(), f);
      fclose(f);

      bool use_utf8_bom = false;
      if (uint8_t(src_blob[0]) == 239 && uint8_t(src_blob[1]) == 187 &&
          uint8_t(src_blob[2]) == 191) {
        use_utf8_bom = true;
        src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
      }
      load_blob(filename, full_path, src_blob, use_utf8_bom);
      break;
    }
  }
  if (!found) {
    LOG_R("Couldn't find %s in path!\n", filename);
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------

void MtModLibrary::load_blob(const std::string& filename,
                             const std::string& full_path,
                             const std::string& src_blob, bool use_utf8_bom) {
  assert(!sources_loaded);
  auto source_file = new MtSourceFile(filename, full_path, src_blob);
  source_file->use_utf8_bom = use_utf8_bom;
  add_source(source_file);
}

//------------------------------------------------------------------------------

void MtModLibrary::process_sources() {
  assert(!sources_loaded);
  assert(!sources_processed);

  sources_loaded = true;
  
  for (auto s : source_files) {
    for (auto m : *s->modules) {
      modules.push_back(m);
    }
  }

  for (auto& mod : modules) mod->load_pass1();
  for (auto& mod : modules) mod->load_pass2();

  // Verify that tick()/tock() obey read/write ordering rules.
  bool any_fail_dirty_check = false;

  for (auto& mod : modules) {
    mod->check_dirty_ticks();
    mod->check_dirty_tocks();
    mod->dirty_check_done = true;
    any_fail_dirty_check |= mod->dirty_check_fail;
  }

  if (any_fail_dirty_check) {
    printf("Dirty check fail!\n");
  }
  sources_processed = true;
}

//------------------------------------------------------------------------------

MtModule* MtModLibrary::get_mod(const std::string& name) {
  assert(sources_loaded);
  for (auto mod : modules) {
    if (mod->mod_name == name) return mod;
  }
  return nullptr;
}

bool MtModLibrary::has_mod(const std::string& name) {
  assert(sources_loaded);
  return get_mod(name) != nullptr;
}

//------------------------------------------------------------------------------
