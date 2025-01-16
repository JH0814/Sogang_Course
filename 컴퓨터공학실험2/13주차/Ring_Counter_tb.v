`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 10:51:11
// Design Name: 
// Module Name: Ring_Counter_tb
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


module Ring_Counter_tb;
reg clk, rst;
wire[3:0] out;
Ring_Counter u_test(
.clk (clk),
.rst (rst),
.out (out)
);
initial begin
clk = 1'b0;
rst = 1'b1;
end
always clk = #10 ~clk;
always@(rst) begin
rst = #50 ~rst;
end
endmodule
