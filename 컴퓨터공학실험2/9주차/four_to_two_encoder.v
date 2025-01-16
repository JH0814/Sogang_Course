
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 14:39:43
// Design Name: 
// Module Name: four_to_two_encoder
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
module four_to_two_encoder(
input A,B,C,D,
output E0, E1
    );
    assign E0 = A|B;
    assign E1 = A|C;
endmodule
