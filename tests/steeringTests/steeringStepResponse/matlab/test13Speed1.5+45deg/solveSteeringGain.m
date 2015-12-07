clear all;
close all;
clc;

% syms K
% 
% Kp = 1.8;
% ThetaRef = 0;

% Read the data from the test
Data1 = csvread('gain1Test1.csv');
Data2 = csvread('gain1Test2.csv');
Data3 = csvread('gain1Test3.csv');
Data4 = csvread('gain1.5Test1.csv');
Data5 = csvread('gain1.5Test2.csv');
Data6 = csvread('gain1.5Test3.csv');


t1     = Data1(:,1);
Theta1 = Data1(:,2)*(-1);
t2     = Data2(:,1);
Theta2 = Data2(:,2)*(-1);
t3     = Data3(:,1);
Theta3 = Data3(:,2)*(-1);
t4     = Data4(:,1);
Theta4 = Data4(:,2)*(-1);
t5     = Data5(:,1);
Theta5 = Data5(:,2)*(-1);
t6     = Data6(:,1);
Theta6 = Data6(:,2)*(-1);
%ThetaError = Data1(:,4);

% Plots
hold on
P1 = scatter(t1,Theta1);
P1 = scatter(t2,Theta2);
P1 = scatter(t3,Theta3);
P1 = scatter(t4,Theta4);
P1 = scatter(t5,Theta5);
P1 = scatter(t6,Theta6);
%P2 = scatter(t,ThetaError);
% ThetaCalc = ThetaRef*Kp*K*exp(-Kp*K*t)

% Make the plots beautiful
% set( P1, 'color', '[0 0 1]', 'LineWidth', 2 );
title('Step response of steering')
xlabel('Time [ms]')
ylabel('Angle [°]')
% xlim([3 7]);
legend('Theta at 1,1 m\cdot s^{-1}', 'Theta at 1,2 m\cdot s^{-1}', 'Theta at 1,3 m\cdot s^{-1}', 'Theta at 1,4 m\cdot s^{-1}', 'Theta at 1,7 m\cdot s^{-1}','Theta at 1,8 m\cdot s^{-1}')
grid on
%set(gca,'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)

%% Gain calculations
figure
syms K

Kp = 1.5;
ThetaRef = 0;

% Read the data from the test
dat = dlmread('gainCalc.tsv','\t');


% Plots
hold on
P1 = scatter(dat(:,2),dat(:,5))
% P1 = scatter(ThetaError,Theta);
% ThetaCalc = ThetaRef*Kp*K*exp(-Kp*K*t)

P = polyfit(dat(:,2),dat(:,5),1)
x = [0:0.01:2.5];
y = 0.4880 * x - 0.2173;

plot(x,y);


% Make the plots beautiful
% set( P1, 'color', '[0 0 1]', 'LineWidth', 2 );
title('Steering gain')
ylabel('Steering gain')
xlabel('Vehicle velocity')
% xlim([3 7]);
% legend('Theta error', 'Theta','Velocity of X m \cdot s^{-1}')
grid on
%set(gca,'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)