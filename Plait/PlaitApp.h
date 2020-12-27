#pragma once

#include "AppLib/App.h"
#include "AppLib/BoxPainter.h"
#include "AppLib/GridPainter.h"
#include "AppLib/LinePainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"

#include "CoreLib/Debug.h"

#include "plait_wrapper.h"

#include <map>
#include <set>
#include <vector>

struct Node;

typedef std::vector<Node*> NodeVec;
typedef std::map<std::string, Node*> NameToNode;
typedef std::multimap<std::string, std::string> NameToEdges;

//-----------------------------------------------------------------------------

struct Cell {
  std::string name;
  std::string tag;
  std::string func;
  std::string tail;

  std::vector<std::string> args;
};

//-----------------------------------------------------------------------------

struct CellDB {
};

//-----------------------------------------------------------------------------

struct Node {
  //void dump(Dumper& d);

  int rank;
  int mark;

  dvec2 pos_old;
  dvec2 pos_new;

  Cell * cell;
  std::vector<Node*> prev;
  std::vector<Node*> next;

#if 0
  void update_rank1() {
    mark = 1;
    rank = 0;
    for(auto p : prev) {
      if (p->mark) continue;
      if (p->rank == -1) p->update_rank1();
      if (p->rank >= rank) {
        rank = p->rank + 1;
      }
    }
    mark = 0;
  }

  void update_rank2() {
    if (prev.empty() && next.empty()) {
      rank = 0;
    }
    else if (prev.empty()) {
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;
      rank = min_next - 1;
    }
    else if (next.empty()) {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      rank = max_prev + 1;
    }
    else {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;

      rank = (max_prev + min_next) / 2;
    }
  }
#endif
};

//-----------------------------------------------------------------------------

struct Frame {
  std::string name;
  dvec2 origin;
  dvec2 size;
  dvec2 scale;

  std::vector<Frame*> frames;
  std::vector<Node*>  nodes;
};

//-----------------------------------------------------------------------------

struct Plait {
  void  parse_line(const std::string& line);

  //Node* get_node(const std::string& name);
  //Node* get_or_create_node(const std::string& name);

  /*
  void update_rank() {
    for (auto n : nodes) {
      n->rank = -1;
      n->mark = 0;
    }
    for (auto n : nodes) {
      n->update_rank1();
    }
    for (auto n : nodes) {
      n->update_rank2();
    }
  }
  */

  std::map<std::string, Cell*> cell_db;
  Frame* root_frame;

  //NodeVec     nodes;
  //NameToNode  names;
  //NameToEdges edges;
  //std::set<Node*> roots;
};

//-----------------------------------------------------------------------------

class PlaitApp : public App {
public:
  ~PlaitApp() override;

  const char* app_get_title() override;
  void app_init() override;
  void app_close() override;
  void app_update(Viewport view, double delta) override;
  void app_render_frame(Viewport view) override;
  void app_render_ui(Viewport view) override;

  bool is_mouse_locked() const override { return mouse_locked; }

  Node* pick_node(dvec2 pos);

  //----------------------------------------

  Plait plait;

  GridPainter grid_painter;
  LinePainter edge_painter;
  LinePainter outline_painter;
  TextPainter text_painter;
  BoxPainter  box_painter;
  BoxPainter  port_painter;
  Blitter     blitter;
  int tex = 0;

  const uint8_t* keyboard_state = nullptr;

  dvec2 drag_start;
  dvec2 drag_end;
  dvec2 mouse_pos;
  bool mouse_locked;

  int mouse_buttons;
  bool dragging;
  Node* hit_node;
};

//-----------------------------------------------------------------------------