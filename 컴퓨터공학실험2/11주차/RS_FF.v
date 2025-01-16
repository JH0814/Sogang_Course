
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/18 11:16:33
// Design Name: 
// Module Name: RS_FF
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
module RS_FF(
input clk, R, S,
output Q1, Q2
    );
    assign Q1 = ~(~S | ~clk) | (~Q2);
    assign Q2 = ~(~R | ~clk) | (~Q1);
endmodule
