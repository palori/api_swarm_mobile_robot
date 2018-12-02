img = imread('image_20180302_160452.253_000.png');
size(img);
figure
imshow(img)

figure
subplot(3,1,1)
imshow(img(:,:,1))
subplot(3,1,2)
imshow(img(:,:,2))
subplot(3,1,3)
imshow(img(:,:,3))

imgY = rgb2ycbcr(img);
figure
imshow(imgY)

figure
subplot(3,1,1)
imshow(imgY(:,:,1))
subplot(3,1,2)
imshow(imgY(:,:,2))
subplot(3,1,3)
imshow(imgY(:,:,3))

imgB = imbinarize(imgY(:,:,1)); %instead of 'im2bw'
figure
imshow(imgB)

imgB1 = imbinarize(img(:,:,1));
figure
imshow(imgB1)

se = strel('disk',1);
imgE=imerode(imgB1,se);
figure
imshow(imgE)

imgB2 = imbinarize(img(:,:,2));
figure
imshow(imgB2)
