function [ b ] = diagnostics( reflector, anzahl_reflectoren, anzahl_cam , robot)

    position_in_cam=NaN(3,anzahl_cam+1);
    position_in_cam(:,anzahl_cam+1)=[robot(1,1).xpos robot(1,1).ypos 0]';
    averaged_position=[robot(1,1).xpos robot(1,1).ypos];

    zaehler1=1;
    zaehler2=0;
    for i=1 : anzahl_cam
        zaehler2=zaehler2+anzahl_reflectoren(i);
        for j=zaehler1 : zaehler2
            r=sqrt( (reflector(1,j).x0 - averaged_position(1))^2 + (reflector(1,j).y0 - averaged_position(2))^2 );
            if r < 45
                position_in_cam(1,i)=reflector(1,j).x0;
                position_in_cam(2,i)=reflector(1,j).y0;
                position_in_cam(3,i)=r;
            end
        end
        zaehler1=zaehler1+anzahl_reflectoren(i);
    end

    b=reshape(position_in_cam,1,(anzahl_cam+1)*3);

end