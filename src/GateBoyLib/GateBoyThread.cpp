#include "GateBoyThread.h"

#include "GateBoyLib/GateBoy.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#ifdef _MSC_VER
#include <Windows.h>
void dump_thread_times() {
  uint64_t creation_time;
  uint64_t exit_time;
  uint64_t kernel_time;
  uint64_t user_time;
  GetThreadTimes(
    GetCurrentThread(),
    (LPFILETIME)&creation_time,
    (LPFILETIME)&exit_time,
    (LPFILETIME)&kernel_time,
    (LPFILETIME)&user_time);

  printf("times c %09lld e %09lld k %09lld u %09lld\n", creation_time, exit_time, kernel_time, user_time);
}
#endif

//------------------------------------------------------------------------------

GateBoyThread::GateBoyThread(IGateBoy* prototype) : gb(prototype)
{
  gb.reset_history();
  cart_blob = Assembler::create_dummy_cart();
}

//----------------------------------------

void GateBoyThread::start() {
  CHECK_P(main == nullptr);
  main = new std::thread([this] { thread_main(); });
}

//----------------------------------------

void GateBoyThread::stop() {
  if (!main) return;
  LOG_B("Stopping gateboy thread\n");

  sync.set(REQ_EXIT);
  main->join();

  LOG_B("Gateboy thread stopped\n");
  delete main;
}

//----------------------------------------

void GateBoyThread::pause() {
  if (!main) return;
  std::unique_lock<std::mutex> lock(sync.mut);
  sync.set(REQ_PAUSE);
  sync.cond.wait(lock, [this] { return sync.test(ACK_PAUSE); });
}

//----------------------------------------

void GateBoyThread::resume() {
  if (!main) return;
  sync.clear(REQ_PAUSE);
}

//----------------------------------------

void GateBoyThread::poweron(bool fastboot) {
  CHECK_P(sim_paused());
  clear_steps();
  gb.reset_history();
  gb->poweron(fastboot);
}

void GateBoyThread::reset() {
  CHECK_P(sim_paused());
  clear_steps();
  gb.reset_history();
  gb->reset();
}

//----------------------------------------

void GateBoyThread::add_steps(int64_t steps) {
  CHECK_P(sim_paused());
  gb.push();
  step_count += steps;
}

void GateBoyThread::run_to(uint64_t phase) {
  while(gb->get_sys().gb_phase_total_old != phase) {
    gb->next_phase(cart_blob);
  }
}

void GateBoyThread::rewind(int steps) {
  CHECK_P(sim_paused());
  clear_steps();
  while (steps--) {
    gb.pop();
  }
}

int64_t GateBoyThread::get_steps() const {
  return step_count;
}

void GateBoyThread::set_buttons(uint8_t buttons) {
  gb->set_buttons(buttons);
}

bool GateBoyThread::sim_paused() const {
  return (main == nullptr) || sync.test(ACK_PAUSE);
}

bool GateBoyThread::has_work() const {
  return step_count != 0;
}

blob& GateBoyThread::get_cart() {
  return cart_blob;
}

const blob& GateBoyThread::get_cart() const {
  return cart_blob;
}

//----------------------------------------

void GateBoyThread::clear_steps() {
  CHECK_P(sim_paused());
  step_count = 0;
  old_sim_time = 0;
  prev_phase_total = gb->get_sys().gb_phase_total_old;
  phase_rate_smooth = 0;
}

//------------------------------------------------------------------------------

void GateBoyThread::dump(Dumper& d) {
  CHECK_P(sim_paused());

  next_phase_total = gb->get_sys().gb_phase_total_old;

  d("State count   : %d\n", gb.state_count());
  d("State size    : %d K\n", gb.state_size_bytes() / 1024);
  //d("BGB cycle     : 0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock     : %f\n",      double(next_phase_total) / (4194304.0 * 2));
  d("Steps left    : %lld\n", step_count.load());

  double phase_rate = (next_phase_total - prev_phase_total) / (sim_time - old_sim_time);

  if (sim_time > old_sim_time && phase_rate > 0) {
    if (sim_time == old_sim_time) phase_rate = 0;
    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);
  d("Sim paused    : %d\n", sim_paused());

  prev_phase_total = next_phase_total;
  old_sim_time = sim_time;
}

//------------------------------------------------------------------------------

