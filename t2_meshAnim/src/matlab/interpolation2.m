% syms p1x p1y p2x p2y p3x p3y
% syms v1x v1y v2x v2y v3x v3y
% P = [p1x, p1y, 0, 0, 1, 0; ...
%     0, 0, p1x, p1y, 0, 1; ...
%     p2x, p2y, 0, 0, 1, 0; ...
%     0, 0, p2x, p2y, 0, 1; ...
%     p3x, p3y, 0, 0, 1, 0; ...
%     0, 0, p3x, p3y, 0, 1];
% V = [v1x; v1y; v2x; v2y; v3x; v3x];
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

%p = [0,2; 2,0; 2,2; 4,0];
%q = [1,2; 2,0; 2,5; 5,0];

% Define triangles.
% T = [1, 2, 3; 2, 3, 4; 3, 4, 5];%; 4, 5, 7; 2, 4, 6; 4, 6, 8; 7, 8, 9];
T = [1, 2, 3; 2, 4, 3; 3, 4, 5; 6, 8, 4; 4, 7, 5; 2, 6, 4; 6, 8, 4; 8, 9, 7];

%T = [1, 2, 3; 2, 3, 4];

% Main loop.
for t = 0:0.1:1
    
    v1x = (1-t)*p(1,1) + t*q(1,1);
    v1y = (1-t)*p(1,2) + t*q(1,2);
    
    
    % First triangle. Ap + l = q.
    Ptr = [p(1,1), p(1,2), 0, 0, 1, 0; ...
        0, 0, p(1,1), p(1,2), 0, 1; ...
        p(2,1), p(2,2), 0, 0, 1, 0; ...
        0, 0, p(2,1), p(2,2), 0, 1; ...
        p(3,1), p(3,2), 0, 0, 1, 0; ...
        0, 0, p(3,1), p(3,2), 0, 1];
    
    Qtr = [q(1,1); q(1,2); q(2,1); q(2,2); q(3,1); q(3,2)];
    
    % A and l.
    Afulltr = Ptr \ Qtr;
    Atr = [Afulltr(1) Afulltr(2); Afulltr(3) Afulltr(4)];
    
    % Decompose A for interpolation.
    [U, D, V] = svd(Atr);
    Rgamma = U*V';
    S = V*D*V';
    
    inP = inv(Ptr);
    
    A = ((1-t)*eye(2) + t*Rgamma) * ((1-t)*eye(2) + t*S);
    
    c = norm(A,'fro')^2 - 2* (A(1,1)*inP(1,1)*v1x + A(1,2)*inP(2,1)*v1x + ...
        A(2,1)*inP(3,2)*v1y + A(2,2)*inP(4,2)*v1y) + ...
        inP(1,1)^2*v1x^2 + inP(2,1)^2*v1x^2 + inP(3,2)^2*v1y^2 + inP(4,2)^2*v1y^2;
    
    % Base case - one triangle.
    H = zeros((size(p,1)-1)*2);
    H(1,1) = inP(1,3)^2 + inP(2,3)^2;
    H(2,2) = inP(3,4)^2 + inP(4,4)^2;
    H(3,3) = inP(1,5)^2 + inP(2,5)^2;
    H(4,4) = inP(3,6)^2 + inP(4,6)^2;
    H(3,1) = inP(1,3)*inP(1,5) + inP(2,3)*inP(2,5);
    H(4,2) = inP(3,4)*inP(3,6) + inP(4,4)*inP(4,6);
    H(1,3) = inP(1,3)*inP(1,5) + inP(2,3)*inP(2,5);
    H(2,4) = inP(3,4)*inP(3,6) + inP(4,4)*inP(4,6);
    
    G = zeros((size(p,1)-1)*2,1);
    G(1) = -(A(1,1)*inP(1,3) + A(1,2)*inP(2,3)) + inP(1,1)*inP(1,3)*v1x + inP(2,1)*inP(2,3)*v1x;
    G(2) = -(A(2,1)*inP(3,4) + A(2,2)*inP(4,4)) + inP(3,2)*inP(3,4)*v1y + inP(4,2)*inP(4,4)*v1y;
    G(3) = -(A(1,1)*inP(1,5) + A(1,2)*inP(2,5)) + inP(1,1)*inP(1,5)*v1x + inP(2,1)*inP(2,5)*v1x;
    G(4) = -(A(2,1)*inP(3,6) + A(2,2)*inP(4,6)) + inP(3,2)*inP(3,6)*v1y + inP(4,2)*inP(4,6)*v1y;
    
    for i = 2: size(T,1)
        vertex1 = T(i,1);
        vertex2 = T(i,2);
        vertex3 = T(i,3);
        
        P = [p(vertex1,1), p(vertex1,2), 0, 0, 1, 0; ...
            0, 0, p(vertex1,1), p(vertex1,2), 0, 1; ...
            p(vertex2,1), p(vertex2,2), 0, 0, 1, 0; ...
            0, 0, p(vertex2,1), p(vertex2,2), 0, 1; ...
            p(vertex3,1), p(vertex3,2), 0, 0, 1, 0; ...
            0, 0, p(vertex3,1), p(vertex3,2), 0, 1];
        
        Q = [q(vertex1,1); q(vertex1,2); q(vertex2,1); q(vertex2,2); q(vertex3,1); q(vertex3,2)];
        
        Afulltri = P \ Q;
        Atri = [Afulltri(1) Afulltri(2); Afulltri(3) Afulltri(4)];
        
        [U, D, V] = svd(Atri);
        Rgamma = U*V';
        S = V*D*V';
        
        inP = inv(P);
        
        % Add for more than one triangle.
        vh1 = 2*vertex1 - 3;
        vh2 = 2*vertex2 - 3;
        vh3 = 2*vertex3 - 3;
        
        H(vh1,vh1) = H(vh1,vh1) + inP(1,1)^2 + inP(2,1)^2;
        H(vh1+1,vh1+1) = H(vh1+1,vh1+1) + inP(3,2)^2 + inP(4,2)^2;
        H(vh2,vh2) = H(vh2,vh2) + inP(1,3)^2 + inP(2,3)^2;
        H(vh2+1,vh2+1) = H(vh2+1,vh2+1) + inP(3,4)^2 + inP(4,4)^2;
        H(vh3,vh3) = H(vh3,vh3) + inP(1,5)^2 + inP(2,5)^2;
        H(vh3+1,vh3+1) = H(vh3+1,vh3+1) + inP(3,6)^2 + inP(4,6)^2;
        
        H(vh1,vh2) = H(vh1,vh2) + inP(1,1)*inP(1,3) + inP(2,1)*inP(2,3);
        H(vh2,vh1) = H(vh1,vh2);
        H(vh1,vh3) = H(vh1,vh3) + inP(1,1)*inP(1,5) + inP(2,1)*inP(2,5);
        H(vh3,vh1) = H(vh1,vh3);
        H(vh2,vh3) = H(vh2,vh3) + inP(1,3)*inP(1,5) + inP(2,3)*inP(2,5);
        H(vh3,vh2) = H(vh2,vh3);
        H(vh1+1,vh2+1) = H(vh1+1,vh2+1) + inP(3,2)*inP(3,4) + inP(4,2)*inP(4,4);
        H(vh2+1,vh1+1) = H(vh1+1,vh2+1);
        H(vh1+1,vh3+1) = H(vh1+1,vh3+1) + inP(3,2)*inP(3,6) + inP(4,2)*inP(4,6);
        H(vh3+1,vh1+1) = H(vh1+1,vh3+1);
        H(vh2+1,vh3+1) = H(vh2+1,vh3+1) + inP(3,4)*inP(3,6) + inP(4,4)*inP(4,6);
        H(vh3+1,vh2+1) = H(vh2+1,vh3+1);
        
        A = ((1-t)*eye(2) + t*Rgamma) * ((1-t)*eye(2) + t*S);
        
        c = c + norm(A,'fro')^2;
        
        G(vh1) = G(vh1) - A(1,1)*inP(1,1) - A(1,2)*inP(2,1);
        G(vh1+1) = G(vh1+1) - A(2,1)*inP(3,2) - A(2,2)*inP(4,2);
        G(vh2) = G(vh2) - A(1,1)*inP(1,3) - A(1,2)*inP(2,3);
        G(vh2+1) = G(vh2+1) - A(2,1)*inP(3,4) - A(2,2)*inP(4,4);
        G(vh3) = G(vh3) - A(1,1)*inP(1,5) - A(1,2)*inP(2,5);
        G(vh3+1) = G(vh3+1) - A(2,1)*inP(3,6) - A(2,2)*inP(4,6);
        
    end
    
    
    % Formulate and solve a quadratic form.
    cGH = [c G'; G H];
    
    f = zeros(size(cGH,1),1);
    Aeq = zeros(size(cGH,1));
    beq = zeros(size(cGH,1),1);
    Aeq(1,1) = 1;
    beq(1) = 1;
    u = quadprog(cGH,f,[],[],Aeq, beq);
    
    % Deshape the solution for plotting.
    uno1 = u(2:end);
    for j = 1:2:size(uno1,1)
        ureshape(j/2+1/2,:) = [uno1(j),uno1(j+1)];
    end
    x = [v1x, v1y; ureshape]
    
    % Plot.
    figure(1);
    hold on;
    triplot(T, p(:, 1), p(:,2));
    triplot(T, q(:, 1), q(:,2), 'r');
    triplot(T, x(:, 1), x(:,2), 'g');
    waitforbuttonpress;
    
end