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

  void start();
  void stop();
  void pause();
  void resume();
  bool paused() { return sig_break; }

  void set_cart(const blob& new_cart_blob);
  const blob& get_cart() const {
    return cart_blob;
  }
  void reset_to_bootrom();
  void reset_to_cart();

  void step_phase(int steps);
  void step_back(int steps);
  void clear_work();

  bool busy() {
    pause();
    bool result = step_count != 0;
    resume();
    return result;
  }

  void dump(Dumper& d);

  StateStack<GateBoy> gb;

private:

  void thread_main();

  std::thread* main;

  blob cart_blob;

  int pause_count = 0;
  std::atomic_bool sig_break   = false;
  std::atomic_bool sig_paused  = false;
  std::atomic_bool sig_exit    = false;

  Barrier2 pause_barrier;
  Barrier2 resume_barrier;

  int step_count;

  double   old_sim_time = 0;
  uint64_t old_phase_total = 0;
  double   phase_rate_smooth = 0;
};

//--------------------------------------------------------------------------------
