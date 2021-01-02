#include "Plait/Plait.h"

#include <iostream>
#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

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
  for (auto cursor = anchor; cursor; cursor = cursor->anchor) {
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

void Plait::load(const char* filename, CellDB& cell_db) {
  printf("Loading plait\n");
  std::ifstream lines(filename);

  CHECK_P(node_map.empty());

  int node_count;
  lines >> node_count;
  printf("Node count %d\n", node_count);

  std::map<std::string, std::string> anchor_map;

  for (int i = 0; i < node_count; i++) {
    std::string tag;
    bool locked;
    std::string anchor_tag;
    dvec2 offset = {0,0};

    // Deserialize fields
    lines >> tag;
    lines >> locked;
    lines >> anchor_tag;
    lines >> offset.x;
    lines >> offset.y;

    if (!anchor_tag.empty() && (anchor_tag != "<no_anchor>")) {
      anchor_map[tag] = anchor_tag;
    }

    // Initialize node
    auto cell = cell_db.tag_to_cell[tag];
    if (cell == nullptr) {
      printf("Did not recognize cell tag %s\n", tag.c_str());
      continue;
    }

    auto node = new Node(cell);
    node_map[tag] = node;

    node->locked = locked;
    node->offset_old = offset;
    node->offset_new = offset;
  }

  // Connect anchors
  printf("anchor count %zd\n", anchor_map.size());
  //for (auto& [tag, anchor_tag] : anchor_map) printf("tag %s -> anchor %s\n", tag.c_str(), anchor_tag.c_str());
  //for (auto& [tag, node] : node_map) printf("tag %s -> anchor %p\n", tag.c_str(), node->anchor);

  for (auto& [tag, anchor_tag] : anchor_map) {
    if (tag == anchor_tag) {
      printf("node %s anchored to itself?", tag.c_str());
      continue;
    }

    auto node = node_map[tag];
    auto anchor = node_map[anchor_tag];

    if (node && anchor) {
      node->anchor = anchor;
    }
    else if (node == nullptr) {
      printf("bad tag %s\n", tag.c_str());
    }
    else if (anchor == nullptr) {
      printf("bad anchor tag %s\n", anchor_tag.c_str());
    }
  }

#if 0
  // Check for missing tags
  for (auto& [tag, cell] : cell_db.tag_to_cell) {
    if (node_map.count(tag) == 0) {
      printf("Did not load node for tag \"%s\", creating placeholder\n", tag.c_str());
      node_map[tag] = new Node(cell);
    }
  }
#endif

  // Connect ports
  for (auto& [tag, node] : node_map) {
    for (auto& port : node->cell->args) {
      auto prev = node_map[port.tag];
      if (prev == nullptr) {
        printf("Did not recognize arg tag %s\n", tag.c_str());
      }
      else {
        node->prev.push_back(prev);
        prev->next.push_back(node);
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::save_json(const char* filename) {
  printf("Saving plait %s\n", filename);
  using namespace nlohmann;

  json root;

  for (auto& [tag, node] : node_map) {
    auto& jnode = root[tag];
    jnode["hidden"]     = node->hidden;
    jnode["locked"]     = node->locked;
    jnode["offset_x"]   = node->offset_old.x;
    jnode["offset_y"]   = node->offset_old.y;
    if (node->anchor) jnode["anchor_tag"] = node->anchor->cell->tag;
  }

  std::ofstream(filename) << root.dump(2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::load_json(const char* filename, CellDB& cell_db) {
  printf("Loading plait %s\n", filename);
  using namespace nlohmann;

  CHECK_P(node_map.empty());

  json root;
  std::ifstream(filename) >> root;

  for (auto& it : root.items()) {
    auto& tag = it.key();
    auto& jnode = it.value();

    auto cell = cell_db.tag_to_cell[tag];
    if (cell == nullptr) {
      printf("Did not recognize cell tag %s\n", tag.c_str());
      continue;
    }

    auto node = new Node(cell);
    node_map[tag] = node;

    jnode["hidden"]  .get_to(node->hidden);
    jnode["locked"]  .get_to(node->locked);
    jnode["offset_x"].get_to(node->offset_old.x);
    jnode["offset_y"].get_to(node->offset_old.y);
    node->offset_new = node->offset_old;
  }

#if 0
  // Check for missing tags
  for (auto& [tag, cell] : cell_db.tag_to_cell) {
    if (node_map.count(tag) == 0) {
      printf("Did not load node for tag \"%s\", creating placeholder\n", tag.c_str());
      node_map[tag] = new Node(cell);
    }
  }
#endif

  // Connect anchors
  for (auto& [tag, jnode] : root.items()) {
    if (!jnode["anchor_tag"].is_string()) continue;
    const auto& anchor_tag = jnode["anchor_tag"].get<std::string>();

    if (tag == anchor_tag) {
      printf("node %s anchored to itself?\n", tag.c_str());
    }

    auto node = node_map[tag];
    auto anchor = node_map[anchor_tag];

    if (node && anchor) {
      node->anchor = anchor;
    }
    else if (node == nullptr) {
      printf("bad tag %s\n", tag.c_str());
    }
    else if (anchor == nullptr) {
      printf("bad anchor tag %s\n", anchor_tag.c_str());
    }
  }

  // Connect ports
  for (auto& [tag, node] : node_map) {
    for (auto& port : node->cell->args) {
      auto prev = node_map[port.tag];

      if (prev == nullptr) {
        printf("Did not recognize arg tag %s\n", tag.c_str());
      }
      else {
        node->prev.push_back(prev);
        prev->next.push_back(node);
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
