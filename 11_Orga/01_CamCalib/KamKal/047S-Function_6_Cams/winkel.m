function [ theta ] = winkel( x1,x2,y1,y2 )
            x=x2-x1;
            y=y2-y1;
            if x > 0 && y > 0   %Quadrant 1
                theta=atand(y/x);
            elseif x < 0 && y > 0   %Quadrant 2
                theta=180+atand(y/x);
            elseif x < 0 && y < 0   %Quadrant 3
                theta=180+atand(y/x);
            elseif x > 0 && y < 0   %Quadrant 4
                theta=360+atand(y/x);
            elseif x > 0 && y == 0
                theta=0;
            elseif x == 0 && y > 0
                theta=90;
            elseif x < 0 && y == 0
                theta=180;
            elseif x == 0 && y < 0
                theta=270;
            end
end