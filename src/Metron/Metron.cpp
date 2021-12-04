#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <tree_sitter/api.h>
#include <vector>
#include <deque>
#include <regex>
#include <span>
#include "../Plait/TreeSymbols.h"
#include <functional>
#include "Node.h"
#include "Utils.h"

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

struct CodeEmitter;
typedef std::function<void(CodeEmitter* self, Node n)> Visitor1;

//------------------------------------------------------------------------------
/*
argument_list
array_declarator
assignment_expression
auto
binary_expression
call_expression
cast_expression
compound_statement
condition_clause
conditional_expression
declaration
expression_statement
field_expression
for_statement
function_declarator
if_statement
init_declarator
initializer_list
parameter_declaration
parameter_list
parenthesized_expression
pointer_expression
preproc_def
preproc_function_def
preproc_if
preproc_include
preproc_params
qualified_identifier
sizeof_expression
subscript_expression
translation_unit
type_descriptor
unary_expression
update_expression
while_statement 
*/

//------------------------------------------------------------------------------

struct TSNodeIt {

  TSNodeIt& operator++() {
    index++;
    return *this;
  }

  bool operator != (const TSNodeIt& n) {
    if (parent.context[0] != n.parent.context[0]) return true;
    if (parent.context[1] != n.parent.context[1]) return true;
    if (parent.context[2] != n.parent.context[2]) return true;
    if (parent.context[3] != n.parent.context[3]) return true;
    if (parent.id != n.parent.id)         return true;
    if (parent.tree != n.parent.tree)       return true;

    if (index != n.index) return true;

    return false;
  }

  TSNode operator*() const {
    return ts_node_child(parent, index);
  }

  TSNode parent;
  int index;
};

TSNodeIt begin(TSNode& parent) {
  return { parent, 0 };
}

TSNodeIt end(TSNode& parent) {
  return { parent, (int)ts_node_child_count(parent) };
}

//------------------------------------------------------------------------------

struct CodeEmitter {

  const char* field_name(int field_id) {
    return field_id == -1 ? nullptr : ts_language_field_name_for_id(lang, field_id);
  }

  //----------------------------------------

  CodeEmitter(const char* filename) {
    src_blob = load_blob(filename);
    src_blob.push_back(0);

    source = (const char*)src_blob.data();
    parser = ts_parser_new();
    lang = tree_sitter_cpp();

    ts_parser_set_language(parser, lang);
    tree = ts_parser_parse_string(parser, NULL, source, (uint32_t)src_blob.size());

    root = ts_tree_root_node(tree);
  }

  //----------------------------------------

  ~CodeEmitter() {
    ts_tree_delete(tree);
    ts_parser_delete(parser);

    src_blob.clear();
    lang = nullptr;
    parser = nullptr;
    tree = nullptr;
    source = nullptr;
  }

  //----------------------------------------

  void visit_chunks(Node n, Visitor1 visitor) {
    auto child_count = ts_node_child_count(n.tsnode);
    if (child_count == 0) return;

    auto cursor = ts_node_start_byte(n.tsnode);
    for (uint32_t i = 0; i < child_count; i++) {

      TSNode c = ts_node_child(n.tsnode, i);
      Node c2 = {
        c,
        n.depth + 1,
        (int)i,
        ts_node_start_byte(c),
        ts_node_end_byte(c),
        ts_node_field_id_for_child(n.tsnode, i)
      };

      if (cursor < c2.span_a) {
        visitor(this, { n.depth + 1, (int)i, cursor, c2.span_a });
        cursor = c2.span_a;
      }

      visitor(this, c2);
      cursor = c2.span_b;
    }

    if (cursor < n.span_b) {
      visitor(this, { n.depth + 1, (int)child_count, cursor, n.span_b });
      cursor = n.span_b;
    }
  }

  //----------------------------------------

  void dump_node(Node n) {
    indent(n.depth);
    printf("[%d] ", n.index);

    if (n.field_id != -1) {
      printf("%s.", field_name(n.field_id));
    }

    if (n.is_branch()) {
      printf("%s: ", n.type());
    }
    else if (n.is_leaf()) {
      printf("%s: ", n.type());
      print_escaped(source, n.span_a, n.span_b);
    }
    else if (n.is_null()) {
      printf("text: ");
      print_escaped(source, n.span_a, n.span_b);
    }
    else {
      // Unnamed nodes usually have their node body as their "type",
      // and their symbol is something like "aux_sym_preproc_include_token1"
      printf("lit: ");
      //printf("%d: ", n.symbol());
      //printf("%s: ", n.type());
      print_escaped(source, n.span_a, n.span_b);
    }

    printf("\n");
  }

