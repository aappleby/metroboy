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

  bool anchored_to(Node* target) {
    Node* cursor = anchor;
    while(cursor) {
      if (cursor == target) return true;
      cursor = cursor->anchor;
    }
    return false;
  }

  void set_anchor(Node* new_anchor) {
    if (new_anchor && new_anchor->anchored_to(this)) return;
    if (anchor) {
      offset_old += anchor->get_pos_old();
      offset_new += anchor->get_pos_new();
      anchor = nullptr;
    }
    if (new_anchor) {
      anchor = new_anchor;
      offset_old -= anchor->get_pos_old();
      offset_new -= anchor->get_pos_new();
    }
  }
  bool anchored() { return anchor != nullptr; }
  Node* get_anchor() { return anchor; }

  void commit_pos() {
    set_pos_old(get_pos_new());
  }

  void set_pos_old(dvec2 pos) {
    offset_old = pos;
    if (anchor) offset_old -= anchor->get_pos_old();
  }
  void set_pos_new(dvec2 pos) {
    offset_new = pos;
    if (anchor) offset_new -= anchor->get_pos_new();
  }

  dvec2 get_pos_old() { return anchor ? anchor->get_pos_old() + offset_old : offset_old; }
  dvec2 get_pos_new() { return anchor ? anchor->get_pos_new() + offset_new : offset_new; }

  Cell* cell = nullptr;
  int rank = 0;
  int mark = 0;

  bool locked = 0;

  uint32_t color = 0xFFFF00FF;

  std::vector<Node*> prev;
  std::vector<Node*> next;

private:
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


