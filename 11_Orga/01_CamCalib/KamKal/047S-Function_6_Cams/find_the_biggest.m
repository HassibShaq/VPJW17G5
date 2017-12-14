function [ grosse_ref,zaehler_grosse_ref ] = find_the_biggest( reflector, anzahl_objekte )
%Ermitteln der großen Reflektoren
abstand=zeros(anzahl_objekte-1,anzahl_objekte);
gruppe=zeros(anzahl_objekte,anzahl_objekte);
anzahl_gruppen=1;
zeile=1;
for i = 1 : anzahl_objekte-1 %Abstandsmatrix erstellen
    for j = i+1 : anzahl_objekte
        abstand(i,j)=sqrt( (reflector(j).x0-reflector(i).x0)^2 + (reflector(j).y0-reflector(i).y0)^2 );
    end
end
for i = 1 : anzahl_objekte-1 %Reflektoren zu Gruppen zusammenfügen 
    if zeile > 2 %nächste Gruppe bilden, wenn in der aktuellen Gruppe an zweiter 
        anzahl_gruppen=anzahl_gruppen+1;    %Stelle an der ein Reflektor eingetragen ist
    else    %den ertsen Reflektor wieder aus der Gruppe entfernen
        gruppe(zeile,anzahl_gruppen)=0; 
    end
    zeile=1;
    gruppe(zeile,anzahl_gruppen)=i;
    
    zeile=zeile+1;    
    for j = i+1 : anzahl_objekte
        if abstand(i,j) > 0 && abstand(i,j) <= 210   %Abstand kleiner 210mm
            gruppe(zeile,anzahl_gruppen)=j; %zuweisen in eine Gruppe
            zeile=zeile+1;
        end
    end
end
if anzahl_gruppen >= 2  %wenn es mehr als eine Gruppe gibt
    for i = 1 : anzahl_gruppen-1  %Zählt die Gruppe 1
        for k = i+1 : anzahl_gruppen     %Zählt die Gruppe 2
            for j = 1 : laenge_gruppe(gruppe(:,i))    %Zählt die Zeilen der Gruppe 1
                for l = 1 : laenge_gruppe(gruppe(:,k))   %Zählt die Zeilen der Gruppe 2
                    if gruppe(j,i)==gruppe(l,k) %Wenn ein Reflektor in beiden Gruppen vorkommt
                        %-  Füge Werte der Gruppe 2 an die Werte der
                        %   Gruppe 1 an.
                        a=laenge_gruppe(gruppe(:,i))+1;
                        b=laenge_gruppe(gruppe(:,i))+laenge_gruppe(gruppe(:,k));
                        c=laenge_gruppe(gruppe(:,k));
                        gruppe(a : b, i)=gruppe(1:c,k); 
                        %-  Setze Werte der Gruppe 2 auf Null
                        gruppe(:,k)=0;

                        break;
                    end
                end
            end
        end
    end
end
anzahl_gruppen2=0;  %Neue Anzahl von Gruppen ermitteln
for i=1: anzahl_objekte
    if gruppe(1,i) > 0
        anzahl_gruppen2=anzahl_gruppen2+1;
    end
end
if min(gruppe(1,1:anzahl_gruppen2))==0  %Null-Gruppen finden und herauslöschen
    for i = 1 : anzahl_gruppen-1
        if gruppe(1,i)==0
            for j = i+1 : anzahl_gruppen    %Nächste Gruppe mit Werten finden
                if gruppe(1,j)>0
                    gruppe(:,i)=gruppe(:,j);    %In Nullgruppe einfügen
                    gruppe(:,j)=0;  %Gruppe aus der eingefügt löschen
                    break;
                end
            end
        end
    end
end
for u = 1 : anzahl_gruppen2   %Doppelte Werte aus einer Gruppe löschen
    a=laenge_gruppe(gruppe(:,u));
    for v = 1 : a-1
        for w = v+1 : a
            if gruppe(v,u)==gruppe(w,u) %Wenn ein Wert doppelt vorhanden 
                gruppe(w,u)=0;  %Wert einmal löschen
            end
        end
    end
    for i=1 : a-1   %Null-Werte aus Gruppen herauslöschen
        if gruppe(i,u)==0
            for j=i+1 : a   %Nächsten Wert in der Gruppe finden
                if gruppe(j,u)>0    
                    gruppe(i,u)=gruppe(j,u);
                    gruppe(j,u)=0;
                    break;
                end
            end
        end
    end
end
%Große Reflektoren aus jeder Gruppe herausfinden
toleranz=7;    %Toleranz in Pixel um Mittleren Reflektor
grosse_ref(anzahl_objekte)=0;
zaehler_grosse_ref=0;
for i = 1 : anzahl_gruppen2    %Gruppe in der gesucht wird
    a=laenge_gruppe(gruppe(:,i));
    groesster=0;
    for j = 1 : a   %Den größten Reflektor der Gruppe finden
        if reflector(gruppe(j,i)).size >= groesster
            groesster=reflector(gruppe(j,i)).size;
        end
    end
    for j = 1 : a   %Die Großen Reflektoren der Gruppen finden
        if reflector(gruppe(j,i)).size >= groesster-toleranz
            zaehler_grosse_ref=zaehler_grosse_ref+1;
            grosse_ref(zaehler_grosse_ref)=gruppe(j,i);
        end
    end    
end


end

