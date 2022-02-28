#pragma once
#include "Platform.h"

#include "MtNode.h"
#include <set>
#include <map>

struct MtModLibrary;
typedef std::vector<uint8_t> blob;

//------------------------------------------------------------------------------

struct MtMethod : public MtNode {
  std::string name;
  std::vector<std::string> params;
};

struct MtSubmod : public MtNode {
  std::string name;
  MtModule* mod;
};

struct MtCall : public MtNode {
  MtSubmod* submod;
  MtMethod* method;
  std::vector<std::string> args;
};

struct MtEnum : public MtNode {
  MtEnum(MtNode n) : MtNode(n) {

    if (n.sym == sym_field_declaration) {
      auto enum_type = n.get_field(field_type);
      auto enum_name = enum_type.get_field(field_name);
      name = enum_name.text();
    }
    else {
      n.dump_tree();
      debugbreak();
    }
  }

  std::string name;
};

struct MtParam : public MtNode {
  MtParam(MtNode n) : MtNode(n) {
    assert(sym == sym_parameter_declaration ||
           sym == sym_optional_parameter_declaration || 
           sym == sym_field_declaration);

    if (sym == sym_parameter_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else if (sym == sym_optional_parameter_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else if (sym == sym_field_declaration) {
      name = n.get_field(field_declarator).text();
    }
    else {
      n.dump_tree();
      debugbreak();
    }
  }
  std::string name;
};

struct MtField : public MtNode {
  MtField(MtNode n, std::string _type_name, std::string _name)
    : MtNode(n), name(_name), type_name(_type_name) {

    for (auto c : (MtNode)*this) {
      if (c.sym == sym_storage_class_specifier) {
        if (c.match("static")) is_static = true;;
      }
      if (c.sym == sym_type_qualifier) {
        if (c.match("const")) is_const = true;
      }
    }
  }

  std::string name;
  std::string type_name;
  int index = 0; // if this is part of a multi-decl, index of the sub-decl. not hooked up yet.
  bool is_static = false;
  bool is_const = false;
};

//------------------------------------------------------------------------------

struct MtModule {

  MtModule();
  ~MtModule();

  void load_pass1(const char* full_path, blob& src_blob);
  void load_pass2();
  void load_pass3();

  void dump_banner();
  void dump_method_list(std::vector<MtMethod>& methods);
  void dump_call_list(std::vector<MtCall>& calls);

  MtMethod node_to_method(MtNode n);
  MtCall   node_to_submod_call(MtNode n);

  // Rule checker

  void check_dirty_ticks();
  void check_dirty_tocks();
  void check_dirty_read(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_write(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_dispatch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_assign(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_if(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_call(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);
  void check_dirty_switch(MtNode n, bool is_seq, std::set<MtField>& dirty_fields, int depth);

  MtSubmod* get_submod(const std::string& name) {
    for (auto& n : submods) {
      if (n.name == name) return &n;
    }
    return nullptr;
  }

  MtMethod* get_method(const std::string& name) {
    for (auto& n : tick_methods) if (n.name == name) return &n;
    for (auto& n : tock_methods) if (n.name == name) return &n;
    return nullptr;
  }

  MtField* get_output(const std::string& name) {
    for (auto& n : outputs) if (n.name == name) return &n;
    return nullptr;
  }

  //----------

  MtModLibrary* lib;
  std::string full_path;

  blob src_blob;
  bool use_utf8_bom = false;

  const char* source = nullptr;
  const char* source_end = nullptr;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;

  std::string mod_name;

  MtTranslationUnit   mod_root;
  MtStructSpecifier   mod_struct;
  MtTemplateDecl      mod_template;
  MtTemplateParamList mod_param_list;

  std::vector<MtParam>  modparams;
  std::vector<MtParam>  localparams;

  std::vector<MtEnum>   enums;
  std::vector<MtField>  inputs;
  std::vector<MtField>  outputs;
  std::vector<MtField>  fields;
  std::vector<MtSubmod> submods;

  std::vector<MtMethod> init_methods;
  std::vector<MtMethod> tick_methods;
  std::vector<MtMethod> tock_methods;
  std::vector<MtMethod> task_methods;
  std::vector<MtMethod> func_methods;
  
  std::vector<MtCall>   tick_calls;
  std::vector<MtCall>   tock_calls;

  std::map<std::string, std::string> port_map;
};

//------------------------------------------------------------------------------
