clear; clc; close all;
% Se FBD Ipad og 

%Lenght from assy drawing
l1 = 156.9/1000;
l2 = 27.45/1000;
l3 = 179.42/1000;
l4 = 26.86/1000;
%From Telescope drawing (IN assy drawing)
%l5 = 163.43; 
l5 = 167.25/1000; % Corrected after sensor calibration 
l6 = 1.75/1000; %mm
W1 = -l2+l3+l5;
W2 = l4;
W3 = l1-l6;


% Position of each joint
a1 = [0; 0; 0];
a2 = [-l2; 0; l1];
a3 = [-l2+l3; 0; l1];
a4 = [W1; W2; W3];
a5 = [W1; W2; W3];
a6 = [W1; W2; W3];

% Screw axis of ech joint
Sw1 = [0; 0; 1];
Sw2 = [0; 1; 0];
Sw3 = [0; 1; 0];
Sw4 = [0; 0; 0];
Sw5 = [0; 1; 0];
Sw6 = [0; 0; 1];

Sv1 = cross(a1,Sw1);
Sv2 = cross(a2,Sw2);
Sv3 = cross(a3,Sw3);
Sv4 = [1;0;0];%translation along xaxis
Sv5 = cross(a5,Sw5);
Sv6 = cross(a6,Sw6);

S1 = [Sw1;Sv1]
S2 = [Sw2;Sv2]
S3 = [Sw3;Sv3]
S4 = [Sw4;Sv4]
S5 = [Sw5;Sv5]
S6 = [Sw6;Sv6]



M = [[1 0 0 W1];
    [0 1 0 W2];
    [0 0 1 W3];
    [0 0 0 1];]

% INPUT THETA TO SEE FORWARD KINEMATICS
% Joints: 
q1 = -pi/10;
q2 = -pi/4;
q3 = pi/2;
q4 = 100/1000; %m
q5 = 7;
q6 = 5;


thetalistFORWARD = [q1; q2 ; q3; q4; q5; q6]
Slist = [S1,S2,S3,S4,S5,S6]

T = FKinSpace(M, Slist, thetalistFORWARD)

%Checked against DH Table method: findTransformatrionMatrixDH.m Appox 1mm difference. OK
%Could be investigated????? 


%% Testing inversekinematics

q1 = -0.497;
q2 = -0.19;
q3 = 1.02;
q4 = 185;

%Dummy
q5 = -(q2+q3);
q6 = -q1;

X = 350;
Y = -158;
Z = -70;


TInverseDesired = [[1, 0, 0, X]; [0, 1, 0, Y]; [0, 0, 1, Z]; [0, 0, 0, 1]];
thetalist0 = [q1; q2; q3; q4; q5; q6];
eomg = 0.01;
ev = 0.001;
[thetalist, success] = IKinSpace(Slist, M, TInverseDesired, thetalist0, eomg, ev)

%%SEEMS TO WORK GREAT WITH EXTRA JOINTS


%% Inverse velocity kinematics

%Find jacobian:

q1 = -pi/10;
q2 = -pi/4;
q3 = pi/2;
q4 = 100/1000;
q5 = 0;
q6 = 0;





thetalist = [q1; q2 ; q3; q4; q5; q6]
Vs = [0 0 0 50/1000 0 0]'
J = JacobianSpace(Slist, thetalist);
Jinv = J^-1;
JinvPinv = pinv(J);
thetalistDot = pinv(J) * Vs
%thetalistDotTESTNORMALINV = Jinv * Vs
% Changig M matrix for cost
M = eye(6);
M(4,4) = 0.1;



%thetalistDotFromFunc = PerwightedPsudoInv(J,M,Vs)

%thetaListDotTEST = M^-1*J'*(J*M^-1*J')^-1*Vs %Formula pgage 200 (M matrix for cost)
%thetaListDotTEST2 = M^-1*J'*(J*M^-1*J')^-1*Vs

%% play with svd
J 
[U,S,V] = svd(J,"econ")

JtestInv = (U*S*V')

thetaTest = M^-1*JtestInv'*(JtestInv*M^-1*JtestInv')^-1*Vs



%% Manual data for input WRONG, SHOULD BE IN M!!!!!!!
Slist = [0            0            0            0            0            0;
            0            1            1            0            1            0;
            1            0            0            0            0            1;
            0       -156.9       -156.9            1      -155.15        26.86;
            0            0            0            0            0      -319.22;
            0       -27.45       151.97            0       319.22            0];

M = [[1 0 0 319.22];
    [0 1 0 26.86];
    [0 0 1 155.15];
    [0 0 0 1];];



