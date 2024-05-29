function Arot = ArotY(theta)
% Returns the rotation matrix around y axis for given theta input
Arot = [ cos(theta) 0 sin(theta);
            0        1      0;
    -sin(theta)     0   cos(theta)];

end