  //----------------------------------------

  void dump(Node n) {
    if (n.is_null()) {
      dump_node(n);
    }
    else switch (n.symbol()) {
    case sym_comment:
      dump_node(n);
      break;
    default:
      dump_node(n);
      visit_chunks(n, &CodeEmitter::dump);
      break;
    }
  }

  blob src_blob;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;
  const char* source = nullptr;
  TSNode root;

  //------------------------------------------------------------------------------

  void advance_to(uint32_t& cursor, uint32_t end, const char* source) {
    if (cursor < end) {
      fwrite(&source[cursor], 1, end - cursor, stdout);
      cursor = end;
    }
  }

  //------------------------------------------------------------------------------

  typedef std::function<void(CodeEmitter*, TSNode, const char* source)> ChunkVisitor;

  void visit_chunks(TSNode n, const char* source, ChunkVisitor cv) {

    auto cursor = ts_node_start_byte(n);

    for (const auto& c : n) {
      advance_to(cursor, ts_node_start_byte(c), source);
      cv(this, c, source);
      cursor = ts_node_end_byte(c);
    }

    advance_to(cursor, ts_node_end_byte(n), source);
  }

  //------------------------------------------------------------------------------

  void emit_raw_text(const char* a, const char* b) {
    fwrite(a, 1, b - a, stdout);
  }

  void emit_body(TSNode n, const char* source) {
    emit_raw_text(&source[ts_node_start_byte(n)], &source[ts_node_end_byte(n)]);
  }

  void emit_gap(TSNode a, TSNode b, const char* source) {
    emit_raw_text(&source[ts_node_end_byte(a)], &source[ts_node_start_byte(b)]);
  }

  void emit_leaf(TSNode n, const char* source) {
    assert(!ts_node_is_null(n) && !ts_node_child_count(n));
    emit_body(n, source);
  }

  void emit_error(TSNode n, const char* source) {
    if (ts_node_is_named(n)) {
      printf("XXX %s ", ts_node_type(n));
      printf("XXX");
      emit_body(n, source);
      printf("XXX");
    }
    else {
      printf("XXXXXXX");
      emit_body(n, source);
      printf("XXXXXXX");
    }
    printf("\n");
    printf("\n");
    dump({ n, 0, 0, ts_node_start_byte(n), ts_node_end_byte(n), -1 });
    printf("\n");
  }

  void emit_placeholder(TSNode n, const char* source) {
    if (ts_node_is_named(n)) {
      printf("%s ", ts_node_type(n));
      printf("<<<");
      emit_body(n, source);
      printf(">>>");
    }
    else {
      printf("<<<");
      emit_body(n, source);
      printf(">>>");
    }

    printf("\n");
    printf("\n");
    dump({ n, 0, 0, ts_node_start_byte(n), ts_node_end_byte(n), -1 });
    printf("\n");
  }

  //------------------------------------------------------------------------------

  void emit_access_specifier(TSNode n, const char* source) {
    printf("// %s", body(n, source).c_str());
  }

  //------------------------------------------------------------------------------

  void emit_comment(TSNode n, const char* source) {
    printf("%s", body(n, source).c_str());
  }

  //------------------------------------------------------------------------------

  void emit_include(TSNode n, const char* source) {
    assert(ts_node_symbol(n) == sym_preproc_include);
    auto path = body(ts_node_child_by_field_id(n, field_path), source);
    static regex rx_trim(R"(\.h)");
    path = std::regex_replace(path, rx_trim, ".sv");
    printf("`include %s\n", path.c_str());
  }

  //------------------------------------------------------------------------------

  void emit_lvalue(TSNode n, const char* source) {
    if (ts_node_symbol(n) == sym_identifier) {
      emit_leaf(n, source);
    }
    else {
      emit_error(n, source);
    }
  }

  //------------------------------------------------------------------------------

  void emit_rvalue(TSNode n, const char* source) {
    emit_expression(n, source);
  }

  //------------------------------------------------------------------------------

  void emit_binary_expression(TSNode n, const char* source) {
    auto exp_lv = ts_node_child(n, 0);
    auto exp_op = ts_node_child(n, 1);
    auto exp_rv = ts_node_child(n, 2);

    emit_lvalue(exp_lv, source);
    emit_gap(exp_lv, exp_op, source);
    emit_leaf(exp_op, source);
    emit_gap(exp_op, exp_rv, source);
    emit_rvalue(exp_rv, source);
  }

