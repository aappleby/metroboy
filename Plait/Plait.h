#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>

using namespace glm;

struct PlaitCell;
struct PlaitNode;
struct PlaitPath;
struct PlaitEdge;

struct PlaitPath {
  std::string cell_tag;
  std::string node_name;
  std::string port_name;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitPortIndex {
  PlaitNode* node;
  int port;
};

struct PlaitEdge {
  PlaitNode* prev_node;
  int prev_port;
  PlaitNode* next_node;
  int next_port;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitNode {
  PlaitNode(PlaitCell* _cell, const std::string& _name)
  :plait_cell(_cell), name(_name)
  {
  }

  std::string name;
  PlaitCell* plait_cell;

  bool locked = 0;
  dvec2 pos_abs = {0,0};
  dvec2 pos_rel = {0,0};
  std::string anchor_tag;
  PlaitNode* anchor = nullptr;
  bool pinned = 1;
  int  mark = 0;
  bool ghost = 0;
  bool selected = 0; // need this because we don't want a log(n) lookup per node per frame...
  dvec2 spring_force = {0,0};
  uint32_t color = 0xFFFF00FF;

  std::vector<std::string> prev_ports;
  std::vector<std::string> next_ports;

  std::vector<PlaitEdge*> prev_edges;
  std::vector<PlaitEdge*> next_edges;

  int find_prev_port(const std::string& port_name) {
    for (auto i = 0; i < prev_ports.size(); i++) {
      if (prev_ports[i] == port_name) return i;
    }
    return -1;
  }

  int find_next_port(const std::string& port_name) {
    for (auto i = 0; i < next_ports.size(); i++) {
      if (next_ports[i] == port_name) return i;
    }
    return -1;
  }

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
  PlaitCell(const DieCell* _cell) {
    die_cell = _cell;
  }

  PlaitNode* add_node(const std::string& name) {
    auto node = new PlaitNode(this, name);
    nodes.push_back(node);
    return node;
  }

  PlaitPortIndex find_prev_port(const std::string& port) {
    for (auto node : nodes) {
      int port_index = node->find_prev_port(port);
      if (port_index >= 0) return {node, port_index};
    }
    return {nullptr, -1};
  }

  PlaitPortIndex find_next_port(const std::string& port) {
    for (auto node : nodes) {
      int port_index = node->find_next_port(port);
      if (port_index >= 0) return {node, port_index};
    }
    return {nullptr, -1};
  }

  const char* tag() const  { return die_cell ? die_cell->tag.c_str()  : "<no_tag>"; }
  const char* name() const { return die_cell ? die_cell->name.c_str() : "<no_cell>"; }
  const char* gate() const { return die_cell ? die_cell->gate.c_str() : "<no_gate>"; }

  const DieCell* die_cell = nullptr;
  std::vector<PlaitNode*> nodes;
  //std::vector<PlaitCell*>  prev_cells;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void save_json(const char* filename);
  void load_json(const char* filename, DieDB& die_db);

  void split_node(PlaitNode* node);

  std::map<std::string, PlaitCell*> tag_to_cell;
  std::vector<PlaitEdge*> edges;

  PlaitCell* get_or_create_node(const std::string& tag, DieDB& die_db) {
    auto cell = die_db.tag_to_cell[tag];
    if (!cell) {
      printf("Tag \"%s\" not in cell db\n", tag.c_str());
      return nullptr;
    }

    auto node = tag_to_cell[tag];
    if (node) return node;

    node = new PlaitCell(cell);
    tag_to_cell[tag] = node;
    return node;
  }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
