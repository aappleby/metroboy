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

#include "json/single_include/nlohmann/json.hpp"

using namespace std;

NLOHMANN_JSON_SERIALIZE_ENUM( DieCellType, {
  {DieCellType::UNKNOWN, "UNKNOWN"},
  {DieCellType::PIN_IN,  "PIN_IN"},
  {DieCellType::PIN_OUT, "PIN_OUT"},
  {DieCellType::PIN_IO,  "PIN_IO"},
  {DieCellType::PIN_CLK, "PIN_CLK"},
  {DieCellType::SIG_IN,  "SIG_IN"},
  {DieCellType::SIG_OUT, "SIG_OUT"},
  {DieCellType::BUS,     "BUS"},
  {DieCellType::DFF,     "DFF"},
  {DieCellType::LATCH,   "LATCH"},
  {DieCellType::TRIBUF,  "TRIBUF"},
  {DieCellType::ADDER,   "ADDER"},
  {DieCellType::LOGIC,   "LOGIC"},
});

std::map<DieCellType, string> cell_type_to_name = {
  {DieCellType::UNKNOWN, "UNKNOWN"},
  {DieCellType::PIN_IN,  "PIN_IN"},
  {DieCellType::PIN_OUT, "PIN_OUT"},
  {DieCellType::PIN_IO,  "PIN_IO"},
  {DieCellType::PIN_CLK, "PIN_CLK"},
  {DieCellType::SIG_IN,  "SIG_IN"},
  {DieCellType::SIG_OUT, "SIG_OUT"},
  {DieCellType::BUS,     "BUS"},
  {DieCellType::DFF,     "DFF"},
  {DieCellType::LATCH,   "LATCH"},
  {DieCellType::TRIBUF,  "TRIBUF"},
  {DieCellType::ADDER,   "ADDER"},
  {DieCellType::LOGIC,   "LOGIC"},
};

std::map<DieCellType, std::vector<std::string>> cell_type_to_out_ports = {
  {DieCellType::UNKNOWN, {}},
  {DieCellType::PIN_IN,  {"qp"}},
  {DieCellType::PIN_OUT, {}},
  {DieCellType::PIN_IO,  {"qp"}},
  {DieCellType::PIN_CLK, {"clock_good", "clock"}},
  {DieCellType::SIG_IN,  {"qp"}},
  {DieCellType::SIG_OUT, {}},
  {DieCellType::BUS,     {"qp"}},
  {DieCellType::DFF,     {"qp", "qn"}},
  {DieCellType::LATCH,   {"qp", "qn"}},
  {DieCellType::TRIBUF,  {"qp"}},
  {DieCellType::ADDER,   {"sum", "carry"}},
  {DieCellType::LOGIC,   {"qp"}},
};

std::map<string, DieCellType> decl_to_cell_type = {
  {"DFF8p",     DieCellType::DFF},
  {"DFF8n",     DieCellType::DFF},
  {"DFF9",      DieCellType::DFF},
  {"DFF11",     DieCellType::DFF},
  {"DFF13",     DieCellType::DFF},
  {"DFF17",     DieCellType::DFF},
  {"DFF20",     DieCellType::DFF},
  {"DFF22",     DieCellType::DFF},
  {"NorLatch",  DieCellType::LATCH},
  {"NandLatch", DieCellType::LATCH},
  {"TpLatch",   DieCellType::LATCH},
  {"PinIn",     DieCellType::PIN_IN},
  {"PinOut",    DieCellType::PIN_OUT},
  {"PinIO",     DieCellType::PIN_IO},
  {"PinClock",  DieCellType::PIN_CLK},
  {"SigIn",     DieCellType::SIG_IN},
  {"SigOut",    DieCellType::SIG_OUT},
  {"Bus",       DieCellType::BUS},
  {"Gate",      DieCellType::LOGIC},
  {"Adder",     DieCellType::ADDER},
};

