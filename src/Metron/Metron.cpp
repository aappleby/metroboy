#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }

  args = {
    "-I.",
    "-Iuart",
    "-Iuart_test",
  };

  /*
  args.push_back("config.h");
  args.push_back("adder.h");
  args.push_back("alu.h");
  args.push_back("alu_control.h");
  args.push_back("control_transfer.h");
  args.push_back("data_memory_interface.h");
  args.push_back("immediate_generator.h");
  args.push_back("instruction_decoder.h");
  args.push_back("multiplexer.h");
  */

  //args.push_back("uart_top.h");
  //args.push_back("uart_hello.h");
  //args.push_back("uart_tx.h");
  args.push_back("uart_rx.h");

  MtModLibrary lib;

  for (auto& arg : args) {
    if (arg[0] == '-'){
      switch(arg[1]) {
      case 'I':
        printf("Adding search path %s\n", &arg[2]);
        lib.add_search_path(&arg[2]);
        break;
      default:
        printf("Bad command line arg '%s'\n", arg.c_str());
        return -1;
      }
    }
    else {
      lib.load(arg);
    }
  }

  for (auto& module : lib.modules)
  {
    //auto& module = lib.modules.back();

    MtCursor cursor(module, module->out_file);

    if (!module->mod_class.is_null()) {      
      printf("//--------------------------------------------------------------------------------\n");
      printf("// MODULE:       ");
      printf("%s\n", module->mod_class.node_to_name().c_str());

      printf("// MODULEPARAMS: ");
      for (auto f : module->modparams) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// INPUTS:       ");
      for (auto f : module->inputs) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// OUTPUTS:      ");
      for (auto f : module->outputs) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// LOCALPARAMS:  ");
      for (auto f : module->localparams) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// FIELDS:       ");
      for (auto f : module->fields) printf("%s, ", f.name.node_to_name().c_str());
      printf("\n");

      printf("// ENUMS:       ");
      for (auto f : module->enums) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// SUBMODULES:   ");
      for (auto f : module->submodules) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// TASKS:        ");
      for (auto f : module->tasks) printf("%s, ", f.node_to_name().c_str());
      printf("\n");

      printf("// FUNCTIONS:    ");
      for (auto f : module->functions) printf("%s, ", f.node_to_name().c_str());
      printf("\n");
      printf("//--------------------------------------------------------------------------------\n");
    }
    printf("\n");

    //module->mod_root.dump_tree();

    cursor.cursor = module->source;
    cursor.emit_dispatch(module->mod_root);
    printf("\n");
  }

  return 0;
}

//------------------------------------------------------------------------------
