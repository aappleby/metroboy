#include "Timer.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

void Timer::reset() {
  *this = {};

  counter = 10994 * 4 + 2;
  tima = 0x00;
  tma = 0x00;
  tac = 0xF8;
}

//-----------------------------------------------------------------------------

static const uint16_t masks[] = { 0x200, 0x08, 0x20, 0x80 };

Bus Timer::tick() const {
  return timer_to_bus;
}

bool Timer::get_interrupt() const {
  return do_interrupt;
}

void Timer::tock(int tcycle_, Bus bus_to_timer_) {
  tphase = tcycle_ & 3;
  bus_to_timer = bus_to_timer_;
  timer_to_bus = {};

  uint16_t tima_ = tima;
  bool tick_ = do_tick;

  if (tphase == 0 || tphase == 2) {
    counter++;
    counter++;
    tick_ = (counter & masks[tac & 3]) && (tac & 0b100);
    if (do_tick && !tick_) tima_ = tima + 1;
  }

  if (bus_to_timer.write) {
    if (bus_to_timer.addr == ADDR_DIV)  counter = 0;
    if (bus_to_timer.addr == ADDR_TIMA) tima_ = (uint8_t)bus_to_timer.data;
    if (bus_to_timer.addr == ADDR_TMA)  tma = (uint8_t)bus_to_timer.data;
    if (bus_to_timer.addr == ADDR_TAC)  tac = (uint8_t)bus_to_timer.data;
  }
  else if (bus_to_timer.read) {
    if (bus_to_timer.addr == ADDR_DIV)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = uint8_t(counter >> 8); }
    if (bus_to_timer.addr == ADDR_TIMA) { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = uint8_t(tima); }
    if (bus_to_timer.addr == ADDR_TMA)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = tma; }
    if (bus_to_timer.addr == ADDR_TAC)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = tac | 0b11111000; }
  }

  // should this go in tphase 0 now?
  do_interrupt = tima >> 8;
  if (do_interrupt) tima_ = tma;

  tima = tima_;
  do_tick = tick_;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d,   "CNT         0x%04x\n", counter);
  sprintf(d,   "DIV         0x%02x\n", (counter >> 8) & 0xFF);
  sprintf(d,   "TIMA        0x%02x\n", tima);
  sprintf(d,   "TMA         0x%02x\n", tma);
  sprintf(d,   "TAC         %s\n",     byte_to_bits(tac));
  sprintf(d,   "do_int      %d\n", do_interrupt);
  sprintf(d,   "\n");
  print_bus(d, "bus_to_tmr", bus_to_timer);
  print_bus(d, "tmr_to_bus", timer_to_bus);
}

//-----------------------------------------------------------------------------
// not sure if the mux2s are top=0 bot=1 or reverse...
// and level-triggered dtffs are probably wrong...

