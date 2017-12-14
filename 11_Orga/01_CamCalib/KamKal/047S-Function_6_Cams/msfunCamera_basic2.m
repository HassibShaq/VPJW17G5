function msfunCamera_basic(block)
%MSFUNTMPL_BASIC A template for a Leve-2 M-file S-function
%   The M-file S-function is written as a MATLAB function with the
%   same name as the S-function. Replace 'msfuntmpl_basic' with the 
%   name of your S-function.
%
%   It should be noted that the M-file S-function is very similar
%   to Level-2 C-Mex S-functions. You should be able to get more
%   information for each of the block methods by referring to the
%   documentation for C-Mex S-functions.
%
%   Copyright 2003-2007 The MathWorks, Inc.

%%
%% The setup method is used to setup the basic attributes of the
%% S-function such as ports, parameters, etc. Do not add any other
%% calls to the main body of the function.
%%
setup(block);

%endfunction

%% Function: setup ===================================================
%% Abstract:
%%   Set up the S-function block's basic characteristics such as:
%%   - Input ports
%%   - Output ports
%%   - Dialog parameters
%%   - Options
%%
%%   Required         : Yes
%%   C-Mex counterpart: mdlInitializeSizes
%%
function setup(block)

% Register number of ports
block.NumInputPorts  = 2;
block.NumOutputPorts = 1;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;


% Override input port1 properties
block.InputPort(1).Dimensions        = 2;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = false;
% Override input port2 properties
block.InputPort(2).Dimensions        = 8;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = false;

% Override output port properties
block.OutputPort(1).Dimensions       = 56;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';
% Register parameters
block.NumDialogPrms     = 0;

% Register sample times
%  [0 offset]            : Continuous sample time
%  [positive_num offset] : Discrete sample time
%
%  [-1, 0]               : Inherited sample time
%  [-2, 0]               : Variable sample time
block.SampleTimes = [0.1 0];

% Specify the block simStateComliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'CustomSimState',  < Has GetSimState and SetSimState methods
%    'DisallowSimState' < Error out when saving or restoring the model sim state
block.SimStateCompliance = 'DefaultSimState';

%% -----------------------------------------------------------------
%% The M-file S-function uses an internal registry for all
%% block methods. You should register all relevant methods
%% (optional and required) as illustrated below. You may choose
%% any suitable name for the methods and implement these methods
%% as local functions within the same file. See comments
%% provided for each function for more information.
%% -----------------------------------------------------------------

% block.RegBlockMethod('PostPropagationSetup',    @DoPostPropSetup);
block.RegBlockMethod('InitializeConditions', @InitializeConditions);
% block.RegBlockMethod('Start', @Start);
block.RegBlockMethod('Outputs', @Outputs);     % Required
% block.RegBlockMethod('Update', @Update);
% block.RegBlockMethod('Derivatives', @Derivatives);
block.RegBlockMethod('Terminate', @Terminate); % Required

%end setup

%%
%% PostPropagationSetup:
%%   Functionality    : Setup work areas and state variables. Can
%%                      also register run-time methods here
%%   Required         : No
%%   C-Mex counterpart: mdlSetWorkWidths
%%
% function DoPostPropSetup(block)
% block.NumDworks = 1;
%   
%   block.Dwork(1).Name            = 'x0';
%   block.Dwork(1).Dimensions      = 1;
%   block.Dwork(1).DatatypeID      = 0;      % double
%   block.Dwork(1).Complexity      = 'Real'; % real
%   block.Dwork(1).UsedAsDiscState = true;

%%
%% InitializeConditions:
%%   Functionality    : Called at the start of simulation and if it is 
%%                      present in an enabled subsystem configured to reset 
%%                      states, it will be called when the enabled subsystem
%%                      restarts execution to reset the states.
%%   Required         : No
%%   C-MEX counterpart: mdlInitializeConditions
%%
function InitializeConditions(block)

