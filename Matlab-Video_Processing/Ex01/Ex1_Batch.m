E = imread("peppers.png");

Ex1_batch;

function Ex1_batch
    for i= 1:6
        File_name=join(['c_',num2str(i)]);
        img{i} = Process(imread(join([File_name,'.jpg'])));
        imwrite(img{i},join([File_name,'.bmp']));
    end
end

function result = Process(I)
    tmp = fliplr(imresize(I,0.75)); 
    result = imrotate(tmp(:,1:floor(size(tmp,2)/2),:),90);
    
end
