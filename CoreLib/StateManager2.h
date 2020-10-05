#pragma once
#include "CoreLib/Types.h"

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
struct StateManager2 {
  StateManager2() { states.resize(1); }
  StateManager2(const StateManager2&) = delete;
  StateManager2& operator=(const StateManager2&) = delete;

  T* state()      { return &states.back(); }
  T* operator->() { return &states.back(); }

  size_t state_count() const      { return states.size(); }
  size_t state_size_bytes() const { return state_count() * sizeof(T); }

  void push() { states.push_back(states.back()); }
  void pop()  { if (states.size() > 1) { states.pop_back(); } }

  void reset() {
    states.clear();
    states.resize(1);
  }

  std::vector<T> states;
};

//-----------------------------------------------------------------------------
