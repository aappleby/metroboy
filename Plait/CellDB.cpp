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

set<string> valid_reg_types = {
  "DFF8p",
  "DFF8n",
  "DFF9",
  "DFF11",
  "DFF13",
  "DFF17",
  "DFF20",
  "DFF22",
  "wire",
  "Gate",
  "NorLatch",
  "NandLatch",
  "TpLatch",

  "Signal",
  "Bus",
  "PinIn",
  "PinOut",
  "PinIO",
};

set<string> valid_gate_types = {
  "not1b",
  "and2", "and3", "and4",
  "or2", "or3", "or4",
  "xor2",
  "xnor2b",

  "nand2b",
  "nand3b",
  "nand4b",
  "nand5b",
  "nand6b",
  "nand7b",

  "nor2b",
  "nor3b",
  "nor4b",
  "nor5b",
  "nor6b",
  "nor8b",

  "and_or3",
  "or_and3",
  "not_or_and3",

  "add_s",
  "add_c",

  "mux2nb",
  "mux2p",
  "amux2",
  "amux4",

  "dff9",
  "dff22",
  "dff17",
  "dff20",
  "dff17_any",
  "dff13",
  "dff8n",
  "dff11",
  "dff8p",

  "tri10_np",
  "tri6_pn",
  "tri6_nn",

  "nand_latch",
  "nor_latch",
  "tp_latchn",
  "tp_latchp",

  "pin_out_pull_hilo",
  "pin_out_pull_hilo_any",
  "pin_out_dp",
  "pin_out_hilo",
};

void Cell::sanity_check() const {
  CHECK_N(page.empty());
  CHECK_N(tag.empty());
  CHECK_N(gate.empty());
  CHECK_N(args.empty());
}

void Cell::merge(const Cell& c) {
  if (verified.empty()) verified = c.verified;
  if (page.empty())     page = c.page;
  if (tag.empty())      tag = c.tag;
  if (gate.empty())     gate = c.gate;
  if (bus.empty())      bus = c.bus;
  if (pin.empty())      pin = c.pin;
  if (sig.empty())      sig = c.sig;
  if (doc.empty())      doc = c.doc;
  if (args.empty())     args = c.args;
  for (auto n : c.names) names.insert(n);

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

  CHECK_P(c.bus.empty() || bus == c.bus);
  CHECK_P(c.pin.empty() || pin == c.pin);
  CHECK_P(c.sig.empty() || sig == c.sig);
  CHECK_P(c.doc.empty() || doc == c.doc);

  if (!c.args.empty()) {
    CHECK_P(args.size() == c.args.size());
    for (size_t i = 0; i < args.size(); i++) {
      CHECK_P(args[i] == c.args[i]);
    }
  }
}

void Cell::dump(Dumper& d) {
  //d("p%s.%s = %s(", page.c_str(), tag.c_str(), gate.c_str());
  //for (auto arg : args)   d("%s ", arg.c_str());
  //d(");\n");

  d("%s", verified.c_str());
  d("p%s.", page.c_str());
  d("%s ", tag.c_str());
  d("%-10s ", gate.c_str());
  d("%-20s ", bus.c_str());
  d("("); for (auto arg : args)   d("%s ", arg.c_str());  d(") ");
  d("("); for (auto name : names) d("%s ", name.c_str()); d(") ");
  d("%s ", doc.c_str());
  d("\n");

  /*
  d("Page  : %s\n", page.c_str());
  d("Tag   : %s\n", tag.c_str());
  d("Gate  : %s\n", gate.c_str());
  d("Bus   : %s\n", bus.c_str());
  if (!doc.empty()) d("Doc   : \"%s\"\n", doc.c_str());
  d("Args  : "); for (auto arg : args)   d("%s ", arg.c_str());  d("\n");
  d("Names : "); for (auto name : names) d("%s ", name.c_str()); d("\n");
  d("\n");
  */
}

//-----------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

