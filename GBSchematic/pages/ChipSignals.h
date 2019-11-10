#pragma once

// Signals that go on/off chip

struct ChipSignals {
  //----------
  // Address pins

  /* PIN_01 */
  bool A00_A; // <- P08.KUPO
  bool A00_C; // -> P08.KOVA
  bool A00_D; // <- P08.KOTY

  /* PIN_02 */
  bool A01_A; // <- P08.CABA
  bool A01_C; // -> P08.CAMU
  bool A01_D; // <- P08.COTU

  /* PIN_03 */
  bool A02_A; // <- P08.BOKU
  bool A02_C; // -> P08.BUXU
  bool A02_D; // <- P08.BAJO

  /* PIN_04 */
  bool A03_A; // <- P08.BOTY
  bool A03_C; // -> P08.BASE
  bool A03_D; // <- P08.BOLA

  /* PIN_05 */
  bool A04_A; // <- P08.BYLA
  bool A04_C; // -> P08.AFEC
  bool A04_D; // <- P08.BEVO

  /* PIN_06 */
  bool A05_A; // <- P08.BADU
  bool A05_C; // -> P08.ABUP
  bool A05_D; // <- P08.AJAV

  /* PIN_07 */
  bool A06_A; // <- P08.CEPU
  bool A06_C; // -> P08.CYGU
  bool A06_D; // <- P08.CYKA

  /* PIN_08 */
  bool A07_A; // <- P08.DEFY
  bool A07_C; // -> P08.COGO
  bool A07_D; // <- P08.COLO

  /* PIN_09 */
  bool A08_A; // <- P08.MYNY
  bool A08_C; // -> P08.MUJY
  bool A08_D; // <- P08.MEGO

  /* PIN_10 */
  bool A09_A; // <- P08.MUNE
  bool A09_C; // -> P08.NENA
  bool A09_D; // <- P08.MENY

  /* PIN_11 */
  bool A10_A; // <- P08.ROXU
  bool A10_C; // -> P08.SURA
  bool A10_D; // <- P08.RORE

  /* PIN_12 */
  bool A11_A; // <- P08.LEPY
  bool A11_C; // -> P08.MADY
  bool A11_D; // <- P08.LYNY

  /* PIN_13 */
  bool A12_A; // <- P08.LUCE
  bool A12_C; // -> P08.LAHE
  bool A12_D; // <- P08.LOSO

  /* PIN_14 */
  bool A13_A; // <- P08.LABE
  bool A13_C; // -> P08.LURA
  bool A13_D; // <- P08.LEVA

  /* PIN_15 */
  bool A14_A; // <- P08.PUHE
  bool A14_C; // -> P08.PEVO
  bool A14_D; // <- P08.PAHY

  /* PIN_16 */
  bool A15_A; // <- P08.SUZE
  bool A15_C; // -> P08.RAZA
  bool A15_D; // <- P08.RULO

  //----------
  // Data pins

  /* PIN_17 */
  bool D0_A;  // <- P08.RUXA
  bool D0_B;  // <- P08.LULA
  bool D0_C;  // -> P08.TOVO,SOMA
  bool D0_D;  // <- P08.RUNE

  /* PIN_18 */
  bool D1_A;  // <- P08.RUJA
  bool D1_B;  // <- P08.LULA
  bool D1_C;  // -> P08.RUZY,RONY
  bool D1_D;  // <- P08.RYPU

  /* PIN_19 */
  bool D2_A;  // <- P08.RABY
  bool D2_B;  // <- P08.LULA
  bool D2_C;  // -> P08.ROME,RAXY
  bool D2_D;  // <- P08.SULY

  /* PIN_20 */
  bool D3_A;  // <- P08.RERA
  bool D3_B;  // <- P08.LULA
  bool D3_C;  // -> P08.SAZA,SELO
  bool D3_D;  // <- P08.SEZE

  /* PIN_21 */
  bool D4_A;  // <- P08.RORY
  bool D4_B;  // <- P08.LULA
  bool D4_C;  // -> P08.TEHE,SODY
  bool D4_D;  // <- P08.RESY

  /* PIN_22 */
  bool D5_A;  // <- P08.RYVO
  bool D5_B;  // <- P08.LULA
  bool D5_C;  // -> P08.RATU,SAGO
  bool D5_D;  // <- P08.TAMU

  /* PIN_23 */
  bool D6_A;  // <- P08.RAFY
  bool D6_B;  // <- P08.LULA
  bool D6_C;  // -> P08.SOCA,RUPA
  bool D6_D;  // <- P08.ROGY

  /* PIN_24 */
  bool D7_A;  // <- P08.RAVU
  bool D7_B;  // <- P08.LULA
  bool D7_C;  // -> P08.RYBA,SAZY
  bool D7_D;  // <- P08.RYDA

  //----------
  // VRAM data pins

  /* PIN_25 */
  bool MD7_A; // <- P25.RYZE
  bool MD7_B; // <- P25.ROFA
  bool MD7_C;
  bool MD7_D;

  /* PIN_26 */
  bool MD6_A; // <- P25.REKU
  bool MD6_B; // <- P25.ROFA
  bool MD6_C;
  bool MD6_D;

  /* PIN_27 */
  bool MD5_A; // <- P25.REVU
  bool MD5_B; // <- P25.ROFA
  bool MD5_C;
  bool MD5_D;

  /* PIN_28 */
  bool MD4_A; // <- P25.RYRO
  bool MD4_B; // <- P25.ROFA
  bool MD4_C;
  bool MD4_D;

