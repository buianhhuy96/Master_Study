import numpy as np
from numpy.fft import fft, ifft, fftshift
import librosa as lb
import librosa.display
from matplotlib import pyplot as plt
import sounddevice
from librosa.feature.spectral import melspectrogram
# interactive plotting


    ############################### Bonus ##################################
# My own implementation of onset_strength function
def myOnset_Strength(audioIn, fs):
    # Calculate stft
    fft = (librosa.core.stft(audioIn))
        
    # Convert to dB scale
    fftLog = librosa.amplitude_to_db((fft), ref=np.max)
    logCompression = np.transpose(np.log10(1+5*np.abs(fftLog)))
    spectralNovelty = np.sum(np.diff(logCompression),axis=1)
    return spectralNovelty



def detectOnset(audioIn, fs, audioName):
    # Calculating onset and picking peaks based on calculated result
    onsetStrength = librosa.onset.onset_strength(audioIn,sr = fs)
    peaks = librosa.util.peak_pick(onsetStrength,3, 3, 1, 1, 0.5, 10)
    timeIndex = lb.frames_to_time(np.arange(len(onsetStrength)),sr=fs)
    
    # Calculating onset and picking peaks based on calculated result
    # Using own implementation
    myOnsetStrength = myOnset_Strength(audioIn, fs)
    
    # Multiply by 80 to increase amplitude for easier to choose delta parameter
    myPeaks = librosa.util.peak_pick(myOnsetStrength*80,3, 3, 1, 1,1, 10)
    mytimeIndex = lb.frames_to_time(np.arange(len(myOnsetStrength)),sr=fs)
    
    audio_click = lb.clicks(frames=peaks, sr=fs,length=len(audioIn), click_freq=500)
    sounddevice.play((audioIn+audio_click*1.5), samplerate=fs)

    
    # Plot the spectrogram and picked peaks
    fig,window2 =  plt.subplots(3,1)
    window2[0].title.set_text(audioName + ' Onset strength with peaks')
    window2[0].plot(timeIndex, onsetStrength, alpha=0.8, label='Onset strength')
    window2[0].vlines(timeIndex[peaks], 0, onsetStrength.max(), color='r', alpha=0.8)    
    

    window2[1].title.set_text(audioName + ' My onset strength with peaks')
    window2[1].plot(mytimeIndex, myOnsetStrength, alpha=0.8, label='Onset strength')
    window2[1].vlines(mytimeIndex[myPeaks], 0, myOnsetStrength.max(), color='r', alpha=0.8)
    
    
    D = librosa.stft(audioIn)
    window2[2].title.set_text('Constant Q Spectrtrogram')
    window2[2].set_ylabel('Frequency (Hz)')   
    librosa.display.specshow(librosa.amplitude_to_db(D, ref=np.max), y_axis='log', x_axis='time', ax = window2[2])
    
    
def compareSpectrogram(audioIn, fs, audioName):
    # Calculate 3 spectrograms
    spectrogram = lb.amplitude_to_db(lb.core.stft(audioIn),np.max)
    constantQSpectrogram = lb.amplitude_to_db(lb.core.cqt(audioIn, sr = fs),np.max)
    chormagram = lb.amplitude_to_db(lb.feature.chroma_cqt(audioIn, sr = fs),np.max)
    
    # plotting
    fig,window1 =  plt.subplots(3,1)
    window1[0].title.set_text(audioName + ' Spectrogram (dB)')
    window1[0].set_ylabel('Frequency (Hz)')
    
    librosa.display.specshow(spectrogram, sr =fs, y_axis='log' , ax=window1[0])
    
    window1[1].title.set_text('Constant Q Spectrtrogram')    
    window1[1].set_ylabel('Frequency (Hz)')
   
    librosa.display.specshow(constantQSpectrogram,  sr =fs, y_axis='log',  ax=window1[1])
    
    window1[2].title.set_text('Chromagram')    
    window1[2].set_xlabel('Time(s)')
    window1[2].set_ylabel('Frequency (Hz)')
    librosa.display.specshow(chormagram,  sr =fs, y_axis='log', x_axis='time', ax=window1[2])
    
    


def main(): 
    
    ############################### Answer 1 ##################################
    # # read audio
    audioIn, fs = lb.load('brahms_hungarian_dance_5_short.wav', sr=None)
  
    # Calculate and plot 3 types of spectrogram
    compareSpectrogram(audioIn, fs, "brahms_hungarian_dance_5_short")
    
    
    ############################### Answer 2 ##################################   
    audioIn2, fs2   = lb.load('conga_groove.wav', sr=None)
    detectOnset(audioIn2,fs2,"conga_groove")
    plt.show()
if __name__== "__main__":
    main()







