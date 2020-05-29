#pragma once
#include <stdint.h>
#include "TextPainter.h"

// lcd x/y, lyc match, stat mode & int

struct LCD {

  void tick();
  void tock();

  void dump_regs(TextPainter& text_painter);

  uint8_t X;
  uint8_t Y;

  uint8_t LY;
  uint8_t LYC;
  uint8_t STAT;
};