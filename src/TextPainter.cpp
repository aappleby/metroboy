#include "TextPainter.h"

//-----------------------------------------------------------------------------

// const int glyph_width = 6;
// const int glyph_height = 12;

//   012345
// 0 
// 1 
// 2 .
// 3 .
// 4 .
// 5 .
// 6 .
// 7 .
// 8 .
// 9 ######
// A
// B

//   012345
// 0 
// 1 
// 2 ######
// 3 #.....
// 4 #.....
// 5 #.....
// 6 #.....
// 7 #.....
// 8 #.....
// 9 #.....
// A
// B

const char* trace_hi = 
"      "
"|#####"
"|#####"
"|#####"
"|#####"
"|#####"
"|#####"
"|#####"
"|#####"
"|#####"
"......"
"      ";

const char* trace_lo = 
"      "
"|     "
"|     "
"|     "
"|     "
"|     "
"|     "
"|     "
"|     "
"|     "
"......"
"      ";

void TextPainter::trace(const char* name, bool c) {
  int dst_x = trace_x + col * glyph_width;
  int dst_y = trace_y + row * glyph_height;

  if (clock == 0) {
    render_text(dst_x - (6 * 12), dst_y, name);
  }


  int dst_cursor = dst_x + dst_y * fb_width;

  const char* src = c ? trace_hi : trace_lo;

  for (int y = 0; y < glyph_height; y++) {
    for (int x = 0; x < glyph_width; x++) {
      char p = src[x + y * glyph_width];

      uint32_t c2 = 0x00000000;

      if (p == ' ') c2 = 0x00000000;
      if (p == '|') c2 = 0xFF444444;
      if (p == '.') c2 = 0xFF666666;
      if (p == '#') c2 = 0xFF888888;

      if (row == highlight_row) c2 += 0x00003000;
      if (col == highlight_col) c2 += 0x00404000;

      if (c2) {
        framebuffer[dst_cursor] = c2;
      }
      dst_cursor++;
    }
    dst_cursor += (fb_width - glyph_width);
  }
  row++;
}

void TextPainter::render_glyph(int dst_x, int dst_y, char c, uint32_t color) {
  int glyph_row = (c >> 5) * 16 + 2;
  int glyph_col = (c & 31) * 8;
  int src_cursor = glyph_col + (glyph_row * glyph_stride);
  int dst_cursor = dst_x + dst_y * fb_width;

  for (int y = 0; y < glyph_height; y++) {
    for (int x = 0; x < glyph_width; x++) {
      if (font[src_cursor]) framebuffer[dst_cursor] = color;
      src_cursor++;
      dst_cursor++;
    }
    src_cursor += (glyph_stride - glyph_width);
    dst_cursor += (fb_width - glyph_width);
  }
}

void TextPainter::render_text(int dst_x, int dst_y, const char* text) {
  int xcursor = 0;
  int ycursor = 0;

  uint32_t color = 0xFFCCCCCC;

  for (const char* c = text; *c; c++) {
    switch(*c) {
    case 1: color = 0xFFCCCCCC; continue;
    case 2: color = 0xFFFF8888; continue;
    case 3: color = 0xFF88FF88; continue;
    case 4: color = 0xFF8888FF; continue;
    case 5: color = 0xFF88FFFF; continue;
    case 6: color = 0xFFFF88FF; continue;
    case 7: color = 0xFFFFFF88; continue;
    case '\n':
      xcursor = 0;
      ycursor += glyph_height;
      continue;
    default:
      break;
    }

    render_glyph(dst_x + xcursor, dst_y + ycursor, *c, color);

    xcursor += glyph_width;
    if (xcursor > (fb_width - glyph_width)) {
      xcursor = 0;
      ycursor += glyph_height;
    }
  }
}

//-----------------------------------------------------------------------------

void TextPainter::draw_bbox(int sx, int sy, int w, int h, uint32_t color) {
  int ax = sx;
  int bx = sx + w;
  int ay = sy;
  int by = sy + h;
  int x, y;

  for (x = ax, y = ay; x <= bx; x++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = ax, y = by; x <= bx; x++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = ax, y = ay + 1; y <= by - 1; y++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = bx, y = ay + 1; y <= by - 1; y++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }
}



#if 0
//-----------------------------------------------------------------------------
// Console

void MetroBoyApp::render_console(int sx, int sy, uint8_t* font) {

  for (int cy = 0; cy < console_height; cy++) {
    char* line = &console_buf[(((cy + cursor_y)) % console_height) * console_width];

    for (int cx = 0; cx < console_width; cx++) {
      char c = *line;
      int row = (c >> 5) * 16 + 3;
      int col = (c & 31) * 8;

      int src_cursor = col + (row * glyph_stride);
      int dst_cursor = (sx + (cx * glyph_width)) + (sy + (cy * glyph_height)) * fb_width;

      for (int y = 0; y < glyph_height; y++) {
        for (int x = 0; x < glyph_width; x++) {
          if (font[src_cursor]) framebuffer[dst_cursor] = 0xFF00FF00;
          src_cursor++;
          dst_cursor++;
        }
        src_cursor += (glyph_stride - glyph_width);
        dst_cursor += (fb_width - glyph_width);
      }

      line++;
    }
  }

  draw_bbox(sx - 2, sy - 2, console_width * glyph_width + 4, console_height * glyph_height + 4, 0xFF00FF00);
}

#endif