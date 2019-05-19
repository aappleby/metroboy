#include "Platform.h"
#include "Assembler.h"

#include "Common.h"

void Assembler::loc(int x) {
  if (emit_source) {
    char buf[256];
    sprintf(buf, ".org $%04x\n", x);
    generated_source.append(buf);
  }
}

void copy_line(const char*& source, char* dest) {
  // skip leading whitespace
  while (isspace(*source)) source++;

  // copy until eol or comment
  while (*source != 0 && *source != '\n' && *source != '/' && *source != ';') {
    *dest++ = *source++;
  }

  // trim
  do {
    *dest-- = 0;
  } while (isspace((uint8_t)(*dest)));

  // skip to real eol

  while (1) {
    if (*source == 0) break;
    if (*source == '\n') {
      source++;
      break;
    }
    if (*source == ';') {
      source++;
      break;
    }
  }
}

char* Assembler::disassemble_one(const uint8_t* code, char* cursor) {
  uint8_t op0 = code[0];

  if (op0 == 0xCB) {
    uint8_t op1 = code[1];
    const char* op_string = cb_strings[op1];
    cursor += sprintf(cursor, "%s", op_string);
    return cursor;
  }

  const char* op_string = op_strings[op0];
  int size = op_sizes[op0];

  if (size == 1) {
    cursor += sprintf(cursor, "%s", op_string);
  }
  if (size == 2) {
    uint8_t op1 = code[1];
    cursor += sprintf(cursor, op_string, op1);
  }
  if (size == 3) {
    uint8_t op1 = code[1];
    uint8_t op2 = code[2];
    cursor += sprintf(cursor, op_string, (op2 << 8) | op1);
  }
  return cursor;
}

int Assembler::disassemble(const uint8_t* code, uint16_t code_base, int code_cursor, int opcount, char* cursor) {
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

uint8_t* Assembler::assemble(const char* source, uint8_t* code) {
  const char* source_cursor = source;
  int code_cursor = 0;
  int ops = 0;

  while (*source_cursor) {
    char line_buf[256];
    char* line = line_buf;
    copy_line(source_cursor, line);
    if (*line == 0) continue;

    uint32_t loc = 0;
    //if (sscanf(line, "%04x:", &loc) == 1) {
    if (line[4] == ':') {
      sscanf(line, "%04x:", &loc);
      line += 5;
      while (isspace(*line)) line++;
      code_cursor = loc;
      printf(".org $%04x\n", loc);
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
        uint8_t* temp = &code[code_cursor];

        int op_size = op_sizes[op];
        code[code_cursor++] = uint8_t(op);
        if (op_size > 1) code[code_cursor++] = arg & 0xFF;
        if (op_size > 2) code[code_cursor++] = (arg >> 8) & 0xFF;
        ops++;

        if (emit_source) {
          char blah[256];
          disassemble_one(temp, blah);
          generated_source.append(blah);
          generated_source.append("\n");
        }

        break;
      }
    }
    if (op == 256) {
      printf("BAD ASM\n");
    }
  }
  return &code[code_cursor];
}

/*
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
*/