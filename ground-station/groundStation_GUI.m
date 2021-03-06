function varargout = groundStation_GUI(varargin)
% GROUNDSTATION_GUI MATLAB code for groundStation_GUI.fig
%      GROUNDSTATION_GUI, by itself, creates a new GROUNDSTATION_GUI or raises the existing
%      singleton*.
%
%      H = GROUNDSTATION_GUI returns the handle to a new GROUNDSTATION_GUI or the handle to
%      the existing singleton*.
%
%      GROUNDSTATION_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GROUNDSTATION_GUI.M with the given input arguments.
%
%      GROUNDSTATION_GUI('Property','Value',...) creates a new GROUNDSTATION_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before groundStation_GUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to groundStation_GUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help groundStation_GUI


% Last Modified by GUIDE v2.5 11-Sep-2018 10:22:29

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @groundStation_GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @groundStation_GUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before groundStation_GUI is made visible.
function groundStation_GUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to groundStation_GUI (see VARARGIN)

% Choose default command line output for groundStation_GUI
handles.output = hObject;

global tabledata;
global tabledata2;
global calcHeightVector;
tabledata=zeros(10,11);
tabledata2=zeros(10,2);
calcHeightVector=0;

global tabledataAirflow tabledataPressure tabledataTemperature ...
    tabledataHumidity green_light red_light tabledataHeight time timePlot... 
    row dataBuffer t; 
tabledataAirflow     = single(zeros(10,1));
tabledataPressure    = single(zeros(10,6));
tabledataTemperature = single(zeros(10,9));
tabledataHumidity    = single(zeros(10,1));
tabledataHeight      = [];
row                = 0;
dataBuffer         = [];
t = [];
time           = ["Not available";"Not available";"Not available";...
    "Not available";"Not available";"Not available";"Not available";...
    "Not available";"Not available";"Not available"];

% Initialise tabs
handles.tabManager = TabManager( hObject );

% Set-up a selection changed function on the create tab groups
tabGroups = handles.tabManager.TabGroups;
for tgi=1:length(tabGroups)
    set(tabGroups(tgi),'SelectionChangedFcn',@tabChangedCB)
end

%logo generation
tubular_logo = imread('tubular_logo.png');
bexrex_logo = imread('bexrex_logo.png');

axes(handles.tubular_logo);
imshow(tubular_logo);

axes(handles.bexus_logo);
imshow(bexrex_logo);
%end of logo generation

%image for valves pump indicator
green_light = imread('green-light-small.png');
red_light = imread('red-light-small.png');
setInitialState(handles, red_light);
%end

%telecommand initialization
command = [];
handles.command = command;
%end of telecommand initialization

%altitude figure conf
xlabel(handles.axes4, 'Time [hr]');
ylabel(handles.axes4, 'Altitude [m]');
xlim(handles.axes4, 'manual');
ylim(handles.axes4, 'manual');
xlim(handles.axes4, [0 10]);
ylim(handles.axes4, [0 32000]);
grid(handles.axes4, 'on');

% Update handles structure
guidata(hObject, handles);

function setInitialState(handles, red_light)
%set initial state
axes(handles.ind_valve1);
imshow(red_light);
axes(handles.ind_valve2);
imshow(red_light);
axes(handles.ind_valve3);
imshow(red_light);
axes(handles.ind_valve4);
imshow(red_light);
axes(handles.ind_valve5);
imshow(red_light);
axes(handles.ind_valve6);
imshow(red_light);
% axes(handles.ind_valve7);
% imshow(red_light);
% axes(handles.ind_valve8);
% imshow(red_light);
axes(handles.ind_flush);
imshow(red_light);
axes(handles.ind_cac);
imshow(red_light);
axes(handles.ind_pump);
imshow(red_light);
axes(handles.ind_htr1);
imshow(red_light);
axes(handles.ind_htr2);
imshow(red_light);
% UIWAIT makes groundStation_GUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% Called when a user clicks on a tab
function tabChangedCB(src, eventdata)

disp(['Changing tab from ' eventdata.OldValue.Title ' to ' eventdata.NewValue.Title ] );



