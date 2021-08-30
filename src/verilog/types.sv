`ifndef TYPES_SV
`define TYPES_SV

package types;

//-----------------------------------------------------------------------------

typedef logic bool;
typedef logic [7:0]  uint8_t;
typedef logic [15:0] uint16_t;
typedef logic [31:0] uint32_t;
typedef logic [15:0] sample_t;

typedef logic signed [7:0] int8_t;

parameter bool true=1;
parameter bool false=0;

`define bit_set(a,b) a[b]

typedef struct packed {
  uint16_t addr;
  uint8_t data;
  bool read;
  bool write;
} CpuBus;

typedef struct packed {
  uint8_t data;
  bool oe;
} BusOut;

parameter uint8_t INT_VBLANK  = 8'b00000001;
parameter uint8_t INT_STAT    = 8'b00000010;
parameter uint8_t INT_TIMER   = 8'b00000100;
parameter uint8_t INT_SERIAL  = 8'b00001000;
parameter uint8_t INT_JOYPAD  = 8'b00010000;

/* verilator lint_off VARHIDDEN */
/* verilator lint_off IMPORTSTAR */

//-----------------------------------------------------------------------------

endpackage;

`endif // TYPES_V