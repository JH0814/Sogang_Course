
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/14 11:09:24
// Design Name: 
// Module Name: Two_Bit_Cmp
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
module Two_Bit_Cmp(
input a, b, c, d,
output f1, f2, f3
    );
    assign f1 = (a & (~c)) | (b & (~c) & (~d)) | (a & b & (~d));
    assign f2 = (~(a ^ c)) & (~(b ^ d));
    assign f3 = (c & (~a)) | (d & (~a) & (~b)) | (c & d & (~b));
endmodule
