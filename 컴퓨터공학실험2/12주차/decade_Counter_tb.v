
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/24 13:22:03
// Design Name: 
// Module Name: decade_Counter_tb
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
module decade_Counter_tb;
reg clk, x, reset;
wire [3:0] Q;
wire z;
decade_Counter u_test(
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
