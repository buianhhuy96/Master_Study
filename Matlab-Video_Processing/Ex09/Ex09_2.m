clear all;
close all;

% Load image
I = (imread("lake.jpg"));
% Convert to HSV and get Saturation Component
I_HSV = rgb2hsv(I);
Is = I_HSV(:,:,2);
% Filter image component with Weighted Median Filter
weight = ones(5);
weight(3,3) = 5;
Is = (med_filter(Is,weight));
% Convert filtered image to Binary
IBW = imbinarize(Is,graythresh(Is)*1);

% Label each part of binary image
L = bwlabel(IBW);

% Get the intensity of each part
histogram_y = myhist(L);

% Descendence sort to get parts that cover largest area of image
[sorted_intensity,index] = sort(histogram_y, 'descend');

% List indices of 3 largest part of the image
k1= find(L==index(1)-1);
k2= find(L==index(2)-1);
k3= find(L==index(3)-1);




%%%%%%%%%%%%%%%%%%%%%%%%%% Exercise Output  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('Name', 'HSV component');
subplot(1,3,1), imshow(I_HSV(:,:,1)),title('Hue');
subplot(1,3,2), imshow(I_HSV(:,:,2)),title('Saturation');
subplot(1,3,3), imshow(I_HSV(:,:,3)),title('Value');


figure('Name', 'Exercise Output');
subplot(2,3,1), imshow((I)),title('Original');
subplot(2,3,2), imshow(Is,[]),title('Saturation component');
subplot(2,3,3), imshow(IBW),title('Binary Image');

subplot(2,3,4), imshow(extractImage(I,k1)),title('Largest Area');% 
subplot(2,3,5), imshow(extractImage(I,k2)),title('2nd Largest Area');
subplot(2,3,6), imshow(extractImage(I,k3)),title('3rd Largest Area');


function img_part = extractImage(img,k)
    r = img(:,:,1);
    g = img(:,:,2);
    b = img(:,:,3);
    [m n p] = size(img);
    rr = zeros(m,n);
    gg = zeros(m,n);
    bb = zeros(m,n);
    rr(k) = r(k);
    gg(k) = g(k);
    bb(k) = b(k);
    img_part = uint8(cat(3,rr,gg,bb));
end

function histogram_y = myhist(img)
    histogram_y = zeros(1,max(img(:))+1);
    for i= 0:max(img(:))
        histogram_y(i+1) = length(img(img==i));
    end  
end

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
   
    % Remove the padding
    output_img = (filtered_img(padding:length(padding_add_img)-padding, ...
                    padding:height(padding_add_img)-padding));
end