#include "Plait/Plait.h"

#include <iostream>
#include <fstream>

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

const char* Node::tag() const {
  if (cell) {
    return cell->tag.c_str();
  }
  else {
    return "<no_tag>";
  }
}

const char* Node::name() const {
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
    return "<no_cell>";
  }
}

const char* Node::gate() const {
  if (cell) {
    return cell->gate.c_str();
  }
  else {
    return "<no_gate>";
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Node::anchored_to(Node* target) {
  for (Node* cursor = anchor; cursor; cursor = cursor->anchor) {
    if (cursor == target) return true;
  }
  return false;
}

void Node::set_anchor(Node* new_anchor) {
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::save(const char* filename) {
  printf("Saving plait\n");
  std::ofstream out(filename);

  out << node_map.size() << std::endl;

  for (auto& [tag, node] : node_map) {
    out << tag << std::endl;
    out << node->locked << std::endl;
    out << (node->anchor ? node->anchor->tag() : "<no_anchor>") << std::endl;
    out << node->offset_old.x << std::endl;
    out << node->offset_old.y << std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::load(const char* filename) {
  printf("Loading plait\n");
  std::ifstream lines(filename);

  int node_count;
  lines >> node_count;
  printf("Node count %d\n", node_count);

  for (int i = 0; i < node_count; i++) {
    std::string tag;
    bool locked;
    std::string anchor_tag;
    dvec2 offset;

    lines >> tag;
    lines >> locked;
    lines >> anchor_tag;
    lines >> offset.x;
    lines >> offset.y;

    //printf("Tag: %s, offset {%f,%f}\n", tag.c_str(), offset_old.x, offset_old.y);

    auto it = node_map.find(tag);
    if (it != node_map.end()) {
      (*it).second->offset_old = offset;
      (*it).second->offset_new = offset;
      (*it).second->commit_pos();
      (*it).second->locked = locked;
    }
    else {
      printf("Did not recognize tag %s\n", tag.c_str());
    }

    if (anchor_tag != "<no_anchor>") {
      auto it2 = node_map.find(anchor_tag);
      if (it2 != node_map.end()) {
        (*it).second->anchor = (*it2).second;
      }
      else {
        printf("Did not recognize anchor tag %s\n", tag.c_str());
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