std::map<std::string, std::vector<std::string>> gate_to_in_ports = {
  {"pin_in", {}},

  {"pin_out_dp",            {"DP"}},
  {"pin_out_hilo",          {"HI", "LO"}},

  {"pin_io_out_pull_hilo",     {"PUn", "HI", "LO"}},
  {"pin_io_out_pull_hilo_any", {"PUn", "HI", "LO"}},

  {"pin_clk", {}},

  {"sig_in", {}},
  {"sig_out", {"Dp"}},

  // bus is special, it has a variable number of inputs

  {"dff9",      {"CLKp", "SETn", "Dp"}},
  {"dff22",     {"CLKp", "SETn", "RSTn", "Dp"}},
  {"dff17",     {"CLKp", "RSTn", "Dp"}},
  {"dff20",     {"CLKn", "LOADp", "Dp"}},
  {"dff17_any", {"CLKp", "RSTn", "Dp"}},
  {"dff13",     {"CLKp", "RSTn", "Dp"}},
  {"dff8n",     {"CLKn", "Dn"}},
  {"dff11",     {"CLKp", "RSTn", "Dp"}},
  {"dff8p",     {"CLKp", "Dn"}},

  {"nand_latch", {"SETn", "RSTn"}},
  {"nor_latch",  {"SETp", "RSTp"}},
  {"tp_latchn",  {"HOLDn", "Dp"}},
  {"tp_latchp",  {"HOLDp", "Dp"}},

  {"tri10_np", {"OEn", "Dp"}},
  {"tri6_pn",  {"OEp", "Dn"}},
  {"tri6_nn",  {"OEn", "Dn"}},

  {"add3", {"a", "b", "carry"}},

  {"not1",  {"a"}},
  {"and2",  {"a", "b"}},
  {"and3",  {"a", "b", "c"}},
  {"and4",  {"a", "b", "c", "d"}},
  {"or2",   {"a", "b"}},
  {"or3",   {"a", "b", "c"}},
  {"or4",   {"a", "b", "c", "d"}},
  {"xor2",  {"a", "b"}},
  {"xnor2", {"a", "b"}},
  {"nand2", {"a", "b"}},
  {"nand3", {"a", "b", "c"}},
  {"nand4", {"a", "b", "c", "d"}},
  {"nand5", {"a", "b", "c", "d", "e"}},
  {"nand6", {"a", "b", "c", "d", "e", "f"}},
  {"nand7", {"a", "b", "c", "d", "e", "f", "g"}},
  {"nand8", {"a", "b", "c", "d", "e", "f", "g", "h"}},
  {"nor2",  {"a", "b"}},
  {"nor3",  {"a", "b", "c"}},
  {"nor4",  {"a", "b", "c", "d"}},
  {"nor5",  {"a", "b", "c", "d", "e"}},
  {"nor6",  {"a", "b", "c", "d", "e", "f"}},
  {"nor7",  {"a", "b", "c", "d", "e", "f", "g"}},
  {"nor8",  {"a", "b", "c", "d", "e", "f", "g", "h"}},

  {"and_or3",      {"a", "b", "c"}},
  {"or_and3",      {"a", "b", "c"}},
  {"not_or_and3",  {"a", "b", "c"}},

  {"mux2n", {"mux", "a1", "a0"}},
  {"mux2p", {"mux", "a1", "a0"}},
  {"amux2", {"a0", "b0", "a1", "b1"}},
  {"amux4", {"a0", "b0", "a1", "b1", "a2", "b2", "a3", "b3"}},
};


//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, DieCell*& c) {
  c = new DieCell();

  c->cell_type    = j.value("cell_type",    DieCellType::UNKNOWN);
  c->page         = j.value("page",         "<no_page>");
  c->tag          = j.value("tag",          "<no_tag>");
  c->gate         = j.value("gate",         "<no_gate>");
  c->long_name    = j.value("long_name",    "<no_name>");
  c->doc          = j.value("doc",          "<no_doc>");
  c->input_ports  = j.value("input_ports",  std::vector<std::string>());
  c->output_ports = j.value("output_ports", std::vector<std::string>());
  c->fanout       = j.value("fanout",       0);
}

void to_json(nlohmann::json& j, const DieCell* c) {
  j["cell_type"]    = c->cell_type;
  j["page"]         = c->page;
  j["tag"]          = c->tag;
  j["gate"]         = c->gate;
  j["long_name"]    = c->long_name;
  j["doc"]          = c->doc;
  j["input_ports"]  = c->input_ports;
  j["output_ports"] = c->output_ports;
  j["fanout"]       = c->fanout;
}

//--------------------------------------------------------------------------------

void check_cell_tag(const std::string& tag) {
  (void)tag;
  CHECK_P(tag.size() == 4);
  CHECK_P(tag[0] >= 'A' && tag[0] <= 'Z');
  CHECK_P(tag[1] >= 'A' && tag[1] <= 'Z');
  CHECK_P(tag[2] >= 'A' && tag[2] <= 'Z');
  CHECK_P(tag[3] >= 'A' && tag[3] <= 'Z');
}

//--------------------------------------------------------------------------------

