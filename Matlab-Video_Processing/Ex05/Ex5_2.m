clear all;
close all;
I = imread("cameraman.tif");
noise_I = uint8(double(I) + (2*rand(size(I))-1)*10);
% Display images with noise added 
figure('Name','cameraman')
subplot(2,1,1), imshow(I), title('Original');
subplot(2,1,2), imshow(noise_I), title('Noise image');

% Applied defined filter into image
filter_I_3 = directional_filtering(noise_I, 3);

% Display images with 4 filters applied 
figure('Name','directional_filtering 3x3')
subplot(2,2,1), imshow(uint8(filter_I_3(:,:,1))), title('0 degree');
subplot(2,2,2), imshow(uint8(filter_I_3(:,:,2))), title('45 degree');
subplot(2,2,3), imshow(uint8(filter_I_3(:,:,3))), title('90 degree');
subplot(2,2,4), imshow(uint8(filter_I_3(:,:,4))), title('135 degree');

% Applied defined filter into image
filter_I_5 = directional_filtering(noise_I, 5);

% Display images with 4 filters applied 
figure('Name','directional_filtering 5x5')
subplot(2,2,1), imshow(uint8(filter_I_5(:,:,1))), title('0 degree');
subplot(2,2,2), imshow(uint8(filter_I_5(:,:,2))), title('45 degree');
subplot(2,2,3), imshow(uint8(filter_I_5(:,:,3))), title('90 degree');
subplot(2,2,4), imshow(uint8(filter_I_5(:,:,4))), title('135 degree');

% Applied defined filter into image
filter_I_7 = directional_filtering(noise_I, 5);

% Display images with 4 filters applied 
figure('Name','directional_filtering 7x7')
subplot(2,2,1), imshow(uint8(filter_I_7(:,:,1))), title('0 degree');
subplot(2,2,2), imshow(uint8(filter_I_7(:,:,2))), title('45 degree');
subplot(2,2,3), imshow(uint8(filter_I_7(:,:,3))), title('90 degree');
subplot(2,2,4), imshow(uint8(filter_I_7(:,:,4))), title('135 degree');

% Display combined image of 4 filtered images 
figure('Name','Combined filtered')
imshow(uint8((filter_I_7(:,:,1)+ filter_I_7(:,:,2)+ ...
            filter_I_7(:,:,3)+filter_I_7(:,:,4))/4)), title('0 degree');

function output_images=directional_filtering(img, filter_size)
    % 0 degree filter
    filer_0=zeros(filter_size);
    filer_0(uint8(filter_size/2+0.5),:) = 1/filter_size;
    % 45 degree filter
    filer_45 = rot90(eye(filter_size))/filter_size;
    % 90 degree filter
    filer_90=zeros(filter_size);
    filer_90(:,uint8(filter_size/2+0.5)) = 1/filter_size;
    % 135 degree filter
    filer_135 = (eye(filter_size))/filter_size;
    
    % Export images with filters
    output_images = [];
    output_images(:,:,1) = imfilter(img,filer_0);
    output_images(:,:,2) = imfilter(img,filer_45);
    output_images(:,:,3) = imfilter(img,filer_90);
    output_images(:,:,4) = imfilter(img,filer_135);
end

