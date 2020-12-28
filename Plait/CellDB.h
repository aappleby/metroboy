#pragma once

#include "CoreLib/Types.h"
#include <string>
#include <vector>
#include <set>

struct Cell {
  void dump(Dumper& d);

  std::string verified;
  std::string page;
  std::string tag;
  std::string gate;
  std::string bus;
  std::string doc;
  std::set<std::string> names;
  std::vector<std::string> args;
};

struct CellDB {
  bool parse_dir(const std::string& path);
  bool parse_file(const std::string& path);
  bool parse_line(Cell& c, const std::string& line);
  bool parse_rest(Cell& c, const std::string& rest);

  bool parse_cell_name(Cell& c, const std::string& cell_name);
  bool parse_cell_def(Cell& c, const std::string& value);
  bool parse_cell_input_list(Cell& c, const std::string& arglist);
  bool parse_cell_input(Cell& c, const std::string& input);

  bool parse_bus_name(Cell& c, const std::string& bus_name);

  bool parse_tag_comment(Cell& c, const std::string& tag_comment);
  bool parse_reg_type(Cell& c, const std::string& type);
  bool parse_gate_type(Cell& c, const std::string& type);

  std::set<std::string> all_pages;
  std::set<std::string> all_names;
  std::set<std::string> all_tags;
  std::set<std::string> all_buses;
  std::set<std::string> all_signals;
  std::set<std::string> all_pins;

  std::set<std::string> verified_tags;

  int total_lines = 0;
  int total_tagged_lines = 0;
};

#if 0

#include "protos/plait_wrapper.h"

#include <map>
#include <set>
#include <vector>


struct Node;

typedef std::vector<Node*> NodeVec;
typedef std::map<std::string, Node*> NameToNode;
typedef std::multimap<std::string, std::string> NameToEdges;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct CellDB {
};

//-----------------------------------------------------------------------------

struct Node {
  //void dump(Dumper& d);

  int rank;
  int mark;

  dvec2 pos_old;
  dvec2 pos_new;

  Cell * cell;
  std::vector<Node*> prev;
  std::vector<Node*> next;

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
  void  parse_line(const std::string& line);

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

  std::map<std::string, Cell*> cell_db;
  Frame* root_frame;

  //NodeVec     nodes;
  //NameToNode  names;
  //NameToEdges edges;
  //std::set<Node*> roots;
};

#endif

