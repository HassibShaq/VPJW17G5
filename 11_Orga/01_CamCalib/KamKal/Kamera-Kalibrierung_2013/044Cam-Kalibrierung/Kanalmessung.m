%--------------------------------------------------------------------------
% Dieses M-File dient zur Ermittlung der Grenzen (z. B. xmax) des 
% Überlappungsbereiches zweier Kamerabilder. Nach jedem Aufruf werden die
% Abweichungen in mm zwischen den beiden Bildern ausgegeben. 
%--------------------------------------------------------------------------
if exist('vid1','var')  %Prüfen ob die Variable 'vid1' existiert
    stop(vid1);
    delete(vid1);
end
if exist('vid2','var')  %Prüfen ob die Variable 'vid2' existiert
    stop(vid2);
    delete(vid2);
end

clear all;
clc;
%Koordinaten der Mittelpunkte der Kameras
x0_1=6261;
y0_1=3058;
x0_2=6210;
y0_2=1283;
hr=215; % Höhe des Reflektors in mm
hr=0;
hc=2870; % Höhe der Kameras in mm
load Cam5DataR462_6;
load Cam4DataR462_6;
xmma=xmm5;
ymma=ymm5;
xpixa=xpix5;
ypixa=ypix5;
xmmb=xmm4;
ymmb=ymm4;
xpixb=xpix4;
ypixb=ypix4;

%Video Input Objekt
vid1 = videoinput('winvideo',5,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid1,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid1, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid1,'manual');    %Trigger auf manuel setzen

vid2 = videoinput('winvideo',4,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid2,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid2, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid2,'manual');    %Trigger auf manuel setzen

start(vid1); %Aktivieren des Video Input Objektes
start(vid2); %Aktivieren des Video Input Objektes

stop(vid1);
stop(vid2);

%Video Source Objekt
vid_src1 = getselectedsource(vid1);   %Erstellen eines Video Source Objektes
set(vid_src1, 'BrightnessMode','manual');
set(vid_src1, 'Brightness',128);   %Einstellen der Helligkeit
set(vid_src1, 'Contrast',62);   %Einstellen des Kontrastes
set(vid_src1, 'Exposure',-10);   %Einstellen der Belichtung

vid_src2 = getselectedsource(vid2);   %Erstellen eines Video Source Objektes
set(vid_src2, 'BrightnessMode','manual');
set(vid_src2, 'Brightness',128);   %Einstellen der Helligkeit
set(vid_src2, 'Contrast',113);   %Einstellen des Kontrastes
set(vid_src2, 'Exposure',-10);   %Einstellen der Belichtung

start(vid1); %Aktivieren des Video Input Objektes
start(vid2); %Aktivieren des Video Input Objektes
pause(0.5); %Kurze Pause

