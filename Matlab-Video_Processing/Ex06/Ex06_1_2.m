clear all;
close all;

% Create 1st image
I1 = 0.5*ones(128);

% Create 2nd image
I2 = zeros(128);
I2(55:74,55:74) = ones(20);

% Create 3rd image
x3 = linspace(0,1,128);
[I3,~] = meshgrid(x3);  

% % Create 4nd image
I4 = zeros(128);
I4(64,64) = 1;

% Create 5nd image
x5 = linspace(0,1,128);
[X,Y] = meshgrid(x5);
periods=4;
I5 = cos(Y*periods*2*pi).*cos(X*periods*2*pi);%*0.5+0.5;
%surf(I5);
figure('Name','Original')
subplot(2,3,1), imshow(I1,[]), title("image 1");
subplot(2,3,2), imshow(I2,[]), title("image 2");
subplot(2,3,3), imshow(I3,[]), title("image 3");
subplot(2,3,4), imshow(I4,[]), title("image 4");
subplot(2,3,5), imshow(I5,[]), title("image 5");

% FFT for all images

figure('Name','Applying Fourier Transform')
% Expected: single dot at center of image. 
% I1 has contant value across all pixels 
% => There is only 1 DC component (at f =0) for both axes 
subplot(2,3,1), imshow(imageDFT(I1),[]), title("image 1");

% Expected: A white cross in the center expand to 4 edges
% I2 has an white square at the center of image, which can be considered
% as a square wave for both axes
% => Output is similar to a sinc function which is FFT of square wave
subplot(2,3,2), imshow(imageDFT(I2),[]), title("image 2");

% Expected: A white line across left to right in the middle of image
% On one x-axis, each element after has value incremented by 1
% On one y-axis, all elements has the same value.
% => only 1 line is white, and its white level is result of FFT of ramp
% function
subplot(2,3,3), imshow(imageDFT(I3),[]), title("image 3");

% Expected: Gray-image.
% It should be completely black but because 1 noise pixel at the center
% that makes the output of FFT consists of multiple frequency. Although
% they may be unnoticable, "imshow" scale it from black to white with lowest and 
% highest value. Therefore we'll see gray image
% => only 1 line is white, and its white level is result of FFT of ramp
% function
subplot(2,3,4), imshow(imageDFT(I4),[]), title("image 4");

% Expected: A white cross in the center expand to 4 edges
% I5 has a sinuous wave in both axes
% => Output is 4 dots represents positive and negative parts of x and y
% axes
subplot(2,3,5), imshow(imageDFT(I5),[]), title("image 5");

function Spectrum_dB = imageDFT(img)
    %FFT_1D = fftshift(fft(fft(img).').');
    FFT = fftshift(fft2(img));
    Spectrum_dB = log((4*abs(FFT)/length(img)/height(img))+0.0001);
    
end