global vid1;
vid1 = videoinput('winvideo',2,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid1,'TriggerRepeat',Inf);   %Setzen der mˆglichen Triggerwiederholungen
set(vid1, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid1,'manual');    %Trigger auf manuel setzen

global vid2;
vid2 = videoinput('winvideo',1,'Y800_640x480'); %Erstellen eines Video Input Objektes
set(vid2,'TriggerRepeat',Inf);   %Setzen der mˆglichen Triggerwiederholungen
set(vid2, 'FramesPerTrigger', 1);    %Anzahl Bilder pro Triggerung
triggerconfig(vid2,'manual');    %Trigger auf manuel setzen

%Video Source Objekt
global vid_src1;
vid_src1 = getselectedsource(vid1);   %Erstellen eines Video Source Objektes
set(vid_src1, 'Brightness',128);   %Einstellen der Helligkeit
set(vid_src1, 'Contrast',28);   %Einstellen des Kontrastes
set(vid_src1, 'Exposure',-10);   %Einstellen der Belichtung
set(vid_src1, 'BrightnessMode','manual');

global vid_src2;
vid_src2 = getselectedsource(vid2);   %Erstellen eines Video Source Objektes
set(vid_src2, 'Brightness',128);   %Einstellen der Helligkeit
set(vid_src2, 'Contrast',28);   %Einstellen des Kontrastes
set(vid_src2, 'Exposure',-10);   %Einstellen der Belichtung
set(vid_src2, 'BrightnessMode','manual');

start(vid1); %Aktivieren des Video Input Objektes
start(vid2); %Aktivieren des Video Input Objektes
pause(0.5); %Kurze Pause

fig1=figure(1);
set(fig1,'Position',[5 527 560 420]);

global cam;
global hr;
global checkpoint;
checkpoint(1:10*2)=0;

load Cam1DataR484_c;
load Cam2DataR484_c;

hr=222.5;
cam=[struct('x0',1820,'y0',1291,'h',2885,'xmin',nan,'xmax',3160,'ymin',nan,'ymax',nan,'xpix',xpix1,'ypix',ypix1,'xmm',xmm1,'ymm',ymm1)...
     struct('x0',4262,'y0',1232,'h',2885,'xmin',2910,'xmax',nan,'ymin',nan,'ymax',nan,'xpix',xpix2,'ypix',ypix2,'xmm',xmm2,'ymm',ymm2)];

% n=linspace(0,2*pi,100);
% xkreis1=400*cos(n)+3000;
% ykreis1=400*sin(n)+1070;
% xkreis2=600*cos(n)+3000;
% ykreis2=600*sin(n)+1070;

global plotmm;
% subplot(2,2,[1 2])
plotmm=plot(...
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-',...   %Robbi 1
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+90)+0], [0 75*sind(0+90)+0], 'bx-',...  %Robbi 2
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+180)+0], [0 75*sind(0+180)+0], 'bx-',...    %Robbi 3
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+270)+0], [0 75*sind(0+270)+0], 'bx-',...    %Robbi 4
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+90)+0], [0 75*sind(0+90)+0], 'bx-', [0 75*cosd(0+180)+0], [0 75*sind(0+180)+0], 'bx-',...   %Robbi 5
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+90)+0], [0 75*sind(0+90)+0], 'bx-', [0 75*cosd(0+270)+0], [0 75*sind(0+270)+0], 'bx-',...   %Robbi 6
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+180)+0], [0 75*sind(0+180)+0], 'bx-', [0 75*cosd(0+270)+0], [0 75*sind(0+270)+0], 'bx-',... %Robbi 7
    0, 0, 'bo', [0 90*cosd(0)+0], [0 90*sind(0)+0], 'b*-', [0 75*cosd(0+90)+0], [0 75*sind(0+90)+0], 'bx-', [0 75*cosd(0+180)+0], [0 75*sind(0+180)+0], 'bx-', [0 75*cosd(0+270)+0], [0 75*sind(0+270)+0], 'bx-',...    %Robbi 8
    0, 0, 'ko');%...
