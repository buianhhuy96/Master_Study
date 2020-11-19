clear all;
close all;
I = imread("lena.jpg");

salt_pepper_noise_I = imnoise(I,'salt & pepper',0.02);

% Create weighted window for filter
weight = ones(5);
weight(3,3) = weight(3,3)*5;

% Display images
figure('Name','Ex05.1')
subplot(2,2,1), imshow(I), title('Original');
subplot(2,2,2), imshow(salt_pepper_noise_I), title('Noise Image');
subplot(2,2,3), imshow(med_filter(salt_pepper_noise_I,weight)), title('Weighted Median Filter');
subplot(2,2,4), imshow(med_filter(salt_pepper_noise_I,ones(5))), title('Median Filter');

function output_img=med_filter(img, weight)
    [m,n]=size(img);
    % get filter size by the length of window
    filter_size = length(weight);
    % add padding to image
    padding_add_img = zeros(m+uint16(filter_size),n+uint16(filter_size));
    padding = uint16(filter_size/2);
    padding_add_img(padding:length(padding_add_img)-padding, ...
                    padding:height(padding_add_img)-padding)= img;
                
    [m,n]=size(padding_add_img);
    filtered_img=zeros(size(padding_add_img));
    % A loop to shift filter window throughout padding_add_image
    for i=1:uint16(m-(filter_size-1))
        for j=1:uint16(n-(filter_size-1))
            filter = reshape(padding_add_img(i:(i+filter_size-1),j:(j+filter_size-1)),1,[]);
            % Get the image area inside window filter 
            % Cre
            temp_for_median = [];
            for t=1:numel(weight)
                temp_for_median = cat(2, temp_for_median, ones(1,weight(t))*filter(t));
            end
            % Calculate median of multiplication result
            filtered_img(i+padding-1,j+padding-1)=median(temp_for_median);
        end
    end
    output_img = ones(size(img));
    % Remove the padding
    output_img = uint8(filtered_img(padding:length(padding_add_img)-padding, ...
                    padding:height(padding_add_img)-padding));
end