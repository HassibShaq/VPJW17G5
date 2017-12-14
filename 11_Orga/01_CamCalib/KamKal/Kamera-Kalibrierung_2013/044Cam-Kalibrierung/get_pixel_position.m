%--------------------------------------------------------------------------
% Bestimmt mit Hilfe der kleinsten Quadrate-Schätzung die Pixelposition der 
% Reflektoren und gibt diese als Struktur zurück.
%--------------------------------------------------------------------------
function [reflector] = get_pixel_position(bounds, anzahl_objekte)
%Vordefinieren der Reflektoren
reflector(1:anzahl_objekte)=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);

%   Schätzung eines Kreises
% for i=1 : anzahl_objekte
%     x=bounds{i}(:,2);
%     y=bounds{i}(:,1);
%     A=[2*x 2*y ones(size(x))];
%     C=-x.^2-y.^2;
%     def=A \ C;
%     reflector(i).x0=-def(1);
%     reflector(i).y0=-def(2);
%     a=size(bounds{i});  %Größe des Reflectors bestimmen
%     a=a(1);
%     reflector(i).size=a;  %Größe des Reflectors bestimmen
%     % r=sqrt(def(1)^2 + def(2)^2 - def(3));
% end

%   Schätzung einer Ellipse
for i=1 : anzahl_objekte
    x=bounds{i}(:,2);
    y=bounds{i}(:,1);
    % Urspr. Schätzgleichung: a*x^2 + 2*b*x*y + c*y^2 + d*x + e*y + f = 1 
    % Durch (1-f) dividiert:  as*x^2 + 2*bs*x*y + cs*y^2 + ds*x + es*y= 1
    A=[x.^2 2.*x.*y y.^2 2*x 2*y];
    B=ones(size(x));
    P= A\B;     % Errechnen der skalierten Parameter
    as = P(1);
    bs = P(2);
    cs = P(3);
    ds = P(4);  % Entspricht dem ichtigen Parameter e
    es = P(5);  % Entspricht dem ichtigen Parameter d
    reflector(i).x0 = -(ds*cs-es*bs)/(as*cs-bs^2); % Skalenfaktor kürzt sich hier
    reflector(i).y0 = -(es*as-ds*bs)/(as*cs-bs^2); % also schon richtig    
    a=size(bounds{i});  %Größe des Reflectors bestimmen
    a=a(1);
    reflector(i).size=a;  %Größe des Reflectors bestimmen
    
%     phi=(1/2)*atan(2*bs/(as-cs));  % Erechnen des Winkels in rad
% 
%     fs = as*x0^2+2*bs*x0*y0+cs*y0^2; % absoluter Term mit skalierten a,b,c
%     g = 1/(1+fs); % Skalenfaktor der Schätzung
% 
%     a = as*g;   % Errechnen der richtigen Parametern
%     b = bs*g;
%     c = cs*g;
%     d = ds*g;
%     e = es*g;
%     f = a*x0^2+2*b*x0*y0+c*y0^2;
end