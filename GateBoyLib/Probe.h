#pragma once
#include "CoreLib/Types.h"

void probe(const char* signal_name, wire s);
void dump_probes(Dumper& d);
void clear_probes();