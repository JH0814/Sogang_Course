
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/28 23:51:00
// Design Name: 
// Module Name: seven_display_tb
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
module seven_display_tb;
reg ww, xx, yy, zz;
wire aa, bb, cc, dd, ee, ff, gg, ddpp;
seven_display u_test(
.w (ww),
.x (xx),
.y (yy),
.z (zz),

.A (aa),
.B (bb),
.C (cc),
.D (dd),
.E (ee),
.F (ff),
.G (gg),
.Dp (ddpp)
);

initial begin
ww = 1'b0;
xx = 1'b0;
yy = 1'b0;
zz = 1'b0;
end

always@(ww or xx or yy or zz) begin
ww <= #50 ~ww;
xx <= #100 ~xx;
yy <= #200 ~yy;
zz <= #400 ~zz;
end

initial begin
#1000
$finish;
end
endmodule
