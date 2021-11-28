/* verilator lint_off VARHIDDEN */
/* verilator lint_off IMPORTSTAR */
/* verilator lint_off UNUSED */

typedef logic bool;
typedef logic [7:0]  uint8_t;
typedef logic [15:0] uint16_t;
typedef logic [31:0] uint32_t;
typedef logic [15:0] sample_t;
typedef logic signed [7:0] int8_t;

parameter bool true=1;
parameter bool false=0;

//-----------------------------------------------------------------------------

module ParseTest(input bool clk, input bool rst, output bool d);

  uint8_t data;

  task reset;
    data <= 0;
  endtask;

  task tock();
    data <= data + 1;
    d <= data[7];
  endtask;

  always @(posedge clk) begin
    if (rst) reset();
    else tock();
  end

endmodule;

//-----------------------------------------------------------------------------
