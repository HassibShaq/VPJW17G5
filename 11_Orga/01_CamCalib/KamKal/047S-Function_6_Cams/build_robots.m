function [ robot ] = build_robots( grosse_ref, anzahl_grosse_ref, anzahl_reflectoren, reflector )
%Reflektoren zu Robotern zusammenf¸gen

a=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0,'averaged',0);
robot(1)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a]);

% robot(2:4)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a]);
robot(2)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a]);
robot(3)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a]); % neue Nr.3 (vorher Nr.6)
robot(4)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a]);

% robot(5:7)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a]);
robot(5)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a]);
robot(6)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a]);
robot(7)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a]);

robot(8)=struct('xpos',0,'ypos',0,'theta',0,'reflector',[a a a a a]);
zaehler=1;
for i = 1 : anzahl_grosse_ref
    gruppe(1:anzahl_reflectoren)=0;  %Vordefinieren einer Gruppe. Maximale Anzahl von Reflektoren 5
    gruppe(1)=grosse_ref(i);    %Den groﬂen Reflektor an die erste Stelle der Gruppe schreiben
    zaehler=zaehler+1;
    for j = 1 : anzahl_reflectoren   %Kleinere Reflektoren dem Groﬂen zuordnen
        abstand=sqrt( (reflector(j).x0-reflector(grosse_ref(i)).x0)^2 + (reflector(j).y0-reflector(grosse_ref(i)).y0)^2 );
        if abstand < 115 && abstand > 0     %115mm
            gruppe(zaehler)=j;
            zaehler=zaehler+1;
        end
    end
    
    if laenge_gruppe(gruppe') == 2  %Hat der Roboter 2 Reflekroren
        robot(1).xpos=reflector(gruppe(1)).x0;
        robot(1).ypos=reflector(gruppe(1)).y0;
%         robot(1).xpos=griddata( xpix, ypix, xmm, reflector(gruppe(1)).x0, reflector(gruppe(1)).y0 );
%         robot(1).ypos=griddata( xpix, ypix, ymm, reflector(gruppe(1)).x0, reflector(gruppe(1)).y0 );
        robot(1).theta=winkel(reflector(gruppe(1)).x0, reflector(gruppe(2)).x0, reflector(gruppe(1)).y0, reflector(gruppe(2)).y0);
        robot(1).reflector(1)=reflector(gruppe(1));
        robot(1).reflector(2)=reflector(gruppe(2));
    else    %Hat der Roboter mehrere Reflekroren
        grosse=0;   %Den Mittelgroﬂen Reflektor finden
        gruppe_position=0;
        for k = 2 : laenge_gruppe(gruppe')
            if grosse < reflector(gruppe(k)).size
                grosse=reflector(gruppe(k)).size;
                gruppe_position=k;
            end
        end
        if gruppe_position > 2    %Mittelgroﬂen Reflektor an 2. Stelle Bringen
            hilfe=gruppe(2);
            gruppe(2)=gruppe(gruppe_position);
            gruppe(gruppe_position)=hilfe;
        end
        if laenge_gruppe(gruppe') == 3  %Hat der Roboter 3 Reflekroren
            theta=winkel(reflector(gruppe(1)).x0, reflector(gruppe(2)).x0, reflector(gruppe(1)).y0, reflector(gruppe(2)).y0);
            winkel_ref1=winkel(reflector(gruppe(1)).x0, reflector(gruppe(3)).x0, reflector(gruppe(1)).y0, reflector(gruppe(3)).y0)-theta;
            if winkel_ref1 < 0
                winkel_ref1 = winkel_ref1+360;
            end
            if winkel_ref1 > 70 && winkel_ref1 < 110    %Fall des 2. Roboters
                robot(2).xpos=reflector(gruppe(1)).x0;
                robot(2).ypos=reflector(gruppe(1)).y0;
                robot(2).theta=theta;
                robot(2).reflector(1)=reflector(gruppe(1));
                robot(2).reflector(2)=reflector(gruppe(2));
                robot(2).reflector(3)=reflector(gruppe(3));
                robot(2).reflector(3).position_in_grad=winkel_ref1;
%             elseif winkel_ref1 > 160 && winkel_ref1 < 200   %Fall des 3. Roboters (alt)
%                 robot(3).xpos=reflector(gruppe(1)).x0;
%                 robot(3).ypos=reflector(gruppe(1)).y0;
%                 robot(3).theta=theta;
%                 robot(3).reflector(1)=reflector(gruppe(1));
%                 robot(3).reflector(2)=reflector(gruppe(2));
%                 robot(3).reflector(3)=reflector(gruppe(3));
%                 robot(3).reflector(3).position_in_grad=winkel_ref1;
            elseif winkel_ref1 > 250 && winkel_ref1 < 290   %Fall des 4. Roboters
                robot(4).xpos=reflector(gruppe(1)).x0;
                robot(4).ypos=reflector(gruppe(1)).y0;
                robot(4).theta=theta;
                robot(4).reflector(1)=reflector(gruppe(1));
                robot(4).reflector(2)=reflector(gruppe(2));
                robot(4).reflector(3)=reflector(gruppe(3));
                robot(4).reflector(3).position_in_grad=winkel_ref1;
            end
        elseif laenge_gruppe(gruppe') == 4  %Hat der Roboter 4 Reflekroren
            theta=winkel(reflector(gruppe(1)).x0, reflector(gruppe(2)).x0, reflector(gruppe(1)).y0, reflector(gruppe(2)).y0);
            winkel_ref1=winkel(reflector(gruppe(1)).x0, reflector(gruppe(3)).x0, reflector(gruppe(1)).y0, reflector(gruppe(3)).y0)-theta;
            winkel_ref2=winkel(reflector(gruppe(1)).x0, reflector(gruppe(4)).x0, reflector(gruppe(1)).y0, reflector(gruppe(4)).y0)-theta;
            if winkel_ref1 < 0
                winkel_ref1 = winkel_ref1+360;
            end
            if winkel_ref2 < 0
                winkel_ref2 = winkel_ref2+360;
            end
            if winkel_ref2 < winkel_ref1    %Die Identifikationsreflektoren sortieren
            hilfe=gruppe(3);
            gruppe(3)=gruppe(4);
            gruppe(4)=hilfe;
            hilfe=winkel_ref1;
            winkel_ref1=winkel_ref2;
            winkel_ref2=hilfe;
            end
            if winkel_ref1 > 70 && winkel_ref1 < 110 && winkel_ref2 > 160 && winkel_ref2 < 200   %Fall des 5. Roboters
                robot(5).xpos=reflector(gruppe(1)).x0;
                robot(5).ypos=reflector(gruppe(1)).y0;
                robot(5).theta=theta;
                robot(5).reflector(1)=reflector(gruppe(1));
                robot(5).reflector(2)=reflector(gruppe(2));
                robot(5).reflector(3)=reflector(gruppe(3));
                robot(5).reflector(3).position_in_grad=winkel_ref1;
                robot(5).reflector(4)=reflector(gruppe(4));
                robot(5).reflector(4).position_in_grad=winkel_ref2;
            elseif winkel_ref1 > 70 && winkel_ref1 < 110 && winkel_ref2 > 250 && winkel_ref2 < 290   %Fall des 6. Roboters
                % neue Nr.3 (ehemals Nr.6)
                robot(3).xpos=reflector(gruppe(1)).x0;
                robot(3).ypos=reflector(gruppe(1)).y0;
                robot(3).theta=theta;
                robot(3).reflector(1)=reflector(gruppe(1));
                robot(3).reflector(2)=reflector(gruppe(2));
                robot(3).reflector(3)=reflector(gruppe(3));
                robot(3).reflector(3).position_in_grad=winkel_ref1;
                robot(3).reflector(4)=reflector(gruppe(4));
                robot(3).reflector(4).position_in_grad=winkel_ref2;
                
                % Nr.6 (werden Nullen in die Positionen reingeschrieben, bei Init in Zeile 14)
%                 robot(6).xpos=reflector(gruppe(1)).x0;
%                 robot(6).ypos=reflector(gruppe(1)).y0;
%                 robot(6).theta=theta;
%                 robot(6).reflector(1)=reflector(gruppe(1));
%                 robot(6).reflector(2)=reflector(gruppe(2));
%                 robot(6).reflector(3)=reflector(gruppe(3));
%                 robot(6).reflector(3).position_in_grad=winkel_ref1;
%                 robot(6).reflector(4)=reflector(gruppe(4));
%                 robot(6).reflector(4).position_in_grad=winkel_ref2;
            elseif winkel_ref1 > 160 && winkel_ref1 < 200 && winkel_ref2 > 250 && winkel_ref2 < 290   %Fall des 7. Roboters
                robot(7).xpos=reflector(gruppe(1)).x0;
                robot(7).ypos=reflector(gruppe(1)).y0;
                robot(7).theta=theta;
                robot(7).reflector(1)=reflector(gruppe(1));
                robot(7).reflector(2)=reflector(gruppe(2));
                robot(7).reflector(3)=reflector(gruppe(3));
                robot(7).reflector(3).position_in_grad=winkel_ref1;
                robot(7).reflector(4)=reflector(gruppe(4));
                robot(7).reflector(4).position_in_grad=winkel_ref2;
            end
        elseif laenge_gruppe(gruppe') == 5  %Hat der Roboter 5 Reflekroren
            theta=winkel(reflector(gruppe(1)).x0, reflector(gruppe(2)).x0, reflector(gruppe(1)).y0, reflector(gruppe(2)).y0);
            winkel_ref1=winkel(reflector(gruppe(1)).x0, reflector(gruppe(3)).x0, reflector(gruppe(1)).y0, reflector(gruppe(3)).y0)-theta;
            winkel_ref2=winkel(reflector(gruppe(1)).x0, reflector(gruppe(4)).x0, reflector(gruppe(1)).y0, reflector(gruppe(4)).y0)-theta;
            winkel_ref3=winkel(reflector(gruppe(1)).x0, reflector(gruppe(5)).x0, reflector(gruppe(1)).y0, reflector(gruppe(5)).y0)-theta;
            if winkel_ref1 < 0
                winkel_ref1 = winkel_ref1+360;
            end
            if winkel_ref2 < 0
                winkel_ref2 = winkel_ref2+360;
            end
            if winkel_ref3 < 0
                winkel_ref3 = winkel_ref3+360;
            end
            if winkel_ref2 < winkel_ref1   %Die Identifikationsreflektoren sortieren
                hilfe=gruppe(3);
                gruppe(3)=gruppe(4);
                gruppe(4)=hilfe;
            end
            if winkel_ref3 < winkel_ref1
                hilfe=gruppe(3);
                gruppe(3)=gruppe(5);
                gruppe(5)=hilfe;
            end
            if winkel_ref3 < winkel_ref2
                hilfe=gruppe(4);
                gruppe(4)=gruppe(5);
                gruppe(5)=hilfe;
            end
            robot(8).xpos=reflector(gruppe(1)).x0;
            robot(8).ypos=reflector(gruppe(1)).y0;
            robot(8).theta=theta;
            robot(8).reflector(1)=reflector(gruppe(1));
            robot(8).reflector(2)=reflector(gruppe(2));
            robot(8).reflector(3)=reflector(gruppe(3));
            robot(8).reflector(3).position_in_grad=winkel_ref1;
            robot(8).reflector(4)=reflector(gruppe(4));
            robot(8).reflector(4).position_in_grad=winkel_ref2;
            robot(8).reflector(5)=reflector(gruppe(5));
            robot(8).reflector(5).position_in_grad=winkel_ref3;
        end 
    end
    zaehler=1;
end
end

