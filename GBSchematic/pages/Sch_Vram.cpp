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
#include "Sch_Clocks.h"
#include "Sch_Debug.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Vram_tick(const BusControl& bus,
               const Pins& pins,
               const LCD& lcd,
               const Registers& regs,
               const Sprites& spr,
               const OAM& oam,
               const Video& vid,
               const Debug& dbg,
               const PixelPipe& pix,
               const Decoder& dec,
               const Clocks& clocks,

               const Vram& prev,
               Vram& next,
               BusControl& bus_out) {
  {
    /*p27.VYPO*/ wire VYPO = not(pins.P10_B);

    //----------
    // vid x/y + scroll x/y

    /*p26.FAFO*/ wire TILE_Y0C  = add_c(lcd.V0, regs.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1C  = add_c(lcd.V1, regs.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2C  = add_c(lcd.V2, regs.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0C   = add_c(lcd.V3, regs.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1C   = add_c(lcd.V4, regs.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2C   = add_c(lcd.V5, regs.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3C   = add_c(lcd.V6, regs.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4C   = add_c(lcd.V7, regs.SCY7, MAP_Y3C);

    /*p26.FAFO*/ wire TILE_Y0S  = add_s(lcd.V0, regs.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1S  = add_s(lcd.V1, regs.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2S  = add_s(lcd.V2, regs.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0S   = add_s(lcd.V3, regs.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1S   = add_s(lcd.V4, regs.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2S   = add_s(lcd.V5, regs.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3S   = add_s(lcd.V6, regs.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4S   = add_s(lcd.V7, regs.SCY7, MAP_Y3C);

    /*p26.ATAD*/ wire TILE_X0C  = add_c(vid.X0, regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1C  = add_c(vid.X1, regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2C  = add_c(vid.X2, regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0C   = add_c(vid.X3, regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1C   = add_c(vid.X4, regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2C   = add_c(vid.X5, regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3C   = add_c(vid.X6, regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4C   = add_c(vid.X7, regs.SCX7, MAP_X3C);

    /*p26.ATAD*/ wire TILE_X0S  = add_s(vid.X0, regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1S  = add_s(vid.X1, regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2S  = add_s(vid.X2, regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0S   = add_s(vid.X3, regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1S   = add_s(vid.X4, regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2S   = add_s(vid.X5, regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3S   = add_s(vid.X6, regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4S   = add_s(vid.X7, regs.SCX7, MAP_X3C);

    //----------


    {
      /*p29.ABON*/ wire SPRITE_READ = not(spr.SPRITE_READn);

      /*p29.WUKY*/ wire FLIP_Y = not(oam.OAM_A_D6);
      /*p29.XUQU*/ wire SPRITE_AB = not(!spr.SPR_DEL1);
      /*p29.FUFO*/ wire LCDC_SPSIZEn = not(regs.LCDC_SPSIZE);

      /*p29.WAGO*/ wire WAGO      = xor(FLIP_Y, spr.TS_LINE_0);
      /*p29.CYVU*/ wire SPRITE_Y0 = xor(FLIP_Y, spr.TS_LINE_1);
      /*p29.BORE*/ wire SPRITE_Y1 = xor(FLIP_Y, spr.TS_LINE_2);
      /*p29.BUVY*/ wire SPRITE_Y2 = xor(FLIP_Y, spr.TS_LINE_3);
      /*p29.GEJY*/ wire SPRITE_Y3 = amux2(LCDC_SPSIZEn, !oam.OAM_B_D0, regs.LCDC_SPSIZE,  WAGO);

      /*p29.ABEM*/ if (SPRITE_READ) bus_out.MA00 = SPRITE_AB;
      /*p29.BAXE*/ if (SPRITE_READ) bus_out.MA01 = SPRITE_Y0;
      /*p29.ARAS*/ if (SPRITE_READ) bus_out.MA02 = SPRITE_Y1;
      /*p29.AGAG*/ if (SPRITE_READ) bus_out.MA03 = SPRITE_Y2;
      /*p29.FAMU*/ if (SPRITE_READ) bus_out.MA04 = SPRITE_Y3;
      /*p29.FUGY*/ if (SPRITE_READ) bus_out.MA05 = oam.OAM_B_D1;
      /*p29.GAVO*/ if (SPRITE_READ) bus_out.MA06 = oam.OAM_B_D2;
      /*p29.WYGA*/ if (SPRITE_READ) bus_out.MA07 = oam.OAM_B_D3;
      /*p29.WUNE*/ if (SPRITE_READ) bus_out.MA08 = oam.OAM_B_D4;
      /*p29.GOTU*/ if (SPRITE_READ) bus_out.MA09 = oam.OAM_B_D5;
      /*p29.GEGU*/ if (SPRITE_READ) bus_out.MA10 = oam.OAM_B_D6;
      /*p29.XEHE*/ if (SPRITE_READ) bus_out.MA11 = oam.OAM_B_D7;
      /*p29.DYSO*/ if (SPRITE_READ) bus_out.MA12 = pins.P10_B;   // sprites always in low half of tile store
    }

    {
      /*p27.NOCU*/ wire NOCU = not(vid.WIN_RST);
      /*p27.PORE*/ wire PORE = not(NOCU);
      /*p27.WAZY*/ wire WAZY = not(PORE);
      /*p27.PORE*/ wire TILE_Y_CLKn = not(WAZY);
      /*p26.AXAD*/ wire TILE_Y_CLK  = not(TILE_Y_CLKn);

      /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(vid.BG_SEQ_xxxx4567);
      /*p27.XUHA*/ wire BG_SEQ_xxxx4567b = not(BG_SEQ_0123xxxx);

      // one of these is window

      /*p27.NETA*/ wire TILE_READa = and(vid.LONYb, vid.BG_SEQ_xx234567);
      /*p25.XUCY*/ wire TILE_READb = nand(TILE_READa, TILE_Y_CLKn);

      /*p26.ASUL*/ wire FETCH_TILEn = and(TILE_Y_CLK, TILE_READa);
      /*p26.BEJE*/ wire FETCH_TILE = not(FETCH_TILEn);

      /*p25.XONU*/ if (TILE_READb) bus_out.MA00 = BG_SEQ_xxxx4567b;
      /*p25.WUDO*/ if (TILE_READb) bus_out.MA01 = vid.TILE_Y0;
      /*p25.WAWE*/ if (TILE_READb) bus_out.MA02 = vid.TILE_Y1;
      /*p25.WOLU*/ if (TILE_READb) bus_out.MA03 = vid.TILE_Y2;

      /*p26.ASUM*/ if (FETCH_TILE) bus_out.MA00 = BG_SEQ_xxxx4567b;
      /*p26.EVAD*/ if (FETCH_TILE) bus_out.MA01 = TILE_Y0S;
      /*p26.DAHU*/ if (FETCH_TILE) bus_out.MA02 = TILE_Y1S;
      /*p26.DODE*/ if (FETCH_TILE) bus_out.MA03 = TILE_Y2S;

      /*p25.VUZA*/ wire TILE_BANK = nor(regs.LCDC_BGTILE, pix.BG_PIX_B7); // register reused

      /*p25.VAPY*/ if (TILE_READa) bus_out.MA04 = pix.BG_PIX_B0; // register reused
      /*p25.SEZU*/ if (TILE_READa) bus_out.MA05 = pix.BG_PIX_B1;
      /*p25.VEJY*/ if (TILE_READa) bus_out.MA06 = pix.BG_PIX_B2;
      /*p25.RUSA*/ if (TILE_READa) bus_out.MA07 = pix.BG_PIX_B3;
      /*p25.ROHA*/ if (TILE_READa) bus_out.MA08 = pix.BG_PIX_B4;
      /*p25.RESO*/ if (TILE_READa) bus_out.MA09 = pix.BG_PIX_B5;
      /*p25.SUVO*/ if (TILE_READa) bus_out.MA10 = pix.BG_PIX_B6;
      /*p25.TOBO*/ if (TILE_READa) bus_out.MA11 = pix.BG_PIX_B7;
      /*p25.VUZA*/ if (TILE_READa) bus_out.MA12 = TILE_BANK;

    }

#if 0
    {
      /*p27.PORE*/ wire BG_OR_WINa = not(prev.WAZY);
      /*p26.AXAD*/ wire BG_OR_WINn = not(BG_OR_WINa);

      /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(prev.BG_SEQ_xx234567);
      /*p27.POTU and */ wire POTU = and(prev.LONYb, BG_SEQ_01xxxxxx);

      /*p25.XEZE nand*/ wire WIN_MAP_READa = nand(POTU, BG_OR_WINa);
      /*p26.ACEN and */ wire BG_MAP_READa  = and (POTU, BG_OR_WINn);

      /*p25.WUKO*/ wire WIN_MAP_READn = not(WIN_MAP_READa);
      /*p26.BAFY*/ wire BG_MAP_READn  = not(BG_MAP_READa);

      {
        // small tri
        /*p27.XEJA*/ if (!WIN_MAP_READn) next.MA00 = prev.MAP_X0;
        /*p27.XAMO*/ if (!WIN_MAP_READn) next.MA01 = prev.MAP_X1;
        /*p27.XAHE*/ if (!WIN_MAP_READn) next.MA02 = prev.MAP_X2;
        /*p27.XULO*/ if (!WIN_MAP_READn) next.MA03 = prev.MAP_X3;
        /*p27.WUJU*/ if (!WIN_MAP_READn) next.MA04 = prev.MAP_X4;
        /*p27.VYTO*/ if (!WIN_MAP_READn) next.MA05 = prev.MAP_Y0;
        /*p27.VEHA*/ if (!WIN_MAP_READn) next.MA06 = prev.MAP_Y1;
        /*p27.VACE*/ if (!WIN_MAP_READn) next.MA07 = prev.MAP_Y2;
        /*p27.VOVO*/ if (!WIN_MAP_READn) next.MA08 = prev.MAP_Y3;
        /*p27.VULO*/ if (!WIN_MAP_READn) next.MA09 = prev.MAP_Y4;
        /*p27.VEVY*/ if (!WIN_MAP_READn) next.MA10 = regs.LCDC_WINMAP;
        /*p27.VEZA*/ if (!WIN_MAP_READn) next.MA11 = spr.P10_Bn;
        /*p27.VOGU*/ if (!WIN_MAP_READn) next.MA12 = spr.P10_Bn;
      }

      {
        // small tri
        /*p26.AXEP*/ if (!BG_MAP_READn) next.MA00 = MAP_X0S;
        /*p26.AFEB*/ if (!BG_MAP_READn) next.MA01 = MAP_X1S;
        /*p26.ALEL*/ if (!BG_MAP_READn) next.MA02 = MAP_X2S;
        /*p26.COLY*/ if (!BG_MAP_READn) next.MA03 = MAP_X3S;
        /*p26.AJAN*/ if (!BG_MAP_READn) next.MA04 = MAP_X4S;
        /*p26.DUHO*/ if (!BG_MAP_READn) next.MA05 = MAP_Y0S;
        /*p26.CASE*/ if (!BG_MAP_READn) next.MA06 = MAP_Y1S;
        /*p26.CYPO*/ if (!BG_MAP_READn) next.MA07 = MAP_Y2S;
        /*p26.CETA*/ if (!BG_MAP_READn) next.MA08 = MAP_Y3S;
        /*p26.DAFE*/ if (!BG_MAP_READn) next.MA09 = MAP_Y4S;
        /*p26.AMUV*/ if (!BG_MAP_READn) next.MA10 = regs.LCDC_BGMAP;
        /*p26.COVE*/ if (!BG_MAP_READn) next.MA11 = spr.P10_Bn;
        /*p26.COXO*/ if (!BG_MAP_READn) next.MA12 = spr.P10_Bn;
      }
    }
#endif
  }

#if 0
  {
    /*p29.SAKY*/     wire SAKY = nor(prev.SPR_SEQ1, prev.SPR_DEL1);
    /*p29.TYSO*/   wire SPRITE_READb = or(SAKY, prev.RENDERINGn); // seems wrong
    /*p29.TEXY*/ next.SPRITE_READn = not(SPRITE_READb);
  }

  {
    /*p25.TAVY*/   wire MOE_Cn = not(pins.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(dec.ADDR_VRAM, prev.sys.AFAS_xxxxEFGx);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_VRAM);
    /*p25.RYLU*/   wire RYLU = nand(CPU_VRAM_CLK2, vid.RENDERINGn);
    /*p27.LURY*/     wire LURY = and(!next.vid.BG_SEQ5_SYNC, vid.RENDERING);
    /*p27.LONY*/   wire LONY = and(LURY, prev.vid.BG_SEQ_RSTn);
    /*p27.MYMA*/   wire MYMA = not(LONY);
    /*p25.APAM*/   wire DMA_READ_VRAMn = not(prev.dma.DMA_READ_VRAM);

    /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, prev.SPR_SEQ_5_SYNCn);
    /*p25.SOHO*/   wire SOHO = and(SPR_SEQ_5_TRIG, prev.SPRITE_READn);
    /*p25.RAWA*/ wire RAWA = not(SOHO);

    /*p25.RACU*/ wire MOE = and(RYLU, RAWA, MYMA, DMA_READ_VRAMn);
    /*p25.SEMA*/ wire MOE_An = and(MOE, DBG_VRAMn);
    /*p25.RUTE*/ wire MOE_Dn = or (MOE, DBG_VRAM); // schematic wrong, second input is RACU
    /*p25.REFO*/ next.MOE_A = not(MOE_An);
    /*p25.SAHA*/ next.MOE_D = not(MOE_Dn);
  }


  {
    /*p25.TEFY*/     wire MCS_Cn = not(pins.MCS_C);
    /*p25.TUCA*/     wire CPU_VRAM_RD    = and (dec.ADDR_VRAM, prev.bus.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/   wire CPU_VRAM_RD2   = mux2(MCS_Cn, CPU_VRAM_RD , DBG_VRAM);
    /*p25.ROPY*/   wire RENDERINGo = not(vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);
    /*p25.SUTU*/     wire MCS = nor(prev.vid.LONYb, prev.dma.DMA_READ_VRAM, prev.spr.SPRITE_READn, SERE);
    /*p25.TODE*/   wire MCS_An = and(MCS, DBG_VRAMn);
    /*p25.SEWO*/   wire MCS_Dn = or (MCS, DBG_VRAM);
    /*p25.SOKY*/ next.MCS_A = not(MCS_An);
    /*p25.SETY*/ next.MCS_D = not(MCS_Dn);
  }

  {
    /*p25.TEFY*/     wire MCS_Cn = not(pins.MCS_C);
    /*p25.TUCA*/     wire CPU_VRAM_RD    = and (dec.ADDR_VRAM, prev.bus.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/   wire CPU_VRAM_RD2   = mux2(MCS_Cn, CPU_VRAM_RD , DBG_VRAM);
    /*p25.ROPY*/   wire RENDERINGo = not(vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);
    /*p25.SUDO*/       wire MWR_Cn = not(pins.MWR_C);
    /*p01.AREV*/           wire AREV = nand(prev.cpu.CPU_RAW_WR, prev.sys.AFAS_xxxxEFGx);
    /*p01.APOV*/         wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p25.TUJA*/       wire CPU_VRAM_WR    = and (dec.ADDR_VRAM, CPU_WR_xxxxEFGx);
    /*p25.TYJY*/     wire CPU_VRAM_WR2   = mux2(MWR_Cn, CPU_VRAM_WR , DBG_VRAM);
    /*p25.SOHY*/   wire MWR = nand(CPU_VRAM_WR2, SERE);
    /*p25.TAXY*/   wire MWR_An = and(MWR, DBG_VRAMn);
    /*p25.SOFY*/   wire MWR_Dn = or (MWR, DBG_VRAM);
    /*p25.SYSY*/ next.MWR_A = not(MWR_An);
    /*p25.RAGU*/ next.MWR_D = not(MWR_Dn);
  }
#endif


  {
    /*p25.TAVY*/   wire MOE_Cn = not(prev.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(dec.ADDR_VRAM, clocks.AFAS_xxxxEFGx);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, dbg.DBG_VRAM);
    // wires traced wrong? doesn't look like it
    // RUVY = not(SALE)
    // SAZO = and(SERE, RUVY)
    // RYJE = not(SAZO)
    // REVO = not(RYJE)

    // RELA = or(...., ???)

    // this can't be right, md_outd and md_outc are effectively the same signal
    // yeah that's what it looks like on the schematic
    /*p25.RUVY*/       wire CPU_VRAM_CLK2n = not(CPU_VRAM_CLK2);
    /*p25.SAZO*/     wire MD_OUTd = and(CPU_VRAM_CLK2n, bus.SERE);
    /*p25.RYJE*/   wire MD_INb  = not(MD_OUTd);
    /*p25.REVO*/ wire MD_OUTc = not(MD_INb);

    // so it must be that some tribuffers are active-high, some are active-low.....

    // TEME - wires on 1 5 6 9 10
    // REBA - much smaller tribuffer

    /*p25.RELA*/ wire VEXT_TO_VBUSb  = or(MD_OUTc, MD_OUTd);
    /*p25.ROCY*/ wire CBUS_TO_VBUSb = and(MD_OUTc, MD_OUTd);

    /*p25.RAHU*/ wire CBUS_TO_VBUSn = not(CBUS_TO_VBUSb);
    /*p25.ROVE*/ wire CBUS_TO_VBUSa = not(CBUS_TO_VBUSn);

    /*p25.RENA*/ wire VEXT_TO_VBUSn = not(VEXT_TO_VBUSb);

    /*p25.ROFA*/ next.MD0_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD1_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD2_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD3_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD4_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD5_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD6_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.MD7_B = not(VEXT_TO_VBUSn);

    /*p25.REGE*/ next.MD0_A = not(/*p25.SEFA*/ and(bus.MD0, CBUS_TO_VBUSa));
    /*p25.RYKY*/ next.MD1_A = not(/*p25.SOGO*/ and(bus.MD1, CBUS_TO_VBUSa));
    /*p25.RAZO*/ next.MD2_A = not(/*p25.SEFU*/ and(bus.MD2, CBUS_TO_VBUSa));
    /*p25.RADA*/ next.MD3_A = not(/*p25.SUNA*/ and(bus.MD3, CBUS_TO_VBUSa));
    /*p25.RYRO*/ next.MD4_A = not(/*p25.SUMO*/ and(bus.MD4, CBUS_TO_VBUSa));
    /*p25.REVU*/ next.MD5_A = not(/*p25.SAZU*/ and(bus.MD5, CBUS_TO_VBUSa));
    /*p25.REKU*/ next.MD6_A = not(/*p25.SAMO*/ and(bus.MD6, CBUS_TO_VBUSa));
    /*p25.RYZE*/ next.MD7_A = not(/*p25.SUKE*/ and(bus.MD7, CBUS_TO_VBUSa));

    /*p25.RURA*/ next.MD0_D = not(/*p25.SYNU*/ or (bus.MD0, CBUS_TO_VBUSn));
    /*p25.RULY*/ next.MD1_D = not(/*p25.SYMA*/ or (bus.MD1, CBUS_TO_VBUSn));
    /*p25.RARE*/ next.MD2_D = not(/*p25.ROKO*/ or (bus.MD2, CBUS_TO_VBUSn));
    /*p25.RODU*/ next.MD3_D = not(/*p25.SYBU*/ or (bus.MD3, CBUS_TO_VBUSn));
    /*p25.RUBE*/ next.MD4_D = not(/*p25.SAKO*/ or (bus.MD4, CBUS_TO_VBUSn));
    /*p25.RUMU*/ next.MD5_D = not(/*p25.SEJY*/ or (bus.MD5, CBUS_TO_VBUSn));
    /*p25.RYTY*/ next.MD6_D = not(/*p25.SEDO*/ or (bus.MD6, CBUS_TO_VBUSn));
    /*p25.RADY*/ next.MD7_D = not(/*p25.SAWU*/ or (bus.MD7, CBUS_TO_VBUSn));
  }

  /*p25.LEXE*/ next.MA00_D = not(/*p25.MYFU*/ not(bus.MA00));
  /*p25.LOZU*/ next.MA01_D = not(/*p25.MASA*/ not(bus.MA01));
  /*p25.LACA*/ next.MA02_D = not(/*p25.MYRE*/ not(bus.MA02));
  /*p25.LUVO*/ next.MA03_D = not(/*p25.MAVU*/ not(bus.MA03));
  /*p25.LOLY*/ next.MA04_D = not(/*p25.MEPA*/ not(bus.MA04));
  /*p25.LALO*/ next.MA05_D = not(/*p25.MYSA*/ not(bus.MA05));
  /*p25.LEFA*/ next.MA06_D = not(/*p25.MEWY*/ not(bus.MA06));
  /*p25.LUBY*/ next.MA07_D = not(/*p25.MUME*/ not(bus.MA07));
  /*p25.TUJY*/ next.MA08_D = not(/*p25.VOVA*/ not(bus.MA08));
  /*p25.TAGO*/ next.MA09_D = not(/*p25.VODE*/ not(bus.MA09));
  /*p25.NUVA*/ next.MA10_D = not(/*p25.RUKY*/ not(bus.MA10));
  /*p25.PEDU*/ next.MA11_D = not(/*p25.RUMA*/ not(bus.MA11));
  /*p25.PONY*/ next.MA12_D = not(/*p25.REHO*/ not(bus.MA12));
}

//-----------------------------------------------------------------------------

};