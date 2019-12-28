#include "../pages/Sch_Common.h"

using namespace Schematics;

void plot(int x, int y, bool v);
void printAt(int x, int y, const char *format, ...);

struct SpriteSeq {
  /*p01.LAPE*/ bool LAPE;
  /*p27.TAVA*/ bool TAVA;

  /*p29.TOXE*/ Reg SEQ_B0;
  /*p29.TULY*/ Reg SEQ_B1;
  /*p29.TESE*/ Reg SEQ_B2;

  /*p29.TOBU*/ Reg SEQ_3;
  /*p29.VONU*/ Reg SPRITE_AB;
  /*p29.SEBA*/ Reg SEQ_4d;

  /*p29.TYFO*/ Reg SEQ_B0d;

  /*p29.TOMA*/ bool SEQ_CLK;
  /*p27.SECA*/ bool SEQ_RST;
  /*p29.SYCU*/ bool SYCU;
  /*p29.XADO*/ bool SPRITE_VRAM_LATCH_A;
  /*p29.PUCO*/ bool SPRITE_VRAM_LATCH_B;
  /*p29.WUTY*/ bool SPRITE_DONE;
  /*p29.TEXY*/ bool SPRITE_READ_VRAM;

  /*p25.VAPE*/ bool VAPE;
  /*p28.WEFY*/ bool WEFY;
  /*p25.XUJY*/ bool XUJY;
  /*p28.XUJA*/ bool XUJA;
};

//-----------------------------------------------------------------------------

void SpriteSeq_tick(SpriteSeq& prev, SpriteSeq& next,
                    wire LAPE, wire TAVA, wire VID_RESET5, wire VID_LINE_TRIG_d4a, wire RENDERING_LATCH, wire SPRITE_TRIG) {

  /*p29.TOXE*/ wire SEQ_B0  = prev.SEQ_B0;
  /*p29.TULY*/ wire SEQ_B1  = prev.SEQ_B1;
  /*p29.TESE*/ wire SEQ_B2  = prev.SEQ_B2;
  /*p29.TOBU*/ wire SEQ_3   = prev.SEQ_3;
  /*p29.VONU*/ wire SPRITE_AB   = prev.SPRITE_AB;
  /*p29.SEBA*/ wire SEQ_4d  = prev.SEQ_4d;
  /*p29.TYFO*/ wire SEQ_B0d = prev.SEQ_B0d;

  // recheck TAME, TYTU, TACU, TYFO
#if 0

  TAME NAND01 TESE
  TAME NAND02 TOXE
  TAME NAND03 TOMA02

  TYTU NOT01 TOXE
  TYTU NOT02 SYCU01

  TACU NAND01 TYTU
  TACU NAND02 TYFO
  TACU NAND03 VAPE

  TYFO DFF01 TYFO12
  TYFO DFF02 LAPE
  TYFO DFF03 TYFO09
  TYFO DFF04 nc
  TYFO DFF05 nc
  TYFO DFF06 VYPO
  TYFO DFF07 TOXE 
  TYFO DFF08 nc
  TYFO DFF09 TYFO03
  TYFO DFF10 nc
  TYFO DFF11 nc
  TYFO DFF12 TYFO01
  TYFO DFF13 VYPO
  TYFO DFF14 nc
  TYFO DFF15 nc
  TYFO DFF16 VUSA01
  TYFO DFF17 TACU SYCU

  TAME = nand(TESE,TOXE)
  TYTU = not(TOXE)
  TACU = nand(TYTU,TYFO)
  TYFO = DFF(LAPE, VYPO, TOXE)


    /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, vram.SPR_SEQ_5_SYNCn);
    /*p25.SOHO*/   wire SPR_READ_VRAM = and(SPR_SEQ_5_TRIG, spr.SPRITE_READn);
    /*p25.RAWA*/ wire SPR_READ_VRAMn = not(SPR_READ_VRAM);

      /*p29.SAKY*/ wire SAKY = nor(spr.SPR_SEQ1, spr.SEQ_xxx34xn);
      /*p29.TEPA*/ wire RENDERINGn = not(vid.RENDERING_LATCH);
      /*p29.TYSO*/ wire SPRITE_READb = or(SAKY, RENDERINGn); // seems wrong
      /*p29.TEXY*/ next.SPRITE_READn = not(SPRITE_READb);

    /*p29.XUQU*/ wire SPRITE_AB = not(!VONU);

