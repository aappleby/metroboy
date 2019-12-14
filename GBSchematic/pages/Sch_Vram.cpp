#include "Sch_Vram.h"
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Vram_tick(const Vram& a, const Vram& b, Vram& next) {
  (void)a;
  (void)b;
  (void)next;


#if 0
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

    /*p26.ATAD*/ wire TILE_X0C  = add_c(prev.X0, regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1C  = add_c(prev.X1, regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2C  = add_c(prev.X2, regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0C   = add_c(prev.X3, regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1C   = add_c(prev.X4, regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2C   = add_c(prev.X5, regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3C   = add_c(prev.X6, regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4C   = add_c(prev.X7, regs.SCX7, MAP_X3C);

    /*p26.ATAD*/ wire TILE_X0S  = add_s(prev.X0, regs.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1S  = add_s(prev.X1, regs.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2S  = add_s(prev.X2, regs.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0S   = add_s(prev.X3, regs.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1S   = add_s(prev.X4, regs.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2S   = add_s(prev.X5, regs.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3S   = add_s(prev.X6, regs.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4S   = add_s(prev.X7, regs.SCX7, MAP_X3C);

    //----------
    // vram read mux

    {
      /*p25.XANE*/ wire VRAM_LOCK = nor(prev.dma.DMA_READ_VRAM, prev.RENDERING);
      /*p25.XEDU*/ wire VRAM_LOCKn = not(VRAM_LOCK);

      /*p25.XAKY*/ if (VRAM_LOCKn) next.vrm.MA00 = bus_in.A00;
      /*p25.XUXU*/ if (VRAM_LOCKn) next.vrm.MA01 = bus_in.A01;
      /*p25.XYNE*/ if (VRAM_LOCKn) next.vrm.MA02 = bus_in.A02;
      /*p25.XODY*/ if (VRAM_LOCKn) next.vrm.MA03 = bus_in.A03;
      /*p25.XECA*/ if (VRAM_LOCKn) next.vrm.MA04 = bus_in.A04;
      /*p25.XOBA*/ if (VRAM_LOCKn) next.vrm.MA05 = bus_in.A05;
      /*p25.XOPO*/ if (VRAM_LOCKn) next.vrm.MA06 = bus_in.A06;
      /*p25.XYBO*/ if (VRAM_LOCKn) next.vrm.MA07 = bus_in.A07;
      /*p25.RYSU*/ if (VRAM_LOCKn) next.vrm.MA08 = bus_in.A08;
      /*p25.RESE*/ if (VRAM_LOCKn) next.vrm.MA09 = bus_in.A09;
      /*p25.RUSE*/ if (VRAM_LOCKn) next.vrm.MA10 = bus_in.A10;
      /*p25.RYNA*/ if (VRAM_LOCKn) next.vrm.MA11 = bus_in.A11;
      /*p25.RUMO*/ if (VRAM_LOCKn) next.vrm.MA12 = bus_in.A12;
    }

    {
      /*p29.ABON*/ wire SPRITE_READ = not(prev.spr.SPRITE_READn);

      /*p29.WUKY*/ wire FLIP_Y = not(prev.spr.OAM_A_D6);
      /*p29.XUQU*/ wire SPRITE_AB = not(!prev.spr.SPR_DEL1);

      /*p29.WAGO*/ wire WAGO      = xor(FLIP_Y, prev.spr.TS_LINE_0);
      /*p29.CYVU*/ wire SPRITE_Y0 = xor(FLIP_Y, prev.spr.TS_LINE_1);
      /*p29.BORE*/ wire SPRITE_Y1 = xor(FLIP_Y, prev.spr.TS_LINE_2);
      /*p29.BUVY*/ wire SPRITE_Y2 = xor(FLIP_Y, prev.spr.TS_LINE_3);

      /*p29.FUFO*/ wire LCDC_SPSIZEn = not(prev.reg.LCDC_SPSIZE);
      /*p29.GEJY*/ wire SPRITE_Y3 = amux2(LCDC_SPSIZEn, !prev.spr.OAM_B_D0, prev.reg.LCDC_SPSIZE,  WAGO);

      /*p29.ABEM*/ if (SPRITE_READ) next.vrm.MA00 = SPRITE_AB;
      /*p29.BAXE*/ if (SPRITE_READ) next.vrm.MA01 = SPRITE_Y0;
      /*p29.ARAS*/ if (SPRITE_READ) next.vrm.MA02 = SPRITE_Y1;
      /*p29.AGAG*/ if (SPRITE_READ) next.vrm.MA03 = SPRITE_Y2;
      /*p29.FAMU*/ if (SPRITE_READ) next.vrm.MA04 = SPRITE_Y3;
      /*p29.FUGY*/ if (SPRITE_READ) next.vrm.MA05 = prev.spr.OAM_B_D1;
      /*p29.GAVO*/ if (SPRITE_READ) next.vrm.MA06 = prev.spr.OAM_B_D2;
      /*p29.WYGA*/ if (SPRITE_READ) next.vrm.MA07 = prev.spr.OAM_B_D3;
      /*p29.WUNE*/ if (SPRITE_READ) next.vrm.MA08 = prev.spr.OAM_B_D4;
      /*p29.GOTU*/ if (SPRITE_READ) next.vrm.MA09 = prev.spr.OAM_B_D5;
      /*p29.GEGU*/ if (SPRITE_READ) next.vrm.MA10 = prev.spr.OAM_B_D6;
      /*p29.XEHE*/ if (SPRITE_READ) next.vrm.MA11 = prev.spr.OAM_B_D7;
      /*p29.DYSO*/ if (SPRITE_READ) next.vrm.MA12 = pins.P10_B;   // sprites always in low half of tile store
    }

    {
      /*p27.PORE*/ wire TILE_Y_CLKn = not(prev.WAZY);
      /*p26.AXAD*/ wire TILE_Y_CLK  = not(TILE_Y_CLKn);

      /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(prev.BG_SEQ_xxxx4567);
      /*p27.XUHA*/ wire BG_SEQ_xxxx4567b = not(BG_SEQ_0123xxxx);

      // one of these is window

      /*p27.NETA*/ wire TILE_READa = and(prev.LONYb, prev.BG_SEQ_xx234567);
      /*p25.XUCY*/ wire TILE_READb = nand(TILE_READa, TILE_Y_CLKn);

      /*p26.ASUL*/ wire FETCH_TILEn = and(TILE_Y_CLK, TILE_READa);
      /*p26.BEJE*/ wire FETCH_TILE = not(FETCH_TILEn);

      /*p25.XONU*/ if (TILE_READb) next.vrm.MA00 = BG_SEQ_xxxx4567b;
      /*p25.WUDO*/ if (TILE_READb) next.vrm.MA01 = prev.TILE_Y0;
      /*p25.WAWE*/ if (TILE_READb) next.vrm.MA02 = prev.TILE_Y1;
      /*p25.WOLU*/ if (TILE_READb) next.vrm.MA03 = prev.TILE_Y2;

      /*p26.ASUM*/ if (FETCH_TILE) next.vrm.MA00 = BG_SEQ_xxxx4567b;
      /*p26.EVAD*/ if (FETCH_TILE) next.vrm.MA01 = TILE_Y0S;
      /*p26.DAHU*/ if (FETCH_TILE) next.vrm.MA02 = TILE_Y1S;
      /*p26.DODE*/ if (FETCH_TILE) next.vrm.MA03 = TILE_Y2S;

      /*p25.VUZA*/ wire TILE_BANK = nor(prev.reg.LCDC_BGTILE, prev.pix.BG_PIX_B7); // register reused

      /*p25.VAPY*/ if (TILE_READa) next.vrm.MA04 = prev.pix.BG_PIX_B0; // register reused
      /*p25.SEZU*/ if (TILE_READa) next.vrm.MA05 = prev.pix.BG_PIX_B1;
      /*p25.VEJY*/ if (TILE_READa) next.vrm.MA06 = prev.pix.BG_PIX_B2;
      /*p25.RUSA*/ if (TILE_READa) next.vrm.MA07 = prev.pix.BG_PIX_B3;
      /*p25.ROHA*/ if (TILE_READa) next.vrm.MA08 = prev.pix.BG_PIX_B4;
      /*p25.RESO*/ if (TILE_READa) next.vrm.MA09 = prev.pix.BG_PIX_B5;
      /*p25.SUVO*/ if (TILE_READa) next.vrm.MA10 = prev.pix.BG_PIX_B6;
      /*p25.TOBO*/ if (TILE_READa) next.vrm.MA11 = prev.pix.BG_PIX_B7;
      /*p25.VUZA*/ if (TILE_READa) next.vrm.MA12 = TILE_BANK;

    }

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
        /*p27.XEJA*/ if (!WIN_MAP_READn) next.vrm.MA00 = prev.MAP_X0;
        /*p27.XAMO*/ if (!WIN_MAP_READn) next.vrm.MA01 = prev.MAP_X1;
        /*p27.XAHE*/ if (!WIN_MAP_READn) next.vrm.MA02 = prev.MAP_X2;
        /*p27.XULO*/ if (!WIN_MAP_READn) next.vrm.MA03 = prev.MAP_X3;
        /*p27.WUJU*/ if (!WIN_MAP_READn) next.vrm.MA04 = prev.MAP_X4;
        /*p27.VYTO*/ if (!WIN_MAP_READn) next.vrm.MA05 = prev.MAP_Y0;
        /*p27.VEHA*/ if (!WIN_MAP_READn) next.vrm.MA06 = prev.MAP_Y1;
        /*p27.VACE*/ if (!WIN_MAP_READn) next.vrm.MA07 = prev.MAP_Y2;
        /*p27.VOVO*/ if (!WIN_MAP_READn) next.vrm.MA08 = prev.MAP_Y3;
        /*p27.VULO*/ if (!WIN_MAP_READn) next.vrm.MA09 = prev.MAP_Y4;
        /*p27.VEVY*/ if (!WIN_MAP_READn) next.vrm.MA10 = prev.reg.LCDC_WINMAP;
        /*p27.VEZA*/ if (!WIN_MAP_READn) next.vrm.MA11 = prev.spr.P10_Bn;
        /*p27.VOGU*/ if (!WIN_MAP_READn) next.vrm.MA12 = prev.spr.P10_Bn;
      }

      {
        // small tri
        /*p26.AXEP*/ if (!BG_MAP_READn) next.vrm.MA00 = MAP_X0S;
        /*p26.AFEB*/ if (!BG_MAP_READn) next.vrm.MA01 = MAP_X1S;
        /*p26.ALEL*/ if (!BG_MAP_READn) next.vrm.MA02 = MAP_X2S;
        /*p26.COLY*/ if (!BG_MAP_READn) next.vrm.MA03 = MAP_X3S;
        /*p26.AJAN*/ if (!BG_MAP_READn) next.vrm.MA04 = MAP_X4S;
        /*p26.DUHO*/ if (!BG_MAP_READn) next.vrm.MA05 = MAP_Y0S;
        /*p26.CASE*/ if (!BG_MAP_READn) next.vrm.MA06 = MAP_Y1S;
        /*p26.CYPO*/ if (!BG_MAP_READn) next.vrm.MA07 = MAP_Y2S;
        /*p26.CETA*/ if (!BG_MAP_READn) next.vrm.MA08 = MAP_Y3S;
        /*p26.DAFE*/ if (!BG_MAP_READn) next.vrm.MA09 = MAP_Y4S;
        /*p26.AMUV*/ if (!BG_MAP_READn) next.vrm.MA10 = prev.reg.LCDC_BGMAP;
        /*p26.COVE*/ if (!BG_MAP_READn) next.vrm.MA11 = prev.spr.P10_Bn;
        /*p26.COXO*/ if (!BG_MAP_READn) next.vrm.MA12 = prev.spr.P10_Bn;
      }
    }
  }
#endif

#if 0
  {
    /*p29.SAKY*/     wire SAKY = nor(b.SPR_SEQ1, b.SPR_DEL1);
    /*p29.TYSO*/   wire SPRITE_READb = or(SAKY, b.RENDERINGn); // seems wrong
    /*p29.TEXY*/ next.SPRITE_READn = not(SPRITE_READb);
  }

  {
    /*p25.TAVY*/   wire MOE_Cn = not(b.pin.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(b.vid.ADDR_VRAM, b.sys.AFAS_xxxxEFGx);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_TUTO);
    /*p25.RYLU*/   wire RYLU = nand(CPU_VRAM_CLK2, b.vid.RENDERINGn);
    /*p27.LURY*/     wire LURY = and(!next.vid.BG_SEQ5_SYNC, b.vid.RENDERING);
    /*p27.LONY*/   wire LONY = and(LURY, b.vid.BG_SEQ_RSTn);
    /*p27.MYMA*/   wire MYMA = not(LONY);
    /*p25.APAM*/   wire DMA_READ_VRAMn = not(b.dma.DMA_READ_VRAM);

    /*p29.TACU*/     wire SPR_SEQ_5_TRIG = nand(SEQ_5, b.SPR_SEQ_5_SYNCn);
    /*p25.SOHO*/   wire SOHO = and(SPR_SEQ_5_TRIG, b.SPRITE_READn);
    /*p25.RAWA*/ wire RAWA = not(SOHO);

    /*p25.RACU*/ wire MOE = and(RYLU, RAWA, MYMA, DMA_READ_VRAMn);
    /*p25.SEMA*/ wire MOE_An = and(MOE, DBG_TUTOn);
    /*p25.RUTE*/ wire MOE_Dn = or (MOE, DBG_TUTO); // schematic wrong, second input is RACU
    /*p25.REFO*/ next.vrm.MOE_A = not(MOE_An);
    /*p25.SAHA*/ next.vrm.MOE_D = not(MOE_Dn);
  }


  {
    /*p25.TEFY*/     wire MCS_Cn = not(b.pin.MCS_C);
    /*p25.TUCA*/     wire CPU_VRAM_RD    = and (b.vid.ADDR_VRAM, b.ctl.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/   wire CPU_VRAM_RD2   = mux2(MCS_Cn, CPU_VRAM_RD , DBG_TUTO);
    /*p25.ROPY*/   wire RENDERINGo = not(b.vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);
    /*p25.SUTU*/     wire MCS = nor(b.vid.LONYb, b.dma.DMA_READ_VRAM, b.spr.SPRITE_READn, SERE);
    /*p25.TODE*/   wire MCS_An = and(MCS, DBG_TUTOn);
    /*p25.SEWO*/   wire MCS_Dn = or (MCS, DBG_TUTO);
    /*p25.SOKY*/ next.vrm.MCS_A = not(MCS_An);
    /*p25.SETY*/ next.vrm.MCS_D = not(MCS_Dn);
  }

  {
    /*p25.TEFY*/     wire MCS_Cn = not(b.pin.MCS_C);
    /*p25.TUCA*/     wire CPU_VRAM_RD    = and (b.vid.ADDR_VRAM, b.ctl.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/   wire CPU_VRAM_RD2   = mux2(MCS_Cn, CPU_VRAM_RD , DBG_TUTO);
    /*p25.ROPY*/   wire RENDERINGo = not(b.vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);
    /*p25.SUDO*/       wire MWR_Cn = not(b.pin.MWR_C);
    /*p01.AREV*/           wire AREV = nand(b.cpu.CPU_RAW_WR, b.sys.AFAS_xxxxEFGx);
    /*p01.APOV*/         wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p25.TUJA*/       wire CPU_VRAM_WR    = and (b.vid.ADDR_VRAM, CPU_WR_xxxxEFGx);
    /*p25.TYJY*/     wire CPU_VRAM_WR2   = mux2(MWR_Cn, CPU_VRAM_WR , DBG_TUTO);
    /*p25.SOHY*/   wire MWR = nand(CPU_VRAM_WR2, SERE);
    /*p25.TAXY*/   wire MWR_An = and(MWR, DBG_TUTOn);
    /*p25.SOFY*/   wire MWR_Dn = or (MWR, DBG_TUTO);
    /*p25.SYSY*/ next.vrm.MWR_A = not(MWR_An);
    /*p25.RAGU*/ next.vrm.MWR_D = not(MWR_Dn);
  }


  {
    /*p25.TAVY*/   wire MOE_Cn = not(b.pin.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(b.vid.ADDR_VRAM, b.sys.AFAS_xxxxEFGx);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_TUTO);
    // wires traced wrong? doesn't look like it
    // RUVY = not(SALE)
    // SAZO = and(SERE, RUVY)
    // RYJE = not(SAZO)
    // REVO = not(RYJE)

    // RELA = or(...., ???)

    // this can't be right, md_outd and md_outc are effectively the same signal
    // yeah that's what it looks like on the schematic
    /*p25.RUVY*/       wire CPU_VRAM_CLK2n = not(CPU_VRAM_CLK2);
    /*p25.SAZO*/     wire MD_OUTd = and(CPU_VRAM_CLK2n, SERE);
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

    /*p25.ROFA*/ next.vrm.MD0_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD1_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD2_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD3_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD4_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD5_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD6_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ next.vrm.MD7_B = not(VEXT_TO_VBUSn);

    /*p25.REGE*/ next.vrm.MD0_A = not(/*p25.SEFA*/ and(b.MD0, CBUS_TO_VBUSa));
    /*p25.RYKY*/ next.vrm.MD1_A = not(/*p25.SOGO*/ and(b.MD1, CBUS_TO_VBUSa));
    /*p25.RAZO*/ next.vrm.MD2_A = not(/*p25.SEFU*/ and(b.MD2, CBUS_TO_VBUSa));
    /*p25.RADA*/ next.vrm.MD3_A = not(/*p25.SUNA*/ and(b.MD3, CBUS_TO_VBUSa));
    /*p25.RYRO*/ next.vrm.MD4_A = not(/*p25.SUMO*/ and(b.MD4, CBUS_TO_VBUSa));
    /*p25.REVU*/ next.vrm.MD5_A = not(/*p25.SAZU*/ and(b.MD5, CBUS_TO_VBUSa));
    /*p25.REKU*/ next.vrm.MD6_A = not(/*p25.SAMO*/ and(b.MD6, CBUS_TO_VBUSa));
    /*p25.RYZE*/ next.vrm.MD7_A = not(/*p25.SUKE*/ and(b.MD7, CBUS_TO_VBUSa));

    /*p25.RURA*/ next.vrm.MD0_D = not(/*p25.SYNU*/ or (b.MD0, CBUS_TO_VBUSn));
    /*p25.RULY*/ next.vrm.MD1_D = not(/*p25.SYMA*/ or (b.MD1, CBUS_TO_VBUSn));
    /*p25.RARE*/ next.vrm.MD2_D = not(/*p25.ROKO*/ or (b.MD2, CBUS_TO_VBUSn));
    /*p25.RODU*/ next.vrm.MD3_D = not(/*p25.SYBU*/ or (b.MD3, CBUS_TO_VBUSn));
    /*p25.RUBE*/ next.vrm.MD4_D = not(/*p25.SAKO*/ or (b.MD4, CBUS_TO_VBUSn));
    /*p25.RUMU*/ next.vrm.MD5_D = not(/*p25.SEJY*/ or (b.MD5, CBUS_TO_VBUSn));
    /*p25.RYTY*/ next.vrm.MD6_D = not(/*p25.SEDO*/ or (b.MD6, CBUS_TO_VBUSn));
    /*p25.RADY*/ next.vrm.MD7_D = not(/*p25.SAWU*/ or (b.MD7, CBUS_TO_VBUSn));

    // big tri
    /*p25.TEME*/ if (!CBUS_TO_VBUSn) next.MD0 = b.D0;
    /*p25.TEWU*/ if (!CBUS_TO_VBUSn) next.MD1 = b.D1;
    /*p25.TYGO*/ if (!CBUS_TO_VBUSn) next.MD2 = b.D2;
    /*p25.SOTE*/ if (!CBUS_TO_VBUSn) next.MD3 = b.D3;
    /*p25.SEKE*/ if (!CBUS_TO_VBUSn) next.MD4 = b.D4;
    /*p25.RUJO*/ if (!CBUS_TO_VBUSn) next.MD5 = b.D5;
    /*p25.TOFA*/ if (!CBUS_TO_VBUSn) next.MD6 = b.D6;
    /*p25.SUZA*/ if (!CBUS_TO_VBUSn) next.MD7 = b.D7;

    // small tri
    /*p25.RODY*/ if (!VEXT_TO_VBUSn) next.MD0 = b.vrm.MD0_C;
    /*p25.REBA*/ if (!VEXT_TO_VBUSn) next.MD1 = b.vrm.MD1_C;
    /*p25.RYDO*/ if (!VEXT_TO_VBUSn) next.MD2 = b.vrm.MD2_C;
    /*p25.REMO*/ if (!VEXT_TO_VBUSn) next.MD3 = b.vrm.MD3_C;
    /*p25.ROCE*/ if (!VEXT_TO_VBUSn) next.MD4 = b.vrm.MD4_C;
    /*p25.ROPU*/ if (!VEXT_TO_VBUSn) next.MD5 = b.vrm.MD5_C;
    /*p25.RETA*/ if (!VEXT_TO_VBUSn) next.MD6 = b.vrm.MD6_C;
    /*p25.RAKU*/ if (!VEXT_TO_VBUSn) next.MD7 = b.vrm.MD7_C;
  }

  //----------
  // Memory address bus driver pin driver

  /*p25.LEXE*/ next.vrm.MA00_D = not(/*p25.MYFU*/ not(b.vrm.MA00));
  /*p25.LOZU*/ next.vrm.MA01_D = not(/*p25.MASA*/ not(b.vrm.MA01));
  /*p25.LACA*/ next.vrm.MA02_D = not(/*p25.MYRE*/ not(b.vrm.MA02));
  /*p25.LUVO*/ next.vrm.MA03_D = not(/*p25.MAVU*/ not(b.vrm.MA03));
  /*p25.LOLY*/ next.vrm.MA04_D = not(/*p25.MEPA*/ not(b.vrm.MA04));
  /*p25.LALO*/ next.vrm.MA05_D = not(/*p25.MYSA*/ not(b.vrm.MA05));
  /*p25.LEFA*/ next.vrm.MA06_D = not(/*p25.MEWY*/ not(b.vrm.MA06));
  /*p25.LUBY*/ next.vrm.MA07_D = not(/*p25.MUME*/ not(b.vrm.MA07));
  /*p25.TUJY*/ next.vrm.MA08_D = not(/*p25.VOVA*/ not(b.vrm.MA08));
  /*p25.TAGO*/ next.vrm.MA09_D = not(/*p25.VODE*/ not(b.vrm.MA09));
  /*p25.NUVA*/ next.vrm.MA10_D = not(/*p25.RUKY*/ not(b.vrm.MA10));
  /*p25.PEDU*/ next.vrm.MA11_D = not(/*p25.RUMA*/ not(b.vrm.MA11));
  /*p25.PONY*/ next.vrm.MA12_D = not(/*p25.REHO*/ not(b.vrm.MA12));

  {
    /*p25.TUTO*/ wire DBG_TUTO = and(b.dbg.MODE_DBG2, !b.vid.DBG_SOTO);
    /*p25.RACO*/ wire DBG_TUTOn = not(DBG_TUTO);
    /*p25.TEFY*/ wire MCS_Cn = not(b.pin.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (b.vid.ADDR_VRAM, b.ctl.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, DBG_TUTO);
    /*p25.ROPY*/ wire RENDERINGo = not(b.vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);

    /*p04.DECY*/ wire FROM_CPU5n = not(b.cpu.FROM_CPU5);
    /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);
    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.ctl.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p25.TYVY*/ wire MD_TO_Dn = nand(SERE, CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire MD_TO_D = not(MD_TO_Dn);

    /*p25.RUGA*/ if (MD_TO_D) next.D0 = /*p25.RERY*/ not(b.vrm.MD0);
    /*p25.ROTA*/ if (MD_TO_D) next.D1 = /*p25.RUNA*/ not(b.vrm.MD1);
    /*p25.RYBU*/ if (MD_TO_D) next.D2 = /*p25.RONA*/ not(b.vrm.MD2);
    /*p25.RAJU*/ if (MD_TO_D) next.D3 = /*p25.RUNO*/ not(b.vrm.MD3);
    /*p25.TYJA*/ if (MD_TO_D) next.D4 = /*p25.SANA*/ not(b.vrm.MD4);
    /*p25.REXU*/ if (MD_TO_D) next.D5 = /*p25.RORO*/ not(b.vrm.MD5);
    /*p25.RUPY*/ if (MD_TO_D) next.D6 = /*p25.RABO*/ not(b.vrm.MD6);
    /*p25.TOKU*/ if (MD_TO_D) next.D7 = /*p25.SAME*/ not(b.vrm.MD7);
  }

#endif
}

//-----------------------------------------------------------------------------

};