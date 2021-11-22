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

#pragma warning(disable:4996)

using namespace std;

typedef std::vector<uint8_t> blob;
typedef std::span<const char> cspan;

extern "C" {
  extern const TSLanguage* tree_sitter_cpp();
}

cspan make_span(TSNode node, const char* src) {
  const char* a = &src[ts_node_start_byte(node)];
  const char* b = &src[ts_node_end_byte(node)];

  while ((a < b) && (isspace(a[0]) || a[0] == '\r'))  a++;
  while ((a < b) && (isspace(b[-1]) || b[-1] == '\r')) b--;
  return cspan(a, b);
}

std::string body(TSNode node, const char* src) {
  auto s = make_span(node, src);
  return std::string(s.begin(), s.end());
}

TSNode child(TSNode node, TSFieldId field_id, TSSymbol sym) {
  auto child = ts_node_child_by_field_id(node, field_id);
  assert(ts_node_symbol(child) == sym);
  return child;
}

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

struct BaseNode {
  BaseNode() {}
  BaseNode(TSNode tsnode, const char* source, int depth) : tsnode(tsnode), source(source), depth(depth) {}

  void set(TSNode node, const char* source, int depth) {
    this->tsnode = node;
    this->source = source;
    this->depth = depth;
  }

  void set(TSNode node, const char* source, int depth, TSSymbol symbol) {
    assert(ts_node_symbol(node) == symbol);
    this->tsnode = node;
    this->source = source;
    this->depth = depth;
  }

  void set(BaseNode node, TSSymbol symbol) {
    assert(node.symbol() == symbol);
    this->tsnode = node.tsnode;
    this->source = node.source;
    this->depth = node.depth;
  }

  TSSymbol symbol() const {
    return ts_node_symbol(tsnode);
  }

  BaseNode field(TSFieldId field_id) const {
    auto child = ts_node_child_by_field_id(tsnode, field_id);
    return { child, source, depth + 1 };
  }

  BaseNode field(TSFieldId field_id, TSSymbol sym) const {
    auto child = ts_node_child_by_field_id(tsnode, field_id);
    auto csym = ts_node_symbol(child);

    if (csym != sym) {
      printf("%d %d\n", csym, sym);
    }

    assert(ts_node_symbol(child) == sym);
    return BaseNode(child, source, depth + 1);
  }

  TSNode tsnode = {};
  const char* source = nullptr;
  int depth = 0;

protected:

  std::vector<BaseNode> children() const {
    std::vector<BaseNode> result;
    auto child_count = ts_node_child_count(tsnode);
    for (uint32_t i = 0; i < child_count; i++) {
      auto child = BaseNode(ts_node_child(tsnode, i), source, depth + 1);
      result.push_back(child);
    }
    return result;
  }
};

//------------------------------------------------------------------------------

struct CommentNode : public BaseNode {
  CommentNode(const BaseNode& node) { set(node, sym_comment); }
  void dump() {
    printf("Comment node %s\n", body(tsnode, source).c_str());
  }
};

//------------------------------------------------------------------------------

struct IdentifierNode : public BaseNode {
  IdentifierNode(const BaseNode& node) { set(node, sym_identifier); }
  void dump() {
    printf("IdentifierNode %s\n", body(tsnode, source).c_str());
  }
};

//------------------------------------------------------------------------------

struct TranslationUnitNode : public BaseNode {
  TranslationUnitNode(const BaseNode& node) { set(node, sym_translation_unit); }
  
  TranslationUnitNode(TSNode root, const char* source, int depth) {
    this->tsnode = root;
    this->source = source;
    this->depth = depth;
  }

  using BaseNode::children;
};

//------------------------------------------------------------------------------

struct PreprocArgNode : public BaseNode {
  PreprocArgNode(const BaseNode& node) { set(node, sym_preproc_arg); }
};

//------------------------------------------------------------------------------
// preproc_def : "#define", name:identifier, value:preproc_arg, "",

struct PreprocDefNode : public BaseNode {
  PreprocDefNode(const BaseNode& node) {
    set(node, sym_preproc_def);
    //_name = new IdentifierNode(field(field_name, sym_identifier));
    //_value = new PreprocArgNode(field(field_value, sym_preproc_arg));
  }

  ~PreprocDefNode() {
    //delete _name;
    //delete _value;
  }

  void dump() {
    printf("PreprocDefNode %s\n", body(tsnode, source).c_str());
  }

