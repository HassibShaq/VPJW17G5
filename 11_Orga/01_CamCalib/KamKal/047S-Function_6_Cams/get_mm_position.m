function [ ref_ges anzahl_ref_ges reflector] = get_mm_position( reflector, anzahl_reflectoren, cam, anzahl_cam, hr )
%Mittelpunkte der Reflektoren in mm bestimmen
zaehler1=1;
zaehler2=0;
ax=zeros(1,sum(anzahl_reflectoren));
ay=zeros(1,sum(anzahl_reflectoren));
xrobot=zeros(1,sum(anzahl_reflectoren));
yrobot=zeros(1,sum(anzahl_reflectoren));
for j=1 : sum(anzahl_reflectoren)
    ax(1,j)=reflector(1,j).x0;
    ay(1,j)=reflector(1,j).y0;
end
for i=1 : anzahl_cam
    zaehler2=zaehler2+anzahl_reflectoren(1,i);
    if anzahl_reflectoren(1,i)>=1
%         if anzahl_reflectoren(1,i) > 1
            xrobot(1,zaehler1:zaehler2) = griddata(cam(1,i).xpix,cam(1,i).ypix,cam(1,i).xmm,ax(1,zaehler1:zaehler2),ay(1,zaehler1:zaehler2),'v4');
            yrobot(1,zaehler1:zaehler2) = griddata(cam(1,i).xpix,cam(1,i).ypix,cam(1,i).ymm,ax(1,zaehler1:zaehler2),ay(1,zaehler1:zaehler2),'v4');
            xrobot(1,zaehler1:zaehler2) = (xrobot(1,zaehler1:zaehler2)-cam(1,i).x0)*(1-hr/cam(1,i).h)+cam(1,i).x0;  %Höhenkorrektur
            yrobot(1,zaehler1:zaehler2) = (yrobot(1,zaehler1:zaehler2)-cam(1,i).y0)*(1-hr/cam(1,i).h)+cam(1,i).y0;
%         end
    end
    zaehler1=zaehler1+anzahl_reflectoren(1,i);
end
%mm-Werte auf Reflektoren schreiben
for i=1 : sum(anzahl_reflectoren)
    reflector(1,i).x0=xrobot(1,i);
    reflector(1,i).y0=yrobot(1,i);
end
%Ungültige Messwerte löschen
zaehler1=1;
zaehler2=0;
for i=1 : anzahl_cam
    zaehler2=zaehler2+anzahl_reflectoren(1,i);
    for j=zaehler1 : zaehler2
        %Ist xref größer als xmin der cam oder kein xmin
        %der cam angegeben und ist xref kleiner als xmax der
        %cam oder kein xmmax der cam angegeben
        if (reflector(1,j).x0 < cam(1,i).xmin && ~isnan(cam(1,i).xmin))||...
           (reflector(1,j).x0 > cam(1,i).xmax && ~isnan(cam(1,i).xmax))||...
           (reflector(1,j).y0 < cam(1,i).ymin && ~isnan(cam(1,i).ymin))||...
           (reflector(1,j).y0 > cam(1,i).ymax && ~isnan(cam(1,i).ymax))
            reflector(1,j).x0=nan;
            reflector(1,j).y0=nan;
        end
    end
    zaehler1=zaehler1+anzahl_reflectoren(1,i);
