#include "GateBoyThread.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

//------------------------------------------------------------------------------

GateBoyThread::GateBoyThread()
{
  gb_a->wipe();
  gb_b->wipe();
}

//----------------------------------------

void GateBoyThread::start() {
  if (main) return;
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
  std::unique_lock<std::mutex> lock(sync.mut);
  sync.set(REQ_PAUSE);
  sync.cond.wait(lock, [this] { return sync.test(ACK_PAUSE); });
}

//----------------------------------------

void GateBoyThread::resume() {
  sync.clear(REQ_PAUSE);
}

//----------------------------------------

void GateBoyThread::reset_to_bootrom() {
  CHECK_P(sim_paused());
  clear_steps();
  gb_a.reset_states();
  gb_b.reset_states();
  gb_a->reset_to_bootrom(cart_blob, true);
  gb_b->reset_to_bootrom(cart_blob, true);
}

void GateBoyThread::reset_to_cart() {
  CHECK_P(sim_paused());
  clear_steps();
  gb_a.reset_states();
  gb_b.reset_states();
  gb_a->reset_to_cart(cart_blob);
  gb_b->reset_to_cart(cart_blob);
}

//----------------------------------------

void GateBoyThread::add_steps(int steps) {
  CHECK_P(sim_paused());
  gb_a.push();
  gb_b.push();
  step_count += steps;
}

void GateBoyThread::rewind(int steps) {
  CHECK_P(sim_paused());
  clear_steps();
  while (steps--) {
    gb_a.pop();
    gb_b.pop();
  }
}

//----------------------------------------

void GateBoyThread::clear_steps() {
  CHECK_P(sim_paused());
  step_count = 0;
  old_sim_time = 0;
  old_phase_total = 0;
  phase_rate_smooth = 0;
}

//------------------------------------------------------------------------------

void GateBoyThread::dump(Dumper& d) {
  CHECK_P(sim_paused());

  d("State count A  : %d\n", gb_a.state_count());

  size_t state_size = gb_a.state_size_bytes() + gb_b.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size    : %d K\n", state_size / 1024);
  }
  else {
    d("State size    : %d M\n", state_size / (1024 * 1024));
  }
  //d("BGB cycle     : 0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock     : %f\n",      double(gb_a->phase_total) / (4194304.0 * 2));
  d("Steps left    : %d\n", step_count.load());

  double phase_rate = (gb_a->phase_total - old_phase_total) / (gb_a->sim_time - old_sim_time);

  if (phase_rate > 0) {
    if (gb_a->sim_time == old_sim_time) {
      phase_rate = 0;
    }

    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);

  d("Sim paused    : %d\n", sim_paused());
  //d("sig_break     : %d\n", (int)sig_break);
  //d("sig_exit      : %d\n", (int)sig_exit);


  old_phase_total = gb_a->phase_total;
  old_sim_time = gb_a->sim_time;
}

//------------------------------------------------------------------------------

void GateBoyThread::load_raw_dump(const char* filename) {
  CHECK_P(sim_paused());

  LOG_B("Loading raw dump from %s\n", filename);

  clear_steps();
  blob raw_dump = ::load_blob(filename);
  if (raw_dump.empty()) return;

  gb_a->from_blob(raw_dump);
  gb_b->from_blob(raw_dump);

  int cart_size = (int)raw_dump.size() - sizeof(GateBoy);
  cart_blob.resize(cart_size);
  memcpy(cart_blob.data(), raw_dump.data() + sizeof(GateBoy), cart_size);
}

//------------------------------------------------------------------------------

void GateBoyThread::save_raw_dump(const char* filename) {
  CHECK_P(sim_paused());

  LOG_B("Saving raw dump to %s\n", filename);

  clear_steps();
  blob raw_dump;
  gb_a->to_blob(raw_dump);
  raw_dump.insert(raw_dump.end(), cart_blob.begin(), cart_blob.end());
  save_blob(filename, raw_dump);
}

//------------------------------------------------------------------------------

void GateBoyThread::load_blob(const blob& _cart_blob) {
  CHECK_P(sim_paused());

  cart_blob = _cart_blob;
  reset_to_cart();
}

//------------------------------------------------------------------------------

void GateBoyThread::load_rom(const char* filename) {
  CHECK_P(sim_paused());

  clear_steps();

  LOG_B("Loading %s\n", filename);

  cart_blob = ::load_blob(filename);

  LOG_B("GateBoyApp::load_rom cart_blob %p %d\n", cart_blob.data(), (int)cart_blob.size());

  reset_to_cart();

  LOG_B("Loaded %zd bytes from rom %s\n", cart_blob.size(), filename);
}

