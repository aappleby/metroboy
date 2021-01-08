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

// foo.bar.baz. foo.bar.baz._
#define MATCH_CELL_TAG_PREFIX "^(?:\\w+\\.)*_?"

// ASDF, ASDFq, ASDFn
#define MATCH_CELL_TAG        "[A-Z]{4}[a-z]?"

// PIN_12
#define MATCH_PIN_TAG         "PIN_[0-9]{2}"

// SIG_WHATEVER
#define MATCH_SIG_TAG         "SIG_\\w+"

// BUS_WHATEVER
#define MATCH_BUS_TAG         "BUS_\\w+"

// stuff.foo.bar.baz
#define MATCH_CELL_TAG_SUFFIX "(?:\\.[^.]+)*"

// literal /* at start of line
#define MATCH_COMMENT_BEGIN_LINE   R"(^\/\*)"
// literal */ at end of line
#define MATCH_COMMENT_END_LINE     R"(\*\/$)"

#define MATCH_WHITESPACE R"(\s*)"
#define MATCH_PAGE_TAG   R"(p[0-9]{2})"

#define CAPTURE_PIN_TAG      "(" MATCH_PIN_TAG  ")"
#define CAPTURE_PAGE_TAG     "(" MATCH_PAGE_TAG ")"
#define CAPTURE_VERIFIED_TAG "(.?)"

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

