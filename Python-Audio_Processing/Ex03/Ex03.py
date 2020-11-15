
import matplotlib.pyplot as plt
import numpy as np

from scipy.io import wavfile
from scipy.fftpack import fft

def main():
    Fs = 16000
    Carri_freq = 880 
    Mod_freq = 220
    Mod_index = 2
    Signal_time = 1
    time = np.linspace(0,1,Signal_time*Fs);
    Mod_sin = np.sin(2*np.pi*Mod_freq*time);
    FM = np.sin(2*np.pi*(Carri_freq + Mod_index*Mod_sin)*time)
    
    plt.figure()
    plt.plot(time[50*int(Fs/100):51*int(Fs/100)],FM[50*int(Fs/100):51*int(Fs/100)])
    
      # Write sum signal to file
    wavfile.write('Sine.wav', Fs, FM)
    
    FFT = fft(FM)
    time = np.linspace(0,Fs,Fs);
    FFT_amp = 2/len(FFT)*abs(FFT)
    plt.figure()
    plt.plot(time[int(0):int(Fs)],FFT_amp[int(0):int(Fs)])
if __name__== "__main__":
    main()