close all;
clear all;
clear home;
clc;

s=tf('s');

R = 0.178;      %The motor's internal resistance
L = 119.26e-6;  %The motor's inductance
ke = 1.89e-3;   %the electromotive constant
kt = 0.0016;   %The motor constant
rt = 0.166/(2*pi);
N = 187/4773;
J = 5.5980e-06;
B = 4.7088e-6;

K = kt / (R * B + kt * ke )

tau = (R * J )/ (R * B + kt * ke )

num1 = K;
den1 = tau * s + 1;

H1 = num1/den1

Kp = 1/(K*N*rt)
Ki = Kp/tau
%proportional controller
H2 = Kp * H1

%proportional controller with feed forward:
H3 = (1+Kp)*H1

%proportional integral controller (PI)
H4 = (Kp+(Ki/s))*H1

bode(H1, 'b')
hold on;
bode(H2, '--r')
bode(H3, '--g')
bode(H4, '--m')

%adding legend
legend('1st Order Model', 'With Proportional Controller', 'Width Feed Forward', 'Integral', 'Location', 'southwest' )

grid on
set(gca,'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)