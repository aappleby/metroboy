#include "Plait/PlaitApp.h"
#include "AppLib/GLBase.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <deque>
#include <algorithm>

#include <windows.h>

using namespace std;

std::map<std::string, uint32_t> node_type_to_color;

std::set<string> all_tags;
std::set<string> verified_tags;

int total_matches = 0;

std::set<string> valid_cell_types = {
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

//-----------------------------------------------------------------------------

#if 0
Node* Plait::get_or_create_node(const std::string& name) {
  Node* new_node = get_node(name);
  if (new_node) return new_node;

  new_node = new Node();
  new_node->name = name;

  nodes.push_back(new_node);
  names.insert({name, new_node});

  return new_node;
}

Node* Plait::get_node(const std::string& name) {
  auto it = names.find(name);
  if (it == names.end()) {
    return nullptr;
  }
  else {
    return (*it).second;
  }
}
#endif

//-----------------------------------------------------------------------------

bool has_tag(const plait::CellDB& cell_db, const std::string& tag) {
  for (auto& cell : cell_db.cells()) {
    if (cell.tag() == tag) return true;
  }
  return false;
}

//-----------------------------------------------------------------------------

bool parse_tag(const std::string& tag) {
  static regex valid_tag(R"(^(.?)p([0-9]{2})\.([A-Z]{4})$)");

  smatch matches;
  if (regex_match(tag,  matches, valid_tag)) {
    //string verified = matches[1].str();
    //string page = matches[2].str();
    //string tag = matches[3].str();
    //if (verified == "#") verified_tags.insert(tag);
    return true;
  }
  else {
    printf("Could not parse tag %s\n", tag.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_type(const std::string type) {
  if (valid_cell_types.contains(type)) {
    return true;
  }
  else {
    printf("Could not parse type %s\n", type.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_arg(const std::string& arg) {

  static regex tagged_arg(R"(^(?:\w+\.)*[A-Z]{4}.*)");
  //static regex tagged_arg(R"(^(?:\w+\.)*[A-Z]{4}(?:_\w+)*(?:\.\w+\(\))*$)");

  //static regex simple_arg(R"(^(?:\w+\.)*[A-Z]{4}(?:_\w+)*(?:\.\w+\(\))*$)");
  //static regex scoped_arg(R"(^(?:\w+\.)*(\w+)(?:\..*)?)");
  static regex bus_arg(R"(^(?:\w+\.)*BUS_.*)");
  static regex sig_arg(R"(^(?:\w+\.)*SIG_.*)");
  static regex pin_arg(R"(^(?:\w+\.)*PIN\d{2}_.*)");
  //static regex call_arg(R"(^[A-Z]{4}_.*\(\)$)");

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

bool parse_arglist(const std::string& arglist) {
  static std::regex arg_regex("\\s*_?(.*?),");

  bool result = true;
  for (std::sregex_iterator i = std::sregex_iterator(arglist.begin(), arglist.end(), arg_regex); i != std::sregex_iterator(); i++) {
    result &= parse_arg((*i)[1].str().c_str());
  }
  return result;
}

//-----------------------------------------------------------------------------

bool parse_value(const std::string& value) {
  static regex valid_value(R"(.*?\((.*)\);.*)");

  smatch matches;
  if (regex_match(value, matches, valid_value)) {
    return parse_arglist(matches[1].str());
  }
  else {
    printf("Could not parse value %s\n", value.c_str());
    return false;
  }

}

//-----------------------------------------------------------------------------

bool parse_name(const std::string& name) {
  static regex valid_name(R"(^(?:\w+\.)*_?([A-Z]{4})(_?\w*)\s*$)");
  smatch matches;
  if (regex_match(name, matches, valid_name)) {
    return true;
  } else {
    printf("Could not parse name %s\n", name.c_str());
    return false;
  }
}

//-----------------------------------------------------------------------------

bool parse_rest(const std::string& rest) {
  static regex member_decl(R"(^(\w+)\s+(\w+);.*)");
  static regex member_assign(R"(^(?:\w+\.)*(\w+)\s*=\s*(.+;).*)");
  static regex local_decl(R"(^wire\s+(\w+)\s*=\s(.*)$)");
  static regex func_decl(R"(^(inline\s+)?wire\s+(\w+)\s*.*\{\s*return\s*(.*)\}.*)");
  static regex wire_decl(R"(^wire\s*(\w+);.*$)");
  static regex signal_decl(R"(^Signal\s*(\w+);.*$)");
  static regex tri_call(R"(^(.*)\.\s*(tri\w+\(.*$))");
  static regex dff_call(R"(^(.*)\.\s*(dff\w+\(.*$))");
  static regex latch_call(R"(^(.*)\.\s*(.*_latch[pn]?\(.*$))");
  static regex set_call(R"(^(.*)\.\s*set\((.*)\).*)");

  smatch matches;

  //line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

  bool result = true;

  if (regex_match(rest, matches, member_decl)) {
    result &= parse_type(matches[1].str());
    result &= parse_name(matches[2].str());
  }
  else if (regex_match(rest, matches, local_decl)) {
    result &= parse_name(matches[1].str());
    result &= parse_value(matches[2].str());
  }
  else if (regex_match(rest, matches, func_decl)) {
    result &= parse_name(matches[2].str());
    result &= parse_value(matches[3].str());
  }
  else if (regex_match(rest, matches, wire_decl)) {
    result &= parse_name(matches[1].str());
  }
  else if (regex_match(rest, matches, signal_decl)) {
    result &= parse_name(matches[1].str());
  }
  else if (regex_match(rest, matches, tri_call)) {
    //string bus = matches[1].str();
    result &= parse_value(matches[2].str());
  }
  else if (regex_match(rest, matches, dff_call)) {
    result &= parse_name(matches[1].str());
    result &= parse_value(matches[2].str());
  }
  else if (regex_match(rest, matches, latch_call)) {
    result &= parse_name(matches[1].str());
    result &= parse_value(matches[2].str());
  }
  else if (regex_match(rest, matches, set_call)) {
    result &= parse_name(matches[1].str());
    result &= parse_value(matches[2].str());
  }
  else if (regex_match(rest, matches, member_assign)) {
    result &= parse_name(matches[1].str());
    result &= parse_value(matches[2].str());
  }
  else {
    result = false;
  }
  return result;
}

//-----------------------------------------------------------------------------

void parse_line(std::string& line, plait::CellDB& cell_db) {
  static regex tagged_line_regex(R"(^\s*\/\*(.*?)\*\/\s*(.*))");
  (void)cell_db;

  bool result = true;

  smatch matches;
  if (regex_match(line, matches, tagged_line_regex)) {
    string tag = matches[1].str();
    string rest = matches[2].str();
    result &= parse_tag(tag);
    result &= parse_rest(rest);
  }

  if (!result) {
    printf("Could not parse line : \"%s\"\n", line.c_str());
  }
}

//-----------------------------------------------------------------------------

#if 0
void Node::dump(Dumper& d) {
  d("Node : \"%s\"\n", cell->name.c_str());
  d("Tag  : \"%s\"\n", cell->tag.c_str());
  d("Func : \"%s\"\n", cell->func.c_str());
  d("Tail : \"%s\"\n", cell->tail.c_str());
  for(auto p : prev) {
    d("Arg  : \"%s\"\n", p->cell->name.c_str());
  }
  d("\n");
}
#endif

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  printf("Loading plait cell db %f\n", timestamp());
  //std::ifstream lines("plait_data.txt");
  std::ifstream lines("conglom.txt");
  printf("\n");

  ConsoleDumper console;

  printf("Parsing plait cell db %f\n", timestamp());

  plait::CellDB* cell_db = new plait::CellDB();

  //string test_line = R"(  /* p21.TOBE*/ wire _TOBE_FF41_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.VARY_FF41p());)";
  //parse_line(test_line, *cell_db);

  string test_rest = "DFF9 AFUR_xxxxEFGHp;";
  parse_rest(test_rest);

  for (string line; getline(lines, line); ) {
    parse_line(line, *cell_db);
  }
  printf("\n");


  printf("Total matches %d\n", total_matches);
  printf("Unique tags %zd\n", all_tags.size());
  printf("Verified tags %zd\n", verified_tags.size());

#if 0
  printf("Cell count %d\n", cell_db->cells_size());

  printf("Printing plait cell db %f\n", timestamp());

  string blah;

  google::protobuf::TextFormat::PrintToString(*cell_db, &blah);
  //printf("dump:\n%s\n", blah.c_str());

  {
    plait::CellDB* cell_db2 = new plait::CellDB();
    google::protobuf::TextFormat::ParseFromString(blah, cell_db2);

    string blah2;
    google::protobuf::TextFormat::PrintToString(*cell_db2, &blah2);
    //printf("dump:\n%s\n", blah2.c_str());
  }


  //PlaitApp* app = new PlaitApp();
  //AppHost* app_host = new AppHost(app);
  //ret = app_host->app_main(argc, argv);
  //delete app;

  //plait::dvec2 blep;

  //blep.set_x(10);
  //blep.set_y(20);

  //printf("blep x %f\n", blep.x());
  //printf("blep y %f\n", blep.y());
#endif

  printf("Done %f\n", timestamp());

  return ret;
}

//-----------------------------------------------------------------------------

PlaitApp::~PlaitApp() {
};

const char* PlaitApp::app_get_title() {
  return "PlaitApp";
}

//-----------------------------------------------------------------------------

void PlaitApp::app_init() {
  node_type_to_color["not1b"] = 0xFF808080;
  node_type_to_color[""]      = 0xFF008000;

  node_type_to_color["and4"]    = 0xFF000080;
  node_type_to_color["nand4b"]  = 0xFF000080;
  node_type_to_color["nand3b"]  = 0xFF000080;

  node_type_to_color["mux2nb"] = 0xFF004080;
  node_type_to_color["mux2b"]  = 0xFF004080;

  node_type_to_color["or2"]   = 0xFF800000;
  node_type_to_color["nor2b"] = 0xFF800000;
  node_type_to_color["nor3b"] = 0xFF800000;

  node_type_to_color["dff17"] = 0xFF004040;
  node_type_to_color["dff20"] = 0xFF004040;
  node_type_to_color["dff22"] = 0xFF004040;

  check_gl_error();
  box_painter.init();
  check_gl_error();
  port_painter.init();
  check_gl_error();
  grid_painter.init();
  check_gl_error();
  edge_painter.init();
  check_gl_error();
  outline_painter.init();
  check_gl_error();
  text_painter.init();
  check_gl_error();
  blitter.init();
  check_gl_error();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix);

  keyboard_state = SDL_GetKeyboardState(nullptr);

#if 0
  Node test_nodes[] = {
    { { 256,  256},  { 256,  256}, "+====\\\n|REPU >\n+====/"},
    { { 20,  20},  { 20,  20}, "SARY"},
    { { 30,  30},  { 30,  30}, "REJO"},
    { { 40,  40},  { 40,  40}, "XOFO"},
    { { 50,  50},  { 50,  50}, "NUNU"},
    { { 60,  60},  { 60,  60}, "PYNU"},
    { { 70,  70},  { 70,  70}, "NOPA"},
    { { 80,  80},  { 80,  80}, "NAFY"},
    { { 90,  90},  { 90,  90}, "PYGO"},
    { {128, 128},  {128, 128}, "PORY"},
  };

  for (int i = 0; i < 10; i++) {
    Node* new_node = new Node();
    new_node->name = test_nodes[i].name;
    new_node->pos_old = test_nodes[i].pos_old;
    new_node->pos_new = test_nodes[i].pos_new;

    plait.nodes.push_back(new_node);
    plait.names[new_node->name] = new_node;
  }
#endif

  // Hook up next pointers
  /*
  for (auto next : plait.nodes) {
    for (auto prev : next->prev) {
      prev->next.push_back(next);
    }
  }
  */

  // Mark levels
  /*
  deque<Node*> queue;
  for (auto& n : plait.nodes) {
    n->mark = 0;
    if (n->prev.empty()) {
      n->rank = 0;
      plait.roots.insert(n);
      queue.push_back(n);
    }
    else {
      n->rank = -1;
    }
  }

  plait.update_rank();
  */

  // Initial placement
  /*
  vector<int> cursor_y(100, 0);

  for (auto& n : plait.nodes) {
    if (n->rank < 0) __debugbreak();

    n->pos_old.x = n->rank * 128;
    n->pos_old.y = cursor_y[n->rank] * 128;
    cursor_y[n->rank]++;
  }

  for (auto& n : plait.nodes) n->pos_new = n->pos_old;
  */

  printf("Done %f\n", timestamp());
}

//-----------------------------------------------------------------------------

void PlaitApp::app_close() {
}

//-----------------------------------------------------------------------------

Node* PlaitApp::pick_node(dvec2 pos) {
  (pos);

  /*
  printf("pick at %f %f\n", pos.x, pos.y);
  for (auto n : plait.nodes) {
    int width = 128;
    int height = 64;

    if (pos.x >= n->pos_old.x &&
        pos.y >= n->pos_old.y &&
        pos.x <= n->pos_old.x + width &&
        pos.y <= n->pos_old.y + height) {
      printf("picked %s\n", n->name.c_str());
      return n;
    }
  }
  */

  return nullptr;
}

//-----------------------------------------------------------------------------

void PlaitApp::app_update(Viewport view, double delta) {
  (void)delta;

  int mouse_x, mouse_y;
  mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse_pos = view.screenToWorld({mouse_x, mouse_y});

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) continue;

    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      hit_node = pick_node(mouse_pos);
      if (hit_node) {
        mouse_locked = true;
        drag_start = mouse_pos;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if (hit_node) {
        hit_node->pos_old = hit_node->pos_new;
        hit_node = nullptr;
      }
      drag_end = mouse_pos;
      mouse_locked = false;
      break;
    }
    }
  }

  if (mouse_buttons & SDL_BUTTON_LMASK) {
    if (hit_node) {
      hit_node->pos_new = hit_node->pos_old + (mouse_pos - drag_start);
      hit_node->pos_new.x = round(hit_node->pos_new.x / 16) * 16.0;
      hit_node->pos_new.y = round(hit_node->pos_new.y / 16) * 16.0;
    }
  }
}

//-----------------------------------------------------------------------------

void PlaitApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  text_painter.bg_col = {0,0.5,0,0.5};

  const int node_height = 64;
  const int node_width = 128;
  const int port_height = 4;
  const int port_width = 4;

#if 0
  for (auto n : plait.nodes) {
    float nx = (float)n->pos_new.x;
    float ny = (float)n->pos_new.y;

    outline_painter.push(nx,       ny,      nx + 128, ny);
    outline_painter.push(nx + 128, ny,      nx + 128, ny + 64);
    outline_painter.push(nx + 128, ny + 64, nx,       ny + 64);
    outline_painter.push(nx,       ny + 64, nx,       ny);

    {
      auto it = node_type_to_color.find(n->func);
      if (it == node_type_to_color.end()) {
        box_painter.push(nx + 4, ny + 4, 120, 56, 0xFFFF00FF);
      }
      else {
        box_painter.push(nx + 4, ny + 4, 120, 56, (*it).second);
      }
    }

    text_painter.add_text_at(n->name.c_str(), int(nx + 8), int(ny + 8));
    text_painter.add_text_at(n->func.c_str(), int(nx + 8), int(ny + 24));

    /*
    for (size_t i = 0; i < p->next.size(); i++) {
      auto n = p->next[i];
      float nx = float(n->pos_new.x);
      float ny = float(n->pos_new.y) + 8 + 16 * i;

      edge_painter.push(px + 128, py + 32, nx, ny + 32);
    }
    */

    /*
    float space = (64 - 4) / float(n->prev.size());

    for (size_t i = 0; i < n->prev.size(); i++) {
      auto p = n->prev[i];
      float px = float(p->pos_new.x) + 128;
      float py = float(p->pos_new.y) + 32;


      port_painter.push(nx,
                       ny + space * i + space / 2,
                       4, 4,
                       0x80808080);
    }
    */

    // input port(s)
    {
      const int port_count = int(n->prev.size());

      float gap = (node_height - (port_height * port_count)) / float(port_count + 1);
      float stride = gap + port_height;

      for (size_t i = 0; i < port_count; i++) {
        auto p = n->prev[i];
        float px = float(p->pos_new.x) + node_width;
        float py = float(p->pos_new.y) + node_height / 2;

        float port_x = nx + port_width / 2;
        float port_y = ny + gap + stride * i;

        edge_painter.push(px - port_width / 2,
                          py,
                          port_x,
                          port_y);

        port_painter.push(port_x - port_width / 2,
                          port_y - port_height / 2,
                          float(port_width),
                          float(port_height),
                          0x80808080);
      }
    }

    {
      port_painter.push(nx + node_width - port_width,
                        ny + node_height / 2 - port_height / 2,
                        float(port_width),
                        float(port_height),
                        0x80008000);
    }

  }
#endif

  port_painter.render(view, 0, 0, 1);
  edge_painter.render(view, 0, 0, 1);
  box_painter.render(view, 0, 0, 1);
  text_painter.render(view, 0, 0, 1);
  outline_painter.render(view, 0, 0, 1);
}

//-----------------------------------------------------------------------------

void PlaitApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
