#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "metrolib/core/File.h"
#include "Plait/PTree.h"
#include "Plait/CellDB.h"

#pragma warning(disable:4267)

#if 0
//-----------------------------------------------------------------------------

void tree_dump(PNode node, const char* source, int depth, const char* field_name) {
  if (node.is_wire_decl()) {
    printf("//----------------------------------------\n");
    for (int i = 0; i < depth * 4; i++) printf(" ");
    node.print(source, field_name);
    node.next().print(source, nullptr);
    printf("//----------\n");
    node.next().dump(source, 0, nullptr);
    printf("\n");
  }

  for (uint32_t i = 0; i < node.child_count(); i++) {
    tree_dump(node.child(i), source, depth + 1, node.field(i));
  }
}

//-----------------------------------------------------------------------------

void dump_all_comments(PNode node, const char* source) {
  if (node.is_comment()) node.print(source, nullptr);
    for (uint32_t i = 0; i < node.child_count(); i++) {
      dump_all_comments(node.child(i), source);
    }
}


//-----------------------------------------------------------------------------
#endif
