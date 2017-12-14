% clear all;
% load test;
clc;

size_out=size(yout);

figure(3);
abweichung=NaN(size_out(1),size_out(2)/3);
j=0;
for i=3 : +3 : size_out(2)
    j=j+1;
    abweichung(:,j)=yout(:,i);
end
plot(tout,abweichung(:,1:size_out(2)/3-1));
legend('Kamera1','Kamera2','Kamera3','Kamera4','Kamera5','Kamera6');
title('Abstand');

figure(4);
fahrtx=NaN(size_out(1),size_out(2)/3);
fahrty=NaN(size_out(1),size_out(2)/3);
j=0;
for i=1 : +3 : size_out(2)
    j=j+1;
    fahrtx(:,j)=yout(:,i);
    fahrty(:,j)=yout(:,i+1);
end
h=plot(fahrtx(:,1:size_out(2)/3),fahrty(:,1:size_out(2)/3));
set(h(size_out(2)/3),'LineStyle','-.');
set(h(size_out(2)/3),'Marker','.');
legend('Kamera1','Kamera2','Kamera3','Kamera4','Kamera5','Kamera6','Gemittelter Wert');
title('XY-Plot');
axis equal;

figure(5)
abweichungx=NaN(size_out(1),size_out(2)/3-1);
for i=1 : size_out(2)/3-1
    abweichungx(:,i)=fahrtx(:,i)-fahrtx(:,size_out(2)/3);
end
plot(tout,abweichungx(:,1:end));
legend('Kamera1','Kamera2','Kamera3','Kamera4','Kamera5','Kamera6');
title('Diffrenz x-Werte');

figure(6)
abweichungy=NaN(size_out(1),size_out(2)/3-1);
for i=1 : size_out(2)/3-1
    abweichungy(:,i)=fahrty(:,i)-fahrty(:,size_out(2)/3);
end
plot(tout,abweichungy(:,1:end));
legend('Kamera1','Kamera2','Kamera3','Kamera4','Kamera5','Kamera6');
title('Diffrenz y-Werte');
