#pragma once
#include "CoreLib/Types.h"

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
struct StateStack {
  StateStack() { states.resize(1); }
  StateStack(const StateStack&) = delete;
  StateStack& operator=(const StateStack&) = delete;

  T* state()      { return &states.back(); }
  T* operator->() { return &states.back(); }

  size_t state_count() const      { return states.size(); }
  size_t state_size_bytes() const { return state_count() * sizeof(T); }

  void push() { states.push_back(states.back()); }
  void pop()  { if (states.size() > 1) { states.pop_back(); } }

  void reset_history() {
    states.resize(1);
  }

  std::vector<T> states;
};

//-----------------------------------------------------------------------------

template<class T>
struct StatePointerStack {
  StatePointerStack(T* prototype) {
    this->prototype = prototype;
    states.push_back(prototype->clone());
  }
  StatePointerStack(const StatePointerStack&) = delete;
  StatePointerStack& operator=(const StatePointerStack&) = delete;

  T* state()      { return states.back(); }
  T* operator->() { return states.back(); }

  size_t state_count() const      { return states.size(); }
  size_t state_size_bytes() const { return states.size() * prototype->size_bytes(); }

  void push() { states.push_back(states.back()->clone()); }
  void pop()  { if (states.size() > 1) { delete states.back(); states.pop_back(); } }

  void reset_history() {
    for (auto s : states) delete s;
    states.clear();
    states.push_back(prototype->clone());
  }

  T* prototype;
  std::vector<T*> states;
};

//-----------------------------------------------------------------------------
