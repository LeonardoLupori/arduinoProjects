function varargout = cameraPreview(varargin)
% CAMERAPREVIEW MATLAB code for cameraPreview.fig
%      CAMERAPREVIEW, by itself, creates a new CAMERAPREVIEW or raises the existing
%      singleton*.
%
%      H = CAMERAPREVIEW returns the handle to a new CAMERAPREVIEW or the handle to
%      the existing singleton*.
%
%      CAMERAPREVIEW('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CAMERAPREVIEW.M with the given input arguments.
%
%      CAMERAPREVIEW('Property','Value',...) creates a new CAMERAPREVIEW or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before cameraPreview_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to cameraPreview_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help cameraPreview

% Last Modified by GUIDE v2.5 01-Mar-2021 09:33:02

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @cameraPreview_OpeningFcn, ...
                   'gui_OutputFcn',  @cameraPreview_OutputFcn, ...
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


% --- Executes just before cameraPreview is made visible.
function cameraPreview_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to cameraPreview (see VARARGIN)

% Choose default command line output for cameraPreview
handles.output = hObject;

handles.vid = varargin{1};
handles.src = varargin{2};
handles.settings = varargin{3};

defaultImg = zeros(handles.vid.VideoResolution,'uint16')';

handles.img = image(defaultImg,'Parent',handles.axImg);
handles.axImg.XTick = []; handles.axImg.YTick = [];
handles.axImg.Box = 'on';

setappdata(handles.img,'UpdatePreviewWindowFcn',@liveSaturimeter);

handles.edtShtt.String = num2str(handles.src.Shutter);
handles.edtGain.String = num2str(handles.src.Gain);
handles.edtFr.String = num2str(handles.src.FrameRate);

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes cameraPreview wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = cameraPreview_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function edtGain_Callback(hObject, eventdata, handles)

newValue = str2double(get(hObject,'String'));
handles = guidata(hObject);

info = propinfo(handles.src,'Gain');
constaints = info.ConstraintValue;

% Check if newValue is out of the hardware capabilities
if newValue < constaints(1)
    newValue = constaints(1);
elseif newValue > constaints(2)
    newValue = constaints(2);
end

% Put the new value in the camera and in the settings struct
handles.src.Gain = newValue;
handles.settings.camera.Gain = newValue;
% Put the new value in the GUI
set(hObject,'String',num2str(newValue))
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function edtGain_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edtGain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edtFr_Callback(hObject, eventdata, handles)
% hObject    handle to edtFr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
newValue = str2double(get(hObject,'String'));
handles = guidata(hObject);

info = propinfo(handles.src,'FrameRate');
constaints = info.ConstraintValue;

% Check if newValue is out of the interval allowed by the shutter speed
margin = handles.settings.camera.confidenceMargin;
maxFramerate = (1 / (handles.src.Shutter + margin)) * 1000;
if newValue > maxFramerate
    newValue = maxFramerate;
    fprintf('FrameRate higher than what possible with current shutter speed.')
    fprintf('...corrected to maximum (%2.2f)\n',maxFramerate)
end

% Check if newValue is out of the hardware capabilities
if newValue < constaints(1)
    newValue = constaints(1);
    fprintf('FrameRate lower than the minimum possible for the hardware.\n')
    fprintf('...corrected to minimum (%2.2f)\n',constaints(1))
elseif newValue > constaints(2)
    newValue = constaints(2);
    fprintf('FrameRate higher than the maximum possible for the hardware.\n')
    fprintf('...corrected to maximum (%2.2f)\n',constaints(2))
end

% Put the new value in the camera and in the settings struct
handles.src.FrameRate = newValue;
handles.settings.camera.FrameRate = newValue;
% Put the new value in the GUI
set(hObject,'String',num2str(newValue))
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function edtFr_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edtFr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in btnStart.
function btnStart_Callback(hObject, eventdata, handles)
% hObject    handle to btnStart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles = guidata(hObject);

if get(hObject,'Value') %The button was pressed
    preview(handles.vid,handles.img)
    
    set(hObject,'String','Stop Preview')

else %the button was released
    stoppreview(handles.vid)
    set(hObject,'String','Start Preview')
end



function edtShtt_Callback(hObject, eventdata, handles)
% hObject    handle to edtShtt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
newValue = str2double(get(hObject,'String'));

handles = guidata(hObject);
settings = handles.settings;

if newValue > 1/settings.FrameRate*1000 - settings.confidenceMargin
    newValue = 1/settings.FrameRate*1000 - settings.confidenceMargin;
end

handles.src.Shutter = newValue;
handles.settings.Shutter = newValue;
set(hObject,'String',num2str(newValue))

guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function edtShtt_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edtShtt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% --- Executes on button press in btnROI.
function btnROI_Callback(hObject, eventdata, handles)
% hObject    handle to btnROI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
roi = drawrectangle(handles.axImg);
roiPos = floor(roi.Position);
warning('off', 'imaq:pointgrey:roiChanged');
stoppreview(handles.vid);
handles.vid.ROIPosition = roiPos;
preview(handles.vid,handles.img);
delete(roi)

% --- Executes on button press in btnRstROI.
function btnRstROI_Callback(hObject, eventdata, handles)
% hObject    handle to btnRstROI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
stoppreview(handles.vid);
handles.vid.ROIPosition = [0, 0, handles.vid.VideoResolution];
preview(handles.vid,handles.img);

