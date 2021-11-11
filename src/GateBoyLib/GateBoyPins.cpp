#include "GateBoyLib/GateBoyPins.h"

#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void PinsControl::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
void PinsControl::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
void PinsControl::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }

//-----------------------------------------------------------------------------

void PinsABusLo::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xFF); }
void PinsABusLo::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xFF); }
void PinsABusLo::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xB2); }

//-----------------------------------------------------------------------------

void PinsABusHi::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }
void PinsABusHi::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }
void PinsABusHi::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }

//-----------------------------------------------------------------------------

void PinsDBus::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }
void PinsDBus::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }
void PinsDBus::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }

//-----------------------------------------------------------------------------

void PinsJoypad::reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }
void PinsJoypad::reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }
void PinsJoypad::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }

//-----------------------------------------------------------------------------

void PinsSerial::reset_to_cart() {
  PIN_68_SCK. state = 0b00010100;
  PIN_69_SIN. state = 0b00011000;
  PIN_70_SOUT.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void PinsVramControl::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void PinsVramControl::reset_to_bootrom() {
  PIN_43_VRAM_CSn.state = BIT_OLD | BIT_DRIVEN | 0;
  PIN_45_VRAM_OEn.state = BIT_OLD | BIT_DRIVEN | 1;
  PIN_49_VRAM_WRn.state = BIT_OLD | BIT_DRIVEN | 0;
}

void PinsVramControl::reset_to_cart() {
  PIN_43_VRAM_CSn.state = BIT_OLD | BIT_DRIVEN | 0;
  PIN_45_VRAM_OEn.state = BIT_OLD | BIT_DRIVEN | 1;
  PIN_49_VRAM_WRn.state = BIT_OLD | BIT_DRIVEN | 0;
}

//-----------------------------------------------------------------------------

void PinsVramABus::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void PinsVramABus::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

void PinsVramABus::reset_to_cart() {
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

void PinsVramDBus::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void PinsVramDBus::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

void PinsVramDBus::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
}

//-----------------------------------------------------------------------------

void PinsSys::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void PinsSys::reset_to_bootrom() {
  PIN_71_RST.state         = 0b00011000;
  PIN_73_CLK_DRIVE.state   = 0b00011000;
  PIN_74_CLK.CLK.state     = 0b00011000;
  PIN_74_CLK.CLKGOOD.state = 0b00011001;
  PIN_75_CLK_OUT.state     = 0b00011000;
  PIN_76_T2.state          = 0b00011000;
  PIN_77_T1.state          = 0b00011000;
}

void PinsSys::reset_to_cart() {
  PIN_71_RST.state         = 0b00011000;
  PIN_73_CLK_DRIVE.state   = 0b00011000;
  PIN_74_CLK.CLK.state     = 0b00011000;
  PIN_74_CLK.CLKGOOD.state = 0b00011001;
  PIN_75_CLK_OUT.state     = 0b00011001;
  PIN_77_T1.state          = 0b00011000;
  PIN_76_T2.state          = 0b00011000;
}

wire PinsSys::UCOB_CLKBADp_old()    const { /*_p01.UCOB*/ return not1(PIN_74_CLK.clkgood_old()); }
wire PinsSys::ATEZ_CLKBADp_old()    const { /*_p01.ATEZ*/ return not1(PIN_74_CLK.clkgood_old()); }

wire PinsSys::UBETp_old()           const { /*_p07.UBET*/ return not1(PIN_77_T1.qp_int_old()); }
wire PinsSys::UVARp_old()           const { /*_p07.UVAR*/ return not1(PIN_76_T2.qp_int_old()); }

wire PinsSys::UMUT_MODE_DBG1p_old() const { /*_p07.UMUT*/ return and2(PIN_77_T1.qp_int_old(), UVARp_old()); }
wire PinsSys::MULE_MODE_DBG1n_old() const { /*_p08.MULE*/ return not1(UMUT_MODE_DBG1p_old()); }
wire PinsSys::UNOR_MODE_DBG2p_old() const { /*_p07.UNOR*/ return and2(PIN_76_T2.qp_int_old(), UBETp_old()); }
wire PinsSys::RYCA_MODE_DBG2n_old() const { /*_p08.RYCA*/ return not1(UNOR_MODE_DBG2p_old()); }
wire PinsSys::TOVA_MODE_DBG2n_old() const { /*_p08.TOVA*/ return not1(UNOR_MODE_DBG2p_old()); }
wire PinsSys::UPOJ_MODE_PRODn_old() const { /*_p07.UPOJ*/ return nand3(UBETp_old(), UVARp_old(), PIN_71_RST.qp_int_old()); }

wire PinsSys::UCOB_CLKBADp_new()    const { /*_p01.UCOB*/ return not1(PIN_74_CLK.clkgood_new()); }
wire PinsSys::ATEZ_CLKBADp_new()    const { /*_p01.ATEZ*/ return not1(PIN_74_CLK.clkgood_new()); }

wire PinsSys::UBETp_new()           const { /*_p07.UBET*/ return not1(PIN_77_T1.qp_int_new()); }
wire PinsSys::UVARp_new()           const { /*_p07.UVAR*/ return not1(PIN_76_T2.qp_int_new()); }

wire PinsSys::UMUT_MODE_DBG1p_new() const { /*_p07.UMUT*/ return and2(PIN_77_T1.qp_int_new(), UVARp_new()); }
wire PinsSys::MULE_MODE_DBG1n_new() const { /*_p08.MULE*/ return not1(UMUT_MODE_DBG1p_new()); }
wire PinsSys::UNOR_MODE_DBG2p_new() const { /*_p07.UNOR*/ return and2(PIN_76_T2.qp_int_new(), UBETp_new()); }
wire PinsSys::RYCA_MODE_DBG2n_new() const { /*_p08.RYCA*/ return not1(UNOR_MODE_DBG2p_new()); }
wire PinsSys::TOVA_MODE_DBG2n_new() const { /*_p08.TOVA*/ return not1(UNOR_MODE_DBG2p_new()); }
wire PinsSys::UPOJ_MODE_PRODn_new() const { /*_p07.UPOJ*/ return nand3(UBETp_new(), UVARp_new(), PIN_71_RST.qp_int_new()); }


//-----------------------------------------------------------------------------

void PinsLCD::reset_to_poweron() {
  PIN_50_LCD_DATA1.state    = BIT_OLD | BIT_DRIVEN;
  PIN_51_LCD_DATA0.state    = BIT_OLD | BIT_DRIVEN;
  PIN_54_LCD_HSYNC.state    = BIT_OLD | BIT_DRIVEN;
  PIN_56_LCD_FLIPS.state    = BIT_OLD | BIT_DRIVEN;
  PIN_52_LCD_CNTRL.state    = BIT_OLD | BIT_DRIVEN;
  PIN_55_LCD_LATCH.state    = BIT_OLD | BIT_DRIVEN;
  PIN_53_LCD_CLOCK.state    = BIT_OLD | BIT_DRIVEN;
  PIN_57_LCD_VSYNC.state    = BIT_OLD | BIT_DRIVEN;
}

void PinsLCD::reset_to_bootrom() {
  PIN_50_LCD_DATA1.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_51_LCD_DATA0.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_54_LCD_HSYNC.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_56_LCD_FLIPS.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_52_LCD_CNTRL.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_55_LCD_LATCH.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_53_LCD_CLOCK.state    = BIT_OLD | BIT_DRIVEN | 1;
  PIN_57_LCD_VSYNC.state    = BIT_OLD | BIT_DRIVEN | 1;
}

void PinsLCD::reset_to_cart() {
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

void GateBoyPins::reset_to_poweron() {
  abus_lo.reset_to_poweron();
  abus_hi.reset_to_poweron();
  dbus.reset_to_poweron();
  vram_dbus.reset_to_poweron();
  vram_abus.reset_to_poweron();
  vram_ctrl.reset_to_poweron();
  lcd.reset_to_poweron();
  joy.reset_to_poweron();
  sys.reset_to_poweron();
  ctrl.reset_to_poweron();
}
  
void GateBoyPins::reset_to_bootrom() {
  abus_lo.reset_to_bootrom();
  abus_hi.reset_to_bootrom();
  dbus.reset_to_bootrom();
  vram_dbus.reset_to_bootrom();
  vram_abus.reset_to_bootrom();
  vram_ctrl.reset_to_bootrom();
  lcd.reset_to_bootrom();
  joy.reset_to_bootrom();
  sys.reset_to_bootrom();
  ctrl.reset_to_bootrom();
}
  
void GateBoyPins::reset_to_cart()    {
  abus_lo.reset_to_cart();
  abus_hi.reset_to_cart();
  dbus.reset_to_cart();
  vram_dbus.reset_to_cart();
  vram_abus.reset_to_cart();
  vram_ctrl.reset_to_cart();
  lcd.reset_to_cart();
  joy.reset_to_cart();
  sys.reset_to_cart();
  ctrl.reset_to_cart();
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
