#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.fftpack import fft
import librosa as lbs

import librosa.display
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
    figure_index+=1
    plt.figure(figure_index)
    fig, ax = plt.subplots() 
    Audio_Spectrogram =np.abs(lbs.stft(audio))
    img = librosa.display.specshow(librosa.amplitude_to_db(Audio_Spectrogram,ref=np.max),sr = Fs ,y_axis='linear', x_axis='time', ax=ax)
    fig.colorbar(img, ax=ax, format="%+2.0f dB")
    
def main():
    global figure_index
   
    
    # Load audio1.wav with sample rate and signal
    [audio1, Fs1] = lbs.load('audio1.wav',sr=None)
    
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
    [audio2, Fs2] = lbs.load('audio2.wav',sr=None)
    
    # Define time range from 0.5s to 1s
    # There are "Fs" samples in 1s, so for 0.5s, number of samples is Fs/2
    audio2_t = np.linspace(1/2, 1, int(Fs2/2))
    audio2_x = audio1[int(Fs2/2): int(Fs2)]
    
    # Plot the audio2 in range of 0.5s to 1s
    # figure_index+=1
    # plt.figure(figure_index)
    # plt.plot(audio2_t,audio2_x)
    # plt.title('audio2 from 0.5s to 1s')
    # plt.xlabel('Time (s)')
    # plt.ylabel('audio2')
    # # Calculate DFT for each segment in audio2
    # # and plot the 1st segment
    # DFTandPlot(Fs2, audio2,'2')
    
    # plt.show()
    
    
    
if __name__== "__main__":
    main()
