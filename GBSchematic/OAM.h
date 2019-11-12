#pragma once

struct OAM {
  void tick();

  bool OAM_CLK;
  bool OAM_ADDR_RENDER;
  bool OAM_ADDR_PARSE;
  bool OAM_ADDR_CPU;
  bool OAM_ADDR_DMA;

  bool OAM_A_CS;
  bool OAM_B_CS;

  bool OAM_A1;
  bool OAM_A2;
  bool OAM_A3;
  bool OAM_A4;
  bool OAM_A5;
  bool OAM_A6;
  bool OAM_A7;

  bool OAM_A_D0;
  bool OAM_A_D1;
  bool OAM_A_D2;
  bool OAM_A_D3;
  bool OAM_A_D4;
  bool OAM_A_D5;
  bool OAM_A_D6;
  bool OAM_A_D7;

  bool OAM_B_D0;
  bool OAM_B_D1;
  bool OAM_B_D2;
  bool OAM_B_D3;
  bool OAM_B_D4;
  bool OAM_B_D5;
  bool OAM_B_D6;
  bool OAM_B_D7;
};

