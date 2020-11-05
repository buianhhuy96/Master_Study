#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.fftpack import fft

# This number is used to count plot windows
figure_index = 0

# Define DFT-point 
N_point = 512

# Define segment period (in second)
period = 0.1

def DFTandPlot(Fs, audio, audio_index):
    global figure_index
    global N_point
    global period 
    NumOfSample = len(audio)
    # Loop from 1s to the end, each iteration last "period (s)"
    # NumOfSample/Fs1 return the time in seconds;
    #   subtract 1 as it is the first second.
    # Div by "period" to transform into multiple of "period"
    for i in range(0,int((NumOfSample/Fs - 1)/period)):
        # Define a time step which last period start from second 1 
        # Extract a segment from audio
        step_time =  np.linspace(1+(i*period),1+(i+1)*period,int(Fs*period))
        step_amp = audio[(int(i*period*Fs) +Fs): (int((i+1)*period*Fs) +Fs)]
        
        if (i < 1):
            # Plot the segment
            figure_index+=1
            plt.figure(figure_index)  
            plt.plot(step_time,step_amp)
            plt.title('A segment of audio' + audio_index)
            plt.xlabel('Time (s)')
            plt.ylabel('audio1')
            
            # Calculate FFT with the segment
            audio_f = np.linspace(0, int(Fs), N_point)
            AUDIOinF = fft(step_amp,N_point)
            # Plot FFT
            figure_index+=1
            plt.figure(figure_index)    
            plt.plot(audio_f[0: int(N_point/3)], 2/N_point * np.abs(AUDIOinF)[0: int(N_point/3)], 
                    label ='Magnitude audio')
            plt.title('DFT - Spectral density of audio' + audio_index +'(t)')
            plt.xlabel('Frequency (Hz)')
            plt.ylabel('Magnitude')
            plt.legend()

def main():
    global figure_index
   
    
    # Load audio1.wav with sample rate and signal
    [Fs1, audio1] = wavfile.read('audio1.wav')
    
    # Define time range from 0.5s to 1s
    # There are "Fs" samples in 1s, so for 0.5s, number of samples is Fs/2
    audio1_t = np.linspace(1/2, 1, int(Fs1/2))
    audio1_x = audio1[int(Fs1/2): int(Fs1)]
    
    # Plot the audio1 in range of 0.5s to 1s
    figure_index+=1
    plt.figure(figure_index)
    plt.plot(audio1_t,audio1_x)
    plt.title('audio1 from 0.5s to 1s')
    plt.xlabel('Time (s)')
    plt.ylabel('audio1')
    # Calculate DFT for each segment in audio1
    # and plot the 1st segment
    DFTandPlot(Fs1, audio1,'1')
    
    # Load audio2.wav with sample rate and signal
    [Fs2, audio2] = wavfile.read('audio2.wav')
    
    # Define time range from 0.5s to 1s
    # There are "Fs" samples in 1s, so for 0.5s, number of samples is Fs/2
    audio2_t = np.linspace(1/2, 1, int(Fs2/2))
    audio2_x = audio1[int(Fs2/2): int(Fs2)]
    
    # Plot the audio2 in range of 0.5s to 1s
    figure_index+=1
    plt.figure(figure_index)
    plt.plot(audio2_t,audio2_x)
    plt.title('audio2 from 0.5s to 1s')
    plt.xlabel('Time (s)')
    plt.ylabel('audio2')
    # Calculate DFT for each segment in audio2
    # and plot the 1st segment
    DFTandPlot(Fs2, audio2,'2')
    
    plt.show()
    
    
    
if __name__== "__main__":
    main()
