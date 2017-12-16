%Video Input Objekt
vid1 = videoinput('winvideo',2,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid1,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid1, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid1,'manual');    %Trigger auf manuel setzen

vid2 = videoinput('winvideo',1,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid2,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid2, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid2,'manual');    %Trigger auf manuel setzen

%Video Source Objekt
vid_src1 = getselectedsource(vid1);   %Erstellen eines Video Source Objektes
set(vid_src1, 'Brightness',0);   %Einstellen der Helligkeit
set(vid_src1, 'Contrast',255);   %Einstellen des Kontrastes
set(vid_src1, 'Exposure',-10);   %Einstellen der Belichtung
set(vid_src1, 'BrightnessMode','manual');

vid_src2 = getselectedsource(vid2);   %Erstellen eines Video Source Objektes
set(vid_src2, 'Brightness',0);   %Einstellen der Helligkeit
set(vid_src2, 'Contrast',255);   %Einstellen des Kontrastes
set(vid_src2, 'Exposure',-10);   %Einstellen der Belichtung
set(vid_src2, 'BrightnessMode','manual');

load Cam1DataR484_c;
load Cam2DataR484_c;
% start(vid); %Aktivieren des Video Input Objektes
% pause(0.5); %Kurze Pause