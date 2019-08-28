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

/* verilator lint_off VARHIDDEN */
/* verilator lint_off IMPORTSTAR */

//-----------------------------------------------------------------------------

endpackage;

`endif // TYPES_V