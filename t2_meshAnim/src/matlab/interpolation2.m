% syms p1x p1y p1z p2x p2y p2z p3x p3y p3z
% syms v1x v1y v1z v2x v2y v2z v3x v3y v3z
% P = [p1x, p1y, p1z, 0, 0, 0, 0, 0, 0, 1, 0 ,0;
%     0,0,0, p1x, p1y, p1z, 0, 0, 0, 0, 1 ,0;
%     0,0,0, 0,0 0, p1x, p1y, p1z, 0, 0 ,1;
%     p2x, p2y, p2z, 0, 0, 0, 0, 0, 0, 1, 0 ,0;
%     0,0,0, p2x, p2y, p2z, 0, 0, 0, 0, 1 ,0;
%     0,0,0, 0,0 0, p2x, p2y, p2z, 0, 0 ,1;
%     p3x, p3y, p3z, 0, 0, 0, 0, 0, 0, 1, 0 ,0;
%     0,0,0, p3x, p3y, p3z, 0, 0, 0, 0, 1 ,0;
%     0,0,0, 0,0 0, p3x, p3y, p3z, 0, 0 ,1]
% V = [v1x; v1y; v1z; v2x; v2y; v2z ; v3x; v3x; v3z];
%
% Bfull = inv(P)*V;
%
% B = [Bfull(1), Bfull(2); Bfull(3), Bfull(4)];
%
% syms alpha1 alpha2 alpha3 beta1 beta2 beta3 gamma1 gamma2 gamma3 mu1 mu2 mu3 nu1 nu2 nu3 phi1 phi2 phi3
% Bsimple = [alpha1*v1x + gamma1*v2x + nu1*v3x, alpha2*v1x + gamma2*v2x + nu2*v3x;
%     beta1*v1y + mu1*v2y + phi1*v3y, beta2*v1y + mu2*v2y + phi2*v3y];
%
% syms a1 a2 a3 a4
%
% A = [a1, a2; a3, a4];
%
% E = norm(A-Bsimple, 'fro')^2;
%% As-rigid-as possible shape interpolation.
clear all;
close all;

% Comment all unused points.
p = [0,0; 0,2; 2,0; 2,2; 4,0; 0,4; 4,2; 2,4; 4,4];
q = [0,0; 0,2; 2,0; 2,2; 5,0; 0,4; 5,2; 1,4; 4,5];
%q = [0,0; 0,4; 4,0; 4,3; 5,1; 0,5; 4,2; 2,4; 5,5];
p(:,3) = 0;
p(1,3) = 0.5;
p(2,3) = -0.5;
p(3,3) = -0.5;
p(4,3) = 0.25;

%p = [0,0,1; 2,0,1; 0,2,1; 3,0,1];
% q = [0,0,1; 2,0,1; 0,2,2; 5,0,1];
%q = bsxfun(@plus, p, [6, 0, 0]);

% Define triangles.
% T = [1, 2, 3; 2, 3, 4; 3, 4, 5];%; 4, 5, 7; 2, 4, 6; 4, 6, 8; 7, 8, 9];
T = [1, 2, 3; 2, 4, 3; 3, 4, 5; 6, 8, 4; 4, 7, 5; 2, 6, 4; 6, 8, 4; 8, 9, 7];

%T = [1, 2, 3; 2, 3, 4];

%% Compute invariant matrix H

% Compute for first triangle
Rgamma = cell(size(T,1),1);
S = cell(size(T,1),1);
inP = cell(size(T,1),1);

% First triangle. Ap + l = q.
zero3 = [0,0,0];
Ptr = [p(1,:), zero3, zero3, 1 0 0;
    zero3, p(1,:), zero3, 0 1 0;
    zero3, zero3, p(1,:), 0 0 1;
    p(2,:), zero3, zero3, 1 0 0;
    zero3, p(2,:), zero3, 0 1 0;
    zero3, zero3, p(2,:), 0 0 1;
    p(3,:), zero3, zero3, 1 0 0;
    zero3, p(3,:), zero3, 0 1 0;
    zero3, zero3, p(3,:), 0 0 1];

Qtr = [q(1,:), q(2,:), q(3,:)]';

% A and l.
% Doing the inverse like this yields the same result for AFulltr if we do
% inP{1} * Qtr
inP{1} = Ptr\eye(size(Ptr));
% Save only the A values of the inverse
inP{1} = inP{1}(1:9, 1:9);

