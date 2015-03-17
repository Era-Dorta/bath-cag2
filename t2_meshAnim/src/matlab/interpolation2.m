%% As-rigid-as possible shape interpolation.
clearvars -except sourceShapeObj targetShapeObj;
close all; clc;

save_path = '~/workspaces/matlab/cag2/data/interpolateResult/interpolated_t_';

if ~(exist('sourceShapeObj', 'var') && exist('targetShapeObj', 'var'))
    sourceShapeObj = read_wobj('~/workspaces/matlab/cag2/data/horse_source.obj');
    targetShapeObj = read_wobj('~/workspaces/matlab/cag2/data/horse_target.obj');
    toSaveObj.objects = sourceShapeObj.objects;
    toSaveObj.material = sourceShapeObj.material;
    toSaveObj.vertices = sourceShapeObj.vertices;
    toSaveObj.vertices_point = sourceShapeObj.vertices_point;
    toSaveObj.vertices_normal = sourceShapeObj.vertices_normal;
    toSaveObj.vertices_texture = sourceShapeObj.vertices_texture;
end

T = sourceShapeObj.objects(1,5).data.vertices;
p = sourceShapeObj.vertices;
q = targetShapeObj.vertices;

extraP = (p(T(1,1),:) + p(T(1,2),:) + p(T(1,3),:)) / 3;
extraQ = (q(T(1,1),:) + q(T(1,2),:) + q(T(1,3),:)) / 3;
p = [extraP; p];
q = [extraQ; q];

T = T + 1;
T = [1, T(1,1), T(1,2); T];


%% Compute invariant matrix H
disp('Building H')
% Compute for first triangle
Rgamma = cell(size(T,1),1);
S = cell(size(T,1),1);
bt = cell(size(T,1),1);

% First triangle. Ap + l = q.
zero3 = [0,0,0];

i = 1;

vertex1 = T(i,1);
vertex2 = T(i,2);
vertex3 = T(i,3);

Ptr = [p(vertex1,:), zero3, zero3, 1 0 0;
    zero3, p(vertex1,:), zero3, 0 1 0;
    zero3, zero3, p(vertex1,:), 0 0 1;
    p(vertex2,:), zero3, zero3, 1 0 0;
    zero3, p(vertex2,:), zero3, 0 1 0;
    zero3, zero3, p(vertex2,:), 0 0 1;
    p(vertex3,:), zero3, zero3, 1 0 0;
    zero3, p(vertex3,:), zero3, 0 1 0;
    zero3, zero3, p(vertex3,:), 0 0 1];

Qtr = [q(vertex1,:), q(vertex2,:), q(vertex3,:)]';

% inP = inv(Ptr);
inP = Ptr\eye(size(Ptr));
inP = inP(1:9, 1:9);

