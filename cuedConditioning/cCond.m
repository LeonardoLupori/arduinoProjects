clear all

% Get all the available serial ports on this PC
serialportlist("available")

% Connect to a specific serial port
arduinoObj = serialport("COM3",9600,'Timeout',2);
% configureTerminator(arduinoObj,"CR/LF")
pause(2)
writeline(arduinoObj,'handshakeFromMATLAB')
a = readline(arduinoObj)

% configureCallback(arduinoObj,"terminator",@echoSerial)
%%

% writeline(arduinoObj,'handshakeFromMATLAB')
writeline(arduinoObj,'handshakeFromMATLAB')
a = readline(arduinoObj)

%%

writeline(app.serialObj,'handshakeFromMATLAB');
msg = readline(app.serialObj)


%%

disp(readline(arduinoObj))



%%
UserData = struct(...
    'timeMillis',[],...
    'event',{});

arduinoObj.UserData = UserData;

% Clear serial port device buffers
flush(arduinoObj);
% Set a callbackFcn to trigger whenever a terminator is available to be read
% Default terminator is linefeed
configureCallback(arduinoObj,"terminator",@readEventData)

%%

d = arduinoObj.UserData;
plot([d.timeMillis],1,'o','MarkerFaceColor',lines(1),'MarkerEdgeColor',lines(1))


%%

delete(arduinoObj)
