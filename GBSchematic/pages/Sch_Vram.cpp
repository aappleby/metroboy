#include "Sch_Vram.h"

#include "Sch_BusControl.h"
#include "Sch_Pins.h"
#include "Sch_LCD.h"
#include "Sch_Registers.h"
#include "Sch_Video.h"
#include "Sch_BusControl.h"
#include "Sch_Sprites.h"
#include "Sch_OAM.h"
#include "Sch_PixelPipe.h"
#include "Sch_Decoder.h"
#include "Sch_DMA.h"
#include "Sch_Clocks.h"
#include "Sch_Debug.h"
#include "Sch_SpriteStore.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Vram_tick(const LcdSignals& lcd_sig,
               const VidRegisters2& vid_regs,
               const SpriteSignals& spr_sig,
               const DmaSignals& dma_sig,
               const OamSignals& oam_sig,
               const VidSignals& vid_sig,
               const PixelPipeSignals& pix_sig,
               const JoypadPins& joy_pins,
               const SpriteTristate& spr_tri,
               BusTristates& bus_tri,
               VramBus& vram_bus) {
#if 0
  {
    /*p25.TAVY*/     wire MOE_Cn = not(vram_pins.MOE_C);
    /*p25.TEGU*/     wire CPU_VRAM_CLK = nand(dec.ADDR_VRAM, clocks.AFAS_xxxxEFGx);
    /*p25.SALE*/   wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_VRAM);
    /*p25.RYLU*/ wire CPU_READ_VRAMn = nand(CPU_VRAM_CLK2, vid.RENDERINGn);

    /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, vram.SPR_SEQ_5_SYNCn);
    /*p25.SOHO*/   wire SPR_READ_VRAM = and(SPR_SEQ_5_TRIG, spr.SPRITE_READn);
    /*p25.RAWA*/ wire SPR_READ_VRAMn = not(SPR_READ_VRAM);

    /*p27.LURY*/     wire LURY = and(!vid.BG_SEQ5_SYNC, vid.RENDERING); // probably SYNCn
    /*p27.LONY*/   wire BG_READ_VRAM = and(LURY, vid.BG_SEQ_RSTn);
    /*p27.MYMA*/ wire BG_READ_VRAMn = not(BG_READ_VRAM);

    /*p25.APAM*/ wire DMA_READ_VRAMn = not(dma.DMA_READ_VRAM);

    /*p25.RACU*/ wire MOEn = and(CPU_READ_VRAMn,
                                 SPR_READ_VRAMn,
                                 BG_READ_VRAMn,
                                 DMA_READ_VRAMn);
  }

  {
    /*p25.TEFY*/ wire MCS_Cn = not(vram.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (dec.ADDR_VRAM, dec.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, dbg.DBG_VRAM);
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(vid.RENDERING_LATCH);
    /*p25.SERE*/ next.SERE = and(CPU_VRAM_RD2, ROPY_RENDERINGn);
  }

  {
    /*p25.SUTU*/     wire MCS = nor(vid.BG_READ_VRAM, dma.DMA_READ_VRAM, spr.SPRITE_READn, SERE);
  }

  {
    /*p25.SUDO*/       wire MWR_Cn = not(pins.MWR_C);
    /*p01.AREV*/           wire AREV = nand(vram.cpu.CPU_RAW_WR, vram.sys.AFAS_xxxxEFGx);
    /*p01.APOV*/         wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p25.TUJA*/       wire CPU_VRAM_WR    = and (dec.ADDR_VRAM, CPU_WR_xxxxEFGx);
    /*p25.TYJY*/     wire CPU_VRAM_WR2   = mux2(MWR_Cn, CPU_VRAM_WR , DBG_VRAM);
    /*p25.SOHY*/   wire MWR = nand(CPU_VRAM_WR2, bus.SERE);
  }
#endif



  // VRAM address driver
  {
    //----------
    // vid x/y + scroll x/y

    /*p26.FAFO*/ wire TILE_Y0C  = add_c(lcd_sig.Y0, vid_regs.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1C  = add_c(lcd_sig.Y1, vid_regs.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2C  = add_c(lcd_sig.Y2, vid_regs.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0C   = add_c(lcd_sig.Y3, vid_regs.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1C   = add_c(lcd_sig.Y4, vid_regs.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2C   = add_c(lcd_sig.Y5, vid_regs.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3C   = add_c(lcd_sig.Y6, vid_regs.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4C   = add_c(lcd_sig.Y7, vid_regs.SCY7, MAP_Y3C);

    /*p26.FAFO*/ wire TILE_Y0S  = add_s(lcd_sig.Y0, vid_regs.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1S  = add_s(lcd_sig.Y1, vid_regs.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2S  = add_s(lcd_sig.Y2, vid_regs.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0S   = add_s(lcd_sig.Y3, vid_regs.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1S   = add_s(lcd_sig.Y4, vid_regs.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2S   = add_s(lcd_sig.Y5, vid_regs.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3S   = add_s(lcd_sig.Y6, vid_regs.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4S   = add_s(lcd_sig.Y7, vid_regs.SCY7, MAP_Y3C);

    /*p26.ATAD*/ wire TILE_X0C  = add_c(vid_sig.X0, vid_regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1C  = add_c(vid_sig.X1, vid_regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2C  = add_c(vid_sig.X2, vid_regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0C   = add_c(vid_sig.X3, vid_regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1C   = add_c(vid_sig.X4, vid_regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2C   = add_c(vid_sig.X5, vid_regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3C   = add_c(vid_sig.X6, vid_regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4C   = add_c(vid_sig.X7, vid_regs.SCX7, MAP_X3C);

    /*p26.ATAD*/ wire TILE_X0S  = add_s(vid_sig.X0, vid_regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1S  = add_s(vid_sig.X1, vid_regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2S  = add_s(vid_sig.X2, vid_regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0S   = add_s(vid_sig.X3, vid_regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1S   = add_s(vid_sig.X4, vid_regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2S   = add_s(vid_sig.X5, vid_regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3S   = add_s(vid_sig.X6, vid_regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4S   = add_s(vid_sig.X7, vid_regs.SCX7, MAP_X3C);

    //----------

    /*p29.WUKY*/ wire FLIP_Y = not(oam_sig.OAM_A_D6);
    /*p29.XUQU*/ wire SPRITE_AB = not(!spr_sig.SEQ_xxx34xn);
    /*p29.FUFO*/ wire LCDC_SPSIZEn = not(vid_regs.LCDC_SPSIZE);

    /*p29.WAGO*/ wire WAGO      = xor(FLIP_Y, spr_tri.TS_LINE_0);
    /*p29.CYVU*/ wire SPRITE_Y0 = xor(FLIP_Y, spr_tri.TS_LINE_1);
    /*p29.BORE*/ wire SPRITE_Y1 = xor(FLIP_Y, spr_tri.TS_LINE_2);
    /*p29.BUVY*/ wire SPRITE_Y2 = xor(FLIP_Y, spr_tri.TS_LINE_3);
    /*p29.GEJY*/ wire SPRITE_Y3 = amux2(LCDC_SPSIZEn, !oam_sig.OAM_B_D0, vid_regs.LCDC_SPSIZE,  WAGO);

    /*p29.ABON*/ wire SPRITE_READn = not(spr_sig.SPRITE_READ);
    /*p29.ABEM*/ if (!SPRITE_READn) vram_bus.MA00 = SPRITE_AB;
    /*p29.BAXE*/ if (!SPRITE_READn) vram_bus.MA01 = SPRITE_Y0;
    /*p29.ARAS*/ if (!SPRITE_READn) vram_bus.MA02 = SPRITE_Y1;
    /*p29.AGAG*/ if (!SPRITE_READn) vram_bus.MA03 = SPRITE_Y2;
    /*p29.FAMU*/ if (!SPRITE_READn) vram_bus.MA04 = SPRITE_Y3;
    /*p29.FUGY*/ if (!SPRITE_READn) vram_bus.MA05 = oam_sig.OAM_B_D1;
    /*p29.GAVO*/ if (!SPRITE_READn) vram_bus.MA06 = oam_sig.OAM_B_D2;
    /*p29.WYGA*/ if (!SPRITE_READn) vram_bus.MA07 = oam_sig.OAM_B_D3;
    /*p29.WUNE*/ if (!SPRITE_READn) vram_bus.MA08 = oam_sig.OAM_B_D4;
    /*p29.GOTU*/ if (!SPRITE_READn) vram_bus.MA09 = oam_sig.OAM_B_D5;
    /*p29.GEGU*/ if (!SPRITE_READn) vram_bus.MA10 = oam_sig.OAM_B_D6;
    /*p29.XEHE*/ if (!SPRITE_READn) vram_bus.MA11 = oam_sig.OAM_B_D7;
    /*p29.DYSO*/ if (!SPRITE_READn) vram_bus.MA12 = joy_pins.P10_B;   // sprites always in low half of tile store

    /*p25.VUZA*/ wire TILE_BANK = nor(vid_regs.LCDC_BGTILE, pix_sig.BG_PIX_B7); // register reused

    /*p25.XONU*/ if (vid_sig.WIN_TILE_READ) vram_bus.MA00 = vid_sig.TILE_READ_AB;
    /*p25.WUDO*/ if (vid_sig.WIN_TILE_READ) vram_bus.MA01 = vid_sig.TILE_Y0;
    /*p25.WAWE*/ if (vid_sig.WIN_TILE_READ) vram_bus.MA02 = vid_sig.TILE_Y1;
    /*p25.WOLU*/ if (vid_sig.WIN_TILE_READ) vram_bus.MA03 = vid_sig.TILE_Y2;

    /*p26.ASUM*/ if (vid_sig.BG_TILE_READ) vram_bus.MA00 = vid_sig.TILE_READ_AB;
    /*p26.EVAD*/ if (vid_sig.BG_TILE_READ) vram_bus.MA01 = TILE_Y0S;
    /*p26.DAHU*/ if (vid_sig.BG_TILE_READ) vram_bus.MA02 = TILE_Y1S;
    /*p26.DODE*/ if (vid_sig.BG_TILE_READ) vram_bus.MA03 = TILE_Y2S;

    /*p25.VAPY*/ if (vid_sig.TILE_READ) vram_bus.MA04 = pix_sig.BG_PIX_B0; // register reused
    /*p25.SEZU*/ if (vid_sig.TILE_READ) vram_bus.MA05 = pix_sig.BG_PIX_B1;
    /*p25.VEJY*/ if (vid_sig.TILE_READ) vram_bus.MA06 = pix_sig.BG_PIX_B2;
    /*p25.RUSA*/ if (vid_sig.TILE_READ) vram_bus.MA07 = pix_sig.BG_PIX_B3;
    /*p25.ROHA*/ if (vid_sig.TILE_READ) vram_bus.MA08 = pix_sig.BG_PIX_B4;
    /*p25.RESO*/ if (vid_sig.TILE_READ) vram_bus.MA09 = pix_sig.BG_PIX_B5;
    /*p25.SUVO*/ if (vid_sig.TILE_READ) vram_bus.MA10 = pix_sig.BG_PIX_B6;
    /*p25.TOBO*/ if (vid_sig.TILE_READ) vram_bus.MA11 = pix_sig.BG_PIX_B7;
    /*p25.VUZA*/ if (vid_sig.TILE_READ) vram_bus.MA12 = TILE_BANK;

    // small tri
    /*p25.WUKO*/ wire WIN_MAP_READn = not(vid_sig.WIN_MAP_READ);
    /*p27.XEJA*/ if (!WIN_MAP_READn) vram_bus.MA00 = vid_sig.MAP_X0;
    /*p27.XAMO*/ if (!WIN_MAP_READn) vram_bus.MA01 = vid_sig.MAP_X1;
    /*p27.XAHE*/ if (!WIN_MAP_READn) vram_bus.MA02 = vid_sig.MAP_X2;
    /*p27.XULO*/ if (!WIN_MAP_READn) vram_bus.MA03 = vid_sig.MAP_X3;
    /*p27.WUJU*/ if (!WIN_MAP_READn) vram_bus.MA04 = vid_sig.MAP_X4;
    /*p27.VYTO*/ if (!WIN_MAP_READn) vram_bus.MA05 = vid_sig.MAP_Y0;
    /*p27.VEHA*/ if (!WIN_MAP_READn) vram_bus.MA06 = vid_sig.MAP_Y1;
    /*p27.VACE*/ if (!WIN_MAP_READn) vram_bus.MA07 = vid_sig.MAP_Y2;
    /*p27.VOVO*/ if (!WIN_MAP_READn) vram_bus.MA08 = vid_sig.MAP_Y3;
    /*p27.VULO*/ if (!WIN_MAP_READn) vram_bus.MA09 = vid_sig.MAP_Y4;
    /*p27.VEVY*/ if (!WIN_MAP_READn) vram_bus.MA10 = vid_regs.LCDC_WINMAP;
    /*p27.VEZA*/ if (!WIN_MAP_READn) vram_bus.MA11 = spr_sig.P10_Bn;
    /*p27.VOGU*/ if (!WIN_MAP_READn) vram_bus.MA12 = spr_sig.P10_Bn;

    // small tri
    /*p26.BAFY*/ wire BG_MAP_READn  = not(vid_sig.BG_MAP_READ);
    /*p26.AXEP*/ if (!BG_MAP_READn) vram_bus.MA00 = MAP_X0S;
    /*p26.AFEB*/ if (!BG_MAP_READn) vram_bus.MA01 = MAP_X1S;
    /*p26.ALEL*/ if (!BG_MAP_READn) vram_bus.MA02 = MAP_X2S;
    /*p26.COLY*/ if (!BG_MAP_READn) vram_bus.MA03 = MAP_X3S;
    /*p26.AJAN*/ if (!BG_MAP_READn) vram_bus.MA04 = MAP_X4S;
    /*p26.DUHO*/ if (!BG_MAP_READn) vram_bus.MA05 = MAP_Y0S;
    /*p26.CASE*/ if (!BG_MAP_READn) vram_bus.MA06 = MAP_Y1S;
    /*p26.CYPO*/ if (!BG_MAP_READn) vram_bus.MA07 = MAP_Y2S;
    /*p26.CETA*/ if (!BG_MAP_READn) vram_bus.MA08 = MAP_Y3S;
    /*p26.DAFE*/ if (!BG_MAP_READn) vram_bus.MA09 = MAP_Y4S;
    /*p26.AMUV*/ if (!BG_MAP_READn) vram_bus.MA10 = vid_regs.LCDC_BGMAP;
    /*p26.COVE*/ if (!BG_MAP_READn) vram_bus.MA11 = spr_sig.P10_Bn;
    /*p26.COXO*/ if (!BG_MAP_READn) vram_bus.MA12 = spr_sig.P10_Bn;
  }

  // VRAM address bus
  {
    /*p04.AHOC*/ wire DMA_READ_VRAMo = not(dma_sig.DMA_READ_VRAM);
    /*p04.ECAL*/ if (!DMA_READ_VRAMo) vram_bus.MA00 = dma_sig.DMA_A00;
    /*p04.EGEZ*/ if (!DMA_READ_VRAMo) vram_bus.MA01 = dma_sig.DMA_A01;
    /*p04.FUHE*/ if (!DMA_READ_VRAMo) vram_bus.MA02 = dma_sig.DMA_A02;
    /*p04.FYZY*/ if (!DMA_READ_VRAMo) vram_bus.MA03 = dma_sig.DMA_A03;
    /*p04.DAMU*/ if (!DMA_READ_VRAMo) vram_bus.MA04 = dma_sig.DMA_A04;
    /*p04.DAVA*/ if (!DMA_READ_VRAMo) vram_bus.MA05 = dma_sig.DMA_A05;
    /*p04.ETEG*/ if (!DMA_READ_VRAMo) vram_bus.MA06 = dma_sig.DMA_A06;
    /*p04.EREW*/ if (!DMA_READ_VRAMo) vram_bus.MA07 = dma_sig.DMA_A07;
    /*p04.EVAX*/ if (!DMA_READ_VRAMo) vram_bus.MA08 = dma_sig.DMA_A08;
    /*p04.DUVE*/ if (!DMA_READ_VRAMo) vram_bus.MA09 = dma_sig.DMA_A09;
    /*p04.ERAF*/ if (!DMA_READ_VRAMo) vram_bus.MA10 = dma_sig.DMA_A10;
    /*p04.FUSY*/ if (!DMA_READ_VRAMo) vram_bus.MA11 = dma_sig.DMA_A11;
    /*p04.EXYF*/ if (!DMA_READ_VRAMo) vram_bus.MA12 = dma_sig.DMA_A12;

    /*p25.XANE*/ wire VRAM_LOCKn = nor(dma_sig.DMA_READ_VRAM, vid_sig.RENDERING_LATCH);
    /*p25.XEDU*/ wire VRAM_LOCK = not(VRAM_LOCKn);

    /*p25.XAKY*/ if (!VRAM_LOCK) vram_bus.MA00 = bus_tri.A00();
    /*p25.XUXU*/ if (!VRAM_LOCK) vram_bus.MA01 = bus_tri.A01();
    /*p25.XYNE*/ if (!VRAM_LOCK) vram_bus.MA02 = bus_tri.A02();
    /*p25.XODY*/ if (!VRAM_LOCK) vram_bus.MA03 = bus_tri.A03();
    /*p25.XECA*/ if (!VRAM_LOCK) vram_bus.MA04 = bus_tri.A04();
    /*p25.XOBA*/ if (!VRAM_LOCK) vram_bus.MA05 = bus_tri.A05();
    /*p25.XOPO*/ if (!VRAM_LOCK) vram_bus.MA06 = bus_tri.A06();
    /*p25.XYBO*/ if (!VRAM_LOCK) vram_bus.MA07 = bus_tri.A07();
    /*p25.RYSU*/ if (!VRAM_LOCK) vram_bus.MA08 = bus_tri.A08();
    /*p25.RESE*/ if (!VRAM_LOCK) vram_bus.MA09 = bus_tri.A09();
    /*p25.RUSE*/ if (!VRAM_LOCK) vram_bus.MA10 = bus_tri.A10();
    /*p25.RYNA*/ if (!VRAM_LOCK) vram_bus.MA11 = bus_tri.A11();
    /*p25.RUMO*/ if (!VRAM_LOCK) vram_bus.MA12 = bus_tri.A12();
  }
}

//-----------------------------------------------------------------------------
// VRAM data driver

#if 0
void Vram_tick2(const BusTristates& /*bus*/, const BusSignals& /*ctl*/, const Vram& /*vram*/, Vram& /*next*/, BusTristates& /*bus_out*/) {

  /*p25.ROVE*/ wire CBUS_TO_VBUS = not(ctl.CBUS_TO_VBUSn);
  /*p25.REGE*/ next.MD0_A = not(/*p25.SEFA*/ and(vram.MD0, CBUS_TO_VBUS));
  /*p25.RYKY*/ next.MD1_A = not(/*p25.SOGO*/ and(vram.MD1, CBUS_TO_VBUS));
  /*p25.RAZO*/ next.MD2_A = not(/*p25.SEFU*/ and(vram.MD2, CBUS_TO_VBUS));
  /*p25.RADA*/ next.MD3_A = not(/*p25.SUNA*/ and(vram.MD3, CBUS_TO_VBUS));
  /*p25.RYRO*/ next.MD4_A = not(/*p25.SUMO*/ and(vram.MD4, CBUS_TO_VBUS));
  /*p25.REVU*/ next.MD5_A = not(/*p25.SAZU*/ and(vram.MD5, CBUS_TO_VBUS));
  /*p25.REKU*/ next.MD6_A = not(/*p25.SAMO*/ and(vram.MD6, CBUS_TO_VBUS));
  /*p25.RYZE*/ next.MD7_A = not(/*p25.SUKE*/ and(vram.MD7, CBUS_TO_VBUS));

  /*p25.RURA*/ next.MD0_D = not(/*p25.SYNU*/ or (vram.MD0, ctl.CBUS_TO_VBUSn));
  /*p25.RULY*/ next.MD1_D = not(/*p25.SYMA*/ or (vram.MD1, ctl.CBUS_TO_VBUSn));
  /*p25.RARE*/ next.MD2_D = not(/*p25.ROKO*/ or (vram.MD2, ctl.CBUS_TO_VBUSn));
  /*p25.RODU*/ next.MD3_D = not(/*p25.SYBU*/ or (vram.MD3, ctl.CBUS_TO_VBUSn));
  /*p25.RUBE*/ next.MD4_D = not(/*p25.SAKO*/ or (vram.MD4, ctl.CBUS_TO_VBUSn));
  /*p25.RUMU*/ next.MD5_D = not(/*p25.SEJY*/ or (vram.MD5, ctl.CBUS_TO_VBUSn));
  /*p25.RYTY*/ next.MD6_D = not(/*p25.SEDO*/ or (vram.MD6, ctl.CBUS_TO_VBUSn));
  /*p25.RADY*/ next.MD7_D = not(/*p25.SAWU*/ or (vram.MD7, ctl.CBUS_TO_VBUSn));

  /*p25.RUGA*/ if (!ctl.MD_TO_Dn) bus_out.D0 = /*p25.RERY*/ not(vram.MD0);
  /*p25.ROTA*/ if (!ctl.MD_TO_Dn) bus_out.D1 = /*p25.RUNA*/ not(vram.MD1);
  /*p25.RYBU*/ if (!ctl.MD_TO_Dn) bus_out.D2 = /*p25.RONA*/ not(vram.MD2);
  /*p25.RAJU*/ if (!ctl.MD_TO_Dn) bus_out.D3 = /*p25.RUNO*/ not(vram.MD3);
  /*p25.TYJA*/ if (!ctl.MD_TO_Dn) bus_out.D4 = /*p25.SANA*/ not(vram.MD4);
  /*p25.REXU*/ if (!ctl.MD_TO_Dn) bus_out.D5 = /*p25.RORO*/ not(vram.MD5);
  /*p25.RUPY*/ if (!ctl.MD_TO_Dn) bus_out.D6 = /*p25.RABO*/ not(vram.MD6);
  /*p25.TOKU*/ if (!ctl.MD_TO_Dn) bus_out.D7 = /*p25.SAME*/ not(vram.MD7);

  // big tri
  /*p25.TEME*/ if (!ctl.CBUS_TO_VBUSn) next.MD0 = \1();
  /*p25.TEWU*/ if (!ctl.CBUS_TO_VBUSn) next.MD1 = \1();
  /*p25.TYGO*/ if (!ctl.CBUS_TO_VBUSn) next.MD2 = \1();
  /*p25.SOTE*/ if (!ctl.CBUS_TO_VBUSn) next.MD3 = \1();
  /*p25.SEKE*/ if (!ctl.CBUS_TO_VBUSn) next.MD4 = \1();
  /*p25.RUJO*/ if (!ctl.CBUS_TO_VBUSn) next.MD5 = \1();
  /*p25.TOFA*/ if (!ctl.CBUS_TO_VBUSn) next.MD6 = \1();
  /*p25.SUZA*/ if (!ctl.CBUS_TO_VBUSn) next.MD7 = \1();

  // small tri
  /*p25.RODY*/ if (!ctl.VEXT_TO_VBUSn) next.MD0 = vram_pins.MD0_C;
  /*p25.REBA*/ if (!ctl.VEXT_TO_VBUSn) next.MD1 = vram_pins.MD1_C;
  /*p25.RYDO*/ if (!ctl.VEXT_TO_VBUSn) next.MD2 = vram_pins.MD2_C;
  /*p25.REMO*/ if (!ctl.VEXT_TO_VBUSn) next.MD3 = vram_pins.MD3_C;
  /*p25.ROCE*/ if (!ctl.VEXT_TO_VBUSn) next.MD4 = vram_pins.MD4_C;
  /*p25.ROPU*/ if (!ctl.VEXT_TO_VBUSn) next.MD5 = vram_pins.MD5_C;
  /*p25.RETA*/ if (!ctl.VEXT_TO_VBUSn) next.MD6 = vram_pins.MD6_C;
  /*p25.RAKU*/ if (!ctl.VEXT_TO_VBUSn) next.MD7 = vram_pins.MD7_C;
}
#endif

//-----------------------------------------------------------------------------

};