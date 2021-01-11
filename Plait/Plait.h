#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>
#include <iostream>

using namespace glm;

struct PlaitCell;
struct PlaitNode;
struct PlaitTrace;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void clear();

  void save_json(const char* filename);
  void save_json(std::ostream& stream);

  void load_json(const char* filename, DieDB& die_db);
  void load_json(std::istream& stream, DieDB& die_db);

  void split_node(PlaitNode* root_node);
  void merge_node(PlaitNode* root_node);
  void swap_anchors(PlaitNode* old_node, PlaitNode* new_node);
  void swap_edges  (PlaitNode* old_node, PlaitNode* new_prev, PlaitNode* new_next);

  void check_dead(PlaitNode* node);

  std::map<std::string, PlaitCell*>  cell_map;
  std::map<std::string, PlaitTrace*> trace_map;


  /*
  PlaitCell* get_or_create_node(const std::string& tag, DieDB& die_db) {
    auto cell = die_db.cell_map[tag];
    if (!cell) {
      printf("Tag \"%s\" not in cell db\n", tag.c_str());
      return nullptr;
    }

    auto node = cell_map[tag];
    if (node) return node;

    node = new PlaitCell();
    node->die_cell = cell;
    cell_map[tag] = node;
    return node;
  }
  */
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitCell {
  ~PlaitCell();

  int get_prev_port_index(const std::string& port_name) const {
    return die_cell->get_prev_port_index(port_name);
  }

  int get_next_port_index(const std::string& port_name) const {
    return die_cell->get_next_port_index(port_name);
  }

  const char* tag() const  { return die_cell ? die_cell->tag.c_str()  : "<no_tag>"; }
  const char* name() const { return die_cell ? die_cell->name.c_str() : "<no_cell>"; }
  const char* gate() const { return die_cell ? die_cell->gate.c_str() : "<no_gate>"; }

  PlaitNode* find_node(const std::string& name) const;

  // Serialized
  std::vector<PlaitNode*> nodes;

  // Not serialized
  DieCell* die_cell = nullptr;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitNode {

  //----------------------------------------
  // Serialized

  std::string name;
  bool locked = 0;
  dvec2 pos_abs = {0,0};
  dvec2 pos_rel = {0,0};
  std::string anchor_tag;
  int anchor_index = -1;

  //----------------------------------------
  // Not serialized

  PlaitCell* plait_cell = nullptr;
  PlaitNode* anchor = nullptr;
  bool pinned = 1;
  int  mark = 0;
  bool ghost = 0;
  bool selected = 0; // need this because we don't want a log(n) lookup per node per frame...
  dvec2 spring_force = {0,0};
  uint32_t color = 0xFFFF00FF;

  //----------------------------------------

  bool  anchored() { return anchor != nullptr; }
  bool  anchored_to(PlaitNode* target);
  const PlaitNode* get_anchor() { return anchor; }
  void  set_anchor(PlaitNode* new_anchor);

  void toggle_ghost()  { ghost = !ghost; }

  dvec2 get_pos_abs_new() const {
    if (!pinned) {
      return pos_abs;
    }
    else {
      return get_pos_anchor_abs_new() + pos_rel;
    }
  }

  dvec2 get_pos_rel_new() const {
    if (!pinned) {
      return pos_abs - get_pos_anchor_abs_new();
    }
    else {
      return pos_rel;
    }
  }

  dvec2 get_pos_abs_old() const {
    return get_pos_anchor_abs_old() + pos_rel;
  }

  dvec2 get_pos_rel_old() const {
    return pos_rel;
  }


  dvec2 get_pos_anchor_abs_new() const {
    return anchor ? anchor->get_pos_abs_new() : dvec2(0,0);
  }

  dvec2 get_pos_anchor_abs_old() const {
    return anchor ? anchor->get_pos_abs_old() : dvec2(0,0);
  }

  void move(dvec2 delta) {
    unpin();
    pos_abs += delta;
  }

  void set_pos_abs_new(dvec2 p) {
    pos_abs = p;
    pinned = false;
  }

  void commit_pos() {
    if (!pinned) {
      printf("commit_pos\n");
      pos_rel = pos_abs - get_pos_anchor_abs_new();
      pinned = true;
    }
  }

  void revert_pos() {
    if (!pinned) {
      printf("revert_pos\n");
      pos_abs = pos_rel + get_pos_anchor_abs_new();
      pinned = true;
    }
  }

  void unpin() {
    if (pinned) {
      pos_abs = pos_rel + get_pos_anchor_abs_new();
      pinned = false;
    }
  }

  void toggle_locked() { locked = !locked; }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitTrace {
  //----------------------------------------
  // Serialized

  std::string prev_node_name;
  std::string next_node_name;

  //----------------------------------------
  // Not serialized

  DieTrace* die_trace = nullptr;
  PlaitNode* prev_node = nullptr;
  PlaitNode* next_node = nullptr;

  int prev_port_index;
  int next_port_index;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
