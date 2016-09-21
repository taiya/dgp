clc, clear, close all;

%--- convenience functions (silly matlab)
% HP: 2xN matrices, one point per column
quiver2 = @(P,V,varargin) quiver( P(1,:), P(2,:), V(1,:), V(2,:), varargin{:} );
plot2 = @(P,varargin) plot(P(1,:), P(2,:), varargin{:} );
nnsearch = @(P,X,Y) knnsearch(KDTreeSearcher(P'), [X(:), Y(:)]);