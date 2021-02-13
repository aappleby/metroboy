#pragma once
#include "CoreLib/Types.h"

struct Probes {
  Probes();
  void reset_to_cart();
  void probe_wire(int index, const char* signal_name, char s);
  void begin_pass(int phase_tag);
  void end_pass();

  void dump(Dumper& d);

  static const int channel_count = 32;
  static const int sample_count = 32;

  int  phase_cursor;
  char names[channel_count][sample_count];
  char phase_samples[channel_count][sample_count];
  int  phase_tags[sample_count];
};

void probe_wire (int index, const char* signal_name, wire s);
void probe_char(int index, const char* signal_name, char s);

#define PROBE(A, B) probe_wire(A, #B, B)