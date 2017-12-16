%--------------------------------------------------------------------------
% Dieses M-File dient zur Einstellung des Kameraojektives und der Parameter
% Helligkeit, Kontrast und Belichtung. Darüberhinaus kann es genutzt werden
% um den Mittelpunkt und die Ränder des Kamerabildes auf dem Fußboden zu
% makieren.
%--------------------------------------------------------------------------


if exist('vid','var')   %Prüfen ob die Variable 'vid' existiert
    stop(vid);
    delete(vid);
end

clear all;
clc;
vio_number=1; %Nummer des Video Input Objekt
global flag;
global value;

%Video Input Objekt
vid = videoinput('winvideo',vio_number,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid,'manual');    %Trigger auf manuel setzen

%Video Source Objekt
vid_src = getselectedsource(vid);   %Erstellen eines Video Source Objektes
% set(vid_src, 'Brightness',0);   %Einstellen der Helligkeit
% set(vid_src, 'Contrast',255);   %Einstellen des Kontrastes
% set(vid_src, 'Exposure',-10);   %Einstellen der Belichtung
% set(vid_src, 'BrightnessMode','manual');

prev=preview(vid);
disp('Objektiv der Kamera einstellen');
input('Um fortzufahren drücken sie "Return": ');
delete(prev);

% Brightness=118;
% Contrast=0;
% Exposure=-10;
Brightness=500;
Contrast=220;
Exposure=-5.8;
auswahl=0;
set(vid_src, 'BrightnessMode','manual');
set(vid_src, 'Brightness',Brightness);   %Einstellen der Helligkeit
set(vid_src, 'Contrast',Contrast);   %Einstellen des Kontrastes
set(vid_src, 'Exposure',Exposure);   %Einstellen der Belichtung
fig =figure('KeyPressFcn',@bediener_value);     % Grafikfenster einrichten
set(fig,'Name','Frame');
start(vid); %Aktivieren des Video Input Objektes
pause(0.1); %Kurze Pause
clc;
trigger(vid);   %Triggern
while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame = getdata(vid,1); %Bild speichern
stop(vid);
ims =imshow(frame);
