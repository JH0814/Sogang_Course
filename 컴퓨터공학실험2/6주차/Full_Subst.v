
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/07 22:06:03
// Design Name: 
// Module Name: Full_Subst
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
module Full_Subst(
input a, b, c,
output s, cout
    );
    assign s = (a^b)^c;
    assign cout = ((~(a ^ b)) & c) | ((~a) & b);
endmodule
