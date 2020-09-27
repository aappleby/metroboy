#pragma once

#include <vector>

//-----------------------------------------------------------------------------

template<typename T>
class StateStack {
public:

  StateStack() {
    states.push_back(new T());
  }

  T* top() {
    return states.back();
  }

  T* operator->() {
    return top();
  }

  size_t state_size_bytes() {
    return states.size() * sizeof(T);
  }

  void push() {
    states.push_back(new T(*top()));
  }

  void pop() {
    if (states.size() > 1) {
      delete states.back();
      states.pop_back();
    }
  }

  void clear_history() {
    T* temp = states.back();
    states.pop_back();
    for (auto m : states) delete m;
    states.clear();
    states.push_back(temp);
  }

  std::vector<T*> states;
};

//-----------------------------------------------------------------------------
