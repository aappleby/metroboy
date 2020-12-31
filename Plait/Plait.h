#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"
//#include "protos/plait_wrapper.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <array>

using namespace glm;

//-----------------------------------------------------------------------------

struct Node {

  const char* name() const {
    if (cell) {
      if (cell->names.empty()) {
        return cell->tag.c_str();
      }
      else {
        const auto& it = cell->names.begin();
        return (*it).c_str();
      }
    }
    else {
      return "<no cell>";
    }
  }

  const char* gate() const {
    if (cell) {
      return cell->gate.c_str();
    }
    else {
      return "<no gate>";
    }
  }

  //----------------------------------------

  bool anchored_to(Node* target) {
    for (Node* cursor = anchor; cursor; cursor = cursor->anchor) {
      if (cursor == target) return true;
    }
    return false;
  }

  void set_anchor(Node* new_anchor) {
    // Ignore invalid anchors, or anchors that would create a loop.
    if (this == new_anchor) return;
    if (anchor == new_anchor) return;
    if (new_anchor && new_anchor->anchored_to(this)) return;

    // Unlink from old anchor.
    if (anchor) {
      offset_old += anchor->get_pos_old();
      offset_new += anchor->get_pos_new();
      anchor = nullptr;
    }

    // Link to new anchor.
    if (new_anchor) {
      anchor = new_anchor;
      offset_old -= anchor->get_pos_old();
      offset_new -= anchor->get_pos_new();
    }
  }

  bool  anchored() { return anchor != nullptr; }
  Node* get_anchor() { return anchor; }

  void set_pos_old(dvec2 pos) { offset_old = anchor ? pos - anchor->get_pos_old() : pos; }
  void set_pos_new(dvec2 pos) { offset_new = anchor ? pos - anchor->get_pos_new() : pos; }
  dvec2 get_pos_old() { return anchor ? anchor->get_pos_old() + offset_old : offset_old; }
  dvec2 get_pos_new() { return anchor ? anchor->get_pos_new() + offset_new : offset_new; }
  void commit_pos() {
    //printf("Committing %s\n", cell->tag.c_str());
    offset_old = offset_new;
  }
  void revert_pos() { offset_new = offset_old; }

  void toggle_lock() { locked = !locked; }

  //----------------------------------------

  int rank = 0;
  int mark = 0;

  bool locked = 0;
  bool selected = 0;

  uint32_t color = 0xFFFF00FF;

  std::vector<Node*> prev;
  std::vector<Node*> next;

  dvec2 spring_force = {0,0};

  const Cell* get_cell()            { return cell; }
  void        set_cell(Cell* _cell) { cell = _cell; }

private:
  Cell* cell = nullptr;
  Node* anchor = nullptr;
  dvec2 offset_old = {0,0};
  dvec2 offset_new = {0,0};

#if 0
  void update_rank1() {
    mark = 1;
    rank = 0;
    for(auto p : prev) {
      if (p->mark) continue;
      if (p->rank == -1) p->update_rank1();
      if (p->rank >= rank) {
        rank = p->rank + 1;
      }
    }
    mark = 0;
  }

  void update_rank2() {
    if (prev.empty() && next.empty()) {
      rank = 0;
    }
    else if (prev.empty()) {
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;
      rank = min_next - 1;
    }
    else if (next.empty()) {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      rank = max_prev + 1;
    }
    else {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;

      rank = (max_prev + min_next) / 2;
    }
  }
#endif
};

//-----------------------------------------------------------------------------

struct Frame {
  std::string name;
  dvec2 origin;
  dvec2 size;
  dvec2 scale;

  std::vector<Frame*> frames;
  std::vector<Node*>  nodes;
};

//-----------------------------------------------------------------------------

struct Plait {
  //Node* get_node(const std::string& name);
  //Node* get_or_create_node(const std::string& name);

  /*
  void update_rank() {
    for (auto n : nodes) {
      n->rank = -1;
      n->mark = 0;
    }
    for (auto n : nodes) {
      n->update_rank1();
    }
    for (auto n : nodes) {
      n->update_rank2();
    }
  }
  */

  //std::map<std::string, Cell*> cell_db;
  //Frame* root_frame;

  //NodeVec     nodes;
  //NameToNode  names;
  //NameToEdges edges;
  //std::set<Node*> roots;

  //typedef std::vector<Node*> NodeVec;
  //typedef std::map<std::string, Node*> NameToNode;
  //typedef std::multimap<std::string, std::string> NameToEdges;

  std::vector<Node*> nodes;
};


