#pragma once

#include <vector>

//-----------------------------------------------------------------------------

template<typename T>
class StateManagerBase {
public:

  StateManagerBase() {
    current = new T();
  }

  T* operator->() {
    return current;
  }

  void clear() {
    clear_frame_history();
  }

  //----------------------------------------

  void push_frame() {
    clear_line_history();
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
    clear_phase_history();
    mb_cycle.push_back(current);
    current = new T();
    *current = *mb_cycle.back();
  }

  void push_phase() {
    mb_phase.push_back(current);
    current = new T();
    *current = *mb_phase.back();
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

  void pop_phase() {
    if (mb_phase.empty()) return;
    
    delete current;
    curernt = mb_phase.back();
    mb_phase.pop_back();
  }

  //----------------------------------------

  void clear_frame_history() {
    clear_line_history();
    for (auto m : mb_frame) delete m;
    mb_frame.clear();
  }

  void clear_line_history() {
    clear_cycle_history();
    for (auto m : mb_line) delete m;
    mb_line.clear();
  }

  void clear_cycle_history() {
    clear_phase_history();
    for (auto m : mb_cycle) delete m;
    mb_cycle.clear();
  }

  void clear_phase_history() {
    for (auto m : mb_phase) delete m;
    mb_phase.clear();
  }
 
  //----------------------------------------

  T* current;
  std::vector<T*> mb_frame;
  std::vector<T*> mb_line;
  std::vector<T*> mb_cycle;
};

//-----------------------------------------------------------------------------
