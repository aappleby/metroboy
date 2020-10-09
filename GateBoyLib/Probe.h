#pragma once
#include "CoreLib/Types.h"

struct Probes {

  Probes();
  void probe(int index, const char* signal_name, char s);
  void begin_pass();
  void end_pass();
  void dump(Dumper& d);

  static const int channel_count = 24;
  static const int sample_count = 96;

  int cursor;
  char names[channel_count][sample_count];
  char samples[channel_count][sample_count];
};

void probe(int index, const char* signal_name, char s);
void dump_probes(Dumper& d);
void clear_probes();