#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Vram {

  //----------
  // Internal VRAM bus

  bool MA00;
  bool MA01;
  bool MA02;
  bool MA03;
  bool MA04;
  bool MA05;
  bool MA06;
  bool MA07;
  bool MA08;
  bool MA09;
  bool MA10;
  bool MA11;
  bool MA12;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;

  //----------
  // VRAM control pins

  /* PIN_43 */ bool MCS_A;   // <- P25.SOKY
  /* PIN_43 */ bool MCS_C;   // -> P25.TEFY
  /* PIN_43 */ bool MCS_D;   // <- P25.SETY
                             
  /* PIN_45 */ bool MOE_A;   // <- P25.REFO
  /* PIN_45 */ bool MOE_C;   // -> P25.TAVY
  /* PIN_45 */ bool MOE_D;   // <- P25.SAHA
                             
  /* PIN_49 */ bool MWR_A;   // <- P25.SYSY
  /* PIN_49 */ bool MWR_C;   // -> P25.SUDO
  /* PIN_49 */ bool MWR_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ bool MA00_D;    // <- P04.ECAL
  /* PIN_35 */ bool MA01_D;    // <- P04.EGEZ
  /* PIN_36 */ bool MA02_D;    // <- P04.FUHE
  /* PIN_37 */ bool MA03_D;    // <- P04.FYZY
  /* PIN_38 */ bool MA04_D;    // <- P04.DAMU
  /* PIN_39 */ bool MA05_D;    // <- P04.DAVA
  /* PIN_40 */ bool MA06_D;    // <- P04.ETEG
  /* PIN_41 */ bool MA07_D;    // <- P04.EREW
  /* PIN_48 */ bool MA08_D;    // <- P04.EVAX
  /* PIN_47 */ bool MA09_D;    // <- P04.DUVE
  /* PIN_44 */ bool MA10_D;    // <- P04.ERAF
  /* PIN_46 */ bool MA11_D;    // <- P04.FUSY
  /* PIN_42 */ bool MA12_D;    // <- P04.EXYF

  //----------
  // VRAM data pins

  /* PIN_33 */ bool MD0_A;   // <- P25.REGE
  /* PIN_33 */ bool MD0_B;   // <- P25.ROFA
  /* PIN_25 */ bool MD0_C;
  /* PIN_33 */ bool MD0_D;

  /* PIN_31 */ bool MD1_A;   // <- P25.RYKY
  /* PIN_31 */ bool MD1_B;   // <- P25.ROFA
  /* PIN_26 */ bool MD1_C;
  /* PIN_31 */ bool MD1_D;   
                             
  /* PIN_30 */ bool MD2_A;   // <- P25.RAZO
  /* PIN_30 */ bool MD2_B;   // <- P25.ROFA
  /* PIN_27 */ bool MD2_C;
  /* PIN_30 */ bool MD2_D;   
                             
  /* PIN_29 */ bool MD3_A;   // <- P25.RADA
  /* PIN_29 */ bool MD3_B;   // <- P25.ROFA
  /* PIN_28 */ bool MD3_C;
  /* PIN_29 */ bool MD3_D;   
                             
  /* PIN_28 */ bool MD4_A;   // <- P25.RYRO
  /* PIN_28 */ bool MD4_B;   // <- P25.ROFA
  /* PIN_29 */ bool MD4_C;
  /* PIN_28 */ bool MD4_D;   

  /* PIN_27 */ bool MD5_A;   // <- P25.REVU
  /* PIN_27 */ bool MD5_B;   // <- P25.ROFA
  /* PIN_30 */ bool MD5_C;
  /* PIN_27 */ bool MD5_D;   
                            
  /* PIN_26 */ bool MD6_A;   // <- P25.REKU
  /* PIN_26 */ bool MD6_B;   // <- P25.ROFA
  /* PIN_31 */ bool MD6_C;
  /* PIN_26 */ bool MD6_D;   
                             
  /* PIN_25 */ bool MD7_A;   // <- P25.RYZE
  /* PIN_25 */ bool MD7_B;   // <- P25.ROFA
  /* PIN_33 */ bool MD7_C;
  /* PIN_25 */ bool MD7_D;   
};

//-----------------------------------------------------------------------------

};