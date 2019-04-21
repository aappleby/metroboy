#pragma once
#include <stdint.h>

int disassemble(const uint8_t* code, uint16_t code_base, int code_cursor, int opcount, char* cursor);
int assemble(const char* source, uint8_t* code, int& out_size);

static const char* op_strings[256] = {
  "nop",                "ld bc, $%04x",  "st (bc), a",     "inc bc",      "inc b",          "dec b",        "ld b, $%02x",     "rlca",
  "st ($%04x), sp",     "add hl, bc",    "ld a, (bc)",     "dec bc",      "inc c",          "dec c",        "ld c, $%02x",     "rrca",
  "stop $%02x",         "ld de, $%04x",  "st (de), a",     "inc de",      "inc d",          "dec d",        "ld d, $%02x",     "rla",
  "jr $%02x",           "add hl, de",    "ld a, (de)",     "dec de",      "inc e",          "dec e",        "ld e, $%02x",     "rra",
  "jr nz, $%02x",       "ld hl, $%04x",  "ld (hl+), a",    "inc hl",      "inc h",          "dec h",        "ld h, $%02x",     "daa",
  "jr z, $%02x",        "add hl, hl",    "ld a, (hl+)",    "dec hl",      "inc l",          "dec l",        "ld l, $%02x",     "cpl",
  "jr nc, $%02x",       "ld sp, $%04x",  "ld (hl-), a",    "inc sp",      "inc (hl)",       "dec (hl)",     "ld (hl), $%02x",  "scf",
  "jr c, $%02x",        "add hl, sp",    "ld a, (hl-)",    "dec sp",      "inc a",          "dec a",        "ld a, $%02x",     "ccf",

  "ld b, b",            "ld b, c",       "ld b, d",        "ld b, e",     "ld b, h",        "ld b, l",      "ld b, hl",        "ld b, a",
  "ld c, b",            "ld c, c",       "ld c, d",        "ld c, e",     "ld c, h",        "ld c, l",      "ld c, hl",        "ld c, a",
  "ld d, b",            "ld d, c",       "ld d, d",        "ld d, e",     "ld d, h",        "ld d, l",      "ld d, hl",        "ld d, a",
  "ld e, b",            "ld e, c",       "ld e, d",        "ld e, e",     "ld e, h",        "ld e, l",      "ld e, hl",        "ld e, a",
  "ld h, b",            "ld h, c",       "ld h, d",        "ld h, e",     "ld h, h",        "ld h, l",      "ld h, hl",        "ld h, a",
  "ld l, b",            "ld l, c",       "ld l, d",        "ld l, e",     "ld l, h",        "ld l, l",      "ld l, hl",        "ld l, a",
  "ld (hl), b",         "ld (hl), c",    "ld (hl), d",     "ld (hl), e",  "ld (hl), h",     "ld (hl), l",   "halt",            "ld (hl), a",
  "ld a, b",            "ld a, c",       "ld a, d",        "ld a, e",     "ld a, h",        "ld a, l",      "ld a, hl",        "ld a, a",

  "add a, b",           "add a, c",      "add a, d",       "add a, e",    "add a, h",       "add a, l",     "add a, hl",       "add a, a",
  "adc a, b",           "adc a, c",      "adc a, d",       "adc a, e",    "adc a, h",       "adc a, l",     "adc a, hl",       "adc a, a",
  "sub a, b",           "sub a, c",      "sub a, d",       "sub a, e",    "sub a, h",       "sub a, l",     "sub a, hl",       "sub a, a",
  "sbc a, b",           "sbc a, c",      "sbc a, d",       "sbc a, e",    "sbc a, h",       "sbc a, l",     "sbc a, hl",       "sbc a, a",
  "and a, b",           "and a, c",      "and a, d",       "and a, e",    "and a, h",       "and a, l",     "and a, hl",       "and a, a",
  "xor a, b",           "xor a, c",      "xor a, d",       "xor a, e",    "xor a, h",       "xor a, l",     "xor a, hl",       "xor a, a",
  "or a, b",            "or a, c",       "or a, d",        "or a, e",     "or a, h",        "or a, l",      "or a, hl",        "or a, a",
  "cp a, b",            "cp a, c",       "cp a, d",        "cp a, e",     "cp a, h",        "cp a, l",      "cp a, hl",        "cp a, a",

  "ret nz",             "pop bc",        "jp nz, $%04x",   "jp $%04x",    "call nz, $%04x", "push bc",      "add a, $%02x",    "rst $00",
  "ret z",              "ret",           "jp z, $%04x",    "prefix cb",   "call z, $%04x",  "call $%04x",   "adc a, $%02x",    "rst $08",
  "ret nc",             "pop de",        "jp nc, $%04x",   "undefined1",  "call nc, $%04x", "push de",      "sub a, $%02x",    "rst $10",
  "ret c",              "reti",          "jp c, $%04x",    "undefined2",  "call c, $%04x",  "undefined3",   "sbc a, $%02x",    "rst $18",
  "ldh ($%02x), a",     "pop hl",        "ldh (c), a",     "undefined4",  "undefined5",     "push hl",      "and a, $%02x",    "rst $20",
  "add sp, $%02x",      "jp hl",         "ldh ($%04x), a", "undefined6",  "undefined7",     "undefined8",   "xor a, $%02x",    "rst $28",
  "ldh a, ($%02x)",     "pop af",        "ldh a, (c)",     "di",          "undefined9",     "push af",      "or a, $%02x",     "rst $30",
  "ld hl, sp+($%02x)",  "ld sp, hl",     "ldh a, ($%04x)", "ei",          "undefined10",    "undefined11",  "cp a, $%02x",     "rst $38",
};


