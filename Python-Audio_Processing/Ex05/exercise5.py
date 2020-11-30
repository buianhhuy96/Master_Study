import numpy as np
import scipy
from scipy.io import wavfile
from scipy.fftpack import dct
from numpy.fft import fft, ifft, fftshift
import librosa as lb
import librosa.display
from scipy.signal import *
from matplotlib import pyplot as plt

import math


    
def myMfcc(audioIn, fs, order, melFilter, n_fft):
    wLen = n_fft                  # window length
    wHopLen = int(wLen//4)
    winAn = scipy.signal.hamming(wLen, sym=False) # analysis window
    
    inInd = 0
    melSpectrum = []
    
    powerSpectrum = []
    logMelSpectrum = []
    logPowerSpectrum = []
    DCT = []
    while inInd< (len(order)-wLen)+1:
    
      # selct the frame and multiply with window function
      frame = order[inInd:inInd+wLen]* winAn 
      
      # compute DFT
      f = fft(frame,n_fft)
      
      # calculate magnitudes and power spectrum with Mel filter
      framePSpectrum  = np.power((np.abs(f[:n_fft//2+1])),2)
      frameMelSpectrum = np.matmul(melFilter, framePSpectrum)
      
      frameLogPSpectrum  = np.log10(framePSpectrum)
      frameLogMelSpectrum = np.log10(frameMelSpectrum)
      
      frameDCT = dct(frameLogMelSpectrum)
      
      
      ####################
      # processing in spectral domain 
      #######################
       
      powerSpectrum.append(framePSpectrum)
      melSpectrum.append(frameMelSpectrum)    
      
      logPowerSpectrum.append(frameLogPSpectrum)
      logMelSpectrum.append(frameLogMelSpectrum)
      DCT.append(frameDCT)
      
      # Recover the complex FFT back
      #ft = (abs(f)* np.exp(1j*phi0))  
      
      # inverse DFT and windowing
     
      
      # Ovelap add
      #audioOut[outInd :outInd+wLen] =  audioOut[outInd :outInd + wLen] + framex
      
      
      
      # frame advanced by winHopAn
      inInd = inInd + wHopLen
      
      

   
   
    freq_axis = np.linspace(0,fs/2,n_fft//2+1)
    
    fig,Ex5c =  plt.subplots(3,1)
    Ex5c[0].title.set_text('Power Spectrum (dB)')    
    Ex5c[0].set_xlabel('Frequency (Hz)')
    Ex5c[0].set_ylabel('Magnitude (dB)')
    Ex5c[0].imshow(np.transpose(logPowerSpectrum), aspect='auto')
    
    Ex5c[1].title.set_text('Mel Spectrum')    
    Ex5c[1].set_xlabel('Frequency (Hz)')
    Ex5c[1].set_ylabel('Magnitude')
    Ex5c[1].imshow(np.transpose(melSpectrum), aspect='auto')
    
    Ex5c[2].title.set_text('Mel Spectrum (dB)')    
    Ex5c[2].set_xlabel('Frequency (Hz)')
    Ex5c[2].set_ylabel('Magnitude (dB)')
    Ex5c[2].imshow(np.transpose(logMelSpectrum), aspect='auto')
    return np.swapaxes(DCT,1,0)
    
    # plt.figure()
    # plt.title('DCT of ' + audioName)
    # plt.xlabel('Frame')
    # plt.ylabel('Frequency (Hz)')

    


def main(): 
    
    alpha = 0.97    
    nfft = 512
    nmel = 40
    ############################### Answer 1 ##################################
    # # read audio
    audioIn, fs   = lb.load('audio.wav', sr=None)
    melFilterBank = lb.filters.mel(fs,nfft,nmel)
    plt.figure()
    plt.title("Mel Filter Bank")
    librosa.display.specshow(melFilterBank, y_axis='linear',sr = fs)
    
    ############################### Answer 2 ##################################
    Pre_emphasis = []
    for i in range(0,len(audioIn)):
        if i == 0:
            Pre_emphasis.append(audioIn[i])
        else:
            Pre_emphasis.append(audioIn[i] - alpha*audioIn[i-1])
            
    
    mfcc = myMfcc(audioIn, fs, Pre_emphasis, melFilterBank,nfft)
    librosa_mfcc = librosa.feature.mfcc(y=audioIn, sr = fs, n_mfcc = 40,hop_length=nfft )
    fig, ax = plt.subplots()
    img = librosa.display.specshow(librosa_mfcc, x_axis='time', ax=ax)
    fig.colorbar(img, ax=ax)
    ax.set(title='Librosa MFCC')

    fig, ax = plt.subplots()
    img = librosa.display.specshow(mfcc, x_axis='time', ax=ax)
    fig.colorbar(img, ax=ax)
    ax.set(title='my MFCC')
    #fig,MFCC =  plt.subplots(2,1)
    #fig.colorbar(img, ax=MFCC[0])
    #MFCC[0].set(title='MFCC')
    #MFCC[0].imshow(librosa_mfcc,aspect = 'auto')
    #MFCC[1].imshow(mfcc,aspect = 'auto')
    plt.show()
if __name__== "__main__":
    main()







