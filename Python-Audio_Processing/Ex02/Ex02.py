#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
import librosa as librosa
import librosa.display
from scipy import signal
from scipy.fftpack import fft


# Overlapping factor of each frame to previous frame
overlapping_factor = 0.5

def segmentFFT(Fs, audio, audio_index, period):
    global overlapping_factor
    NumOfSample = len(audio)
    # Create rounded period to calculate sample easier
    rounded_period = round(Fs*period)/Fs
    
    # Define length of each segment (number of samples in a segment)
    Frame_length = int(rounded_period*Fs)
    
    # Define N-point for FFT calculation
    FFT_point = Frame_length
    
    # Define 2 array to contains calculated data of all segments in the audio
    Audio_abs = np.zeros((1 , int(FFT_point/2) ))
    Audio_dB = np.zeros((1 , int(FFT_point/2) ))
    
    # Loop through the audio, each iteration last 1 "Frame_length"
    i=0
    end_sample=0
    while (end_sample < NumOfSample-(1-overlapping_factor)*Frame_length):
        # Define start and end samples of the segment
        start_sample = max((i*(1-overlapping_factor)*rounded_period),0)*Fs
        end_sample = int(start_sample) + Frame_length
        
        # Define start and end time according to start and end samples
        start_time = start_sample/Fs
        end_time = end_sample/Fs
        # Define a time step to plot audio segment
        # Extract a segment from audio
        step_time =  np.linspace(start_time,end_time,Frame_length)
        step_amp = audio[int(start_sample):int(end_sample)]
        
        # Windowing the extracted audio segment
        windowed_amp = np.multiply(signal.hamming(Frame_length, sym=False), (step_amp))
        
        # Apply FFT to the windowed signal and calculate magnitude
        Segment_FFT = fft(windowed_amp,FFT_point)
        Segment_abs = (2/FFT_point*np.abs(Segment_FFT[0: int(FFT_point/2)]))**2 
        
        # Append calculated segment into 2 arrays which later contain the whole
        # audio signal
        Audio_abs = np.vstack( [Audio_abs,Segment_abs])
        Audio_dB = np.vstack( [Audio_dB,np.log10(Segment_abs)])
        
        # Increment iterator to the next frame
        i+=1
        if (i == 30):
            
            # Plot the segment
            plt.figure()  
            plt.plot(step_time,windowed_amp)
            plt.title('A segment of audio' + audio_index)
            plt.xlabel('Time (s)')
            plt.ylabel('audio' + audio_index)
            plt.show()
        
    # plot spectrogram of audio with flipud() in order that
    # it can be easier to read and compare.
    plt.figure()     
    plt.imshow((np.flipud(np.swapaxes(Audio_abs,0,1))),
               extent=[0,NumOfSample/Fs,0,Fs/2],aspect='auto')
    plt.title('Spectrogram (magnitude) of audio' + audio_index)
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
    
    # plot spectrogram of audio (in logarithm) with flipud() in order that
    # it can be easier to read and compare.
    plt.figure()     
    plt.imshow(np.flipud(np.swapaxes(Audio_dB,0,1)),
               extent=[0,NumOfSample/Fs,0,Fs/2],aspect='auto')
    plt.title('Spectrogram (dB) of audio' + audio_index)
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
 



def spectrogramStft(Fs, audio, audio_index,period):
    
    # Define length of each segment (number of samples in a segment)
    # librosa Fs is 22050
    Frame_length = int(period*22050)
    # Calculate Fourier Transform using ready-made stft
    Audio_Spectrogram =np.abs(librosa.stft(audio,Frame_length ))
    
    # plot spectrogram of audio
    plt.figure()
    fig, ax = plt.subplots() 
    img = librosa.display.specshow((Audio_Spectrogram),sr = Fs ,
                                   y_axis='linear', x_axis='time', ax=ax)
   
    fig.colorbar(img, ax=ax, format="%+2.0f dB")
    ax.set_title('stft (magnitude) of audio' + audio_index)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Frequency (Hz)')
    
    # plot spectrogram of audio in logarithm scale
    plt.figure()
    fig, ax = plt.subplots() 
    img = librosa.display.specshow(librosa.amplitude_to_db(Audio_Spectrogram,ref=np.max),
                                   sr = Fs ,y_axis='linear', x_axis='time', ax=ax)
    fig.colorbar(img, ax=ax, format="%+2.0f dB")
    ax.set_title('stft (dB) of audio' + audio_index)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Frequency (Hz)')
    
def generateSinusoid(Fsx):
    
    SimulationTime = 3
    NumOfSample = int(SimulationTime*Fsx)
    
    # Define 4 sine signal with given frequency
    # and different chosen amplitudes and phases
    t = np.linspace(0, SimulationTime,NumOfSample)
    x = [ 3*np.sin(2 * np.pi * 100 * t +1), 
          1.5*np.sin(2 * np.pi * 500 * t +2),
          np.sin(2 * np.pi * 1500 * t +3),
          2*np.sin(2 * np.pi * 2500 * t +4)]
    # Calculate sum of 4 signals
    x_sum = 0
    for i in range(0,len(x)):
        x_sum += x[i]
    return x_sum

def main():
    
    # Load audio1.wav with sample rate and signal
    [audio1, Fs1] = librosa.load('audio1.wav',sr=None)
    segmentFFT(Fs1, audio1,'1',0.016)
    segmentFFT(Fs1, audio1,'1',0.064)
    segmentFFT(Fs1, audio1,'1',0.128)
    spectrogramStft(Fs1, audio1,'1',0.128)
    
    [audio2, Fs2] = librosa.load('audio2.wav',sr=None)
    segmentFFT(Fs2, audio2,'2',0.1)
    spectrogramStft(Fs2, audio2,'2',0.1)
    
    Fsx = 20000
    x = generateSinusoid(Fsx)
    
    segmentFFT(Fsx, x,'_x',0.1)
    spectrogramStft(Fsx, x,'_x',0.1)
    
    plt.show()
    
    
    
    
if __name__== "__main__":
    main()
