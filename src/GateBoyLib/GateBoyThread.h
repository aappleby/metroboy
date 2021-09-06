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
    gb_a->wipe();
    gb_b->wipe();
  }

  void start();
  void stop();
  void pause();
  void resume();
  bool paused() { return sig_break; }

  //void set_cart(const blob& new_cart_blob);

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

  void load_raw_dump(const char* filename) {
    LOG_B("Loading raw dump from %s\n", filename);

    pause();
    clear_work();
    blob raw_dump = ::load_blob(filename);

    int gb_size = gb_a->from_blob(raw_dump);
    int cart_size = (int)raw_dump.size() - gb_size;

    if (config_regression) gb_b->from_blob(raw_dump);

    cart_blob.resize(cart_size);
    memcpy(cart_blob.data(), raw_dump.data() + gb_size, cart_size);

    resume();
  }

  void save_raw_dump(const char* filename) {
    LOG_B("Saving raw dump to %s\n", filename);

    pause();
    clear_work();
    blob raw_dump;
    gb_a->to_blob(raw_dump);
    raw_dump.insert(raw_dump.end(), get_cart().begin(), get_cart().end());
    save_blob(filename, raw_dump);
    resume();
  }

  void load_blob(const blob& _cart_blob) {
    pause();
    cart_blob = _cart_blob;
    reset_to_cart();
    resume();
  }

  void load_rom(const char* filename) {
    pause();
    clear_work();

    LOG_B("Loading %s\n", filename);

    cart_blob = ::load_blob(filename);

    LOG_B("GateBoyApp::load_rom cart_blob %p %d\n", cart_blob.data(), (int)cart_blob.size());

    reset_to_cart();

    LOG_B("Loaded %zd bytes from rom %s\n", cart_blob.size(), filename);
    resume();
  }


  // Load a flat (just raw contents of memory addresses, no individual regs) dump
  // and copy it into the various regs and memory chunks.

  void load_flat_dump(const char* filename) {
    cart_blob = ::load_blob(filename);
    reset_to_cart();

    memcpy(gb_a->vid_ram,  cart_blob.data() + 0x8000, 8192);
    memcpy(gb_a->cart_ram, cart_blob.data() + 0xA000, 8192);
    memcpy(gb_a->int_ram,  cart_blob.data() + 0xC000, 8192);
    memcpy(gb_a->oam_ram,  cart_blob.data() + 0xFE00, 256);
    memcpy(gb_a->zero_ram, cart_blob.data() + 0xFF80, 128);

    gb_a->dbg_write(get_cart(), ADDR_BGP,  cart_blob[ADDR_BGP]);
    gb_a->dbg_write(get_cart(), ADDR_OBP0, cart_blob[ADDR_OBP0]);
    gb_a->dbg_write(get_cart(), ADDR_OBP1, cart_blob[ADDR_OBP1]);
    gb_a->dbg_write(get_cart(), ADDR_SCY,  cart_blob[ADDR_SCY]);
    gb_a->dbg_write(get_cart(), ADDR_SCX,  cart_blob[ADDR_SCX]);
    gb_a->dbg_write(get_cart(), ADDR_WY,   cart_blob[ADDR_WY]);
    gb_a->dbg_write(get_cart(), ADDR_WX,   cart_blob[ADDR_WX]);
    gb_a->dbg_write(get_cart(), ADDR_LCDC, cart_blob[ADDR_LCDC]);

    if (config_regression) memcpy(gb_b.state(), gb_a.state(), sizeof(GateBoy));
  }

  void set_buttons(uint8_t buttons) {
    pause();
    gb_a->sys_buttons = buttons;;
    gb_b->sys_buttons = buttons;;
    resume();
  }

  void dump(Dumper& d);

  StateStack<GateBoy> gb_a;
  StateStack<GateBoy> gb_b;

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
