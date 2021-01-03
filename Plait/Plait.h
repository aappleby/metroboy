#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>

using namespace glm;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Node {
  Node(const Cell* _cell) : cell(_cell) {}

  const Cell* get_cell()            { return cell; }

  const char* tag() const;
  const char* name() const;
  const char* gate() const;

  bool  anchored() { return anchor != nullptr; }
  bool  anchored_to(Node* target);
  const Node* get_anchor() { return anchor; }
  void  set_anchor(Node* new_anchor);

  void set_pos_old(dvec2 pos) { offset_old = anchor ? pos - anchor->get_pos_old() : pos; }
  void set_pos_new(dvec2 pos) { offset_new = anchor ? pos - anchor->get_pos_new() : pos; }

  dvec2 get_pos_old() const { return anchor ? anchor->get_pos_old() + offset_old : offset_old; }
  dvec2 get_pos_new() const { return anchor ? anchor->get_pos_new() + offset_new : offset_new; }

  void commit_pos() { offset_old = offset_new; }
  void revert_pos() { offset_new = offset_old; }

  void toggle_locked() { locked = !locked; }
  void toggle_ghost()  { ghost = !ghost; }

  //----------------------------------------
  // Serialized state

  bool ghost = 0;
  bool locked = 0;
  dvec2 offset_old = {0,0};
  dvec2 offset_new = {0,0};
  const Node* anchor = nullptr;

  //----------------------------------------
  // State from cell db

  const Cell* cell = nullptr;
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
