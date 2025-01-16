
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/09 00:49:39
// Design Name: 
// Module Name: Mealy_tb
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
module Mealy_tb;
reg clk, rst, in;
wire[3:0] out;
wire z;
Mealy u_test(
.clk (clk),
.rst (rst),
.in (in),
.out (out),
.z (z)
);
initial begin
clk = 1'b0;
rst = 1'b1;
end
always clk = #10 ~clk;
initial begin
#20
rst = 0;
in = 1;
#40
in = 0;
#20
in = 1;
#20;
in = 0;
#60
in = 1;
#60
$finish;
end
endmodule