void GateBoyThread::load_raw_dump(BlobStream& bs) {
  CHECK_P(sim_paused());
  gb.reset_history();
  clear_steps();
  gb->load_raw_dump(bs);
  cart_blob = bs.rest();
  prev_phase_total = gb->get_sys().gb_phase_total_old;
}

//------------------------------------------------------------------------------

void GateBoyThread::save_raw_dump(BlobStream& bs) {
  CHECK_P(sim_paused());
  clear_steps();
  gb->save_raw_dump(bs);
  bs.write(cart_blob.data(), cart_blob.size());
}

//------------------------------------------------------------------------------

void GateBoyThread::load_cart_blob(blob& b) {
  CHECK_P(sim_paused());
  cart_blob = b;
}

//------------------------------------------------------------------------------

void GateBoyThread::load_program(const char* source) {
  CHECK_P(sim_paused());
  Assembler as;
  as.assemble(source);
  cart_blob = as.link();
}

void GateBoyThread::load_bootrom(const char* source) {
  CHECK_P(sim_paused());
  Assembler as;
  as.assemble(source);
  gb->load_bootrom(as.block_code->data(), (int)as.block_code->size());
}

//------------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

/*
void GateBoyThread::load_flat_dump(BlobStream& bs) {
  CHECK_P(sim_paused());
  cart_blob = bs.rest();
  memcpy(gbp->gba.mem.vid_ram,  cart_blob.data() + 0x8000, 8192);
  memcpy(gbp->gba.mem.cart_ram, cart_blob.data() + 0xA000, 8192);
  memcpy(gbp->gba.mem.int_ram,  cart_blob.data() + 0xC000, 8192);
  memcpy(gbp->gba.mem.oam_ram,  cart_blob.data() + 0xFE00, 256);
  memcpy(gbp->gba.mem.zero_ram, cart_blob.data() + 0xFF80, 128);

  gbp->gba.dbg_write(cart_blob, ADDR_BGP,  cart_blob[ADDR_BGP]);
  gbp->gba.dbg_write(cart_blob, ADDR_OBP0, cart_blob[ADDR_OBP0]);
  gbp->gba.dbg_write(cart_blob, ADDR_OBP1, cart_blob[ADDR_OBP1]);
  gbp->gba.dbg_write(cart_blob, ADDR_SCY,  cart_blob[ADDR_SCY]);
  gbp->gba.dbg_write(cart_blob, ADDR_SCX,  cart_blob[ADDR_SCX]);
  gbp->gba.dbg_write(cart_blob, ADDR_WY,   cart_blob[ADDR_WY]);
  gbp->gba.dbg_write(cart_blob, ADDR_WX,   cart_blob[ADDR_WX]);
  gbp->gba.dbg_write(cart_blob, ADDR_LCDC, cart_blob[ADDR_LCDC]);
}
*/


//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================

void GateBoyThread::thread_main() {
  LOG_B("Command loop starting\n");

  while (1) {
    {
      std::unique_lock<std::mutex> lock(sync.mut);
      if (sync.test(REQ_PAUSE) || step_count == 0) {
        sync.set(ACK_PAUSE);
        sync.cond.wait(lock, [this] {
          return sync.test(REQ_EXIT) || (sync.test_none(REQ_PAUSE) && (step_count != 0));
        });
        sync.clear(ACK_PAUSE);
      }
    }

    // Run the sim until we get a break signal
    double time_begin = timestamp();
    run_steps();
    double time_end = timestamp();

    // Update stats
    sim_time += (time_end - time_begin);

    if (sync.test(REQ_EXIT)) {
      sync.set(ACK_EXIT);
      return;
    }
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_steps() {
  run_normal();
}

//------------------------------------------------------------------------------

void GateBoyThread::next_phase() {
  if (gb->get_sys().gb_phase_total_old % 10000 == 0) {
    gb.push();

#ifdef _MSC_VER
    //dump_thread_times();
#endif
  }

  gb->next_phase(cart_blob);
  if (step_count) step_count--;
}

void GateBoyThread::run_sync() {
  while (step_count != 0) {
    next_phase();
  }
}

void GateBoyThread::run_normal() {
  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    next_phase();
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::panic() {
  step_count = 0;
  old_sim_time = 0;
  prev_phase_total = 0;
  phase_rate_smooth = 0;
}

//------------------------------------------------------------------------------
