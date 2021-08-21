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

NLOHMANN_JSON_SERIALIZE_ENUM(DieCellType, {
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


struct GateInfo {
  string gate;
  //string decl;
  DieCellType cell_type;
  //vector<string> input_ports;
  //vector<string> output_ports;
};

struct GateInfo gate_db[] = {
  {"pin_ext",     DieCellType::PIN_IN },

  {"pin_out",     DieCellType::PIN_OUT},
  {"pin_tri",     DieCellType::PIN_OUT},

  {"pin_clk",     DieCellType::PIN_CLK},

  {"sig_in",      DieCellType::SIG_IN },
  {"sig_out",     DieCellType::SIG_OUT},

  {"dff9",        DieCellType::DFF    },
  {"dff22",       DieCellType::DFF    },
  {"dff17",       DieCellType::DFF    },
  {"dff20",       DieCellType::DFF    },
  {"dff13",       DieCellType::DFF    },
  {"dff8n",       DieCellType::DFF    },
  {"dff11",       DieCellType::DFF    },
  {"dff8p",       DieCellType::DFF    },

  {"nand_latch",  DieCellType::LATCH  },
  {"nor_latch",   DieCellType::LATCH  },
  {"tp_latchn",   DieCellType::LATCH  },
  {"tp_latchp",   DieCellType::LATCH  },

  {"tri10_np",    DieCellType::TRIBUF },
  {"tri6_pn",     DieCellType::TRIBUF },
  {"tri6_nn",     DieCellType::TRIBUF },

  {"not1",        DieCellType::LOGIC  },
  {"and2",        DieCellType::LOGIC  },
  {"and3",        DieCellType::LOGIC  },
  {"and4",        DieCellType::LOGIC  },
  {"or2",         DieCellType::LOGIC  },
  {"or3",         DieCellType::LOGIC  },
  {"or4",         DieCellType::LOGIC  },
  {"xor2",        DieCellType::LOGIC  },
  {"xnor2",       DieCellType::LOGIC  },
  {"nand2",       DieCellType::LOGIC  },
  {"nand3",       DieCellType::LOGIC  },
  {"nand4",       DieCellType::LOGIC  },
  {"nand5",       DieCellType::LOGIC  },
  {"nand6",       DieCellType::LOGIC  },
  {"nand7",       DieCellType::LOGIC  },
  {"nor2",        DieCellType::LOGIC  },
  {"nor3",        DieCellType::LOGIC  },
  {"nor4",        DieCellType::LOGIC  },
  {"nor5",        DieCellType::LOGIC  },
  {"nor6",        DieCellType::LOGIC  },
  {"nor8",        DieCellType::LOGIC  },
  {"and_or3",     DieCellType::LOGIC  },
  {"or_and3",     DieCellType::LOGIC  },
  {"not_or_and3", DieCellType::LOGIC  },
  {"mux2n",       DieCellType::LOGIC  },
  {"mux2p",       DieCellType::LOGIC  },
  {"amux2",       DieCellType::LOGIC  },
  {"amux4",       DieCellType::LOGIC  },

  {"add3",        DieCellType::ADDER  },
};

DieCellType gate_to_cell_type(const std::string& gate) {
  for (const auto& g : gate_db) {
    if (g.gate == gate) return g.cell_type;
  }
  CHECK_P(false);
  return DieCellType::UNKNOWN;
}

std::map<DieCellType, std::vector<std::string>> cell_type_to_out_ports = {
  {DieCellType::UNKNOWN, {}},
  {DieCellType::PIN_IN,  {"qp_int", "qp_ext"}},
  {DieCellType::PIN_OUT, {"qp_ext", "qn_ext"}},
  {DieCellType::PIN_IO,  {"qp_int", "qp_ext"}},
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
  {"set_bus", {"ext"}},
  {"tri_bus", {"tri"}},

  {"set_pin_int", {"ext"}},
  {"set_pin_int_tristate", {"hi", "lo"}},
  {"set_pin_io", {"PUn", "HI", "LO", "OEp", "Dp"}},
  {"set_pin_ext", {"Dp"}},

  {"pin_in", {}},

  {"pin_out_dp",            {"DP"}},
  {"pin_out_hilo",          {"HI", "LO"}},

  {"pin_clk", {"clk", "clkgood"}},

  {"sig_in", {"ext"}},
  {"sig_out", {"Dp"}},

  {"dff8p",     {"CLKp", "Dn"}},
  {"dff8n",     {"CLKn", "Dn"}},
  {"dff9",      {"CLKp", "SETn", "Dp"}},
  {"dff11",     {"CLKp", "RSTn", "Dp"}},
  {"dff13",     {"CLKp", "RSTn", "Dp"}},
  {"dff17",     {"CLKp", "RSTn", "Dp"}},
  {"dff20",     {"CLKn", "LOADp", "Dp"}},
  {"dff22",     {"CLKp", "SETn", "RSTn", "Dp"}},

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

std::vector<std::string> split(const std::string& text, const regex& rx) {
  std::vector<std::string> result;
  for (sregex_iterator it = sregex_iterator(text.begin(), text.end(), rx); it != sregex_iterator(); it++) {
    result.push_back((*it)[1].str());
  }
  return result;
}

std::vector<std::string> split_path(const std::string& text) {
  static regex rx_split_path(R"(\s*([^\.]+?)(\.|$))");
  return split(text, rx_split_path);
}

std::vector<std::string> split_args(const std::string& text) {
  static regex rx_split_args(R"(\s*_?([^,]+?)(,|$))");
  return split(text, rx_split_args);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, DieCell*& c) {
  c = new DieCell();

  c->cell_type    = j.value("cell_type",    DieCellType::UNKNOWN);
  c->page         = j.value("page",         "<no_page>");
  c->tag          = j.value("tag",          "<no_tag>");
  c->gate         = j.value("gate",         "<no_gate>");
  c->name         = j.value("long_name",    "<no_name>");
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
  j["long_name"]    = c->name;
  j["doc"]          = c->doc;
  j["input_ports"]  = c->input_ports;
  j["output_ports"] = c->output_ports;
  j["fanout"]       = c->fanout;
}

//--------------------------------------------------------------------------------

std::string name_to_tag(std::string name) {
  auto it = name.rfind('.');
  if (it != std::string::npos) {
    name = name.substr(0, it);
  }

  if (name.starts_with("PIN_")) {
    return name.substr(0, 6);
  }

  if (name.starts_with("SIG_") || name.starts_with("BUS_") || name.starts_with("EXT_")) {
    return name;
  }

  return name.substr(0, 4);
}

std::string name_to_port(const std::string& name) {
  auto it = name.rfind('.');
  if (it != std::string::npos) {
    return name.substr(it + 1);
  }
  else {
    return std::string();
  }
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

  if (name.empty()) {
    printf("No name for tag %s\n", tag.c_str());
  }

  if (cell_type == DieCellType::PIN_IN) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate == "set_pin_ext");
  }
  else if (cell_type == DieCellType::PIN_OUT) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate == "set_pin_int" || gate == "set_pin_int_tristate");
  }
  else if (cell_type == DieCellType::PIN_IO) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate.starts_with("set_pin_io"));
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
    CHECK_P(gate == "set_bus" || gate == "tri_bus");
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
  //printf("DieDB::sanity_check()\n");

  for (auto& [tag, cell] : cell_map) {
    cell->sanity_check();
    cell->mark = 0;
  }

  for (const auto& trace : traces) {
    if (trace.output_tag.starts_with("EXT_")) continue;

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

  /*
  for (auto& [tag, cell] : cell_map) {
    CHECK_P(cell->mark);
  }
  */
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

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  auto parse_begin = timestamp();

  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      current_filename = entry.path().string();
      if (current_filename.ends_with(".h")) parse_file(current_filename);
    }
  }

  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      current_filename = entry.path().string();
      if (current_filename.ends_with(".cpp")) parse_file(current_filename);
    }
  }

  auto parse_end = timestamp();

  printf("Parsing took %f msec\n", (parse_end - parse_begin) * 1000.0);

  //----------------------------------------
  // Postprocess the cells.

  current_filename = "<postprocess>";
  current_line = "";

  for (auto& [tag, cell] : cell_map) {
    if (cell->gate == "set_pin_int") {
      cell->cell_type = DieCellType::PIN_OUT;
    }
    else if (cell->gate == "pin_clk") {
      cell->cell_type = DieCellType::PIN_CLK;
    }
    else if (cell->gate == "set_pin_io") {
      cell->cell_type = DieCellType::PIN_IO;
    }
    else if (cell->gate == "set_pin_ext") {
      cell->cell_type = DieCellType::PIN_IN;
    }
    else if (cell->decl == "Bus") {
      cell->cell_type = DieCellType::BUS;
    }
    else {
      cell->cell_type = gate_to_cell_type(cell->gate);
    }

    if (cell_type_to_out_ports.contains(cell->cell_type)) {
      cell->output_ports = cell_type_to_out_ports[cell->cell_type];
    }
    else {
      CHECK_P(false);
    }

    if (gate_to_in_ports.contains(cell->gate)) {
      cell->input_ports = gate_to_in_ports[cell->gate];
    }
    else {
      CHECK_P(false);
    }

    if (cell->cell_type == DieCellType::UNKNOWN) {
      printf("flag: %1s ",      cell->flag.c_str());
      printf("page: %2s ",      cell->page.c_str());
      printf("path: %-24.24s ", cell->path.c_str());
      printf("type: %-16s ",    cell_type_to_name[cell->cell_type].c_str());
      printf("tag: %-16.16s ", cell->tag.c_str());
      printf("decl: %-16.16s ", cell->decl.c_str());
      printf("gate: %-12s ",    cell->gate.c_str());
      printf("name: %-24.24s ", cell->name.c_str());
      printf("doc: %-24.24s ", cell->doc.c_str());
      printf("args: %s ", cell->args.c_str());
      printf("\n");
    }

    parse_cell_arglist(*cell);

  }

  sanity_check();


  printf("Parsed %d files\n", total_files);
  printf("Parsed %d lines\n", total_lines);
  printf("Found %d cells\n", (int)cell_map.size());
  printf("Found %d args\n", total_args);

  return true;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_file(const std::string& source_path) {
  std::ifstream lines(source_path);
  current_linenum = 0;

  for (string line; getline(lines, line); ) {
    current_line = line;
    current_linenum++;
    total_lines++;

    static regex rx_noparse("plait_noparse");
    if (regex_search(line, rx_noparse)) {
      printf("Not parsing %s due to 'plait_noparse'\n", current_filename.c_str());
      break;
    }

    DieCell* cell = parse_tag(line);
    if (cell == nullptr) continue;

    if (cell->tag == "BUS_VRAM_A00n") {
      int x = 1;
      x++;
    }

    int match_count = 0;

    match_count += parse_member_decl(line, cell);
    match_count += parse_method_decl(line, cell);
    match_count += parse_gate(line, cell);
    match_count += parse_adder(line, cell);
    match_count += parse_assignment(line, cell);
    match_count += parse_method_call(line, cell);
    match_count += parse_sig_vcc_gnd(line, cell);

    if (!match_count) {
      printf("XXX %s\n", current_line.c_str());
    }
    CHECK_P(match_count == 1);

    parse_tail(line, cell);

    /*
    cell->cell_type = gate_to_cell_type[cell->gate];
    if (cell->cell_type == DieCellType::UNKNOWN) {
      printf("XXXXX %s %s\n", cell->gate.c_str(), line.c_str());
    }
    */

  }

  total_files++;
  return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

DieCell* DieDB::parse_tag(std::string& line) {
  smatch match;
  static regex rx_extract_tag(R"(^\s*\/\*\s*(.*?)\s*\*\/\s*)");
  if (!regex_search(line, match, rx_extract_tag)) {
    return nullptr;
  }
  string whole_tag = match[1].str();
  line = match.suffix();

  static regex pin_tag(R"((.?)()(PIN_\w+))");
  static regex sig_tag(R"((.?)()(SIG_\w+))");
  static regex bus_tag(R"((.?)()(BUS_\w+))");
  static regex cell_tag(R"((.?)p([0-9]{2})\.([A-Z]{4}\w*))");

  std::string flag = " ";
  std::string page = "XX";
  std::string tag = "<??>";
  std::string name = "";

  if (regex_search(whole_tag, match, pin_tag)) {
    flag = match[1].str();
    tag = match[3].str();
    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);

    cell->set_flag(flag);
    cell->set_page(page);
    cell->set_tag(tag);
    return cell;
  }
  else if (regex_search(whole_tag, match, sig_tag)) {
    flag = match[1].str();
    tag = match[3].str();
    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);

    cell->set_flag(flag);
    cell->set_page(page);
    cell->set_tag(tag);
    return cell;
  }
  else if (regex_search(whole_tag, match, bus_tag)) {
    flag = match[1].str();
    tag = match[3].str();
    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);

    cell->set_flag(flag);
    cell->set_page(page);
    cell->set_tag(tag);
    return cell;
  }
  else if (regex_search(whole_tag, match, cell_tag)) {
    flag = match[1].str();
    page = match[2].str();
    tag = match[3].str().substr(0, 4);
    if (name.size() > 4) {
      name = match[3].str();
    }
    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);

    cell->set_flag(flag);
    cell->set_page(page);
    cell->set_tag(tag);
    return cell;
  }

  return nullptr;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

