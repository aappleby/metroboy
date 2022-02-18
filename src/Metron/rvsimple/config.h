// RISC-V SiMPLE SV -- common configuration for testbench
// BSD 3-Clause License
// (c) 2017-2021, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RV_CONFIG
#define RV_CONFIG

// Select ISA extensions
// `define M_MODULE    // multiplication and division

//////////////////////////////////////////
//              Memory config           //
//////////////////////////////////////////

// Program counter initial value
#define INITIAL_PC      0x00400000

// Instruction memory
#define TEXT_BEGIN      INITIAL_PC
#define TEXT_BITS       16
#define TEXT_WIDTH      pow2(TEXT_BITS)
#define TEXT_END        (TEXT_BEGIN + TEXT_WIDTH - 1)

// Data memory
#define DATA_BEGIN      0x80000000
#define DATA_BITS       17
#define DATA_WIDTH      pow2(DATA_BITS)
#define DATA_END        (DATA_BEGIN + DATA_WIDTH - 1)

#define TEXT_HEX        "add.text.vh"
#define DATA_HEX        "add.data.vh"

#endif