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
#include "Sch_Sprites.h"
#include "Sch_SpriteStore.h"

#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------
// I should probably go over this again, I think I got some signals mixed up.

OamSignals OamRegisters::tick(const Cpu& cpu,
                              const ClkSignals& clk_sig,
                              const BusSignals& bus_sig,
                              const CartPins& cart_pins,
                              const JoypadPins& joy_pins,
                              const VclkSignals& vid_clk,
                              const DecoderSignals& dec_sig,
                              const SpriteSignals& spr_sig,
                              const DmaSignals& dma_sig,
                              const VidSignals& vid_sig,
                              const SpriteTristate& spr_tri,
                              const VramBus& vram_tri,
                              BusTristates& bus_tri,
                              OamPins& oam_pins) {

  // TUVO is low for the first 4 phases of a sprite fetch
  // XUJY seems to have a one-phase glitch?

  // ppu can read oam when there's rendering but no dma
  /*p28.BOGE*/ wire DMA_CLKENn = not(dma_sig.DMA_CLKEN);
  /*p28.AJON*/ wire OAM_ADDR_RENDER  = and(DMA_CLKENn, vid_sig.RENDERING_LATCH);
  /*p28.BETE*/ wire OAM_ADDR_RENDERn = not(OAM_ADDR_RENDER);
  /*p28.AJUJ*/ wire OAM_BUSYn = nor(dma_sig.DMA_CLKEN, spr_sig.OAM_ADDR_PARSE, OAM_ADDR_RENDERn);
  /*p28.AMAB*/ wire OAM_LOCKn = and(dec_sig.ADDR_OAM, OAM_BUSYn);

  //----------
  // OAM address generator

  {
    /*p28.GARO*/ wire A0n = not(bus_tri.A00());
    /*p28.WACU*/ wire A1n = not(bus_tri.A01());
    /*p28.GOSE*/ wire A2n = not(bus_tri.A02());
    /*p28.WAPE*/ wire A3n = not(bus_tri.A03());
    /*p28.FEVU*/ wire A4n = not(bus_tri.A04());
    /*p28.GERA*/ wire A5n = not(bus_tri.A05());
    /*p28.WAXA*/ wire A6n = not(bus_tri.A06());
    /*p28.FOBY*/ wire A7n = not(bus_tri.A07());

    /*p28.FYKE*/ wire IDX_0n = not(spr_tri.TS_IDX_0);
    /*p28.FUGU*/ wire IDX_1n = not(spr_tri.TS_IDX_1);
    /*p28.FACO*/ wire IDX_2n = not(spr_tri.TS_IDX_2);
    /*p28.FABY*/ wire IDX_3n = not(spr_tri.TS_IDX_3);
    /*p28.GYKA*/ wire IDX_4n = not(spr_tri.TS_IDX_4);
    /*p28.GYBU*/ wire IDX_5n = not(spr_tri.TS_IDX_5);

    /*p28.WUWE*/ wire SCANZn = not(joy_pins.P10_B);
    /*p28.GUSE*/ wire SCAN0n = not(spr_sig.SCAN0);
    /*p28.GEMA*/ wire SCAN1n = not(spr_sig.SCAN1);
    /*p28.FUTO*/ wire SCAN2n = not(spr_sig.SCAN2);
    /*p28.FAKU*/ wire SCAN3n = not(spr_sig.SCAN3);
    /*p28.GAMA*/ wire SCAN4n = not(spr_sig.SCAN4);
    /*p28.GOBY*/ wire SCAN5n = not(spr_sig.SCAN5);

    /*p28.FETU*/ wire DMA_A07n = not(dma_sig.DMA_A07);
    /*p28.FYDU*/ wire DMA_A06n = not(dma_sig.DMA_A06);
    /*p28.EDOL*/ wire DMA_A05n = not(dma_sig.DMA_A05);
    /*p28.ELUG*/ wire DMA_A04n = not(dma_sig.DMA_A04);
    /*p28.FYKY*/ wire DMA_A03n = not(dma_sig.DMA_A03);
    /*p28.FAGO*/ wire DMA_A02n = not(dma_sig.DMA_A02);
    /*p28.FESA*/ wire DMA_A01n = not(dma_sig.DMA_A01);
    /*p28.FODO*/ wire DMA_A00n = not(dma_sig.DMA_A00);

    // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p04.DUGA*/ wire OAM_ADDR_DMAn    = not(dma_sig.DMA_CLKEN);
    /*p28.APAR*/ wire OAM_ADDR_PARSEn  = not(spr_sig.OAM_ADDR_PARSE);

    // cpu can read oam when there's no parsing, rendering, or dma
    /*p28.ASAM*/ wire OAM_ADDR_CPUn    = or(spr_sig.OAM_ADDR_PARSE, vid_sig.RENDERING_LATCH, dma_sig.DMA_CLKEN);

    /*p28.WEFE*/ wire WEFE = not(joy_pins.P10_B);
    /*p28.GEFY*/ wire GEFY = not(joy_pins.P10_B);
    /*p28.GECA*/ wire GECA = not(WEFE);
    /*p28.WYDU*/ wire WYDU = not(WEFE);

    // so byte 0 is the palette number? something wrong here...

    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GEKA*/ oam_pins.A0 = not((A0n & !OAM_ADDR_CPUn) | (GECA   & !OAM_ADDR_RENDERn) | (GEFY   & !OAM_ADDR_PARSEn) | (DMA_A00n & !OAM_ADDR_DMAn));
    /*p28.ZYFO*/ oam_pins.A1 = not((A1n & !OAM_ADDR_CPUn) | (WYDU   & !OAM_ADDR_RENDERn) | (SCANZn & !OAM_ADDR_PARSEn) | (DMA_A01n & !OAM_ADDR_DMAn));
    /*p28.YFOT*/ oam_pins.A2 = not((A2n & !OAM_ADDR_CPUn) | (IDX_5n & !OAM_ADDR_RENDERn) | (SCAN0n & !OAM_ADDR_PARSEn) | (DMA_A02n & !OAM_ADDR_DMAn));
    /*p28.YFOC*/ oam_pins.A3 = not((A3n & !OAM_ADDR_CPUn) | (IDX_4n & !OAM_ADDR_RENDERn) | (SCAN1n & !OAM_ADDR_PARSEn) | (DMA_A03n & !OAM_ADDR_DMAn));
    /*p28.YVOM*/ oam_pins.A4 = not((A4n & !OAM_ADDR_CPUn) | (IDX_3n & !OAM_ADDR_RENDERn) | (SCAN2n & !OAM_ADDR_PARSEn) | (DMA_A04n & !OAM_ADDR_DMAn));
    /*p28.YMEV*/ oam_pins.A5 = not((A5n & !OAM_ADDR_CPUn) | (IDX_2n & !OAM_ADDR_RENDERn) | (SCAN3n & !OAM_ADDR_PARSEn) | (DMA_A05n & !OAM_ADDR_DMAn));
    /*p28.XEMU*/ oam_pins.A6 = not((A6n & !OAM_ADDR_CPUn) | (IDX_1n & !OAM_ADDR_RENDERn) | (SCAN4n & !OAM_ADDR_PARSEn) | (DMA_A06n & !OAM_ADDR_DMAn));
    /*p28.YZET*/ oam_pins.A7 = not((A7n & !OAM_ADDR_CPUn) | (IDX_0n & !OAM_ADDR_RENDERn) | (SCAN5n & !OAM_ADDR_PARSEn) | (DMA_A07n & !OAM_ADDR_DMAn));
  }


  {
    /*p29.TUVO*/ wire SPR_OAM_RDn = or(vid_sig.RENDERINGn, spr_sig.SPR_SEQ1, spr_sig.SEQ_xxx34xn);
    /*p28.AJEP*/   wire AJEP = and(spr_sig.OAM_ADDR_PARSE, vid_clk.XOCE_ABxxEFxx);
    /*p28.WEFY*/     wire WEFY = and(SPR_OAM_RDn, spr_sig.SEQ_B0d);
    /*p28.XUJA*/   wire XUJA = not(WEFY);
    /*p04.DECY*/     wire FROM_CPU5n  = not(cpu.FROM_CPU5);
    /*p28.BOTA*/   wire CPU_RD_OAMn = nand(FROM_CPU5n, dec_sig.ADDR_OAM, bus_sig.ASOT_CPURD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire OAM_LATCHn = and (AJEP, XUJA, CPU_RD_OAMn);

    /*p29.TAME*/ wire SEQ_5n = nand(spr_sig.SPR_SEQ2, spr_sig.SPR_SEQ0);
    /*p29.TYTU*/ wire SEQ_5 = not(SEQ_5n);
    /*p29.TACU*/ wire SPR_SEQ_5_TRIG = nand(SEQ_5, spr_sig.SEQ_B0d);
    /*p25.AVER*/ wire AVER = and(spr_sig.OAM_ADDR_PARSE, vid_clk.XYSO_ABCxDEFx);
    /*p25.VAPE*/ wire VAPE = and(SPR_OAM_RDn, SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire XUJY = not(VAPE);

    /*p25.CUFE*/ wire CUFE_OAM_WR = and(or(dec_sig.ADDR_OAM, dma_sig.DMA_CLKEN), clk_sig.MOPA_xxxxEFGH); // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())
    /*p25.BYCU*/ wire OAM_IN_CLK = nand(AVER, XUJY, CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    
    // this has to be write
    /*p25.COTA*/ oam_pins.CLK_A = not(OAM_IN_CLK);

    // this has to be read
    /*p28.BODE*/ wire OAM_LATCH  = not(OAM_LATCHn); // to the tribus receiver below
    /*p28.YVAL*/ wire OAM_LATCHo = not(OAM_LATCH);
    /*p28.YRYV*/ wire OAM_LATCHb = not(OAM_LATCHo);
    /*p28.ZODO*/ oam_pins.CLK_B = not(OAM_LATCHb);

    /*p28.WAFO*/ wire WAFO_OAM_A0n   = not(oam_pins.A0);
    /*p04.WYJA*/ wire WYJA_OAM_WR    = unk3(OAM_LOCKn, bus_sig.CUPA_CPUWR, dma_sig.DMA_WRITE_OAM);
    /*p28.YNYC*/ wire YNYC_OAM_B_WRn = and(WYJA_OAM_WR, WAFO_OAM_A0n);
    /*p28.YLYC*/ wire YLYC_OAM_A_WRn = and(WYJA_OAM_WR, oam_pins.A0);

    // this has to be chip select or something
    /*p28.ZONE*/ oam_pins.WR_A = not(YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ oam_pins.WR_B = not(YNYC_OAM_B_WRn);

    /*p31.XYKY*/ OAM_A_LATCH0.latch(OAM_LATCH, oam_pins.A_D0);
    /*p31.YRUM*/ OAM_A_LATCH1.latch(OAM_LATCH, oam_pins.A_D1);
    /*p31.YSEX*/ OAM_A_LATCH2.latch(OAM_LATCH, oam_pins.A_D2);
    /*p31.YVEL*/ OAM_A_LATCH3.latch(OAM_LATCH, oam_pins.A_D3);
    /*p31.WYNO*/ OAM_A_LATCH4.latch(OAM_LATCH, oam_pins.A_D4);
    /*p31.CYRA*/ OAM_A_LATCH5.latch(OAM_LATCH, oam_pins.A_D5);
    /*p31.ZUVE*/ OAM_A_LATCH6.latch(OAM_LATCH, oam_pins.A_D6);
    /*p31.ECED*/ OAM_A_LATCH7.latch(OAM_LATCH, oam_pins.A_D7);

    /*p29.YDYV*/ OAM_B_LATCH0.latch(OAM_LATCH, oam_pins.B_D0);
    /*p29.YCEB*/ OAM_B_LATCH1.latch(OAM_LATCH, oam_pins.B_D1);
    /*p29.ZUCA*/ OAM_B_LATCH2.latch(OAM_LATCH, oam_pins.B_D2);
    /*p29.WONE*/ OAM_B_LATCH3.latch(OAM_LATCH, oam_pins.B_D3);
    /*p29.ZAXE*/ OAM_B_LATCH4.latch(OAM_LATCH, oam_pins.B_D4);
    /*p29.XAFU*/ OAM_B_LATCH5.latch(OAM_LATCH, oam_pins.B_D5);
    /*p29.YSES*/ OAM_B_LATCH6.latch(OAM_LATCH, oam_pins.B_D6);
    /*p29.ZECA*/ OAM_B_LATCH7.latch(OAM_LATCH, oam_pins.B_D7);
  }

  // OAM data tribus driver

  {
    /*p28.ADAH*/ wire ADDR_OAMn = not(dec_sig.ADDR_OAM);
    /*p28.XUTO*/ wire CPU_OAM_WR = and(dec_sig.ADDR_OAM, bus_sig.CUPA_CPUWR);
    /*p28.XYNY*/ wire CPU_OAM_WR_CLK1 = not(clk_sig.MOPA_xxxxEFGH);
    /*p28.WUJE*/ wire CPU_OAM_WR_CLK2 = or(CPU_OAM_WR_CLK1, CPU_OAM_WR);
    /*p28.XUPA*/ wire CPU_OAM_WR_CLK3 = not(CPU_OAM_WR_CLK2);
    /*p28.APAG*/ wire D_TO_OAMDn = amux2(CPU_OAM_WR_CLK3, OAM_LOCKn, OAM_BUSYn, ADDR_OAMn);
    /*p28.AZUL*/ wire D_TO_OAMD = not(D_TO_OAMDn);
    /*p28.ZAXA*/ if (D_TO_OAMD) oam_pins.A_D0 = bus_tri.D0();
    /*p28.ZAKY*/ if (D_TO_OAMD) oam_pins.A_D1 = bus_tri.D1();
    /*p28.WULE*/ if (D_TO_OAMD) oam_pins.A_D2 = bus_tri.D2();
    /*p28.ZOZO*/ if (D_TO_OAMD) oam_pins.A_D3 = bus_tri.D3();
    /*p28.ZUFO*/ if (D_TO_OAMD) oam_pins.A_D4 = bus_tri.D4();
    /*p28.ZATO*/ if (D_TO_OAMD) oam_pins.A_D5 = bus_tri.D5();
    /*p28.YVUC*/ if (D_TO_OAMD) oam_pins.A_D6 = bus_tri.D6();
    /*p28.ZUFE*/ if (D_TO_OAMD) oam_pins.A_D7 = bus_tri.D7();

    /*p28.ZAMY*/ if (D_TO_OAMD) oam_pins.B_D0 = bus_tri.D0();
    /*p28.ZOPU*/ if (D_TO_OAMD) oam_pins.B_D1 = bus_tri.D1();
    /*p28.WYKY*/ if (D_TO_OAMD) oam_pins.B_D2 = bus_tri.D2();
    /*p28.ZAJA*/ if (D_TO_OAMD) oam_pins.B_D3 = bus_tri.D3();
    /*p28.ZUGA*/ if (D_TO_OAMD) oam_pins.B_D4 = bus_tri.D4();
    /*p28.ZUMO*/ if (D_TO_OAMD) oam_pins.B_D5 = bus_tri.D5();
    /*p28.XYTO*/ if (D_TO_OAMD) oam_pins.B_D6 = bus_tri.D6();
    /*p28.ZYFA*/ if (D_TO_OAMD) oam_pins.B_D7 = bus_tri.D7();
  
    /*p28.AZAR*/ wire DMA_READ_VRAMp = not(dma_sig.DMA_READ_VRAM);
    /*p28.WUZU*/ if (!DMA_READ_VRAMp) oam_pins.A_D0 = vram_tri.MD0;
    /*p28.AXER*/ if (!DMA_READ_VRAMp) oam_pins.A_D1 = vram_tri.MD1;
    /*p28.ASOX*/ if (!DMA_READ_VRAMp) oam_pins.A_D2 = vram_tri.MD2;
    /*p28.CETU*/ if (!DMA_READ_VRAMp) oam_pins.A_D3 = vram_tri.MD3;
    /*p28.ARYN*/ if (!DMA_READ_VRAMp) oam_pins.A_D4 = vram_tri.MD4;
    /*p28.ACOT*/ if (!DMA_READ_VRAMp) oam_pins.A_D5 = vram_tri.MD5;
    /*p28.CUJE*/ if (!DMA_READ_VRAMp) oam_pins.A_D6 = vram_tri.MD6;
    /*p28.ATER*/ if (!DMA_READ_VRAMp) oam_pins.A_D7 = vram_tri.MD7;

    /*p28.WOWA*/ if (!DMA_READ_VRAMp) oam_pins.B_D0 = vram_tri.MD0;
    /*p28.AVEB*/ if (!DMA_READ_VRAMp) oam_pins.B_D1 = vram_tri.MD1;
    /*p28.AMUH*/ if (!DMA_READ_VRAMp) oam_pins.B_D2 = vram_tri.MD2;
    /*p28.COFO*/ if (!DMA_READ_VRAMp) oam_pins.B_D3 = vram_tri.MD3;
    /*p28.AZOZ*/ if (!DMA_READ_VRAMp) oam_pins.B_D4 = vram_tri.MD4;
    /*p28.AGYK*/ if (!DMA_READ_VRAMp) oam_pins.B_D5 = vram_tri.MD5;
    /*p28.BUSE*/ if (!DMA_READ_VRAMp) oam_pins.B_D6 = vram_tri.MD6;
    /*p28.ANUM*/ if (!DMA_READ_VRAMp) oam_pins.B_D7 = vram_tri.MD7;

    /*p25.WEJO*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D0 = not(/*p25.RALO*/ not(cart_pins.D0_C));
    /*p25.BUBO*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D1 = not(/*p25.TUNE*/ not(cart_pins.D1_C));
    /*p25.BETU*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D2 = not(/*p25.SERA*/ not(cart_pins.D2_C));
    /*p25.CYME*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D3 = not(/*p25.TENU*/ not(cart_pins.D3_C));
    /*p25.BAXU*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D4 = not(/*p25.SYSA*/ not(cart_pins.D4_C));
    /*p25.BUHU*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D5 = not(/*p25.SUGY*/ not(cart_pins.D5_C));
    /*p25.BYNY*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D6 = not(/*p25.TUBE*/ not(cart_pins.D6_C));
    /*p25.BYPY*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.A_D7 = not(/*p25.SYZO*/ not(cart_pins.D7_C));

    /*p25.WASA*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D0 = not(/*p25.RALO*/ not(cart_pins.D0_C));
    /*p25.BOMO*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D1 = not(/*p25.TUNE*/ not(cart_pins.D1_C));
    /*p25.BASA*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D2 = not(/*p25.SERA*/ not(cart_pins.D2_C));
    /*p25.CAKO*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D3 = not(/*p25.TENU*/ not(cart_pins.D3_C));
    /*p25.BUMA*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D4 = not(/*p25.SYSA*/ not(cart_pins.D4_C));
    /*p25.BUPY*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D5 = not(/*p25.SUGY*/ not(cart_pins.D5_C));
    /*p25.BASY*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D6 = not(/*p25.TUBE*/ not(cart_pins.D6_C));
    /*p25.BAPE*/ if (!dma_sig.DMA_READ_CARTn) oam_pins.B_D7 = not(/*p25.SYZO*/ not(cart_pins.D7_C));
  }

  {
    /*p31.XEGA*/ wire OAM_A_IN_CLK = not(oam_pins.CLK_A);
    /*p31.YLOR*/ OAM_A_D0.set(OAM_A_IN_CLK, OAM_A_LATCH0);
    /*p31.ZYTY*/ OAM_A_D1.set(OAM_A_IN_CLK, OAM_A_LATCH1);
    /*p31.ZYVE*/ OAM_A_D2.set(OAM_A_IN_CLK, OAM_A_LATCH2);
    /*p31.ZEZY*/ OAM_A_D3.set(OAM_A_IN_CLK, OAM_A_LATCH3);
    /*p31.GOMO*/ OAM_A_D4.set(OAM_A_IN_CLK, OAM_A_LATCH4);
    /*p31.BAXO*/ OAM_A_D5.set(OAM_A_IN_CLK, OAM_A_LATCH5);
    /*p31.YZOS*/ OAM_A_D6.set(OAM_A_IN_CLK, OAM_A_LATCH6);
    /*p31.DEPO*/ OAM_A_D7.set(OAM_A_IN_CLK, OAM_A_LATCH7);

    /*p29.YWOK*/ wire OAM_B_IN_CLK = not(oam_pins.CLK_A);
    /*p29.XUSO*/ OAM_B_D0.set(OAM_B_IN_CLK, OAM_B_LATCH0);
    /*p29.XEGU*/ OAM_B_D1.set(OAM_B_IN_CLK, OAM_B_LATCH1);
    /*p29.YJEX*/ OAM_B_D2.set(OAM_B_IN_CLK, OAM_B_LATCH2);
    /*p29.XYJU*/ OAM_B_D3.set(OAM_B_IN_CLK, OAM_B_LATCH3);
    /*p29.YBOG*/ OAM_B_D4.set(OAM_B_IN_CLK, OAM_B_LATCH4);
    /*p29.WYSO*/ OAM_B_D5.set(OAM_B_IN_CLK, OAM_B_LATCH5);
    /*p29.XOTE*/ OAM_B_D6.set(OAM_B_IN_CLK, OAM_B_LATCH6);
    /*p29.YZAB*/ OAM_B_D7.set(OAM_B_IN_CLK, OAM_B_LATCH7);
  }

  {
    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(bus_sig.ASOT_CPURD, cpu.FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
  
    /*p28.WAFO*/ wire OAM_A0n   = not(oam_pins.A0);
    /*p28.WUKU*/ wire OAM_A_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, oam_pins.A0);
    /*p28.GUKO*/ wire OAM_B_CPU_RDn = and(CPU_READ_MYSTERY, OAM_LOCKn, OAM_A0n);
    /*p28.WEWU*/ wire OAM_A_CPU_RD  = not(OAM_A_CPU_RDn);
    /*p28.WUME*/ wire OAM_B_CPU_RD  = not(OAM_B_CPU_RDn);

    if (OAM_A_CPU_RD) bus_tri.set_data(
      /*p31.XACA*/ OAM_A_LATCH0,
      /*p31.XAGU*/ OAM_A_LATCH1,
      /*p31.XEPU*/ OAM_A_LATCH2,
      /*p31.XYGU*/ OAM_A_LATCH3,
      /*p31.XUNA*/ OAM_A_LATCH4,
      /*p31.DEVE*/ OAM_A_LATCH5,
      /*p31.ZEHA*/ OAM_A_LATCH6,
      /*p31.FYRA*/ OAM_A_LATCH7
    );

    if (OAM_B_CPU_RD) bus_tri.set_data(
      /*p29.YFAP*/ OAM_B_LATCH0,
      /*p29.XELE*/ OAM_B_LATCH1,
      /*p29.YPON*/ OAM_B_LATCH2,
      /*p29.XUVO*/ OAM_B_LATCH3,
      /*p29.ZYSA*/ OAM_B_LATCH4,
      /*p29.YWEG*/ OAM_B_LATCH5,
      /*p29.XABU*/ OAM_B_LATCH6,
      /*p29.YTUX*/ OAM_B_LATCH7
    );
  }

  return {
    /*p31.ZAGO*/ .OAM_A_D0b = not(!OAM_A_D0),
    /*p31.ZOCY*/ .OAM_A_D1b = not(!OAM_A_D1),
    /*p31.YPUR*/ .OAM_A_D2b = not(!OAM_A_D2),
    /*p31.YVOK*/ .OAM_A_D3b = not(!OAM_A_D3),
    /*p31.COSE*/ .OAM_A_D4b = not(!OAM_A_D4),
    /*p31.AROP*/ .OAM_A_D5b = not(!OAM_A_D5),
    /*p31.XATU*/ .OAM_A_D6b = not(!OAM_A_D6),
    /*p31.BADY*/ .OAM_A_D7b = not(!OAM_A_D7),
  };
}

//-----------------------------------------------------------------------------

};