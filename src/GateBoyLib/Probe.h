#pragma once
#include "CoreLib/Types.h"

struct Probes {
  Probes();
  void reset();

  void probe_wire(const char* signal_name, char s);
  void begin_pass(int phase_tag);
  void end_pass();

  void dump(Dumper& d) const;

  static const int channel_count = 32;
  static const int sample_count = 40;

  int  phase_cursor;
  int  channel_cursor;
  char names[channel_count][sample_count];
  char phase_samples[channel_count][sample_count];
  int  phase_tags[sample_count];
};

void probe(const char* signal_name, wire s);
