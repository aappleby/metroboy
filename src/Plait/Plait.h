#pragma once

#include "metrolib/appbase/Viewport.h"
#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"
#include "json/single_include/nlohmann/json.hpp"
#include "metrolib/core/Utils.h"

#include <vector>
#include <iostream>

using namespace glm;

struct PlaitCell;
struct PlaitNode;
struct PlaitTrace;
struct PlaitLabel;
struct PlaitFrame;

//---------------------------------------------------------------------------------------------------------------------

struct PlaitLabel {
  std::string text;
  dvec2 pos_old;
  dvec2 pos_new;
  double scale;
};

//---------------------------------------------------------------------------------------------------------------------

struct Plait {
  void from_json(nlohmann::json& jroot, DieDB& die_db);
  void to_json(nlohmann::json& jroot);

  void clear();

  void spawn_root_node (PlaitNode* core_node);
  void spawn_leaf_node (PlaitNode* core_node);
  void delete_roots(PlaitNode* node);
  void delete_leaves(PlaitNode* node);

  void link_nodes(PlaitNode* node_a, PlaitNode* node_b);
  void swap_input_edges(PlaitNode* old_node, PlaitNode* new_node);
  void swap_output_edges(PlaitNode* old_node, PlaitNode* new_node);

  void delete_node(PlaitNode* dead_leaf);
  void check_dead(PlaitNode* node);

  std::map<std::string, PlaitCell*>  cell_map;
  std::vector<PlaitTrace*> traces;
  std::vector<PlaitLabel*> labels;
  std::vector<PlaitFrame*> frames;

  uint32_t new_guid() {
    _guid = mix(_guid);
    return _guid;
  }

  uint32_t _guid = 0x00000001;
};

//---------------------------------------------------------------------------------------------------------------------

struct PlaitCell {
  ~PlaitCell();

  void dump(Dumper& d);

  //int get_input_index(const std::string& port_name) const {
  //  return die_cell->get_input_index(port_name);
  //}

  //int get_output_index(const std::string& port_name) const {
  //  return die_cell->get_output_index(port_name);
  //}

  const char* tag() const  { return die_cell ? die_cell->tag.c_str()  : "<no_tag>"; }
  const char* name() const { return die_cell ? die_cell->name.c_str() : "<no_cell>"; }
  const char* gate() const { return die_cell ? die_cell->gate.c_str() : "<no_gate>"; }

  void       add_root_node(PlaitNode* node);
  void       add_leaf_node(PlaitNode* node);

  PlaitNode* find_root_node(const std::string& name) const;
  PlaitNode* find_leaf_node(const std::string& name) const;

  PlaitNode* spawn_root_node(PlaitNode* neighbor, uint32_t guid);
  PlaitNode* spawn_leaf_node(PlaitNode* neighbor, uint32_t guid);

  bool selected() { return selected_node_count > 0; }

  // Serialized
  PlaitNode* core_node;
  std::map<std::string, PlaitNode*> root_nodes;
  std::map<std::string, PlaitNode*> leaf_nodes;

  // Not serialized
  DieCell* die_cell = nullptr;
  int selected_node_count = 0;
};

//---------------------------------------------------------------------------------------------------------------------

struct PlaitFrame {
  //----------------------------------------
  // Serialized

  std::string title;
  std::string text;
  dvec2 pos = { 0,0 };
  ivec2 size = { 0, 0 };
  vec4 color = { 0, 0, 0, 0.2f };
  int text_scale = 1;
};

//---------------------------------------------------------------------------------------------------------------------

struct PlaitNode {

  //----------------------------------------
  // Serialized

  std::string name;
  dvec2 pos_new = {0,0};
  dvec2 pos_old = {0,0};
  bool old = false;
  bool global = false;

  //----------------------------------------
  // Not serialized

  PlaitCell* plait_cell = nullptr;
  bool ghosted = 0;
  uint32_t color = 0xFFFF00FF;

  //----------------------------------------

  bool is_root() {
    return name[0] == 'r' && name[1] == 'o' && name[2] == 'o' && name[3] == 't';
  }

  bool is_leaf() {
    return name[0] == 'l' && name[1] == 'e' && name[2] == 'a' && name[3] == 'f';
  }

  bool selected() {
    return _selected;
  }

  void select() {
    if (!_selected) {
      plait_cell->selected_node_count++;
      _selected = true;
    }
  }

  void deselect() {
    if (_selected) {
      plait_cell->selected_node_count--;
      _selected = false;
    }
  }

  void dump(Dumper& d);
  void toggle_global() { global = !global; }
  void toggle_ghosted() { ghosted = !ghosted; }
  void toggle_old()     { old = !old; }

  dvec2 get_pos_old() const  { return pos_old; }
  dvec2 get_pos_new() const  { return pos_new; }
  void  set_pos_new(dvec2 p) { pos_new = p; }
  void  move(dvec2 delta)    { pos_new += delta; }

  bool is_global() const {
    return plait_cell->core_node->global;
  }

  void commit_pos()          {
    pos_old = pos_new;
    while(pos_old.x >  32768) pos_old.x -= 65536;
    while(pos_old.x < -32768) pos_old.x += 65536;
    while(pos_old.y >  32768) pos_old.y -= 65536;
    while(pos_old.y < -32768) pos_old.y += 65536;
    pos_new = pos_old;

  }
  void revert_pos() {
    pos_new = pos_old;
  }

  void update_visibility(const Viewport& view) {
    (void)view;
    visible = true;
    /*
    if (pos_new.x > view.max.x) visible = false;
    if (pos_new.y > view.max.y) visible = false;
    if (pos_new.x + 128 < view.min.x) visible = false;
    if (pos_new.y + 64  < view.min.y) visible = false;
    */
  }

  bool _selected = false;
  bool visible = true;

};

//---------------------------------------------------------------------------------------------------------------------

struct PlaitTrace {
  //----------------------------------------
  // Serialized

  //std::string output_cell_name;
  //std::string output_node_name;
  //std::string output_port_name;

  //std::string input_cell_name;
  //std::string input_node_name;
  //std::string input_port_name;

  //----------------------------------------
  // Not serialized

  const DieTrace* die_trace = nullptr;
  PlaitNode* output_node = nullptr;
  PlaitNode* input_node  = nullptr;

  int output_port_index;
  int input_port_index;
};

//---------------------------------------------------------------------------------------------------------------------
