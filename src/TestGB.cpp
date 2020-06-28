#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void TestGB::ext_preset() {
  ext_bus.preset();

  if (!ext_bus.PIN_RD_A && !ext_bus.PIN_CS_A) {
    uint16_t ext_addr = ext_bus.get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    ext_bus.preset_d(false, 0);
  }
}

//-----------------------------------------------------------------------------

bool TestGB::commit_everything() {
  bool changed = false;

  /*p07.BURO*/ changed |= dbg_reg.BURO_FF60_0.commit_reg();
  /*p07.AMUT*/ changed |= dbg_reg.AMUT_FF60_1.commit_reg();

  /*p04.MATU*/ changed |= dma_reg.MATU_DMA_OAM_WRp.commit_reg(); // -> p25,p28
  /*p04.MYTE*/ changed |= dma_reg.MYTE_DMA_DONE.commit_reg();
  /*p04.LUVY*/ changed |= dma_reg.LUVY_DMA_TRIG_d0.commit_reg();
  /*p04.LENE*/ changed |= dma_reg.LENE_DMA_TRIG_d4.commit_reg();
  /*p04.NAKY*/ changed |= dma_reg.DMA_A00.commit_reg();
  /*p04.PYRO*/ changed |= dma_reg.DMA_A01.commit_reg(); 
  /*p04.NEFY*/ changed |= dma_reg.DMA_A02.commit_reg(); 
  /*p04.MUTY*/ changed |= dma_reg.DMA_A03.commit_reg(); 
  /*p04.NYKO*/ changed |= dma_reg.DMA_A04.commit_reg(); 
  /*p04.PYLO*/ changed |= dma_reg.DMA_A05.commit_reg(); 
  /*p04.NUTO*/ changed |= dma_reg.DMA_A06.commit_reg(); 
  /*p04.MUGU*/ changed |= dma_reg.DMA_A07.commit_reg(); 
  /*p04.NAFA*/ changed |= dma_reg.DMA_A08.commit_reg(); 
  /*p04.PYNE*/ changed |= dma_reg.DMA_A09.commit_reg(); 
  /*p04.PARA*/ changed |= dma_reg.DMA_A10.commit_reg(); 
  /*p04.NYDO*/ changed |= dma_reg.DMA_A11.commit_reg(); 
  /*p04.NYGY*/ changed |= dma_reg.DMA_A12.commit_reg(); 
  /*p04.PULA*/ changed |= dma_reg.DMA_A13.commit_reg(); 
  /*p04.POKU*/ changed |= dma_reg.DMA_A14.commit_reg(); 
  /*p04.MARU*/ changed |= dma_reg.DMA_A15.commit_reg(); 
  /*p04.LYXE*/ changed |= dma_reg.LYXE_DMA_LATCHn.commit_latch();

  // NAND latch
  /*p04.LARA*/ changed |= dma_reg.LARA_DMA_LATCHn.commit_gate();
  /*p04.LOKY*/ changed |= dma_reg.LOKY_DMA_LATCHp.commit_gate();

  /*p02.LOPE*/ changed |= int_reg.LOPE_FF0F_0.commit_reg();
  /*p02.UBUL*/ changed |= int_reg.UBUL_FF0F_3.commit_reg();
  /*p02.ULAK*/ changed |= int_reg.ULAK_FF0F_4.commit_reg();
  /*p02.LALU*/ changed |= int_reg.LALU_FF0F_1.commit_reg();
  /*p02.NYBO*/ changed |= int_reg.NYBO_FF0F_2.commit_reg();
  /*p02.MATY*/ changed |= int_reg.FF0F_L0.commit_latch();
  /*p02.NEJY*/ changed |= int_reg.FF0F_L1.commit_latch();
  /*p02.NUTY*/ changed |= int_reg.FF0F_L2.commit_latch();
  /*p02.MOPO*/ changed |= int_reg.FF0F_L3.commit_latch();
  /*p02.PAVY*/ changed |= int_reg.FF0F_L4.commit_latch();

  /*p02.BATU*/ changed |= joy_reg.JP_GLITCH0.commit_reg();
  /*p02.ACEF*/ changed |= joy_reg.JP_GLITCH1.commit_reg();
  /*p02.AGEM*/ changed |= joy_reg.JP_GLITCH2.commit_reg();
  /*p02.APUG*/ changed |= joy_reg.JP_GLITCH3.commit_reg();
  /*p05.JUTE*/ changed |= joy_reg.JOYP_RA.commit_reg();
  /*p05.KECY*/ changed |= joy_reg.JOYP_LB.commit_reg();
  /*p05.JALE*/ changed |= joy_reg.JOYP_UC.commit_reg();
  /*p05.KYME*/ changed |= joy_reg.JOYP_DS.commit_reg();
  /*p05.KELY*/ changed |= joy_reg.JOYP_UDLR.commit_reg();
  /*p05.COFY*/ changed |= joy_reg.JOYP_ABCS.commit_reg();
  /*p05.KUKO*/ changed |= joy_reg.DBG_FF00_D6.commit_reg();
  /*p05.KERU*/ changed |= joy_reg.DBG_FF00_D7.commit_reg();
  /*p05.KEVU*/ changed |= joy_reg.JOYP_L0.commit_reg();
  /*p05.KAPA*/ changed |= joy_reg.JOYP_L1.commit_reg();
  /*p05.KEJA*/ changed |= joy_reg.JOYP_L2.commit_reg();
  /*p05.KOLO*/ changed |= joy_reg.JOYP_L3.commit_reg();
  /*p02.AWOB*/ changed |= joy_reg.WAKE_CPU.commit_reg();

  changed |= lcd_reg.commit();

  changed |= pxp_reg.commit();

  /*p29.TOXE*/ changed |= ppu_reg.TOXE_SFETCH_S0.commit_reg();
  /*p29.TULY*/ changed |= ppu_reg.TULY_SFETCH_S1.commit_reg();
  /*p29.TESE*/ changed |= ppu_reg.TESE_SFETCH_S2.commit_reg();
  /*p29.TOBU*/ changed |= ppu_reg.TOBU_SFETCH_S1_D2.commit_reg();
  /*p29.VONU*/ changed |= ppu_reg.VONU_SFETCH_S1_D4.commit_reg();
  /*p29.SEBA*/ changed |= ppu_reg.SEBA_SFETCH_S1_D5.commit_reg();
  /*p29.TYFO*/ changed |= ppu_reg.TYFO_SFETCH_S0_D1.commit_reg();

  changed |= sst_reg.commit();

  changed |= tim_reg.commit();

  changed |= ppu_reg.commit();

  changed |= win_reg.commit();

  changed |= vram_bus.TS_MA00.commit_tribuf();
  changed |= vram_bus.TS_MA01.commit_tribuf();
  changed |= vram_bus.TS_MA02.commit_tribuf();
  changed |= vram_bus.TS_MA03.commit_tribuf();
  changed |= vram_bus.TS_MA04.commit_tribuf();
  changed |= vram_bus.TS_MA05.commit_tribuf();
  changed |= vram_bus.TS_MA06.commit_tribuf();
  changed |= vram_bus.TS_MA07.commit_tribuf();
  changed |= vram_bus.TS_MA08.commit_tribuf();
  changed |= vram_bus.TS_MA09.commit_tribuf();
  changed |= vram_bus.TS_MA10.commit_tribuf();
  changed |= vram_bus.TS_MA11.commit_tribuf();
  changed |= vram_bus.TS_MA12.commit_tribuf();
  changed |= vram_bus.TS_MD0.commit_tribuf();
  changed |= vram_bus.TS_MD1.commit_tribuf();
  changed |= vram_bus.TS_MD2.commit_tribuf();
  changed |= vram_bus.TS_MD3.commit_tribuf();
  changed |= vram_bus.TS_MD4.commit_tribuf();
  changed |= vram_bus.TS_MD5.commit_tribuf();
  changed |= vram_bus.TS_MD6.commit_tribuf();
  changed |= vram_bus.TS_MD7.commit_tribuf();

  changed |= ext_bus.commit();
  //----------------------------------------
  // PINS

  changed |= cpu_bus.PIN_TO_CPU2.commit_pinout();       // <- P02.AWOB
  changed |= cpu_bus.PIN_CPU_RAW_RD.clear_preset();     // PORTA_00: -> UJYV, LAGU, LAVO
  changed |= cpu_bus.PIN_CPU_RAW_WR.clear_preset();     // PORTA_01: -> AREV, LAGU.
  changed |= cpu_bus.PIN_READ_BOOTROM.commit_pinout();  // PORTA_04: <- TUTU
  changed |= cpu_bus.PIN_ADDR_VALID.clear_preset();     // PORTA_06: -> APAP, TEXO
  changed |= int_reg.PIN_ACK_VBLANK.clear_preset();     // PORTB_01: -> LETY, vblank int ack
  changed |= int_reg.PIN_INT_VBLANK.commit_pinout();    // PORTB_03: <- LOPE, vblank int
  changed |= int_reg.PIN_ACK_STAT.clear_preset();       // PORTB_05: -> LEJA, stat int ack
  changed |= int_reg.PIN_INT_STAT.commit_pinout();      // PORTB_07: <- LALU, stat int
  changed |= int_reg.PIN_ACK_TIMER.clear_preset();      // PORTB_09: -> LESA, timer int ack
  changed |= int_reg.PIN_INT_TIMER.commit_pinout();     // PORTB_11: <- NYBO, timer int
  changed |= int_reg.PIN_ACK_SERIAL.clear_preset();     // PORTB_13: -> LUFE, serial int ack
  changed |= int_reg.PIN_INT_SERIAL.commit_pinout();    // PORTB_15: <- UBUL, serial int
  changed |= int_reg.PIN_ACK_JOYPAD.clear_preset();     // PORTB_17: -> LAMO, joypad int ack
  changed |= int_reg.PIN_INT_JOYPAD.commit_pinout();    // PORTB_19: <- ULAK, joypad int
  changed |= cpu_bus.PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL
  changed |= cpu_bus.PIN_AFER.commit_pinout();          // PORTC_01: <- AFER
  changed |= cpu_bus.PIN_RESET.commit_pinout();         // PORTC_02: <- PIN_RESET directly connected to the pad
  changed |= cpu_bus.PIN_CLK_GOOD.commit_pinout();      // PORTC_03: <- CLKIN_A
  changed |= cpu_bus.PIN_CPU_RESET.commit_pinout();     // PORTC_04: <- TABA
  changed |= cpu_bus.PIN_FROM_CPU6.clear_preset();      // PORTD_00: -> LEXY, doesn't do anything

  changed |= cpu_bus.PIN_BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- BOWA
  changed |= cpu_bus.PIN_BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- BEDO _____fgh

  changed |= cpu_bus.PIN_BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  changed |= cpu_bus.PIN_BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- BUDE abcd____
  
  changed |= cpu_bus.PIN_BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO
  changed |= cpu_bus.PIN_FROM_CPU5p.clear_preset();      // PORTD_05: -> FROM_CPU5
  changed |= cpu_bus.PIN_BUKE_ABxxxxxH.commit_pinout(); // PORTD_06: <- BUKE _____f__

  changed |= cpu_bus.PIN_BOMA_xBxxxxxx.commit_pinout(); // PORTD_07: <- BOMA _____fgh
  changed |= cpu_bus.PIN_BOGA_AxCDEFGH.commit_pinout(); // PORTD_08: <- BOGA abcde___

  changed |= cpu_bus.PIN_A00.clear_preset();
  changed |= cpu_bus.PIN_A01.clear_preset();
  changed |= cpu_bus.PIN_A02.clear_preset();
  changed |= cpu_bus.PIN_A03.clear_preset();
  changed |= cpu_bus.PIN_A04.clear_preset();
  changed |= cpu_bus.PIN_A05.clear_preset();
  changed |= cpu_bus.PIN_A06.clear_preset();
  changed |= cpu_bus.PIN_A07.clear_preset();
  changed |= cpu_bus.PIN_A08.clear_preset();
  changed |= cpu_bus.PIN_A09.clear_preset();
  changed |= cpu_bus.PIN_A10.clear_preset();
  changed |= cpu_bus.PIN_A11.clear_preset();
  changed |= cpu_bus.PIN_A12.clear_preset();
  changed |= cpu_bus.PIN_A13.clear_preset();
  changed |= cpu_bus.PIN_A14.clear_preset();
  changed |= cpu_bus.PIN_A15.clear_preset();
  changed |= cpu_bus.TS_D0.commit_tribuf();
  changed |= cpu_bus.TS_D1.commit_tribuf();
  changed |= cpu_bus.TS_D2.commit_tribuf();
  changed |= cpu_bus.TS_D3.commit_tribuf();
  changed |= cpu_bus.TS_D4.commit_tribuf();
  changed |= cpu_bus.TS_D5.commit_tribuf();
  changed |= cpu_bus.TS_D6.commit_tribuf();
  changed |= cpu_bus.TS_D7.commit_tribuf();

  // Package pins

  /* PIN_25 */ changed |= vram_pins.PIN_MD7_A.commit_pinout();    // <- RYZE
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_B.commit_pinout();    // <- ROFA
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_C.clear_preset();     // -> RAKU
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_D.commit_pinout();    // <- RADY
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_A.commit_pinout();    // <- REKU
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_B.commit_pinout();    // <- ROFA
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_C.clear_preset();     // -> RETA
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_D.commit_pinout();    // <- RYTY
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_A.commit_pinout();    // <- REVU
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_B.commit_pinout();    // <- ROFA
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_C.clear_preset();     // -> ROPU
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_D.commit_pinout();    // <- RUMU
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_A.commit_pinout();    // <- RYRO
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_B.commit_pinout();    // <- ROFA
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_C.clear_preset();     // -> ROCE
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_D.commit_pinout();    // <- RUBE
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_A.commit_pinout();    // <- RADA
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_B.commit_pinout();    // <- ROFA
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_C.clear_preset();     // -> REMO
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_D.commit_pinout();    // <- RODU
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_A.commit_pinout();    // <- RAZO
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_B.commit_pinout();    // <- ROFA
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_C.clear_preset();     // -> RYDO
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_D.commit_pinout();    // <- RARE
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_A.commit_pinout();    // <- RYKY
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_B.commit_pinout();    // <- ROFA
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_C.clear_preset();     // -> REBA
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_D.commit_pinout();    // <- RULY
  /* PIN_32 */ /*GND*/
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_A.commit_pinout();    // <- REGE
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_B.commit_pinout();    // <- ROFA
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_C.clear_preset();     // -> RODY
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_D.commit_pinout();    // <- RURA
  /* PIN_34 */ changed |= vram_pins.PIN_MA00_AD.commit_pinout();  // <- ECAL
  /* PIN_35 */ changed |= vram_pins.PIN_MA01_AD.commit_pinout();  // <- EGEZ
  /* PIN_36 */ changed |= vram_pins.PIN_MA02_AD.commit_pinout();  // <- FUHE
  /* PIN_37 */ changed |= vram_pins.PIN_MA03_AD.commit_pinout();  // <- FYZY
  /* PIN_38 */ changed |= vram_pins.PIN_MA04_AD.commit_pinout();  // <- DAMU
  /* PIN_39 */ changed |= vram_pins.PIN_MA05_AD.commit_pinout();  // <- DAVA
  /* PIN_40 */ changed |= vram_pins.PIN_MA06_AD.commit_pinout();  // <- ETEG
  /* PIN_41 */ changed |= vram_pins.PIN_MA07_AD.commit_pinout();  // <- EREW
  /* PIN_42 */ changed |= vram_pins.PIN_MA12_AD.commit_pinout();  // <- EXYF
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_A.commit_pinout();   // <- SOKY
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_C.clear_preset();     // -> TEFY
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_D.commit_pinout();   // <- SETY
  /* PIN_44 */ changed |= vram_pins.PIN_MA10_AD.commit_pinout();  // <- ERAF
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_A.commit_pinout();   // <- REFO
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_C.clear_preset();     // -> TAVY
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_D.commit_pinout();   // <- SAHA
  /* PIN_46 */ changed |= vram_pins.PIN_MA11_AD.commit_pinout();  // <- FUSY
  /* PIN_47 */ changed |= vram_pins.PIN_MA09_AD.commit_pinout();  // <- DUVE
  /* PIN_48 */ changed |= vram_pins.PIN_MA08_AD.commit_pinout();  // <- EVAX
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_A.commit_pinout();   // <- SYSY
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_C.clear_preset();     // -> SUDO
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_D.commit_pinout();   // <- RAGU
  
  changed |= lcd_reg.commit_pins();
  
  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/
  /* PIN_62 */ changed |= joy_pin.P15_A.commit_pinout();   // <- CELA
  /* PIN_62 */ changed |= joy_pin.P15_D.commit_pinout();   // <- COFY
  /* PIN_63 */ changed |= joy_pin.P14_A.commit_pinout();   // <- KARU
  /* PIN_63 */ changed |= joy_pin.P14_D.commit_pinout();   // <- KELY
  /* PIN_64 */ changed |= joy_pin.P13_A.commit_pinout();   // <- KORY
  /* PIN_64 */ changed |= joy_pin.P13_B.commit_pinout();   
  /* PIN_64 */ changed |= joy_pin.P13_C.clear_preset();    // -> KERY, P05.KOLO
  /* PIN_64 */ changed |= joy_pin.P13_D.commit_pinout();   // <- KALE
  /* PIN_65 */ changed |= joy_pin.P12_A.commit_pinout();   // <- KYHU
  /* PIN_65 */ changed |= joy_pin.P12_B.commit_pinout();   
  /* PIN_65 */ changed |= joy_pin.P12_C.clear_preset();    // -> KERY, P05.KEJA
  /* PIN_65 */ changed |= joy_pin.P12_D.commit_pinout();   // <- KASY
  /* PIN_66 */ changed |= joy_pin.P11_A.commit_pinout();   // <- KYTO
  /* PIN_66 */ changed |= joy_pin.P11_B.commit_pinout();   
  /* PIN_66 */ changed |= joy_pin.P11_C.clear_preset();    // -> KERY, P05.KAPA
  /* PIN_66 */ changed |= joy_pin.P11_D.commit_pinout();   // <- KABU
  /* PIN_67 */ changed |= joy_pin.P10_A.commit_pinout();   // <- KOLE
  /* PIN_67 */ changed |= joy_pin.P10_B.commit_pinout();   
  /* PIN_67 */ changed |= joy_pin.P10_C.clear_preset();    // -> KERY, KEVU
  /* PIN_67 */ changed |= joy_pin.P10_D.commit_pinout();   // <- KYBU

  changed |= ser_reg.commit();

  /* PIN_71 */ changed |= ext_bus.PIN_RST.clear_preset();
  /* PIN_72 */ /*GND*/
  /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= ext_bus.PIN_CLK_GOOD.clear_preset();
  /* PIN_74 */ changed |= ext_bus.PIN_CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ changed |= ext_bus.PIN_PHI.commit_pinout();     // <- BUDE/BEVA
  /* PIN_76 */ changed |= ext_bus.PIN_T2.clear_preset();
  /* PIN_77 */ changed |= ext_bus.PIN_T1.clear_preset();
  /* PIN_78 */ changed |= ext_bus.PIN_WR_A.commit_pinout();    // <- UVER
  /* PIN_78 */ changed |= ext_bus.PIN_WR_C.clear_preset();     // -> UBAL
  /* PIN_78 */ changed |= ext_bus.PIN_WR_D.commit_pinout();    // <- USUF
  /* PIN_79 */ changed |= ext_bus.PIN_RD_A.commit_pinout();    // <- UGAC
  /* PIN_79 */ changed |= ext_bus.PIN_RD_C.clear_preset();     // -> UJYV
  /* PIN_79 */ changed |= ext_bus.PIN_RD_D.commit_pinout();    // <- URUN
  /* PIN_80 */ changed |= ext_bus.PIN_CS_A.commit_pinout();    // <- TYHO

  return changed;
}

//-----------------------------------------------------------------------------
