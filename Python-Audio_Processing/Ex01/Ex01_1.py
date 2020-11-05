#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
from scipy.io import wavfile
from scipy.fftpack import fft


def main():
    # - Define sampling parameters
    # - Highest frequency in this exercise is 2500 so 
    # Fs must be at least 5000 (Nyquist Theorem), I chose 20000 
    # to preserve waveform 
    Fs = 20000
    SimulationTime = 3
    NumOfSample = int(SimulationTime*Fs)
    
    # Define 4 sine signal with given frequency
    # and different chosen amplitudes and phases
    t = np.linspace(0, SimulationTime,NumOfSample)
    x = [ 3*np.sin(2 * np.pi * 100 * t +1), 
          1.5*np.sin(2 * np.pi * 500 * t +2),
          np.sin(2 * np.pi * 1500 * t +3),
          2*np.sin(2 * np.pi * 2500 * t +4)]
    
    # Define plot range to have a better observation of waveform
    plot_range = int(Fs/300)     
    plt.figure(1)
    for i in range(0,len(x)):
        plt.plot(t[0:plot_range],x[i][0:plot_range], label = 'x_' + str(i) + '(t)')
    plt.title('x_n(t)')
    plt.xlabel('Time (s)')
    plt.ylabel('x')
    plt.legend()
    
    
    # Calculate sum of 4 signals
    x_sum = 0
    for i in range(0,len(x)):
        x_sum += x[i]
    
    # Plot sum signal
    plt.figure(2)
    plt.plot(t[0:plot_range],x_sum[0:plot_range])
    plt.title('x_sum(t)')
    plt.xlabel('Time (s)')
    plt.ylabel('x')
    
    # Write sum signal to file
    wavfile.write('Sine.wav', Fs, x_sum)
    
    
    # Define DFT-point 
    N_point = 512
    
    # Fast Fourier Transform summed signal to get Spectral Density of x_sum 
    # fftshift to plot both negative and positive part
    ff = np.linspace(0, int(Fs), N_point)
    xf = fft(x_sum,N_point)
    
    # Plot DFT in suitable range for better observation
    
    plt.figure(3)    
    plt.plot(ff[0: int(N_point/3)], 2/N_point * np.abs(xf)[0: int(N_point/3)], 
               label ='X_f')
    plt.title('DFT - Spectral density of x_sum(t)')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.legend()
    
    
    ################################
    # Resample x with 2 times less samples
    resampled_x_sum = signal.resample(x_sum,int(NumOfSample/2))
    
    # Fast Fourier Transform resampled signal to get Spectral Density  
    # fftshift to plot both negative and positive part
    # Because number of samples is reduced by 2 times, so is Fs
    ff = np.linspace(0, int(Fs/2), N_point)
    xf = fft(resampled_x_sum,N_point)
    
    # Plot DFT in suitable range for better observation
    
    plt.figure(4)    
    plt.plot(ff[0: int(N_point/3)], 2/N_point * np.abs(xf)[0: int(N_point/3)], 
               label ='X_f')
    plt.title('DFT - Spectral density of resampled x_sum(t)')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.legend()
    
    
    
    plt.show()
    
    
    
    
if __name__== "__main__":
    main()
