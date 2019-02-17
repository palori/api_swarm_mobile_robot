

irData = [249,238,0.10;
          171,167,0.15;
          138,130, 0.20;
          115,108,0.25;
          100,93,0.30;
          78,71,0.40;
          62,59,0.50;
          49,46,0.60;
          41,37,0.70];

k0 = [16;10];
calibFun = @(k,d)k(1)./d +k(2);
k1 = lsqcurvefit(calibFun,k0,irData(:,3),irData(:,1));
k2 = lsqcurvefit(calibFun,k0,irData(:,3),irData(:,2));

hold on;
plot(irData(:,3),irData(:,1),'.');
plot(irData(:,3),calibFun(k1,irData(:,3)));

figure
hold on;
plot(irData(:,3),irData(:,2),'.');
plot(irData(:,3),calibFun(k2,irData(:,3)));