static const char* cb_strings[256] = {
  "rlc b",    "rlc c",    "rlc d",    "rlc e",    "rlc h",    "rlc l",    "rlc (hl)",    "rlc a",
  "rrc b",    "rrc c",    "rrc d",    "rrc e",    "rrc h",    "rrc l",    "rrc (hl)",    "rrc a",
  "rl b",     "rl c",     "rl d",     "rl e",     "rl h",     "rl l",     "rl (hl)",     "rl a",
  "rr b",     "rr c",     "rr d",     "rr e",     "rr h",     "rr l",     "rr (hl)",     "rr a",
  "sla b",    "sla c",    "sla d",    "sla e",    "sla h",    "sla l",    "sla (hl)",    "sla a",
  "sra b",    "sra c",    "sra d",    "sra e",    "sra h",    "sra l",    "sra (hl)",    "sra a",
  "swap b",   "swap c",   "swap d",   "swap e",   "swap h",   "swap l",   "swap (hl)",   "swap a",
  "srl b",    "srl c",    "srl d",    "srl e",    "srl h",    "srl l",    "srl (hl)",    "srl a",

  "bit 0, b", "bit 0, c", "bit 0, d", "bit 0, e", "bit 0, h", "bit 0, l", "bit 0, (hl)", "bit 0, a",
  "bit 1, b", "bit 1, c", "bit 1, d", "bit 1, e", "bit 1, h", "bit 1, l", "bit 1, (hl)", "bit 1, a",
  "bit 2, b", "bit 2, c", "bit 2, d", "bit 2, e", "bit 2, h", "bit 2, l", "bit 2, (hl)", "bit 2, a",
  "bit 3, b", "bit 3, c", "bit 3, d", "bit 3, e", "bit 3, h", "bit 3, l", "bit 3, (hl)", "bit 3, a",
  "bit 4, b", "bit 4, c", "bit 4, d", "bit 4, e", "bit 4, h", "bit 4, l", "bit 4, (hl)", "bit 4, a",
  "bit 5, b", "bit 5, c", "bit 5, d", "bit 5, e", "bit 5, h", "bit 5, l", "bit 5, (hl)", "bit 5, a",
  "bit 6, b", "bit 6, c", "bit 6, d", "bit 6, e", "bit 6, h", "bit 6, l", "bit 6, (hl)", "bit 6, a",
  "bit 7, b", "bit 7, c", "bit 7, d", "bit 7, e", "bit 7, h", "bit 7, l", "bit 7, (hl)", "bit 7, a",

  "res 0, b", "res 0, c", "res 0, d", "res 0, e", "res 0, h", "res 0, l", "res 0, (hl)", "res 0, a",
  "res 1, b", "res 1, c", "res 1, d", "res 1, e", "res 1, h", "res 1, l", "res 1, (hl)", "res 1, a",
  "res 2, b", "res 2, c", "res 2, d", "res 2, e", "res 2, h", "res 2, l", "res 2, (hl)", "res 2, a",
  "res 3, b", "res 3, c", "res 3, d", "res 3, e", "res 3, h", "res 3, l", "res 3, (hl)", "res 3, a",
  "res 4, b", "res 4, c", "res 4, d", "res 4, e", "res 4, h", "res 4, l", "res 4, (hl)", "res 4, a",
  "res 5, b", "res 5, c", "res 5, d", "res 5, e", "res 5, h", "res 5, l", "res 5, (hl)", "res 5, a",
  "res 6, b", "res 6, c", "res 6, d", "res 6, e", "res 6, h", "res 6, l", "res 6, (hl)", "res 6, a",
  "res 7, b", "res 7, c", "res 7, d", "res 7, e", "res 7, h", "res 7, l", "res 7, (hl)", "res 7, a",

  "set 0, b", "set 0, c", "set 0, d", "set 0, e", "set 0, h", "set 0, l", "set 0, (hl)", "set 0, a",
  "set 1, b", "set 1, c", "set 1, d", "set 1, e", "set 1, h", "set 1, l", "set 1, (hl)", "set 1, a",
  "set 2, b", "set 2, c", "set 2, d", "set 2, e", "set 2, h", "set 2, l", "set 2, (hl)", "set 2, a",
  "set 3, b", "set 3, c", "set 3, d", "set 3, e", "set 3, h", "set 3, l", "set 3, (hl)", "set 3, a",
  "set 4, b", "set 4, c", "set 4, d", "set 4, e", "set 4, h", "set 4, l", "set 4, (hl)", "set 4, a",
  "set 5, b", "set 5, c", "set 5, d", "set 5, e", "set 5, h", "set 5, l", "set 5, (hl)", "set 5, a",
  "set 6, b", "set 6, c", "set 6, d", "set 6, e", "set 6, h", "set 6, l", "set 6, (hl)", "set 6, a",
  "set 7, b", "set 7, c", "set 7, d", "set 7, e", "set 7, h", "set 7, l", "set 7, (hl)", "set 7, a",
};