static std::map<DieCellType, string> cell_type_to_name = {
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

/*
      switch(arg_cell->cell_type) {
      case DieCellType::PIN_IN:   { CHECK_P(arg.port.starts_with("qp")); break;}
      case DieCellType::PIN_IO:   { CHECK_P(arg.port.starts_with("qp")); break;}
      case DieCellType::PIN_CLK:  { CHECK_P(arg.port == "clock_good" || arg.port == "clock"); break;}
      case DieCellType::SIG_IN:   { CHECK_P(arg.port == ""); break; }
      case DieCellType::BUS:      { CHECK_P(arg.port.starts_with("qp")); break;}
      case DieCellType::DFF:      { CHECK_P(arg.port.starts_with("qn") || arg.port.starts_with("qp")); break;}
      case DieCellType::LATCH:    { CHECK_P(arg.port.starts_with("qn") || arg.port.starts_with("qp")); break;}
      case DieCellType::TRIBUF:   { CHECK_P(arg.port == ""); break; }
      case DieCellType::LOGIC:    { CHECK_P(arg.port == ""); break; }
      case DieCellType::ADDER:    { CHECK_P(arg.port == "sum" || arg.port == "carry"); break; }
*/

std::map<DieCellType, std::vector<std::string>> DieDB::cell_type_to_out_ports = {
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

std::map<std::string, std::vector<std::string>> DieDB::gate_type_to_in_ports = {
  {"pin_in", {}},
  {"pin_out_pull_hilo",     {"PUn", "HI", "LO"}},
  {"pin_out_pull_hilo_any", {"PUn", "HI", "LO"}},
  {"pin_out_dp",            {"DP"}},
  {"pin_out_hilo",          {"HI", "LO"}},
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

void DieDB::clear() {
  for (auto& [tag, cell] : tag_to_cell) delete cell;
  tag_to_cell.clear();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void to_json(nlohmann::json& j, const DieCellArg& p) {
  j["prev_tag"]  = p.tag;
  j["prev_port"] = p.port;
}

void from_json(const nlohmann::json& j, DieCellArg& p) {
  j["prev_tag"] .get_to(p.tag);
  j["prev_port"].get_to(p.port);
}

void to_json(nlohmann::json& j, const DieCellEdge& e) {
  j["prev_tag"]  = e.prev_tag;
  j["prev_port"] = e.prev_port;
  j["next_tag"]  = e.next_tag;
  j["next_port"] = e.next_port;
}

void from_json(const nlohmann::json& j, DieCellEdge& e) {
  j["prev_tag"] .get_to(e.prev_tag);
  j["prev_port"].get_to(e.prev_port);
  j["next_tag"] .get_to(e.next_tag);
  j["next_port"].get_to(e.next_port);
}

void to_json(nlohmann::json& j, const DieCell* c) {
  j["cell_type"]  = c->cell_type;
  //j["verified"]   = c->verified;
  j["page"]       = c->page;
  //j["tag"]        = c->tag;
  j["gate"]       = c->gate;
  //j["args"]       = c->cell_args;
  j["name"]       = c->name;
  j["doc"]        = c->doc;
  j["prev_ports"] = c->prev_ports;
  j["next_ports"] = c->next_ports;
}

void from_json(const nlohmann::json& j, DieCell*& c) {
  c = new DieCell();

  c->cell_type  = j.value("cell_type",  DieCellType::UNKNOWN);
  //c->verified   = j.value("verified",   false);
  c->page       = j.value("page",       "<no_page>");
  //c->tag        = j.value("tag",        "<no_tag>");
  c->gate       = j.value("gate",       "<no_gate>");
  //c->cell_args       = j.value("args",       std::vector<DieCellArg>());
  c->name       = j.value("name",       "<no_name>");
  c->doc        = j.value("doc",        "<no_doc>");
  c->prev_ports = j.value("prev_ports", std::vector<std::string>());
  c->next_ports = j.value("next_ports", std::vector<std::string>());
}

void DieDB::save_json(const char* filename) {
  using namespace nlohmann;

  json root;

  root["cells"] = tag_to_cell;
  root["edges"] = edges;

  std::ofstream(filename) << root.dump(2);
}

void DieDB::load_json(const char* filename) {
  using namespace nlohmann;
  CHECK_P(tag_to_cell.empty());
  json root;
  std::ifstream(filename) >> root;

  root["cells"].get_to(tag_to_cell);
  root["edges"].get_to(edges);
}

void DieCell::dump(Dumper& d) const {
  using namespace nlohmann;

  json j;
  ::to_json(j, this);

  d(j.dump(2).c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void check_cell_tag(const std::string& tag) {
  (void)tag;
  CHECK_P(tag.size() == 4);
  CHECK_P(tag[0] >= 'A' && tag[0] <= 'Z');
  CHECK_P(tag[1] >= 'A' && tag[1] <= 'Z');
  CHECK_P(tag[2] >= 'A' && tag[2] <= 'Z');
  CHECK_P(tag[3] >= 'A' && tag[3] <= 'Z');
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    CHECK_P(gate.starts_with("pin_out"));
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
    __debugbreak();
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

#if 0
void DieCell::merge(const DieCell& c) {
  if (cell_type == DieCellType::UNKNOWN) cell_type = c.cell_type;
  if (verified.empty()) verified = c.verified;
  if (page.empty())     page = c.page;
  if (tag.empty())      tag = c.tag;
  if (gate.empty())     gate = c.gate;
  if (doc.empty())      doc = c.doc;
  if (cell_args.empty())     cell_args = c.cell_args;
  if (name.empty())     name = c.name;

  CHECK_P(cell_type == c.cell_type);

  if (!c.gate.empty() && (gate != c.gate)) {
    ConsoleDumper d;
    dump(d);
    d("------\n");
    c.dump(d);
  }

  CHECK_P(c.verified.empty() || verified == c.verified);
  CHECK_P(c.page.empty()     || page == c.page);
  CHECK_P(c.tag.empty()      || tag == c.tag);
  CHECK_P(c.gate.empty()     || gate == c.gate);
  CHECK_P(c.doc.empty()      || doc == c.doc);
  CHECK_P(c.name.empty()     || name == c.name);

  if (!c.cell_args.empty()) {
    CHECK_P(cell_args.size() == c.cell_args.size());
    for (size_t i = 0; i < cell_args.size(); i++) {
      CHECK_P(cell_args[i].tag  == c.cell_args[i].tag);
      CHECK_P(cell_args[i].port == c.cell_args[i].port);
    }
  }
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*#p00.ABCD*/
// /* p00.ABCD*/
// /*p00.ABCD*/

bool DieDB::parse_tag(const std::string& tag_comment, std::string& page_out, std::string& tag_out) {
  //static regex pin_tag (R"(^\/\*(.?)(PIN_[0-9]{2})\s*\*\/$)");

  static regex pin_tag(
    MATCH_COMMENT_BEGIN_LINE
    CAPTURE_VERIFIED_TAG
    CAPTURE_PIN_TAG
    MATCH_WHITESPACE
    MATCH_COMMENT_END_LINE
  );

  static regex sig_tag (R"(^\/\*(.?)(SIG_\w+)\s*\*\/$)");
  static regex bus_tag (R"(^\/\*(.?)(BUS_\w+)\s*\*\/$)");
  static regex cell_tag(R"(^\/\*(.?)p([0-9]{2})\.([A-Z]{4}\w*)\s*\*\/$)");

  smatch match;
  if (regex_match(tag_comment, match, pin_tag)) {
    //c.verified = match[1].str();
    tag_out = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment, match, sig_tag)) {
    //c.verified = match[1].str();
    tag_out = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment, match, bus_tag)) {
    //c.verified = match[1].str();
    tag_out = match[2].str();
    return true;
  }
  else if (regex_match(tag_comment,  match, cell_tag)) {
    //c.verified = match[1].str();
    page_out = match[2].str();
    tag_out = match[3].str();
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
  "mux2n", "mux2p", "amux2", "amux4",
};

/*
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
*/

std::map<std::string, DieCellType> gate_to_cell_type = {
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

  // FIXME pin types, sig types
};

bool DieDB::parse_cell_gate(DieCell& c, const std::string& gate) {
  CHECK_P(c.gate.empty() || c.gate == gate);
  c.gate = gate;

  if (valid_dff_gates.contains(gate)) {
    CHECK_P(c.cell_type == DieCellType::DFF || c.cell_type == DieCellType::UNKNOWN);
    c.cell_type = DieCellType::DFF;
    return true;
  }
  else if (valid_latch_gates.contains(gate)) {
    CHECK_P(c.cell_type == DieCellType::LATCH || c.cell_type == DieCellType::UNKNOWN);
    c.cell_type = DieCellType::LATCH;
    return true;
  }
  else if (valid_pin_gates.contains(gate)) {
    CHECK_P(c.cell_type == DieCellType::PIN_IN ||
            c.cell_type == DieCellType::PIN_OUT ||
            c.cell_type == DieCellType::PIN_IO ||
            c.cell_type == DieCellType::PIN_CLK);
    return true;
  }
  else if (valid_tribuf_gates.contains(gate)) {
    CHECK_P(c.cell_type == DieCellType::TRIBUF || c.cell_type == DieCellType::UNKNOWN);
    c.cell_type = DieCellType::TRIBUF;
    return true;
  }
  else if (valid_logic_gates.contains(gate)) {
    CHECK_P(c.cell_type == DieCellType::LOGIC || c.cell_type == DieCellType::UNKNOWN);
    c.cell_type = DieCellType::LOGIC;
    return true;
  }
  else if (gate == "add3") {
    CHECK_P(c.cell_type == DieCellType::ADDER || c.cell_type == DieCellType::UNKNOWN);
    c.cell_type = DieCellType::ADDER;
    return true;
  }
  else {
    printf("Could not parse gate \"%s\"\n", gate.c_str());
    __debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_reg_type(DieCell& c, const std::string& type) {
  static std::map<string, DieCellType> decl_to_reg_type = {
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


  if (decl_to_reg_type.contains(type)) {
    CHECK_P(c.cell_type == DieCellType::UNKNOWN ||
            c.cell_type == decl_to_reg_type[type]);
    c.cell_type = decl_to_reg_type[type];
    return true;
  }
  else {
    printf("Could not parse cell type %s\n", type.c_str());
    __debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_cell_arg(const std::string& arg, DieCellArg& arg_out) {

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
    "\\(\\)"
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
    arg_out = {match[1].str(), "qp"};
    return true;
  }
  else if (regex_match(arg, match, cell_arg_with_port)) {
    const auto& raw_port = match[2].str();

    if (raw_port == "qp_old") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qn_old") {
      arg_out = {match[1].str(), "qn"};
    }
    else if (raw_port == "qp_new") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qn_new") {
      arg_out = {match[1].str(), "qn"};
    }
    else if (raw_port == "qp_any") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qn_any") {
      arg_out = {match[1].str(), "qn"};
    }
    else if (raw_port == "carry") {
      arg_out = {match[1].str(), "carry"};
    }
    else if (raw_port == "sum") {
      arg_out = {match[1].str(), "sum"};
    }
    else {
      printf("port %s\n", match[2].str().c_str());
      __debugbreak();
    }

    return true;
  }
  else if (regex_match(arg, match, cell_arg_function)) {
    arg_out = {match[1].str(), "qp"};
    return true;
  }
  else if (regex_match(arg, match, sig_arg)) {
    arg_out = {match[1].str(), "qp"};
    return true;
  }
  else if (regex_match(arg, match, bus_arg)) {
    const auto& raw_port = match[2].str();

    if (raw_port == "qp_old") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qp_new") {
      arg_out = {match[1].str(), "qp"};
    }
    else {
      printf("bus port %s\n", match[2].str().c_str());
      __debugbreak();
    }

    return true;
  }
  else if (regex_match(arg, match, pin_arg_with_port)) {
    const auto& raw_port = match[2].str();

    if      (raw_port == "qp_new") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qp_any") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "qp_old") {
      arg_out = {match[1].str(), "qp"};
    }
    else if (raw_port == "clock") {
      arg_out = {match[1].str(), "clock"};
    }
    else if (raw_port == "clock_good") {
      arg_out = {match[1].str(), "clock_good"};
    }
    else {
      printf("pin port %s\n", match[2].str().c_str());
      __debugbreak();
    }

    return true;
  }
  else if (regex_match(arg, match, pin_arg)) {
    arg_out = {match[1].str(), "qp"};
    return true;
  }
  else {
    printf("Could not parse arg \"%s\"\n", arg.c_str());
    __debugbreak();
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <input>, <input>...

bool DieDB::parse_cell_arglist(DieCell& c, const string& arglist_c) {
  static regex arg_regex(R"(\s*_?(.+?)(,|$))");

  CHECK_N(c.tag.empty());
  CHECK_N(c.gate.empty());

  const auto& next_ports = gate_type_to_in_ports[c.gate];
  CHECK_N(next_ports.empty());

  string arglist = arglist_c;
  arglist.erase(remove_if(arglist.begin(), arglist.end(), ::isspace), arglist.end());

  bool result = true;
  int i = 0;
  for (sregex_iterator it = sregex_iterator(arglist.begin(), arglist.end(), arg_regex); it != sregex_iterator(); it++) {
    DieCellArg prev_arg;
    result &= parse_cell_arg((*it)[1].str(), prev_arg);
    CHECK_P(result);

    DieCellEdge edge = {
      prev_arg.tag,
      prev_arg.port,
      c.tag,
      next_ports[i]
    };

    edges.push_back(edge);

    i++;
  }

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// <gate>( <input list> );

bool DieDB::parse_cell_def(DieCell& c, const string& value) {
  static regex valid_value(R"((.*?)\s*\((.*)\);\s*(.*))");

  //printf("value %s\n", value.c_str());

  bool result = true;
  smatch match;
  if (regex_match(value, match, valid_value)) {
    result &= parse_cell_gate(c, match[1].str());
    result &= parse_cell_arglist(c, match[2].str());
    CHECK_P(c.doc.empty());
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
    printf("Could not parse name %s\n", name.c_str());
    return false;
  }

  if (base_name.ends_with("_old") || base_name.ends_with("_new") || base_name.ends_with("_any")) {
    base_name.resize(base_name.size() - 4);
  }

  if (base_name.size()) {
    CHECK_P(c.name.empty() || c.name == base_name);
    c.name = base_name;
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
    bus_cell->gate = "bus";

    size_t new_port_index = bus_cell->prev_ports.size();
    char new_port_name[256];
    sprintf_s(new_port_name, 256, "arg%02zd", new_port_index);

    bus_cell->prev_ports.push_back(new_port_name);

    DieCellEdge edge = {
      c.tag,
      "qp",
      bus_cell->tag,
      new_port_name
    };

    edges.push_back(edge);

    return true;
  } else {
    printf("Could not parse bus name \"%s\"\n", bus_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_pin_name(DieCell& c, const string& pin_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(PIN_\d{2}_\w+)$)");

  smatch match;
  if (regex_match(pin_name, match, valid_pin_name)) {
    CHECK_P(c.name.empty());
    c.name = match[1].str();
    return true;
  } else {
    printf("Could not parse pin name %s\n", pin_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_sig_name(DieCell& c, const string& sig_name) {
  static regex valid_pin_name(R"(^(?:\w+\.)*(SIG_\w+)\s*$)");

  smatch match;
  if (regex_match(sig_name, match, valid_pin_name)) {
    CHECK_P(c.name.empty());
    c.name = match[1].str();
    return true;
  } else {
    printf("Could not parse sig name %s\n", sig_name.c_str());
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// everything after the comment tag

bool DieDB::parse_rest(DieCell& c, const string& rest) {
  static regex member_decl(R"(^(\w+)\s+(\w+)\s*;.*)");
  static regex member_assign(R"(^(?:\w+\.)*(\w+)\s*=\s*(.+;).*)");
  static regex local_decl(R"(^wire\s+(\w+)\s*=\s*(.*)$)");
  static regex auto_decl (R"(^auto\s+(\w+)\s*=\s*(.*)$)");
  static regex func_decl(R"(^(inline\s+)?wire\s+(\w+)\s*.*\{\s*return\s*(.*)\}.*)");
  static regex wire_decl(R"(^wire\s*(\w+);.*$)");
  static regex signal_decl(R"(^Signal\s*(\w+);.*$)");
  static regex tri_call(R"(^(.*)\.\s*(tri\w+\(.*$))");
  static regex dff_call(R"(^(.*)\.\s*(dff\w+\(.*$))");
  static regex latch_call(R"(^(.*)\.\s*(.*_latch[pn]?\(.*$))");

  //static regex sig_set_call(R"(^(.*)\.\s*set\((.*)\).*)");
  //static regex sig_in_call(R"(^(.*)\.\s*set\((.*)\).*)");
  static regex sig_out_call(R"(^(.*)\.\s*sig_out\((.*)\).*)");

  static regex pin_call(R"(^(.*)\.\s*(pin\w+\(.*$))");

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
  else if (regex_match(rest, match, auto_decl)) {
    result &= parse_cell_name(c, match[1].str());
    result &= parse_cell_def(c, match[2].str());
  }
  else if (regex_match(rest, match, func_decl)) {
    result &= parse_cell_name(c, match[2].str());
    result &= parse_cell_def(c, match[3].str());
  }
  else if (regex_match(rest, match, wire_decl)) {
    printf("are we using this?");
    __debugbreak();
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
    printf("Could not parse rest : \"%s\"\n", rest.c_str());
    result = false;
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_line(const std::string& line) {
  static regex tagged_line_regex(R"(^\s*(\/\*.*?\*\/)\s*(.*))");

  bool result = true;
  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {

    std::string tag;
    std::string page;

    if (parse_tag(matches[1].str(), page, tag)) {
      auto cell = get_or_create_cell(tag);
      CHECK_P(cell->page.empty() || cell->page == page);
      cell->page = page;
      result &= parse_rest(*cell, matches[2].str());
    }
    else {
      printf("Could not parse line : \"%s\"\n", line.c_str());
      result = false;
    }
  }
  else {
    result = false;
  }

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_file(const std::string& path) {
  ConsoleDumper d;

  bool result = true;
  printf("Parsing %s\n", path.c_str());
  std::ifstream lines(path);
  for (string line; getline(lines, line); ) {
    total_lines++;
    if (parse_line(line)) {
      total_tagged_lines++;
    }
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool DieDB::parse_dir(const std::string& path) {
  (void)path;

  ConsoleDumper d;

  //----------------------------------------
  // Parse all the matching files and create the cell database.

  bool result = true;
#if 0
  {
    string line = R"(  /* p29.ERUC*/ auto _ERUC_YDIFF0 = add3(EBOS_LY0n, oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND);)";
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
  // Postprocess the cells.

  for (auto& [tag, cell] : tag_to_cell) {
    if (cell->cell_type == DieCellType::TRIBUF) {
      // Tribufs don't get names, add the tag as the default name.
      CHECK_P(cell->name.empty());
      cell->name = cell->tag;
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
      CHECK_P(cell->cell_type == DieCellType::UNKNOWN);
      CHECK_P(cell->gate.empty());
      cell->cell_type = DieCellType::BUS;
      cell->gate = "bus";
    }

    if (cell->cell_type == DieCellType::BUS) {
    }
    else {
      CHECK_P(gate_type_to_in_ports.contains(cell->gate));
      cell->prev_ports = gate_type_to_in_ports[cell->gate];
    }

    CHECK_P(cell_type_to_out_ports.contains(cell->cell_type));
    cell->next_ports = cell_type_to_out_ports[cell->cell_type];
  }

  //----------------------------------------
  // Check that all cells are sane.

  for (auto& [tag, cell] : tag_to_cell) {
    cell->sanity_check();

#if 0
    for (auto arg : cell->cell_args) {
      auto arg_cell = tag_to_cell[arg.tag];
      CHECK_P(arg_cell);

      // Tribufs can only drive a bus, buses can only be driven by tribufs.
      if (cell->cell_type == DieCellType::BUS) {
        CHECK_P(arg_cell->cell_type == DieCellType::TRIBUF);
      }
      if (arg_cell->cell_type == DieCellType::TRIBUF) {
        CHECK_P(cell->cell_type == DieCellType::BUS);
      }

      if (std::find(arg_cell->next_ports.begin(), arg_cell->next_ports.end(), arg.port) == arg_cell->next_ports.end()) {
        printf("Bad port %s\n", arg.port.c_str());
        __debugbreak();
      }
    }
#endif
  }

  //----------------------------------------
  // Check that all cells are used by some other cell, or are output cells.

#if 0
  for (auto& [tag, cell] : tag_to_cell) {
    for (const auto& arg : cell->cell_args) {
      CHECK_P(has_cell(arg.tag));
      get_cell(arg.tag)->mark++;
    }
  }
#endif

#if 0
  bool any_unused = false;
  for (auto& [tag, cell] : tag_to_cell) {
    if (cell->cell_type == DieCellType::PIN_OUT) continue;
    if (cell->cell_type == DieCellType::SIG_OUT) continue;

    // HACK - this is the one bus bit that isn't used by anything other than the OAM ram itself...
    if (cell->tag == "BUS_OAM_A01n") continue;

    if (cell->mark == 0) {
      printf("Cell %s unused\n", cell->name.c_str());
      any_unused = true;
    }
    cell->mark = 0;
  }

  if (any_unused) __debugbreak();
#endif

  //----------------------------------------
  // Dump all edges to the edge list

#if 0
  for (auto& [next_tag, next_cell] : tag_to_cell) {
    for (auto i = 0; i < next_cell->cell_args.size(); i++) {
      auto& next_port = next_cell->prev_ports[i];
      auto& prev_tag = next_cell->cell_args[i].tag;
      auto& prev_port = next_cell->cell_args[i].port;
      edges1.push_back({
        prev_tag,
        prev_port,
        next_tag,
        next_port
      });
    }
  }
#endif

  //----------------------------------------
  // Done, dump stats.

  printf("Total lines %d\n", total_lines);
  printf("Total tagged lines %d\n", total_tagged_lines);
  printf("Tag map size %zd\n", tag_to_cell.size());
  //printf("Unused tags %d\n", unused_count);

  //ConsoleDumper d;
  //for (auto& [key, value] : cell_map) value->dump(d);
  //for (auto& [key, value] : pin_map)  value->dump(d);
  //for (auto& [key, value] : bus_map)  value->dump(d);
  //for (auto& [key, value] : sig_map)  value->dump(d);

  printf("edges %zd should be 5871\n", edges.size());

  printf("\n");
#endif

  return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

