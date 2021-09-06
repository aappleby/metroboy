#include "GateBoyThread.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

//------------------------------------------------------------------------------

void GateBoyThread::start() {
  if (main) return;
  main = new std::thread([this] { thread_main(); });
}

//----------------------------------------

void GateBoyThread::stop() {
  if (!main) return;
  LOG_B("Stopping gateboy thread\n");

  sig_break = true;
  pause_barrier.arrive_and_wait();
  sig_exit = true;
  resume_barrier.arrive_and_wait();

  main->join();

  LOG_B("Gateboy thread stopped\n");
  delete main;
}

//----------------------------------------

void GateBoyThread::pause() {
  if (!pause_count++) {
    sig_break = true;
    pause_barrier.arrive_and_wait();
  }
}

//----------------------------------------

void GateBoyThread::resume() {
  if (pause_count && !--pause_count) {
    sig_break = false;
    resume_barrier.arrive_and_wait();
  }
}

//----------------------------------------

//void GateBoyThread::set_cart(const blob& new_cart_blob) {
//  cart_blob = new_cart_blob;
//}

void GateBoyThread::reset_to_bootrom() {
  CHECK_P(sig_paused);
  clear_work();
  gb_a.reset_states();
  gb_b.reset_states();
  gb_a->reset_to_bootrom(cart_blob, true);
  gb_b->reset_to_bootrom(cart_blob, true);
}

void GateBoyThread::reset_to_cart() {
  CHECK_P(sig_paused);
  clear_work();
  gb_a.reset_states();
  gb_b.reset_states();
  gb_a->reset_to_cart(cart_blob);
  gb_b->reset_to_cart(cart_blob);
}

//----------------------------------------

void GateBoyThread::step_phase(int steps) {
  CHECK_P(sig_paused);
  clear_work();
  gb_a.push();
  gb_b.push();
  step_count = steps;
}

void GateBoyThread::step_back(int steps) {
  CHECK_P(sig_paused);
  clear_work();
  while (steps--) {
    gb_a.pop();
    gb_b.pop();
  }
}

//----------------------------------------

void GateBoyThread::clear_work() {
  CHECK_P(sig_paused);
  step_count = 0;
  old_sim_time = 0;
  old_phase_total = 0;
  phase_rate_smooth = 0;
}

//------------------------------------------------------------------------------

void GateBoyThread::dump(Dumper& d) {
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
  d("Steps left    : %d\n", step_count);

  double phase_rate = (gb_a->phase_total - old_phase_total) / (gb_a->sim_time - old_sim_time);

  if (phase_rate > 0) {
    if (gb_a->sim_time == old_sim_time) {
      phase_rate = 0;
    }

    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);
  d("sig_break     : %d\n", (int)sig_break);
  d("sig_exit      : %d\n", (int)sig_exit);


  old_phase_total = gb_a->phase_total;
  old_sim_time = gb_a->sim_time;
}

//------------------------------------------------------------------------------

#pragma optimize("", off)

void GateBoyThread::thread_main() {
  LOG_B("Command loop starting\n");

  while(1) {
    sig_paused = true;
    pause_barrier.arrive_and_wait();
    resume_barrier.arrive_and_wait();
    sig_paused = false;

    if (sig_exit) return;

    if (config_regression) {
      double time_begin = timestamp();
      while (step_count && !sig_break) {
        auto gba = gb_a.state();
        auto gbb = gb_b.state();

        uint64_t hash_a_old = gb_a->hash();
        uint64_t hash_b_old = gb_b->hash();

        if (hash_a_old != hash_b_old) {
          LOG_R("Regression test mismatch OLD!\n");
          diff_blob(gb_a.state(), 0, sizeof(GateBoy), gb_b.state(), 0, sizeof(GateBoy), 0x01);
          LOG_R("Regression test mismatch OLD!\n");
        }

        gb_a->next_phase(cart_blob, true);
        gb_b->next_phase(cart_blob, false);

        uint64_t hash_a_new = gb_a->hash();
        uint64_t hash_b_new = gb_b->hash();

        if (hash_a_new != hash_b_new) {
          LOG_R("Regression test mismatch NEW!\n");
          diff_blob(gb_a.state(), 0, sizeof(GateBoy), gb_b.state(), 0, sizeof(GateBoy), 0x01);
          LOG_R("Regression test mismatch NEW!\n");
        }

        step_count--;
      }
      double time_end = timestamp();
      gb_a->sim_time += (time_end - time_begin);
      gb_b->sim_time += (time_end - time_begin);
    }
    else if (config_idempotence) {

      gb_a->tock_gates(cart_blob, 0);
      gb_a->update_framebuffer(gb_a->pix_count.get_old() - 8, gb_a->reg_ly.get_old(), gb_a->lcd.PIN_51_LCD_DATA0.qp_ext_old(), gb_a->lcd.PIN_50_LCD_DATA1.qp_ext_old());

      uint64_t hash_a = gb_a->hash();

      gb_a->tock_gates(cart_blob, 0);
      gb_a->update_framebuffer(gb_a->pix_count.get_old() - 8, gb_a->reg_ly.get_old(), gb_a->lcd.PIN_51_LCD_DATA0.qp_ext_old(), gb_a->lcd.PIN_50_LCD_DATA1.qp_ext_old());

      uint64_t hash_b = gb_a->hash();

      gb_a->phase_total++;

      if (hash_a != hash_b) {
        LOG_R("Sim not stable after second pass!\n");
      }
    }
    else {
      double time_begin = timestamp();
      while (step_count && !sig_break) {
        gb_a->next_phase(cart_blob, config_fastmode);
        step_count--;
      }
      double time_end = timestamp();
      gb_a->sim_time += (time_end - time_begin);
    }
  }
}

//------------------------------------------------------------------------------
