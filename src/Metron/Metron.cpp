#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

#include <sys/stat.h>
#include <sys/types.h>

#ifdef __GNUC__
#include <unistd.h>
#define _getcwd getcwd
#define _mkdir mkdir
#else
#include <direct.h>
int _mkdir(const char* path, int mode) { return _mkdir(path); }
#endif

#pragma warning(disable:4996) // unsafe fopen

//------------------------------------------------------------------------------

std::vector<std::string> split_path(const std::string& input) {
  std::vector<std::string> result;
  std::string temp;

  const char* c = input.c_str();

  do {
    if (*c == '/' || *c == '\\' || *c == 0) {
      if (temp.size()) result.push_back(temp);
      temp.clear();
    }
    else {
      temp.push_back(*c);
    }
  }
  while(*c++ != 0);

  return result;
}

//------------------------------------------------------------------------------

std::string join(std::vector<std::string>& path) {
  std::string result = path[0];
  for (int i = 1; i < path.size(); i++) {
    result += "/";
    result += path[i];
  }
  return result;
}

//------------------------------------------------------------------------------

void mkdir_all(const std::vector<std::string>& full_path) {
  std::string temp;
  for (size_t i = 0; i < full_path.size() - 1; i++) {
    if (temp.size()) temp += "/";
    temp += full_path[i];
    auto d = _mkdir(temp.c_str(), 0x777);
  }
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  /*
  {
    char * cwd;
    cwd = (char*) malloc( FILENAME_MAX * sizeof(char) );
    _getcwd(cwd,FILENAME_MAX);
    printf("cwd %s\n", cwd);
    free(cwd);
  }
  */

  for (int i = 1; i < argc; i++) {
    printf("argv[%02d] = %s\n", i, argv[i]);
  }

  // Parse args

  std::vector<std::string> mod_names;
  std::vector<std::string> mod_paths = {""};
  std::string out_dir = "";
  bool quiet = false;

  for (int i = 1; i < argc; i++) {
    char* arg_cursor = argv[i];
    if (*arg_cursor == '-'){
      arg_cursor++;
      auto option = *arg_cursor++;
      while(*arg_cursor && (*arg_cursor == ' ' || *arg_cursor == '=')) arg_cursor++;

      switch(option) {
      case 'I':
        printf("Adding search path \"%s\"\n", arg_cursor);
        mod_paths.push_back(arg_cursor);
        break;
      case 'O':
        printf("Adding output directory \"%s\"\n", arg_cursor);
        out_dir = arg_cursor;
        break;
      case 'q':
        quiet = true;
        break;
      default:
        printf("Bad command line arg \"%s\"\n", argv[i]);
        return -1;
      }
    }
    else {
      mod_names.push_back(arg_cursor);
    }
  }

  // Load all modules.

  MtModLibrary library;
  for (auto& name : mod_names) {
    for (auto& path : mod_paths) {
      auto full_path = path.size() ? path + "/" + name : name;
      struct stat s;
      auto stat_result = stat(full_path.c_str(), &s);
      if (stat_result == 0) {
        printf("loading %s from %s\n", name.c_str(), full_path.c_str());
        library.load(full_path);
        break;
      }
    }
  }
  printf("\n");

  // Cross-reference all modules

  {
    for (auto& mod : library.modules) {
      mod->collect_modparams();
      mod->collect_localparams();
      mod->collect_functions();
      mod->collect_ports();
      mod->collect_fields();
      mod->dedup_inputs();

      // Verify that tick()/tock() obey read/write ordering rules.

      mod->check_dirty_ticks();
      mod->check_dirty_tocks();
    }
  }

  // Emit all modules.

  for (auto& module : library.modules)
  {
    //auto& module = library.modules.back();
    //if (module->mod_class.is_null()) continue;

    auto out_path = out_dir + "/" + module->full_path + ".sv";
    mkdir_all(split_path(out_path));

    FILE* out_file = fopen(out_path.c_str(), "wb");
    if (!out_file) {
      printf("ERROR Could not open %s for output\n", out_path.c_str());
      //continue;
    }

    printf("Opened %s for output\n", out_path.c_str());

    MtCursor cursor(module);
    cursor.out.push_back(out_file);
    if (!quiet) cursor.out.push_back(stdout);

    // Copy the BOM over if needed.
    if (module->use_utf8_bom) {
      uint8_t bom[3] = { 239, 187, 191 };
      fwrite(bom, 1, 3, out_file);
    }

    if (!quiet) {
      /*
      std::vector<MtNode> modparams;
      std::vector<MtNode> localparams;
      std::vector<MtNode> enums;

      std::vector<MtNode> inits;
      std::vector<MtNode> ticks;
      std::vector<MtNode> tocks;
      std::vector<MtNode> tasks;
      std::vector<MtNode> funcs;

      std::vector<MtField> inputs;
      std::vector<MtField> outputs;
      std::vector<MtField> fields;
      std::vector<MtNode>  submodules;
      */

      printf("\n");
      printf("//--------------------------------------------------------------------------------\n");
      printf("// MODULE:       ");
      printf("%s\n", module->mod_class.node_to_name().c_str());

      printf("// MODULEPARAMS: ");
      for (auto f : module->modparams) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// LOCALPARAMS:  ");
      for (auto f : module->localparams) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// ENUMS:        ");
      for (auto f : module->enums) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// INITS:        ");
      for (auto f : module->inits) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// TICKS:        ");
      for (auto f : module->ticks) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// TOCKS:        ");
      for (auto f : module->tocks) printf("%s, ", f.node_to_name().c_str());
      printf("\n");
      printf("// TASKS:        ");
      for (auto f : module->tasks) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// FUNCS:        ");
      for (auto f : module->funcs) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// INPUTS:       ");
      for (auto f : module->inputs) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// OUTPUTS:      ");
      for (auto f : module->outputs) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// FIELDS:       ");
      for (auto f : module->fields) printf("%s, ", f.name.node_to_name().c_str());
      printf("\n");

      printf("// SUBMODULES:   ");
      for (auto f : module->submodules) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("//--------------------------------------------------------------------------------\n");

      printf("\n");
    }

    cursor.cursor = module->source;
    cursor.emit_dispatch(module->mod_root);

    fclose(out_file);
  }

  return 0;
}

//------------------------------------------------------------------------------
