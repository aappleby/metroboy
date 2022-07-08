#pragma once

#include "CoreLib/Assembler.h"
#include "CoreLib/Types.h"
#include "CoreLib/StateManager2.h"
#include "CoreLib/File.h"

#include "GateBoyLib/GateBoyPair.h"

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

  GateBoyThread(IGateBoy* prototype);

  void start();
  void stop();
  void pause();
  void resume();

  void reset_to_poweron(bool fastboot);
  void reset_to_bootrom();
  void reset_to_cart();

  void add_steps(int64_t steps);
  void run_to(uint64_t phase);
  void clear_steps();
  int64_t get_steps() const;

  void rewind(int steps);

  void load_raw_dump(BlobStream& bs);
  void save_raw_dump(BlobStream& bs);
  void load_flat_dump(BlobStream& bs);

  void load_cart_blob(blob& bs);
  void load_program(const char* source);
  void set_buttons(uint8_t buttons);

  const int REQ_PAUSE = 0b0001;
  const int ACK_PAUSE = 0b0010;
  const int REQ_EXIT  = 0b0100;
  const int ACK_EXIT  = 0b1000;

  bool sim_paused() const;
  bool has_work() const;
  blob& get_cart();
  const blob& get_cart() const;

  void dump(Dumper& d);

  StatePointerStack<IGateBoy> gb;

  void run_steps();


  void thread_main();

  void run_normal();
  void run_regression();
  void run_idempotence();
  void panic();

private:

  std::thread* main = nullptr;
  AtomicFlags sync;

  blob cart_blob;
  std::atomic_int64_t step_count = 0;

  double   sim_time = 0;
  double   old_sim_time = 0;
  uint64_t old_phase_total = 0;
  double   phase_rate_smooth = 0;

};

//--------------------------------------------------------------------------------
