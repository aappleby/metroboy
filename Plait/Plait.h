#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>

using namespace glm;

struct PlaitCell;

struct PlaitPath {
  std::string cell_tag;
  std::string node_name;
  std::string port_name;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitNode {

  bool locked = 0;
  dvec2 pos_abs = {0,0};
  dvec2 pos_rel = {0,0};
  PlaitNode* anchor = nullptr;
  bool pinned = 1;
  int  mark = 0;
  bool ghost = 0;
  bool selected = 0; // need this because we don't want a log(n) lookup per node per frame...
  dvec2 spring_force = {0,0};
  uint32_t color = 0xFFFF00FF;
  std::vector<PlaitNode*>       prev_node;
  std::vector<std::string> prev_port;
  PlaitCell* group;

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

struct PlaitCell {
  PlaitCell(const Cell* _cell) {
    cell = _cell;
  }

  PlaitNode* add_node() {
    auto node = new PlaitNode();
    node->group = this;
    nodes.push_back(node);
    return node;
  }

  const char* tag() const  { return cell ? cell->tag.c_str()  : "<no_tag>"; }
  const char* name() const { return cell ? cell->name.c_str() : "<no_cell>"; }
  const char* gate() const { return cell ? cell->gate.c_str() : "<no_gate>"; }

  const Cell* cell = nullptr;
  std::vector<PlaitNode*> nodes;
  std::vector<PlaitCell*>  prev_group;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void save_json(const char* filename);
  void load_json(const char* filename, CellDB& cell_db);

  std::map<std::string, PlaitCell*> tag_to_group;

  PlaitCell* get_or_create_node(const std::string& tag, CellDB& cell_db) {
    auto cell = cell_db.tag_to_cell[tag];
    if (!cell) {
      printf("Tag \"%s\" not in cell db\n", tag.c_str());
      return nullptr;
    }

    auto node = tag_to_group[tag];
    if (node) return node;

    node = new PlaitCell(cell);
    tag_to_group[tag] = node;
    return node;
  }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
