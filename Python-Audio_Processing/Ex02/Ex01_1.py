#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
import librosa as lbs
import librosa.display
from scipy import signal
from scipy.io import wavfile
from scipy.fftpack import fft

# This number is used to count plot windows
figure_index = 0

# Define DFT-point 
N_point = 1024

# Define segment period (in second)
period = 0.1

# Time of signal at the beginning of calculation (at second)
start_calculation_time = 0.0

# Overlapping factor of each frame to previous frame
overlapping_factor = 0.0

def DFTandPlot(Fs, audio, audio_index):
    global figure_index
    global N_point
    global period 
    global overlapping_factor
    NumOfSample = len(audio)
    round_sample_period = round(Fs*period)/Fs
    Frame_length = int(round_sample_period*Fs)
    Audio_FFT = np.zeros((1 , int(Frame_length) ))
    
   
   # AUDIOinF.append([])
    # Loop from 1s to the end, each iteration last "period (s)"
    # NumOfSample/Fs1 return the time in seconds;
    #   subtract 1 as it is the first second.
    # Div by "period" to transform into multiple of "period"
    i=0
    end_sample=0
    while (end_sample < NumOfSample-(1-overlapping_factor)*round_sample_period*Fs):
    #for i in range(0,int(2*(NumOfSample/Fs - start_calculation_time)/round_sample_period)):
        
        start_sample = max((start_calculation_time+i*(1-overlapping_factor)*round_sample_period),0)*Fs
        end_sample = int(start_sample) + Frame_length
        # Define a time step which last period start from second 1 
        # Extract a segment from audio
        step_time =  np.linspace(start_sample/Fs,end_sample/Fs,Frame_length)
        step_amp = audio[int(start_sample):int(end_sample)]
        Hamming_window = signal.hann(Frame_length, sym=False)
        windowed_amp = np.multiply((Hamming_window), (step_amp))
        # # Calculate FFT with the segment
        Freq_range = np.linspace(0, (Fs/2), int(Frame_length/2))
        Segment_FFT = fft(step_amp,Frame_length)
        Audio_FFT = np.vstack( [Audio_FFT,
                                np.flip(np.log10(( 2/(Frame_length)*np.abs(Segment_FFT[0: int((Frame_length))]))**2 ))])
       
        if (i < 60):
            
            # Plot the segment
            # figure_index+=1
            # plt.figure(figure_index)  
            # plt.plot(step_time,windowed_amp)
            # plt.title('A segment of audio' + audio_index)
            # plt.xlabel('Time (s)')
            # plt.ylabel('audio1')
            
           
        
            
            # Plot FFT
            figure_index+=1
            plt.figure(figure_index)    
            plt.plot(Freq_range[0: int(Frame_length/3)], 
                     np.log10(2/(Frame_length) * np.abs(Segment_FFT)[0: int((Frame_length)/3)]), 
                    label ='Magnitude audio')
            plt.title('DFT - Spectral density of audio' + audio_index +'(t)')
            plt.xlabel('Frequency (Hz)')
            plt.ylabel('Magnitude')
            plt.legend()
        i+=1
    # figure_index+=1
    # plt.figure(figure_index)     
    # plt.imshow((np.swapaxes(Audio_FFT,0,1)),aspect='auto')
    
    
    # plt.figure(figure_index)
    # librosa.display.specshow((Audio_Spectro), y_axis='linear', sr=Fs)

    # plt.xlabel('Time [s]')
    # plt.ylabel('Frequency [Hz]')
    # plt.ylim(0, 2600)
    # plt.tight_layout()


def main():
    global figure_index
    
    # Load audio1.wav with sample rate and signal
    [audio1, Fs1] = lbs.load('audio1.wav',sr=None)
    #[Fs1, audio1] = wavfile.read('audio1.wav')
    DFTandPlot(Fs1, audio1,'1')
    # figure_index+=1
    # plt.figure(figure_index)
    # librosa.display.specshow(lbs.amplitude_to_db(lbs.stft(audio1),ref=np.max), y_axis='linear', sr=Fs1)

    # plt.xlabel('Time [s]')
    # plt.ylabel('Frequency [Hz]')
    # plt.ylim(0, 2600)
    # plt.tight_layout()
    
    plt.show()
    
    
    
    
if __name__== "__main__":
    main()
