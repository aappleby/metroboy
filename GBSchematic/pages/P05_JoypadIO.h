#pragma once

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P05_JoypadIO {

  // joypad int is in p02, so there's no outputs here

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

//private:

  // FF00 JOYP
  bool JUTE_00,KECY_01,JALE_02,KYME_03,KELY_04,COFY_05,KUKO_06,KERU_07;

  // FF00 JOYP read low bits
  bool KEVU_00,KAPA_01,KEJA_02,KOLO_03;
  bool KEMA_00,KURO_01,KUVE_02,JEKU_03,KOCE_04,CUDY_05;

  // some debug thing?
  bool ANOC_00,ATAJ_01,AJEC_02,ASUZ_03,BENU_04,AKAJ_05,ARAR_06,BEDA_07;

  bool BYZO,AXYN,ADYR,APYS,AFOP;
  bool KOLE,KYBU;
  bool KYTO,KABU;
  bool KORY,KALE;
  bool KURA,CELA,KARU;
  bool KYHU,KASY;

  bool KORE,JEVA,KYWE;
  bool KENA;
};