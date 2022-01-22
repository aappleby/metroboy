#include <stdio.h>

#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  std::vector<std::string> inputs = {
    "src/uart_test/uart_top.h",
    "src/uart_test/uart_hello.h",
    "src/uart_test/uart_tx.h",
    "src/uart_test/uart_rx.h",
  };

  ModLibrary lib;
  for (auto& input : inputs) {
    lib.load(input.c_str(), (input + ".sv").c_str());
  }

  for (auto& module : lib.modules) {

    ModCursor cursor = {
      &lib,
      module,
      module->source,
      fopen(module->output_filename.c_str(), "wb"),
    };

    cursor.emit("//--------------------------------------------------------------------------------\n");
    cursor.emit("// MODULE:       ");
    cursor.emit("%s\n", module->class_to_name(module->module_class).c_str());

    cursor.emit("// MODULEPARAMS: ");
    for (auto f : module->moduleparams) {
      cursor.emit("%s, ", module->field_to_name(f).c_str());
    }
    cursor.emit("\n");

    cursor.emit("// INPUTS:       ");
    for (auto f : module->inputs) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// OUTPUTS:      ");
    for (auto f : module->outputs) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// LOCALPARAMS:  ");
    for (auto f : module->localparams) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("// FIELDS:       ");
    for (auto f : module->fields) {
      cursor.emit("%s, ", module->field_to_name(f).c_str());
    }
    cursor.emit("\n");

    cursor.emit("// SUBMODULES:   ");
    for (auto f : module->submodules) cursor.emit("%s, ", module->field_to_name(f).c_str());
    cursor.emit("\n");

    cursor.emit("\n");
    cursor.cursor = module->source;
    cursor.emit_dispatch(module->root);
    printf("\n\n\n");

    //e.mod->dump_tree(e.mod->root);
    //printf("\n\n");
  }

  return 0;
}

//------------------------------------------------------------------------------
