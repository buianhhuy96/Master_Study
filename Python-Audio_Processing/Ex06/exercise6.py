import numpy as np
from numpy.fft import fft, ifft, fftshift
import librosa as lb
import librosa.display
from matplotlib import pyplot as plt
import sounddevice
from librosa.feature.spectral import melspectrogram
# interactive plotting
plt.ion()

def myOnset_Strength(audioIn, fs):
    fft = (librosa.core.stft(audioIn))
    fft = melspectrogram(audioIn, fs)
    fftLog = librosa.amplitude_to_db((fft), ref=np.max)
    logCompression = np.transpose(np.log10(1+2*np.abs(fftLog)))
    spectralNovelty = np.sum(np.diff(logCompression),axis=1)
    return spectralNovelty

def detectOnset(audioIn, fs, audioName):
    onsetStrength = librosa.onset.onset_strength(audioIn,sr = fs)
    myOnsetStrength = myOnset_Strength(audioIn, fs)
    peaks = librosa.util.peak_pick(onsetStrength,1, 1, 1, 1, 1, 20)
    timeIndex = lb.frames_to_time(np.arange(len(onsetStrength)),sr=fs)
    mytimeIndex = lb.frames_to_time(np.arange(len(myOnsetStrength)),sr=fs)
    
    
    fig,Ex5c =  plt.subplots(3,1)
    Ex5c[0].title.set_text(audioName + ' Onset strength with peaks')
    Ex5c[0].plot(timeIndex, onsetStrength, alpha=0.8, label='Onset strength')
    Ex5c[0].vlines(timeIndex[peaks], 0, onsetStrength.max(), color='r', alpha=0.8,label='Selected peaks')


    Ex5c[1].title.set_text(audioName + ' My onset strength with peaks')
    Ex5c[1].plot(mytimeIndex, myOnsetStrength, alpha=0.8, label='Onset strength')
    Ex5c[1].vlines(mytimeIndex[peaks], 0, myOnsetStrength.max(), color='r', alpha=0.8,label='Selected peaks')
    
    D = librosa.stft(audioIn)
    Ex5c[2].title.set_text('Constant Q Spectrtrogram')
    Ex5c[2].set_ylabel('Frequency (Hz)')
   
    librosa.display.specshow(librosa.amplitude_to_db(D, ref=np.max), y_axis='log', x_axis='time', ax = Ex5c[2])
    
    
    audio_click = lb.clicks(frames=peaks, sr=fs,length=len(audioIn), click_freq=100)
    sounddevice.play((audioIn+audio_click*1.5), samplerate=fs)

    
def compareSpectrogram(audioIn, fs, audioName):
    spectrogram = lb.amplitude_to_db(lb.core.stft(audioIn),np.max)
    constantQSpectrogram = lb.amplitude_to_db(lb.core.cqt(audioIn, sr = fs),np.max)
    chormagram = lb.amplitude_to_db(lb.feature.chroma_cqt(audioIn, sr = fs),np.max)
    
    
    fig,Ex5c =  plt.subplots(3,1)
    Ex5c[0].title.set_text(audioName + ' Spectrogram (dB)')
    Ex5c[0].set_ylabel('Frequency (Hz)')
    
    librosa.display.specshow(spectrogram, sr =fs, y_axis='log' , ax=Ex5c[0])
    #Ex5c[0].imshow(np.transpose(logPowerSpectrum),
    #               extent=[0,len(audioIn)/fs,0,fs/2],aspect='auto')
    
    Ex5c[1].title.set_text('Constant Q Spectrtrogram')    
    Ex5c[1].set_ylabel('Frequency (Hz)')
   
    librosa.display.specshow(constantQSpectrogram,  sr =fs, y_axis='log',  ax=Ex5c[1])
    
    Ex5c[2].title.set_text('Chromagram')    
    Ex5c[2].set_xlabel('Time(s)')
    Ex5c[2].set_ylabel('Frequency (Hz)')
    librosa.display.specshow(chormagram,  sr =fs, y_axis='log', x_axis='time', ax=Ex5c[2])
    #return np.transpose(MFCC)
    
    
    


def main(): 
    
    alpha = 0.97    
    nfft = 512
    nmel = 40
    ############################### Answer 1 ##################################
    # # read audio
    audioIn, fs = lb.load('brahms_hungarian_dance_5_short.wav', sr=None)
  
    # Calculate MFCC in 2 different ways
    #compareSpectrogram(audioIn, fs, "brahms_hungarian")
    
    
    
    audioIn2, fs2   = lb.load('classic_rock_beat.wav', sr=None)
    #librosa_mfcc = librosa.fea0ture.mfcc(y=audioIn, sr = fs,hop_length=nfft//4, n_mfcc = 40)
    #sounddevice.play(audioIn2, samplerate=fs2)
    detectOnset(audioIn2,fs2,"classic_rock")
            
if __name__== "__main__":
    main()







