#include "Platform.h"
#include "Assembler.h"

#include "Common.h"

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

void copy_line(const char*& source, char* dest) {
  // skip leading whitespace

  while (isspace(*source)) source++;

  // copy until eol or comment
  while (*source != 0 && *source != '\n' && *source != '/') {
    *dest++ = *source++;
  }

  // trim
  do {
    *dest-- = 0;
  } while (isspace((uint8_t)(*dest)));

  // skip to real eol
  while (*source != 0 && *source != '\n') source++;
}

int disassemble(const uint8_t* code, uint16_t code_base, int code_cursor, int opcount, char* cursor) {
  char* start = cursor;

  for (int i = 0; i < opcount; i++) {
    uint8_t op0 = code[code_cursor + 0];

    if (op0 == 0xCB) {
      uint8_t op1 = code[code_cursor + 1];
      const char* op_string = cb_strings[op1];
      int size = 2;
      cursor += sprintf(cursor, "%04x: %02x%02x   ", code_base + code_cursor, op0, op1);
      cursor += sprintf(cursor, "%s", op_string);
      *cursor++ = '\n';
      code_cursor += size;
      continue;
    }

    const char* op_string = op_strings[op0];
    int size = op_sizes[op0];

    if (size == 1) {
      cursor += sprintf(cursor, "%04x: %02x     ", code_base + code_cursor, op0);
      cursor += sprintf(cursor, "%s", op_string);
    }
    if (size == 2) {
      uint8_t op1 = code[code_cursor + 1];
      cursor += sprintf(cursor, "%04x: %02x%02x   ", code_base + code_cursor, op0, op1);
      cursor += sprintf(cursor, op_string, op1);
    }
    if (size == 3) {
      uint8_t op1 = code[code_cursor + 1];
      uint8_t op2 = code[code_cursor + 2];
      cursor += sprintf(cursor, "%04x: %02x%02x%02x ", code_base + code_cursor, op0, op1, op2);
      cursor += sprintf(cursor, op_string, (op2 << 8) | op1);
    }
    *cursor++ = '\n';

    code_cursor += size;
  }
  *cursor++ = 0;

  return static_cast<int>(cursor - start - 1);
}

int assemble(const char* source, uint8_t* code, int& out_size) {
  const char* source_cursor = source;
  int code_cursor = 0;
  int ops = 0;

  while (*source_cursor) {
    char line_buf[256];
    char* line = line_buf;
    copy_line(source_cursor, line);
    if (*line == 0) continue;

    uint32_t loc = 0;
    if (sscanf(line, "%04x:", &loc) == 1) {
      line += 5;
      while (isspace(*line)) line++;
      code_cursor = loc;
    }
    if (*line == 0) continue;

    int match = -1;
    int op = 0;
    int arg = 0;
    for (; op < 256; op++) {
      if (op_sizes[op] == 1) {
        match = strcmp(line, op_strings[op]);
      }
      else {
        match = sscanf(line, op_strings[op], &arg) - 1;
      }
      if (match == 0) {
        int op_size = op_sizes[op];
        code[code_cursor++] = uint8_t(op);
        if (op_size > 1) code[code_cursor++] = arg & 0xFF;
        if (op_size > 2) code[code_cursor++] = (arg >> 8) & 0xFF;
        ops++;
        break;
      }
    }
  }
  out_size = code_cursor;
  return ops;
}

void fuzz_assembler() {
  uint8_t code1[65536];
  char source1[65536];
  uint8_t code2[65536];
  char source2[65536];

  for (int rep = 0; rep <= 1000; rep++) {
    srand(rep);
    for (int i = 0; i < 100 * 3; i++) {
      code1[i] = 0 + rand() % 256;
    }

    disassemble(code1, 0, 0, 100, source1);

    int size = 0;
    int opcount = assemble(source1, code2, size);
    int cmp = memcmp(code1, code2, size);

    disassemble(code2, 0, 0, opcount, source2);

    if (cmp != 0) {
      printf("---- %d ------\n", rep);
      printf("%s", source1);
      printf("----------\n");
      printf("%s", source2);
    }
  }
}

