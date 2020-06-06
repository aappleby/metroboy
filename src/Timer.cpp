#include "Timer.h"
#include "Constants.h"
#include <assert.h>

#pragma warning(disable:4189)

static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };

//-----------------------------------------------------------------------------

void NewTimer::reset() {
  div  = 10995;
  tima = 0;
  tma  = 0;
  tac  = 0;

  tima_c = 0;
  tima_max = 0;
  tima_clk = 0;
  int_timer = 0;
}

bool NewTimer::get_interrupt() const {
  return int_timer;
}

void NewTimer::tick(int phase, const Req& req, Ack& ack) {
  bool read_en  = (0b11111111) & (1 << phase);
  bool write_en = (0b10001111) & (1 << phase); // ABCDxxxH

  if (read_en && req.read) switch(req.addr) {
  case ADDR_DIV:  /*ack.read++; ack.data += uint8_t(div >> 6);*/ break;
  case ADDR_TIMA: /*ack.read++; ack.data += tima;             */ break;
  case ADDR_TMA:  /*ack.read++; ack.data += tma;              */ break;
  case ADDR_TAC:  /*ack.read++; ack.data += tac | 0b11111000; */ break;
  };

  if (write_en && req.write) switch(req.addr) {
  case ADDR_DIV:  div = 0;                  break;
  case ADDR_TIMA: tima = uint8_t(req.data); break;
  case ADDR_TMA:  tma = uint8_t(req.data);  break;
  case ADDR_TAC:  tac = uint8_t(req.data);  break;
  }

  if (int_timer) {
    tima = tma;
    tima_max = 0;
  }

  update_tima();
}

void NewTimer::tock(int phase, const Req& req) {

  if (phase == 2) {
    div = div + 1;

    if (int_timer || (req.write && req.addr == 0xFF05)) {
      int_timer = 0;
    }
    else {
      int_timer = tima_max && !tima_c;
    }

    tima_max = tima_c;

    update_tima();
  }
}

void NewTimer::update_tima() {
  bool old_clk = tima_clk;
  bool new_clk = (div & masks[tac & 3]) && (tac & TAC_RUN);

  if (old_clk && !new_clk) {
    tima_c = (tima == 0xFF);
    tima = tima + 1;
  }

  tima_clk = new_clk;
}

























































//-----------------------------------------------------------------------------

void Timer::reset() {
  counter = 10995; // only one value will pass poweron_004_div and poweron_005_div
  old_tima = 0;
  new_tima = 0;
  tma = 0;
  tac = 0;
  do_int = 0;
  do_tick = 0;

  //new_timer.reset();
}

//-----------------------------------------------------------------------------

bool Timer::get_interrupt() const {
  return do_int;
}

//-----------------------------------------------------------------------------

void Timer::tick(int phase, const Req& req, Ack& ack) {
  if (do_int) new_tima = tma;

  if (req.read) switch(req.addr) {
  case ADDR_DIV:  ack.read++; ack.data += uint8_t(counter >> 6); break;
  case ADDR_TIMA: ack.read++; ack.data += new_tima;              break;
  case ADDR_TMA:  ack.read++; ack.data += tma;                   break;
  case ADDR_TAC:  ack.read++; ack.data += tac | 0b11111000;      break;
  };

  //new_timer.tick(phase, req, ack);
}

//----------

void Timer::tock(int phase, const Req& req) {
  if (phase == 2) 
  {
    do_int = (old_tima == 0xFF) && (new_tima == 0x00);

    counter = counter + 1;

    bool do_tick_ = (counter & masks[tac & 3]) && (tac & TAC_RUN);
    if (do_tick && !do_tick_) {
      old_tima = new_tima;
      new_tima = old_tima + 1;
    }
    do_tick = do_tick_;
  }

  bool read_en  = (0b11111111) & (1 << phase);
  bool write_en = (0b10001111) & (1 << phase); // ABCDxxxH

  if (write_en && req.write) switch(req.addr) {
  case ADDR_DIV:  counter = 0;                  break;
  case ADDR_TIMA: new_tima = uint8_t(req.data); break;
  case ADDR_TMA:  tma = uint8_t(req.data);      break;
  case ADDR_TAC:  tac = uint8_t(req.data);      break;
  }

  //new_timer.tock(phase, req);
}

//-----------------------------------------------------------------------------

/*
void Timer::reg_test() {
  if (new_timer.div  != counter) {
    printf("div  mismatch: %d %d\n", new_timer.div,  counter);
  }
  
  if (new_timer.tima != new_tima) {
    printf("tima mismatch: %d %d\n", new_timer.tima, new_tima);
  }

  if (new_timer.tima_clk != do_tick) {
    printf("tick mismatch: %d %d\n", new_timer.tima_clk, do_tick);
  }
  
  //if (new_timer.tma  != tma)      printf("tma  mismatch: %d %d\n", new_timer.tma,  tma);
  //if (new_timer.tac  != tac)      printf("tac  mismatch: %d %d\n", new_timer.tac,  tac);
}
*/

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d, "\002--------------TIMER------------\001\n");
  sprintf(d, "CNT         0x%04x\n", counter);
  sprintf(d, "DIV         0x%02x\n", uint8_t(counter >> 6));
  sprintf(d, "tima        0x%02x\n", new_tima);
  sprintf(d, "tma         0x%02x\n", tma);
  sprintf(d, "TAC         %s\n",     byte_to_bits(tac));
  sprintf(d, "DO_INT      %d\n",     do_int);
  sprintf(d, "DO_TICK     %d\n",     do_tick);
  sprintf(d, "\n");
}

//-----------------------------------------------------------------------------



#if 0

{
  on posedge(BOGA_AxCDEFGH) {
    div <= div + 1; // so this is on phase c? hmm....

    if (int_timer || FF05_WR) {
      int_timer <= 0;
    }
    else {
      int_timer <= and(tima_max, !tima_c);
    }

    tima_max <= tima_c;

    on posedge(tima_clk) {
      tima   <= tima + 1;
      tima_c <= tima == 0xFF;
    }
  }


  if (int_timer) {
    tima = tma;
    tima_max = 0;
  }

  if (FF04_WR) div  = 0;
  if (FF05_WR) tima = dbus;
  if (FF05_WR) tima_max = 0;
  if (FF06_WR) tma  = dbus;
  if (FF07_WR) tac  = dbus;

  if (FF04_RD) dbus = div[13:6];
  if (FF05_RD) dbus = tima;
  if (FF06_RD) dbus = tma;
  if (FF07_RD) dbus = tac;
}

#endif