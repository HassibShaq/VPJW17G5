%--------------------------------------------------------------------------
% Ermittelt die dichtesten Koordinaten zum Bildmittelpunkt und gibt diese 
% zurück.
%--------------------------------------------------------------------------
function [ output ] = find_midpoint( a,anzahl_reflektoren )
distance=100;
x0=0;
y0=0;
for i=1 : anzahl_reflektoren    %Ermitteln der dichtesten Koordinaten zum Bildmittelpunkt
    r=sqrt( (a(i,1)-320)^2 + (a(i,2)-240)^2 );
    if r<distance
        distance=r;
        x0=a(i,1);
        y0=a(i,2);
    end
end
output=[x0 y0];
end