void DieCell::sanity_check() const {
  if (gate.empty()) {
    ConsoleDumper d;
    dump(d);
  }

  CHECK_N(cell_type == DieCellType::UNKNOWN);
  CHECK_N(tag.empty());
  CHECK_N(gate.empty());

  if (long_name.empty()) {
    printf("No name for tag %s\n", tag.c_str());
  }

  if (cell_type == DieCellType::PIN_IN) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate == "pin_in");
  }
  else if (cell_type == DieCellType::PIN_OUT) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate.starts_with("pin_out"));
  }
  else if (cell_type == DieCellType::PIN_IO) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate.starts_with("pin_io"));
  }
  else if (cell_type == DieCellType::PIN_CLK) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate == "pin_clk");
  }
  else if (cell_type == DieCellType::SIG_IN) {
    CHECK_P(tag.starts_with("SIG_"));
    CHECK_P(gate == "sig_in");
  }
  else if (cell_type == DieCellType::SIG_OUT) {
    CHECK_P(tag.starts_with("SIG_"));
    CHECK_P(gate == "sig_out");
  }
  else if (cell_type == DieCellType::BUS) {
    CHECK_P(tag.starts_with("BUS_"));
    CHECK_P(gate == "bus");
  }
  else if (cell_type == DieCellType::DFF) {
    check_cell_tag(tag);
  }
  else if (cell_type == DieCellType::LATCH) {
    check_cell_tag(tag);
  }
  else if (cell_type == DieCellType::TRIBUF) {
    check_cell_tag(tag);
  }
  else if (cell_type == DieCellType::ADDER) {
    check_cell_tag(tag);
  }
  else if (cell_type == DieCellType::LOGIC) {
    check_cell_tag(tag);
  }
  else {
    debugbreak();
  }
}

//--------------------------------------------------------------------------------

