#pragma once
#include "CoreLib/Types.h"

void audio_init();
void audio_stop();
void audio_post(sample_t in_l_i, sample_t in_r_i);
int audio_queue_size();
extern sample_t spu_ring_buffer[];
extern uint16_t spu_ring_cursor;