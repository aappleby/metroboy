// RISC-V SiMPLE SV -- common configuration for testbench
// BSD 3-Clause License
// (c) 2017-2021, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_CONFIG_H
`define RVSIMPLE_CONFIG_H


`include "metron_tools.sv"

package rv_config;

// Select ISA extensions
// `define M_MODULE    // multiplication and division

//////////////////////////////////////////
//              Memory config           //
//////////////////////////////////////////

// Program counter initial value
parameter int unsigned INITIAL_PC = 32'h00400000;

// Instruction memory
parameter int unsigned TEXT_BEGIN = INITIAL_PC;
parameter int unsigned TEXT_BITS = 16;
parameter int unsigned TEXT_WIDTH = (1 << TEXT_BITS);
parameter int unsigned TEXT_END = (TEXT_BEGIN + TEXT_WIDTH - 1);

// Data memory
parameter int unsigned DATA_BEGIN = 32'h80000000;
parameter int unsigned DATA_BITS = 17;
parameter int unsigned DATA_WIDTH = (1 << DATA_BITS);
parameter int unsigned DATA_END = (DATA_BEGIN + DATA_WIDTH - 1);

localparam string TEXT_HEX = "add.text.vh";
localparam string DATA_HEX = "add.data.vh";

endpackage;  // namespace rv_config

/*verilator lint_off width*/

`endif  // RVSIMPLE_CONFIG_H
