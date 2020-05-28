#include <stdint.h>

//-----------------------------------------------------------------------------

extern const uint32_t gb_colors[8] = {
  0xFF7F7F7F,
  0xFF5F5F5F,
  0xFF3F3F3F,
  0xFF1F1F1F,
  0xFF9F7F7F,
  0xFF7F5F5F,
  0xFF5F3F3F,
  0xFF3F1F1F,
};

//-----------------------------------------------------------------------------

extern const char* op_strings[256] = {
  "nop",                "ld bc, $%04hx",  "ld (bc), a",     "inc bc",      "inc b",          "dec b",        "ld b, $%02hhx",      "rlca",
  "ld ($%04hx), sp",    "add hl, bc",     "ld a, (bc)",     "dec bc",      "inc c",          "dec c",        "ld c, $%02hhx",      "rrca",
  "stop",               "ld de, $%04hx",  "ld (de), a",     "inc de",      "inc d",          "dec d",        "ld d, $%02hhx",      "rla",
  "jr %hhd",            "add hl, de",     "ld a, (de)",     "dec de",      "inc e",          "dec e",        "ld e, $%02hhx",      "rra",
  "jr nz, %hhd",        "ld hl, $%04hx",  "ld (hl+), a",    "inc hl",      "inc h",          "dec h",        "ld h, $%02hhx",      "daa",
  "jr z, %hhd",         "add hl, hl",     "ld a, (hl+)",    "dec hl",      "inc l",          "dec l",        "ld l, $%02hhx",      "cpl",
  "jr nc, %hhd",        "ld sp, $%04hx",  "ld (hl-), a",    "inc sp",      "inc (hl)",       "dec (hl)",     "ld (hl), $%02hhx",   "scf",
  "jr c, %hhd",         "add hl, sp",     "ld a, (hl-)",    "dec sp",      "inc a",          "dec a",        "ld a, $%02hhx",      "ccf",

  "ld b, b",            "ld b, c",        "ld b, d",        "ld b, e",     "ld b, h",        "ld b, l",      "ld b, (hl)",    "ld b, a",
  "ld c, b",            "ld c, c",        "ld c, d",        "ld c, e",     "ld c, h",        "ld c, l",      "ld c, (hl)",    "ld c, a",
  "ld d, b",            "ld d, c",        "ld d, d",        "ld d, e",     "ld d, h",        "ld d, l",      "ld d, (hl)",    "ld d, a",
  "ld e, b",            "ld e, c",        "ld e, d",        "ld e, e",     "ld e, h",        "ld e, l",      "ld e, (hl)",    "ld e, a",
  "ld h, b",            "ld h, c",        "ld h, d",        "ld h, e",     "ld h, h",        "ld h, l",      "ld h, (hl)",    "ld h, a",
  "ld l, b",            "ld l, c",        "ld l, d",        "ld l, e",     "ld l, h",        "ld l, l",      "ld l, (hl)",    "ld l, a",
  "ld (hl), b",         "ld (hl), c",     "ld (hl), d",     "ld (hl), e",  "ld (hl), h",     "ld (hl), l",   "halt",          "ld (hl), a",
  "ld a, b",            "ld a, c",        "ld a, d",        "ld a, e",     "ld a, h",        "ld a, l",      "ld a, (hl)",    "ld a, a",
                                          
  "add b",              "add c",          "add d",          "add e",       "add h",          "add l",        "add (hl)",      "add a",
  "adc b",              "adc c",          "adc d",          "adc e",       "adc h",          "adc l",        "adc (hl)",      "adc a",
  "sub b",              "sub c",          "sub d",          "sub e",       "sub h",          "sub l",        "sub (hl)",      "sub a",
  "sbc b",              "sbc c",          "sbc d",          "sbc e",       "sbc h",          "sbc l",        "sbc (hl)",      "sbc a",
  "and b",              "and c",          "and d",          "and e",       "and h",          "and l",        "and (hl)",      "and a",
  "xor b",              "xor c",          "xor d",          "xor e",       "xor h",          "xor l",        "xor (hl)",      "xor a",
  "or b",               "or c",           "or d",           "or e",        "or h",           "or l",         "or (hl)",       "or a",
  "cp b",               "cp c",           "cp d",           "cp e",        "cp h",           "cp l",         "cp (hl)",       "cp a",
                                          
  "ret nz",             "pop bc",         "jp nz, $%04hx",   "jp $%04hx",   "call nz, $%04hx", "push bc",      "add $%02hhx",        "rst $00",
  "ret z",              "ret",            "jp z, $%04hx",    "prefix cb",   "call z, $%04hx",  "call $%04hx",  "adc $%02hhx",        "rst $08",
  "ret nc",             "pop de",         "jp nc, $%04hx",   "undefined1",  "call nc, $%04hx", "push de",      "sub $%02hhx",        "rst $10",
  "ret c",              "reti",           "jp c, $%04hx",    "undefined2",  "call c, $%04hx",  "undefined3",   "sbc $%02hhx",        "rst $18",
  "ldh ($%02hhx), a",   "pop hl",         "ldh (c), a",      "undefined4",  "undefined5",      "push hl",      "and $%02hhx",        "rst $20",
  "add sp, %d",         "jp hl",          "ld ($%04hx), a",  "undefined6",  "undefined7",      "undefined8",   "xor $%02hhx",        "rst $28",
  "ldh a, ($%02hhx)",   "pop af",         "ldh a, (c)",      "di",          "undefined9",      "push af",      "or $%02hhx",        "rst $30",
  "ld hl, sp + %d",     "ld sp, hl",      "ld a, ($%04hx)",  "ei",          "undefined10",     "undefined11",  "cp $%02hhx",        "rst $38",
};

extern const char* cb_strings[256] = {
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

extern const int op_sizes[256] = {
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

extern const int op_times_min[256] = {
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

extern const int op_times_max[256] = {
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

uint8_t flip(uint8_t x) {
  x = ((x & 0b10101010) >> 1) | ((x & 0b01010101) << 1);
  x = ((x & 0b11001100) >> 2) | ((x & 0b00110011) << 2);
  x = ((x & 0b11110000) >> 4) | ((x & 0b00001111) << 4);
  return x;
}

//-----------------------------------------------------------------------------

