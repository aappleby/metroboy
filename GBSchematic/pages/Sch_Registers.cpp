#include "Sch_Registers.h"

#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_BusControl.h"


namespace Schematics {

//-----------------------------------------------------------------------------

void VidRegisters::pwron() {
  big_pwron(LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
            LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);

  big_pwron(SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
  big_pwron(SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
  big_pwron(LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
  big_pwron(BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
  big_pwron(OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
  big_pwron(OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);
  big_pwron(WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
  big_pwron(WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);
}

void VidRegisters::reset() {
  big_reset(LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
            LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);

  big_reset(SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
  big_reset(SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
  big_reset(LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
  big_reset(BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
  big_reset(OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
  big_reset(OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);
  big_reset(WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
  big_reset(WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);
}

void VidRegisters::phase_begin() {
}

void VidRegisters::phase_end() {
}

void VidRegisters::pass_begin() {
}

bool VidRegisters::pass_end() {
  bool changed = false;
  changed |= big_commit(LCDC_BGEN, LCDC_SPEN, LCDC_SPSIZE, LCDC_BGMAP,
                        LCDC_BGTILE, LCDC_WINEN, LCDC_WINMAP, LCDC_EN);
  changed |= big_commit(SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
  changed |= big_commit(SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
  changed |= big_commit(LYC0,  LYC1,  LYC2,  LYC3,  LYC4,  LYC5,  LYC6,  LYC7);
  changed |= big_commit(BGP0,  BGP1,  BGP2,  BGP3,  BGP4,  BGP5,  BGP6,  BGP7);
  changed |= big_commit(OBP00, OBP01, OBP02, OBP03, OBP04, OBP05, OBP06, OBP07);
  changed |= big_commit(OBP10, OBP11, OBP12, OBP13, OBP14, OBP15, OBP16, OBP17);
  changed |= big_commit(WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
  changed |= big_commit(WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);
  return changed;
}

//-----------------------------------------------------------------------------

VidSignals VidRegisters::tick(const SysSignals& /*sys_sig*/,
                              const RstSignals& rst_sig,
                              const BusSignals& bus_sig,
                              const Decoder&    dec_sig,
                              BusTristates & bus_out) {

  //----------

  /*p23.VYRE*/ wire FF40_RD = and(dec_sig.FF40, bus_sig.ASOT_CPURD);
  /*p23.WYCE*/ wire FF40_RDn = not(FF40_RD);
  /*p23.WARU*/ wire FF40_WR = and(dec_sig.FF40, bus_sig.CUPA_CPUWR);
  /*p23.XUBO*/ wire FF40_WRn = not(FF40_WR);

  if (!FF40_RDn) bus_out.set_data(
    /*p23.WYPO*/ LCDC_BGEN,
    /*p23.XERO*/ LCDC_SPEN,
    /*p23.WYJU*/ LCDC_SPSIZE,
    /*p23.WUKA*/ LCDC_BGMAP,
    /*p23.VOKE*/ LCDC_BGTILE,
    /*p23.VATO*/ LCDC_WINEN,
    /*p23.VAHA*/ LCDC_WINMAP,
    /*p23.XEBU*/ LCDC_EN
  );

  /*p23.VYXE*/ LCDC_BGEN    .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D0());
  /*p23.XYLO*/ LCDC_SPEN    .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D1());
  /*p23.XYMO*/ LCDC_SPSIZE  .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D2());
  /*p23.XAFO*/ LCDC_BGMAP   .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D3());
  /*p23.WEXU*/ LCDC_BGTILE  .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D4());
  /*p23.WYMO*/ LCDC_WINEN   .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D5());
  /*p23.WOKY*/ LCDC_WINMAP  .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D6());
  /*p23.XONA*/ LCDC_EN      .set(FF40_WRn, rst_sig.XARE_RESET, bus_out.D7());

  //----------

  /*p23.ANYP*/ wire FF42_RD = and(dec_sig.FF42, bus_sig.ASOT_CPURD);
  /*p23.BUWY*/ wire FF42_RDn = not(FF42_RD);
  /*p23.BEDY*/ wire FF42_WR = and(dec_sig.FF42, bus_sig.CUPA_CPUWR);
  /*p23.CAVO*/ wire FF42_WRn = not(FF42_WR);

  if (!FF42_RDn) bus_out.set_data(
    /*p23.WARE*/ SCY0,
    /*p23.GOBA*/ SCY1,
    /*p23.GONU*/ SCY2,
    /*p23.GODO*/ SCY3,
    /*p23.CUSA*/ SCY4,
    /*p23.GYZO*/ SCY5,
    /*p23.GUNE*/ SCY6,
    /*p23.GYZA*/ SCY7
  );

  /*p23.GAVE*/ SCY0.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D0());
  /*p23.FYMO*/ SCY1.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D1());
  /*p23.FEZU*/ SCY2.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D2());
  /*p23.FUJO*/ SCY3.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D3());
  /*p23.DEDE*/ SCY4.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D4());
  /*p23.FOTY*/ SCY5.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D5());
  /*p23.FOHA*/ SCY6.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D6());
  /*p23.FUNY*/ SCY7.set(FF42_WRn, rst_sig.CUNU_RESETn, bus_out.D7());

  //----------

  /*p23.AVOG*/ wire FF43_RD = and(dec_sig.FF43, bus_sig.ASOT_CPURD);
  /*p23.BEBA*/ wire FF43_RDn = not(FF43_RD);
  /*p23.ARUR*/ wire FF43_WR = and(dec_sig.FF43, bus_sig.CUPA_CPUWR);
  /*p23.AMUN*/ wire FF43_WRn = not(FF43_WR);

  if (!FF43_RDn) bus_out.set_data(
    /*p23.EDOS*/ SCX0,
    /*p23.EKOB*/ SCX1,
    /*p23.CUGA*/ SCX2,
    /*p23.WONY*/ SCX3,
    /*p23.CEDU*/ SCX4,
    /*p23.CATA*/ SCX5,
    /*p23.DOXE*/ SCX6,
    /*p23.CASY*/ SCX7
  );

  /*p23.DATY*/ SCX0.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D0());
  /*p23.DUZU*/ SCX1.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D1());
  /*p23.CYXU*/ SCX2.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D2());
  /*p23.GUBO*/ SCX3.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D3());
  /*p23.BEMY*/ SCX4.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D4());
  /*p23.CUZY*/ SCX5.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D5());
  /*p23.CABU*/ SCX6.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D6());
  /*p23.BAKE*/ SCX7.set(FF43_WRn, rst_sig.CUNU_RESETn, bus_out.D7());

  //----------

  /*p23.XYLY*/ wire FF45_RD = and(bus_sig.ASOT_CPURD, dec_sig.FF45);
  /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);
  /*p23.XUFA*/ wire FF45_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF45);
  /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

  if (!FF45_RDn) bus_out.set_data(
    /*p23.RETU*/ LYC0,
    /*p23.VOJO*/ LYC1,
    /*p23.RAZU*/ LYC2,
    /*p23.REDY*/ LYC3,
    /*p23.RACE*/ LYC4,
    /*p23.VAZU*/ LYC5,
    /*p23.VAFE*/ LYC6,
    /*p23.PUFY*/ LYC7
  );

  /*p23.SYRY*/ LYC0.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D0());
  /*p23.VUCE*/ LYC1.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D1());
  /*p23.SEDY*/ LYC2.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D2());
  /*p23.SALO*/ LYC3.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D3());
  /*p23.SOTA*/ LYC4.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D4());
  /*p23.VAFA*/ LYC5.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D5());
  /*p23.VEVO*/ LYC6.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D6());
  /*p23.RAHA*/ LYC7.set(FF45_WRn, rst_sig.WESY_RESET, bus_out.D7());

  //----------

  /*p36.VUSO*/ wire FF47_RD = and(bus_sig.ASOT_CPURD, dec_sig.FF47);
  /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);
  /*p36.VELY*/ wire FF47_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF47);
  /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

  if (!FF47_RDn) bus_out.set_data(
    /*p36.RARO*/ BGP0,
    /*p36.PABA*/ BGP1,
    /*p36.REDO*/ BGP2,
    /*p36.LOBE*/ BGP3,
    /*p36.LACE*/ BGP4,
    /*p36.LYKA*/ BGP5,
    /*p36.LODY*/ BGP6,
    /*p36.LARY*/ BGP7
  );

  /*p36.PAVO*/ BGP0.set(FF47_WRn, 1, bus_out.D0());
  /*p36.NUSY*/ BGP1.set(FF47_WRn, 1, bus_out.D1());
  /*p36.PYLU*/ BGP2.set(FF47_WRn, 1, bus_out.D2());
  /*p36.MAXY*/ BGP3.set(FF47_WRn, 1, bus_out.D3());
  /*p36.MUKE*/ BGP4.set(FF47_WRn, 1, bus_out.D4());
  /*p36.MORU*/ BGP5.set(FF47_WRn, 1, bus_out.D5());
  /*p36.MOGY*/ BGP6.set(FF47_WRn, 1, bus_out.D6());
  /*p36.MENA*/ BGP7.set(FF47_WRn, 1, bus_out.D7());

  //----------

  /*p36.XUFY*/ wire FF48_RD  = and(bus_sig.ASOT_CPURD, dec_sig.FF48);
  /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);
  /*p36.XOMA*/ wire FF48_WR  = and(bus_sig.CUPA_CPUWR, dec_sig.FF48);
  /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

  if (!FF48_RDn) bus_out.set_data(
    /*p36.XARY*/ OBP00,
    /*p36.XOKE*/ OBP01,
    /*p36.XUNO*/ OBP02,
    /*p36.XUBY*/ OBP03,
    /*p36.XAJU*/ OBP04,
    /*p36.XOBO*/ OBP05,
    /*p36.XAXA*/ OBP06,
    /*p36.XAWO*/ OBP07
  );

  /*p36.XUFU*/ OBP00.set(FF48_WRn, 1, bus_out.D0());
  /*p36.XUKY*/ OBP01.set(FF48_WRn, 1, bus_out.D1());
  /*p36.XOVA*/ OBP02.set(FF48_WRn, 1, bus_out.D2());
  /*p36.XALO*/ OBP03.set(FF48_WRn, 1, bus_out.D3());
  /*p36.XERU*/ OBP04.set(FF48_WRn, 1, bus_out.D4());
  /*p36.XYZE*/ OBP05.set(FF48_WRn, 1, bus_out.D5());
  /*p36.XUPO*/ OBP06.set(FF48_WRn, 1, bus_out.D6());
  /*p36.XANA*/ OBP07.set(FF48_WRn, 1, bus_out.D7());

  //----------

  /*p36.MUMY*/ wire FF49_RD  = and(bus_sig.ASOT_CPURD, dec_sig.FF49);
  /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD);
  /*p36.MYXE*/ wire FF49_WR  = and(bus_sig.CUPA_CPUWR, dec_sig.FF49);
  /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

  if (!FF48_RDn) bus_out.set_data(
    /*p36.LAJU*/ OBP10,
    /*p36.LEPA*/ OBP11,
    /*p36.LODE*/ OBP12,
    /*p36.LYZA*/ OBP13,
    /*p36.LUKY*/ OBP14,
    /*p36.LUGA*/ OBP15,
    /*p36.LEBA*/ OBP16,
    /*p36.LELU*/ OBP17
  );

  /*p36.MOXY*/ OBP10.set(FF49_WRn, 1, bus_out.D0());
  /*p36.LAWO*/ OBP11.set(FF49_WRn, 1, bus_out.D1());
  /*p36.MOSA*/ OBP12.set(FF49_WRn, 1, bus_out.D2());
  /*p36.LOSE*/ OBP13.set(FF49_WRn, 1, bus_out.D3());
  /*p36.LUNE*/ OBP14.set(FF49_WRn, 1, bus_out.D4());
  /*p36.LUGU*/ OBP15.set(FF49_WRn, 1, bus_out.D5());
  /*p36.LEPU*/ OBP16.set(FF49_WRn, 1, bus_out.D6());
  /*p36.LUXO*/ OBP17.set(FF49_WRn, 1, bus_out.D7());

  //----------

  /*p23.WAXU*/ wire FF4A_RD = and(bus_sig.ASOT_CPURD, dec_sig.FF4A);
  /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);
  /*p23.WEKO*/ wire FF4A_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF4A);
  /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

  if (!FF4A_RDn) bus_out.set_data(
    /*p23.PUNU*/ WY0,
    /*p23.PODA*/ WY1,
    /*p23.PYGU*/ WY2,
    /*p23.LOKA*/ WY3,
    /*p23.MEGA*/ WY4,
    /*p23.PELA*/ WY5,
    /*p23.POLO*/ WY6,
    /*p23.MERA*/ WY7
  );

  /*p23.NESO*/ WY0.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D0());
  /*p23.NYRO*/ WY1.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D1());
  /*p23.NAGA*/ WY2.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D2());
  /*p23.MELA*/ WY3.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D3());
  /*p23.NULO*/ WY4.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D4());
  /*p23.NENE*/ WY5.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D5());
  /*p23.NUKA*/ WY6.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D6());
  /*p23.NAFU*/ WY7.set(FF4A_WRn, rst_sig.WALU_RESET, bus_out.D7());

  //----------

  /*p23.WYZE*/ wire FF4B_RD = and(bus_sig.ASOT_CPURD, dec_sig.FF4B);
  /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);
  /*p23.WUZA*/ wire FF4B_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF4B);
  /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

  if (!FF4B_RDn) bus_out.set_data(
    /*p23.LOVA*/ WX0,
    /*p23.MUKA*/ WX1,
    /*p23.MOKO*/ WX2,
    /*p23.LOLE*/ WX3,
    /*p23.MELE*/ WX4,
    /*p23.MUFE*/ WX5,
    /*p23.MULY*/ WX6,
    /*p23.MARA*/ WX7
  );

  /*p23.MYPA*/ WX0.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D0());
  /*p23.NOFE*/ WX1.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D1());
  /*p23.NOKE*/ WX2.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D2());
  /*p23.MEBY*/ WX3.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D3());
  /*p23.MYPU*/ WX4.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D4());
  /*p23.MYCE*/ WX5.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D5()); 
  /*p23.MUVO*/ WX6.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D6());
  /*p23.NUKU*/ WX7.set(FF4B_WRn, rst_sig.WALU_RESET, bus_out.D7());

  return {};
}

//-----------------------------------------------------------------------------

};