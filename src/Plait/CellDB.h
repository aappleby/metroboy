#pragma once

#include "metrolib/core/Dumper.h"
#include "metrolib/core/Check.h"
#include "metrolib/core/Log.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

#include "Plait/PTree.h"

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

struct GateInfo {
  std::string decl;
  std::string gate;
  DieCellType cell_type;
  std::vector<std::string> input_ports;
  std::vector<std::string> output_ports;
};

std::string trim_name(std::string raw_name);

//-----------------------------------------------------------------------------

struct DieCell {
  void sanity_check() const;
  void dump(Dumper& d) const;

  void set_type(DieCellType _cell_type) {
    CHECK_N(_cell_type == DieCellType::UNKNOWN);
    if (cell_type == DieCellType::UNKNOWN) cell_type = _cell_type;
    CHECK_P(cell_type == _cell_type);
  }

  void set_flag(const std::string& _flag) { if (flag.empty()) flag = _flag; CHECK_P(flag == _flag); }
  void set_page(const std::string& _page) { if (page.empty()) page = _page; CHECK_P(page == _page); }
  void set_tag (const std::string& _tag)  { if (tag.empty())  tag  = _tag;  CHECK_P(tag  == _tag);  }
  void set_gate(const std::string& _gate) { if (gate.empty()) gate = _gate; CHECK_P(gate == _gate); }
  //void set_args(const std::string& _args) { if (args.empty()) args = _args; CHECK_P(args == _args); }
  void set_name(const std::string& _name) {
    if (_name.starts_with("ALUR")) {
      int x = 1;
      x++;
    }

    auto n = trim_name(_name);
    if (name.empty()) name = n;
    CHECK_P(name == n);
  }
  //void set_doc (const std::string& _doc)  { if (doc.empty())  doc  = _doc;  CHECK_P(doc  == _doc);  }

  void set_args(const std::vector<std::string>& _args) {

    if (_args[0] == "sys_rst.AVOR_SYS_RSTp") {
      int x = 1;
      x++;
    }

    if (cell_type == DieCellType::BUS) {
      input_ports.insert(input_ports.end(), _args.begin(), _args.end());
      return;
    }

    if (input_ports.empty()) {
      input_ports = _args;
    }
    else {
      CHECK_P(input_ports.size() == _args.size());
      for (int i = 0; i < input_ports.size(); i++) {
        CHECK_P(input_ports[i] == _args[i]);
      }
    }
  }

  DieCellType cell_type = DieCellType::UNKNOWN; // The general type of cell - logic, dff, etc.
  std::string flag; // Flag characters to denote that the cell has been manually checked
  std::string page; // Where this cell appears in Furrtek's schematics, or XX if it doesn't
  std::string tag;  // The four-character tag used by Furrtek's schematic, or SIG/BUS/PIN/etc
  std::string gate; // The specific type of cell
  std::string name; // The descriptive name of this cell. Should include the tag.
  //std::string doc;  // Any comment string appended after the cell's declaration.
  //std::string args; // The inputs to the cell

  std::vector<std::string> input_ports;
  std::vector<std::string> output_ports;

  int fanout = 0;

  void* plait_cell = nullptr;
  int mark = 0;

  int get_input_index(const std::string& port_name) const {
    for (auto i = 0; i < input_ports.size(); i++) {
      if (input_ports[i] == port_name) return i;
    }
    LOG_R("no prev port for %s\n", port_name.c_str());
    return -1;
  }

  int get_output_index(const std::string& port_name) const {
    for (auto i = 0; i < output_ports.size(); i++) {
      if (output_ports[i] == port_name) return i;
    }
    LOG_R("no next port for %s\n", port_name.c_str());
    return -1;
  }
};

//-----------------------------------------------------------------------------

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

  std::string to_key() const {
    return output_tag + "." + output_port + " -> " + input_tag + "." + input_port;
  }
};

//-----------------------------------------------------------------------------

struct DieDB {
  void clear();

  void save_json(const char* filename);
  void save_json(std::ostream& stream);

  void load_json(const char* filename);
  void load_json(std::istream& stream);

  void sanity_check();

  bool parse_dir(const std::string& path);
  bool parse_header(const std::string& path);
  bool parse_source(const std::string& path);

  bool parse_struct(PNode node, const char* source);

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
  std::vector<DieTrace> traces;

  int total_tags = 0;
  int total_files = 0;
  int total_lines = 0;
};

//-----------------------------------------------------------------------------
