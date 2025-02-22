clear all;
close all;
clc;

syms K

Kp = 1.8;
ThetaRef = 0;

% Read the data from the test
Data1 = csvread('stepresponse1.1-45.csv');
Data2 = csvread('stepresponse1.2-45.csv');
Data3 = csvread('stepresponse1.3-45.csv');
Data4 = csvread('stepresponse1.4-45.csv');
Data5 = csvread('stepresponse1.7-45.csv');
Data6 = csvread('stepresponse1.8-45.csv');


t1     = Data1(:,2)/1000;
Theta1 = Data1(:,3)*(-1);
t2     = Data2(:,2)/1000;
Theta2 = Data2(:,3)*(-1);
t3     = Data3(:,2)/1000;
Theta3 = Data3(:,3)*(-1);
t4     = Data4(:,2)/1000;
Theta4 = Data4(:,3)*(-1);
t5     = Data5(:,2)/1000;
Theta5 = Data5(:,3)*(-1);
t6     = Data6(:,2)/1000;
Theta6 = Data6(:,3)*(-1);
%ThetaError = Data1(:,4);

% Plots
hold on
P1 = plot(t1,Theta1);
P1 = plot(t2,Theta2);
P1 = plot(t3,Theta3);
P1 = plot(t4,Theta4);
P1 = plot(t5,Theta5);
P1 = plot(t6,Theta6);
%P2 = scatter(t,ThetaError);
% ThetaCalc = ThetaRef*Kp*K*exp(-Kp*K*t)

% Make the plots beautiful
set( P1, 'color', '[0 0 1]', 'LineWidth', 2 );
title('Step response of steering')
xlabel('Time [s]', 'fontsize', 12)
ylabel('Angle [°]', 'fontsize', 12)
xlim([2 10]);
legend('Theta at 1,1 m\cdot s^{-1}', 'Theta at 1,2 m\cdot s^{-1}', 'Theta at 1,3 m\cdot s^{-1}', 'Theta at 1,4 m\cdot s^{-1}', 'Theta at 1,7 m\cdot s^{-1}','Theta at 1,8 m\cdot s^{-1}', 'Location', 'southeast', 'fontsize', 12)

grid on
set(gca,'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)
