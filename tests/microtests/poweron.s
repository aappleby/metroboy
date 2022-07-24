.include "header.inc"


// NR10 FF10 0b10000000 -PPP NSSS Sweep period, negate, shift
// NR11 FF11 0b10111111 DDLL LLLL Duty, Length load (64-L)
// NR12 FF12 0b11110011 VVVV APPP Starting volume, Envelope add mode, period
// NR13 FF13 0b11111111 FFFF FFFF Frequency LSB
// NR14 FF14 0b10111111 TL-- -FFF Trigger, Length enable, Frequency MSB

//      FF15 0b11111111 ---- ---- Not used
// NR21 FF16 0b11000000 DDLL LLLL Duty, Length load (64-L)
// NR22 FF17 0b00000000 VVVV APPP Starting volume, Envelope add mode, period
// NR23 FF18 0b11111111 FFFF FFFF Frequency LSB
// NR24 FF19 0b10111111 TL-- -FFF Trigger, Length enable, Frequency MSB


// NR30 FF1A 0b01111111 E--- ---- DAC power
// NR31 FF1B 0b11111111 LLLL LLLL Length load (256-L)
// NR32 FF1C 0b10011111 -VV- ---- Volume code (00=0%, 01=100%, 10=50%, 11=25%)
// NR33 FF1D 0b11111111 FFFF FFFF Frequency LSB
// NR34 FF1E 0b10111111 TL-- -FFF Trigger, Length enable, Frequency MSB

//        Noise
//      FF1F 0b11111111 ---- ---- Not used
// NR41 FF20 0b11111111 --LL LLLL Length load (64-L)
// NR42 FF21 0b00000000 VVVV APPP Starting volume, Envelope add mode, period
// NR43 FF22 0b00000000 SSSS WDDD Clock shift, Width mode of LFSR, Divisor code
// NR44 FF23 0b10111111 TL-- ---- Trigger, Length enable

//        Control/Status
// NR50 FF24 0b01110111 ALLL BRRR Vin L enable, Left vol, Vin R enable, Right vol
// NR51 FF25 0b11110011 NW21 NW21 Left enables, Right enables
// NR52 FF26 0b11110001 P--- NW21 Power control/status, Channel length statuses

/*
static const char read_mask[GB_IO_WAV_END - GB_IO_NR10 + 1] = {
        0x80, 0x3F, 0x00, 0xFF, 0xBF, // NR1X
        0xFF, 0x3F, 0x00, 0xFF, 0xBF, // NR2X
        0x7F, 0xFF, 0x9F, 0xFF, 0xBF, // NR3X
        0xFF, 0xFF, 0x00, 0x00, 0xBF, // NR4X
        0x00, 0x00, 0x70, 0xFF, 0xFF, // NR5X
    };
*/

main:
  ld a, $00
  ld ($FF10), a
  ld a, ($FF10)
  test_display_a