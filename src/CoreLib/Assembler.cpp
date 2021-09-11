#include "CoreLib/Assembler.h"
#include "CoreLib/Types.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Opcodes.h"
#include "CoreLib/Debug.h"
#include "CoreLib/Log.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#pragma warning(disable : 4996)

extern const char* op_strings[];
extern const char* cb_strings[];
extern const int op_sizes[];

const char* source_header = R"(
.gbheader
name "microtest"
nintendologo
.endgb
.memorymap
defaultslot 0
slot 0 start $0000 size $4000
.endme
.rombanksize $4000
.rombanks 2
.bank 0 slot 0

.macro nops ARGS count
.repeat count
  nop
.endr
.endm

.org $100
  nop
  jp $150

)";

uint8_t metadata[] = {
  // nop; jp $150
  0x00, 0xc3, 0x50, 0x01,

  // logo
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
  0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
  0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
  0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,

  // title
  0x6d, 0x69, 0x63, 0x72, 0x6f, 0x74, 0x65, 0x73, 0x74, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, // mfr code
  0x00,       // cgb flag
  0x00, 0x00, // new license code
  0x00,       // sgb flag
  0x02,       // cart type MBC1 + RAM
  0x00,       // rom size 32k
  0x02,       // ram size 8k
  0x01,       // dest code non-japanese
  0x33,       // old license code
  0x00,       // mask rom version
  0x0d,       // header checksum
  0x1a, 0x41, // global checksum
};

blob Assembler::link() {
  blob ret(32768);
  link_to(ret.data());
  return ret;
}

void Assembler::link_to(uint8_t* rom_buf) {
  memset(rom_buf, 0, 32768);
  memcpy(rom_buf + 0x100, metadata, sizeof(metadata));

  // copy blocks into rom_buf

  for (const auto& block : block_map) {
    uint16_t addr = block.first;
    const blob& code = block.second;
    memcpy(rom_buf + addr, code.data(), code.size());
  }

  uint8_t checksum = 0;
  for (int i = 0x0134; i <= 0x014C; i++) {
    checksum = checksum - rom_buf[i] - 1;
  }
  rom_buf[0x014D] = checksum;
}


void Assembler::write_source() {
  StringDumper dump;
  dump("%s", source_header);
  disassemble(dump);

  save_blob((name + ".s").c_str(), dump.s.data(), dump.s.size());
}

void Assembler::begin_label(std::string label) {
  label_map[label] = block_addr + (uint16_t)block_code->size();
}

void Assembler::begin_block(uint16_t addr) {
  blob& code = block_map[addr];
  assert(code.empty());
  block_addr = addr;
  block_code = &code;
}

void Assembler::emit(uint8_t x) {
  block_code->push_back(x);
}

void Assembler::emit(uint8_t a, uint8_t b) {
  block_code->push_back(a);
  block_code->push_back(b);
}

void Assembler::emit(uint8_t a, uint8_t b, uint8_t c) {
  block_code->push_back(a);
  block_code->push_back(b);
  block_code->push_back(c);
}

//-----------------------------------------------------------------------------

