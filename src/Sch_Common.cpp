#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void dump_long(TextPainter& text_painter, const char* label, SignalState a) {
  text_painter.add_text(label);
  text_painter.add_char(a.hiz ? 3 : 1);
  text_painter.add_char(a.val ? '1' : '0');
  text_painter.add_char(1);
  text_painter.add_char(a.clk ? '^' : 'v');
  if (a.set)     text_painter.add_char(2, 'S');
  if (a.rst)     text_painter.add_char(3, 'R');
  //if (a.changed) text.add_char('D');
  if (a.error)   text_painter.add_char('E');
  text_painter.newline();
}

void dump2(TextPainter& text_painter, SignalState a) {
  text_painter.add_char(a.error ? 6 : a.hiz ? 3 : 2);
  text_painter.add_char(a.val ? '1' : '0');
}

void dump_pin(TextPainter& text_painter, SignalState a, SignalState d) {
  if (a.error || a.hiz) { text_painter.add_char(6, 'X'); return; }
  if (d.error || d.hiz) { text_painter.add_char(6, 'X'); return; }

  if (a.val == 0 && d.val == 0) text_painter.add_char(2, '1');
  if (a.val == 1 && d.val == 1) text_painter.add_char(2, '0');
  if (a.val == 1 && d.val == 0) text_painter.add_char(3, 'Z');
  if (a.val == 0 && d.val == 1) text_painter.add_char(6, 'X');
}

void dump_pin(TextPainter& text_painter, SignalState a, SignalState b, SignalState c, SignalState d) {
  if (a.error || a.hiz) { text_painter.add_char(6, 'X'); return; }
  if (b.error || b.hiz) { text_painter.add_char(6, 'X'); return; }
  if (c.error || c.hiz) { text_painter.add_char(6, 'X'); return; }
  if (d.error || d.hiz) { text_painter.add_char(6, 'X'); return; }

  if (b.val) {
    if (a.val == 0 && d.val == 0) text_painter.add_char(2, '1');
    if (a.val == 1 && d.val == 1) text_painter.add_char(2, '0');
    if (a.val == 1 && d.val == 0) text_painter.add_char(3, 'Z');
    if (a.val == 0 && d.val == 1) text_painter.add_char(6, 'X');
  }
  else {
    if (c.hiz == 0 && c.val == 0) text_painter.add_char(4, '0');
    if (c.hiz == 0 && c.val == 1) text_painter.add_char(4, '1');
    if (c.hiz == 1 && c.val == 0) text_painter.add_char(5, '0');
    if (c.hiz == 1 && c.val == 1) text_painter.add_char(5, '1');
  }
}

void dump(TextPainter& text_painter, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g) {
  text_painter.add_text(label);
  dump2(text_painter, g.a);
  dump2(text_painter, f.a);
  dump2(text_painter, e.a);
  dump2(text_painter, d.a);
  dump2(text_painter, c.a);
  dump2(text_painter, b.a);
  dump2(text_painter, a.a);
  uint8_t val = (uint8_t)pack(a, b, c, d, e, f, g);
  text_painter.dprintf("\1 0x%02x", val);
  text_painter.newline();
}

void dump(TextPainter& text_painter, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g, SignalBase h) {
  text_painter.add_text(label);
  dump2(text_painter, h.a);
  dump2(text_painter, g.a);
  dump2(text_painter, f.a);
  dump2(text_painter, e.a);
  dump2(text_painter, d.a);
  dump2(text_painter, c.a);
  dump2(text_painter, b.a);
  dump2(text_painter, a.a);
  uint8_t val = (uint8_t)pack(a, b, c, d, e, f, g, h);
  text_painter.add_char(1, a.a.clk ? '^' : 'v');
  if (a.a.set)     text_painter.add_char(2, 'S');
  if (a.a.rst)     text_painter.add_char(3, 'R');
  if (a.a.error)   text_painter.add_char(1, 'E');
  text_painter.dprintf("\1 0x%02x", val);
  text_painter.newline();
}

//-----------------------------------------------------------------------------

};