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
    FILE* dump_file = fopen("dump.gb", "rb");
    fread(states.back(), 1, sizeof(T), dump_file);
    fclose(dump_file);
  }

  void save_dump() {
    FILE* dump_file = fopen("dump.gb", "wb");
    fwrite(state(), 1, sizeof(T), dump_file);
    fclose(dump_file);
  }

  void step() {
    auto s = new T(*states.back());
    states.push_back(s);
    step_callback(s);
  }

  void unstep() {
    if (states.size() == 1) return;
    delete states.back();
    states.pop_back();
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
