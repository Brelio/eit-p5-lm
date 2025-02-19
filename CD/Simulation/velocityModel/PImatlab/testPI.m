p = get(gcf, 'Position')           % These two lines makes the figuresappear the same
set(0, 'DefaultFigurePosition', p) % place on the screen as where you left them last
clear all
close all;
clc;

%csvwrite('simulationPImultiStep.csv', ScopeData6);
simData = csvread('simulationPI.csv', 0, 0);
simData(:,1) = simData(:,1)-2

data = csvread('LOG00217.csv');%, 0, 0); 18 to 22    use step 16
data(:,1) = (data(:,1)-4035)/1000;

plot( simData(:,1), simData(:,2), 'r', 'LineWidth', 2);
hold on;
plot( data(:,1), data(:,2), 'b', 'LineWidth', 2);
scatter( data(:,1), data(:,2), 20, [0 .5 0], 'o' );

%Title and axis labels added
title('PI-Controller Simulation and Implementation Compared')
xlabel('Time [s]')
ylabel('Linear Velocity [ m \cdot s^{-1} ]')

legend('Simulation data','Test data', 'Test data resolution', 'Location', 'southeast')

xlim([ 0 5 ]);

grid on
set(gca,'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)