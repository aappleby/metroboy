#pragma once
#include "CoreLib/Types.h"

struct Probes {
  Probes();
  void probe(int index, const char* signal_name, char s);
  void begin_pass(int pass_count);
  void end_pass(bool stable);

  void dump(Dumper& d, bool draw_passes = true);

  static const int channel_count = 24;
  static const int sample_count = 96;

  int  pass_cursor;
  int  phase_cursor;
  char names[channel_count][sample_count];
  char pass_samples[channel_count][sample_count];
  char phase_samples[channel_count][sample_count];
  char stable[sample_count];
};

void probe(int index, const char* signal_name, char s);


#define PROBE(A, B) probe(A, #B, B)