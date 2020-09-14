#pragma once

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
class StateManager2 {
public:

  StateManager2() {}

  typedef std::function<void(T*)> step_func;

  size_t state_count() const {
    return states.size();
  }

  size_t state_size_bytes() const {
    return states.size() * sizeof(T);
  }

  void init(step_func _step, step_func _unstep) {
    states.push_back(new T());
    step_callback = _step;
    unstep_callback = _unstep;
  }

  void reset() {
    for (auto s : states) delete s;
    states.clear();
    states.push_back(new T());
  }

  T* state() {
    return states.back();
  }

  void step(int count = 1) {
    auto s = new T(*states.back());
    states.push_back(s);
    for (int i = 0; i < count; i++) {
      step_callback(s);
    }
  }

  void unstep(int count = 1) {
    if (states.size() == 1) return;
    for (int i = 0; i < count; i++) {
      delete states.back();
      states.pop_back();
      if (unstep_callback) {
        unstep_callback(states.back());
      }
    }
  }

  void clear_history() {
    auto old = states.back();
    states.pop_back();
    for (auto s : states) delete s;
    states.clear();
    states.push_back(old);
  }

//private:

  step_func step_callback = nullptr;
  step_func unstep_callback = nullptr;
  std::vector<T*> states;

  StateManager2(const StateManager2&) = delete;
  StateManager2& operator=(const StateManager2&) = delete;
};

//-----------------------------------------------------------------------------
