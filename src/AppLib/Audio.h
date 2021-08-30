#pragma once
#include "CoreLib/Types.h"

void audio_init();
void audio_stop();
void audio_begin();
void audio_post(sample_t in_l_i, sample_t in_r_i);
void audio_end();
