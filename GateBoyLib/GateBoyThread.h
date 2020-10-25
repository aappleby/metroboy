#pragma once

#include "CoreLib/Assembler.h"
#include "CoreLib/Types.h"
#include "CoreLib/StateManager2.h"
#include "CoreLib/File.h"

#include "GateBoyLib/GateBoy.h"

#include <mutex>
#include <condition_variable>
#include <atomic>

//--------------------------------------------------------------------------------

struct Semaphore {

  void signal() {
    std::unique_lock<std::mutex> lock(mut);
    count = 1;
    lock.unlock();
    cv.notify_one();
  }

  void wait() {
    std::unique_lock<std::mutex> lock(mut);
    while (!count) cv.wait(lock);
  }

  void wait_and_clear() {
    std::unique_lock<std::mutex> lock(mut);
    while (!count) cv.wait(lock);
    count = 0;
  }

  void clear() {
    std::unique_lock<std::mutex> lock(mut);
    count = 0;
  }

  bool isSignalled() {
    std::unique_lock<std::mutex> lock(mut);
    return count != 0;
  }

private:
  std::mutex mut;
  std::condition_variable cv;
  int count = 0;
};

//--------------------------------------------------------------------------------

struct Throttle {

  bool is_paused() {
    std::unique_lock<std::mutex> lock(_mut);
    return _paused;
  }

  void pause() {
    std::unique_lock<std::mutex> lock(_mut);
    if (_paused) return;
    _break = true;
    while (!_paused) _cv.wait(lock);
    _break = false;
  }

  void resume() {
    std::unique_lock<std::mutex> lock(_mut);
    if (!_paused) return;
    _paused = 0;
    _resume = 1;
    lock.unlock();
    _cv.notify_one();
  }

  void wait_for_resume() {
    std::unique_lock<std::mutex> lock(_mut);
    _paused = 1;
    lock.unlock();
    _cv.notify_one();
    lock.lock();
    while (!_resume) _cv.wait(lock);
    _resume = 0;
  }

  std::condition_variable _cv;
  std::mutex _mut;
  std::atomic_bool _break = false;

  int _paused = 0;
  int _resume = 0;
};

//--------------------------------------------------------------------------------

struct GateBoyThread {

  void init();
  void reset();
  void start();
  void stop();

  void pause();
  void resume();
  void request_steps(int steps);

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

  StateStack<GateBoy> gb;

//private:

  bool isPaused() {
    return throttle.is_paused();
  }

  Assembler assembler;
  std::thread* main;
  std::atomic_int  sim_grain = 1;

  int steps_remaining = 0;

  std::atomic_bool exit = false;

  Throttle throttle;

  double  sim_time = 0;
  int64_t pass_count = 0;
  int64_t phase_count = 0;

  blob rom_buf;
};