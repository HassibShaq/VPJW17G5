%--------------------------------------------------------------------------
% Errechnet die Rechenzeit und die Zykluszeit und gibt diese grafisch aus.
%--------------------------------------------------------------------------
clc;

ztime=0.1-yout(:,57);
figure(3);
plot(tout,ztime);
r_average=sum(ztime)/length(ztime);
p_average(1:length(ztime))=r_average;
hold on
plot(tout,p_average,'r-');
hold off
xlabel('Zeit t in sec');
ylabel('Rechenzeit in sec');
title('Rechenzeitmessung');
legend('Rechenzeit','Durschnittsrechenzeit');

stime=zeros(length(ztime)-1,1);
for i=1 : length(stime)
    time1=yout(i,25)*3600 + yout(i,26)*60 + yout(i,27);
    time2=yout(i+1,25)*3600 + yout(i+1,26)*60 + yout(i+1,27);
    stime(i)=time2-time1;
%     stime(i)=etime(yout(i+1,25), yout(i,25));
end
z_average=sum(stime)/length(stime);
q_average(1:length(stime))=z_average;
figure(4);
plot(tout(1:length(stime)),stime);
hold on
plot(tout(1:length(stime)),q_average,'r-');
hold off
xlabel('Zeit t in sec');
ylabel('Zykluszeit in sec');
title('Zykluszeitmessung');
legend('Zykluszeit','Durschnittszykluszeit');