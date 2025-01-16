
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/28 23:23:41
// Design Name: 
// Module Name: seven_display
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
module seven_display(
input w, x, y, z,
output A, B, C, D, E, F, G, Dp, seg
    );
assign A = (~w&y)|(x&y)|(y&~z)|(~x&~y&~z)|(w&~y&~z)|(w&~x&~y)|(~w&x&z);
assign B = (~w&~x)|(~w&~y&~z)|(~w&y&z)|(w&~y&z)|(w&~x&~z);
assign C = (~y&z)|(~w&x)|(w&~x)|(~w&~y)|(~w&z);
assign D = (x&y&~z)|(x&~y&z)|(w&~y&~z)|(~x&y&z)|(~w&~x&~z);
assign E = (w&x)|(y&~z)|(w&y)|(~x&~z);
assign F = (w&y)|(~y&~z)|(w&~x)|(x&~z)|(~w&x&~y);
assign G = (y&~z)|(w&z)|(w&~x)|(~w&x&~y)|(~w&~x&y);
assign Dp = 1;
assign seg = 1;
endmodule
