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

struct GateInfo gate_db[] = {
  {"sig_in",      DieCellType::SIG_IN,  {"in"},  {"out"} },
  {"sig_out",     DieCellType::SIG_OUT, {"in"},  {"out"} },
  {"tri_bus",     DieCellType::BUS,     {"tri"}, {"out"} },

  {"pin_in",      DieCellType::PIN_IN,  {"Dp"},                           {"qp_int"} },
  {"pin_out",     DieCellType::PIN_OUT, {"hi", "lo"},                     {"qp_ext", "qn_ext"} },
  {"pin_io",      DieCellType::PIN_IO,  {"PUn", "HI", "LO", "OEp", "Dp"}, {"qp_int"} },
  {"pin_clk",     DieCellType::PIN_CLK, {"clk", "clkgood"},               {"clk", "clkgood"} },

  {"dff8n",       DieCellType::DFF,     {"CLKn", "Dn"},                 {"qp", "qn"} },
  {"dff8p",       DieCellType::DFF,     {"CLKp", "Dn"},                 {"qp", "qn"} },
  {"dff9",        DieCellType::DFF,     {"CLKp", "SETn", "Dn"},         {"qp", "qn"} },
  {"dff11",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"dff13",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"dff17",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"dff20",       DieCellType::DFF,     {"CLKn", "LOADp", "Dp"},        {"qp", "qn"} },
  {"dff22",       DieCellType::DFF,     {"CLKp", "SETn", "RSTn", "Dp"}, {"qp", "qn"} },

  {"nand_latch",  DieCellType::LATCH,   {"SETn", "RSTn"},               {"qp", "qn"} },
  {"nor_latch",   DieCellType::LATCH,   {"SETp", "RSTp"},               {"qp", "qn"} },
  {"tp_latchn",   DieCellType::LATCH,   {"HOLDn", "Dp"},                {"qp", "qn"} },
  {"tp_latchp",   DieCellType::LATCH,   {"HOLDp", "Dp"},                {"qp", "qn"} },

  {"tri10_np",    DieCellType::TRIBUF,  {"OEn", "Dp"},                  {"out"} },
  {"tri6_pn",     DieCellType::TRIBUF,  {"OEp", "Dn"},                  {"out"} },
  {"tri6_nn",     DieCellType::TRIBUF,  {"OEn", "Dn"},                  {"out"} },

  {"add3",        DieCellType::ADDER,   {"a", "b", "carry"},            {"sum", "carry"} },

  {"not1",        DieCellType::LOGIC,   {"a"},                {"out"} },
  {"and2",        DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"and3",        DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"and4",        DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"or2",         DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"or3",         DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"or4",         DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"xor2",        DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"xnor2",       DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"and_or3",     DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"or_and3",     DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"not_or_and3", DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },

  {"nand2",       DieCellType::LOGIC,   {"a", "b"}, {"out"} },
  {"nand3",       DieCellType::LOGIC,   {"a", "b", "c"}, {"out"} },
  {"nand4",       DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"nand5",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e"}, {"out"} },
  {"nand6",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f"}, {"out"} },
  {"nand7",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g"}, {"out"} },
  {"nand8",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g", "h"}, {"out"} },

  {"nor2",        DieCellType::LOGIC,   {"a", "b"}, {"out"} },
  {"nor3",        DieCellType::LOGIC,   {"a", "b", "c"}, {"out"} },
  {"nor4",        DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"nor5",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e"}, {"out"} },
  {"nor6",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f"}, {"out"} },
  {"nor7",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g"}, {"out"} },
  {"nor8",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g", "h"}, {"out"} },

  {"mux2n",       DieCellType::LOGIC,   {"mux", "a1", "a0"}, {"out"} },
  {"mux2p",       DieCellType::LOGIC,   {"mux", "a1", "a0"}, {"out"} },
  {"amux2",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1"}, {"out"} },
  {"amux4",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1", "a2", "b2", "a3", "b3"}, {"out"} },
  {"amux6",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1", "a2", "b2", "a3", "b3", "a4", "b4", "a5", "b5"}, {"out"} },
};

const GateInfo& gate_info(const std::string& gate) {
  for (const auto& g : gate_db) {
    if (g.gate == gate) return g;
  }
  CHECK_P(false);
  static GateInfo dummy;
  return dummy;
}

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
  static regex rx_split_args(R"(\s*([^,]+?)(,|$))");
  return split(text, rx_split_args);
}

std::string trim_name(std::string raw_name) {
  CHECK_P(!raw_name.empty());
  static regex rx_trim(R"((_any|_new|_old|_mid|_odd|_evn|\(\)))");
  auto name = std::regex_replace(raw_name, rx_trim, "");
  return name;
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
    CHECK_P(gate == "pin_in");
  }
  else if (cell_type == DieCellType::PIN_OUT) {
    CHECK_P(tag.starts_with("PIN_"));
    CHECK_P(gate == "pin_out");
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
    CHECK_P(gate == "tri_bus");
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
  CHECK_P(traces.empty());

  json root;
  stream >> root;

  root["cell_map"].get_to(cell_map);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isword(char c) {
  if (c >= 'a' && c <= 'z') return true;
  if (c >= 'A' && c <= 'Z') return true;
  if (c >= '0' && c <= '9') return true;
  if (c == '_') return true;
  return false;
}

bool isspace(char c) {
  if (c == ' ') return true;
  if (c == '\n') return true;
  if (c == '\t') return true;
  return false;
}

bool ispunct(char c) {
  return (c != 0) && !isspace(c) && !isword(c);
}

std::string clean_line(const char* line) {
  std::string result;

  while (*line && isspace(*line)) line++;
  if (*line == 0) return result;

  char prev = *line++;
  while (*line) {
    char next = *line++;

    if (isspace(prev) && isspace(next)) { continue; }
    if (isspace(prev) && ispunct(next)) { prev = next; continue; }
    if (ispunct(prev) && isspace(next)) { continue; }

    if (prev) result.push_back(prev);
    prev = next;
  }

  if (!isspace(prev)) {
    result.push_back(prev);
  }

  return result;
}

string name_to_tag(const string& name) {
  static regex rx_match_tag(R"(^(BUS_.*|PIN_\d{2}|SIG_.*|EXT_.*|[A-Z]{4}))");
  smatch match;
  CHECK_P(regex_search(name, match, rx_match_tag));
  return match[1].str();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_dir(const std::string& path) {
  (void)path;

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  auto parse_begin = timestamp();

  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      auto filename = entry.path().string();
      if (filename.ends_with(".h")) parse_file(filename);
    }
  }

  for (const auto& entry : filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      auto filename = entry.path().string();
      if (filename.ends_with(".cpp")) parse_file(filename);
    }
  }

  auto parse_end = timestamp();

  printf("Parsing took %f msec\n", (parse_end - parse_begin) * 1000.0);

  //----------------------------------------
  // Postprocess the cells.

  for (auto& [tag, cell] : cell_map) {
    auto& info = gate_info(cell->gate);
    (void)info;
    vector<string> args = split_args(cell->args);

    cell->cell_type = info.cell_type;
    cell->output_ports = info.output_ports;

    if (cell->gate == "tri_bus") {
      for (int i = 0; i < args.size(); i++) {
        char buf[256];
        sprintf_s(buf, 256, "tri%02d", i);
        cell->input_ports.push_back(buf);
      }
    }
    else {
      CHECK_P(args.size() == info.input_ports.size());
      cell->input_ports = info.input_ports;
    }


    for (int iarg = 0; iarg < args.size(); iarg++) {
      const auto& arg = args[iarg];
      vector<string> parts = split_path(arg);
      static regex rx_match_tag(R"(^(BUS_|PIN_|SIG_|EXT_|[A-Z]{4}))");
      while (!regex_search(parts.front(), rx_match_tag)) parts.erase(parts.begin());
      CHECK_P(parts.size() == 1 || parts.size() == 2);

      string src_tag = name_to_tag(parts[0]);
      string src_port = parts.size() == 2 ? parts[1] : "out";
      if (src_tag.starts_with("EXT_")) continue;

      DieCell* src_cell = get_cell(src_tag);
      CHECK_P(src_cell);

      src_cell->fanout++;

      DieTrace trace = {
        src_cell->tag,
        src_port,
        cell->tag,
        cell->input_ports[iarg],
      };

      traces.push_back(trace);

      //printf("%s.%s -> %s.%s\n", src_cell->tag.c_str(), src_port.c_str(), cell->tag.c_str(), cell->input_ports[iarg].c_str());

    }

    /*
    printf("flag: %1s ",      cell->flag.c_str());
    printf("page: %3s ",      cell->page.c_str());
    printf("tag: %-16.16s ",  cell->tag.c_str());
    printf("name: %-24.24s ", cell->name.c_str());
    printf("gate: %-12s ",    cell->gate.c_str());
    printf("args: %s ",       cell->args.c_str());
    printf("doc: %-24.24s ",  cell->doc.c_str());
    printf("\n");
    */
  }

  sanity_check();


  printf("Parsed %d files\n", total_files);
  printf("Parsed %d lines\n", total_lines);
  printf("Found %d cells\n", (int)cell_map.size());
  printf("Found %d traces\n", (int)traces.size());

  return true;
}

//-----------------------------------------------------------------------------

bool DieDB::parse_file(const std::string& source_path) {
  printf("Parsing %s\n", source_path.c_str());

  std::ifstream lines(source_path);

  for (string line; getline(lines, line); ) {
    line = clean_line(line.c_str());
    total_lines++;

    static regex rx_noparse("plait_noparse");
    if (regex_search(line, rx_noparse)) {
      printf("Not parsing %s due to 'plait_noparse'\n", source_path.c_str());
      break;
    }

    smatch match;
    static regex rx_extract_tag(R"(^\/\*(.*?)\*\/)");
    if (!regex_search(line, match, rx_extract_tag)) continue;

    string whole_tag = match[1].str();
    line = match.suffix();

    static regex pin_tag(R"((#?)()(PIN_\d{2}))");
    static regex sig_tag(R"((#?)()(SIG_\w+))");
    static regex bus_tag(R"((#?)()(BUS_\w+))");
    static regex cell_tag(R"((#?)(p[0-9]{2})\.([A-Z]{4}))");

    bool found_match = false;
    if (!found_match) found_match = regex_search(whole_tag, match, pin_tag);
    if (!found_match) found_match = regex_search(whole_tag, match, sig_tag);
    if (!found_match) found_match = regex_search(whole_tag, match, bus_tag);
    if (!found_match) found_match = regex_search(whole_tag, match, cell_tag);
    CHECK_P(found_match);

    auto flag = match[1].str();
    auto page = match[2].str();
    auto tag = match[3].str();

    auto cell = get_or_create_cell(tag);
    CHECK_P(cell != nullptr);
    cell->set_flag(flag);
    cell->set_page(page);

    if (cell == nullptr) continue;

    static regex rx_decl(R"(\w+ (\w+);)");
    static regex rx_assign(R"((\w+)=(\w+)\((.*)\);)");
    static regex rx_method(R"((\w+)\(\)const\{return (\w+)\((.*)\);\})");
    static regex rx_method_call(R"((\w+)\.(\w+)\((.*)\);)");

    if (regex_search(line, match, rx_decl)) {
      cell->set_name(trim_name(match[1].str()));
      continue;
    }

    found_match = false;
    if (!found_match) found_match = regex_search(line, match, rx_assign);
    if (!found_match) found_match = regex_search(line, match, rx_method);
    if (!found_match) found_match = regex_search(line, match, rx_method_call);

    CHECK_P(found_match);

    auto name = trim_name(match[1].str());
    auto gate = trim_name(match[2].str());
    auto args = trim_name(match[3].str());
    auto docs = match.suffix().str();

    cell->set_name(name);
    cell->set_gate(gate);

    if (cell->gate == "tri_bus") {
      cell->args = cell->args + ", " + args;
    }
    else {
      cell->set_args(args);
    }

    cell->set_doc(docs);
  }

  total_files++;
  return true;
}
