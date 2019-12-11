#include "Schematics.h"

namespace Schematics {

struct SpriteStoreTickIn {
  bool STORE_MATCH_IN;
  bool MATCH_EN; 
  bool X0,X1,X2,X3,X4,X5,X6,X7;
};

struct SpriteStoreTickOut {
  bool STORE_MATCH_OUT;
  bool MATCHn;

  bool TS_IDX_0;
  bool TS_IDX_1;
  bool TS_IDX_2;
  bool TS_IDX_3;
  bool TS_IDX_4;
  bool TS_IDX_5;

  bool TS_LINE_0;
  bool TS_LINE_1;
  bool TS_LINE_2;
  bool TS_LINE_3;
};

struct SpriteStore {
  void tick(const SpriteStoreTickIn& in, SpriteStoreTickOut& out);

  bool SPRITE_GET_SYNCn;

  bool STORE_CLKa;
  bool STORE_X0;
  bool STORE_X1;
  bool STORE_X2;
  bool STORE_X3;
  bool STORE_X4;
  bool STORE_X5;
  bool STORE_X6;
  bool STORE_X7;

  bool STORE_CLKb;
  bool STORE_IDX0;
  bool STORE_IDX1;
  bool STORE_IDX2;
  bool STORE_IDX3;
  bool STORE_IDX4;
  bool STORE_IDX5;

  bool STORE_CLKc;
  bool STORE_LINE0;
  bool STORE_LINE1;
  bool STORE_LINE2;
  bool STORE_LINE3;
};

struct SpriteStoreTockIn {
  bool SPRITE_DONE_A;
  bool SPRITE_DONE_B;
  bool NEW_LINE3;
  bool NEW_LINE4;

  bool STORE_MATCH_IN;
  bool STORE_EN;
  
  bool STORE_SEL;
  
  bool SPRITE_X0;
  bool SPRITE_X1;
  bool SPRITE_X2;
  bool SPRITE_X3;
  bool SPRITE_X4;
  bool SPRITE_X5;
  bool SPRITE_X6;
  bool SPRITE_X7;

  bool TS_IDX_0;
  bool TS_IDX_1;
  bool TS_IDX_2;
  bool TS_IDX_3;
  bool TS_IDX_4;
  bool TS_IDX_5;

