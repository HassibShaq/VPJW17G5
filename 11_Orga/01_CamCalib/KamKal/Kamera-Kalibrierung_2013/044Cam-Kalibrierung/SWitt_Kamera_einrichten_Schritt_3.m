%--------------------------------------------------------------------------
set(vid_src, 'Brightness',128);   %Einstellen der Helligkeit
set(vid_src, 'Contrast',128);   %Einstellen des Kontrastes
set(vid_src, 'Exposure',-5);   %Einstellen der Belichtung
set(vid_src, 'BrightnessMode','auto');
start(vid); %Aktivieren des Video Input Objektes
pause(0.5); %Kurze Pause
flag=1;
fig =figure('KeyPressFcn',@bediener);     % Grafikfenster einrichten  
while(flag)   %Bestimmen des Mittelpunktes
    clc;
    disp('Makieren Sie den Mittelpunktes und die Bildränder auf dem Fußboden.');
    disp('Drücken Sie "r", wenn Sie fertig sind.');
    trigger(vid);   %Triggern
    while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
    end
    frame = getdata(vid,1); %Bild speichern
    imshow(frame);
    hold on;
    plot([1 640],[240 240],'b-');
    plot([320 320],[1 480],'b-');
    hold off;
    pause (0.1);
end
delete(fig);
stop(vid);

disp('Notieren Sie sich Folgende Werte für die aktuelle Kamera.');
disp(['Helligkeit: ',num2str(Brightness)]);
disp(['Kontrast: ',num2str(Contrast)]);
disp(['Belichtung: ',num2str(Exposure)]);

% delete(fig);
% stop(vid);