% --- Outputs from this function are returned to the command line.
function varargout = groundStation_GUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function NetworkDispose2(t)
    %stopasync(t);
    fclose(t);delete(t);
    delete(t);
    clear t;
    
    
   % echotcpip('off');


% --- Executes on button press in tcp_initialize.
function tcp_initialize_Callback(hObject, eventdata, handles)
% hObject    handle to tcp_initialize (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

remPort=4000;      
host='172.16.18.161';  
locPort=4000;

t = tcpip(host,'RemotePort',remPort,'LocalPort',locPort, 'NetworkRole','Client', 'Timeout', 2);
t.InputBufferSize = 10000;
t.OutputBufferSize = 10000;
% t.ReadAsyncMode = 'continuous';
% t.BytesAvailableFcnCount = 2;
% t.BytesAvailableFcnMode = 'byte';
% t.BytesAvailableFcn = {@update_tcpoutput, handles};
handles.t=t;


fopen(t);

if (~strcmp(t.Status,'open'))
    %NetworkError(t,'Connection failed!');
    set(handles.constat_tcp, 'String', 'Failed');
else
    set(handles.constat_tcp, 'String', 'Connected');
end

%readasync(t);

guidata(gcbf, handles);


% --- Executes on button press in tcp_stop.
function tcp_stop_Callback(hObject, eventdata, handles)
% hObject    handle to tcp_stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
t=handles.t;
NetworkDispose2(t);
set(handles.constat_tcp, 'String', 'Disconnected');

% --- Executes on button press in udp_initialize.
function udp_initialize_Callback(hObject, eventdata, handles)
% hObject    handle to udp_initialize (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%u = udp('1.1.1.1',8888);
currentFolder = pwd;
baseFileName = 'logFile.txt';
fullFileName = fullfile(currentFolder, baseFileName);
% if exist(fullFileName)
%   % Open the file however you do
%   
% else
%   % File does not exist.
%   warningMessage = sprintf('%s does not exist', fullFileName);
%   uiwait(warndlg(warningMessage));
% end

  [logFile, warnMessage] = fopen(fullFileName, 'wt');
  handles.logFile = logFile;
  handles.warnMessage = warnMessage;


remPort=8888;      
host='172.16.18.161';  
locPort=8888;
u = udp(host,'RemotePort',remPort,'LocalPort',locPort); 
u.InputBufferSize = 120;
u.OutputBufferSize = 10000;
u.ReadAsyncMode = 'continuous';
u.BytesAvailableFcnMode = 'byte';
u.BytesAvailableFcnCount =120;
u.BytesAvailableFcn = {@update_udpoutput, handles};
handles.u=u;


fopen(u);
readasync(u);
if (~strcmp(u.Status,'open'))
    %NetworkError(u,'Connection failed!');
    set(handles.constat_udp, 'String', 'Connection Closed');
else
    set(handles.constat_udp, 'String', 'Connection Opened');
end
disp('udp initialized');



guidata(hObject, handles);


% --- Executes on button press in udp_stop.
function udp_stop_Callback(hObject, eventdata, handles)
% hObject    handle to udp_stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

u=handles.u;
stopasync(u);
fclose(u);
delete(u);
clear u;
fclose(handles.logFile);
global dataBuffer;
dataBuffer = [];

set(handles.constat_udp, 'String', 'Connection Closed');

function update_udpoutput(u, evt, handles)
%global tabledata calcHeightVector;
global tabledataAirflow tabledataPressure tabledataTemperature ...
    tabledataHumidity green_light red_light tabledataHeight time timePlot...
    row dataBuffer t; 
data=fread(u)';
%disp(data);
dataSize=length(data);
gsString = strfind(data,'gs');
if gsString(1)==1
    
    i = 4;
    time_stamp = uint8(data(i:(i+3)));
    time_stamp = swapbytes(typecast(time_stamp, 'single'));
    i=i+1;
    
    while i<dataSize(1)
        textq = char(data(i:(i+1)));
        switch(textq)
            case 'ts'
                i=i+3;
                for j=1:(data(i))
                    i=i+2;
                    temp = uint8([data(i:(i+3))]);
                    %disp([text(i:(i+3))]);
                    temp = typecast(temp, 'single');
%                     temp = swapbytes(temp);
                    tempSensorVal(j) = (temp);
                    i=i+3;
%                     disp(textq);
                end
            case 'ps'
                i=i+3;
                for j=1:(data(i))
                    i=i+2;
                    temp = uint8([data(i:(i+3))]);
                    %disp([text(i:(i+3))]);
                    temp = typecast(temp, 'single');
%                     temp = swapbytes(temp);
                    pressSensorVal(j) = (temp);
                    i=i+3;
%                     disp(textq);
                end
            case 'hs'
                i=i+3;
                for j=1:(data(i))
                    i=i+2;
                    temp = uint8([data(i:(i+3))]);
                    %disp([text(i:(i+3))]);
                    temp = typecast(temp, 'single');
%                     temp = swapbytes(temp);
                    humidSensorVal(j) = (temp);
                    i=i+3;
%                     disp(textq);
                end
            case 'as'
                i=i+3;
                for j=1:(data(i))
                    i=i+2;
                    temp = uint8([data(i:(i+3))]);
                    %disp([text(i:(i+3))]);
                    temp = typecast(temp, 'single');
%                     temp = swapbytes(temp);
                    airFSensorVal(j) = (temp);
                    i=i+3;
%                     disp(textq);
                end
            case 'st'
                i=i+3;
                statusVal = typecast(uint8(data(i:(i+1))),'uint16');
                %disp([text(i:(i+1))]);
                i=i+1;
%                 disp(textq);
                
            case 'md'
                i=i+3;
                modeVal = [data(i)];
                %disp(text(i));
%                 disp(textq);
                break
        end
        
        i=i+1;
    end
    
end
%%   Telemetry sources 
 %   tempSensorVal;  Contains the values of the temperature sensor(s)
 %   pressSensorVal; Contains the values of the pressure sensor(s)
 %   humidSensorVal; Contains the values of the humidity sensor(s)
 %   airFSensorVal;  Contains the values of the airFlow sensor(s)
 %   statusVal;      Contains the data of the valves status
 %   modeVal;        Contains the mode state of the onboard software
 %   time_stamp;     Contains the time stamp of the transmission
 %
 %
time_str = sprintf("%d:%d:%.0f", evt.Data.AbsTime(4), evt.Data.AbsTime(5), evt.Data.AbsTime(6));
 %%   Write Pressure 
pressSensorMean = sum(pressSensorVal)/length(pressSensorVal);
tabledataPressure = [ pressSensorVal; tabledataPressure(1:end-1,:)];
time = [time_str; time(1:end-1,:)];
pressTable = [cellstr(time), num2cell(tabledataPressure)];
% pressCell = table2cell(pressTable);
set(handles.table_pressure, 'Data', pressTable);
% drawnow;

%   Write temperature
tempSensorMean =sum(tempSensorVal)/length(tempSensorVal);
tabledataTemperature = [tempSensorVal; tabledataTemperature(1:end-1,:)];
set(handles.table_temperature, 'Data', [cellstr(time), num2cell(tabledataTemperature)]);
% drawnow;

%%   Write Humidity
humidSensorMean = sum(humidSensorVal)/length(humidSensorVal);
tabledataHumidity = [humidSensorVal; tabledataHumidity(1:end-1,:)];
set(handles.table_humidity, 'Data', [cellstr(time), num2cell(tabledataHumidity)]);
% drawnow;

%%   Write airflow
airFSensorMean = sum(airFSensorVal)/length(airFSensorVal);
tabledataAirflow = [airFSensorVal; tabledataAirflow(1:end-1,:)];
set(handles.table_airflow, 'Data', [cellstr(time), num2cell(tabledataAirflow)]);
% drawnow;

%%   Write status of valves
light_hanles = light_handles(handles);
expStatus = zeros(1,15);
for i=1:15
    if(bitget(statusVal,i)==1)
        %axes(light_hanles(i+1));
        %imshow(green_light);
        set(light_hanles(i), 'Cdata', green_light)
    elseif(bitget(statusVal,i)==0)
        if (i==8||i==9||i==10||i==11) 
        else
            %axes(light_hanles(i+1));
            %imshow(red_light);
            set(light_hanles(i), 'Cdata', red_light)
        end
    end
    expStatus(1,i) = bitget(statusVal,i);
end


     
     
%%   Write mode state
switch(modeVal)
    case 0
        modeDisp = 'Standby mode';
    case 1
        modeDisp = 'Normal mode - ascent';
    case 2
        modeDisp = 'Normal mode - descent';
    case 3
        modeDisp = 'safe mode';
    case 4
        modeDisp = 'Manual mode';
    otherwise
        modeDisp = 'Unknown mode';     
end
set(handles.status_mode, 'String', modeDisp);
     
%% Calculate height and plot it as a function of time
 % Sources: Vertical pressure variation, read 2018-05-05
 %          https://en.wikipedia.org/wiki/Vertical_pressure_variation
 
 T_0_refernce   = 288.5;        % K
 L_lapse_rate   = -6.5*10^(-3); % K/m
 press_refernce = 1013;         % mbar
 R_gas_constant = 287.053;      % J/(kg K)
 gravity        = 9.81;         % m/s^2
 
 pressValue = medianFind(pressSensorVal);
 
 calcHeight = T_0_refernce/L_lapse_rate *((pressValue/press_refernce)...
     ^(-(L_lapse_rate*R_gas_constant/gravity))-1);
 
  tabledataHeight = [tabledataHeight calcHeight];
  t = [t (length(tabledataHeight)/3600)];
  cla(handles.axes4);
  line(handles.axes4, t, tabledataHeight);
  
  
%   tableCompiled = [time(1,1), tabledataPressure(1,1:6), tabledataTemperature(1,1:9), tabledataAirflow(1,1), tabledataHumidity(1,1)];
  tableCompiled = [tabledataTemperature(1,1:9), tabledataPressure(1,1:6), tabledataAirflow(1,1), tabledataHumidity(1,1), expStatus(1,1:7), expStatus(1,12:15), modeVal(1,1)];
  
  formatSpec = '%d:%d:%.0f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,\n';
  if (handles.logFile < 0)
    warningMessage = handles.warnMessage;
    uiwait(warndlg(warningMessage));
  else
      fprintf(handles.logFile, formatSpec, evt.Data.AbsTime(4), evt.Data.AbsTime(5), evt.Data.AbsTime(6), tableCompiled);
  end
  
 drawnow;

%% Write data to file
 %save('TUBULAR.mat','data','-append')

 function light_handles_vector = light_handles(handles)
 % Creates an array of handles of the valve states in the GUI for
 % easier management
 light_handles_vector(1)    = handles.ind_pump.Children;
 light_handles_vector(2)    = handles.ind_valve1.Children;
 light_handles_vector(3)    = handles.ind_valve2.Children;
 light_handles_vector(4)    = handles.ind_valve3.Children;
 light_handles_vector(5)    = handles.ind_valve4.Children;
 light_handles_vector(6)    = handles.ind_valve5.Children;
 light_handles_vector(7)    = handles.ind_valve6.Children;
 light_handles_vector(12)    = handles.ind_flush.Children;
 light_handles_vector(13)    = handles.ind_cac.Children;
 light_handles_vector(14)   = handles.ind_htr1.Children;
 light_handles_vector(15)   = handles.ind_htr2.Children;
 
 function medianPressureReading = medianFind(pressSensor)
 for i = 1:3
     for j = 2:3
         if (pressSensor(i) > pressSensor(j))
             dummy = pressSensor(i);
             pressSensor(i) = pressSensor(j);
             pressSensor(j) = dummy;
         end
     end
 end
 medianPressureReading = pressSensor(2);
 
function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in check_mode_setting.
function check_mode_setting_Callback(hObject, eventdata, handles)
% hObject    handle to check_mode_setting (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of check_mode_setting


% --- Executes on button press in check_heater_control.
function check_heater_control_Callback(hObject, eventdata, handles)
% hObject    handle to check_heater_control (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of check_heater_control


% --- Executes on button press in check_pump_control.
function check_pump_control_Callback(hObject, eventdata, handles)
% hObject    handle to check_pump_control (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of check_pump_control


% --- Executes on button press in check_valves_control.
function check_valves_control_Callback(hObject, eventdata, handles)
% hObject    handle to check_valves_control (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of check_valves_control




function commandToBuild = buildModeCommand(handles)
commandToBuild(1:5) = 'md,1,';

switch get(get(handles.set_mode_buttons, 'SelectedObject'), 'Tag')
    case 'button_set_mode_standby'
        commandToBuild(6:7) = '0,';
    case 'button_set_mode_normalAscent'
        commandToBuild(6:7) = '1,';
    case 'button_set_mode_normalDescent'
        commandToBuild(6:7) = '2,';
    case 'button_set_mode_safe'
        commandToBuild(6:7) = '3,';
    case 'button_set_mode_manual'
        commandToBuild(6:7) = '4,';
end

%%%%%%%%%%%%%%%%%%%%%%
function commandToBuild = buildHeaterCommand(handles)
commandToBuild(1:5) = 'ht,2,';
switch get(get(handles.heater1_control, 'SelectedObject'), 'Tag')
    case 'control_heater1_on'
        commandToBuild(6:7) = '1,';
    case 'control_heater1_off'
        commandToBuild(6:7) = '0,';
end
switch get(get(handles.heater2_control, 'SelectedObject'), 'Tag')
    case 'control_heater2_on'
        commandToBuild(8:9) = '1,';
    case 'control_heater2_off'
        commandToBuild(8:9) = '0,';
end

%%%%%%%%%%%%%%%%%%%%%%
function commandToBuild = buildAscCommand(handles)
commandToBuild(1:5) = 'sc,9,';

switch get(get(handles.pump_control, 'SelectedObject'), 'Tag')
    case 'control_pump_on'
        commandToBuild(6:7) = '1,';
    case 'control_pump_off'
        commandToBuild(6:7) = '0,';
end

switch get(get(handles.button_valve_1, 'SelectedObject'), 'Tag')
    case 'valve_1_on'
        commandToBuild(8:9) = '1,';
    case 'valve_1_off'
        commandToBuild(8:9) = '0,';
end    
switch get(get(handles.button_valve_2, 'SelectedObject'), 'Tag')
    case 'valve_2_on'
        commandToBuild(10:11) = '1,';
    case 'valve_2_off'
        commandToBuild(10:11) = '0,';
end
switch get(get(handles.button_valve_3, 'SelectedObject'), 'Tag')
    case 'valve_3_on'
        commandToBuild(12:13) = '1,';
    case 'valve_3_off'
        commandToBuild(12:13) = '0,';
end
switch get(get(handles.button_valve_4, 'SelectedObject'), 'Tag')
    case 'valve_4_on'
        commandToBuild(14:15) = '1,';
    case 'valve_4_off'
        commandToBuild(14:15) = '0,';
end
switch get(get(handles.button_valve_5, 'SelectedObject'), 'Tag')
    case 'valve_5_on'
        commandToBuild(16:17) = '1,';
    case 'valve_5_off'
        commandToBuild(16:17) = '0,';
end
switch get(get(handles.button_valve_6, 'SelectedObject'), 'Tag')
    case 'valve_6_on'
        commandToBuild(18:19) = '1,';
    case 'valve_6_off'
        commandToBuild(18:19) = '0,';
end
% switch get(get(handles.button_valve_7, 'SelectedObject'), 'Tag')
%     case 'valve_7_on'
%         commandToBuild(20:21) = '1,';
%     case 'valve_7_off'
%         commandToBuild(20:21) = '0,';
% end
% switch get(get(handles.button_valve_8, 'SelectedObject'), 'Tag')
%     case 'valve_8_on'
%         commandToBuild(22:23) = '1,';
%     case 'valve_8_off'
%         commandToBuild(22:23) = '0,';
% end
switch get(get(handles.button_valve_flush, 'SelectedObject'), 'Tag')
    case 'valve_flush_on'
        commandToBuild(20:21) = '1,';
    case 'valve_flush_off'
        commandToBuild(20:21) = '0,';
end
switch get(get(handles.button_valve_cac, 'SelectedObject'), 'Tag')
    case 'valve_cac_on'
        commandToBuild(22:23) = '1,';
    case 'valve_cac_off'
        commandToBuild(22:23) = '0,';
end


% function commandToBuild = buildSensorCommand(handles)
% commandToBuild(1:5) = 'ss,1,';
% commandToBuild(6,7) = '

%%%%%%%%%%%%%%%%%%%%%%
function commandToBuild = buildScheduler(handles)
% Determine the selected data set.
str = get(handles.bags_menu,'String');
val = get(handles.bags_menu,'Value');

commandToBuild(1:5) = 'sd,3,';

switch str{val}
    case 'Bag 1'
        commandToBuild(6:7) = '1,';
    case 'Bag 2'
        commandToBuild(6:7) = '2,';
    case 'Bag 3'
        commandToBuild(6:7) = '3,';
    case 'Bag 4'
        commandToBuild(6:7) = '4,';
    case 'Bag 5'
        commandToBuild(6:7) = '5,';
    case 'Bag 6'
        commandToBuild(6:7) = '6,';
end

firstParam = get(handles.first_parameter, 'String');
firstParam = str2double(firstParam);
firstParam = num2str(round(firstParam));
secondParam = get(handles.second_parameter, 'String');
secondParam = str2double(secondParam);
secondParam = num2str(round(secondParam));

commandToBuild = [commandToBuild, firstParam, ',', secondParam, ',']; 


        

% --- Executes on button press in send_telecommand.
function send_telecommand_Callback(hObject, eventdata, handles)
% hObject    handle to send_telecommand (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
t = handles.t;
data = handles.command;
fwrite(t, data);


% --- Executes on button press in compile_command.
function compile_command_Callback(hObject, eventdata, handles)
% hObject    handle to compile_command (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
command = buildCommand(handles);
set(handles.telecommand_format, 'String', command);
handles.command = command;
guidata(gcbf, handles);

%disp(breakpoint);

function command = buildCommand(handles)
id = 'tub,';
modeCommand = [];
heaterCommand = [];
ascCommand = [];
scheduleCommand = [];
if (get(handles.check_mode_setting, 'Value') == 1)
    modeCommand = buildModeCommand(handles);
end
if (get(handles.check_heater_control, 'Value') == 1)
    heaterCommand = buildHeaterCommand(handles);
end
if (get(handles.check_valves_control, 'Value') == 1)
        ascCommand = buildAscCommand(handles);
end
if (get(handles.check_sampling_schedule, 'Value') == 1)
    scheduleCommand = buildScheduler(handles);
end


nrSubCommand = get(handles.check_mode_setting, 'Value') + ...
               get(handles.check_heater_control, 'Value') +...
               get(handles.check_sampling_schedule, 'Value') +...
               get(handles.check_valves_control, 'Value');
strSubCommand = sprintf('%d,', nrSubCommand);
command = [id strSubCommand modeCommand heaterCommand ascCommand scheduleCommand];
%=======
% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in save_data.
function save_data_Callback(hObject, eventdata, handles)
% hObject    handle to save_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


xlswrite('log/logFile.xls', dataBuffer, 'Sheet1', range);




% --- Executes on selection change in bags_menu.
function bags_menu_Callback(hObject, eventdata, handles)
% hObject    handle to bags_menu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns bags_menu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from bags_menu


% --- Executes during object creation, after setting all properties.
function bags_menu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bags_menu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function first_parameter_Callback(hObject, eventdata, handles)
% hObject    handle to first_parameter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of first_parameter as text
%        str2double(get(hObject,'String')) returns contents of first_parameter as a double


% --- Executes during object creation, after setting all properties.
function first_parameter_CreateFcn(hObject, eventdata, handles)
% hObject    handle to first_parameter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in check_sampling_schedule.
function check_sampling_schedule_Callback(hObject, eventdata, handles)
% hObject    handle to check_sampling_schedule (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of check_sampling_schedule



function edit5_Callback(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit5 as text
%        str2double(get(hObject,'String')) returns contents of edit5 as a double


% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in phase_menu.
function phase_menu_Callback(hObject, eventdata, handles)
% hObject    handle to phase_menu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns phase_menu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from phase_menu


% --- Executes during object creation, after setting all properties.
function phase_menu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to phase_menu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function second_parameter_Callback(hObject, eventdata, handles)
% hObject    handle to second_parameter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of second_parameter as text
%        str2double(get(hObject,'String')) returns contents of second_parameter as a double


% --- Executes during object creation, after setting all properties.
function second_parameter_CreateFcn(hObject, eventdata, handles)
% hObject    handle to second_parameter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
