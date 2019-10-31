#include "TextPainter.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

static const int glyph_width = 6;
static const int glyph_height = 12;
static const int glyph_stride = 256;

static const int console_width = 80;
static const int console_height = 10;

TextPainter::TextPainter() {
  SDL_Surface* terminus_surface = SDL_LoadBMP("terminus2.bmp");
  font = (uint8_t*)terminus_surface->pixels;
}

void TextPainter::begin_frame(uint32_t* fb, int width, int height) {
  framebuffer = fb;
  fb_width = width;
  fb_height = height;
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