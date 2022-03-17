// RISC-V SiMPLE SV -- common configuration for testbench
// BSD 3-Clause License
// (c) 2017-2021, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_CONFIG_H
#define RVSIMPLE_CONFIG_H

#pragma once
#include "metron_tools.h"

namespace rv_config {

// Select ISA extensions
// `define M_MODULE    // multiplication and division

//////////////////////////////////////////
//              Memory config           //
//////////////////////////////////////////

// Program counter initial value
static const unsigned int INITIAL_PC = 0x00400000;

// Instruction memory
static const unsigned int TEXT_BEGIN = INITIAL_PC;
static const unsigned int TEXT_BITS = 16;
static const unsigned int TEXT_WIDTH = (1 << TEXT_BITS);
static const unsigned int TEXT_END = (TEXT_BEGIN + TEXT_WIDTH - 1);

// Data memory
static const unsigned int DATA_BEGIN = 0x80000000;
static const unsigned int DATA_BITS = 17;
static const unsigned int DATA_WIDTH = (1 << DATA_BITS);
static const unsigned int DATA_END = (DATA_BEGIN + DATA_WIDTH - 1);

static const char* TEXT_HEX = "add.text.vh";
static const char* DATA_HEX = "add.data.vh";

};  // namespace rv_config

/*verilator lint_off width*/

#endif  // RVSIMPLE_CONFIG_H