clear all;
clc;
load sollbahn1;
% load binframe;
load Cam1DataR484_c;
load Cam2DataR484_c;
tic

target_track=1;
hr=222.5;
cam=[struct('x0',1820,'y0',1291,'h',2885,'xmin',nan,'xmax',3160,'ymin',nan,'ymax',nan,'xpix',xpix1,'ypix',ypix1,'xmm',xmm1,'ymm',ymm1)...
     struct('x0',4262,'y0',1232,'h',2885,'xmin',2910,'xmax',nan,'ymin',nan,'ymax',nan,'xpix',xpix2,'ypix',ypix2,'xmm',xmm2,'ymm',ymm2)];

frame=[struct('frame',frame1) struct('frame',frame2)];

anzahl_cam=size(frame);
anzahl_cam=anzahl_cam(1,2);
f=false(480, 640);
bin_frame(1:anzahl_cam)=struct('bin_frame',f);
for i=1 : anzahl_cam
    bin_frame(1,i).bin_frame = transform_frame(frame(1,i).frame); %Binäres Bild erstellen
end
% anzahl_cam=size(bin_frame);
% anzahl_cam=anzahl_cam(1,2);

bounds(1:anzahl_cam)=struct('bounds',[]);
anzahl_reflectoren(1:anzahl_cam)=0;
for i=1 : anzahl_cam
    bounds_hilfe=bwboundaries(bin_frame(1,i).bin_frame,4,'noholes');   %Ermitteln der äußeren Grenzen von weißen Flächen
    [bounds(1,i).bounds,anzahl_reflectoren(1,i)] = filtering(bounds_hilfe);  %Zu kleine Reflektionsflächen werden Herausgefiltert
end

%Mittelpunkte der Reflektoren in pixel bestimmen
reflector(1:sum(anzahl_reflectoren))=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
zaehler1=1;
zaehler2=0;
for i=1 : anzahl_cam
    zaehler2=zaehler2+anzahl_reflectoren(1,i);
    if anzahl_reflectoren(1,i) > 1
        reflector(zaehler1:zaehler2) = get_pixel_position(bounds(1,i).bounds, anzahl_reflectoren(1,i));
    end
    zaehler1=zaehler1+anzahl_reflectoren(1,i);
end

if target_track < 0.5
    [ ref_ges anzahl_ref_ges ] = get_mm_position( reflector, anzahl_reflectoren, cam, anzahl_cam, hr );
    if anzahl_ref_ges < 2
        a(1:24)=0;
    else
        [grosse_ref, anzahl_grosse_ref] = find_the_biggest(ref_ges, anzahl_ref_ges); %Ermitteln der großen Reflektoren
        robot = build_robots( grosse_ref, anzahl_grosse_ref, anzahl_ref_ges, ref_ges ); %Erstellen von Robotern

        j=1;
        a(1:24)=0;
        for i = 1 : 8   %Werte auf Ausgangsvector schreiben
            a(j)=robot(1,i).xpos;
            a(j+1)=robot(1,i).ypos;
            a(j+2)=robot(1,i).theta;
            j=j+3;
        end
    end
elseif target_track >= 0.5
    a(1:24)=0;
    [ ref_ges anzahl_ref_ges ] = get_mm_position( reflector, anzahl_reflectoren, cam, anzahl_cam, 0 );
    j=1;
    checkpoint(1:10*2)=0;
    x(1:anzahl_ref_ges)=0;
    y(1:anzahl_ref_ges)=0;
    for i = 1 : anzahl_ref_ges   %Werte auf Ausgangsvector schreiben
            checkpoint(j)=ref_ges(1,i).x0;
            checkpoint(j+1)=ref_ges(1,i).y0;
            j=j+2;
            x(i)=ref_ges(1,i).x0;
            y(i)=ref_ges(1,i).y0;
    end
    xx = min(x):0.25:max(x);
    yy = interp1(x,y,xx,'pchip');
    plot(x,y,'o',xx,yy)
end

toc