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

struct CellPort {
  std::string tag;
  std::string port;

  bool operator==(const CellPort& b) {
    return tag == b.tag && port == b.port;
  }
};

std::string trim_name(const std::string& raw_name);

//------------------------------------------------------------------------------------------------------------------------

struct DieCell {
  void sanity_check() const;
  void dump(Dumper& d) const;

  void set_type(DieCellType _cell_type) {
    if (cell_type == DieCellType::UNKNOWN) cell_type = _cell_type;
    CHECK_P(cell_type == _cell_type);
  }

  void set_flag(const std::string& _flag) { if (flag.empty()) flag = _flag; CHECK_P(flag == _flag); }
  void set_page(const std::string& _page) { if (page.empty()) page = _page; CHECK_P(page == _page); }
  void set_path(const std::string& _path) { if (path.empty()) path = _path; CHECK_P(path == _path); }
  void set_decl(const std::string& _decl) { if (decl.empty()) decl = _decl; CHECK_P(decl == _decl); }
  void set_tag (const std::string& _tag)  { if (tag.empty())  tag  = _tag;  CHECK_P(tag  == _tag);  }
  void set_gate(const std::string& _gate) { if (gate.empty()) gate = _gate; CHECK_P(gate == _gate); }
  //void set_args(const std::string& _args) { if (args.empty()) args = _args; CHECK_P(args == _args); }
  void set_name(const std::string& _name) { if (name.empty()) name = _name; CHECK_P(name == _name); }
  void set_doc (const std::string& _doc)  { if (doc.empty())  doc  = _doc;  CHECK_P(doc  == _doc);  }

  std::string remove_path(const std::string& _path) {
    auto it = _path.rfind('.');
    if (it != std::string::npos) {
      return _path.substr(0, it);
    }
    else {
      return _path;
    }
  }

  void set_full_path(const std::string& _path) {
    auto it = _path.rfind('.');
    if (it != std::string::npos) {
      set_path(_path.substr(0, it));
      set_name(trim_name(_path.substr(it + 1)));
    }
    else {
      set_name(_path);
    }
  }

  DieCellType cell_type = DieCellType::UNKNOWN; // The general type of cell - logic, dff, etc.
  std::string flag; // Flag characters to denote that the cell has been manually checked
  std::string page; // Where this cell appears in Furrtek's schematics, or XX if it doesn't
  std::string path; // Where this cell lives in the GateBoy hierarchy - for "foo.bar.BAZZ", this is "foo.bar"
  std::string tag;  // The four-character tag used by Furrtek's schematic, or SIG/BUS/PIN/etc
  std::string decl; // The name of the C++ type used to represent this cell - wire, DFF8n, TpLatch
  std::string gate; // The specific type of cell
  //std::string args; // The inputs to the cell
  std::string name; // The descriptive name of this cell. Should include the tag.
  std::string doc;  // Any comment string appended after the cell's declaration.

  std::vector<CellPort> args;

  std::vector<std::string> input_ports;
  std::vector<std::string> output_ports;

  int fanout = 0;

  void* plait_cell = nullptr;
  int mark = 0;

  int get_input_index(const std::string& port_name) const {
    for (auto i = 0; i < input_ports.size(); i++) {
      if (input_ports[i] == port_name) return i;
    }
    printf("no prev port for %s\n", port_name.c_str());
    return -1;
  }

  int get_output_index(const std::string& port_name) const {
    for (auto i = 0; i < output_ports.size(); i++) {
      if (output_ports[i] == port_name) return i;
    }
    printf("no next port for %s\n", port_name.c_str());
    return -1;
  }
};

//------------------------------------------------------------------------------------------------------------------------

struct DieTrace {
  std::string output_tag;
  std::string output_port;
  std::string input_tag;
  std::string input_port;

  //void* plait_trace = nullptr;

  bool operator < (const DieTrace& e) const {
    if (output_tag  > e.output_tag)  return false;
    if (output_tag  < e.output_tag)  return true;
    if (output_port > e.output_port) return false;
    if (output_port < e.output_port) return true;
    if (input_tag  > e.input_tag)  return false;
    if (input_tag  < e.input_tag)  return true;
    if (input_port > e.input_port) return false;
    if (input_port < e.input_port) return true;
    return false;
  }

  bool operator == (const DieTrace& e) const {
    if (output_tag  != e.output_tag)  return false;
    if (output_port != e.output_port) return false;
    if (input_tag  != e.input_tag)  return false;
    if (input_port != e.input_port) return false;
    return true;
  }

  /*
  void from_key(const std::string& key) {
    auto first_dot = key.find('.');
    auto arrow = key.find(" -> ");
    auto second_dot = key.find('.', first_dot + 1);
    //printf("%zd\n", first_dot);
    //printf("%zd\n", arrow);
    //printf("%zd\n", second_dot);

    output_tag  = key.substr(0,              first_dot  - 0);
    output_port = key.substr(first_dot + 1,  arrow      - (first_dot + 1));
    input_tag  = key.substr(arrow + 4,      second_dot - (arrow + 4));
    input_port_old = key.substr(second_dot + 1, key.size() - (second_dot + 1));
  }
  */

  std::string to_key() const {
    return output_tag + "." + output_port + " -> " + input_tag + "." + input_port;
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
  DieCell* parse_tag(std::string& line);

  bool parse_gate(const std::string& line, DieCell* cell);
  bool parse_member_decl(const std::string& line, DieCell* cell);
  bool parse_method_decl(const std::string& line, DieCell* cell);
  bool parse_adder(const std::string& line, DieCell* cell);
  bool parse_assignment(const std::string& line, DieCell* cell);
  bool parse_method_call(const std::string& line, DieCell* cell);
  bool parse_sig_vcc_gnd(const std::string& line, DieCell* cell);
  bool parse_tail(const std::string& line, DieCell* cell);

  bool parse_cell_arg(const std::string& arg, std::string& tag_out, std::string& port_out);
  bool parse_cell_arglist(DieCell& c, const std::string& arglist);

  bool parse_tribuf_bus_target(DieCell& c, const std::string& bus_name);


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

  void error(const char* format, ...);

  std::map<std::string, DieCell*> cell_map;

  //std::vector<DieTrace*> traces;
  std::vector<DieTrace> traces;

  //std::map<std::string, DieTrace*> trace_map_old;

  std::string current_filename;
  std::string current_line;
  int current_linenum;

  int total_lines = 0;
  int total_files = 0;
  int total_tagged_lines = 0;
};

//------------------------------------------------------------------------------------------------------------------------