%--------------------------------------------------------------------------
% Dieses M-File dient zur Ermittlung der Matritzen xpix, ypix, xmm und ymm,
% die für die Umrechnung von Pixel in Millimeter benötigt werden.
%--------------------------------------------------------------------------

if exist('vid','var')   %Prüfen ob die Variable 'vid' existiert
    stop(vid);
    delete(vid);
end

clear all;
clc;

global flag;
global shutter;
vio_number=4; %Nummer des Video Input Objekt

% load sector;

%Video Input Objekt
vid = videoinput('winvideo',vio_number,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid,'TriggerRepeat',Inf);   %Setzen der möglichen Triggerwiederholungen
set(vid, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid,'manual');    %Trigger auf manuel setzen

start(vid);
stop(vid);

%Video Source Objekt
vid_src = getselectedsource(vid);   %Erstellen eines Video Source Objektes

Brightness=input('Geben Sie einen Wert zwischen 0 und 256 für die Helligkeit ein: ');
if Brightness < 0
    Brightness=0;
elseif Brightness > 256
    Brightness=256;
end
Contrast=input('Geben Sie einen Wert zwischen 0 und 255 für den Kontrast ein: ');
if Contrast < 0
    Contrast=0;
elseif Contrast > 255
    Contrast=255;
end
Exposure=input('Geben Sie einen Wert zwischen -11 und -5 für die Belichtung ein: ');
if Exposure < -11
    Exposure=-11;
elseif Exposure > -5
    Exposure=-5;
end

