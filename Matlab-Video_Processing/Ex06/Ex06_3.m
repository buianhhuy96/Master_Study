clear all;
close all;

% Load image
I = im2double(imread("cameraman.tif"));

figure, imshow(I,[]);

[~,FFT_I] = imageDFT(I);
[LPF, HPF] = BWLPfilter(I, 20, 2);

% Filtering images in frequency domain
G1 = LPF .*FFT_I;
G2 = HPF .*FFT_I;

gi1 = ifft2(ifftshift(G1));
g1 = real(gi1);

gi2 = ifft2(ifftshift(G2));
g2 = real(gi2);

% Images which have strong color difference in its nearby pixels, 
% or in other words, more abrupt transitions in brightness or color,
% can be  understood as high frequency,
% (Image noise can be considered as high frequency)

% HPF eliminates the high frequency part of
% the image, makes color changes more gradual. 

% LPF vice versa
figure, imshow(g1,[]);
figure, imshow(g2,[]);

function [LPF, HPF] = BWLPfilter(image, cutoff, n)
    
    [rows,cols] = size(image);
    [u,v] = meshgrid(1:1:cols,1:1:rows);
    D = sqrt( (u-cols/2).^2 + (v-rows/2).^2 );
    LPF = 1./(1+((D./cutoff).^(2*n)));
    HPF = 1 - LPF;
end

function [Spectrum_dB, FFT] = imageDFT(img)
    %FFT_1D = fftshift(fft(fft(img).').');
    FFT = fftshift(fft2(img));
    Spectrum_dB = log((2*abs(FFT)/numel(img))+0.0001);
    
end