Afulltr = inP * Qtr;
Atr = [Afulltr(1:3)'; Afulltr(4:6)'; Afulltr(7:9)'];

% Decompose A for interpolation.
[U, D, V] = svd(Atr);
Rgamma{1} = U*V';
S{1} = V*D*V';

% Base case - one triangle.
bt{1} = zeros(9,3);
bt{1}(1:3, :) = [inP(1:3,1), inP(1:3,4), inP(1:3,7)];
bt{1}(4:6, :) = [inP(4:6,2), inP(4:6,5), inP(4:6,8)];
bt{1}(7:9, :) = [inP(7:9,3), inP(7:9,6), inP(7:9,9)];

H = zeros((size(p,1)-1)*3);
H(1,1) = dot(bt{1}(1:3,2), bt{1}(1:3,2));
H(2,2) = dot(bt{1}(4:6,2), bt{1}(4:6,2));
H(3,3) = dot(bt{1}(7:9,2), bt{1}(7:9,2));
H(4,4) = dot(bt{1}(1:3,3), bt{1}(1:3,3));
H(5,5) = dot(bt{1}(4:6,3), bt{1}(4:6,3));
H(6,6) = dot(bt{1}(7:9,3), bt{1}(7:9,3));

H(1,4) = dot(bt{1}(1:3,2), bt{1}(1:3,3));
H(4,1) = H(1,4);
H(2,5) = dot(bt{1}(4:6,2), bt{1}(4:6,3));
H(5,2) = H(2,5);
H(3,6) = dot(bt{1}(7:9,2), bt{1}(7:9,3));
H(6,3) = H(3,6);

%% H for the rest of the triangles
for i = 2: size(T,1)
    vertex1 = T(i,1);
    vertex2 = T(i,2);
    vertex3 = T(i,3);
    
    Ptr = [p(vertex1,:), zero3, zero3, 1 0 0;
        zero3, p(vertex1,:), zero3, 0 1 0;
        zero3, zero3, p(vertex1,:), 0 0 1;
        p(vertex2,:), zero3, zero3, 1 0 0;
        zero3, p(vertex2,:), zero3, 0 1 0;
        zero3, zero3, p(vertex2,:), 0 0 1;
        p(vertex3,:), zero3, zero3, 1 0 0;
        zero3, p(vertex3,:), zero3, 0 1 0;
        zero3, zero3, p(vertex3,:), 0 0 1];
    
    Qtr = [q(vertex1,:), q(vertex2,:), q(vertex3,:)]';
    
    % inP = inv(Ptr);
    inP = Ptr\eye(size(Ptr));
    inP = inP(1:9, 1:9);
    
    Afulltr = inP * Qtr;
    Atr = [Afulltr(1:3)'; Afulltr(4:6)'; Afulltr(7:9)'];
    
    % Decompose A for interpolation.
    [U, D, V] = svd(Atr);
    Rgamma{i} = U*V';
    S{i} = V*D*V';
    
    bt{i} = zeros(9,3);
    bt{i}(1:3, :) = [inP(1:3,1), inP(1:3,4), inP(1:3,7)];
    bt{i}(4:6, :) = [inP(4:6,2), inP(4:6,5), inP(4:6,8)];
    bt{i}(7:9, :) = [inP(7:9,3), inP(7:9,6), inP(7:9,9)];
    
    vh1 = 3*vertex1 - 5;
    vh2 = 3*vertex2 - 5;
    vh3 = 3*vertex3 - 5;
    
    H(vh1,vh1) = H(vh1,vh1) + dot(bt{i}(1:3,1), bt{i}(1:3,1));
    H(vh1+1,vh1+1) = H(vh1+1,vh1+1) + dot(bt{i}(4:6,1), bt{i}(4:6,1));
    H(vh1+2,vh1+2) = H(vh1+2,vh1+2) + dot(bt{i}(7:9,1), bt{i}(7:9,1));
    H(vh2,vh2) = H(vh2,vh2) + dot(bt{i}(1:3,2), bt{i}(1:3,2));
    H(vh2+1,vh2+1) = H(vh2+1,vh2+1) + dot(bt{i}(4:6,2), bt{i}(4:6,2));
    H(vh2+2,vh2+2) = H(vh2+2,vh2+2) + dot(bt{i}(7:9,2), bt{i}(7:9,2));
    H(vh3,vh3) = H(vh3,vh3) + dot(bt{i}(1:3,3), bt{i}(1:3,3));
    H(vh3+1,vh3+1) = H(vh3+1,vh3+1) + dot(bt{i}(4:6,3), bt{i}(4:6,3));
    H(vh3+2,vh3+2) = H(vh3+2,vh3+2) + dot(bt{i}(7:9,3), bt{i}(7:9,3));
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt{i}(1:3,1), bt{i}(1:3,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt{i}(1:3,1), bt{i}(1:3,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt{i}(1:3,2), bt{i}(1:3,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
    vh1 = vh1 + 1;
    vh2 = vh2 + 1;
    vh3 = vh3 + 1;
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt{i}(4:6,1), bt{i}(4:6,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt{i}(4:6,1), bt{i}(4:6,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt{i}(4:6,2), bt{i}(4:6,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
    vh1 = vh1 + 1;
    vh2 = vh2 + 1;
    vh3 = vh3 + 1;
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt{i}(7:9,1), bt{i}(7:9,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt{i}(7:9,1), bt{i}(7:9,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt{i}(7:9,2), bt{i}(7:9,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
end

disp('Built H for all triangles');

H = sparse(H);
x = zeros(size(p,1)-1, 3);

%% Main loop.
for t = 0:0.1:1
    disp('Computing next frame');
    v1x = (1-t)*p(T(1,1),1) + t*q(T(1,1),1);
    v1y = (1-t)*p(T(1,1),2) + t*q(T(1,1),2);
    v1z = (1-t)*p(T(1,1),3) + t*q(T(1,1),3);
    
    A = ((1-t)*eye(3) + t*Rgamma{1}) * ((1-t)*eye(3) + t*S{1});
    
    G = zeros((size(p,1)-1)*3,1);
    G(1) = -dot(A(1,:), bt{1}(1:3, 2)) + dot(bt{1}(1:3,1)*v1x, bt{1}(1:3, 2));
    G(2) = -dot(A(2,:), bt{1}(4:6, 2)) + dot(bt{1}(4:6,1)*v1y, bt{1}(4:6, 2));
    G(3) = -dot(A(3,:), bt{1}(7:9, 2)) + dot(bt{1}(7:9,1)*v1z, bt{1}(7:9, 2));
    G(4) = -dot(A(1,:), bt{1}(1:3, 3)) + dot(bt{1}(1:3,1)*v1x, bt{1}(1:3, 3));
    G(5) = -dot(A(2,:), bt{1}(4:6, 3)) + dot(bt{1}(4:6,1)*v1y, bt{1}(4:6, 3));
    G(6) = -dot(A(3,:), bt{1}(7:9, 3)) + dot(bt{1}(7:9,1)*v1z, bt{1}(7:9, 3));
    
    disp('Building G');
    for i = 2: size(T,1)
        vertex1 = T(i,1);
        vertex2 = T(i,2);
        vertex3 = T(i,3);
        
        % Add for more than one triangle.
        vh1 = 3*vertex1 - 5;
        vh2 = 3*vertex2 - 5;
        vh3 = 3*vertex3 - 5;
        
        A = ((1-t)*eye(3) + t*Rgamma{i}) * ((1-t)*eye(3) + t*S{i});
        
        G(vh1) = G(vh1) -dot(A(1,:), bt{i}(1:3, 1));
        G(vh1+1) = G(vh1+1) -dot(A(2,:), bt{i}(4:6, 1));
        G(vh1+2) = G(vh1+2) -dot(A(3,:), bt{i}(7:9, 1));
        G(vh2) = G(vh2) -dot(A(1,:), bt{i}(1:3, 2));
        G(vh2+1) = G(vh2+1) -dot(A(2,:), bt{i}(4:6, 2));
        G(vh2+2) = G(vh2+2) -dot(A(3,:), bt{i}(7:9, 2));
        G(vh3) = G(vh3) -dot(A(1,:), bt{i}(1:3, 3));
        G(vh3+1) = G(vh3+1) -dot(A(2,:), bt{i}(4:6, 3));
        G(vh3+2) = G(vh3+2) -dot(A(3,:), bt{i}(7:9, 3));
    end
    
    disp('Solving u');
    % Solve the system
    u = - H \ G;
    
    disp('Reshaping u');
    
    % Deshape the solution for plotting.
    x(1,:) = [v1x, v1y, v1z];
    
    k = 2;
    for j = 1:3:size(u,1)
        x(k,:) = [u(j),u(j+1),u(j+2)];
        toSaveObj.vertices(k-1,:) = x(k,:);
        k = k + 1;
    end
    
    write_wobj(toSaveObj, strcat(save_path, num2str(t), '.obj'));
    
    fprintf('Displaying t = %f\n',t);
    % Plot.
    figure(1);
    hold on;
    trisurf(T(2:end,:), p(:, 1), p(:,2), p(:,3), ones(1,size(p,1)));
    trisurf(T(2:end,:), x(:, 1) + 30, x(:,2), x(:,3), ones(1,size(x,1))+2);
    trisurf(T(2:end,:), q(:, 1) + 60, q(:,2), q(:,3), ones(1,size(p,1))+1);
    view(50,30);
    pause('on');
    pause;
    clf(figure(1));
end
close(figure(1));
disp('done');