bool CellDB::parse_tag_comment(Cell& c, const std::string& tag_comment) {
  static regex valid_tag(R"(^\/\*(.?)p([0-9]{2})\.([A-Z]{4})\*\/$)");

  static regex pin_tag(R"(^\/\*\s*(.?)(PIN[0-9]{2})\s*\*\/$)");
  static regex sig_tag(R"(^\/\*\s*(SIG_\w+)\s*\*\/$)");
  static regex bus_tag(R"(^\/\*\s*(BUS_\w+)\s*\*\/$)");

  smatch match;
  if (regex_match(tag_comment,  match, valid_tag)) {
    c.verified = match[1].str();
    c.page = match[2].str();
    c.tag = match[3].str();
    return true;
  }
  else if (regex_match(tag_comment, match, pin_tag)) {
    //printf("Pin tag %s\n", tag_comment.c_str());
    c.verified = match[1].str();
    c.pin = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment, match, sig_tag)) {
    //printf("Sig tag %s\n", tag_comment.c_str());
    c.sig = match[1].str();
    return true;
  }
  else if (regex_match(tag_comment, match, bus_tag)) {
    //printf("Bus tag %s\n", tag_comment.c_str());
    c.bus = match[1].str();
    return true;
  }
  else {
    printf("Could not parse tag comment %s\n", tag_comment.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool CellDB::parse_gate_type(Cell& c, const std::string& type) {
  if (valid_gate_types.contains(type)) {
    c.gate = type;
    return true;
  }
  else {
    printf("Could not parse gate type \"%s\"\n", type.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool CellDB::parse_reg_type(Cell& c, const std::string& type) {
  (void)c;
  if (valid_reg_types.contains(type)) {
    return true;
  }
  else {
    printf("Could not parse reg type %s\n", type.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool CellDB::parse_cell_input(Cell& c, const std::string& arg) {

  static regex tagged_arg(R"(^(?:\w+\.)*([A-Z]{4}).*)");
  static regex bus_arg(R"(^(?:\w+\.)*(BUS_[^.]*)(?:\.[^.]+)*)");
  static regex sig_arg(R"(^(?:\w+\.)*(SIG_[^.]*)(?:\.[^.]+)*)");
  static regex pin_arg(R"(^(?:\w+\.)*(PIN\d{2})_[^.]*(?:\.[^.]+)*)");

  smatch match;
  if (regex_match(arg, match, tagged_arg)) {
    c.args.push_back(match[1].str());
    return true;
  }
  else if (regex_match(arg, match, bus_arg)) {
    //printf("bus arg %s\n", arg.c_str());
    c.args.push_back(match[1].str());
    return true;
  }
  else if (regex_match(arg, match, sig_arg)) {
    c.args.push_back(match[1].str());
    return true;
  }
  else if (regex_match(arg, match, pin_arg)) {
    c.args.push_back(match[1].str());
    return true;
  }
  else {
    printf("Could not parse arg \"%s\"\n", arg.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------
// <input>, <input>...

bool CellDB::parse_cell_input_list(Cell& c, const string& arglist_c) {
  static regex arg_regex(R"(\s*_?(.+?)(,|$))");

  string arglist = arglist_c;
  arglist.erase(remove_if(arglist.begin(), arglist.end(), ::isspace), arglist.end());

  bool result = true;
  for (sregex_iterator i = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); i != sregex_iterator(); i++) {
    result &= parse_cell_input(c, (*i)[1].str().c_str());
  }
  return result;
}

//-----------------------------------------------------------------------------
// <cell_type> ( <input list> );

bool CellDB::parse_cell_def(Cell& c, const string& value) {
  static regex valid_value(R"((.*?)\s*\((.*)\);\s*(.*))");

  //printf("value %s\n", value.c_str());

  bool result = true;
  smatch match;
  if (regex_match(value, match, valid_value)) {
    result &= parse_gate_type(c, match[1].str());
    result &= parse_cell_input_list(c, match[2].str());
    c.doc = match[3].str();
  }
  else {
    printf("Could not parse value %s\n", value.c_str());
    result = false;
  }
  return result;
}

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------

bool CellDB::parse_bus_name(Cell& c, const string& bus_name) {
  static regex valid_bus_name1(R"(^(?:\w+\.)*(BUS_\w+\[\s*\d+\])$)");

  static regex valid_bus_name2(R"(^(?:\w+\.)*(BUS_\w+)$)");

  smatch match;
  if (regex_match(bus_name, match, valid_bus_name1)) {
    c.bus = match[1].str();
    return true;
  }
  else if (regex_match(bus_name, match, valid_bus_name2)) {
    c.bus = match[1].str();
    return true;
  } else {
    printf("Could not parse bus name \"%s\"\n", bus_name.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

bool CellDB::parse_sig_name(Cell& c, const string& sig_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(SIG_\w+)$)");

  smatch match;
  if (regex_match(sig_name, match, valid_pin_name)) {
    c.names.insert(match[1].str());
    return true;
  } else {
    printf("Could not parse sig name %s\n", sig_name.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------
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
    result &= parse_bus_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
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
    result &= parse_cell_input_list(c, match[2].str());
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

//-----------------------------------------------------------------------------

bool CellDB::parse_line(Cell& c, const std::string& line) {
  static regex tagged_line_regex(R"(^\s*(\/\*.*?\*\/)\s*(.*))");

  bool result = true;
  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {
    result &= parse_tag_comment(c, matches[1].str());
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

//-----------------------------------------------------------------------------

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

      if (!c.tag.empty()) {
        CHECK_P(c.tag.size() == 4);
        cell_map[c.tag].merge(c);
      }
      else if (!c.pin.empty()) {
        pin_map[c.pin].merge(c);
      }
      else if (!c.sig.empty()) {
        sig_map[c.sig].merge(c);
      }
      else if (!c.bus.empty()) {
        bus_map[c.bus].merge(c);
      }
      else {
        printf("Don't know what to do with this\n");
      }
    }
  }
  return result;
}

//-----------------------------------------------------------------------------

bool CellDB::parse_dir(const std::string& path) {
  (void)path;

  bool result = true;
#if 0
  {
    ConsoleDumper d;
    string line = R"(  /*SIG_CPU_INT_STAT  */ Signal SIG_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int)";
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

  //----------------------------------------
  // Check that all cells are sane and all args have a corresponding cell

  for (const auto& [key, value] : cell_map) {
    value.sanity_check();

    for (const auto& arg : value.args) {
      if (!has_cell(arg)) {
        printf("Can't find definition for cell %s\n", arg.c_str());
      }
      else {
        get_cell(arg).mark++;
      }
    }
  }

  //----------------------------------------
  // Check that all cells are used by some other cell or pin

  // FIXME - need cells for pins...

  for (auto& [key, value] : cell_map) {
    value.mark = 0;
    if (!value.bus.empty()) value.mark++;
  }

  for (auto& [key, value] : cell_map) {
    for (const auto& arg : value.args) {
      if (has_cell(arg)) get_cell(arg).mark++;
    }
  }
  for (auto& [key, value] : pin_map) {
    for (const auto& arg : value.args) {
      if (has_cell(arg)) get_cell(arg).mark++;
    }
  }
  for (auto& [key, value] : sig_map) {
    for (const auto& arg : value.args) {
      if (has_cell(arg)) get_cell(arg).mark++;
    }
  }

  int unused_count = 0;
  for (auto& [key, value] : cell_map) {
    if (value.mark == 0) {
      printf("Cell %s unused\n", value.tag.c_str());
      unused_count++;
    }
  }

  //----------------------------------------

  printf("Total lines %d\n", total_lines);
  printf("Total tagged lines %d\n", total_tagged_lines);
  printf("Cell map size %zd\n", cell_map.size());
  printf("Pin map size %zd\n", pin_map.size());
  printf("Sig map size %zd\n", sig_map.size());
  printf("Unused tags %d\n", unused_count);

  //for (auto& [key, value] : cell_map) printf("cell key %s\n", key.c_str());

  //for (auto tag : all_tags) printf("%s ", tag.c_str());
  //for (auto bus : all_buses) printf("%s\n", bus.c_str());
  //for (auto name : all_names) printf("%s\n", name.c_str());

  printf("\n");
#endif

  return result;
}

//-----------------------------------------------------------------------------
