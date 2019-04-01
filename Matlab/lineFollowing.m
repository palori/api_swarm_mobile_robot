V = 0.4;
b = 0.156;
R = [0.5:0.1:1,1:1:10,10:10:100,100:100:800];
cm = -100:1:1;
for i=1:10
    K(i) = (-V * b) / (cm(i)*(R(i)+b/2));
end

Kc=0.0015:0.0005:0.004;
figure
for i=1:size(Kc,2)
    Rc = (-V*b) ./ (Kc(i) .* cm) - b/2;
    hold on;
    subplot(2,1,1);
    plot(cm,Rc);
    v1=V+Kc(i).*cm
    hold on;
    subplot(2,1,2);
    plot(cm,v1);
end

hold off;

x=-100:1:100

y = (1/40000)*(x)+0.004;
figure
plot(x,y);


