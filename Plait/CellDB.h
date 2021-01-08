#pragma once

#include "CoreLib/Types.h"
#include <string>
#include <vector>
#include <set>
#include <map>

enum class DieCellType {
  UNKNOWN = 0,
  PIN_IN,
  PIN_OUT,
  PIN_IO,
  PIN_CLK,
  SIG_IN,
  SIG_OUT,
  BUS,
  DFF,
  LATCH,
  TRIBUF,
  ADDER,
  LOGIC,
};

//------------------------------------------------------------------------------------------------------------------------

struct DieCellArg {
  std::string tag;
  std::string port;
};

struct DieCellEdge {
  std::string prev_tag;
  std::string prev_port;
  std::string next_tag;
  std::string next_port;

  bool operator < (const DieCellEdge& e) const {
    if (prev_tag  > e.prev_tag)  return false;
    if (prev_tag  < e.prev_tag)  return true;
    if (prev_port > e.prev_port) return false;
    if (prev_port < e.prev_port) return true;
    if (next_tag  > e.next_tag)  return false;
    if (next_tag  < e.next_tag)  return true;
    if (next_port > e.next_port) return false;
    if (next_port < e.next_port) return true;
    return false;
  }

  bool operator == (const DieCellEdge& e) const {
    if (prev_tag != e.prev_tag) return false;
    if (prev_port != e.prev_port) return false;
    if (next_tag != e.next_tag) return false;
    if (next_port != e.next_port) return false;
    return true;
  }
};

//------------------------------------------------------------------------------------------------------------------------

struct DieCell {
  void sanity_check() const;
  void merge(const DieCell& c);
  void dump(Dumper& d) const;

  DieCellType              cell_type = DieCellType::UNKNOWN;
  std::string              page;
  std::string              tag;
  std::string              gate;
  std::string              name;
  std::string              doc;

  std::vector<std::string> prev_ports;
  std::vector<std::string> next_ports;

  int mark = 0;

  //void add_arg(const std::string& _tag, const std::string& _port) {
  //  cell_args.push_back({_tag,_port});
  //}

  //std::vector<std::string>& get_out_ports();
};

//------------------------------------------------------------------------------------------------------------------------

struct DieDB {
  void clear();
  void save_json(const char* filename);
  void load_json(const char* filename);

  bool parse_dir(const std::string& path);
  bool parse_file(const std::string& path);
  bool parse_line(const std::string& line);
  bool parse_rest(DieCell& c, const std::string& rest);

  bool parse_cell_name(DieCell& c, const std::string& cell_name);
  bool parse_cell_def(DieCell& c, const std::string& value);
  bool parse_cell_arg(const std::string& arg, DieCellArg& arg_out);
  bool parse_cell_arg(DieCell& c, const std::string& input);
  bool parse_cell_arglist(DieCell& c, const std::string& arglist);

  bool parse_tribuf_bus_target(DieCell& c, const std::string& bus_name);
  bool parse_pin_name(DieCell& c, const std::string& pin_name);
  bool parse_sig_name(DieCell& c, const std::string& sig_name);

  bool parse_tag(const std::string& tag_comment, std::string& page_out, std::string& tag_out);
  bool parse_reg_type(DieCell& c, const std::string& type);
  bool parse_cell_gate(DieCell& c, const std::string& type);

  bool  has_cell(const std::string& tag) {
    return tag_to_cell.find(tag) != tag_to_cell.end();
  }

  DieCell* get_cell(const std::string& tag) {
    CHECK_P(has_cell(tag));
    return tag_to_cell[tag];
  }

  DieCell* get_or_create_cell(const std::string& tag) {
    auto it = tag_to_cell.find(tag);
    if (it != tag_to_cell.end()) return (*it).second;

    DieCell* new_cell = new DieCell();
    tag_to_cell[tag] = new_cell;
    new_cell->tag = tag;
    return new_cell;
  }

  std::map<std::string, DieCell*> tag_to_cell;

  static std::map<DieCellType, std::vector<std::string>> cell_type_to_out_ports;
  static std::map<std::string, std::vector<std::string>> gate_type_to_in_ports;

  std::vector<DieCellEdge> edges;

  int total_lines = 0;
  int total_tagged_lines = 0;
};

//------------------------------------------------------------------------------------------------------------------------