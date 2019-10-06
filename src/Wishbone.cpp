#include "Types.h"

struct WishboneBus {
  uint32_t addr;
  uint32_t data;
  uint8_t  tag;
  bool     we;
  bool     sel;
  bool     stb;
  bool     cyc;
  bool     lock;
};

struct WishboneOut {
  uint32_t data;
  uint8_t  tag;
  bool     ack;
  bool     stall;
  bool     retry;
  bool     err;
};

class WishboneSrc {
public:

  void reset();
  void tick();
  void tock();
};


class WishboneDst {
public:
 
  void reset();
  void tick();
  WishboneOut tock(WishboneBus bus) {
    WishboneOut ret = {};

    switch (state) {
    case WB_OFF:
      break;
    case WB_IDLE:
      break;
    case WB_HANDSHAKE:
      break;
    case WB_READ:
      break;
    case WB_WRITE:
      break;
    case WB_STALL:
      break;
    };

    if (bus.stb) {
      if (bus.sel) {
        if (bus.we) {
          mem[bus.addr & 0xFF] = bus.data;
          ret.tag = 0;
          ret.ack = 1;
          ret.stall = 0;
          ret.retry = 0;
          ret.err = 0;
        }
        else {
          ret.data = mem[bus.addr & 0xFF];
          ret.tag = 0;
          ret.ack = 1;
          ret.stall = 0;
          ret.retry = 0;
          ret.err = 0;
        }
      }
    }

    return ret;
  }

  enum {
    WB_OFF,
    WB_IDLE,
    WB_HANDSHAKE,
    WB_READ,
    WB_WRITE,
    WB_STALL,
  };

  uint8_t state;

  uint32_t mem[256];
};