#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.fftpack import fft

def main():
    # - Define sampling parameters
    # - Highest frequency in this exercise is 2500 so 
    # Fs must be at least 5000 (Nyquist Theorem), I chose 10000 
    # to preserve waveform 
    Fs = 10000
    SimulationTime = 3
    NumOfSample = int(SimulationTime*Fs)
    
    # Define 4 sine signal with given frequency
    # and different chosen amplitudes and phases
    t = np.linspace(0, SimulationTime,NumOfSample)
    x = [ 3*np.sin(2 * np.pi * 100 * t +1), 
          1.5*np.sin(2 * np.pi * 500 * t +2),
          np.sin(2 * np.pi * 1500 * t +3),
          2*np.sin(2 * np.pi * 2500 * t +4)]
    
   
    x_sum = 0
    for i in range(0,len(x)):
        x_sum += x[i]
        
    wavfile.write('Sine.wav', Fs, x_sum)
    
    # Define DFT-point 
    N_point = 512
    
    # Fast Fourier Transform summed signal to get output in frequency field 
    # fftshift to plot both negative and positive part
    ff = np.linspace(0, int(Fs), N_point)
    xf = fft(x_sum,N_point)
    
    plot_range = int(Fs/300)     
    plt.figure(1)
    for i in range(0,len(x)):
        plt.plot(t[0:plot_range],x[i][0:plot_range], label = 'x_' + str(i) + '(t)')
    plt.legend()
    plt.figure(2)
    plt.plot(t[0:plot_range],x_sum[0:plot_range])
    
    
    plt.figure(3)    
    plt.plot(ff[0: int(N_point/3)], 2/N_point * np.abs(xf)[0: int(N_point/3)], label ='X_f')
    plt.title('DFT - Spectral density of x_sum(t)')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.legend()
    plt.show()
    
if __name__== "__main__":
    main()
