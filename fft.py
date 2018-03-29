import csv
import numpy as np
import matplotlib.pyplot as plt


#pylab inline --no-import-all


with open("V_1000_2.txt") as f:
    data = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
data = [x.strip() for x in data] 
data = [int(x) for x in data]

data = [x*(5/1024)-0.1 for x in data ]

#print(data)
s_rate = 1000
samples = len(data)
t_end = samples/s_rate

time = np.linspace(0,t_end,samples,endpoint=True)

plt.figure("raw data")
plt.plot(time,data[:samples])
plt.xlabel('Time ($s$)')
plt.ylabel('Amplitude ($V$)')

V_fft = np.fft.fft(data)

#half 
N = int(len(V_fft/2))


plt.figure("fft of raw data")
freq= np.linspace(0, s_rate/2,N)
plt.plot(freq, np.abs(V_fft[0:N]))
plt.xlabel('Frequency ($Hz$)')
plt.ylabel('Amplitude ($Unit$)')


V_hann = np.hanning(samples)
plt.figure("hanning")
plt.plot(time,V_hann*data[:samples])
plt.xlabel('Time ($s$)')
plt.ylabel('Amplitude ($V$)')




V_hann_fft = np.fft.fft(V_hann*data[:samples])
N = int(len(V_hann_fft/2+1))

freq= np.linspace(0, s_rate/2,N)

plt.figure("fft of hanning")
plt.plot(freq[:N], 2.0*np.abs(V_hann_fft[:N])/N)
plt.xlabel('Frequency ($Hz$)')
plt.ylabel('Amplitude ($Unit$)')



plt.show()
