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

enum class CellType {
  UNKNOWN = 0,
  PIN_IN,
  PIN_OUT,
  PIN_IO,
  SIG_IN,
  SIG_OUT,
  BUS,
  DFF,
  LATCH,
  TRIBUF,
  LOGIC,
};

struct Cell {
  void sanity_check() const;
  void merge(const Cell& c);
  void dump(Dumper& d);

  CellType              cell_type = CellType::UNKNOWN;
  std::string           verified;
  std::string           page;
  std::string           tag;
  std::string           gate;
  std::vector<Port>     args;
  std::set<std::string> names;
  std::string           doc;

  int mark = 0;
};

//------------------------------------------------------------------------------------------------------------------------

struct CellDB {
  void clear();
  void save_json(const char* filename);
  void load_json(const char* filename);


  bool parse_dir(const std::string& path);
  bool parse_file(const std::string& path);
  bool parse_line(Cell& c, const std::string& line);
  bool parse_rest(Cell& c, const std::string& rest);

  bool parse_cell_name(Cell& c, const std::string& cell_name);
  bool parse_cell_def(Cell& c, const std::string& value);
  bool parse_cell_arg(Cell& c, const std::string& input);
  bool parse_cell_arglist(Cell& c, const std::string& arglist);

  bool parse_tribuf_bus_target(Cell& c, const std::string& bus_name);
  bool parse_pin_name(Cell& c, const std::string& pin_name);
  bool parse_sig_name(Cell& c, const std::string& sig_name);

  bool parse_tag(Cell& c, const std::string& tag_comment);
  bool parse_reg_type(Cell& c, const std::string& type);
  bool parse_cell_gate(Cell& c, const std::string& type);

  bool  has_cell(const std::string& tag) {
    return tag_to_cell.find(tag) != tag_to_cell.end();
  }

  Cell* get_cell(const std::string& tag) {
    CHECK_P(has_cell(tag));
    return tag_to_cell[tag];
  }

  Cell* get_or_create_cell(const std::string& tag) {
    auto it = tag_to_cell.find(tag);
    if (it != tag_to_cell.end()) return (*it).second;

    Cell* new_cell = new Cell();
    tag_to_cell[tag] = new_cell;
    return new_cell;
  }

  std::map<std::string, Cell*> tag_to_cell;

  int total_lines = 0;
  int total_tagged_lines = 0;
};

//------------------------------------------------------------------------------------------------------------------------