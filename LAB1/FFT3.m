function [P,f] = FFT3(Sig)
L=size(Sig);
H = hann(L(1));
%Y = fft(Sig);
Y = fft(Sig.*H);
P2 = abs(Y/L(1));
P1 = P2(1:L(1)/2+1);
P1(2:end-1) = 2*P1(2:end-1);
Fs=1000;
f = Fs*(0:(L/2))/L(1);
P(:,1) = mag2db(P1);
end
