#include "Sch_OAM.h"

#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Sprites.h"
#include "Sch_Video.h"
#include "Sch_DMA.h"
#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_CpuSignals.h"
#include "Sch_Vram.h"
#include "Sch_SpriteStore.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void OAM_tick(const Bus& bus,
              const BusControl& ctl,
              const Cpu& cpu,
              const Pins& pins,
              const ClockSignals1& clk_sig1,
              const ClockSignals2& clk_sig2,
              const Decoder& dec,
              const Sprites& spr,
              const DMA& dma,
              const Video& vid,
              const Vram& vram,
              const OAM& prev,
              const SpriteIndexLine& sil,

              OAM& next,
              Bus& bus_out) {

  {
    // TUVO is low for the first 4 phases of a sprite fetch
    // XUJY seems to have a one-phase glitch?

    
#if 0
    {
      // this is how OAM_CLK is driven during sprite fetch
    
      /*p29.TUVO*/ wire SPR_OAM_RDn = or(vid.RENDERINGn, spr.SPR_SEQ1, spr.SEQ_xxx34xn);
      /*p29.TAME*/ wire SEQ_5n = nand(spr.SPR_SEQ2, spr.SPR_SEQ0);
      /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);
      /*p29.TACU*/ wire SPR_SEQ_5_TRIG = nand(SEQ_5, spr.TYFO);

      /*p25.VAPE*/ wire VAPE = and(SPR_OAM_RDn, SPR_SEQ_5_TRIG);
      /*p28.WEFY*/ wire WEFY = and(SPR_OAM_RDn, spr.TYFO);
      /*p25.XUJY*/ wire XUJY = not(VAPE);
      /*p28.XUJA*/ wire XUJA = not(WEFY);

      /*p28.BODE*/ next.OAM_LATCH = not(XUJA); // to the tribus receiver below
      /*p25.COTA*/ next.OAM_CLK_A = XUJY;
      /*p28.ZODO*/ next.OAM_CLK_B = XUJA;
    }
#endif

    /*p29.WOJO*/ wire WOJO = nor(!clk_sig2.WUVU_xxCDxxGH, !clk_sig2.WOSU_xBCxxFGx);
    /*p29.XYSO*/ wire XYSO = not(WOJO);
    /*p25.AVER*/ wire AVER = and(spr.OAM_ADDR_PARSE, XYSO);
    /*p28.AJEP*/ wire AJEP = and(spr.OAM_ADDR_PARSE, clk_sig2.XOCE_AxxDExxH);
    
    /*p29.TUVO*/ wire TUVO = or(vid.RENDERINGn, spr.SPR_SEQ1, spr.SEQ_xxx34xn);
    /*p29.TAME*/ wire TAME = nand(spr.SPR_SEQ2, spr.SPR_SEQ0);
    /*p29.TYTU*/ wire TYTU = not(TAME);
    /*p29.TACU*/ wire TACU = nand(TYTU, spr.SEQ_B0d);

    /*p25.VAPE*/ wire VAPE = and(TUVO, TACU);
    /*p28.WEFY*/ wire WEFY = and(TUVO, spr.SEQ_B0d);
    /*p25.XUJY*/ wire XUJY = not(VAPE);
    /*p28.XUJA*/ wire XUJA = not(WEFY);


                                                     // so does FROM_CPU5 mean "this is not a real read?"
    /*p04.DECY*/ wire FROM_CPU5n  = not(cpu.FROM_CPU5);
    /*p28.BOTA*/ wire CPU_RD_OAMn = nand(FROM_CPU5n, dec.ADDR_OAM, ctl.CPU_RD2); // Schematic wrong, this is NAND
    /*p25.CUFE*/ wire OAM_WR      = and(or(dec.ADDR_OAM, dma.DMA_CLKEN), clk_sig1.MOPA_ABCxxxxH); // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())



    /*p25.BYCU*/ wire OAM_IN_CLK = nand(AVER, XUJY, OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p28.ASYT*/ wire OAM_LATCHn = and (AJEP, XUJA, CPU_RD_OAMn);

    /*p28.YVAL*/ wire OAM_LATCHo = not(prev.OAM_LATCH);
    /*p28.YRYV*/ wire OAM_LATCHb = not(OAM_LATCHo);

    /*p28.BODE*/ next.OAM_LATCH = not(OAM_LATCHn); // to the tribus receiver below

    /*p25.COTA*/ next.OAM_CLK_A = not(OAM_IN_CLK); // -> OAM
    /*p28.ZODO*/ next.OAM_CLK_B = not(OAM_LATCHb); // -> OAM
  }
    

  {
    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(dma.DMA_CLKEN);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, vid.RENDERING_LATCH);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(dma.DMA_CLKEN, spr.OAM_ADDR_PARSE, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(dec.ADDR_OAM, OAM_BUSYn);


    /*p28.WAFO*/ wire OAM_A0n   = not(prev.OAM_A0);
    /*p04.WYJA*/ wire OAM_WR    = unk3(OAM_LOCKn, ctl.CPU_WR2, dma.DMA_WRITE_OAM);
    /*p28.YNYC*/ wire OAM_B_WRn = and(OAM_WR, OAM_A0n);
    /*p28.YLYC*/ wire OAM_A_WRn = and(OAM_WR, prev.OAM_A0);
    /*p28.ZOFE*/ next.OAM_B_WR  = not(OAM_B_WRn); // -> OAM
    /*p28.ZONE*/ next.OAM_A_WR  = not(OAM_A_WRn); // -> OAM
  }

  //----------
  // OAM address generator

  {
    /*p28.GARO*/ wire A0n = not(bus.A00);
    /*p28.WACU*/ wire A1n = not(bus.A01);
    /*p28.GOSE*/ wire A2n = not(bus.A02);
    /*p28.WAPE*/ wire A3n = not(bus.A03);
    /*p28.FEVU*/ wire A4n = not(bus.A04);
    /*p28.GERA*/ wire A5n = not(bus.A05);
    /*p28.WAXA*/ wire A6n = not(bus.A06);
    /*p28.FOBY*/ wire A7n = not(bus.A07);

    /*p28.FYKE*/ wire IDX_0n = not(sil.TS_IDX_0);
    /*p28.FUGU*/ wire IDX_1n = not(sil.TS_IDX_1);
    /*p28.FACO*/ wire IDX_2n = not(sil.TS_IDX_2);
    /*p28.FABY*/ wire IDX_3n = not(sil.TS_IDX_3);
    /*p28.GYKA*/ wire IDX_4n = not(sil.TS_IDX_4);
    /*p28.GYBU*/ wire IDX_5n = not(sil.TS_IDX_5);

    /*p28.WUWE*/ wire SCANZn = not(pins.P10_B);
    /*p28.GUSE*/ wire SCAN0n = not(spr.SCAN0);
    /*p28.GEMA*/ wire SCAN1n = not(spr.SCAN1);
    /*p28.FUTO*/ wire SCAN2n = not(spr.SCAN2);
    /*p28.FAKU*/ wire SCAN3n = not(spr.SCAN3);
    /*p28.GAMA*/ wire SCAN4n = not(spr.SCAN4);
    /*p28.GOBY*/ wire SCAN5n = not(spr.SCAN5);

    /*p28.FETU*/ wire DMA_A07n = not(dma.DMA_A07);
    /*p28.FYDU*/ wire DMA_A06n = not(dma.DMA_A06);
    /*p28.EDOL*/ wire DMA_A05n = not(dma.DMA_A05);
    /*p28.ELUG*/ wire DMA_A04n = not(dma.DMA_A04);
    /*p28.FYKY*/ wire DMA_A03n = not(dma.DMA_A03);
    /*p28.FAGO*/ wire DMA_A02n = not(dma.DMA_A02);
    /*p28.FESA*/ wire DMA_A01n = not(dma.DMA_A01);
    /*p28.FODO*/ wire DMA_A00n = not(dma.DMA_A00);

    /*p28.BOGE*/ wire DMA_CLKENn = not(dma.DMA_CLKEN);

    // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p04.DUGA*/ wire OAM_ADDR_DMAn    = not(dma.DMA_CLKEN);
    /*p28.APAR*/ wire OAM_ADDR_PARSEn  = not(spr.OAM_ADDR_PARSE);

    // ppu can read oam when there's rendering but no dma
    /*p28.AJON*/ wire OAM_ADDR_RENDER  = and(DMA_CLKENn, vid.RENDERING_LATCH);
    /*p28.BETE*/ wire OAM_ADDR_RENDERn = not(OAM_ADDR_RENDER);

    // cpu can read oam when there's no parsing, rendering, or dma
    /*p28.ASAM*/ wire OAM_ADDR_CPUn    = or(spr.OAM_ADDR_PARSE, vid.RENDERING_LATCH, dma.DMA_CLKEN);

    /*p28.GECA*/ wire GECA = not(spr.P10_Bn);
    /*p28.WYDU*/ wire WYDU = not(spr.P10_Bn);
    /*p28.GEFY*/ wire GEFY = not(pins.P10_B);

    // so byte 0 is the palette number? something wrong here...

    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GEKA*/ next.OAM_A0 = not((A0n & !OAM_ADDR_CPUn) | (GECA   & !OAM_ADDR_RENDERn) | (GEFY   & !OAM_ADDR_PARSEn) | (DMA_A00n & !OAM_ADDR_DMAn));
    /*p28.ZYFO*/ next.OAM_A1 = not((A1n & !OAM_ADDR_CPUn) | (WYDU   & !OAM_ADDR_RENDERn) | (SCANZn & !OAM_ADDR_PARSEn) | (DMA_A01n & !OAM_ADDR_DMAn));
    /*p28.YFOT*/ next.OAM_A2 = not((A2n & !OAM_ADDR_CPUn) | (IDX_5n & !OAM_ADDR_RENDERn) | (SCAN0n & !OAM_ADDR_PARSEn) | (DMA_A02n & !OAM_ADDR_DMAn));
    /*p28.YFOC*/ next.OAM_A3 = not((A3n & !OAM_ADDR_CPUn) | (IDX_4n & !OAM_ADDR_RENDERn) | (SCAN1n & !OAM_ADDR_PARSEn) | (DMA_A03n & !OAM_ADDR_DMAn));
    /*p28.YVOM*/ next.OAM_A4 = not((A4n & !OAM_ADDR_CPUn) | (IDX_3n & !OAM_ADDR_RENDERn) | (SCAN2n & !OAM_ADDR_PARSEn) | (DMA_A04n & !OAM_ADDR_DMAn));
    /*p28.YMEV*/ next.OAM_A5 = not((A5n & !OAM_ADDR_CPUn) | (IDX_2n & !OAM_ADDR_RENDERn) | (SCAN3n & !OAM_ADDR_PARSEn) | (DMA_A05n & !OAM_ADDR_DMAn));
    /*p28.XEMU*/ next.OAM_A6 = not((A6n & !OAM_ADDR_CPUn) | (IDX_1n & !OAM_ADDR_RENDERn) | (SCAN4n & !OAM_ADDR_PARSEn) | (DMA_A06n & !OAM_ADDR_DMAn));
    /*p28.YZET*/ next.OAM_A7 = not((A7n & !OAM_ADDR_CPUn) | (IDX_0n & !OAM_ADDR_RENDERn) | (SCAN5n & !OAM_ADDR_PARSEn) | (DMA_A07n & !OAM_ADDR_DMAn));
  }

  {
    // OAM data tribus driver

    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(dma.DMA_CLKEN);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, vid.RENDERING_LATCH);

    /*p28.AJUJ*/ wire OAM_BUSYn = nor(dma.DMA_CLKEN, spr.OAM_ADDR_PARSE, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(dec.ADDR_OAM, OAM_BUSYn);

    /*p28.ADAH*/ wire ADDR_OAMn = not(dec.ADDR_OAM);
    /*p28.XUTO*/ wire CPU_OAM_WR = and(dec.ADDR_OAM, ctl.CPU_WR2);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(clk_sig1.MOPA_ABCxxxxH);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);

    /*p28.AZAR*/ wire DMA_READ_VRAMp = not(dma.DMA_READ_VRAM);

    /*p28.ZAXA*/ if (D_TO_OAMD) next.TS_OAM_A_D0 = bus.D0;
    /*p28.ZAKY*/ if (D_TO_OAMD) next.TS_OAM_A_D1 = bus.D1;
    /*p28.WULE*/ if (D_TO_OAMD) next.TS_OAM_A_D2 = bus.D2;
    /*p28.ZOZO*/ if (D_TO_OAMD) next.TS_OAM_A_D3 = bus.D3;
    /*p28.ZUFO*/ if (D_TO_OAMD) next.TS_OAM_A_D4 = bus.D4;
    /*p28.ZATO*/ if (D_TO_OAMD) next.TS_OAM_A_D5 = bus.D5;
    /*p28.YVUC*/ if (D_TO_OAMD) next.TS_OAM_A_D6 = bus.D6;
    /*p28.ZUFE*/ if (D_TO_OAMD) next.TS_OAM_A_D7 = bus.D7;

    /*p28.ZAMY*/ if (D_TO_OAMD) next.TS_OAM_B_D0 = bus.D0;
    /*p28.ZOPU*/ if (D_TO_OAMD) next.TS_OAM_B_D1 = bus.D1;
    /*p28.WYKY*/ if (D_TO_OAMD) next.TS_OAM_B_D2 = bus.D2;
    /*p28.ZAJA*/ if (D_TO_OAMD) next.TS_OAM_B_D3 = bus.D3;
    /*p28.ZUGA*/ if (D_TO_OAMD) next.TS_OAM_B_D4 = bus.D4;
    /*p28.ZUMO*/ if (D_TO_OAMD) next.TS_OAM_B_D5 = bus.D5;
    /*p28.XYTO*/ if (D_TO_OAMD) next.TS_OAM_B_D6 = bus.D6;
    /*p28.ZYFA*/ if (D_TO_OAMD) next.TS_OAM_B_D7 = bus.D7;
  
    /*p28.WUZU*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D0 = vram.MD0;
    /*p28.AXER*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D1 = vram.MD1;
    /*p28.ASOX*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D2 = vram.MD2;
    /*p28.CETU*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D3 = vram.MD3;
    /*p28.ARYN*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D4 = vram.MD4;
    /*p28.ACOT*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D5 = vram.MD5;
    /*p28.CUJE*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D6 = vram.MD6;
    /*p28.ATER*/ if (!DMA_READ_VRAMp) next.TS_OAM_A_D7 = vram.MD7;

    /*p28.WOWA*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D0 = vram.MD0;
    /*p28.AVEB*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D1 = vram.MD1;
    /*p28.AMUH*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D2 = vram.MD2;
    /*p28.COFO*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D3 = vram.MD3;
    /*p28.AZOZ*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D4 = vram.MD4;
    /*p28.AGYK*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D5 = vram.MD5;
    /*p28.BUSE*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D6 = vram.MD6;
    /*p28.ANUM*/ if (!DMA_READ_VRAMp) next.TS_OAM_B_D7 = vram.MD7;

    /*p25.WEJO*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D0 = not(/*p25.RALO*/ not(pins.D0_C));
    /*p25.BUBO*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D1 = not(/*p25.TUNE*/ not(pins.D1_C));
    /*p25.BETU*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D2 = not(/*p25.SERA*/ not(pins.D2_C));
    /*p25.CYME*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D3 = not(/*p25.TENU*/ not(pins.D3_C));
    /*p25.BAXU*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D4 = not(/*p25.SYSA*/ not(pins.D4_C));
    /*p25.BUHU*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D5 = not(/*p25.SUGY*/ not(pins.D5_C));
    /*p25.BYNY*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D6 = not(/*p25.TUBE*/ not(pins.D6_C));
    /*p25.BYPY*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_A_D7 = not(/*p25.SYZO*/ not(pins.D7_C));

    /*p25.WASA*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D0 = not(/*p25.RALO*/ not(pins.D0_C));
    /*p25.BOMO*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D1 = not(/*p25.TUNE*/ not(pins.D1_C));
    /*p25.BASA*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D2 = not(/*p25.SERA*/ not(pins.D2_C));
    /*p25.CAKO*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D3 = not(/*p25.TENU*/ not(pins.D3_C));
    /*p25.BUMA*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D4 = not(/*p25.SYSA*/ not(pins.D4_C));
    /*p25.BUPY*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D5 = not(/*p25.SUGY*/ not(pins.D5_C));
    /*p25.BASY*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D6 = not(/*p25.TUBE*/ not(pins.D6_C));
    /*p25.BAPE*/ if (!dma.DMA_READ_CARTn) next.TS_OAM_B_D7 = not(/*p25.SYZO*/ not(pins.D7_C));
  }

  // OAM data tribus receiver

  {
    /*p31.XYKY*/ next.OAM_A_LATCH0 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH0, prev.TS_OAM_A_D0);
    /*p31.YRUM*/ next.OAM_A_LATCH1 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH1, prev.TS_OAM_A_D1);
    /*p31.YSEX*/ next.OAM_A_LATCH2 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH2, prev.TS_OAM_A_D2);
    /*p31.YVEL*/ next.OAM_A_LATCH3 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH3, prev.TS_OAM_A_D3);
    /*p31.WYNO*/ next.OAM_A_LATCH4 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH4, prev.TS_OAM_A_D4);
    /*p31.CYRA*/ next.OAM_A_LATCH5 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH5, prev.TS_OAM_A_D5);
    /*p31.ZUVE*/ next.OAM_A_LATCH6 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH6, prev.TS_OAM_A_D6);
    /*p31.ECED*/ next.OAM_A_LATCH7 = latch_pos(prev.OAM_LATCH, prev.OAM_A_LATCH7, prev.TS_OAM_A_D7);

    /*p31.XEGA*/ wire OAM_A_IN_CLK = not(prev.OAM_CLK_A);
    /*p31.YLOR*/ next.OAM_A_D0.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH0);
    /*p31.ZYTY*/ next.OAM_A_D1.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH1);
    /*p31.ZYVE*/ next.OAM_A_D2.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH2);
    /*p31.ZEZY*/ next.OAM_A_D3.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH3);
    /*p31.GOMO*/ next.OAM_A_D4.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH4);
    /*p31.BAXO*/ next.OAM_A_D5.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH5);
    /*p31.YZOS*/ next.OAM_A_D6.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH6);
    /*p31.DEPO*/ next.OAM_A_D7.tock(OAM_A_IN_CLK, 0, prev.OAM_A_LATCH7);

    /*p31.ZAGO*/ next.OAM_A_D0b = not(!prev.OAM_A_D0);
    /*p31.ZOCY*/ next.OAM_A_D1b = not(!prev.OAM_A_D1);
    /*p31.YPUR*/ next.OAM_A_D2b = not(!prev.OAM_A_D2);
    /*p31.YVOK*/ next.OAM_A_D3b = not(!prev.OAM_A_D3);
    /*p31.COSE*/ next.OAM_A_D4b = not(!prev.OAM_A_D4);
    /*p31.AROP*/ next.OAM_A_D5b = not(!prev.OAM_A_D5);
    /*p31.XATU*/ next.OAM_A_D6b = not(!prev.OAM_A_D6);
    /*p31.BADY*/ next.OAM_A_D7b = not(!prev.OAM_A_D7);
  }

  {
    /*p29.YDYV*/ next.OAM_B_LATCH0 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH0, prev.TS_OAM_B_D0);
    /*p29.YCEB*/ next.OAM_B_LATCH1 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH1, prev.TS_OAM_B_D1);
    /*p29.ZUCA*/ next.OAM_B_LATCH2 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH2, prev.TS_OAM_B_D2);
    /*p29.WONE*/ next.OAM_B_LATCH3 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH3, prev.TS_OAM_B_D3);
    /*p29.ZAXE*/ next.OAM_B_LATCH4 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH4, prev.TS_OAM_B_D4);
    /*p29.XAFU*/ next.OAM_B_LATCH5 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH5, prev.TS_OAM_B_D5);
    /*p29.YSES*/ next.OAM_B_LATCH6 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH6, prev.TS_OAM_B_D6);
    /*p29.ZECA*/ next.OAM_B_LATCH7 = latch_pos(prev.OAM_LATCH, prev.OAM_B_LATCH7, prev.TS_OAM_B_D7);

    /*p29.YWOK*/ wire OAM_B_IN_CLK = not(prev.OAM_CLK_A);
    /*p29.XUSO*/ next.OAM_B_D0.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH0);
    /*p29.XEGU*/ next.OAM_B_D1.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH1);
    /*p29.YJEX*/ next.OAM_B_D2.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH2);
    /*p29.XYJU*/ next.OAM_B_D3.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH3);
    /*p29.YBOG*/ next.OAM_B_D4.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH4);
    /*p29.WYSO*/ next.OAM_B_D5.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH5);
    /*p29.XOTE*/ next.OAM_B_D6.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH6);
    /*p29.YZAB*/ next.OAM_B_D7.tock(OAM_B_IN_CLK, 0, prev.OAM_B_LATCH7);
  }

  {
    /*p28.BOGE*/ wire DMA_RUNNING_SYNCb = not(dma.DMA_CLKEN);
    /*p28.AJON*/ wire OAM_ADDR_RENDERn = and(DMA_RUNNING_SYNCb, vid.RENDERING_LATCH);
    /*p28.AJUJ*/ wire OAM_BUSYn = nor(dma.DMA_CLKEN, spr.OAM_ADDR_PARSE, OAM_ADDR_RENDERn);
    /*p28.AMAB*/ wire OAM_LOCKn = and(dec.ADDR_OAM, OAM_BUSYn);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(ctl.CPU_RD2, cpu.FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);

    /*p28.WAFO*/ wire OAM_A0n   = not(prev.OAM_A0);

    /*p28.WUKU*/ wire OAM_A_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, prev.OAM_A0);
    /*p28.WEWU*/ wire OAM_A_CPU_RD  = not(OAM_A_CPU_RDn);
    /*p31.XACA*/ if (OAM_A_CPU_RD) bus_out.D0 = prev.OAM_A_LATCH0;
    /*p31.XAGU*/ if (OAM_A_CPU_RD) bus_out.D1 = prev.OAM_A_LATCH1;
    /*p31.XEPU*/ if (OAM_A_CPU_RD) bus_out.D2 = prev.OAM_A_LATCH2;
    /*p31.XYGU*/ if (OAM_A_CPU_RD) bus_out.D3 = prev.OAM_A_LATCH3;
    /*p31.XUNA*/ if (OAM_A_CPU_RD) bus_out.D4 = prev.OAM_A_LATCH4;
    /*p31.DEVE*/ if (OAM_A_CPU_RD) bus_out.D5 = prev.OAM_A_LATCH5;
    /*p31.ZEHA*/ if (OAM_A_CPU_RD) bus_out.D6 = prev.OAM_A_LATCH6;
    /*p31.FYRA*/ if (OAM_A_CPU_RD) bus_out.D7 = prev.OAM_A_LATCH7;

    /*p28.GUKO*/ wire OAM_B_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, OAM_A0n);
    /*p28.WUME*/ wire OAM_B_CPU_RD  = not(OAM_B_CPU_RDn);
    /*p29.YFAP*/ if (OAM_B_CPU_RD) bus_out.D0 = prev.OAM_B_LATCH0;
    /*p29.XELE*/ if (OAM_B_CPU_RD) bus_out.D1 = prev.OAM_B_LATCH1;
    /*p29.YPON*/ if (OAM_B_CPU_RD) bus_out.D2 = prev.OAM_B_LATCH2;
    /*p29.XUVO*/ if (OAM_B_CPU_RD) bus_out.D3 = prev.OAM_B_LATCH3;
    /*p29.ZYSA*/ if (OAM_B_CPU_RD) bus_out.D4 = prev.OAM_B_LATCH4;
    /*p29.YWEG*/ if (OAM_B_CPU_RD) bus_out.D5 = prev.OAM_B_LATCH5;
    /*p29.XABU*/ if (OAM_B_CPU_RD) bus_out.D6 = prev.OAM_B_LATCH6;
    /*p29.YTUX*/ if (OAM_B_CPU_RD) bus_out.D7 = prev.OAM_B_LATCH7;
  }
}

//-----------------------------------------------------------------------------

};