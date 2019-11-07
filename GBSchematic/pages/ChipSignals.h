#pragma once

// Signals that go on/off chip

struct ChipSignals {
  bool RESET;
  bool CLKIN_A;
  bool CLKIN_B;

  bool T1;
  bool T2;

  // should move these to Gameboy
  bool T1nT2;
  bool T1nT2n;
  bool T1T2n;

  bool RD_A; // P08 UGAC -> RD_A
  bool RD_C; // P07 RD_C -> UJYV
  bool RD_D; // P08 URUN -> RD_D

  bool WR_A; // P08 UVER -> WR_A
  bool WR_C; // P07 WR_C -> UBAL
  bool WR_D; // P08 USUF -> WR_D

  // signals to/from physical address pins
  bool A0_A,A1_A,A2_A,A3_A,A4_A,A5_A,A6_A,A7_A,A8_A,A9_A,A10_A,A11_A,A12_A,A13_A,A14_A,A15_A;
  bool A0_B,A1_B,A2_B,A3_B,A4_B,A5_B,A6_B,A7_B,A8_B,A9_B,A10_B,A11_B,A12_B,A13_B,A14_B,A15_B;
  bool A0_D,A1_D,A2_D,A3_D,A4_D,A5_D,A6_D,A7_D,A8_D,A9_D,A10_D,A11_D,A12_D,A13_D,A14_D,A15_D;

  // signals to/from physical data pins
  bool D0_A,D1_A,D2_A,D3_A,D4_A,D5_A,D6_A,D7_A;
  bool D0_B,D1_B,D2_B,D3_B,D4_B,D5_B,D6_B,D7_B; // these are all driven by LULA
  bool D0_C,D1_C,D2_C,D3_C,D4_C,D5_C,D6_C,D7_C;
  bool D0_D,D1_D,D2_D,D3_D,D4_D,D5_D,D6_D,D7_D;

  // signals to/from vram address pins
  bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;

  // signals to/from vram data pins
  bool MD0_A,MD1_A,MD2_A,MD3_A,MD4_A,MD5_A,MD6_A,MD7_A;
  bool MD0_B,MD1_B,MD2_B,MD3_B,MD4_B,MD5_B,MD6_B,MD7_B; // <- P25.ROFA
  bool MD0_C,MD1_C,MD2_C,MD3_C,MD4_C,MD5_C,MD6_C,MD7_C;
  bool MD0_D,MD1_D,MD2_D,MD3_D,MD4_D,MD5_D,MD6_D,MD7_D;

  bool MCS_A; // <- P25.SOKY
  bool MCS_C; // -> P25.TEFY
  bool MCS_D; // <- P25.SETY

  bool MOE_A; // <- P25.REFO
  bool MOE_C; // -> P25.TAVY
  bool MOE_D; // <- P25.SAHA

  bool RST;

  bool SOUT_A;

  bool SIN_A;
  bool SIN_B;
  bool SIN_C;
  bool SIN_D;
  
  bool SCK_A; // <- P06.KEXU, out high
  bool SCK_B; // <- P06.CULY, out dir
  bool SCK_C; // -> P06.CAVE, in
  bool SCK_D; // <- P06.KUJO, out low

  bool P10_A,P10_B,P10_C,P10_D;
  bool P11_A,P11_B,P11_C,P11_D;
  bool P12_A,P12_B,P12_C,P12_D;
  bool P13_A,P13_B,P13_C,P13_D;
  bool P14_A,P14_B,P14_D; // not sure about P14_B, i think it's mislabeled
  bool P15_A,P15_D;
};