  //IdentifierNode* _name = nullptr;
  //PreprocArgNode* _value = nullptr;
};

//------------------------------------------------------------------------------
// preproc_params : "(", identifier, ")",

struct PreprocParamsNode : public BaseNode {
  PreprocParamsNode(const BaseNode& node) { set(node, sym_preproc_params); }
};

//------------------------------------------------------------------------------
// preproc_function_def : "#define", name:identifier, parameters:preproc_params, value:preproc_arg, "",

struct PreprocFunctionDefNode : public BaseNode {
  PreprocFunctionDefNode(const BaseNode& node) { set(node, sym_preproc_function_def); }

  void dump() {
    printf("PreprocFunctionDefNode %s\n", body(tsnode, source).c_str());
  }

  IdentifierNode    name()   const { return IdentifierNode(field(field_name, sym_identifier)); }
  PreprocParamsNode params() const { return PreprocParamsNode(field(field_name, sym_preproc_params)); }
};

//------------------------------------------------------------------------------

struct IncludeNode : public BaseNode {
  IncludeNode(const BaseNode& node) { set(node, sym_preproc_include); }

  void dump() {
    printf("IncludeNode::path %s\n", path().c_str());
  }

  std::string path() const { return body(ts_node_child_by_field_id(tsnode, field_path), source); }
};

//------------------------------------------------------------------------------

struct TypeIdNode : public BaseNode {
  TypeIdNode(const BaseNode& node) { set(node, alias_sym_type_identifier); }

  void dump() {
    printf("TypeIdNode %.40s\n", ::body(tsnode, source).c_str());
  }
};

//------------------------------------------------------------------------------

struct QualIDNode : public BaseNode {
  QualIDNode(const BaseNode& node) { set(node, sym_qualified_identifier); }
};

//------------------------------------------------------------------------------

struct ParamListNode : public BaseNode {
  ParamListNode(const BaseNode& node) { set(node, sym_parameter_list); }
};

//------------------------------------------------------------------------------

struct DeclNode : public BaseNode {
  DeclNode(const BaseNode& node) { set(node, sym_declaration); }

  void dump() {
    printf("DeclNode %.40s...\n", body(tsnode, source).c_str());
  }
};


//------------------------------------------------------------------------------
// function_declarator : declarator:qualified_identifier, parameters:parameter_list, type_qualifier,

struct FuncDeclNode : public BaseNode {
  FuncDeclNode(const BaseNode& node) { set(node, sym_function_declarator); }

  void dump() {
    printf("FuncDeclNode %.40s\n", ::body(tsnode, source).c_str());
  }


  QualIDNode    decl()   const { return QualIDNode(field(field_declarator, sym_qualified_identifier)); }
  ParamListNode params() const { return ParamListNode(field(field_parameters, sym_parameter_list)); }
};

//------------------------------------------------------------------------------

struct CompoundNode : public BaseNode {
  CompoundNode(const BaseNode& node) { set(node, sym_compound_statement); }

  void dump() {
    printf("CompoundNode %.40s\n", ::body(tsnode, source).c_str());
  }
};

//------------------------------------------------------------------------------

struct FuncDefNode : public BaseNode {
  FuncDefNode(const BaseNode& node) { set(node, sym_function_definition); }

  void dump() {
    printf("FuncDefNode %.40s\n", ::body(tsnode, source).c_str());
    //printf("  "); type().dump();
    printf("  "); decl().dump();
    printf("  "); body().dump();
  }

  // sym_primitive_type = 78,
  TypeIdNode   type() const { return TypeIdNode(field(field_type, alias_sym_type_identifier)); }
  FuncDeclNode decl() const { return FuncDeclNode(field(field_declarator, sym_function_declarator)); }
  CompoundNode body() const { return CompoundNode(field(field_body, sym_compound_statement)); }
};

//------------------------------------------------------------------------------


blob load_blob(const char* filename) {
  FILE* f = fopen(filename, "rb");
  assert(f);

  blob result;
  fseek(f, 0, SEEK_END);
  result.resize(ftell(f));
  fseek(f, 0, SEEK_SET);

  fread(result.data(), 1, result.size(), f);
  fclose(f);
  return result;
}


