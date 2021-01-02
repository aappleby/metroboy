#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>

using namespace glm;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Node {

  const Cell* get_cell()            { return cell; }
  void        set_cell(Cell* _cell) { cell = _cell; }

  const char* tag() const;
  const char* name() const;
  const char* gate() const;

  bool  anchored() { return anchor != nullptr; }
  bool  anchored_to(Node* target);
  Node* get_anchor() { return anchor; }
  void  set_anchor(Node* new_anchor);

  void set_pos_old(dvec2 pos) { offset_old = anchor ? pos - anchor->get_pos_old() : pos; }
  void set_pos_new(dvec2 pos) { offset_new = anchor ? pos - anchor->get_pos_new() : pos; }

  dvec2 get_pos_old() { return anchor ? anchor->get_pos_old() + offset_old : offset_old; }
  dvec2 get_pos_new() { return anchor ? anchor->get_pos_new() + offset_new : offset_new; }

  void commit_pos() { offset_old = offset_new; }
  void revert_pos() { offset_new = offset_old; }

  void toggle_lock() { locked = !locked; }

  //----------------------------------------
  // Serialized state

  bool hidden = 0;
  bool locked = 0;
  Node* anchor = nullptr;
  dvec2 offset_old = {0,0};
  dvec2 offset_new = {0,0};

  //----------------------------------------
  // State from cell db

  Cell* cell = nullptr;
  std::vector<Node*> prev;
  std::vector<Node*> next;

  //----------------------------------------
  // Sim/UI state

  int  rank = 0;
  int  mark = 0;
  bool selected = 0;
  uint32_t color = 0xFFFF00FF;
  dvec2 spring_force = {0,0};
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void save(const char* filename);
  void load(const char* filename);

  std::map<std::string, Node*> node_map;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
