
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/24 11:15:37
// Design Name: 
// Module Name: two_bit_Counter_tb
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
module two_bit_Counter_tb;
reg clk, x, reset;
wire [1:0] Q;
wire z;
two_bit_Counter u_test(
.clk (clk),
.x (x),
.reset (reset),
.Q (Q),
.z (z)
);
initial begin
clk = 1'b0;
    forever #10 clk = ~clk;
end
initial begin
x = 1'b0;
    forever #20 x = ~x;
end
initial reset = 1'b1;
always@(reset) begin
reset = #50 ~reset;
end
endmodule
