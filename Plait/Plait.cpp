#include "Plait/Plait.h"

#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitTrace*& plait_trace) {
  plait_trace = new PlaitTrace();
  plait_trace->input_cell_name  = j["input_cell"];
  plait_trace->input_node_name  = j["input_node"];
  plait_trace->output_cell_name = j["output_cell"];
  plait_trace->output_node_name = j["output_node"];
}

void to_json(nlohmann::json& j, const PlaitTrace* plait_trace) {
  j["output_cell"]  = plait_trace->input_node->plait_cell->die_cell->tag;
  j["output_node"]  = plait_trace->input_node->name;
  j["input_cell"] = plait_trace->output_node->plait_cell->die_cell->tag;
  j["input_node"] = plait_trace->output_node->name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitNode*& plait_node) {
  plait_node = new PlaitNode();

  plait_node->pos_new.x    = j.value("pos_x", 0.0);
  plait_node->pos_new.y    = j.value("pos_y", 0.0);
  plait_node->pos_old = plait_node->pos_new;
}

void to_json(nlohmann::json& j, const PlaitNode* plait_node) {
  j["pos_x"]    = plait_node->pos_new.x;
  j["pos_y"]    = plait_node->pos_new.y;
}

void PlaitNode::dump(Dumper& d) {
  using namespace nlohmann;
  json j = this;
  d(j.dump(2).c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitCell*& plait_cell) {
  plait_cell = new PlaitCell();

  j["nodes"].get_to(plait_cell->nodes);
}

//----------------------------------------

void to_json(nlohmann::json& j, const PlaitCell* plait_cell) {
  j["nodes"] = plait_cell->nodes;
}

//--------------------------------------------------------------------------------

PlaitCell::~PlaitCell() {
  for (auto& [name, node] : nodes) {
    delete node;
  }
}

//--------------------------------------------------------------------------------

void PlaitCell::dump(Dumper& d) {
  using namespace nlohmann;
  json j = this;
  d("Cell %s:\n", die_cell->tag.c_str());
  d(j.dump(2).c_str());
  d("\n");
}

//--------------------------------------------------------------------------------

void PlaitCell::add_node(PlaitNode* node) {
  CHECK_N(node->plait_cell);
  CHECK_N(nodes[node->name]);
  nodes[node->name] = node;
  node->plait_cell = this;
}

//--------------------------------------------------------------------------------

PlaitNode* PlaitCell::find_node(const std::string& name) const {
  auto it = nodes.find(name);
  if (it == nodes.end()) {
    it = nodes.find("root");
    if (it == nodes.end()) {
      CHECK_P(false);
      return nullptr;
    }
    else {
      return (*it).second;
    }
  }
  else {
    return (*it).second;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------

void Plait::split_node(PlaitNode* root_node) {
  auto plait_cell = root_node->plait_cell;
  if (plait_cell->nodes.size() > 1) return;

  printf("Splitting cell %s\n", plait_cell->die_cell->tag.c_str());

  CHECK_P(root_node->name == "root");
  CHECK_P(plait_cell->nodes["root"] == root_node);

  root_node->pos_old.x -= 64;
  root_node->pos_new.x -= 64;

  auto new_output_node = new PlaitNode();
  new_output_node->name = "next";
  new_output_node->pos_new = root_node->pos_new;
  new_output_node->pos_old = root_node->pos_old;
  new_output_node->pos_old.x += 64;
  new_output_node->pos_new.x += 64;
  new_output_node->color = root_node->color;

  plait_cell->add_node(new_output_node);

  swap_output_edges(root_node, new_output_node);

  delete root_node;
}

//--------------------------------------------------------------------------------

void Plait::merge_node(PlaitNode* root_node) {
  auto plait_cell = root_node->plait_cell;
  if (plait_cell->nodes.size() == 1) return;

  std::map<std::string, PlaitNode*> old_nodes;
  old_nodes.swap(plait_cell->nodes);

  for (auto& [name, dead_node] : old_nodes) {
    if (dead_node == root_node) continue;
    //swap_edges(dead_node, root_node, root_node);

    swap_output_edges(dead_node, root_node);
    swap_input_edges(dead_node, root_node);

    check_dead(dead_node);
    delete dead_node;
  }

  root_node->name = "root";
  plait_cell->nodes[root_node->name] = root_node;
}

//--------------------------------------------------------------------------------

void Plait::swap_input_edges(PlaitNode* old_node, PlaitNode* new_node) {
  CHECK_P(old_node->plait_cell == new_node->plait_cell);

  for (auto& [trace_key, plait_trace] : trace_map) {
    if (plait_trace->input_node == old_node) {
      plait_trace->input_node_name = new_node->name;
      plait_trace->input_node = new_node;
    }
  }
}

void Plait::swap_output_edges(PlaitNode* old_node, PlaitNode* new_node) {
  CHECK_P(old_node->plait_cell == new_node->plait_cell);

  for (auto& [trace_key, plait_trace] : trace_map) {
    if (plait_trace->output_node == old_node) {
      plait_trace->output_node_name = new_node->name;
      plait_trace->output_node = new_node;
    }
  }
}

//--------------------------------------------------------------------------------

void Plait::check_dead(PlaitNode* dead_node) {
  for (auto& [tag, plait_cell] : cell_map) {
    for (auto& [name, node] : plait_cell->nodes) {
      CHECK_P(node != dead_node);
    }
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->input_node != dead_node);
    CHECK_P(plait_trace->output_node != dead_node);
  }
}

//--------------------------------------------------------------------------------

//----------------------------------------


void Plait::save_json(const char* filename) {
  printf("Saving plait %s\n", filename);
  std::ofstream stream(filename);
  save_json(stream);
}

//----------------------------------------


void Plait::save_json(std::ostream& stream) {
  using namespace nlohmann;

  // FIXME this shouldn't be here
  for (auto& [tag, cell] : cell_map) {
    for (auto& [name, node] : cell->nodes) {
      node->commit_pos();
    }
  }

  json jroot;

  jroot["cells"] = cell_map;
  jroot["traces"] = trace_map;

  stream << jroot.dump(2);
}

//--------------------------------------------------------------------------------

void Plait::load_json(const char* filename, DieDB& die_db) {
  printf("Loading plait %s\n", filename);
  std::ifstream stream(filename);
  load_json(stream, die_db);
}

//----------------------------------------

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

    for (auto& [name, node] : plait_cell->nodes) {
      node->name = name;
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
    plait_trace->input_node = prev_cell->find_node(plait_trace->input_node_name);
    plait_trace->output_node = next_cell->find_node(plait_trace->output_node_name);

    CHECK_P(plait_trace->input_node);
    CHECK_P(plait_trace->output_node);

    plait_trace->input_port_index = prev_cell->get_output_index(die_trace->prev_port);
    plait_trace->output_port_index = next_cell->get_input_index(die_trace->next_port);

    die_trace->plait_trace = plait_trace;
  }

  // Fill in missing cells.

  for (auto& [tag, die_cell] : die_db.cell_map) {
    if (die_cell->plait_cell) continue;

    printf("Did not load node for tag \"%s\", creating default node\n", tag.c_str());

    auto root_node = new PlaitNode();
    root_node->name = "root";

    auto new_cell = new PlaitCell();
    new_cell->die_cell = die_cell;
    new_cell->add_node(root_node);

    die_cell->plait_cell = new_cell;
    cell_map[tag] = new_cell;
  }

  // Fill in missing edges.

  for (auto& [trace_key, die_trace] : die_db.trace_map) {
    if (die_trace->plait_trace) continue;

    printf("Did not load plait trace for die trace \"%s\", creating default trace\n", die_trace->to_key().c_str());

    auto prev_cell = cell_map[die_trace->prev_tag];
    auto next_cell = cell_map[die_trace->next_tag];
    auto input_node = prev_cell->nodes["root"];
    auto output_node = next_cell->nodes["root"];
    auto input_port_index = prev_cell->get_output_index(die_trace->prev_port);
    auto output_port_index = next_cell->get_input_index(die_trace->next_port);

    CHECK_P(prev_cell);
    CHECK_P(next_cell);
    CHECK_P(input_node);
    CHECK_P(output_node);
    CHECK_P(input_port_index >= 0);
    CHECK_P(output_port_index >= 0);

    auto plait_trace = new PlaitTrace();

    plait_trace->input_node_name = input_node->name;
    plait_trace->output_node_name = output_node->name;
    plait_trace->die_trace = die_trace;
    plait_trace->input_node = input_node;
    plait_trace->output_node = output_node;
    plait_trace->input_port_index = prev_cell->get_output_index(die_trace->prev_port);
    plait_trace->output_port_index = next_cell->get_input_index(die_trace->next_port);

    die_trace->plait_trace = plait_trace;

    trace_map[die_trace->to_key()] = plait_trace;
  }

  // HACK merge everything

  for (auto& [tag, plait_cell] : cell_map) {
    merge_node(plait_cell->find_node("root"));
  }

  // Sanity checks

  for (auto& [tag, plait_cell] : cell_map) {
    // Every cell should have a root node.
    CHECK_P(plait_cell->nodes["root"]);

    // Every node should have a link to the cell.
    for (auto& [name, node] : plait_cell->nodes) {
      CHECK_P(node);
      CHECK_P(node->plait_cell == plait_cell);
    }
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->input_node);
    CHECK_P(plait_trace->output_node);
    //CHECK_P(plait_trace->next_node->name == "root");
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
