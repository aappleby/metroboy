#include "Plait/Plait.h"

#include <fstream>

#pragma warning(disable:4996)

//---------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitLabel*& plait_label) {
  plait_label = new PlaitLabel();
  plait_label->text      = j["text"];
  plait_label->pos_old.x = j["pos_x"];
  plait_label->pos_old.y = j["pos_y"];
  plait_label->scale     = j["scale"];
  plait_label->pos_new = plait_label->pos_old;
}

void to_json(nlohmann::json& j, const PlaitLabel* plait_label) {
  j["text"]  = plait_label->text;
  j["pos_x"] = plait_label->pos_old.x;
  j["pos_y"] = plait_label->pos_old.y;
  j["scale"] = plait_label->scale;
}

//---------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitFrame*& plait_frame) {
  plait_frame = new PlaitFrame();
  plait_frame->title   = j.value("title", "<no title>");
  plait_frame->text    = j.value("text", "<no text>");
  plait_frame->pos.x   = j.value("pos_x", 0.0);
  plait_frame->pos.y   = j.value("pos_y", 0.0);
  plait_frame->size.x  = j.value("size_x", 64);
  plait_frame->size.y  = j.value("size_y", 16);
  plait_frame->color.r = j.value("color_r", 0.0f);
  plait_frame->color.g = j.value("color_g", 0.0f);
  plait_frame->color.b = j.value("color_b", 0.0f);
  plait_frame->color.a = j.value("color_a", 0.2f);
  plait_frame->text_scale = j.value("text_scale", 1);
}

void to_json(nlohmann::json& j, const PlaitFrame* plait_frame) {
  j["title"] = plait_frame->title;
  j["text"]  = plait_frame->text;
  j["pos_x"] = plait_frame->pos.x;
  j["pos_y"] = plait_frame->pos.y;
  j["size_x"] = plait_frame->size.x; 
  j["size_y"] = plait_frame->size.y;
  j["color_r"] = plait_frame->color.r;
  j["color_g"] = plait_frame->color.g;
  j["color_b"] = plait_frame->color.b;
  j["color_a"] = plait_frame->color.a;
  j["text_scale"] = plait_frame->text_scale;
}

//---------------------------------------------------------------------------------------------------------------------

/*
void from_json(const nlohmann::json& j, PlaitTrace*& plait_trace) {
  plait_trace = new PlaitTrace();
  plait_trace->output_cell_name = j["output_cell"];
  plait_trace->output_node_name = j["output_node"];
  if (j.contains("output_port")) plait_trace->output_port_name = j["output_port"];

  plait_trace->input_cell_name  = j["input_cell"];
  plait_trace->input_node_name  = j["input_node"];
  if (j.contains("input_port")) plait_trace->input_port_name = j["input_port"];
}
*/

/*
void to_json(nlohmann::json& j, const PlaitTrace* plait_trace) {
  j["output_cell"] = plait_trace->output_node->plait_cell->die_cell->tag;
  j["output_node"] = plait_trace->output_node->name;
  j["output_port"] = plait_trace->die_trace->output_port;
  j["input_cell"]  = plait_trace->input_node->plait_cell->die_cell->tag;
  j["input_node"]  = plait_trace->input_node->name;
  j["input_port"]  = plait_trace->die_trace->input_port_new;
}
*/

//---------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitNode*& plait_node) {
  plait_node = new PlaitNode();

  plait_node->name      = j.value("name", "<missing_node_name>");
  plait_node->pos_old.x = j.value("pos_x", 0.0);
  plait_node->pos_old.y = j.value("pos_y", 0.0);
  plait_node->old       = j.value("old", false);
  plait_node->global    = j.value("global", false);
  plait_node->pos_new = plait_node->pos_old;
}

void to_json(nlohmann::json& j, const PlaitNode* plait_node) {
  j["name"]   = plait_node->name;
  j["pos_x"]  = plait_node->pos_new.x;
  j["pos_y"]  = plait_node->pos_new.y;
  j["old"]    = plait_node->old;
  j["global"] = plait_node->global;
}

