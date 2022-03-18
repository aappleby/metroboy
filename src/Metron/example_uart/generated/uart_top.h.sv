
`include "metron_tools.h.sv"
`include "uart_hello.h.sv"
`include "uart_rx.h.sv"
`include "uart_tx.h.sv"

//==============================================================================

module uart_top
#(parameter int cycles_per_bit = 3)
(
  input logic clock,
  input logic i_rstn,
  output logic o_serial,
  output logic[7:0] o_data,
  output logic o_valid,
  output logic o_done,
  output logic[31:0] o_sum
);
  // Submodule output port bindings
  logic[7:0] hello_o_data;
  logic hello_o_req;
  logic hello_o_done;
  logic tx_o_serial;
  logic tx_o_cts;
  logic tx_o_idle;
  logic[7:0] rx_o_data;
  logic rx_o_valid;
  logic[31:0] rx_o_sum;

 /*public:*/
  /*verilator public_module*/

  // Our "hello world" transmitter.
  uart_hello hello(
    // Inputs
    .clock(clock),
    .i_rstn(i_rstn), 
    .i_cts(tx_o_cts), 
    .i_idle(tx_o_idle), 
    // Outputs
    .o_data(hello_o_data), 
    .o_req(hello_o_req), 
    .o_done(hello_o_done)
  );


  // Serializer
  uart_tx #(cycles_per_bit) tx(
    // Inputs
    .clock(clock),
    .i_rstn(i_rstn), 
    .i_data(hello_o_data), 
    .i_req(hello_o_req), 
    // Outputs
    .o_serial(tx_o_serial), 
    .o_cts(tx_o_cts), 
    .o_idle(tx_o_idle)
  );


  // Deserializer
  uart_rx #(cycles_per_bit) rx(
    // Inputs
    .clock(clock),
    .i_rstn(i_rstn), 
    .i_serial(tx_o_serial), 
    // Outputs
    .o_data(rx_o_data), 
    .o_valid(rx_o_valid), 
    .o_sum(rx_o_sum)
  );


  /*logic<1> o_serial;*/
  /*logic<8> o_data;*/
  /*logic<1> o_valid;*/
  /*logic<1> o_done;*/
  /*logic<32> o_sum;*/

  //----------------------------------------

  initial begin : init
    $write("uart_top.init()\n");
    /*hello.init();*/
  end

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    /*hello.tick(i_rstn, tx.o_cts, tx.o_idle);*/
    /*tx.tick(i_rstn, hello.o_data, hello.o_req);*/
    /*rx.tick(i_rstn, tx.o_serial);*/
  end

  //----------------------------------------

  always_comb begin : tock
    /*hello.tock();*/
    /*tx.tock();*/
    /*rx.tock();*/

    o_serial = tx_o_serial;
    o_data = rx_o_data;
    o_valid = rx_o_valid;
    o_done = hello_o_done && tx_o_idle;
    o_sum = rx_o_sum;
  end

endmodule

//==============================================================================

