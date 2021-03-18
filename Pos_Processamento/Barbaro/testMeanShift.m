%testDistCluters

% clear
% profile on

% nPtsPerClust = 500;
% nClust  = 3;
% totalNumPts = nPtsPerClust*nClust;
% m(:,1) = [40 0]';
% m(:,2) = [25 25]';
% m(:,3) = [100 0]';
% var = 20;
bandwidth = 30;
clustMed = [];
%clustCent;

%%%%%%%%%%%%%%%%%%%%%%%%%%
x = filtroFreezer(:,[2 4])'; % ABS? Transposta?

%x = dPA(:,[2 3])';

%*** build the point set
% for i = 1:nClust
%     x(:,1+(i-1)*nPtsPerClust:(i)*nPtsPerClust)       = x(:,1+(i-1)*nPtsPerClust:(i)*nPtsPerClust) + repmat(m(:,i),1,nPtsPerClust);   
% end

tic
[clustCent,point2cluster,clustMembsCell] = MeanShiftCluster(x,bandwidth);
toc

numClust = length(clustMembsCell);


figure(10),clf,hold on
cVec = 'bgrcmykbgrcmykbgrcmykbgrcmyk';%, cVec = [cVec cVec];
for k = 1:min(numClust,length(cVec))
    myMembers = clustMembsCell{k};
    myClustCen = clustCent(:,k);
    plot(x(1,myMembers),x(2,myMembers),[cVec(k) '.'])
    plot(myClustCen(1),myClustCen(2),'o','MarkerEdgeColor','k','MarkerFaceColor',cVec(k), 'MarkerSize',8)
end
% title(['no shifting, numClust:' int2str(numClust)])


%9/3 reuniao Cuerda
figure
stem(filtroFreezer(clustMembsCell{1},1),filtroFreezer(clustMembsCell{1},2),'r')
hold
plot(PA1toN)
stem(filtroFreezer(clustMembsCell{2},1),filtroFreezer(clustMembsCell{2},2),'g')
stem(filtroFreezer(clustMembsCell{3},1),filtroFreezer(clustMembsCell{3},2),'m')
stem(filtroFreezer(clustMembsCell{4},1),filtroFreezer(clustMembsCell{4},2),'k')