end
reflector_m(1:sum(anzahl_reflectoren))=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0,'averaged',0);
%Doppelte Reflectoren mitteln und löschen
if anzahl_cam > 1   %Prüfen, ob minimal 2 Cams vorhanden
    zaehler1=1;
    zaehler2=0;
    for i=1 : anzahl_cam-1
        zaehler2=zaehler2+anzahl_reflectoren(1,i);
        for j=zaehler1 : zaehler2
            
            zaehler3=zaehler2+1;
            zaehler4=zaehler2;
            for k=i+1 : anzahl_cam
                %Position der Cams zueinander bestimmen
                position=winkel(cam(1,i).x0,cam(1,k).x0,cam(1,i).y0,cam(1,k).y0);
                zaehler4=zaehler4+anzahl_reflectoren(1,k);
                for l=zaehler3 : zaehler4
                    if ~isnan(reflector(1,j).x0)&&~isnan(reflector(1,l).x0) %Messwertgültigkeit prüfen
                        r=sqrt( (reflector(1,l).x0 - reflector(1,j).x0)^2 + (reflector(1,l).y0 - reflector(1,j).y0)^2 );
                        if r<45 %mm
                            if position > 345 || position < 15  %Vergleichskamera liegt rechts
                                %Faktor bestimmen
                                factor= ( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmin )*1/(cam(1,i).xmax-cam(1,k).xmin);
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;
                                
                            elseif position > 20  && position < 70  %Vergleichskamera liegt oben rechts
                                %Faktor bestimmen
                                factor= (( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                           (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmin )*1/(cam(1,i).xmax-cam(1,k).xmin)+...
                                         ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                           (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymin )*1/(cam(1,i).ymax-cam(1,k).ymin))/2;
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 75  && position < 105  %Vergleichskamera liegt oben
                                %Faktor bestimmen
                                factor= ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymin )*1/(cam(1,i).ymax-cam(1,k).ymin);
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 110 && position < 160  %Vergleichskamera liegt oben links
                                %Faktor bestimmen
                                factor= (( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmax )*1/(cam(1,i).xmin-cam(1,k).xmax)+...
                                        ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymin )*1/(cam(1,i).ymax-cam(1,k).ymin))/2;
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 165 && position < 195  %Vergleichskamera liegt links
                                %Faktor bestimmen
                                factor= ( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmax )*1/(cam(1,i).xmin-cam(1,k).xmax);
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 200 && position < 250  %Vergleichskamera liegt unten links
                                %Faktor bestimmen
                                factor= (( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmax )*1/(cam(1,i).xmin-cam(1,k).xmax)+...
                                        ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymax )*1/(cam(1,i).ymin-cam(1,k).ymax))/2;
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 255 && position < 285  %Vergleichskamera liegt unten
                                %Faktor bestimmen
                                factor= ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymax )*1/(cam(1,i).ymin-cam(1,k).ymax);
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            elseif position > 290 && position < 340  %Vergleichskamera liegt unten rechts
                                %Faktor bestimmen
                                factor= (( (reflector(1,l).valence*reflector(1,l).x0 + reflector(1,j).valence*reflector(1,j).x0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).xmin )*1/(cam(1,i).xmax-cam(1,k).xmin)+...
                                        ( (reflector(1,l).valence*reflector(1,l).y0 + reflector(1,j).valence*reflector(1,j).y0)/...
                                        (reflector(1,j).valence+reflector(1,l).valence) - cam(1,k).ymax )*1/(cam(1,i).ymin-cam(1,k).ymax))/2;
                                %Werte mitteln
                                reflector_m(1,j).x0= (reflector_m(1,j).x0*(reflector_m(1,j).valence-1) + reflector(1,j).x0*(1-factor)+ reflector(1,l).x0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).y0= (reflector_m(1,j).y0*(reflector_m(1,j).valence-1) + reflector(1,j).y0*(1-factor)+ reflector(1,l).y0*factor) /...
                                                      reflector_m(1,j).valence;
                                reflector_m(1,j).size=(reflector_m(1,j).size*(reflector_m(1,j).valence-1) + reflector(1,j).size*(1-factor)+reflector(1,l).size*factor)/...
                                                       reflector_m(1,j).valence;
                                reflector_m(1,j).valence=reflector(1,j).valence+1;
                                reflector(1,j).averaged=1;
                                reflector(1,l).averaged=1;

                            end
                        end
                    end
                end
                zaehler3=zaehler3+anzahl_reflectoren(1,k);
            end
            
        end
        zaehler1=zaehler1+anzahl_reflectoren(1,i);
    end
    
    %ungemittelte Reflektoren hinzufügen
    j=1;
    for i=1 : sum(anzahl_reflectoren)
        if reflector(1,i).averaged==0
            for j=j : sum(anzahl_reflectoren)
              if reflector_m(1,j).x0==0
                  reflector_m(1,j)=reflector(1,i);
                  break;
              end
            end
        end
    end
    for i=1 : sum(anzahl_reflectoren)
        if reflector_m(1,i).x0==0
            reflector_m(1,i).x0=nan;
            reflector_m(1,i).y0=nan;
        end
    end
    %Durschnitte der gleichen Reflektoren mitteln
    for i=1 : sum(anzahl_reflectoren)-1
        for j=i+1 : sum(anzahl_reflectoren)
            if ~isnan(reflector_m(1,i).x0)&&~isnan(reflector_m(1,j).x0) %Messwertgültigkeit prüfen
                r=sqrt( (reflector_m(1,i).x0 - reflector_m(1,j).x0)^2 + (reflector_m(1,i).y0 - reflector_m(1,j).y0)^2 );
                if r<45 %mm
                    reflector_m(1,i).x0=(reflector_m(1,i).x0*reflector_m(1,i).valence + reflector_m(1,j).x0*reflector_m(1,j).valence) /...
                                        (reflector_m(1,i).valence+reflector_m(1,j).valence);
                    reflector_m(1,i).y0=(reflector_m(1,i).y0*reflector_m(1,i).valence + reflector_m(1,j).y0*reflector_m(1,j).valence) /...
                                        (reflector_m(1,i).valence+reflector_m(1,j).valence);
                    reflector_m(1,i).size=(reflector_m(1,i).size*reflector_m(1,i).valence + reflector_m(1,j).size*reflector_m(1,j).valence) /...
                                          (reflector_m(1,i).valence+reflector_m(1,j).valence);
                    reflector_m(1,i).valence=reflector_m(1,i).valence+reflector_m(1,j).valence;
                    reflector_m(1,j).x0=nan;
                    reflector_m(1,j).y0=nan;
                end
            end
        end
    end
    %Anzahl übriger Reflektoren bestimmen
    anzahl_ref_ges=0;
    for i=1 : sum(anzahl_reflectoren)
        if ~isnan(reflector_m(1,i).x0)
            anzahl_ref_ges=anzahl_ref_ges+1;
        end
    end
    ref_ges(1:anzahl_ref_ges)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0,'averaged',0);
    zaehler=1;
    for i=1 : sum(anzahl_reflectoren)
        if ~isnan(reflector_m(1,i).x0)
            ref_ges(1,zaehler)=reflector_m(1,i);
            zaehler=zaehler+1;
        end
    end
else
    anzahl_ref_ges=sum(anzahl_reflectoren);
    ref_ges=reflector_m;
end


end