#endif

  // all of these cells have been checked against the die

  /*p29.TAME*/ wire TAME = nand(SEQ_B2, SEQ_B0);
  /*p29.TYTU*/ wire SEQ_B0n = not(SEQ_B0); // schematic wrong?
  /*p29.TACU*/ wire TACU = nand(SEQ_B0n, SEQ_B0d);

  /*p29.TOMA*/ wire SEQ_CLK = nand(LAPE, TAME);
  /*p27.SECA*/ wire SEQ_RST = nor (SPRITE_TRIG, VID_RESET5, VID_LINE_TRIG_d4a);
  /*p27.VYPO*/ wire VYPO = 1;
  /*p29.TEPA*/ wire RENDERINGn = not(RENDERING_LATCH);
  /*p29.SYCU*/ wire SYCU = nor(SEQ_B0n, RENDERINGn, SEQ_B0d);
  
  /*p29.TOPU*/       wire TOPU = and(SEQ_B1, SYCU);
  /*p29.VYWA*/     wire VYWA = not(TOPU);
  /*p29.WENY*/   wire WENY = not(VYWA);
  /*p29.XADO*/ wire SPRITE_VRAM_LATCH_A = not(WENY);
  
  /*p29.RACA*/       wire RACA = and(SPRITE_AB, SYCU);
  /*p29.PEBY*/     wire PEBY = not(RACA);
  /*p29.NYBE*/   wire NYBE = not(PEBY);
  /*p29.PUCO*/ wire SPRITE_VRAM_LATCH_B = not(NYBE);

  /*p29.TYNO*/     wire TYNO = nand(SEQ_B0, SEQ_4d, SPRITE_AB);
  /*p29.VUSA*/   wire VUSA = or(!SEQ_B0d, TYNO);
  /*p29.WUTY*/ wire SPRITE_DONE = not(VUSA);
  /*p29.SAKY*/     wire SAKY = nor(SEQ_B1, SPRITE_AB);
  /*p29.TYSO*/   wire TYSO = or(SAKY, RENDERINGn);
  /*p29.TEXY*/ wire SPRITE_READ_VRAM = not(TYSO);

  // TUVO = nor(TEPA, TULY, TESE)
  // VAPE = and(TACU, TUVO)
  // XUJY = not(VAPE)

  /*p29.TUVO*/ wire TUVO = or(RENDERINGn, SEQ_B1, SEQ_B2);

  /*p25.VAPE*/ wire VAPE = and(TUVO, TACU);
  /*p28.WEFY*/ wire WEFY = and(TUVO, SEQ_B0d);
  /*p25.XUJY*/ wire XUJY = not(VAPE);
  /*p28.XUJA*/ wire XUJA = not(WEFY);

  next.LAPE = LAPE;
  next.TAVA = TAVA;

  /*p29.TOMA*/ next.SEQ_CLK = SEQ_CLK;
  /*p27.SECA*/ next.SEQ_RST = SEQ_RST;
  /*p29.SYCU*/ next.SYCU = SYCU;
  /*p29.XADO*/ next.SPRITE_VRAM_LATCH_A = SPRITE_VRAM_LATCH_A;
  /*p29.PUCO*/ next.SPRITE_VRAM_LATCH_B = SPRITE_VRAM_LATCH_B;
  /*p29.WUTY*/ next.SPRITE_DONE = SPRITE_DONE;
  /*p29.TEXY*/ next.SPRITE_READ_VRAM = SPRITE_READ_VRAM;

  /*p25.VAPE*/ next.VAPE = VAPE;
  /*p28.WEFY*/ next.WEFY = WEFY;
  /*p25.XUJY*/ next.XUJY = XUJY;
  /*p28.XUJA*/ next.XUJA = XUJA;

  /*p29.TOXE*/ next.SEQ_B0.tock( SEQ_CLK, SEQ_RST, !SEQ_B0);
  /*p29.TULY*/ next.SEQ_B1.tock(!SEQ_B0,  SEQ_RST, !SEQ_B1);
  /*p29.TESE*/ next.SEQ_B2.tock(!SEQ_B1,  SEQ_RST, !SEQ_B2);

  /*p29.TOBU*/ next.SEQ_3.tock (TAVA, RENDERING_LATCH, SEQ_B1);
  /*p29.VONU*/ next.SPRITE_AB.tock (TAVA, RENDERING_LATCH, SEQ_3);
  /*p29.SEBA*/ next.SEQ_4d.tock(LAPE, RENDERING_LATCH, SPRITE_AB);

  /*p29.TYFO*/ next.SEQ_B0d.tock( LAPE, VYPO, SEQ_B0); // am i suuuure this is seq_b0d?
}

//-----------------------------------------------------------------------------

static void labels() {
  int line = 3;
  printAt(0, line++, "LAPE");
  printAt(0, line++, "TAVA");
  line++;
  printAt(0, line++, "SEQ_CLK");
  printAt(0, line++, "SEQ_B0");
  printAt(0, line++, "SEQ_B1");
  printAt(0, line++, "SEQ_B2");
  printAt(0, line++, "SEQ_B0d");
  line++;
  printAt(0, line++, "SEQ_3");
  printAt(0, line++, "SEQ_4d");
  line++;
  printAt(0, line++, "SEQ_RST");
  printAt(0, line++, "SYCU");
  line++;
  printAt(0, line++, "SPRITE_READ_VRAM");
  printAt(0, line++, "SPRITE_AB");
  printAt(0, line++, "SPRITE_VRAM_LATCH_A");
  printAt(0, line++, "SPRITE_VRAM_LATCH_B");
  printAt(0, line++, "SPRITE_DONE");
  line++;
  printAt(0, line++, "VAPE");
  printAt(0, line++, "WEFY");
  printAt(0, line++, "XUJY");
  printAt(0, line++, "XUJA");
  line++;
}

