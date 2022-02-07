#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  MtModLibrary lib;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] == '-') {
    } else {
      lib.load(arg, arg + ".sv");
    }
  }

  for (auto& module : lib.modules)
  {
    //auto& module = lib.modules[0];

    MtCursor cursor(module, module->out_file);

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

    cursor.cursor = module->source;
    cursor.emit_dispatch(module->mod_root);
    printf("\n");
  }

  return 0;
}

//------------------------------------------------------------------------------
