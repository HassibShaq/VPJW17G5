%--------------------------------------------------------------------------
% Diese Funktion dient zur Abfrage, ob eine bestimmte Taste gedrückt wurde.
% Diese Funktion verhält sich wie ein Interrupt.
%--------------------------------------------------------------------------

function bediener_value(src,evnt)

global flag;
global value;
if evnt.Character == 'r'    
    flag = 0;
elseif evnt.Character == '+'
    value=value+1;
elseif evnt.Character == '-' 
    value=value-1;
elseif evnt.Character == '#' 
    value=value+10;
elseif evnt.Character == '.' 
    value=value-10;
end

clear flag;           %Speicher fuer Zeiger auf globale Variablen
clear value;

end 
