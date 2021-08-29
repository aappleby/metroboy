#include "GateBoyThread.h"

#include "CoreLib/Constants.h"

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
  printf("Stopping gateboy thread\n");

  sig_break = true;
  pause_barrier.arrive_and_wait();
  sig_exit = true;
  resume_barrier.arrive_and_wait();

  main->join();

  printf("Gateboy thread stopped\n");
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

void GateBoyThread::set_cart(const blob& new_cart_blob) {
  cart_blob = new_cart_blob;
}

void GateBoyThread::reset_to_bootrom() {
  CHECK_P(sig_paused);
  clear_work();
  gb.reset_states();
  gb->reset_to_bootrom(cart_blob, true);
}

void GateBoyThread::reset_to_cart() {
  CHECK_P(sig_paused);
  clear_work();
  gb.reset_states();
  gb->reset_to_cart(cart_blob);
}

//----------------------------------------

void GateBoyThread::step_phase(int steps) {
  CHECK_P(sig_paused);
  clear_work();
  gb.push();
  step_count = steps;
}

void GateBoyThread::step_back(int steps) {
  CHECK_P(sig_paused);
  clear_work();
  while (steps--) gb.pop();
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
  d("State count   : %d\n", gb.state_count());
  size_t state_size = gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size    : %d K\n", state_size / 1024);
  }
  else {
    d("State size    : %d M\n", state_size / (1024 * 1024));
  }
  //d("BGB cycle     : 0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock     : %f\n",      double(gb->phase_total) / (4194304.0 * 2));
  d("Steps left    : %d\n", step_count);

  double phase_rate = (gb->phase_total - old_phase_total) / (gb->sim_time - old_sim_time);

  if (phase_rate > 0) {
    if (gb->sim_time == old_sim_time) {
      phase_rate = 0;
    }

    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);
  d("sig_break     : %d\n", (int)sig_break);
  d("sig_exit      : %d\n", (int)sig_exit);


  old_phase_total = gb->phase_total;
  old_sim_time = gb->sim_time;
}

//------------------------------------------------------------------------------

void GateBoyThread::thread_main() {
  printf("Command loop starting\n");

  while(1) {
    sig_paused = true;
    pause_barrier.arrive_and_wait();
    resume_barrier.arrive_and_wait();
    sig_paused = false;

    if (sig_exit) return;

    double time_begin = timestamp();
    while (step_count && !sig_break) {
      gb->next_phase(cart_blob);
      step_count--;
    }

    double time_end = timestamp();
    gb->sim_time += (time_end - time_begin);
  }
}

//------------------------------------------------------------------------------
