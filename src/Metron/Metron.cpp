#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"

#include "../uart_test/Logic.h"

#if 1
#include "../uart_test/uart_rx.h"
#include "../uart_test/uart_tx.h"
#include "../uart_test/uart_hello.h"
#include "../uart_test/uart_top.h"

#include "../uart_test/ibex_pkg.h"
#include "../uart_test/ibex_alu.h"
#include "../uart_test/ibex_multdiv_slow.h"

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
#include "../../riscv-simple-sv/core/common/example_data_memory.h"
#include "../../riscv-simple-sv/core/common/example_data_memory_bus.h"
#include "../../riscv-simple-sv/core/singlecycle/riscv_core.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_control.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_ctlpath.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_datapath.h"
#include "../../riscv-simple-sv/core/singlecycle/toplevel.h"
#endif

//#define TEXT_BITS 12

void test_ibex_alu() {
  using namespace ibex_pkg;

  ibex_alu<rv32b_e::RV32BNone> alu;

  logic<1>  instr_first_cycle_i = 0;
  alu_op_e  operator_i = alu_op_e::ALU_XPERM_N;
  logic<32> operand_a_i = 0;
  logic<32> operand_b_i = 0;

  alu.tock1(0);
  alu.tock2(ibex_pkg::alu_op_e::ALU_XPERM_N);
  alu.tock3(0, 0, 0, 0, 0);
  alu.tock4(ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
  alu.tock5(instr_first_cycle_i, operator_i, operand_a_i, operand_b_i);
  alu.tock6(0, ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
  alu.tock8(ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
}

void test_rvs() {
  toplevel t;
  t.tick(1);
  t.tock();
  t.tick(0);
  t.tock();
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  {
    logic<33> op_b_shift_q;
    op_b_shift_q = b33(0x0);
  }

  {
    logic<7> a = 0;
    b32(a) = 0xDEADBEEF;
    printf("a = 0x%08x\n", a.get());
    printf("sizeof(a) = %d\n", (int)sizeof(a));
  }


  {
    logic<16> a = b16(0x12345678, 8);
    logic<16> b;
    b8(b, 8) = b8(a);
    printf("0x%08x\n", b.get());
  }

  {
    //logic<8> a = 1;
    //logic<9> b = a;
  }

  {
    const uint32_t temp = 0x0F0F0F0F;
    for (int i = 0; i < 32; i++) {
      logic<8> a = b8(temp, i);
      //logic<8> a = make_slice<8, uint32_t, 32>(temp, i);
      assert(a.get() == ((temp >> i) & 0xFF));
    }
  }

  {
    uint32_t temp = 0x00000000;
    for (int i = 0; i < 32; i++) {
      b1(temp, i) = 1;
      assert(temp == 0xFFFFFFFF >> (32 - i - 1));
    }
  }

  {
    const logic<32> temp = b32(0x0F0F0F0F);
    for (int i = 0; i < 32; i++) {
      logic<8> a = b8(temp, i);
      assert(a == ((temp >> i) & 0xFF));
    }
  }

  {
    logic<32> temp = b32(0x00000000);
    for (int i = 0; i < 32; i++) {
      b1(temp, i) = 1;

      //printf("0x%08x 0x%08x\n", temp.get(), 0xFFFFFFFF >> (32 - i - 1));

      assert(temp == 0xFFFFFFFF >> (32 - i - 1));
    }
  }

  {
    logic<3> a = 0b100;
    int8_t b = a.as_signed();
    assert(a == 4);
    assert(b == -4);
  }

  {
    logic<32> a = b32(0x80000000);
    logic<5>  b = b5(15);

    logic<32> c = b32(unsigned(a) >> b);
    logic<32> d = b32(  signed(a) >> b);

    assert(c == 0x00010000);
    assert(d == 0xFFFF0000);
  }


#if 0
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }

  args = {
    "-I.",
    "-Isrc/uart_test",
    "-Iriscv-simple-sv",
    "-Iriscv-simple-sv/synth",
    "-Iriscv-simple-sv/core/common",
    "-Iriscv-simple-sv/core/singlecycle",
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

  args.push_back("uart_top.h");
  args.push_back("uart_hello.h");
  args.push_back("uart_tx.h");
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

#if 1
  for (auto& module : lib.modules)
  {
    //auto& module = lib.modules.back();

    MtCursor cursor(module, module->out_file);

    if (!module->mod_class.is_null()) {      
      cursor.emit("//--------------------------------------------------------------------------------\n");
      cursor.emit("// MODULE:       ", module);
      cursor.emit("%s\n", module->mod_class.node_to_name().c_str());

      cursor.emit("// MODULEPARAMS: ");
      for (auto f : module->modparams) cursor.emit("%s, ", f.node_to_name().c_str());
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
      for (auto f : module->fields) cursor.emit("%s, ", f.name.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// ENUMS:       ");
      for (auto f : module->enums) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// SUBMODULES:   ");
      for (auto f : module->submodules) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// TASKS:        ");
      for (auto f : module->tasks) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");

      cursor.emit("// FUNCTIONS:    ");
      for (auto f : module->functions) cursor.emit("%s, ", f.node_to_name().c_str());
      cursor.emit("\n");
      cursor.emit("//--------------------------------------------------------------------------------\n");
    }

    //module->mod_root.dump_tree();

    cursor.emit("\n");
    cursor.cursor = module->source;
    cursor.emit_dispatch(module->mod_root);
    printf("\n");
  }
#endif
#endif

  return 0;
}

//------------------------------------------------------------------------------
