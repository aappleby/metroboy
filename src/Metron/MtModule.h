#pragma once
#include <map>
#include <set>

#include "MtCall.h"
#include "MtEnum.h"
#include "MtField.h"
#include "MtMethod.h"
#include "MtParam.h"
#include "MtSubmod.h"
#include "Platform.h"

struct MtMethod;
struct MtModLibrary;
struct MtSourceFile;
typedef std::vector<uint8_t> blob;
typedef std::set<std::string> name_set;

//------------------------------------------------------------------------------

struct MtModule {
  MtModule(MtSourceFile* source_file, MtTemplateDecl node);
  MtModule(MtSourceFile* source_file, MtClassSpecifier node);

  MtSubmod* get_submod(const std::string& name);
  MtMethod* get_method(const std::string& name);
  MtField* get_output(const std::string& name);
  bool has_submod(const std::string& name);
  bool has_enum(const std::string& name);
  bool has_field(const std::string& name);
  bool has_output(const std::string& name);

  void load_pass1();
  void load_pass2();
  void load_pass3();

  void dump_method_list(std::vector<MtMethod>& methods);
  void dump_call_list(std::vector<MtCall>& calls);
  void dump_banner();
  void dump_deltas();

  MtMethod node_to_method(MtNode n);
  MtCall node_to_call(MtNode n);

  void check_dirty_ticks();
  void check_dirty_tocks();

  //----------

  MtSourceFile* source_file = nullptr;

  std::string mod_name;

  bool load_error = false;

  bool dirty_check_done = false;
  bool dirty_check_fail = false;

  MtClassSpecifier mod_struct;
  MtTemplateDecl mod_template;
  MtTemplateParamList mod_param_list;

  std::vector<MtParam> modparams;
  std::vector<MtParam> localparams;

  std::vector<MtEnum> enums;
  std::vector<MtField> inputs;
  std::vector<MtField> outputs;
  std::vector<MtField> fields;
  std::vector<MtSubmod> submods;

  std::vector<MtMethod> init_methods;
  std::vector<MtMethod> tick_methods;
  std::vector<MtMethod> tock_methods;
  std::vector<MtMethod> task_methods;
  std::vector<MtMethod> func_methods;

  std::vector<MtCall> tick_calls;
  std::vector<MtCall> tock_calls;

  std::map<std::string, std::string> port_map;
};

//------------------------------------------------------------------------------
