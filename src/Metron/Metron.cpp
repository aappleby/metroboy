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
#include <stdarg.h>

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

struct CodeEmitter;
typedef std::function<void(CodeEmitter* self, Node n)> Visitor1;

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

  blob src_blob;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;
  const char* source = nullptr;
  TSNode root;
  const char* cursor = nullptr;
  FILE* out = stdout;


  const char* field_name(int field_id) {
    return field_id == -1 ? nullptr : ts_language_field_name_for_id(lang, field_id);
  }

  //----------------------------------------

  CodeEmitter(const char* filename) {
    src_blob = load_blob(filename);
    src_blob.push_back(0);

    source = (const char*)src_blob.data();
    cursor = source;
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

  void emit_span(const char* a, const char* b) {
    fwrite(a, 1, b - a, out);
  }

  void emit(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
  }

  void emit_escaped(char s) {
    if      (s == '\n') emit("\\n");
    else if (s == '\r') emit("\\r");
    else if (s == '\t') emit("\\t");
    else if (s == '"')  emit("\\\"");
    else if (s == '\\') emit("\\\\");
    else                emit("%c", s);
  }

  void emit_escaped(const char* source, uint32_t a, uint32_t b) {
    emit("\"");
    for (; a < b; a++) {
      emit_escaped(source[a]);
    }
    emit("\"");
  }

  void emit_escaped(const std::string& s) {
    for (auto c : s) emit_escaped(c);
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
    emit("[%d] ", n.index);

    if (n.field_id != -1) emit("f%d ", n.field_id);
    if (n.symbol() != -1) emit("s%d ", n.symbol());
    if (n.field_id != -1) emit("%s.", field_name(n.field_id));

    if (n.is_branch()) {
      emit("%s: ", n.type());
    }
    else if (n.is_leaf()) {
      emit("%s: ", n.type());
      emit_escaped(source, n.span_a, n.span_b);
    }
    else if (n.is_null()) {
      emit("text: ");
      emit_escaped(source, n.span_a, n.span_b);
    }
    else {
      // Unnamed nodes usually have their node body as their "type",
      // and their symbol is something like "aux_sym_preproc_include_token1"
      emit("lit: ");
      //emit("%d: ", n.symbol());
      //emit("%s: ", n.type());
      emit_escaped(source, n.span_a, n.span_b);
    }

    emit("\n");
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

  //------------------------------------------------------------------------------

  typedef std::function<void(CodeEmitter*, TSNode)> ChunkVisitor;

  void visit_chunks(TSNode n, ChunkVisitor cv) {
    for (const auto& c : n) {
      cv(this, c);
    }
  }

  //------------------------------------------------------------------------------

  const char* start(TSNode n) {
    return &source[ts_node_start_byte(n)];
  }

  const char* end(TSNode n) {
    return &source[ts_node_end_byte(n)];
  }

  void emit_body(TSNode n) {
    emit_span(cursor, end(n));
    cursor = end(n);
  }

  void emit_leaf(TSNode n) {
    assert(!ts_node_is_null(n) && !ts_node_child_count(n));
    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_error(TSNode n) {
    if (ts_node_is_named(n)) {
      emit("XXX %s ", ts_node_type(n));
      emit("XXX");
      emit_body(n);
      emit("XXX");
    }
    else {
      emit("XXXXXXX");
      emit_body(n);
      emit("XXXXXXX");
    }
    emit("\n");
    emit("\n");
    dump({ n, 0, 0, ts_node_start_byte(n), ts_node_end_byte(n), -1 });
    emit("\n");

    cursor = end(n);
  }

  //------------------------------------------------------------------------------

  void emit_placeholder(TSNode n) {
    if (ts_node_is_named(n)) {
      emit("%s ", ts_node_type(n));
      emit("<<<");
      emit_body(n);
      emit(">>>");
    }
    else {
      emit("<<<");
      emit_body(n);
      emit(">>>");
    }

    emit("\n");
    emit("\n");
    dump({ n, 0, 0, ts_node_start_byte(n), ts_node_end_byte(n), -1 });
    emit("\n");

    cursor = end(n);
  }

  //------------------------------------------------------------------------------

  void emit_access_specifier(TSNode n) {
    emit("// ");
    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_comment(TSNode n) {
    emit_body(n);
  }

  //------------------------------------------------------------------------------

  void emit_include(TSNode n) {
    assert(ts_node_symbol(n) == sym_preproc_include);
    auto path = std::string(start(n), end(n));
    static regex rx_trim(R"(\.h)");
    path = std::regex_replace(path, rx_trim, ".sv");
    emit("`include %s\n", path.c_str());
    cursor = end(n);
  }

  //------------------------------------------------------------------------------

  void emit_lvalue(TSNode n) {
    if (ts_node_symbol(n) == sym_identifier) {
      emit_leaf(n);
    }
    else {
      emit_error(n);
    }
  }

  //------------------------------------------------------------------------------

  void emit_rvalue(TSNode n) {
    emit_dispatch(n);
  }

  //------------------------------------------------------------------------------

  void emit_binary_expression(TSNode n) {
    auto exp_lv = ts_node_child(n, 0);
    auto exp_op = ts_node_child(n, 1);
    auto exp_rv = ts_node_child(n, 2);

    emit_lvalue(exp_lv);
    emit_leaf(exp_op);
    emit_rvalue(exp_rv);
  }

  //------------------------------------------------------------------------------

  void emit_call_expression(TSNode n) {

    auto call_func = ts_node_child(n, 0);
    auto call_args = ts_node_child(n, 1);

    emit_leaf(call_func);

    visit_chunks(call_args, &CodeEmitter::emit_dispatch);
  }

  //------------------------------------------------------------------------------

  void emit_number_literal(TSNode n) {
    emit_leaf(n);
  }

  //------------------------------------------------------------------------------

  void emit_return_statement(TSNode n) {
    auto ret_keyword = ts_node_child(n, 0);
    auto ret_expr = ts_node_child(n, 1);
    auto ret_tail = ts_node_child(n, 2);

    emit_leaf(ret_keyword);
    emit_dispatch(ret_expr);
    emit_leaf(ret_tail);
  }

  //------------------------------------------------------------------------------

  void emit_expression_statement(TSNode n) {
    auto statement_body = ts_node_child(n, 0);
    auto statement_tail = ts_node_child(n, 1);

    emit_dispatch(statement_body);
    emit_leaf(statement_tail);
  }

  //------------------------------------------------------------------------------

  void emit_function_arg(TSNode n) {
    auto param_type = ts_node_child_by_field_id(n, field_type);
    auto param_decl = ts_node_child_by_field_id(n, field_declarator);

    emit_leaf(param_type);
    emit_leaf(param_decl);
  }

  //------------------------------------------------------------------------------

  void emit_function_definition(TSNode n) {

    auto func_type = ts_node_child_by_field_id(n, field_type);
    auto func_decl = ts_node_child_by_field_id(n, field_declarator);
    auto func_body = ts_node_child_by_field_id(n, field_body);

    auto task_name = ts_node_child_by_field_id(func_decl, field_declarator);
    emit("task ");
    emit_body(task_name);

    auto func_args = ts_node_child_by_field_id(func_decl, field_parameters);
    visit_chunks(func_args, [&](CodeEmitter* ce, TSNode n) {

      if (ts_node_symbol(n) == sym_parameter_declaration) {
        emit_function_arg(n);
      }
      else {
        emit_leaf(n);
      }
      });

    emit(";");

    for (uint32_t i = 0; i < ts_node_child_count(func_body); i++) {
      auto c = ts_node_child(func_body, i);
      if (i == 0) {
        // opening bracket
      }
      else if (i == ts_node_child_count(func_body) - 1) {
        // closing bracket
        emit("endtask");
      }
      else {
        emit_dispatch(c);
      }
    }
  }

  //------------------------------------------------------------------------------

  void emit_field_declaration(TSNode n) {
    if (ts_node_child_count(n) == 5) {
      auto ftype = ts_node_child_by_field_id(n, field_type);
      auto fdecl = ts_node_child_by_field_id(n, field_declarator);
      auto op = ts_node_child(n, 2);
      auto val = ts_node_child(n, 3);
      auto ftail = ts_node_child(n, 4);

      emit_body(ftype);
      emit_body(fdecl);
      emit_body(op);
      emit_dispatch(val);
      emit_body(ftail);
    }
    else {
      auto ftype = ts_node_child_by_field_id(n, field_type);
      auto fdecl = ts_node_child_by_field_id(n, field_declarator);
      auto ftail = ts_node_child(n, 2);

      emit_body(ftype);
      emit_body(fdecl);
      emit_body(ftail);
    }

  }

  //------------------------------------------------------------------------------

  void emit_declaration(TSNode n) {
    emit_placeholder(n);
  }

  //------------------------------------------------------------------------------
  // class_specifier : class_head '{' [ member_specification ] '}'

  void emit_class_specifier(TSNode n) {
    auto node_class = ts_node_child(n, 0);
    auto node_name = ts_node_child(n, 1);
    auto node_body = ts_node_child(n, 2);

    emit("module ");
    emit_body(node_name);
    emit("(); ");

    auto cursor = ts_node_start_byte(node_body);

    for (uint32_t i = 0; i < ts_node_child_count(node_body); i++) {
      auto c = ts_node_child(node_body, i);

      if (i == 0) {
        // opening bracket
      }
      else if (i == ts_node_child_count(node_body) - 1) {
        // closing bracket
        emit("endmodule");
      }
      else if (ts_node_symbol(c) == sym_access_specifier) {
        emit_access_specifier(c);
      }
      else if (ts_node_symbol(c) == sym_comment) {
        emit_comment(c);
      }
      else if (ts_node_symbol(c) == sym_field_declaration) {
        emit_field_declaration(c);
      }
      else if (ts_node_symbol(c) == sym_function_definition) {
        emit_function_definition(c);
      }
      else {
        emit_placeholder(c);
      }

      cursor = ts_node_end_byte(c);
    }
  }

  //------------------------------------------------------------------------------

  void emit_for_statement(TSNode n) {
    // for (initializer, condition, update) body

    auto node_init = ts_node_child_by_field_id(n, field_initializer);
    auto node_cond = ts_node_child_by_field_id(n, field_condition);
    auto node_update = ts_node_child_by_field_id(n, field_update);
    auto node_body = ts_node_child(n, ts_node_child_count(n) - 1);

    emit_body(node_init);
    emit_body(node_cond);
    emit_body(node_update);
    emit_body(node_body);
  }

  //------------------------------------------------------------------------------

  void emit_dispatch(TSNode n) {
    if (ts_node_has_error(n)) {
      emit_error(n);
    }
    else if (!ts_node_is_named(n)) {
      emit_leaf(n);
    }
    else if (ts_node_symbol(n) == sym_class_specifier) {
      emit_class_specifier(n);
    }
    else if (ts_node_symbol(n) == sym_comment) {
      emit_comment(n);
    }
    else if (ts_node_symbol(n) == sym_expression_statement) {
      emit_expression_statement(n);
    }
    else if (ts_node_symbol(n) == sym_function_definition) {
      emit_function_definition(n);
    }
    else if (ts_node_symbol(n) == sym_preproc_if) {
      // FIXME check preprocessor condition?
    }
    else if (ts_node_symbol(n) == sym_preproc_include) {
      emit_include(n);
    }
    else if (ts_node_symbol(n) == sym_return_statement) {
      emit_return_statement(n);
    }
    else if (ts_node_symbol(n) == sym_declaration) {
      emit_declaration(n);
    }
    else if (ts_node_symbol(n) == sym_for_statement) {
      emit_for_statement(n);
    }
    else if (ts_node_symbol(n) == sym_number_literal) {
      emit_number_literal(n);
    }
    else if (ts_node_symbol(n) == sym_identifier) {
      emit_leaf(n);
    }
    else if (ts_node_symbol(n) == sym_call_expression) {
      emit_call_expression(n);
    }
    else if (ts_node_symbol(n) == sym_binary_expression) {
      emit_binary_expression(n);
    }
    else if (ts_node_symbol(n) == sym_assignment_expression) {
      emit_binary_expression(n);
    }
    else if (ts_node_symbol(n) == sym_parenthesized_expression) {
      visit_chunks(n, &CodeEmitter::emit_dispatch);
    }
    else if (ts_node_symbol(n) == sym_true) {
      emit_leaf(n);
    }
    else {
      emit_error(n);
    }
  }

  //------------------------------------------------------------------------------

  void emit_translation_unit(TSNode n) {
    emit("// ========= start =========\n\n");

    visit_chunks(n, &CodeEmitter::emit_dispatch);

    emit("\n");
    emit("// =========  end  =========\n");
  }
};
//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  //const char* filename = "src/GateBoyLib/GateBoy.h";
  const char* filename = "src/Metron/parse_test.cpp";

  CodeEmitter e(filename);

  //e.emit({ e.root, 0, 0, ts_node_start_byte(e.root), ts_node_end_byte(e.root), -1 });
  //emit("\n\n\n");

  e.emit_translation_unit(e.root);

  return 0;
}

//------------------------------------------------------------------------------
