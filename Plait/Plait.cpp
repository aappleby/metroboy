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
  j["input_cell"]  = plait_trace->input_node->plait_cell->die_cell->tag;
  j["input_node"]  = plait_trace->input_node->name;
  j["output_cell"] = plait_trace->output_node->plait_cell->die_cell->tag;
  j["output_node"] = plait_trace->output_node->name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitNode*& plait_node) {
  plait_node = new PlaitNode();

  plait_node->name      = j.value("name", "<missing_node_name>");
  plait_node->pos_old.x = j.value("pos_x", 0.0);
  plait_node->pos_old.y = j.value("pos_y", 0.0);
  plait_node->pos_new = plait_node->pos_old;
}

void to_json(nlohmann::json& j, const PlaitNode* plait_node) {
  j["name"]  = plait_node->name;
  j["pos_x"] = plait_node->pos_new.x;
  j["pos_y"] = plait_node->pos_new.y;
}

void PlaitNode::dump(Dumper& d) {
  using namespace nlohmann;
  json j = this;
  d(j.dump(2).c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitCell*& plait_cell) {
  plait_cell = new PlaitCell();
  j["root"].get_to(plait_cell->root_node);
  j["leaves"].get_to(plait_cell->leaf_nodes);
}

//----------------------------------------

void to_json(nlohmann::json& j, const PlaitCell* plait_cell) {
  j["root"] = plait_cell->root_node;
  j["leaves"] = plait_cell->leaf_nodes;
}

//--------------------------------------------------------------------------------

PlaitCell::~PlaitCell() {
  delete root_node;
  for (auto& [name, leaf] : leaf_nodes) {
    delete leaf;
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

void PlaitCell::add_leaf_node(PlaitNode* node) {
  CHECK_N(node->plait_cell);
  CHECK_N(node->name == "root");
  CHECK_N(leaf_nodes[node->name]);

  leaf_nodes[node->name] = node;
  node->plait_cell = this;
}

//--------------------------------------------------------------------------------

PlaitNode* PlaitCell::find_leaf_node(const std::string& name) const {
  CHECK_N(name == "root");

  auto it = leaf_nodes.find(name);
  if (it == leaf_nodes.end()) {
    CHECK_P(false);
    return nullptr;
  }
  else {
    return (*it).second;
  }
}

//--------------------------------------------------------------------------------

PlaitNode* PlaitCell::spawn_leaf_node(uint32_t guid) {
  auto new_leaf = new PlaitNode();

  char buf[256];
  sprintf_s(buf, 256, "leaf_%08x", guid);

  new_leaf->name = buf;
  new_leaf->pos_new = root_node->pos_old + dvec2(128, 0);
  new_leaf->pos_old = root_node->pos_old + dvec2(128, 0);
  new_leaf->color   = root_node->color;

  leaf_nodes[new_leaf->name] = new_leaf;
  new_leaf->plait_cell = this;

  return new_leaf;
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

void Plait::split_node(PlaitNode* node) {
  auto plait_cell = node->plait_cell;
  if (!plait_cell->leaf_nodes.empty()) return;

  printf("Splitting cell %s\n", plait_cell->die_cell->tag.c_str());

  guid = mix(guid);

  auto new_leaf = plait_cell->spawn_leaf_node(guid);
  (void)new_leaf;
  //swap_output_edges(plait_cell->root_node, new_leaf);
}

//--------------------------------------------------------------------------------

void Plait::merge_node(PlaitNode* node) {
  auto plait_cell = node->plait_cell;
  auto root_node = plait_cell->root_node;

  if (plait_cell->leaf_nodes.empty()) return;

  std::map<std::string, PlaitNode*> dead_leaves;
  dead_leaves.swap(plait_cell->leaf_nodes);

  for (auto& [name, dead_leaf] : dead_leaves) {
    if (dead_leaf == root_node) continue;
    swap_output_edges(dead_leaf, root_node);
    check_dead(dead_leaf);
    delete dead_leaf;
  }
}

//--------------------------------------------------------------------------------

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

void Plait::check_dead(PlaitNode* dead_leaf) {
  for (auto& [tag, plait_cell] : cell_map) {
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      CHECK_P(leaf != dead_leaf);
    }
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->input_node  != dead_leaf);
    CHECK_P(plait_trace->output_node != dead_leaf);
  }
}

//--------------------------------------------------------------------------------

void Plait::save_json(const char* filename) {
  using namespace nlohmann;

  json jroot;

  jroot["cells"] = cell_map;
  jroot["traces"] = trace_map;
  jroot["guid"] = guid;

  std::ofstream stream(filename);
  stream << jroot.dump(2);
}

//----------------------------------------


void Plait::save_json(std::ostream& stream) {
  using namespace nlohmann;

  json jroot;
  jroot["cells"] = cell_map;
  jroot["traces"] = trace_map;

  stream << jroot.dump(2);
}

//--------------------------------------------------------------------------------

void Plait::load_json(const char* filename, DieDB& die_db) {
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
  guid = jroot.value("guid", 0x00000001);

  // Hook up plait_cell pointers.

  for (auto& [tag, plait_cell] : cell_map) {
    auto die_cell = die_db.cell_map[tag];
    CHECK_P(die_cell);
    plait_cell->die_cell = die_cell;
    die_cell->plait_cell = plait_cell;

    plait_cell->root_node->plait_cell = plait_cell;

    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      leaf->plait_cell = plait_cell;
    }
  }

  // Hook up plait_trace pointers.

  for (auto& [trace_key, plait_trace] : trace_map) {
    auto die_trace = die_db.trace_map[trace_key];
    CHECK_P(die_trace);

    auto output_cell = cell_map[die_trace->output_tag];
    auto input_cell  = cell_map[die_trace->input_tag];

    plait_trace->die_trace = die_trace;
    if (plait_trace->output_node_name == "root") {
      plait_trace->output_node = output_cell->root_node;
    }
    else {
      plait_trace->output_node = output_cell->find_leaf_node(plait_trace->output_node_name);
    }
    plait_trace->input_node  = input_cell->root_node;

    CHECK_P(plait_trace->input_node);
    CHECK_P(plait_trace->output_node);

    plait_trace->output_port_index = output_cell->get_output_index(die_trace->output_port);
    plait_trace->input_port_index  = input_cell->get_input_index(die_trace->input_port);

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
    new_cell->root_node = root_node;

    die_cell->plait_cell = new_cell;
    cell_map[tag] = new_cell;
  }

  // Fill in missing edges.

  for (auto& [trace_key, die_trace] : die_db.trace_map) {
    if (die_trace->plait_trace) continue;

    printf("Did not load plait trace for die trace \"%s\", creating default trace\n", die_trace->to_key().c_str());

    auto output_cell = cell_map[die_trace->output_tag];
    auto output_node = output_cell->root_node;
    auto output_port_index = output_cell->get_output_index(die_trace->output_port);

    auto input_cell = cell_map[die_trace->input_tag];
    auto input_node = input_cell->root_node;
    auto input_port_index = input_cell->get_input_index(die_trace->input_port);

    CHECK_P(output_cell);
    CHECK_P(output_node);
    CHECK_P(output_port_index >= 0);

    CHECK_P(input_cell);
    CHECK_P(input_node);
    CHECK_P(input_port_index >= 0);

    auto plait_trace = new PlaitTrace();

    plait_trace->output_cell_name = output_cell->tag();
    plait_trace->output_node_name = output_node->name;

    plait_trace->input_cell_name = input_node->name;
    plait_trace->input_node_name = input_node->name;

    plait_trace->die_trace = die_trace;
    plait_trace->output_node = output_node;
    plait_trace->input_node = input_node;

    plait_trace->output_port_index = output_cell->get_input_index(die_trace->output_port);
    plait_trace->input_port_index  = input_cell->get_output_index(die_trace->input_port);

    die_trace->plait_trace = plait_trace;

    trace_map[die_trace->to_key()] = plait_trace;
  }

  // Sanity checks

  for (auto& [tag, plait_cell] : cell_map) {
    // Every cell should have a root node.
    CHECK_P(plait_cell->root_node);
    CHECK_N(plait_cell->root_node->name.empty());
    CHECK_P(plait_cell->root_node->plait_cell == plait_cell);

    // Every node should have a link to the cell.
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      CHECK_P(leaf);
      CHECK_N(leaf->name.empty());
      CHECK_P(leaf->plait_cell == plait_cell);
    }
  }

  for (auto& [trace_key, plait_trace] : trace_map) {
    CHECK_P(plait_trace->die_trace);
    CHECK_P(plait_trace->output_node);
    CHECK_P(plait_trace->input_node);

    CHECK_P(plait_trace->output_cell_name == plait_trace->die_trace->output_tag);
    CHECK_P(plait_trace->output_node_name == plait_trace->output_node->name);

    CHECK_P(plait_trace->input_cell_name == plait_trace->die_trace->input_tag);
    CHECK_P(plait_trace->input_node_name == plait_trace->input_node->name);

    CHECK_P(plait_trace->die_trace->plait_trace == plait_trace);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