set(vid_src, 'BrightnessMode','manual');
set(vid_src, 'Brightness',Brightness);   %Einstellen der Helligkeit
set(vid_src, 'Contrast',Contrast);   %Einstellen des Kontrastes
set(vid_src, 'Exposure',Exposure);   %Einstellen der Belichtung
fig =figure('KeyPressFcn',@bediener_cal);     % Grafikfenster einrichten
auswahl=0;
while(auswahl~=5)   %Einstellen des Video Source Objektes
    clc;
    disp('Kalibrieren der Kamera');
    disp('Drücken Sie 1 um eine Aufnahme des oberen linken Sektor zu erstellen.');
    disp('Drücken Sie 2 um eine Aufnahme des unteren linken Sektor zu erstellen.');
    disp('Drücken Sie 3 um eine Aufnahme des oberen rechten Sektor zu erstellen.');
    disp('Drücken Sie 4 um eine Aufnahme des unteren rechten Sektor zu erstellen.');
    disp('Drücken Sie 5, wenn Aufnahmen von allen 4 Sektoren vorhanden sind.');
    auswahl=input('Mit "Return" bestätigen: ');
    
    switch auswahl
        case 1
            flag=1;
            shutter=0;
            start(vid); %Aktivieren des Video Input Objektes
            pause(0.1); %Kurze Pause
            trigger(vid);   %Triggern
            while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
            end
            frame = getdata(vid,1); %Bild speichern
            bin_frame = transform_frame(frame); %Binäres Bild erstellen
            bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
            [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
%             reflector(1:anzahl_reflektoren)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
            reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
            a=zeros(anzahl_reflektoren,2);
            for i=1 : anzahl_reflektoren
                a(i,1)=reflector(1,i).x0;
                a(i,2)=reflector(1,i).y0;
            end
            sector_ol=a;
            midpoint=find_midpoint(a, anzahl_reflektoren);
            imshow(bin_frame);
            while(flag)   %Einstellen der Helligkeit
                clc;
                disp('Aufnahme des oberen linken Sektors erstellen.');
                disp('Drücken Sie "n" um eine neue Aufnahme zu machen.');
                disp('Drücken Sie "r", wenn Sie fertig sind.');
                disp(['Anzahl der Reflektoren: ',num2str(anzahl_reflektoren)]);
                disp('Reflektorporitionen des Reflektors auf dem Mittelpunkt: ');
                disp(num2str(midpoint));
                if shutter==1
                    trigger(vid);   %Triggern
                    while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
                    end
                    frame = getdata(vid,1); %Bild speichern
                    bin_frame = transform_frame(frame); %Binäres Bild erstellen
                    bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
                    [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
                    reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
                    a=zeros(anzahl_reflektoren,2);
                    for i=1 : anzahl_reflektoren
                        a(i,1)=reflector(1,i).x0;
                        a(i,2)=reflector(1,i).y0;
                    end
                    sector_ol=a;
                    midpoint=find_midpoint(a, anzahl_reflektoren);  %Dichteste Koordinate zum Bildmittelpunkt finden
                    shutter=0;
                end
                imshow(bin_frame);
                pause(0.1); %Kurze Pause
            end
            stop(vid);
            
        case 2
            flag=1;
            shutter=0;
            start(vid); %Aktivieren des Video Input Objektes
            pause(0.1); %Kurze Pause
            trigger(vid);   %Triggern
            while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
            end
            frame = getdata(vid,1); %Bild speichern
            bin_frame = transform_frame(frame); %Binäres Bild erstellen
            bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
            [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
%             reflector(1:anzahl_reflektoren)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
            reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
            a=zeros(anzahl_reflektoren,2);
            for i=1 : anzahl_reflektoren
                a(i,1)=reflector(1,i).x0;
                a(i,2)=reflector(1,i).y0;
            end
            sector_ul=a;
            midpoint=find_midpoint(a, anzahl_reflektoren);
            imshow(bin_frame);
            while(flag)   %Einstellen der Helligkeit
                clc;
                disp('Aufnahme des unteren linken Sektors erstellen.');
                disp('Drücken Sie "n" um eine neue Aufnahme zu machen.');
                disp('Drücken Sie "r", wenn Sie fertig sind.');
                disp(['Anzahl der Reflektoren: ',num2str(anzahl_reflektoren)]);
                disp('Reflektorporitionen des Reflektors auf dem Mittelpunkt: ');
                disp(num2str(midpoint));
                if shutter==1
                    trigger(vid);   %Triggern
                    while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
                    end
                    frame = getdata(vid,1); %Bild speichern
                    bin_frame = transform_frame(frame); %Binäres Bild erstellen
                    bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
                    [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
                    reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
                    a=zeros(anzahl_reflektoren,2);
                    for i=1 : anzahl_reflektoren
                        a(i,1)=reflector(1,i).x0;
                        a(i,2)=reflector(1,i).y0;
                    end
                    sector_ul=a;
                    midpoint=find_midpoint(a, anzahl_reflektoren);
                    shutter=0;
                end
                imshow(bin_frame);
                pause(0.1); %Kurze Pause
            end
            stop(vid);
            
        case 3
            flag=1;
            shutter=0;
            start(vid); %Aktivieren des Video Input Objektes
            pause(0.1); %Kurze Pause
            trigger(vid);   %Triggern
            while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
            end
            frame = getdata(vid,1); %Bild speichern
            bin_frame = transform_frame(frame); %Binäres Bild erstellen
            bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
            [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
%             reflector(1:anzahl_reflektoren)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
            reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
            a=zeros(anzahl_reflektoren,2);
            for i=1 : anzahl_reflektoren
                a(i,1)=reflector(1,i).x0;
                a(i,2)=reflector(1,i).y0;
            end
            sector_or=a;
            midpoint=find_midpoint(a, anzahl_reflektoren);
            imshow(bin_frame);
            while(flag)   %Einstellen der Helligkeit
                clc;
                disp('Aufnahme des oberen rechten Sektors erstellen.');
                disp('Drücken Sie "n" um eine neue Aufnahme zu machen.');
                disp('Drücken Sie "r", wenn Sie fertig sind.');
                disp(['Anzahl der Reflektoren: ',num2str(anzahl_reflektoren)]);
                disp('Reflektorporitionen des Reflektors auf dem Mittelpunkt: ');
                disp(num2str(midpoint));
                if shutter==1
                    trigger(vid);   %Triggern
                    while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
                    end
                    frame = getdata(vid,1); %Bild speichern
                    bin_frame = transform_frame(frame); %Binäres Bild erstellen
                    bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
                    [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
                    reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
                    a=zeros(anzahl_reflektoren,2);
                    for i=1 : anzahl_reflektoren
                        a(i,1)=reflector(1,i).x0;
                        a(i,2)=reflector(1,i).y0;
                    end
                    sector_or=a;
                    midpoint=find_midpoint(a, anzahl_reflektoren);
                    shutter=0;
                end
                imshow(bin_frame);
                pause(0.1); %Kurze Pause
            end
            stop(vid);
            
        case 4
            flag=1;
            shutter=0;
            start(vid); %Aktivieren des Video Input Objektes
            pause(0.1); %Kurze Pause
            trigger(vid);   %Triggern
            while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
            end
            frame = getdata(vid,1); %Bild speichern
            bin_frame = transform_frame(frame); %Binäres Bild erstellen
            bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
            [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
%             reflector(1:anzahl_reflektoren)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
            reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
            a=zeros(anzahl_reflektoren,2);
            for i=1 : anzahl_reflektoren
                a(i,1)=reflector(1,i).x0;
                a(i,2)=reflector(1,i).y0;
            end
            sector_ur=a;
            midpoint=find_midpoint(a, anzahl_reflektoren);
            imshow(bin_frame);
            while(flag)   %Einstellen der Helligkeit
                clc;
                disp('Aufnahme des unteren rechten Sektors erstellen.');
                disp('Drücken Sie "n" um eine neue Aufnahme zu machen.');
                disp('Drücken Sie "r", wenn Sie fertig sind.');
                disp(['Anzahl der Reflektoren: ',num2str(anzahl_reflektoren)]);
                disp('Reflektorporitionen des Reflektors auf dem Mittelpunkt: ');
                disp(num2str(midpoint));
                if shutter==1
                    trigger(vid);   %Triggern
                    while (vid.FramesAvailable == 0)    %Warten bis Bild vorhanden
                    end
                    frame = getdata(vid,1); %Bild speichern
                    bin_frame = transform_frame(frame); %Binäres Bild erstellen
                    bounds_hilfe=bwboundaries(bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
                    [bounds,anzahl_reflektoren] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
                    reflector = get_pixel_position(bounds, anzahl_reflektoren); %Pixelposition ermitteln
                    a=zeros(anzahl_reflektoren,2);
                    for i=1 : anzahl_reflektoren
                        a(i,1)=reflector(1,i).x0;
                        a(i,2)=reflector(1,i).y0;
                    end
                    sector_ur=a;
                    midpoint=find_midpoint(a, anzahl_reflektoren);
                    shutter=0;
                end
                imshow(bin_frame);
                pause(0.1); %Kurze Pause
            end
            stop(vid);
            
        case 5  %Prüfen ob die Sektorne-Matrizen die richtige Größe haben
            def=48;
            help=size(sector_ol);
            if help(1,1)~=def
                disp('Aufnahme des oberen linken Sektors ist ungültig.');
                auswahl=0;
                pause(0.5);
                break;
            end
            help=size(sector_ul);
            if help(1,1)~=def
                disp('Aufnahme des unteren linken Sektors ist ungültig.');
                auswahl=0;
                pause(0.5);
                break;
            end
            help=size(sector_or);
            if help(1,1)~=def
                disp('Aufnahme des oberen rechten Sektors ist ungültig.');
                auswahl=0;
                pause(0.5);
                break;
            end
            help=size(sector_ur);
            if help(1,1)~=def
                disp('Aufnahme des unteren rechten Sektors ist ungültig.');
                auswahl=0;
                pause(0.5);
                break;
            end
            break;
        otherwise
            disp('Ungültige Eingabe');
            pause(0.5); %Kurze Pause
    end
end
delete(fig);

%--------------------------------------------------------------------------
help=input('Geben Sie die x-Koordinate in mm des Mittekpunktes an: ');

xmm=zeros(11,15);
for i=1 : 11
    wert=help-7*200;   %1820mm - 7*200mm für Cam1  4263 - 7*200 für Cam2
    for j=1 : 15
        xmm(i,j)=wert;
        wert=wert+200;
    end
end

help=input('Geben Sie die y-Koordinate in mm des Mittekpunktes an: ');

ymm=zeros(11,15);
for i=1 : 15
    wert=help+5*200;   %1291mm + 5*200mm für Cam1   1232 + 5*200 für Cam2
    for j=1 : 11
        ymm(j,i)=wert;
        wert=wert-200;
    end
end
xpix=zeros(11,15);
ypix=zeros(11,15);
% Messwertaufbereitung
sector_ol_sort=sortrows(sector_ol,1);   %aufsteigend nach x sortieren
sector_ul_sort=sortrows(sector_ul,1);   %aufsteigend nach x sortieren
sector_or_sort=sortrows(sector_or,1);   %aufsteigend nach x sortieren
sector_ur_sort=sortrows(sector_ur,1);   %aufsteigend nach x sortieren

sector_ol_sort=corection( sector_ol_sort );   %y-werte für die verschiedenen x-werte absteigend sortieren
sector_ul_sort=corection( sector_ul_sort );   %y-werte für die verschiedenen x-werte absteigend sortieren
sector_or_sort=corection( sector_or_sort );   %y-werte für die verschiedenen x-werte absteigend sortieren
sector_ur_sort=corection( sector_ur_sort );   %y-werte für die verschiedenen x-werte absteigend sortieren

% Zusammenfügen der Messwerte
j=1;
k=1;
for i=1 : 15    %Zusammenfügen der Bilder und mitteln der mehrfachen Reflektoren
    if i<8
        xpix(1:5,i)=sector_ol_sort(j:j+4,1);
        ypix(1:5,i)=sector_ol_sort(j:j+4,2);
        xpix(6,i)=( sector_ol_sort(j+5,1) + sector_ul_sort(j,1) ) /2;
        ypix(6,i)=( sector_ol_sort(j+5,2) + sector_ul_sort(j,2) ) /2;
        xpix(7:11,i)=sector_ul_sort(j+1:j+5,1);
        ypix(7:11,i)=sector_ul_sort(j+1:j+5,2);
        j=j+6;
    elseif i==8
        xpix(1:5,i)=( sector_ol_sort(j:j+4,1) + sector_or_sort(k:5,1) )/2;
        ypix(1:5,i)=( sector_ol_sort(j:j+4,2) + sector_or_sort(k:5,2) )/2;
        xpix(6,i)=( sector_ol_sort(j+5,1) + sector_ul_sort(j,1) + sector_or_sort(k+5,1) + sector_ur_sort(k,1) ) /4;
        ypix(6,i)=( sector_ol_sort(j+5,2) + sector_ul_sort(j,2) + sector_or_sort(k+5,2) + sector_ur_sort(k,2) ) /4;
        xpix(7:11,i)=( sector_ul_sort(j+1:j+5,1) + sector_ur_sort(k+1:k+5,1) )/2;
        ypix(7:11,i)=( sector_ul_sort(j+1:j+5,2) + sector_ur_sort(k+1:k+5,2) )/2;
        k=k+6;
    elseif i>8
        xpix(1:5,i)=sector_or_sort(k:k+4,1);
        ypix(1:5,i)=sector_or_sort(k:k+4,2);
        xpix(6,i)=( sector_or_sort(k+5,1) + sector_ur_sort(k,1) ) /2;
        ypix(6,i)=( sector_or_sort(k+5,2) + sector_ur_sort(k,2) ) /2;
        xpix(7:11,i)=sector_ur_sort(k+1:k+5,1);
        ypix(7:11,i)=sector_ur_sort(k+1:k+5,2);
        k=k+6;
    end
    
end

disp('Speicher Sie die Matritzen xpix, ypix, xmm und ymm unter dem Namen "CamXDataRxxx".');