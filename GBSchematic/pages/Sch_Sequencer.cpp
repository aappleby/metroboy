#include "Schematics.h"
#include <memory.h>

namespace Schematics {

//-----------------------------------------------------------------------------

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

  bool ALET_AxCxExGx;
  bool MYVO_xBxDxFxH;

  bool WIN_RST_SYNC;
};

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

void Sequencer_tick(const SequencerIn& in, const Sequencer& a, const Sequencer& b, Sequencer& c) {

  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(in.ROOTCLK_xBxDxFxH);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
  /*p01.ALET*/ c.ALET_AxCxExGx = not(ZEME_xBxDxFxH);
  /*p27.MYVO*/ c.MYVO_xBxDxFxH = not(c.ALET_AxCxExGx);

  /*p27.XAHY*/ wire NEW_LINEn = not(in.NEW_LINE);
  /*p27.XOFO*/ wire MAP_X_RST = nand(in.LCDC_WINEN, NEW_LINEn, in.VID_RESETn);
  /*p27.PYNU*/ wire WIN_RST = or(in.WIN_MATCH_SYNC2, MAP_X_RST);

  /*p27.NOPA*/ c.WIN_RST_SYNC = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, in.VID_RESETn, b.WIN_RST_SYNC, WIN_RST);
  /*p27.NUNY*/ wire WIN_RST_TRIG = and(!b.WIN_RST_SYNC, WIN_RST);
  
  /*p27.PUKU*/ c.PUKU = nor(b.RYDY, WIN_RST_TRIG);
  /*p27.RYDY*/ c.RYDY = nor(b.PUKU, in.VID_RESET4, b.BG_SEQ_7);


  /*p27.SOVY*/ c.REG_SOVY = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, 1, b.REG_SOVY, b.RYDY);

  /*p27.NYXU*/ c.BG_SEQ_RSTn = or(and(b.REG_SOVY, not(b.RYDY)),
                                  and(in.RENDERING, !b.BG_SEQ_8, b.BG_SEQ_6, b.BG_SEQ_7));

  /*p27.MOCE*/ c.BG_SEQ_5n = nand(b.BG_SEQ_x1x3x5x7, b.BG_SEQ_xxxx4567, b.BG_SEQ_RSTn);
  /*p27.LYRY*/ c.BG_SEQ_5 = not(b.BG_SEQ_5n);

  /*p27.LEBO*/ c.BG_SEQ_CLK      = nand(b.ALET_AxCxExGx, b.BG_SEQ_5n);
  /*p27.LAXU*/ c.BG_SEQ_x1x3x5x7 = tock_pos( a.BG_SEQ_CLK,       b.BG_SEQ_CLK,      b.BG_SEQ_RSTn, b.BG_SEQ_x1x3x5x7, !b.BG_SEQ_x1x3x5x7);
  /*p27.MESU*/ c.BG_SEQ_xx23xx67 = tock_pos(!a.BG_SEQ_x1x3x5x7, !b.BG_SEQ_x1x3x5x7, b.BG_SEQ_RSTn, b.BG_SEQ_xx23xx67, !b.BG_SEQ_xx23xx67);
  /*p27.NYVA*/ c.BG_SEQ_xxxx4567 = tock_pos(!a.BG_SEQ_xx23xx67, !b.BG_SEQ_xx23xx67, b.BG_SEQ_RSTn, b.BG_SEQ_xxxx4567, !b.BG_SEQ_xxxx4567);

  /*p24.NAFY*/ c.RENDERING_AND_NOT_WIN_TRIG = nor(in.WIN_RST_TRIGb, in.RENDERINGn);

  /*p27.LOVY*/ c.BG_SEQ5_SYNC = tock_pos(a.MYVO_xBxDxFxH, b.MYVO_xBxDxFxH, b.BG_SEQ_RSTn,                b.BG_SEQ5_SYNC, b.BG_SEQ_5);
  /*p24.NYKA*/ c.BG_SEQ_6     = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, b.RENDERING_AND_NOT_WIN_TRIG, b.BG_SEQ_6,     b.BG_SEQ_5);
  /*p24.PORY*/ c.BG_SEQ_7     = tock_pos(a.MYVO_xBxDxFxH, b.MYVO_xBxDxFxH, b.RENDERING_AND_NOT_WIN_TRIG, b.BG_SEQ_7,     b.BG_SEQ_6);
  /*p24.PYGO*/ c.BG_SEQ_8     = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, in.RENDERING,                 b.BG_SEQ_8,     b.BG_SEQ_7);
}

void Sequencer_test() {
  Sequencer a = {};
  Sequencer b = {};
  Sequencer c = {};

  a.PUKU = 1;
  b.PUKU = 1;
  c.PUKU = 1;

  SequencerIn in = {
    .ROOTCLK_xBxDxFxH = 0,
    .RENDERING = 1,
    .RENDERINGn = 0,
    .WIN_RST_TRIGb = 0,
  };

  for (int i = 0; i < 60; i++) {
    printf("phase %2d clk:%c seq0:%c seq1:%c seq2:%c seq5:%c seq6:%c seq7:%c seq8:%c puku:%c rydy:%c\n",
           i,
           in.ROOTCLK_xBxDxFxH ? '#' : ' ',
           b.BG_SEQ_x1x3x5x7 ? '#' : ' ',
           b.BG_SEQ_xx23xx67 ? '#' : ' ',
           b.BG_SEQ_xxxx4567 ? '#' : ' ',
           b.BG_SEQ_5 ? '#' : ' ',
           b.BG_SEQ_6 ? '#' : ' ',
           b.BG_SEQ_7 ? '#' : ' ',
           b.BG_SEQ_8 ? '#' : ' ',
           b.PUKU ? '#' : ' ',
           b.RYDY ? '#' : ' '
    );

    for (int j = 0; j < 20; j++) {
      Sequencer_tick(in, a, b, c);
      a = b;
      b = c;
    }

    in.ROOTCLK_xBxDxFxH = !in.ROOTCLK_xBxDxFxH;
  }

}

//-----------------------------------------------------------------------------

}; // namespace Schematics

int main(int /*argc*/, char** /*argv*/) {
  Schematics::Sequencer_test();
}