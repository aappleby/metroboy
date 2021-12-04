typedef logic bool;
typedef logic [7:0]  uint8_t;
typedef logic [15:0] uint16_t;
typedef logic [31:0] uint32_t;
typedef logic [15:0] sample_t;
typedef logic signed [7:0] int8_t;

parameter bool true=1;
parameter bool false=0;

`define bit(a,b) a[b]

/* verilator lint_off VARHIDDEN */
/* verilator lint_off IMPORTSTAR */
/* verilator lint_off UNUSED */

/*
function int sum(int a, int b);
  sum = a+b;  
endfunction
*/