int count_comments(TSNode root, const char* source) {
  deque<TSNode> queue;
  queue.push_back(root);

  int total = 0;

  while (!queue.empty()) {
    TSNode node = queue.back();
    queue.pop_back();

    if (ts_node_symbol(node) == sym_comment) {
      string s = body(node, source);

      smatch match;
      static regex rx_extract_tag(R"(^\/\*(.*?)\*\/)");
      if (regex_search(s, match, rx_extract_tag)) {
        //printf("%s\n", s.c_str());
        total++;
      }
      else {
        printf("%s\n", s.c_str());
      }

    }

    uint32_t child_count = ts_node_child_count(node);
    for (uint32_t i = 0; i < child_count; i++) {
      queue.push_back(ts_node_child(node, i));
    }
  }

  return total;
}



typedef std::function<void(BaseNode)> base_node_callback;

void traverse(BaseNode node, base_node_callback c) {
  c(node);
  uint32_t child_count = ts_node_child_count(node.tsnode);
  for (uint32_t i = 0; i < child_count; i++) {
    BaseNode child = { ts_node_child(node.tsnode, i), node.source, node.depth + 1 };
    traverse(child, c);
  }
}


/*
array_declarator : declarator, (null), (null),
assignment_expression : left, operator, right,
binary_expression : left, operator, right,
call_expression : function, arguments,
cast_expression : (null), type, (null), value,
condition_clause : (null), value, (null),
conditional_expression : condition, (null), consequence, (null), alternative,
declaration : type, declarator, (null),
declaration : type, declarator, (null), (null),
field_expression : argument, operator, field,
for_statement : (null), (null), initializer, condition, (null), update, (null), (null),
function_declarator : declarator, parameters,
function_declarator : declarator, parameters, (null),
function_definition : type, declarator, body,
if_statement : (null), condition, consequence,
if_statement : (null), condition, consequence, (null), alternative,
init_declarator : declarator, (null), value,
parameter_declaration : type, declarator,
parameter_declaration : type, declarator, (null),
pointer_expression : operator, argument,
preproc_def : (null), name, value, (null),
preproc_function_def : (null), name, parameters, value, (null),
preproc_if : (null), condition, (null), (null), (null), (null),
preproc_if : (null), condition, (null), (null), (null), (null), (null), (null), (null),
preproc_if : (null), condition, (null), (null), (null), (null), (null), (null), (null), (null), (null), (null),
preproc_include : (null), path, (null),
qualified_identifier : scope, name, (null),
sizeof_expression : (null), value,
subscript_expression : argument, (null), index, (null),
type_descriptor : type,
unary_expression : operator, argument,
update_expression : argument, operator,
while_statement : (null), condition, body,
*/


int dump_fields(TSNode root, const char* source) {
  deque<TSNode> queue;
  queue.push_back(root);

  int total = 0;

  while (!queue.empty()) {
    TSNode node = queue.back();
    queue.pop_back();

    printf("%s : ", ts_node_type(node));

    uint32_t child_count = ts_node_child_count(node);

    if (child_count) {
      for (uint32_t i = 0; i < child_count; i++) {
        auto child = ts_node_child(node, i);

        const char* field = ts_node_field_name_for_child(node, i);
        if (field) {
          printf("%s:", field);
        }

        if (ts_node_is_named(child)) {
          printf("%s, ", ts_node_type(child));
        }
        else if (ts_node_is_null(child)) {
          printf("\"nul\", ");
        }
        else {
          printf("\"%s\", ", body(child, source).c_str());
        }

        queue.push_back(child);
      }
    }
    else {
      printf("\"%s\", ", body(node, source).c_str());
    }

    printf("\n");
  }

  return total;
}










void print_source(TSNode node, const char* source, int max_len) {
  max_len = 999999999;
  const char* old_a = &source[ts_node_start_byte(node)];
  const char* old_b = &source[ts_node_end_byte(node)];
  const char* a = old_a;
  const char* b = old_b;

  while ((isspace(a[0]) || a[0] == '\r') && (a < b)) {
    a++;
  }
  while ((isspace(b[-1]) || b[-1] == '\r') && (a < b)) {
    b--;
  }
  auto len = (b - a) > max_len ? max_len : b - a;
  if (len > 0) {
    fwrite(a, len, 1, stdout);
  }
  else {
    fwrite("<whitespace>", 12, 1, stdout);
  }
}

bool any_newlines(TSNode node, const char* src) {
  for (const char* a = &src[ts_node_start_byte(node)];
    a != &src[ts_node_end_byte(node)];
    a++) {
    if (*a == '\n') return true;
  }
  return false;
}

