`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/19 15:23:12
// Design Name: 
// Module Name: RS_FF_NOR
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


module RS_FF_NOR(
input clk, R, S,
output Q1, Q2
    );
    assign Q1 = ~(R&clk)&~(Q2);
    assign Q2 = ~(S&clk)&~(Q1);
endmodule
