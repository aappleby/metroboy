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
#include <thread>

//--------------------------------------------------------------------------------
// Trivial barrier implementation because std::barrier doesn't seem to be in
// Ubuntu 20.04 by default...

class Barrier2 {
public:

  Barrier2(int _max) : count(0), max(_max) {}

  void arrive_and_wait() {
    std::unique_lock<std::mutex> lock(mut);

    count++;

    if (count == max) {
      count = 0;
      cond.notify_all();
    }
    else {
      cond.wait(lock, [this] { return count == 0; });
    }
  }

  std::mutex mut;
  std::condition_variable cond;
  int count;
  int max;
};

//--------------------------------------------------------------------------------

struct GateBoyThread {

  GateBoyThread() : pause_barrier(2), resume_barrier(2)
  {
  }

  void reset_to_bootrom();
  void reset_to_cart();
  void load_cart(const blob& _boot, const blob& _cart);

  void start();
  void stop();
  void pause();
  void resume();
  bool paused() { return sig_break; }

  void step_phase(int steps);
  void step_back(int steps);
  void clear_work();

  void dump(Dumper& d);

  StateStack<GateBoy> gb;
  blob boot;
  blob cart;

private:

  struct Command {
    int64_t op;
    int64_t count;
  };

  void thread_main();
  void post_work(Command c);

  void run_step_phase();
  void run_step_back();

  std::thread* main;

  int pause_count = 0;
  std::atomic_bool sig_break   = false;
  std::atomic_bool sig_exit    = false;

  Barrier2 pause_barrier;
  Barrier2 resume_barrier;

  enum {
    CMD_None,
    CMD_Exit,
    CMD_StepPhase,
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
