#pragma once

#include "CoreLib/Assembler.h"
#include "CoreLib/Types.h"
#include "CoreLib/StateManager2.h"
#include "CoreLib/File.h"

#include "GateBoyLib/GateBoy.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <deque>

//--------------------------------------------------------------------------------

struct GateBoyThread {

  struct Command {
    int64_t op;
    int64_t count;
  };

  void reset_to_bootrom();
  void reset_to_cart();
  void load_cart(const blob& _boot, const blob& _cart);

  void start();
  void stop();

  void thread_main();
  void pause();
  void resume();
  void post_work(Command c);
  void clear_work();

  void step_phase(int steps);
  void step_pass(int steps);
  void step_back(int steps);

  void run_step_phase();
  void run_step_pass();
  void run_step_back();

  std::thread* main;

  StateStack<GateBoy> gb;
  blob boot;
  blob cart;
  Assembler assembler;

  int pause_count = 0;
  std::mutex mut;
  std::atomic_bool sig_pause   = false;
  std::atomic_bool sig_waiting = false;
  std::atomic_bool sig_exit    = false;
  std::condition_variable cv_thread_pause;
  std::condition_variable cv_thread_resume;

  enum {
    CMD_Exit,
    CMD_StepPhase,
    CMD_StepPass,
    CMD_StepBack,
  };

  uint8_t cursor_r = 0;
  uint8_t cursor_w = 0;
  Command ring[256];

  Command command = {0,0};

  double   old_sim_time = 0;
  uint64_t old_phase_total = 0;
  double   phase_rate_smooth = 0;
};

//--------------------------------------------------------------------------------
