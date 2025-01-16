
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 14:50:50
// Design Name: 
// Module Name: BCD_to_decimal
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
module BCD_to_decimal(
input A0, A1, A2, A3,
output D1, D2, D3, D4, D5, D6, D7, D8, D9
    );
    assign D1 = A0 & (~A1) & (~A2) & (~A3);
    assign D2 = (~A0) & A1 & (~A2) & (~A3);
    assign D3 = A0 & A1 & (~A2) & (~A3);
    assign D4 = (~A0) & (~A1) & A2 & (~A3);
    assign D5 = A0 & (~A1) & A2 & (~A3);
    assign D6 = (~A0) & A1 & A2 & (~A3);
    assign D7 = A0 & A1 & A2 & (~A3);
    assign D8 = (~A0) & (~A1) & (~A2) & A3;
    assign D9 = A0 & (~A1) & (~A2) & A3;
endmodule