  bool TS_LINE_0;
  bool TS_LINE_1;
  bool TS_LINE_2;
  bool TS_LINE_3;
};

//-----------------------------------------------------------------------------

void SpriteStore::tick(const SpriteStoreTickIn& in, SpriteStoreTickOut& out) {
  /*p21.ACAM*/ wire X0n = not(in.X0);
  /*p21.AZUB*/ wire X1n = not(in.X1);
  /*p21.AMEL*/ wire X2n = not(in.X2);
  /*p21.AHAL*/ wire X3n = not(in.X3);
  /*p21.APUX*/ wire X4n = not(in.X4);
  /*p21.ABEF*/ wire X5n = not(in.X5);
  /*p21.ADAZ*/ wire X6n = not(in.X6);
  /*p21.ASAH*/ wire X7n = not(in.X7);

  /*p31.YMAM*/ wire MATCH0 = xor(STORE_X0, X0n);
  /*p31.YTYP*/ wire MATCH1 = xor(STORE_X1, X1n);
  /*p31.YFOP*/ wire MATCH2 = xor(STORE_X2, X2n);
  /*p31.YVAC*/ wire MATCH3 = xor(STORE_X3, X3n);
  /*p31.ZYWU*/ wire MATCH4 = xor(STORE_X4, X4n);
  /*p31.ZUZA*/ wire MATCH5 = xor(STORE_X5, X5n);
  /*p31.ZEJO*/ wire MATCH6 = xor(STORE_X6, X6n);
  /*p31.ZEDA*/ wire MATCH7 = xor(STORE_X7, X7n);
  /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.YGEM*/ wire MATCHn = nand(in.MATCH_EN, MATCHA, MATCHB);

  /*p29.GUZE*/ wire SPRITE_GETn = nor(MATCHn, in.STORE_MATCH_IN);
  /*p29.FADO*/ wire SPRITE_GET = not(SPRITE_GETn);
  /*p30.YHAL*/ if (SPRITE_GET) out.TS_IDX_0  = not(!STORE_IDX0);
  /*p30.YRAD*/ if (SPRITE_GET) out.TS_IDX_1  = not(!STORE_IDX1);
  /*p30.XYRA*/ if (SPRITE_GET) out.TS_IDX_2  = not(!STORE_IDX2);
  /*p30.YNEV*/ if (SPRITE_GET) out.TS_IDX_3  = not(!STORE_IDX3);
  /*p30.ZOJY*/ if (SPRITE_GET) out.TS_IDX_4  = not(!STORE_IDX4);
  /*p30.ZARO*/ if (SPRITE_GET) out.TS_IDX_5  = not(!STORE_IDX5);
  /*p30.CAWO*/ if (SPRITE_GET) out.TS_LINE_0 = not(!STORE_LINE0);
  /*p30.BYME*/ if (SPRITE_GET) out.TS_LINE_1 = not(!STORE_LINE1);
  /*p30.COHO*/ if (SPRITE_GET) out.TS_LINE_2 = not(!STORE_LINE2);
  /*p30.GATE*/ if (SPRITE_GET) out.TS_LINE_3 = not(!STORE_LINE3);

  out.MATCHn = MATCHn;
}

//-----------------------------------------------------------------------------

void SpriteStore9_tock(const SpriteStoreTickOut& tick_out, const SpriteStoreTockIn& in, const SpriteStore& a, const SpriteStore& b, SpriteStore& c) {

  ///*p29.DOGU*/ wire STORE_SEL = nand(in.SPRITE_COUNT0b, in.SPRITE_COUNT1n, in.SPRITE_COUNT2n, in.SPRITE_COUNT3b); // 1001
  /*p29.CATO*/ wire STORE_CLK = or(in.STORE_EN, in.STORE_SEL);
  /*p29.DECU*/ wire STORE_CLKn = not(STORE_CLK);

  /*p29.GUZE*/ wire SPRITE_GETn = nor(tick_out.MATCHn, in.STORE_MATCH_IN);
  /*p29.FONO*/ c.SPRITE_GET_SYNCn = tock_pos(in.SPRITE_DONE_A, in.SPRITE_DONE_B, in.NEW_LINE3, b.SPRITE_GET_SYNCn, SPRITE_GETn);

  /*p29.DUBU*/ wire STORE_RST  = or(in.NEW_LINE4, b.SPRITE_GET_SYNCn);
  /*p29.DOSY*/ wire STORE_RSTn = not(STORE_RST);
    
  /*p29.WEME*/ c.STORE_CLKa = not(STORE_CLKn);
  /*p31.XUVY*/ c.STORE_X0 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X0, in.SPRITE_X0);
  /*p31.XERE*/ c.STORE_X1 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X1, in.SPRITE_X1);
  /*p31.XUZO*/ c.STORE_X2 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X2, in.SPRITE_X2);
  /*p31.XEXA*/ c.STORE_X3 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X3, in.SPRITE_X3);
  /*p31.YPOD*/ c.STORE_X4 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X4, in.SPRITE_X4);
  /*p31.YROP*/ c.STORE_X5 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X5, in.SPRITE_X5);
  /*p31.YNEP*/ c.STORE_X6 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X6, in.SPRITE_X6);
  /*p31.YZOF*/ c.STORE_X7 = tock_pos(a.STORE_CLKa, b.STORE_CLKa, STORE_RSTn, b.STORE_X7, in.SPRITE_X7);

  /*p29.WUFA*/ c.STORE_CLKb = not(STORE_CLKn);
  /*p30.XUFO*/ c.STORE_IDX0 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX0,in.TS_IDX_0);
  /*p30.XUTE*/ c.STORE_IDX1 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX1,in.TS_IDX_1);
  /*p30.XOTU*/ c.STORE_IDX2 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX2,in.TS_IDX_2);
  /*p30.XYFE*/ c.STORE_IDX3 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX3,in.TS_IDX_3);
  /*p30.YZOR*/ c.STORE_IDX4 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX4,in.TS_IDX_4);
  /*p30.YBER*/ c.STORE_IDX5 = tock_pos(a.STORE_CLKb, b.STORE_CLKb, 0, b.STORE_IDX5,in.TS_IDX_5);

  /*p29.FAKA*/ c.STORE_CLKc = not(STORE_CLKn);
  /*p30.DEWU*/ c.STORE_LINE0 = tock_pos(a.STORE_CLKc, b.STORE_CLKc, 0, b.STORE_LINE0, in.TS_LINE_0);
  /*p30.CANA*/ c.STORE_LINE1 = tock_pos(a.STORE_CLKc, b.STORE_CLKc, 0, b.STORE_LINE1, in.TS_LINE_1);
  /*p30.DYSY*/ c.STORE_LINE2 = tock_pos(a.STORE_CLKc, b.STORE_CLKc, 0, b.STORE_LINE2, in.TS_LINE_2);
  /*p30.FOFO*/ c.STORE_LINE3 = tock_pos(a.STORE_CLKc, b.STORE_CLKc, 0, b.STORE_LINE3, in.TS_LINE_3);

}

//-----------------------------------------------------------------------------

struct SpriteStoreAll {

  SpriteStore store0;
  SpriteStore store1;
  SpriteStore store2;
  SpriteStore store3;
  SpriteStore store4;
  SpriteStore store5;
  SpriteStore store6;
  SpriteStore store7;
  SpriteStore store8;
  SpriteStore store9;

};

void SpriteStore_tick(const SpriteStoreAll& a, const SpriteStoreAll& b, SpriteStoreAll& c) {
  (void)a;
  (void)b;
  (void)c;
}

void SpriteStore_tock(const SpriteStoreAll& a, const SpriteStoreAll& b, SpriteStoreAll& c) {
  (void)a;
  (void)b;
  (void)c;
}


//-----------------------------------------------------------------------------
};