clear all;
close all;

%% Initialise
numT = 2;
numV = 5;

T = [1, 2, 3; 3, 4, 5];

P = [0, 0;
    0, 1;
    1, 0;
    2, 1;
    2, 0];

Q = [3, 1;
    4, 1;
    4, 0;
    6, 0;
    5, -1];

H = eye(4 * numT);
beq = zeros(numT * 4, 1);

%% If a vertex is shared by two or more triangles, set a constraint in 
% Aeq so that the transformation of both triangles leaves the vertex in 
% the same location.
Aeq = zeros(4 * numT);
prevVInd = 0;
nextFRow = 1;
for t1=1:size(T, 1) - 1
    for t2=t1 + 1:size(T, 1)
        if t1 ~= t2
            for k=1:3
                ind = find(T(t2, :) == T(t1, k));
                if ~isempty(ind)
                    Aeq( nextFRow, 4 * t1 - 3:4 * t1 - 2) = P(T(t1, k), :);
                    Aeq( nextFRow, 4 * t2 - 3:4 * t2 - 2) = -P(T(t1, k), :);
                    Aeq( nextFRow + 1, 4 * t1 - 1:4 * t1) = P(T(t1, k), :);
                    Aeq( nextFRow + 1, 4 * t2 - 1:4 * t2) = -P(T(t1, k), :);
                    nextFRow = nextFRow + 2;
                end
            end
        end
    end
end

f = zeros(numT * 4, 1);
auxA = zeros(6, 6);
auxA([1,3,5],3) = 1;
auxA([2,4,6],6) = 1;

% Compute an affine transformation matrix for each triangle
for i=1:numT
    auxA(1, 1:2) = P(T(i,1), :);
    auxA(2, 4:5) = P(T(i,1), :);
    auxA(3, 1:2) = P(T(i,2), :);
    auxA(4, 4:5) = P(T(i,2), :);
    auxA(5, 1:2) = P(T(i,3), :);
    auxA(6, 4:5) = P(T(i,3), :);
    q = [Q(T(i,1), :), Q(T(i,2), :), Q(T(i,3), :)]';
    A = auxA \ q;
    f(i * 4 - 3:i * 4) = -1 * A([1,2,4,5]);
end

%% Compute the new transformation matrices such that they comply with the 
% constraints and minimize the difference between the transformations
x = quadprog(H, f, [], [], Aeq, beq);

%% Plot the results
B1 = reshape(x(1:4), 2, 2);
B2 = reshape(x(5:8), 2, 2);
I = eye(2);
for t=0:0.01:1
    newB1 = I* (1 - t) + t*B1;
    newB2 = I* (1 - t) + t*B2;
    
    newV(1, :) = P(1, :) * newB1;
    newV(2, :) = P(2, :) * newB1;
    newV(3, :) = P(3, :) * newB1;
    newV(4, :) = P(4, :) * newB2;
    newV(5, :) = P(5, :) * newB2;
    
%   Uncomment to show linear interpolation    
%     newV2(1, :) = P(1, :) * (1 - t) +  t * Q(1, :);
%     newV2(2, :) = P(2, :) * (1 - t) +  t * Q(2, :);
%     newV2(3, :) = P(3, :) * (1 - t) +  t * Q(3, :);
%     newV2(4, :) = P(4, :) * (1 - t) +  t * Q(4, :);
%     newV2(5, :) = P(5, :) * (1 - t) +  t * Q(5, :);
    
    figure(1);
    hold on;
    triplot(T, P(:, 1), P(:,2));
    triplot(T, Q(:, 1), Q(:,2), 'r');
    triplot(T, newV(:, 1), newV(:,2), 'g');
    %triplot(T, newV2(:, 1), newV2(:,2), 'b');
    waitforbuttonpress;
    clf(figure(1));
end