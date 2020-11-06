clear all;
close all;
I = imread("moon.png");
imshow((I));
imhist(myhisteq(I));


function new_img = myhisteq(img, c)
    reshaped_img = reshape(img,1,[]);
    %probability of
    p = 0;
    sum_of_p = zeros(1,256);
    new_img =reshaped_img;
    for i = 0:255
        p = p + double(length(reshaped_img(reshaped_img==i))/length(reshaped_img));
        sum_of_p(i+1) = sum_of_p(i+1) + p;
        
    end
     for j = 1:length(reshaped_img)
         new_img(j) = double(floor(255 * sum_of_p(reshaped_img(j)+1)));
        
    end
   
    new_img = (reshape(new_img,size(img)));
end