static const int op_sizes[256] = {
  1, 3, 1, 1, 1, 1, 2, 1,
  3, 1, 1, 1, 1, 1, 2, 1,
  2, 3, 1, 1, 1, 1, 2, 1,
  2, 1, 1, 1, 1, 1, 2, 1,
  2, 3, 1, 1, 1, 1, 2, 1,
  2, 1, 1, 1, 1, 1, 2, 1,
  2, 3, 1, 1, 1, 1, 2, 1,
  2, 1, 1, 1, 1, 1, 2, 1,

  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,

  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,

  1, 1, 3, 3, 3, 1, 2, 1,
  1, 1, 3, 1, 3, 3, 2, 1,
  1, 1, 3, 1, 3, 1, 2, 1,
  1, 1, 3, 1, 3, 1, 2, 1,
  2, 1, 1, 1, 1, 1, 2, 1,
  2, 1, 3, 1, 1, 1, 2, 1,
  2, 1, 1, 1, 1, 1, 2, 1,
  2, 1, 3, 1, 1, 1, 2, 1,
};

static const int op_times_min[256] = {
  1, 3, 2, 2, 1, 1, 2, 1,
  5, 2, 2, 2, 1, 1, 2, 1,
  1, 3, 2, 2, 1, 1, 2, 1,
  3, 2, 2, 2, 1, 1, 2, 1,
  2, 3, 2, 2, 1, 1, 2, 1,
  2, 2, 2, 2, 1, 1, 2, 1,
  2, 3, 2, 2, 3, 3, 3, 1,
  2, 2, 2, 2, 1, 1, 2, 1,

  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  2, 2, 2, 2, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 2, 1,

  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,

  2, 3, 3, 4, 3, 4, 2, 4,
  2, 4, 3, 1, 3, 6, 2, 4,
  2, 3, 3, 0, 3, 4, 2, 4,
  2, 4, 3, 0, 3, 0, 2, 4,
  3, 3, 2, 0, 0, 4, 2, 4,
  4, 1, 4, 0, 0, 0, 2, 4,
  3, 3, 2, 1, 0, 4, 2, 4,
  3, 2, 4, 1, 0, 0, 2, 4,
};

