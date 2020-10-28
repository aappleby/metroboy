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

enum {
  CMD_Exit,
  CMD_StepPhase,
  CMD_StepPass,
  CMD_StepBack,
};

//--------------------------------------------------------------------------------

struct Command {
  int64_t op;
  int64_t count;
};

//--------------------------------------------------------------------------------

/*
struct ThreadSync {

  void sync() {
    std::unique_lock<std::mutex> lock(mut);
    if (count == 0) {
      count = 1;
      while(count != 2) cv.wait(lock);
      count = 0;
    }
    else if (count == 1) {
      count = 2;
      lock.unlock();
      cv.notify_one();
    }
    else {
      CHECK_P(false);
    }
  }

  std::mutex mut;
  std::condition_variable cv;
  int count = 0;
};
*/

//--------------------------------------------------------------------------------

/*
struct Semaphore {

  Semaphore(int sem_min, int sem_max) : count_min(sem_min), count_max(sem_max) {};

  void raise() {
    std::unique_lock<std::mutex> lock(mut);
    while (count == count_max) {
      printf("cv_lower.wait(lock);\n");
      cv_lower.wait(lock);
    }
    count++;
  }

  void lower() {
    std::unique_lock<std::mutex> lock(mut);
    while (count == count_min) {
      printf("cv_raise.wait(lock)\n");
      cv_raise.wait(lock);
    }
    count--;
  }

  std::mutex mut;
  std::condition_variable cv_raise;
  std::condition_variable cv_lower;
  int count = 0;
  const int count_min;
  const int count_max;
};
*/

//--------------------------------------------------------------------------------

/*
struct RingQueue {

  int size() {
    std::unique_lock<std::mutex> lock(mut);
    return cursor_w - cursor_r;
  }

  void clear() {
    std::unique_lock<std::mutex> lock(mut);
    cursor_r = cursor_w = 0;
  }

  void push_back(Command c) {
    std::unique_lock<std::mutex> lock(mut);
    while((cursor_w + 1) == cursor_r) cv.wait(lock);
    ring[cursor_w++] = c;
    lock.unlock();
    cv.notify_one();
  }

  Command pop_front() {
    std::unique_lock<std::mutex> lock(mut);
    while(cursor_r == cursor_w) cv.wait(lock);
    Command c = ring[cursor_r++];
    lock.unlock();
    cv.notify_one();
    return c;
  }

  std::mutex mut;
  std::condition_variable cv;

  uint8_t cursor_r = 0;
  uint8_t cursor_w = 0;

  Command ring[256];
};
*/

//--------------------------------------------------------------------------------

/*
struct ThreadControl {

  void pause() {
    if (!pause_count++) {
      std::unique_lock<std::mutex> lock(mut);
      sig_yield = true;
      while (sig_yield) cv.wait(lock);
    }
  }

  void resume() {
    if (pause_count && !--pause_count) {
      std::unique_lock<std::mutex> lock(mut);
      sig_resume = true;
      lock.unlock();
      cv.notify_one();
    }
  }

  void force_resume() {
    if (pause_count) {
      pause_count = 0;
      std::unique_lock<std::mutex> lock(mut);
      sig_resume = true;
      lock.unlock();
      cv.notify_one();
    }
  }

  void yield() {
    if (sig_yield) {
      std::unique_lock<std::mutex> lock(mut);
      sig_yield = false;
      lock.unlock();
      cv.notify_one();
      lock.lock();
      while (!sig_resume) cv.wait(lock);
      sig_resume = false;
    }
  }

  std::mutex mut;
  std::condition_variable cv;
  bool sig_yield  = false;
  bool sig_resume = false;
  int pause_count = 0;
};
*/

//--------------------------------------------------------------------------------

struct GateBoyThread {

  void init();
  void reset();
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

  void step_sim();

  void load_raw_dump();
  void save_raw_dump();
  void load_flat_dump(const char* filename);
  void load_rom(const char* filename);

  void dump1(Dumper& d);
  void dump2(Dumper& d);
  void dump3(Dumper& d);
  void dump4(Dumper& d);
  void dump5(Dumper& d);
  void dump6(Dumper& d);


  std::thread* main;

  StateStack<GateBoy> gb;
  blob rom_buf;
  Assembler assembler;

  int pause_count = 0;
  std::mutex mut;
  std::atomic_bool sig_pause   = false;
  std::atomic_bool sig_waiting = false;
  std::atomic_bool sig_exit    = false;
  std::condition_variable cv_thread_pause;
  std::condition_variable cv_thread_resume;

  uint8_t cursor_r = 0;
  uint8_t cursor_w = 0;
  Command ring[256];

  Command command = {0,0};

  double   old_sim_time = 0;
  int32_t  old_phase_total = 0;
  double   phase_rate_smooth = 0;
};

//--------------------------------------------------------------------------------
