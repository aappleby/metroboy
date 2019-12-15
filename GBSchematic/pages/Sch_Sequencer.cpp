#include "Sch_Sequencer.h"
#include "Schematics.h"
#include <memory.h>

namespace Schematics {

//-----------------------------------------------------------------------------

void Sequencer_tick(const SequencerIn& in,
                    const Sequencer& a,
                    const Sequencer& b,
                    Sequencer& next) {
  (void)in;
  (void)a;
  (void)b;
  (void)next;

#if 0
  /*p27.XAHY*/ wire NEW_LINEn = not(in.NEW_LINE);
  /*p27.XOFO*/ wire MAP_X_RST = nand(in.LCDC_WINEN, NEW_LINEn, in.VID_RESETn);
  /*p27.PYNU*/ wire WIN_RST = or(in.WIN_MATCH_SYNC2, MAP_X_RST);

  /*p27.NOPA*/ next.WIN_RST_SYNC = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, in.VID_RESETn, b.WIN_RST_SYNC, WIN_RST);
  /*p27.NUNY*/ wire WIN_RST_TRIG = and(!b.WIN_RST_SYNC, WIN_RST);
  
  /*p27.PUKU*/ next.PUKU = nor(b.RYDY, WIN_RST_TRIG);
  /*p27.RYDY*/ next.RYDY = nor(b.PUKU, in.VID_RESET4, b.BG_SEQ_7);

  /*p27.SOVY*/ next.REG_SOVY = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, 1, b.REG_SOVY, b.RYDY);

  /*p27.NYXU*/ next.BG_SEQ_RSTn = or(and(b.REG_SOVY, not(b.RYDY)),
                                  and(in.RENDERING, !b.BG_SEQ_8, b.BG_SEQ_6, b.BG_SEQ_7));

  /*p27.MOCE*/ next.BG_SEQ_5n = nand(b.BG_SEQ_x1x3x5x7, b.BG_SEQ_xxxx4567, b.BG_SEQ_RSTn);
  /*p27.LYRY*/ next.BG_SEQ_5 = not(b.BG_SEQ_5n);

  /*p27.LEBO*/ next.BG_SEQ_CLK      = nand(b.ALET_AxCxExGx, b.BG_SEQ_5n);
  /*p27.LAXU*/ next.BG_SEQ_x1x3x5x7 = tock_pos( a.BG_SEQ_CLK,       b.BG_SEQ_CLK,      b.BG_SEQ_RSTn, b.BG_SEQ_x1x3x5x7, !b.BG_SEQ_x1x3x5x7);
  /*p27.MESU*/ next.BG_SEQ_xx23xx67 = tock_pos(!a.BG_SEQ_x1x3x5x7, !b.BG_SEQ_x1x3x5x7, b.BG_SEQ_RSTn, b.BG_SEQ_xx23xx67, !b.BG_SEQ_xx23xx67);
  /*p27.NYVA*/ next.BG_SEQ_xxxx4567 = tock_pos(!a.BG_SEQ_xx23xx67, !b.BG_SEQ_xx23xx67, b.BG_SEQ_RSTn, b.BG_SEQ_xxxx4567, !b.BG_SEQ_xxxx4567);

  /*p24.NAFY*/ next.RENDERING_AND_NOT_WIN_TRIG = nor(in.WIN_RST_TRIGb, in.RENDERINGn);

  /*p27.LOVY*/ next.BG_SEQ5_SYNC = tock_pos(a.MYVO_xBxDxFxH, b.MYVO_xBxDxFxH, b.BG_SEQ_RSTn,                b.BG_SEQ5_SYNC, b.BG_SEQ_5);
  /*p24.NYKA*/ next.BG_SEQ_6     = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, b.RENDERING_AND_NOT_WIN_TRIG, b.BG_SEQ_6,     b.BG_SEQ_5);
  /*p24.PORY*/ next.BG_SEQ_7     = tock_pos(a.MYVO_xBxDxFxH, b.MYVO_xBxDxFxH, b.RENDERING_AND_NOT_WIN_TRIG, b.BG_SEQ_7,     b.BG_SEQ_6);
  /*p24.PYGO*/ next.BG_SEQ_8     = tock_pos(a.ALET_AxCxExGx, b.ALET_AxCxExGx, in.RENDERING,                 b.BG_SEQ_8,     b.BG_SEQ_7);
#endif
}

//-----------------------------------------------------------------------------

void Sequencer_test() {
  Sequencer a = {};
  Sequencer b = {};
  Sequencer next = {};

  a.PUKU = 1;
  b.PUKU = 1;
  next.PUKU = 1;

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
      Sequencer_tick(in, a, b, next);
      a = b;
      b = next;
    }

    in.ROOTCLK_xBxDxFxH = !in.ROOTCLK_xBxDxFxH;
  }

}

//-----------------------------------------------------------------------------

}; // namespace Schematics

#if 0
int main(int /*argc*/, char** /*argv*/) {
  Schematics::Sequencer_test();
}
#endif