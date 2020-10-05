#pragma once
#include "CoreLib/Types.h"

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
class StateManager2 {
public:

  StateManager2() {
    states.push_back(new T());
  }
  StateManager2(const StateManager2&) = delete;
  StateManager2& operator=(const StateManager2&) = delete;

  typedef std::function<void(T*, StepSize)> step_func;

  size_t state_count() const {
    return states.size();
  }

  size_t state_size_bytes() const {
    return state_count() * sizeof(T);
  }

  void reset() {
    for (auto s : states) delete s;
    states.clear();
    states.push_back(new T());
  }

  T* state() {
    return states.back();
  }

  T* operator->() {
    return states.back();
  }

  void push() {
    auto s = new T(*states.back());
    states.push_back(s);
  }

  void pop() {
    if (states.size() > 1) {
      delete states.back();
      states.pop_back();
    }
  }

  std::vector<T*> states;
};

//-----------------------------------------------------------------------------
