#include "Timer.h"
#include "Constants.h"
#include <assert.h>

// Passes mooneye timer tests

static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };

//-----------------------------------------------------------------------------

void NewTimer::reset() {
  div  = 10995;
  tima = 0;
  tma  = 0;
  tac  = 0;

  tima_clk = 0;
  tima_7_sync = 0;
  timer_int = 0;
}

//-----------------------------------------------------------------------------

void NewTimer::tick(const Req& req, Ack& ack) {
  if (req.read) switch(req.addr) {
  case ADDR_DIV:  ack.read++; ack.data += uint8_t(div >> 6); break;
  case ADDR_TIMA: ack.read++; ack.data += tima;              break;
  case ADDR_TMA:  ack.read++; ack.data += tma;               break;
  case ADDR_TAC:  ack.read++; ack.data += tac | 0b11111000;  break;
  };
}

//-----------------------------------------------------------------------------
// Timer interrupt fires when the high bit of tima (after sync with phase C)
// goes low. Writing to tima clears the synchronized bit for some reason.

void NewTimer::tock(int phase, const Req& req) {

  if (PHASE_C) {
    div++;
    timer_int = tima_7_sync && !(tima & 0x80);
    tima_7_sync = (tima & 0x80);
    update_tima();
  }

  if (PHASE_F && req.write) {
    switch(req.addr) {
    case ADDR_DIV:  div  = 0; break;
    case ADDR_TIMA: tima = req.data; tima_7_sync = false; break;
    case ADDR_TMA:  tma  = req.data; break;
    case ADDR_TAC:  tac  = req.data; break;
    }
    update_tima();
  }

  // In the circuit, tima_7_sync is reset whenever timer_int is high,
  // presumably to stop the interrupt from retriggering until the cpu acks the
  // interrupt. But that doesn't actually make sense, as timer_int is set and
  // tima_7_sync is cleared at the same time and the interrupt is only high
  // for one mcycle (c to c).
  if (timer_int) tima_7_sync = 0;
}

//-----------------------------------------------------------------------------
// tima is "clocked" off a signal derived from div and tac.

void NewTimer::update_tima() {
  bool tima_clk_ = (div & masks[tac & 3]) && (tac & TAC_RUN);
  if (tima_clk && !tima_clk_) tima++;
  tima_clk = tima_clk_;

  if (timer_int) {
    tima = tma;
    tima_7_sync = 0;
  }
}

//-----------------------------------------------------------------------------

void NewTimer::dump(std::string& d) {
  sprintf(d, "\002--------------TIMER2------------\001\n");
  sprintf(d, "cnt         0x%04x\n", div);
  sprintf(d, "div         0x%02x\n", uint8_t(div >> 6));
  sprintf(d, "tima        0x%02x\n", tima);
  sprintf(d, "tma         0x%02x\n", tma);
  sprintf(d, "tac         %s\n",     byte_to_bits(tac));
  sprintf(d, "tima_7_sync %d\n",     tima_7_sync);
  sprintf(d, "timer_int        %d\n",     timer_int);
  sprintf(d, "tima_clk    %d\n",     tima_clk);

  sprintf(d, "\n");
}

//-----------------------------------------------------------------------------
