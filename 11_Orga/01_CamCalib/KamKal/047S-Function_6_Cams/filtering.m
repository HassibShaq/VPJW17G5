%--------------------------------------------------------------------------
% Sortiert zu kleine Reflektoren aus.
%--------------------------------------------------------------------------
function [ bounds,anzahl_objekte ] = filtering( bounds_hilfe )
%Zu kleine Reflektionsfl‰chen werden Herausgefiltert
anzahl_objekte=0;
bound_size = size(bounds_hilfe);
for i=1 : bound_size(1) %Z‰hlen wie viele Reflektoren es gibt
    groesse = size(bounds_hilfe{i}(:,1));
    groesse = groesse(1);
    if groesse >= 9  %Reflektionsfl‰chen die zu klein sind werden herausgefiltert
        anzahl_objekte=anzahl_objekte+1;   %Anzahl weiﬂer Objekte bestimmen
    end
end
bounds=cell(0);
zaehler=1;
for i=1 : bound_size(1)
    groesse = size(bounds_hilfe{i}(:,1));
    groesse = groesse(1);
    if groesse >= 9  %Reflektionsfl‰chen die zu klein sind werden herausgefiltert
        bounds{zaehler}(:,:)=bounds_hilfe{i}(:,:);   %Anzahl weiﬂer Objekte bestimmen
        zaehler=zaehler+1;
    end
end
bounds=bounds';


end

