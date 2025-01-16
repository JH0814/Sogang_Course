
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/04 10:36:52
// Design Name: 
// Module Name: four_to_one_MUX
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
module four_to_one_MUX(
input a,b,A,B,C,D,
output X
    );
    assign X = (~a&~b&A)|(a&~b&B)|(~a&b&C)|(a&b&D);
endmodule
