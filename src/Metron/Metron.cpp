#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  MtModLibrary lib;

  /*
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] == '-') {
    } else {
      lib.load(arg, arg + ".sv");
    }
  }
  */

  //uart_top.h uart_hello.h uart_tx.h uart_rx.h ibex_compressed_decoder.h prim_arbiter_fixed.h

  //lib.load("src/uart_test/, arg + ".sv");
  //lib.load("uart_rx.h", "uart_rx.h.sv");

  std::string file = "ibex_multdiv_slow.h";
  //std::string file = "ibex_pkg.h";
  lib.load(file, file + ".sv");

  for (auto& module : lib.modules)
  {
    //auto& module = lib.modules[5];

    MtCursor cursor(module, module->out_file);

    if (!module->input_filename.starts_with("src/uart_test/ibex")) {
      cursor.emit("//--------------------------------------------------------------------------------\n");
      cursor.emit("// MODULE:       ", module);

      cursor.emit("%s\n", module->mod_class.node_to_name().c_str());

      cursor.emit("// MODULEPARAMS: ");
      for (auto f : module->modparams) {
        cursor.emit("%s, ", f.node_to_name().c_str());
      }
      cursor.emit("\n");

      cursor.emit("// INPUTS:       ");
      for (auto f : module->inputs) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// OUTPUTS:      ");
      for (auto f : module->outputs) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// LOCALPARAMS:  ");
      for (auto f : module->localparams) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// FIELDS:       ");
      for (auto f : module->fields) {
        cursor.emit("%s, ", f.name.node_to_name().c_str());
      }
      cursor.emit("\n");

      cursor.emit("// ENUMS:       ");
      for (auto f : module->enums) {
        cursor.emit("%s, ", f.node_to_name().c_str());
      }
      cursor.emit("\n");

      cursor.emit("// SUBMODULES:   ");
      for (auto f : module->submodules) {
        cursor.emit("%s, ", f.node_to_name().c_str());
      }
      cursor.emit("\n");

      cursor.emit("// TASKS:        ");
      for (auto f : module->tasks) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// FUNCTIONS:    ");
      for (auto f : module->functions) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");
    }

    //module->mod_root.dump_tree();

    cursor.emit("\n");
    cursor.cursor = module->source;
    cursor.emit_dispatch(module->mod_root);
    printf("\n");
  }

  return 0;
}

//------------------------------------------------------------------------------
