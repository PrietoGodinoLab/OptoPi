clc
clear all

[boolean_value] = readvars('Drosophila performance 9600 bauds','Range','C:C');
[t] = readvars('Drosophila performance 9600 bauds','Range','B:B');


plot (t,boolean_value);
xlabel('t(ms)')
ylabel('Boolean value (1 = Red & 0 = No color')
ylim([0 1.2]);

%Sampling period
T = mean(diff(t));
smaples = length(t);
f = 1/(T*1e-3);




