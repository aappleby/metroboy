#include "GateBoyThread.h"

#include "CoreLib/Constants.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

//-----------------------------------------------------------------------------

void GateBoyThread::reset_poweron(const blob& _boot, const blob& _cart) {
  printf("Resetting sim to run boot rom\n");
  boot = _boot;
  cart = _cart;
  gb.reset_states();
  gb->reset_poweron(boot.data(), boot.size(),
                    cart.data(), cart.size(), true);
}

void GateBoyThread::reset_cart(const blob& _boot, const blob& _cart) {
  printf("Resetting sim to run cart\n");
  boot = _boot;
  cart = _cart;
  gb.reset_states();
  gb->reset_cart(boot.data(), boot.size(),
                 cart.data(), cart.size());
}

void GateBoyThread::set_cart(const blob& _boot, const blob& _cart) {
  boot = _boot;
  cart = _cart;
  gb->set_cart(boot.data(), boot.size(),
               cart.data(), cart.size());
}

//------------------------------------------------------------------------------

void GateBoyThread::thread_main() {
  std::unique_lock<std::mutex> lock(mut, std::defer_lock);

  printf("Thread starting\n");

  //gb->tock_slow();
  //gb->commit_and_hash();


  printf("Command loop starting\n");
  while(!sig_exit) {
    // Lock and wait until we're unpaused and we have a job in the queue.
    lock.lock();
    while (sig_pause || (command.count == 0 && (cursor_r == cursor_w))) {
      sig_waiting = true;
      cv_thread_pause.notify_one();
      cv_thread_resume.wait(lock);
    }
    sig_waiting = false;

    // Grab the next job off the queue.
    if (command.count == 0 && (cursor_r != cursor_w)) {
      command = ring[cursor_r++];
      if (command.op == CMD_StepPhase) gb.push();
      if (command.op == CMD_StepPass)  gb.push();
    }

    // Unlock and do the job if we have one.
    lock.unlock();
    switch(command.op) {
    case CMD_Exit:      sig_exit = true;  break;
    case CMD_StepPhase: run_step_phase(); break;
    case CMD_StepPass:  run_step_pass();  break;
    case CMD_StepBack:  run_step_back();  break;
    default:
      printf("BAD COMMAND\n");
      command.count = 0;
      break;
    }
  }
}

//----------------------------------------

void GateBoyThread::pause() {
  if (!pause_count++) {
    std::unique_lock<std::mutex> lock(mut);
    sig_pause = true;
    while (!sig_waiting) cv_thread_pause.wait(lock);
  }
}

//----------------------------------------

void GateBoyThread::resume() {
  if (pause_count && !--pause_count) {
    std::unique_lock<std::mutex> lock(mut);
    sig_pause = false;
    cv_thread_resume.notify_one();
  }
}

//----------------------------------------

void GateBoyThread::post_work(Command c) {
  std::unique_lock<std::mutex> lock(mut);
  if (uint8_t(cursor_w + 1) != cursor_r) {
    ring[cursor_w++] = c;
  }
  cv_thread_resume.notify_one();
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

void GateBoyThread::run_step_phase() {
  double time_begin = timestamp();

  for(;command.count && !sig_pause; command.count--) {
    gb->next_phase();
  }

  double time_end = timestamp();
  gb->sim_time += (time_end - time_begin);
}

//----------------------------------------

void GateBoyThread::run_step_pass() {
  for(;command.count && !sig_pause; command.count--) {
    //gb->next_pass();
  }
}

//----------------------------------------

void GateBoyThread::run_step_back() {
  for(;command.count && !sig_pause; command.count--) {
    gb.pop();
  }
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

void GateBoyThread::step_phase(int steps) {
  post_work({CMD_StepPhase, steps});
}

void GateBoyThread::step_pass(int steps) {
  post_work({CMD_StepPass, steps});
}

void GateBoyThread::step_back(int steps) {
  post_work({CMD_StepBack, steps});
}

//------------------------------------------------------------------------------
