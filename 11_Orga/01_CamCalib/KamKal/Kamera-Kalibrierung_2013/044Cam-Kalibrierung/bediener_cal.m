%--------------------------------------------------------------------------
% Diese Funktion dient zur Abfrage, ob eine bestimmte Taste gedrückt wurde.
% Diese Funktion verhält sich wie ein Interrupt.
%--------------------------------------------------------------------------

function bediener_cal(src,evnt)          
 global flag;
 global shutter;
 if evnt.Character == 'r'
  flag = 0;                    %Programm beenden
 elseif evnt.Character == 'n'
  shutter = 1;                    %Programm Trigger auf 1 setzen
 end

 clear flag;           %Speicher fuer Zeiger auf globale Variablen
 clear shutter;
end 
