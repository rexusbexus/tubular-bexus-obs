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

% Last Modified by GUIDE v2.5 26-Apr-2018 14:30:35

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
tabledata=zeros(10,2);
tabledata2=zeros(10,2);

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

% Update handles structure
guidata(hObject, handles);

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
host='1.1.1.1';  
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

u = udp('1.1.1.1',8888);
u.InputBufferSize = 10000;
u.OutputBufferSize = 10000;
u.ReadAsyncMode = 'continuous';
u.BytesAvailableFcn = {@update_udpoutput, handles};
u.BytesAvailableFcnMode = 'byte';
handles.u=u;

fopen(u);

if (~strcmp(u.Status,'open'))
    %NetworkError(u,'Connection failed!');
    set(handles.constat_udp, 'String', 'Connection Closed');
else
    set(handles.constat_udp, 'String', 'Connection Opened');
end
disp('Hello');
u
data=fread(u)';
disp(data);
u
guidata(gcbf, handles);


% --- Executes on button press in udp_stop.
function udp_stop_Callback(hObject, eventdata, handles)
% hObject    handle to udp_stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

u=handles.u;
fclose(u);
delete(u);
clear u;

set(handles.constat_udp, 'String', 'Connection Closed');

function update_udpoutput(u, evt, handles)
global tabledata;
data=fread(u)';
disp('data');
tabledata = [tabledata(2:end,:); data]
set(handles.axes4, 'Data', tabledata);
drawnow;


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


% --- Executes on button press in send_telecommand.
function send_telecommand_Callback(hObject, eventdata, handles)
% hObject    handle to send_telecommand (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


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
