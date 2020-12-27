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

#include "plait_wrapper.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <deque>

#include <windows.h>

using namespace std;

std::map<std::string, uint32_t> node_type_to_color;

#if 0
  const string lines[] = {
    "/* p01.RAVE*/ RAVE_DIV11n_evn <= not1b(SOLA_DIV11p);",
    "/* p01.RYSO*/ RYSO_DIV12n_evn <= not1b(SUBU_DIV12p);",
    "/* p01.RYSO*/ RYSO_DIV12n_evn <= not1b(SUBU_DIV12p);",
    "/* p01.SAPY*/ SAPY_DIV09n_evn <= not1b(TOFE_DIV09p);",
    "/* p01.SOLA*/ SOLA_DIV11p <= dff17(TERU_DIV10p, UFOL_DIV_RSTn, SOLA_DIV11p);",
    "/* p01.SUBU*/ SUBU_DIV12p <= dff17(SOLA_DIV11p, UFOL_DIV_RSTn, SUBU_DIV12p);",
    "/* p01.TAGY*/ TAGY_FF04_RDp_ext <= and4(TEDO_CPU_RDp, RYFO_FF04_FF07p, TOLA_A01n, TOVY_A00n);",
    "/* p01.TAMA*/ TAMA_DIV05p <= dff17(UNYK_DIV04p, UFOL_DIV_RSTn, TAMA_DIV05p);",
    "/* p01.TAPE*/ TAPE_FF04_WRp <= and4(TAPU_CPU_WRp, RYFO_FF04_FF07p, TOLA_A01n, TOVY_A00n);",
    "/* p01.TEKA*/ TEKA_DIV13p <= dff17(SUBU_DIV12p, UFOL_DIV_RSTn, TEKA_DIV13p);",
    "/* p01.TERU*/ TERU_DIV10p <= dff17(TOFE_DIV09p, UFOL_DIV_RSTn, TERU_DIV10p);",
    "/* p01.TOFE*/ TOFE_DIV09p => dff17(TUGO_DIV08p, UFOL_DIV_RSTn, TOFE_DIV09p);",
    "/* p01.TUGO*/ TUGO_DIV08p <= dff17(TULU_DIV07p, UFOL_DIV_RSTn, TUGO_DIV08p);",
    "/* p01.TULU*/ TULU_DIV07p <= dff17(UGOT_DIV06p, UFOL_DIV_RSTn, TULU_DIV07p);",
    "/* p01.UDOR*/ UDOR_DIV13n_evn <= not1b(TEKA_DIV13p);",
    "/* p01.UFOL*/ UFOL_DIV_RSTn <= nor3b(UCOB_CLKBADp, PIN71_int_qp_new, TAPE_FF04_WRp);",
    "/* p01.UFOR*/ UFOR_DIV01p <= dff17(UKUP_DIV00p, UFOL_DIV_RSTn, UFOR_DIV01p);",
    "/* p01.UGOT*/ UGOT_DIV06p <= dff17(TAMA_DIV05p, UFOL_DIV_RSTn, UGOT_DIV06p);"
  };
#endif


//regex line_regex(R"((.*?)\s*(<=|=>)\s*(.*))");
regex line_regex(R"((.*?)\s*<=\s*(.+))");
regex prefix_regex(R"((.*?)\s*(\S+)$)");
regex func_regex(R"((\w*)\((.*)\);?\s*(.*?)\s*$)");
regex arg_regex(R"(\s*(\S+?)\s*(,|$)\s*)");
regex signal_regex(R"(^\s*(\S+);\s*(.*?)$)"); // not sure if this one works

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

void parse_line(const std::string& line, plait::CellDB& cell_db) {

  string prefix;
  string suffix;
  string arglist;

  string tag;
  string name;
  string dir;
  string func;
  vector<string> args;
  string tail;

  smatch line_matches;
  if (regex_search(line, line_matches, line_regex)) {
    prefix = line_matches[1].str();
    suffix = line_matches[2].str();

    smatch prefix_matches;
    if (regex_search(prefix, prefix_matches, prefix_regex)) {
      tag = prefix_matches[1].str();
      name = prefix_matches[2].str();
    } else {
      printf("prefix match fail\n");
      return;
    }

    smatch suffix_matches;
    if (regex_search(suffix, suffix_matches, func_regex)) {
      func = suffix_matches[1].str();
      arglist = suffix_matches[2].str();
      tail = suffix_matches[3].str();

      auto arg_begin = sregex_iterator(arglist.begin(), arglist.end(), arg_regex);
      auto arg_end = sregex_iterator();
      for (sregex_iterator i = arg_begin; i != arg_end; ++i) {
        args.push_back((*i)[1].str());
      }
    }
    else if (regex_search(suffix, suffix_matches, signal_regex)) {
      args.push_back(suffix_matches[1].str());
      func = "<signal>";
      tail = suffix_matches[2].str();
    }
    else {
      printf("suffix match fail\n");
      return;
    }
  }
  else {
    printf("line match fail\n");
    return;
  }

  auto new_cell = cell_db.mutable_cells()->Add();

  new_cell->set_name(name);
  new_cell->set_tag(tag);
  new_cell->set_type(func);
  new_cell->set_doc(tail);

  for (auto& arg : args) {
    new_cell->add_args(arg);
  }

  /*
  Node* gate_node = get_or_create_node(name);
  gate_node->tag = tag;
  gate_node->func = func;
  gate_node->tail = tail;
  gate_node->prev = prev;
  */
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

  printf("Loading plait %f\n", timestamp());
  std::ifstream lines("plait_data.txt");
  printf("\n");

  ConsoleDumper console;

  printf("Parsing plait %f\n", timestamp());

  plait::CellDB* cells = new plait::CellDB();

  for (string line; getline(lines, line); ) {
    //console("Line : \"%s\"\n", line.c_str());
    parse_line(line, *cells);
  }

  string blah;

  google::protobuf::TextFormat::PrintToString(*cells, &blah);

  printf("dump:\n, %s\n", blah.c_str());


  //PlaitApp* app = new PlaitApp();
  //AppHost* app_host = new AppHost(app);
  //ret = app_host->app_main(argc, argv);
  //delete app;

  //plait::dvec2 blep;

  //blep.set_x(10);
  //blep.set_y(20);

  //printf("blep x %f\n", blep.x());
  //printf("blep y %f\n", blep.y());

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
