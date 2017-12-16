if exist('vid','var')   %Prüfen ob die Variable 'vid' existiert
    stop(vid);
    delete(vid);
end

clear all;
clc;
vio_number=1; %Nummer des Video Input Objekt
Brightness=128;
Contrast=98;
Exposure=-9;
level=0.01; %Festlegen, wie hell die Bildpunkte sein müssen, um als weiß zu gelten. 0.99

%Video Input Objekt
vid = videoinput('winvideo',vio_number,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid,'manual');    %Trigger auf manuel setzen

%Video Source Objekt
vid_src = getselectedsource(vid);   %Erstellen eines Video Source Objektes
set(vid_src, 'BrightnessMode','manual');
set(vid_src, 'Brightness',Brightness);   %Einstellen der Helligkeit
set(vid_src, 'Contrast',Contrast);   %Einstellen des Kontrastes
set(vid_src, 'Exposure',Exposure);   %Einstellen der Belichtung

start(vid); %Aktivieren des Video Input Objektes
pause(0.1); %Kurze Pause
trigger(vid);   %Triggern
while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame = getdata(vid,1); %Bild speichern
stop(vid);
figure(3);
imshow(frame);

bin_frame=im2bw(frame,level);   %Funktion, die aus einem Bild ein digitales macht.

figure(4);
imshow(bin_frame);