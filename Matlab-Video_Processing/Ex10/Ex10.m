clear all;
close all;

% Load image
I = (imread("cheetah.jpg"));
I2 = (imread("chameleon.jpg"));

figure('Name', 'cheetah');
subplot(1,2,1), imshow(uint8(sliceCube(I,40))),title('sliceCube');
subplot(1,2,2), imshow(uint8(sliceSphere(I,40))),title('sliceSphere');


figure('Name', 'chameleon');
subplot(1,2,1), imshow(uint8(sliceCube(I2,40))),title('sliceCube');
subplot(1,2,2), imshow(uint8(sliceSphere(I2,40))),title('sliceSphere');

function output_img = sliceSphere(img,dist)
    r = double(img(:,:,1));
    g = double(img(:,:,2));
    b = double(img(:,:,3));
    chosen_color = double(impixel(img));
    color_dist = (r - chosen_color(1)).^2 + ...
                 (g - chosen_color(2)).^2 + ...
                 (b - chosen_color(3)).^2;
    mask = (color_dist < dist^2);  
    r_filtered = (double(~mask) .* 127) + double(mask) .* r;     
    g_filtered = (double(~mask) .* 127) + double(mask) .* g; 
    b_filtered = (double(~mask) .* 127) + double(mask) .* b; 
    output_img = cat(3,r_filtered,g_filtered,b_filtered);
end

function output_img = sliceCube(img,dist)
    r = double(img(:,:,1));
    g = double(img(:,:,2));
    b = double(img(:,:,3));
    chosen_color = double(impixel(img));
    
    fprintf('\n The chosen color is %0.15f\n', chosen_color);
    color_dist = abs(r - chosen_color(1)) + ...
                 abs(g - chosen_color(2)) + ...
                 abs(b - chosen_color(3));
    mask = (color_dist < dist);  
    r_filtered = (double(~mask) .* 127) + double(mask) .* r;     
    g_filtered = (double(~mask) .* 127) + double(mask) .* g; 
    b_filtered = (double(~mask) .* 127) + double(mask) .* b; 
    output_img = cat(3,r_filtered,g_filtered,b_filtered);
    
end
