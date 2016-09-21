clc, clear, close all;

%--- convenience functions (silly matlab)
% HP: 2xN matrices, one point per column
quiver2 = @(P,V,varargin) quiver( P(1,:), P(2,:), V(1,:), V(2,:), varargin{:} );
plot2 = @(P,varargin) plot(P(1,:), P(2,:), varargin{:} );
kdtree = @(P) KDTreeSearcher(P');

%--- TODO
% phi = zeros(200, 200);
% tree = kdtree(P);
% for every [x,y] in [1:200,1:200] in image
%    I = tree.knnsearch([x,y]);
%    p = extract i-th column of P;
%    n = extract i-th column of N;
%    phi(x,y) = ... plane's implicit function
% end
