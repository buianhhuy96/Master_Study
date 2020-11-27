clear all;
close all;

% Load image
I = (imread("cheetah.jpg"));
I2 = (imread("chameleon.jpg"));
dist = 100;
cubeI = sliceCube(I,dist);
sphereI = sliceSphere(I,dist);


f1 = figure('Name', 'cheetah');
subplot(1,2,1), imshow(uint8(cubeI)),title('sliceCube');
subplot(1,2,2), imshow(uint8(sphereI)),title('sliceSphere');


cubeI2 = sliceCube(I2,40);
sphereI2 = sliceSphere(I2,dist);

f2 = figure('Name', 'chameleon');
subplot(1,2,1), imshow(uint8(cubeI2)),title('sliceCube');
subplot(1,2,2), imshow(uint8(sphereI2)),title('sliceSphere');
function chosen_color = chooseColor(img)
    f = figure('Name', 'Choose Color');
    chosen_color = double(impixel(img));
    close (f);
end

function output_img = sliceSphere(img, dist)
    r = double(img(:,:,1));
    g = double(img(:,:,2));
    b = double(img(:,:,3));
    chosen_color = chooseColor(img);
    color_dist = (r - chosen_color(1)).^2 + ...
                 (g - chosen_color(2)).^2 + ...
                 (b - chosen_color(3)).^2;
    mask = (color_dist < dist^2);  
    r_filtered = (double(~mask) .* 127) + double(mask) .* r;     
    g_filtered = (double(~mask) .* 127) + double(mask) .* g; 
    b_filtered = (double(~mask) .* 127) + double(mask) .* b; 
    output_img = cat(3,r_filtered,g_filtered,b_filtered);
end

function output_img = sliceCube(img, dist)
    r = double(img(:,:,1));
    g = double(img(:,:,2));
    b = double(img(:,:,3));
    chosen_color = chooseColor(img);    
    fprintf('\n The chosen color is [%0.2f %0.2f %0.2f] ', chosen_color.');
    color_dist = abs(r - chosen_color(1)) + ...
                 abs(g - chosen_color(2)) + ...
                 abs(b - chosen_color(3));
    mask = (color_dist < dist);  
    r_filtered = (double(~mask) .* 127) + double(mask) .* r;     
    g_filtered = (double(~mask) .* 127) + double(mask) .* g; 
    b_filtered = (double(~mask) .* 127) + double(mask) .* b; 
    output_img = cat(3,r_filtered,g_filtered,b_filtered);
    
end
