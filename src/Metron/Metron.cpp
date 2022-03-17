#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../CoreLib/Log.h"
#include "MtCursor.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtSourceFile.h"
#include "Platform.h"
#include "Logic.h"

#include "metron_tools.h"

#pragma warning(disable : 4996)

//------------------------------------------------------------------------------

std::vector<std::string> split_path(const std::string& input) {
  std::vector<std::string> result;
  std::string temp;

  const char* c = input.c_str();

  do {
    if (*c == '/' || *c == '\\' || *c == 0) {
      if (temp.size()) result.push_back(temp);
      temp.clear();
    } else {
      temp.push_back(*c);
    }
  } while (*c++ != 0);

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
    auto d = plat_mkdir(temp.c_str(), 0x777);
  }
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  LOG_B("Metron v0.0.1\n");

  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }

#if 0
  args.clear();
  args.push_back("-Iuart");
  args.push_back("-Iuart_test");
  args.push_back("-Irvsimple");
  args.push_back("-Ogenerated");

  /*
  args.push_back("uart_top.h");
  args.push_back("uart_hello.h");
  args.push_back("uart_tx.h");
  args.push_back("uart_rx.h");
  */

  args.push_back("adder.h");
  args.push_back("config.h");
  args.push_back("constants.h");
  args.push_back("alu.h");
  args.push_back("alu_control.h");
  args.push_back("control_transfer.h");
  args.push_back("data_memory_interface.h");
  args.push_back("immediate_generator.h");
  args.push_back("instruction_decoder.h");
  args.push_back("multiplexer.h");
  args.push_back("multiplexer2.h");
  args.push_back("multiplexer4.h");
  args.push_back("multiplexer8.h");
  args.push_back("regfile.h");
  args.push_back("register.h");
  args.push_back("example_memory_bus.h");
  args.push_back("example_text_memory.h");
  args.push_back("example_text_memory_bus.h");
  args.push_back("example_data_memory.h");
  args.push_back("example_data_memory_bus.h");
  args.push_back("singlecycle_control.h");
  args.push_back("singlecycle_ctlpath.h");
  args.push_back("singlecycle_datapath.h");
  args.push_back("riscv_core.h");
  args.push_back("toplevel.h");
#endif

  for (auto& arg : args) {
    LOG_R("arg = %s\n", arg.c_str());
  }
  LOG_R("\n");

  //----------
  // Parse args

  //std::vector<std::string> path_names;
  std::vector<std::string> source_names;
  std::string out_dir = ".";
  std::string root_dir = ".";
  bool quiet = false;

  for (auto& arg : args) {
    const char* arg_cursor = arg.c_str();
    if (*arg_cursor == '-') {
      arg_cursor++;
      auto option = *arg_cursor++;
      while (*arg_cursor && (*arg_cursor == ' ' || *arg_cursor == '='))
        arg_cursor++;

      switch (option) {
        case 'R':
          LOG_G("Adding root directory \"%s\"\n", arg_cursor);
          root_dir = arg_cursor;
          break;
        //case 'I':
        //  LOG_G("Adding search path \"%s\"\n", arg_cursor);
        //  path_names.push_back(arg_cursor);
        //  break;
        case 'O':
          LOG_G("Adding output directory \"%s\"\n", arg_cursor);
          out_dir = arg_cursor;
          break;
        case 'q':
          quiet = true;
          break;
        default:
          LOG_G("Bad command line arg \"%s\"\n", arg.c_str());
          return -1;
      }
    } else {
      source_names.push_back(arg_cursor);
    }
  }
  LOG("\n");

  //----------
  // Load all source files.

  MtModLibrary library;
  library.add_search_path("");
  //for (auto& path : path_names) {
  //  library.add_search_path(path);
  //}
  library.add_search_path(root_dir);

  for (auto& name : source_names) {
    library.load_source(name.c_str());
  }
  library.process_sources();

  //----------
  // Dump out info on modules for debugging.

  for (auto& mod : library.modules) {
    if (mod->dirty_check_fail) {
      printf("Module %s failed dirty check\n", mod->mod_name.c_str());
      return -1;
    }
    //mod->dump_banner();
    //mod->dump_deltas();
  }

  //----------
  // Emit all modules.

  for (auto& source_file : library.source_files)
  {
    //auto source_file = library.find_source("example_text_memory.h");

    //source_file->modules->at(0)->mod_struct.dump_tree();

    auto out_name = source_file->filename;
    assert(out_name.ends_with(".h"));
    out_name.resize(out_name.size() - 2);
    auto out_path = out_dir + "/" + out_name + ".sv";
    mkdir_all(split_path(out_path));

    LOG_G("%s -> %s\n", source_file->full_path.c_str(), out_path.c_str());

    std::string out_string;

    MtCursor cursor(&library, source_file, &out_string);
    cursor.quiet = quiet;
    cursor.cursor = source_file->source;
    cursor.source_file = source_file;
    cursor.emit(source_file->mt_root);
    cursor.emit("\n");

    FILE* out_file = fopen(out_path.c_str(), "wb");
    if (!out_file) {
      LOG_R("ERROR Could not open %s for output\n", out_path.c_str());
    } else {
      // Copy the BOM over if needed.
      if (source_file->use_utf8_bom) {
        uint8_t bom[3] = {239, 187, 191};
        fwrite(bom, 1, 3, out_file);
      }

      fwrite(out_string.data(), 1, out_string.size(), out_file);
      fclose(out_file);
    }
  }

  return 0;
}

//------------------------------------------------------------------------------
