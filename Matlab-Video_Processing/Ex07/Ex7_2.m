clear all;
close all;

I = imread("miranda1.tif");
noise = (2*rand(101)-1)*10;
% add noise to image
I(length(I)/2-50:length(I)/2+50,height(I)/2-50:height(I)/2+50) = ...
    uint8(double(I(length(I)/2-50:length(I)/2+50,height(I)/2-50:height(I)/2+50)) ...
        + noise);

filter_T = med_filter(I,5);
threshold_filter_T = threshold_med_filter(I,5,2);
figure('Name','miranda1')
subplot(1,2,1), imshow(im2double(threshold_filter_T)), title('Threshold filtered image');
subplot(1,2,2), imshow(uint8(filter_T)), title('Filtered image');


function output_img=med_filter(img, filter_size)
    [m,n]=size(img);
    added_pixel_img = ones(m+uint16(filter_size),n+uint16(filter_size));
    
    padding = uint16(filter_size/2);
    added_pixel_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding)= img;
    [m,n]=size(added_pixel_img);
    filtered_img=zeros(size(added_pixel_img));
    for i=1:uint16(m-(filter_size-1))
        for j=1:uint16(n-(filter_size-1))
            filter = reshape(added_pixel_img(i:(i+filter_size-1),j:(j+filter_size-1)),1,[]);
            filtered_img(i+padding-1,j+padding-1)=median(filter);
                         
        end
    end
    output_img = ones(size(img));
    output_img = filtered_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding);
end

function output_img=threshold_med_filter(img, filter_size, alpha)
    
    [m,n]=size(img);
    added_pixel_img = ones(m+uint16(filter_size),n+uint16(filter_size));
    
    padding = uint16(filter_size/2);
    added_pixel_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding)= img;
                
    [m,n]=size(added_pixel_img);
    filtered_img=zeros(size(added_pixel_img));
    for i=1:uint16(m-(filter_size-1))
        for j=1:uint16(n-(filter_size-1))
            filter = reshape(added_pixel_img(i:(i+filter_size-1),j:(j+filter_size-1)),1,[]);
            
            if (alpha > abs(median(filter)- added_pixel_img(i+uint16(filter_size/2),j+uint16(filter_size/2))))
                filtered_img(i+padding-1,j+padding-1)=median(filter);
            
            else
                filtered_img(i+padding-1,j+padding-1)= ...
                    added_pixel_img(i+padding-1,j+padding-1);
            end
                
        end
    end
    output_img = ones(size(img));
    output_img = filtered_img(padding:length(added_pixel_img)-padding, ...
                    padding:height(added_pixel_img)-padding);
end


% Threshold filter keep original color/gray-level of the pixel when it detects the difference between median color/gray-level
% and original one higher than alpha (an allowed threshold). Therefore, it can be used in image with strong color difference 
% among image area or pixels.