#include "GateBoyApp/TraceViewer.h"

#if 0

#include <vector>
#include <cstddef>
#include <float.h>

void TraceViewer::render(void* blob, int size, int count, const std::vector<SignalData>& signals, double center, double span) {

  double tmin = center - span / 2;
  double tmax = center + span / 2;

  for (int i = 0; i < 1024; i++) {
    double t = double(i) / 1024.0f;

    t *= (tmax - tmin);
    t += tmin;

    int s = (int)floor(t);
    if (s < 0) continue;
    if (s >= count) continue;

    uint8_t* base = (uint8_t*)blob + (s * size);
    render_signals(signals, base, screen_x + i, screen_y);
  }
}

int TraceViewer::render_signals(const std::vector<SignalData>& signals, void* blob, int x, int y) {
  int cx = x;
  int cy = y;

  for (const SignalData& s : signals) {
    uint8_t* src = (uint8_t*)blob + s.byte_offset;

    if (!s.children.empty()) {
      cy += 12;
      cy += render_signals(s.children, src, cx, cy);
    }
    else {
      if (s.bit_width == 0) {
        cy += 12;
      }
      else {
        for (int bit = s.bit_offset; bit < (s.bit_offset + s.bit_width); bit++) {
          int b = (src[bit >> 3] >> (bit & 7)) & 1;

          for (int by = 0; by < 11; by++) {
            framebuffer[(cx + 0) + (cy + by) * fb_width] = b ? 0xFFCCCC99 : 0xFF484365;
          }
          cy += 12;
        }
      }
    }

  }

  return cy - y;
}


#endif













#if 0


void plot_signal(int x, int y, void* d, std::vector<SignalMap>& map) {
  for (int field = 0; field < map.size(); field++) {
    SignalMap sig = map[field];
    uint8_t* src = (uint8_t*)d + sig.offset;
    for (int bit = 0; bit < sig.width; bit++) {
      int b = (src[bit >> 3] >> (bit & 7)) & 1;

      for (int by = 0; by < 12; by++) {
        for (int bx = 0; bx < 6; bx++) {
          framebuffer[x + bx + y * fb_width] = b ? 0xFFCCCC99 : 0xFF484365;
        }
        y++;
      }
    }
  }
}

void draw_labels(int x, int y, std::vector<SignalMap>& map) {
  for (int field = 0; field < map.size(); field++) {
    SignalMap sig = map[field];
    qp.render_text(x, y, sig.name);

    y += sig.width * 12;
  }
}


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

const char* trace_0101 = 
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

const char* trace_0001 = 
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

const char* trace_0100 = 
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

//-----------------------------------------------------------------------------

const char* trace_0010 = 
"            "
"            "
"  ....####  "
"  .....###  "
"  ......##  "
"  .......#  "
"  .......#  "
"  ......##  "
"  .....###  "
"  ....####  "
"            "
"            ";

const char* trace_0111 = 
"            "
"            "
"  ....####  "
"  .....###  "
"  ......##  "
"  .......#  "
"  .......#  "
"  ......##  "
"  .....###  "
"  ....####  "
"            "
"            ";

const char* trace_0011 = 
"            "
"            "
"  ........  "
"  ........  "
"  ........  "
"  .......#  "
"  ......##  "
"  .....###  "
"  ....####  "
"  ...#####  "
"            "
"            ";

const char* trace_0110 = 
"            "
"            "
"  ...#####  "
"  ....####  "
"  .....###  "
"  ......##  "
"  .......#  "
"  ........  "
"  ........  "
"  ........  "
"            "
"            ";

//-----------------------------------------------------------------------------

const char* trace_1000 = 
"            "
"            "
"  ####....  "
"  #####...  "
"  ######..  "
"  #######.  "
"  #######.  "
"  ######..  "
"  #####...  "
"  ####....  "
"            "
"            ";

const char* trace_1101 = 
"            "
"            "
"  ####....  "
"  #####...  "
"  ######..  "
"  #######.  "
"  #######.  "
"  ######..  "
"  #####...  "
"  ####....  "
"            "
"            ";

const char* trace_1001 = 
"            "
"            "
"  ########  "
"  ########  "
"  ########  "
"  #######.  "
"  ######..  "
"  #####...  "
"  ####....  "
"  ###.....  "
"            "
"            ";

const char* trace_1100 = 
"            "
"            "
"  ###.....  "
"  ####....  "
"  #####...  "
"  ######..  "
"  #######.  "
"  ########  "
"  ########  "
"  ########  "
"            "
"            ";

//-----------------------------------------------------------------------------

const char* trace_1010 = 
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

const char* trace_1111 = 
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

const char* trace_1011 = 
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

const char* trace_1110 = 
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

void TextPainter::trace_qd(const char* name, const reg& q, const reg& d) {
  int icon_width  = 12;
  int icon_height = 12;

  int dst_x = trace_x + (col * 1 + 0) * icon_width ;
  int dst_y = trace_y + row * icon_height;

  if (clock == 0) {
    render_text(dst_x - (6 * 12), dst_y, name);
  }

  int t = (q.q() << 3) | (q.c() << 2) | (d.q() << 1) | (d.c() << 0);

  const char* src = trace_icons[t];
  draw_icon(framebuffer, fb_width,
    src, icon_width, icon_height,
    dst_x, dst_y,
    col == highlight_col);
  row++;
}

#endif
