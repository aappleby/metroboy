#pragma once
#include "Gameboy.h"

#include <vector>

extern uint8_t rom_buf[];

//-----------------------------------------------------------------------------

template<typename T>
class StateManagerBase {
public:

  StateManagerBase::StateManagerBase() {
    current = new T();
  }

  void StateManagerBase::load_dump() {
    FILE* dump_file = nullptr;
    fopen_s(&dump_file, "dump.MetroBoy", "rb");
    size_t size = fread(current, 1, sizeof(T), dump_file);
    assert(size == sizeof(Gameboy));
    size = fread(rom_buf, 1, 1024 * 1024, dump_file);
    assert(size == 1024 * 1024);
    fclose(dump_file);
  }

  void StateManagerBase::save_dump() {
    FILE* dump_file = nullptr;
    fopen_s(&dump_file, "dump.MetroBoy", "wb");
    fwrite(current, 1, sizeof(T), dump_file);
    fwrite(rom_buf, 1, 1024 * 1024, dump_file);
    fclose(dump_file);
  }

  //----------------------------------------

  void push_frame() {
    clear_line_history();
    clear_cycle_history();

    mb_frame.push_back(current);
    current = new T();
    *current = *mb_frame.back();
  }

  void push_line() {
    clear_cycle_history();

    mb_line.push_back(current);
    current = new T();
    *current = *mb_line.back();
  }

  void push_cycle() {
    mb_cycle.push_back(current);
    current = new T();
    *current = *mb_cycle.back();
  }

  //----------------------------------------

  void pop_frame() {
    if (mb_frame.empty()) return;

    delete current;
    current = mb_frame.back();
    mb_frame.pop_back();
  }

  void pop_line() {
    if (mb_line.empty()) return;

    delete current;
    current = mb_line.back();
    mb_line.pop_back();
  }

  void pop_cycle() {
    if (mb_cycle.empty()) return;

    delete current;
    current = mb_cycle.back();
    mb_cycle.pop_back();
  }

  //----------------------------------------

  void clear() {
    clear_frame_history();
    clear_line_history();
    clear_cycle_history();
  }

  void clear_frame_history() {
    for (auto m : mb_frame) delete m;
    mb_frame.clear();
  }

  void clear_line_history() {
    for (auto m : mb_line) delete m;
    mb_line.clear();
  }

  void clear_cycle_history() {
    for (auto m : mb_cycle) delete m;
    mb_cycle.clear();
  }
 
  //----------------------------------------

protected:

  T* current;
  std::vector<T*> mb_frame;
  std::vector<T*> mb_line;
  std::vector<T*> mb_cycle;
};

//-----------------------------------------------------------------------------
