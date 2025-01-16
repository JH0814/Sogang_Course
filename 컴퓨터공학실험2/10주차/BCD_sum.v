
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/10 23:11:57
// Design Name: 
// Module Name: BCD_sum
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
module BCD_sum(
input [3:0] a,b,
input cin,
output [3:0] s, c,
output [3:0] sum,
output cout
    );
    wire [3:0] C;
    assign s[0] = a[0] ^ b[0] ^ cin;
    assign c[0] = (cin & (a[0] ^ b[0])) | (a[0] & b[0]);
    
    assign s[1] = a[1] ^ b[1] ^ c[0];
    assign c[1] = (c[0] & (a[1] ^ b[1])) | (a[1] & b[1]);
    
    assign s[2] = a[2] ^ b[2] ^ c[1];
    assign c[2] = (c[1] & (a[2] ^ b[2])) | (a[2] & b[2]);
    
    assign s[3] = a[3] ^ b[3] ^ c[2];
    assign c[3] = (c[2] & (a[3] ^ b[3])) | (a[3] & b[3]);
    
    assign cout = c[3] | (s[2] & s[3]) | (s[1] & s[3]);
    
    assign sum[0] = s[0];
    assign C[0] = (0 & (s[0] ^ 0)) | (s[0] & 0);
    
    assign sum[1] = s[1] ^ cout ^ C[0];
    assign C[1] = (C[0] & (s[1] ^ cout)) | (s[1] & cout);
    
    assign sum[2] = s[2] ^ cout ^ C[1];
    assign C[2] = (C[1] & (s[2] ^ cout)) | (s[2] & cout);
    
    assign sum[3] = s[3] ^ 0 ^ C[2];
    assign C[3] = (C[2] & (s[3] ^ 0)) | (s[3] & 0);
endmodule
