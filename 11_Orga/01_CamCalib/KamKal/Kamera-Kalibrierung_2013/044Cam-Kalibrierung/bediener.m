%--------------------------------------------------------------------------
% Diese Funktion dient zur Abfrage, ob die Taste "r" gedrückt wurde. Diese
% Funktion verhält sich wie ein Interupt.
%--------------------------------------------------------------------------

function bediener(src,evnt)          
 global flag;
 if evnt.Character == 'r'
  flag = 0;                    %Programm beenden
 end

 clear flag;           %Speicher fuer Zeiger auf globale Variablen
end 
