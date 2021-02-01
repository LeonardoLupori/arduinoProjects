% -------------------------------------------------------------------------
% BLACKFLY CAMERA SETTINGS
% -------------------------------------------------------------------------
% if NaN, the default values ar going to be set

settings.adaptor = 'pointgrey';                  % AdaptorName. See the folder camera_support for installing the adaptor
settings.deviceID = 1;                           % In case there are multiple cameras
settings.format = 'F7_Mono8_644x482_Mode1';     % Acquisition format (1x1, 2x2,4x4 binning; various bit-depth)
% **********************************
%       AVAILABLE FORMATS
%     {'F7_Mono12_1288x964_Mode0'}
%     {'F7_Mono12_320x240_Mode5' }
%     {'F7_Mono12_644x482_Mode1' }
%     {'F7_Mono16_1288x964_Mode0'}
%     {'F7_Mono16_320x240_Mode5' }
%     {'F7_Mono16_644x482_Mode1' }
%     {'F7_Mono8_1288x964_Mode0' }
%     {'F7_Mono8_320x240_Mode5'  }
%     {'F7_Mono8_644x482_Mode1'  }
% **********************************

settings.Brightness = NaN;                   % Adjust the black level of the camera
settings.ExposureMode = 'off';               % Automatic exposure level
settings.GammaMode = 'off';                  % Gamma correction for display
settings.SharpnessMode = 'off';              % Postprocessing for increased sharpness
settings.FrameRateMode = 'Manual';           % 'Manual' or 'Auto'
settings.ShutterMode = 'Manual';             % 'Manual' or 'Auto'
settings.gainMode = 'Manual';                % 'Manual' or 'Auto'

settings.TriggerMode = 'manual';             % 'immediate', 'manual' or 'hardware'
settings.TriggerCondition = 'risingEdge';    % Only if mode = hardware. 'risingEdge' or 'fallingEdge'
settings.TriggerSource = 'externalTriggerMode0-Source0';
% **********************************
%       AVAILABLE SOURCES
%     'externalTriggerMode0-Source0'                Standard External Trigger 
%     'externalTriggerMode1-Source0'                Bulb Shutter Trigger
%     'externalTriggerMode13-Source0'               Low Smear Trigger
%     'externalTriggerMode14-Source0'               Overlapped Exposure Readout Trigger 
%     'externalTriggerMode15-Source0'               Multi-Shot Trigger
% **********************************

settings.FrameRate = 10;                    % Rate of acquisition (Hz)
settings.Gain = 20;                         % Gain level in the range [0   23.9905]
settings.Shutter = 40;                      % in MATLAB I could not get the shutter value
settings.confidenceMargin = 1;               % Additional time in ms for data transfer to image buffer
% (exposure time) to be constained by the selected framerate and vice vera,
% so for this reason these two parameters need to be adjusted explicitly
% here in MATLAB so that 1/framerate > shutter + confidenceMargin.
% A confidenceMargin of a few milliseconds is added to allow for data
% transfer to the image buffer