static const int op_times_max[256] = {
  1, 3, 2, 2, 1, 1, 2, 1,
  5, 2, 2, 2, 1, 1, 2, 1,
  1, 3, 2, 2, 1, 1, 2, 1,
  3, 2, 2, 2, 1, 1, 2, 1,
  3, 3, 2, 2, 1, 1, 2, 1,
  3, 2, 2, 2, 1, 1, 2, 1,
  3, 3, 2, 2, 3, 3, 3, 1,
  3, 2, 2, 2, 1, 1, 2, 1,

  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  2, 2, 2, 2, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 2, 1,

  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 2, 1,

  5, 3, 4, 4, 6, 4, 2, 4,
  5, 4, 4, 1, 6, 6, 2, 4,
  5, 3, 4, 0, 6, 4, 2, 4,
  5, 4, 4, 0, 6, 0, 2, 4,
  3, 3, 2, 0, 0, 4, 2, 4,
  4, 1, 4, 0, 0, 0, 2, 4,
  3, 3, 2, 1, 0, 4, 2, 4,
  3, 2, 4, 1, 0, 0, 2, 4,
};

//-----------------------------------------------------------------------------
// quad 0

#define NOP            (0x00)
#define LD_BC_D16(A)   (0x01), uint8_t(A), uint8_t((A) >> 8)
#define ST_BC_A        (0x02)
#define INC_BC         (0x03)
#define INC_B          (0x04)
#define DEC_B          (0x05)
#define LD_B_D8(A)     (0x06), uint8_t(A)
#define RLCA           (0x07)
                       
#define ST_A16_SP(A)   (0x08), uint8_t(A), uint8_t((A) >> 8)
#define ADD_HL_BC      (0x09)
#define LD_A_BC        (0x0A)
#define DEC_BC         (0x0B)
#define INC_C          (0x0C)
#define DEC_C          (0x0D)
#define LD_C_D8(A)     (0x0E), uint8_t(A)
#define RRCA           (0x0F)
                       
#define STOP           (0x10)
#define LD_DE_D16(A)   (0x11), uint8_t(A), uint8_t((A) >> 8)
#define ST_DE_A        (0x12)
#define INC_DE         (0x13)
#define INC_D          (0x14)
#define DEC_D          (0x15)
#define LD_D_D8(A)     (0x16), uint8_t(A)
#define RLA            (0x17)
                       
#define JR_R8(A)       (0x18), uint8_t(A)
#define ADD_HL_DE      (0x19)
#define LD_A_DE        (0x1A)
#define DEC_DE         (0x1B)
#define INC_E          (0x1C)
#define DEC_E          (0x1D)
#define LD_E_D8(A)     (0x1E), uint8_t(A)
#define RRA            (0x1F)
                       
#define JR_NZ_R8(A)    (0x20), uint8_t(A)
#define LD_HL_D16(A)   (0x21), uint8_t(A), uint8_t((A) >> 8)
#define ST_HLP_A       (0x22)
#define INC_HL         (0x23)
#define INC_H          (0x24)
#define DEC_H          (0x25)
#define LD_H_D8(A)     (0x26), uint8_t(A)
#define DAA            (0x27)
                       
#define JR_Z_R8(A)     (0x28), uint8_t(A)
#define ADD_HL_HL      (0x29)
#define LD_A_HLP       (0x2A)
#define DEC_HL         (0x2B)
#define INC_L          (0x2C)
#define DEC_L          (0x2D)
#define LD_L_D8(A)     (0x2E), uint8_t(A)
#define CPL            (0x2F)
                       
#define JR_NC_R8(A)    (0x30), uint8_t(A)
#define LD_SP_D16(A)   (0x31), uint8_t(A), uint8_t((A) >> 8)
#define ST_HLM_A       (0x32)
#define INC_SP         (0x33)
#define INC_AT_HL      (0x34)
#define DEC_AT_HL      (0x35)
#define ST_HL_D8(A)    (0x36), uint8_t(A)
#define SCF            (0x37)
                       
#define JR_C_R8(A)     (0x38), uint8_t(A)
#define ADD_HL_SP      (0x39)
#define LD_A_HLM       (0x3A)
#define DEC_SP         (0x3B)
#define INC_A          (0x3C)
#define DEC_A          (0x3D)
#define LD_A_D8(A)     (0x3E), uint8_t(A)
#define CCF            (0x3F)
                       
//-----------------------------------------------------------------------------
// quad 1              
                       
// special "magic" op  code for "ld b,b" that the mooneye tests use to indicate completion
#define BREAK          (0x40)
                       
#define LD_B_B         (0x40)
#define LD_B_C         (0x41)
#define LD_B_D         (0x42)
#define LD_B_E         (0x43)
#define LD_B_H         (0x44)
#define LD_B_L         (0x45)
#define LD_B_HL        (0x46)
#define LD_B_A         (0x47)
                       