uint8_t HardTimer::tock(int tcycle, bool RESET2, uint16_t addr, uint8_t data, bool CPU_RD, bool CPU_WR) {
  bool BOGA1MHZ = tcycle & 0x02;
  bool CLK_256K = tcycle & 0x08;
  bool CLK_64K  = tcycle & 0x20;
  bool CLK_16K  = tcycle & 0x80;

  bool A2 = addr & 0x04;
  bool A1 = addr & 0x02;
  bool A0 = addr & 0x01;

  bool D7 = data & 0x80;
  bool D6 = data & 0x40;
  bool D5 = data & 0x20;
  bool D4 = data & 0x10;
  bool D3 = data & 0x08;
  bool D2 = data & 0x04;
  bool D1 = data & 0x02;
  bool D0 = data & 0x01;

  const bool FFXX   = (addr & 0xFF00) == 0xFF00;
  const bool A00_07 = (addr & 0x00FF) <= 0x0007;

  const bool RYFO = (A2 && A00_07 && FFXX);
  const bool FF04_FF07 = RYFO;

  const bool SARA = !(A0 && A1 && CPU_WR && FF04_FF07);
  const bool SORA = (FF04_FF07 && CPU_RD && A1 && A0);
  const bool UVYR = !CLK_64K;
  const bool UBOT = !CLK_256K;

  if (SARA) {
    SABO = RESET2 ? 0 : D2;
    SAMY = RESET2 ? 0 : D1;
    SOPU = RESET2 ? 0 : D0;
  }

  const bool SUPE = SORA ? SABO : 0;
  const bool ROTE = SORA ? SAMY : 0;
  const bool RYLA = SORA ? SOPU : 0;

  if (SORA) {
    D2 = SABO;
    D1 = SAMY;
    D0 = SOPU;
  }

  const bool UKAP = SOPU ? CLK_16K : UVYR;
  const bool TEKO = SOPU ? UBOT : !FF04_D1;
  const bool TECY = SAMY ? UKAP : TEKO;
    
  const bool SOGU = !(TECY || SABO);

  const bool TOPE = (A0 && TOLA_NA1 && CPU_WR && FF04_FF07);
  const bool MUZU = FROM_CPU5 | TOPE;
  const bool MEKE = !_INT_TIMER;
  const bool MEXU = !(MUZU && RESET2 && MEKE);

  const bool MULO = !RESET2;
  const bool TOVY = !A0;
  const bool TOVY_NA0 = TOVY;
  const bool TUBY = (FF04_FF07 && CPU_RD && A1 && TOVY);
  const bool TYJU = (TOVY && A1 && CPU_WR && FF04_FF07);

  if (TYJU) {
    PETO = D6;
    MURU = D2;
    NYKE = D1;
    SETA = D7;
    SABU = D0;
    TYRU = D4;
    SUFY = D5;
    TYVA = D3;
  }

  const bool REVA = TUBY ? PETO : 0;
  const bool NOLA = TUBY ? MURU : 0;
  const bool PYRE = TUBY ? NYKE : 0;
  const bool SAPU = TUBY ? SETA : 0;
  const bool SETE = TUBY ? SABU : 0;
  const bool SUPO = TUBY ? TYRU : 0;
  const bool SOTU = TUBY ? SUFY : 0;
  const bool SALU = TUBY ? TYVA : 0;

  if (TUBY) {
    D6 = REVA;
    D2 = NOLA;
    D1 = PYRE;
    D7 = SAPU;
    D0 = SETE;
    D4 = SUPO;
    D5 = SOTU;
    D3 = SALU;
  }

  const bool REFU = TOPE ? PETO : D6;
  const bool NYKU = TOPE ? MURU : D2;
  const bool PETU = TOPE ? NYKE : D1;
  const bool RATO = TOPE ? SETA : D7;
  const bool ROKE = TOPE ? SABU : D0;
  const bool SALA = TOPE ? TYRU : D4;
  const bool SYRU = TOPE ? SUFY : D5;
  const bool SOCE = TOPE ? TYVA : D3;

  const bool PYMA = !(REFU || MULO);
  const bool NADA = !(NYKU || MULO);
  const bool NERO = !(PETO || MULO);
  const bool PAGU = !(RATO || MULO);
  const bool PUXY = !(ROKE || MULO);
  const bool ROLU = !(SALA || MULO);
  const bool RUGY = !(SYRU || MULO);
  const bool REPA = !(SOCE || MULO);

  if (MEXU) {
    REGA = PUXY;
    POVY = NERO;
    PERU = NADA;
    RATE = REPA;
    RUBY = ROLU;
    RAGE = RUGY;
    PEDA = PYMA;
    NUGA = PAGU;
  }

  bool NUGA_COUT = 0;

  if (SOGU) {
    uint16_t x = (REGA << 0) | (POVY << 1) | (PERU << 2) | (RATE << 3) | (RUBY << 4) | (RAGE << 5) | (PEDA << 6) | (NUGA << 7);
    x++;
    REGA = x & 0x01;
    POVY = x & 0x02;
    PERU = x & 0x04;
    RATE = x & 0x08;
    RUBY = x & 0x10;
    RAGE = x & 0x20;
    PEDA = x & 0x40;
    NUGA = x & 0x80;
    NUGA_COUT = x & 0x100;
  }

  bool TEDA = (FF04_FF07 && CPU_RD && TOLA_NA1 && A0);

  const bool SOKU = TEDA ? REGA : 0;
  const bool RACY = TEDA ? POVY : 0;
  const bool RAVY = TEDA ? PERU : 0;
  const bool SOSY = TEDA ? RATE : 0;
  const bool SOMU = TEDA ? RUBY : 0;
  const bool SURO = TEDA ? RAGE : 0;
  const bool ROWU = TEDA ? PEDA : 0;
  const bool PUSO = TEDA ? NUGA : 0;

  if (TEDA) {
    D0 = SOKU;
    D1 = RACY;
    D2 = RAVY;
    D3 = SOSY;
    D4 = SOMU;
    D5 = SURO;
    D6 = ROWU;
    D7 = PUSO;
  }

  const bool MUGY = !MEXU;

  if (BOGA1MHZ) {
    NYDU = MUGY ? 0 : NUGA_COUT;
  }

  const bool MERY = !(!NYDU || NUGA_COUT);

  if (BOGA1MHZ) {
    MOBA = RESET2 ? 0 : MERY;
  }

  _INT_TIMER = MOBA;

  data = (D0 << 0) | (D1 << 1) | (D2 << 2) | (D3 << 3) | (D4 << 4) | (D5 << 5) | (D6 << 6) | (D7 << 7);
  return data;
}

//-----------------------------------------------------------------------------







/*
if (tphase == 0) {
counter = counter + 2;
old_tima = new_tima;

bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
if (tick && !new_tick) new_tima++;
tick = new_tick;
}

if (bus.write && bus.addr == ADDR_TIMA) new_tima = bus.data;
if (bus.write && bus.addr == ADDR_DIV) counter = 0;
if (bus.write && bus.addr == ADDR_TAC) tac = bus.data;
if (bus.write && bus.addr == ADDR_TMA) tma = bus.data;

if (tphase == 1) {
if (overflow) new_tima = tma;
overflow = (old_tima == 0xFF) && (new_tima == 0x00);

}

out.data = 0x00;
out.oe = false;
if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 7); }
if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }

out.overflow = overflow;

return out;
*/