std::string trim_name(std::string raw_name) {
  CHECK_P(!raw_name.empty());
  static regex rx_trim(R"((^_| _| |_any|_new|_old|_mid|_odd|_evn|\(\)))");
  auto name = std::regex_replace(raw_name, rx_trim, "");
  return name;
}

//-----------------------------------------------------------------------------
// wire _name_ = _gate_(_args_);

bool DieDB::parse_gate(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_gate(R"(^wire\s+(\w+)\s*=\s*(\w+)\s*\((.*)\);)");
  if (regex_search(line, match, rx_gate)) {
    cell->set_decl("wire");
    cell->set_name(match[1].str());
    cell->set_gate(match[2].str());
    cell->set_args(match[3].str());
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// _type_ _name_;

bool DieDB::parse_member_decl(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_decl(R"((\w+)\s+(\w+)\s*;)");
  if (regex_search(line, match, rx_decl)) {
    cell->set_decl(match[1].str());
    cell->set_name(match[2].str());
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// wire _name_() { return _gate_(_args_); }

bool DieDB::parse_method_decl(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_method(R"(^wire\s*(\w+)\s*.*return\s+(\w+)\((.*)\);)");
  if (regex_search(line, match, rx_method)) {
    cell->set_decl("wire");
    cell->set_name(match[1].str());
    cell->set_gate(match[2].str());
    cell->set_args(match[3].str());
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// Adder _name_ = _gate_(_argss);

bool DieDB::parse_adder(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_adder(R"(^Adder\s+(\w+)\s*=\s*(\w+)\s*\((.*)\);)");
  if (regex_search(line, match, rx_adder)) {
    cell->set_decl("Adder");
    cell->set_name(match[1].str());
    cell->set_gate(match[2].str());
    cell->set_args(match[3].str());
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// _name_ = _gate_(_args_);

bool DieDB::parse_assignment(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_gate2(R"(^([\w\.]+)\s*=\s*(\w+)\((.*)\);)");
  if (regex_search(line, match, rx_gate2)) {
    cell->set_full_path(match[1].str());
    cell->set_gate(match[2].str());
    cell->set_args(match[3].str());
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// _path_name_._method_(_args_)

bool DieDB::parse_method_call(const string& line, DieCell* cell) {
  smatch match;
  static regex rx_method_call(R"(^([\w\.]+)\s*\.\s*(\w+)\s*\((.*)\);)");
  if (regex_search(line, match, rx_method_call)) {

    auto path = match[1].str();
    auto it = path.rfind('.');
    if (it != std::string::npos) {
      cell->set_path(path.substr(0, it));
      cell->set_name(path.substr(it + 1));
    }
    else {
      cell->set_name(path);
    }
    cell->set_gate(match[2].str());

    if (cell->gate == "tri_bus") {
      //printf("*** %s\n", line.c_str());
      if (cell->args.empty()) {
        cell->args = match[3].str();
      }
      else {
        cell->args = cell->args + ", " + match[3].str();
      }
    }
    else {
      cell->set_args(match[3].str());
    }

    //cell->arglists.insert(trim_arglist(match[3].str()));
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// _name_ = _constant_;

bool DieDB::parse_sig_vcc_gnd(const string& line, DieCell* cell) {
  smatch match;
  static regex rx_vccgnd(R"(^(SIG_VCC|SIG_GND)\s*=\s*(\w+);)");
  if (regex_search(line, match, rx_vccgnd)) {
    cell->set_name(match[1].str());
    cell->set_args(match[2].str());
    cell->set_gate("sig_in");
    return true;
  }
  else {
    return false;
  }
}

//-----------------------------------------------------------------------------
// stuff stuff stuff; // _docstring_

bool DieDB::parse_tail(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_docstring(R"(\/\/\s*(.*?)\s*$)");
  if (regex_search(line, match, rx_docstring)) {
    cell->set_doc(match[1].str());
  }
  return true;
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
    port_out = match[2].str();
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
    port_out = match[2].str();
    return true;
  }
  else if (regex_match(arg, match, pin_arg_with_port)) {
    tag_out = match[1].str();
    port_out = match[2].str();
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

bool DieDB::parse_cell_arglist(DieCell& die_cell) {
  auto& input_ports = gate_to_in_ports[die_cell.gate];
  vector<string> args = split_args(die_cell.args);

  if (die_cell.cell_type != DieCellType::BUS) {
    CHECK_P(args.size() == input_ports.size());
  }

  for (int iarg = 0; iarg < args.size(); iarg++) {
    auto& a = args[iarg];
    regex rx_match_tag(R"((BUS_.*|PIN_.*|SIG_.*|EXT_.*|[A-Z]{4}.*))");

    smatch match;
    if (regex_search(a, match, rx_match_tag)) {
      total_args++;
      auto arg = match[1].str();
      auto arg_tag = name_to_tag(arg);
      auto arg_port = name_to_port(arg);

      if (arg_port == "") {
        arg_port = "qp";
      }

      if (!arg_tag.starts_with("EXT_")) {
        auto source_cell = cell_map[arg_tag];
        CHECK_P(source_cell);
        source_cell->fanout++;
      }

      std::string input_port;
      if (die_cell.cell_type == DieCellType::BUS) {
        input_port = "tri";
      }
      else {
        input_port = input_ports[iarg];
      }

      DieTrace trace = {
        arg_tag,
        arg_port,
        die_cell.tag,
        input_port
      };

      traces.push_back(trace);
    }
    else {
      CHECK_P(false);
    }
  }

#if 0
    DieTrace trace = {
      output_tag,
      output_port,
      die_cell.tag,
      input_ports[i]
    };

    traces.push_back(trace);
#endif

  return true;
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

