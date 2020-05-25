#pragma once

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
class StateManager2 {
public:

  typedef std::function<void(T*)> step_func;

  StateManager2() {
    states.push_back(new T());
  }

  void set_step(step_func s) {
    step_callback = s;
  }

  T* state() {
    return states.back();
  }

  void load_dump() {
    clear_history();
    load_obj("dump.gb", states.back());
  }

  void save_dump() {
    save_obj("dump.gb", state());
  }

  void step(int count = 1) {
    for (int i = 0; i < count; i++) {
      auto s = new T(*states.back());
      states.push_back(s);
      step_callback(s);
    }
  }

  void unstep(int count = 1) {
    if (states.size() == 1) return;
    for (int i = 0; i < count; i++) {
      delete states.back();
      states.pop_back();
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
  std::vector<T*> states;

  StateManager2(const StateManager2&) = delete;
  StateManager2& operator=(const StateManager2&) = delete;
};

//-----------------------------------------------------------------------------
