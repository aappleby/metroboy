#include "GateBoyLib/Probe.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "GateBoyLib/Gates.h"
#include "GateBoyLib/Regs.h"
#include "MetroLib/src/CoreLib/Dumper.h"

#pragma warning(disable: 4996)

thread_local Probes* thread_probes = nullptr;

static char cp_ext(uint8_t state) {
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_DRIVEN) return bit0(state) ? '0' : '1';
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_PULLED) return bit0(state) ? 'v' : '^';
  return 'X';
}

Probes::Probes() {
  reset();
}

void Probes::reset() {
  phase_cursor = 0;
  channel_cursor = 0;
  for (int i = 0; i < channel_count; i++) {
    memset(names[i], 0, 32);
    sprintf(names[i], "<probe %02d>", i);
  }

  memset(phase_samples, '_', channel_count * sample_count);
  memset(phase_tags, '_', sample_count);
}

void Probes::probe_wire(const char* signal_name, char s) {
  int index = channel_cursor++;
  if (index >= channel_count) return;

  strcpy(names[index], signal_name);
  if (s <= 1) {
    phase_samples[index][phase_cursor] = s + 30; //? '#' : '_';
  }
  else {
    phase_samples[index][phase_cursor] = cp_ext(s);
  }
}

void Probes::begin_pass(int phase_tag) {
  CHECK_P(thread_probes == nullptr);
  thread_probes = this;
  phase_cursor = (phase_cursor + 1) % sample_count;
  phase_tags[phase_cursor] = phase_tag;
  channel_cursor = 0;
}

void Probes::end_pass() {
  CHECK_P(thread_probes == this);
  thread_probes = nullptr;
}

void Probes::dump(Dumper& d) const {
  for (int y = 0; y < channel_count; y++) {

    //d("\001%02d %-22s : ", y, names[y]);
    d("\001%-22s : ", names[y]);
    auto s = phase_samples[y];
    for (int x = 0; x < sample_count; x++) {
      int idx = (phase_cursor + x + 1) % sample_count;

      d.add_char(phase_tags[idx] == 0 ? '\004' : '\001');
      d.add_char(s[idx]);
    }
    d.add_char('\001');
    d.add_char('\n');
  }
}

void probe(const char* signal_name, bool s) {
  if (thread_probes) {
    thread_probes->probe_wire(signal_name, s);
  }
}
