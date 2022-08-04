#include "GateBoyLib/GateBoyPins.h"

#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void PinsControl::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }

//-----------------------------------------------------------------------------

void PinsABusLo::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xB2); }

//-----------------------------------------------------------------------------

void PinsABusHi::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }

//-----------------------------------------------------------------------------

void PinsDBus::reset()    { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }

//-----------------------------------------------------------------------------

void PinsJoypad::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }

//-----------------------------------------------------------------------------

void PinsSerial::reset() {
  PIN_68_SCK. state = 0b00010100;
  PIN_69_SIN. state = 0b00011000;
  PIN_70_SOUT.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void PinsVramControl::reset() {
  PIN_43_VRAM_CSn.state = BIT_OLD | BIT_DRIVEN | 0;
  PIN_45_VRAM_OEn.state = BIT_OLD | BIT_DRIVEN | 1;
  PIN_49_VRAM_WRn.state = BIT_OLD | BIT_DRIVEN | 0;
}

//-----------------------------------------------------------------------------

void PinsVramABus::reset() {
  PIN_34_VRAM_A00.state = 0b00011001;
  PIN_35_VRAM_A01.state = 0b00011001;
  PIN_36_VRAM_A02.state = 0b00011001;
  PIN_37_VRAM_A03.state = 0b00011001;
  PIN_38_VRAM_A04.state = 0b00011000;
  PIN_39_VRAM_A05.state = 0b00011001;
  PIN_40_VRAM_A06.state = 0b00011000;
  PIN_41_VRAM_A07.state = 0b00011001;
  PIN_48_VRAM_A08.state = 0b00011000;
  PIN_47_VRAM_A09.state = 0b00011000;
  PIN_44_VRAM_A10.state = 0b00011000;
  PIN_46_VRAM_A11.state = 0b00011000;
  PIN_42_VRAM_A12.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void PinsVramDBus::reset() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

//-----------------------------------------------------------------------------

void PinsSys::reset() {
  PIN_71_RST.state         = 0b00011000;
  PIN_73_CLK_DRIVE.state   = 0b00011000;
  PIN_74_CLK.CLK.state     = 0b00011000;
  PIN_74_CLK.CLKGOOD.state = 0b00011001;
  PIN_75_CLK_OUT.state     = 0b00011001;
  PIN_77_T1.state          = 0b00011000;
  PIN_76_T2.state          = 0b00011000;
}

/*_p01.UCOB*/ wire PinsSys::UCOB_CLKBADp_old() const { return not1(PIN_74_CLK.clkgood_old()); }
/*_p01.ATEZ*/ wire PinsSys::ATEZ_CLKBADp_old() const { return not1(PIN_74_CLK.clkgood_old()); }

/*_p07.UBET*/ wire PinsSys::UBETp_old() const { return not1(PIN_77_T1.qp_int_old()); }
/*_p07.UVAR*/ wire PinsSys::UVARp_old() const { return not1(PIN_76_T2.qp_int_old()); }

/*_p07.UMUT*/ wire PinsSys::UMUT_MODE_DBG1p_old() const { return and2(PIN_77_T1.qp_int_old(), UVARp_old()); }
/*_p08.MULE*/ wire PinsSys::MULE_MODE_DBG1n_old() const { return not1(UMUT_MODE_DBG1p_old()); }
/*_p07.UNOR*/ wire PinsSys::UNOR_MODE_DBG2p_old() const { return and2(PIN_76_T2.qp_int_old(), UBETp_old()); }
/*_p08.RYCA*/ wire PinsSys::RYCA_MODE_DBG2n_old() const { return not1(UNOR_MODE_DBG2p_old()); }
/*_p08.TOVA*/ wire PinsSys::TOVA_MODE_DBG2n_old() const { return not1(UNOR_MODE_DBG2p_old()); }
/*_p07.UPOJ*/ wire PinsSys::UPOJ_MODE_PRODn_old() const { return nand3(UBETp_old(), UVARp_old(), PIN_71_RST.qp_int_old()); }

/*_p01.UCOB*/ wire PinsSys::UCOB_CLKBADp_new() const { return not1(PIN_74_CLK.clkgood_new()); }
/*_p01.ATEZ*/ wire PinsSys::ATEZ_CLKBADp_new() const { return not1(PIN_74_CLK.clkgood_new()); }

/*_p07.UBET*/ wire PinsSys::UBETp_new() const { return not1(PIN_77_T1.qp_int_new()); }
/*_p07.UVAR*/ wire PinsSys::UVARp_new() const { return not1(PIN_76_T2.qp_int_new()); }

/*_p07.UMUT*/ wire PinsSys::UMUT_MODE_DBG1p_new() const { return and2(PIN_77_T1.qp_int_new(), UVARp_new()); }
/*_p08.MULE*/ wire PinsSys::MULE_MODE_DBG1n_new() const { return not1(UMUT_MODE_DBG1p_new()); }
/*_p07.UNOR*/ wire PinsSys::UNOR_MODE_DBG2p_new() const { return and2(PIN_76_T2.qp_int_new(), UBETp_new()); }
/*_p08.RYCA*/ wire PinsSys::RYCA_MODE_DBG2n_new() const { return not1(UNOR_MODE_DBG2p_new()); }
/*_p08.TOVA*/ wire PinsSys::TOVA_MODE_DBG2n_new() const { return not1(UNOR_MODE_DBG2p_new()); }
/*_p07.UPOJ*/ wire PinsSys::UPOJ_MODE_PRODn_new() const { return nand3(UBETp_new(), UVARp_new(), PIN_71_RST.qp_int_new()); }


//-----------------------------------------------------------------------------

void PinsLCD::reset() {
  PIN_50_LCD_DATA1.state    = 0b00011000;
  PIN_51_LCD_DATA0.state    = 0b00011000;
  PIN_54_LCD_HSYNC.state    = 0b00011001;
  PIN_56_LCD_FLIPS.state    = 0b00011001;
  PIN_52_LCD_CNTRL.state    = 0b00011001;
  PIN_55_LCD_LATCH.state    = 0b00011001;
  PIN_53_LCD_CLOCK.state    = 0b00011001;
  PIN_57_LCD_VSYNC.state    = 0b00011000;
}

//-----------------------------------------------------------------------------

void GateBoyPins::poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}
  
void GateBoyPins::reset()    {
  abus_lo.reset();
  abus_hi.reset();
  dbus.reset();
  vram_dbus.reset();
  vram_abus.reset();
  vram_ctrl.reset();
  lcd.reset();
  joy.reset();
  sys.reset();
  ctrl.reset();
}

void GateBoyPins::commit() {
  if (!config_check_flags && !config_use_flags) return;

  uint8_t* cursor = (uint8_t*)this;
  bool bad_bits = false;
  for (size_t i = 0; i < sizeof(*this); i++) {
    uint8_t s = *cursor;
    if (config_check_flags) {
      auto drive_flags = s & (BIT_DRIVEN | BIT_PULLED);

      if (drive_flags == (BIT_DRIVEN | BIT_PULLED)) {
        LOG_Y("Bit %d both driven and pulled up!\n", i);
        bad_bits = true;
      }

      if (drive_flags == 0) {
        LOG_Y("Bit %d floating!\n", i);
        bad_bits = true;
      }

      auto oldnew_flags = s & (BIT_OLD | BIT_NEW);

      if (oldnew_flags != BIT_NEW) {
        LOG_Y("Bit %d not dirty after sim pass!\n", i);
        bad_bits = true;
      }
    }

    *cursor++ = (s & 0b00001111) | BIT_OLD;
  }
  CHECK_N(bad_bits);
}

//-----------------------------------------------------------------------------
