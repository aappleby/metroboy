#include "TextPainter.h"

//-----------------------------------------------------------------------------

uint32_t pal(char c) {
  if (c == '.') return 0xFF383355 + 0x00101010;
  if (c == '|') return 0x00000000;
  if (c == '#') return 0xFFCCCC99;

  return 0x00000000;
}


void draw_icon(uint32_t* framebuffer, int fb_width,
               const char* icon, int icon_width, int icon_height,
               int dst_x, int dst_y, bool highlight) {
  //dst_x *= 2;
  //dst_y *= 2;
  for (int y = 0; y < icon_height; y++) {
    for (int x = 0; x < icon_width; x++) {
      char p = icon[x + y * icon_width];

      uint32_t c2 = pal(p);

      //if (x == 0 && c2 == 0)              c2 = 0x00335533;
      //if (x == icon_width - 1 && c2 == 0) c2 = 0x00335533;

      if (c2) {
        if (highlight) c2 += 0x00303030;
        framebuffer[(x * 1 + 0 + dst_x) + (y * 1 + 0 + dst_y) * fb_width] = c2;
        //framebuffer[(x * 2 + 0 + dst_x) + (y * 2 + 0 + dst_y) * fb_width] = c2;
        //framebuffer[(x * 2 + 1 + dst_x) + (y * 2 + 0 + dst_y) * fb_width] = c2;
        //framebuffer[(x * 2 + 0 + dst_x) + (y * 2 + 1 + dst_y) * fb_width] = c2;
        //framebuffer[(x * 2 + 1 + dst_x) + (y * 2 + 1 + dst_y) * fb_width] = c2;
      }
    }
  }
}

const char* trace_0000 = 
"            "
"            "
"............"
"............"
"............"
"............"
"............"
"............"
"............"
"............"
"            "
"            ";

const char* trace_0101 = 
"            "
"            "
"............"
"............"
"............"
"............"
"............"
"............"
"............"
"............"
"            "
"            ";

const char* trace_0001 = 
"            "
"            "
"........|||."
".......|||.."
"......|||..."
".....|||...."
"....|||....."
"...|||......"
"..|||......."
".|||........"
"            "
"            ";

const char* trace_0100 = 
"            "
"            "
".|||........"
"..|||......."
"...|||......"
"....|||....."
".....|||...."
"......|||..."
".......|||.."
"........|||."
"            "
"            ";

//-----------------------------------------------------------------------------

const char* trace_0010 = 
"            "
"            "
"......######"
".......#####"
"........####"
".........###"
".........###"
"........####"
".......#####"
"......######"
"            "
"            ";

const char* trace_0111 = 
"            "
"            "
"......######"
".......#####"
"........####"
".........###"
".........###"
"........####"
".......#####"
"......######"
"            "
"            ";

const char* trace_0011 = 
"            "
"            "
"........|||#"
".......|||##"
"......|||###"
".....|||####"
"....|||#####"
"...|||######"
"..|||#######"
".|||########"
"            "
"            ";

const char* trace_0110 = 
"            "
"            "
".|||########"
"..|||#######"
"...|||######"
"....|||#####"
".....|||####"
"......|||###"
".......|||##"
"........|||#"
"            "
"            ";

//-----------------------------------------------------------------------------

const char* trace_1000 = 
"            "
"            "
"######......"
"#######....."
"########...."
"#########..."
"#########..."
"########...."
"#######....."
"######......"
"            "
"            ";

const char* trace_1101 = 
"            "
"            "
"######......"
"#######....."
"########...."
"#########..."
"#########..."
"########...."
"#######....."
"######......"
"            "
"            ";

const char* trace_1001 = 
"            "
"            "
"########|||."
"#######|||.."
"######|||..."
"#####|||...."
"####|||....."
"###|||......"
"##|||......."
"#|||........"
"            "
"            ";

const char* trace_1100 = 
"            "
"            "
"#|||........"
"##|||......."
"###|||......"
"####|||....."
"#####|||...."
"######|||..."
"#######|||.."
"########|||."
"            "
"            ";

//-----------------------------------------------------------------------------

const char* trace_1010 = 
"            "
"            "
"############"
"############"
"############"
"############"
"############"
"############"
"############"
"############"
"            "
"            ";

