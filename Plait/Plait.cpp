#include "Plait/Plait.h"

#if 0
void Node::dump(Dumper& d) {
  d("Node : \"%s\"\n", cell->name.c_str());
  d("Tag  : \"%s\"\n", cell->tag_comment.c_str());
  d("Func : \"%s\"\n", cell->func.c_str());
  d("Tail : \"%s\"\n", cell->tail.c_str());
  for(auto p : prev) {
    d("Arg  : \"%s\"\n", p->cell->name.c_str());
  }
  d("\n");
}
#endif

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

/*
bool has_tag(const plait::CellDB& cell_db, const std::string& tag) {
  for (auto& cell : cell_db.cells()) {
    if (cell.tag() == tag) return true;
  }
  return false;
}
*/


