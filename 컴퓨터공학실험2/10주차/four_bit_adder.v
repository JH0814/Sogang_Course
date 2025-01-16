
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/10 17:42:11
// Design Name: 
// Module Name: four_bit_adder
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
module four_bit_adder(
input [3:0] a,b,
input cin,
output [3:0] s, c
);

    assign s[0] = a[0] ^ b[0] ^ cin;
    assign c[0] = (cin & (a[0] ^ b[0])) | (a[0] & b[0]);
    
    assign s[1] = a[1] ^ b[1] ^ c[0];
    assign c[1] = (c[0] & (a[1] ^ b[1])) | (a[1] & b[1]);
    
    assign s[2] = a[2] ^ b[2] ^ c[1];
    assign c[2] = (c[1] & (a[2] ^ b[2])) | (a[2] & b[2]);
    
    assign s[3] = a[3] ^ b[3] ^ c[2];
    assign c[3] = (c[2] & (a[3] ^ b[3])) | (a[3] & b[3]);
endmodule
