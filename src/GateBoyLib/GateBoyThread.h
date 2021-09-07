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

struct AtomicFlags {

  void set(int mask) {
    flags |= mask;
    cond.notify_all();
  }
  
  void clear(int mask) {
    flags &= ~mask;
    cond.notify_all();
  }

  bool test(int mask) const {
    return (flags & mask) == mask;
  }

  bool test_any(int mask) const {
    return (flags & mask) != 0;
  }

  bool test_none(int mask) const {
    return (flags & mask) == 0;
  }

  /*
  void wait(int mask) {
    std::unique_lock<std::mutex> lock(mut);
    cond.wait(lock, [this, mask] { return test(mask); });
  }

  void wait_any(int mask) {
    std::unique_lock<std::mutex> lock(mut);
    cond.wait(lock, [this, mask] { return test_any(mask); });
  }

  void wait_none(int mask) {
    std::unique_lock<std::mutex> lock(mut);
    cond.wait(lock, [this, mask] { return test_none(mask); });
  }
  */

  std::atomic_int flags;
  std::mutex mut;
  std::condition_variable cond;
};

//--------------------------------------------------------------------------------

struct GateBoyThread {

  GateBoyThread();

  void start();
  void stop();
  void pause();
  void resume();

  void reset_to_bootrom();
  void reset_to_cart();

  void add_steps(int steps);
  void clear_steps();
  int  get_steps() const { return step_count; }

  void rewind(int steps);

  void load_raw_dump(const char* filename);
  void save_raw_dump(const char* filename);
  void load_blob(const blob& _cart_blob);
  void load_rom(const char* filename);
  void load_flat_dump(const char* filename);

  void set_buttons(uint8_t buttons) {
    gb_a->sys_buttons = buttons;
    gb_b->sys_buttons = buttons;
  }

  const int REQ_PAUSE = 0b0001;
  const int ACK_PAUSE = 0b0010;
  const int REQ_EXIT  = 0b0100;
  const int ACK_EXIT  = 0b1000;

  bool sim_paused() const { return sync.test(ACK_PAUSE); }

  bool has_work() const {
    return step_count != 0;
  }

  const blob& get_cart() const {
    return cart_blob;
  }

  void dump(Dumper& d);

  GateBoy& get_gb() {
    CHECK_P(sim_paused());
    return *gb_a.state();
  }

  StateStack<GateBoy> gb_a;
  StateStack<GateBoy> gb_b;

private:

  void thread_main();
  void run_normal();
  void run_regression();
  void run_idempotence();
  void panic();

  std::thread* main = nullptr;

  blob cart_blob;
  std::atomic_int step_count = 0;


  double   old_sim_time = 0;
  uint64_t old_phase_total = 0;
  double   phase_rate_smooth = 0;

  //----------

  // 00 - both threads running
  // 10 - app thread waiting on sim thread to pause
  // 11 - sim thread paused, app thread running
  // 01 - app thread waiting on sim thread to resume

  AtomicFlags sync;
};

//--------------------------------------------------------------------------------
