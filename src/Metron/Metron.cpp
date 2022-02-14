#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

#include "../uart_test/ibex_pkg.h"
#include "../uart_test/ibex_alu.h"

#include "../../riscv-simple-sv/synth/config.h"
#include "../../riscv-simple-sv/core/common/constants.h"
#include "../../riscv-simple-sv/core/common/adder.h"
#include "../../riscv-simple-sv/core/common/alu.h"
#include "../../riscv-simple-sv/core/common/alu_control.h"
#include "../../riscv-simple-sv/core/common/control_transfer.h"
#include "../../riscv-simple-sv/core/common/data_memory_interface.h"
#include "../../riscv-simple-sv/core/common/immediate_generator.h"
#include "../../riscv-simple-sv/core/common/instruction_decoder.h"
#include "../../riscv-simple-sv/core/common/multiplexer.h"
#include "../../riscv-simple-sv/core/common/multiplexer2.h"
#include "../../riscv-simple-sv/core/common/multiplexer4.h"
#include "../../riscv-simple-sv/core/common/multiplexer8.h"
#include "../../riscv-simple-sv/core/common/regfile.h"
#include "../../riscv-simple-sv/core/common/register.h"
#include "../../riscv-simple-sv/core/common/example_text_memory.h"
#include "../../riscv-simple-sv/core/common/example_text_memory_bus.h"
#include "../../riscv-simple-sv/core/singlecycle/riscv_core.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_control.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_ctlpath.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_datapath.h"

multiplexer<> blah_multiplexer;

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }

  args = {
    "-I.",
    "-I../../riscv-simple-sv",
    "-Isrc/uart_test",

    "-Iriscv-simple-sv/core/common",
    "-Iriscv-simple-sv/core/multicycle",
    "-Iriscv-simple-sv/core/pipeine",
    "-Iriscv-simple-sv/core/singlecycle",

    "ibex_alu.h"
  };


  MtModLibrary lib;
  //lib.add_search_path(".");
  //lib.add_search_path("../../riscv-simple-sv");
  //lib.add_search_path("src/uart_test");

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

  //lib.load("ibex_multdiv_slow.h", "ibex_multdiv_slow.h.sv");
  //lib.load("ibex_compressed_decoder.h", "ibex_compressed_decoder.h.sv");
  //lib.load("ibex_pkg.h", "ibex_pkg.h.sv");
  //lib.load("ibex_alu.h");
  //lib.load("prim_arbiter_fixed.h", "prim_arbiter_fixed.h.sv");

#if 0
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
#endif

  return 0;
}

//------------------------------------------------------------------------------
