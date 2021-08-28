#include "GateBoyThread.h"

#include "CoreLib/Constants.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

//-----------------------------------------------------------------------------

void GateBoyThread::reset_to_bootrom() {
  gb.reset_states();
  gb->reset_to_bootrom(true);
}

void GateBoyThread::reset_to_cart() {
  gb.reset_states();
  gb->reset_to_cart();
}

void GateBoyThread::load_cart(const blob& _boot, const blob& _cart) {
  boot = _boot;
  cart = _cart;
  gb->load_cart(boot.data(), boot.size(), cart.data(), cart.size());
}

//------------------------------------------------------------------------------

void GateBoyThread::start() {
  if (main) return;
  main = new std::thread([this] { thread_main(); });
}

//----------------------------------------

void GateBoyThread::stop() {
  if (!main) return;
  clear_work();
  post_work({CMD_Exit, 1});
  while (pause_count) resume();
  main->join();
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

//------------------------------------------------------------------------------

void GateBoyThread::step_phase(int steps) {
  post_work({CMD_StepPhase, steps});
}

//----------------------------------------

void GateBoyThread::step_back(int steps) {
  post_work({CMD_StepBack, steps});
}

//----------------------------------------

void GateBoyThread::clear_work() {
  pause();
  cursor_r = 0;
  cursor_w = 0;
  command.count = 0;
  resume();
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

  d("Commands left : %d\n",    uint8_t(cursor_w - cursor_r));
  d("Steps left    : %d\n",    command.count);

  double phase_rate = (gb->phase_total - old_phase_total) / (gb->sim_time - old_sim_time);

  if (phase_rate > 0) {
    if (gb->sim_time == old_sim_time) {
      phase_rate = 0;
    }

    phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);
  }

  d("Phase rate    : %f\n",      phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * phase_rate_smooth / PHASES_PER_SECOND);

  old_phase_total = gb->phase_total;
  old_sim_time = gb->sim_time;
}

//------------------------------------------------------------------------------

void GateBoyThread::thread_main() {
  printf("Command loop starting\n");
  while(!sig_exit) {
    // Pause until we have a job in the queue.

    while (sig_break || (command.count == 0 && (cursor_r == cursor_w))) {
      pause_barrier.arrive_and_wait();
      resume_barrier.arrive_and_wait();
    }

    // Grab the next job off the queue.
    if (command.count == 0 && (cursor_r != cursor_w)) {
      command = ring[cursor_r++];
      if (command.op == CMD_StepPhase) gb.push();
    }

    // Do the job if we have one.
    switch(command.op) {
    case CMD_None:                        break;
    case CMD_Exit:      sig_exit = true;  break;
    case CMD_StepPhase: run_step_phase(); break;
    case CMD_StepBack:  run_step_back();  break;
    default:
      printf("BAD COMMAND\n");
      command.count = 0;
      break;
    }
  }
}

//----------------------------------------

void GateBoyThread::post_work(Command c) {
  pause();
  if (uint8_t(cursor_w + 1) != cursor_r) {
    ring[cursor_w++] = c;
  }
  resume();
}

//------------------------------------------------------------------------------

void GateBoyThread::run_step_phase() {
  double time_begin = timestamp();

  while(command.count && !sig_break) {
    gb->next_phase();
    command.count--;

    // this doesn't work
    /*
    // Pause at the end of the bootrom, if we're running it.
    if (gb->phase_total == gb->phase_origin) {
      clear_work();
      pause_barrier.arrive_and_wait();
      resume_barrier.arrive_and_wait();
    }
    */
  }

  double time_end = timestamp();
  gb->sim_time += (time_end - time_begin);
}

//----------------------------------------

void GateBoyThread::run_step_back() {
  for(;command.count && !sig_break; command.count--) {
    gb.pop();
  }
}

//------------------------------------------------------------------------------
