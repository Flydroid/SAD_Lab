function [y,t] = SineGen()

Amp=8;
ts=3.125;
%T=0.8;
t=0:0.003125:1;
y(:,1)=round(Amp*sin(2*pi*t)+Amp);

end

