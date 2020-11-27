import numpy as np
import scipy
from scipy.io import wavfile
from scipy.fftpack import dct
from numpy.fft import fft, ifft, fftshift
import librosa as lb
import librosa.display
from scipy.signal import hamming, hann
from matplotlib import pyplot as plt


import math
from audiolazy import lazy_lpc as lpc

# interactive plotting
plt.ion()



def est_predictor_gain(x, a, p):
    '''
    A function to compute gain of the residual signal in LP analyis.
    x:  signal 
    a: LPC coefficients
    p: order of the filter
    '''
    cor = np.correlate(x, x, mode='full')
    
    rr = cor[len(cor)//2: len(cor)//2+p+1]
    g = np.sqrt(np.sum(a*rr))
    return g

   
    
def reject_outliers(data, m=2):
    '''
    Function to reject outliers. All values beyond m standard deviations from means are excluded
    '''
    return data[abs(data - np.mean(data)) < m * np.std(data)]
    
def LPC_Formants(audioIn, fs, order, audioName,melFilter):
    winHopTime = 0.008
    wLenTime = 0.032
    n_fft = 512
    wLen = int(wLenTime*fs)                   # window length
    winAn = hamming(wLen, sym=False) # analysis window
    
    
    winHopAn = int(winHopTime*fs)       # Hop length or frame advance for analysis window
    #winHopSyn = int(winHopAn*R)         # Hop Syn for shifting synthesis window
    inInd = 0
    outInd = 0
    melSpectrum = []
    logMelSpectrum = []
    powerSpectrum = []
    DCT = []
    while inInd< (len(order)-wLen)+1:
    
      # selct the frame and multiply with window function
      frame = order[inInd:inInd+wLen]* winAn 
      
      # compute DFT
      f = fft(frame,n_fft)
      
      # save magnitudes and phases
      framePSpectrum  = np.power((np.abs(f[:n_fft//2+1])),2)
      frameMelSpectrum = np.matmul(melFilter, framePSpectrum)
      frameLogMelSpectrum = 20*np.log10(frameMelSpectrum)
      
      frameDCT = dct(frameLogMelSpectrum)
      
      
      ####################
      # processing in spectral domain 
      #######################
      
      melSpectrum.append(frameMelSpectrum)
      logMelSpectrum.append(frameLogMelSpectrum)
      DCT.append(frameDCT)
      powerSpectrum.append(framePSpectrum)
      
      # Recover the complex FFT back
      #ft = (abs(f)* np.exp(1j*phi0))  
      
      # inverse DFT and windowing
     
      
      # Ovelap add
      #audioOut[outInd :outInd+wLen] =  audioOut[outInd :outInd + wLen] + framex
      
      
      
      # frame advanced by winHopAn
      inInd = inInd + wLen
      # Output frame advanced by winHopSyn
      outInd = outInd + wLen
      
      

   
   
    a = librosa.feature.mfcc(y=audioIn, sr = fs, n_mfcc = 40,hop_length=wLen )
    plt.figure()
    plt.title('DCT of ')
    plt.plot(melSpectrum)
    plt.xlabel('Frame')
    plt.ylabel('Frequency (Hz)')
    
    fig,Ex5 =  plt.subplots(2,1)
    Ex5[0].imshow(a,aspect = 'auto')
    Ex5[1].imshow(np.swapaxes(DCT,1,0),aspect = 'auto')
    # plt.figure()
    # plt.title('DCT of ' + audioName)
    # plt.xlabel('Frame')
    # plt.ylabel('Frequency (Hz)')
    
    


def main(): 
    
    # filter order
    p = 8                      # has to be tuned
    alpha = 0.97
    ############################### Answer 1 ##################################
    # # read audio
    audioIn, fs       =lb.load('audio.wav', sr=None)
    nfft = 512
    nmel = 40
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
            
    
    LPC_Formants(audioIn    ,fs   ,Pre_emphasis,'i.wav',melFilterBank)
    
            
if __name__== "__main__":
    main()







