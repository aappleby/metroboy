#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void dump_long(TextPainter& text, const char* label, SignalState a) {
  text.add_text(label);
  text.add_char(a.hiz ? 3 : 1);
  text.add_char(a.val ? '1' : '0');
  text.add_char(1);
  text.add_char(a.clk ? '^' : 'v');
  if (a.set)     text.add_char(2, 'S');
  if (a.rst)     text.add_char(3, 'R');
  if (a.carry)   text.add_char('+');
  //if (a.changed) text.add_char('D');
  if (a.error)   text.add_char('E');
  text.newline();
}

void dump2(TextPainter& text, SignalState a) {
  text.add_char(a.error ? 6 : a.hiz ? 3 : 2);
  text.add_char(a.val ? '1' : '0');
}

void dump_pin(TextPainter& text, SignalState a, SignalState d) {
  if (a.error || a.hiz) { text.add_char(6, 'X'); return; }
  if (d.error || d.hiz) { text.add_char(6, 'X'); return; }

  if (a.val == 0 && d.val == 0) text.add_char(2, '1');
  if (a.val == 1 && d.val == 1) text.add_char(2, '0');
  if (a.val == 1 && d.val == 0) text.add_char(3, 'Z');
  if (a.val == 0 && d.val == 1) text.add_char(6, 'X');
}

void dump_pin(TextPainter& text, SignalState a, SignalState b, SignalState c, SignalState d) {
  if (a.error || a.hiz) { text.add_char(6, 'X'); return; }
  if (b.error || b.hiz) { text.add_char(6, 'X'); return; }
  if (c.error || c.hiz) { text.add_char(6, 'X'); return; }
  if (d.error || d.hiz) { text.add_char(6, 'X'); return; }

  if (b.val) {
    if (a.val == 0 && d.val == 0) text.add_char(2, '1');
    if (a.val == 1 && d.val == 1) text.add_char(2, '0');
    if (a.val == 1 && d.val == 0) text.add_char(3, 'Z');
    if (a.val == 0 && d.val == 1) text.add_char(6, 'X');
  }
  else {
    if (c.hiz == 0 && c.val == 0) text.add_char(4, '0');
    if (c.hiz == 0 && c.val == 1) text.add_char(4, '1');
    if (c.hiz == 1 && c.val == 0) text.add_char(5, '0');
    if (c.hiz == 1 && c.val == 1) text.add_char(5, '1');
  }
}

void dump(TextPainter& text, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g) {
  text.add_text(label);
  dump2(text, g.a);
  dump2(text, f.a);
  dump2(text, e.a);
  dump2(text, d.a);
  dump2(text, c.a);
  dump2(text, b.a);
  dump2(text, a.a);
  uint8_t val = (uint8_t)pack(a, b, c, d, e, f, g);
  text.dprintf("\1 0x%02x", val);
  text.newline();
}

void dump(TextPainter& text, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g, SignalBase h) {
  text.add_text(label);
  dump2(text, h.a);
  dump2(text, g.a);
  dump2(text, f.a);
  dump2(text, e.a);
  dump2(text, d.a);
  dump2(text, c.a);
  dump2(text, b.a);
  dump2(text, a.a);
  uint8_t val = (uint8_t)pack(a, b, c, d, e, f, g, h);
  text.add_char(1, a.a.clk ? '^' : 'v');
  if (a.a.set)     text.add_char(2, 'S');
  if (a.a.rst)     text.add_char(3, 'R');
  if (a.a.carry)   text.add_char(1, '+');
  if (a.a.error)   text.add_char(1, 'E');
  text.dprintf("\1 0x%02x", val);
  text.newline();
}

//-----------------------------------------------------------------------------

};