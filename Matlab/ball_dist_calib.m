y=[0.15,0.2,0.3,0.40,0.45,0.5];
x=[40, 29, 18, 15, 14, 12];
close all;
plot(x,y,'o')

p = polyfit(x,y,2)

hold on;

xp=10:1:40;
yp=p(1).*xp.*xp + p(2).*xp+p(3);

plot(xp,yp)