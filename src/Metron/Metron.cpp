#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"
#include "MtSourceFile.h"

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
MtModule* load_pass1(const char* _full_path, blob& _src_blob);

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

  //args.push_back("uart/uart_top.h");
  //args.push_back("uart/uart_hello.h");
  //args.push_back("uart/uart_tx.h");
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
  args.push_back("singlecycle_datapath.h");
  */
  /*
  args.push_back("riscv_core.h");
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
        LOG_INDENT_SCOPE();

        std::string src_blob;
        src_blob.resize(s.st_size);

        auto f = fopen(full_path.c_str(), "rb");
        fread(src_blob.data(), 1, src_blob.size(), f);
        fclose(f);

        bool use_utf8_bom = false;
        if (src_blob[0] == 239 && src_blob[1] == 187 && src_blob[2] == 191) {
          use_utf8_bom = true;
          src_blob.erase(src_blob.begin(), src_blob.begin() + 3);
        }
        src_blob.push_back(0);
 
        auto source_file = new MtSourceFile(&library, full_path, src_blob);
        source_file->use_utf8_bom = use_utf8_bom;

        library.source_files.push_back(source_file);

        break;
      }
    }
    if (!found) {
      LOG_R("Couldn't find %s in path!\n", name.c_str());
    }
  }

#if 0
  {
    std::string source = R"(
//----------------------------------------

struct derp {
  logic<1> o_foo;

  void tock() {
    o_foo = 1;
  }
  
};

//----------------------------------------

struct gerp {
  derp m;
  logic<1> o_bar;

  void tock(logic<1> i_foo) {
    m.tock();
    o_bar = m.o_foo;
  }
};

//----------------------------------------

)";


    auto source_file = new MtSourceFile(&library, "<inline>", source);

    library.source_files.push_back(source_file);
  }
#endif

  //----------

  for(auto& source : library.source_files) {
    for (auto& mod : source->modules) {
      library.modules.push_back(mod);
    }
  }

  {
    LOG_G("Library\n");
    LOG_INDENT_SCOPE();
    for(auto& source_file : library.source_files) {
      LOG_G("Source file %s\n", source_file->full_path.c_str());
      LOG_INDENT_SCOPE();
      for (auto& mod : source_file->modules) {
        LOG_G("Module %s\n", mod->mod_name.c_str());
      }
    }
    for (auto& mod : library.modules) {
      LOG_G("Module %s\n", mod->mod_name.c_str());
    }
  }


  for (auto& mod : library.modules) mod->load_pass1();
  for (auto& mod : library.modules) mod->load_pass2();
  for (auto& mod : library.modules) mod->load_pass3();

  for (auto& mod : library.modules) {
    mod->load_pass3();
  }

  // Dump out info on modules for debugging.

  for (auto& mod : library.modules) {
    mod->dump_banner();
  }

  // Verify that tick()/tock() obey read/write ordering rules.

  bool any_fail_dirty_check = false;

  {
    LOG_G("Checking tick/tock rules\n")
    LOG_INDENT_SCOPE();
    for (auto& mod : library.modules)
    {
      //auto& mod = library.modules[0];
      LOG_G("Checking %s\n", mod->mod_name.c_str());
      LOG_INDENT_SCOPE();
      mod->check_dirty_ticks();
      mod->check_dirty_tocks();
      mod->dirty_check_done = true;
      any_fail_dirty_check |= mod->dirty_check_fail;
    }
  }

  if (any_fail_dirty_check) {
    printf("Dirty check fail!\n");
    return -1;
  }

  for (auto& mod : library.modules)
  {
    if (mod->mod_struct.is_null()) continue;

    LOG_G("%s\n", mod->mod_name.c_str());
    LOG_INDENT_SCOPE();

    for (auto& tick : mod->tick_methods) {
      LOG_G("%s\n", tick.name.c_str());
      LOG_INDENT_SCOPE();
      for (auto always : tick.always_dirty) LOG_G("%s always dirty\n", always.c_str());
      for (auto maybe : tick.maybe_dirty)   LOG_G("%s maybe dirty\n", maybe.c_str());
    }

    for (auto& tock : mod->tock_methods) {
      LOG_G("%s\n", tock.name.c_str());
      LOG_INDENT_SCOPE();
      for (auto always : tock.always_dirty) LOG_G("%s always dirty\n", always.c_str());
      for (auto maybe : tock.maybe_dirty)   LOG_G("%s maybe dirty\n", maybe.c_str());
    }
  }


#if 1
  // Emit all modules.

  for (auto& source_file : library.source_files) {

    auto out_path = out_dir + "/" + source_file->full_path + ".sv";
    mkdir_all(split_path(out_path));

    std::string out_string;

    MtCursor cursor(&library, source_file, &out_string);

    //if (!quiet) module->dump_banner();
    LOG_G("Emitting SystemVerilog\n");

    cursor.cursor = source_file->source;
    cursor.source_file = source_file;
    cursor.emit(source_file->mt_root);
    cursor.emit("\n");

    /*
    LOG_G("Saving SystemVerilog\n");
    FILE* out_file = fopen(out_path.c_str(), "wb");
    if (!out_file) {
      LOG_R("ERROR Could not open %s for output\n", out_path.c_str());
      continue;
    }
    LOG_G("Opened %s for output\n", out_path.c_str());

    // Copy the BOM over if needed.
    if (source_file->use_utf8_bom) {
      uint8_t bom[3] = { 239, 187, 191 };
      fwrite(bom, 1, 3, out_file);
    }

    fwrite(out_string.data(), 1, out_string.size(), out_file);
    fclose(out_file);
    */

    LOG_G("Done\n");
  }
#endif

  return 0;
}

//------------------------------------------------------------------------------
