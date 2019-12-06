#include "Regs.h"

//-----------------------------------------------------------------------------

void Reg::reset() {
  val = 0;
  clk = 0;
}

void Reg::tick() {
}

void Reg::tock(bool clk_in, bool rst_in, bool reg_in) {
  if (!rst_in) val = 0;
  else if (!clk && clk_in) val = reg_in;
  clk = clk_in;
}

Reg::operator bool() const {
  return val;
}

//-----------------------------------------------------------------------------

void Reg8::reset() {
  val = 0;
  clk = 0;
}

void Reg8::tick() {
}

void Reg8::tock(bool clk_in, bool rst_in, uint8_t reg_in) {
  if (!rst_in) val = 0;
  else if (!clk && clk_in) val = reg_in;
  clk = clk_in;
}

Reg8::operator uint8_t() const {
  return val;
}

//-----------------------------------------------------------------------------

void RegSR::reset() {
  val = 0;
  clk = 0;
}

void RegSR::tick() {
}

void RegSR::tock(bool clk_in, bool set_in, bool rst_in, bool reg_in) {
  if (!rst_in) val = 0;
  else if (!set_in) val = 1;
  else if (!clk && clk_in) val = reg_in;
  clk = clk_in;
}

RegSR::operator bool() const {
  return val;
}

//-----------------------------------------------------------------------------

void Counter::reset() {
  val = 0;
  clk = 0;
}

void Counter::tick() {
}

void Counter::tock(bool clk_in, bool load_in, bool reg_in) {
  bool old_val = val;
  if (!load_in) val = reg_in;
  else if (!clk && clk_in) val = !old_val;
  clk = clk_in;
}

Counter::operator bool() const {
  return val;
}

//-----------------------------------------------------------------------------

void DuoReg::reset() {
  val = 0;
  clk = 0;
}

void DuoReg::tick() {
}

void DuoReg::tock(bool clk_in, bool rst_in, bool reg_in) {
  if (!rst_in) val = 0;
  else if (clk != clk_in) val = reg_in;
  clk = clk_in;
}

DuoReg::operator bool() const {
  return val;
}

//-----------------------------------------------------------------------------

bool Latch::latch(bool clk_in, bool latch_in) {
  if (clk_in) val = latch_in;
  return val;
}

Latch::operator bool() const {
  return val;
}

//-----------------------------------------------------------------------------
