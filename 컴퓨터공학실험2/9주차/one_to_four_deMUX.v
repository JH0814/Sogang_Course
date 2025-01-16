
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/04 11:05:36
// Design Name: 
// Module Name: one_to_four_deMUX
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
module one_to_four_deMUX(
input a, b, F,
output A,B,C,D
    );
    assign A = ~a&~b&F;
    assign B = a&~b&F;
    assign C = ~a&b&F;
    assign D = a&b&F;
endmodule