#define LD_C_B         (0x48)
#define LD_C_C         (0x49)
#define LD_C_D         (0x4A)
#define LD_C_E         (0x4B)
#define LD_C_H         (0x4C)
#define LD_C_L         (0x4D)
#define LD_C_HL        (0x4E)
#define LD_C_A         (0x4F)
                       
#define LD_D_B         (0x50)
#define LD_D_C         (0x51)
#define LD_D_D         (0x52)
#define LD_D_E         (0x53)
#define LD_D_H         (0x54)
#define LD_D_L         (0x55)
#define LD_D_HL        (0x56)
#define LD_D_A         (0x57)
                       
#define LD_E_B         (0x58)
#define LD_E_C         (0x59)
#define LD_E_D         (0x5A)
#define LD_E_E         (0x5B)
#define LD_E_H         (0x5C)
#define LD_E_L         (0x5D)
#define LD_E_HL        (0x5E)
#define LD_E_A         (0x5F) 
                       
#define LD_H_B         (0x60)
#define LD_H_C         (0x61)
#define LD_H_D         (0x62)
#define LD_H_E         (0x63)
#define LD_H_H         (0x64)
#define LD_H_L         (0x65)
#define LD_H_HL        (0x66)
#define LD_H_A         (0x67)
                       
#define LD_L_B         (0x68)
#define LD_L_C         (0x69)
#define LD_L_D         (0x6A)
#define LD_L_E         (0x6B)
#define LD_L_H         (0x6C)
#define LD_L_L         (0x6D)
#define LD_L_HL        (0x6E)
#define LD_L_A         (0x6F)
                       
#define ST_HL_B        (0x70)
#define ST_HL_C        (0x71)
#define ST_HL_D        (0x72)
#define ST_HL_E        (0x73)
#define ST_HL_H        (0x74)
#define ST_HL_L        (0x75)
#define HALT           (0x76)
#define ST_HL_A        (0x77)
                       
#define LD_A_B         (0x78)
#define LD_A_C         (0x79)
#define LD_A_D         (0x7A)
#define LD_A_E         (0x7B)
#define LD_A_H         (0x7C)
#define LD_A_L         (0x7D)
#define LD_A_HL        (0x7E)
#define LD_A_A         (0x7F)
                       
//-----------------------------------------------------------------------------
// quad 2              
                       
#define ADD_A_B        (0x80)
#define ADD_A_C        (0x81)
#define ADD_A_D        (0x82)
#define ADD_A_E        (0x83)
#define ADD_A_H        (0x84)
#define ADD_A_L        (0x85)
#define ADD_A_HL       (0x86)
#define ADD_A_A        (0x87)
                       
#define ADC_A_B        (0x88)
#define ADC_A_C        (0x89)
#define ADC_A_D        (0x8A)
#define ADC_A_E        (0x8B)
#define ADC_A_H        (0x8C)
#define ADC_A_L        (0x8D)
#define ADC_A_HL       (0x8E)
#define ADC_A_A        (0x8F)
                       
#define SUB_A_B        (0x90)
#define SUB_A_C        (0x91)
#define SUB_A_D        (0x92)
#define SUB_A_E        (0x93)
#define SUB_A_H        (0x94)
#define SUB_A_L        (0x95)
#define SUB_A_HL       (0x96)
#define SUB_A_A        (0x97)
                       
#define SBC_A_B        (0x98)
#define SBC_A_C        (0x99)
#define SBC_A_D        (0x9A)
#define SBC_A_E        (0x9B)
#define SBC_A_H        (0x9C)
#define SBC_A_L        (0x9D)
#define SBC_A_HL       (0x9E)
#define SBC_A_A        (0x9F)
                       
#define AND_A_B        (0xA0)
#define AND_A_C        (0xA1)
#define AND_A_D        (0xA2)
#define AND_A_E        (0xA3)
#define AND_A_H        (0xA4)
#define AND_A_L        (0xA5)
#define AND_A_HL       (0xA6)
#define AND_A_A        (0xA7)
                       
#define XOR_A_B        (0xA8)
#define XOR_A_C        (0xA9)
#define XOR_A_D        (0xAA)
#define XOR_A_E        (0xAB)
#define XOR_A_H        (0xAC)
#define XOR_A_L        (0xAD)
#define XOR_A_HL       (0xAE)
#define XOR_A_A        (0xAF)
                       
