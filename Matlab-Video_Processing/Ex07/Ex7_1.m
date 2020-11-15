clear all;
close all;
I = imread("lena.jpg");
gaussian_noise_I = imnoise(I,'gaussian',0.03);
salt_pepper_noise_I = imnoise(I,'salt & pepper',0.05);
R_noise = sqrt(-2000*log((1-rand(size(I)))));
Rayleigh_noise_I = (I) + uint8(R_noise);

% Display images with 4 filters applied 
figure('Name','directional_filtering 3x3')
subplot(2,2,1), imshow(I), title('Original');
subplot(2,2,2), imshow(gaussian_noise_I), title('gaussian');
subplot(2,2,3), imshow(salt_pepper_noise_I), title('salt & pepper');
subplot(2,2,4), imshow(Rayleigh_noise_I), title('Rayleigh');

filter_size = [5 5];
filtered_gaussian = filters_test(gaussian_noise_I,filter_size);

% Display image with 3 filters applied 
figure('Name','Gaussian-noised image')
subplot(2,2,1), imshow(gaussian_noise_I), title('Gaussian-noised image');
subplot(2,2,2), imshow(filtered_gaussian(:,:,1)), title('Arithmetic Mean');
subplot(2,2,3), imshow(filtered_gaussian(:,:,2)), title('Geometric Mean');
subplot(2,2,4), imshow(filtered_gaussian(:,:,3)), title('Harmonic Mean');

filtered_salt_pepper = filters_test(salt_pepper_noise_I,filter_size);

% Display image with 3 filters applied 
figure('Name','Salt&Pepper-noised image')
subplot(2,2,1), imshow(salt_pepper_noise_I), title('Salt&Pepper-noised image');
subplot(2,2,2), imshow(filtered_salt_pepper(:,:,1)), title('Arithmetic Mean');
subplot(2,2,3), imshow(filtered_salt_pepper(:,:,2)), title('Geometric Mean');
subplot(2,2,4), imshow(filtered_salt_pepper(:,:,3)), title('Harmonic Mean');

filtered_Rayleigh = filters_test(Rayleigh_noise_I,filter_size);

% Display image with 3 filters applied 
figure('Name','Rayleigh-noised image')
subplot(2,2,1), imshow(Rayleigh_noise_I), title('Rayleigh-noised image');
subplot(2,2,2), imshow(filtered_Rayleigh(:,:,1)), title('Arithmetic Mean');
subplot(2,2,3), imshow(filtered_Rayleigh(:,:,2)), title('Geometric Mean');
subplot(2,2,4), imshow(filtered_Rayleigh(:,:,3)), title('Harmonic Mean');



 
function output_images=filters_test(img, filter_size)
% Export images with filters
    m = filter_size(1); n = filter_size(2);
    double_img = double(img);
    output_images = uint8([]);
    
    output_images(:,:,1) = uint8(imfilter(double_img,fspecial('average',[m n])));
    output_images(:,:,2) = uint8(exp(imfilter(log(double_img),ones(m,n),'replicate')).^(1/(m*n)));
    output_images(:,:,3) = uint8((m*n) ./ imfilter(1 ./ ( double_img + eps), ones(m,n), 'replicate'));
end
 
