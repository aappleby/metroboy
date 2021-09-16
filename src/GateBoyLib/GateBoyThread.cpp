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
  reset_gb();
}

//----------------------------------------

void GateBoyThread::start() {
  if (main) return;
  main = new std::thread([this] { thread_main(); });
  pause();
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

void GateBoyThread::reset_gb() {
  gbp.reset_states();
  //gbp->gba.wipe();
  //gbp->gbb.wipe();
}

//----------------------------------------


void GateBoyThread::reset_to_bootrom() {
  CHECK_P(sim_paused());
  clear_steps();
  reset_gb();
  gbp->gba.reset_to_bootrom(cart_blob, true);
  //gbp->gbb.reset_to_bootrom(cart_blob, true);
}

void GateBoyThread::reset_to_cart() {
  CHECK_P(sim_paused());
  clear_steps();
  reset_gb();
  gbp->gba.reset_to_cart(cart_blob);
  //gbp->gbb.reset_to_cart(cart_blob);
}

//----------------------------------------

void GateBoyThread::add_steps(int steps) {
  CHECK_P(sim_paused());
  gbp.push();
  step_count += steps;
}

void GateBoyThread::rewind(int steps) {
  CHECK_P(sim_paused());
  clear_steps();
  while (steps--) {
    gbp.pop();
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

  d("State count A  : %d\n", gbp.state_count());

  size_t state_size = gbp.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size    : %d K\n", state_size / 1024);
  }
  else {
    d("State size    : %d M\n", state_size / (1024 * 1024));
  }
  //d("BGB cycle     : 0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock     : %f\n",      double(gbp->gba.sys.phase_total) / (4194304.0 * 2));
  d("Steps left    : %d\n", step_count.load());

  double phase_rate = (gbp->gba.sys.phase_total - old_phase_total) / (gbp->gba.sys.sim_time - old_sim_time);

  if (phase_rate > 0) {
    if (gbp->gba.sys.sim_time == old_sim_time) {
      phase_rate = 0;
    }

    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);

  d("Sim paused    : %d\n", sim_paused());
  //d("sig_break     : %d\n", (int)sig_break);
  //d("sig_exit      : %d\n", (int)sig_exit);


  old_phase_total = gbp->gba.sys.phase_total;
  old_sim_time = gbp->gba.sys.sim_time;
}

//------------------------------------------------------------------------------

void GateBoyThread::load_raw_dump(BlobStream& bs) {
  CHECK_P(sim_paused());
  reset_gb();
  clear_steps();
  gbp->gba.load_raw_dump(bs);
  cart_blob = bs.rest();
}

//------------------------------------------------------------------------------

void GateBoyThread::save_raw_dump(BlobStream& bs) {
  CHECK_P(sim_paused());
  clear_steps();
  gbp->gba.save_raw_dump(bs);
  bs.write(cart_blob.data(), cart_blob.size());
}

//------------------------------------------------------------------------------

void GateBoyThread::load_cart_blob(blob& b) {
  CHECK_P(sim_paused());
  cart_blob = b;
}

//------------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

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
    gbp->gba.sys.sim_time += (time_end - time_begin);
    //gbp->gbb.sys.sim_time += (time_end - time_begin);

    if (sync.test(REQ_EXIT)) {
      sync.set(ACK_EXIT);
      return;
    }
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_steps() {
  if      (config_regression)  run_regression();
  else if (config_idempotence) run_idempotence();
  else                         run_normal();
}

//------------------------------------------------------------------------------

void GateBoyThread::run_normal() {
  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    gbp->next_phase(cart_blob);
    step_count--;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_regression() {
  /*
  auto& gba = gbp->gba;
  auto& gbb = gbp->gbb;

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    gba.next_phase(cart_blob);
    gbb.next_phase(cart_blob);

    uint64_t hash_a_new = gba.hash_regression();
    uint64_t hash_b_new = gbb.hash_regression();

    if (hash_a_new != hash_b_new) {
      LOG_R("Regression test mismatch @ phase %lld!\n", gba.sys.phase_total);
      gba.gb_state.diff(gbb.gb_state, 0x01);
      step_count = 0;
      return;
    }

    step_count--;
  }
  */

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    gbp->next_phase(cart_blob);
    step_count--;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::run_idempotence() {
  //auto& gba = gbp->gba;
  //auto& gbb = gbp->gbb;

  while ((step_count != 0) && sync.test_none(REQ_PAUSE | REQ_EXIT)) {
    gbp->next_phase(cart_blob);
    //gba.tock_cpu();
    //gba.tock_gates(cart_blob);
    //gba.gb_state.commit();
    //gba.update_framebuffer();

    /*
    uint64_t hash_a = gba.gb_state.hash_all();

    memcpy(&gbp->gbb, &gbp->gba, sizeof(GateBoy));
    gbb.tock_gates(cart_blob);
    gbb.update_framebuffer(bit_pack(gbb.gb_state.pix_count) - 8, bit_pack(gbb.gb_state.reg_ly), gbb.gb_state.lcd.PIN_51_LCD_DATA0.qp_ext_old(), gbb.gb_state.lcd.PIN_50_LCD_DATA1.qp_ext_old());
    uint64_t hash_b = gbb.gb_state.hash_all();

    if (hash_a != hash_b) {
      LOG_R("Sim not stable after second pass!\n");
      gba.gb_state.diff(gbb.gb_state, 0xFF);
      LOG_R("Sim not stable after second pass!\n");
      step_count = 0;
      return;
    }
    */

    //gba.sys.phase_total++;
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
