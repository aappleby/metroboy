#pragma once
#include "CoreLib/Types.h"

#include <vector>
#include <functional>

//-----------------------------------------------------------------------------

template<class T>
class StateManager2 {
public:

  StateManager2() {}

  typedef std::function<void(T*, StepSize)> step_func;

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

  void clear() {
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

  void step(int count, StepSize step_size) {
    auto s = new T(*states.back());
    states.push_back(s);
    for (int i = 0; i < count; i++) {
      if (step_callback) step_callback(s, step_size);
    }
  }

  void unstep(int count, StepSize step_size) {
    if (count == 0) return;
    if (states.size() == 1) return;

    while((states.size() > 1) && (count > 0)) {
      unstep_one(step_size);
      count--;
    }
  }

  void unstep_one(StepSize step_size) {
    delete states.back();
    states.pop_back();
    if (unstep_callback) {
      unstep_callback(states.back(), step_size);
    }
  }

  void load_megadump(FILE* f) {
    size_t state_count = 0;
    fread(&state_count, 1, sizeof(state_count), f);

    for (auto s : states) {
      delete s;
    }
    states.resize(state_count);

    for (size_t i = 0; i < state_count; i++) {
      states[i] = new T();
      fread(states[i], 1, sizeof(T), f);
      states[i]->check_sentinel();
    }

    printf("Loaded %zd states\n", state_count);
  }

  void save_megadump(FILE* f) {
    size_t state_count = states.size();
    fwrite(&state_count, 1, sizeof(state_count), f);

    for (size_t i = 0; i < state_count; i++) {
      fwrite(states[i], 1, sizeof(T), f);
    }

    printf("Saved %zd states\n", state_count);
  }

  void scan(std::function<bool(const T*)> lambda) {
    for (const auto& s : states) {
      if (!lambda(s)) return;
    }
  }

  void rev_scan(std::function<bool(const T*)> lambda) {
    for (auto i = states.size(); i--;) {
      if (!lambda(states[i])) return;
    }
  }

private:

  step_func step_callback = nullptr;
  step_func unstep_callback = nullptr;

  std::vector<T*> states;

  StateManager2(const StateManager2&) = delete;
  StateManager2& operator=(const StateManager2&) = delete;
};

//-----------------------------------------------------------------------------
