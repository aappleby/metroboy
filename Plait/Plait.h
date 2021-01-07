#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>

using namespace glm;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct NodeInstance {

  bool locked = 0;
  dvec2 pos_abs = {0,0};
  dvec2 pos_rel = {0,0};
  NodeInstance* anchor = nullptr;
  bool pinned = 1;
  const Cell* cell = nullptr;
  int  mark = 0;

  bool  anchored() { return anchor != nullptr; }
  bool  anchored_to(NodeInstance* target);
  const NodeInstance* get_anchor() { return anchor; }
  void  set_anchor(NodeInstance* new_anchor);

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

};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Node {
  Node(const Cell* _cell) {
    inst.cell = _cell;
  }

  const Cell* get_cell() {
    return inst.cell;
  }

  const char* tag() const;
  const char* name() const;
  const char* gate() const;

  void move(dvec2 delta) {
    unpin();
    inst.pos_abs += delta;
  }

  void set_pos_abs_new(dvec2 p) {
    inst.pos_abs = p;
    inst.pinned = false;
  }

  void commit_pos() {
    if (!inst.pinned) {
      printf("commit_pos\n");
      inst.pos_rel = inst.pos_abs - inst.get_pos_anchor_abs_new();
      inst.pinned = true;
    }
  }

  void revert_pos() {
    if (!inst.pinned) {
      printf("revert_pos\n");
      inst.pos_abs = inst.pos_rel + inst.get_pos_anchor_abs_new();
      inst.pinned = true;
    }
  }

  void unpin() {
    if (inst.pinned) {
      inst.pos_abs = inst.pos_rel + inst.get_pos_anchor_abs_new();
      inst.pinned = false;
    }
  }

  void toggle_locked() { inst.locked = !inst.locked; }
  void toggle_ghost()  { ghost = !ghost; }

  //----------------------------------------
  // Serialized state

  NodeInstance inst;

  //----------------------------------------
  // State from cell db

  std::vector<Node*>       prev_node;
  std::vector<std::string> prev_port;
  std::vector<Node*> next;

  //----------------------------------------
  // Sim/UI state

  bool ghost = 0;
  int  rank = 0;
  bool selected = 0; // need this because we don't want a log(n) lookup per node per frame...
  uint32_t color = 0xFFFF00FF;
  dvec2 spring_force = {0,0};
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void save_json(const char* filename);
  void load_json(const char* filename, CellDB& cell_db);

  std::map<std::string, Node*> node_map;

  Node* get_or_create_node(const std::string& tag, CellDB& cell_db) {
    auto cell = cell_db.tag_to_cell[tag];
    if (!cell) {
      printf("Tag \"%s\" not in cell db\n", tag.c_str());
      return nullptr;
    }

    auto node = node_map[tag];
    if (node) return node;

    node = new Node(cell);
    node_map[tag] = node;
    return node;
  }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
