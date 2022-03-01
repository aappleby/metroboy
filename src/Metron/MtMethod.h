#pragma once
#include "Platform.h"

#include "MtNode.h"
#include <set>
#include <map>

struct MtModule;
struct MtModLibrary;
typedef std::set<std::string> name_set;

//------------------------------------------------------------------------------

struct MtMethod : public MtNode {
  MtMethod(MtNode n, MtModule* _mod);

  MtModule* mod;
  MtModLibrary* lib;
  std::string name;
  std::vector<std::string> params;
  bool dirty_check_done = false;
  bool dirty_check_fail = false;
  name_set always_dirty;
  name_set maybe_dirty;
  bool is_tick = false;
  bool is_tock = false;

  void check_dirty();
  void check_dirty_read_identifier    (MtNode n);
  void check_dirty_read_submod(MtNode n);
  void check_dirty_write   (MtNode n);
  void check_dirty_dispatch(MtNode n);
  void check_dirty_assign  (MtNode n);
  void check_dirty_if      (MtNode n);
  void check_dirty_call    (MtNode n);
  void check_dirty_switch  (MtNode n);
};

//------------------------------------------------------------------------------
