import numpy as np
from scipy.io import wavfile
from numpy.fft import fft, ifft, fftshift

import matplotlib.pyplot as plt
import librosa as lb
#import sys
from scipy.signal import hann



def princarg(phase_in):
  """
  Computes principle argument,  wraps phase to (-pi, pi]
  """
  phase = np.mod(phase_in + np.pi,-2*np.pi)+np.pi;
  return phase
  

def speedx(sound_array, factor):
    """ Multiplies the sound's speed by some `factor` """
    indices = (np.round( np.arange(0, len(sound_array), factor) ))#.astype(int)
    indices = indices[indices < len(sound_array)].astype(int)
    return sound_array[ indices.astype(int) ]

def delta_phi_(Phase_current, Phase_previous, winHopAn, wLen):
    """
    Function for calculating unwrapped phase difference between consecutive frames
    
    Phase_current: current frame phase
    Phase_previous: previous frame phase
    winHopAn: Analysis hop length
    wLen: window length
    """
    
    # nominal phase increment for the analysis hop size for each bin
    omega = 2*np.pi*(winHopAn/wLen)*np.arange(0, wLen)
    delta_phi = omega + princarg(Phase_current-Phase_previous-omega)
    
    return delta_phi
    

def main():
    # A Loop for overap add reconstruction  with no spectral processing in between    
    audioIn, fs=lb.load('audio.wav', sr=None)   # read audio
    
    R=1.55
    wLen = int(0.032*fs)                   # window length
    audioOut = np.zeros(int(len(audioIn)*R))        # placeholder for reconstructed audio
    winAn = np.sqrt(hann(wLen, sym=False)) # analysis window
    winSyn =winAn
    
    winHopAn = int(0.002*fs/2)             # Hop length or frame advance
    winHopSyn = int(winHopAn*R)
    inInd = 0
    outInd = 0
    prevPhi = 0
    prevNewPhi = 0
    while inInd< len(audioIn)-wLen:
    
      # selct the frame and multiply with window function
      frame = audioIn[inInd:inInd+wLen]* winAn 
      
      # compute DFT
      f = fft(frame)
      
      # save magnitudes and phases
      mag_f = np.abs(f)
      phi0 = np.angle(f) 
      
      # processing phase
      delta_phi = delta_phi_(phi0,prevPhi,winHopAn,wLen)
      newPhi = princarg(prevNewPhi + R*delta_phi)
      
      
      
      
      ####################
      # processing in spectral domain 
      #######################
      
      # Recover the complex FFT back
      my_ft = (mag_f* np.exp(1j*newPhi))  
      
      
      # Recover the complex FFT back
      #ft = (abs(f)* np.exp(1j*phi0))  
      
      # inverse DFT and windowing
      framex = np.real(ifft(my_ft))*winSyn
      
      # Ovelap add
      audioOut[outInd :outInd+wLen] =  audioOut[outInd :outInd + wLen] + framex
      
      
      prevPhi = phi0
      prevNewPhi = newPhi
      
      # frame advance by winHopAn
      inInd = inInd + winHopAn
      outInd = outInd + winHopSyn
    # Write sum signal to file
    audioOut = speedx(audioOut,R/1.4)

    wavfile.write('AudioOut.wav', fs, audioOut) 
    plt.figure()
    plt.plot(audioIn)
    
    
   # plt.figure()
    plt.plot(audioOut)
    
if __name__== "__main__":
    main()

