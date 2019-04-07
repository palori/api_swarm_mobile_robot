% Image Analysis Projcet
% Imma Martínez 17/03/2019

close all;
clear all;
clc;
%n=13;
%n=[4 5 13 23 26 41 46 53];
% n=6;
 n=[31]
%n=[1 2 3 4 5 6 7 8 9 10];


for i=1:length(n)

Imagename = sprintf('test1_%03d.png', n(i));
I=imread(Imagename);

%% Transfrom to gray level
if n<51 
    I = rgb2gray(I);
end

%% Resize to 320 pixels
s=size(I);
factor=320/max(s);
I2=imresize(I,factor);
newname = sprintf('pic_img_%03d.png', n(i))
imwrite(I2,newname);


%% Mean filter

%% Hist strech

I3=double(I2);

%Max and min from directly the data of image (matrix)
vmax=max(max(I3));
vmin=min(min(I3));

%Max and min desireds
vmax_d=255;
vmin_d=0;

I3=(vmax_d-vmin_d)/(vmax-vmin)*(I2-vmin)+vmin_d;
I3=uint8(I3);

%% Gamma Mapping
gamma=2.5;
I4=double(I3);
I4=((I4/255).^gamma)*255;
I4=uint8(I4);

figure
subplot(2,3,1)
imshow(I);
subplot(2,3,4)
imhist(I);
subplot(2,3,2)
imshow(I3);
subplot(2,3,5)
imhist(I3);
subplot(2,3,3)
imshow(I4);
subplot(2,3,6)
imhist(I4);

%% Edge detection & Thresholding

level=graythresh(I4);
level*255
%I5_2 = imbinarize(I4,level);
%figure
%subplot(1,2,1)
level=0.6;
I5 = edge(I4,'Canny',level);   %with gamma
%imshow(I5)
%subplot(1,2,2)
%I5_2 = edge(I3,'Canny',level);  %without gamma
%imshow(I5_2)

%% BLOB
I6 = bwlabel(I5); % apply bwlabel with 8-neighbours

% Eliminate Blobs with area <80
stats8=regionprops(I6,'Area');
A=[stats8.Area];
idx=find([stats8.Area]>80);
I7=ismember(I6,idx);


I7=bwlabel(I7);
stats8=regionprops(I7,'Area','BoundingBox');
A=[stats8.Area];
B=[stats8.BoundingBox];


numB(i)=max(max(I6));
numB2(i)=max(max(I7));

%Bounding box

figure
subplot(2,2,1)
imshow(I5);
subplot(2,2,2)
imagesc(I6);
subplot(2,2,3)
imagesc(I7);
hold on 
for s=1:numB2(i)
rectangle('Position', stats8(s).BoundingBox,'EdgeColor','k');
end
subplot(2,2,4)

end
