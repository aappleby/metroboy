#pragma once

int disassemble(const uint8_t* code, uint16_t code_base, int code_cursor, int opcount, char* cursor);
int assemble(const char* source, uint8_t* code, int& out_size);
