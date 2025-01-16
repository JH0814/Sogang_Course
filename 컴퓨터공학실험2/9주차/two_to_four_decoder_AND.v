`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 14:16:36
// Design Name: 
// Module Name: two_to_four_decoder_AND
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


module two_to_four_decoder_AND(
input A,B,
output D0,D1,D2,D3
    );
 assign D0 = (~A)&(~B);
 assign D1 = (~A)&B;
 assign D2 = A&(~B);
 assign D3 = A&B;
endmodule