Afulltr = Ptr \ Qtr;
Atr = [Afulltr(1:3)'; Afulltr(4:6)'; Afulltr(7:9)'];

% Decompose A for interpolation.
[U, D, V] = svd(Atr);
Rgamma{1} = U*V';
S{1} = V*D*V';

% Base case - one triangle.
bt = zeros(9,3);
bt(1:3, :) = [inP{1}(1:3,1), inP{1}(1:3,4), inP{1}(1:3,7)];
bt(4:6, :) = [inP{1}(4:6,2), inP{1}(4:6,5), inP{1}(4:6,8)];
bt(7:9, :) = [inP{1}(7:9,3), inP{1}(7:9,6), inP{1}(7:9,9)];

H = zeros((size(p,1)-1)*3);
H(1,1) = dot(bt(1:3,2), bt(1:3,2));
H(2,2) = dot(bt(4:6,2), bt(4:6,2));
H(3,3) = dot(bt(7:9,2), bt(7:9,2));
H(4,4) = dot(bt(1:3,3), bt(1:3,3));
H(5,5) = dot(bt(4:6,3), bt(4:6,3));
H(6,6) = dot(bt(7:9,3), bt(7:9,3));

H(1,4) = dot(bt(1:3,2), bt(1:3,3));
H(4,1) = H(1,4);
H(2,5) = dot(bt(4:6,2), bt(4:6,3));
H(5,2) = H(2,5);
H(3,6) = dot(bt(7:9,2), bt(7:9,3));
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
    
    Afulltr = Ptr \ Qtr;
    Atr = [Afulltr(1:3)'; Afulltr(4:6)'; Afulltr(7:9)'];
    
    % Decompose A for interpolation.
    [U, D, V] = svd(Atr);
    Rgamma{i} = U*V';
    S{i} = V*D*V';
    
    % inP{i} = inv(Ptr);
    inP{i} = Ptr\eye(size(Ptr));
    inP{i} = inP{i}(1:9, 1:9);
    
    bt(1:3, :) = [inP{i}(1:3,1), inP{i}(1:3,4), inP{i}(1:3,7)];
    bt(4:6, :) = [inP{i}(4:6,2), inP{i}(4:6,5), inP{i}(4:6,8)];
    bt(7:9, :) = [inP{i}(7:9,3), inP{i}(7:9,6), inP{i}(7:9,9)];
    
    vh1 = 3*vertex1 - 5;
    vh2 = 3*vertex2 - 5;
    vh3 = 3*vertex3 - 5;
    
    H(vh1,vh1) = H(vh1,vh1) + dot(bt(1:3,1), bt(1:3,1));
    H(vh1+1,vh1+1) = H(vh1+1,vh1+1) + dot(bt(4:6,1), bt(4:6,1));
    H(vh1+2,vh1+2) = H(vh1+2,vh1+2) + dot(bt(7:9,1), bt(7:9,1));
    H(vh2,vh2) = H(vh2,vh2) + dot(bt(1:3,2), bt(1:3,2));
    H(vh2+1,vh2+1) = H(vh2+1,vh2+1) + dot(bt(4:6,2), bt(4:6,2));
    H(vh2+2,vh2+2) = H(vh2+2,vh2+2) + dot(bt(7:9,2), bt(7:9,2));
    H(vh3,vh3) = H(vh3,vh3) + dot(bt(1:3,3), bt(1:3,3));
    H(vh3+1,vh3+1) = H(vh3+1,vh3+1) + dot(bt(4:6,3), bt(4:6,3));
    H(vh3+2,vh3+2) = H(vh3+2,vh3+2) + dot(bt(7:9,3), bt(7:9,3));
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt(1:3,1), bt(1:3,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt(1:3,1), bt(1:3,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt(1:3,2), bt(1:3,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
    vh1 = vh1 + 1;
    vh2 = vh2 + 1;
    vh3 = vh3 + 1;
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt(4:6,1), bt(4:6,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt(4:6,1), bt(4:6,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt(4:6,2), bt(4:6,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
    vh1 = vh1 + 1;
    vh2 = vh2 + 1;
    vh3 = vh3 + 1;
    
    H(vh1,vh2) = H(vh1,vh2) + (dot(bt(7:9,1), bt(7:9,2)));
    H(vh2,vh1) = H(vh1,vh2);
    H(vh1,vh3) = H(vh1,vh3) + (dot(bt(7:9,1), bt(7:9,3)));
    H(vh3,vh1) = H(vh1,vh3);
    H(vh2,vh3) = H(vh2,vh3) + (dot(bt(7:9,2), bt(7:9,3)));
    H(vh3,vh2) = H(vh2,vh3);
    
end

%% Main loop.
for t = 0:0.1:1
    
    v1x = (1-t)*p(1,1) + t*q(1,1);
    v1y = (1-t)*p(1,2) + t*q(1,2);
    v1z = (1-t)*p(1,3) + t*q(1,3);
    
    A = ((1-t)*eye(3) + t*Rgamma{1}) * ((1-t)*eye(3) + t*S{1});
    
    bt(1:3, :) = [inP{1}(1:3,1), inP{1}(1:3,4), inP{1}(1:3,7)];
    bt(4:6, :) = [inP{1}(4:6,2), inP{1}(4:6,5), inP{1}(4:6,8)];
    bt(7:9, :) = [inP{1}(7:9,3), inP{1}(7:9,6), inP{1}(7:9,9)];
    
    c = sum(sum(A.^2)) - 2*(dot(A(1,:), bt(1:3,1))*v1x + dot(A(2,:), bt(4:6,1))*v1y ...
        + dot(A(3,:), bt(7:9,1))*v1z ) + sum(bt(1:3,1).^2*v1x^2) + ...
        sum(bt(4:6,1).^2*v1y^2) + sum(bt(7:9,1).^2*v1z^2); %sum(bt(:,1).^2)
    
    G = zeros((size(p,1)-1)*3,1);
    G(1) = -dot(A(1,:), bt(1:3, 2)) + dot(bt(1:3,1)*v1x, bt(1:3, 2));
    G(2) = -dot(A(2,:), bt(4:6, 2)) + dot(bt(4:6,1)*v1y, bt(4:6, 2));
    G(3) = -dot(A(3,:), bt(7:9, 2)) + dot(bt(7:9,1)*v1z, bt(7:9, 2));
    G(4) = -dot(A(1,:), bt(1:3, 3)) + dot(bt(1:3,1)*v1x, bt(1:3, 3));
    G(5) = -dot(A(2,:), bt(4:6, 3)) + dot(bt(4:6,1)*v1y, bt(4:6, 3));
    G(6) = -dot(A(3,:), bt(7:9, 3)) + dot(bt(7:9,1)*v1z, bt(7:9, 3));
    
    for i = 2: size(T,1)
        vertex1 = T(i,1);
        vertex2 = T(i,2);
        vertex3 = T(i,3);
        
        % Add for more than one triangle.
        vh1 = 3*vertex1 - 5;
        vh2 = 3*vertex2 - 5;
        vh3 = 3*vertex3 - 5;
        
        A = ((1-t)*eye(3) + t*Rgamma{i}) * ((1-t)*eye(3) + t*S{i});
        
        c = c + sum(sum(A.^2));
        
        bt(1:3, :) = [inP{i}(1:3,1), inP{i}(1:3,4), inP{i}(1:3,7)];
        bt(4:6, :) = [inP{i}(4:6,2), inP{i}(4:6,5), inP{i}(4:6,8)];
        bt(7:9, :) = [inP{i}(7:9,3), inP{i}(7:9,6), inP{i}(7:9,9)];
        
        G(vh1) = G(vh1) -dot(A(1,:), bt(1:3, 1));
        G(vh1+1) = G(vh1+1) -dot(A(2,:), bt(4:6, 1));
        G(vh1+2) = G(vh1+2) -dot(A(3,:), bt(7:9, 1));
        G(vh2) = G(vh2) -dot(A(1,:), bt(1:3, 2));
        G(vh2+1) = G(vh2+1) -dot(A(2,:), bt(4:6, 2));
        G(vh2+2) = G(vh2+2) -dot(A(3,:), bt(7:9, 2));
        G(vh3) = G(vh3) -dot(A(1,:), bt(1:3, 3));
        G(vh3+1) = G(vh3+1) -dot(A(2,:), bt(4:6, 3));
        G(vh3+2) = G(vh3+2) -dot(A(3,:), bt(7:9, 3));
    end
    
    
    % Solve the system
    u = - H \ G;
    
    % Deshape the solution for plotting.
    for j = 1:3:size(u,1)
        ureshape(j/3+2/3,:) = [u(j),u(j+1),u(j+2)];
    end
    x = [v1x, v1y, v1z; ureshape]
    
    % Plot.
    figure(1);
    hold on;
    trisurf(T, p(:, 1), p(:,2), p(:,3), ones(1,size(p,1)));
    trisurf(T, q(:, 1), q(:,2), q(:,3), ones(1,size(p,1))+1);
    trisurf(T, x(:, 1), x(:,2), x(:,3), ones(1,size(p,1))+2);
    view(15,35);
    pause('on');
    pause;
    clf(figure(1));
end
close(figure(1));
disp('done');