  //------------------------------------------------------------------------------

  void emit_call_expression(TSNode n, const char* source) {

    auto call_func = ts_node_child(n, 0);
    auto call_args = ts_node_child(n, 1);

    emit_leaf(call_func, source);

    visit_chunks(call_args, source, [&](CodeEmitter* ce, TSNode n, const char* source) {
      if (ts_node_is_named(n)) {
        emit_expression(n, source);
      }
      else {
        emit_leaf(n, source);
      }
      });

  }

  //------------------------------------------------------------------------------

  void emit_number_literal(TSNode n, const char* source) {
    emit_leaf(n, source);
  }

  //------------------------------------------------------------------------------

  void emit_return_statement(TSNode n, const char* source) {
    auto ret_keyword = ts_node_child(n, 0);
    auto ret_expr = ts_node_child(n, 1);
    auto ret_tail = ts_node_child(n, 2);

    emit_leaf(ret_keyword, source);
    emit_gap(ret_keyword, ret_expr, source);
    emit_expression(ret_expr, source);
    emit_leaf(ret_tail, source);
  }

  //------------------------------------------------------------------------------

  void emit_expression_statement(TSNode n, const char* source) {
    auto statement_body = ts_node_child(n, 0);
    auto statement_tail = ts_node_child(n, 1);

    emit_expression(statement_body, source);
    emit_leaf(statement_tail, source);
  }

  //------------------------------------------------------------------------------

  void emit_function_arg(TSNode n, const char* source) {
    auto param_type = ts_node_child_by_field_id(n, field_type);
    auto param_decl = ts_node_child_by_field_id(n, field_declarator);

    emit_leaf(param_type, source);
    emit_gap(param_type, param_decl, source);
    emit_leaf(param_decl, source);
  }

  //------------------------------------------------------------------------------

  void emit_function_definition(TSNode n, const char* source) {

    auto func_type = ts_node_child_by_field_id(n, field_type);
    auto func_decl = ts_node_child_by_field_id(n, field_declarator);
    auto func_body = ts_node_child_by_field_id(n, field_body);

    printf("task %s", body(ts_node_child_by_field_id(func_decl, field_declarator), source).c_str());

    auto func_args = ts_node_child_by_field_id(func_decl, field_parameters);
    visit_chunks(func_args, source, [&](CodeEmitter* ce, TSNode n, const char* source) {

      if (ts_node_symbol(n) == sym_parameter_declaration) {
        emit_function_arg(n, source);
      }
      else {
        emit_leaf(n, source);
      }
      });

    printf(";");

    auto cursor = ts_node_start_byte(func_body);

    for (uint32_t i = 0; i < ts_node_child_count(func_body); i++) {
      auto c = ts_node_child(func_body, i);
      advance_to(cursor, ts_node_start_byte(c), source);
      if (i == 0) {
        // opening bracket
      }
      else if (i == ts_node_child_count(func_body) - 1) {
        // closing bracket
        printf("endtask");
      }
      else {
        emit_statement(c, source);
      }
      cursor = ts_node_end_byte(c);
    }

    advance_to(cursor, ts_node_end_byte(func_body), source);
  }

  //------------------------------------------------------------------------------

  void emit_field_declaration(TSNode n, const char* source) {
    if (ts_node_child_count(n) == 5) {
      auto ftype = ts_node_child_by_field_id(n, field_type);
      auto fdecl = ts_node_child_by_field_id(n, field_declarator);
      auto op = ts_node_child(n, 2);
      auto val = ts_node_child(n, 3);
      auto ftail = ts_node_child(n, 4);

      emit_body(ftype, source);
      emit_gap(ftype, fdecl, source);
      emit_body(fdecl, source);
      emit_gap(fdecl, op, source);
      emit_body(op, source);
      emit_gap(op, val, source);
      emit_expression(val, source);
      emit_body(ftail, source);
    }
    else {
      auto ftype = ts_node_child_by_field_id(n, field_type);
      auto fdecl = ts_node_child_by_field_id(n, field_declarator);
      auto ftail = ts_node_child(n, 2);

      emit_body(ftype, source);
      emit_gap(ftype, fdecl, source);
      emit_body(fdecl, source);
      emit_body(ftail, source);
    }

  }