bool dont_print_source(TSNode node) {
  return
    ts_node_symbol(node) == sym_translation_unit ||
    ts_node_symbol(node) == sym_class_specifier ||
    ts_node_symbol(node) == sym_field_declaration_list ||
    ts_node_symbol(node) == sym_function_definition ||
    ts_node_symbol(node) == sym_compound_statement ||
    ts_node_symbol(node) == sym_while_statement ||
    ts_node_symbol(node) == sym_if_statement ||
    ts_node_symbol(node) == sym_for_statement ||
    ts_node_symbol(node) == sym_declaration;
}


void print(TSNode node, const char* source, int depth, const char* field_name = nullptr) {
  printf("%04d: ", ts_node_symbol(node));
  for (int i = 0; i < depth; i++) printf("|  ");
  if (field_name) {
    printf("%s: ", field_name);
  }

  if (ts_node_is_named(node)) {
    printf("%s ", ts_node_type(node));
  }
  else {
    printf("lit ");
  }

  if (!any_newlines(node, source)) {
    printf("'");
    print_source(node, source, 40);
    printf("'");
  }
  printf("\n");
}

void dump(TSNode node, const char* source, int depth = 0, const char* field_name = nullptr) {

  if (ts_node_symbol(node) == sym_function_definition) {
    print(node, source, depth, field_name);
  }
  for (uint32_t i = 0; i < ts_node_child_count(node); i++) {
    auto child = ts_node_child(node, i);
    auto field = ts_node_field_name_for_child(node, i);

    dump(child, source, depth + 1, field);
  }
}











void check_nesting(TSNode n) {
  auto begin = ts_node_start_byte(n);
  auto end = ts_node_end_byte(n);
  auto child_count = ts_node_child_count(n);

  printf("%d-%d\n", begin, end);

  for (uint32_t i = 0; i < child_count; i++) {
    auto c = ts_node_child(n, i);
    auto cbegin = ts_node_start_byte(c);
    auto cend = ts_node_end_byte(c);
    printf("  %d-%d\n", cbegin, cend);
  }
}

void emit(TSNode n, const char* source) {
  auto cursor = ts_node_start_byte(n);
  auto child_count = ts_node_child_count(n);

  for (uint32_t i = 0; i < child_count; i++) {
    auto c = ts_node_child(n, i);
    auto cbegin = ts_node_start_byte(c);

    if (cursor < cbegin) {
      fwrite(source + cursor, 1, cbegin - cursor, stdout);
      cursor = cbegin;
    }

    emit(c, source);
    cursor = ts_node_end_byte(c);
  }

  auto end = ts_node_end_byte(n);
  if (cursor < end) {
    fwrite(source + cursor, 1, end - cursor, stdout);
    cursor = end;
  }
}



















int main(int argc, char** argv) {
  //const char* filename = "src/Metron/parse_test.cpp";
  const char* filename = "src/GateBoyLib/GateBoy.cpp";

  auto src_blob = load_blob(filename);
  src_blob.push_back(0);
  const char* source = (const char*)src_blob.data();

  auto parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_cpp());
  TSTree* tree = ts_parser_parse_string(parser, NULL, (const char*)src_blob.data(), (uint32_t)src_blob.size());
  
  //check_nesting(ts_tree_root_node(tree));

  /*
  TranslationUnitNode root(ts_tree_root_node(tree), source, 0);

  auto children = root.children();

  for (auto& c : children) {
    switch (c.symbol()) {
    case sym_preproc_include:
      IncludeNode(c).dump();
      break;
    case sym_comment:
      CommentNode(c).dump();
      break;
    case sym_identifier:
      IdentifierNode(c).dump();
      break;
    case sym_preproc_def:
      PreprocDefNode(c).dump();
      break;
    case sym_preproc_function_def:
      PreprocFunctionDefNode(c).dump();
      break;
    case sym_declaration:
      DeclNode(c).dump();
      break;
    case sym_function_definition:
      FuncDefNode(c).dump();
      break;
    default:
      printf("##### %s\n", ts_node_type(c.tsnode));
      break;
    }
  }
  */

  /*
  traverse(root, [](BaseNode node) {
    if (node.depth == 1) {
      printf("%s\n", ts_node_type(node));
    }
  });
  */

  //printf("==========\n");
  //printf("%s\n", source + 56555);
  //printf("==========\n");


  emit(ts_tree_root_node(tree), source);


  
  ts_parser_delete(parser);
  return 0;
}

