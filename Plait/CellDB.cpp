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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, DieCell*& c) {
  c = new DieCell();

  c->cell_type    = j.value("cell_type",    DieCellType::UNKNOWN);
  c->page         = j.value("page",         "<no_page>");
  c->tag          = j.value("tag",          "<no_tag>");
  c->gate         = j.value("gate",         "<no_gate>");
  c->name    = j.value("long_name",    "<no_name>");
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

  auto parse_begin = timestamp();

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
    if (!parse_file(hdr_name)) {
      return false;
    }
  }

  for (auto src_name : src_names) {
    current_filename = src_name;
    printf("Parsing source %s\n", src_name.c_str());
    if (!parse_file(src_name)) {
      return false;
    }
  }

  auto parse_end = timestamp();

  printf("Parsing took %f msec\n", (parse_end - parse_begin) * 1000.0);


  current_filename = "<postprocess>";
  current_line = "";

  //----------------------------------------
  // Postprocess the cells.

  for (auto& [tag, cell] : cell_map) {

    //if (cell->gate.empty()) {
    {
      printf("%1s ", cell->flag.c_str());
      printf("%2s ", cell->page.c_str());
      printf("%-16.16s ", cell->tag.c_str());
      printf("%-16.16s ", cell->name.c_str());
      //printf("%-12s ", cell_type_to_name[cell->cell_type].c_str());
      printf("%-12s ", cell->gate.c_str());
      //printf("%s ", cell->args.c_str());
      printf("\n");
    }


#if 0
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
#endif
  }

#if 0
  // Postprocess the traces

  std::sort(traces.begin(), traces.end());
  traces.erase(std::unique(traces.begin(), traces.end()), traces.end());

  //for (auto& [trace_key, trace] : trace_map_old) {
  for (auto& trace : traces) {
    cell_map[trace.output_tag]->fanout++;
  }

  sanity_check();
#endif


  printf("Parsed %d files\n", total_files);
  printf("Parsed %d lines\n", total_lines);
  printf("Found %d cells\n", (int)cell_map.size());
  //printf("Cell map size %zd\n", cell_map.size());
  //printf("Trace list size %zd - should be 5871\n", traces.size());

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

std::string trim_name(const std::string& raw_name) {
  std::string result = raw_name;
  CHECK_P(!result.empty());
  if (result[0] == '_') {
    result.erase(result.begin());
  }
  if (result.ends_with("_old") || result.ends_with("_new") || result.ends_with("_any") || result.ends_with("_mid")) {
    result.resize(result.size() - 4);
  }
  return result;
}

//-----------------------------------------------------------------------------
// wire _name_ = _gate_(_args_);

bool DieDB::parse_gate(const std::string& line, DieCell* cell) {
  smatch match;
  static regex rx_gate(R"(^wire\s+(\w+)\s*=\s*(\w+)\s*\((.*)\);)");
  if (regex_search(line, match, rx_gate)) {
    cell->set_decl("wire");
    cell->set_name(trim_name(match[1].str()));
    cell->set_gate(match[2].str());
    cell->arglists.insert(match[3].str());
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
    cell->set_name(trim_name(match[2].str()));
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
    cell->set_name(trim_name(match[1].str()));
    cell->set_gate(match[2].str());
    cell->arglists.insert(match[3].str());
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
    cell->set_name(trim_name(match[1].str()));
    cell->set_gate(match[2].str());
    cell->arglists.insert(match[3].str());
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
    cell->arglists.insert(match[3].str());
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
      cell->set_name(trim_name(path.substr(it + 1)));
    }
    else {
      cell->set_name(path);
    }
    cell->set_gate(match[2].str());
    cell->arglists.insert(match[3].str());
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
  static regex rx_vccgnd(R"(^(\w+)\s*=\s*([0-9]+);)");
  if (regex_search(line, match, rx_vccgnd)) {
    cell->set_name(trim_name(match[1].str()));
    cell->arglists.insert(match[2].str());
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
  (void)die_cell;

  //const auto& input_ports = gate_to_in_ports[die_cell.gate];
  //CHECK_N(input_ports.empty());

  string arglist = arglist_c;
  auto remover = [](unsigned char x) {
    if (x == ' ') return true;
    if (x == '\t') return true;
    if (x == '\n') return true;
    if (x == '(') return true;
    if (x == ')') return true;
    return false;
  };
  arglist.erase(remove_if(arglist.begin(), arglist.end(), ::isspace), arglist.end());

  bool result = true;
  int i = 0;
  static regex arg_regex(R"(\s*_?([\w\.]+)\s*(,|$))");

  std::vector<CellPort> args;

  for (sregex_iterator it = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); it != sregex_iterator(); it++) {
    std::string output_tag;
    std::string output_port;
    result &= parse_cell_arg((*it)[1].str(), output_tag, output_port);
    CHECK_P(result);

    /*
    DieTrace trace = {
      output_tag,
      output_port,
      die_cell.tag,
      input_ports[i]
    };

    //trace_map_old[trace->to_key_old()] = trace;
    traces.push_back(trace);
    */

    i++;
  }

  /*
  if (!die_cell.args.size()) {
    CHECK_P(args.size() == die_cell.args.size());
    for (size_t j = 0; j < args.size(); j++) {
      CHECK_P(args[j] == die_cell.args[j]);
    }
  }
  else {
    die_cell.args = args;
  }
  */

  return result;
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

