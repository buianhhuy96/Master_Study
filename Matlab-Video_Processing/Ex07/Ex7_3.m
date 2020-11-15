clear all;
close all;
I = imread("baboon.png");

salt_pepper_noise_I = imnoise(I,'salt & pepper',0.02);


weight = ones(5);
weight(3,3) = weight(3,3)*15;
% Apply log enhancement to image with different scale of c 
% Display results
figure('Name','Ex05.1')
subplot(2,2,1), imshow(salt_pepper_noise_I), title('Original');
subplot(2,2,2), imshow(med_filter(salt_pepper_noise_I,weight)), title('A = 8');
subplot(2,2,3), imshow(med_filter(salt_pepper_noise_I,ones(5))), title('A = 8');
function output_img=med_filter(img, weight)
    [m,n]=size(img);
    filter_size = length(weight);
    added_pixel_img = ones(m+uint16(filter_size),n+uint16(filter_size));
    
    padding = uint16(filter_size/2);
    added_pixel_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding)= img;
    [m,n]=size(added_pixel_img);
    filtered_img=zeros(size(added_pixel_img));
    for i=1:uint16(m-(filter_size-1))
        for j=1:uint16(n-(filter_size-1))
            filter = reshape(added_pixel_img(i:(i+filter_size-1),j:(j+filter_size-1)),1,[]);
            filter = filter.*reshape(weight,1,[]);
            filtered_img(i+padding-1,j+padding-1)=median(filter);
                         
        end
    end
    output_img = ones(size(img));
    output_img = uint8(filtered_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding));
end