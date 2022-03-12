#pragma once
#include <map>
#include <set>

#include "MtNode.h"
#include "Platform.h"

struct MtModule;
struct MtModLibrary;

//------------------------------------------------------------------------------

enum FieldState {
  CLEAN = 0,
  MAYBE = 1,
  DIRTY = 2,
  ERROR = 3,
};

inline const char* to_string(FieldState s) {
  switch (s) {
    case CLEAN:
      return "CLEAN";
    case MAYBE:
      return "MAYBE";
    case DIRTY:
      return "DIRTY";
    case ERROR:
      return "ERROR";
    default:
      return "INVALID";
  }
}

typedef std::map<std::string, FieldState> field_state_map;

//------------------------------------------------------------------------------

struct MtDelta {
  bool valid = false;
  bool error = false;
  field_state_map state_old;
  field_state_map state_new;

  void add_prefix(const std::string& p) {
    field_state_map temp_old;
    field_state_map temp_new;

    temp_old.swap(state_old);
    temp_new.swap(state_new);

    for (auto& n : temp_old) state_old[p + "." + n.first] = n.second;
    for (auto& n : temp_new) state_new[p + "." + n.first] = n.second;
  }

  void wipe() {
    valid = false;
    error = false;
    state_old.clear();
    state_new.clear();
  }

  void sanity_check() const {
    for (auto& s1 : state_old) {
      auto& s2 = *state_new.find(s1.first);
      assert(s1.second != ERROR);
      assert(s2.second != ERROR);
      if (s1.second == MAYBE) assert(s2.second != CLEAN);
      if (s1.second == DIRTY) assert(s2.second == DIRTY);
    }
  }
};

//------------------------------------------------------------------------------

struct MtMethod : public MtNode {
  MtMethod(MtNode n, MtModule* _mod);

  MtModule* mod = nullptr;
  MtModLibrary* lib = nullptr;
  std::string name;

  std::vector<std::string>* params = nullptr;
  MtDelta* delta = nullptr;

  bool is_tick = false;
  bool is_tock = false;

  void update_delta();

  void check_dirty_read_identifier(MtNode n, MtDelta& d);
  void check_dirty_read_submod(MtNode n, MtDelta& d);
  void check_dirty_write(MtNode n, MtDelta& d);
  void check_dirty_dispatch(MtNode n, MtDelta& d);
  void check_dirty_assign(MtNode n, MtDelta& d);
  void check_dirty_if(MtNode n, MtDelta& d);
  void check_dirty_call(MtNode n, MtDelta& d);
  void check_dirty_switch(MtNode n, MtDelta& d);
};

//------------------------------------------------------------------------------
