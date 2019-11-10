#pragma once

struct P17_WaveRam {
  union { bool BUTU; bool BUTU_512K; };
  union { bool DATE; bool WAVE_PLAY_D0; };
  union { bool DAZY; bool WAVE_PLAY_D1; };
  union { bool CUZO; bool WAVE_PLAY_D2; };
  union { bool COPO; bool WAVE_PLAY_D3; };
  union { bool CAZU; bool WAVE_RAM_RDn; };
};