%     xkreis1,ykreis1,'k',xkreis2,ykreis2,'k');
axis equal;
axis([0 5700 0 2700]);
% axis square
global fig2;
fig2=figure(2);
% global plotchp;
% plotchp = plot(...
%     checkpoint(1:10),checkpoint(11:20),'ko-',...
%     checkpoint(1),checkpoint(2),'ro-');
set(fig2,'Position',[5 35 560 420])
axis equal;
axis([0 5700 0 2700]);
global h;
h = [text(0,0,'1') text(0,0,'2') text(0,0,'3') text(0,0,'4') text(0,0,'5')...
    text(0,0,'6') text(0,0,'7') text(0,0,'8') text(0,0,'9') text(0,0,'10')];

% Start(block);

%end InitializeConditions


%%
%% Start:
%%   Functionality    : Called once at start of model execution. If you
%%                      have states that should be initialized once, this 
%%                      is the place to do it.
%%   Required         : No
%%   C-MEX counterpart: mdlStart
%%
% function Start(block)

%endfunction

%%
%% Outputs:
%%   Functionality    : Called to generate block outputs in
%%                      simulation step
%%   Required         : Yes
%%   C-MEX counterpart: mdlOutputs
%%
function Outputs(block)
global vid1;
global vid2;
global cam;
global hr;
global checkpoint;
global plotmm;
% global plotchp;
global h;

