function [thetaDotList] = PerwightedPsudoInv(J,M,Vs)

% [U,S,V] = svd(J,"econ");
% 
% Jtest = (U*S*V');

thetaDotList = M^-1*J'*(J*M^-1*J')^-1*Vs; %Formula pgage 200 (M matrix for cost)
end