//------------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyThread::load_flat_dump(const char* filename) {
  CHECK_P(sim_paused());

  cart_blob = ::load_blob(filename);
  reset_to_cart();

  memcpy(gb_a->vid_ram,  cart_blob.data() + 0x8000, 8192);
  memcpy(gb_a->cart_ram, cart_blob.data() + 0xA000, 8192);
  memcpy(gb_a->int_ram,  cart_blob.data() + 0xC000, 8192);
  memcpy(gb_a->oam_ram,  cart_blob.data() + 0xFE00, 256);
  memcpy(gb_a->zero_ram, cart_blob.data() + 0xFF80, 128);

  gb_a->dbg_write(cart_blob, ADDR_BGP,  cart_blob[ADDR_BGP]);
  gb_a->dbg_write(cart_blob, ADDR_OBP0, cart_blob[ADDR_OBP0]);
  gb_a->dbg_write(cart_blob, ADDR_OBP1, cart_blob[ADDR_OBP1]);
  gb_a->dbg_write(cart_blob, ADDR_SCY,  cart_blob[ADDR_SCY]);
  gb_a->dbg_write(cart_blob, ADDR_SCX,  cart_blob[ADDR_SCX]);
  gb_a->dbg_write(cart_blob, ADDR_WY,   cart_blob[ADDR_WY]);
  gb_a->dbg_write(cart_blob, ADDR_WX,   cart_blob[ADDR_WX]);
  gb_a->dbg_write(cart_blob, ADDR_LCDC, cart_blob[ADDR_LCDC]);

  memcpy(gb_b.state(), gb_a.state(), sizeof(GateBoy));
}

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
    if      (config_regression)  run_regression();
    else if (config_idempotence) run_idempotence();
    else                         run_normal();
    double time_end = timestamp();

    // Update stats
    gb_a->sim_time += (time_end - time_begin);
    gb_b->sim_time += (time_end - time_begin);

    if (sync.test(REQ_EXIT)) {
      sync.set(ACK_EXIT);
      return;
    }
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_normal() {
  auto& gba = *gb_a.state();

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {

    gba.next_phase(cart_blob, config_fastmode);

    step_count--;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_regression() {
  auto& gba = *gb_a.state();
  auto& gbb = *gb_b.state();

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {

    gba.next_phase(cart_blob, true);
    gbb.next_phase(cart_blob, false);

    uint64_t hash_a_new = gba.hash();
    uint64_t hash_b_new = gbb.hash();

    if (hash_a_new != hash_b_new) {
      LOG_R("Regression test mismatch NEW!\n");
      diff_blob(gb_a.state(), 0, sizeof(GateBoy), gb_b.state(), 0, sizeof(GateBoy), 0x01);
      LOG_R("Regression test mismatch NEW!\n");
      clear_steps();
      return;
    }

    step_count--;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_idempotence() {
  auto& gba = *gb_a.state();
  auto& gbb = *gb_b.state();

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    gba.tock_cpu();

    gba.tock_gates(cart_blob);
    gba.update_framebuffer(pack(gba.pix_count) - 8, pack(gba.reg_ly), gba.lcd.PIN_51_LCD_DATA0.qp_ext_old(), gba.lcd.PIN_50_LCD_DATA1.qp_ext_old());

    uint64_t hash_a = gba.hash();

    memcpy(gb_b.state(), gb_a.state(), sizeof(GateBoy));

    gbb.tock_gates(cart_blob);
    gbb.update_framebuffer(pack(gbb.pix_count) - 8, pack(gbb.reg_ly), gbb.lcd.PIN_51_LCD_DATA0.qp_ext_old(), gbb.lcd.PIN_50_LCD_DATA1.qp_ext_old());

    uint64_t hash_b = gbb.hash();

    gba.phase_total++;

    if (hash_a != hash_b) {
      LOG_R("Sim not stable after second pass!\n");
      diff_blob(gb_a.state(), 0, sizeof(GateBoy), gb_b.state(), 0, sizeof(GateBoy), 0xFF);
      LOG_R("Sim not stable after second pass!\n");
      step_count = 0;
      return;
    }

    step_count--;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::panic() {
  step_count = 0;
  old_sim_time = 0;
  old_phase_total = 0;
  phase_rate_smooth = 0;
}

//------------------------------------------------------------------------------