static void dump(int x, SpriteSeq& seq) {
  int line = 3;
  plot(x, line++, seq.LAPE);
  plot(x, line++, seq.TAVA);
  line++;
  plot(x, line++, seq.SEQ_CLK);
  plot(x, line++, seq.SEQ_B0);
  plot(x, line++, seq.SEQ_B1);
  plot(x, line++, seq.SEQ_B2);
  plot(x, line++, seq.SEQ_B0d);
  line++;
  plot(x, line++, seq.SEQ_3);
  plot(x, line++, seq.SEQ_4d);
  line++;
  plot(x, line++, seq.SEQ_RST);
  plot(x, line++, seq.SYCU);
  line++;
  plot(x, line++, seq.SPRITE_READ_VRAM);
  plot(x, line++, seq.SPRITE_AB);
  plot(x, line++, seq.SPRITE_VRAM_LATCH_A);
  plot(x, line++, seq.SPRITE_VRAM_LATCH_B);
  plot(x, line++, seq.SPRITE_DONE);
  line++;
  plot(x, line++, seq.VAPE);
  plot(x, line++, seq.WEFY);
  plot(x, line++, seq.XUJY);
  plot(x, line++, seq.XUJA);
  line++;
}

static int phase = 0;
static int cursor = 20;

static void sim(int clocks, SpriteSeq& a, SpriteSeq& b,
         wire VID_RESET5, wire VID_LINE_TRIG_d4a, wire RENDERING_LATCH, wire SPRITE_TRIG) {
  for (int i = 0; i < clocks; i++) {
    wire LAPE = (phase++) & 1;
    wire TAVA = !LAPE;
    for (int j = 0; j < 8; j++) {
      SpriteSeq_tick(a, b, LAPE, TAVA, VID_RESET5, VID_LINE_TRIG_d4a, RENDERING_LATCH, SPRITE_TRIG);
      a = b;
    }
    dump(cursor++, b);
  }
}

#if 0
void TestSpriteSeq() {
  printf("\033[?6l");
  labels();

  bool VID_RESET = false;
  bool NEW_LINE = true;
  bool RENDERING = false;
  bool SPRITE_TRIG = false;

  SpriteSeq prev = {};
  SpriteSeq next = {};

  for(int i = 0; i < 100; i++) {
    int clk = i / 2;
    int phase = i & 1;

    VID_RESET   = (i < 20);
    NEW_LINE    = (i == 40);
    RENDERING   = (i >= 60);
    SPRITE_TRIG = (i == 80);

    if (i == 20) printf("\n");
    if (i == 40) printf("\n");
    if (i == 60) printf("\n");
    if (i == 80) printf("\n");

    for (int j = 0; j < 10; j++) {
      sprite_seq(prev, next, phase, !phase, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
      prev = next;
    }

    int seq = (next.TESE * 4) + (next.TULY * 2) + next.TOXE;
    printf("%2d:%c ", clk, phase ? '+' : '-');

    printf("seq:%d ", seq);
    printf("d:%d%d%d ",
             (bool)next.TOBU,
             (bool)next.VONU,
             (bool)next.SEBA);

    printf("TYFO:%d ", (bool)next.TYFO);
    printf("TUVO:%d ", (bool)next.TUVO);
    printf("XADO:%d ", (bool)next.XADO);
    printf("PUCO:%d ", (bool)next.PUCO);
    printf("TEXY:%d ", (bool)next.TEXY);
    printf("WUTY:%d ", (bool)next.WUTY);
    printf("XUJY:%d ", (bool)next.XUJY);

    printf("\n");
  }
}
#endif

void TestSpriteSeq() {
  printf("\033[?6l");
  labels();

  SpriteSeq a = {};
  SpriteSeq b = {};

  bool VID_RESET = false;
  bool NEW_LINE = true;
  bool RENDERING = false;
  bool SPRITE_TRIG = false;

  //----------

  VID_RESET = true;
  NEW_LINE = false;
  RENDERING = false;
  SPRITE_TRIG = false;

  printAt(cursor, 1, "rst");
  sim(4, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  VID_RESET = false;

  printAt(cursor, 1, "!vid_rst");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  NEW_LINE = true;

  printAt(cursor, 1, "newline");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  NEW_LINE = false;

  printAt(cursor, 1, "!newline");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  RENDERING = true;

  printAt(cursor, 1, "rendering");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  SPRITE_TRIG = true;

  printAt(cursor, 1, "sp");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  SPRITE_TRIG = false;

  printAt(cursor, 1, "!sp");
  sim(16, a, b, VID_RESET, NEW_LINE, RENDERING, SPRITE_TRIG);
  cursor++;

  printAt(0, 50, "done");
}