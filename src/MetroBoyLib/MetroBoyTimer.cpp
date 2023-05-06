#include "MetroBoyLib/MetroBoyTimer.h"
#include "metrolib/core/Constants.h"
#include <assert.h>

#include "metrolib/core/Dumper.h"
#include "metrolib/gameboy/MetroBoyCPU.h"
#include "metrolib/gameboy/GBConstants.h"

// Passes mooneye timer tests

static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };

//-----------------------------------------------------------------------------

void MetroBoyTimer::reset() {
  div  = 0xEAF2;
  tima = 0;
  tma  = 0;
  tac  = 0;

  tima_clk = 0;
  tima_7_sync = 0;
  timer_int = 0;
}

//-----------------------------------------------------------------------------

void MetroBoyTimer::tick(int phase_total, const Req& req, Ack& ack) {
  (void)phase_total;

  if (req.read) switch(req.addr) {
  case ADDR_DIV:  ack.read++; ack.data_lo += uint8_t(div >> 6); break;
  case ADDR_TIMA: ack.read++; ack.data_lo += tima;              break;
  case ADDR_TMA:  ack.read++; ack.data_lo += tma;               break;
  case ADDR_TAC:  ack.read++; ack.data_lo += tac | 0b11111000;  break;
  };
}

//-----------------------------------------------------------------------------
// Timer interrupt fires when the high bit of tima (after sync with phase C)
// goes low. Writing to tima clears the synchronized bit for some reason.

void MetroBoyTimer::tock(int phase_total, const Req& req) {

  if (MB_DELTA_CD) {
    div++;
    timer_int = tima_7_sync && !(tima & 0x80);
    tima_7_sync = (tima & 0x80);
    update_tima();
  }

  if (MB_DELTA_FG && req.write) {
    switch(req.addr) {
    case ADDR_DIV:  div  = 0; break;
    case ADDR_TIMA: tima = req.data_lo; tima_7_sync = false; break;
    case ADDR_TMA:  tma  = req.data_lo; break;
    case ADDR_TAC:  tac  = req.data_lo; break;
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

void MetroBoyTimer::update_tima() {
  bool tima_clk_ = (div & masks[tac & 3]) && (tac & TAC_RUN);
  if (tima_clk && !tima_clk_) tima++;
  tima_clk = tima_clk_;

  if (timer_int) {
    tima = tma;
    tima_7_sync = 0;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyTimer::dump(Dumper& d) const {
  d("\002--------------TIMER2------------\001\n");
  d("cnt         0x%04x\n", div);
  d("div         0x%02x\n", uint8_t(div >> 6));
  d("tima        0x%02x\n", tima);
  d("tma         0x%02x\n", tma);
  d("tac         %s\n",     byte_to_bits(tac));
  d("tima_7_sync %d\n",     tima_7_sync);
  d("timer_int        %d\n",     timer_int);
  d("tima_clk    %d\n",     tima_clk);
  d("\n");
}

//-----------------------------------------------------------------------------
