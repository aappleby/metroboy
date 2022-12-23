#pragma once

struct Req;
struct Ack;
struct Dumper;

//-----------------------------------------------------------------------------

struct MetroBoyDMA {

  void tick(int phase_total, const Req& req, Ack& ack);
  void tock(int phase_total, const Req& req);
  void dump(Dumper& dump) const;

  int addr = 0xFFFF;

  /*_p04.LYXE*/ bool DMA_WR_LATCH = 0;
  /*_p04.LUVY*/ bool DMA_RUN_TRIG_d0 = 0;
  /*_p04.LENE*/ bool DMA_RUN_TRIG_d4 = 0;
  /*_p04.LOKY*/ bool DMA_RUN_READ = 0;
  /*_p04.MATU*/ bool DMA_RUN_WRITE = 0;
  /*_p04.MYTE*/ bool DMA_DONE = 0;
};

//-----------------------------------------------------------------------------
