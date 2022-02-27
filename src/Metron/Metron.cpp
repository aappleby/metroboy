#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

#include "../CoreLib/Log.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <stdarg.h>

#pragma warning(disable:4996)

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
    auto d = mkdir(temp.c_str(), 0x777);
  }
}

//------------------------------------------------------------------------------

void metron_test_suite();

int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    //LOG_R("argv[%02d] = %s\n", i, argv[i]);
    args.push_back(argv[i]);
  }

  /*
  {
    std::string cwd;
    cwd.resize(FILENAME_MAX);
    getcwd(cwd.data(),FILENAME_MAX);
    LOG_R("cwd %s\n", cwd);
  }
  */

  args.clear();
  args.push_back("-Iuart");
  args.push_back("-Iuart_test");
  args.push_back("-Irvsimple");
  args.push_back("-Oout");

  args.push_back("uart/uart_top.h");
  args.push_back("uart/uart_hello.h");
  args.push_back("uart/uart_tx.h");
  args.push_back("uart/uart_rx.h");

  /*
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
  args.push_back("example_text_memory.h");
  args.push_back("example_text_memory_bus.h");
  args.push_back("example_data_memory.h");
  args.push_back("example_data_memory_bus.h");
  args.push_back("singlecycle_control.h");
  args.push_back("singlecycle_ctlpath.h");
  args.push_back("riscv_core.h");
  args.push_back("singlecycle_datapath.h");
  args.push_back("toplevel.h");
  */

  for (auto& arg : args) {
    LOG_R("arg = %s\n", arg.c_str());
  }

  // Parse args
  std::vector<std::string> mod_names;
  std::vector<std::string> mod_paths = {""};
  std::string out_dir = "";
  bool quiet = false;

  for (auto& arg : args) {
    const char* arg_cursor = arg.c_str();
    if (*arg_cursor == '-'){
      arg_cursor++;
      auto option = *arg_cursor++;
      while(*arg_cursor && (*arg_cursor == ' ' || *arg_cursor == '=')) arg_cursor++;

      switch(option) {
      case 'I':
        LOG_G("Adding search path \"%s\"\n", arg_cursor);
        mod_paths.push_back(arg_cursor);
        break;
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
    }
    else {
      mod_names.push_back(arg_cursor);
    }
  }

  // Run test suite?
  //metron_test_suite();

  // Load all modules.

  MtModLibrary library;
  for (auto& name : mod_names) {
    bool found = false;
    for (auto& path : mod_paths) {
      auto full_path = path.size() ? path + "/" + name : name;
      struct stat s;
      auto stat_result = stat(full_path.c_str(), &s);
      if (stat_result == 0) {
        found = true;
        LOG_B("loading %s from %s\n", name.c_str(), full_path.c_str());

        {
          LOG_INDENT_SCOPE();
          blob src_blob;
          src_blob.resize(s.st_size);
          auto f = fopen(full_path.c_str(), "rb");
          fread(src_blob.data(), 1, src_blob.size(), f);
          fclose(f);

          LOG_B("parsing %s\n", name.c_str());
          auto mod = new MtModule();
          mod->load_pass1(full_path.c_str(), src_blob);
          mod->lib = &library;

          LOG_B("parsing %s done\n", name.c_str());
          library.modules.push_back(mod);
        }

        break;
      }
    }
    if (!found) {
      LOG_R("Couldn't find %s in path!\n", name.c_str());
    }
  }

  for (auto& mod : library.modules) {
    mod->load_pass2();
  }

  for (auto& mod : library.modules) {
    mod->load_pass3();
  }

  // Verify that tick()/tock() obey read/write ordering rules.
  LOG_G("Checking tick/tock rules\n")
  for (auto& mod : library.modules) {
    mod->check_dirty_ticks();
    mod->check_dirty_tocks();
  }

  // Dump out info on modules for debugging.

  for (auto& mod : library.modules) {
    mod->dump_banner();
  }


#if 0
  // Emit all modules.

  for (auto& module : library.modules)
  {
    //auto& module = library.modules.back();

    module->mod_root.dump_tree(0, 0, 2);

    LOG_G("Processing module %s\n", module->mod_name.c_str());
    //auto& module = library.modules.back();
    //if (module->mod_class.is_null()) continue;

    auto out_path = out_dir + "/" + module->full_path + ".sv";
    mkdir_all(split_path(out_path));

    FILE* out_file = fopen(out_path.c_str(), "wb");
    if (!out_file) {
      LOG_R("ERROR Could not open %s for output\n", out_path.c_str());
      //continue;
    }

    LOG_G("Opened %s for output\n", out_path.c_str());

    std::string out_string;

    MtCursor cursor(module, &out_string);

    // Copy the BOM over if needed.
    if (module->use_utf8_bom) {
      uint8_t bom[3] = { 239, 187, 191 };
      fwrite(bom, 1, 3, out_file);
    }

    if (!quiet) module->dump_banner();

    LOG_G("Emitting SystemVerilog\n");

    cursor.cursor = module->source;
    cursor.emit(module->mod_root);
    cursor.emit("\n");

    //cursor.emit("foo\n");
    //cursor.emit("bar\n");
    //cursor.emit("baz\n");

    //printf(out_string.c_str());

    LOG_G("Saving SystemVerilog\n");

    fwrite(out_string.data(), 1, out_string.size(), out_file);
    fclose(out_file);

    LOG_G("Done\n");
  }
#endif

  return 0;
}

//------------------------------------------------------------------------------
