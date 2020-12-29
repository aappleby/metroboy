#pragma once

#include "CoreLib/Types.h"
#include <string>
#include <vector>
#include <set>
#include <map>

//------------------------------------------------------------------------------------------------------------------------

struct Port {
  std::string tag;
  std::string port;
};

//------------------------------------------------------------------------------------------------------------------------

struct Cell {
  void sanity_check() const;
  void merge(const Cell& c);
  void dump(Dumper& d);

  std::string verified;
  std::string page;
  std::string tag;
  std::string gate;
  std::string bus;
  std::string pin;
  std::string sig;
  std::string doc;
  std::vector<Port> args;
  std::set<std::string> names;

  void* node = nullptr;

  int mark = 0;

  int16_t origin_x;
  int16_t origin_y;
};

//------------------------------------------------------------------------------------------------------------------------

struct CellDB {
  void sanity_check();

  bool parse_dir(const std::string& path);
  bool parse_file(const std::string& path);
  bool parse_line(Cell& c, const std::string& line);
  bool parse_rest(Cell& c, const std::string& rest);

  bool parse_cell_name(Cell& c, const std::string& cell_name);
  bool parse_cell_def(Cell& c, const std::string& value);
  bool parse_cell_arg(Cell& c, const std::string& input);
  bool parse_cell_arglist(Cell& c, const std::string& arglist);

  bool parse_bus_name(Cell& c, const std::string& bus_name);
  bool parse_pin_name(Cell& c, const std::string& pin_name);
  bool parse_sig_name(Cell& c, const std::string& sig_name);

  bool parse_tag(Cell& c, const std::string& tag_comment);
  bool parse_reg_type(Cell& c, const std::string& type);
  bool parse_gate_type(Cell& c, const std::string& type);

  bool  has_cell(const std::string& tag) {
         if (tag.starts_with("BUS_")) return bus_map.count(tag) != 0;
    else if (tag.starts_with("SIG_")) return sig_map.count(tag) != 0;
    else if (tag.starts_with("PIN"))  return pin_map.count(tag) != 0;
    else                              return cell_map.count(tag) != 0;
  }

  Cell* get_cell(const std::string& tag) {
         if (tag.starts_with("BUS_")) return bus_map[tag];
    else if (tag.starts_with("SIG_")) return sig_map[tag];
    else if (tag.starts_with("PIN"))  return pin_map[tag];
    else                              return cell_map[tag];
  }

  std::map<std::string, Cell*> cell_map;
  std::map<std::string, Cell*> bus_map;
  std::map<std::string, Cell*> pin_map;
  std::map<std::string, Cell*> sig_map;

  int total_lines = 0;
  int total_tagged_lines = 0;
};

//------------------------------------------------------------------------------------------------------------------------