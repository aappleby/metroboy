#include "Plait/CellDB.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

set<string> all_tags;
set<string> verified_tags;

int total_tagged_lines = 0;

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
  "Signal",
  "NorLatch",
  "NandLatch",
  "TpLatch",
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
};

set<string> pages;

//-----------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

bool parse_tag_comment(const std::string& tag_comment) {
  static regex valid_tag(R"(^\/\*(.?)p([0-9]{2})\.([A-Z]{4})\*\/$)");

  smatch matches;
  if (regex_match(tag_comment,  matches, valid_tag)) {
    string verified = matches[1].str();
    string page = matches[2].str();
    string tag = matches[3].str();
    if (verified == "#") verified_tags.insert(tag);
    pages.insert(page);
    all_tags.insert(tag);

    return true;
  }
  else {
    printf("Could not parse tag comment %s\n", tag_comment.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_gate_type(const std::string& type) {
  if (valid_gate_types.contains(type)) {
    return true;
  }
  else {
    printf("Could not parse gate type \"%s\"\n", type.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_reg_type(const std::string& type) {
  if (valid_reg_types.contains(type)) {
    return true;
  }
  else {
    printf("Could not parse reg type %s\n", type.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_cell_input(const std::string& arg) {

  static regex tagged_arg(R"(^(?:\w+\.)*[A-Z]{4}.*)");
  static regex bus_arg(R"(^(?:\w+\.)*BUS_.*)");
  static regex sig_arg(R"(^(?:\w+\.)*SIG_.*)");
  static regex pin_arg(R"(^(?:\w+\.)*PIN\d{2}_.*)");

  smatch matches;

  if (regex_match(arg, matches, tagged_arg)) {
    return true;
  }
  else if (regex_match(arg, matches, bus_arg)) {
    return true;
  }
  else if (regex_match(arg, matches, sig_arg)) {
    return true;
  }
  else if (regex_match(arg, matches, pin_arg)) {
    return true;
  }
  else {
    printf("Could not parse arg %s\n", arg.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------
// <input>, <input>...

bool parse_input_list(const string& arglist) {
  static regex arg_regex("\\s*_?(.*?),");

  bool result = true;
  for (sregex_iterator i = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); i != sregex_iterator(); i++) {
    result &= parse_cell_input((*i)[1].str().c_str());
  }
  return result;
}

//-----------------------------------------------------------------------------
// <cell_type> ( <input list> );

bool parse_cell_def(const string& value) {
  static regex valid_value(R"((.*?)\s*\((.*)\);.*)");

  //printf("value %s\n", value.c_str());

  bool result = true;
  smatch matches;
  if (regex_match(value, matches, valid_value)) {
    result &= parse_gate_type(matches[1].str());
    result &= parse_input_list(matches[2].str());
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

bool parse_cell_name(const string& name) {
  static regex valid_name(R"(^(?:\w+\.)*_?([A-Z]{4})(_?\w*)\s*$)");

  //printf("name %s\n", name.c_str());

  smatch matches;
  if (regex_match(name, matches, valid_name)) {
    return true;
  } else {
    printf("Could not parse name %s\n", name.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------
// everything after the comment tag

bool parse_rest(const string& rest) {
  static regex member_decl(R"(^(\w+)\s+(\w+);.*)");
  static regex member_assign(R"(^(?:\w+\.)*(\w+)\s*=\s*(.+;).*)");
  static regex local_decl(R"(^wire\s+(\w+)\s*=\s*(.*)$)");
  static regex func_decl(R"(^(inline\s+)?wire\s+(\w+)\s*.*\{\s*return\s*(.*)\}.*)");
  static regex wire_decl(R"(^wire\s*(\w+);.*$)");
  static regex signal_decl(R"(^Signal\s*(\w+);.*$)");
  static regex tri_call(R"(^(.*)\.\s*(tri\w+\(.*$))");
  static regex dff_call(R"(^(.*)\.\s*(dff\w+\(.*$))");
  static regex latch_call(R"(^(.*)\.\s*(.*_latch[pn]?\(.*$))");
  static regex set_call(R"(^(.*)\.\s*set\((.*)\).*)");

  smatch matches;

  //line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

  bool result = true;

  if (regex_match(rest, matches, member_decl)) {
    result &= parse_reg_type(matches[1].str());
    result &= parse_cell_name(matches[2].str());
  }
  else if (regex_match(rest, matches, local_decl)) {
    result &= parse_cell_name(matches[1].str());
    result &= parse_cell_def(matches[2].str());
  }
  else if (regex_match(rest, matches, func_decl)) {
    result &= parse_cell_name(matches[2].str());
    result &= parse_cell_def(matches[3].str());
  }
  else if (regex_match(rest, matches, wire_decl)) {
    result &= parse_cell_name(matches[1].str());
  }
  else if (regex_match(rest, matches, signal_decl)) {
    result &= parse_cell_name(matches[1].str());
  }
  else if (regex_match(rest, matches, tri_call)) {
    //string bus = matches[1].str();
    result &= parse_cell_def(matches[2].str());
  }
  else if (regex_match(rest, matches, dff_call)) {
    result &= parse_cell_name(matches[1].str());
    result &= parse_cell_def(matches[2].str());
  }
  else if (regex_match(rest, matches, latch_call)) {
    result &= parse_cell_name(matches[1].str());
    result &= parse_cell_def(matches[2].str());
  }
  else if (regex_match(rest, matches, set_call)) {
    result &= parse_cell_name(matches[1].str());
    result &= parse_cell_def(matches[2].str());
  }
  else if (regex_match(rest, matches, member_assign)) {
    result &= parse_cell_name(matches[1].str());
    result &= parse_cell_def(matches[2].str());
  }
  else {
    result = false;
  }
  return result;
}

//-----------------------------------------------------------------------------

void parse_line(string& line) {
  static regex tagged_line_regex(R"(^\s*(\/\*.*?\*\/)\s*(.*))");

  bool result = true;

  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {
    total_tagged_lines++;
    string tag = matches[1].str();
    string rest = matches[2].str();
    result &= parse_tag_comment(tag);
    result &= parse_rest(rest);
  }

  if (!result) {
    printf("Could not parse line : \"%s\"\n", line.c_str());
  }
}

//-----------------------------------------------------------------------------

void parse_file(string path) {
  printf("Parsing %s\n", path.c_str());
  std::ifstream lines(path);
  for (string line; getline(lines, line); ) {
    parse_line(line);
  }
}

//-----------------------------------------------------------------------------

void parse_dir(string path) {
  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      static regex src_names(R"(^GateBoy\w*\.(cpp|h)$)");

      std::string p = entry.path().filename().string();
      smatch matches;
      if (regex_match(p, matches, src_names)) {
        parse_file(entry.path().string());
      }
    }
  }

  printf("Total tagged lines %d\n", total_tagged_lines);
  printf("Unique tags %zd\n", all_tags.size());
  printf("Verified tags %zd\n", verified_tags.size());
}

//-----------------------------------------------------------------------------

#if 0

#include <filesystem>

void blah() {
  //const fs::path pathToShow = filesystem::current_path();
  //const fs::path pathToShow = ".";

  for (const auto& entry : filesystem::directory_iterator(".")) {
    const auto filenameStr = entry.path().filename().string();
    if (entry.is_directory()) {
      printf("dir %s\n", filenameStr.c_str());
    }
    else if (entry.is_regular_file()) {
      printf("file %s\n", filenameStr.c_str());
    }
    else {
      printf("??? %s\n", filenameStr.c_str());
    }
  }
}

#endif