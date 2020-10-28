import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.fftpack import fft
#define omega(f) (2 * np.pi * f)

Fs = 8000
f100 = 100
f500 = 500
f1500 = 1500
f2500 = 2500
SimulationTime = 3
NumOfSample = int(SimulationTime*Fs);
x = np.linspace(0, SimulationTime,NumOfSample)
y1 = np.sin(2 * np.pi * f100 * x +1)
y2 = 2*np.sin(2 * np.pi * f500 * x +2)
y3 = 3*np.sin(2 * np.pi * f1500 * x +3)
y4 = 1.5*np.sin(2 * np.pi * f2500 * x +4)
y_sum=y1+y2+y3+y4
wavfile.write('Sine.wav', Fs, y_sum)

yf = np.fft.fftshift(fft(y_sum,512))
xf = np.linspace(int(-Fs/(2.0)), int(Fs/(2.0)), 512)#int(2*Fs ))
fig, axs = plt.subplots(2)

axs[0].plot(xf, 2/512 * np.abs(yf))#[int(NumOfSample/2-Fs):int(NumOfSample/2+Fs)]))
#axs[1].plot(xf, np.abs(yf)/NumOfSample)
#axs[1].xlabel('sample(n)')
#axs[1].ylabel('voltage(V)')
#axs[1].axis('tight')
plt.show()