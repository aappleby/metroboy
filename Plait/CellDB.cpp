#include "Plait/CellDB.h"

#include "CoreLib/Debug.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

static std::map<CellType, string> cell_type_to_name = {
  {CellType::UNKNOWN, "UNKNOWN"},
  {CellType::PIN_IN,  "PIN_IN"},
  {CellType::PIN_OUT, "PIN_OUT"},
  {CellType::PIN_IO,  "PIN_IO"},
  {CellType::SIG_IN,  "SIG_IN"},
  {CellType::SIG_OUT, "SIG_OUT"},
  {CellType::BUS,     "BUS"},
  {CellType::LOGIC,   "LOGIC"},
  {CellType::TRIBUF,  "TRIBUF"},
  {CellType::DFF,     "DFF"},
  {CellType::LATCH,   "LATCH"},
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Cell::sanity_check() const {
  CHECK_N(cell_type == CellType::UNKNOWN);
  CHECK_N(tag.empty());
  CHECK_N(gate.empty());
  CHECK_N(names.empty());

  if (cell_type == CellType::PIN_IN) {
    CHECK_P(tag.starts_with("PIN"));
    CHECK_P(gate == "pin_input");
    CHECK_P(args.empty());
  }
  else if (cell_type == CellType::PIN_OUT) {
    CHECK_P(tag.starts_with("PIN"));
    CHECK_P(gate != "pin_input");
    CHECK_P(args.size());
  }
  else if (cell_type == CellType::PIN_IO) {
    CHECK_P(tag.starts_with("PIN"));
    CHECK_P(gate != "pin_input");
    CHECK_P(args.size());
  }
  else if (cell_type == CellType::SIG_IN) {
    CHECK_P(tag.starts_with("SIG_"));
    CHECK_P(gate == "sig_input");
    CHECK_P(args.empty());
  }
  else if (cell_type == CellType::SIG_OUT) {
    CHECK_P(tag.starts_with("SIG_"));
    CHECK_P(gate == "set");
    CHECK_P(args.size() == 1);
  }
  else if (cell_type == CellType::BUS) {
    CHECK_P(tag.starts_with("BUS_"));
    CHECK_P(gate == "bus_input" || args.size());
  }
  else {
    CHECK_P(args.size());
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Cell::merge(const Cell& c) {
  if (cell_type == CellType::UNKNOWN) cell_type = c.cell_type;
  if (verified.empty()) verified = c.verified;
  if (page.empty())     page = c.page;
  if (tag.empty())      tag = c.tag;
  if (gate.empty())     gate = c.gate;
  if (doc.empty())      doc = c.doc;
  if (args.empty())     args = c.args;
  names.insert(c.names.begin(), c.names.end());

  CHECK_P(cell_type == c.cell_type);
  CHECK_P(c.verified.empty() || verified == c.verified);
  CHECK_P(c.page.empty() || page == c.page);
  CHECK_P(c.tag.empty() || tag == c.tag);

  if (!c.gate.empty()) {
    // FIXME need a better way of handling gates with multiple outputs...
    if (gate == "add_s" || gate == "add_c") {
      CHECK_P(c.gate == "add_s" || c.gate == "add_c");
    }
    else {
      CHECK_P(gate == c.gate);
    }
  }

  if (!c.args.empty()) {
    CHECK_P(args.size() == c.args.size());
    for (size_t i = 0; i < args.size(); i++) {
      CHECK_P(args[i].tag  == c.args[i].tag);
      CHECK_P(args[i].port == c.args[i].port);
    }
  }

  CHECK_P(c.doc.empty() || doc == c.doc);

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Cell::dump(Dumper& d) {
  d("%-10s",       cell_type_to_name[cell_type].c_str());
  d("%-1s",        verified.c_str());
  d("p%-2s.",      page.c_str());
  d("%-24s ",       tag.c_str());
  d("gate:%-24s ", gate.c_str());

  d("args:(");
  for (auto arg : args) {
    if (arg.port.empty()) {
      d("%s ", arg.tag.c_str());
    }
    else {
      d("%s.%s ", arg.tag.c_str(), arg.port.c_str());
    }
  }
  d(") ");

  d("names:(");
  for (auto name : names) d("%s ", name.c_str());
  d(") ");

  d("\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

bool CellDB::parse_tag(Cell& c, const std::string& tag_comment) {
  static regex pin_tag (R"(^\/\*(.?)(PIN[0-9]{2})\s*\*\/$)");
  static regex sig_tag (R"(^\/\*(.?)(SIG_\w+)\s*\*\/$)");
  static regex bus_tag (R"(^\/\*(.?)(BUS_\w+)\s*\*\/$)");
  static regex cell_tag(R"(^\/\*(.?)p([0-9]{2})\.([A-Z]{4})\s*\*\/$)");

  smatch match;
  if (regex_match(tag_comment, match, pin_tag)) {
    //c.cell_type = CellType::PIN;
    c.verified = match[1].str();
    c.tag = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment, match, sig_tag)) {
    //c.cell_type = CellType::SIGNAL;
    c.verified = match[1].str();
    c.tag = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment, match, bus_tag)) {
    //c.cell_type = CellType::BUS;
    c.verified = match[1].str();
    c.tag = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment,  match, cell_tag)) {
    //c.cell_type = CellType::UNKNOWN;
    c.verified = match[1].str();
    c.page = match[2].str();
    c.tag = match[3].str();
    return true;
  }
  else {
    printf("Could not parse tag comment %s\n", tag_comment.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

set<string> valid_dff_gates    = { "dff9", "dff22", "dff17", "dff20", "dff17_any", "dff13", "dff8n", "dff11", "dff8p" };
set<string> valid_latch_gates  = { "nand_latch", "nor_latch", "tp_latchn", "tp_latchp" };
set<string> valid_pin_gates    = { "pin_out_pull_hilo", "pin_out_pull_hilo_any", "pin_out_dp", "pin_out_hilo" };
set<string> valid_tribuf_gates = { "tri10_np", "tri6_pn", "tri6_nn" };
set<string> valid_logic_gates  = {
  "not1",
  "and2", "and3", "and4",
  "or2", "or3", "or4",
  "xor2", "xnor2",
  "nand2", "nand3", "nand4", "nand5", "nand6", "nand7",
  "nor2", "nor3", "nor4", "nor5", "nor6", "nor8",
  "and_or3", "or_and3", "not_or_and3",
  "add_s", "add_c",
  "mux2n", "mux2p", "amux2", "amux4",
};

bool CellDB::parse_cell_gate(Cell& c, const std::string& gate) {
  if (valid_dff_gates.contains(gate)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::DFF;
    c.gate = gate;
    return true;
  }
  else if (valid_latch_gates.contains(gate)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::LATCH;
    c.gate = gate;
    return true;
  }
  else if (valid_pin_gates.contains(gate)) {
    //CHECK_P(c.cell_type == CellType::PIN);
    c.gate = gate;
    return true;
  }
  else if (valid_tribuf_gates.contains(gate)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::TRIBUF;
    c.gate = gate;
    return true;
  }
  else if (valid_logic_gates.contains(gate)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::LOGIC;
    c.gate = gate;
    return true;
  }
  else {
    printf("Could not parse gate \"%s\"\n", gate.c_str());
    __debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

set<string> valid_dff_types    = { "DFF8p", "DFF8n", "DFF9", "DFF11", "DFF13", "DFF17", "DFF20", "DFF22" };
set<string> valid_latch_types  = { "NorLatch", "NandLatch", "TpLatch" };
set<string> valid_pin_types    = { "PinIn", "PinOut", "PinIO" };

bool CellDB::parse_reg_type(Cell& c, const std::string& type) {

  if (valid_dff_types.contains(type)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::DFF;
    return true;
  }
  else if (valid_latch_types.contains(type)) {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::LATCH;
    return true;
  }
  else if (valid_pin_types.contains(type)) {
    if (type == "PinIn")  c.cell_type = CellType::PIN_IN;
    if (type == "PinOut") c.cell_type = CellType::PIN_OUT;
    if (type == "PinIO")  c.cell_type = CellType::PIN_IO;
    return true;
  }
  else if (type == "Gate") {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::LOGIC;
    return true;
  }
  else if (type == "Signal") {
    //CHECK_P(c.cell_type == CellType::SIGNAL);
    return true;
  }
  else if (type == "Bus") {
    //CHECK_P(c.cell_type == CellType::BUS);
    //c.cell_type = CellType::BUS;
    return true;
  }
  else if (type == "wire") {
    CHECK_P(c.cell_type == CellType::UNKNOWN);
    c.cell_type = CellType::LOGIC;
    return true;
  }
  else {
    printf("Could not parse cell type %s\n", type.c_str());
    __debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_cell_arg(Cell& c, const std::string& arg) {

  static regex cell_arg(R"(^(?:\w+\.)*_?([A-Z]{4})\w*(\.[^.]+)?)");
  static regex cell_func_arg(R"(^(?:\w+\.)*([A-Z]{4})\w+\(\))");

  static regex bus_arg (R"(^(?:\w+\.)*(BUS_[^.]*)(\.[^.]+)?)");
  static regex sig_arg (R"(^(?:\w+\.)*(SIG_[^.]*)(\.[^.]+)?)");
  static regex pin_arg (R"(^(?:\w+\.)*(PIN\d{2})_[^.]*(\.[^.]+)?)");

  smatch match;
  if (regex_match(arg, match, cell_arg)) {
    c.args.push_back({match[1].str(), ""});
    return true;
  }
  else if (regex_match(arg, match, cell_func_arg)) {
    c.args.push_back({match[1].str(), ""});
    return true;
  }
  else if (regex_match(arg, match, bus_arg)) {
    c.args.push_back({match[1].str(), ""});
    return true;
  }
  else if (regex_match(arg, match, sig_arg)) {
    c.args.push_back({match[1].str(), ""});
    return true;
  }
  else if (regex_match(arg, match, pin_arg)) {
    c.args.push_back({match[1].str(), ""});
    return true;
  }
  else {
    printf("Could not parse arg \"%s\"\n", arg.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <input>, <input>...

bool CellDB::parse_cell_arglist(Cell& c, const string& arglist_c) {
  static regex arg_regex(R"(\s*_?(.+?)(,|$))");

  string arglist = arglist_c;
  arglist.erase(remove_if(arglist.begin(), arglist.end(), ::isspace), arglist.end());

  bool result = true;
  for (sregex_iterator i = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); i != sregex_iterator(); i++) {
    result &= parse_cell_arg(c, (*i)[1].str().c_str());
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <gate>( <input list> );

bool CellDB::parse_cell_def(Cell& c, const string& value) {
  static regex valid_value(R"((.*?)\s*\((.*)\);\s*(.*))");

  //printf("value %s\n", value.c_str());

  bool result = true;
  smatch match;
  if (regex_match(value, match, valid_value)) {
    result &= parse_cell_gate(c, match[1].str());
    result &= parse_cell_arglist(c, match[2].str());
    c.doc = match[3].str();
  }
  else {
    printf("Could not parse value %s\n", value.c_str());
    result = false;
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// foo.bar.baz._ABCD_OTHER_STUFF
// foo.bar.baz.ABCD_OTHER_STUFF

bool CellDB::parse_cell_name(Cell& c, const string& name) {
  static regex valid_cell_name(R"(^(?:\w+\.)*_?([A-Z]{4}\w*)\s*$)");

  static regex valid_pin_name(R"(^(PIN\d{2}_\w+)$)");

  static regex valid_sig_name(R"(^(SIG_\w+)$)");

  static regex valid_bus_name(R"(^(BUS_\w+)$)");

  smatch match;
  if (regex_match(name, match, valid_cell_name)) {
    c.names.insert(match[1].str());
    return true;
  }
  else if (regex_match(name, match, valid_pin_name)) {
    c.names.insert(match[1].str());
    return true;
  }
  else if (regex_match(name, match, valid_sig_name)) {
    c.names.insert(match[1].str());
    return true;
  }
  else if (regex_match(name, match, valid_bus_name)) {
    c.names.insert(match[1].str());
    return true;
  }
  else {
    printf("Could not parse name %s\n", name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_tribuf_bus_target(Cell& c, const string& bus_name) {
  static regex valid_bus_name(R"(^(?:\w+\.)*(BUS_\w+)$)");

  smatch match;
  if (regex_match(bus_name, match, valid_bus_name)) {
    std::string bus = match[1].str();
    Cell* bus_cell = get_or_create_cell(bus);
    bus_cell->gate = "bus";
    bus_cell->args.push_back({c.tag, ""});

    return true;
  } else {
    printf("Could not parse bus name \"%s\"\n", bus_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_pin_name(Cell& c, const string& pin_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(PIN\d{2}_\w+)$)");

  smatch match;
  if (regex_match(pin_name, match, valid_pin_name)) {
    c.names.insert(match[1].str());
    return true;
  } else {
    printf("Could not parse pin name %s\n", pin_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_sig_name(Cell& c, const string& sig_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(SIG_\w+)\s*$)");

  smatch match;
  if (regex_match(sig_name, match, valid_pin_name)) {
    c.names.insert(match[1].str());
    return true;
  } else {
    printf("Could not parse sig name %s\n", sig_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// everything after the comment tag

bool CellDB::parse_rest(Cell& c, const string& rest) {
  static regex member_decl(R"(^(\w+)\s+(\w+)\s*;.*)");
  static regex member_assign(R"(^(?:\w+\.)*(\w+)\s*=\s*(.+;).*)");
  static regex local_decl(R"(^wire\s+(\w+)\s*=\s*(.*)$)");
  static regex func_decl(R"(^(inline\s+)?wire\s+(\w+)\s*.*\{\s*return\s*(.*)\}.*)");
  static regex wire_decl(R"(^wire\s*(\w+);.*$)");
  static regex signal_decl(R"(^Signal\s*(\w+);.*$)");
  static regex tri_call(R"(^(.*)\.\s*(tri\w+\(.*$))");
  static regex dff_call(R"(^(.*)\.\s*(dff\w+\(.*$))");
  static regex latch_call(R"(^(.*)\.\s*(.*_latch[pn]?\(.*$))");
  static regex sig_set_call(R"(^(.*)\.\s*set\((.*)\).*)");

  static regex pin_call(R"(^(.*)\.\s*(pin\w+\(.*$))");

  //rest.erase(remove_if(rest.begin(), rest.end(), ::isspace), rest.end());
  bool result = true;

  smatch match;
  if (regex_match(rest, match, member_decl)) {
    result &= parse_reg_type(c, match[1].str());
    result &= parse_cell_name(c, match[2].str());
  }
  else if (regex_match(rest, match, local_decl)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, func_decl)) {
    result &= parse_cell_name(c, match[2].str());
    result &= parse_cell_def(c, match[3].str());
  }
  else if (regex_match(rest, match, wire_decl)) {
    result &= parse_cell_name(c, match[1].str());
  }
  else if (regex_match(rest, match, signal_decl)) {
    result &= parse_cell_name(c, match[1].str());
  }
  else if (regex_match(rest, match, tri_call)) {
    // parse the tribuf first
    result &= parse_cell_def(c, match[2].str());

    // so we can add it as an input to the bus.
    result &= parse_tribuf_bus_target(c, match[1].str());
  }
  else if (regex_match(rest, match, pin_call)) {
    result &= parse_pin_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, dff_call)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, latch_call)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, sig_set_call)) {
    result &= parse_sig_name(c, match[1].str());
    result &= parse_cell_arglist(c, match[2].str());
    c.gate = "set";
  }
  else if (regex_match(rest, match, member_assign)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else {
    printf("Could not parse rest : \"%s\"\n", rest.c_str());
    result = false;
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_line(Cell& c, const std::string& line) {
  static regex tagged_line_regex(R"(^\s*(\/\*.*?\*\/)\s*(.*))");

  bool result = true;
  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {
    result &= parse_tag(c, matches[1].str());
    result &= parse_rest(c, matches[2].str());

    if (!result) {
      printf("Could not parse line : \"%s\"\n", line.c_str());
    }
  }
  else {
    result = false;
  }

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_file(const std::string& path) {
  ConsoleDumper d;

  bool result = true;
  printf("Parsing %s\n", path.c_str());
  std::ifstream lines(path);
  for (string line; getline(lines, line); ) {
    total_lines++;
    Cell c;
    if (parse_line(c, line)) {
      total_tagged_lines++;
      CHECK_N(c.tag.empty());
      get_or_create_cell(c.tag)->merge(c);
    }
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CellDB::parse_dir(const std::string& path) {
  (void)path;

  ConsoleDumper d;

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  bool result = true;
#if 0
  {
    string line = R"(  /*PIN58*/ PinIn PIN58_VCC;)";
    Cell c;
    parse_line(c, line);
    c.dump(d);
  }
#else
  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      static regex src_names(R"(^GateBoy\w*\.(cpp|h)$)");

      std::string p = entry.path().filename().string();
      smatch matches;
      if (regex_match(p, matches, src_names)) {
        result &= parse_file(entry.path().string());
      }
    }
  }
#endif

  //----------------------------------------
  // Postprocess the cells.

  for (auto& [tag, cell] : tag_map) {
    // Tribufs don't get names, add the tag as the default name.
    if (cell->cell_type == CellType::TRIBUF) {
      CHECK_P(cell->names.empty());
      cell->names.insert(cell->tag);
    }

    // All pins without gates are inputs from the outside world.
    if (cell->cell_type == CellType::PIN_IN) {
      CHECK_P(cell->gate.empty());
      cell->gate = "pin_input";
    }

    // All buses without args are inputs from the CPU.
    if (cell->tag.starts_with("BUS_")) {
      cell->cell_type = CellType::BUS;
      if (cell->args.size()) {
        cell->gate = "bus";
      }
      else {
        cell->gate = "bus_input";
      }
    }

    // All signals without gates are inputs from the CPU.
    if (cell->tag.starts_with("SIG_")) {
      if (cell->gate.empty()) {
        CHECK_P(cell->cell_type == CellType::UNKNOWN);
        cell->cell_type = CellType::SIG_IN;
        cell->gate = "sig_input";
      }
      else {
        CHECK_P(cell->cell_type == CellType::UNKNOWN);
        cell->cell_type = CellType::SIG_OUT;
      }
    }
  }

  //----------------------------------------
  // Check that all cells are sane.

  for (auto& [tag, cell] : tag_map) cell->sanity_check();

  //----------------------------------------
  // Check that all cells are used by some other cell, or are output cells.

  for (auto& [tag, cell] : tag_map) {
    for (const auto& arg : cell->args) {
      CHECK_P(has_cell(arg.tag));
      get_cell(arg.tag)->mark++;
    }
  }

  bool any_unused = false;
  for (auto& [tag, cell] : tag_map) {
    if (cell->cell_type == CellType::PIN_OUT) continue;
    if (cell->cell_type == CellType::SIG_OUT) continue;

    // HACK - this is the one bus bit that isn't used by anything other than the OAM ram itself...
    if (cell->tag == "BUS_OAM_A01n") continue;

    if (cell->mark == 0) {
      printf("Cell %s unused\n", (*cell->names.begin()).c_str());
      any_unused = true;
    }
  }

  if (any_unused) __debugbreak();

  //----------------------------------------
  // Done, dump stats.

  printf("Total lines %d\n", total_lines);
  printf("Total tagged lines %d\n", total_tagged_lines);
  printf("Tag map size %zd\n", tag_map.size());
  //printf("Unused tags %d\n", unused_count);

  //ConsoleDumper d;
  //for (auto& [key, value] : cell_map) value->dump(d);
  //for (auto& [key, value] : pin_map)  value->dump(d);
  //for (auto& [key, value] : bus_map)  value->dump(d);
  //for (auto& [key, value] : sig_map)  value->dump(d);

  printf("\n");

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

