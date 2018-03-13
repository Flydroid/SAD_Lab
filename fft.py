import csv
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


#pylab inline --no-import-all


with open("log1.txt") as f:
    data = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
data = [x.strip() for x in data] 
data = [int(x) for x in data]

data = [x*(5/1024)-0.1 for x in data ]

#print(data)
s_rate = 200
samples = 300
t_end = samples/s_rate

time = np.linspace(0,t_end,samples,endpoint=True)

plt.figure(1)
plt.plot(time,data)
plt.xlabel('Time ($s$)')
plt.ylabel('Amplitude ($V$)')

V_fft = np.fft.fft(data)

#half 
N = int(len(V_fft/2+1))


plt.figure(2)
freq= np.linspace(0, s_rate/2,N)
plt.plot(freq, 2.0*np.abs(V_fft[:N]/N))
plt.xlabel('Frequency ($Hz$)')
plt.ylabel('Amplitude ($V$)')


V_hann = np.hanning(samples)
plt.figure(3)
plt.plot(time,V_hann*data)
plt.xlabel('Time ($s$)')
plt.ylabel('Amplitude ($V$)')


V_hann_fft = np.fft.fft(V_hann*data)
plt.figure(4)
plt.plot(freq, V_hann_fft[:N])
plt.xlabel('Frequency ($Hz$)')
plt.ylabel('Amplitude ($V$)')



plt.show()
