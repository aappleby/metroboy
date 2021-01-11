#include "Plait/Plait.h"

#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitNode::anchored_to(PlaitNode* target) {
  for (auto cursor = anchor; cursor; cursor = cursor->anchor) {
    if (cursor == target) return true;
  }
  return false;
}

void PlaitNode::set_anchor(PlaitNode* new_anchor) {
  CHECK_P(new_anchor == nullptr || pinned);
  CHECK_P(new_anchor == nullptr || new_anchor->pinned);

  // Ignore invalid anchors, or anchors that would create a loop.
  if (this == new_anchor) return;
  if (anchor == new_anchor) return;
  if (new_anchor && new_anchor->plait_cell == plait_cell) return;
  if (new_anchor && new_anchor->anchored_to(this)) return;

  // Unlink from old anchor.
  if (anchor) {
    pos_rel += anchor->get_pos_abs_old();
    anchor = nullptr;
    anchor_tag.clear();
  }

  // Link to new anchor.
  if (new_anchor) {
    pos_rel -= new_anchor->get_pos_abs_old();
    anchor = new_anchor;
    anchor_tag = new_anchor->plait_cell->tag();
  }
}

PlaitNode* PlaitCell::find_node(const std::string& name) const {
  for(auto node : nodes) {
    if (node->name == name) return node;
  }
  return nullptr;
}

void Plait::split_node(PlaitNode* root_node) {
  auto plait_cell = root_node->plait_cell;
  CHECK_N(plait_cell->nodes.empty());
  if (plait_cell->nodes.size() > 1) return;
  if (plait_cell->nodes[0]->name != "default") return;

  printf("Splitting cell %s\n", plait_cell->die_cell->tag.c_str());

  root_node->set_anchor(nullptr);

  auto node_prev = new PlaitNode();
  auto node_next = new PlaitNode();

  node_prev->name = "prev";
  node_prev->pos_abs = root_node->pos_abs;
  node_prev->pos_rel = root_node->pos_rel;
  node_prev->pos_rel.x += 64;
  node_prev->pos_abs.x += 64;
  node_prev->plait_cell = root_node->plait_cell;
  node_prev->color = root_node->color;

  node_next->name = "next";
  node_next->pos_abs = root_node->pos_abs;
  node_next->pos_rel = root_node->pos_rel;
  node_next->pos_rel.x -= 64;
  node_next->pos_abs.x -= 64;
  node_next->plait_cell = root_node->plait_cell;
  node_next->color = root_node->color;

  plait_cell->nodes.clear();
  plait_cell->nodes.push_back(node_prev);
  plait_cell->nodes.push_back(node_next);

  swap_edges(root_node, node_prev, node_next);
  swap_anchors(root_node, nullptr);

  delete root_node;
}

void Plait::swap_anchors(PlaitNode* old_node, PlaitNode* new_node) {
  for (auto& [tag, plait_cell] : cell_map) {
    for (auto node : plait_cell->nodes) {
      if (node->anchor == old_node) {
        node->set_anchor(new_node);
      }
    }
  }
}

void Plait::swap_edges(PlaitNode* old_node, PlaitNode* new_prev, PlaitNode* new_next) {
  CHECK_P(old_node->plait_cell == new_prev->plait_cell);
  CHECK_P(old_node->plait_cell == new_next->plait_cell);

  for (auto& [trace_key, plait_trace] : trace_map) {
    if (plait_trace->prev_node == old_node) {
      plait_trace->prev_node_name = new_prev->name;
      plait_trace->prev_node = new_prev;
    }
    if (plait_trace->next_node == old_node) {
      plait_trace->next_node_name = new_next->name;
      plait_trace->next_node = new_next;
    }
  }
}

void Plait::merge_node(PlaitNode* root_node) {
  auto plait_cell = root_node->plait_cell;
  if (plait_cell->nodes.size() == 1) return;

  root_node->name = "default";

  auto old_nodes = plait_cell->nodes;
  plait_cell->nodes = {root_node};

  for (auto node : old_nodes) {
    if (node == root_node) continue;
    swap_anchors(node, root_node);
    swap_edges(node, root_node, root_node);

    check_dead(node);
    delete node;
  }
}

