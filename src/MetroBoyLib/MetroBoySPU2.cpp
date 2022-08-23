  //----------------------------------------

  /*
  void tock_ack(uint16_t addr) {
    ack_data = 0;
    ack_valid = 0;

    if (addr >= 0xFF30 && addr <= 0xFF3F) {
      // wavetable
      ack_data = s3_wave[addr & 0xF];
    }
    else if (addr >= 0xFF10 && addr <= 0xFF26) {
      switch (addr) {
        case 0xFF10: ack_data = nr10 | 0x80; break;
        case 0xFF11: ack_data = nr11 | 0x3F; break;
        case 0xFF12: ack_data = nr12 | 0x00; break;
        case 0xFF13: ack_data = nr13 | 0xFF; break;
        case 0xFF14: ack_data = nr14 | 0xBF; break;

        case 0xFF15: ack_data = nr20 | 0xFF; break;
        case 0xFF16: ack_data = nr21 | 0x3F; break;
        case 0xFF17: ack_data = nr22 | 0x00; break;
        case 0xFF18: ack_data = nr23 | 0xFF; break;
        case 0xFF19: ack_data = nr24 | 0xBF; break;

        case 0xFF1A: ack_data = nr30 | 0x7F; break;
        case 0xFF1B: ack_data = nr31 | 0xFF; break;
        case 0xFF1C: ack_data = nr32 | 0x9F; break;
        case 0xFF1D: ack_data = nr33 | 0xFF; break;
        case 0xFF1E: ack_data = nr34 | 0xBF; break;

        case 0xFF1F: ack_data = nr40 | 0xFF; break;
        case 0xFF20: ack_data = nr41 | 0xFF; break;
        case 0xFF21: ack_data = nr42 | 0x00; break;
        case 0xFF22: ack_data = nr43 | 0x00; break;
        case 0xFF23: ack_data = nr44 | 0xBF; break;

        case 0xFF24: ack_data = nr50 | 0x00; break;
        case 0xFF25: ack_data = nr51 | 0x00; break;

        case 0xFF26: {
          uint8_t bus_out_ = (nr52 & 0x80) | 0x70;
          if (s1_running) bus_out_ |= 0b00000001;
          if (s2_running) bus_out_ |= 0b00000010;
          if (s3_running) bus_out_ |= 0b00000100;
          if (s4_running) bus_out_ |= 0b00001000;
          ack_data = bus_out_;
          break;
        }
      }
    }
  }
  */
