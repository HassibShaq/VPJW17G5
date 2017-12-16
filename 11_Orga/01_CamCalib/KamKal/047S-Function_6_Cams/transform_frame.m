%--------------------------------------------------------------------------
% Macht aus einem Bild ein digitales Bild
%--------------------------------------------------------------------------

function [bin_frame] = transform_frame(frame)
%r=25/4.6875;
%R=30/4.6875;

% historgram=imhist(frame, 265);  %Häufigkeitsverteilung von Messwerten
% level = graythresh(frame);
level=0.01;
bin_frame=im2bw(frame,level);

%subplot(1,2,1)
%imshow(bin_frame);