%function [ frame1 ] = get_frame( vid1 )
trigger(vid1);   %Triggern
while (vid1.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame1 = getdata(vid1,1); %Bild speichern
% figure(1);
% imshow(frame);
%function [ frame2 ] = get_frame( vid2 )
trigger(vid2);   %Triggern
while (vid2.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame2 = getdata(vid2,1); %Bild speichern
% figure(2);
% imshow(frame2);

% level=0.01;
bin_frame1=transform_frame(frame1);    %Bild in ein binäres Bild wandeln 
bin_frame2=transform_frame(frame2);    %Bild in ein binäres Bild wandeln 
figure(3);
% subplot(1,2,1);
% imshow(bin_frame1);
imshow(bin_frame1);
t1=linspace(1 , 640, 640);
t2=linspace(1 , 480, 480);
hold on
plot(t1,240,'b-');
plot(320,t2,'b-');
plot(1,1,'b+');
hold off

hold on;
plot (xpixa,ypixa,'b+');
hold off;


figure(4);
% subplot(1,2,2);
% imshow(bin_frame2);
imshow(bin_frame2);
t1=linspace(1 , 640, 640);
t2=linspace(1 , 480, 480);
hold on
plot(t1,240,'b-');
plot(320,t2,'b-');
hold off

hold on;
plot (xpixb,ypixb,'b+');
hold off;

min_ref_groesse=16; %mit bwboundaries(x,4,y) =16

anzahl_objekte1=0;
bounds_hilfe1=bwboundaries(bin_frame1,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
bound_size1 = size(bounds_hilfe1);
for i=1 : bound_size1(1)    %Anzahl weißer Objekte bestimmen
    groesse = size(bounds_hilfe1{i}(:,1));
    groesse = groesse(1);
    if groesse >= min_ref_groesse
        anzahl_objekte1=anzahl_objekte1+1;   
    end
end
if anzahl_objekte1 > 0
    bounds1=cell(0);
    zaehler=1;
    for i=1 : bound_size1(1)    %Zu kleine Objekte aussortieren
        groesse = size(bounds_hilfe1{i}(:,1));
        groesse = groesse(1);
        if groesse >= min_ref_groesse
            bounds1{zaehler}(:,:)=bounds_hilfe1{i}(:,:);   
            zaehler=zaehler+1;
        end
    end
end
bounds1=bounds1';
if anzahl_objekte1 > 0
    reflector1=get_pixel_position(bounds1, anzahl_objekte1);    %Pixelposition ermitteln

    a1(1:anzahl_objekte1,1:2)=0;
    for i=1 : anzahl_objekte1
        a1(i,1)=reflector1(1,i).x0;
        a1(i,2)=reflector1(1,i).y0;
    end
    xrobot(:,1)=griddata(xpixa,ypixa,xmma,a1(:,1),a1(:,2),'v4');    %Umrechnen in mm
    yrobot(:,1)=griddata(xpixa,ypixa,ymma,a1(:,1),a1(:,2),'v4');    %Umrechnen in mm
    xrobot(:,1)=(xrobot(:,1)-x0_1)*(1-hr/hc)+x0_1; %Höhenkorrektur
    yrobot(:,1)=(yrobot(:,1)-y0_1)*(1-hr/hc)+y0_1; %Höhenkorrektur
end

anzahl_objekte2=0;
bounds_hilfe2=bwboundaries(bin_frame2,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
bound_size2 = size(bounds_hilfe2);
for i=1 : bound_size2(1)    %Anzahl weißer Objekte bestimmen
    groesse = size(bounds_hilfe2{i}(:,1));
    groesse = groesse(1);
    if groesse >= min_ref_groesse
        anzahl_objekte2=anzahl_objekte2+1;
    end
end
if anzahl_objekte2 > 0
    bounds2=cell(0);
    zaehler=1;
    for i=1 : bound_size2(1)    %Zu kleine Objekte aussortieren
        groesse = size(bounds_hilfe2{i}(:,1));
        groesse = groesse(1);
        if groesse >= min_ref_groesse
            bounds2{zaehler}(:,:)=bounds_hilfe2{i}(:,:);
            zaehler=zaehler+1;
        end
    end
end
bounds2=bounds2';
if anzahl_objekte2 > 0
    reflector2=get_pixel_position(bounds2, anzahl_objekte2);    %Pixelposition ermitteln

    a2(1:anzahl_objekte2,1:2)=0;
    for i=1 : anzahl_objekte2
        a2(i,1)=reflector2(1,i).x0;
        a2(i,2)=reflector2(1,i).y0;
    end 
    xrobot(:,2)=griddata(xpixb,ypixb,xmmb,a2(:,1),a2(:,2),'v4');    %Umrechnen in mm
    yrobot(:,2)=griddata(xpixb,ypixb,ymmb,a2(:,1),a2(:,2),'v4');    %Umrechnen in mm
    xrobot(:,2)=(xrobot(:,2)-x0_2)*(1-hr/hc)+x0_2; %Höhenkorrektur
    yrobot(:,2)=(yrobot(:,2)-y0_2)*(1-hr/hc)+y0_2; %Höhenkorrektur
end

stop(vid1);
stop(vid2);
% delete(vid1);
% delete(vid2);



disp(['Differenz der x-Position: ',num2str(xrobot(:,1)-xrobot(:,2)), ' mm']);
disp(['Differenz der y-Position: ',num2str(yrobot(:,1)-yrobot(:,2)), ' mm']);
disp(['Abstand der 2 berechneten Positionen des Reflektors: ',num2str( sqrt( (xrobot(:,1)-xrobot(:,2))^2 + (yrobot(:,1)-yrobot(:,2))^2 ) ), ' mm']);
disp(['x-Position des Reflektors im 1. Kamerabild: ',num2str(xrobot(:,1)), ' mm']);
disp(['x-Position des Reflektors im 2. Kamerabild: ',num2str(xrobot(:,2)), ' mm']);
disp(['y-Position des Reflektors im 1. Kamerabild: ',num2str(yrobot(:,1)), ' mm']);
disp(['y-Position des Reflektors im 2. Kamerabild: ',num2str(yrobot(:,2)), ' mm']);
