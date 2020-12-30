function readEventData(src,~)
    data = readline(src);
    
    % Parse data
    data = strsplit(data,'_');
    time = str2double(data(1));
    event = data(2);
    
    UD = src.UserData;
    UD(end+1).timeMillis = time;
    UD(end+1).event = event;
    
    src.UserData = UD;
    disp('recorded.')
end