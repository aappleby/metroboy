#include "Plait/CellDB.h"
#include "metrolib/core/Dumper.h"
#include "metrolib/core/Log.h"
#include "metrolib/core/Utils.h"
#include "Plait/PTree.h"

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
  {"PinIn",     "pin_in",      DieCellType::PIN_IN,  {"Dp"},                           {"qp_int"} },
  {"PinOut",    "pin_out",     DieCellType::PIN_OUT, {"HI", "LO"},                     {"qp_ext", "qn_ext"} },
  {"PinIO",     "pin_io",      DieCellType::PIN_IO,  {"PUn", "HI", "LO", "OEp", "Dp"}, {"qp_int"} },
  {"PinClock",  "pin_clk",     DieCellType::PIN_CLK, {"CLK", "CLKGOOD"},               {"clk", "clkgood"} },

  {"SigIn",     "sig_in",      DieCellType::SIG_IN,  {"in"},  {"out"} },
  {"SigOut",    "sig_out",     DieCellType::SIG_OUT, {"in"},  {"out"} },

  {"Bus",       "tri_bus",     DieCellType::BUS,     {"tri"}, {"out"} },

  {"DFF8n",     "dff8n",       DieCellType::DFF,     {"CLKn", "Dn"},                 {"qp", "qn"} },
  {"DFF8p",     "dff8p",       DieCellType::DFF,     {"CLKp", "Dn"},                 {"qp", "qn"} },
  {"DFF9",      "dff9",        DieCellType::DFF,     {"CLKp", "SETn", "Dn"},         {"qp", "qn"} },
  {"DFF11",     "dff11",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"DFF13",     "dff13",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"DFF17",     "dff17",       DieCellType::DFF,     {"CLKp", "RSTn", "Dp"},         {"qp", "qn"} },
  {"DFF20",     "dff20",       DieCellType::DFF,     {"CLKn", "LOADp", "Dp"},        {"qp", "qn"} },
  {"DFF22",     "dff22",       DieCellType::DFF,     {"CLKp", "SETn", "RSTn", "Dp"}, {"qp", "qn"} },

  {"NandLatch", "nand_latch",  DieCellType::LATCH,   {"SETn", "RSTn"},               {"qp", "qn"} },
  {"NorLatch",  "nor_latch",   DieCellType::LATCH,   {"SETp", "RSTp"},               {"qp", "qn"} },
  {"TpLatch",   "tp_latchn",   DieCellType::LATCH,   {"HOLDn", "Dp"},                {"qp", "qn"} },
  {"TpLatch",   "tp_latchp",   DieCellType::LATCH,   {"HOLDp", "Dp"},                {"qp", "qn"} },

  {"Adder",     "add3",        DieCellType::ADDER,   {"a", "b", "carry"},            {"sum", "carry"} },

  {"Gate",      "tri10_np",    DieCellType::LOGIC,   {"OEn", "Dp"},                  {"out"} },
  {"Gate",      "tri6_pn",     DieCellType::LOGIC,   {"OEp", "Dn"},                  {"out"} },
  {"Gate",      "tri6_nn",     DieCellType::LOGIC,   {"OEn", "Dn"},                  {"out"} },

  {"Gate",      "not1",        DieCellType::LOGIC,   {"a"},                {"out"} },
  {"Gate",      "and2",        DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"Gate",      "and3",        DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"Gate",      "and4",        DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"Gate",      "or2",         DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"Gate",      "or3",         DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"Gate",      "or4",         DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"Gate",      "xor2",        DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"Gate",      "xnor2",       DieCellType::LOGIC,   {"a", "b"},           {"out"} },
  {"Gate",      "and_or3",     DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"Gate",      "or_and3",     DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },
  {"Gate",      "not_or_and3", DieCellType::LOGIC,   {"a", "b", "c"},      {"out"} },

  {"Gate",      "nand2",       DieCellType::LOGIC,   {"a", "b"}, {"out"} },
  {"Gate",      "nand3",       DieCellType::LOGIC,   {"a", "b", "c"}, {"out"} },
  {"Gate",      "nand4",       DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"Gate",      "nand5",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e"}, {"out"} },
  {"Gate",      "nand6",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f"}, {"out"} },
  {"Gate",      "nand7",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g"}, {"out"} },
  {"Gate",      "nand8",       DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g", "h"}, {"out"} },

  {"Gate",      "nor2",        DieCellType::LOGIC,   {"a", "b"}, {"out"} },
  {"Gate",      "nor3",        DieCellType::LOGIC,   {"a", "b", "c"}, {"out"} },
  {"Gate",      "nor4",        DieCellType::LOGIC,   {"a", "b", "c", "d"}, {"out"} },
  {"Gate",      "nor5",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e"}, {"out"} },
  {"Gate",      "nor6",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f"}, {"out"} },
  {"Gate",      "nor7",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g"}, {"out"} },
  {"Gate",      "nor8",        DieCellType::LOGIC,   {"a", "b", "c", "d", "e", "f", "g", "h"}, {"out"} },

  {"Gate",      "mux2n",       DieCellType::LOGIC,   {"mux", "a1", "a0"}, {"out"} },
  {"Gate",      "mux2p",       DieCellType::LOGIC,   {"mux", "a1", "a0"}, {"out"} },
  {"Gate",      "amux2",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1"}, {"out"} },
  {"Gate",      "amux4",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1", "a2", "b2", "a3", "b3"}, {"out"} },
  {"Gate",      "amux6",       DieCellType::LOGIC,   {"a0", "b0", "a1", "b1", "a2", "b2", "a3", "b3", "a4", "b4", "a5", "b5"}, {"out"} },
};

const GateInfo& decl_to_info(const std::string& decl) {
  for (const auto& g : gate_db) {
    if (g.decl == decl) return g;
  }
  CHECK_P(false);
  static GateInfo dummy;
  return dummy;
}

const GateInfo& gate_to_info(const std::string& gate) {
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

//-----------------------------------------------------------------------------


std::string trim_name(std::string raw_name) {
  CHECK_P(!raw_name.empty());
  //if (raw_name.ends_with("()")) {
  //  raw_name.resize(raw_name.size() - 2);
  //}
  static regex rx_trim(R"((_sync|_async|_any|_new|_old|_mid|_odd|_evn|\(\)))");
  auto name = std::regex_replace(raw_name, rx_trim, "");
  return name;
}

//---------------------------------------------------------------------------------------------------------------------

void from_json(const nlohmann::json& j, DieCell*& c) {
  c = new DieCell();
  c->cell_type    = j.value("cell_type",    DieCellType::UNKNOWN);
  c->page         = j.value("page",         "<no_page>");
  c->tag          = j.value("tag",          "<no_tag>");
  c->gate         = j.value("gate",         "<no_gate>");
  c->name         = j.value("long_name",    "<no_name>");
  //c->doc          = j.value("doc",          "<no_doc>");
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
  //j["doc"]          = c->doc;
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
  DCHECK_N(cell_type == DieCellType::UNKNOWN);
  DCHECK_N(tag.empty());
  DCHECK_N(gate.empty());
  DCHECK_N(name.empty());

  //CHECK_N(args.empty()); // we don't necessarily want to save the raw arg string to the db...

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

//---------------------------------------------------------------------------------------------------------------------

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
  for (auto& [tag, cell] : cell_map) {
    cell->sanity_check();
    cell->mark = 0;
  }

  for (const auto& trace : traces) {
    // Traces connected to EXT_* do not appear in the schematic.
    if (trace.output_tag.starts_with("EXT_")) continue;

    // Cells at both ends of the trace should exist.
    auto prev_cell = cell_map[trace.output_tag];
    auto next_cell = cell_map[trace.input_tag];
    CHECK_P(prev_cell);
    CHECK_P(next_cell);

    // Tribufs should only connect to buses and vice versa.
    if (next_cell->cell_type == DieCellType::BUS)    {
      CHECK_P(prev_cell->cell_type == DieCellType::TRIBUF);
    }
    if (prev_cell->cell_type == DieCellType::TRIBUF) {
      CHECK_P(next_cell->cell_type == DieCellType::BUS);
    }

    // Ports at both ends of the trace should exist.
    CHECK_P(std::find(prev_cell->output_ports.begin(), prev_cell->output_ports.end(), trace.output_port) != prev_cell->output_ports.end());
    CHECK_P(std::find(next_cell->input_ports.begin(), next_cell->input_ports.end(), trace.input_port) != next_cell->input_ports.end());

    // Mark the cells at both ends of the trace as connected.
    prev_cell->mark++;
    next_cell->mark++;
  }

  // There should be no unconnected cells.
  for (auto& [tag, cell] : cell_map) {
    CHECK_P(cell->mark);
  }
}

//---------------------------------------------------------------------------------------------------------------------

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

// Remove all double-spaces and all spaces that are adjacent to non-word characters.
std::string clean_line(const char* line) {
  // Skip leading whitespace.
  while (*line && isspace(*line)) line++;
  if (*line == 0) return "";

  // Scan pairs of characters and store required ones to result.
  std::string result;
  char prev = *line++;
  while (*line) {
    char next = *line++;

    if (isspace(prev) && isspace(next)) {
      // Double-spaces get skipped.
    }
    else if (isspace(prev) && ispunct(next)) {
      // Spaces before punctuation get skipped.
      prev = next;
    }
    else if (ispunct(prev) && isspace(next)) {
      // Spaces after punctuation get skipped, but we hold on to the punctuation.
    }
    else {
      // In all other cases the previous character moves to the result.
      result.push_back(prev);
      prev = next;
    }
  }

  // Handle the tail character.
  if (!isspace(prev)) {
    result.push_back(prev);
  }

  return result;
}

string name_to_tag(const string& name) {
  static regex rx_match_tag(R"(^(BUS_.*|PIN_\d{2}|SIG_.*|EXT_.*|[A-Z]{4}))");
  smatch match;
  if (regex_search(name, match, rx_match_tag)) {
    return match[1].str();
  }
  else {
    CHECK_P(false);
    return "";
  }
}

//---------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_dir(const std::string& path) {
  (void)path;

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  auto parse_begin = timestamp();

  {
    LOG_Y("parsing headers for %s\n", path.c_str());
    LOG_INDENT_SCOPE();

    for (const auto& entry : filesystem::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        auto filename = entry.path().string();
        if (filename.starts_with("src\\GateBoyLib\\GateBoy") && filename.ends_with(".h")) {
          parse_header(filename);
        }
      }
    }
  }

  {
    LOG_Y("parsing source for %s\n", path.c_str());
    LOG_INDENT_SCOPE();

    for (const auto& entry : filesystem::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        auto filename = entry.path().string();
        if (filename.starts_with("src\\GateBoyLib\\GateBoy") && filename.ends_with(".cpp")) {
          parse_source(filename);
        }
      }
    }
  }
  auto parse_end = timestamp();

  LOG_B("Parsing took %f msec\n", (parse_end - parse_begin) * 1000.0);

  ConsoleDumper dumper;
  for (auto& [tag, cell] : cell_map) {
    CHECK_N(cell->flag.empty());
    CHECK_N(cell->tag.empty());
    CHECK_N(cell->name.empty());
    CHECK_N(cell->gate.empty());

    //if (cell->gate.empty()) cell->dump(dumper); // DFFs missing gates

    for (auto& input : cell->input_ports) {
      printf("%s\n", input.c_str());
    }
  }

  //----------------------------------------
  // Postprocess the cells.

#if 0
  auto process_begin = timestamp();
  for (auto& [tag, cell] : cell_map) {
    auto& info = gate_info(cell->gate);
    (void)info;
    vector<string> args = split_args(cell->args);

    // Cell type and output ports come from the DB.
    cell->cell_type = info.cell_type;
    cell->output_ports = info.output_ports;

    if (cell->gate == "tri_bus") {
      // Buses get all their arguments added as input ports.
      for (int i = 0; i < args.size(); i++) {
        char buf[256];
        snprintf(buf, 256, "tri%02d", i);
        cell->input_ports.push_back(buf);
      }
    }
    else {
      // All other cells use the input port list from the DB.
      CHECK_P(args.size() == info.input_ports.size());
      cell->input_ports = info.input_ports;
    }


    for (int iarg = 0; iarg < args.size(); iarg++) {
      // Split the argument up by '.' delimiters.
      const auto& arg = args[iarg];
      vector<string> parts = split_path(arg);

      // Skip any leading path parts that aren't a tag: foo.bar.baz.TAGG.qp -> TAGG.qp
      static regex rx_match_tag(R"(^(BUS_|PIN_|SIG_|EXT_|[A-Z]{4}))");
      while (!regex_search(parts.front(), rx_match_tag)) parts.erase(parts.begin());
      CHECK_P(parts.size() == 1 || parts.size() == 2);

      // Args without ports specified default to "out".
      string src_tag = name_to_tag(parts[0]);
      string src_port = parts.size() == 2 ? parts[1] : "out";

      // EXT_ args don't generate traces.
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
    }
  }
  sanity_check();
  auto process_end = timestamp();
  LOG_B("Processing took %f msec\n", (process_end - process_begin) * 1000.0);
  //LOG_B("Parsed %d tags\n",  unique_tags.size());
  LOG_B("Parsed %d files\n", total_files);
  LOG_B("Parsed %d lines\n", total_lines);

  //for (auto& t : unique_tags) printf("%s\n", t.c_str());
#endif

  return true;
}

//-----------------------------------------------------------------------------

std::string lhs_to_identifier(PNode node, const char* src) {
  if (node.is_identifier()) {
    auto name = node.body(src);
    return trim_name(name);
  }
  else if (node.is_field_expr()) {
    auto name = node.get_field(field_field).body(src);
    return trim_name(name);
  }
  else {
    CHECK_P(false);
    return "";
  }
}

//-----------------------------------------------------------------------------

std::string argument_to_identifier(PNode node, const char* src) {
  if (node.is_identifier()) {
    auto name = node.body(src);
    //trim_oldnew(name);
    return trim_name(name);
	}
	else if (node.is_call_expr()) {
		return argument_to_identifier(node.get_field(field_function), src);
	}
	else if (node.is_field_expr()) {
    auto prefix = node.get_field(field_argument);
    if (!prefix.is_identifier()) {
      prefix = prefix.get_field(field_field);
    }

		auto suffix = node.get_field(field_field).body(src);
    return trim_name(suffix);
	}
	else {
		CHECK_P(false);
		return "";
	}
}

//-----------------------------------------------------------------------------

std::vector<std::string> arglist_to_args(PNode node, const char* src) {
	CHECK_P(node.is_arglist());
	std::vector<std::string> arg_names;
	for (int i = 0; i < node.named_child_count(); i++) {
		arg_names.push_back(argument_to_identifier(node.named_child(i), src));
	}
	return arg_names;
}

//-----------------------------------------------------------------------------

static regex pin_tag(R"(\/\*(.)()(PIN_\d{2}) *\*\/)");
static regex sig_tag(R"(\/\*(.)()(SIG_\w+) *\*\/)");
static regex bus_tag(R"(\/\*(.)()(BUS_\w+) *\*\/)");
static regex cell_tag(R"(\/\*(.)(p[0-9]{2})\.([A-Z]{4}) *\*\/)");

bool DieDB::parse_header(const std::string& header_path) {
  LOG_G("%s\n", header_path.c_str());
  LOG_INDENT_SCOPE();

  PTree tree(header_path.c_str());
  auto src = tree.source();
  auto root = tree.root();

  std::deque<PNode> queue;
  queue.push_front(tree.root());
  ConsoleDumper dumper;
  int tag_count = 0;

  while(!queue.empty()) {
    PNode node = queue.front();
    queue.pop_front();
    node.enqueue_children(queue);
    if (!node.is_comment()) continue;
    if (node.body(src) == "/// plait_noparse") {
      LOG_R("Skipping\n");
      return false;
    }

    PNode next = node.next();
    if (next.is_null()) continue;

    smatch match;
    bool matched = false;
    //std::string flag;
    //std::string page;
    //std::string tag;
    //std::string name;
    //std::string gate;
    //std::string args;
    //std::string docs;

    std::string s = node.body(src);

    if (regex_match(s, match, pin_tag)) {
      tag_count++;
      auto flag = match[1].str();
      auto page = match[2].str();
      auto tag = match[3].str();
      matched = true;

      auto cell = get_or_create_cell(tag);
      cell->set_flag(flag);
      cell->set_page(page);

      auto decl = next.get_field(field_type).body(src);
      cell->set_type(decl_to_info(decl).cell_type);

      auto name = next.get_field(field_declarator).body(src);
      cell->set_name(name);

    }
    else if (regex_match(s, match, sig_tag)) {
      tag_count++;
      auto flag = match[1].str();
      auto page = match[2].str();
      auto tag = match[3].str();
      matched = true;

      auto cell = get_or_create_cell(tag);
      cell->set_name(tag);
      cell->set_flag(flag);
      cell->set_page(page);

      auto decl = next.get_field(field_type).body(src);
      cell->set_type(decl_to_info(decl).cell_type);
    }
    else if (regex_match(s, match, bus_tag)) {
      tag_count++;
      auto flag = match[1].str();
      auto page = match[2].str();
      auto tag = match[3].str();
      matched = true;

      auto cell = get_or_create_cell(tag);
      cell->set_flag(flag);
      cell->set_page(page);

      auto decl = next.get_field(field_type).body(src);
      cell->set_type(decl_to_info(decl).cell_type);

      auto name = next.get_field(field_declarator).body(src);
      cell->set_name(name);
    }
    else if (regex_match(s, match, cell_tag)) {
      tag_count++;
      auto flag = match[1].str();
      auto page = match[2].str();
      auto tag = match[3].str();
      matched = true;

      auto cell = get_or_create_cell(tag);
      cell->set_flag(flag);
      cell->set_page(page);

      auto decl_type = next.get_field(field_type).body(src);

      if (next.get_field(field_declarator).is_func_decl()) {
        CHECK_P(decl_type == "wire");
        cell->set_type(DieCellType::LOGIC);
      }
      else if (next.get_field(field_declarator).is_field_id()) {
        cell->set_type(decl_to_info(decl_type).cell_type);
      }
      else {
        CHECK_P(false);
      }

      auto decl_name = next.get_field(field_declarator);

      if (decl_name.is_field_id()) {
        cell->set_name(decl_name.body(src));
      }
      else if (decl_name.is_func_decl()) {
        decl_name = decl_name.get_field(field_declarator);
        cell->set_name(decl_name.body(src));
      }
      else {
        CHECK_P(false);
      }

    }
  }

  LOG_B("%d tags found\n", tag_count);

  return true;
}

//-----------------------------------------------------------------------------
// The qualified_identifier node has a bug... "name" field pointing to "::"

bool DieDB::parse_source(const std::string& source_path) {
  LOG_G("%s\n", source_path.c_str());
  LOG_INDENT_SCOPE();

  PTree tree(source_path.c_str());
  auto src = tree.source();

  std::deque<PNode> queue;
  queue.push_front(tree.root());
  ConsoleDumper dumper;
  int tag_count = 0;

  while(!queue.empty()) {
    PNode tag_node = queue.front();
    queue.pop_front();

    if (tag_node.is_preproc_if()) {
      if (tag_node.get_field(field_condition).body(src) == "0") {
        continue;
      }
      //tag_node.dump(src);
    }
    tag_node.enqueue_children(queue);

    if (!tag_node.is_comment()) continue;
    if (tag_node.body(src) == "/// plait_noparse") {
      LOG_R("Skipping\n");
      return false;
    }

    PNode node = tag_node.next();
    if (node.is_null()) continue;

    auto node_body = node.body(src);

    if (node.is_expression()) node = node.child(0);

    smatch match;

    std::string s = tag_node.body(src);

    if (regex_match(s, match, pin_tag)) {
      CHECK_P(node.is_call_expr());
      tag_count++;
      auto cell = get_cell(match[3].str());
      cell->set_flag(match[1].str());
      cell->set_page(match[2].str());

      cell->set_gate(node.get_field(field_function).get_field(field_field).body(src));
      cell->set_args(arglist_to_args(node.get_field(field_arguments), src));
    }
    else if (regex_match(s, match, sig_tag)) {
      CHECK_P(node.is_call_expr());
      tag_count++;
      auto cell = get_cell(match[3].str());
      cell->set_flag(match[1].str());
      cell->set_page(match[2].str());

      cell->set_gate(node.get_field(field_function).get_field(field_field).body(src));
      cell->set_args(arglist_to_args(node.get_field(field_arguments), src));
    }
    else if (regex_match(s, match, bus_tag)) {
      CHECK_P(node.is_call_expr());
      tag_count++;
      auto cell = get_cell(match[3].str());
      cell->set_flag(match[1].str());
      cell->set_page(match[2].str());

      cell->set_gate(node.get_field(field_function).get_field(field_field).body(src));
      cell->set_args(arglist_to_args(node.get_field(field_arguments), src));
    }
    else if (regex_match(s, match, cell_tag)) {
      tag_count++;
      auto cell = get_or_create_cell(match[3].str());
      cell->set_flag(match[1].str());
      cell->set_page(match[2].str());

      if (node.is_decl()) {
        auto type = node.get_field(field_type).body(src);

        if (type == "wire" || type == "triwire" || type == "Adder" || type == "uint8_t") {
          auto assignment = node.get_field(field_declarator);
          auto lhs = assignment.get_field(field_declarator);
          auto rhs = assignment.get_field(field_value);

          cell->set_name(lhs.body(src));
          cell->set_gate(rhs.get_field(field_function).body(src));
          cell->set_args(arglist_to_args(rhs.get_field(field_arguments), src));
        }
        else {
          node.dump(src);
        }
      }
      else if (node.is_assignment()) {
        auto lhs = node.get_field(field_left);
        auto rhs = node.get_field(field_right);

        CHECK_P(rhs.symbol() == sym_call_expression);

        cell->set_name(lhs_to_identifier(lhs, src));
        cell->set_gate(rhs.get_field(field_function).body(src));
        cell->set_args(arglist_to_args(rhs.get_field(field_arguments), src));
      }
      else if (node.is_call_expr()) {
        //node.dump(src);
        //printf("%s\n", s.c_str());
        CHECK_P(!cell->name.empty());
        //cell->dump(dumper);

        auto func = node.get_field(field_function);
        CHECK_P(func.is_field_expr());
        auto obj = func.get_field(field_argument);
        auto gate = func.get_field(field_field);
        CHECK_P(gate.is_field_id());
        cell->set_gate(trim_name(gate.body(src)));


        //cell->set_gate(rhs.get_field(field_function).body(src));
        //
        //func.dump(src);
        auto func_args = node.arglist();
      }
      else if (node.is_function()) {
        auto func_decl = node.get_field(field_declarator);
        auto func_name = func_decl.get_field(field_declarator).named_child(1);
        CHECK_P(func_name.is_identifier());

        auto func_return = node.get_field(field_body).named_child(0);
        CHECK_P(func_return.is_return());

        auto ret_statement = func_return.named_child(0);
        CHECK_P(ret_statement.is_call_expr());

        cell->set_name(func_name.body(src));
        cell->set_gate(ret_statement.get_field(field_function).body(src));
        cell->set_args(arglist_to_args(ret_statement.get_field(field_arguments), src));
      }
      else {
        CHECK_P(false);
        printf("%s\n", node.type());
      }


      tag_count++;
    }
  }

  LOG_B("%d tags found\n", tag_count);

  total_files++;
  return true;
}

//-----------------------------------------------------------------------------
