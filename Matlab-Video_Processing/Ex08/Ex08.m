clear all;
close all;

% Load image
I = double(imread("DIP.jpg"));

%%%%%%%%%%%%%%%%%%%%%%%%%% Exercise 1 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[mag_I,FFT_I] = imageDFT(I);
motionFilter = motionBlurring(I, 1,0.1,135);

% Filtering images in frequency domain
FFT_result = motionFilter .*FFT_I;
blurredI = real(ifft2(ifftshift(FFT_result)));

% Inverse Filtering
recontructed_Inverse_I = InverseFilter(blurredI, motionFilter);



%%%%%%%%%%%%%%%%%%%%%%%%%% Exercise 2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Add noise to image
noise = 50*(2*rand(size(blurredI))-1);
noised_blurred_I = blurredI + noise;

% Calculate noise spectrum
[mag_noise,FFT_noise] = imageDFT(noise);

% Inverse Filtering
recontructed_Inverse_noisy_I = InverseFilter(noised_blurred_I, motionFilter);

% Wiener Filtering
reconstructed_Wiener_I = WienerFilter(noised_blurred_I, motionFilter,...
                        (mag_noise^2)./(mag_I^2));

% Wiener Filtering with different k
signal_var = var(I(:));
noise_var1 = 10;
NSR1 = noise_var1 / 10000;   
reconstructed_Wiener_I1 = WienerFilter(noised_blurred_I, motionFilter,NSR1);

noise_var2 = 650;
NSR2 = noise_var2 / 10000;   
reconstructed_Wiener_I2 = WienerFilter(noised_blurred_I, motionFilter,NSR2);

noise_var3 = 2000;
NSR3 = noise_var3 / 10000;   
reconstructed_Wiener_I3 = WienerFilter(noised_blurred_I, motionFilter,NSR3);

%%%%%%%%%%%%%%%%%%%%%%%%%% Exercise 1 Output  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
err = immse(recontructed_Inverse_I, I);
fprintf('\n The mean-squared error is %0.15f\n', err);
figure('Name', 'Exercise 1');
subplot(2,2,1), imshow(I,[]),title('Original');
subplot(2,2,2), imshow(blurredI,[]),title('Motion Blurred');
subplot(2,2,3), imshow(recontructed_Inverse_I,[]),title('Recontructed');



%%%%%%%%%%%%%%%%%%%%%%%%%% Exercise 2 Output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('Name', 'Exercise 2');
subplot(2,2,1), imshow(noised_blurred_I,[]),title('Noisy original');
subplot(2,2,2), imshow(recontructed_Inverse_noisy_I,[]),...
                title('Inverse Filter Recontructed');
subplot(2,2,3), imshow(reconstructed_Wiener_I,[]), ...
                title('Wiener Filter Recontructed');
        
figure('Name', 'Exercise 2 - Wiener with different NSR');
subplot(2,2,1), imshow(noised_blurred_I,[]),title('Noisy original');
subplot(2,2,2), imshow(reconstructed_Wiener_I1,[]),...
                title('Wiener Filter with k = '+string(NSR1));
subplot(2,2,3), imshow(reconstructed_Wiener_I2,[]), ...
                title('Wiener Filter with k = '+string(NSR2));
subplot(2,2,4), imshow(reconstructed_Wiener_I3,[]), ...
                title('Wiener Filter with k = '+string(NSR3));

            
            
            
function recontruced_img = WienerFilter(img, degr_func, k)

	[~,FFT_img] = imageDFT(img);
    mag_degr_func = abs(degr_func).^2;
    H = double(1./degr_func).*double((mag_degr_func./(mag_degr_func+k)));
    FFT_reconstructed_img = FFT_img .* H;
    recontruced_img = real(ifft2(ifftshift(FFT_reconstructed_img)));
end


function recontruced_img = InverseFilter(img, degr_func)
    
	[~,FFT_img] = imageDFT(img);
    FFT_reconstructed_img = FFT_img ./ degr_func;
    recontruced_img = real(ifft2(ifftshift(FFT_reconstructed_img)));
end


function H = motionBlurring(image, T, distance, phi)
    
    [rows,cols] = size(image);
    a = distance*sind(phi);
    b = distance*cosd(phi);
    [u, v] = meshgrid(-rows/2:rows/2-1, -cols/2:cols/2-1);
    tmp = pi*(u*a+v*b);
    tmp(tmp(:)==0) = 0.00001;
    H = T./tmp.*sin(tmp).*exp(-1i*tmp);
end

function [Spectrum_mag, Spectrum] = imageDFT(img)
    Spectrum = fftshift(fft2(img));
    Spectrum_mag = abs(Spectrum);
    
end