
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 10:36:34
// Design Name: 
// Module Name: Shift_regi_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`timescale 1ns / 1ps
module Shift_regi_tb;
reg clk, rst, in;
wire [3:0] out;
Shift_regi u_test(
.clk (clk),
.rst (rst),
.in (in),
.out (out)
);
initial begin
clk = 1'b0;
rst = 1'b0;
in = 1'b1;
end
always clk = #10 ~clk;
always in = #50 ~in;
initial rst <= #400 ~rst;
endmodule