%Bild erzeugen
trigger([vid1 vid2]);   %Triggern
t=clock;    %Zeitstempel f¸r das Bild erstellen
t=t(4:6);
while (vid1.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame1 = getdata(vid1,1); %Bild speichern
while (vid2.FramesAvailable == 0)    %Warten bis Bild vorhanden
end
frame2 = getdata(vid2,1); %Bild speichern

frame=[struct('frame',frame1) struct('frame',frame2)];
% subplot(2,2,3)
% imshow(frame(1,1).frame);
% subplot(2,2,4)
% imshow(frame(1,2).frame);

anzahl_cam=size(frame);
anzahl_cam=anzahl_cam(1,2);
f=false(480, 640);
bin_frame(1:anzahl_cam)=struct('bin_frame',f);
for i=1 : anzahl_cam
    bin_frame(1,i).bin_frame = transform_frame(frame(1,i).frame); %Bin‰res Bild erstellen
end
% subplot(2,2,3)
% figure(2);
% imshow(bin_frame(1,1).bin_frame);
% subplot(2,2,4)
% imshow(bin_frame(1,2).bin_frame);

bounds(1:anzahl_cam)=struct('bounds',[]);
anzahl_reflectoren(1:anzahl_cam)=0;
for i=1 : anzahl_cam
    bounds_hilfe=bwboundaries(bin_frame(1,i).bin_frame,4,'noholes');   %Ermitteln der ‰uﬂeren Grenzen von weiﬂen Fl‰chen
    [bounds(1,i).bounds,anzahl_reflectoren(1,i)] = filtering(bounds_hilfe);  %Zu kleine Reflektionsfl‰chen werden Herausgefiltert
end

%Mittelpunkte der Reflektoren in pixel bestimmen
reflector(1:sum(anzahl_reflectoren))=struct('x0',0,'y0',0,'valence',1,'size',0,'position_in_grad',0);
zaehler1=1;
zaehler2=0;
for i=1 : anzahl_cam
    zaehler2=zaehler2+anzahl_reflectoren(1,i);
    if anzahl_reflectoren(1,i) >= 1
        reflector(zaehler1:zaehler2) = get_pixel_position(bounds(1,i).bounds, anzahl_reflectoren(1,i));
    end
    zaehler1=zaehler1+anzahl_reflectoren(1,i);
end


target_track=abs(block.InputPort(1).Data(2)');
if target_track < 0.5
    [ ref_ges anzahl_ref_ges ] = get_mm_position( reflector, anzahl_reflectoren, cam, anzahl_cam, hr );
    if anzahl_ref_ges < 2
        a(1:24)=0;
    else
        [grosse_ref, anzahl_grosse_ref] = find_the_biggest(ref_ges, anzahl_ref_ges); %Ermitteln der groﬂen Reflektoren
        robot = build_robots( grosse_ref, anzahl_grosse_ref, anzahl_ref_ges, ref_ges ); %Erstellen von Robotern

        j=1;
        a(1:24)=0;
        for i = 1 : 8   %Werte auf Ausgangsvector schreiben
            a(j)=robot(1,i).xpos;
            a(j+1)=robot(1,i).ypos;
            a(j+2)=robot(1,i).theta;
            j=j+3;
        end
    end
    on=block.InputPort(1).Data(1)';
elseif target_track >= 0.5
    a(1:24)=0;
    [ ref_ges anzahl_ref_ges ] = get_mm_position( reflector, anzahl_reflectoren, cam, anzahl_cam, 0 );
    if anzahl_ref_ges > 10
        anzahl_ref_ges=10;
    end
    j=1;
    chpx(1:anzahl_ref_ges)=0;
    chpy(1:anzahl_ref_ges)=0;
    checkpoint(1:10*2)=0;
    for i = 1 : anzahl_ref_ges   %Werte auf Ausgangsvector schreiben
            checkpoint(j)=ref_ges(1,i).x0;
            checkpoint(j+1)=ref_ges(1,i).y0;
            set(h(i),'Position',[checkpoint(j) checkpoint(j+1)]);
            j=j+2;
            chpx(i)=ref_ges(1,i).x0;
            chpy(i)=ref_ges(1,i).y0;
    end
%     set(plotchp(1),'Xdata',chpx,'Ydata',chpy);
%     set(plotchp(2),'Xdata',chpx(1),'Ydata',chpy(1));
    set(plotmm(29),'Xdata',chpx,'Ydata',chpy);
%     help=1;
%     for i=1 : 10
%         set(h(i),'Position',[checkpoint(help) checkpoint(help+1)]);
%         help=help+2;
%     end

    on=0;
end

% Robbi 1
set(plotmm(1),'Xdata',a(1),'Ydata',a(2));
set(plotmm(2),'Xdata',[a(1) 90*cosd(a(3))+a(1)],'Ydata',[a(2) 90*sind(a(3))+a(2)]);
% Robbi 2
set(plotmm(3),'Xdata',a(4),'Ydata',a(5));
set(plotmm(4),'Xdata',[a(4) 90*cosd(a(6))+a(4)],'Ydata',[a(5) 90*sind(a(6))+a(5)]);
set(plotmm(5),'Xdata',[a(4) 75*cosd(a(6)+90)+a(4)],'Ydata',[a(5) 75*sind(a(6)+90)+a(5)]);
% Robbi 3
set(plotmm(6),'Xdata',a(7),'Ydata',a(8));
set(plotmm(7),'Xdata',[a(7) 90*cosd(a(9))+a(7)],'Ydata',[a(8) 90*sind(a(9))+a(8)]);
set(plotmm(8),'Xdata',[a(7) 75*cosd(a(9)+180)+a(7)],'Ydata',[a(8) 75*sind(a(9)+180)+a(8)]);
% Robbi 4
set(plotmm( 9),'Xdata',a(10),'Ydata',a(11));
set(plotmm(10),'Xdata',[a(10) 90*cosd(a(12))+a(10)],'Ydata',[a(11) 90*sind(a(12))+a(11)]);
set(plotmm(11),'Xdata',[a(10) 75*cosd(a(12)+270)+a(10)],'Ydata',[a(11) 75*sind(a(12)+270)+a(11)]);
% Robbi 5
set(plotmm(12),'Xdata',a(13),'Ydata',a(14));
set(plotmm(13),'Xdata',[a(13) 90*cosd(a(15))+a(13)],'Ydata',[a(14) 90*sind(a(15))+a(14)]);
set(plotmm(14),'Xdata',[a(13) 75*cosd(a(15)+90)+a(13)],'Ydata',[a(14) 75*sind(a(15)+90)+a(14)]);
set(plotmm(15),'Xdata',[a(13) 75*cosd(a(15)+180)+a(13)],'Ydata',[a(14) 75*sind(a(15)+180)+a(14)]);
% Robbi 6
set(plotmm(16),'Xdata',a(16),'Ydata',a(17));
set(plotmm(17),'Xdata',[a(16) 90*cosd(a(18))+a(16)],'Ydata',[a(17) 90*sind(a(18))+a(17)]);
set(plotmm(18),'Xdata',[a(16) 75*cosd(a(18)+90)+a(16)],'Ydata',[a(17) 75*sind(a(18)+90)+a(17)]);
set(plotmm(19),'Xdata',[a(16) 75*cosd(a(18)+270)+a(16)],'Ydata',[a(17) 75*sind(a(18)+270)+a(17)]);
% Robbi 7
set(plotmm(20),'Xdata',a(19),'Ydata',a(20));
set(plotmm(21),'Xdata',[a(19) 90*cosd(a(21))+a(19)],'Ydata',[a(20) 90*sind(a(21))+a(20)]);
set(plotmm(22),'Xdata',[a(19) 75*cosd(a(21)+180)+a(19)],'Ydata',[a(20) 75*sind(a(21)+180)+a(20)]);
set(plotmm(23),'Xdata',[a(19) 75*cosd(a(21)+270)+a(19)],'Ydata',[a(20) 75*sind(a(21)+270)+a(20)]);
% Robbi 8
set(plotmm(24),'Xdata',a(22),'Ydata',a(23));
set(plotmm(25),'Xdata',[a(22) 90*cosd(a(24))+a(22)],'Ydata',[a(23) 90*sind(a(24))+a(23)]);
set(plotmm(26),'Xdata',[a(22) 75*cosd(a(24)+90)+a(22)],'Ydata',[a(23) 75*sind(a(24)+90)+a(23)]);
set(plotmm(27),'Xdata',[a(22) 75*cosd(a(24)+180)+a(22)],'Ydata',[a(23) 75*sind(a(24)+180)+a(23)]);
set(plotmm(28),'Xdata',[a(22) 75*cosd(a(24)+270)+a(22)],'Ydata',[a(23) 75*sind(a(24)+270)+a(23)]);

var_input=block.InputPort(2).Data';

for i=1 : 8 %Umrechnung in Bogenmaﬂ
    a(i*3)=a(i*3)*pi/180;
end

% if a(7)==0 || a(10)==0
%     save bin_frame;
% end

block.OutputPort(1).Data = [a t on var_input checkpoint];
%end Outputs

%%
%% Update:
%%   Functionality    : Called to update discrete states
%%                      during simulation step
%%   Required         : No
%%   C-MEX counterpart: mdlUpdate
%%
% function Update(block)

%end Update

%%
%% Derivatives:
%%   Functionality    : Called to update derivatives of
%%                      continuous states during simulation step
%%   Required         : No
%%   C-MEX counterpart: mdlDerivatives
%%
% function Derivatives(block)

%end Derivatives

%%
%% Terminate:
%%   Functionality    : Called at the end of simulation for cleanup
%%   Required         : Yes
%%   C-MEX counterpart: mdlTerminate
%%
function Terminate(block)
global vid1;
global vid2;
global fig2
stop(vid1);
delete(vid1);
stop(vid2);
delete(vid2);
delete(fig2);
%end Terminate

