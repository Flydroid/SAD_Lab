close all;
Sine = Sine50*(5/1024);
S= size(Sine);
[Gen,t] = SineGen(50,S(1));
YMatrix = [Sine(1:S(1)), Gen(1:S(1))];
figure1 = figure;
axes1 = axes('Parent',figure1);
hold(axes1,'on');
plot1= plot(t(1:S(1)),YMatrix);
set(plot1(1),'DisplayName','Generated Signal');
set(plot1(2),'DisplayName','Acquired Signal');
ylabel('Signal [V]');
xlabel('Time [s]');
ylim(axes1,[0 3.1]);
box(axes1,'on');
set(axes1,'XGrid','on','YGrid','on');
legend1 = legend(axes1,'show');
%FFT
[FFT_GEN,f] = FFT3(Gen);
FFT_SIG = FFT3(Sine);
S= size(FFT_SIG);
YMatrix2 = [FFT_GEN(1:S(1)),FFT_SIG(1:S(1))];
figure2 = figure;
axes2 = axes('Parent',figure2);
plot2= plot(f(1:S(1)),YMatrix2);
set(plot2(1),'DisplayName','Generated Signal');
set(plot2(2),'DisplayName','Acquired Signal');
set(axes2,'XGrid','on','YGrid','on');
xlabel('Frequency [Hz]')
ylabel('Magnitude [db]')
box(axes2,'on');
legend1 = legend(axes2,'show');