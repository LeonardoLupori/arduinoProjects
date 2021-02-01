function liveSaturimeter(obj,event,hImage)

set(hImage, 'CData', event.Data);

img =  cat(3,event.Data,event.Data,event.Data);

% Color saturated and black pixels
tmp = img(:,:,1);
tmp(event.Data(:)==255) = 255;
img(:,:,1) = tmp;
tmp(event.Data(:)==0) = 0;
img(:,:,1) = tmp;

tmp = img(:,:,2);
tmp(event.Data(:)==255) = 0;
img(:,:,2) = tmp;
tmp(event.Data(:)==0) = 0;
img(:,:,2) = tmp;

tmp = img(:,:,3);
tmp(event.Data(:)==255) = 0;
img(:,:,3) = tmp;
tmp(event.Data(:)==0) = 255;
img(:,:,3) = tmp;


avgLuminance = mean(event.Data(:))/255*100;
img = insertText(img, [1 1], sprintf('AVG: %3.2f%%',avgLuminance),...
    'BoxColor',[255 255 255],'TextColor',[0 0 0],'FontSize',18);
saturated = sum(event.Data(:)==255)/numel(event.Data)*100;
img = insertText(img, [1 35], sprintf('Sat: %3.2f%%',saturated),...
    'BoxColor',[255 255 255],'TextColor',[0 0 0],'FontSize',18);


set(hImage, 'CData', img);

drawnow