  /* PIN_29 */
  bool MD3_A; // <- P25.RADA
  bool MD3_B; // <- P25.ROFA
  bool MD3_C;
  bool MD3_D;

  /* PIN_30 */
  bool MD2_A; // <- P25.RAZO
  bool MD2_B; // <- P25.ROFA
  bool MD2_C;
  bool MD2_D;

  /* PIN_31 */
  bool MD1_A; // <- P25.RYKY
  bool MD1_B; // <- P25.ROFA
  bool MD1_C;
  bool MD1_D;

  /* PIN_32 = GND */

  /* PIN_33 */
  bool MD0_A; // <- P25.REGE
  bool MD0_B; // <- P25.ROFA
  bool MD0_C; // -> P25.RODY
  bool MD0_D;

  //----------
  // VRAM address pins

  /* PIN_34 */
  bool MA00; // <- P04.ECAL

  /* PIN_35 */
  bool MA01; // <- P04.EGEZ

  /* PIN_36 */
  bool MA02; // <- P04.FUHE

  /* PIN_37 */
  bool MA03; // <- P04.FYZY

  /* PIN_38 */
  bool MA04; // <- P04.DAMU

  /* PIN_39 */
  bool MA05; // <- P04.DAVA

  /* PIN_40 */
  bool MA06; // <- P04.ETEG

  /* PIN_41 */
  bool MA07; // <- P04.EREW

  /* PIN_42 */
  bool MA12; // <- P04.EXYF

  /* PIN_43 */
  bool MCS_A; // <- P25.SOKY
  bool MCS_C; // -> P25.TEFY
  bool MCS_D; // <- P25.SETY

  /* PIN_44 */
  bool MA10;  // <- P04.ERAF

  /* PIN_45 */
  bool MOE_A; // <- P25.REFO
  bool MOE_C; // -> P25.TAVY
  bool MOE_D; // <- P25.SAHA

  /* PIN_46 */
  bool MA11;  // <- P04.FUSY

  /* PIN_47 */
  bool MA09;  // <- P04.DUVE

  /* PIN_48 */
  bool MA08;  // <- P04.EVAX

  /* PIN_49 */
  bool MWR_A; // <- P25.SYSY
  bool MWR_C; // -> P25.SUDO
  bool MWR_D; // <- P25.RAGU

  //----------
  // LCD pins

  /* PIN_50 */
  bool LD1;

  /* PIN_51 */
  bool LD0;

  /* Unconnected pad between LDO and CPG */
  bool PIN_NC; // <- P07.LEXY

  /* PIN_52 */
  bool CPG;

  /* PIN_53 */
  bool CP;

  /* PIN_54 */
  bool ST;

  /* PIN_55 */
  bool CPL;

  /* PIN_56 */
  bool FR;

  /* PIN_57 */
  bool S;

  //----------
  // Audio pins

  /* PIN_58 = VCC */

  /* PIN_59 = ROUT */

  /* PIN_60 = LOUT */

  /* PIN_61 = VIN */

  //----------
  // Joypad pins

  /* PIN_62 */
  bool P15_A;
  bool P15_D;

  /* PIN_63 */
  bool P14_A;
  bool P14_D;

  // P10/11/12/13_B are all tied together, but I can't figure out who drives them. :/

  /* PIN_64 */
  bool P13_A; // <- P05.KORY
  bool P13_B;
  bool P13_C; // -> P02.KERY, P05.KOLO
  bool P13_D; // <- P05.KALE

  /* PIN_65 */
  bool P12_A; // <- P05.KYHU
  bool P12_B;
  bool P12_C; // -> P02.KERY, P05.KEJA
  bool P12_D; // <- P05.KASY

  /* PIN_66 */
  bool P11_A; // <- P05.KYTO
  bool P11_B;
  bool P11_C; // -> P02.KERY, P05.KAPA
  bool P11_D; // <- P05.KABU

  /* PIN_67 */
  bool P10_A; // <- KOLE
  bool P10_B;
  bool P10_C; // -> P02.KERY, P05.KEVU
  bool P10_D; // <- KYBU

  //----------
  // Serial pins

  /* PIN_68 */
  bool SCK_A; // <- P06.KEXU, out high
  bool SCK_B; // <- P06.CULY, out dir
  bool SCK_C; // -> P06.CAVE, in
  bool SCK_D; // <- P06.KUJO, out low

  /* PIN_69 */
  bool SIN_A;
  bool SIN_B;
  bool SIN_C; // -> P06.CAGE (check this)
  bool SIN_D;

  /* PIN_70 */
  bool SOUT; // <- P05.KENA

  //----------
  // Misc pins

  /* PIN_71 */
  bool RST;

  /* PIN_72 = GND */

  /* PIN_73 = XO */

  /* PIN_74 */
  bool CLKIN_A;
  bool CLKIN_B; // _b_d_f_h

  /* PIN_75 */
  bool PHI;

  /* PIN_76 */
  bool T2;   // -> P07.UVAR

  /* PIN_77 */
  bool T1;   // -> P07.UBET

  //----------
  // Cart rd/wr/cs pins

  /* PIN_78 */
  bool WR_A; // <- P08.UVER
  bool WR_C; // -> P07.UBAL
  bool WR_D; // <- P08.USUF

  /* PIN_79 */
  bool RD_A; // <- P08.UGAC
  bool RD_C; // -> P07.UJYV
  bool RD_D; // <- P08.URUN

  /* PIN_80 */
  bool CS_A; // <- P08.TYHO
};