const char* trace_1111 = 
"            "
"            "
"############"
"############"
"############"
"############"
"############"
"############"
"############"
"############"
"            "
"            ";

const char* trace_1011 = 
"            "
"            "
"########|||#"
"#######|||##"
"######|||###"
"#####|||####"
"####|||#####"
"###|||######"
"##|||#######"
"#|||########"
"            "
"            ";

const char* trace_1110 = 
"            "
"            "
"#|||########"
"##|||#######"
"###|||######"
"####|||#####"
"#####|||####"
"######|||###"
"#######|||##"
"########|||#"
"            "
"            ";

//-----------------------------------------------------------------------------


const char* trace_dot_hi = 
"            "
"            "
"  ########  "
"  ########  "
"  ########  "
"  ########  "
"  ########  "
"  ########  "
"  ########  "
"  ########  "
"            "
"            ";

const char* trace_dot_lo = 
"            "
"            "
"  ........  "
"  ........  "
"  ........  "
"  ........  "
"  ........  "
"  ........  "
"  ........  "
"  ........  "
"            "
"            ";









/*
const char* trace_dot_hi = 
"            "
"     #      "
"    ###     "
"   #####    "
"  #######   "
" #########  "
"  #######   "
"   #####    "
"    ###     "
"     #      "
"            "
"            ";

const char* trace_dot_lo = 
"            "
"     .      "
"    ...     "
"   .....    "
"  .......   "
" .........  "
"  .......   "
"   .....    "
"    ...     "
"     .      "
"            "
"            ";
*/

const char* trace_icons[16] = {
  trace_0000,
  trace_0001,
  trace_0010,
  trace_0011,
  trace_0100,
  trace_0101,
  trace_0110,
  trace_0111,
  trace_1000,
  trace_1001,
  trace_1010,
  trace_1011,
  trace_1100,
  trace_1101,
  trace_1110,
  trace_1111,
};

void TextPainter::trace_q(const char* name, bool q) {
  int icon_width  = 12;
  int icon_height = 12;

  int dst_x = trace_x + (col * 1 + 0) * icon_width ;
  int dst_y = trace_y + row * icon_height;

  if (clock == 0) {
    render_text(dst_x - (6 * 12), dst_y, name);
  }

  const char* src_q = q ? trace_dot_hi : trace_dot_lo;

  draw_icon(framebuffer, fb_width,
            src_q, icon_width, icon_height,
            dst_x, dst_y,
            col == highlight_col);
  row++;
}

void TextPainter::trace_s(const char* name, bool s) {
  int icon_width  = 12;
  int icon_height = 12;

  int dst_x = trace_x + (col * 1 + 0) * icon_width ;
  int dst_y = trace_y + row * icon_height;

  if (clock == 0) {
    int x = trace_x + col * 3 * icon_width;
    render_text(x - (6 * 12), dst_y, name);
  }

  const char* src_s = s ? trace_dot_hi : trace_dot_lo;

  draw_icon(framebuffer, fb_width,
    src_s, icon_width, icon_height,
    dst_x, dst_y,
    col == highlight_col);

  row++;
}

void TextPainter::trace_d(const char* name, bool d) {
  int icon_width  = 12;
  int icon_height = 12;

  int dst_x = trace_x + (col * 1 + 0) * icon_width ;
  int dst_y = trace_y + row * icon_height;

  if (clock == 0) {
    int x = trace_x + col * 3 * icon_width;
    render_text(x - (6 * 12), dst_y, name);
  }

  const char* src_d = d ? trace_dot_hi : trace_dot_lo;

  draw_icon(framebuffer, fb_width,
    src_d, icon_width, icon_height,
    dst_x, dst_y,
    col == highlight_col);

  row++;
}

void TextPainter::trace_qd(const char* name, bool q, bool qc, bool d, bool dc) {
  int icon_width  = 12;
  int icon_height = 12;

  int dst_x = trace_x + (col * 1 + 0) * icon_width ;
  int dst_y = trace_y + row * icon_height;

  if (clock == 0) {
    render_text(dst_x - (6 * 12), dst_y, name);
  }

  //q = 0;
  //d = 0;

  int t = (q << 3) | (qc << 2) | (d << 1) | (dc << 0);

  const char* src = trace_icons[t];
  draw_icon(framebuffer, fb_width,
    src, icon_width, icon_height,
    dst_x, dst_y,
    col == highlight_col);
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