void Plait::check_dead(PlaitNode* dead_node) {
  for (auto& [tag, plait_cell] : cell_map) {
    for (auto node : plait_cell->nodes) {
      CHECK_P(node != dead_node);
      CHECK_P(node->anchor != dead_node);
    }
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->prev_node != dead_node);
    CHECK_P(plait_trace->next_node != dead_node);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

#if 0
  void update_rank1() {
    mark = 1;
    rank = 0;
    for(auto p : prev) {node
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

PlaitCell::~PlaitCell() {
  for (auto& node : nodes) delete node;
}

void Plait::clear() {
  for (auto& [tag, plait_cell] : cell_map) {
    delete plait_cell;
  }
  for (auto& [trace_key, plait_trace] : trace_map) {
    delete plait_trace;
  }

  cell_map.clear();
  trace_map.clear();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitNode*& node) {
  node = new PlaitNode();

  node->name         = j.value("name", "default");
  node->locked       = j.value("locked", false);
  node->pos_rel.x    = j.value("pos_rel_x", 0.0);
  node->pos_rel.y    = j.value("pos_rel_y", 0.0);
  node->pos_abs.x    = j.value("pos_abs_x", 0.0);
  node->pos_abs.y    = j.value("pos_abs_y", 0.0);
  node->anchor_tag   = j.value("anchor_tag", "");
  node->anchor_index = j.value("anchor_index", 0);
}

void from_json(const nlohmann::json& j, PlaitTrace*& trace) {
  trace = new PlaitTrace();

  trace->prev_node_name = j["prev_node_name"];
  trace->next_node_name = j["next_node_name"];
}

void from_json(const nlohmann::json& j, PlaitCell*& cell) {
  cell = new PlaitCell();
  j["nodes"].get_to(cell->nodes);
}

//----------------------------------------

void to_json(nlohmann::json& j, const PlaitNode* node) {
  j["name"]         = node->name;
  j["locked"]       = node->locked;
  j["pos_rel_x"]    = node->pos_rel.x;
  j["pos_rel_y"]    = node->pos_rel.y;
  j["pos_abs_x"]    = node->pos_abs.x;
  j["pos_abs_y"]    = node->pos_abs.y;
  j["anchor_tag"]   = node->anchor_tag;
  j["anchor_index"] = node->anchor_index;
}

void to_json(nlohmann::json& j, const PlaitTrace* trace) {
  j["prev_node_name"] = trace->prev_node->name;
  j["next_node_name"] = trace->next_node->name;
}

void to_json(nlohmann::json& j, const PlaitCell* cell) {
  j["nodes"] = cell->nodes;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::save_json(const char* filename) {
  printf("Saving plait %s\n", filename);
  std::ofstream stream(filename);
  save_json(stream);
}

void Plait::save_json(std::ostream& stream) {
  using namespace nlohmann;

  // FIXME this shouldn't be here
  for (auto& [tag, cell] : cell_map) {
    for (auto& node : cell->nodes) node->commit_pos();
  }

  json jroot;

  jroot["cells"] = cell_map;
  jroot["traces"] = trace_map;

  stream << jroot.dump(2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::load_json(const char* filename, DieDB& die_db) {
  printf("Loading plait %s\n", filename);
  std::ifstream stream(filename);
  load_json(stream, die_db);
}

void Plait::load_json(std::istream& stream, DieDB& die_db) {
  using namespace nlohmann;

  CHECK_P(cell_map.empty());

  json jroot;
  stream >> jroot;

  jroot["cells"].get_to(cell_map);

  jroot["traces"].get_to(trace_map);

  // Hook up plait_cell pointers.

  for (auto& [tag, plait_cell] : cell_map) {
    auto die_cell = die_db.cell_map[tag];
    CHECK_P(die_cell);
    plait_cell->die_cell = die_cell;
    die_cell->plait_cell = plait_cell;

    for (auto& node : plait_cell->nodes) {
      node->plait_cell = plait_cell;
    }
  }

  // Hook up plait_trace pointers.

  for (auto& [trace_key, plait_trace] : trace_map) {
    auto die_trace = die_db.trace_map[trace_key];
    CHECK_P(die_trace);

    auto prev_cell = cell_map[die_trace->prev_tag];
    auto next_cell = cell_map[die_trace->next_tag];

    plait_trace->die_trace = die_trace;
    plait_trace->prev_node = prev_cell->find_node(plait_trace->prev_node_name);
    plait_trace->next_node = next_cell->find_node(plait_trace->next_node_name);

    if (plait_trace->prev_node == nullptr) {
      printf("Fixing broken prev_node link for %s\n", die_trace->prev_tag.c_str());
      plait_trace->prev_node = prev_cell->nodes[0];
      plait_trace->prev_node_name = prev_cell->nodes[0]->name;
    }
    if (plait_trace->next_node == nullptr) {
      printf("Fixing broken next_node link for %s\n", die_trace->prev_tag.c_str());
      plait_trace->next_node = next_cell->nodes[0];
      plait_trace->next_node_name = next_cell->nodes[0]->name;
    }
    CHECK_P(plait_trace->prev_node);
    CHECK_P(plait_trace->next_node);

    plait_trace->prev_port_index = prev_cell->get_next_port_index(die_trace->prev_port);
    plait_trace->next_port_index = next_cell->get_prev_port_index(die_trace->next_port);

    die_trace->plait_trace = plait_trace;
  }

  // Hook up anchor pointers.

  for (auto& [tag, plait_cell] : cell_map) {
    for (auto& node : plait_cell->nodes) {
      if (node->anchor_tag.empty()) continue;
      auto anchor_cell = cell_map[node->anchor_tag];
      CHECK_P(anchor_cell);
      CHECK_P(node->anchor_index < anchor_cell->nodes.size());
      node->anchor = anchor_cell->nodes[node->anchor_index];
    }
  }

  // Fill in missing cells.

  for (auto& [tag, die_cell] : die_db.cell_map) {
    if (die_cell->plait_cell) continue;

    printf("Did not load node for tag \"%s\", creating default node\n", tag.c_str());

    auto plait_cell = new PlaitCell();
    plait_cell->die_cell = die_cell;

    PlaitNode* node = new PlaitNode();
    node->name = "default";

    plait_cell->nodes.push_back(node);

    die_cell->plait_cell = plait_cell;

    cell_map[tag] = plait_cell;
  }

  // Fill in missing edges.

  for (auto& [trace_key, die_trace] : die_db.trace_map) {
    if (die_trace->plait_trace) continue;

    printf("Did not load plait trace for die trace \"%s\", creating default trace\n", die_trace->to_key().c_str());

    auto prev_cell = cell_map[die_trace->prev_tag];
    auto next_cell = cell_map[die_trace->next_tag];
    auto prev_node = prev_cell->nodes[0];
    auto next_node = next_cell->nodes[0];
    auto prev_port_index = prev_cell->get_next_port_index(die_trace->prev_port);
    auto next_port_index = next_cell->get_prev_port_index(die_trace->next_port);

    CHECK_P(prev_cell);
    CHECK_P(next_cell);
    CHECK_P(prev_node);
    CHECK_P(next_node);
    CHECK_P(prev_port_index >= 0);
    CHECK_P(next_port_index >= 0);

    auto plait_trace = new PlaitTrace();

    plait_trace->prev_node_name = prev_node->name;
    plait_trace->next_node_name = next_node->name;
    plait_trace->die_trace = die_trace;
    plait_trace->prev_node = prev_node;
    plait_trace->next_node = next_node;
    plait_trace->prev_port_index = prev_cell->get_next_port_index(die_trace->prev_port);
    plait_trace->next_port_index = next_cell->get_prev_port_index(die_trace->next_port);

    die_trace->plait_trace = plait_trace;

    trace_map[die_trace->to_key()] = plait_trace;
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->prev_node);
    CHECK_P(plait_trace->next_node);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
