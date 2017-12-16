%--------------------------------------------------------------------------
% Macht aus einem Bild ein digitales Bild
%--------------------------------------------------------------------------
function [bin_frame] = transform_frame(frame)

level=0.01; %Festlegen, wie hell die Bildpunkte sein müssen, um als weiß zu gelten. 0.99
bin_frame=im2bw(frame,level);   %Funktion, die aus einem Bild ein digitales macht.
%subplot(1,2,1)
%imshow(bin_frame);