void DieCell::dump(Dumper& d) const {
  using namespace nlohmann;

  json j;
  ::to_json(j, this);

  d(j.dump(2).c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void DieDB::clear() {
  for (auto& [tag, cell] : cell_map) delete cell;
  cell_map.clear();

  //trace_map_old.clear();
  //for (auto trace : traces) delete trace;
  traces.clear();
}

//--------------------------------------------------------------------------------

void to_json(nlohmann::json& j, const DieTrace& t) {
  j["output_cell"] = t.output_tag;
  j["output_port"] = t.output_port;
  j["input_cell"]  = t.input_tag;
  j["input_port"]  = t.input_port;
}

void from_json(const nlohmann::json& j, DieTrace& t) {
  j["output_cell"] .get_to(t.output_tag);
  j["output_port"] .get_to(t.output_port);
  j["input_cell"]  .get_to(t.input_tag);
  j["input_port"]  .get_to(t.input_port);
}

//----------------------------------------

void DieDB::save_json(const char* filename) {
  std::ofstream stream(filename);
  save_json(stream);
}

void DieDB::save_json(std::ostream& stream) {
  using namespace nlohmann;

  sanity_check();

  json root;
  root["cell_map"] = cell_map;
  //root["trace_map"] = trace_map_old;
  root["traces"] = traces;

  stream << root.dump(2);
}

//----------------------------------------

void DieDB::load_json(const char* filename) {
  std::ifstream stream(filename);
  load_json(stream);
}

void DieDB::load_json(std::istream& stream) {
  using namespace nlohmann;

  CHECK_P(cell_map.empty());
  //CHECK_P(trace_map_old.empty());
  CHECK_P(traces.empty());

  json root;
  stream >> root;

  root["cell_map"].get_to(cell_map);
  //root["trace_map"].get_to(trace_map_old);
  root["traces"].get_to(traces);

  sanity_check();
}

//----------------------------------------

void DieDB::sanity_check() {

  for (auto& [tag, cell] : cell_map) {
    cell->sanity_check();
    cell->mark = 0;
  }

  for (const auto& trace : traces) {
    auto prev_cell = cell_map[trace.output_tag];
    auto next_cell = cell_map[trace.input_tag];

    CHECK_P(prev_cell);
    CHECK_P(next_cell);

    if (next_cell->cell_type == DieCellType::BUS)    {
      CHECK_P(prev_cell->cell_type == DieCellType::TRIBUF);
    }
    if (prev_cell->cell_type == DieCellType::TRIBUF) {
      CHECK_P(next_cell->cell_type == DieCellType::BUS);
    }

    CHECK_P(std::find(prev_cell->output_ports.begin(),
                      prev_cell->output_ports.end(),
                      trace.output_port) != prev_cell->output_ports.end());

    CHECK_P(std::find(next_cell->input_ports.begin(), next_cell->input_ports.end(), trace.input_port) != next_cell->input_ports.end());

    prev_cell->mark++;
    next_cell->mark++;
  }

  for (auto& [tag, cell] : cell_map) {
    CHECK_P(cell->mark);
  }
}

//----------------------------------------

void DieDB::error(const char* format, ...) {
  char source_buf[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(source_buf, 1024, format, args);
  va_end(args);
  printf("Error @ %s:%d - ", current_filename.c_str(), current_linenum);
  printf(source_buf);
  printf("Line: '%s'\n", current_line.c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma warning(disable:4702)

bool DieDB::parse_dir(const std::string& path) {

#if 0
  static regex test_rx(R"(^\s*\/\*\s*(.*?)\s*\*\/\s*)");
  const string line = R"(  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp_old() const;)";

  const char* test_cursor = line.data();
  const char* test_end = test_cursor + line.size();

  while (1) {

    cmatch matches;
    printf("line  '%s'\n", test_cursor);
    if (regex_search(test_cursor, test_end, matches, test_rx)) {
      printf("match '%s'\n", matches[1].str().c_str());
      test_cursor += matches.position();
      test_cursor += matches.length();
    }
    else {
      printf("no match\n");
      break;
    }
  }

  return false;
#endif


  (void)path;

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  static set<string> hdr_names;
  static set<string> src_names;

  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      string filename = entry.path().string();

      if (filename.ends_with(".h")) {
        hdr_names.insert(filename);
      }
      else if (filename.ends_with(".cpp")) {
        src_names.insert(filename);
      }
      else {
        //printf("Skipping %s\n", filename.c_str());
      }
    }
  }

  for (auto hdr_name : hdr_names) {
    printf("Parsing header %s\n", hdr_name.c_str());
    current_filename = hdr_name;
    if (!parse_header_file(hdr_name)) {
      return false;
    }
  }

  /*
  for (auto src_name : src_names) {
    current_filename = src_name;
    printf("Parsing source %s\n", src_name.c_str());
    if (!parse_file(src_name)) {
      return false;
    }
  }
  */


#if 0
  current_filename = "<postprocess>";
  current_line = 0;

  //----------------------------------------
  // Postprocess the cells.

  for (auto& [tag, cell] : cell_map) {
    std::sort(cell->input_ports.begin(), cell->input_ports.end());
    std::sort(cell->output_ports.begin(), cell->output_ports.end());


    if (cell->long_name.empty()) {
      error("Cell %s needs a name\n", cell->tag.c_str());
      cell->long_name = cell->tag;
    }

    if (cell->cell_type == DieCellType::TRIBUF) {
      // Tribufs don't get names, add the tag as the default name.
      //CHECK_P(cell->name.empty());
      //cell->name = cell->tag;
      //CHECK_N(cell->name.empty());
    }

    if (cell->cell_type == DieCellType::PIN_IN) {
      CHECK_P(cell->gate.empty());
      cell->gate = "pin_in";
    }

    if (cell->cell_type == DieCellType::PIN_CLK) {
      CHECK_P(cell->gate.empty());
      cell->gate = "pin_clk";
    }

    if (cell->cell_type == DieCellType::SIG_IN) {
      CHECK_P(cell->gate.empty());
      cell->gate = "sig_in";
    }

    if (cell->tag.starts_with("BUS_")) {
      CHECK_P(cell->gate.empty());
      cell->gate = "bus";
    }

    if (cell->cell_type != DieCellType::BUS) {

      if (!gate_to_in_ports.contains(cell->gate)) {
        error("bad cell \"%s\"\n", cell->tag.c_str());
        error("bad gate \"%s\"\n", cell->gate.c_str());
      }
      CHECK_P(gate_to_in_ports.contains(cell->gate));

      CHECK_P(cell->input_ports.empty());
      cell->input_ports = gate_to_in_ports[cell->gate];
    }

    CHECK_P(cell_type_to_out_ports.contains(cell->cell_type));
    CHECK_P(cell->output_ports.empty());
    cell->output_ports = cell_type_to_out_ports[cell->cell_type];

    CHECK_P(cell->fanout == 0);
  }

  // Postprocess the traces

  std::sort(traces.begin(), traces.end());
  traces.erase(std::unique(traces.begin(), traces.end()), traces.end());

  //for (auto& [trace_key, trace] : trace_map_old) {
  for (auto& trace : traces) {
    cell_map[trace.output_tag]->fanout++;
  }

  sanity_check();

  //----------------------------------------
  // Done, dump stats.

  printf("Total lines %d\n", total_lines);
  printf("Total tagged lines %d\n", total_tagged_lines);
  printf("Cell map size %zd\n", cell_map.size());
  printf("Trace list size %zd - should be 5871\n", traces.size());

  printf("\n");
#endif

  printf("Parsed %d files\n", total_files);
  printf("Parsed %d lines\n", total_lines);
  printf("Found %d cells\n", (int)cell_map.size());

  return true;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_header_file(const std::string& path) {
  std::ifstream lines(path);
  current_linenum = 0;

  static regex rx_noparse("plait_noparse");
  static regex rx_extract_tag(R"(^\s*\/\*\s*(.*?)\s*\*\/\s*)");
  static regex rx_decl(R"((\w+)\s+(\w+)\s*;)");
  static regex rx_method(R"(^wire\s*(\w+)\s*\(\))");


  for (string line; getline(lines, line); ) {
    current_line = line;
    current_linenum++;
    total_lines++;

    if (regex_search(line, rx_noparse)) {
      //printf("Not parsing %s due to 'plait_noparse'\n", current_filename.c_str());
      break;
    }

    // Extract tag

    smatch matches;
    if (!regex_search(line, matches, rx_extract_tag)) {
      continue;
    }

    std::string flag;
    std::string page;
    std::string tag;
    std::string name;

    if (!parse_tag(matches[1].str(), flag, page, tag, name)) {
      error("Bad tag\n");
      return false;
    }

    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);

    cell->set_page(page);
    cell->set_name(name);

    line = matches.suffix();

    bool any_match = false;

    if (regex_search(line, matches, rx_decl)) {
      any_match = true;
      // Handle member variable declarations
      string decl_type;
      string decl_name;
      decl_type = matches[1].str();
      decl_name = matches[2].str();

      auto it = decl_to_cell_type.find(decl_type);
      if (it == decl_to_cell_type.end()) {
        error("Unknown member decl\n");
        return false;
      }

      cell->set_type((*it).second);
      cell->set_name(decl_name);

      line = matches.suffix();
      //printf("rest '%s'\n", line.c_str());
    }
    else if (regex_search(line, matches, rx_method)) {
      any_match = true;
      // Handle method decls
      //printf("%s\n", matches[1].str().c_str());
      cell->set_name(matches[1].str());
      line = matches.suffix();
    }

    // Handle line tail, which can contain docstring
    static regex rx_docstring(R"(\/\/\s*(.*?)\s*$)");
    if (regex_search(line, matches, rx_docstring)) {
      cell->set_doc(matches[1].str());
    }


    if (!any_match) {
      //printf("XXX %s\n", line.c_str());
    }
  }

  total_files++;
  return true;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_source_file(const std::string& path) {
  (void)path;

  total_files++;
  return true;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_line() {
  /*
  auto cell = get_or_create_cell(tag);
  CHECK_P(cell->page.empty() || cell->page == page);
  cell->page = page;

  if (!name.empty() && name != tag) {
    CHECK_P(cell->long_name.empty() || cell->long_name == name);
    cell->long_name = name;
  }
  */

  return true;

#if 0
  static regex tagged_line_regex(R"(^\s*(\/\*.*?\*\/)\s*(.*)$)");

  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {
    total_tagged_lines++;

    std::string page;
    std::string tag;
    std::string name;

    if (parse_tag(page, tag, name)) {
      auto cell = get_or_create_cell(tag);
      CHECK_P(cell->page.empty() || cell->page == page);
      cell->page = page;

      if (!name.empty() && name != tag) {
        CHECK_P(cell->long_name.empty() || cell->long_name == name);
        cell->long_name = name;
      }

      return parse_rest(*cell, matches[2].str());
    }
    else {
      return false;
    }
  }
  else {
    return true;
  }
#endif
}

//-----------------------------------------------------------------------------
// everything after the comment tag

bool DieDB::parse_rest(DieCell& c, const string& rest) {
  (void)c;

  smatch match;

  static regex has_assign(R"(^(.*?)\s*=\s*(.*);.*$)");

  if (regex_match(rest, match, has_assign)) {
    bool result = true;
    result &= parse_lhs(c, match[1].str());
    result &= parse_rhs(c, match[2].str());
    return result;
    //return parse_assign(c, match[1].str(), match[2].str());
  }

  static regex has_method_call(R"(^.*\(.*\);.*$)");
  if (regex_match(rest, match, has_method_call)) {
    //printf("  !assign: %s\n", rest.c_str());
    return parse_method_call(c, rest);
  }

  /*
  if (regex_match(rest, match, has_call)) {
    //printf("  src %s\n", match[1].str().c_str());
    //printf("  args %s\n", match[2].str().c_str());
    //printf("\n");
  }
  */
  return false;


#if 0
  static regex static_member_decl(R"(^static SigIn (\w+);.*)");
  static regex static_member_def(R"(^SigIn GateBoy::(\w+) .*)");

  static regex member_decl(R"(^(\w+)\s+(\w+)\s*;.*)");
  static regex local_decl(R"(^wire\s+(\w+)\s*=\s*(.*)$)");
  static regex auto_decl(R"(^auto\s+(\w+)\s*=\s*(.*)$)");
  static regex func_decl(R"(^wire\s+(\w+)\(.*\) const;.*$)");
  static regex inline_func_decl(R"(^(?:inline\s+)?wire\s+(\w+)\s*.*\{\s*return\s*(.*)\}.*)");
  static regex wire_decl(R"(^wire\s*(\w+);.*$)");
  static regex signal_decl(R"(^Signal\s*(\w+);.*$)");

  static regex member_assign(R"(^(?:\w+\.)*(\w+)\s*=\s*(.+;).*)");

  static regex tri_call(R"(^(.*)\.\s*(tri\w+\(.*$))");
  static regex dff_call(R"(^(.*)\.\s*(dff\w+\(.*$))");
  static regex latch_call(R"(^(.*)\.\s*(.*_latch[pn]?\(.*$))");
  static regex sig_out_call(R"(^(.*)\.\s*sig_out\((.*)\).*)");

  static regex pin_call(R"(^(.*)\.\s*(pin\w+\(.*$))");

  bool result = true;

  smatch match;
  if (regex_match(rest, match, static_member_decl)) {
    result &= parse_reg_type(c, "SigIn");
    result &= parse_cell_name(c, match[1].str());
  }
  else if (regex_match(rest, match, static_member_def)) {
    result &= parse_reg_type(c, "SigIn");
    result &= parse_cell_name(c, match[1].str());
  }
  else if (regex_match(rest, match, member_decl)) {
    result &= parse_reg_type(c, match[1].str());
    result &= parse_cell_name(c, match[2].str());
  }
  else if (regex_match(rest, match, local_decl)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, auto_decl)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, func_decl)) {
    result &= parse_cell_name(c, match[1].str());
  }
  else if (regex_match(rest, match, inline_func_decl)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, wire_decl)) {
    error("are we using this?");
    debugbreak();
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
    printf("rest %s\n", rest.c_str());
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
  else if (regex_match(rest, match, sig_out_call)) {
    c.cell_type = DieCellType::SIG_OUT;
    c.gate = "sig_out";
    result &= parse_sig_name(c, match[1].str());
    result &= parse_cell_arglist(c, match[2].str());
  }
  else if (regex_match(rest, match, member_assign)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else {
    error("Could not parse rest : \"%s\"\n", rest.c_str());
    result = false;
  }
  return result;
#endif
}

//-----------------------------------------------------------------------------

bool DieDB::parse_lhs(DieCell& c, const string& lhs) {
  (void)c;
  smatch match;

  static regex lhs_has_decl(R"(^(\w+)\s+(.*)$)");
  if (regex_match(lhs, match, lhs_has_decl)) {
    return parse_decl(c, match[1].str(), match[2].str());
  }
  else {
    //printf("  !decl: %s\n", lhs.c_str());
  }

  return false;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_rhs(DieCell& c, const string& rhs) {
  (void)c;
  (void)rhs;

  return false;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_decl(DieCell& c, const string& ctype, const string& cname) {
  (void)c;
  (void)ctype;
  (void)cname;

  const static set<string> valid_decls = { "wire", "adder" };

  if (!valid_decls.contains(ctype)) {
    return false;
  }

  //printf("  decl ('%s', '%s', '%s')\n", ctype.c_str(), cname.c_str(), rhs.c_str());
  return false;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_method_call(DieCell& c, const std::string& rest) {
  (void)c;
  (void)rest;
  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

bool DieDB::parse_tag(const std::string& whole_tag,
                      std::string& flag_out,
                      std::string& page_out,
                      std::string& tag_out,
                      std::string& name_out) {

  static regex pin_tag (R"((.?)()(PIN_\w+))");
  static regex sig_tag (R"((.?)()(SIG_\w+))");
  static regex bus_tag (R"((.?)()(BUS_\w+))");
  static regex cell_tag(R"((.?)p([0-9]{2})\.([A-Z]{4}\w*))");

  smatch match;

  bool matched = false;

  if (regex_search(whole_tag, match, pin_tag)) {
    flag_out = match[1].str();
    tag_out  = match[3].str();
    name_out = tag_out;
    matched = true;
  }
  else if (regex_search(whole_tag, match, sig_tag)) {
    flag_out = match[1].str();
    tag_out = match[3].str();
    name_out = tag_out;
    matched = true;
  }
  else if (regex_search(whole_tag, match, bus_tag)) {
    flag_out = match[1].str();
    tag_out = match[3].str();
    name_out = tag_out;
    matched = true;
  }
  else if (regex_search(whole_tag, match, cell_tag)) {
    flag_out = match[1].str();
    page_out = match[2].str();
    tag_out  = match[3].str().substr(0, 4);
    name_out = match[3].str();
    matched = true;
  }

  if (matched) {
    if (flag_out.empty()) flag_out = " ";
    if (page_out.empty()) page_out = "XX";
  }

  return matched;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

std::map<std::string, DieCellType> gate_to_cell_type = {
  {"pin_in_dp",      DieCellType::PIN_IN},

  {"set_pin_int",    DieCellType::PIN_OUT},
  {"pin_out_dp",     DieCellType::PIN_OUT},
  {"pin_out_hilo",   DieCellType::PIN_OUT},
  {"pin_out_oehilo", DieCellType::PIN_OUT},

  {"pin_io_out_pull_hilo",     DieCellType::PIN_IO},
  {"pin_io_out_pull_hilo_any", DieCellType::PIN_IO},
  {"pin_io_in_oedp",           DieCellType::PIN_IO},
  //{"pin_io_in_oedp_any",       DieCellType::PIN_IO},

  {"pin_clk",     DieCellType::PIN_CLK},

  {"sig_in",      DieCellType::SIG_IN},
  {"sig_out",     DieCellType::SIG_OUT},

  {"dff9",        DieCellType::DFF},
  {"dff22",       DieCellType::DFF},
  {"dff17",       DieCellType::DFF},
  {"dff20",       DieCellType::DFF},
  {"dff17_any",   DieCellType::DFF},
  {"dff13",       DieCellType::DFF},
  {"dff8n",       DieCellType::DFF},
  {"dff11",       DieCellType::DFF},
  {"dff8p",       DieCellType::DFF},

  {"nand_latch",  DieCellType::LATCH},
  {"nor_latch",   DieCellType::LATCH},
  {"tp_latchn",   DieCellType::LATCH},
  {"tp_latchp",   DieCellType::LATCH},

  {"tri10_np",    DieCellType::TRIBUF},
  {"tri6_pn",     DieCellType::TRIBUF},
  {"tri6_nn",     DieCellType::TRIBUF},

  {"not1",        DieCellType::LOGIC},
  {"and2",        DieCellType::LOGIC},
  {"and3",        DieCellType::LOGIC},
  {"and4",        DieCellType::LOGIC},
  {"or2",         DieCellType::LOGIC},
  {"or3",         DieCellType::LOGIC},
  {"or4",         DieCellType::LOGIC},
  {"xor2",        DieCellType::LOGIC},
  {"xnor2",       DieCellType::LOGIC},
  {"nand2",       DieCellType::LOGIC},
  {"nand3",       DieCellType::LOGIC},
  {"nand4",       DieCellType::LOGIC},
  {"nand5",       DieCellType::LOGIC},
  {"nand6",       DieCellType::LOGIC},
  {"nand7",       DieCellType::LOGIC},
  {"nor2",        DieCellType::LOGIC},
  {"nor3",        DieCellType::LOGIC},
  {"nor4",        DieCellType::LOGIC},
  {"nor5",        DieCellType::LOGIC},
  {"nor6",        DieCellType::LOGIC},
  {"nor8",        DieCellType::LOGIC},
  {"and_or3",     DieCellType::LOGIC},
  {"or_and3",     DieCellType::LOGIC},
  {"not_or_and3", DieCellType::LOGIC},
  {"mux2n",       DieCellType::LOGIC},
  {"mux2p",       DieCellType::LOGIC},
  {"amux2",       DieCellType::LOGIC},
  {"amux4",       DieCellType::LOGIC},

  {"add3",        DieCellType::ADDER},
};

bool DieDB::parse_cell_gate(DieCell& c, const std::string& gate) {
  CHECK_P(c.gate.empty() || c.gate == gate);
  c.gate = gate;

  CHECK_P(gate_to_cell_type.contains(gate));
  auto cell_type = gate_to_cell_type[gate];

  CHECK_P(c.cell_type == cell_type || c.cell_type == DieCellType::UNKNOWN);
  c.cell_type = cell_type;

  return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_reg_type(DieCell& c, const std::string& type) {
  CHECK_P(decl_to_cell_type.contains(type));
  auto cell_type = decl_to_cell_type[type];

  CHECK_P(c.cell_type == DieCellType::UNKNOWN || c.cell_type == cell_type);
  c.cell_type = cell_type;

  return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

std::string trim_name(const std::string& raw_port) {
  std::string result = raw_port;
  if (result.ends_with("_old") || result.ends_with("_new") || result.ends_with("_any") || result.ends_with("_mid")) {
    result.resize(result.size() - 4);
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_cell_arg(const std::string& arg, std::string& tag_out, std::string& port_out) {

  static regex simple_arg(
    "^"
    "(?:\\w+\\.)*"
    "_?"
    "([A-Z]{4})"
    "\\w*"
    "$"
  );

  static regex cell_arg_with_port(
    "^"
    "(?:\\w+\\.)*"
    "_?"
    "([A-Z]{4})"
    "\\w*"
    "\\."
    "(\\w*)"
    ".*"
    "$"
  );

  static regex cell_arg_function(
    "^"
    "(?:\\w+\\.)*"
    "([A-Z]{4})\\w+"
    "\\(\\w*\\)"
    "$"
  );

  static regex sig_arg(
    "^"
    "(?:\\w+\\.)*"
    "(SIG_\\w+)"
    ".*"
    "$"
  );

  static regex bus_arg(
    "^"
    "(?:\\w+\\.)*"
    "(BUS_\\w+)"
    "\\."
    "(\\w*)"
    ".*"
    "$"
  );

  static regex pin_arg_with_port(
    "^"
    "(?:\\w+\\.)*"
    "(PIN_\\d{2})\\w*"
    "\\."
    "(\\w*)"
    ".*"
    "$"
  );

  static regex pin_arg(
    "^"
    "(?:\\w+\\.)*"
    "(PIN_\\d{2})\\w*"
    ".*"
    "$"
  );

  smatch match;
  if (regex_match(arg, match, simple_arg)) {
    tag_out = match[1].str();
    port_out = "qp";
    return true;
  }
  else if (regex_match(arg, match, cell_arg_with_port)) {
    tag_out = match[1].str();
    port_out = trim_name(match[2].str());
    return true;
  }
  else if (regex_match(arg, match, cell_arg_function)) {
    tag_out = match[1].str();
    port_out = "qp";
    return true;
  }
  else if (regex_match(arg, match, sig_arg)) {
    tag_out = match[1].str();
    port_out = "qp";
    return true;
  }
  else if (regex_match(arg, match, bus_arg)) {
    tag_out = match[1].str();
    port_out = trim_name(match[2].str());
    return true;
  }
  else if (regex_match(arg, match, pin_arg_with_port)) {
    tag_out = match[1].str();
    port_out = trim_name(match[2].str());
    return true;
  }
  else if (regex_match(arg, match, pin_arg)) {
    tag_out = match[1].str();
    port_out = "qp";
    return true;
  }
  else {
    error("Could not parse arg \"%s\"\n", arg.c_str());
    debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <input>, <input>...

bool DieDB::parse_cell_arglist(DieCell& die_cell, const string& arglist_c) {
  static regex arg_regex(R"(\s*_?(.+?)(,|$))");

  CHECK_N(die_cell.tag.empty());
  CHECK_N(die_cell.gate.empty());

  const auto& input_ports = gate_to_in_ports[die_cell.gate];
  CHECK_N(input_ports.empty());

  string arglist = arglist_c;
  arglist.erase(remove_if(arglist.begin(), arglist.end(), ::isspace), arglist.end());

  bool result = true;
  int i = 0;
  for (sregex_iterator it = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); it != sregex_iterator(); it++) {
    std::string output_tag;
    std::string output_port;
    result &= parse_cell_arg((*it)[1].str(), output_tag, output_port);
    CHECK_P(result);

    DieTrace trace = {
      output_tag,
      output_port,
      die_cell.tag,
      input_ports[i]
    };

    //trace_map_old[trace->to_key_old()] = trace;
    traces.push_back(trace);

    i++;
  }

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <gate>( <input list> );

bool DieDB::parse_cell_def(DieCell& c, const string& value) {
  static regex valid_value(R"((.*?)\s*\((.*)\);\s*(.*))");

  bool result = true;
  smatch match;
  if (regex_match(value, match, valid_value)) {
    result &= parse_cell_gate(c, match[1].str());
    result &= parse_cell_arglist(c, match[2].str());
    string doc = match[3].str();
    if (!doc.empty()) {
      CHECK_P(c.doc.empty() || c.doc == doc);
      c.doc = doc;
    }
  }
  else {
    error("Could not parse value %s\n", value.c_str());
    result = false;
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// foo.bar.baz._ABCD_OTHER_STUFF
// foo.bar.baz.ABCD_OTHER_STUFF

bool DieDB::parse_cell_name(DieCell& c, const string& name) {
  static regex valid_cell_name(R"(^(?:\w+\.)*_?([A-Z]{4}\w*)\s*$)");
  static regex valid_pin_name (R"(^(PIN_\d{2}_\w+)$)");
  static regex valid_sig_name (R"(^(SIG_\w+)$)");
  static regex valid_bus_name (R"(^(BUS_\w+)$)");

  string base_name;
  smatch match;
  if (regex_match(name, match, valid_cell_name)) {
    base_name = match[1].str();
  }
  else if (regex_match(name, match, valid_pin_name)) {
    base_name = match[1].str();
  }
  else if (regex_match(name, match, valid_sig_name)) {
    base_name = match[1].str();
  }
  else if (regex_match(name, match, valid_bus_name)) {
    base_name = match[1].str();
  }
  else {
    error("Could not parse name %s\n", name.c_str());
    return false;
  }

  if (base_name.size()) {
    base_name = trim_name(base_name);
    CHECK_P(c.long_name.empty() || c.long_name == base_name);
    c.long_name = base_name;
    return true;
  }
  else {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_tribuf_bus_target(DieCell& c, const string& bus_name) {
  static regex valid_bus_name(R"(^(?:\w+\.)*(BUS_\w+)$)");
  CHECK_N(c.tag.empty());

  smatch match;
  if (regex_match(bus_name, match, valid_bus_name)) {
    std::string bus = match[1].str();
    DieCell* bus_cell = get_or_create_cell(bus);

    bus_cell->input_ports.push_back(c.tag);

    DieTrace bus_trace = {
      c.tag,
      "qp",
      bus_cell->tag,
      c.tag
    };

    traces.push_back(bus_trace);
    //trace_map_old[trace->to_key_old()] = trace;

    return true;
  } else {
    error("Could not parse bus name \"%s\"\n", bus_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_pin_name(DieCell& c, const string& pin_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(PIN_\d{2}_\w+)$)");

  smatch match;
  if (regex_match(pin_name, match, valid_pin_name)) {
    CHECK_P(c.long_name.empty());
    c.long_name = trim_name(match[1].str());
    return true;
  } else {
    error("Could not parse pin name %s\n", pin_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_sig_name(DieCell& c, const string& sig_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(SIG_\w+)\s*$)");

  smatch match;
  if (regex_match(sig_name, match, valid_pin_name)) {
    CHECK_P(c.long_name.empty());
    c.long_name = trim_name(match[1].str());
    return true;
  } else {
    error("Could not parse sig name %s\n", sig_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

