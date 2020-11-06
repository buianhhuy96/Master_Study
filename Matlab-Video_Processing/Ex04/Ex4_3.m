clear all;
close all;
A = imread("corel.png");
B = imread("spine.jpg");

imshow(imhistmatch(B,A));


function new_img = myhisteq(img,ref_img)
    reshaped_img = reshape(img,1,[]);
    reshaped_ref_img = reshape(ref_img,1,[]);
       %probability of
    p = 0;
    sum_of_p = zeros(1,256);
    pr = 0;
    sum_of_pr = zeros(1,256);
    for i = 0:255
        p = p + double(length(reshaped_img(reshaped_img==i))/length(reshaped_img));
        sum_of_p(i+1) = sum_of_p(i+1) + p;
        pr = pr + double(length(reshaped_ref_img(reshaped_ref_img==i))/length(reshaped_ref_img));
        sum_of_pr(i+1) = sum_of_pr(i+1) + pr;
    end
    M = zeros(256,1,'uint8');
    for idx = 1 : 256
        [~,ind] = min(abs(sum_of_p(idx) - sum_of_pr));
        M(idx) = ind-1;
    end
   
    new_img = M(double(img)+1);%reshape(new_img,size(img)));
end

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
