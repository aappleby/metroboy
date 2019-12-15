#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct SequencerIn {
  bool ROOTCLK_xBxDxFxH;

  bool NEW_LINE;
  bool LCDC_WINEN;
  bool WIN_MATCH_SYNC2;

  bool RENDERING;
  bool RENDERINGn;

  bool VID_RESET4;
  bool VID_RESETn;
  bool WIN_RST_TRIGb;
};

//----------

struct Sequencer {
  bool BG_SEQ_x1x3x5x7;
  bool BG_SEQ_xx23xx67;
  bool BG_SEQ_xxxx4567;

  bool BG_SEQ_5;
  bool BG_SEQ_5n;
  bool BG_SEQ5_SYNC;
  bool BG_SEQ_CLK;

  bool RENDERING_AND_NOT_WIN_TRIG;

  bool BG_SEQ_6;
  bool BG_SEQ_7;
  bool BG_SEQ_8;

  bool BG_SEQ_RSTn;

  bool PUKU;
  bool RYDY;
  bool REG_SOVY;

  bool WIN_RST_SYNC;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
