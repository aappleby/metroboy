#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct OamSignals;
struct PixelPipeSignals;

//-----------------------------------------------------------------------------
// Internal VRAM bus

struct VramBus {
  static VramBus tick(const LcdSignals& lcd_sig,
                      const VidRegisters2& vid_regs,
                      const SpriteSignals& spr_sig,
                      const DmaSignals& dma_sig,
                      const OamSignals& oam_sig,
                      const VidSignals& vid_sig,
                      const PixelPipeSignals& pix_sig,
                      const JoypadPins& joy_pins,
                      const SpriteTristate& spr_tri,
                      const BusTristates& bus_tri);

  bool MA00;
  bool MA01;
  bool MA02;
  bool MA03;
  bool MA04;
  bool MA05;
  bool MA06;
  bool MA07;
  bool MA08;
  bool MA09;
  bool MA10;
  bool MA11;
  bool MA12;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;
};

//-----------------------------------------------------------------------------

};