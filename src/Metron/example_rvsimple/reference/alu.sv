// RISC-V SiMPLE SV -- ALU module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module alu (
    input        [4:0]  alu_function,
    input signed [31:0] operand_a,
    input signed [31:0] operand_b,
    output logic [31:0] result,
    output              result_equal_zero
);

    `ifdef M_MODULE
        logic [63:0] signed_multiplication;
        logic [63:0] unsigned_multiplication;
        logic [63:0] signed_unsigned_multiplication;
    `endif
    
    assign result_equal_zero = (result == 32'b0);
    
    always_comb begin
        result = `ZERO;
        case (alu_function)
            `ALU_ADD:   result = operand_a +    operand_b;
            `ALU_SUB:   result = operand_a -    operand_b;
            `ALU_SLL:   result = operand_a <<   operand_b[4:0];
            `ALU_SRL:   result = operand_a >>   operand_b[4:0];
            `ALU_SRA:   result = operand_a >>>  operand_b[4:0];
            `ALU_SEQ:   result = {31'b0, operand_a == operand_b};
            `ALU_SLT:   result = {31'b0, operand_a < operand_b};
            `ALU_SLTU:  result = {31'b0, $unsigned(operand_a) < $unsigned(operand_b)};
            `ALU_XOR:   result = operand_a ^    operand_b;
            `ALU_OR:    result = operand_a |    operand_b;
            `ALU_AND:   result = operand_a &    operand_b;
    `ifdef M_MODULE
            `ALU_MUL:   result = signed_multiplication[31:0];
            `ALU_MULH:  result = signed_multiplication[63:32];
            `ALU_MULHSU:    result = signed_unsigned_multiplication[63:32];
            `ALU_MULHU: result = unsigned_multiplication[63:32];
            `ALU_DIV:
                if (operand_b == `ZERO)
                    result = 32'b1;
                else if ((operand_a == 32'h80000000) && (operand_b == 32'b1))
                    result = 32'h80000000;
                else
                    result = operand_a / operand_b;
            `ALU_DIVU:
                if (operand_b == `ZERO)
                    result = 32'b1;
                else
                    result = $unsigned(operand_a) / $unsigned(operand_b);
            `ALU_REM:
                if (operand_b == `ZERO)
                    result = operand_a;
                else if ((operand_a == 32'h80000000) && (operand_b == 32'b1))
                    result = `ZERO;
                else
                    result = operand_a % operand_b;
            `ALU_REMU:
                if (operand_b == `ZERO)
                    result = operand_a;
                else
                    result = $unsigned(operand_a) % $unsigned(operand_b);
    `endif
            default:
                result = `ZERO;
        endcase
    end
    
    `ifdef M_MODULE
        always_comb begin
            signed_multiplication   = operand_a * operand_b;
            unsigned_multiplication = $unsigned(operand_a) * $unsigned(operand_b);
            signed_unsigned_multiplication = $signed(operand_a) * $unsigned(operand_b);
        end
    `endif

endmodule