#define OR_A_B         (0xB0)
#define OR_A_C         (0xB1)
#define OR_A_D         (0xB2)
#define OR_A_E         (0xB3)
#define OR_A_H         (0xB4)
#define OR_A_L         (0xB5)
#define OR_A_HL        (0xB6)
#define OR_A_A         (0xB7)
                       
#define CP_A_B         (0xB8)
#define CP_A_C         (0xB9)
#define CP_A_D         (0xBA)
#define CP_A_E         (0xBB)
#define CP_A_H         (0xBC)
#define CP_A_L         (0xBD)
#define CP_A_HL        (0xBE)
#define CP_A_A         (0xBF)

//-----------------------------------------------------------------------------
// quad 3

#define RET_NZ         (0xC0)
#define POP_BC         (0xC1)
#define JP_NZ_A16(A)   (0xC2), uint8_t(A), uint8_t((A) >> 8)
#define JP_A16(A)      (0xC3), uint8_t(A), uint8_t((A) >> 8)
#define CALL_NZ_A16(A) (0xC4), uint8_t(A), uint8_t((A) >> 8)
#define PUSH_BC        (0xC5)
#define ADD_A_D8(A)    (0xC6), uint8_t(A)
#define RST_00         (0xC7)
                       
#define RET_Z          (0xC8)
#define RET            (0xC9)
#define JP_Z_A16(A)    (0xCA), uint8_t(A), uint8_t((A) >> 8)
#define PREFIX_CB      (0xCB)
#define CALL_Z_A16(A)  (0xCC), uint8_t(A), uint8_t((A) >> 8)
#define CALL_A16(A)    (0xCD), uint8_t(A), uint8_t((A) >> 8)
#define ADC_A_D8(A)    (0xCE), uint8_t(A)
#define RST_08         (0xCF)
                       
#define RET_NC         (0xD0)
#define POP_DE         (0xD1)
#define JP_NC_A16(A)   (0xD2)
#define UNDEFINED1     (0xD3)
#define CALL_NC_A16(A) (0xD4), uint8_t(A), uint8_t((A) >> 8)
#define PUSH_DE        (0xD5)
#define SUB_A_D8(A)    (0xD6), uint8_t(A)
#define RST_10         (0xD7)
                       
#define RET_C          (0xD8)
#define RETI           (0xD9)
#define JP_C_A16(A)    (0xDA), uint8_t(A), uint8_t((A) >> 8)
#define UNDEFINED2     (0xDB)
#define CALL_C_A16(A)  (0xDC), uint8_t(A), uint8_t((A) >> 8)
#define UNDEFINED3     (0xDD)
#define SBC_A_D8(A)    (0xDE), uint8_t(A)
#define RST_18         (0xDF)
                       
#define ST_A8_A(A)     (0xE0), uint8_t(A)
#define POP_HL         (0xE1)
#define ST_C_A         (0xE2)
#define UNDEFINED4     (0xE3)
#define UNDEFINED5     (0xE4)
#define PUSH_HL        (0xE5)
#define AND_A_D8(A)    (0xE6), uint8_t(A)
#define RST_20         (0xE7)
                       
#define ADD_SP_R8(A)   (0xE8), uint8_t(A)
#define JP_HL          (0xE9)
#define ST_A16_A       (0xEA), uint8_t(A), uint8_t((A) >> 8)
#define UNDEFINED6     (0xEB)
#define UNDEFINED7     (0xEC)
#define UNDEFINED8     (0xED)
#define XOR_A_D8(A)    (0xEE), uint8_t(A)
#define RST_28         (0xEF)
                       
#define LD_A_A8(A)     (0xF0), uint8_t(A)
#define POP_AF         (0xF1)
#define LD_A_AT_C      (0xF2)
#define DI             (0xF3)
#define UNDEFINED9     (0xF4)
#define PUSH_AF        (0xF5)
#define OR_A_D8(A)     (0xF6), uint8_t(A)
#define RST_30         (0xF7)
                       
#define LD_HL_SP_R8(A) (0xF8), uint8_t(A)
#define LD_SP_HL       (0xF9)
#define LD_A_A16(A)    (0xFA), uint8_t(A), uint8_t((A) >> 8)
#define EI             (0xFB)
#define UNDEFINED10    (0xFC)
#define UNDEFINED11    (0xFD)
#define CP_A_D8(A)     (0xFE), uint8_t(A)
#define RST_38         (0xFF)