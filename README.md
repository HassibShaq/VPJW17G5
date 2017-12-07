# VPJW17G5

## Softwareliste
Visual Studio 2015 Enterprise 32/64 Bit
-> DreamSpark

QT 5.9.3
> mscv2015 64-bit<br>
> Qt Creator 4.5.0

https://www1.qt.io/download-open-source/?hsCtaTracking=f977210e-de67-475f-a32b-65cec207fd03%7Cd62710cd-e1db-46aa-8d4d-2f1c1ffdacea

OpenCV 3.3.1 VC14<br>
https://opencv.org/releases.html

MySql Connector/C (libmysqlclient) 6.1.11 (64 Bit? Installer oder Zip)<br>
https://dev.mysql.com/downloads/connector/c/

GitHub Desktop Client
https://desktop.github.com/

## Installation / Einrichtung
1. Visual Studio 2015 Enterprise 
> Benutzerdefinierte Installation<br>
> Programming Languages -> C++
2. QT
> mscv2015 64-bit<br>
> Qt Creator 4.5.0
3. OpenCV
4. MySql Connector

5. Qt Creator Starten
Tools -> Options -> Build & Run -> Kits
-- Bestehendes Anpassen oder
-- Neues Hinzfügen
-- Compiler auswählen

6. Projekt öffnen
7. Projects -> Build Settings -> Shadow Build (für alle Configurations) deaktivieren
8. OpenCV / MySql Include- und Lib-Pfad eintragen (Pfad anpassen)
```
INCLUDEPATH += \
    "D:/opencv/build/include" \

LIBS += \
    "-LD:/opencv/build/x64/vc14/lib" \
    -lopencv_world331
```

9. DLLs von OpenCV / MySql zum System PATH hinzufügen oder in das Projektverzeicnis (Release) kopieren
