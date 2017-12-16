%--------------------------------------------------------------------------
% Sortiert jeweils 6 Koordinaten absteigend nach y
%--------------------------------------------------------------------------
function [ matrix ] = corection( matrix )
    for i=1 : 8
        ende=0;
        while(ende~=1)
        von=i*6-5;
        bis=i*6;
        zaehler=0;
        for j=von : bis-1
            if matrix(j,2) > matrix(j+1,2)
                hilfe=matrix(j,1:2);
                matrix(j,1:2)=matrix(j+1,1:2);
                matrix(j+1,1:2)=hilfe;
                zaehler=zaehler+1;
            end
            if j==bis-1 && zaehler==0
                ende=1;
            end
        end
        end
    end
end

