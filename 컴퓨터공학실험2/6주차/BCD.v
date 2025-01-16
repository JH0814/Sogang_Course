
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/07 22:20:04
// Design Name: 
// Module Name: BCD
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
module BCD(
input p, q, r, s,
output a, b, c, d
    );
    assign a = ~(~p&~(q&r)&~(q&s));
    assign b = ~(~p&~(q&~r&~s)&~(q&r));
    assign c = ~(~p&~(~q&r) & ~(q&~r&s));
    assign d = s;
endmodule