void copy_line(const char*& source, char* dest) {
  // skip leading whitespace
  while (isspace(*source)) source++;

  // copy until eol or comment
  while (*source != 0 && *source != '\n' && *source != '/' && *source != ';') {
    *dest++ = (char)tolower(*source++);
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

//-----------------------------------------------------------------------------

void Assembler::disassemble_one(const uint8_t* code, Dumper& dump) {
  uint8_t op0 = code[0];

  if (op0 == 0xCB) {
    uint8_t op1 = code[1];
    const char* op_string = cb_strings[op1];
    dump("%s", op_string);
  }

  const char* op_string = op_strings[op0];
  int size = op_sizes[op0];

  if (size == 1) {
    dump("%s", op_string);
  }
  if (size == 2) {
    uint8_t op1 = code[1];
    dump(op_string, op1);
  }
  if (size == 3) {
    uint8_t op1 = code[1];
    uint8_t op2 = code[2];
    dump(op_string, (op2 << 8) | op1);
  }
}

//-----------------------------------------------------------------------------

void Assembler::disassemble(const uint8_t* code, int code_size, int code_base, int pc, int opcount, Dumper& dump, bool /*collapse_nops*/) {

  size_t code_cursor = pc - code_base;

  for (int i = 0; i < opcount; i++) {
    if (code_cursor >= code_size) return;

    uint8_t op0 = code[code_cursor + 0];
    int size = op_sizes[op0];
    const char* op_string = op_strings[op0];

    /*
    if (collapse_nops && op0 == 0) {
      int nop_count = 0;
      while (code[code_cursor] == 0) {
        nop_count++;
        code_cursor++;
        if (code_cursor >= code_size) break;
      }
      if (nop_count > 1) {
        //dump("%04x:        ", code_base + code_cursor, op0);
        dump("nops %d", nop_count);
        dump("\n");
      }
      else {
        //dump("%04x: 00     ", code_base + code_cursor, op0);
        dump("nop");
        dump("\n");
      }
      continue;
    }
    */

    dump("%04x: ", code_base + code_cursor);

    int arg = 0;

    if (op0 == 0xCB) {
      dump("%02x", op0);
      uint8_t op1 = code[code_cursor + 1];
      dump("%02x   ", op1);
      op_string = cb_strings[op1];
      size = 2;
    }
    else if (size == 1) {
      dump("%02x     ", op0);
      arg = 0;
    }
    else if (size == 2) {
      dump("%02x", op0);
      uint8_t lo = code[code_cursor + 1];
      dump("%02x   ", lo);
      arg = lo;
    }
    else if (size == 3) {
      dump("%02x", op0);
      uint8_t lo = code[code_cursor + 1];
      dump("%02x", lo);
      uint8_t hi = code[code_cursor + 2];
      dump("%02x ", hi);
      arg = ((hi << 8) | lo);

    }

    dump(op_string, arg);
    dump("\n");
    code_cursor += size;
  }
}

//-----------------------------------------------------------------------------

void Assembler::disassemble(Dumper& dump) {
  for (const auto& block : block_map) {
    uint16_t addr = block.first;
    dump(".org $%04x\n", addr);

    const blob& code = block.second;
    disassemble(code.data(), (int)code.size(), addr, addr, 100000, dump);
  }
}

//-----------------------------------------------------------------------------

bool match_op(const char* line, const char* op_string, int& arg) {
  char buf[256];
  sscanf(line, op_string, &arg);
  sprintf(buf, op_string, arg);
  return strcmp(line, buf) == 0;
}

void Assembler::assemble(const char* source) {
  while (*source) {
    char line_buf[256] = {0};
    char* line = line_buf;
    copy_line(source, line);
    if (*line == 0) continue;

    uint32_t loc = 0;
    if (line[4] == ':') {
      sscanf(line, "%04x:", &loc);
      line += 5;
      while (isspace(*line)) line++;
      begin_block(uint16_t(loc));
    }
    if (*line == 0) continue;

    int op = 0;
    int arg = 0;
    for (; op < 256; op++) {

      bool found_match = match_op(line, op_strings[op], arg);
      if (!found_match) {
        continue;
      }

      /*
      int match = -1;
      if (op_sizes[op] == 1) {
        match = strcmp(line, op_strings[op]);
      }
      else {
        match = sscanf(line, op_strings[op], &arg) - 1;
      }
      if (match != 0) {
        continue;
      }
      */

      int op_size = op_sizes[op];
      emit(uint8_t(op));
      if (op_size > 1) emit(arg & 0xFF);
      if (op_size > 2) emit((arg >> 8) & 0xFF);
      break;
    }
    if (op == 256) {
      LOG_R("BAD ASM\n");
    }
  }
}

//-----------------------------------------------------------------------------

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
      LOG_B("---- %d ------\n", rep);
      LOG_B("%s", source1);
      LOG_B("-=====\001\n");
      LOG_B("%s", source2);
    }
  }
}
*/


//-----------------------------------------------------------------------------

template<typename ... Args>
void Assembler::assemble(const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  assemble(source_buf);
}

//-----------------------------------------------------------------------------

void Assembler::nops(int n) {
  for (int i = 0; i < n; i++) emit(0);
}


void Assembler::jr_bookmark() {
  size_t src = block_code->size();
  size_t dst = bookmarks.back();

  emit(JR_R8(dst - src - 2));

  bookmarks.pop_back();
}

void Assembler::assemble_jr() {
  bookmarks.push_back(block_code->size());
  emit(JR_Z_R8(0));
}

void Assembler::patch_jr() {
  size_t dst = block_code->size();
  size_t src = bookmarks.back();

  (*block_code)[src + 1] = uint8_t(dst - src - 2);

  bookmarks.pop_back();
}

//-----------------------------------------------------------------------------

void Assembler::clear_oam() {
	emit(LD_A_D8(0));
  emit(LD_B_D8(0));

  //emit(LD_HL_D16(uint16_t(0xFE00)));
  emit(0x21);
  emit(0x00);
  emit(0xFE);

  size_t dst = block_code->size();

  emit(LD_HLP_A);
  emit(DEC_B);

  size_t src = block_code->size();

  emit(JR_NZ_D8(dst - src - 2));
}

void Assembler::lcd_off_unsafe() {
  emit(LD_A_D8(0));
  emit(LDH_A8_A(0x40));
}

void Assembler::lcd_on_sprites() {
  emit(LD_A_D8(0x93));
  emit(LDH_A8_A(0x40));
}

void Assembler::scx(int x) {
  emit(LD_A_D8(x));
  emit(LDH_A8_A(0x43));
}

void Assembler::load_sprite(int i, int y, int x, int p, int f) {
  emit(LD_HL_D16(0xFE00 + 4 * i));

  emit(LD_A_D8(y));
  emit(LD_HLP_A);

  emit(LD_A_D8(x));
  emit(LD_HLP_A);

  emit(LD_A_D8(p));
  emit(LD_HLP_A);

  emit(LD_A_D8(f));
  emit(LD_HLP_A);
}

void Assembler::test_finish() {
  bookmarks.push_back(block_code->size());
  emit(LDI_A16_A(0x8000));
  jr_bookmark();
}

void Assembler::test_finish_stat(int result) {
  emit(LDH_A_A8(0x41));
  emit(CP_D8(result));
  assemble_jr();
  emit(LD_A_D8(0xFF));
  test_finish();
  patch_jr();
  emit(LD_A_D8(0x55));
  test_finish();
}

// Cart that just loops writing 0xFF to vram
blob Assembler::create_dummy_cart() {
  const char* app = R"(
  0150:
    ld a, $FF
    ld hl, $8000
    ld (hl), a
    inc l
    jr -4
  )";

  Assembler as;
  as.assemble(app);
  return as.link();
}

//-----------------------------------------------------------------------------
