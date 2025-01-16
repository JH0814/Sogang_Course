
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/14 10:53:55
// Design Name: 
// Module Name: Parity_Check
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
module Parity_Check(
input a,b,c,d,p,
output pec
    );
    assign pec = a ^ b ^ c ^ d ^ p;
endmodule
