#pragma once

#include "CoreLib/Types.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

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

struct DieCell {
  void sanity_check() const;
  void dump(Dumper& d) const;

  DieCellType cell_type = DieCellType::UNKNOWN;
  std::string page;
  std::string tag;
  std::string gate;
  std::string name;
  std::string doc;

  std::vector<std::string> prev_ports;
  std::vector<std::string> next_ports;
  void* plait_cell = nullptr;
  int mark = 0;

  int get_prev_port_index(const std::string& port_name) const {
    for (auto i = 0; i < prev_ports.size(); i++) {
      if (prev_ports[i] == port_name) return i;
    }
    printf("no prev port for %s\n", port_name.c_str());
    return -1;
  }

  int get_next_port_index(const std::string& port_name) const {
    for (auto i = 0; i < next_ports.size(); i++) {
      if (next_ports[i] == port_name) return i;
    }
    printf("no next port for %s\n", port_name.c_str());
    return -1;
  }
};

//------------------------------------------------------------------------------------------------------------------------

struct DieTrace {
  std::string prev_tag;
  std::string prev_port;
  std::string next_tag;
  std::string next_port;
  void* plait_trace = nullptr;

  bool operator < (const DieTrace& e) const {
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

  bool operator == (const DieTrace& e) const {
    if (prev_tag  != e.prev_tag)  return false;
    if (prev_port != e.prev_port) return false;
    if (next_tag  != e.next_tag)  return false;
    if (next_port != e.next_port) return false;
    return true;
  }

  void from_key(const std::string& key) {
    auto first_dot = key.find('.');
    auto arrow = key.find(" -> ");
    auto second_dot = key.find('.', first_dot + 1);
    printf("%zd\n", first_dot);
    printf("%zd\n", arrow);
    printf("%zd\n", second_dot);

    prev_tag  = key.substr(0,              first_dot  - 0);
    prev_port = key.substr(first_dot + 1,  arrow      - (first_dot + 1));
    next_tag  = key.substr(arrow + 4,      second_dot - (arrow + 4));
    next_port = key.substr(second_dot + 1, key.size() - (second_dot + 1));
  }

  std::string to_key() const {
    return prev_tag + "." + prev_port + " -> " + next_tag + "." + next_port;
  }
};

//------------------------------------------------------------------------------------------------------------------------

struct DieDB {
  void clear();

  void save_json(const char* filename);
  void save_json(std::ostream& stream);

  void load_json(const char* filename);
  void load_json(std::istream& stream);

  void sanity_check();

  bool parse_dir(const std::string& path);
  bool parse_file(const std::string& path);
  bool parse_line(const std::string& line);
  bool parse_rest(DieCell& c, const std::string& rest);

  bool parse_cell_name(DieCell& c, const std::string& cell_name);
  bool parse_cell_def(DieCell& c, const std::string& value);
  bool parse_cell_arg(const std::string& arg, std::string& tag_out, std::string& port_out);
  bool parse_cell_arglist(DieCell& c, const std::string& arglist);

  bool parse_tribuf_bus_target(DieCell& c, const std::string& bus_name);
  bool parse_pin_name(DieCell& c, const std::string& pin_name);
  bool parse_sig_name(DieCell& c, const std::string& sig_name);

  bool parse_tag(const std::string& tag_comment, std::string& page_out, std::string& tag_out);
  bool parse_reg_type(DieCell& c, const std::string& type);
  bool parse_cell_gate(DieCell& c, const std::string& type);

  bool  has_cell(const std::string& tag) {
    return cell_map.find(tag) != cell_map.end();
  }

  DieCell* get_cell(const std::string& tag) {
    CHECK_P(has_cell(tag));
    return cell_map[tag];
  }

  DieCell* get_or_create_cell(const std::string& tag) {
    auto it = cell_map.find(tag);
    if (it != cell_map.end()) return (*it).second;

    DieCell* new_cell = new DieCell();
    cell_map[tag] = new_cell;
    new_cell->tag = tag;
    return new_cell;
  }

  std::map<std::string, DieCell*> cell_map;

  std::map<std::string, DieTrace*> trace_map;

  int total_lines = 0;
  int total_tagged_lines = 0;
};

//------------------------------------------------------------------------------------------------------------------------