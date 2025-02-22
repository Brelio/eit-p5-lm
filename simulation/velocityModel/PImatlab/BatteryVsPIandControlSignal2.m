p = get(gcf, 'Position')           % These two lines makes the figuresappear the same
set(0, 'DefaultFigurePosition', p) % place on the screen as where you left them last
clear all
close all;
clc;

%csvwrite('simulationPIcalculated.csv', ScopeData3);
%simData = csvread('simulationPIcalculated.csv', 0, 0);
%simData(:,1) = simData(:,1)-2

data = csvread('PIconstantDisturbance2.csv'); % step 294 for calculated PI controller
                                              % step 288 for tuned PI controller
                                % step 299 for P controller with feed forward
                                % step 316 (NO) and 322 (width) anti-windup
                                % moreData step 336 (NO) step 331 (width) anti-windup
% syms x;
% x = 1;
% syms i;
% i = 1001;
% while i > 0
%     if x == 2
%         data(i,:) = [];
%         x = 1;
%     else
%         x = 2;
%     end
%     i = i-1;
% end

%data = csvread('LOG00217.csv');%, 0, 0); 18 to 22    use step 16
data(:,1) = (data(:,1)-4000)/1000;
%data1 = csvread('LOG00224.csv');%, 0, 0);
%data1(:,1) = (data1(:,1)-4035)/1000;
%data2 = csvread('LOG00225.csv');%, 0, 0);
%data2(:,1) = (data2(:,1)-4035)/1000;
%data3 = csvread('LOG00227.csv');%, 0, 0);
%data3(:,1) = (data3(:,1)-4035)/1000;

%data(:,3) = data(:,3);%-5.5;

%[AX, vel, bat] = plotyy( simData(:,1), simData(:,2), data(:,1), data(:,3));
plot( data(:,1), data(:,2));
%set(AX,'NextPlot','add')
%scatter(AX(2), data(:,1), data(:,2), 'g', '.');
%scatter(AX(2), data(:,1), data(:,2), 'r', '.');
%scatter(AX(2), data(:,1), data(:,2)), 'b', '.';

%plot(AX(2), data(:,1), data(:,4), 'Color', [1 0 0], 'LineWidth', 2, 'LineStyle', ':');
%plot(AX(2), data(:,1), data(:,6), 'Color', [0 .5 0], 'LineWidth', 2, 'LineStyle', ':');
%plot(AX(2), data(:,1), data(:,5), 'Color', [.8 0 .8], 'LineWidth', 2, 'LineStyle', ':');
%pAdd = plot(AX(2), data(:,1), data(:,4) );
%set(pAdd, 'Color', [1 0 0], 'LineWidth', 2 );


%pAdd = plot(AX(1), data(:,1), data(:,5) );
%set(pAdd, 'Color', [0 .5 0], 'LineWidth', 2 );

xlim([0 10])
%set(vel, 'Color', [0 0 1], 'LineWidth', 2);
%set(AX(2),'xlim',[0 9], 'ylim', [0 8], 'ycolor', [.7 0 0]);
%set(bat, 'Color', [.3 0 0], 'LineWidth', 2);

hline = line([1 10], [1.4 1.4]);
%hline = line([1 10], [2 2]);
set(hline, 'Color', [.3 .3 .3], 'LineWidth', 1, 'LineStyle', '--');

hline = line([0 1], [0.21 0.21]);
%hline = line([1 10], [2 2]);
set(hline, 'Color', [.3 .3 .3], 'LineWidth', 1, 'LineStyle', '--');

legend('Measured step response', 'Input step','Location','southeast')
%legend('Measured step response', 'Input step to 2 m\cdot s^{-1}', 'Battery voltage', 'Controller output', 'Location', 'southeast')

hline = line([1 1], [0.21 1.4]);
%hline = line([1 1], [0 2]);
set(hline, 'Color', [.3 .3 .3], 'LineWidth', 1, 'LineStyle', '--');

%hline = line([1.3 1.3], [0 3]);
%set(hline, 'Color', [0 0 0], 'LineWidth', 1, 'LineStyle', '-');

%%%%%plot( data(:,1), data(:,3), 'k', 'LineWidth', 2);
%scatter( data(:,1), data(:,2), 100, [0 .3 0], '.' );


%plot( simData(:,1), simData(:,4), 'r', 'LineWidth', 2);
%plot( data1(:,1), data1(:,2), 'b', 'LineWidth', 2);
%scatter( data1(:,1), data1(:,2), 100, [0 .3 0], '.' );

%plot( simData(:,1), simData(:,6), 'r', 'LineWidth', 2);
%plot( data2(:,1), data2(:,2), 'b', 'LineWidth', 2);
%scatter( data2(:,1), data2(:,2), 100, [0 .3 0], '.' );

%plot( simData(:,1), simData(:,8), 'r', 'LineWidth', 2);
%plot( data3(:,1), data3(:,2), 'b', 'LineWidth', 2);
%scatter( data3(:,1), data3(:,2), 100, [0 .3 0], '.' );

%Title and axis labels added
title('PI-Controller at 1.4 m \cdot s^{-1} Uphill')
xlabel('Time [s]')
ylabel('Linear velocity [ m \cdot s^{-1} ]')
%ylabel(AX(2), 'Voltage [ V ]')
%xlim([ 0 3 ]);

%set(AX(1), 'Xgrid', 'on', 'Ygrid', 'on', 'ytick', [0 .5 1 1.5 2], 'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)
%set(AX(2), 'Xgrid', 'on', 'Ygrid', 'on', 'ytick', [3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8], 'GridLineStyle',':', 'GridColor', 'k', 'GridAlpha', .6)