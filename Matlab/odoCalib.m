var=num2cell([0.20,0.20,0.12,-0.12,1,0.26]);
[Xcw,Ycw,Xccw,Yccw,L,b0] = var{:};

beta_x=(Xcw-Xccw)/(-4*L);
beta_y=(Ycw+Yccw)/(-4*L);
beta=(beta_x + beta_y)/2;
R=(L/2)/(beta*0.5);
beta=rad2deg(beta);


Ed=(R+b0/2)/(R-b0/2);

alpha_y=(Ycw-Yccw)/(-4*L);
alpha_x=(Xcw+Xccw)/(-4*L);
alpha=(alpha_x + alpha_y)/2;
alpha=rad2deg(alpha);


Eb=90/(90-alpha);

b=Eb*b0




