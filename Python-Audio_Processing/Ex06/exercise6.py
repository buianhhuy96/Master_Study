import numpy as np
from numpy.fft import fft, ifft, fftshift
import librosa as lb
import librosa.display
from matplotlib import pyplot as plt
import sounddevice

# interactive plotting
plt.ion()



def Hz2Mel(freq):
    return 2595.0 * np.log10(1.0 + freq / 700.0)

def Mel2Hz(mels):
    return 700.0 * (10.0**(mels / 2595.0) - 1.0)

def myMelFilter(fs, nfft, nmel):
    minMel = Hz2Mel(0)
    maxMel = Hz2Mel(fs/2)
    
    mels = np.linspace(minMel, maxMel, nmel+2)
    freqs = Mel2Hz(mels)
    
    filterPoints = np.floor((nfft + 1) / fs * freqs).astype(int)
    filters = np.zeros((len(filterPoints)-2,int(nfft/2+1)))
    
    for n in range(1,len(filterPoints)-1):
        filters[n-1, filterPoints[n-1] : filterPoints[n]] = np.linspace(0, 1, filterPoints[n] - filterPoints[n-1])
        filters[n-1, filterPoints[n] : filterPoints[n + 1]] = np.linspace(1, 0, filterPoints[n + 1] - filterPoints[n])
    # normalizing signal for flatten DFT
    enorm = 1.0 / (freqs[1:nmel+1] - freqs[:nmel])
    filters *= enorm[:, np.newaxis]
    return filters

def detectOnset(audioIn, fs, audioName):
    onsetStrength = librosa.onset.onset_strength(audioIn,sr = fs)
    peaks = librosa.util.peak_pick(onsetStrength,3, 3, 3, 5, 0.5, 10)
    timeIndex = lb.frames_to_time(np.arange(len(onsetStrength)),sr=fs)
    
    
    
    fig,Ex5c =  plt.subplots(2,1)
    Ex5c[0].title.set_text(audioName + ' Spectrogram (dB)')
    Ex5c[0].set_ylabel('Frequency (Hz)')
    Ex5c[0].plot(timeIndex, onsetStrength, alpha=0.8, label='Onset strength')
    Ex5c[0].vlines(timeIndex[peaks], 0, onsetStrength.max(), color='r', alpha=0.8,label='Selected peaks')
    
    D = librosa.stft(audioIn)
    Ex5c[1].title.set_text('Constant Q Spectrtrogram')    
    Ex5c[1].set_ylabel('Frequency (Hz)')
   
    librosa.display.specshow(librosa.amplitude_to_db(D, ref=np.max), y_axis='log', x_axis='time', ax = Ex5c[1])
    
    
    audio_click = lb.clicks(frames=timeIndex[peaks], sr=fs,length=len(audioIn))
    audioIn + audio_click, sounddevice.play
    
    # Ex5c[2].title.set_text('Chromagram')    
    # Ex5c[2].set_xlabel('Time(s)')
    # Ex5c[2].set_ylabel('Frequency (Hz)')
    # librosa.display.specshow(chormagram,  sr =fs, y_axis='log', x_axis='time', ax=Ex5c[2])
    
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
    audioIn, fs   = lb.load('brahms_hungarian_dance_5_short.wav', sr=None)
  
    # Calculate MFCC in 2 different ways
    compareSpectrogram(audioIn, fs, "brahms_hungarian")
    
    
    
    audioIn2, fs2   = lb.load('classic_rock_beat.wav', sr=None)
    #librosa_mfcc = librosa.feature.mfcc(y=audioIn, sr = fs,hop_length=nfft//4, n_mfcc = 40)
    audioIn2, sounddevice.play
    #detectOnset(audioIn2,fs2,"classic_rock")
    # plotting 2 mfccs
    
    # time_axis1 = np.linspace(0,len(audioIn)/fs,len(librosa_mfcc[0]))
    # fig, (ax1, ax2) = plt.subplots(nrows=2, ncols=1)
    # ax1 = plt.subplot(2,1,1)
    # img = librosa.display.specshow(librosa_mfcc, 
    #                                x_coords = time_axis1, ax=ax1)
    # ax1.set(title='Librosa MFCC')
    
    
    # time_axis2 = np.linspace(0,len(audioIn)/fs,len(mfcc[0]))
    # ax2 = plt.subplot(2,1,2)
    # img = librosa.display.specshow(mfcc, x_axis='time', 
    #                                x_coords = time_axis2, ax=ax2)
    # ax2.set(title='My MFCC')
    
    # fig.subplots_adjust(right=0.8)
    # cbar_ax = fig.add_axes([0.85, 0.15, 0.05, 0.7])
    # fig.colorbar(img, cax=cbar_ax)
            
if __name__== "__main__":
    main()







