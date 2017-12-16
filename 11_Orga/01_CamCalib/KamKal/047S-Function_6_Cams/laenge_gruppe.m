function [ laenge ] = laenge_gruppe( gruppe )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
groesse=size(gruppe);
laenge=0;
for i = 1 : groesse(1)
    if (gruppe(i)>0)
        laenge=laenge+1;
    end
end

end