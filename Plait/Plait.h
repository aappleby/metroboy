#pragma once

#include "Plait/CellDB.h"
#include "glm/glm/glm.hpp"

#include <vector>
#include <iostream>

using namespace glm;

struct PlaitCell;
struct PlaitNode;
struct PlaitTrace;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Plait {
  void clear();

  void split_node(PlaitNode* root_node);
  void merge_node(PlaitNode* root_node);

  void swap_input_edges (PlaitNode* old_node, PlaitNode* new_node);
  void swap_output_edges(PlaitNode* old_node, PlaitNode* new_node);

  void check_dead(PlaitNode* node);

  void save_json(const char* filename);
  void save_json(std::ostream& stream);

  void load_json(const char* filename, DieDB& die_db);
  void load_json(std::istream& stream, DieDB& die_db);

  std::map<std::string, PlaitCell*>  cell_map;
  std::map<std::string, PlaitTrace*> trace_map;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitCell {
  ~PlaitCell();

  void dump(Dumper& d);

  int get_input_index(const std::string& port_name) const {
    return die_cell->get_input_index(port_name);
  }

  int get_output_index(const std::string& port_name) const {
    return die_cell->get_output_index(port_name);
  }

  const char* tag() const  { return die_cell ? die_cell->tag.c_str()  : "<no_tag>"; }
  const char* name() const { return die_cell ? die_cell->name.c_str() : "<no_cell>"; }
  const char* gate() const { return die_cell ? die_cell->gate.c_str() : "<no_gate>"; }

  void       add_node(PlaitNode* node);
  PlaitNode* find_node(const std::string& name) const;

  // Serialized
  std::map<std::string, PlaitNode*> nodes;

  // Not serialized
  DieCell* die_cell = nullptr;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitNode {

  //----------------------------------------
  // Serialized

  std::string name;
  dvec2 pos_new = {0,0};
  dvec2 pos_old = {0,0};

  //----------------------------------------
  // Not serialized

  PlaitCell* plait_cell = nullptr;
  bool ghost = 0;
  bool selected = 0; // need this because we don't want a log(n) lookup per node per frame...
  uint32_t color = 0xFFFF00FF;

  //----------------------------------------

  void dump(Dumper& d);
  void toggle_ghost()        { ghost = !ghost; }

  dvec2 get_pos_old() const  { return pos_old; }
  dvec2 get_pos_new() const  { return pos_new; }
  void  set_pos_new(dvec2 p) { pos_new = p; }
  void  move(dvec2 delta)    { pos_new += delta; }

  void commit_pos()          { pos_old = pos_new; }
  void revert_pos()          { pos_new = pos_old; }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PlaitTrace {
  //----------------------------------------
  // Serialized

  std::string output_cell_name;
  std::string output_node_name;

  std::string input_cell_name;
  std::string input_node_name;

  //----------------------------------------
  // Not serialized

  DieTrace* die_trace = nullptr;
  PlaitNode* output_node = nullptr;
  PlaitNode* input_node  = nullptr;

  int output_port_index;
  int input_port_index;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