  //------------------------------------------------------------------------------

  void emit_declaration(TSNode n, const char* source) {
    emit_placeholder(n, source);
  }

  //------------------------------------------------------------------------------

  void emit_class_specifier(TSNode n, const char* source) {
    auto node_class = ts_node_child(n, 0);
    auto node_name = ts_node_child(n, 1);
    auto node_body = ts_node_child(n, 2);

    printf("module %s();", body(node_name, source).c_str());

    auto cursor = ts_node_start_byte(node_body);

    for (uint32_t i = 0; i < ts_node_child_count(node_body); i++) {
      auto c = ts_node_child(node_body, i);
      advance_to(cursor, ts_node_start_byte(c), source);

      if (i == 0) {
        // opening bracket
      }
      else if (i == ts_node_child_count(node_body) - 1) {
        // closing bracket
        printf("endmodule");
      }
      else if (ts_node_symbol(c) == sym_access_specifier) {
        emit_access_specifier(c, source);
      }
      else if (ts_node_symbol(c) == sym_comment) {
        emit_comment(c, source);
      }
      else if (ts_node_symbol(c) == sym_field_declaration) {
        emit_field_declaration(c, source);
      }
      else if (ts_node_symbol(c) == sym_function_definition) {
        emit_function_definition(c, source);
      }
      else {
        emit_placeholder(c, source);
      }

      cursor = ts_node_end_byte(c);
    }

    advance_to(cursor, ts_node_end_byte(n), source);
  }

  //------------------------------------------------------------------------------

  void emit_statement(TSNode n, const char* source) {
    if (ts_node_has_error(n)) {
      emit_error(n, source);
    }
    else if (!ts_node_is_named(n)) {
      emit_leaf(n, source);
    }
    else if (ts_node_symbol(n) == sym_class_specifier) {
      emit_class_specifier(n, source);
    }
    else if (ts_node_symbol(n) == sym_comment) {
      emit_comment(n, source);
    }
    else if (ts_node_symbol(n) == sym_expression_statement) {
      emit_expression_statement(n, source);
    }
    else if (ts_node_symbol(n) == sym_function_definition) {
      emit_function_definition(n, source);
    }
    else if (ts_node_symbol(n) == sym_preproc_if) {
      // FIXME check condition?
    }
    else if (ts_node_symbol(n) == sym_preproc_include) {
      emit_include(n, source);
    }
    else if (ts_node_symbol(n) == sym_return_statement) {
      emit_return_statement(n, source);
    }
    else if (ts_node_symbol(n) == sym_declaration) {
      emit_declaration(n, source);
    }
    else {
      emit_error(n, source);
    }
  }

  //------------------------------------------------------------------------------

  void emit_expression(TSNode n, const char* source) {
    if (!ts_node_is_named(n)) {
      emit_leaf(n, source);
    }
    else if (ts_node_symbol(n) == sym_number_literal) {
      emit_number_literal(n, source);
    }
    else if (ts_node_symbol(n) == sym_identifier) {
      emit_leaf(n, source);
    }
    else if (ts_node_symbol(n) == sym_call_expression) {
      emit_call_expression(n, source);
    }
    else if (ts_node_symbol(n) == sym_binary_expression) {
      emit_binary_expression(n, source);
    }
    else if (ts_node_symbol(n) == sym_assignment_expression) {
      emit_binary_expression(n, source);
    }
    else if (ts_node_symbol(n) == sym_parenthesized_expression) {
      visit_chunks(n, source, &CodeEmitter::emit_expression);
    }
    else if (ts_node_symbol(n) == sym_true) {
      emit_leaf(n, source);
    }
    else {
      emit_placeholder(n, source);
    }
  }

  //------------------------------------------------------------------------------

  void emit_translation_unit(TSNode n, const char* source) {
    printf("// ========= start =========\n\n");

    visit_chunks(n, source, &CodeEmitter::emit_statement);

    printf("\n");
    printf("// =========  end  =========\n");
  }
};
//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  //const char* filename = "src/GateBoyLib/GateBoy.h";
  const char* filename = "src/Metron/parse_test.cpp";

  CodeEmitter e(filename);

  //e.emit({ e.root, 0, 0, ts_node_start_byte(e.root), ts_node_end_byte(e.root), -1 });
  //printf("\n\n\n");

  e.emit_translation_unit(e.root, e.source);

  return 0;
}

//------------------------------------------------------------------------------