void PlaitNode::dump(Dumper& d) {
  using namespace nlohmann;
  json j = this;
  d(j.dump(2).c_str());
}

//---------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, PlaitCell*& plait_cell) {
  plait_cell = new PlaitCell();
  j["core"].get_to(plait_cell->core_node);
  if (j.contains("roots")) j["roots"].get_to(plait_cell->root_nodes);
  j["leaves"].get_to(plait_cell->leaf_nodes);
}

//----------------------------------------

void to_json(nlohmann::json& j, const PlaitCell* plait_cell) {
  j["core"] = plait_cell->core_node;
  j["roots"] = plait_cell->root_nodes;
  j["leaves"] = plait_cell->leaf_nodes;
}

//--------------------------------------------------------------------------------

PlaitCell::~PlaitCell() {
  delete core_node;
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

void PlaitCell::add_root_node(PlaitNode* node) {
  DCHECK_N(node->plait_cell);
  DCHECK_N(node->name == "core");
  DCHECK_N(root_nodes[node->name]);

  root_nodes[node->name] = node;
  node->plait_cell = this;
}

void PlaitCell::add_leaf_node(PlaitNode* node) {
  DCHECK_N(node->plait_cell);
  DCHECK_N(node->name == "core");
  DCHECK_N(leaf_nodes[node->name]);

  leaf_nodes[node->name] = node;
  node->plait_cell = this;
}

//--------------------------------------------------------------------------------

PlaitNode* PlaitCell::find_root_node(const std::string& name) const {
  if (name == "core") return core_node;

  auto it = root_nodes.find(name);
  if (it == root_nodes.end()) {
    //ASSERT_P(false);
    return nullptr;
  }
  else {
    return (*it).second;
  }
}

PlaitNode* PlaitCell::find_leaf_node(const std::string& name) const {
  if (name == "core") return core_node;

  auto it = leaf_nodes.find(name);
  if (it == leaf_nodes.end()) {
    //ASSERT_P(false);
    return nullptr;
  }
  else {
    return (*it).second;
  }
}

//--------------------------------------------------------------------------------

PlaitNode* PlaitCell::spawn_root_node(PlaitNode* neighbor, uint32_t guid) {
  auto new_root = new PlaitNode();

  char buf[256];
  sprintf(buf, "root_%08x", guid);

  new_root->name = buf;
  new_root->pos_new = neighbor->pos_old - dvec2(128, 0);
  new_root->pos_old = neighbor->pos_old - dvec2(128, 0);
  new_root->color   = core_node->color;

  root_nodes[new_root->name] = new_root;
  new_root->plait_cell = this;

  return new_root;
}

PlaitNode* PlaitCell::spawn_leaf_node(PlaitNode* neighbor, uint32_t guid) {
  auto new_leaf = new PlaitNode();

  char buf[256];
  sprintf(buf, "leaf_%08x", guid);

  new_leaf->name = buf;
  new_leaf->pos_new = neighbor->pos_old + dvec2(128, 0);
  new_leaf->pos_old = neighbor->pos_old + dvec2(128, 0);
  new_leaf->color   = core_node->color;

  leaf_nodes[new_leaf->name] = new_leaf;
  new_leaf->plait_cell = this;

  return new_leaf;
}

//---------------------------------------------------------------------------------------------------------------------

void Plait::clear() {
  for (auto& [tag, plait_cell] : cell_map) {
    delete plait_cell;
  }
  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {
    delete plait_trace;
  }

  cell_map.clear();
  traces.clear();
}

//---------------------------------------------------------------------------------------------------------------------

void Plait::spawn_root_node(PlaitNode* node) {
  node->plait_cell->spawn_root_node(node, new_guid());
}

void Plait::spawn_leaf_node(PlaitNode* node) {
  node->plait_cell->spawn_leaf_node(node, new_guid());
}

//--------------------------------------------------------------------------------

void Plait::delete_roots(PlaitNode* core_node) {
  if (core_node->name != "core") return;
  auto plait_cell = core_node->plait_cell;

  auto dead_roots = decltype(plait_cell->root_nodes)();
  dead_roots.swap(plait_cell->root_nodes);

  for (auto& [name, dead_root] : dead_roots) {
    swap_input_edges(dead_root, plait_cell->core_node);
    check_dead(dead_root);
    delete dead_root;
  }
}

void Plait::delete_leaves(PlaitNode* core_node) {
  if (core_node->name != "core") return;
  auto plait_cell = core_node->plait_cell;

  auto dead_leaves = decltype(plait_cell->leaf_nodes)();
  dead_leaves.swap(plait_cell->leaf_nodes);

  for (auto& [name, dead_leaf] : dead_leaves) {
    swap_output_edges(dead_leaf, plait_cell->core_node);
    check_dead(dead_leaf);
    delete dead_leaf;
  }
}

//--------------------------------------------------------------------------------

void Plait::link_nodes(PlaitNode* node_a, PlaitNode* node_b) {
  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {

    if (plait_trace->output_node->plait_cell == node_a->plait_cell) {
      if (plait_trace->input_node->plait_cell == node_b->plait_cell) {
        //plait_trace->output_node_name = node_a->name;
        plait_trace->output_node = node_a;
        //plait_trace->input_node_name = node_b->name;
        plait_trace->input_node = node_b;
      }
    }

    /*
    if (plait_trace->output_node->plait_cell == node_b->plait_cell) {
      if (plait_trace->input_node->plait_cell == node_a->plait_cell) {
        //plait_trace->output_node_name = node_b->name;
        plait_trace->output_node = node_b;
        //plait_trace->input_node_name = node_a->name;
        plait_trace->input_node = node_a;
      }
    }
    */
  }
}

//---------------------------------------------------------------------------------------------------------------------

void Plait::swap_input_edges(PlaitNode* old_node, PlaitNode* new_node) {
  CHECK_P(old_node->plait_cell == new_node->plait_cell);

  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {
    if (plait_trace->input_node == old_node) {
      //plait_trace->input_node_name = new_node->name;
      plait_trace->input_node = new_node;
    }
  }
}

void Plait::swap_output_edges(PlaitNode* old_node, PlaitNode* new_node) {
  CHECK_P(old_node->plait_cell == new_node->plait_cell);

  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {

    if (plait_trace->output_node == old_node) {
      //plait_trace->output_node_name = new_node->name;
      plait_trace->output_node = new_node;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------

void Plait::delete_node(PlaitNode* dead_node) {
  DCHECK_N(dead_node->name == "core");

  auto plait_cell = dead_node->plait_cell;
  plait_cell->root_nodes.erase(dead_node->name);
  plait_cell->leaf_nodes.erase(dead_node->name);

  swap_input_edges (dead_node, plait_cell->core_node);
  swap_output_edges(dead_node, plait_cell->core_node);
  check_dead(dead_node);
  delete dead_node;
}

//--------------------------------------------------------------------------------

void Plait::check_dead(PlaitNode* dead_leaf) {
  (void)dead_leaf;

  for (auto& [tag, plait_cell] : cell_map) {

    CHECK_P(dead_leaf != plait_cell->core_node);

    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      CHECK_P(leaf != dead_leaf);
    }
    for (auto& [name, leaf] : plait_cell->root_nodes) {
      CHECK_P(leaf != dead_leaf);
    }
  }

  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {
    (void)plait_trace;
    CHECK_P(plait_trace->input_node  != dead_leaf);
    CHECK_P(plait_trace->output_node != dead_leaf);
  }
}

//---------------------------------------------------------------------------------------------------------------------

void Plait::to_json(nlohmann::json& jroot) {

  jroot["cells"]  = cell_map;
  jroot["labels"] = labels;
  jroot["guid"]   = _guid;

  auto& jtraces = jroot["traces"];

  for (auto plait_trace : traces) {
    auto& j = jtraces[plait_trace->die_trace->to_key()];
    j["output_cell"] = plait_trace->output_node->plait_cell->die_cell->tag;
    j["output_node"] = plait_trace->output_node->name;
    j["output_port"] = plait_trace->die_trace->output_port;
    j["input_cell"]  = plait_trace->input_node->plait_cell->die_cell->tag;
    j["input_node"]  = plait_trace->input_node->name;
    j["input_port"]  = plait_trace->die_trace->input_port;
  }

  jroot["frames"] = frames;
}

//----------------------------------------

void Plait::from_json(nlohmann::json& jroot, DieDB& die_db) {
  using namespace nlohmann;

  CHECK_P(cell_map.empty());

  jroot["cells"] .get_to(cell_map);

  // Hook up plait_cell pointers.

  for (auto& [tag, plait_cell] : cell_map) {
    auto die_cell = die_db.cell_map[tag];
    if (die_cell == nullptr) {
      LOG_R("Could not find tag %s\n", tag.c_str());
    }
  }

  for (auto& [tag, plait_cell] : cell_map) {
    auto die_cell = die_db.cell_map[tag];
    CHECK_P(die_cell);
    plait_cell->die_cell = die_cell;
    die_cell->plait_cell = plait_cell;

    plait_cell->core_node->plait_cell = plait_cell;

    for (auto& [name, root] : plait_cell->root_nodes) {
      root->plait_cell = plait_cell;
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      leaf->plait_cell = plait_cell;
    }
  }

  //if (jroot.contains("traces_new")) jroot["traces_new"].get_to(trace_map_new);
  jroot["labels"].get_to(labels);
  jroot["guid"]  .get_to(_guid);

  if (jroot.contains("frames")) {
    jroot["frames"].get_to(frames);
  }

  if (frames.empty()) {
    auto frame = new PlaitFrame();
    frame->title = "Some Title";
    frame->text = "Placeholder frame text";
    frame->pos = dvec2(0, 0);
    frame->size = dvec2(128, 64);
    frame->text_scale = 2;
    frames.push_back(frame);
  }

  //----------------------------------------


  std::map<std::string, const DieTrace*> trace_map;
  std::map<const DieTrace*, bool> trace_used;

  for (auto& die_trace : die_db.traces) {
    trace_map[die_trace.to_key()] = &die_trace;
  }

#if 1
  for (auto& [trace_key, j] : jroot["traces"].items()) {
    std::string output_cell = j["output_cell"];
    std::string output_node = j["output_node"];
    std::string output_port;
    if (j.contains("output_port")) output_port = j["output_port"];

    std::string input_cell  = j["input_cell"];
    std::string input_node  = j["input_node"];
    std::string input_port;
    if (j.contains("input_port")) input_port = j["input_port"];


    const DieTrace* die_trace = nullptr;
    if (trace_map.contains(trace_key)) die_trace = trace_map[trace_key];

    /*
    if (die_trace == nullptr) {
      auto first_dot = trace_key.find('.');
      auto arrow = trace_key.find(" -> ");
      auto second_dot = trace_key.find('.', first_dot + 1);
      std::string trace_output_tag = trace_key.substr(0, first_dot - 0);
      std::string trace_output_port = trace_key.substr(first_dot + 1, arrow - (first_dot + 1));
      std::string trace_input_tag = trace_key.substr(arrow + 4, second_dot - (arrow + 4));
      std::string trace_input_port = trace_key.substr(second_dot + 1, trace_key.size() - (second_dot + 1));
      std::string new_key = trace_output_tag + ".out -> " + trace_input_tag + "." + trace_input_port;
      if (trace_map.contains(new_key)) die_trace = trace_map[new_key];
    }
    */

    //ASSERT_P(die_trace);
    if (!die_trace) continue;

    if (output_port.empty()) output_port = die_trace->output_port;
    if (input_port.empty()) input_port = die_trace->input_port;

    trace_used[die_trace] = true;
    auto plait_output_cell = cell_map[die_trace->output_tag];
    auto plait_input_cell  = cell_map[die_trace->input_tag];
    auto plait_trace = new PlaitTrace();

    plait_trace->die_trace         = die_trace;
    plait_trace->output_node       = plait_output_cell->find_leaf_node(output_node);
    plait_trace->input_node        = plait_input_cell->find_root_node(input_node);
    plait_trace->output_port_index = plait_output_cell->die_cell->get_output_index(die_trace->output_port);
    plait_trace->input_port_index  = plait_input_cell->die_cell->get_input_index(die_trace->input_port);

    if (!plait_trace->output_node || !plait_trace->input_node) {
      LOG_R("Could not link %s.%s.%s -> %s.%s.%s, resetting link\n",
        output_cell.c_str(),
        output_node.c_str(),
        output_port.c_str(),
        input_cell.c_str(),
        input_node.c_str(),
        input_port.c_str());

      plait_trace->output_node = plait_input_cell->core_node;
      plait_trace->input_node  = plait_input_cell->core_node;
    }

    CHECK_P(plait_trace->input_node);
    CHECK_P(plait_trace->output_node);

    traces.push_back(plait_trace);
  }
#endif

  // Fill in missing cells.

  for (auto& [tag, die_cell] : die_db.cell_map) {
    if (die_cell->plait_cell) continue;

    LOG_R("Did not load node for tag \"%s\", creating default node\n", tag.c_str());

    auto core_node = new PlaitNode();
    core_node->name = "core";

    auto new_cell = new PlaitCell();
    new_cell->die_cell = die_cell;
    new_cell->core_node = core_node;
    new_cell->core_node->plait_cell = new_cell;

    die_cell->plait_cell = new_cell;
    cell_map[tag] = new_cell;
  }

  // Fill in missing edges.

  //for (auto& [trace_key, die_trace] : die_db.trace_map_old) {
  for (auto& die_trace : die_db.traces) {
    if (trace_used[&die_trace]) continue;

    LOG_R("Did not load plait trace for die trace \"%s\", creating default trace\n", die_trace.to_key().c_str());

    auto output_cell = cell_map[die_trace.output_tag];
    auto output_node = output_cell->core_node;
    auto output_port_index = output_cell->die_cell->get_output_index(die_trace.output_port);

    auto input_cell = cell_map[die_trace.input_tag];
    auto input_node = input_cell->core_node;
    auto input_port_index = input_cell->die_cell->get_input_index(die_trace.input_port);

    CHECK_P(output_cell);
    CHECK_P(output_node);
    CHECK_P(output_port_index >= 0);

    CHECK_P(input_cell);
    CHECK_P(input_node);
    CHECK_P(input_port_index >= 0);

    auto plait_trace = new PlaitTrace();

    plait_trace->die_trace = &die_trace;
    plait_trace->output_node = output_node;
    plait_trace->input_node = input_node;
    plait_trace->output_port_index = output_port_index;
    plait_trace->input_port_index  = input_port_index;

    traces.push_back(plait_trace);
  }

  // Sanity checks

  for (auto& [tag, plait_cell] : cell_map) {
    // Every cell should have a core node.
    CHECK_P(plait_cell->core_node);
    DCHECK_N(plait_cell->core_node->name.empty());
    CHECK_P(plait_cell->core_node->plait_cell == plait_cell);

    // Every node should have a link to the cell.
    for (auto& [name, root] : plait_cell->root_nodes) {
      CHECK_P(root);
      DCHECK_N(root->name.empty());
      CHECK_P(root->plait_cell == plait_cell);
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      CHECK_P(leaf);
      DCHECK_N(leaf->name.empty());
      CHECK_P(leaf->plait_cell == plait_cell);
    }
  }

  //for (auto& [trace_key, plait_trace] : trace_map_old) {
  for (auto& plait_trace : traces) {
    (void)plait_trace;
    //ASSERT_P(plait_trace->die_trace->plait_trace == plait_trace);
    CHECK_P(plait_trace->output_node);
    CHECK_P(plait_trace->input_node);

    //ASSERT_P(plait_trace->output_cell_name == plait_trace->die_trace->output_tag);
    //ASSERT_P(plait_trace->output_node_name == plait_trace->output_node->name);

    //ASSERT_P(plait_trace->input_cell_name == plait_trace->die_trace->input_tag);
    //ASSERT_P(plait_trace->input_node_name == plait_trace->input_node->name);
  }
}

//---------------------------------------------------------------------------------------------------------------------
